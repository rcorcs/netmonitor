/*
 * cpacket.h
 *            Global definitions for a network packet.
 * version:   1.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __PACKET
#define __PACKET

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "protocols/arp.h"
#include "protocols/dns.h"
#include "protocols/eth.h"
#include "protocols/vlan8021q.h"
#include "protocols/icmpv4.h"
#include "protocols/icmpv6.h"
#include "protocols/ipv4.h"
#include "protocols/ipv6.h"
#include "protocols/ppp.h"
#include "protocols/tcp.h"
#include "protocols/udp.h"


//package
typedef struct __packet {
   eth_hdr_t        * ethh;
   vlan8021q_hdr_t  * vlan8021qh;
   arp_hdr_t        * arph;
   ppp_hdr_t        * ppph;
   ipv4_hdr_t       * ipv4h;
   ipv6_hdr_t       * ipv6h;
   union {
      tcp_hdr_t     * tcph;
      udp_hdr_t     * udph;
      icmpv4_hdr_t  * icmpv4h;
      icmpv6_hdr_t  * icmpv6h;
   };
   uint_t             hdrlen;
   uint_t             dlen;
   byte_t           * data;
} packet_t;

void decode_packet(packet_t *, const byte_t *, uint_t);

void fprint_packet(FILE *, packet_t *);
__inline void print_packet(packet_t *);


#ifdef __cplusplus
}
#endif

#endif
