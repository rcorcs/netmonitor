#include "node.h"

node_t *create_node(size_t size, node_t *prev, node_t *next)
{
    node_t *node = (node_t*)malloc(sizeof(node_t));
    //node->dtype = dtype;
    node->size = size;
    node->data = malloc(size);
    node->prev = prev;
    node->next = next;
    return node;
}

void destroy_node(node_t **node)
{
    if(node && *node){
        free((*node)->data);
        free(*node);
        *node = 0;
    }
}

void swap_nodes(node_t *node1, node_t *node2)
{
    void *data;
//    int dtype;
    size_t size;

//    dtype = node1->dtype;
//    node1->dtype = node2->dtype;
//    node2->dtype = dtype;

    size = node1->size;
    node1->size = node2->size;
    node2->size = size;

    data = node1->data;
    node1->data = node2->data;
    node2->data = data;

    /*
    temp = node1->prev;
    node1->prev = node2->prev;
    node2->prev = temp;

    temp = node1->next;
    node1->next = node2->next;
    node2->next = temp;
    */
}
