#ifndef __ICMP_STATS
#define __ICMP_STATS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../../util.h"

typedef struct __icmp_record {
   uint_t total;    //total of UDP packets analysed
   uint_t techoreply;
   uint_t techoreq;
} icmp_record_t;

void fprint_icmp_record(FILE *out, icmp_record_t *icmprecord);
__inline void print_icmp_record(icmp_record_t *icmprecord);

void store_icmp_record(FILE *out, icmp_record_t *icmprecord);
void load_icmp_record(FILE *out, icmp_record_t *icmprecord);

typedef struct __icmp_stats {
   uint_t        tticks;
   icmp_record_t in;
   icmp_record_t out;
} icmp_stats_t;

void fprint_icmp_stats(FILE *out, icmp_stats_t *icmpstats);
__inline void print_icmp_stats(icmp_stats_t *icmpstats);

void store_icmp_stats(FILE *out, icmp_stats_t *icmpstats);
void load_icmp_stats(FILE *out, icmp_stats_t *icmpstats);

void fprint_icmp_record_avg_per_instants(FILE *out, icmp_record_t *icmprecord, ulong_t intants);
__inline void print_icmp_record_avg_per_instants(icmp_record_t *icmprecord, ulong_t intants);
void fprint_icmp_stats_avg_per_instants(FILE *out, icmp_stats_t *icmpstats);
__inline void print_icmp_stats_avg_per_instants(icmp_stats_t *icmpstats);
#ifdef __cplusplus
}
#endif

#endif


