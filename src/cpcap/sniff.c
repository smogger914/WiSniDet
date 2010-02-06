# include <stdio.h>
# include <stdlib.h>
# include <pcap.h>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netinet/if_ether.h>
# include <net/ethernet.h>

# define MAXBYTES2CAPTURE 2048

# ifndef ETHER_HDRLEN
# define ETHER_HDRLEN 14
# endif

struct my_ip {
  u_int8_t          ip_vhl;         // header length, version
# define IP_V(ip)   (((ip)->ip_vhl & 0xf0) >> 4)
# define IP_HL(ip)  ((ip)->ip_vhl & 0x0f)
  u_int8_t          ip_tos;         // type of service
  u_int16_t         ip_len;         // total length
  u_int16_t         ip_id;          // identification
  u_int16_t         ip_off;         // fragment offset field
# define IP_DF  0x4000
# define IP_MF  0x2000
# define IP_OFFMASK 0x1fff
  u_int8_t          ip_ttl;         // time to live
  u_int8_t          ip_p;           // protocol
  u_int16_t         ip_sum;         // checksum
  struct in_addr    ip_src,ip_dst;  // source and dest address
};

pcap_if_t * chooseDevice () {

  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs;
  pcap_if_t *d;
  int i = 0;
  int ch = -1;

  // Find and print all devices
  if (pcap_findalldevs (&alldevs, errbuf) == -1) {
    fprintf (stderr, "Error in pcap_findalldevs: %s\n", errbuf);
    return NULL;
  }
  for (d = alldevs; d; d = d->next) {
    printf ("%d %s", ++i, d->name);
    if (d->description)
      printf (" (%s)\n", d->description);
    else
      printf (" (No description available)\n");
  }
  if (i == 0) {
    printf ("\nNo interfaces found\n");
    return NULL;
  }

  // Let user choose
  while ( (ch < 0) || (ch > i) ) {
  printf ("Choose a device: ");
  scanf ("%d", &ch);
  }
    
  // Find the choice and set it
  d = alldevs;
  for (i = 1; i < ch; i++) {
    d = d->next;
  }

  return d;

}
 
u_char * handle_IP (u_char * args, const struct pcap_pkthdr * pkthdr,
                    const u_char * packet) {

  const struct my_ip * ip;
  u_int length = pkthdr->len;
  u_int hlen, off, version;
  int i;

  int len;

  // Jump past ethernet header
  ip = (struct my_ip *)(packet + sizeof (struct ether_header));
  length -= sizeof (struct ether_header);

  // Check if have packet of valid length
  if (length < sizeof (struct my_ip)) {
    printf ("truncated ip %d", length);
    return NULL;
  }

  len     = ntohs (ip->ip_len);
  hlen    = IP_HL(ip);
  version = IP_V(ip);

  // Check version
  if (version != 4) {
    fprintf (stderr, "Unknown version %d\n", version);
    return NULL;
  }

  // Check header length
  if (hlen < 5) {
    fprintf (stderr, "bad hlen %d\n", hlen);
  }

  // See if we have as much packet as we should
  if (length < len) 
    printf ("\ntruncated IP - %d bytes missing\n", len - length);

  // Check if have first fragment
  off = ntohs (ip->ip_off);
  if ((off & 0x1fff) == 0) { // no 1s in first 13 bits
    fprintf (stdout, "IP: ");
    fprintf (stdout, "%s ", inet_ntoa (ip->ip_src));
    fprintf (stdout, "%s %d %d %d %d\n", inet_ntoa (ip->ip_dst),
                                          hlen, version, len, off);
  }

  return NULL;
}
  
u_int16_t handle_ethernet (u_char *args, const struct pcap_pkthdr * pkthdr,
                          const u_char * packet) {

  struct ether_header *eptr;

  // Start with ether header
  eptr = (struct ether_header *) packet;

  fprintf (stdout, "eth_hdr source: %d\n",
          ether_ntoa ((const struct ether_addr *)&eptr->ether_shost));
  fprintf (stdout, "eth_hdr destin: %d\n\n",
          ether_ntoa ((const struct ether_addr *)&eptr->ether_dhost));

  // Have IP packet?
  if (ntohs (eptr->ether_type) == ETHERTYPE_IP) {
    fprintf (stdout, "(IP)");
  }
  fprintf (stdout, "\n");

  return eptr->ether_type;
}

void processpkt (u_char *args, const struct pcap_pkthdr * pkthdr,
                const u_char * packet) {

  u_int16_t type = handle_ethernet (args, pkthdr, packet);

  if (type == ETHERTYPE_IP) {
    // IP packet
    fprintf (stdout, "ip?\n");
    handle_IP (args, pkthdr, packet);
  }
  else if (type == ETHERTYPE_ARP) {
    // ARP packet
  }
}

int openAndRead (pcap_if_t * dev) {

  bpf_u_int32 netp;
  struct bpf_program filter;
  int cnt = 8;
  int i;
  pcap_t * descr;
  char errbuf[PCAP_ERRBUF_SIZE];
  const u_char *packet;
  struct pcap_pkthdr hdr;
  struct ether_header *eptr;

  u_char *ptr;

  if (dev == NULL) {
    fprintf (stderr, "Device not found\n");
    exit(2);
  }
  printf ("Device: %s\n", dev->name);
  
  // Open device listening
  descr = pcap_open_live (dev->name, MAXBYTES2CAPTURE, 1, 512, errbuf);
  if (descr == NULL) {
    fprintf (stderr, "pcap_open_live() %s: %s\n", dev, errbuf);
    return 2;
  }
  if (pcap_compile (descr, &filter, "", 0, netp) == -1) {
    fprintf (stderr, "pcap_compile()\n");
    exit (1);
  }
  if (pcap_setfilter (descr, &filter) == -1) {
    fprintf (stderr, "pcap_setfilter: %s\n", pcap_geterr(descr));
    exit (1);
  }
  
  pcap_loop (descr, cnt, processpkt, NULL);

  return 0;
}



int main () {

  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t * dev;

  // Choose a device
  dev = chooseDevice ();
  openAndRead (dev);

  return 0;
}
