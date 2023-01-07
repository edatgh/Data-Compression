
#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__


typedef struct _huffman_node_t
{
	int  weight;
	int  used;
	char c;
	int  left;
	int  right;
} huffman_node_t;

typedef struct _huffman_code_t
{
	int   max_length;
	int   length;
	char* code;
} huffman_code_t;

typedef struct _huffman_context_t
{
	unsigned int    freq[256];  /* character frequencies, char code is the index of the frequency          */
	unsigned int    freq_count; /* count of different characters                                           */
	huffman_code_t  codes[256];  /* array of character huffman codes, char code is the index of the code   */
	huffman_node_t* tree;
	int             node_count;
	int             nodes_left;
} huffman_context_t;



huffman_context_t* huffman_create_context (void);

int                huffman_encode         (huffman_context_t* huffman_context, char* data, unsigned int data_size,
					   char** code, unsigned int* code_size,
					   char** internal_data, unsigned int* internal_data_size);

int                huffman_decode         (huffman_context_t* huffman_context, char* code, unsigned int code_size,
					   char** data, unsigned int* data_size,
					   char* internal_data, unsigned int internal_data_size);

void               huffman_destroy_context(huffman_context_t* huffman_context);


#endif /* __HUFFMAN_H__ */
