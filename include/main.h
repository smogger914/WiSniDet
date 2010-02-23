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

# define SERVER_IP    "129.210.16.41"
# define SERVER_PORT  5316 
# define BUFFER_SIZE  1000

void sighandler (int);
int notifyController();

#endif
