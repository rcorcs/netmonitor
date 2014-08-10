#include "stack.h"

//#define cstack_create() clist_create()
stack_t *create_stack()
{
    stack_t *stack = (stack_t*)malloc(sizeof(stack_t));
    stack->top = 0;
    return stack;
}

//#define cstack_destroy(_STACK_) clist_destroy(_STACK_)
void destroy_stack(stack_t **stack)
{
    if(stack){
        free(*stack);
        *stack = 0;
    }
}

//#define cstack_push(_STACK_, _NODE_) clist_push_back(_STACK_, _NODE_)
void stack_push(stack_t *stack, node_t *node)
{
    if(!stack || !node) return;

    node_t  *last = stack->top;
    if(last){
        last->next = node;
        node->prev = last;
        node->next = 0;
        stack->top = node;
    }else {
        stack->top = node;
        node->prev = node->next = 0;
    }
}

//#define cstack_pop(_STACK_)  clist_pop_back(_STACK_)
node_t *stack_pop(stack_t *stack)
{
    if(!stack || !stack->top) return 0;

    node_t *node = stack->top;
    stack->top = node->prev;
    node->prev = node->next = 0;
    if(stack->top) stack->top->next = 0;

    return node;
}

void destroy_stack_nodes(stack_t *stack)
{
    if(stack && stack->top){
        node_t *temp;
        node_t *node = stack->top;
        while(node!=0){
            temp = node->prev;
            destroy_node(&node);
            node = temp;
        }
        stack->top = 0;
    }
}
