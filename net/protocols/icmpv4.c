/*
 * cicmphdr.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "icmpv4.h"

void fprint_icmpv4_hdr(FILE *out, icmpv4_hdr_t *icmph)
{
   fprintf(out, "[Internet Control Message Protocol: %hu]\n", ICMPV4_HDR_LEN);
   fprintf(out, "Type               : 0x%x        : %hu\n", icmph->type, icmph->type);
   fprintf(out, "Code               : 0x%x        : %hu\n", icmph->code, icmph->code);
}

__inline void print_icmpv4_hdr(icmpv4_hdr_t *icmph)
{
   fprint_icmpv4_hdr(stdout, icmph);
}
