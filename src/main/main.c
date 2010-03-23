/*!
 *  \file main.c
 *  \brief Begins program execution
 *  \ingroup cbackend
 *  \author Ken Ko
 *
 *  Copyright (c) 2010 Really? <BR>
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
 *  \ingroup cclient
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

  if (yesno == 1) {
    strncpy (buf, "1234567890", sizeof(buf));
  }
  else {
    strncpy (buf, "123", sizeof(buf));
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
 *  \ingroup cclient
 *  \brief Begins the entire c backend and c client side.
 *  \param void
 *  \return int : 0 signifies success.
 */
int main () {

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

    s = isPromiscMonitor();
    if (s) {
      notifyController(1, ip);
    }
    else {
      notifyController(0, ip);
    }
    sleep(5);
  }
  
  /** Fin */
  exit (EXIT_SUCCESS);
}
