
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "util/common.h"
#include "util/my_io.h"
#include "transformation/bwt.h"
#include "transformation/mtf.h"
#include "transformation/rle.h"
#include "transformation/huffman.h"
#include "transformation/rle2.h"
#include "transformation/dict.h"
#include "transformation/rle0.h"
#include "data_model.h"


data_model_t* data_model_create(int* transformation_names, int transformation_count)
{
	data_model_t* data_model;
	int           i;
	int           idx;


	data_model = (data_model_t*)calloc(1, sizeof(*data_model));

	data_model->transformations = (transformation_t*)calloc(transformation_count,
								sizeof(*data_model->transformations));

	data_model->transformation_names = (int*)calloc(transformation_count,
							sizeof(*data_model->transformation_names));
	memcpy(data_model->transformation_names, transformation_names,
	       transformation_count * sizeof(*data_model->transformation_names));

	idx = 0;

	for (i = 0; i < transformation_count; i++)
	{
		switch (transformation_names[i])
		{
		case BWT_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("BWT(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = bwt_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)bwt_encode;
			idx++;
			break;

		case MTF_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("MTF(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = mtf_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)mtf_encode;
			idx++;
			break;

		case RLE_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = rle_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)rle_encode;
			idx++;
			break;

		case HUFFMAN_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("HUFFMAN(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = huffman_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)huffman_encode;
			idx++;
			break;

		case RLE2_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE2(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = rle2_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)rle2_encode;
			idx++;
			break;

		case DICT_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("DICT(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = dict_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)dict_encode;
			idx++;
			break;

		case RLE0_FORWARD_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE0(forward)");
			data_model->transformations[idx].type = FORWARD_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(forward_transformation_t));
			((forward_transformation_t*)data_model->transformations[idx].x)->encode_context = rle0_create_context();
			((forward_transformation_t*)data_model->transformations[idx].x)->p_encode = (ENCODE_METHOD_TYPE)rle0_encode;
			idx++;
			break;

		case BWT_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("BWT(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = bwt_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)bwt_decode;
			idx++;
			break;

		case MTF_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("MTF(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = mtf_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)mtf_decode;
			idx++;
			break;

		case RLE_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = rle_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)rle_decode;
			idx++;
			break;

		case HUFFMAN_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("HUFFMAN(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = huffman_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)huffman_decode;
			idx++;
			break;

		case RLE2_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE2(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = rle2_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)rle2_decode;
			idx++;
			break;

		case DICT_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("DICT(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = dict_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)dict_decode;
			idx++;
			break;

		case RLE0_INVERSE_TRANSFORMATION:
			data_model->transformations[idx].name = strdup("RLE0(inverse)");
			data_model->transformations[idx].type = INVERSE_TRANSFORMATION;
			data_model->transformations[idx].x = calloc(1, sizeof(inverse_transformation_t));
			((inverse_transformation_t*)data_model->transformations[idx].x)->decode_context = rle0_create_context();
			((inverse_transformation_t*)data_model->transformations[idx].x)->p_decode = (DECODE_METHOD_TYPE)rle0_decode;
			idx++;
			break;

		default:
			free(data_model->transformations);
			free(data_model);
			break;
		}
	}

	data_model->transformation_count = transformation_count;

	return data_model;
}


int data_model_execute_transformation(data_model_t* data_model, char* input, unsigned int input_size,
				      char** output, unsigned int* output_size)
{
	int            i;
	char*          p_data;
	unsigned int   size;
	struct timeval tv1;
	struct timeval tv2;


	p_data = input;
	size   = input_size;

	for (i = 0; i < data_model->transformation_count; i++)
	{
		printf("Executing transformation: %s\n", data_model->transformations[i].name);
		printf("\tInput size  : %u\n", size);

		switch (data_model->transformations[i].type)
		{
		case FORWARD_TRANSFORMATION:
			gettimeofday(&tv1, NULL);
			((forward_transformation_t*)data_model->transformations[i].x)->p_encode(
				((forward_transformation_t*)data_model->transformations[i].x)->encode_context,
				p_data,
				size,
				&((forward_transformation_t*)data_model->transformations[i].x)->code,
				&((forward_transformation_t*)data_model->transformations[i].x)->code_size,
				&((forward_transformation_t*)data_model->transformations[i].x)->internal_data,
				&((forward_transformation_t*)data_model->transformations[i].x)->internal_data_size);
			gettimeofday(&tv2, NULL);

			p_data = ((forward_transformation_t*)data_model->transformations[i].x)->code;
			size = ((forward_transformation_t*)data_model->transformations[i].x)->code_size;
			break;

		case INVERSE_TRANSFORMATION:
			break;

		default:
			continue;
			break;
		}

		printf("\tOutput size : %u\n", size);
		if (tv2.tv_usec < tv1.tv_usec)
		{
		    tv2.tv_usec += 1000000;
		    tv1.tv_sec  -= 1;
		}

		printf("\tEntropy     : %f\n", calculate_entropy(p_data, size));

		printf("\tTime elapsed: %lu.%lu sec.\n", tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);
	}

	*output = p_data;
	*output_size = size;

	return 0;
}


int data_model_save_to_file(data_model_t* data_model, char* data, unsigned int data_size, const char* file_name)
{
	int          i;
	char*        p;
	unsigned int s;


	my_io_write_file(file_name, (char*)&data_model->transformation_count,
			 sizeof(data_model->transformation_count), 0);

	my_io_write_file(file_name, (char*)data_model->transformation_names,
			 data_model->transformation_count * sizeof(*data_model->transformation_names), 1);

	for (i = 0; i < data_model->transformation_count; i++)
	{
		switch (data_model->transformations[i].type)
		{
		case FORWARD_TRANSFORMATION:
			p = ((forward_transformation_t*)data_model->transformations[i].x)->internal_data;
			s = ((forward_transformation_t*)data_model->transformations[i].x)->internal_data_size;
			if (s > 0)
			{
				my_io_write_file(file_name, (char*)&s, sizeof(s), 1);
				my_io_write_file(file_name, (char*)p, s, 1);
			}
			break;

		case INVERSE_TRANSFORMATION:
			p = ((inverse_transformation_t*)data_model->transformations[i].x)->internal_data;
			s = ((inverse_transformation_t*)data_model->transformations[i].x)->internal_data_size;
			if (s > 0)
			{
				my_io_write_file(file_name, (char*)&s, sizeof(s), 1);
				my_io_write_file(file_name, (char*)p, s, 1);
			}
			break;

		default:
			break;
		}
	}

	my_io_write_file(file_name, data, data_size, 1);
	
	return 0;
}


data_model_t* data_model_load_from_file(const char* file_name)
{
	int*          transformation_names;
	int           transformation_count;
	data_model_t* data_model;
	int           i;


	my_io_read_file(file_name, (char*)&transformation_count, sizeof(transformation_count));

	transformation_names = (int*)calloc(transformation_count, sizeof(*transformation_names));

	my_io_read_file(file_name, (char*)transformation_names,
			transformation_count * sizeof(*transformation_names));

	data_model = data_model_create(transformation_names, transformation_count);

	free(transformation_names);

	for (i = 0; i < transformation_count; i++)
	{
//		data_model->
	}

	return data_model;
}


void data_model_destroy(data_model_t* data_model)
{
	/* TODO */
}
