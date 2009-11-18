# include <stdio.h>
# include <string.h>
# include <stdlib.h>

int returnd (int i, void * p) {

	int retVal = 0;
	switch (i) {
	case 	1:
				p = 3;
				break;
	case 	2:
				strncpy ((char*)p, "hello world\n", sizeof(p));
				break;
	}
	return retVal;
}

int main () {

	char h[30];
	int i;

	returnd (1, &i);
	printf ("i = %d\n", i);
	returnd (2, h);
	printf ("h = %s\n", h);
}
