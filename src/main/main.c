# include "main.h"

void sighandler ( int sig ) {

	fprintf (stderr, "\nShutdown.\n");
	exit(sig);
}

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
		break;
	}

  return 1;
}

