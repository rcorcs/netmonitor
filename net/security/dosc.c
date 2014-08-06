
#include "dosc.h"

#include "../../ml/nbcio.h"

void dos_classify_tcpsyn(nbc_t *nbc, hash_tab_t *htab, struct tm *time);
void dos_classify_udp(nbc_t *nbc, hash_tab_t *htab, struct tm *time);
void print_vec(vec_t *vec);
void print_mat(mat_t *mat);

void destroy_dosc(dosc_t **dosc)
{
   destroy_nbc(&((*dosc)->tcpsyn_nbc));
   destroy_nbc(&((*dosc)->udp_nbc));
   free(*dosc);
   *dosc = 0;
}

dosc_t *load_dosc(FILE *in)
{
   dosc_t *dosc = (dosc_t *)malloc(sizeof(dosc_t));
   dosc->tcpsyn_nbc = create_nbc(2, 1);
   dosc->udp_nbc = create_nbc(2, 1);
   if(in) {
      load_nbc(in, dosc->tcpsyn_nbc);
      load_nbc(in, dosc->udp_nbc);
      printf("TCP SYN Trained:\n");
      print_mat(dosc->tcpsyn_nbc->training);
      printf("UDP Trained:\n");
      print_mat(dosc->udp_nbc->training);
   }
   return dosc;
}

void store_dosc(FILE *out, dosc_t *dosc)
{
   store_nbc(out, dosc->tcpsyn_nbc);
   store_nbc(out, dosc->udp_nbc);
}

void dos_classify(dosc_t *dosc, packetp_t *pktp, struct tm *time)
{
   dos_classify_tcpsyn(dosc->tcpsyn_nbc, pktp->tcp_htab, time);
   dos_classify_udp(dosc->udp_nbc, pktp->udp_htab, time);
}

void print_vec(vec_t *vec)
{
   int i;
   for(i=0; i<vec->len; ++i){
       printf("%f\t", VECGET(double, vec, i));
   }
   printf("\n");
}

void print_mat(mat_t *mat)
{
   int i, j;
   for(i=0; i<mat->rows; ++i){
      for(j=0; j<mat->cols; ++j){
         printf("%f\t", MATGET(double, mat, i, j));
      }
      printf("\n");
   }
}

void dos_classify_tcpsyn(nbc_t *nbc, hash_tab_t *htab, struct tm *time)
{
   hash_node_t *hnode;
   uint_t i;
   tcpp_t * tcpp;
   vec_t  * vec;
   int isattack;
   //printf("here1\n");
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
          tcpp = (tcpp_t *)hnode->val;
          if(tcpp){
             extern FILE *tempfile;
             //printf("here2\n");
             vec = create_tcpsyn_vec(&tcpp->out);


             fprintf(tempfile, "TCP\n");
             isattack = nbc_classify(nbc, vec);

             //print_vec(vec);
             destroy_vec(&vec);
             //printf("class: %d\n", c);
             if(isattack){
                printf("TCP SYN Flood Attack: ");
                if(hnode->klen==IPV4_ADDR_LEN){
                   printf("%s (", ipv4_addr_to_c_str((ipv4_addr_t*)hnode->key));
                }else if(hnode->klen==IPV6_ADDR_LEN){
                   printf("%s (", ipv6_addr_to_c_str((ipv6_addr_t*)hnode->key));
                }
                print_time(time);
                printf(")\n");
                //printf("\n");
                //exit(0);
             }
          }
      }
   }
}

void dos_classify_udp(nbc_t *nbc, hash_tab_t *htab, struct tm *time)
{
   hash_node_t *hnode;
   uint_t i;
   udpp_t * udpp;
   vec_t  * vec;
   int isattack;
   //printf("here1\n");
   for(i=0; i<htab->len; i++){
      for(hnode = htab->buckets[i]; hnode!=0; hnode = hnode->next){
          udpp = (udpp_t *)hnode->val;
          if(udpp){
             extern FILE *tempfile;

             //printf("here2\n");
             vec = create_udp_vec(&udpp->out);

             fprintf(tempfile, "UDP\n");
             isattack = nbc_classify(nbc, vec);
             //print_vec(vec);
             destroy_vec(&vec);
             //printf("class: %d\n", c);
             if(isattack){
                printf("UDP Flood Attack: ");
                if(hnode->klen==IPV4_ADDR_LEN){
                   printf("%s (", ipv4_addr_to_c_str((ipv4_addr_t*)hnode->key));
                }else if(hnode->klen==IPV6_ADDR_LEN){
                   printf("%s (", ipv6_addr_to_c_str((ipv6_addr_t*)hnode->key));
                }
                print_time(time);
                printf(")\n");
                //printf("\n");
                //exit(0);
             }
          }
      }
   }
}

