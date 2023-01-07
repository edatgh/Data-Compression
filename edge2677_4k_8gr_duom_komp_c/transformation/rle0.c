
#include <stdlib.h>
#include "../ext/compression.h"
#include "rle0.h"


/*
 * RLE0 interface
 */
extern engine_t rle0_engine;
extern int      rle0_o;


rle0_context_t* rle0_create_context(void)
{
	return NULL;
}


int rle0_encode(rle0_context_t* rle0_context, char* data, unsigned int data_size,
		char** code, unsigned int* code_size,
		char** internal_data, unsigned int* internal_data_size)
{
	data_t  in;
	data_t* out;


	in.data = data;
	in.size = data_size;

	out = rle0_engine.enc(&in);

	*code = out->data;
	*code_size = out->size;

	*internal_data = NULL;
	*internal_data_size = 0;

	return 0;
}


int rle0_decode(rle0_context_t* rle0_context, char* code, unsigned int code_size,
		char** data, unsigned int* data_size,
		char* internal_data, unsigned int internal_data_size)
{
	return 0;
}


void rle0_destroy_context(rle0_context_t* rle0_context)
{
	/* TODO */
}
