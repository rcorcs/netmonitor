/*
 * cudphdr.h
 *            Global definitions for the User Datagram Protocol.
 * version:   1.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __UDP
#define __UDP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#define UDP_HDR_LEN 8

typedef uint16_t udp_port_t;

typedef struct __udp_hdr {
	udp_port_t    src_port;       /* source port */
	udp_port_t    dst_port;       /* destination port */
	uint16_t      len;            /* udp length */
	uint16_t      checksum;       /* udp checksum */
} udp_hdr_t;

void fprint_udp_hdr(FILE *, udp_hdr_t *);
__inline void print_udp_hdr(udp_hdr_t *);


#ifdef __cplusplus
}
#endif

#endif
