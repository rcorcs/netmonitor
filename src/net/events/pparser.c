#include "pparser.h"

#include "../../datatypes/list.h"
#include "../../datatypes/str.h"

int parse_tcp_connection(packet_t *, const struct pcap_pkthdr *);

void parse_http_request(packet_t *, const struct pcap_pkthdr *);
void parse_http_reply(packet_t *, const struct pcap_pkthdr *);

void parse_dns_request(packet_t *, const struct pcap_pkthdr *);
void parse_dns_reply(packet_t *, const struct pcap_pkthdr *);

list_t *tcp_conn_addrs;

hash_tab_t *http_replies;

void parser_init()
{
   tcp_conn_addrs = create_list();
   http_replies = create_hash_tab(64, hash_tab_compare);
}

void parser_close()
{
   destroy_list_nodes(tcp_conn_addrs);
   destroy_list(&tcp_conn_addrs);

   destroy_hash_tab_nodes(http_replies);
   destroy_hash_tab(&http_replies);
}

void BLA_parse_packet(packet_t *pkt, const struct pcap_pkthdr *cap_header){}

void parse_packet(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   int datapad = 0;
   int code;

   if(pkt->arph){
      //fprint_arp_hdr(out, pkt->arph);
   }

   if(pkt->vlan8021qh){
      //fprint_vlan8021q_hdr(out, pkt->vlan8021qh);
   }

   if(pkt->ppph){
      //fprint_ppp_hdr(out, pkt->ppph);
   }

   if(pkt->ipv4h) {
       if(pkt->tcph){
             switch(pkt->ipv4h->proto){
             case IP_PROTO_TCP:
                if(code = parse_tcp_connection(pkt, cap_header)){
                   tcp_connection(pkt, code);
                }else{
                   if(ntohs(pkt->tcph->dst_port)==80){
                      parse_http_request(pkt, cap_header);
                   }else if(ntohs(pkt->tcph->src_port)==80){
                      parse_http_reply(pkt, cap_header);
                   }
                }
                break;
             case IP_PROTO_UDP:
                if(ntohs(pkt->udph->dst_port)==53){
                   parse_dns_request(pkt, cap_header);
                }else if(ntohs(pkt->udph->src_port)==53){
                   parse_dns_reply(pkt, cap_header);
                }else{
                }
                break;
             case IP_PROTO_ICMP:
                //fprint_icmpv4_hdr(out, pkt->icmpv4h);
                break;
             default:
                break;
             }
          }
   }else if(pkt->ipv6h) {

      if(pkt->tcph){
             switch(pkt->ipv6h->nexthdr){
             case IP_PROTO_TCP:
                if(code = parse_tcp_connection(pkt, cap_header)){
                   tcp_connection(pkt, code);
                }else{
                   if(ntohs(pkt->tcph->dst_port)==80){
                      parse_http_request(pkt, cap_header);
                   }else if(ntohs(pkt->tcph->src_port)==80){
                      parse_http_reply(pkt, cap_header);
                   }
                }
                break;
             case IP_PROTO_UDP:
                if(ntohs(pkt->udph->dst_port)==53){
                   parse_dns_request(pkt, cap_header);
                }else if(ntohs(pkt->udph->src_port)==53){
                   parse_dns_reply(pkt, cap_header);
                }else{
                }
                break;
             case IP_PROTO_IPV6_ICMP:
                //fprint_icmpv6_hdr(out, pkt->icmpv6h);
                break;
             default:
                break;
             }
          }
   }

   if(pkt->data && ((long int)(pkt->dlen-datapad))>0){
      //fdump(out, pkt->data+datapad, (pkt->dlen-datapad));
   }
}

