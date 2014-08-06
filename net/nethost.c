#include "nethost.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __WIN32

#include <time.h>
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>

//#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
//#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
// Note: could also use malloc() and free()

net_host_t *get_host_info()
{
    net_host_t *nethost = NULL, *nethost_it = NULL;
    ipv4_addr_node_t * ipv4_it = NULL;
    // Declare and initialize variables

// It is possible for an adapter to have multiple
// IPv4 addresses, gateways, and secondary WINS servers
// assigned to the adapter.
//
// Note that this sample code only prints out the
// first entry for the IP address/mask, and gateway, and
// the primary and secondary WINS server for each adapter.

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    PIP_ADDR_STRING pAddrString = NULL;
    DWORD dwRetVal = 0;

    // variables used to print DHCP time info
//    struct tm newtime;
//    char buffer[32];
//    int error;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        //printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return NULL;
    }
// Make an initial call to GetAdaptersInfo to get
// the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) malloc(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            //printf("Error allocating memory needed to call GetAdaptersinfo\n");
            return NULL;
        }
    }

    //nethost = (net_host_t*)malloc(sizeof(net_host_t));


    nethost = NULL;
    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            if(nethost==NULL){
                nethost = (net_host_t*)malloc(sizeof(net_host_t));
                nethost_it = nethost;
            }else {
                nethost_it->next = (net_host_t*)malloc(sizeof(net_host_t));
                nethost_it = nethost_it->next;
            }
            nethost_it->ipv4_node = NULL;
            nethost_it->ipv4_node = NULL;
            nethost_it->ipv6_node = NULL;
            nethost_it->next = NULL;

            //printf("\tComboIndex: \t%d\n", (int)pAdapter->ComboIndex);
            //printf("\tAdapter Name: \t%s\n", (char*)pAdapter->AdapterName);
            nethost->name.len = strlen((char*)pAdapter->AdapterName);
            //nethost->name.str = (char*)malloc(nethost->name.len);
            nethost->name.str = (char*)malloc(nethost->name.len+1);
            strcpy(nethost->name.str, pAdapter->AdapterName);

            //printf("\tAdapter Desc: \t%s\n", (char*)pAdapter->Description);
            nethost->desc.len = strlen((char*)pAdapter->Description);
            //nethost->desc.str = (char*)malloc(nethost->desc.len);
            nethost->desc.str = (char*)malloc(nethost->desc.len+1);
            strcpy(nethost->desc.str, pAdapter->Description);

            memcpy(nethost->hwaddr.addr, pAdapter->Address, ETH_ADDR_LEN);

            switch (pAdapter->Type) {
            case MIB_IF_TYPE_ETHERNET:
                nethost->ltype = LTYPE_ETHERNET;
                break;
            case MIB_IF_TYPE_TOKENRING:
                nethost->ltype = LTYPE_TOKENRING;
                break;
            case MIB_IF_TYPE_FDDI:
                nethost->ltype = LTYPE_FDDI;
                break;
            case MIB_IF_TYPE_PPP:
                nethost->ltype = LTYPE_PPP;
                break;
            case MIB_IF_TYPE_LOOPBACK:
                nethost->ltype = LTYPE_LOOPBACK;
                break;
            case MIB_IF_TYPE_SLIP:
                nethost->ltype = LTYPE_SLIP;
                break;
            case MIB_IF_TYPE_OTHER:
            default:
                nethost->ltype = LTYPE_UNKNOWN;
                break;
            }

            for(pAddrString = &pAdapter->IpAddressList; pAddrString!=NULL; pAddrString = pAddrString->Next){
                if(nethost->ipv4_node==NULL){
                    //nethost->ipv4_node = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
                    nethost->ipv4_node = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
                    ipv4_it = nethost->ipv4_node;
                }else{
                    //ipv4_it->next = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
                    ipv4_it->next = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
                    ipv4_it = ipv4_it->next;
                }
                if(ipv4_it){
                    ipv4_it->next = NULL;
                    c_str_to_ipv4_addr(&ipv4_it->addr, pAddrString->IpAddress.String);
                    c_str_to_ipv4_addr(&ipv4_it->mask, pAddrString->IpMask.String);
                }
            }
            //printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);

            //if (pAdapter->DhcpEnabled)
            //   puts(pAdapter->DhcpServer.IpAddress.String);
            pAdapter = pAdapter->Next;
            //printf("\n");
        }
    } else {
        //printf("GetAdaptersInfo failed with error: %d\n", (int)dwRetVal);
    }

    if (pAdapterInfo)
        free(pAdapterInfo);

    return nethost;
}

