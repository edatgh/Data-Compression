
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bword_io.h"
#include "rb_tree.h"
#include "my_io.h"


#define NR_BITS_IN_WORD 8


typedef struct _my_data_t
{
	uint32_t cnt;
	uint64_t val;
} my_data_t;


typedef struct _cb_data_t
{
	uint32_t    n;
	double      entropy;
	uint64_t    freq_max;
	rbt_node_t* freq_freq;
	uint32_t    n_freq;
} cb_data_t;


static uint64_t* p_buf = NULL;


void my_cb(void* data, void* arg_cb)
{
	my_data_t*  my_data;
	cb_data_t*  cb_data;
	double      p;
	rbt_node_t* node;
	uint32_t    cnt;


	my_data = (my_data_t*)data;
	cb_data = (cb_data_t*)arg_cb;

	cb_data->n_freq++;

	p = (double)my_data->cnt / (double)cb_data->n;

	cb_data->entropy -= p * (log(p) / log(2));

	if (cb_data->freq_max < my_data->cnt)
	{
		cb_data->freq_max = my_data->cnt;
	}

	cnt = my_data->cnt;

	if (cb_data->freq_freq != NULL)
	{
		//*p_buf++ = my_data->val;
#if 0
		printf("Symbol: 0x%08x%08x, freq: %u\n", (unsigned int)(my_data->val >> 32),
	       (unsigned int)(my_data->val & 0xffffffff), my_data->cnt);
#endif
		node = rbt_find(cb_data->freq_freq, cnt);
		if (node != NULL)
		{
			my_data = rbt_get(node);
			my_data->cnt++;
		}
		else
		{
			my_data = (my_data_t*)calloc(1, sizeof(*my_data));
			my_data->cnt = 1;
			my_data->val = cnt;
			cb_data->freq_freq = rbt_insert(cb_data->freq_freq, cnt, my_data);
		}
	}
	else
	{
#if 0
		printf("Frequency: 0x%08x%08x, freq: %u\n", (unsigned int)(my_data->val >> 32),
		       (unsigned int)(my_data->val & 0xffffffff), my_data->cnt);
#endif
	}
}


static void usage(const char* cmd)
{
	printf("USAGE: %s <file_name>\n", cmd);
}


int main(int argc, char** argv)
{
	bword_io_context_t* bword_io_context;
	bword_t*            bword;
	rbt_node_t*         freq;
	rbt_node_t*         node;
	rbt_node_t*         freq_freq;
	char*               buf;
	unsigned int        buf_size;
	my_data_t*          my_data;
	cb_data_t*          cb_data;
	uint32_t            n;
	void*               a_buf;


	//a_buf = malloc((8125400 + 10000) * sizeof(uint64_t));
	p_buf = (uint64_t*)a_buf;

	if (argc != 2)
	{
		usage(argv[0]);

		return -1;
	}

	freq = rbt_init();
	if (freq == NULL)
	{
		printf("rbt_init() failed!\n");

		return -1;
	}

	freq_freq = rbt_init();
	if (freq_freq == NULL)
	{
		rbt_destroy(freq);
		printf("rbt_init() failed!\n");

		return -1;
	}

	if (my_io_read_file(argv[1], &buf, &buf_size) == -1)
	{
		rbt_destroy(freq_freq);
		rbt_destroy(freq);
		printf("my_io_read_file() failed!\n");

		return -1;
	}

	bword_io_context = bword_io_create_context();
	if (bword_io_context == NULL)
	{
		free(buf);
		rbt_destroy(freq_freq);
		rbt_destroy(freq);
		printf("bword_io_context_create() failed!\n");

		return -1;
	}

	n = 0;

	while ((bword = bword_io_read_word(bword_io_context, buf, buf_size, NR_BITS_IN_WORD)) != NULL)
	{
		node = rbt_find(freq, bword->word);
		if (node != NULL)
		{
			my_data = rbt_get(node);
			my_data->cnt++;
		}
		else
		{
			my_data = (my_data_t*)calloc(1, sizeof(*my_data));
			my_data->cnt = 1;
			my_data->val = bword->word;
			freq = rbt_insert(freq, bword->word, my_data);
		}

		n++;
	}

	cb_data = (cb_data_t*)calloc(1, sizeof(*cb_data));

	cb_data->n = n;
	cb_data->freq_freq = freq_freq;

	printf("--- Symbols ----------------------------------------------------\n");

	rbt_traverse(freq, my_cb, cb_data);

	printf("Entropy of symbols: %f\n", cb_data->entropy);
	printf("Max. frequency    : 0x%08x%08x\n",
	       (uint32_t)(cb_data->freq_max >> 32),
	       (uint32_t)(cb_data->freq_max & 0xffffffff));

	printf("\nCount of symbols: %u\n", n);

	n = cb_data->n_freq; /* take a number of frequencies (after adding them to a tree) */
	freq_freq = cb_data->freq_freq;

	memset(cb_data, 0, sizeof(*cb_data));

	cb_data->n = n;

	printf("\n--- Frequencies ------------------------------------------------\n");

	rbt_traverse(freq_freq, my_cb, cb_data);

	printf("Entropy of frequencies: %f\n", cb_data->entropy);
	printf("Max. frequency        : 0x%08x%08x\n",
	       (uint32_t)(cb_data->freq_max >> 32),
	       (uint32_t)(cb_data->freq_max & 0xffffffff));

	printf("\nCount of frequencies (count of different symbols in the input): %u\n", n);

	free(cb_data);
	bword_io_destroy_context(bword_io_context);
	free(buf);
	rbt_destroy(freq_freq);
	rbt_destroy(freq);

	/*
	 * Writing different symbols to a file
	 */
	//my_io_write_file("diff_syms.out", (char*)a_buf, (unsigned int)p_buf - (unsigned int)a_buf, 0);

	return 0;
}
