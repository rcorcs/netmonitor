#ifndef __NET_HOST
#define __NET_HOST

#include "packet.h"
#include "../datatypes/str.h"

typedef struct __ipv4_addr_node {
    struct __ipv4_addr_node     * next;
    ipv4_addr_t                   addr;
    ipv4_addr_t                   mask;
} ipv4_addr_node_t;

typedef struct __ipv6_addr_node {
    struct __ipv6_addr_node     * next;
    ipv6_addr_t                   addr;
    ipv6_addr_t                   mask;
} ipv6_addr_node_t;

#define LTYPE_UNKNOWN    0
#define LTYPE_ETHERNET   1
#define LTYPE_PPP        2
#define LTYPE_LOOPBACK   3
#define LTYPE_TOKENRING  4
#define LTYPE_FDDI       5
#define LTYPE_SLIP       6

typedef struct __net_host {
   struct __net_host    * next;
   str_t                  name;
   str_t                  desc;
   uint_t                 ltype; //Linux: (ifconfig) Link encap; Windows: Type; ->  link connection type: ethernet, ppp
   eth_addr_t             hwaddr;
   ipv4_addr_node_t     * ipv4_node;
   ipv6_addr_node_t     * ipv6_node;
} net_host_t;


net_host_t *get_net_host_by_name(net_host_t *nethost, const char *if_name);

bool_t get_hw_addr(const char *interface_name, eth_addr_t *hwaddr );

net_host_t *get_host_info();

void destroy_net_host(net_host_t **nethost);

void fprint_net_host(FILE *out, net_host_t *nethost);
__inline void print_net_host(net_host_t *nethost);

#endif
