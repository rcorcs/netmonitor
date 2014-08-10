
#include "vlan8021q.h"

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

void fprint_vlan8021q_hdr(FILE *out, vlan8021q_hdr_t *vlanh)
{
   fprintf(out, "[IEEE 802.1Q: %hu]\n", VLAN8021Q_HDR_LEN);
   fprintf(out, "TPID:          0x%04x\n", ntohs(vlanh->tpid));
   fprintf(out, "TCI:           0x%04x\n", ntohs(vlanh->tci));
   fprintf(out, "TYPE:          0x%04x\n", ntohs(vlanh->type));
}

__inline void print_vlan8021q_hdr(vlan8021q_hdr_t *vlanh)
{
   fprint_vlan8021q_hdr(stdout, vlanh);
}
