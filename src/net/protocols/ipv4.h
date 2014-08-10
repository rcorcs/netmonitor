/*
 * ciphdr.h
 *            Global definitions for the Internet Protocol version 4.
 * version:   1.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __IPV4
#define __IPV4

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

#define IP_PROTO_HOPOPT       0x00   /* IPv6 Hop-by-Hop Option - RFC 2460*/
#define IP_PROTO_ICMP         0x01   /* Internet Control Message Protocol - RFC 792 */
#define IP_PROTO_IGMP         0x02   /* Internet Group Management Protocol - RFC 1112 */
#define IP_PROTO_GGP          0x03   /* Gateway-to-Gateway Protocol - RFC 823 */
#define IP_PROTO_IP           0x04   /* IP in IP (encapsulation) - RFC 2003*/
#define IP_PROTO_ST           0x05   /* Internet Stream Protocol - RFC 1190, RFC 1819 */
#define IP_PROTO_TCP          0x06   /* Transmission Control Protocol - RFC 793*/
#define IP_PROTO_CBT          0x07   /* Core-based trees - RFC 2189 */
#define IP_PROTO_EGP          0x08   /* Exterior Gateway Protocol - RFC 888 */
#define IP_PROTO_IGP          0x09   /* Interior Gateway Protocol (any private interior gateway
                                   *(used by Cisco for their IGRP)) */
