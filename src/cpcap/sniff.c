# include <stdio.h>
# include <pcap.h>

int chooseDevice (char ** dev) {

	char errbuf[PCAP_ERRBUF_SIZE];

	*dev = pcap_lookupdev (errbuf);
	if (dev == NULL) {
		fprintf (stderr, "Couldn't find default device: %s\n", errbuf);
		return 2;
	}
	return 0;
}



int main () {

	char * buffer;
	chooseDevice (&buffer);
	printf ("Device: %s\n", buffer);
	return 0;
}
