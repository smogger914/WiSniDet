# include "main.h"

int checkIPv4(char * ipAddr) {

	int retVal = 0;

	struct sockaddr_in sa;
	if (inet_pton (AF_INET, ipAddr, &(sa.sin_addr)) < 1)
		retVal = 1;

	return retVal;

}

/*
 *	DESCRIPTION:	Passes the strings to another function
 *								that may be more suited for GUI inetgration
 *					
 *	PARAMS:
 *	*start		123.123.123.123
 *	*end			255.255.255.255
 *
 *	RETURN:
 *	0					Success
 *	1					*start is not valid IP
 *	2					*end is not valid IP
 *
 *	NOTE:
 *	s1.s2.s3.s4 to e1.e2.e3.e4
 */
int ip4StrRange (char * start, char * end) {
	
	char * tmp;
	char * last;
	int s1, s2, s3, s4, e1, e2, e3, e4;

	char starts[IP_STR_LEN];
	char ends[IP_STR_LEN];

	// Validation of arguments
	if (checkIPv4 (start) != 0)
		return 1;
	if (checkIPv4 (end) != 0)
		return 2;

	// Copying to local variables
	strncpy (starts, start, sizeof(starts));
	strncpy (ends, end, sizeof(ends));


	// Breaking apart *start
	tmp = strtok_r (starts, ".", &last);
	s1 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	s2 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	s3 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	s4 = atoi (tmp);

	// Breaking apart *end
	tmp = strtok_r (ends, ".", &last);
	e1 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	e2 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	e3 = atoi (tmp);
	tmp = strtok_r (NULL, ".", &last);
	e4 = atoi (tmp);

	printf ("h1 there\n");
	// Pass this along
	ip4NumRange (s1, s2, s3, s4, e1, e2, e3, e4);

	printf ("hi there\n");
	return 0;
}

/*
 *	Process the IP range
 */

int ip4NumRange (int s1, int s2, int s3, int s4, 
									int e1, int e2, int e3, int e4) {

	int i, j, k, l;
	char ipAddr[IP_STR_LEN];
	char tmp[IP_STR_LEN];

#ifdef DEBUG
printf ("%d %d %d %d\n%d %d %d %d\n", s1, s2, s3, s4, e1, e2, e3, e4);
#endif

	for (i = s1; i <= e1; i++) {
		for (j = s2; j <= e2; j++) {
			for (k = s3; k <= e3; k++) {
				for (l = s4; l <= e4; l++) {
					// Do stuff here
					printf ("%d.%d.%d.%d\n", i, j, k, l);

					// Clear the string, construct ipAddress
					strncpy (ipAddr, "", sizeof(ipAddr));
					sprintf (tmp, "%d", i);
					strcat (ipAddr, tmp);
					strcat (ipAddr, ".");
					sprintf (tmp, "%d", j);
					strcat (ipAddr, tmp);
					strcat (ipAddr, ".");
					sprintf (tmp, "%d", k);
					strcat (ipAddr, tmp);
					strcat (ipAddr, ".");
					sprintf (tmp, "%d", l);
					strcat (ipAddr, tmp);

					printf ("	%s\n", ipAddr);

					// pass to the average ping time
					printf ("average ping time: %d\n", avgPing (ipAddr));
				}
			}
		}
	}

	return 0;
}

/*
 *	LOL SIGNAL HANDLER LOL
 */
void sighandler ( int sig ) {

	fprintf (stderr, "\nShutdown.\n");
	exit(sig);
}

/*
 * LOL MAIN FUNCTION LOL
 */
int main ( int argc, char * argv[] ) {


	// Signal catching
	if (signal (SIGINT, sighandler) == SIG_IGN)
		signal (SIGINT, SIG_IGN);
	if (signal (SIGHUP, sighandler) == SIG_IGN)
		signal (SIGHUP, SIG_IGN);
	if (signal (SIGTERM, sighandler) == SIG_IGN)
		signal (SIGTERM, SIG_IGN);
	if (signal (SIGKILL, sighandler) == SIG_IGN)
		signal (SIGKILL, SIG_IGN);

	// Allocate memory
  dataTable = malloc (DATA_SIZE * sizeof (struct dataElement));
  hashTable = malloc (HASH_SIZE * sizeof (struct dataElement *));

	// Set memory to zero
  memset (dataTable, 0, sizeof (dataTable));
  memset (hashTable, 0, sizeof (hashTable));

	// NULL checks
  if (dataTable == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }
  if (hashTable == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }

	while (1) {

		/* Listen for signal */

		struct sockaddr_in sa;
		char s[INET_ADDRSTRLEN] = "74.125.67.100";

		printf ("s = %s\n", s);
		inet_pton (AF_INET, s, &(sa.sin_addr));
		inet_ntop (AF_INET, &(sa.sin_addr), s, INET_ADDRSTRLEN);
		printf ("s = %s\n", s);
		printf ("\n\n%d\n\n", avgPing(s));
		printf ("also: %ld\n", (long)sa.sin_addr.s_addr);
		printf ("we have %ld\n", (long)sa.sin_addr.s_addr);
	
		printf ("hello world\n");

		//ip4StrRange ("74.125.67.100", "74.125.67.105");
		ip4StrRange ("123.123.123.123", "123.123.123.130");

		break;
	}

  return 0;
}

