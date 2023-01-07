
#include "compression.h"

void
data_check(data_t *in, size_t p)
{
	if (in->size < p)
	{
		if (in->data)
		{
			in->size = p + 256;
			in->data = (unsigned char *)realloc(in->data, in->size);
		}
		else
		{
			in->size = p + 256;
			in->data = (unsigned char *)malloc(in->size);
		}
	}
}


