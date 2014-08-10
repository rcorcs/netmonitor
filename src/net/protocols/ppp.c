/*
 * cppphdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "ppp.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_ppp_hdr(FILE *out, ppp_hdr_t *ppph)
{
   fprintf(out, "[Point-to-Point Protocol over Ethernet: %hu]\n", PPP_HDR_LEN);
   fprintf(out, "Version  : 0x%x    : %hu\n", ppph->v, ppph->v);
   fprintf(out, "Type     : 0x%x    : %hu\n", ppph->type, ppph->type);
   fprintf(out, "Code     : 0x%02x   : %hu\n", ppph->code, ppph->code);
   fprintf(out, "ID       : 0x%04x : %hu\n", ntohs(ppph->sid), ntohs(ppph->sid));
   fprintf(out, "Length   : 0x%04x : %hu\n", ntohs(ppph->len), ntohs(ppph->len));
   fprintf(out, "Protocol : 0x%04x : %hu\n", ntohs(ppph->proto), ntohs(ppph->proto));
}

__inline void print_ppp_hdr(ppp_hdr_t *ppph)
{
   fprint_ppp_hdr(stdout, ppph);
}
