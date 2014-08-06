#ifndef __VEC
#define __VEC

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "primitives.h"

typedef struct __vec {
    //int dtype;
    size_t size;
    uint_t len;
    byte_t *data;
} vec_t;

#define VECGET( _TYPE_, _VECTOR_, _INDEX_ )   (((_TYPE_*)(_VECTOR_)->data)[_INDEX_])

vec_t *create_vec(size_t size, uint_t length);
void destroy_vec(vec_t **vec);

//void zero_vec(vec_t *vec);


#ifdef __cplusplus
}
#endif

#endif
