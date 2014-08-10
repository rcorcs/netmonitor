/*
 * net/dns.h
 *            Global definitions for Domain Name System Headers.
 * version:   2.0.0
 * date:      08 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
 *
 * Changes:
 *   12 September 2011   changed pattern of names
*/
#ifndef __DNS
#define __DNS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../../datatypes/primitives.h"

/* QR, Query/Response. 1 bit. */
#define DNS_QR_QUERY       0 /* Query. */
#define DNS_QR_RESPONSE    1 /* Response. */

/* Opcode. 4 bits */
#define DNS_OP_STD_QUERY   0 /* QUERY, Standard query. - RFC 1035 */
#define DNS_OP_INV_QUERY   1 /* IQUERY, Inverse query. - RFC 1035, RFC 3425 */
#define DNS_OP_STATUS      2 /* STATUS, Server status request. - RFC 1035 */
                         /* 3 NOT USED */
#define DNS_OP_NOTIFY      4 /* Notify. - RFC 1996 */
#define DNS_OP_UPDATE      5 /* Update. - RFC 2136 */
                         /* 6..15 RESERVED */

/* AA, Authoritative Answer. 1 bit. */
#define DNS_AA_NOT_AUTH    0
#define DNS_AA_AUTH        1  /* Is authoritative */

/* TC, Truncated. 1 bit. - Indicates that only the first 512 bytes of the reply was returned. */
#define DNS_TC_NOT_TRUNC   0
#define DNS_TC_TRUNC       1

/* RD, Recursion Desired. 1 bit. */
#define DNS_RD_NOT_DESIRED 0
#define DNS_RD_DESIRED     1

/* RA, Recursion Available. 1 bit. */
#define DNS_RD_NOT_AV      0 /* Recursive query support not available. */
#define DNS_RD_AVAILABLE   1 /* Recursive query support available. */

/* Z. 1 bit. */
// "Dust"

/* AD, Authenticated data. 1 bit. */
// Boolean value

/* CD, Checking Disabled. 1 bit. */
// Boolean value

/* Rcode, Return code. 4 bits. */
#define DNS_RC_NO_ERR      0   /* (RFC 1035) No error. The request completed successfully. */
#define DNS_RC_FERROR      1   /* (RFC 1035) Format error. The name server was unable to interpret the query. */
#define DNS_RC_SERV_FAIL   2   /* (RFC 1035) Server failure. The name server was unable to process this query due to a problem with the name server. */
#define DNS_RC_NAME_ERR    3   /* (RFC 1035) Name Error. Meaningful only for responses from an authoritative name server, this code signifies that the domain name referenced in the query does not exist. */
#define DNS_RC_NOT_IMPL    4   /* (RFC 1035) Not Implemented. The name server does not support the requested kind of query. */
#define DNS_RC_REFUSED     5   /* (RFC 1035) Refused. The name server refuses to perform the specified operation for policy reasons. */
#define DNS_RC_YXDOMAIN    6   /* (RFC 2136) YXDomain. Name Exists when it should not. */
#define DNS_RC_YXRRSET     7   /* (RFC 2136) YXRRSet. RR Set Exists when it should not.*/
#define DNS_RC_NXRRSET     8   /* (RFC 2136) NXRRSet. RR Set that should exist does not. */
#define DNS_RC_NOT_AUTH    9   /* (RFC 2136) NotAuth. Server Not Authoritative for zone. */
#define DNS_RC_NOT_ZONE    10  /* (RFC 2136) NotZone. Name not contained in zone. */
                         /* 11..15 */
