# include <sys/ioctl.h>
# include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>

# include "iwlib.h"
# include "wireless.h"
//# include <net/if.h>

# include <unistd.h>
# include <stdlib.h>
# include <string.h>

/*
 *  This array is from iwlib.c
 */
const char * const iw_operation_mode[] = { "Auto",
                                        "Ad-Hoc",
                                        "Managed",
                                        "Master",
                                        "Repeater",
                                        "Secondary",
                                        "Monitor",
                                        "Unknown/bug" };

int main () {

  char              buf[1024];
  struct ifconf     ifc;
  struct ifreq      ifr;
  struct iwreq      iwr;
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
      fprintf (stdout, "%s: y u down, charlie brown?\n", ifr.ifr_name);
      continue;
    }

    if (ifr.ifr_flags & IFF_ALLMULTI) {
      fprintf (stdout, "%s:\tLRRRRROLOLOL\n", ifr.ifr_name);
    }
    
    if (ifr.ifr_flags & IFF_PROMISC) {
      fprintf (stdout, "%s:\tLOLOLOL\n", ifr.ifr_name);
    }

/*
    if (ioctl (s, SIOCGIFADDR, &ifr) == -1) {
      fprintf (stderr, "ioctl(SIOCGIFADDR) , %s\n", ifr.ifr_name);
      continue;
    }
*/
    /*
     *  wireless.h
     */

    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr)  == -1) {
      fprintf (stderr, "ioctl(SIOCGIWMODE) , %s\n", ifr.ifr_name);
      continue;
    }

    if (iwr.u.mode & IW_MODE_MONITOR) {
      fprintf (stderr, "%s:\tMONITOR\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_AUTO) {
      fprintf (stderr, "%s:\tAUTO\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_ADHOC) {
      fprintf (stderr, "%s:\tADHOC\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_INFRA) {
      fprintf (stderr, "%s:\tINFRA\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_MASTER) {
      fprintf (stderr, "%s:\tMASTER\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_REPEAT) {
      fprintf (stderr, "%s:\tREPEAT\n", ifr.ifr_name);
    }
    if (iwr.u.mode & IW_MODE_SECOND) {
      fprintf (stderr, "%s:\tSECOND\n", ifr.ifr_name);
    }
    
    if (ifr.ifr_flags & IW_MODE_MONITOR) {
      fprintf (stdout, "%s:\tLOLOLOL\n", ifr.ifr_name);
    }

    wireless_config info;
    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr) >= 0) {
      info.has_mode = 1;
      if (iwr.u.mode < IW_NUM_OPER_MODE)
        info.mode = iwr.u.mode;
      else
        info.mode = IW_NUM_OPER_MODE;
    }
    printf ("Mode: %s\n", iw_operation_mode[info.mode]);

  }
    
  return 0;
}
