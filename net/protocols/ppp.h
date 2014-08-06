/*
 * ppp.h
 *            Global definitions for the Point-to-Point Protocol over Ethernet.
 * version:   1.0.0
 * date:      03 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __PPP
#define __PPP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#define PPP_PROTO_IPv4     0x0021
#define PPP_PROTO_OSI      0x0023
#define PPP_PROTO_IPX      0x002B
#define PPP_PROTO_PPPMP    0x003D
#define PPP_PROTO_NETBIOS  0x003F
#define PPP_PROTO_CISCO    0x0041
#define PPP_PROTO_IBMSNA   0x004D
#define PPP_PROTO_IPv6     0x0057

#define PPP_HDR_LEN 8     // 8 bytes header

typedef struct __ppp_hdr {
#if ( defined BYTE_ORDER && (defined LITTLE_ENDIAN || defined BIG_ENDIAN) )
#if BYTE_ORDER == LITTLE_ENDIAN
   uint8_t  type:4,  //type
            v:4;     //version
#elif BYTE_ORDER == BIG_ENDIAN
   uint8_t  v:4,     //version
            type:4;  //type
#endif //endif BYTE_ORDER
#else //default: intel x86 and x86-64 use little-endian format
   uint8_t  type:4,  //type
            v:4;     //version
#endif //endif defined BYTE_ORDER
   uint8_t  code;    //code
   uint16_t sid;     //session id
   uint16_t len;     //length => 2 + sizeof(payload), the 2 value is the size of the proto
   uint16_t proto;   //protocol
} ppp_hdr_t;

void fprint_ppp_hdr(FILE *, ppp_hdr_t *);
__inline void print_ppp_hdr(ppp_hdr_t *);


#ifdef __cplusplus
}
#endif

#endif

