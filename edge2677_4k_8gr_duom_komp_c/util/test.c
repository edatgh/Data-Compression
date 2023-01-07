
#include <stdio.h>
#include "bword_io.h"
#include "rb_tree.h"
#include "my_io.h"


int main(int argc, char** argv)
{
	bword_io_context_t* bword_io_context;
	bword_t*            bword;


	bword_io_context = bword_io_create_context();
	if (bword_io_context == NULL)
	{
		printf("bword_io_context_create() failed!\n");

		return -1;
	}

	while ((bword = bword_io_read_word(bword_io_context, "76543210", 8, 64)) != NULL)
	{
		printf("0x%08x%08x\n", (uint32_t)(bword->word >> 32), (uint32_t)(bword->word & 0xffffffff));
	}

	bword_io_destroy_context(bword_io_context);

	return 0;
}
