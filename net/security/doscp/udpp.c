#include "udpp.h"


#ifdef __cplusplus
extern "C" {
#endif


void fprint_udp_record(FILE *out, udp_record_t *udprecord)
{
  fprintf(out, "Total:    %u\n", udprecord->total);
}

__inline void print_udp_record(udp_record_t *udprecord)
{
   fprint_udp_record(stdout, udprecord);
}

void store_udp_record(FILE *out, udp_record_t *udprecord)
{
    fwrite(&udprecord->total, sizeof(udprecord->total), 1, out);
}

void load_udp_record(FILE *out, udp_record_t *udprecord)
{
    fread(&udprecord->total, sizeof(udprecord->total), 1, out);
}

vec_t *create_udp_vec(udp_record_t *udprecord)
{
   vec_t *vec = create_vec(sizeof(double), 1);

   VECGET(double, vec, 0) = (double)(udprecord->total);

   return vec;
}

mat_t *create_udp_mat(list_t *list)
{
   mat_t *mat = create_mat(sizeof(double), list_size(list), 1);
   node_t *node;
   udpp_t *udpp;
   udp_record_t *udprecord;

   int i = 0;
   LIST_FOREACH(node, list){
      udpp = (udpp_t *)node->data;
      udprecord = &(udpp->out);

      MATGET(double, mat, i, 0) = (double)(udprecord->total);
      ++i;
   }

   return mat;
}


udpp_t *create_udpp()
{
   udpp_t *udpp = (udpp_t *)malloc(sizeof(udpp_t));
   return udpp;
}

void destroy_udpp(udpp_t **udpp)
{
   free(*udpp);
   *udpp = 0;
}


void fprint_udpp(FILE *out, udpp_t *udpp)
{
   fprintf(out, "[Incoming Packets]\n");
   fprint_udp_record(out, &udpp->in);
   fprintf(out, "[Outgoing Packets]\n");
   fprint_udp_record(out, &udpp->out);
}

__inline void print_udpp(udpp_t *udpp)
{
   fprint_udpp(stdout, udpp);
}

void store_udpp(FILE *out, udpp_t *udpp)
{
    store_udp_record(out, &udpp->in);
    store_udp_record(out, &udpp->out);
}

void load_udpp(FILE *in, udpp_t *udpp)
{
    load_udp_record(in, &udpp->in);
    load_udp_record(in, &udpp->out);
}

#ifdef __cplusplus
}
#endif
