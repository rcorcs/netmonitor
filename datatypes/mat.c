#include "mat.h"

mat_t *create_mat(size_t size, uint_t rows, uint_t cols)
{
    mat_t *mat = (mat_t*)malloc(sizeof(mat_t));
//    mat->dtype = dtype;
    mat->size = size;
    mat->rows = rows;
    mat->cols = cols;
    mat->data = malloc(size*rows*cols);
    return mat;
}

void destroy_mat(mat_t **mat)
{
    free((*mat)->data);
    free(*mat);
    *mat = 0;
}

void zero_mat(mat_t *mat)
{
    int i, limit;
    char *data = (char*)mat->data;
    limit = mat->size*mat->rows*mat->cols;
    for(i=0;i<limit; i++)
        data[i] = 0;
}
