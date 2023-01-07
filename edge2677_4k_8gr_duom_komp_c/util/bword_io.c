
#include <stdlib.h>
#include <stdio.h>
#include "bword_io.h"


/*
 * Private
 */

static int32_t read_byte(uint8_t* data, uint32_t data_size, uint32_t byte_idx, uint32_t* byte)
{
	if (byte_idx < data_size)
	{
		*byte = data[byte_idx] & 0xff;

		return 0;
	}
	else
	{
		return -1;
	}
}


static int32_t read_bit(uint8_t* data, uint32_t data_size, uint32_t bit_idx, uint32_t* bit)
{
	uint32_t byte_idx;
	uint32_t byte;


	byte_idx = bit_idx / 8;

	if (read_byte(data, data_size, byte_idx, &byte) == -1)
	{
		return -1;
	}

	*bit = ((byte & (1 << (bit_idx % 8))) > 0);

	return 0;
}


/*
 * Public
 */
bword_io_context_t* bword_io_create_context(void)
{
	bword_io_context_t* bword_context;


	bword_context = (bword_io_context_t*)calloc(1, sizeof(*bword_context));
	if (bword_context == NULL)
	{
		return NULL;
	}

	return bword_context;
}


bword_t* bword_io_read_word(bword_io_context_t* bword_io_context, uint8_t* data, uint32_t data_size, uint32_t word_size)
{
	bword_t* bword;
	uint32_t n;
	uint32_t i;
	uint32_t bit;


	if (data_size == bword_io_context->bits_read / 8)
	{
		return NULL;
	}

	bword = (bword_t*)calloc(1, sizeof(*bword));
	if (bword == NULL)
	{
		return NULL;
	}

	n = bword_io_context->bits_read;

	for (i = n; i < n + word_size; i++)
	{
		if (read_bit(data, data_size, i, &bit) == 0)
		{
			bword->word |= ((uint64_t)bit) << (i - n);
			bword_io_context->bits_read++;
		}
		else
		{
			break;
		}
	}

	return bword;
}


void bword_io_destroy_context(bword_io_context_t* bword_io_context)
{
	if (bword_io_context == NULL)
	{
		return;
	}

	free(bword_io_context);
}
