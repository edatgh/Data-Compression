
#include <stdlib.h>
#include <string.h>
#include "bwt.h"


/*
 * Private
 */

/*
 * Global
 */
static unsigned char* compar_buf      = NULL;
static int   compar_buf_size = 0;


static int my_compar(const void* a, const void *b)
{
	int ai;
	int bi;
	int l;


	ai = *(const int*)a;
	bi = *(const int*)b;
	l  = compar_buf_size;

	while (compar_buf[ai] == compar_buf[bi])
	{
		if (++ai == compar_buf_size)
		{
			ai = 0;
		}

		if (++bi == compar_buf_size)
		{
			bi = 0;
		}

		if (--l == 0)
		{
			return 0;
		}
	}

	if (compar_buf[ai] > compar_buf[bi])
	{
		return 1;
	}
	else
	{
		return -1;
	}
}


/*
 * Public
 */

bwt_context_t* bwt_create_context(void)
{
	return NULL;
}


int bwt_encode(bwt_context_t* bwt_context, char* data, unsigned int data_size,
	       char** code, unsigned int* code_size,
	       char** internal_data, unsigned int* internal_data_size)
{
	int* indexes;
	int  i;
	int  primary_index;


	indexes = (int*)calloc(data_size, sizeof(*indexes));

	for (i = 0; i < data_size; i++)
	{
		indexes[i] = i;
	}

	compar_buf      = data;
	compar_buf_size = data_size;

	qsort(indexes, data_size, sizeof(*indexes), my_compar);

	*code      = (char*)calloc(data_size, sizeof(char));
	*code_size = data_size;

	for (i = 0; i < data_size; i++)
	{
		(*code)[i] = data[(indexes[i] + data_size - 1) % data_size];
	}

	for (i = 0; i < data_size; i++)
	{
		if (indexes[i] ==  1)
		{
			primary_index = i;
		}
	}

	*internal_data = (char*)calloc(1, sizeof(primary_index));
	memcpy(*internal_data, &primary_index, sizeof(primary_index));
	*internal_data_size = sizeof(primary_index);

	free(indexes);

	return 0;
}


int bwt_decode(bwt_context_t* bwt_context, char* code, unsigned int code_size,
	       char** data, unsigned int* data_size,
	       char* internal_data, unsigned int internal_data_size)
{
	unsigned char* aux;
	int            buckets[256];
	int            primary_index;
	int            i, j, k;
	int*           indexes;


	aux     = (char*)calloc(code_size, sizeof(*aux));
	indexes = (int*)calloc(code_size, sizeof(*indexes));

	memcpy(&primary_index, internal_data, sizeof(primary_index));

	*data      = (char*)calloc(code_size, sizeof(char));
	*data_size = code_size;

	for (i = 0; i < 256; i++)
	{
		buckets[i] = 0;
	}

	for (i = 0; i < code_size; i++)
	{
		buckets[(unsigned char)code[i]]++;
	}

	for (i = k = 0; i < 256; i++)
	{
		for (j = 0; j < buckets[i]; j++)
		{
			aux[k++] = i;
		}
	}

	for (i = j = 0; i < 256; i++)
	{
		while (i > aux[j] && j < code_size)
		{
			j++;
		}

		buckets[i] = j;
	}

	for (i = 0; i < code_size; i++)
	{
		indexes[buckets[(unsigned char)code[i]]++] = i;
	}

	for (i = 0, j = primary_index; i < code_size; i++)
	{
		(*data)[i] = code[j];
		j = indexes[j];
	}

	free(indexes);
	free(aux);

	return 0;
}


void bwt_destroy_context(bwt_context_t* bwt_context)
{
	if (bwt_context == NULL)
	{
		return;
	}

	free(bwt_context);
}