void train_tcpsyn_nbc(nbc_t *nbc, FILE *in_normal, FILE *in_attack)
{
   list_t *list = create_list();
   mat_t *mat;

   //printf("TRAINING NORMAL\n");

   load_tcpp_file(in_normal, list);

   mat = create_tcpsyn_mat(list);

   destroy_list_nodes(list);
   destroy_list(&list);

   //print_mat(mat);
   nbc_train(nbc, 0, mat);

   destroy_mat(&mat);

   //printf("TRAINED NORMAL\n");

   //printf("TRAINING ATTACK\n");

   list = create_list();
   load_tcpp_file(in_attack, list);

   mat = create_tcpsyn_mat(list);

   destroy_list_nodes(list);
   destroy_list(&list);

   //print_mat(mat);
   nbc_train(nbc, 1, mat);

   destroy_mat(&mat);

   printf("TCP SYN Trained:\n");
   print_mat(nbc->training);
}

void train_udp_nbc(nbc_t *nbc, FILE *in_normal, FILE *in_attack)
{
   list_t *list = create_list();
   mat_t *mat;

   //printf("TRAINING NORMAL\n");

   load_udpp_file(in_normal, list);

   mat = create_udp_mat(list);

   destroy_list_nodes(list);
   destroy_list(&list);

   //print_mat(mat);
   nbc_train(nbc, 0, mat);

   destroy_mat(&mat);

   //printf("TRAINED NORMAL\n");

   //printf("TRAINING ATTACK\n");

   list = create_list();
   load_udpp_file(in_attack, list);

   mat = create_udp_mat(list);

   destroy_list_nodes(list);
   destroy_list(&list);

   //print_mat(mat);
   nbc_train(nbc, 1, mat);

   destroy_mat(&mat);

   printf("UDP Trained:\n");
   print_mat(nbc->training);

}



void train_dosc(dosc_t *dosc, char *dosc_file, char **tcpsyn_files, char **udp_files)
{
   FILE *fdosc;

   train_tcpsyn(dosc->tcpsyn_nbc, tcpsyn_files[0], tcpsyn_files[1]);
   train_udp(dosc->udp_nbc, udp_files[0], udp_files[1]);

   fdosc = fopen(dosc_file, "wb");
   if(fdosc){
      store_dosc(fdosc, dosc);
      fclose(fdosc);
   }else{
      printf("Could not create the training file: %s\n", dosc_file);
   }
}

void train_tcpsyn(nbc_t *nbc, char *normal_file, char *attack_file)
{
   //FILE *ftcpp_normal = fopen("tcpp_normal.dat", "rb");
   //FILE *ftcpp_attack = fopen("tcpp_attack.dat", "rb");
   FILE *ftcpp_normal = fopen(normal_file, "rb");
   FILE *ftcpp_attack = fopen(attack_file, "rb");

   if(!ftcpp_normal || !ftcpp_attack){
      if(!ftcpp_normal){
         printf("Could not find the training file: %s\n", normal_file);
      }else fclose(ftcpp_normal);
      if(!ftcpp_attack){
         printf("Could not find the training file: %s\n", attack_file);
      }else fclose(ftcpp_attack);
      exit(1);
   }

   train_tcpsyn_nbc(nbc, ftcpp_normal, ftcpp_attack);


   fclose(ftcpp_normal);
   fclose(ftcpp_attack);
}


void train_udp(nbc_t *nbc, char *normal_file, char *attack_file)
{
   FILE *fudpp_normal = fopen(normal_file, "rb");
   FILE *fudpp_attack = fopen(attack_file, "rb");

   if(!fudpp_normal || !fudpp_attack){
      if(!fudpp_normal){
         printf("Could not find the training file: %s\n", normal_file);
      }else fclose(fudpp_normal);
      if(!fudpp_attack){
         printf("Could not find the training file: %s\n", attack_file);
      }else fclose(fudpp_attack);
      exit(1);
   }

   train_udp_nbc(nbc, fudpp_normal, fudpp_attack);

   fclose(fudpp_normal);
   fclose(fudpp_attack);
}
