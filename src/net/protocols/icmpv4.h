/*
 * net/icmp.h
 *            Global definitions for the Internet Control Message Protocol.
 * version:   2.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
 * Changes:
 *   12 September 2011   changed pattern of names
*/
#ifndef __ICMPV4
#define __ICMPV4

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"


#define ICMPV4_TYPE_ECHO_REPLY           0  //Echo reply (used to ping)
#  define ICMPV4_CODE_ECHO_REPLY         0  //Echo reply (used to ping)

#define ICMPV4_TYPE_DST_UNREACH          3  //Destination Unreachable

#define ICMPV4_TYPE_SRC_QUENCH           4  //Source Quench
#  define ICMPV4_CODE_SRC_QUENCH         4  //Source quench (congestion control)

#define ICMPV4_TYPE_REDIRECT_MSG         5  //Redirect Message

#define ICMPV4_TYPE_ECHO_REQUEST         8  //Echo Request
#  define ICMPV4_CODE_ECHO_REQUEST       0  //Echo Request

#define ICMPV4_TYPE_ROUTER_AD            9  //Router Advertisement
#  define ICMPV4_CODE_ROUTER_AD          0	 //Router Advertisement

#define ICMPV4_TYPE_ROUTER_SOLIC         10 //Router Solicitation
#  define ICMPV4_CODE_ROUTER_SOLIC       0  //Router discovery/selection/solicitation
#  define ICMPV4_CODE_ROUTER_SELECT      0  //Router discovery/selection/solicitation

#define ICMPV4_TYPE_TIME_EXCEEDED        11 //Time Exceeded
#  define ICMPV4_CODE_TTL_EXPIRED        0  //TTL expired in transit
#  define ICMPV4_CODE_FRAG_TIME_EXPIRED  1  //Fragment reassembly time exceeded

#define ICMPV4_TYPE_BAD_IP_HEADER        12 //Parameter Problem: Bad IP header
#  define ICMPV4_CODE_POINTER_ERR        0  //Pointer indicates the error
#  define ICMPV4_CODE_MISSING_OPT        1  //Missing a required option
#  define ICMPV4_CODE_BAD_LENGTH         2  //Bad length

#define ICMPV4_TYPE_TIMESTAMP            13 //Timestamp
#  define ICMPV4_CODE_TIMESTAMP          0  //Timestamp

#define ICMPV4_TYPE_TIMESTAMP_REPLY      14 //Timestamp reply
#  define ICMPV4_CODE_TIMESTAMP_REPLY    0  //Timestamp reply

#define ICMPV4_TYPE_INFO_REQUEST         15  //Information Request
#  define ICMPV4_CODE_INFO_REQUEST       0   //Information Request

#define ICMPV4_TYPE_INFO_REPLY           16  //Information Reply
#  define ICMPV4_CODE_INFO_REPLY         0   //Information Reply

#define ICMPV4_TYPE_ADDR_MASK_REQUEST    17  //Address Mask Request
#  define ICMPV4_CODE_ADDR_MASK_REQUEST  0   //Address Mask Request

#define ICMPV4_TYPE_ADDR_MASK_REPLY      18  //Address Mask Reply
#  define ICMPV4_CODE_ADDR_MASK_REPLY    0   //Address Mask Reply

#define ICMPV4_TYPE_TRACEROUTE           30  //Traceroute

#define ICMPV4_HDR_LEN 8

typedef struct __icmpv4_hdr {
   uint8_t type;         /* ICMP type */
   uint8_t code;         /* Subtype to the given type */
   uint16_t checksum;    /* Error checking data. Calculated from the ICMP header+data,
                          * with value 0 for this field. The algorithm is the same as the header checksum for IPv4. */
   uint32_t rest;
} icmpv4_hdr_t;

void fprint_icmpv4_hdr(FILE *, icmpv4_hdr_t *);
__inline void print_icmpv4_hdr(icmpv4_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif
