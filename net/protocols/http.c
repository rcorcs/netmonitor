
#include "http.h"

#include <string.h>

int decode_http_request(http_request_t *http_req, byte_t *data, uint_t dlen)
{
    hash_node_t *node;
    byte_t *key;
    uint_t klen;
    byte_t *val;
    uint_t vlen;

    byte_t *pnt;
    byte_t *pnt_temp;
    byte_t *end_pnt;
    byte_t sep[2] = {0x0d, 0x0a}; //default separator
    byte_t ws[2] = {0x3a, 0x20}; //default white space ": "
    byte_t end[4] = {0x0d, 0x0a, 0x0d, 0x0a};

    if(!dlen || !data[0]){
       return 1;
    }

    if( !memcmp(data, "GET", 3) ){
       http_req->method = HTTP_METHOD_GET;
       data += 4;
       dlen -= 4;
    }else if( !memcmp(data, "POST", 4) ){
       http_req->method = HTTP_METHOD_POST;
       data += 5;
       dlen -= 5;
    }else if( !memcmp(data, "HEAD", 4) ){
       http_req->method = HTTP_METHOD_HEAD;
       data += 5;
       dlen -= 5;
    }else if( !memcmp(data, "PUT", 3) ){
       http_req->method = HTTP_METHOD_PUT;
       data += 4;
       dlen -= 4;
    }else if( !memcmp(data, "OPTIONS", 7) ){
       http_req->method = HTTP_METHOD_OPTIONS;
       data += 8;
       dlen -= 8;
    }else if( !memcmp(data, "DELETE", 6) ){
       http_req->method = HTTP_METHOD_DELETE;
       data += 7;
       dlen -= 7;
    }else if( !memcmp(data, "TRACE", 5) ){
       http_req->method = HTTP_METHOD_TRACE;
       data += 6;
       dlen -= 6;
    }else if( !memcmp(data, "CONNECT", 7) ){
       http_req->method = HTTP_METHOD_CONNECT;
       data += 8;
       dlen -= 8;
    }else if( !memcmp(data, "PATCH", 5) ){
       http_req->method = HTTP_METHOD_PATCH;
       data += 6;
       dlen -= 6;
    }else{
       return 2;
    }

    if(data[0]){
       pnt = memchr(data, ' ', dlen);
       http_req->urilen = (pnt - data + 1);
       http_req->uri = (uchar_t *)malloc(http_req->urilen);
       memcpy(http_req->uri, data, http_req->urilen - 1);
       http_req->uri[http_req->urilen - 1] = 0;
       data += http_req->urilen;
       dlen -= http_req->urilen;
    }

    pnt = memfind(data, dlen, sep, 2);
    if(pnt){
       http_req->vlen = (pnt - data + 1);
       http_req->version = (uchar_t *)malloc(http_req->vlen);
       memcpy(http_req->version, data, http_req->vlen - 1);
       http_req->version[http_req->vlen - 1] = 0;
       data = pnt + 2;
       dlen -= (http_req->vlen + 1);
    }else{
       http_req->vlen = 0;
       http_req->version = 0;
    }

    end_pnt = memfind(data, dlen, end, 4);
    while(data<end_pnt){
       pnt_temp = memfind(data, dlen, ws, 2);
       if(pnt_temp){
          klen =  (pnt_temp - data) + 1;
          key = (char*)malloc(klen);
          memcpy(key, data, klen-1);
          key[klen-1] = 0;
          dlen -= ((pnt_temp + 2) - data);
          data = pnt_temp + 2;
          pnt = memfind(data, dlen, sep, 2);
          if(pnt){
             vlen = (pnt - data + 1);
             val = (uchar_t *)malloc(vlen);
             memcpy(val, data, vlen - 1);
             val[vlen - 1] = 0;
             data = pnt + 2;
             dlen -= (vlen + 1);
             node = create_hash_node(key, klen, val, vlen, 0);
             //printf("**%s -> %s\n", key, val);
             //printf("%d\n", http_req->fields);
             hash_tab_insert(http_req->fields, node);
             //printf("Here!!!\n");

          }else{
             free(key);
          }
       }
    }

    http_req->data = data;
    http_req->dlen = dlen;

    return 0;
}

