# include <sys/ioctl.h>
# include <net/if.h>
# include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>

# include <unistd.h>
# include <stdlib.h>
# include <string.h>

int main () {

  char              buf[1024];
  struct ifconf    ifc;
  struct ifreq      *ifr;
  int               s;
  int               nInterfaces;
  int               i;

  struct ifreq      ifr2;

  /* Get a socket handle */
  s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror ("socket");
    return 1;
  }

  /* Query available interfaces */
  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if (ioctl (s, SIOCGIFCONF, &ifc) < 0) {
//  if (ioctl (s, SIOCGIFNAME, &ifc) < 0) {
    perror ("ioctl(SIOCIFCONF)");
    return 1;
  }

  /* Iterate through the list of interfaces */
  ifr = ifc.ifc_req;
  nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
  for (i = 0; i < nInterfaces; i++) {
    struct ifreq *item = &ifr[i];

    printf ("%s:\tIP\t%s", item->ifr_name,
            inet_ntoa (((struct sockaddr_in *)&item->ifr_addr)->sin_addr));

    if ( ioctl (s, SIOCGIFFLAGS, (char *) ifr) < 0) {
      perror ("ioctl(SIOCGIFFLAGS)");
      return 1;
    }

    if ((ifr->ifr_flags & IFF_UP) == 1) {
      printf ("\tLOL?");
    }

    if ((ifr->ifr_flags & IFF_PROMISC) == 1) {
      printf ("\tLOL!");
    }

    printf (",\t%s", item);

    printf ("\n");
  }

  printf ("\n\n==========\n\n");

  for (i = 0; i < 255; i++) {
    memset (&ifr2, 0, sizeof(ifr2));
    ifr2.ifr_ifindex = i;
    if (ioctl (s, SIOCGIFNAME, &ifr2) == -1)
      continue;
    if (ioctl (s, SIOCGIFFLAGS, &ifr2) == -1) {
      fprintf (stderr, "ioctl(SIOCGIFFLAGS) , %s\n", ifr2.ifr_name);
      continue;
    }
    if (! (ifr2.ifr_flags & IFF_UP)) {
      continue;
    }

    if (ifr2.ifr_flags & IFF_PROMISC) {
      fprintf (stdout, "%s:\tLOLOLOL\n", ifr2.ifr_name);
    }

    if (ioctl (s, SIOCGIFADDR, &ifr2) == -1) {
      fprintf (stderr, "ioctl(SIOCGIFADDR) , %s\n", ifr2.ifr_name);
      continue;
    }

  }
    

  return 0;
}
