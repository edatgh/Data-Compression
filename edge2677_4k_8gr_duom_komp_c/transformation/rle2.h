
#ifndef __RLE2_H__
#define __RLE2_H__


typedef struct _rle2_context_t
{
	/* ? */
} rle2_context_t;


rle2_context_t* rle2_create_context (void);

int             rle2_encode         (rle2_context_t* rle2_context, char* data, unsigned int data_size,
				     char** code, unsigned int* code_size,
				     char** internal_data, unsigned int* internal_data_size);

int             rle2_decode         (rle2_context_t* rle2_context, char* code, unsigned int code_size,
				     char** data, unsigned int* data_size,
				     char* internal_data, unsigned int internal_data_size);

void            rle2_destroy_context(rle2_context_t* rle2_context);


#endif /* __RLE2_H__ */
