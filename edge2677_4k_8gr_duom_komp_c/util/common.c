
#include <string.h>
#include <math.h>
#include "common.h"


double calculate_entropy(char* data, unsigned int data_size)
{
	unsigned int freq[256];
	int          i;
	double       i_e; /* information entropy */
	double       p;   /* probability         */


	memset(freq, 0, sizeof(freq));

	for (i = 0; i < data_size; i++)
	{
		freq[(unsigned char)data[i]]++;
	}

	i_e = 0.0;

	for (i = 0; i < 256; i++)
	{
		if (freq[i] != 0)
		{
			p = (double)freq[i] / (double)data_size;
			i_e += p * (log(p) / log(2));
		}
	}

	i_e *= -1;

	return i_e;
}
