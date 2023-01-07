
#ifndef __DATA_MODEL_H__
#define __DATA_MODEL_H__


/*
 * Constants
 */
#define BWT_FORWARD_TRANSFORMATION     0
#define MTF_FORWARD_TRANSFORMATION     1
#define RLE_FORWARD_TRANSFORMATION     2
#define HUFFMAN_FORWARD_TRANSFORMATION 3
#define BWT_INVERSE_TRANSFORMATION     4
#define MTF_INVERSE_TRANSFORMATION     5
#define RLE_INVERSE_TRANSFORMATION     6
#define HUFFMAN_INVERSE_TRANSFORMATION 7

#define RLE2_FORWARD_TRANSFORMATION    8
#define RLE2_INVERSE_TRANSFORMATION    9

#define DICT_FORWARD_TRANSFORMATION    10
#define DICT_INVERSE_TRANSFORMATION    11

#define RLE0_FORWARD_TRANSFORMATION    12
#define RLE0_INVERSE_TRANSFORMATION    13

#define FORWARD_TRANSFORMATION 0
#define INVERSE_TRANSFORMATION 1


/*
 * Types
 */
typedef int (*ENCODE_METHOD_TYPE)(void*, char*, unsigned int, char**, unsigned int*, char**, unsigned int*);
typedef int (*DECODE_METHOD_TYPE)(void*, char*, unsigned int, char**, unsigned int*, char*, unsigned int);

typedef struct _forward_transformation_t
{
	/*
	 * Attributes
	 */
	void*        encode_context; /* encoding produces code */
	char*        code;
	unsigned int code_size;

	char*        internal_data;
	unsigned int internal_data_size;

	/*
	 * Methods
	 */
	ENCODE_METHOD_TYPE p_encode;
} forward_transformation_t;

typedef struct _inverse_transformation_t
{
	/*
	 * Attributes
	 */
	void*        decode_context;
	char*        data;
	unsigned int data_size;

	char*        internal_data;
	unsigned int internal_data_size;

	/*
	 * Methods
	 */
	DECODE_METHOD_TYPE p_decode;
} inverse_transformation_t;

typedef struct _transformation_t
{
	char* name;

	int   type; /* { FORWARD_TRANSFORMATION, INVERSE_TRANSFORMATION } */
	void* x;
} transformation_t;

typedef struct _data_model_t
{
	transformation_t* transformations;
	int*              transformation_names;
	int               transformation_count;
} data_model_t;


/*
 * Function prototypes
 */
data_model_t* data_model_create                (int* transformation_names, int transformation_count);

int           data_model_execute_transformation(data_model_t* data_model, char* input, unsigned int input_size,
						char** output, unsigned int* output_size);

void          data_model_destroy               (data_model_t* data_model);


#endif /* __DATA_MODEL_H__ */
