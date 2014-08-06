#include "packetp.h"

void fprint_tcp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   ipv4_addr_t * ipv4addr;
   ipv6_addr_t * ipv6addr;
   tcpp_t * tcpp;
   fprintf(out, "TCP Parameters\n");
   //printf("HERE\n");
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         if(hnode->klen==IPV4_ADDR_LEN){
            ipv4addr = (ipv4_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv4_addr_to_c_str(ipv4addr));
         }else if(hnode->klen==IPV6_ADDR_LEN){
            ipv6addr = (ipv6_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv6_addr_to_c_str(ipv6addr));
         }
         tcpp = (tcpp_t *)hnode->val;
         //printf("HERE (2)\n");
         if(tcpp){
            fprint_tcpp(out, tcpp);
            //printf("HERE (3)\n");
         }
      }
   }
   fputc('\n',out);
}

__inline void print_tcp_hash_tab(hash_tab_t *htab)
{
   fprint_tcp_hash_tab(stdout, htab);
}


void fprint_udp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   ipv4_addr_t * ipv4addr;
   ipv6_addr_t * ipv6addr;
   udpp_t * udpp;
   fprintf(out, "UDP Parameters\n");
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         if(hnode->klen==IPV4_ADDR_LEN){
            ipv4addr = (ipv4_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv4_addr_to_c_str(ipv4addr));
         }else if(hnode->klen==IPV6_ADDR_LEN){
            ipv6addr = (ipv6_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv6_addr_to_c_str(ipv6addr));
         }
         udpp = (udpp_t *)hnode->val;
         if(udpp)
            fprint_udpp(out, udpp);
      }
   }
   fputc('\n',out);
}

__inline void print_udp_hash_tab(hash_tab_t *htab)
{
    fprint_udp_hash_tab(stdout, htab);
}

void fprint_icmp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   ipv4_addr_t * ipv4addr;
   ipv6_addr_t * ipv6addr;
   icmp_stats_t * icmpstats;
   fprintf(out, "ICMP Parameters\n");
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         if(hnode->klen==IPV4_ADDR_LEN){
            ipv4addr = (ipv4_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv4_addr_to_c_str(ipv4addr));
         }else if(hnode->klen==IPV6_ADDR_LEN){
            ipv6addr = (ipv6_addr_t *)hnode->key;
            fprintf(out, "------- %s -------\n", ipv6_addr_to_c_str(ipv6addr));
         }
         icmpstats = (icmp_stats_t *)hnode->val;
         if(icmpstats)
            fprint_icmp_stats(out, icmpstats);
      }
   }
   fputc('\n',out);
}

__inline void print_icmp_hash_tab(hash_tab_t *htab)
{
    fprint_icmp_hash_tab(stdout, htab);
}

void fprint_packetp(FILE *out, packetp_t *pktp)
{
    fprint_tcp_hash_tab(out, pktp->tcp_htab);
    fprint_udp_hash_tab(out, pktp->udp_htab);
    fprint_icmp_hash_tab(out, pktp->icmp_htab);
    fprintf(out, "**************************************************************\n");
}

__inline void print_packetp(packetp_t *pktp)
{
   fprint_packetp(stdout, pktp);
}


void store_tcp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   //ipv4_addr_t * ipv4addr;
   //ipv6_addr_t * ipv6addr;
   tcpp_t * tcpp;
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         tcpp = (tcpp_t *)hnode->val;
         if(tcpp){
             if(hnode->klen==IPV4_ADDR_LEN){
                //ipv4addr = (ipv4_addr_t *)hnode->key;
                fwrite("[IPv4]", 6, 1, out);
                fwrite(hnode->key, IPV4_ADDR_LEN, 1, out);
             }else if(hnode->klen==IPV6_ADDR_LEN){
                //ipv6addr = (ipv6_addr_t *)hnode->key;
                fwrite("[IPv6]", 6, 1, out);
                fwrite(hnode->key, IPV6_ADDR_LEN, 1, out);
             }
            store_tcpp(out, tcpp);
         }
      }
   }
}

