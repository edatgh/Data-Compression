
#include <stdlib.h>
#include <string.h>
#include "huffman.h"


/*
 * Private
 */

static void count_char_frequencies(huffman_context_t* huffman_context, char* data, unsigned long int data_size)
{
	int i;


	huffman_context->freq_count = 0;

	for (i = 0; i < data_size; i++)
	{
		if (huffman_context->freq[(unsigned char)data[i]]++ == 0)
		{
			huffman_context->freq_count++;
		}
	}
}


static void build_initial_tree(huffman_context_t* huffman_context)
{
	int             t_node_count; /* total tree node count */
	int             i;
	huffman_node_t* p;


	t_node_count = 2 * huffman_context->freq_count - 1;

	p = huffman_context->tree = (huffman_node_t *)calloc(t_node_count, sizeof(*huffman_context->tree));

	huffman_context->node_count = 0;

	for (i = 0; i < 256; i++)
	{
		if (huffman_context->freq[i] > 0)
		{
			p->weight   = huffman_context->freq[i];
			p->used     = 0;
			p->c        = (char)i;
			p->left     = -1;
			p->right    = -1;

			huffman_context->node_count++;
			p++;
		}
	}

	huffman_context->nodes_left = huffman_context->node_count;
}


static void engage_two_mins(huffman_context_t* huffman_context, int* min1, int* min2)
{
	int             i;
	int             min;   /* min value          */
	int             min_i; /* index of min value */
	huffman_node_t* p;


	min   = 1 << 30; /* TODO: avoid such decisions */
	min_i = -1;

	for (i = 0; i < huffman_context->node_count; i++)
	{
		p = huffman_context->tree + i;

		if (p->weight > 0 && p->used == 0 && p->weight < min)
		{
			min   = p->weight;
			min_i = i;
		}
	}

	/*
	 * Engage the first node with min weight
	 */
	p = huffman_context->tree + min_i;
	p->used = 1;

	*min1 = min_i;

	min = 1 << 30; /* TODO: avoid such decisions */

	for (i = 0; i < huffman_context->node_count; i++)
	{
		p = huffman_context->tree + i;

		if (p->weight > 0 && p->used == 0 && p->weight < min)
		{
			min   = p->weight;
			min_i = i;
		}
	}

	/*
	 * Engage the second node with min weight
	 */
	p = huffman_context->tree + min_i;
	p->used = 1;

	*min2 = min_i;
}


static void push_code(huffman_context_t* huffman_context, char c, char code)
{
	if (huffman_context->codes[(unsigned char)c].length == huffman_context->codes[(unsigned char)c].max_length)
	{
		huffman_context->codes[(unsigned char)c].max_length += 8;
		huffman_context->codes[(unsigned char)c].code = (char *)realloc(huffman_context->codes[(unsigned char)c].code,
										huffman_context->codes[(unsigned char)c].max_length);
	}

	huffman_context->codes[(unsigned char)c].code[huffman_context->codes[(unsigned char)c].length++] = code;
}


static void add_0_code(huffman_context_t* huffman_context, huffman_node_t* tree)
{
	if (tree->left == -1 && tree->right == -1)
	{
		push_code(huffman_context, tree->c, 0);
		return;
	}

	add_0_code(huffman_context, huffman_context->tree + tree->left);
	add_0_code(huffman_context, huffman_context->tree + tree->right);
}


static void add_1_code(huffman_context_t* huffman_context, huffman_node_t* tree)
{
	if (tree->left == -1 && tree->right == -1)
	{
		push_code(huffman_context, tree->c, 1);
		return;
	}

	add_1_code(huffman_context, huffman_context->tree + tree->left);
	add_1_code(huffman_context, huffman_context->tree + tree->right);
}


static void merge_nodes(huffman_context_t* huffman_context, int idx_node1, int idx_node2, int construct_code)
{
	huffman_node_t* p;


	p = huffman_context->tree;

	p += huffman_context->node_count++;

	p->weight   = huffman_context->tree[idx_node1].weight + huffman_context->tree[idx_node2].weight;
	p->used     = 0;
	p->left     = idx_node1;
	p->right    = idx_node2;

	if (construct_code)
	{
		add_0_code(huffman_context, huffman_context->tree + p->left);
		add_1_code(huffman_context, huffman_context->tree + p->right);
	}
}


static void build_tree(huffman_context_t* huffman_context)
{
	int min1;
	int min2;


	while (huffman_context->nodes_left-- > 1)
	{
		engage_two_mins(huffman_context, &min1, &min2);
		merge_nodes(huffman_context, min1, min2, 1);
	}
}


static void build_tree_no_codes(huffman_context_t* huffman_context)
{
	int min1;
	int min2;


	while (huffman_context->nodes_left-- > 1)
	{
		engage_two_mins(huffman_context, &min1, &min2);
		merge_nodes(huffman_context, min1, min2, 0);
	}
}


