/*
 * ctcphdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "tcp.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_tcp_hdr(FILE *out, tcp_hdr_t *tcph)
{
   fprintf(out, "[Transmission Control Protocol: %hu]\n", TCP_HDR_LEN(tcph));
   fprintf(out, "Source Port        : 0x%04x     : %hu\n", ntohs(tcph->src_port), ntohs(tcph->src_port));
   fprintf(out, "Destination Port   : 0x%04x     : %hu\n", ntohs(tcph->dst_port), ntohs(tcph->dst_port));
#ifdef __WIN32
   fprintf(out, "Sequence Number    : 0x%08lx : %lu\n", ntohl(tcph->seq), ntohl(tcph->seq));
   fprintf(out, "Acknowledge Number : 0x%08lx : %lu\n", ntohl(tcph->ack), ntohl(tcph->ack));
#else
   fprintf(out, "Sequence Number    : 0x%08x : %u\n", ntohl(tcph->seq), ntohl(tcph->seq));
   fprintf(out, "Acknowledge Number : 0x%08x : %u\n", ntohl(tcph->ack), ntohl(tcph->ack));
#endif
   fprintf(out, "Data Offset        : 0x%x        : %hu\n", tcph->doffset, tcph->doffset);
   fprintf(out, "URG Flag           : 0x%x        : %hu\n", tcph->urgbit, tcph->urgbit);
   fprintf(out, "ACK Flag           : 0x%x        : %hu\n", tcph->ackbit, tcph->ackbit);
   fprintf(out, "PUSH Flag          : 0x%x        : %hu\n", tcph->pshbit, tcph->pshbit);
   fprintf(out, "RST Flag           : 0x%x        : %hu\n", tcph->rstbit, tcph->rstbit);
   fprintf(out, "SYN Flag           : 0x%x        : %hu\n", tcph->synbit, tcph->synbit);
   fprintf(out, "FIN Flag           : 0x%x        : %hu\n", tcph->finbit, tcph->finbit);
   fprintf(out, "Window Size        : 0x%04x     : %hu\n", ntohs(tcph->win_size), ntohs(tcph->win_size));
   fprintf(out, "Checksum           : 0x%04x     : %hu\n", ntohs(tcph->checksum), ntohs(tcph->checksum));
   fprintf(out, "Urgent Pointer     : 0x%04x     : %hu\n", ntohs(tcph->urg_ptr), ntohs(tcph->urg_ptr));
}

__inline void print_tcp_hdr(tcp_hdr_t *tcph)
{
   fprint_tcp_hdr(stdout, tcph);
}
