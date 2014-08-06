
#ifndef __NBCIO
#define __NBCIO

#ifdef __cplusplus
extern "C" {
#endif

#include "nbc.h"

#include <stdio.h>

void store_nbc(FILE *out, nbc_t *nbc);
void load_nbc(FILE *in, nbc_t *nbc);

#ifdef __cplusplus
}
#endif

#endif
