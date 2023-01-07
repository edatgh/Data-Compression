#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN 3

#include "compression.h"

int dict_free = 0;

typedef struct _zod_t
{
	struct _zod_t *next;
	int id;
	int c;
	int len;
	char *zod;
} zod_t;

char b[1024*1024*3];
int Bp = 0;

static zod_t * r = NULL;
static int count = 0;
FILE *in, *out;

static int
pack_list(void)
{
	zod_t *t, **p;
	int n = 0;

	for (p = &r; *p; p = &(*p)->next)
	{
again:
		if ((*p)->c == 1)
		{
			// remove
			t = *p;
			count--;
			dict_free += t->len;
			*p = t->next;
			free(t->zod);
			free(t);
			goto again;
		}
		(*p)->id = n;
		n++;
	}

	fprintf(stderr, "freed %d\n", dict_free);

	return n;
}

static int
append(char *x, int n)
{
	zod_t *t, **p;
	int i;

	if (n < MIN)
	{
		return 0;
	}
	
	t = (zod_t *)malloc(sizeof(zod_t));
	t->zod = strndup(x, n);
	t->next = NULL;
	t->len = n;
	
	t->c = 1;

	for (p = &r; *p; p = &(*p)->next)
	{
		if (n != (*p)->len)
		{
			continue;
		}

		i = strncmp(x, (*p)->zod, n);
		if (i == 0)
		{
			free(t->zod);
			free(t);
			(*p)->c++;
			return 0;
		}
	}

	t->next = r;
	r = t;
	return n;
}

static int
search(char *x, int n)
{
	zod_t *t;
	int i;

	if (n < MIN)
	{
		return -1;
	}
	
	for (t = r; t; t = t->next)
	{
		if (n != t->len)
		{
			continue;
		}

		i = memcmp(x, t->zod, n);
		if (i == 0)
		{
			return t->id;
		}
	}

	return -1;
}

static data_t *
dict_comp(data_t *in)
{
	data_t *out;
	size_t i, out_p, count, n, k;
	unsigned short id;
	zod_t *t;

	/* search for words */
	count = 0;

	i = 0;
	while (i < in->size)
	{
		if (!isalpha(in->data[i]))
		{
			i++;
			continue;
		}

		for (n = 1; i + n < in->size; n++)
		{
			if (!isalpha(in->data[i + n]))
			{
				break;
			}
		}
		count += append(in->data + i, n);
		i += n;
	}

	out = (data_t *)malloc(sizeof(*out));
	out->data = NULL;
	out->size = 0;

	/* write dictionary */
	data_check(out, count + 4);
	count = pack_list();
	out->data[0] = (count >> 24) & 0xFF;
	out->data[1] = (count >> 16) & 0xFF;
	out->data[2] = (count >> 8) & 0xFF;
	out->data[3] = count & 0xFF;
	out_p = 4;
 
	for (t = r; t; t = t->next)
	{
		t->zod[t->len - 1] ^= 0x80;
		memcpy(out->data + out_p, t->zod, t->len);
		out_p += t->len;
		t->zod[t->len - 1] ^= 0x80;
	}

	/* write data */
	i = 0;
	data_check(out, out_p + in->size);
	while (i < in->size)
	{
		if (!isalpha(in->data[i]))
		{
			out->data[out_p] = in->data[i];
			out_p++;
			i++;
			continue;
		}

		for (n = 1; i + n < in->size; n++)
		{
			if (!isalpha(in->data[i + n]))
			{
				break;
			}
		}
		data_check(out, out_p + in->size - i + 2);
		k =  search(in->data + i, n);
		if (k != -1)
		{
			id = k;
			out->data[out_p] = ((id >> 8) & 0x7F) | 0x80;
			out->data[out_p + 1] = id & 0xFF;
			out_p += 2;
		}
		else
		{
			memcpy(out->data + out_p, in->data + i, n);
			out_p += n;
		}
		i += n;
	}

	out->size = out_p;
	return out;
}




static data_t *
dict_decomp(data_t *in)
{
	data_t *out;
	size_t i, out_p, count, n, k;
	char **zod;
	int *lens;
	unsigned char v[1 << 10], c;

	out = (data_t *)malloc(sizeof(*out));
	out->data = NULL;
	out->size = 0;

	out_p = 0;

	count = (in->data[0] << 24) | (in->data[1] << 16) | 
		(in->data[2] << 8) | in->data[3];

	zod = (char **)malloc(count * sizeof(*zod));
	lens = (int *)malloc(count * sizeof(*lens));

	i = 4;
	/* load dictionary */
	for (k = 0; k < count; k++)
	{
		n = 0;
		while (1)
		{
			if (i >= in->size)
			{
				printf("data corruption2\n");
				exit(0);
			}
			c = v[n] = in->data[i];
			if (v[n] & 0x80)
			{
				v[n] ^= 0x80;
				n ++;
				zod[k] = strndup(v, n);
				lens[k] = n;
				i ++;
				break;
			}
			n++;
			i++;
		}
	}

	data_check(out, in->size - i);
	while (i < in->size)
	{
		c = in->data[i];
		if (c & 0x80)
		{
			if (i + 1 >= in->size)
			{
				printf("data corruption0\n");
				exit(0);
			}
			//n = count - 1 - (((c & 0x7f) << 8) | in->data[i + 1]);
			n = (((c & 0x7f) << 8) | in->data[i + 1]);
			if (n < 0)
			{
				printf("data corruption1\n");
				exit(0);
			}
			data_check(out, out_p + in->size - i + lens[n]);
			memcpy(out->data + out_p, zod[n], lens[n]);
			out_p += lens[n];
			i += 2;
		}
		else
		{
			out->data[out_p] = in->data[i];
			out_p++;
			i++;
		}
	}

	out->size = out_p;
	return out;
}

engine_t dict_engine =
{
	dict_comp,
	dict_decomp
};
