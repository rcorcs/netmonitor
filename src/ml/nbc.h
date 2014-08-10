
#ifndef __NBC
#define __NBC

#ifdef __cplusplus
extern "C" {
#endif


#include "../datatypes/mat.h"
#include "../datatypes/vec.h"
#include "../datatypes/primitives.h"

typedef struct __nbc {
   int nparam; //number of parameters
   int nc; //number of classes
   mat_t *training; //training data
} nbc_t;

nbc_t *create_nbc(int nc, int nparam);
void destroy_nbc(nbc_t **nbc);

void nbc_train(nbc_t *nbc, int class, mat_t *data);
int nbc_classify(nbc_t *nbc, vec_t *sample);

#ifdef __cplusplus
}
#endif

#endif
