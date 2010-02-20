# include <sys/ioctl.h>
# include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>

# include "pmd.h"

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

/*
 *  Name:         isPromiscMonitor
 *  Description:  Checks to see if any NIC is in promiscuous
 *                or monitor mode.
 *  Return:       >0    True, there exists at least 1 card in promisc/monitor
 *                0     False
 */

int isPromiscMonitor() {

  char                    buf[1024];
  struct ifconf           ifc;
  struct ifreq            ifr;
  struct iwreq            iwr;
  struct wireless_config  info;
  int                     s;
  int                     nInterfaces;
  int                     i;
  int                     retVal = 0;

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
    // ALLMULTI flag bit set
    if (ifr.ifr_flags & IFF_ALLMULTI) {
      //fprintf (stdout, "%s:\tALLMULTI\n", ifr.ifr_name);
      retVal = 1;
    }
    // PROMISC flag bit set 
    if (ifr.ifr_flags & IFF_PROMISC) {
      //fprintf (stdout, "%s:\tPROMISC\n", ifr.ifr_name);
      retVal = 2;
    }
    
    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr) >= 0) {
      info.has_mode = 1;
      if (iwr.u.mode < IW_NUM_OPER_MODE)
        info.mode = iwr.u.mode;
      else
        info.mode = IW_NUM_OPER_MODE;
      //printf ("Mode: %s\n", iw_operation_mode[info.mode]);
      //printf ("Monitor is %d\n", info.mode);
      if (info.mode == 6) { /* Monitor mode */
        retVal = 3; 
      } /* IF */
    } /* IF */
  } /* FOR */
    
  return retVal;

}

int main () {

  int retVal = 0;
  retVal = isPromiscMonitor();
  printf ("Result: %d\n", retVal);
  return 0;
}
