#include "tcpp.h"

#ifdef __cplusplus
extern "C" {
#endif

void fprint_tcp_record(FILE *out, tcp_record_t *tcprecord)
{
   /// total of tcp packets
   fprintf(out, "Total:          %u\n", tcprecord->total);
   /// TCP FLAG bits
   fprintf(out, "ACK:            %u\n", tcprecord->tack);
   fprintf(out, "PSH:            %u\n", tcprecord->tpsh);
   fprintf(out, "RST:            %u\n", tcprecord->trst);
   fprintf(out, "SYN:            %u\n", tcprecord->tsyn);
   fprintf(out, "FIN:            %u\n", tcprecord->tfin);
   /// Total of Data length
   fprintf(out, "Data Length:    %u\n", tcprecord->tdatalen);
}

__inline void print_tcp_record(tcp_record_t *tcprecord)
{
   fprint_tcp_record(stdout, tcprecord);
}

void store_tcp_record(FILE *out, tcp_record_t *tcprecord)
{
    fwrite(&tcprecord->total, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->tack, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->tpsh, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->trst, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->tsyn, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->tfin, sizeof(uint_t), 1, out);
    fwrite(&tcprecord->tdatalen, sizeof(uint_t), 1, out);
}

void load_tcp_record(FILE *out, tcp_record_t *tcprecord)
{
    fread(&tcprecord->total, sizeof(uint_t), 1, out);
    fread(&tcprecord->tack, sizeof(uint_t), 1, out);
    fread(&tcprecord->tpsh, sizeof(uint_t), 1, out);
    fread(&tcprecord->trst, sizeof(uint_t), 1, out);
    fread(&tcprecord->tsyn, sizeof(uint_t), 1, out);
    fread(&tcprecord->tfin, sizeof(uint_t), 1, out);
    fread(&tcprecord->tdatalen, sizeof(uint_t), 1, out);
}

vec_t *create_tcpsyn_vec(tcp_record_t *tcprecord)
{
   vec_t *vec = create_vec(sizeof(double), 1);

   VECGET(double, vec, 0) = ((double)(tcprecord->tsyn))-(tcprecord->tfin+tcprecord->trst);

   return vec;
}

mat_t *create_tcpsyn_mat(list_t *list)
{
   mat_t *mat = create_mat(sizeof(double), list_size(list), 1);
   node_t *node;
   tcpp_t *tcpp;
   tcp_record_t *tcprecord;

   int i = 0;
   LIST_FOREACH(node, list){
      tcpp = (tcpp_t *)node->data;
      tcprecord = &(tcpp->out);

      MATGET(double, mat, i, 0) = ((double)(tcprecord->tsyn))-(tcprecord->tfin+tcprecord->trst);
      ++i;
   }

   return mat;
}


tcpp_t *create_tcpp()
{
    tcpp_t *tcpp = (tcpp_t *)malloc(sizeof(tcpp_t));
    return tcpp;
}

void destroy_tcpp(tcpp_t **tcpp)
{
    free(*tcpp);
    *tcpp = 0;
}

void fprint_tcpp(FILE *out, tcpp_t *tcpp)
{
   fprintf(out, "[Incoming Packets]\n");
   fprint_tcp_record(out, &tcpp->in);
   fprintf(out, "[Outgoing Packets]\n");
   fprint_tcp_record(out, &tcpp->out);
}

__inline void print_tcpp(tcpp_t *tcpp)
{
   fprint_tcpp(stdout, tcpp);
}

void store_tcpp(FILE *out, tcpp_t *tcpp)
{
    store_tcp_record(out, &tcpp->in);
    store_tcp_record(out, &tcpp->out);
}

void load_tcpp(FILE *in, tcpp_t *tcpp)
{
    load_tcp_record(in, &tcpp->in);
    load_tcp_record(in, &tcpp->out);
}

#ifdef __cplusplus
}
#endif
