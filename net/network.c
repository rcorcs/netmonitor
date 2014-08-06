/*
 * cnetwork.c
 * version:   1.0.0
 * date:      06 September 2011
 * author:    (rcor) Rodrigo Caetano de Oliveira Rocha
*/
#include "network.h"

//TEMPORARY
pcap_if_t *get_all_devices()
{
  char errbuf[PCAP_ERRBUF_SIZE];
  /* ask pcap to find a valid device for use to sniff on */
  pcap_if_t *alldevsp;

  //dev = pcap_lookupdev(errbuf);
  printf("Finding available devices ... \n");
  if(pcap_findalldevs(&alldevsp, errbuf)) {
     printf("Error finding devices : %s\n" , errbuf);
     exit(1);
  }
  return alldevsp;
}

//TEMPORARY
void list_all_devices()
{
  char errbuf[PCAP_ERRBUF_SIZE];
  /* ask pcap to find a valid device for use to sniff on */
  pcap_if_t *alldevsp;
  pcap_if_t *device;
  int count = 1;
  //dev = pcap_lookupdev(errbuf);
  printf("Finding available devices ... \n");
  if(pcap_findalldevs(&alldevsp, errbuf)) {
     printf("Error finding devices : %s\n" , errbuf);
     exit(1);
  }
  device = alldevsp;
//  if(device != NULL)
 //    dev = device->name;
  while(device != NULL){
     //*(devs + count) = device->name;
     printf("%d. %s - %s\n", count++ , device->name , device->description);
     device = device->next;
  }
}

//TEMPORARY
void device_info()
{
  char *dev; /* name of the device to use */
  char *net; /* dot notation of the network address */
  char *mask;/* dot notation of the network mask    */
  int ret;   /* return code */
  char errbuf[PCAP_ERRBUF_SIZE];
  bpf_u_int32 netp; /* ip          */
  bpf_u_int32 maskp;/* subnet mask */
  struct in_addr addr;
  /* ask pcap to find a valid device for use to sniff on */
  dev = pcap_lookupdev(errbuf);
  /* error checking */
  if(dev == NULL)
  {
   printf("%s\n",errbuf);
   exit(1);
  }

  /* print out device name */
  printf("DEV: %s\n",dev);

  /* ask pcap for the network address and mask of the device */
  ret = pcap_lookupnet(dev,&netp,&maskp,errbuf);
  if(ret == -1)
  {
   printf("%s\n",errbuf);
   exit(1);
  }
  /* get the network address in a human readable form */
  addr.s_addr = netp;
  net = inet_ntoa(addr);
  if(net == NULL)/* thanks Scott :-P */
  {
    perror("inet_ntoa");
    exit(1);
  }
  printf("NET: %s\n",net);
  /* do the same as above for the device's mask */
  addr.s_addr = maskp;
  mask = inet_ntoa(addr);

  if(mask == NULL)
  {
    perror("inet_ntoa");
    exit(1);
  }

  printf("MASK: %s\n",mask);
}
