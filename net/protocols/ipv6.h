/*
 * cipv6hdr.h
 *            Global definitions for the Internet Protocol version 6.
 * version:   1.0.0
 * date:      08 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __IPV6
#define __IPV6

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"
#include "../../datatypes/str.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#endif

#define IPV6_ADDR_LEN 16
typedef struct __ipv6_addr {
   uint8_t addr[IPV6_ADDR_LEN];
} ipv6_addr_t;

const char *ipv6_addr_to_c_str(ipv6_addr_t *ipv6);
str_t *ipv6_addr_to_str(ipv6_addr_t *ipv6);


#define __IPHVEC(_IPH_)             ((byte_t*)(_IPH_))
//VERSION MASK: 0xF0000000
#define IPV6_INFO_VERSION(_IPH_)    (__IPHVEC(_IPH_)[0]>>4)
//TRAFFIC CLASS MASK: 0x0FF00000
#define IPV6_INFO_TC(_IPH_)         ((__IPHVEC(_IPH_)[0]&0x0F)<<4)|((__IPHVEC(_IPH_)[1])>>4)
//FLOW LABEL MASK: 0x000FFFFF
#define IPV6_INFO_FLOWLABEL(_IPH_)  (((uint32_t)(__IPHVEC(_IPH_)[1]&0x0F))<<16)|(((uint32_t)(__IPHVEC(_IPH_)[2]))<<8)|(((uint32_t)(__IPHVEC(_IPH_)[3])))


#define IPV6_HDR_LEN 40
typedef struct __ipv6_hdr {
#if ( defined BYTE_ORDER && (defined LITTLE_ENDIAN || defined BIG_ENDIAN) )
#if BYTE_ORDER == LITTLE_ENDIAN
   uint32_t        info;
#elif BYTE_ORDER == BIG_ENDIAN
union {
   uint32_t        v:4;
                   tc:8,
				   flowl:20;
   uint32_t        info;
};
#endif //endif BYTE_ORDER
#else //default: intel x86 and x86-64 use little-endian format
   uint32_t        info;
#endif //endif defined BYTE_ORDER
   uint16_t        ploadlen;
   uint8_t         nexthdr;
   uint8_t         hoplimit;
   ipv6_addr_t     src;
   ipv6_addr_t     dst;
} ipv6_hdr_t;

void fprint_ipv6_hdr(FILE *, ipv6_hdr_t *);
__inline void print_ipv6_hdr(ipv6_hdr_t *);

/* Hop-by-Hop Options and Destination Options Extension Header - IPv6 */
#define IPV6_HOP_HDR_LEN(_HOP_) (((_HOP_)->hdrlen)+1)
typedef struct __ipv6_hop_hdr {
   uint8_t         nexthdr;
   uint8_t         hdrlen;
   byte_t         *opt;
} ipv6_hop_hdr_t;


/* Routing Extension Header - IPv6 */
#define IPV6_ROUTE_HDR_LEN(_ROUTE_) (((_ROUTE_)->hdrlen)+1)
typedef struct __ipv6_route_hdr {
   uint8_t         nexthdr;
   uint8_t         hdrlen;
   uint8_t         type;
   uint8_t         segleft;
   byte_t         *data;
} ipv6_route_hdr_t;

/* Fragment Extension Header - IPv6 */
typedef struct __ipv6_frag_hdr {
   uint8_t         nexthdr;
   uint8_t         reserved;
   uint16_t        foffset;
   uint32_t        id;
} ipv6_frag_hdr_t;

#ifdef __cplusplus
}
#endif

#endif
