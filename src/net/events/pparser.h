#ifndef __PPARSER
#define __PPARSER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "../network.h"
#include "../util.h"
#include "ehandlers.h"

void parser_init();
void parse_packet(packet_t *, const struct pcap_pkthdr *);
void parser_close();

#ifdef __cplusplus
}
#endif

#endif
