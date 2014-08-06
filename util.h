#ifndef __UTIL
#define __UTIL

#include "datatypes/primitives.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

__inline void zero_mem(byte_t *mem, uint_t size);

#endif
