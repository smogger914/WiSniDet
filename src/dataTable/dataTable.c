
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
 *	Function:			hashFunction
 *	Description:	Calculates the hash position
 *
 *	PARAMS:
 *		ipAddr			Network format for IP address
 *
 *	RETURN:
 *		retVal			Result from hash function
 */

int hashFunction (unsigned long ipAddr) {

	int retVal = 0;

	// Hash function

	// End hash function

	return retVal;
}


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
 *		ipAddr			IP address in network format (unsigned 32bit int)		
 *		*locn				Location of the element, or free space in the data table
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
 *		ipAddr			IP address in network format (32 bit unsigned)
 *		*locn				Location of the empty data table position
 *
 *	RETURN:
 *		0						No error, empty dataPos in *locn
 *		1						Failed to find empty data table position
 */
int findEmptyDataPos (unsigned long ipAddr, int * locn) {

	// Init values
	int retVal = 0;
	*locn = -1;

	for (i = 0; i < DATA_SIZE; i++) {

		if (dataTable[i].status == EMPTY) {
			*locn = i;
			break;
		}
	}

	if (*locn == -1) {
		retVal = 1;	
	}

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
int insertHash (unsigned long ipAddr) {

 int retVal = 0;



}