void store_udp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   //ipv4_addr_t * ipv4addr;
   //ipv6_addr_t * ipv6addr;
   udpp_t * udpp;
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         if(hnode->klen==IPV4_ADDR_LEN){
            //ipv4addr = (ipv4_addr_t *)hnode->key;
            fwrite("[IPv4]", 6, 1, out);
            fwrite(hnode->key, IPV4_ADDR_LEN, 1, out);
         }else if(hnode->klen==IPV6_ADDR_LEN){
            //ipv6addr = (ipv6_addr_t *)hnode->key;
            fwrite("[IPv6]", 6, 1, out);
            fwrite(hnode->key, IPV6_ADDR_LEN, 1, out);
         }else break;
         udpp = (udpp_t *)hnode->val;
         if(udpp)
            store_udpp(out, udpp);
      }
   }
}

void store_tcpp_file(FILE *out, packetp_t *pktp, struct tm *time)
{
   char timestamp[7] = "[TIME]";

    if(time){
        fwrite(timestamp, 6, 1, out);
        fwrite(time, sizeof(struct tm), 1, out);
    }
    store_tcp_hash_tab(out, pktp->tcp_htab);
}

void store_udpp_file(FILE *out, packetp_t *pktp, struct tm *time)
{
   char timestamp[7] = "[TIME]";

    if(time){
        fwrite(timestamp, 6, 1, out);
        fwrite(time, sizeof(struct tm), 1, out);
    }
    store_udp_hash_tab(out, pktp->udp_htab);
}

bool_t fprint_udpp_element(FILE *out, FILE *in)
{
   //hash_node_t *hnode;
   //uint_t i;
   ipv4_addr_t ipv4addr;
   ipv6_addr_t ipv6addr;
   udpp_t udpp;
   char ipv[7];
   ipv[6] = 0;

   printf("UDP Parameters\n");
   while(1){
        //ipv = fgetc(in);
        fread(ipv, 6, 1, in);
        if(feof(in)) break;
        if( !strcmp(ipv, "[IPv4]") ){
            fread(&ipv4addr, IPV4_ADDR_LEN, 1, in);
            printf("%s\n", ipv4_addr_to_c_str(&ipv4addr));
        } else if( !strcmp(ipv, "[IPv6]") ){
            fread(&ipv6addr, IPV6_ADDR_LEN, 1, in);
            printf("%s\n", ipv6_addr_to_c_str(&ipv6addr));
        }else if(!strcmp(ipv, "[TIME]")){
            return TRUE;
        }else return FALSE;

        if(feof(in)) break;
        load_udpp(in, &udpp);
        if(feof(in)) break;
        fprint_udpp(out, &udpp);
        if(feof(in)) break;
   }
   return FALSE;
}

void fprint_udpp_file(FILE *out, FILE *in)
{
    struct tm time;
    char timestamp[7];
    timestamp[6] = 0;

    fread(timestamp, 6, 1, in);
    if(!feof(in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, in);
                print_time(&time);
                putchar('\n');
            }while(!feof(in) && fprint_udpp_element(out, in));
        }
    }
}

bool_t load_udpp_element(FILE *in, list_t *list)
{
   //hash_node_t *hnode;
   //uint_t i;
   ipv4_addr_t ipv4addr;
   ipv6_addr_t ipv6addr;
   //tcpp_t tcpp;
   node_t *node;
   //char ipv;
   char ipv[7];
   ipv[6] = 0;

   //fprintf(out, "TCP Statistics\n");
   while(1){
        //ipv = fgetc(in);
        fread(ipv, 6, 1, in);
        if(feof(in)) break;
        if( !strcmp(ipv, "[IPv4]") ){
            fread(&ipv4addr, IPV4_ADDR_LEN, 1, in);
            //fprintf(out, "%s\n", ipv4_addr_to_c_str(&ipv4addr));
        } else if(!strcmp(ipv, "[IPv6]")){
            fread(&ipv6addr, IPV6_ADDR_LEN, 1, in);
            //fprintf(out, "%s\n", ipv6_addr_to_c_str(&ipv6addr));
        }else if(!strcmp(ipv, "[TIME]")){
            return TRUE;
        }else return FALSE;
        if(feof(in)) break;
        node = create_node(sizeof(udpp_t), 0, 0);
        load_udpp(in, (udpp_t *)node->data);
        //load_tcpp(in, &tcpp);
        if(feof(in)) break;
        list_push_back(list, node);
        //fprint_tcpp(out, &tcpp);
        if(feof(in)) break;
   }
   return FALSE;
}

