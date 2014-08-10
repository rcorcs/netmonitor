#ifndef __ARP
#define __ARP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#include "ipv4.h"
#include "eth.h"

#define ARP_HDR_LEN 28

typedef struct __arp_hdr{
   uint16_t     htype; //
   uint16_t     proto;
   uint8_t      hlen;
   uint8_t      plen;
   uint16_t     opcode;
   eth_addr_t   src_ethh;
   ipv4_addr_t  src_iph;
   eth_addr_t   dst_ethh;
   ipv4_addr_t  dst_iph;
} arp_hdr_t;

void fprint_arp_hdr(FILE *, arp_hdr_t *);
__inline void print_arp_hdr(arp_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif

