#ifndef __EHANDLERS
#define __EHANDLERS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "../network.h"
#include "../util.h"

void tcp_connection(packet_t *, int);
void http_request(http_request_t *, packet_t *);
void http_reply(http_reply_t *, packet_t *);
void dns_request(dns_msg_t *, packet_t *);
void dns_reply(dns_msg_t *, packet_t *);
void http_content_received(http_content_t *, packet_t *);
#ifdef __cplusplus
}
#endif

#endif