void load_udpp_file(FILE *in, list_t *list)
{
    struct tm time;
    char timestamp[7];
    timestamp[6] = 0;

    fread(timestamp, 6, 1, in);
    if(!feof(in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, in);
                //print_time(&time);
                //putchar('\n');
            }while(!feof(in) && load_udpp_element(in, list));
        }
    }
}


bool_t fprint_tcpp_element(FILE *out, FILE *in)
{
   //hash_node_t *hnode;
   //uint_t i;
   ipv4_addr_t ipv4addr;
   ipv6_addr_t ipv6addr;
   tcpp_t tcpp;
   //node_t *node;
   //char ipv;
   char ipv[7];
   ipv[6] = 0;

   fprintf(out, "TCP Parameters\n");
   while(1){
        //ipv = fgetc(in);
        fread(ipv, 6, 1, in);
        if(feof(in)) break;
        if( !strcmp(ipv, "[IPv4]") ){
            fread(&ipv4addr, IPV4_ADDR_LEN, 1, in);
            fprintf(out, "%s\n", ipv4_addr_to_c_str(&ipv4addr));
        } else if(!strcmp(ipv, "[IPv6]")){
            fread(&ipv6addr, IPV6_ADDR_LEN, 1, in);
            fprintf(out, "%s\n", ipv6_addr_to_c_str(&ipv6addr));
        }else if(!strcmp(ipv, "[TIME]")){
            return TRUE;
        }else return FALSE;
        if(feof(in)) break;
        //node = create_node(sizeof(tcpp_t), 0, 0);
        //load_tcpp(in, node.data);
        load_tcpp(in, &tcpp);
        if(feof(in)) break;
        //list_push_back(list, node);
        fprint_tcpp(out, &tcpp);
        if(feof(in)) break;
   }
   return FALSE;
}

void fprint_tcpp_file(FILE *out, FILE *tcp_in)
{
    struct tm time;
    char timestamp[7];
    timestamp[6] = 0;

    fread(timestamp, 6, 1, tcp_in);
    if(!feof(tcp_in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, tcp_in);
                print_time(&time);
                putchar('\n');
            }while(!feof(tcp_in) && fprint_tcpp_element(out, tcp_in));
        }
    }
}

bool_t load_tcpp_element(FILE *in, list_t *list)
{
   //hash_node_t *hnode;
   //uint_t i;
   ipv4_addr_t ipv4addr;
   ipv6_addr_t ipv6addr;
   //tcpp_t tcpp;
   node_t *node;
   //char ipv;
   char ipv[7];
   ipv[6] = 0;

   //fprintf(out, "TCP Statistics\n");
   while(1){
        //ipv = fgetc(in);
        fread(ipv, 6, 1, in);
        if(feof(in)) break;
        if( !strcmp(ipv, "[IPv4]") ){
            fread(&ipv4addr, IPV4_ADDR_LEN, 1, in);
            //fprintf(out, "%s\n", ipv4_addr_to_c_str(&ipv4addr));
        } else if(!strcmp(ipv, "[IPv6]")){
            fread(&ipv6addr, IPV6_ADDR_LEN, 1, in);
            //fprintf(out, "%s\n", ipv6_addr_to_c_str(&ipv6addr));
        }else if(!strcmp(ipv, "[TIME]")){
            return TRUE;
        }else return FALSE;
        if(feof(in)) break;
        node = create_node(sizeof(tcpp_t), 0, 0);
        load_tcpp(in, (tcpp_t *)node->data);
        //load_tcpp(in, &tcpp);
        if(feof(in)) break;
        list_push_back(list, node);
        //fprint_tcpp(out, &tcpp);
        if(feof(in)) break;
   }
   return FALSE;
}

