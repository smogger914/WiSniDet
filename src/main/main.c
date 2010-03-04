# include "main.h"

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

int main () {

  pid_t pid, sid;
  int s = 0;

  /* Daemonize */
  pid = fork();
  if (pid < 0) 
    exit (EXIT_FAILURE);
  else if (pid > 0) 
    exit (EXIT_SUCCESS);
  umask(0);
  sid = setsid();
  if (sid < 0) 
    exit (EXIT_FAILURE);
  if ((chdir("/")) < 0) 
    exit (EXIT_FAILURE);


  char ip[BUFFER_SIZE];
  ipFromFile(ip);
  printf ("1ce moar: %s\n", ip);
  /* The processing */
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
  
  /* Fin */
  exit (EXIT_SUCCESS);
}
