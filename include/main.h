/*!
 *  \file main.h
 *  \brief Core Functions - public header file
 *  \ingroup cbackend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
 *  All Rights Reserved. <BR>
 */

/*!
 *  \defgroup cbackend Backend Functionality
 *  \brief  Daemon process.
 *  Daemon functions for the promiscuous mode detector.
 */

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

# define SERVER_PORT  "5316" 
# define BUFFER_SIZE  100

int ipFromFile (char *);
void sighandler (int);
int notifyController(int, char *);

#endif
