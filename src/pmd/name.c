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
  struct ifconf     ifc;
  struct ifreq      ifr;
  int               s;
  int               nInterfaces;
  int               i;

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

  for (i = 0; i < 255; i++) {
    memset (&ifr, 0, sizeof(ifr));
    ifr.ifr_ifindex = i;
    if (ioctl (s, SIOCGIFNAME, &ifr) == -1)
      continue;
    if (ioctl (s, SIOCGIFFLAGS, &ifr) == -1) {
      fprintf (stderr, "ioctl(SIOCGIFFLAGS) , %s\n", ifr.ifr_name);
      continue;
    }
    if (! (ifr.ifr_flags & IFF_UP)) {
      fprintf (stdout, "%s: IFFUP!!!\n", ifr.ifr_name);
      continue;
    }

    if (ifr.ifr_flags & IFF_LOOPBACK) {
      fprintf (stdout, "%s:\t1111111\n", ifr.ifr_name);
    }
    
    if (ifr.ifr_flags & IFF_UP) {
      fprintf (stdout, "%s:\tUPUPUPUPUPU\n", ifr.ifr_name);
    }

    if (ifr.ifr_flags & IFF_RUNNING) {
      fprintf (stdout, "%s:\tRUN\n", ifr.ifr_name);
    }

    if (ifr.ifr_flags & IFF_BROADCAST) {
      fprintf (stdout, "%s:\tBROADCAST\n", ifr.ifr_name);
    }
    
    if (ifr.ifr_flags & IFF_ALLMULTI) {
      fprintf (stdout, "%s:\tLRRRRROLOLOL\n", ifr.ifr_name);
    }
    
    if (ifr.ifr_flags & IFF_PROMISC) {
      fprintf (stdout, "%s:\tLOLOLOL\n", ifr.ifr_name);
    }

    if (ioctl (s, SIOCGIFADDR, &ifr) == -1) {
      fprintf (stderr, "ioctl(SIOCGIFADDR) , %s\n", ifr.ifr_name);
      continue;
    }

  }
    
  return 0;
}
