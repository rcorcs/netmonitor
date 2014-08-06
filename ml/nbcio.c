
#include "nbcio.h"

#ifdef __cplusplus
extern "C" {
#endif

void store_nbc(FILE *out, nbc_t *nbc)
{
   int i, j;
   fwrite(&nbc->nparam, sizeof(int), 1, out);
   fwrite(&nbc->nc, sizeof(int), 1, out);
   for(i=0; i<nbc->training->rows; ++i){
      for(j=0; j<nbc->training->cols; ++j){
         fwrite( &MATGET( double, (nbc->training), i, j ), sizeof(double), 1, out);
      }
   }
}

void load_nbc(FILE *in, nbc_t *nbc)
{
   int i, j;
   fread(&nbc->nparam, sizeof(int), 1, in);
   fread(&nbc->nc, sizeof(int), 1, in);
   if(nbc->training){
      if(nbc->training->rows!=nbc->nc || nbc->training->cols!=(2*nbc->nparam))
         destroy_mat(&nbc->training);
   }
   if(!nbc->training){
      nbc->training = create_mat(sizeof(double), nbc->nc, 2*nbc->nparam);
   }
   for(i=0; i<nbc->training->rows; ++i){
      for(j=0; j<nbc->training->cols; ++j){
         fread( &MATGET( double, (nbc->training), i, j ), sizeof(double), 1, in);
      }
   }
}

#ifdef __cplusplus
}
#endif
