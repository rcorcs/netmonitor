#ifndef __HTTP
#define __HTTP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"
#include "../../datatypes/hashtab.h"

#define HTTP_METHOD_GET      0x01
#define HTTP_METHOD_POST     0x02
#define HTTP_METHOD_HEAD     0x03
#define HTTP_METHOD_PUT      0x04
#define HTTP_METHOD_OPTIONS  0x05
#define HTTP_METHOD_DELETE   0x06
#define HTTP_METHOD_TRACE    0x07
#define HTTP_METHOD_CONNECT  0x08
#define HTTP_METHOD_PATCH    0x09

typedef struct __http_request {
   uint8_t   method;
   uint_t     urilen;
   uchar_t  * uri;
   uint_t     vlen;
   uchar_t  * version;
   hash_tab_t * fields;
   byte_t *data;
   uint_t  dlen;
} http_request_t;

typedef struct __http_reply {
   uint_t     vlen;
   uchar_t  * version;
   uint_t     code;
   uint_t     msglen;
   uchar_t  * msg;
   hash_tab_t * fields;
   byte_t *data;
   uint_t  dlen;
} http_reply_t;

typedef struct __http_content {
   byte_t *data;
   uint_t dlen;
   uint_t length;
   http_reply_t *http_hdr;
} http_content_t;

http_request_t * create_http_request();
int decode_http_request(http_request_t *, byte_t *, uint_t);
void destroy_http_request(http_request_t **);

void fprint_http_request(FILE *, http_request_t *);
__inline void print_http_request(http_request_t *);

http_reply_t * create_http_reply();
int decode_http_reply(http_reply_t *, byte_t *, uint_t);
void destroy_http_reply(http_reply_t **);

char *get_http_reply_field(http_reply_t *, char *);

void fprint_http_reply(FILE *, http_reply_t *);
__inline void print_http_reply(http_reply_t *);

#ifdef __cplusplus
}
#endif

#endif

