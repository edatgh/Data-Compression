/* $Id: rb_tree.h 3998 2005-05-20 17:21:15Z edvardas $ */

#ifndef __RB_TREE_H__
#define __RB_TREE_H__


#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif

typedef enum { RED, BLACK } color_t;

typedef struct _rbt_node_t {
	color_t             c;
	uint64_t            key;
	void*               data;
	struct _rbt_node_t* p;
	struct _rbt_node_t* left;
	struct _rbt_node_t* right;
} rbt_node_t;

typedef void (*traverse_callback_t)(void *data, void *arg_cb);


rbt_node_t* rbt_init    (void);
void        rbt_destroy (rbt_node_t *tree);
rbt_node_t* rbt_insert  (rbt_node_t *tree, uint64_t key, void *data);
rbt_node_t* rbt_delete  (rbt_node_t *tree, rbt_node_t *node);
void*       rbt_find    (rbt_node_t *tree, uint64_t key);
void*       rbt_get     (rbt_node_t *node);
uint64_t    rbt_get_key (rbt_node_t *node);
int         rbt_set     (rbt_node_t *node, void *data);
void        rbt_traverse(rbt_node_t *tree, traverse_callback_t callback, void *arg_cb);


#ifdef __cplusplus
}
#endif

#endif /* __RB_TREE_H__ */
