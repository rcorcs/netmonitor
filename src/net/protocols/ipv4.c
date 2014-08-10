/*
 * ciphdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "ipv4.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char *ipv4_addr_to_c_str(ipv4_addr_t *ipv4)
{
   static char ipv4_c_str[16];
   sprintf(ipv4_c_str, "%d.%d.%d.%d",(int)ipv4->addr[0],(int)ipv4->addr[1],(int)ipv4->addr[2],(int)ipv4->addr[3]);
   return ipv4_c_str;
}

str_t *ipv4_addr_to_str(ipv4_addr_t *ipv4)
{
   str_t *ipv4str = (str_t *)malloc(sizeof(str_t));
   ipv4str->str = (char*)malloc(16);
   //ipv4str->len = 15;
   sprintf(ipv4str->str, "%d.%d.%d.%d",(int)ipv4->addr[0],(int)ipv4->addr[1],(int)ipv4->addr[2],(int)ipv4->addr[3]);
   ipv4str->len = strlen(ipv4str->str);
   return ipv4str;
}

void c_str_to_ipv4_addr(ipv4_addr_t *ipv4, const char *str)
{
    uint_t i, j;
    uint_t len;
    uint8_t b;

    b = 0;
    len = strlen(str);

    for(i = j = 0; i<len; i++){
        if(str[i]=='.') {
            ipv4->addr[j] = b;
            j++;
            b = 0;
        }else if(isdigit(str[i])) {
            b = b*10 + (str[i]-'0');
        }
    }
    ipv4->addr[j] = b;
}

void fprint_ipv4_hdr(FILE *out, ipv4_hdr_t *iph)
{
   //byte_t *buffer = (byte_t *)iph;
   /*
   str_t *src_str, *dst_str;
   */
   fprintf(out, "[Internet Protocol version 4: %hu]\n", IPV4_HDR_LEN(iph));
   fprintf(out, "IP version         : 0x%x        : %hu\n", iph->v, iph->v);
   fprintf(out, "IHL                : 0x%x        : %hu\n", iph->hl, iph->hl);
   fprintf(out, "ToS                : 0x%02x       : %hu\n", iph->tos, iph->tos);
   fprintf(out, "Total Length       : 0x%04x     : %hu\n", ntohs(iph->len), ntohs(iph->len));
   fprintf(out, "ID                 : 0x%04x     : %hu\n", ntohs(iph->id), ntohs(iph->id));
   fprintf(out, "Flags and Offset   : 0x%04x\n", ntohs(iph->foffset));
   fprintf(out, "Time to Live       : 0x%02x       : %hu\n", iph->ttl, iph->ttl );
   fprintf(out, "Protocol           : 0x%02x       : %hu\n", iph->proto, iph->proto);
   fprintf(out, "Checksum           : 0x%04x     : %hu\n", ntohs(iph->checksum), ntohs(iph->checksum));
   //fprintf(out, "Source IP          : 0x%02x%02x%02x%02x : %s\n", buffer[12], buffer[13], buffer[14], buffer[15], (char*)inet_ntoa(iph->src));
   //fprintf(out, "Destination IP     : 0x%02x%02x%02x%02x : %s\n", buffer[16], buffer[17], buffer[18], buffer[19], (char*)inet_ntoa(iph->dst));
   fprintf(out, "Source IP          : 0x%02x%02x%02x%02x : %s\n", iph->src.addr[0], iph->src.addr[1], iph->src.addr[2], iph->src.addr[3], ipv4_addr_to_c_str(&iph->src));
   fprintf(out, "Destination IP     : 0x%02x%02x%02x%02x : %s\n", iph->dst.addr[0], iph->dst.addr[1], iph->dst.addr[2], iph->dst.addr[3], ipv4_addr_to_c_str(&iph->dst));
   /*
   fprintf(out, "Source IP          : 0x%02x%02x%02x%02x : %s\n", iph->src.addr[0], iph->src.addr[1], iph->src.addr[2], iph->src.addr[3], (src_str=ipv4_addr_to_str(iph->src))->str);
   fprintf(out, "Destination IP     : 0x%02x%02x%02x%02x : %s\n", iph->dst.addr[0], iph->dst.addr[1], iph->dst.addr[2], iph->dst.addr[3], (dst_str=ipv4_addr_to_str(iph->dst))->str);
   destroy_str(&src_str);
   destroy_str(&dst_str);
   */
}

__inline void print_ipv4_hdr(ipv4_hdr_t *iph)
{
   fprint_ipv4_hdr(stdout, iph);
}

