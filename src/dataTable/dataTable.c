
# ifdef DEBUG
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
# endif /* DEBUG */

/**********************************
 *			DEBUG Functions above
 **********************************/

/*
 *	Function:			hasElement
 *	Description:	Looks for a specific value in the data table
 *
 *	PARAMS:
 *
 *	RETURN:
 *		0						Element not found
 *		1						Element found
 */

int hasElement () {

	int found = 0;
	findElement (&found);
	if (found == 0)
		return 0;
	return 1;
}


/*
 *	Function:			findElement
 *	Description:	Looks for a specific value in the data table
 *
 *	PARAMS:
 *		locn				Location of the element, or free space in the data table
 *
 *	RETURN:
 *		0						No error
 */

int findElement (int * locn) {

	int retVal = 0;

	return retVal;
}

/*
 *	Function: 		insertHash
 *	Description:	Inserts a value into the hash table
 *
 *	PARAMS:
 *
 *
 *	RETURN:
 *		0						No error
 */
int insertHash (int pos, int val) {

 int i;
 int retVal = 0;
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