#else

#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>


bool_t get_hw_addr(const char *interface_name, eth_addr_t *hwaddr )
{
    int sockfd; //socket file descriptor
    struct ifreq if_request; //interface request
    struct if_nameindex *if_name_idx, *if_save_idx; //pointer to the interface name index

    if_name_idx = if_save_idx = (struct if_nameindex *)NULL;

#ifndef SIOCGIFADDR
    // The kernel does not support the required ioctls
    return BOOL_FALSE;
#endif


    // Create a socket that we can use for all of our ioctls
    sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    if(sockfd < 0){
        // Socket creation failed, this is a fatal error
        //printf( "File %s: line %d: Socket failed\n", __FILE__, __LINE__ );
        return BOOL_FALSE;
    }

    // Obtain a list of dynamically allocated structures
    if_name_idx = if_save_idx = if_nameindex();

    // Walk thru the array returned and query for each interface's address
    for ( ; *(char *)if_name_idx != 0; if_name_idx++ ){
        // Determine if we are processing the interface that we are interested in
        if ( strcmp(if_name_idx->if_name, interface_name) ){
            // Nope, check the next one in the list
            continue;
        }else{
            strncpy( if_request.ifr_name, if_name_idx->if_name, IF_NAMESIZE );

            // Get the MAC address for this interface
            if ( ioctl(sockfd, SIOCGIFHWADDR, &if_request) != 0 ){
                // We failed to get the MAC address for the interface
                //printf( "File %s: line %d: Ioctl failed\n", __FILE__, __LINE__ );
                return BOOL_FALSE;
            }
            memmove( (void *)&hwaddr->addr[0], (void *)&if_request.ifr_ifru.ifru_hwaddr.sa_data[0], ETH_ADDR_LEN );
            break;
        }
    }

    // Clean up things and return
    if_freenameindex( if_save_idx );
    close( sockfd );
    return BOOL_TRUE;
}

void set_net_host(net_host_t *nethost, struct ifaddrs *netaddrs)
{
    //nethost->name.len = strlen(netaddrs->ifa_name);
    //nethost->name.str = (char *)malloc(nethost->name.len);
    //strcpy(nethost->name.str, netaddrs->ifa_name);

    //get_hw_addr(netaddrs->ifa_name, &nethost->hwaddr);

    if(netaddrs->ifa_addr->sa_family==AF_INET){
        //nethost->ipv6_node = 0;
        ipv4_addr_node_t *ipv4_it = nethost->ipv4_node;
        if(nethost->ipv4_node){
            while(ipv4_it->next!=NULL){
                ipv4_it = ipv4_it->next;
            }
            ipv4_it->next = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
            ipv4_it = ipv4_it->next;
        }else {
            nethost->ipv4_node = (ipv4_addr_node_t*)malloc(sizeof(ipv4_addr_node_t));
            ipv4_it = nethost->ipv4_node;
        }
        ipv4_it->next = NULL;
        memcpy(ipv4_it->addr.addr,  (void *)&(((struct sockaddr_in *)(netaddrs->ifa_addr))->sin_addr), IPV4_ADDR_LEN);

        if(netaddrs->ifa_netmask && netaddrs->ifa_netmask->sa_family==AF_INET){
            memcpy(ipv4_it->mask.addr,  (void *)&(((struct sockaddr_in *)(netaddrs->ifa_netmask))->sin_addr), IPV4_ADDR_LEN);
        }
    }else if(netaddrs->ifa_addr->sa_family==AF_INET6){
        //nethost->ipv4_node = 0;
        //nethost->ipv6_node = (ipv6_addr_node_t*)malloc(sizeof(ipv6_addr_node_t));
        //nethost->ipv6_node->next = NULL;

        ipv6_addr_node_t *ipv6_it = nethost->ipv6_node;
        if(nethost->ipv6_node){
            while(ipv6_it->next!=NULL){
                ipv6_it = ipv6_it->next;
            }
            ipv6_it->next = (ipv6_addr_node_t*)malloc(sizeof(ipv6_addr_node_t));
            ipv6_it = ipv6_it->next;
        }else {
            nethost->ipv6_node = (ipv6_addr_node_t*)malloc(sizeof(ipv6_addr_node_t));
            ipv6_it = nethost->ipv6_node;
        }
        ipv6_it->next = NULL;

        memcpy(ipv6_it->addr.addr,  (void *)&(((struct sockaddr_in6 *)(netaddrs->ifa_addr))->sin6_addr), IPV6_ADDR_LEN);

        if(netaddrs->ifa_netmask && netaddrs->ifa_netmask->sa_family==AF_INET6){
            memcpy(ipv6_it->mask.addr,  (void *)&(((struct sockaddr_in6 *)(netaddrs->ifa_netmask))->sin6_addr), IPV6_ADDR_LEN);
        }
    }
}


