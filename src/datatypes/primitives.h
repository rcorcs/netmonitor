#ifndef __PRIMITIVES
#define __PRIMITIVES

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>

#define TYPE_UNDEF     -1
#define TYPE_VOID       0
#define TYPE_CHAR       1
#define TYPE_INT        2
#define TYPE_FLOAT      3
#define TYPE_DOUBLE     4

typedef unsigned char          uchar_t;
typedef unsigned int           uint_t;
typedef unsigned short int     ushort_t;
typedef unsigned long int      ulong_t;
typedef unsigned long long     ullong_t;
typedef unsigned char          bool_t;
#define BOOL_TRUE              1
#define BOOL_FALSE             0

#if CHAR_BIT == 8
#define SIZEOF_CHAR 1
#else
#define SIZEOF_CHAR (CHAR_BIT/8)
#endif



#if USHRT_MAX == 65535
#define SIZEOF_SHORT 2
#elif SIZEOF_SHORT == 4294967295
#define SIZEOF_SHORT 4
#else
#error ERROR: SIZEOF_SHORT could not be defined!
#endif

#if UINT_MAX == 65535
#define SIZEOF_INT 2
#elif UINT_MAX == 4294967295
#define SIZEOF_INT 4
#else
#error ERROR: SIZEOF_INT could not be defined!
#endif

#if ULONG_MAX == 4294967295
#define SIZEOF_LONG 4
#elif SIZEOF_INT == 4
#define SIZEOF_LONG 8
#else
#error ERROR: SIZEOF_LONG could not be defined!
#endif

#if SIZEOF_CHAR == 1
typedef unsigned char          byte_t;
typedef byte_t                 uint8_t;
#else
#error ERROR: byte_t could not be defined!
#error ERROR: uint8_t could not be defined!
#endif


#if SIZEOF_SHORT == 2
typedef ushort_t               uint16_t;
#elif SIZEOF_INT == 2
typedef uint_t                 uint16_t;
#else
#error ERROR: uint16_t could not be defined!
#endif

#if SIZEOF_INT == 4
typedef uint_t                 uint32_t;
#elif SIZEOF_LONG == 4
typedef ulong_t                uint32_t;
#else
#error ERROR: uint32_t could not be defined!
#endif

#ifdef __cplusplus
extern "C" {
#endif

#endif
