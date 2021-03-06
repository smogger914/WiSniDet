/*!
 *  \file main.c
 *  \brief Core Functions
 *  \ingroup cbackend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
 *  All Rights Reserved. <BR>
 */

# include "main.h"

/*!
 *  \fn int ipFromFile (char * ip)
 *  \ingroup cbackend
 *  \brief Retrieves IP address from config file.
 *  \param[out] ip char * : IP address of the server.
 *  \return int : 0 means successful read.
 */
int ipFromFile(char * ip) {

  FILE * fd;

  //fd = fopen(CONFIG_FILE, "w");
  //fprintf (fd, "127.0.0.1");
  //fclose(fd);

  fd = fopen(CONFIG_FILE, "r");

  if (fgets (ip, BUFFER_SIZE, fd) == NULL) {
    fprintf (stderr, "Config file empty\n");
    exit(1);
  }
  if (ip[strlen(ip)-1] == '\n')
    ip[strlen(ip)-1] = '\0';

  fclose (fd);
  return 0;
}

/*!
 *  \fn int notifyController (int yesno, char * SERVER_IP)
 *  \ingroup cbackend
 *  \brief Sends UDP packet to controller [ server ] of current status.
 *  \param yesno int : 1 for promiscuous / monitor found. 0 for none.
 *  \param SERVER_IP char * : Presentation format of the server's IP address.
 *  \return int : 0 signifies success.
 */
int notifyController(int yesno, char * SERVER_IP) {

  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  char buf[BUFFER_SIZE];
	

	/*!
	 *	Create the message packet to send
	 */
	if (yesno == 1) {
		strncpy (buf, SENDAYES, sizeof (buf));
	}
	else if (yesno == 0) {
		strncpy (buf, SENDANO, sizeof(buf));
	}
	else if (yesno == -2) {
		strncpy (buf, SENDAFAIL, sizeof(buf));
	}
	else if (yesno == -3) {
		strncpy (buf, SENDAFOUND, sizeof	(buf));
	}

  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ( (rv = getaddrinfo(SERVER_IP, SERVER_PORT, &hints, &servinfo)) != 0) {
    perror ("notifyController [getaddrinfo]");
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket (p->ai_family, p->ai_socktype,
                          p->ai_protocol)) == -1) {
      perror ("notifyController [socket]");
      continue;
    }
    break;
  }

  if (p == NULL) {
    fprintf (stderr, "Client: Failed to bind socket\n");
    return 2;
  }

  if ((numbytes = sendto (sockfd, buf, strlen(buf), 0, p->ai_addr,
                          p->ai_addrlen)) == -1) {
    perror ("talker: sendto");
    exit (1);
  }

  freeaddrinfo(servinfo);

  //printf ("talker: sent %d bytes to %s\n", numbytes, SERVER_IP);
  close (sockfd);

  return 0;
}

/*!
 *  \fn int main()
 *  \ingroup cbackend
 *  \brief Begins the entire c backend and c client side.
 *  \return int : 0 signifies success.
 */
int main (void) {

  pid_t pid, sid;
  int s = 0;
  char * home_dir;
  char ip[BUFFER_SIZE];
  struct sockaddr_in sa;

  /*! Daemonize */
  pid = fork();
  if (pid < 0) 
    exit (EXIT_FAILURE);
  else if (pid > 0) 
    exit (EXIT_SUCCESS);
  umask(0);
  sid = setsid();
  if (sid < 0) 
    exit (EXIT_FAILURE);
  home_dir = getenv ("HOME");
  if ((chdir(home_dir)) < 0) 
    exit (EXIT_FAILURE);

  ipFromFile(ip); /// read the IP address from the config file
  if (inet_pton(AF_INET, ip, &(sa.sin_addr)) != 1) {
    fprintf (stderr, "No IP address in config file. Terminating process.\n");
    exit(5);
  }
  printf ("Server IP Address: %s\n", ip);
  /** The processing */
  while (1) {

   
//printf("s is %d\n", s);
		if (s == 1) {	/// found promiscuous card
			notifyController (1, ip);
		}
		else if (s == 0) {	/// no promiscuous card
			notifyController (0, ip);
		}
		else if (s == -2) {	/// yes promiscuous, no turn off
			notifyController (-2, ip);
		}
		else if (s == -3) {	/// yes promiscuous, yes turn off
			notifyController (-3, ip);
		}
	  s = isPromiscMonitor();
    sleep(5);	/// This is in seconds.
  }
  
  /** Fin */
  exit (EXIT_SUCCESS);
}
