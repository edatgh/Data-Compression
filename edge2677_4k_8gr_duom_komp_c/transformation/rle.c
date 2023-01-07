
#include <stdlib.h>
#include <stdio.h>
#include "rle.h"


/*
 * Private
 */

static char* add_byte(char* buf, unsigned int* size, unsigned int* max_size, char byte)
{
	if (*size == *max_size)
	{
		*max_size += 256;
		buf = (char*)realloc(buf, *max_size);
	}

	buf[(*size)++] = byte;

	return buf;
}


static char* read_symbol(char* buf, unsigned int buf_size, unsigned int* symbol_len)
{
	char*        res;
	unsigned int size;
	unsigned int max_size;


	if (buf_size == 0)
	{
		return NULL;
	}

	res      = NULL;
	size     = 0;
	max_size = 0;

	while (buf_size-- > 1)
	{
		if (*buf++ == *(buf + 1))
		{
			res = add_byte(res, &size, &max_size, *buf);
		}
		else
		{
			break;
		}
	}

	res = add_byte(res, &size, &max_size, *(buf - 1));

	*symbol_len = size;

	return res;
}


/*
 * Public
 */

rle_context_t* rle_create_context(void)
{
	return NULL;
}


int rle_encode(rle_context_t* rle_context, char* data, unsigned int data_size,
	       char** code, unsigned int* code_size,
	       char** internal_data, unsigned int* internal_data_size)
{
	char*        p;
	char*        symbol;
	unsigned int symbol_len;
	unsigned int max_size;
	int          n;
	int          i;
	unsigned int overhead = 0;


	*code      = NULL;
	*code_size = 0;
	max_size   = 0;

	p = data;

	while ((symbol = read_symbol(p, data_size, &symbol_len)) != NULL)
	{
		if (symbol_len > 4)
		{
			*code = add_byte(*code, code_size, &max_size, 0);

			if (symbol_len > 65535) /* 4 bytes */
			{
				*code = add_byte(*code, code_size, &max_size, 4);
				n = 4;
			}
			else if (symbol_len > 255) /* 2 bytes */
			{
				*code = add_byte(*code, code_size, &max_size, 2);
				n = 2;
			}
			else /* 1 byte */
			{
				*code = add_byte(*code, code_size, &max_size, 1);
				n = 1;
			}

			for (i = 0; i < n; i++)
			{
				*code = add_byte(*code, code_size, &max_size, (symbol_len >> i * 8) & 0xff);
			}

			*code = add_byte(*code, code_size, &max_size, *symbol);

			overhead += 2 + n + 1;
		}
		else
		{
			for (i = 0; i < symbol_len; i++)
			{
				*code = add_byte(*code, code_size, &max_size, *symbol);
			}
		}

		/*
		 * Kind of shifting in the buffer
		 */
		data_size -= symbol_len;
		p         += symbol_len;

		free(symbol);
	}

	*internal_data      = NULL;
	*internal_data_size = 0;

	/**************************** DEBUG ******************/
	printf("\tRLE overhead: %u\n", overhead);
	fflush(stdout);
	/*****************************************************/

	return 0;
}


int rle_decode(rle_context_t* rle_context, char* code, unsigned int code_size,
	       char** data, unsigned int* data_size,
	       char* internal_data, unsigned int internal_data_size)
{
	unsigned int max_size;
	int          i;
	int          j;
	int          n;
	unsigned int symbol_len;


	*data      = NULL;
	*data_size = 0;
	max_size   = 0;

	i = 0;

	while (code_size > 0)
	{
		if (code[i] == 0) /* prefix found ? */
		{
			n = code[i + 1];

			symbol_len = 0;

			for (j = 0; j < n; j++)
			{
				symbol_len |= (unsigned char)code[i + 2 + j] << j * 8;
			}

			for (j = 0; j < symbol_len; j++)
			{
				*data = add_byte(*data, data_size, &max_size, code[i + 2 + n]);
			}

			i         += 1 + 1 + n + 1;
			code_size -= 1 + 1 + n + 1;
		}
		else
		{
			*data = add_byte(*data, data_size, &max_size, code[i]);

			i        ++;
			code_size--;
		}
	}

	return 0;
}


void rle_destroy_context(rle_context_t* rle_context)
{
	if (rle_context == NULL)
	{
		return;
	}

	free(rle_context);
}
