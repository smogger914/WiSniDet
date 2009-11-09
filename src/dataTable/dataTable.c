
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

int hasElement (unsigned long ipAddr) {

	int found = 0;
	findElement (ipAddr, &found);
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

int findElement (unsigned long ipAddr, int * locn) {

	// Init values, have hash position
	int retVal = 0;
	int hashPos = hashFunction (ipAddr);
	struct dataElement * p = hashTable[hashPos];
	int i;
	*locn = -1;

	while (p != NULL && p->status == FILLED) {

		if (p->ipAddr == ipAddr) {
			*locn = p->dataPos;
			break;
		}
		else {
			p = p->nextData;
		}
	}

	return retVal;
}

/*
 *	Function: 		findEmptyDataPos
 *	Description:	Returns an empty position in the data slot
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

}

