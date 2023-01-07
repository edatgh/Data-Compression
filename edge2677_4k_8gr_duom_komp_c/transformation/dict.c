
#include <stdlib.h>
#include "../ext/compression.h"
#include "dict.h"


/*
 * DICT internal low-level interface
 */
extern engine_t dict_engine;


dict_context_t* dict_create_context(void)
{
	return NULL;
}


int dict_encode(dict_context_t* dict_context, char* data, unsigned int data_size,
		char** code, unsigned int* code_size,
		char** internal_data, unsigned int* internal_data_size)
{
	data_t  in;
	data_t* out;


	in.data = data;
	in.size = data_size;

	out = dict_engine.enc(&in);

	*code = out->data;
	*code_size = out->size;

	*internal_data = NULL;
	*internal_data_size = 0;

	return 0;
}


int dict_decode(dict_context_t* dict_context, char* code, unsigned int code_size,
		char** data, unsigned int* data_size,
		char* internal_data, unsigned int internal_data_size)
{
	return 0;
}


void dict_destroy_context(dict_context_t* dict_context)
{
	/* TODO */
}
