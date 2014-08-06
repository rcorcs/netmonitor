#include "util.h"


__inline void zero_mem(byte_t *mem, uint_t size)
{
    register uint_t i;
    for(i = 0; i<size; i++) mem[i] = 0;
}
