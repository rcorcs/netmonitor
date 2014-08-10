#ifndef __TCP_STATS
#define __TCP_STATS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

#include "../../../util.h"
#include "../../../datatypes/vec.h"
#include "../../../datatypes/mat.h"
#include "../../../datatypes/list.h"
#include "../../../datatypes/node.h"

/// Parameter for TCP classification
typedef struct __tcp_record {
   /// total of tcp packets
   uint_t total;    //total of TCP packets analysed

   /// TCP FLAG bits
   uint_t tack;    //ack bit (flag)
   uint_t tpsh;    //push bit (flag)
   uint_t trst;    //rst bit (flag)
   uint_t tsyn;    //syn bit (flag)
   uint_t tfin;    //fin bit (flag)

   /// Total of Data length
   uint_t tdatalen;

} tcp_record_t;

void fprint_tcp_record(FILE *out, tcp_record_t *tcprecord);
__inline void print_tcp_record(tcp_record_t *tcprecord);

void store_tcp_record(FILE *out, tcp_record_t *tcprecord);
void load_tcp_record(FILE *out, tcp_record_t *tcprecord);

vec_t *create_tcpsyn_vec(tcp_record_t *tcprecord);
mat_t *create_tcpsyn_mat(list_t *list);

typedef struct __tcpp {
   tcp_record_t in;
   tcp_record_t out;
} tcpp_t;

tcpp_t *create_tcpp();
void destroy_tcpp(tcpp_t **tcpp);

void fprint_tcpp(FILE *out, tcpp_t *tcpp);
__inline void print_tcpp(tcpp_t *tcpp);

void store_tcpp(FILE *out, tcpp_t *tcpp);
void load_tcpp(FILE *out, tcpp_t *tcpp);

#ifdef __cplusplus
}
#endif

#endif
