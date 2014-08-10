/*
 * net/protocols/icmpv6.h
 *            Global definitions for the Internet Control Message Protocol.
 * version:   2.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
 * Changes:
 *   12 September 2011   changed pattern of names
*/
#ifndef __ICMPV6
#define __ICMPV6

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#define ICMPV6_TYPE_DST_UNREACH          1   //Destination Unreachable
#  define ICMPV6_CODE_NO_ROUTE           0   //No route to destination
#  define ICMPV6_CODE_COMMUNICATION      1   //communication with destination, administratively prohibited
#  define ICMPV6_CODE_BEYOND_SCOPE       2   //beyond scope of source address
#  define ICMPV6_CODE_ADDR_UNREACH       3   //address unreachable
#  define ICMPV6_CODE_PORT_UNREACH       4   //port unreachable
#  define ICMPV6_CODE_SRC_ADDR_FAILED    5   //source address failed ingress/egress policy
#  define ICMPV6_CODE_REJECT_ROUTE       6   //reject route to destination
#  define ICMPV6_CODE_SRC_HEADER_ERR     7   //Error in Source Routing Header


#define ICMPV6_TYPE_PACKET_TOO_BIG       2   //Packet Too Big
#  define ICMPV6_CODE_PACKET_TOO_BIG     0

#define ICMPV6_TYPE_TIME_EXCEEDED        3   //Time Exceeded
#  define ICMPV6_CODE_HOP_LIMIT          0   //hop limit exceeded in transit
#  define ICMPV6_CODE_FRAG_TIME_EXPIRED  1   //fragment reassembly time exceeded

#define ICMPV6_TYPE_PARAM_PROBLEM        4   //Parameter Problem
#  define ICMPV6_CODE_HEADER_ERR         0   //erroneous header field encountered
#  define ICMPV6_CODE_NEXT_HEADER_ERR    1   //unrecognized Next Header type encountered
#  define ICMPV6_CODE_IPV6_OPT_ERR       2   //unrecognized IPv6 option encountered


#define ICMPV6_TYPE_ECHO_REQUEST         128
#  define ICMPV6_CODE_ECHO_REQUEST       0

#define ICMPV6_TYPE_ECHO_REPLY           129
#  define ICMPV6_CODE_ECHO_REPLY         0

#define ICMPV6_TYPE_ROUTER_SOLIC         133
#  define ICMPV6_CODE_ROUTER_SOLIC       0

#define ICMPV6_TYPE_ROUTER_AD            134
#  define ICMPV6_CODE_ROUTER_AD          0

// COMPLETE THE LIST HERE
//#define ICMPV6_TYPE_
//#  define ICMPV6_CODE_

#define ICMPV6_HDR_LEN 8

typedef struct __icmpv6_hdr {
   uint8_t type;         /* ICMP type */
   uint8_t code;         /* Subtype to the given type */
   uint16_t checksum;    /* Error checking data. Calculated from the ICMP header+data,
                          * with value 0 for this field. The algorithm is the same as the header checksum for IPv4. */
   uint32_t body;
} icmpv6_hdr_t;

void fprint_icmpv6_hdr(FILE *, icmpv6_hdr_t *);
__inline void print_icmpv6_hdr(icmpv6_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif
