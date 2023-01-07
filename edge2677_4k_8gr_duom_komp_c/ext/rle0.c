#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "compression.h"

#define MIN 3

int rle0_o = 0;

static size_t
read_int(unsigned char *buf, size_t *ret, unsigned char *mode)
{
	*ret = 0;

	*mode = buf[0] & 0x80 ? 1 : 0;
	
	if ((buf[0] & 0x40) == 0)
	{
		*ret += buf[0] & 0x3F;
		return 1;
	}
	*ret += 0x40 - 1;

	if ((buf[0] & 0x60) == 0x40)
	{
		*ret += ((buf[0] & 0x1F) << 8) | buf[1];
		return 2;
	}
	*ret += 0x2000 - 1;

	if ((buf[0] & 0x70) == 0x60)
	{
		*ret += ((buf[0] & 0x0F) << 16) |
			(buf[1] << 8) | buf[2];
		return 3;
	}
	*ret += 0x100000 - 1;

	if ((buf[0] & 0x78) == 0x70)
	{
		*ret += ((buf[0] & 0x07) << 24) |
			(buf[1] << 16) | (buf[2] << 8) | buf[3];
		return 4;
	}
	*ret += 0x100000 - 1;

	printf("expand LRE\n");
	exit(0);
}

static data_t *
rle0_decomp(data_t *in)
{
	data_t *out;
	size_t i, out_p, count;
	unsigned char mode;

	out = (data_t *)malloc(sizeof(*out));
	out->data = NULL;
	out->size = 0;

	i = 0;
	out_p = 0;

	while (i < in->size)
	{
		i += read_int(in->data + i, &count, &mode);
		if (i >= in->size)
		{
			printf("data corruption1\n");
			exit(0);
		}

		data_check(out, out_p + count + MIN + 5);
		if (mode) // random
		{
			count ++;
			if (i + count > in->size)
			{
				printf("data corruption0\n");
				exit(0);
			}
			memcpy(out->data + out_p, in->data + i, count);
			i += count;
		}
		else // no random
		{
			count += MIN - 1;
			memset(out->data + out_p, 0, count);
//			memset(out->data + out_p, in->data[i], count);
//			i++;
		}
		out_p += count;
	}

	out->size = out_p;
	return out;
}

static size_t
write_int(char *buf, size_t val, int mode)
{
	unsigned char msb;


	msb = mode ? 0x80 : 0;
	
	if (val <= 0x40 - 1)
	{
		buf[0] = msb | (val & 0x3F);
		rle0_o ++;
		return 1;
	}
	val -= 0x40 - 1;

	if (val <= 0x2000 - 1)
	{
		buf[0] = msb | 0x40 | 
			((val >> 8) & 0x1F);

		buf[1] = (val & 0xFF);
		rle0_o += 2;
		return 2;
	}
	val -= 0x2000 - 1;

	if (val <= 0x100000 - 1)
	{
		buf[0] = msb | 0x40 | 0x20 | 
			((val >> 16) & 0x0F);

		buf[1] = ((val >> 8) & 0xFF);

		buf[2] = (val & 0xFF);
		rle0_o += 3;
		return 3;
	}
	val -= 0x100000 - 1;

	if (val <= 0x8000000 - 1)
	{
		buf[0] = msb | 0x40 | 0x20 | 0x10 |
			((val >> 24) & 0x07);

		buf[1] = ((val >> 16) & 0xFF);
		buf[2] = ((val >> 8) & 0xFF);

		buf[3] = (val & 0xFF);
		rle0_o += 4;
		return 4;
	}
	val -= 0x8000000 - 1;


	printf("expand LRE\n");
	exit(0);
}

static data_t *
rle0_comp(data_t *in)
{
	size_t i, buf_p, buf_pl, out_p, count, from, n;
	data_t *out;
	unsigned char mode, stop;
	unsigned char last;
	unsigned char buf[MIN];

	rle0_o = 0;
	out = (data_t *)malloc(sizeof(*out));
	out->data = NULL;
	out->size = 0;

	out_p = 0;
	i = 0;

	while (i < in->size)
	{
		if (in->data[i] != 0)
		{
			mode = 1;
		}
		else
		{
			mode = 0;
			for (n = 1; n < MIN; n++)
			{
				if (i + n >= in->size || in->data[i] != in->data[i + n])
				{
					mode = 1;
					break;
				}
			}
		}
	
		if (mode) // random data
		{
			buf[0] = i + 1 < in->size ? in->data[i + 1] : 0;
			for (n = 1; n < MIN; n++)
			{
				buf[n] = i + n + 1 < in->size ?
					in->data[i + n + 1] : buf[n - 1] + 1;
			}

			count = 1;
			from = i;
			buf_pl = 2;
			buf_p = 0;
			for (i = i + 1; i < in->size; i++)
			{
				if (buf[0] == 0)
				{
					stop = 1;
					for (n = 1; n < MIN; n++)
					{
						if (buf[n] != 0)
						{
							stop = 0;
							break;
						}
					}
					if (stop)
					{
						break;
					}
				}
				count ++;
				
				buf[buf_p] = i + MIN < in->size ? in->data[i + MIN] : ~buf[buf_pl];
				buf_pl = buf_p;
				buf_p = (buf_p + 1) % MIN;
			}

			data_check(out, out_p + count + 10);
			out_p += write_int(out->data + out_p, count - 1, mode);
			memcpy(out->data + out_p, in->data + from, count);
			out_p += count;
		}
		else // non random
		{
			last = in->data[i];
			count = 1;
			for (i = i + 1; i < in->size; i++)
			{
				if (last != in->data[i])
				{
					break;
				}
				count ++;
			}
			data_check(out, out_p + 10);
			out_p += write_int(out->data + out_p, count - MIN + 1, mode);
//			out->data[out_p] = last;
//			out_p ++;
		}
	}

	out->size = out_p;
	return out;
}

engine_t rle0_engine =
{
	rle0_comp,
	rle0_decomp
};
