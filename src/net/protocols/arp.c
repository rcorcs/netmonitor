#include "arp.h"

void fprint_arp_hdr(FILE *out, arp_hdr_t *arph)
{
    int i;
    fprintf(out, "[Address Resolution Protocol: %hu]\n", ARP_HDR_LEN);
    fprintf(out, "Hardware Type      : 0x%04x\n", ntohs(arph->htype));
    fprintf(out, "Protocol Type      : 0x%04x\n", ntohs(arph->proto));
    fprintf(out, "Hardware Length    : 0x%02x       : %hu\n", arph->hlen, arph->hlen);
    fprintf(out, "Protocol Length    : 0x%02x       : %hu\n", arph->plen, arph->plen);
    fprintf(out, "Opcode             : 0x%04x     : %hu\n", ntohs(arph->opcode), ntohs(arph->opcode));

    fprintf(out, "Source Ethenet     : %02x", arph->src_ethh.addr[0]);
    for(i=1; i<ETH_ADDR_LEN; i++)
       fprintf(out, ":%02x", arph->src_ethh.addr[i]);

    fprintf(out, "\nSource IP          : 0x%02x%02x%02x%02x : %s\n", arph->src_iph.addr[0], arph->src_iph.addr[1], arph->src_iph.addr[2], arph->src_iph.addr[3], ipv4_addr_to_c_str(&arph->src_iph));

    fprintf(out, "Destination Eth.   : %02x", arph->dst_ethh.addr[0]);
    for(i=1; i<ETH_ADDR_LEN; i++)
       fprintf(out, ":%02x", arph->dst_ethh.addr[i]);

    fprintf(out, "\nDestination IP     : 0x%02x%02x%02x%02x : %s\n", arph->dst_iph.addr[0], arph->dst_iph.addr[1], arph->dst_iph.addr[2], arph->dst_iph.addr[3], ipv4_addr_to_c_str(&arph->dst_iph));
}

__inline void print_arp_hdr(arp_hdr_t *arph)
{
    fprint_arp_hdr(stdout, arph);
}