net_host_t *get_host_info()
{
    net_host_t *nethost = NULL, *nethost_it = NULL, *nethost_last = NULL;
    struct ifaddrs *netaddrs, *ifa;
    int status;

    status = getifaddrs(&netaddrs);
    if (status != 0){
        //perror("getifaddrs failed!");
        return NULL;
    }
    //nethost = (net_host_t *)malloc(sizeof(net_host_t));
    //nethost_it = nethost;
    for (ifa = netaddrs; ifa != NULL; ifa = ifa->ifa_next){
        if (ifa->ifa_addr == NULL || (ifa->ifa_flags & IFF_UP) == 0 ){
            continue;
        }
        if(nethost==NULL){
            nethost = (net_host_t *)malloc(sizeof(net_host_t));
            nethost_last = nethost;
            nethost_last->next = NULL;
            nethost_last->desc.len = 0;
            nethost_last->desc.str = NULL;
            nethost_last->ipv4_node = NULL;
            nethost_last->ipv6_node = NULL;

            nethost_last->name.len = strlen(ifa->ifa_name);
            nethost_last->name.str = (char *)malloc(nethost->name.len+1);
            strcpy(nethost_last->name.str, ifa->ifa_name);

            get_hw_addr(ifa->ifa_name, &nethost_last->hwaddr);

            nethost_it = nethost_last;
        }else {
            for(nethost_it = nethost; nethost_it!=NULL; nethost_it = nethost_it->next){
                if(!strcmp(nethost_it->name.str, ifa->ifa_name)){
                    break;
                }
            }
            if(nethost_it==NULL){
                nethost_last = nethost_last->next = (net_host_t *)malloc(sizeof(net_host_t));
                nethost_last->next = NULL;
                nethost_last->desc.len = 0;
                nethost_last->desc.str = NULL;
                nethost_last->ipv4_node = NULL;
                nethost_last->ipv6_node = NULL;

                nethost_last->name.len = strlen(ifa->ifa_name);
                nethost_last->name.str = (char *)malloc(nethost->name.len+1);
                strcpy(nethost_last->name.str, ifa->ifa_name);

                get_hw_addr(ifa->ifa_name, &nethost_last->hwaddr);

                nethost_it = nethost_last;

            }
        }
        set_net_host(nethost_it, ifa);

        if(nethost_it->name.len>=2 && nethost_it->name.str[0]=='l' && nethost_it->name.str[1]=='o'){
            nethost_it->ltype = LTYPE_LOOPBACK;
        }else if(nethost_it->name.len>=3 && nethost_it->name.str[0]=='e' && nethost_it->name.str[1]=='t' && nethost_it->name.str[2]=='h'){
            nethost_it->ltype = LTYPE_ETHERNET;
        }else if(nethost_it->name.len>=3 && nethost_it->name.str[0]=='p' && nethost_it->name.str[1]=='p' && nethost_it->name.str[2]=='p'){
            nethost_it->ltype = LTYPE_PPP;
        }else{
            nethost_it->ltype = LTYPE_UNKNOWN;
        }

        //nethost_it->next = (net_host_t *)malloc(sizeof(net_host_t));
    }
    freeifaddrs(netaddrs);
    //free(nethost_it->next);
    return nethost;
}

#endif

