
#include "nbc.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>

#define GAUSSIAN(_X_, _M_, _D_) (exp((-pow((_X_)-(_M_), 2.0)/(2.0*(_D_)*(_D_))))/sqrt(2.0*3.1415926*(_D_)))

nbc_t *create_nbc(int nc, int nparam)
{
   nbc_t *nbc = (nbc_t*)malloc(sizeof(nbc_t));
   nbc->nparam = nparam;
   nbc->nc = nc;
   nbc->training = create_mat(sizeof(double), nc, 2*nparam); //mean and variance for each parameter
   zero_mat(nbc->training);
   return nbc;
}

void destroy_nbc(nbc_t **nbc)
{
   if(nbc){
      destroy_mat(&((*nbc)->training));
      free(*nbc);
      *nbc = 0;
   }
}

void nbc_train(nbc_t *nbc, int class, mat_t *data)
{
   int i, j;

   for(i=0; i<data->rows; i++){
      for(j=0;j<nbc->nparam; j++){
         MATGET( double, (nbc->training), class, j*2 ) += MATGET(double, data, i, j);
      }
   }
   for(j=0;j<nbc->nparam; j++){
      MATGET( double, (nbc->training), class, j*2 ) /= data->rows;
      //printf("%d: %f\n", j, MATGET( double, (nbc->training), class, j*2 ));
   }

   for(i=0; i<data->rows; i++){
      for(j=0;j<nbc->nparam; j++){
         MATGET( double, (nbc->training), class, j*2 + 1 ) += pow(MATGET(double, data, i, j) - MATGET( double, (nbc->training), class, j*2 ), 2.0);
      }
   }

   for(j=0;j<nbc->nparam; j++){
      MATGET( double, (nbc->training), class, j*2 + 1 ) /= (data->rows-1);
      MATGET( double, (nbc->training), class, j*2 + 1 ) = sqrt(MATGET( double, (nbc->training), class, j*2 + 1 ));
      //printf("%d: %f - %e\n", j, MATGET( double, (nbc->training), class, j*2 + 1 ), MATGET( double, (nbc->training), class, j*2 + 1 ));
   }
}

__inline double gaussian(double x, double m, double d)
{
   //double ex = (-pow(x-m, 2.0)/(2.0*d*d));
   //return (exp(ex)/sqrt(2.0*3.1415926*d));
   return (exp((-pow(x-m, 2.0)/(2.0*d*d)))/sqrt(2.0*3.1415926*d));
}

int nbc_classify(nbc_t *nbc, vec_t *sample)
{
   extern FILE *tempfile;
   int i, j;
   int class = -1;
   double max_posterior = -1.0;
   double posterior;

   fprintf(tempfile, "%f\n", VECGET(double, sample, 0));

   for(i=0; i<nbc->nc; i++){
      //posterior = 1.0/nbc->nc;
      posterior = 1.0;
      for(j=0;j<nbc->nparam; j++){
         //printf("samp: %f\n", VECGET(double, sample, j));
         //printf("mean: %f\n", MATGET( double, (nbc->training), i, j*2));
         //printf("dev: %e\n", MATGET( double, (nbc->training), i, j*2 + 1));

         double g = GAUSSIAN( VECGET(double, sample, j),
                                MATGET( double, (nbc->training), i, j*2),
                                MATGET( double, (nbc->training), i, j*2 + 1 )
                              );
         posterior *= g;
         //printf("%d: %f - %e\n", j, posterior, posterior);
      }

      fprintf(tempfile, "%f\n", posterior);
      //printf("class %d: %f - %e\n", i, posterior, posterior);
      if(class<0 || max_posterior<posterior){
         class = i;
         max_posterior = posterior;
      }
   }
   fprintf(tempfile, "\n");
   return class;
}


#ifdef __cplusplus
}
#endif
