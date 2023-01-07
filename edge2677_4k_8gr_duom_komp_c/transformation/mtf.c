
#include <stdlib.h>
#include <string.h>
#include "mtf.h"


/*
 * Private
 */

static char* add_byte(char* buf, unsigned char byte, unsigned int* size, unsigned int* max_size)
{
	if (*size == *max_size)
	{
		*max_size += 32;
		buf = (char *)realloc(buf, *max_size);
	}

	buf[(*size)++] = byte;

	return buf;
}


static void create_dictionary(mtf_context_t* mtf_context, char* data, unsigned int data_size)
{
	unsigned int freq[256];
	int          i;
	unsigned int size;
	unsigned int max_size;


	memset(freq, 0, sizeof(freq));

	size              = 0;
	max_size          = 0;
	mtf_context->dict = NULL;
	mtf_context->size = 0;

	for (i = 0; i < data_size; i++)
	{
		if (freq[(unsigned char)data[i]]++ == 0)
		{
			mtf_context->dict = add_byte(mtf_context->dict, data[i], &size, &max_size);
		}
	}

	mtf_context->size = size;
}


static void move_to_front(char* data, int idx)
{
	char byte;


	byte = data[idx];

	for (; idx > 0; idx--)
	{
		data[idx] = data[idx - 1];
	}

	data[0] = byte;
}


/*
 * Public
 */

mtf_context_t* mtf_create_context(void)
{
	mtf_context_t* mtf_context;


	mtf_context = (mtf_context_t *)calloc(1, sizeof(*mtf_context));

	return mtf_context;
}


int mtf_encode(mtf_context_t* mtf_context, char* data, unsigned int data_size,
	       char** code, unsigned int* code_size,
	       char** internal_data, unsigned int* internal_data_size)
{
	int      i;
	int      j;
	unsigned max_size;


	/*
	 * Create dictionary, i.e. set of different symbols from data
	 */
	create_dictionary(mtf_context, data, data_size);

	/*
	 * The dictionary is the internal data of the MTF
	 */
	*internal_data = (char *)calloc(mtf_context->size, sizeof(char));
	memcpy(*internal_data, mtf_context->dict, mtf_context->size);
	*internal_data_size = mtf_context->size;

	*code      = NULL;
	*code_size = 0;
	max_size   = 0;

	for (i = 0; i < data_size; i++)
	{
		for (j = 0; j < mtf_context->size; j++)
		{
			if (data[i] == mtf_context->dict[j])
			{
				*code = add_byte(*code, (unsigned char)j, code_size, &max_size);
				move_to_front(mtf_context->dict, j);
				break;
			}
		}
	}
	
	return 0;
}


int mtf_decode(mtf_context_t* mtf_context, char* code, unsigned int code_size,
	       char** data, unsigned int* data_size,
	       char* internal_data, unsigned int internal_data_size)
{
	int          i;
	unsigned int max_size;


	/*
	 * Copy dictionary
	 */
	mtf_context->dict = (char *)calloc(internal_data_size, sizeof(char));
	memcpy(mtf_context->dict, internal_data, internal_data_size);
	mtf_context->size = internal_data_size;

	*data      = NULL;
	*data_size = 0;
	max_size   = 0;

	for (i = 0; i < code_size; i++)
	{
		*data = add_byte(*data, mtf_context->dict[(unsigned char)code[i]], data_size, &max_size);
		move_to_front(mtf_context->dict, code[i]);
	}

	return 0;
}


void mtf_destroy_context(mtf_context_t* mtf_context)
{
	if (mtf_context == NULL)
	{
		return;
	}

	if (mtf_context->dict != NULL)
	{
		free(mtf_context->dict);
	}

	free(mtf_context);
}
