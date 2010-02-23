# include "main.h"

int notifyController() {

  struct sockaddr_in sin;
  int s;
  int len;
  char str[BUFFER_SIZE] = "hello\n";

  /* Build address data structure */
  bzero ((char*) &sin, sizeof(sin));
  sin.sin_family = AF_INET;
  inet_pton (sin.sin_family, SERVER_IP, &(sin.sin_addr));
  sin.sin_port = htons(SERVER_PORT);
  len = sizeof (sin); 

  /* Active open */
  if ((s = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror ("notifyController [socket]");
    exit (1);
  }
  if (connect (s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    perror ("notifyController [connect]");
    close (s);
    exit (1);
  }

  /* Construct message */
  printf ("%s  ^ the string\n", str);
  
  /* Sending */
  ssize_t n = sendto (s, str, BUFFER_SIZE, 0,
                      (struct sockaddr *)&sin, len);
  if (n < 0) {
    printf ("sending error!");
  }

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

  /* The processing */
  while (1) {

    s = isPromiscMonitor();
    if (s) {
      notifyController();
      break;
    }
    sleep(15);

  }
  
  /* Fin */
  exit (EXIT_SUCCESS);
}
