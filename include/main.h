#ifndef MAIN
#define MAIN

# include <string.h>
# include <stdlib.h>
# include <stdio.h>

# include <sys/stat.h>
# include <errno.h>
# include <signal.h>
# include <unistd.h>

# include <arpa/inet.h>

# include "pmd.h"
# include "cpumon.h"

# define CONFIG_FILE  "pmd.conf"

//# define SERVER_IP    "129.210.238.102"
# define SERVER_PORT  "5316" 
# define BUFFER_SIZE  100

int ipFromFile (char *);
void sighandler (int);
int notifyController(int, char *);

#endif
