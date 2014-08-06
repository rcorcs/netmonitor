#ifndef __UDP_STATS
#define __UDP_STATS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../../util.h"
#include "../../../datatypes/vec.h"
#include "../../../datatypes/mat.h"
#include "../../../datatypes/list.h"
#include "../../../datatypes/node.h"

typedef struct __udp_record {
   uint_t total;    //total of UDP packets analysed
} udp_record_t;

void fprint_udp_record(FILE *out, udp_record_t *udprecord);
__inline void print_udp_record(udp_record_t *udprecord);

void store_udp_record(FILE *out, udp_record_t *udprecord);
void load_udp_record(FILE *out, udp_record_t *udprecord);

vec_t *create_udp_vec(udp_record_t *udprecord);
mat_t *create_udp_mat(list_t *list);

typedef struct __udpp {
   udp_record_t in;
   udp_record_t out;
} udpp_t;

udpp_t *create_udpp();
void destroy_udpp(udpp_t **udpp);


void fprint_udpp(FILE *out, udpp_t *udpp);
__inline void print_udpp(udpp_t *udpp);

void store_udpp(FILE *out, udpp_t *udpp);
void load_udpp(FILE *in, udpp_t *udpp);

#ifdef __cplusplus
}
#endif

#endif

