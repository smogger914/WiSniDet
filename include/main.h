#ifndef MAIN
#define MAIN

# include <string.h>
# include <stdlib.h>
# include <stdio.h>

# include <signal.h>


# include "dataTable.h"
# include "modPing.h"

int checkIPv4 (char *);
int ip4StrRange (char *, char *);
int ip4NumRange (int, int, int, int, int, int, int, int);
void sighandler (int);

#endif
