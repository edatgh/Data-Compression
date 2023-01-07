
#ifndef __RLE_H__
#define __RLE_H__


typedef struct _rle_context_t
{
} rle_context_t;


rle_context_t* rle_create_context (void);

int            rle_encode         (rle_context_t* rle_context, char* data, unsigned int data_size,
				   char** code, unsigned int* code_size,
				   char** internal_data, unsigned int* internal_data_size);

int            rle_decode         (rle_context_t* rle_context, char* code, unsigned int code_size,
				   char** data, unsigned int* data_size,
				   char* internal_data, unsigned int internal_data_size);

void           rle_destroy_context(rle_context_t* rle_context);


#endif /* __RLE_H__ */
