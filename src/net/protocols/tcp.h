/*
 * ctcphdr.h
 *            Global definitions for the Transmission Control Protocol.
 * version:   1.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#ifndef __TCP
#define __TCP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#define TCP_HDR_LEN(_TCPH_) (4*(_TCPH_)->doffset)

typedef uint16_t tcp_port_t;

typedef struct __tcp_hdr {
        tcp_port_t  src_port;    //source port
        tcp_port_t  dst_port;    //destination port
        uint32_t    seq;         //sequence number
		uint32_t    ack;         //acknowledge number
#if BYTE_ORDER == LITTLE_ENDIAN
        uint16_t    res1:4,      //reserved (first part)
                    doffset:4,   //data offset
                    finbit:1,    //fin bit (flag)
                    synbit:1,    //syn bit (flag)
                    rstbit:1,    //rst bit (flag)
                    pshbit:1,    //push bit (flag)
                    ackbit:1,    //ack bit (flag)
                    urgbit:1,    //urg bit (flag)
                    res2:2;      //reserver (second part)
#elif BYTE_ORDER == BIG_ENDIAN
        uint16_t    doffset:4,   //data offset
                    res1:4,      //reserver (first part)
                    res2:2,      //reserver (second part)
                    urgbit:1,    //urg bit (flag)
                    ackbit:1,    //ack bit (flag)
                    pshbit:1,    //push bit (flag)
                    rstbit:1,    //rst bit (flag)
                    synbit:1,    //syn bit (flag)
                    finbit:1;    //fin bit (flag)
#endif
        uint16_t    win_size;       //window size
        uint16_t    checksum;       //checksum - used for error-checking of the header and data
        uint16_t    urg_ptr;        //urgent pointer
} tcp_hdr_t;

void fprint_tcp_hdr(FILE *, tcp_hdr_t *);
__inline void print_tcp_hdr(tcp_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif
