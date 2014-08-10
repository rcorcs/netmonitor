/// c0a80067

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <pcap.h>

#include "time/time.h"
#include "time/timer.h"

#include "net/network.h"
#include "net/util.h"
#include "net/netutil.h"
#include "net/nethost.h"
#include "net/events/pparser.h"

#include "net/security/doscp/packetp.h"
#include "net/security/dosc.h"

void pcap_fatal(const char *, const char *);
void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);

#ifdef __WIN32
VOID CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
VOID TimerSignal(MSG msg);
void main_loop(pcap_t *pcap_handle);
#else
void timeout_handler(int signal_code);
#endif

typedef struct __settings {
   bool_t ds;    //debug_sniffer;
   bool_t dp;    //debug_parameter;
   bool_t dd;    //detect_dos;
   bool_t stcpp; //store tcp parameters
   bool_t sudpp; //store udp parameters
} settings_t;

typedef struct __pcap_param {
   net_host_t       * nethost;
   eth_addr_t         hwaddr_filter;
   packetp_t        * pktp;
   dosc_t           * dosc;
} pcap_param_t;

void handle_tick();
void finish();

void setup_signals();
void handle_signal(int signal);

//void count_ticks(packet_stats_t   * pktstats, packet_stats_t   * pktstats_instant);

pcap_param_t param;
pcap_t *pcap_handle;
int wintime_sec;
int wintime_msec;

settings_t config; //TODO

bool_t debug_sniffer;
bool_t debug_parameter;

bool_t detect_dos;
bool_t store_parameters;


//#define __SNIFFER_DEBUG    1
//#define __DEBUG_PARAMETER  2
//#define __STORE_PARAMETER  3
//#define __TRAIN_CLASSIFIER 4
//#define __DETECT_DOS       5

//#ifdef __STORE_PARAMETER
FILE *ftcpp;
FILE *fudpp;

FILE *tempfile;
//#endif
//#ifdef __TRAIN_CLASSIFIER

//#endif

void process_arg(int argc, char **argv);

int main(int argc, char **argv)
{
   process_arg(argc, argv);
   return 0;
}

int run(char *device, char *read_file, char *dosc_file, char **store_files)
{
   char errbuf[PCAP_ERRBUF_SIZE];
   net_host_t *nethost;

   wintime_sec = 1, wintime_msec = 0;

   if(!read_file){
      if(device == NULL)
         device = pcap_lookupdev(errbuf);

      if(device == NULL)
         pcap_fatal("pcap_lookupdev", errbuf);
   }
   //#ifdef __DETECT_DOS
   if(detect_dos)
   {
      //FILE *fdosc = fopen("dosc.dat", "rb");
      FILE *fdosc = fopen(dosc_file, "rb");
      if(!fdosc){
         printf("Could not open the file: %s\n", dosc_file);
         exit(1);
      }
      param.dosc = load_dosc(fdosc); //load dosc file
      fclose(fdosc);
   }
   //#endif

   //#ifdef __STORE_PARAMETER
   if(store_parameters){
      //ftcpp = fopen("tcpp.dat", "a+b");
      ftcpp = fopen(store_files[0], "a+b");
      if(!ftcpp){
         printf("Could not create the parameters file: %s\n", store_files[1]);
         exit(1);
      }

      fudpp = fopen(store_files[1], "a+b");
      if(!ftcpp){
         printf("Could not create the parameters file: %s\n", store_files[1]);
         exit(1);
      }
   }
   //#endif
   if(!read_file){
      nethost = get_host_info();

      if(nethost){
         uint_t i;
         ///print_net_host(nethost);
         param.nethost = nethost;
         #ifdef __WIN32
         param.hwaddr_filter = nethost->hwaddr;
         #else
         if(device==NULL)
            param.hwaddr_filter = get_net_host_by_name(nethost, "eth0")->hwaddr;
         else param.hwaddr_filter = get_net_host_by_name(nethost, device)->hwaddr;
         #endif
         //destroy_net_host(&nethost);
         param.pktp = create_packetp(64);
         //memset( &param.tcp_min, 0xFF, sizeof(tcp_stats_t));
         //memset( &param.tcp_max, 0, sizeof(tcp_stats_t));
         //memset( &param.udp_min, 0xFF, sizeof(udp_stats_t));
         //memset( &param.udp_max, 0, sizeof(udp_stats_t));
         //memset( &param.icmp_min, 0xFF, sizeof(icmp_stats_t));
         //memset( &param.icmp_max, 0, sizeof(icmp_stats_t));
         //param.dosscores = create_dos_scores(10);

         printf("HWAddr Filter: %02x", param.hwaddr_filter.addr[0]);
         for(i=1; i<ETH_ADDR_LEN; i++)
            printf(":%02x", param.hwaddr_filter.addr[i]);
         putchar('\n');
      }

      printf("Sniffing on device %s\n", device);
      pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf);
      if(pcap_handle == NULL)
         pcap_fatal("pcap_open_live", errbuf);
   }else{
      printf("Reading from file: %s\n", read_file);
      pcap_handle = pcap_open_offline(read_file, errbuf);
      if(pcap_handle == NULL)
         pcap_fatal("pcap_open_offline", errbuf);
   }

   tempfile = fopen("tempfile.txt", "w");

   parser_init();
   #ifdef __WIN32
   main_loop(pcap_handle);
   #else
   setup_signals(); //setup signals handler
   timer(wintime_sec, wintime_msec, timeout_handler);
   pcap_loop(pcap_handle, -1, caught_packet, (u_char *)&param); //-1 infinity loop
   #endif

   finish();
   return 0;
}

show_host_info(char *device)
{
   net_host_t *nethost;
   nethost = get_host_info();

   if(nethost){
      print_net_host(nethost);
   }
}

void finish()
{
   pcap_close(pcap_handle);

   fclose(tempfile);

   //#ifdef __DETECT_DOS
   if(param.dosc) destroy_dosc(&param.dosc);
   //#endif

   //#ifdef __STORE_PARAMETER
   if(ftcpp) fclose(ftcpp);
   //#endif

   if(param.pktp) destroy_packetp(&param.pktp);

   parser_close();
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet)
{
   packet_t pkt;
   pcap_param_t *param = (pcap_param_t *)user_args;

   decode_packet(&pkt, packet, cap_header->caplen);

   //#ifdef __SNIFFER_DEBUG
   if(debug_sniffer){
      printf("CAPTURED PACKET LENGTH: %hu of %hu\n", cap_header->caplen, cap_header->len);
      printf("TIME: %s",ctime((const time_t*)&cap_header->ts.tv_sec));
      //dump(packet, cap_header->caplen); //caplen: captured portion packet length
      print_packet(&pkt);
   }
   //#endif

   parse_packet(&pkt, cap_header);

   if( !memcmp(pkt.ethh->src.addr, param->hwaddr_filter.addr, ETH_ADDR_LEN) ){
       packetp_add_out_info(param->pktp, &pkt);
   }else if( !memcmp(pkt.ethh->dst.addr, param->hwaddr_filter.addr, ETH_ADDR_LEN) ){
       packetp_add_in_info(param->pktp, &pkt);
   }
}

void pcap_fatal(const char *failed_in, const char *errbuf)
{
   printf("Fatal Error in %s : %s\n", failed_in, errbuf);
   exit(1);
}

#ifdef __WIN32
void main_loop(pcap_t *pcap_handle)
{
   UINT timerid;
   packet_t pkt;
   u_char *packet = NULL;
   struct pcap_pkthdr *cap_header = (struct pcap_pkthdr *)malloc(sizeof(struct pcap_pkthdr));

   timerid = timer(1,0,TimerProc);

   while(1){
      peek_timer_signal(TimerSignal);
      packet = (u_char *)pcap_next(pcap_handle, cap_header);
      if(packet){
         decode_packet(&pkt, packet, cap_header->caplen);

         //#ifdef __SNIFFER_DEBUG
         if(debug_sniffer){
            printf("CAPTURED PACKET LENGTH: %hu of %hu\n", cap_header->caplen, cap_header->len);
            printf("TIME: %s",ctime((const time_t*)&cap_header->ts.tv_sec));
            //dump(packet, cap_header->caplen); //caplen: captured portion packet length
            print_packet(&pkt);
         }
         //#endif

         parse_packet(&pkt, cap_header);

         if( !memcmp(pkt.ethh->src.addr, param.hwaddr_filter.addr, ETH_ADDR_LEN) ){
            packetp_add_out_info(param.pktp, &pkt);
         }else if( !memcmp(pkt.ethh->dst.addr, param.hwaddr_filter.addr, ETH_ADDR_LEN) ){
            packetp_add_in_info(param.pktp, &pkt);
            //packet_stats_add_in_info(param.pktstats_instant, &pkt);
         }

      }
   }
   free(cap_header);
}

VOID CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) {
    handle_tick();
}

VOID TimerSignal(MSG msg)
{ }


#else
void timeout_handler(int signal_code)
{
    handle_tick();
}
#endif

__inline void handle_tick()
{
   struct tm *time = current_time();

   //#ifdef __DEBUG_PARAMETER
   if(debug_parameter){
      print_packetp(param.pktp);
   }
   //#endif

   //#ifdef __DETECT_DOS
   if(detect_dos){
      dos_classify(param.dosc, param.pktp, time);
   }
   //#endif

   //#ifdef __STORE_PARAMETER
   if(store_parameters){
      store_tcpp_file(ftcpp, param.pktp, time);
      store_udpp_file(fudpp, param.pktp, time);
   }
   //#endif

   clear_packetp(param.pktp);
}

void setup_signals()
{
   signal(SIGINT, handle_signal);
   signal(SIGABRT, handle_signal);
   signal(SIGILL, handle_signal);

   #ifndef __WIN32
   signal(SIGHUP, handle_signal);
   signal(SIGQUIT, handle_signal);
   signal(SIGTRAP, handle_signal);
   signal(SIGKILL, handle_signal);
   signal(SIGSTOP, handle_signal);
   signal(SIGTSTP, handle_signal);
   #endif
}

void handle_signal(int signal)
{
   printf("\nFinishing...\n");
   finish();
   exit(1);
}

void process_arg(int argc, char **argv)
{
   char *device = NULL;
   char *read_file = NULL;

   char *store_files[2];
   char *load_files[2];

   char *tcpsyn_files[2];
   char *udp_files[2];

   char *dosc_file = NULL;

   bool_t train = FALSE;
   bool_t load = FALSE;
   bool_t host = FALSE;
   int i;

   memset(store_files, 0, (sizeof(char *))*2);
   memset(load_files, 0, (sizeof(char *))*2);
   memset(tcpsyn_files, 0, (sizeof(char *))*2);
   memset(udp_files, 0, (sizeof(char *))*2);

   debug_sniffer = FALSE;
   debug_parameter = FALSE;
   detect_dos = FALSE;
   store_parameters = FALSE;


   for(i=1; i<argc; ++i){
      if( !strcmp(argv[i], "-i") ){
         device = argv[++i];
         //printf("Set Device Interface\n");
      }else if( !strcmp(argv[i], "-r") ){
         read_file = argv[++i];
         //printf("Set Device Interface\n");
      }else if( !strcmp(argv[i], "-d") ){
         dosc_file = argv[++i];
         detect_dos = TRUE;
         //printf("Detect Attacks\n");
      }else if( !strcmp(argv[i], "-s") ){
         store_files[0]  = argv[++i];
         store_files[1]  = argv[++i];
         store_parameters = TRUE;
         //printf("Store Parameters\n");
      }else if( !strcmp(argv[i], "-l") ){
         load_files[0]  = argv[++i];
         load_files[1]  = argv[++i];
         load = TRUE;
         //printf("Load Parameters File\n");
      }else if( !strcmp(argv[i], "-t") ){
         dosc_file = argv[++i];
         tcpsyn_files[0]  = argv[++i];
         tcpsyn_files[1]  = argv[++i];
         udp_files[0]  = argv[++i];
         udp_files[1]  = argv[++i];
         train = TRUE;
      }else if( !strcmp(argv[i], "-S") ){
         debug_sniffer = TRUE;
         //printf("Debug Sniffer\n");
      }else if( !strcmp(argv[i], "-P") ){
         debug_parameter = TRUE;
         //printf("Debug Parameters\n");
      }else if( !strcmp(argv[i], "-h") ){
         host = TRUE;
      }
   }

   if(train){
      param.dosc = load_dosc(0);
      train_dosc(param.dosc, dosc_file, tcpsyn_files, udp_files);
      destroy_dosc(&param.dosc);
   }else if(load){
      ftcpp = fopen(load_files[0], "rb");
      if(!ftcpp){
         printf("Could not open the file: %s\n", load_files[0]);
         exit(1);
      }
      fprint_tcpp_file(stdout, ftcpp);
      fclose(ftcpp);

      fudpp = fopen(load_files[1], "rb");
      if(!fudpp){
         printf("Could not open the file: %s\n", load_files[1]);
         exit(1);
      }
      fprint_udpp_file(stdout, fudpp);
      fclose(fudpp);

   }else if(host){
      show_host_info(device);
   }else{
      run(device, read_file, dosc_file, store_files);
   }
}
