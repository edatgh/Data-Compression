
#ifndef __DICT_H__
#define __DICT_H__


typedef struct _dict_context_t
{
	/* reserved */
} dict_context_t;


dict_context_t* dict_create_context(void);

int dict_encode(dict_context_t* dict_context, char* data, unsigned int data_size,
		char** code, unsigned int* code_size,
		char** internal_data, unsigned int* internal_data_size);

int dict_decode(dict_context_t* dict_context, char* code, unsigned int code_size,
		char** data, unsigned int* data_size,
		char* internal_data, unsigned int internal_data_size);

void dict_destroy_context(dict_context_t* dict_context);


#endif /* __DICT_H__ */
