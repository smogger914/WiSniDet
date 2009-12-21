#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main () {
	
	char *d;
	char *e;
	int x;
	char s[30] = "111.222.111.222";
	char w[30] = "123.123.234.345";
	char *q = "123.123.123.123";
	printf ("char original: %s\n", s);

	if (inet_pton (AF_INET, q, d) < 1) printf ("not valid ip\n");

	d = strtok_r (s, ".", &e);
	printf ("char edited  : %s\n", d);
	
	while (d != NULL) {
	d = strtok_r (NULL, ".", &e);
		if (d != NULL) {
	printf ("char edited  : %s\n", d);

	x = atoi (d);
	if (x < 555) printf ("hello        : %d\n", x);
	}
	}

	d = strtok (w, ".");
	printf ("char %s\n", d);
	d = strtok (NULL, ".");
	printf ("char %s\n", d);
	d = strtok (NULL, ".");
	printf ("char %s\n", d);
	d = strtok (NULL, ".");
	printf ("char %s\n", d);

	return 0;
}
