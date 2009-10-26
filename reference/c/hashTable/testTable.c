# include <stdlib.h>
# include <string.h>
# include <stdio.h>

# define ARRAY_SIZE 256
# define HASH_SIZE 512

struct foo {
  int val;
  int next;
};

struct foo * bar;
struct foo ** hash;

int displayHash(int pos) {

  printf ("hash[%d]->val = %d\n", pos, hash[pos]->val);
}

int displayBuckets() {
  
  int i;
  int retVal = 0;
  for (i = 0; i < ARRAY_SIZE; i++) {
    printf ("bar[%d].val = %d\n", i, bar[i].val);
  }
  return retVal;
}

int hashMe (int pos, int val) {

 int i;
 int retVal;
 int empty = -1;

 for (i = 0; i < ARRAY_SIZE; i++) {
  if (bar[i].val == 0) {
    empty = i;
    break;
  }
 }
 if (empty >= 0) {
    hash[pos] = &bar[empty];
    bar[empty].val = val;
    printf ("barval = %d\n", bar[empty].val);
    printf ("pos = %d, empty = %d, val = %d\n", pos, empty, val);
 }
 return retVal;
 
}

int main () {

  int i;
  bar = malloc (ARRAY_SIZE * sizeof (struct foo));
  hash = malloc (HASH_SIZE * sizeof (struct foo *));
  memset (bar, 0, sizeof (bar));
  memset (hash, 0, sizeof (hash));

  if (bar == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }

  if (hash == NULL) {
    printf ("fail malloc error\n");
    exit (1);
  }

  hashMe (10, 20);
  displayHash (10);

  return 1;
}


