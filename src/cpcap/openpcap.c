
#include <stdio.h>
#include <pcap.h>

int main(int argc, char *argv[])
{

    pcap_t *handle;                   /* Session handle */
    char dev[] = "wlan0";             /* Device to sniff on */
   // char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];    /* Error string */
    struct bpf_program filter;        /* The compiled filter expression */
    char filter_app[] = "port 23";    /* The filter expression */
    bpf_u_int32 mask;                 /* The netmask of our sniffing device */
    bpf_u_int32 net;                  /* The IP of our sniffing device */
  //  const u_char *packet; 
  //  struct pcap_pkthdr header;


 //   dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
			fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
			return(2);
    }

    
 		handle = pcap_open_live(dev, BUFSIZ, 1, 0, errbuf);
 //   pcap_compile(handle, &filter, filter_app, 0, net);
 //   pcap_setfilter(handle, &filter);
    pcap_lookupnet(dev, &net, &mask, errbuf);
    
    printf("Device: %s\n", dev);
    printf("Net: %d\n", net);
    printf("Mask: %d\n", mask);
    printf("Errbuf: %s\n", errbuf);
    
    return(0);
}


