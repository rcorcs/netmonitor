#include "icmpp.h"

#ifdef __cplusplus
extern "C" {
#endif


void fprint_icmp_record(FILE *out, icmp_record_t *icmprecord)
{
  fprintf(out, "Total:          %u\n", icmprecord->total);
  fprintf(out, "ECHO REPLY:     %u\n", icmprecord->techoreply);
  fprintf(out, "ECHO REQUEST:   %u\n", icmprecord->techoreq);
}

__inline void print_icmp_record(icmp_record_t *icmprecord)
{
   fprint_icmp_record(stdout, icmprecord);
}

void store_icmp_record(FILE *out, icmp_record_t *icmprecord)
{
    fwrite(&icmprecord->total, sizeof(icmprecord->total), 1, out);
    fwrite(&icmprecord->techoreply, sizeof(icmprecord->techoreply), 1, out);
    fwrite(&icmprecord->techoreq, sizeof(icmprecord->techoreq), 1, out);
}

void load_icmp_record(FILE *out, icmp_record_t *icmprecord)
{
    fread(&icmprecord->total, sizeof(icmprecord->total), 1, out);
    fread(&icmprecord->techoreply, sizeof(icmprecord->techoreply), 1, out);
    fread(&icmprecord->techoreq, sizeof(icmprecord->techoreq), 1, out);
}

void fprint_icmp_stats(FILE *out, icmp_stats_t *icmpstats)
{
   fprintf(out, "Total Ticks: %u\n", icmpstats->tticks);
   fprintf(out, "[Incoming Packets]\n");
   fprint_icmp_record(out, &icmpstats->in);
   fprintf(out, "[Outgoing Packets]\n");
   fprint_icmp_record(out, &icmpstats->out);
}

__inline void print_icmp_stats(icmp_stats_t *icmpstats)
{
   fprint_icmp_stats(stdout, icmpstats);
}

void store_icmp_stats(FILE *out, icmp_stats_t *icmpstats)
{
    fwrite(&icmpstats->tticks, sizeof(uint_t), 1, out);
    store_icmp_record(out, &icmpstats->in);
    store_icmp_record(out, &icmpstats->out);
}

void load_icmp_stats(FILE *in, icmp_stats_t *icmpstats)
{
    fread(&icmpstats->tticks, sizeof(uint_t), 1, in);
    load_icmp_record(in, &icmpstats->in);
    load_icmp_record(in, &icmpstats->out);
}

void fprint_icmp_record_avg_per_instants(FILE *out, icmp_record_t *icmprecord, ulong_t intants)
{
  fprintf(out, "Total:          %f\n", ((float)icmprecord->total)/intants);
  fprintf(out, "ECHO REPLY:     %f\n", ((float)icmprecord->techoreply)/intants);
  fprintf(out, "ECHO REQUEST:   %f\n", ((float)icmprecord->techoreq)/intants);
}

__inline void print_icmp_record_avg_per_instants(icmp_record_t *icmprecord, ulong_t intants)
{
   fprint_icmp_record_avg_per_instants(stdout, icmprecord, intants);
}

void fprint_icmp_stats_avg_per_instants(FILE *out, icmp_stats_t *icmpstats)
{
   fprintf(out, "Total Ticks: %u\n", icmpstats->tticks);
   fprintf(out, "[Incoming Packets]\n");
   fprint_icmp_record_avg_per_instants(out, &icmpstats->in, icmpstats->tticks);
   fprintf(out, "[Outgoing Packets]\n");
   fprint_icmp_record_avg_per_instants(out, &icmpstats->out, icmpstats->tticks);
}

__inline void print_icmp_stats_avg_per_instants(icmp_stats_t *icmpstats)
{
   fprint_icmp_stats_avg_per_instants(stdout, icmpstats);
}

#ifdef __cplusplus
}
#endif
