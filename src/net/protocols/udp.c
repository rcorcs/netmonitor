/*
 * cudphdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "udp.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_udp_hdr(FILE *out, udp_hdr_t *udph)
{
   fprintf(out, "[User Datagram Protocol: %hu]\n", UDP_HDR_LEN);
   fprintf(out, "Source Port        : 0x%04x     : %hu\n", ntohs(udph->src_port), ntohs(udph->src_port));
   fprintf(out, "Destination Port   : 0x%04x     : %hu\n", ntohs(udph->dst_port), ntohs(udph->dst_port));
   fprintf(out, "Length             : 0x%04x     : %hu\n", ntohs(udph->len), ntohs(udph->len));
   fprintf(out, "Checksum           : 0x%04x     : %hu\n", ntohs(udph->checksum), ntohs(udph->checksum));
}

__inline void print_udp_hdr(udp_hdr_t *udph)
{
   fprint_udp_hdr(stdout, udph);
}
