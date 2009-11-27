# include <stdio.h>
# include <string.h>
# include <stdlib.h>

int returnd (int i, void * p, void * q) {

	int retVal = 0;
	switch (i) {
	case 	1:
				printf ("p = %d\n", p);
				break;
	// works
	case 	2:
				printf ("p = %s\n", p);
				break;
	}
	return retVal;
}

int main () {

	int i[30];
	int *p;
	p = 5;
	i[0] = 4;
	i[3] = 3;
	printf ("i[3] = %d\n", i[3]);
	returnd (1, p);
	returnd (2, "hello world");
}