#define DNS_RC_BADVERS     16  /* (RFC 2671) BADVERS.Bad OPT Version. */
#define DNS_RC_BADSIG      16  /* (RFC 2845) BADSIG.TSIG Signature Failure. */
#define DNS_RC_BADKEY      17  /* (RFC 2845) BADKEY. Key not recognized. */
#define DNS_RC_BADTIME     18  /* (RFC 2845) BADTIME. Signature out of time window. */
#define DNS_RC_BADMODE     19  /* (RFC 2930) BADMODE. Bad TKEY Mode. */
#define DNS_RC_BADNAME     20  /* (RFC 2930) BADNAME. Duplicate key name. */
#define DNS_RC_BADALG      21  /* (RFC 2930) BADALG. Algorithm not supported. */
#define DNS_RC_BADTRUNC    22  /* (RFC 4635) BADTRUNC. Bad truncation. */
                         /* 23..3840 Not used */
                         /* 3841..4095 Private use. */
                         /* 4096..65535 Not used */

#define DNS_HDR_LEN 12
typedef struct __dns_hdr {
   uint16_t  id;         /* Identification - Used to match request/reply packets. */
   //uint16_t  flags;    /* FLAGS - [QR:1][Opcode:4][AA:1][TC:1][RD:1] : [RA:1][Z:1][AD:1][CD:1][Rcode:4]*/
   uint8_t   qrbit:1,    /* QR, Query/Response. 1 bit. */
             opcode:4,   /* Opcode. 4 bits */
			 aabit:1,    /* AA, Authoritative Answer. 1 bit. */
			 tcbit:1,    /* TC, Truncated. 1 bit. - Indicates that only the first 512 bytes of the reply was returned. */
			 rdbit:1;    /* RD, Recursion Desired. 1 bit. */
   uint8_t   rabit:1,    /* RA, Recursion Available. 1 bit. */
             zbit:1,     /* Z. 1 bit. */
			 adbit:1,    /* AD, Authenticated data. 1 bit. */
			 cdbit:1,    /* CD, Checking Disabled. 1 bit. */
			 rcode:4;    /* Rcode, Return code. 4 bits. */
   uint16_t  tq;         /* Total Questions */
   uint16_t  tans;       /* Total Answer RRs */
   uint16_t  tauth;      /* Total Authority RRs */
   uint16_t  tadd;       /* Total Additional RRs */
} dns_hdr_t;

void fprint_dns_hdr(FILE *, dns_hdr_t *);
__inline void print_dns_hdr(dns_hdr_t *);

typedef struct __dns_query {
   uchar_t   *name; /* query name */
   uint_t     namelen;
   uint16_t  type;                     /* type */
   uint16_t  c; /*class*/              /* class */
} dns_query_t;

typedef struct __dns_rr {
   uchar_t   *name;   /* rr name */
   uint_t    namelen;
   uint16_t  type;                     /* type */
   uint16_t  c; /*class*/              /* class */
   uint32_t  ttl;
   uint16_t  rdlen;
   byte_t    *rdata;
} dns_rr_t;

typedef struct __dns_msg {
   dns_hdr_t    *dnsh;
   dns_query_t  *dnsq;
   dns_rr_t     *dnsans;
   dns_rr_t     *dnsauth;
   dns_rr_t     *dnsadd;
} dns_msg_t;

void fprint_dns_msg(FILE *, dns_msg_t *);
__inline void print_dns_msg(dns_msg_t *);

uint_t decode_dns_msg(dns_msg_t *, byte_t *);
void destroy_dns_msg(dns_msg_t *);