#define IP_PROTO_BBN_RCC_MON  0x0A   /* BBN RCC Monitoring*/
#define IP_PROTO_NVP          0x0B   /* Network Voice Protocol - RFC 741 */
#define IP_PROTO_PUP          0x0C   /* Xerox PUP */
#define IP_PROTO_ARGUS        0x0D   /* ARGUS */
#define IP_PROTO_EMCON        0x0E   /* EMCON */
#define IP_PROTO_XNET         0x0F   /* Cross Net Debugger - IEN 158 */
#define IP_PROTO_CHAOS        0x10   /* Chaos */
#define IP_PROTO_UDP          0x11   /* User Datagram Protocol - RFC 768 */
#define IP_PROTO_MUX          0x12   /* Multiplexing - IEN 90 */
#define IP_PROTO_DCN_MEAS     0x13   /* DCN Measurement Subsystems */
#define IP_PROTO_HMP          0x14   /* Host Monitoring Protocol - RFC 869 */
#define IP_PROTO_PRM          0x15   /* Packet Radio Measurement */
#define IP_PROTO_XNS_IDP      0x16   /* XEROX NS IDP */
#define IP_PROTO_TRUNK1       0x17   /* Trunk-1 */
#define IP_PROTO_TRUNK2       0x18   /* Trunk-2 */
#define IP_PROTO_LEAF1        0x19   /* Leaf-1 */
#define IP_PROTO_LEAF2        0x1A   /* Leaf-2 */
#define IP_PROTO_RDP          0x1B   /* Reliable Datagram Protocol - RFC 908 */
#define IP_PROTO_IRTP         0x1C   /* Internet Reliable Transaction Protocol - RFC 938 */
#define IP_PROTO_ISO_TP4      0x1D   /* ISO Transport Protocol Class 4 - RFC 905 */
#define IP_PROTO_NETBLT       0x1E   /* Bulk Data Transfer Protocol - RFC 998 */
#define IP_PROTO_MFE_NSP      0x1F   /* MFE Network Services Protocol */
#define IP_PROTO_MERIT_INP    0x20   /* MERIT Internodal Protocol */
#define IP_PROTO_DCCP         0x21   /* Datagram Congestion Control Protocol - RFC 4340 */
#define IP_PROTO_3PC          0x22   /* Third Party Connect Protocol */
#define IP_PROTO_IDPR         0x23   /* Inter-Domain Policy Routing Protocol - RFC 1479 */
#define IP_PROTO_XTP          0x24   /* Xpress Transport Protocol */
#define IP_PROTO_DDP          0x25   /* Datagram Delivery Protocol */
#define IP_PROTO_IDPR_CMTP    0x26   /* IDPR Control Message Transport Protocol */
#define IP_PROTO_TPPP         0x27   /* TP++ Transport Protocol */
#define IP_PROTO_IL           0x28   /* IL Transport Protocol */
#define IP_PROTO_IPV6         0x29   /* IPv6 (encapsulation) - RFC 2473, RFC 3056 */
#define IP_PROTO_SDRP         0x2A   /* Source Demand Routing Protocol - RFC 1940 */
#define IP_PROTO_IPV6_ROUTE   0x2B   /* Routing Header for IPv6 - RFC 2460 */
#define IP_PROTO_IPV6_FRAG    0x2C   /* Fragment Header for IPv6 - RFC 2460 */
#define IP_PROTO_IDRP         0x2D   /* Inter-Domain Routing Protocol */
#define IP_PROTO_RSVP         0x2E   /* Resource Reservation Protocol - RFC 2205 */
#define IP_PROTO_GRE          0x2F   /* Generic Routing Encapsulation */
#define IP_PROTO_MHRP         0x30   /* Mobile Host Routing Protocol */
#define IP_PROTO_BNA          0x31   /* BNA */
#define IP_PROTO_ESP          0x32   /* Encapsulating Security Payload - RFC 2406 */
#define IP_PROTO_AH           0x33   /* Authentication Header - RFC 2402 */
#define IP_PROTO_INLSP        0x34   /* Integrated Net Layer Security Protocol - TUBA */
#define IP_PROTO_SWIPE        0x35   /* SwIPe - IP with Encryption */
#define IP_PROTO_NARP         0x36   /* NBMA Address Resolution Protocol - RFC 1735 */
#define IP_PROTO_MOBILE       0x37   /* IP Mobility (Min Encap) - RFC 2004 */
#define IP_PROTO_TLSP         0x38   /* Transport Layer Security Protocol (using Kryptonet key management) */
#define IP_PROTO_SKIP         0x39   /* Simple Key-Management for Internet Protocol - RFC 2356 */
#define IP_PROTO_IPV6_ICMP    0x3A   /* ICMP for IPv6 - RFC 4443, RFC 4884 */
#define IP_PROTO_IPV6_NO_NXT  0x3B   /* No Next Header for IPv6	- RFC 2460 */
#define IP_PROTO_IPV6_OPTS    0x3C   /* Destination Options for IPv6 - RFC 2460 */
#define IP_PROTO_ANY_INTERNAL 0x3D   /* Any host internal protocol */
#define IP_PROTO_CFTP         0x3E   /* CFTP */
#define IP_PROTO_ANY_LOCAL_NET 0x3F   /* Any local network */
#define IP_PROTO_SAT_EXPAK    0x40   /* SATNET and Backroom EXPAK */
#define IP_PROTO_KRYPTOLAN    0x41   /* Kryptolan */
#define IP_PROTO_RVD          0x42   /* MIT Remote Virtual Disk Protocol */
#define IP_PROTO_IPPC         0x43   /* Internet Pluribus Packet Core */
#define IP_PROTO_ANY_FILE_SYS 0x44   /* Any distributed file system */
#define IP_PROTO_SAT_MON      0x45   /* SATNET Monitoring */
#define IP_PROTO_VISA         0x46   /* VISA Protocol */
#define IP_PROTO_IPCV         0x47   /* Internet Packet Core Utility */
#define IP_PROTO_CPNX         0x48   /* Computer Protocol Network Executive */
#define IP_PROTO_CPHB         0x49   /* Computer Protocol Heart Beat */
#define IP_PROTO_WSN          0x4A   /* Wang Span Network */
#define IP_PROTO_PVP          0x4B   /* Packet Video Protocol */
#define IP_PROTO_BR_SAT_MON   0x4C   /* Backroom SATNET Monitoring */
#define IP_PROTO_SUN_ND       0x4D   /* SUN ND PROTOCOL-Temporary */
#define IP_PROTO_WB_MON       0x4E   /* WIDEBAND Monitoring */
#define IP_PROTO_WB_EXPAK     0x4F   /* WIDEBAND EXPAK */
#define IP_PROTO_ISO_IP       0x50   /* International Organization for Standardization Internet Protocol */
#define IP_PROTO_VMTP         0x51   /* Versatile Message Transaction Protocol - RFC 1045 */
//...
#define IP_PROTO_MANET        0x8A   /* MANET Protocols - RFC 5498 */
#define IP_PROTO_HIP          0x8B   /* Host Identity Protocol - RFC 5201 */
#define IP_PROTO_SHIM6        0x8C   /* Site Multihoming by IPv6 Intermediation - RFC 5533 */

