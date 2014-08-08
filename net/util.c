#include "util.h"

//fatal error
void fatal(const char *msg)
{
   char error_msg[strlen(msg)+20];
   strcpy(error_msg, "[!!] Fatal Error ");
   strcat(error_msg, msg);
   perror(error_msg);
   exit(-1);
}

//malloc with error checking
void *ec_malloc(unsigned int size)
{
   void *ptr = malloc(size);
   if(ptr==NULL)
      fatal("in ec_malloc() on memory allocation");
   return ptr;
}

void ffdump(FILE *stream, const int format, const byte_t *data_buffer, const unsigned int length)
{
   const char *format_str;
   const char *format_ws_str;
   int format_limit;
   byte_t byte;
   unsigned int i, j;

   switch(format){
      case DEC:
        format_limit = 12;
        format_str = "%03d ";
	format_ws_str = "    ";
        break;
     case OCT:
        format_limit = 12;
        format_str = "%03o ";
	format_ws_str = "    ";
        break;
     case HEX:
     default:
        format_limit = 16;
        format_str = "%02x ";
	format_ws_str = "   ";
        break;
   }

   for( i = 0; i<length; i++){
      byte = data_buffer[i];
      fprintf(stream, format_str, byte);
      if( ((i%format_limit)==(format_limit-1)) || (i==length-1) ){
         for(j=0; j<(format_limit-1)-(i%format_limit); j++)
            fputs(format_ws_str, stream);
         fputs("| ", stream);
         for(j=(i-(i%format_limit)); j<=i; j++){
            byte = data_buffer[j];
            if( (byte>31) && (byte<127))
               fputc(byte, stream);
            else fputc('.', stream);
         }
         fprintf(stream, "\n");
      }
   }
}

__inline void fdump(FILE *stream, const byte_t *data_buffer, const unsigned int length)
{
   /*
   unsigned char byte;
   unsigned int i, j;
   for( i = 0; i<length; i++){
      byte = data_buffer[i];
      fprintf(stream, "%02x ", byte);
      if( ((i%16)==15) || (i==length-1) ){
         for(j=0; j<15-(i%16); j++)
            fprintf(stream, "   ");
         fprintf(stream, "| ");
         for(j=(i-(i%16)); j<=i; j++){
            byte = data_buffer[j];
            if( (byte>31) && (byte<127))
               fprintf(stream, "%c", byte);
            else fprintf(stream, ".");
         }
         fprintf(stream, "\n");
      }
   }
   */
   ffdump(stream, HEX, data_buffer, length);
}

__inline void dump(const byte_t *data_buffer, const unsigned int length)
{
   fdump(stdout, data_buffer, length);
}


byte_t *memfind(byte_t *data, uint_t dlen, byte_t *pattern, uint_t plen)
{
   int i, j;
   char found = 0;
   for(i = 0; i<dlen; i++){
      found = 1;
      for(j=0; j<plen; j++){
         if(data[i+j]!=pattern[j]){
            found = 0;
            break;
         }
      }
      if(found){
         return (data+i);
      }
   }
   return 0;
}