http_request_t * create_http_request()
{
   http_request_t *http_req = (http_request_t *)malloc(sizeof(http_request_t));
   http_req->fields = create_hash_tab(10, hash_tab_compare);
   http_req->method = 0;
   http_req->uri = 0;
   http_req->version = 0;
   return http_req;
}

void destroy_http_request(http_request_t **http_req)
{
   if(http_req && (*http_req)){
      free((*http_req)->uri);
      free((*http_req)->version);

      destroy_hash_tab_nodes((*http_req)->fields);
      destroy_hash_tab(&((*http_req)->fields));

      (*http_req) = 0;
   }
}

void fprint_http_request(FILE *out, http_request_t *http_req)
{
   hash_node_t *hnode;
   int i, j;
   fprintf(out, "[Hypertext Transfer Protocol]\n");
   fprintf(out, "Method              : ");
   switch(http_req->method){
   case HTTP_METHOD_GET:
      printf("GET\n");
      break;
   case HTTP_METHOD_POST:
      printf("POST\n");
      break;
   case HTTP_METHOD_PUT:
      printf("PUT\n");
      break;
   case HTTP_METHOD_HEAD:
      printf("HEAD\n");
      break;
   case HTTP_METHOD_OPTIONS:
      printf("OPTIONS\n");
      break;
   case HTTP_METHOD_DELETE:
      printf("DELETE\n");
      break;
   case HTTP_METHOD_TRACE:
      printf("TRACE\n");
      break;
   case HTTP_METHOD_CONNECT:
      printf("CONNECT\n");
      break;
   case HTTP_METHOD_PATCH:
      printf("PATCH\n");
      break;
   default:
      printf("None\n");
      break;
   }
   fprintf(out, "URI                 : %s\n", http_req->uri);
   fprintf(out, "Version             : %s\n", http_req->version);
   for(i=0; i<http_req->fields->len; i++){
      hnode = http_req->fields->buckets[i];
      while(hnode!=0){
         fprintf(out, "%s", hnode->key, hnode->klen);
         for(j=hnode->klen-1; j<20; j++)
            putc(' ', out);
         fprintf(out, ": %s\n", hnode->val);
         hnode = hnode->next;
      }
   }
}

__inline void print_http_request(http_request_t *http_req)
{
   fprint_http_request(stdout, http_req);
}

