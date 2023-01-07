
#ifndef __RLE0_H__
#define __RLE0_H__


typedef struct _rle0_context_t
{
	/* ? */
} rle0_context_t;


rle0_context_t* rle0_create_context (void);

int             rle0_encode         (rle0_context_t* rle0_context, char* data, unsigned int data_size,
				     char** code, unsigned int* code_size,
				     char** internal_data, unsigned int* internal_data_size);

int             rle0_decode         (rle0_context_t* rle0_context, char* code, unsigned int code_size,
				     char** data, unsigned int* data_size,
				     char* internal_data, unsigned int internal_data_size);

void            rle0_destroy_context(rle0_context_t* rle0_context);


#endif /* __RLE0_H__ */
