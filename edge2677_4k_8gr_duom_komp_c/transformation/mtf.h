
#ifndef __MTF_H__
#define __MTF_H__


typedef struct _mtf_context_t
{
	char*        dict;
	unsigned int size;
} mtf_context_t;


mtf_context_t* mtf_create_context (void);

int            mtf_encode         (mtf_context_t* mtf_context, char* data, unsigned int data_size,
				   char** code, unsigned int* code_size,
				   char** internal_data, unsigned int* internal_data_size);

int            mtf_decode         (mtf_context_t* mtf_context, char* code, unsigned int code_size,
				   char** data, unsigned int* data_size,
				   char* internal_data, unsigned int internal_data_size);

void           mtf_destroy_context(mtf_context_t* mtf_context);


#endif /* __MTF_H__ */
