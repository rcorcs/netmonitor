
#ifndef __VLAN8021Q
#define __VLAN8021Q


#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#include "../../datatypes/primitives.h"

#define VLAN8021Q_HDR_LEN     6
typedef struct __vlan8021q_hdr {
   uint16_t   tpid;
   uint16_t   tci;
   uint16_t   type;
} vlan8021q_hdr_t;

void fprint_vlan8021q_hdr(FILE *, vlan8021q_hdr_t *);
__inline void print_vlan8021q_hdr(vlan8021q_hdr_t *);

#ifdef __cplusplus
}
#endif

#endif
