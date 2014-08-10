#ifndef __NODE
#define __NODE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "primitives.h"

typedef struct __node{
    //int dtype;
    size_t size;
    byte_t *data;

    struct __node *prev;
    struct __node *next;
} node_t;

#define NODEGET( _TYPE_, _NODE_ ) (*((_TYPE_*)(_NODE_)->data))

node_t *create_node(size_t size, node_t *prev, node_t *next);
void destroy_node(node_t **node);
void swap_nodes(node_t *node1, node_t *node2);

//cnode *cnode_create(int dtype, size_t size, cnode *prev, cnode *next);
//void cnode_destroy(cnode **node);
//void cnode_swap(cnode *node1, cnode *node2);


#ifdef __cplusplus
}
#endif

#endif
