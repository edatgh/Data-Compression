/* $Id: rb_tree.c 3969 2005-05-19 19:41:42Z edvardas $ */

/** @file common/rb_tree.c Implementation of multi-purpose data storage (balanced Red-Black Trees) */

#include <stdlib.h>
#include "rb_tree.h"

/**
 * Structure describing a leaf
 */
static rbt_node_t leaf =
{
	BLACK,
	0,
	NULL,
	NULL,
	&leaf,
	&leaf
};

#define LEAF &leaf	/**< All tree nodes with children absent are called leafs and have the same state */
 

/**
 * @attention This function is not a part of RB-Tree interface, it is only for
 *            internal use.
 *
 * Inserts node into the @c tree
 */
static rbt_node_t *
bst_insert(rbt_node_t *tree, rbt_node_t **r, uint64_t key, void *data)
{
	rbt_node_t *z;
	rbt_node_t *x;
	rbt_node_t *y;


	z = (rbt_node_t *)malloc(sizeof(*z));
	if (z == NULL)
	{
		return tree;
	}

	z->c     = RED;
	z->key   = key;
	z->data  = data;
	z->p     = NULL;
	z->left  = LEAF;
	z->right = LEAF;

	y = NULL; /*   parent   */
	x = tree;

	while (x != LEAF)
	{
		y = x;
		if (z->key < x->key)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}

	z->p = y;

	if (y == NULL)
	{
		tree = z;
	}
	else
	{
		if (z->key < y->key)
		{
			y->left = z;
		}
		else
		{
			y->right = z;
		}
	}

	*r = z;

	return tree;
}


/**
 * @attention This function is not a part of RB-Tree interface, it is only for
 *            internal use.
 *
 * Does left rotation, relatively to @c x
 */
static rbt_node_t *
rbt_rotate_left(rbt_node_t *tree, rbt_node_t *x)
{
	rbt_node_t *y;


	y        = x->right;
	x->right = y->left;

	if (y->left != LEAF)
	{
		y->left->p = x;
	}

	y->p = x->p;

	if (x->p == NULL)
	{
		tree = y;
	}
	else
	{
		if (x == x->p->left)
		{
			x->p->left = y;
		}
		else
		{
			x->p->right = y;
		}
	}
	y->left = x;
	x->p    = y;

	return tree;
}


/**
 * @attention This function is not a part of RB-Tree interface, it is only for
 *            internal use.
 *
 * Does right rotation, relatively to @c x
 */ 
static rbt_node_t *
rbt_rotate_right(rbt_node_t *tree, rbt_node_t *x)
{
	rbt_node_t *y;


	y       = x->left;
	x->left = y->right;

	if (y->right != LEAF)
	{
		y->right->p = x;
	}

	y->p = x->p;

	if (x->p == NULL)
	{
		tree = y;
	}
	else
	{
		if (x == x->p->right)
		{
			x->p->right = y;
		}
		else
		{
			x->p->left = y;
		}
	}
	y->right = x;
	x->p     = y;

	return tree;
}


/**
 * Initializes RB tree. This function must be called before any other function
 * on RB tree.
 * @return Returns descriptor of RB tree, which must be passed to all RB tree relative
 *         functions
 */ 
rbt_node_t *
rbt_init(void)
{
	return LEAF;
}


/**
 * Destroys RB tree
 * @attention This function does not free data stored in the tree
 * @param[in] tree RB tree descriptor
 */ 
void
rbt_destroy(rbt_node_t *tree)
{
	if (tree == NULL || tree == LEAF)
	{
		return;
	}

	rbt_destroy(tree->left);
	rbt_destroy(tree->right);
	free(tree);
}


/**
 * Inserts new node to the RB tree
 * @param[in] tree RB tree descriptor
 * @param[in] key Key of the node
 * @param[in] data Data to be stored to the tree under the key
 * @return Returns descriptor of RB tree with the node inserted on success, else NULL
 */ 
rbt_node_t *
rbt_insert(rbt_node_t *tree, uint64_t key, void *data)
{
	rbt_node_t *x;
	rbt_node_t *y;


	if (tree == NULL)
	{
		return NULL;
	}

	tree = bst_insert(tree, &x, key, data);
	if (tree == NULL)
	{
		return NULL;
	}

	while (x != tree && x->p->c == RED)
	{
		if (x->p == x->p->p->left)
		{
			y = x->p->p->right;
			if (y->c == RED)
			{
				x->p->c    = BLACK;
				y->c       = BLACK;
				x->p->p->c = RED;
				x          = x->p->p;
			}
			else
			{
				if (x == x->p->right)
				{
					x    = x->p;
					tree = rbt_rotate_left(tree, x);
				}
				x->p->c    = BLACK;
				x->p->p->c = RED;
				tree       = rbt_rotate_right(tree, x->p->p);
			}
		}
		else
		{
			y = x->p->p->left;
			if (y->c == RED)
			{
				x->p->c    = BLACK;
				y->c       = BLACK;
				x->p->p->c = RED;
				x          = x->p->p;
			}
			else
			{
				if (x == x->p->left)
				{
					x    = x->p;
					tree = rbt_rotate_right(tree, x);
				}
				x->p->c    = BLACK;
				x->p->p->c = RED;
				tree       = rbt_rotate_left(tree, x->p->p);
			}
		}
	}

	tree->c = BLACK;

	return tree;
}


