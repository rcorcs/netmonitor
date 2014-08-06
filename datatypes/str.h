#ifndef __STR
#define __STR

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#include "primitives.h"

typedef struct __str {
    char *str;
    uint_t len;
} str_t;

//str_t *alloc_str(uint_t len);
str_t *create_str(const char *c_str);
void   destroy_str(str_t **str);

#ifdef __cplusplus
}
#endif

#endif