int parse_tcp_connection(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   char addrfound = 0;
   node_t *it = 0;
   node_t *srcaddr = 0;
   node_t *dstaddr = 0;
   if(pkt->ipv4h){
      dstaddr = create_node(IPV4_ADDR_LEN, 0, 0);
      memcpy(dstaddr->data, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);

      srcaddr = create_node(IPV4_ADDR_LEN, 0, 0);
      memcpy(srcaddr->data, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
   }else if(pkt->ipv6h){
      dstaddr = create_node(IPV6_ADDR_LEN, 0, 0);
      memcpy(dstaddr->data, pkt->ipv6h->dst.addr, IPV6_ADDR_LEN);

      srcaddr = create_node(IPV6_ADDR_LEN, 0, 0);
      memcpy(srcaddr->data, pkt->ipv6h->dst.addr, IPV6_ADDR_LEN);
   }

   if(dstaddr){
      for(it=tcp_conn_addrs->begin; it!=0; it = it->next){
         if(!memcmp(srcaddr->data, it->data, srcaddr->size) || !memcmp(dstaddr->data, it->data, dstaddr->size)){
            addrfound = 1;
            break;
         }
      }

      if(!addrfound){
         if(pkt->tcph->synbit && !pkt->tcph->ackbit){
            list_push_back(tcp_conn_addrs, dstaddr);
            //printf("TCP Connection: 1st Step\n");
            return 1;
         }
      }else{
         destroy_node(&srcaddr);
         destroy_node(&dstaddr);
         if(pkt->tcph->synbit && pkt->tcph->ackbit){
            //printf("TCP Connection: 2nd Step\n");
            return 2;
         }else if(pkt->tcph->ackbit){
            //printf("TCP Connection: 3nd Step\n");
            list_remove(tcp_conn_addrs, it);
            return 3;
         }
      }
   }

   return 0;
}

void parse_http_request(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   http_request_t *http_req = create_http_request();
   if(!decode_http_request(http_req, pkt->data, pkt->dlen)){
      //print_http_request(http_req);
      http_request(http_req, pkt);
   }
   destroy_http_request(&http_req);
}

/*
void parse_http_reply(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   hash_node_t *hnode;
   byte_t *key;
   uint_t klen;
   byte_t *val;
   uint_t vlen;
   http_reply_t *http_rep = create_http_reply();
   int code;
   if(pkt->ipv4h){
      klen = 2*IPV4_ADDR_LEN + sizeof(tcp_port_t);
      key = (byte_t *)malloc(klen);
      memcpy(key, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
      memcpy(key + IPV4_ADDR_LEN, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
      memcpy(key + 2*IPV4_ADDR_LEN, &pkt->tcph->dst_port, sizeof(tcp_port_t));
   }else if(pkt->ipv6h){
      klen = 2*IPV6_ADDR_LEN + sizeof(tcp_port_t);
      key = (byte_t *)malloc(klen);
      memcpy(key, pkt->ipv4h->src.addr, IPV6_ADDR_LEN);
      memcpy(key + IPV6_ADDR_LEN, pkt->ipv4h->dst.addr, IPV6_ADDR_LEN);
      memcpy(key + 2*IPV6_ADDR_LEN, &pkt->tcph->dst_port, sizeof(tcp_port_t));
   }
   hnode = hash_tab_lookup(http_replies, key, klen);
   code = decode_http_reply(http_rep, pkt->data, pkt->dlen);
   if(hnode){
      if(code>1){
         http_content_t *content = (http_content_t *)hnode->val;
         uint_t cdlen = content->dlen + pkt->dlen;
         byte_t *cdata = (byte_t *)malloc(cdlen);
         memcpy(cdata, content->data, content->dlen);
         memcpy(cdata + content->dlen, pkt->data, pkt->dlen);
         free(content->data);
         content->data = cdata;
         content->dlen = cdlen;
         //printf("Begin %d->%d\n", content->dlen, content->length);
         //dump(http_rep->data, http_rep->dlen);
         //printf("End\n");
         if(content->dlen>=content->length){
            //printf("Finished\n");
            http_content_received(content, pkt);
            free(content->data);
            free(content);
            hash_tab_remove(http_replies, key, klen);
         }
      }else if(code==0){
         http_reply(http_rep, pkt);
         //printf("New Content\n");
         //dump(http_rep->data, http_rep->dlen);
      }
      free(key);
   }else{
      if(!code){
         char *clength = get_http_reply_field(http_rep, "Content-Length");
         if(clength){
            http_content_t *content = (http_content_t *)malloc(sizeof(http_content_t));
            content->length = atoi(clength);
            content->dlen = http_rep->dlen;
            content->data = (byte_t *)malloc(content->dlen);
            memcpy(content->data, http_rep->data, http_rep->dlen);
            //printf("Begin New %d->%d\n", content->dlen, content->length);
            //dump(http_rep->data, http_rep->dlen);
            //printf("End New\n");
            if(content->dlen>=content->length){
               //printf("Finished\n");
               http_content_received(content, pkt);
               free(content->data);
               free(content);
            }else{
               hnode = create_hash_node(key, klen, (byte_t*)content, sizeof(http_content_t), 0);
               hash_tab_insert(http_replies, hnode);
            }
         }else{
            //printf("No Content-Length\n");
            //clength = get_http_reply_field(http_rep, "Content-Range");
            //dump(http_rep->data, http_rep->dlen);
         }
         http_reply(http_rep, pkt);
      }
   }
   destroy_http_reply(&http_rep);
}
*/
void parse_http_reply(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   char destroy_hdr = 1;
   hash_node_t *hnode;
   byte_t *key;
   uint_t klen;
   byte_t *val;
   uint_t vlen;
   http_reply_t *http_rep = create_http_reply();
   int code;
   if(pkt->ipv4h){
      klen = 2*IPV4_ADDR_LEN + sizeof(tcp_port_t);
      key = (byte_t *)malloc(klen);
      memcpy(key, pkt->ipv4h->src.addr, IPV4_ADDR_LEN);
      memcpy(key + IPV4_ADDR_LEN, pkt->ipv4h->dst.addr, IPV4_ADDR_LEN);
      memcpy(key + 2*IPV4_ADDR_LEN, &pkt->tcph->dst_port, sizeof(tcp_port_t));
   }else if(pkt->ipv6h){
      klen = 2*IPV6_ADDR_LEN + sizeof(tcp_port_t);
      key = (byte_t *)malloc(klen);
      memcpy(key, pkt->ipv4h->src.addr, IPV6_ADDR_LEN);
      memcpy(key + IPV6_ADDR_LEN, pkt->ipv4h->dst.addr, IPV6_ADDR_LEN);
      memcpy(key + 2*IPV6_ADDR_LEN, &pkt->tcph->dst_port, sizeof(tcp_port_t));
   }
   hnode = hash_tab_lookup(http_replies, key, klen);
   code = decode_http_reply(http_rep, pkt->data, pkt->dlen);
   if(hnode){
      if(code>1){
         http_content_t *content = (http_content_t *)hnode->val;
         uint_t cdlen = content->dlen + pkt->dlen;
         byte_t *cdata = (byte_t *)malloc(cdlen);
         memcpy(cdata, content->data, content->dlen);
         memcpy(cdata + content->dlen, pkt->data, pkt->dlen);
         free(content->data);
         content->data = cdata;
         content->dlen = cdlen;
         //printf("Begin %d->%d\n", content->dlen, content->length);
         //dump(http_rep->data, http_rep->dlen);
         //printf("End\n");
         if(content->length && content->dlen>=content->length){
            //printf("Finished\n");
            http_content_received(content, pkt);

            hash_tab_remove(http_replies, key, klen);
            free(content->data);
            destroy_http_reply(&(content->http_hdr));
            free(content);

         }
      }else if(code==0){
         char *clength = get_http_reply_field(http_rep, "Content-Length");

         http_content_t *content = (http_content_t *)hnode->val;
         http_content_received(content, pkt);
         free(content->data);

         if(clength) content->length = atoi(clength);
         else content->length = 0;
         content->dlen = http_rep->dlen;
         content->data = (byte_t *)malloc(content->dlen);
         memcpy(content->data, http_rep->data, http_rep->dlen);
         //printf("Begin New %d->%d\n", content->dlen, content->length);
         //dump(http_rep->data, http_rep->dlen);
         //printf("End New\n");
         content->http_hdr = http_rep;
         destroy_hdr = 0;
         if(content->length && content->dlen>=content->length){
            //printf("Finished\n");
            http_content_received(content, pkt);

            free(content->data);
            destroy_http_reply(&(content->http_hdr));
            free(content);
         }else{
            //hnode = create_hash_node(key, klen, (byte_t*)content, sizeof(http_content_t), 0);
            //hash_tab_insert(http_replies, hnode);
         }
         //http_reply(http_rep, pkt);

      }
      free(key);
   }else{
      if(!code){
         char *clength = get_http_reply_field(http_rep, "Content-Length");

         http_content_t *content = (http_content_t *)malloc(sizeof(http_content_t));

         if(clength) content->length = atoi(clength);
         else content->length = 0;
         content->dlen = http_rep->dlen;
         content->data = (byte_t *)malloc(content->dlen);
         memcpy(content->data, http_rep->data, http_rep->dlen);
         //printf("Begin New %d->%d\n", content->dlen, content->length);
         //dump(http_rep->data, http_rep->dlen);
         //printf("End New\n");
         content->http_hdr = http_rep;
         destroy_hdr = 0;
         if(content->length && content->dlen>=content->length){
            //printf("Finished\n");
            http_content_received(content, pkt);
            free(content->data);
            destroy_http_reply(&(content->http_hdr));
            free(content);
         }else{
            hnode = create_hash_node(key, klen, (byte_t*)content, sizeof(http_content_t), 0);
            hash_tab_insert(http_replies, hnode);
         }
         //http_reply(http_rep, pkt);
      }
   }
   if(destroy_hdr) destroy_http_reply(&http_rep);
}
void parse_dns_request(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   dns_msg_t dnsmsg;
   decode_dns_msg(&dnsmsg, (pkt->data));
   //print_dns_msg(&dnsmsg);
   dns_request(&dnsmsg, pkt);
   destroy_dns_msg(&dnsmsg);
}

void parse_dns_reply(packet_t *pkt, const struct pcap_pkthdr *cap_header)
{
   dns_msg_t dnsmsg;
   decode_dns_msg(&dnsmsg, (pkt->data));
   //print_dns_msg(&dnsmsg);
   dns_reply(&dnsmsg, pkt);
   destroy_dns_msg(&dnsmsg);
}
