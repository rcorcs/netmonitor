#ifndef __DOSC
#define __DOSC

#include <stdio.h>

#include "doscp/packetp.h"
#include "../../ml/nbc.h"
#include "../../datatypes/list.h"
#include "../../datatypes/vec.h"
#include "../../datatypes/mat.h"
#include "../../datatypes/node.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __dosc {
    nbc_t *tcpsyn_nbc;
    nbc_t *udp_nbc;
    //nbc_t *icmp_nbc;
} dosc_t;

void destroy_dosc(dosc_t **dosc);

dosc_t *load_dosc(FILE *in);
void store_dosc(FILE *out, dosc_t *dosc);

void dos_classify(dosc_t *dosc, packetp_t *pktp, struct tm *time);

void train_tcpsyn_nbc(nbc_t *nbc, FILE *in_normal, FILE *in_attack);
void train_udp_nbc(nbc_t *nbc, FILE *in_normal, FILE *in_attack);

void train_dosc(dosc_t *dosc, char *dosc_file, char **tcpsyn_files, char **udp_files);
void train_tcpsyn(nbc_t *nbc, char *normal_file, char *attack_file);
void train_udp(nbc_t *nbc, char *normal_file, char *attack_file);

#ifdef __cplusplus
}
#endif

#endif

