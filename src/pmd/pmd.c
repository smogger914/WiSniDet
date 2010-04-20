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
      fprintf (stdout, "%s: interface down\n", ifr.ifr_name);
      continue;
    }
    if (ifr.ifr_flags & IFF_ALLMULTI) {
      retVal = 1;
    //    printf("MULTI: got one: %s, flags are: %x\n", ifr.ifr_name, ifr.ifr_flags);
        //stop allmulti       
        ifr.ifr_flags &= ~IFF_ALLMULTI;//remove allmulticast flag
        ifr.ifr_flags |= IFF_MULTICAST;//set to normal operation mode multicast
        ioctl (s, SIOCSIFFLAGS, &ifr);
    //    printf("MULTI: got two: %s, flags are: %x\n", ifr.ifr_name, ifr.ifr_flags);
    }
    // printf("FLAGS are %d\n",ifr.ifr_flags);
    if (ifr.ifr_flags & IFF_PROMISC) {
      retVal = 2;
        //printf("found promisc!")
        //printf("PROMISC got one: %s, flags are: %d\n", ifr.ifr_name, ifr.ifr_flags);
        //stop promisc
        ifr.ifr_flags &= ~IFF_PROMISC;//remove promiscflag
        ifr.ifr_flags |= IFF_MULTICAST;//set to normal multicast mode
        ioctl (s, SIOCSIFFLAGS, &ifr);
        //printf("PROMISC got two: %s, flags are: %d\n", ifr.ifr_name, ifr.ifr_flags);
    }
   
    if (iw_get_ext (s, ifr.ifr_name, SIOCGIWMODE, &iwr) >= 0) {
      info.has_mode = 1;
      if (iwr.u.mode < IW_NUM_OPER_MODE)
        info.mode = iwr.u.mode;
      else
        info.mode = IW_NUM_OPER_MODE;
      if (info.mode == 6) { /*! Monitor mode */
        retVal = 3;

          //stop monitor mode
          iwr.u.mode = 2;
       
          int k;

          if((k=iw_set_ext(s,ifr.ifr_name,SIOCSIWMODE,&iwr)) < 0){
             printf("k = %d+ errorno %s + epic fail\n", k, strerror(errno));
          }
          printf("got one: %s\n", ifr.ifr_name);

      } /* IF */
    } /* IF */
  } /* FOR */
   
  if (s) {
    close (s);
  }
  return retVal;

}