void destroy_net_host(net_host_t **nethost)
{
   net_host_t *nethost_it = (*nethost);
   void *deletable;
   while(nethost_it!=NULL){
      ipv4_addr_node_t *ipv4_it;
      ipv6_addr_node_t *ipv6_it;

      if(nethost_it->name.str) free(nethost_it->name.str);
      if(nethost_it->desc.str) free(nethost_it->desc.str);

      ipv4_it = nethost_it->ipv4_node;
      while(ipv4_it!=NULL){
          deletable = ipv4_it;
          ipv4_it = ipv4_it->next;
          free(deletable);
      }

      ipv6_it = nethost_it->ipv6_node;
      while(ipv6_it!=NULL){
          deletable = ipv6_it;
          ipv6_it = ipv6_it->next;
          free(deletable);
      }

      deletable = nethost_it;
      nethost_it = nethost_it->next;
      free(deletable);
   }

   free(*nethost);
   *nethost = 0;
}

net_host_t *get_net_host_by_name(net_host_t *nethost, const char *if_name)
{
    net_host_t *nethost_it = nethost;
    while(nethost_it!=NULL){
        if( !strcmp(nethost_it->name.str, if_name) ){
           break;
        }
        nethost_it = nethost_it->next;
    }
    return nethost_it;
}

void fprint_net_host(FILE *out, net_host_t *nethost)
{
   net_host_t *nethost_it = nethost;
   while(nethost_it!=NULL){
      uint_t i;
      ipv4_addr_node_t *ipv4_it;
      ipv6_addr_node_t *ipv6_it;
      fprintf(out, "%s\t", nethost_it->name.str);
      #ifdef __WIN32
      fprintf(out, "\n\tType:        ");
      #endif

      switch(nethost_it->ltype){
      case LTYPE_ETHERNET:
        fprintf(out, "Ethernet\n");
        break;
      case LTYPE_PPP:
        fprintf(out, "Point-to-Point Protocol\n");
        break;
      case LTYPE_LOOPBACK:
        fprintf(out, "Local Loopback\n");
        break;
      }
      /*
      if(nethost_it->name.len>=2 && nethost_it->name.str[0]=='l' && nethost_it->name.str[1]=='o'){
          fprintf(out, "Local Loopback\n");
      }else if(nethost_it->name.len>=3 && nethost_it->name.str[0]=='e' && nethost_it->name.str[1]=='t' && nethost_it->name.str[2]=='h'){
          fprintf(out, "Ethernet\n");
      }else if(nethost_it->name.len>=3 && nethost_it->name.str[0]=='p' && nethost_it->name.str[1]=='p' && nethost_it->name.str[2]=='p'){
          fprintf(out, "Point-to-Point Protocol\n");
      }else{
          fprintf(out, "-\n");
      }
      */

      #ifndef __WIN32
      if(nethost_it->desc.str)
         fprintf(out, "\tDesc:        %s\n", nethost_it->desc.str);
      else fprintf(out, "\tDesc:        -\n");
      #endif

      fprintf(out, "\tHWAddr:      %02x", nethost_it->hwaddr.addr[0]);
      for(i=1; i<ETH_ADDR_LEN; i++)
         fprintf(out, ":%02x", nethost_it->hwaddr.addr[i]);
      putc('\n', out);

      for(ipv4_it = nethost_it->ipv4_node; ipv4_it!=NULL; ipv4_it = ipv4_it->next){
         fprintf(out, "\tIPv4 Addr:   %s\n", ipv4_addr_to_c_str(&ipv4_it->addr) );
         fprintf(out, "\tMask:        %s\n", ipv4_addr_to_c_str(&ipv4_it->mask) );
      }

      for(ipv6_it = nethost_it->ipv6_node; ipv6_it!=NULL; ipv6_it = ipv6_it->next){
         fprintf(out, "\tIPv6 Addr:   %s\n", ipv6_addr_to_c_str(&ipv6_it->addr) );
         fprintf(out, "\tMask:        %s\n", ipv6_addr_to_c_str(&ipv6_it->mask) );
      }

      nethost_it = nethost_it->next;
   }
}

__inline void print_net_host(net_host_t *nethost)
{
    fprint_net_host(stdout, nethost);
}
