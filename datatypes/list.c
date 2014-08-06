#include "list.h"

list_t *create_list()
{
    list_t *list = (list_t*)malloc(sizeof(list_t));
    list->begin = list->end = 0;
    return list;
}

void destroy_list(list_t **list)
{
    if(list){
        free(*list);
        *list = 0;
    }
}

void destroy_list_nodes(list_t *list)
{
    if(list && list->begin && list->end){
        node_t *temp;
        node_t *node = list->begin;
        while(node!=0){
            temp = node->next;
            destroy_node(&node);
            node = temp;
        }
        list->begin = list->end = 0;
    }
}

void list_push_back(list_t *list, node_t *node)
{
    if(!list || !node) return;

    node_t *last = list->end;
    if(last){
        last->next = node;
        node->prev = last;
        node->next = 0;
        list->end = node;
    }else {
        list->begin = list->end = node;
        node->prev = node->next = 0;
    }
}

void list_push_front(list_t *list, node_t *node)
{
    if(!list || !node) return;

    node_t *first = list->begin;
    if(first){
        first->prev = node;
        node->prev = 0;
        node->next = first;
        list->begin = node;
    }else{
        list->begin = list->end = node;
        node->prev = node->next = 0;
    }
}

node_t *list_pop_back(list_t *list)
{
    if(!list || !list->end) return 0;

    node_t *node = list->end;
    list->end = node->prev;
    if(list->end) list->end->next = 0;
    else list->begin = 0;

    node->prev = node->next = 0;
    return node;
}

node_t *list_pop_front(list_t *list)
{
    if(!list || !list->begin) return 0;

    node_t *node = list->begin;
    list->begin = node->next;
    if(list->begin) list->begin->prev = 0;
    else list->end = 0;

    node->prev = node->next = 0;
    return node;
}

void list_remove(list_t *list, node_t *node)
{
    node_t *pnode = node->prev;
    node_t *nnode = node->next;

    if(nnode){
       nnode->prev = node->prev;
    }else{
       list->end = node->prev;
    }

    if(pnode){
       pnode->next = node->next;
    }else{
       list->begin = node->next;
    }
}

void reverse_list(list_t *list)
{
    if(!list || !list->begin) return;

    node_t *next;
    node_t *node;
    next = node = list->begin;
    while(node!=0){
        next = node->next;
        node->next = node->prev;
        node->prev = next;
        node = next;
    }

    node = list->begin;
    list->begin = list->end;
    list->end = node;
}

uint_t list_size(list_t *list)
{
   uint_t size = 0;
   node_t *node;

   if(!list || !list->begin) return 0;

   LIST_FOREACH(node, list){
      ++size;
   }

   return size;
}
