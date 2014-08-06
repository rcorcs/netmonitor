#include "vec.h"

vec_t *create_vec(size_t size, uint_t length)
{
    vec_t *vec = (vec_t*)malloc(sizeof(vec_t));
    //vec->dtype = dtype;
    vec->size = size;
    vec->len = length;
    vec->data = malloc(size*length);
    return vec;
}

void destroy_vec(vec_t **vec)
{
    free((*vec)->data);
    free(*vec);
    *vec = 0;
}
