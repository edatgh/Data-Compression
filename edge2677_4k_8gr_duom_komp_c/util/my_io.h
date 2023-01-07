
#ifndef __MY_IO_H__
#define __MY_IO_H__


int my_io_read_file (const char* file_name, char** buf, unsigned int* size);
int my_io_write_file(const char* file_name, char* buf, unsigned int size, int append);


#endif /* __MY_IO_H__ */
