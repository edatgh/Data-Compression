
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/my_io.h"
#include "data_model.h"


static void usage(const char* cmd)
{
	printf("USAGE: %s <cd> <input_file_name> <output_file_name>\n", cmd);
}


static void compress_file(const char* input_file_name, const char* output_file_name)
{
	data_model_t* dm;

	int           tn[] = {
//		DICT_FORWARD_TRANSFORMATION,
		BWT_FORWARD_TRANSFORMATION,
//		MTF_FORWARD_TRANSFORMATION,
//		BWT_FORWARD_TRANSFORMATION,
//		MTF_FORWARD_TRANSFORMATION,
//		RLE0_FORWARD_TRANSFORMATION,
//		BWT_FORWARD_TRANSFORMATION,
//		MTF_FORWARD_TRANSFORMATION,
//		RLE0_FORWARD_TRANSFORMATION,
//		RLE_FORWARD_TRANSFORMATION,
//		HUFFMAN_FORWARD_TRANSFORMATION,
//		BWT_FORWARD_TRANSFORMATION,
//		MTF_FORWARD_TRANSFORMATION,
	};

	char*         input;
	unsigned int  input_size;
	char*         output;
	unsigned int  output_size;
	


	printf("Reading input file: %s... ", input_file_name);
	fflush(stdout);
	my_io_read_file(input_file_name, &input, &input_size);
	printf("Done.\n");

	dm = data_model_create(tn, sizeof(tn) / sizeof(tn[0]));
	data_model_execute_transformation(dm, input, input_size, &output, &output_size);

	if (my_io_write_file(output_file_name, output, output_size, 0) != 0)
	{
		printf("Unable to write output file!\n");
	}
}


static void decompress_file(const char* input_file_name, const char* output_file_name)
{

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