void load_tcpp_file(FILE *in, list_t *list)
{
    struct tm time;
    char timestamp[7];
    timestamp[6] = 0;

    fread(timestamp, 6, 1, in);
    if(!feof(in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, in);
                //print_time(&time);
                //putchar('\n');
            }while(!feof(in) && load_tcpp_element(in, list));
        }
    }
}


/*
void store_icmp_hash_tab(FILE *out, hash_tab_t *htab)
{
   hash_node_t *hnode;
   uint_t i;
   //ipv4_addr_t * ipv4addr;
   //ipv6_addr_t * ipv6addr;
   icmp_stats_t * icmpstats;
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
         if(hnode->klen==IPV4_ADDR_LEN){
            //ipv4addr = (ipv4_addr_t *)hnode->key;
            fwrite("[IPv4]", 6, 1, out);
            fwrite(hnode->key, IPV4_ADDR_LEN, 1, out);
         }else if(hnode->klen==IPV6_ADDR_LEN){
            //ipv6addr = (ipv6_addr_t *)hnode->key;
            fwrite("[IPv4]", 6, 1, out);
            fwrite(hnode->key, IPV6_ADDR_LEN, 1, out);
         }
         icmpstats = (icmp_stats_t *)hnode->val;
         if(icmpstats)
            store_icmp_stats(out, icmpstats);
      }
   }
}
*/


/*
bool_t load_icmp_hash_tab(FILE *in, hash_tab_t *htab)
{
   //hash_node_t *hnode;
   //uint_t i;
   ipv4_addr_t ipv4addr;
   ipv6_addr_t ipv6addr;
   icmp_stats_t icmpstats;
   char ipv[7];
   ipv[6] = 0;

   printf("ICMP Parameters\n");
   while(1){
        //ipv = fgetc(in);
        fread(ipv, 6, 1, in);
        if(feof(in)) break;
        if( !strcmp(ipv, "[IPv4]") ){
            fread(&ipv4addr, IPV4_ADDR_LEN, 1, in);
            printf("%s\n", ipv4_addr_to_c_str(&ipv4addr));
        } else if( !strcmp(ipv, "[IPv6]") ){
            fread(&ipv6addr, IPV6_ADDR_LEN, 1, in);
            printf("%s\n", ipv6_addr_to_c_str(&ipv6addr));
        }else if(!strcmp(ipv, "[TIME]")){
            return TRUE;
        }else return FALSE;

        if(feof(in)) break;
        load_icmp_stats(in, &icmpstats);
        if(feof(in)) break;
        print_icmp_stats(&icmpstats);
        if(feof(in)) break;
   }
   return FALSE;
}
*/

/*
void load_packetp(FILE *tcp_in, FILE *udp_in, FILE *icmp_in, packetp_t *pktp)
{
    struct tm time;
    char timestamp[7];
    timestamp[6] = 0;

    fread(timestamp, 6, 1, tcp_in);
    if(!feof(tcp_in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, tcp_in);
                print_time(&time);
                putchar('\n');
            }while(!feof(tcp_in) && load_tcp_hash_tab(tcp_in, 0));
        }
    }

    fread(timestamp, 6, 1, udp_in);
    if(!feof(udp_in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do {
                fread(&time, sizeof(struct tm), 1, udp_in);
                print_time(&time);
                putchar('\n');
            }while(load_udp_hash_tab(udp_in, 0));
        }
    }

    fread(timestamp, 6, 1, icmp_in);
    if(!feof(icmp_in)){
        if( !strcmp(timestamp, "[TIME]") ){
            do{
                fread(&time, sizeof(struct tm), 1, icmp_in);
                print_time(&time);
                putchar('\n');
            }while(load_icmp_hash_tab(icmp_in, 0));
        }
    }
}
*/
