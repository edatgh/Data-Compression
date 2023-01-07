
#ifndef __BWORD_H__
#define __BWORD_H__


#include <stdint.h>


typedef struct _bword_t
{
	uint32_t bit_count;
	uint64_t word;
} bword_t;


#endif /* __BWORD_H__ */
