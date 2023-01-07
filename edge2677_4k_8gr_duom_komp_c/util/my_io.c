
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "my_io.h"


static char* add_data(char* buf, unsigned int* buf_size, unsigned int* max_buf_size,
		      char* data, unsigned int data_size)
{
	if (*buf_size == *max_buf_size)
	{
		*max_buf_size += (1 << 16); /* 64k */
		buf = realloc(buf, *max_buf_size);
	}

	memcpy(buf + *buf_size, data, data_size);

	*buf_size += data_size;

	return buf;
}


int my_io_read_file(const char* file_name, char** buf, unsigned int* size)
{
	int          fd;
	char         data[65536];
	int          br;
	unsigned int buf_size;
	unsigned int max_buf_size;


	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		return -1;
	}

	*buf         = NULL;
	buf_size     = 0;
	max_buf_size = 0;

	while ((br = read(fd, data, sizeof(buf))) > 0)
	{
		*buf = add_data(*buf, &buf_size, &max_buf_size, data, br);
	}

	close(fd);

	*size = buf_size;

	return 0;
}


int my_io_write_file(const char* file_name, char* buf, unsigned int size, int append)
{
	int fd;
	int flag;
	int bw;


	if (append)
	{
		flag = O_APPEND;
	}
	else
	{
		flag = O_TRUNC;
	}

	fd = open(file_name, O_WRONLY | O_CREAT | flag, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		return -1;
	}

	bw = write(fd, buf, size);

	while (bw < size && size > 0 && bw >= 0)
	{
		size -= bw;
		buf  += bw;

		bw = write(fd, buf, size);
	}

	if (bw == -1)
	{
		return -1;
	}

	return 0;
}
