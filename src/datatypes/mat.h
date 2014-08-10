#ifndef __MAT
#define __MAT

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "primitives.h"

typedef struct __mat {
//    int dtype;
    size_t size;
    uint_t rows;
    uint_t cols;
    void *data;
} mat_t;

#define MATGET( _TYPE_, _MAT_, _ROW_, _COL_ ) (((_TYPE_*)(_MAT_)->data)[ (_ROW_)*(_MAT_)->cols + (_COL_) ])

//mat_t *create_mat(int dtype, size_t size, uint_t rows, uint_t cols);
mat_t *create_mat(size_t size, uint_t rows, uint_t cols);
void destroy_mat(mat_t **mat);

void zero_mat(mat_t *mat);


#ifdef __cplusplus
}
#endif

#endif
