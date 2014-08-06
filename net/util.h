
#ifndef _UTIL
#define _UTIL

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../datatypes/primitives.h"

#define HEX 1
#define DEC 2
#define OCT 3

//fatal error
void fatal(const char *msg);

//malloc with error checking
void *ec_malloc(unsigned int size);

void ffdump(FILE *stream, const int format, const byte_t *data_buffer, const unsigned int length);
__inline void fdump(FILE *stream, const byte_t *data_buffer, const unsigned int length);
__inline void dump(const byte_t *data_buffer, const unsigned int length);

byte_t *memfind(byte_t *data, uint_t dlen, byte_t *pattern, uint_t plen);

void fprint_vmask(FILE *stream);
__inline void print_vmask();

#ifdef __cplusplus
}//extern "C"
#endif

#endif

