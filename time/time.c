#include "time.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tm *current_time()
{
   time_t     now;
   struct tm *t;

   /* Get the current time */
   now = time(0);

   /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
   t = localtime(&now);
   return t;
}


void fprint_time(FILE *out, struct tm *t)
{
   #ifdef __WIN32
   char buffer[80];
   strftime(buffer, sizeof(buffer), "%a %Y-%m-%d %H:%M:%S %Z", t);
   fprintf(out, "%s", buffer);
   #else
   fprintf(out, "%d-%d-%d %d:%d:%d", (1900+t->tm_year), t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
   #endif
}

__inline void print_time(struct tm *t)
{
    fprint_time(stdout, t);
}


#ifdef __cplusplus
}
#endif
