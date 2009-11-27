
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

	int found = -1;
	findElement (ipAddr, &found);
	if (found == -1)
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
 *		1						Not found
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
			p = p->nextHash;
		}
	}

	if (*locn == -1) {
		retVal = 1;
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
 *	Function:			lastHashElement
 *	Description:	Saves the dataPos of the last element in the 
 *								hash table's linked list into *locn.
 *
 *	PARAMS:
 *		hashPos			hashPos that we search from
 *		*locn				Save the dataPos here
 *
 *	RETURN:
 *		0						No error
 *		1						Found EMPTY but not NULL position
 *								When does this happen?
 */
int lastHashElement (int hashPos, int * locn) {

	int retVal = 0;
	struct dataElement * p;
	p = hashTable[hashPos];

	while (p != NULL) {
		if (p->status == EMPTY) {
			if (*locn == -1) {
				retVal = 1;
			}
			break;
		}
		*locn = p->dataPos;
		p = p->nextHash;
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
	int hashPos = hashFunction (ipAddr);
	int dataPos;
	int tempVal;

	int pbaseline;
	int ponoff;
	int deauthSent;
	int deauthRecv;

	char ipStr[IP_STR_LEN];

	// Convert format
	if (ipStr = inet_ntop(ipAddr)) {

	}


	// Does it already exist?
	if (hashTable[hashPos]->status == EMPTY) {

		// No collision
		if (findEmptyDataPos (ipAddr, &dataPos)) {
			// Error: Data table full
		} /* endif empty data pos */

		// Data table has vacancy, insert
		hashTable[hashPos] = &dataTable[dataPos];

		dataTable[dataPos].hashPos = hashPos;
		dataTable[dataPos].dataPos = dataPos;
		dataTable[dataPos].status = FILLED;
		dataTable[dataPos].ipAddr = ipAddr;
		dataTable[dataPos].nextHash = NULL;
		dataTable[dataPos].prevHash = NULL;

	} /* endif EMPTY */
	else {

		// Collision
		if (findEmptyDataPos (ipAddr, &dataPos)) {
			// Error: Data table full
		} /* endif empty data pos, findEmptyDataPos */
		
		// In hash table linked list already?
		if (findElement (ipAddr, tempVal)) {
			// ipAddr not found in the list

			// Get to end of list
			if (lastHashElement (hashPos, tempVal)) {
				// Error: Absurd
			} // tempVal = dataPos of the last element in hash linked list

			// Insert at end of list
			dataTable[dataPos].hashPos = hashPos;
			dataTable[dataPos].dataPos = dataPos;
			dataTable[dataPos].status = FILLED;
			dataTable[dataPos].ipAddr = ipAddr;
			dataTable[dataPos].nextHash = NULL;
			dataTable[dataPos].prevHash = dataTable[tempVal];	

			// Previous now points to newest
			dataTable[tempVal].nextHash = dataTable[dataPos];

		} /* endif findElement */
		else { 
			// found in hash table linked list--update now?

	
		} /* endelse findElement */
			// tempVal = does this ipAddr already exist in hashTable

	} /* endif FILLED */

	return retVal;

}

/*
 *	Function: 		lookupHash
 *	Description:	Returns the value that we are looking for
 *
 *	PARAMS:
 *		ipAddr			The ip address of the value we are searching for
 *		searching		The value we are searching for (#define in header)
 *		*returning	Store here the value of what we searching
 *
 *	RETURN:
 *		0						No error
 *		1						ipAddr not found in hash table
 */
int lookupHash (unsigned long ipAddr, int searching, int * returning) {

	int retVal = 0;
	int locn;
	struct dataElement p;

	if (findElement(ipAddr, &locn)) {
		// Not found
		// Return not found
		retVal = 1

	} /* endif findElement */

	// Returned dataPos in locn
	p = dataTable[locn];

	switch (searching) {
	case	PING_BASELINE:
				*returning = p.pbaseline;	
				break;
	case 	PING_ONOFF:
				*returning = p.ponoff;
				break;
	case	DEAUTH_SENT:
				*returning = p.dauthSent;
				break;
	case	DEAUTH_RECV
				*returning = p.deauthRecv;
				break;
	}

	return retVal;
}

/*
 *	Function: 		pingBaseline
 *	Description:	Return average ping times in ms	
 *
 *	PARAMS:
 *		ipAddr			The ip address of the value we are searching for
 *		*result			Store here the value of what we searching
 *
 *	RETURN:
 *		0						No error
 *		1						ipAddr not found in hash table
 *		2						Host unreachable (ping is off)
 */

int pingBaseline (unsigned long ipAddr, int * result) {

	int retVal = 0;
	int temp;
	struct sockaddr_in sa;
	char s[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(sa.sin_addr), s, INET_ADDRSTRLEN);
	char * s = inet_ntop(ipAddr);

	temp = avgPing(s);

	if (temp == -1) {
		// Unreachable destination
		return 2;
	}
	// Destination reachable
	*result = temp;

	return retVal;
}

