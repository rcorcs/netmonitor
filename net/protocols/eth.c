/*
 * cethhdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "eth.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_eth_hdr(FILE *out, eth_hdr_t *ethh)
{
   int i;
   fprintf(out, "[Ethernet Protocol: %hu]\n", ETH_HDR_LEN);
   fprintf(out, "Destination:   %02x", ethh->dst.addr[0]);

   for(i=1; i<ETH_ADDR_LEN; i++)
      fprintf(out, ":%02x", ethh->dst.addr[i]);

   fprintf(out, "\nSource:        %02x", ethh->src.addr[0]);
   for(i=1; i<ETH_ADDR_LEN; i++)
      fprintf(out, ":%02x", ethh->src.addr[i]);

   fprintf(out, "\nType:          0x%04x\n", ntohs(ethh->type));
}

__inline void print_eth_hdr(eth_hdr_t *ethh)
{
   fprint_eth_hdr(stdout, ethh);
}
