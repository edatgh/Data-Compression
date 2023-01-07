#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__

#include <stdlib.h>

typedef struct _data_t
{
	size_t size;
	unsigned char *data;
} data_t;

typedef struct _engine_t
{
	data_t *(*enc)(data_t *in);
	data_t *(*dec)(data_t *in);
} engine_t;

void data_check(data_t *in, size_t p);

#endif
