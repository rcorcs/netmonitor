
#include "ipv6.h"

#include <string.h>
#include <stdlib.h>

const char *ipv6_addr_to_c_str(ipv6_addr_t *ipv6)
{
   static char ipv6_c_str[40];
   sprintf(ipv6_c_str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
           (int)ipv6->addr[0],
           (int)ipv6->addr[1],
           (int)ipv6->addr[2],
           (int)ipv6->addr[3],
           (int)ipv6->addr[4],
           (int)ipv6->addr[5],
           (int)ipv6->addr[6],
           (int)ipv6->addr[7],
           (int)ipv6->addr[8],
           (int)ipv6->addr[9],
           (int)ipv6->addr[10],
           (int)ipv6->addr[11],
           (int)ipv6->addr[12],
           (int)ipv6->addr[13],
           (int)ipv6->addr[14],
           (int)ipv6->addr[15]
           );
   return ipv6_c_str;
}

str_t *ipv6_addr_to_str(ipv6_addr_t *ipv6)
{
   str_t *ipv6str = (str_t *)malloc(sizeof(str_t));
   ipv6str->str = (char*)malloc(40);
   ipv6str->len = 40;
   sprintf(ipv6str->str, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
           (int)ipv6->addr[0],
           (int)ipv6->addr[1],
           (int)ipv6->addr[2],
           (int)ipv6->addr[3],
           (int)ipv6->addr[4],
           (int)ipv6->addr[5],
           (int)ipv6->addr[6],
           (int)ipv6->addr[7],
           (int)ipv6->addr[8],
           (int)ipv6->addr[9],
           (int)ipv6->addr[10],
           (int)ipv6->addr[11],
           (int)ipv6->addr[12],
           (int)ipv6->addr[13],
           (int)ipv6->addr[14],
           (int)ipv6->addr[15]
           );
   return ipv6str;
}

void fprint_ipv6_hdr(FILE *out, ipv6_hdr_t *ipv6h)
{
   //ipv6h->info = htonl(0x87654321);
   //uint32_t temp = ntohl(ipv6h->info);
   fprintf(out, "[Internet Protocol version 6: %u]\n", IPV6_HDR_LEN);
   fprintf(out, "IP version         : 0x%x        : %hu\n", IPV6_INFO_VERSION(ipv6h), IPV6_INFO_VERSION(ipv6h));
   //fprintf(out, "IP version         : 0x%02x:0x%02x:0x%02x:0x%02x\n", (uint32_t)((char*)(&ipv6h->info))[0], (uint32_t)((char*)(&ipv6h->info))[1], (uint32_t)((char*)(&ipv6h->info))[2], (uint32_t)((char*)(&ipv6h->info))[3]);
   //fprintf(out, "IP version         : 0x%02x:0x%02x:0x%02x:0x%02x\n", (uint32_t)((char*)(&temp))[0],(uint32_t)((char*)(&temp))[1],(uint32_t)((char*)(&temp))[2],(uint32_t)((char*)(&temp))[3]);
   fprintf(out, "Traffic Class      : 0x%x        : %u\n", IPV6_INFO_TC(ipv6h), IPV6_INFO_TC(ipv6h));
   fprintf(out, "Flow Label         : 0x%x        : %u\n", IPV6_INFO_FLOWLABEL(ipv6h), IPV6_INFO_FLOWLABEL(ipv6h));
   fprintf(out, "PayLoad Length     : 0x%04x    : %hu\n", ntohs(ipv6h->ploadlen), ntohs(ipv6h->ploadlen));
   fprintf(out, "Next Header        : 0x%02x      : %hu\n", ipv6h->nexthdr, ipv6h->nexthdr);
   fprintf(out, "Hop Limit          : 0x%02x      : %hu\n", ipv6h->hoplimit, ipv6h->hoplimit);
   fprintf(out, "Source             : %s\n", ipv6_addr_to_c_str(&ipv6h->src));
   fprintf(out, "Destination        : %s\n", ipv6_addr_to_c_str(&ipv6h->dst));
}

__inline void print_ipv6_hdr(ipv6_hdr_t *ipv6h)
{
    fprint_ipv6_hdr(stdout, ipv6h);
}
