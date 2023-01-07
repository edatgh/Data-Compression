
#ifndef __BWORD_IO_H__
#define __BWORD_IO_H__


#include "bword.h"


typedef struct _bword_io_context_t
{
	uint32_t bits_read;
} bword_io_context_t;


bword_io_context_t* bword_io_create_context (void);
bword_t*            bword_io_read_word      (bword_io_context_t* bword_io_context, uint8_t* data, uint32_t data_size, uint32_t word_size);
void                bword_io_destroy_context(bword_io_context_t* bword_io_context);


#endif /* __BWORD_IO_H__ */
