/*!
 *  \file pmd.c
 *  \brief Promiscuous Mode Detector
 *  \ingroup cbackend
 *  \author Ken Ko
 *
 *  Copyright (c) 2010 Really? <BR>
 *  All Rights Reserved. <BR>
 */

# include <sys/ioctl.h>
# include <netinet/in.h>
# include <stdio.h>
# include <arpa/inet.h>

# include "pmd.h"

# include <unistd.h>
# include <stdlib.h>
# include <string.h>

/**
 *  This string array copied from iwlib.h used to manage the possible modes
 */
const char * const iw_operation_mode[] = { "Auto",
                                        "Ad-Hoc",
                                        "Managed",
                                        "Master",
                                        "Repeater",
                                        "Secondary",
                                        "Monitor",
                                        "Unknown/bug" };

/*!
 *  \fn extern int isPromiscuousMonitor()
 *  \brief Checks to see if a NIC is in promiscuous or monitor mode.
 *  \ingroup pmd
 *  \return int : 0 if none detected , 1 if at least one NIC is found.
 */

extern int isPromiscMonitor() {

  char                    buf[1024];
  struct ifconf           ifc;
  struct ifreq            ifr;
  struct iwreq            iwr;
  struct wireless_config  info;
  int                     s;
  int                     i;
  int                     retVal = 0;

  /*! Get a socket handle */
  s = socket (AF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    perror ("socket");
    return 1;
  }

  /*! Query available interfaces */
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
    if (ifr.ifr_flags & IFF_ALLMULTI) {
      retVal = 1;
    }
    if (ifr.ifr_flags & IFF_PROMISC) {
      retVal = 2;
    }
    
    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr) >= 0) {
      info.has_mode = 1;
      if (iwr.u.mode < IW_NUM_OPER_MODE)
        info.mode = iwr.u.mode;
      else
        info.mode = IW_NUM_OPER_MODE;
      if (info.mode == 6) { /*! Monitor mode */
        retVal = 3; 
      } /* IF */
    } /* IF */
  } /* FOR */
    
  if (s) {
    close (s);
  }
  return retVal;

}

