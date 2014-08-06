#ifndef __STACK
#define __STACK

#ifdef __cplusplus
extern "C" {
#endif

#include "primitives.h"

#include "node.h"
//#include "clist.h"

//typedef clist cstack;

typedef struct __stack {
    node_t *top;
} stack_t;

//#define cstack_create() clist_create()
stack_t *create_stack();

//#define cstack_destroy(_STACK_) clist_destroy(_STACK_)
void destroy_stack(stack_t **stack);

//#define cstack_push(_STACK_, _NODE_) clist_push_back(_STACK_, _NODE_)
void stack_push(stack_t *stack, node_t *node);

//#define cstack_pop(_STACK_)  clist_pop_back(_STACK_)
node_t *stack_pop(stack_t *stack);

void destroy_stack_nodes(stack_t *stack);

#ifdef __cplusplus
}
#endif

#endif
