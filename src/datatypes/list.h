#ifndef __LIST
#define __LIST

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

#include "primitives.h"
#include "node.h"

typedef struct __list {
    node_t *begin;
    node_t *end;
} list_t;

#define LIST_FOREACH(_NODE_, _LIST_) for(_NODE_ = _LIST_->begin; _NODE_!=0; _NODE_ = _NODE_->next)

list_t *create_list();
void destroy_list(list_t **list);
void destroy_list_nodes(list_t *list);

void list_push_back(list_t *list, node_t *node);
void list_push_front(list_t *list, node_t *node);

node_t *list_pop_back(list_t *list);
node_t *list_pop_front(list_t *list);

void list_remove(list_t *list, node_t *node);

void reverse_list(list_t *list);
uint_t list_size(list_t *list);

#ifdef __cplusplus
}
#endif

#endif
