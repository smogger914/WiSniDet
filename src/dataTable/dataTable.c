# include "dataTable.h"

# ifdef DEBUG
int displayHash(int pos) {

	int retVal = 0;

  printf ("hash[%d]->ipAddr = %ld\n", pos, hashTable[pos]->ipAddr);
	
	return retVal;
}

int displayBuckets() {
  
  int i;
  int retVal = 0;
  for (i = 0; i < DATA_SIZE; i++) {
    printf ("dataTable[%d].ipAddr = %ld\n", i, dataTable[i].ipAddr);
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
 *		sa					in_addr resulting from inet_pton	
 *								Used as (struct sockaddr *)sa.sin_addr
 *
 *	RETURN:
 *		retVal			Result from hash function
 */

int hashFunction (short af, unsigned long ipaddr) {

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
	short af = AF_INET;
	int hashPos = hashFunction (af, ipAddr);
	struct dataElement * p = hashTable[hashPos];
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
 *
 *	NOTE:					O(n) potentially
 */
int findEmptyDataPos (unsigned long ipAddr, int * locn) {

	// Init values
	int retVal = 0;
	int i;
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
int insertHash (short af, struct in_addr inaddr) {

	unsigned long ipAddr;
	int retVal = 0;
	int hashPos = hashFunction (af, ipAddr);
	int dataPos;
	int tempVal;

	int pbaseline;
	int ponoff;
	int deauthSent;
	int deauthRecv;

	// Convert format
	getIpAddr (af, inaddr, &ipAddr);

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
		dataTable[dataPos].pbaseline = pbaseline;
		dataTable[dataPos].ponoff = ponoff;
		dataTable[dataPos].deauthSent = deauthSent;
		dataTable[dataPos].deauthRecv = deauthRecv;
		dataTable[dataPos].nextHash = NULL;
		dataTable[dataPos].prevHash = NULL;
		

	} /* endif EMPTY */
	else {

		// Collision
		if (findEmptyDataPos (ipAddr, &dataPos)) {
			// Error: Data table full
		} /* endif empty data pos, findEmptyDataPos */
		
		// In hash table linked list already?
		if (findElement (ipAddr, &tempVal)) {
			// ipAddr not found in the list

			// Get to end of list
			if (lastHashElement (hashPos, &tempVal)) {
				// Error: Absurd
			} // tempVal = dataPos of the last element in hash linked list

			// Insert at end of list
			dataTable[dataPos].hashPos = hashPos;
			dataTable[dataPos].dataPos = dataPos;
			dataTable[dataPos].status = FILLED;
			dataTable[dataPos].ipAddr = ipAddr;
			dataTable[dataPos].pbaseline = pbaseline;
			dataTable[dataPos].ponoff = ponoff;
			dataTable[dataPos].deauthSent = deauthSent;
			dataTable[dataPos].deauthRecv = deauthRecv;
			dataTable[dataPos].nextHash = NULL;
			dataTable[dataPos].prevHash = &dataTable[tempVal];	

			// Previous now points to newest
			dataTable[tempVal].nextHash = &dataTable[dataPos];

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
 *		af					AF_INET or AF_INET6
 *		*inaddr			The ip address as held in the sockaddr struct	
 *		searching		The value we are searching for (#define in header)
 *		*returning	Store here the value of what we searching
 *
 *	RETURN:
 *		0						No error
 *		1						ipAddr not found in hash table
 */
int lookupHash (short af, struct in_addr inaddr, 
								int searching, int * returning) {

	int retVal = 0;
	int locn;
	unsigned long ipAddr;
	struct dataElement p;

	getIpAddr (af, inaddr, &ipAddr);

	if (findElement(ipAddr, &locn)) {
		// Not found
		// Return not found
		retVal = 1;

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
				*returning = p.deauthSent;
				break;
	case	DEAUTH_RECV:
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
 *		af					AF_INET or AF_INET6
 *		inaddr			sin_addr or sin6_addr from sockaddr struct
 *		*result			Store here the value of what we searching
 *
 *	RETURN:
 *		0						No error
 *		1						ipAddr not found in hash table
 *		2						Host unreachable (ping is off)
 */

int pingBaseline (short af, struct in_addr inaddr, int * result) {

	int retVal = 0;
	int temp;
	char *s;

	switch (af) {
	case	AF_INET:
				inet_ntop (AF_INET, &(inaddr), s, INET_ADDRSTRLEN);
				break;
	case 	AF_INET6:
				inet_ntop(AF_INET6, &(inaddr), s, INET6_ADDRSTRLEN);
				break;
	}

	temp = avgPing(s);

	if (temp == -1) {
		// Unreachable destination
		return 2;
	}
	// Destination reachable
	*result = temp;

	return retVal;
}

/*
 *	
 */

int getIpAddr (short af, struct in_addr inaddr, unsigned long * ipAddr) {

	int retVal = 0;

	switch (af) {
	case 	AF_INET:
				*ipAddr = inaddr.s_addr;
				break;
	/*
	case	AF6_INET:
				// must be unsigned char[16]
				break;
	*/
	}

	return retVal;
}
