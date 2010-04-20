/*!
 *  \file pmd.c
 *  \brief Promiscuous Mode Detector
 *  \ingroup cbackend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
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
 *  \fn extern int isPromiscMonitor()
 *  \brief Checks to see if a NIC is in promiscuous or monitor mode.
 *  \ingroup cbackend
 *  \return 1   : At least one NIC is found.
 *  \return 0   : None detected 
 *  \return -1  : The process is broken.
 *  \return -2  : Monitor / Promiscuous found, and could not be stopped.
 *  \return -3  : Monitor wifi card found, stopped.
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

    if (ifr.ifr_flags & IFF_ALLMULTI) {
      retVal = 1;

      //stop allmulti       
      ifr.ifr_flags &= ~IFF_ALLMULTI; ///remove allmulticast flag
      ifr.ifr_flags |= IFF_MULTICAST; ///set to multicast
      if (ioctl (s, SIOCSIFFLAGS, &ifr)) {
        fprintf (stderr, "Error 86: Could not return to normal operation.\n");
        retVal = -2;
      }
      else {
        retVal = -3;
      }
    }
    if (ifr.ifr_flags & IFF_PROMISC) {
      retVal = 1;

      //stop promisc
      ifr.ifr_flags &= ~IFF_PROMISC;  ///remove promiscflag
      ifr.ifr_flags |= IFF_MULTICAST; ///set to normal multicast mode
      if (ioctl (s, SIOCSIFFLAGS, &ifr)) {
        fprintf (stderr, "Error 95: Could not return to normal operation.\n");
        retVal = -2;
      }
      else {
        retVal = -3;
      }
    }
   
    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr) >= 0) {
      info.has_mode = 1;
      if (iwr.u.mode < IW_NUM_OPER_MODE)
        info.mode = iwr.u.mode;
      else
        info.mode = IW_NUM_OPER_MODE;

      if (info.mode == 6) { /*! Monitor mode found. */
        retVal = 1;

        iwr.u.mode = 2; /// Set back to managed mode

        int k;  /// Temporary variable here.

        /*!
        *  First we turn off the card. Then we un-Monitor it.
        */
        ifr.ifr_flags &= ~IFF_UP;
        if (ioctl (s, SIOCSIFFLAGS, &ifr) == -1) {
          retVal = -2;
          continue;
        } /* IF */
        if((k=iw_set_ext(s,ifr.ifr_name,SIOCSIWMODE,&iwr)) < 0){
          retVal = -2;
        }
        else {
          retVal = -3;
        } /* IF-ELSE */
      } /* IF */
    } /* IF */
  } /* FOR */
   
  if (s) {
    close (s);
  }
  return retVal;

}