#define DNS_TYPE_A       1
#define DNS_TYPE_NS      2
#define DNS_TYPE_MD      3
#define DNS_TYPE_MF      4
#define DNS_TYPE_CNAME   5
#define DNS_TYPE_SOA     6
/*
Type	Description	References
0	Not Used
1	A, IPv4 address.	RFC 1035
2	NS, Authoritative name server.	RFC 1035
3	MD, Mail destination. Obsolete use MX instead.	RFC 1035
4	MF, Mail forwarder. Obsolete use MX instead.	RFC 1035
5	CNAME, Canonical name for an alias.	RFC 1035
6	SOA, Marks the start of a zone of authority.	RFC 1035
7	MB, Mailbox domain name.	RFC 1035
8	MG, Mail group member.	RFC 1035
9	MR, Mail rename domain name.	RFC 1035
10	NULL, Null resource record.	RFC 1035
11	WKS, Well known service description.	RFC 1035
12	PTR, Domain name pointer.	RFC 1035
13	HINFO, Host information.	RFC 1035
14	MINFO, Mailbox or mail list information.	RFC 1035
15	MX, Mail exchange.	RFC 1035
16	TXT, Text strings.	RFC 1035
17	RP, Responsible Person.	RFC 1183
18	AFSDB, AFS Data Base location.	RFC 1183, RFC 5864
19	X25, X.25 PSDN address.	RFC 1183
20	ISDN, ISDN address.	RFC 1183
21	RT, Route Through.	RFC 1183
22	NSAP, NSAP address. NSAP style A record.	RFC 1706
23	NSAP-PTR.	RFC 1348
24	SIG, Security signature.	RFC 2931, RFC 4034
25	KEY, Security key.	RFC 3445, RFC 4034
26	PX, X.400 mail mapping information.	RFC 2163
27	GPOS, Geographical Position.	RFC 1712
28	AAAA, IPv6 Address.	RFC 3596
29	LOC, Location Information.	RFC 1876
30	NXT, Next Domain (obsolete).	RFC 2535
31	EID, Endpoint Identifier.
32  NIMLOC, Nimrod Locator.
    NB, NetBIOS general Name Service. RFC 1002

33	SRV, Server Selection.
    NBSTAT, NetBIOS NODE STATUS. RFC 2052, RFC 2782 RFC 1002
34	ATMA, ATM Address.
35	NAPTR, Naming Authority Pointer.	RFC 3403
36	KX, Key Exchanger.	RFC 2230
37	CERT.	RFC 2538, RFC 4398
38	A6.	RFC 2874, RFC 3226
39	DNAME.	RFC 2672
40	SINK.
41	OPT.	RFC 2671
42	APL.	RFC 3123
43	DS, Delegation Signer.	RFC 3658
44	SSHFP, SSH Key Fingerprint.	RFC 4255
45	IPSECKEY.	RFC 4025
46	RRSIG.	RFC 3755
47	NSEC, NextSECure.	RFC 3755, RFC 3845
48	DNSKEY.	RFC 3755
49	DHCID, DHCP identifier.	RFC 4701
50	NSEC3.	RFC 5155
51	NSEC3PARAM.	RFC 5155
52  Not Used
53  Not Used
54  Not Used
55	HIP, Host Identity Protocol.	RFC 5205
56	NINFO.
57	RKEY.
58	TALINK, Trust Anchor LINK.
59..98 Not Used
99	SPF, Sender Policy Framework.	RFC 4408
100	UINFO.
101	UID.
102	GID.
103	UNSPEC.
104..248 Not Used
249	TKEY.	RFC 2930
250	TSIG, Transaction Signature.	RFC 2845, RFC 3645
251	IXFR, Incremental transfer.	RFC 1995
252	AXFR, A request for a transfer of an entire zone.	RFC 1035
253	MAILB, A request for mailbox-related records (MB, MG or MR).	RFC 1035
254	MAILA, A request for mail agent RRs. Obsolete.	RFC 1035
255	*. A request for all records.	RFC 1035
256..32767 Not Used
32768	DNSSEC Trust Authorities.
32769	DNSSEC Lookaside Validation.	RFC 4431, RFC 5074
Class. 16 bits, unsigned.

Class	Description	References
0	Reserved.	RFC 5395
1	IN, Internet.	RFC 1035.
2   Not Used
3	CH, Chaos.	RFC 1035.
4	HS, Hesiod.	RFC 1035.
5..253 Not Used
254	None.	RFC 2136.
255	Any (QCLASS only).	RFC 1035.
256..65279 Not Used
65280..65534 Private use.	RFC 5395
65535 Not Used RFC 5395

*/
//void fprint_dns(FILE *, cdnshdr *);
//void print_dns(cdnshdr *);

#ifdef __cplusplus
}
#endif

#endif

