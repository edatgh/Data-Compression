
#include <stdlib.h>
#include "../ext/compression.h"
#include "rle2.h"


/*
 * RLE2 interface
 */
extern engine_t rle_engine;
extern int      rle_o;


rle2_context_t* rle2_create_context(void)
{
	return NULL;
}


int rle2_encode(rle2_context_t* rle2_context, char* data, unsigned int data_size,
		char** code, unsigned int* code_size,
		char** internal_data, unsigned int* internal_data_size)
{
	data_t  in;
	data_t* out;


	in.data = data;
	in.size = data_size;

	out = rle_engine.enc(&in);

	*code = out->data;
	*code_size = out->size;

	*internal_data = NULL;
	*internal_data_size = 0;

	return 0;
}


int rle2_decode(rle2_context_t* rle2_context, char* code, unsigned int code_size,
		char** data, unsigned int* data_size,
		char* internal_data, unsigned int internal_data_size)
{
	return 0;
}


void rle2_destroy_context(rle2_context_t* rle2_context)
{
	/* TODO */
}
