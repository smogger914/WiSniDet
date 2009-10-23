#include <stdio.h>
#include <pcap.h>

int main (int argc, char *argv[]) {

  pcap_if_t * alldevs;
  pcap_if_t * d;
  char * dev, errbuf[PCAP_ERRBUF_SIZE];

  if (pcap_findalldevs (&alldevs, errbuf) == -1) {
    fprintf (stderr, "Couldn't find default device: %s\n", errbuf);
    return (2);
  }

  d = alldevs;
  while (d != NULL) {
    printf ("dev: %s\n", d->name); 
    d = d->next;
  }
  return 0;
}
