/*
 * net/eth.h
 *            Global definitions for the Ethernet IEEE 802.3 interface.
 * version:   2.0.0
 * date:      05 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
 * Changes:
 *   12 September 2011   changed pattern of names
*/
#ifndef __ETH
#define __ETH

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

#define ETH_TYPE_LOOP      0x0060          /* Ethernet Loopback packet     */
#define ETH_TYPE_PUP       0x0200          /* Xerox PUP packet             */
#define ETH_TYPE_PUPAT     0x0201          /* Xerox PUP Addr Trans packet  */
#define ETH_TYPE_IP        0x0800          /* Internet Protocol packet     */
#define ETH_TYPE_X25       0x0805          /* CCITT X.25                   */
#define ETH_TYPE_ARP       0x0806          /* Address Resolution packet    */
#define ETH_TYPE_BPQ       0x08FF          /* G8BPQ AX.25 Ethernet Packet  [ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_TYPE_IEEEPUP   0x0a00          /* Xerox IEEE802.3 PUP packet */
#define ETH_TYPE_IEEEPUPAT 0x0a01          /* Xerox IEEE802.3 PUP Addr Trans packet */
#define ETH_TYPE_DEC       0x6000          /* DEC Assigned proto           */
#define ETH_TYPE_DNA_DL    0x6001          /* DEC DNA Dump/Load            */
#define ETH_TYPE_DNA_RC    0x6002          /* DEC DNA Remote Console       */
#define ETH_TYPE_DNA_RT    0x6003          /* DEC DNA Routing              */
#define ETH_TYPE_LAT       0x6004          /* DEC LAT                      */
#define ETH_TYPE_DIAG      0x6005          /* DEC Diagnostics              */
#define ETH_TYPE_CUST      0x6006          /* DEC Customer use             */
#define ETH_TYPE_SCA       0x6007          /* DEC Systems Comms Arch       */
#define ETH_TYPE_TEB       0x6558          /* Trans Ether Bridging         */
#define ETH_TYPE_RARP      0x8035          /* Reverse Addr Res packet      */
#define ETH_TYPE_ATALK     0x809B          /* Appletalk DDP                */
#define ETH_TYPE_AARP      0x80F3          /* Appletalk AARP               */
#define ETH_TYPE_8021Q     0x8100          /* 802.1Q VLAN Extended Header  */
#define ETH_TYPE_IPX       0x8137          /* IPX over DIX                 */
#define ETH_TYPE_IPV6      0x86DD          /* IPv6 over bluebook           */
#define ETH_TYPE_PAUSE     0x8808          /* IEEE Pause frames. See 802.3 31B */
#define ETH_TYPE_SLOW      0x8809          /* Slow Protocol. See 802.3ad 43B */
#define ETH_TYPE_WCCP      0x883E          /* Web-cache coordination protocol
                                          * defined in draft-wilson-wrec-wccp-v2-00.txt */
#define ETH_TYPE_PPP_DISC  0x8863          /* PPPoE discovery messages     */
#define ETH_TYPE_PPP_SES   0x8864          /* PPPoE session messages       */
#define ETH_TYPE_MPLS_UC   0x8847          /* MPLS Unicast traffic         */
#define ETH_TYPE_MPLS_MC   0x8848          /* MPLS Multicast traffic       */
#define ETH_TYPE_ATMMPOA   0x884c          /* MultiProtocol Over ATM       */
#define ETH_TYPE_LINK_CTL  0x886c          /* HPNA, wlan link local tunnel */
#define ETH_TYPE_ATMFATE   0x8884          /* Frame-based ATM Transport over Ethernet*/
#define ETH_TYPE_PAE       0x888E          /* Port Access Entity (IEEE 802.1X) */
#define ETH_TYPE_AOE       0x88A2          /* ATA over Ethernet            */
#define ETH_TYPE_8021AD    0x88A8          /* 802.1ad Service VLAN         */
#define ETH_TYPE_TIPC      0x88CA          /* TIPC                         */
#define ETH_TYPE_8021AH    0x88E7          /* 802.1ah Backbone Service Tag */
#define ETH_TYPE_1588      0x88F7          /* IEEE 1588 Timesync */
#define ETH_TYPE_FCOE      0x8906          /* Fibre Channel over Ethernet  */
#define ETH_TYPE_FIP       0x8914          /* FCoE Initialization Protocol */
#define ETH_TYPE_QINQ1     0x9100          /* deprecated QinQ VLAN [ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_TYPE_QINQ2     0x9200          /* deprecated QinQ VLAN [ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_TYPE_QINQ3     0x9300          /* deprecated QinQ VLAN [ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_TYPE_EDSA      0xDADA          /* Ethertype DSA [ NOT AN OFFICIALLY REGISTERED ID ] */

#define ETH_ADDR_LEN    6
typedef struct __eth_addr {
   uint8_t  addr[ETH_ADDR_LEN];
} eth_addr_t;

#define ETH_HDR_LEN     14 // (ETH_ADDR_LEN + ETH_ADDR_LEN + 2)
typedef struct __eth_hdr {
   eth_addr_t  dst;   /* (destination MAC) destination ethernet address */
   eth_addr_t  src;   /* (source MAC) source ethernet address*/
   uint16_t   type;
} eth_hdr_t;

void fprint_eth_hdr(FILE *, eth_hdr_t *);
__inline void print_eth_hdr(eth_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif

