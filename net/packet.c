/*
 * cpacket.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "packet.h"

#ifdef __WIN32
#include <winsock.h>       //Provides declarations for in_addr structure
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

#include "util.h"

void decode_eth_hdr(packet_t *, const byte_t *);
void decode_arp_hdr(packet_t *, const byte_t *);
void decode_vlan8021q_hdr(packet_t *, const byte_t *);
void decode_ppp_hdr(packet_t *, const byte_t *);
void decode_ipv4_hdr(packet_t *, const byte_t *);
void decode_ipv6_hdr(packet_t *, const byte_t *);
__inline void decode_tcp_hdr(packet_t *, const byte_t *);
__inline void decode_udp_hdr(packet_t *, const byte_t *);
__inline void decode_icmpv4_hdr(packet_t *, const byte_t *);
__inline void decode_icmpv6_hdr(packet_t *, const byte_t *);

void decode_packet(packet_t *pkt, const byte_t *buffer, uint_t len)
{
   //uint_t total_hdr_len;

   if(!pkt) return;

   pkt->ethh = 0;
   pkt->vlan8021qh = 0;
   pkt->arph = 0;
   pkt->ppph = 0;
   pkt->ipv4h = 0;
   pkt->ipv6h = 0;
   pkt->tcph = 0;
   pkt->hdrlen = 0;
   pkt->dlen = 0;
   pkt->data = 0;

   if(buffer && len)
      decode_eth_hdr(pkt, buffer);

   /*
   total_hdr_len = CETH_HDR_LEN;
   if(pkt->ppph) total_hdr_len += CPPP_HDR_LEN;
   if(pkt->ipv4h) total_hdr_len += CIP_HDR_LEN(pkt->ipv4h);
   if(pkt->tcph){
      switch(pkt->ipv4h->proto){
      case CIP_PROTO_TCP:
         total_hdr_len += CTCP_HDR_LEN(pkt->tcph); //4*(pkt->tcph->offset)
         break;
      case CIP_PROTO_UDP:
         //total_hdr_len += sizeof(*pkt->udph);
		 total_hdr_len += CUDP_HDR_LEN;
         break;
      case CIP_PROTO_ICMP:
	     //total_hdr_len += sizeof(*pkt->icmph);
         total_hdr_len += CICMP_HDR_LEN;
         break;
      }
   }
   pkt->hdrlen = total_hdr_len;
   */
   if(pkt->hdrlen<len){
      pkt->dlen = len - pkt->hdrlen;
      pkt->data = ((byte_t *)buffer)+pkt->hdrlen;
   } else {
      pkt->dlen = 0;
      pkt->data = 0;
   }
}

void decode_eth_hdr(packet_t *pkt, const byte_t *buffer)
{
   eth_hdr_t *ethh;

   //printf("ETH\n");

   ethh = (eth_hdr_t *)buffer;
   pkt->ethh = ethh;
   pkt->hdrlen = ETH_HDR_LEN;
   //pkt->ppph = 0;
   switch(ntohs(ethh->type)){
   case ETH_TYPE_IP:
      decode_ipv4_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   case ETH_TYPE_PPP_DISC:
      decode_ppp_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   case ETH_TYPE_PPP_SES:
      decode_ppp_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   case ETH_TYPE_IPV6: // TODO (...)
      decode_ipv6_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   case ETH_TYPE_ARP:
      decode_arp_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   case ETH_TYPE_8021Q:
      decode_vlan8021q_hdr(pkt, buffer+ETH_HDR_LEN);
      break;
   default:
      //pkt->ipv4h = 0;
      //pkt->ipv6h = 0;
      //pkt->tcph = 0;
      //pkt->data = 0;
      break;
   }
}

void decode_arp_hdr(packet_t *pkt, const byte_t *buffer)
{
   arp_hdr_t *arph;
   arph = (arp_hdr_t *)buffer;
   pkt->arph = arph;
   pkt->hdrlen += ARP_HDR_LEN;
}

