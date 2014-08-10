#include "packetp.h"

packetp_t *create_packetp(uint_t len)
{
    packetp_t *pktp = (packetp_t *)malloc(sizeof(packetp_t));
    pktp->icmp_htab = create_hash_tab(len, NULL);
    pktp->tcp_htab = create_hash_tab(len, NULL);
    pktp->udp_htab = create_hash_tab(len, NULL);
    return pktp;
}

void destroy_packetp(packetp_t **pktp)
{
    destroy_hash_tab_nodes( (*pktp)->icmp_htab);
    destroy_hash_tab(&(*pktp)->icmp_htab);

    destroy_hash_tab_nodes((*pktp)->tcp_htab);
    destroy_hash_tab(&(*pktp)->tcp_htab);

    destroy_hash_tab_nodes((*pktp)->udp_htab);
    destroy_hash_tab(&(*pktp)->udp_htab);

    free(*pktp);
    *pktp = 0;
}

void clear_packetp(packetp_t *pktp)
{
    destroy_hash_tab_nodes( pktp->icmp_htab );
    destroy_hash_tab_nodes( pktp->tcp_htab );
    destroy_hash_tab_nodes( pktp->udp_htab );
}

void packetp_add_out_info(packetp_t *pktp, packet_t *pkt)
{
    hash_node_t *hnode;
    if(pkt->ipv4h) {
       if(pkt->tcph){
             switch(pkt->ipv4h->proto){
             case IP_PROTO_TCP: {
                    tcpp_t *tcpp;
                    hnode = hash_tab_lookup(pktp->tcp_htab, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(tcpp_t));
                        memcpy(hnode->key, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(tcpp_t));
                        hash_tab_insert(pktp->tcp_htab, hnode);
                    }
                    tcpp = (tcpp_t *)hnode->val;
                    ++(tcpp->out.total);
                    if(pkt->tcph->ackbit) ++(tcpp->out.tack);
                    if(pkt->tcph->pshbit) ++(tcpp->out.tpsh);
                    if(pkt->tcph->rstbit) ++(tcpp->out.trst);
                    if(pkt->tcph->synbit) ++(tcpp->out.tsyn);
                    if(pkt->tcph->finbit) ++(tcpp->out.tfin);
                    tcpp->out.tdatalen += pkt->dlen;
                }
                break;
             case IP_PROTO_UDP: {
                    udpp_t *udpp;
                    hnode = hash_tab_lookup(pktp->udp_htab, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(udpp_t));
                        memcpy(hnode->key, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(udpp_t));
                        hash_tab_insert(pktp->udp_htab, hnode);
                    }
                    udpp = (udpp_t *)hnode->val;
                    ++(udpp->out.total);
                } break;
             case IP_PROTO_ICMP: {
                    //get ICMP STATS of the DST IP and add the info
                    /*
                    icmp_stats_t *icmpstats;
                    hnode = hash_tab_lookup(pktstats->icmp_htab, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(icmp_stats_t));
                        memcpy(hnode->key, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(icmp_stats_t));
                        hash_tab_insert(pktstats->icmp_htab, hnode);
                    }
                    icmpstats = (icmp_stats_t *)hnode->val;
                    icmpstats->out.total++;
                    if(pkt->icmpv4h->type == ICMPV4_TYPE_ECHO_REQUEST && pkt->icmpv4h->code == ICMPV4_CODE_ECHO_REQUEST)
                       icmpstats->out.techoreq++;
                    else if(pkt->icmpv4h->type == ICMPV4_TYPE_ECHO_REPLY && pkt->icmpv4h->code == ICMPV4_CODE_ECHO_REPLY)
                        icmpstats->out.techoreply++;
                    */
                } break;
             default:
                break;
             }
          }
   }else if(pkt->ipv6h) {
      if(pkt->tcph){
             switch(pkt->ipv6h->nexthdr){
             case IP_PROTO_TCP:
                //get TCP STATS of the DST IP and add the info
                break;
             case IP_PROTO_UDP:
                //get UDP STATS of the DST IP and add the info
                break;
             case IP_PROTO_IPV6_ICMP:
                //get ICMP STATS of the DST IP and add the info
                break;
             default:
                break;
             }
          }
   }
}

void packetp_add_in_info(packetp_t *pktp, packet_t *pkt)
{
    hash_node_t *hnode;
    if(pkt->ipv4h) {
       if(pkt->tcph){
             switch(pkt->ipv4h->proto){
             case IP_PROTO_TCP: {
                    tcpp_t *tcpp;
                    hnode = hash_tab_lookup(pktp->tcp_htab, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(tcpp_t));
                        memcpy(hnode->key, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(tcpp_t));
                        hash_tab_insert(pktp->tcp_htab, hnode);
                    }
                    tcpp = (tcpp_t *)hnode->val;
                    ++(tcpp->in.total);
                    if(pkt->tcph->ackbit) ++(tcpp->in.tack);
                    if(pkt->tcph->pshbit) ++(tcpp->in.tpsh);
                    if(pkt->tcph->rstbit) ++(tcpp->in.trst);
                    if(pkt->tcph->synbit) ++(tcpp->in.tsyn);
                    if(pkt->tcph->finbit) ++(tcpp->in.tfin);
                    tcpp->in.tdatalen += pkt->dlen;
                }
                break;
             case IP_PROTO_UDP:  {
                    udpp_t *udpp;
                    hnode = hash_tab_lookup(pktp->udp_htab, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(udpp_t));
                        memcpy(hnode->key, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(udpp_t));
                        hash_tab_insert(pktp->udp_htab, hnode);
                    }
                    udpp = (udpp_t *)hnode->val;
                    ++(udpp->in.total);
                } break;
             case IP_PROTO_ICMP: {
                    //get ICMP STATS of the DST IP and add the info
                    /*
                    icmp_stats_t *icmpstats;
                    hnode = hash_tab_lookup(pktstats->icmp_htab, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                    if(!hnode){
                        hnode = alloc_hash_node(IPV4_ADDR_LEN, sizeof(icmp_stats_t));
                        memcpy(hnode->key, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
                        memset(hnode->val, 0, sizeof(icmp_stats_t));
                        hash_tab_insert(pktstats->icmp_htab, hnode);
                    }
                    icmpstats = (icmp_stats_t *)hnode->val;
                    icmpstats->in.total++;
                    if(pkt->icmpv4h->type == ICMPV4_TYPE_ECHO_REQUEST && pkt->icmpv4h->code == ICMPV4_CODE_ECHO_REQUEST)
                       icmpstats->in.techoreq++;
                    else if(pkt->icmpv4h->type == ICMPV4_TYPE_ECHO_REPLY && pkt->icmpv4h->code == ICMPV4_CODE_ECHO_REPLY)
                        icmpstats->in.techoreply++;
                    */
                } break;
             default:
                break;
             }
          }
   }else if(pkt->ipv6h) {
      if(pkt->tcph){
         switch(pkt->ipv6h->nexthdr){
         case IP_PROTO_TCP:
            //get TCP STATS of the DST IP and add the info
            break;
         case IP_PROTO_UDP:
            //get UDP STATS of the DST IP and add the info
            break;
         case IP_PROTO_IPV6_ICMP:
            //get ICMP STATS of the DST IP and add the info
            break;
         default:
            break;
         }
      }
   }
}
