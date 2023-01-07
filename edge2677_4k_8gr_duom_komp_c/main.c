
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/my_io.h"
#include "transformation/bwt.h"
#include "transformation/mtf.h"
#include "transformation/rle.h"
#include "transformation/huffman.h"


static void usage(const char* cmd)
{
	printf("USAGE: %s <cd> <input_file_name> <output_file_name>\n", cmd);
}


static void compress_file(const char* input_file_name, const char* output_file_name)
{
	char*          file_data;
	unsigned int   data_size;

	bwt_context_t* bwt_enc_ctx;
	char*          bwt_code;
	unsigned int   bwt_code_size;
	char*          bwt_internal_data;
	unsigned int   bwt_internal_data_size;

	mtf_context_t* mtf_enc_ctx;
	char*          mtf_code;
	unsigned int   mtf_code_size;
	char*          mtf_internal_data;
	unsigned int   mtf_internal_data_size;

	rle_context_t* rle_enc_ctx;
	char*          rle_code;
	unsigned int   rle_code_size;
	char*          rle_internal_data;
	unsigned int   rle_internal_data_size;

	bwt_context_t* bwt_enc_ctx1;
	char*          bwt_code1;
	unsigned int   bwt_code_size1;
	char*          bwt_internal_data1;
	unsigned int   bwt_internal_data_size1;

	mtf_context_t* mtf_enc_ctx1;
	char*          mtf_code1;
	unsigned int   mtf_code_size1;
	char*          mtf_internal_data1;
	unsigned int   mtf_internal_data_size1;

	rle_context_t* rle_enc_ctx1;
	char*          rle_code1;
	unsigned int   rle_code_size1;
	char*          rle_internal_data1;
	unsigned int   rle_internal_data_size1;

	huffman_context_t* huffman_enc_ctx;
	char*              huffman_code;
	unsigned int       huffman_code_size;
	char*              huffman_internal_data;
	unsigned int       huffman_internal_data_size;


	printf("Reading input file: %s... ", input_file_name);
	fflush(stdout);
	my_io_read_file(input_file_name, &file_data, &data_size);
	printf("Done.\n");

	bwt_enc_ctx     = bwt_create_context();
	mtf_enc_ctx     = mtf_create_context();
	rle_enc_ctx     = rle_create_context();
	bwt_enc_ctx1    = bwt_create_context();
	mtf_enc_ctx1    = mtf_create_context();
	rle_enc_ctx1    = rle_create_context();
	huffman_enc_ctx = huffman_create_context();

	printf("Running 1st BWT transformation... ");
	fflush(stdout);
	bwt_encode(bwt_enc_ctx, file_data, data_size, &bwt_code, &bwt_code_size,
		   &bwt_internal_data, &bwt_internal_data_size);
	printf("Done.\n");

	printf("Running 1st MTF transformation... ");
	fflush(stdout);
	mtf_encode(mtf_enc_ctx, bwt_code, bwt_code_size, &mtf_code, &mtf_code_size,
		   &mtf_internal_data, &mtf_internal_data_size);
	printf("Done.\n");

	printf("Running 1st RLE transformation... ");
	fflush(stdout);
	rle_encode(rle_enc_ctx, mtf_code, mtf_code_size, &rle_code, &rle_code_size,
		   &rle_internal_data, &rle_internal_data_size);
	printf("Done.\n");

	printf("Running 2nd BWT transformation... ");
	fflush(stdout);
	bwt_encode(bwt_enc_ctx1, rle_code, rle_code_size, &bwt_code1, &bwt_code_size1,
		   &bwt_internal_data1, &bwt_internal_data_size1);
	printf("Done.\n");

	printf("Running 2nd MTF transformation... ");
	fflush(stdout);
	mtf_encode(mtf_enc_ctx1, bwt_code1, bwt_code_size1, &mtf_code1, &mtf_code_size1,
		   &mtf_internal_data1, &mtf_internal_data_size1);
	printf("Done.\n");

	printf("Running 2nd RLE transformation... ");
	fflush(stdout);
	rle_encode(rle_enc_ctx1, mtf_code1, mtf_code_size1, &rle_code1, &rle_code_size1,
		   &rle_internal_data1, &rle_internal_data_size1);
	printf("Done.\n");

	printf("Running 1st HUFFMAN transformation... ");
	fflush(stdout);
	huffman_encode(huffman_enc_ctx, rle_code, rle_code_size, &huffman_code, &huffman_code_size,
		       &huffman_internal_data, &huffman_internal_data_size);
	printf("Done.\n");

	printf("Writting output file: %s... ", output_file_name);
	fflush(stdout);
	my_io_write_file(output_file_name, huffman_code, huffman_code_size, 0);
	printf("Done.\n");

	if (huffman_internal_data != NULL)
	{
		free(huffman_internal_data);
	}
	free(huffman_code);

	if (rle_internal_data1 != NULL)
	{
		free(rle_internal_data1);
	}
	free(rle_code1);

	if (mtf_internal_data1 != NULL)
	{
		free(mtf_internal_data1);
	}
	free(mtf_code1);

	if (bwt_internal_data1 != NULL)
	{
		free(bwt_internal_data1);
	}
	free(bwt_code1);

	if (rle_internal_data != NULL)
	{
		free(rle_internal_data);
	}
	free(rle_code);

	if (mtf_internal_data != NULL)
	{
		free(mtf_internal_data);
	}
	free(mtf_code);

	if (bwt_internal_data != NULL)
	{
		free(bwt_internal_data);
	}
	free(bwt_code);

	huffman_destroy_context(huffman_enc_ctx);
	rle_destroy_context(rle_enc_ctx1);
	mtf_destroy_context(mtf_enc_ctx1);
	bwt_destroy_context(bwt_enc_ctx1);
	rle_destroy_context(rle_enc_ctx);
	mtf_destroy_context(mtf_enc_ctx);
	bwt_destroy_context(bwt_enc_ctx);

	free(file_data);
}


static void decompress_file(const char* input_file_name, const char* output_file_name)
{
#if 0
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
#endif
}


int main(int argc, char** argv)
{
	if (argc != 4)
	{
		usage(argv[0]);
		return -1;
	}

	if (strcmp(argv[1], "c") == 0)
	{
		compress_file(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "d") == 0)
	{
		decompress_file(argv[2], argv[3]);
	}
	else
	{
		usage(argv[0]);
		return -1;
	}

	return 0;
}
