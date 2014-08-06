/*
 * cdnshdr.c
 * version:   1.0.0
 * date:      08 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "dns.h"
#include "ipv4.h"
#include "ipv6.h"

#include <stdlib.h>
#include <string.h>

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_dns_hdr(FILE *out, dns_hdr_t *dnsh)
{
   fprintf(out, "[Domain Name System]\n");
   fprintf(out, "Identifier         : 0x%04x     : %hu\n", ntohs(dnsh->id), ntohs(dnsh->id));
   fprintf(out, "QR                 : 0x%x        : %hu\n", dnsh->qrbit, dnsh->qrbit);
   fprintf(out, "Opcode             : 0x%x        : %hu\n", dnsh->opcode, dnsh->opcode);
   fprintf(out, "AA                 : 0x%x        : %hu\n", dnsh->aabit, dnsh->aabit);
   fprintf(out, "TC                 : 0x%x        : %hu\n", dnsh->tcbit, dnsh->tcbit);
   fprintf(out, "RD                 : 0x%x        : %hu\n", dnsh->rdbit, dnsh->rdbit);
   fprintf(out, "RA                 : 0x%x        : %hu\n", dnsh->rabit, dnsh->rabit);
   fprintf(out, "Z                  : 0x%x        : %hu\n", dnsh->zbit, dnsh->zbit);
   fprintf(out, "AD                 : 0x%x        : %hu\n", dnsh->adbit, dnsh->adbit);
   fprintf(out, "CD                 : 0x%x        : %hu\n", dnsh->cdbit, dnsh->cdbit);
   fprintf(out, "Rcode              : 0x%x        : %hu\n", dnsh->rcode, dnsh->rcode);
   fprintf(out, "Total Question     : 0x%04x     : %hu\n", ntohs(dnsh->tq), ntohs(dnsh->tq));
   fprintf(out, "Total Answers      : 0x%04x     : %hu\n", ntohs(dnsh->tans), ntohs(dnsh->tans));
   fprintf(out, "Total Authority    : 0x%04x     : %hu\n", ntohs(dnsh->tauth), ntohs(dnsh->tauth));
   fprintf(out, "Total Additional   : 0x%04x     : %hu\n", ntohs(dnsh->tadd), ntohs(dnsh->tadd));
}

__inline void print_dns_hdr(dns_hdr_t *dnsh)
{
   fprint_dns_hdr(stdout, dnsh);
}

void fprint_dns_msg(FILE *out, dns_msg_t *dnsmsg)
{
   uint_t i, j;
   uchar_t *str;
   fprint_dns_hdr(out, dnsmsg->dnsh);
   for(i=0;i<ntohs(dnsmsg->dnsh->tq); i++){
      fprintf(out, "[Query #%u]\n", i);
      fprintf(out, "Name:     ");
      for(str = dnsmsg->dnsq[i].name; (*str)!=0; str++){
          if(((*str)>31) && ((*str)<127))
             fputc((*str), out);
          else fputc('.', out);
      }
      fputc('\n', out);
      fprintf(out, "Type:       %hu\n", ntohs(dnsmsg->dnsq[i].type));
      fprintf(out, "Class:      %hu\n", ntohs(dnsmsg->dnsq[i].c));
   }
   for(i=0;i<ntohs(dnsmsg->dnsh->tans); i++){
      fprintf(out, "[Answer #%u]\n", i);
      fprintf(out, "Name:       ");
      if(dnsmsg->dnsans[i].name){
         for(str = dnsmsg->dnsans[i].name; (*str)!=0; str++){
            if(((*str)>31) && ((*str)<127))
               fputc((*str), out);
             else fputc('.', out);
         }
      }
      fputc('\n', out);
      fprintf(out, "Type:       %hu\n", ntohs(dnsmsg->dnsans[i].type));
      fprintf(out, "Class:      %hu\n", ntohs(dnsmsg->dnsans[i].c));
      fprintf(out, "TTL:        %u\n", ntohs(dnsmsg->dnsans[i].ttl));
      fprintf(out, "RD Length:  %hu\n", ntohs(dnsmsg->dnsans[i].rdlen));
      fprintf(out, "RData:      ");
      if(ntohs(dnsmsg->dnsans[i].type)==DNS_TYPE_CNAME && dnsmsg->dnsans[i].rdata){
         for(str = dnsmsg->dnsans[i].rdata; (*str)!=0; str++){
            if(((*str)>31) && ((*str)<127))
               fputc((*str), out);
             else fputc('.', out);
         }
      }else if(ntohs(dnsmsg->dnsans[i].type)==DNS_TYPE_A){
         if(ntohs(dnsmsg->dnsans[i].rdlen)==IPV4_ADDR_LEN){
            fprintf(out, "%d.%d.%d.%d",(int)dnsmsg->dnsans[i].rdata[0],(int)dnsmsg->dnsans[i].rdata[1],(int)dnsmsg->dnsans[i].rdata[2],(int)dnsmsg->dnsans[i].rdata[3]);
         }else if(ntohs(dnsmsg->dnsans[i].rdlen)==IPV6_ADDR_LEN){
            fprintf(out, "ipv6");
         }
      }
      fputc('\n', out);
   }
}

__inline void print_dns_msg(dns_msg_t *dnsmsg)
{
    fprint_dns_msg(stdout, dnsmsg);
}


/***
 * More about DNS Domain Name COMPRESSION: http://tools.ietf.org/html/rfc1035#section-4.1.4
*/
uint_t decode_dns_msg(dns_msg_t *dnsmsg, byte_t *buffer)
{
   uint_t i;
   uint_t len = 0;
   byte_t *temp = buffer;
   dnsmsg->dnsh = (dns_hdr_t *)buffer;

   buffer += DNS_HDR_LEN;
   len = DNS_HDR_LEN;


   if(ntohs(dnsmsg->dnsh->tq)){
      dnsmsg->dnsq = (dns_query_t *)malloc(sizeof(dns_query_t)*ntohs(dnsmsg->dnsh->tq));
      for(i=0;i<ntohs(dnsmsg->dnsh->tq); i++){
         dnsmsg->dnsq[i].namelen = strlen((char*)buffer)+1;
         dnsmsg->dnsq[i].name = (uchar_t *)malloc(dnsmsg->dnsq[i].namelen);
         strcpy((char*)dnsmsg->dnsq[i].name, (char*)buffer);
         buffer += dnsmsg->dnsq[i].namelen;
         len += dnsmsg->dnsq[i].namelen;
         dnsmsg->dnsq[i].type = *((uint16_t*)buffer);
         buffer += sizeof(uint16_t); //size dns_query_t::type
         len += sizeof(uint16_t); //size dns_query_t::type
         dnsmsg->dnsq[i].c = *((uint16_t*)buffer);
         buffer += sizeof(uint16_t); //dns_query_t::class
         len += sizeof(uint16_t); //dns_query_t::class
      }
   }else dnsmsg->dnsq = 0;

   if(ntohs(dnsmsg->dnsh->tans)){
      dnsmsg->dnsans = (dns_rr_t *)malloc(sizeof(dns_rr_t)*ntohs(dnsmsg->dnsh->tans));
      for(i=0;i<ntohs(dnsmsg->dnsh->tans); i++){
         if(!((*buffer)&0xc0)){
            dnsmsg->dnsans[i].namelen = strlen((char*)buffer)+1;
            dnsmsg->dnsans[i].name = (uchar_t *)malloc(dnsmsg->dnsans[i].namelen);
            strcpy((char*)dnsmsg->dnsans[i].name, (char*)buffer);
            buffer += dnsmsg->dnsans[i].namelen;
            len += dnsmsg->dnsans[i].namelen;
         }else{
            int j = 0;
            uint16_t offset = *((uint16_t*)buffer);
            offset = ((uint16_t)ntohs(offset))&((uint16_t)0x3fff);
            char str[255];
            memset(str, 0, 255);
            for(j=0; j<254; ++j){
               if((*(temp+offset))!=0xc0){
                  if(*(temp+offset)){
                     str[j] = *(temp+offset);
                     ++offset;
                  }else {
                      break;
                  }
               }else {
                  offset = *((uint16_t*)(temp+offset));
                  offset = ((uint16_t)ntohs(offset))&((uint16_t)0x3fff);
                  --j;
               }
            }
            dnsmsg->dnsans[i].namelen = strlen(str)+1;
            dnsmsg->dnsans[i].name = (char*)malloc(dnsmsg->dnsans[i].namelen);
            memcpy(dnsmsg->dnsans[i].name, str, dnsmsg->dnsans[i].namelen);
            buffer += 2;
            len += 2;
         }
         dnsmsg->dnsans[i].type = *((uint16_t*)buffer);
         buffer += sizeof(uint16_t);
         len += sizeof(uint16_t);
         dnsmsg->dnsans[i].c = *((uint16_t*)buffer);
         buffer += sizeof(uint16_t);
         len += sizeof(uint16_t);
         dnsmsg->dnsans[i].ttl = *((uint32_t*)buffer);
         buffer += sizeof(uint32_t);
         len += sizeof(uint32_t);
         dnsmsg->dnsans[i].rdlen = *((uint16_t*)buffer);
         buffer += sizeof(uint16_t);
         len += sizeof(uint16_t);
         if(ntohs(dnsmsg->dnsans[i].type)==DNS_TYPE_CNAME){
            int j = 0;
            uint16_t offset = buffer - temp;
            char str[255];
            memset(str, 0, 255);
            for(j=0; j<254; ++j){
               if((*(temp+offset))!=0xc0){
                  if(*(temp+offset)){
                     str[j] = *(temp+offset);
                     ++offset;
                  }else {
                      break;
                  }
               }else {
                  offset = *((uint16_t*)(temp+offset));
                  offset = ((uint16_t)ntohs(offset))&((uint16_t)0x3fff);
                  --j;
               }
            }
            buffer += ntohs(dnsmsg->dnsans[i].rdlen);
            len += ntohs(dnsmsg->dnsans[i].rdlen);
            dnsmsg->dnsans[i].rdlen = htons(strlen(str)+1);
            dnsmsg->dnsans[i].rdata = (char*)malloc(dnsmsg->dnsans[i].rdlen);
            memcpy(dnsmsg->dnsans[i].rdata, str, ntohs(dnsmsg->dnsans[i].rdlen));
         }else{
            dnsmsg->dnsans[i].rdata = (uchar_t *)malloc(ntohs(dnsmsg->dnsans[i].rdlen)+1);
            memcpy((char*)dnsmsg->dnsans[i].rdata, (char*)buffer, ntohs(dnsmsg->dnsans[i].rdlen));
            buffer += ntohs(dnsmsg->dnsans[i].rdlen);
            len += ntohs(dnsmsg->dnsans[i].rdlen);
         }
      }
   }else dnsmsg->dnsans = 0;

   if(ntohs(dnsmsg->dnsh->tauth)){

   }else dnsmsg->dnsauth = 0;

   if(ntohs(dnsmsg->dnsh->tadd)){

   }else dnsmsg->dnsadd = 0;

   return len;
}

void destroy_dns_msg(dns_msg_t *dnsmsg)
{
   uint_t i;
   for(i=0;i<ntohs(dnsmsg->dnsh->tq); i++){
       free(dnsmsg->dnsq[i].name);
   }
   if(dnsmsg->dnsq) free(dnsmsg->dnsq);
}