//TODO complete up to 0x8C

/*
 * Definitions for IP type of service (tos)
 */
#define	IP_TOS_LOWDELAY		0x10
#define	IP_TOS_THROUGHPUT	    0x08
#define	IP_TOS_RELIABILITY	    0x04
#define	IP_TOS_MINCOST		    0x02
/* ECN bits */
#define	IP_TOS_CE		        0x01	/* congestion experienced */
#define	IP_TOS_ECT		        0x02	/* ECN-capable transport */

/*
 * Internet implementation parameters.
 */
#define	IP_TTL_MAX		255     /* maximum time to live (seconds) */
#define	IP_TTL_DEFAULT 64      /* default ttl, from RFC 1340 */
#define	IP_TTL_FRAGS   60      /* time to live for frags, slowhz */
#define	IP_TTL_DEC     1       /* subtracted when forwarding */

#define	IP_MAX_SIZE_SEGMENT  576   /* default maximum segment size */
#define	IP_MSS               576   /* default maximum segment size */

#define IP_VERSION 4
#define IP_MAXSIZE 65535   /* maximum packet size */


#define IPV4_ADDR_LEN 4
typedef struct __ipv4_addr {
   uint8_t addr[IPV4_ADDR_LEN];
} ipv4_addr_t;

const char *ipv4_addr_to_c_str(ipv4_addr_t *ipv4);
str_t *ipv4_addr_to_str(ipv4_addr_t *ipv4);

void c_str_to_ipv4_addr(ipv4_addr_t *ipv4, const char *str);

#define IPV4_HDR_LEN(_IPH_) (4*(_IPH_)->hl)
typedef struct __ipv4_hdr {
#if ( defined BYTE_ORDER && (defined LITTLE_ENDIAN || defined BIG_ENDIAN) )
#if BYTE_ORDER == LITTLE_ENDIAN
   uint8_t         hl:4,        // header length
                    v:4;         // version
#elif BYTE_ORDER == BIG_ENDIAN
   uint8_t         v:4,         // version
                    hl:4;        // header length
#endif //endif BYTE_ORDER
#else //default: intel x86 and x86-64 use little-endian format
   uint8_t         hl:4,        // header length
                    v:4;         // version
#endif //endif defined BYTE_ORDER
   uint8_t         tos;         // type of service
   uint16_t        len;         // length
   uint16_t        id;          // id
/*
   uint16_t        resbit:1,    // reserved bit (flags)
                   dfbit:1,     // DF bit (Don't Fragment bit) (flags)
				   mfbit:1,     // MF bit (More Fragment bit) (flags)
                   foffset:13;   // Fragment Offset
*/
   uint16_t        foffset;     // Flags and Fragment Offset
   uint8_t         ttl;         // Time to Live
   uint8_t         proto;       // protocol
   uint16_t        checksum;    // checksum
   ipv4_addr_t      src;         // source address
   ipv4_addr_t      dst;         // destination address
} ipv4_hdr_t;

void fprint_ipv4_hdr(FILE *, ipv4_hdr_t *);
__inline void print_ipv4_hdr(ipv4_hdr_t *);

/* IPsec : IP Security Architecture */

/* Authentication Header - Both for IPv4 and IPv6 */
typedef struct __ip_auth_hdr {
   uint8_t         nexthdr;
   uint8_t         ploadlen;
   uint16_t        reserved;
   uint32_t        spi;
   uint32_t        seqn;
   byte_t         *icv;
} ip_auth_hdr_t;

#ifdef __cplusplus
}
#endif

#endif