/**
 * Removes a node from RB tree
 * @param[in] tree RB tree descriptor
 * @param[in] node RB tree node to be removed
 * @return Returns RB tree descriptor with the node removed (if any)
 */
rbt_node_t *
rbt_delete(rbt_node_t *tree, rbt_node_t *node)
{
	rbt_node_t *x;
	rbt_node_t *y;
	rbt_node_t *z;


	if (node->left == LEAF || node->right == LEAF)
	{
		y = node;
	}
	else
	{
		y = node->right;
		while (y->left != LEAF)
		{
			y = y->left;
		}
	}

	if (y->left != LEAF)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}

	x->p = y->p;
	if (y->p)
	{
		if (y == y->p->left)
		{
			y->p->left  = x;
		}
		else
		{
			y->p->right = x;
		}
	}
	else
	{
		tree = x;
	}

	if (y != node)
	{
		node->key  = y->key;
		node->data = y->data;
	}

	if (y->c == BLACK)
	{
		while (x != tree && x->c == BLACK)
		{
			if (x == x->p->left)
			{
				z = x->p->right;
				if (z->c == RED)
				{
					z->c    = BLACK;
					x->p->c = RED;
					tree    = rbt_rotate_left(tree, x->p);
					z       = x->p->right;
				}
				if (z->left->c == BLACK && z->right->c == BLACK)
				{
					z->c = RED;
					x    = x->p;
				}
				else
				{
					if (z->right->c == BLACK)
					{
						z->left->c = BLACK;
						z->c       = RED;
						tree       = rbt_rotate_right(tree, z);
						z          = x->p->right;
					}
					z->c        = x->p->c;
					x->p->c     = BLACK;
					z->right->c = BLACK;
					tree        = rbt_rotate_left(tree, x->p);
					x           = tree;
				}
			}
			else
			{
				z = x->p->left;
				if (z->c == RED)
				{
					z->c    = BLACK;
					x->p->c = RED;
					tree    = rbt_rotate_right(tree, x->p);
					z       = x->p->left;
				}
				if (z->right->c == BLACK && z->left->c == BLACK)
				{
					z->c = RED;
					x    = x->p;
				}
				else
				{
					if (z->left->c == BLACK)
					{
						z->right->c = BLACK;
						z->c        = RED;
						tree        = rbt_rotate_left(tree, z);
						z           = x->p->left;
					}
					z->c       = x->p->c;
					x->p->c    = BLACK;
					z->left->c = BLACK;
					tree       = rbt_rotate_right(tree, x->p);
					x          = tree;
				}
			}
		}
		x->c = BLACK;
	}

	free(y);

	return tree;
}


/**
 * Searches for a node in the RB tree
 * @param[in] tree RB tree descriptor
 * @param[in] key Key of the node
 * @return Returns node if found, else NULL
 */ 
void *
rbt_find(rbt_node_t *tree, uint64_t key)
{
	rbt_node_t *node;


	if (tree == NULL)
	{
		return NULL;
	}

	node = tree;
	while (node != LEAF)
	{
		if (node->key == key)
		{
			return node;
		}

		if (key < node->key)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	return NULL;
}


/**
 * Gets data from a node of RB tree
 * @param[in] node Node containing the data
 * @return Returns data of the node on success, else NULL
 */
void *
rbt_get(rbt_node_t *node)
{
	if (node == NULL)
	{
		return NULL;
	}

	return node->data;
}


int
rbt_set(rbt_node_t* node, void* data)
{
	node->data = data;

	return 0;
}


/**
 * Recursively traverses RB tree, calling callback function with
 * each node's data as an argument
 * @param[in] tree RB tree descriptor
 * @param[in] callback Callback function @see traverse_callback_t
 * @param[in] cb_arg User defined argument for the callback function
 */ 
void
rbt_traverse(rbt_node_t *tree, traverse_callback_t callback, void *cb_arg)
{
	if (tree == NULL || tree == LEAF)
	{
		return;
	}

	rbt_traverse(tree->right, callback, cb_arg);
	callback(tree->data, cb_arg);
	rbt_traverse(tree->left, callback, cb_arg);
}
