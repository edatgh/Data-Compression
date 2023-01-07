
#include <stdio.h>
#include <stdlib.h>
#include "transformation/bwt.h"
#include "transformation/mtf.h"
#include "transformation/rle.h"
#include "transformation/huffman.h"


int main(int argc, char** argv)
{
	bwt_context_t* bwt_enc_ctx;
	bwt_context_t* bwt_dec_ctx;
	char*          bwt_data;
	unsigned int   bwt_data_size;
	char*          bwt_code;
	unsigned int   bwt_code_size;
	char*          bwt_internal_data;
	unsigned int   bwt_internal_data_size;

	mtf_context_t* mtf_enc_ctx;
	mtf_context_t* mtf_dec_ctx;
	char*          mtf_data;
	unsigned int   mtf_data_size;
	char*          mtf_code;
	unsigned int   mtf_code_size;
	char*          mtf_internal_data;
	unsigned int   mtf_internal_data_size;

	rle_context_t* rle_enc_ctx;
	rle_context_t* rle_dec_ctx;
	char*          rle_data;
	unsigned int   rle_data_size;
	char*          rle_code;
	unsigned int   rle_code_size;
	char*          rle_internal_data;
	unsigned int   rle_internal_data_size;

	huffman_context_t* huffman_enc_ctx;
	huffman_context_t* huffman_dec_ctx;
	char*              huffman_data;
	unsigned int       huffman_data_size;
	char*              huffman_code;
	unsigned int       huffman_code_size;
	char*              huffman_internal_data;
	unsigned int       huffman_internal_data_size;


	/*
	 * BWT Demo
	 */
	bwt_enc_ctx = bwt_create_context();
	bwt_dec_ctx = bwt_create_context();

	bwt_encode(bwt_enc_ctx, "ababab\0\0\0", 9, &bwt_code, &bwt_code_size,
		   &bwt_internal_data, &bwt_internal_data_size);
	{
		int i;


		printf("BWT code: ");

		for (i = 0; i < bwt_code_size; i++)
		{
			printf("%c", bwt_code[i]);
		}

		printf("\n");
	}

	bwt_decode(bwt_dec_ctx, bwt_code, bwt_code_size, &bwt_data, &bwt_data_size,
		   bwt_internal_data, bwt_internal_data_size);

	{
		int i;


		printf("BWT data: ");

		for (i = 0; i < bwt_data_size; i++)
		{
			printf("%c", bwt_data[i]);
		}

		printf("\n");
	}

	free(bwt_data);
	free(bwt_internal_data);
	free(bwt_code);

	bwt_destroy_context(bwt_dec_ctx);
	bwt_destroy_context(bwt_enc_ctx);


	/*
	 * MTF Demo
	 */
	mtf_enc_ctx = mtf_create_context();
	mtf_dec_ctx = mtf_create_context();

	mtf_encode(mtf_enc_ctx, "aaabbb", 6, &mtf_code, &mtf_code_size,
		   &mtf_internal_data, &mtf_internal_data_size);

	{
		int i;


		printf("MTF code: ");

		for (i = 0; i < mtf_code_size; i++)
		{
			printf("%d", mtf_code[i]);
		}

		printf("\n");
	}

	mtf_decode(mtf_dec_ctx, mtf_code, mtf_code_size, &mtf_data, &mtf_data_size,
		   mtf_internal_data, mtf_internal_data_size);

	{
		int i;


		printf("MTF data: ");

		for (i = 0; i < mtf_data_size; i++)
		{
			printf("%c", mtf_data[i]);
		}

		printf("\n");
	}

	free(mtf_data);
	free(mtf_internal_data);
	free(mtf_code);

	mtf_destroy_context(mtf_dec_ctx);
	mtf_destroy_context(mtf_enc_ctx);


	/*
	 * RLE Demo
	 */
	rle_enc_ctx = rle_create_context();
	rle_dec_ctx = rle_create_context();

	rle_encode(rle_enc_ctx, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacccccccccccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaccccccccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbb", 1109, &rle_code, &rle_code_size,
		   &rle_internal_data, &rle_internal_data_size);
	{
		int i;


		printf("RLE code: ");

		for (i = 0; i < rle_code_size; i++)
		{
			printf("%02x", (unsigned char)rle_code[i]);
		}

		printf("\n");
	}

	rle_decode(rle_dec_ctx, rle_code, rle_code_size, &rle_data, &rle_data_size,
		   rle_internal_data, rle_internal_data_size);

	{
		int i;


		printf("RLE data: ");

		for (i = 0; i < rle_data_size; i++)
		{
			printf("%c", rle_data[i]);
		}

		printf("\n");
	}

	free(rle_data);

	if (rle_internal_data != NULL)
	{
		free(rle_internal_data);
	}

	free(rle_code);

	rle_destroy_context(rle_dec_ctx);
	rle_destroy_context(rle_enc_ctx);


	/*
	 * HUFFMAN Demo
	 */
	huffman_enc_ctx = huffman_create_context();
	huffman_dec_ctx = huffman_create_context();

	huffman_encode(huffman_enc_ctx, "abcdef", 6,
		       &huffman_code, &huffman_code_size,
		       &huffman_internal_data, &huffman_internal_data_size);
	{
		int i;


		printf("HUFFMAN code: ");

		for (i = 0; i < huffman_code_size; i++)
		{
			printf("%02x", (unsigned char)huffman_code[i]);
		}

		printf("\n");
	}

	huffman_decode(huffman_dec_ctx, huffman_code, huffman_code_size, &huffman_data, &huffman_data_size,
		       huffman_internal_data, huffman_internal_data_size);

	{
		int i;


		printf("HUFFMAN data: ");

		for (i = 0; i < huffman_data_size; i++)
		{
			printf("%c", huffman_data[i]);
		}

		printf("\n");
	}

	free(huffman_data);

	if (huffman_internal_data != NULL)
	{
		free(huffman_internal_data);
	}

	free(huffman_code);

	huffman_destroy_context(huffman_dec_ctx);
	huffman_destroy_context(huffman_enc_ctx);

	return 0;
}
