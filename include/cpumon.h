# ifndef CPUMON_H
# define CPUMON_H

# include <stdio.h>
# include <string.h>

# define BUFSIZE  15
# define TEMPSIZE 5
# define TENMIN   1
# define FIVEMIN  2
# define ONEMIN   3

float convertToD (char *);
int separateString (char *, int);
FILE * getLoadFile(void);
int getCpuLoad (FILE *, int);
extern int cpuTen (void);

# endif /* CPUMON_H */
