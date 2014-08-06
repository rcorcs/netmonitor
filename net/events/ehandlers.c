#include "ehandlers.h"

#include "../html.h"

void tcp_connection(packet_t *pkt, int code)
{
   switch(code){
   case 1:
      //printf("TCP Connection: 1st Step\n");
      break;
   case 2:
      //printf("TCP Connection: 2nd Step\n");
      break;
   case 3:
      //printf("TCP Connection: 3rd Step\n");
      break;
   }
}

void http_request(http_request_t *httpreq, packet_t *pkt)
{

}

void http_reply(http_reply_t *httprep, packet_t *pkt)
{
   //print_http_reply(httprep);
}

void dns_request(dns_msg_t *dnsmsg, packet_t *pkt)
{

}

void dns_reply(dns_msg_t *nsmsg, packet_t *pkt)
{

}

void http_content_received(http_content_t *content, packet_t *pkt)
{
    //dump(content->data, content->dlen);
    int i;
    char *ctype = get_http_reply_field(content->http_hdr, "Content-Type");
    if(ctype && strstr(ctype, "html")){
       //printf("***HTML Content: %s***\n", ctype);
       html_extract_urls(content->data, content->dlen);
    }

    content->http_hdr->data = 0;
    content->http_hdr->dlen = 0;
    print_http_reply(content->http_hdr);
    for(i=0;i<content->dlen; ++i){
       if(content->data[i]){
          putchar(content->data[i]);
       }
    }
    printf("\n**********************************************************************************************************\n");
}
