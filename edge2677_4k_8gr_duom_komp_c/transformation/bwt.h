
#ifndef __BWT_H__
#define __BWT_H__


typedef struct _bwt_context_t
{
	/* reserved */
} bwt_context_t;


bwt_context_t* bwt_create_context (void);

int            bwt_encode         (bwt_context_t* bwt_context, char* data, unsigned int data_size,
				   char** code, unsigned int* code_size,
				   char** internal_data, unsigned int* internal_data_size);

int            bwt_decode         (bwt_context_t* bwt_context, char* code, unsigned int code_size,
				   char** data, unsigned int* data_size,
				   char* internal_data, unsigned int internal_data_size);

void           bwt_destroy_context(bwt_context_t* bwt_context);


#endif /* __BWT_H__ */
