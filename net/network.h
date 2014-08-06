#ifndef __NETWORK
#define __NETWORK

#ifdef __cplusplus
extern "C" {
#endif

#include <pcap.h>

#ifdef __WIN32
#include <winsock2.h>       //Provides declarations for in_addr structure
#else
#include <netinet/in.h>       //Provides declarations for in_addr structure
#include <arpa/inet.h>
#endif

#include "protocols/arp.h"
#include "protocols/dns.h"
#include "protocols/eth.h"
#include "protocols/http.h"
#include "protocols/ipv6.h"
#include "protocols/icmpv4.h"
#include "protocols/icmpv6.h"
#include "protocols/ipv4.h"
#include "protocols/ipv6.h"
#include "protocols/ppp.h"
#include "protocols/udp.h"
#include "protocols/vlan8021q.h"
#include "protocols/tcp.h"

#include "packet.h"

//TEMPORARY
pcap_if_t *get_all_devices();

//TEMPORARY
void list_all_devices();

//TEMPORARY
void device_info();


#ifdef __cplusplus
}
#endif

#endif

