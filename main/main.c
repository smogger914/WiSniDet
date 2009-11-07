void sighandler ( int sig ) {

	fprintf (stderr, "\nShutdown.\n");
	exit(sig);
}

int main ( int argc, char * argv[] ) {

  int i;

	// Signal catching
	if (signal (SIGINT, sighandler) == SIG_IGN)
		signal (SIGINT, SIG_IGN)
	if (signal (SIGHUP, sighandler) == SIG_IGN)
		signal (SIGHUP, SIG_IGN)
	if (signal (SIGTERM, sighandler) == SIG_IGN)
		signal (SIGTERM, SIG_IGN)
	if (signal (SIGKILL, sighandler) == SIG_IGN)
		signal (SIGKILL, SIG_IGN)

	// Allocate memory
  bar = malloc (ARRAY_SIZE * sizeof (struct foo));
  hash = malloc (HASH_SIZE * sizeof (struct foo *));

	// Set memory to zero
  memset (bar, 0, sizeof (bar));
  memset (hash, 0, sizeof (hash));

	// NULL checks
  if (bar == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }
  if (hash == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }

	while (1) {

		/* Listen for signal */
	}

  return 1;
}