int decode_http_reply(http_reply_t *http_rep, byte_t *data, uint_t dlen)
{
   hash_node_t *node;
   byte_t *key;
   uint_t klen;
   byte_t *val;
   uint_t vlen;

   byte_t *pnt;
   byte_t *pnt_temp;
   byte_t *end_pnt;
   byte_t sep[2] = {0x0d, 0x0a}; //default separator
   byte_t ws[2] = {0x3a, 0x20}; //default white space ": "
   byte_t end[4] = {0x0d, 0x0a, 0x0d, 0x0a};

   char str[255];

   if(!dlen || !data[0]){
      return 1;
   }

   if(dlen<5){
      return 2;
   }


   if(data[0]!='H' || data[1]!='T' || data[2]!='T' || data[3]!='P' || data[4]!='/'){
      return 3;
   }

   pnt = memchr(data, ' ', dlen);
   if(pnt){
      http_rep->vlen = (pnt - data + 1);
      http_rep->version = (uchar_t *)malloc(http_rep->vlen);
      memcpy(http_rep->version, data, http_rep->vlen - 1);
      http_rep->version[http_rep->vlen - 1] = 0;
      data += http_rep->vlen;
      dlen -= http_rep->vlen;
   }else{
      http_rep->vlen = 0;
      http_rep->version = 0;
   }

   pnt = memchr(data, ' ', dlen);
   if(pnt){
      memset(str, 0, 255);
      memcpy(str, data, (pnt - data));
      http_rep->code = atoi(str);
      data += (pnt - data + 1);
      dlen -= (pnt - data + 1);
   }else{
      http_rep->code = 0;
   }

   pnt = memfind(data, dlen, sep, 2);
   if(pnt){
      http_rep->msglen = (pnt - data + 1);
      http_rep->msg = (uchar_t *)malloc(http_rep->msglen);
      memcpy(http_rep->msg, data, http_rep->msglen - 1);
      http_rep->msg[http_rep->msglen - 1] = 0;
      data = pnt + 2;
      dlen -= (http_rep->msglen + 1);
   }else{
      http_rep->msglen = 0;
      http_rep->msg = 0;
   }

   end_pnt = memfind(data, dlen, end, 4);
   while(data<end_pnt){
      pnt_temp = memfind(data, dlen, ws, 2);
      if(pnt_temp){
         klen =  (pnt_temp - data) + 1;
         key = (char*)malloc(klen);
         memcpy(key, data, klen-1);
         key[klen-1] = 0;
         dlen -= ((pnt_temp + 2) - data);
         data = pnt_temp + 2;
         pnt = memfind(data, dlen, sep, 2);
         if(pnt){
            vlen = (pnt - data + 1);
            val = (uchar_t *)malloc(vlen);
            memcpy(val, data, vlen - 1);
            val[vlen - 1] = 0;
            data = pnt + 2;
            dlen -= (vlen + 1);
            node = create_hash_node(key, klen, val, vlen, 0);
            //printf("**%s -> %s\n", key, val);
            hash_tab_insert(http_rep->fields, node);
         }else{
            free(key);
         }
      }
   }

   http_rep->data = data;
   http_rep->dlen = dlen;
   //dump(data, dlen);

   return 0;
}

http_reply_t * create_http_reply()
{
   http_reply_t *http_rep = (http_reply_t *)malloc(sizeof(http_reply_t));
   http_rep->fields = create_hash_tab(10, hash_tab_compare);
   http_rep->msg = 0;
   http_rep->version = 0;
   return http_rep;
}

void destroy_http_reply(http_reply_t **http_rep)
{
   if(http_rep && (*http_rep)){
      free((*http_rep)->msg);
      free((*http_rep)->version);

      destroy_hash_tab_nodes((*http_rep)->fields);
      destroy_hash_tab(&((*http_rep)->fields));

      (*http_rep) = 0;
   }
}

char *get_http_reply_field(http_reply_t *http_rep, char *key)
{
   hash_node_t *hnode = hash_tab_lookup(http_rep->fields, (byte_t*)key, strlen(key)+1);
   if(hnode){
      //printf("%s => %s\n", (char*)hnode->key, (char*)hnode->val);
      return hnode->val;
   }else return 0;
}

void fprint_http_reply(FILE *out, http_reply_t *http_rep)
{
   hash_node_t *hnode;
   int i, j;
   fprintf(out, "Version             : %s\n", http_rep->version);
   fprintf(out, "Code                : %d\n", http_rep->code);
   fprintf(out, "Message             : %s\n", http_rep->msg);
   for(i=0; i<http_rep->fields->len; i++){
      hnode = http_rep->fields->buckets[i];
      while(hnode!=0){
         fprintf(out, "%s", hnode->key, hnode->klen);
         for(j=hnode->klen-1; j<20; j++)
            putc(' ', out);
         fprintf(out, ": %s\n", hnode->val);
         hnode = hnode->next;
      }
   }
   dump(http_rep->data, http_rep->dlen);
}

__inline void print_http_reply(http_reply_t *http_rep)
{
   fprint_http_reply(stdout, http_rep);
}
