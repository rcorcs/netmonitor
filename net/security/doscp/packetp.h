#ifndef __PACKET_STATS
#define __PACKET_STATS

#ifdef __cplusplus
extern "C" {
#endif

#include "../../util.h"

#include "../../../datatypes/primitives.h"
#include "../../../datatypes/hashtab.h"
#include "../../../datatypes/list.h"
#include "../../../datatypes/node.h"
#include "../../../time/time.h"

#include "../../packet.h"

#include "icmpp.h"
#include "tcpp.h"
#include "udpp.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __packetp {
    hash_tab_t *tcp_htab;
    hash_tab_t *udp_htab;
    hash_tab_t *icmp_htab;
} packetp_t;

packetp_t *create_packetp(uint_t len);
void destroy_packetp(packetp_t **pktp);

void clear_packetp(packetp_t *pktp);


void packetp_add_out_info(packetp_t *pktp, packet_t *pkt);
void packetp_add_in_info(packetp_t *pktp, packet_t *pkt);

void store_tcpp_file(FILE *out, packetp_t *pktp, struct tm *time);
void store_udpp_file(FILE *out, packetp_t *pktp, struct tm *time);
//void store_packetp(FILE *tcp_out, FILE *udp_out, FILE *icmp_out, packetp_t *pktp, struct tm *time);
//void load_packetp(FILE *tcp_out, FILE *udp_out, FILE *icmp_out, packetp_t *pktp);

void load_tcpp_file(FILE *tcp_in, list_t *list);
void load_udpp_file(FILE *tcp_in, list_t *list);

void fprint_tcpp_file(FILE *out, FILE *tcp_in);
void fprint_udpp_file(FILE *out, FILE *tcp_in);

void fprint_packetp(FILE *out, packetp_t *pktp);
__inline void print_packetp(packetp_t *pktp);

/*
void fprint_packet_stats_avg_per_instants(FILE *out, packet_stats_t *pktstats, ulong_t intants);
__inline void print_packet_stats_avg_per_instants(packet_stats_t *pktstats, ulong_t intants);

void process_tcp_max_min(hash_tab_t *htab, tcp_stats_t * tcpmax, tcp_stats_t * tcpmin);
void process_udp_max_min(hash_tab_t *htab, udp_stats_t * udpmax, udp_stats_t * udpmin);
void process_icmp_max_min(hash_tab_t *htab, icmp_stats_t * icmpmax, icmp_stats_t * icmpmin);
*/

#ifdef __cplusplus
}
#endif

#endif