void decode_vlan8021q_hdr(packet_t *pkt, const byte_t *buffer)
{
   vlan8021q_hdr_t *vlanh;

   //printf("PPP\n");

   vlanh = (vlan8021q_hdr_t *)(buffer-2);
   pkt->vlan8021qh = vlanh;
   pkt->hdrlen += (VLAN8021Q_HDR_LEN - 2);
   if(ntohs(vlanh->type)==ETH_TYPE_IP) {
      decode_ipv4_hdr(pkt, (buffer-2)+VLAN8021Q_HDR_LEN);
   } else if(ntohs(vlanh->type)==ETH_TYPE_IPV6){
       decode_ipv6_hdr(pkt, (buffer-2)+VLAN8021Q_HDR_LEN);
   } else {
   //   printf("*** %x : %x ***\n", ppph->proto, CPPP_PROTO_IPv4);
      //pkt->ipv4h = 0;
      //pkt->tcph = 0;
      //pkt->data = 0;
   }
}

void decode_ppp_hdr(packet_t *pkt, const byte_t *buffer)
{
   ppp_hdr_t *ppph;

   //printf("PPP\n");

   ppph = (ppp_hdr_t *)buffer;
   pkt->ppph = ppph;
   pkt->hdrlen += PPP_HDR_LEN;
   if(ntohs(ppph->proto)==PPP_PROTO_IPv4) {
      decode_ipv4_hdr(pkt, buffer+PPP_HDR_LEN);
   } else {
   //   printf("*** %x : %x ***\n", ppph->proto, CPPP_PROTO_IPv4);
      //pkt->ipv4h = 0;
      //pkt->tcph = 0;
      //pkt->data = 0;
   }
}

void decode_ipv4_hdr(packet_t *pkt, const byte_t *buffer)
{
   ipv4_hdr_t *ipv4h;

   //printf("IP\n");

   ipv4h = (ipv4_hdr_t *)buffer;
   pkt->ipv4h = ipv4h;
   pkt->hdrlen += IPV4_HDR_LEN(ipv4h);
   if(ipv4h->proto>=141 && ipv4h->proto<=252){ //UNASSIGNED
      //pkt->tcph = 0;
      //pkt->data = 0;
   }else {
      switch(ipv4h->proto){
      case IP_PROTO_TCP:
         decode_tcp_hdr(pkt, buffer+IPV4_HDR_LEN(ipv4h));
         break;
      case IP_PROTO_UDP:
         decode_udp_hdr(pkt, buffer+IPV4_HDR_LEN(ipv4h));
         break;
      case IP_PROTO_ICMP:
         decode_icmpv4_hdr(pkt, buffer+IPV4_HDR_LEN(ipv4h));
         break;
      default:
         //pkt->tcph = 0;
         //pkt->data = 0;
         break;
      }
   }
}

void decode_ipv6_hdr(packet_t *pkt, const byte_t *buffer)
{
   ipv6_hdr_t *ipv6h;

   //printf("IP\n");

   ipv6h = (ipv6_hdr_t *)buffer;
   pkt->ipv6h = ipv6h;
   pkt->hdrlen += IPV6_HDR_LEN;
   if(ipv6h->nexthdr>=141 && ipv6h->nexthdr<=252){ //UNASSIGNED
      //pkt->tcph = 0;
      //pkt->data = 0;
   }else {
      switch(ipv6h->nexthdr){
      case IP_PROTO_TCP:
         decode_tcp_hdr(pkt, buffer+IPV6_HDR_LEN);
         break;
      case IP_PROTO_UDP:
         decode_udp_hdr(pkt, buffer+IPV6_HDR_LEN);
         break;
      case IP_PROTO_IPV6_ICMP:
         decode_icmpv6_hdr(pkt, buffer+IPV6_HDR_LEN);
         break;
      default:
         //pkt->tcph = 0;
         //pkt->data = 0;
         break;
      }
   }
}

__inline void decode_tcp_hdr(packet_t *pkt, const byte_t *buffer)
{
   pkt->tcph = (tcp_hdr_t *)buffer, pkt->hdrlen += TCP_HDR_LEN(pkt->tcph);
}

__inline void decode_udp_hdr(packet_t *pkt, const byte_t *buffer)
{
   pkt->udph = (udp_hdr_t *)buffer, pkt->hdrlen += UDP_HDR_LEN;
}

__inline void decode_icmpv4_hdr(packet_t *pkt, const byte_t *buffer)
{
   pkt->icmpv4h = (icmpv4_hdr_t *)buffer, pkt->hdrlen += ICMPV4_HDR_LEN;
}

__inline void decode_icmpv6_hdr(packet_t *pkt, const byte_t *buffer)
{
   pkt->icmpv6h = (icmpv6_hdr_t *)buffer, pkt->hdrlen += ICMPV6_HDR_LEN;
}

void fprint_packet(FILE *out, packet_t *pkt)
{
   fprintf(out, "PACKET HEADER LENGTH: %hu\n", pkt->hdrlen);
   fprintf(out, "PACKET DATA LENGTH: %hu\n", pkt->dlen);
   fprint_eth_hdr(out, pkt->ethh);
   int datapad = 0;

   if(pkt->arph)
      fprint_arp_hdr(out, pkt->arph);

   if(pkt->vlan8021qh)
      fprint_vlan8021q_hdr(out, pkt->vlan8021qh);

   if(pkt->ppph)
      fprint_ppp_hdr(out, pkt->ppph);

   if(pkt->ipv4h) {
      fprint_ipv4_hdr(out, pkt->ipv4h);

       if(pkt->tcph){
             switch(pkt->ipv4h->proto){
             case IP_PROTO_TCP:
                fprint_tcp_hdr(out, pkt->tcph);
                break;
             case IP_PROTO_UDP:
                fprint_udp_hdr(out, pkt->udph);
                if(ntohs(pkt->udph->src_port)==53 || ntohs(pkt->udph->dst_port)==53){
                   dns_msg_t dnsmsg;
                   datapad += decode_dns_msg(&dnsmsg, pkt->data);
                   //print_dns(dnsh);
                   print_dns_msg(&dnsmsg);
                   //exit(0);
                   destroy_dns_msg(&dnsmsg);
                }
                break;
             case IP_PROTO_ICMP:
                fprint_icmpv4_hdr(out, pkt->icmpv4h);
                break;
             default:
                break;
             }
          }
   }else if(pkt->ipv6h) {
      fprint_ipv6_hdr(out, pkt->ipv6h);

      if(pkt->tcph){
             switch(pkt->ipv6h->nexthdr){
             case IP_PROTO_TCP:
                fprint_tcp_hdr(out, pkt->tcph);
                break;
             case IP_PROTO_UDP:
                fprint_udp_hdr(out, pkt->udph);
                if(ntohs(pkt->udph->src_port)==53 || ntohs(pkt->udph->dst_port)==53){
                   dns_msg_t dnsmsg;
                   datapad += decode_dns_msg(&dnsmsg, pkt->data);
                   //print_dns(dnsh);
                   print_dns_msg(&dnsmsg);
                   //exit(0);
                   destroy_dns_msg(&dnsmsg);
                }
                break;
             case IP_PROTO_IPV6_ICMP:
                fprint_icmpv6_hdr(out, pkt->icmpv6h);
                break;
             default:
                break;
             }
          }
   }

   if(pkt->data && ((long int)(pkt->dlen-datapad))>0){
      fdump(out, pkt->data+datapad, (pkt->dlen-datapad));
   }

   fprintf(out, "-------------------------------------------------------------------------------\n");
   //fdump(out, pkt->data, pkt->dlen);
}

__inline void print_packet(packet_t *pkt)
{
   fprint_packet(stdout, pkt);
}