char* add_data(char* data, char c, int *len, int* max_len)
{
	if (*len == *max_len)
	{
		*max_len += 128;
		data = (char *)realloc(data, *max_len * sizeof(char));
	}

	data[(*len)++] = c;

	return data;
}


static char* compose_code(char* code, int* code_size, int* max_size, unsigned int* idx_bit, int bit, int is_last_bit)
{
	unsigned char byte;


	if (*idx_bit % 8 == 0)
	{
		code = add_data(code, 0, code_size, max_size);
	}

	byte = code[*code_size - 1];

	if (bit == 1)
	{
		byte |= (1 << (7 - *idx_bit % 8));
	}
	else if (bit == 0)
	{
		byte &= ~(1 << (7 - *idx_bit % 8));
	}

	(*idx_bit)++;

	code[*code_size - 1] = byte;

	if (is_last_bit)
	{
		if (*idx_bit % 8 == 0)
		{
			byte = 8;
		}
		else
		{
			byte = *idx_bit % 8;
		}

		code = add_data(code, byte, code_size, max_size);
	}

	return code;
}


static int decompose_code(char* code, int code_size, unsigned int* idx_bit)
{
	unsigned char byte;
	int           ret;


	byte = code[*idx_bit / 8];

	if (*idx_bit < 8 * (code_size - 2) + code[code_size - 1])
	{
		ret = (byte & (1 << (7 - (*idx_bit % 8)))) != 0;
		(*idx_bit)++;
	}
	else
	{
		ret = -1; /* end of code */
	}

	return ret;
}


/*
 * Public
 */

huffman_context_t* huffman_create_context(void)
{
	huffman_context_t* huffman_context;


	huffman_context = (huffman_context_t *)calloc(1, sizeof(*huffman_context));

	return huffman_context;
}


int huffman_encode(huffman_context_t* huffman_context, char* data, unsigned int data_size, char** code, unsigned int* code_size,
		   char** internal_data, unsigned int* internal_data_size)
{
	count_char_frequencies(huffman_context, data, data_size);
	build_initial_tree(huffman_context);
	build_tree(huffman_context);

	{
		int i, j, max_size;
		unsigned int idx_bit;


		*code_size = 0;
		max_size   = 0;
		*code      = NULL;
		idx_bit    = 0;

		for (i = 0; i < data_size; i++)
		{
			for (j = huffman_context->codes[(unsigned char)data[i]].length - 1; j >= 0; j--)
			{
				*code = compose_code(*code, code_size, &max_size, &idx_bit,
						     huffman_context->codes[(unsigned char)data[i]].code[j],
						     i == data_size - 1 && j == 0);
			}
		}
	}

	*internal_data = (char *)calloc(256, sizeof(unsigned int));
	memcpy(*internal_data, huffman_context->freq, sizeof(huffman_context->freq));
	*internal_data_size = sizeof(huffman_context->freq);

	return 0;
}


int huffman_decode(huffman_context_t* huffman_context, char* code, unsigned int code_size, char** data, unsigned int* data_size,
		   char* internal_data, unsigned int internal_data_size)
{
	int             i;
	unsigned int    idx_bit;
	int             bit;
	huffman_node_t* p;
	int             node;
	int             len;
	int             max_len;


	/*
	 * Internal data contains char frequencies table, copy the table to the context
	 */
	memcpy(huffman_context->freq, internal_data, sizeof(huffman_context->freq));

	/*
	 * Count diff. chars
	 */
	for (i = 0; i < 256; i++)
	{
		if (huffman_context->freq[(unsigned char)i] > 0)
		{
			huffman_context->freq_count++;
		}
	}

	build_initial_tree(huffman_context);
	build_tree_no_codes(huffman_context);

	idx_bit = 0;

	p = huffman_context->tree + huffman_context->node_count - 1; /* p points to the root of the tree */

	*data   = 0;
	len     = 0;
	max_len = 0;

	node = -1;

	while ((bit = decompose_code(code, code_size, &idx_bit)) != -1)
	{
		if (bit == 0)
		{
			node = p->left;
		}
		else if (bit == 1)
		{
			node = p->right;
		}

		p = huffman_context->tree + node;

		if (p->left == -1 && p->right == -1) /* terminal? */
		{
			*data = add_data(*data, p->c, &len, &max_len);
			p = huffman_context->tree + huffman_context->node_count - 1; /* reset to root */
		}
	}

	*data_size = len;

	return 0;
}


void huffman_destroy_context(huffman_context_t* huffman_context)
{
	int i;


	if (huffman_context == NULL)
	{
		return;
	}

	/*
	 * Free character codes
	 */
	for (i = 0; i < 256; i++)
	{
		if (huffman_context->codes[i].code != NULL)
		{
			free(huffman_context->codes[i].code);
		}
	}

	/*
	 * Free huffman tree
	 */
	if (huffman_context->tree != NULL)
	{
		free(huffman_context->tree);
	}

	/*
	 * Free the context itself
	 */
	free(huffman_context);
}
