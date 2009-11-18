# ifndef DATATABLE
# define DATATABLE 

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <time.h>

# ifndef DEBUG
# 	define DEBUG			1							// Debug?
# endif /* ifndef DEBUG */

# define DATA_SIZE		256						// Maximum size of data table array
# define HASH_SIZE 		512 					// Maximum size of hash table size
# define IP_STR_LEN		64						// Length of IP in presentation format
																		// Use this for strn* functions
# define MAC_STR_LEN	64						// Length of MAC in presentation format
																		// Use this for strn* functions
# define IP_STR_ARR		IP_STR_LEN+1	// IP strength length + null
																		// Make arrays this size in the struct
# define MAC_STR_ARR	MAC_STR_LEN+1	// MAC string length + null
																		// Make arrays this size in the struct

# define EMPTY				0
# define FILLED				1

struct dataElement {
  int hashPos;	// Index position in the hash table
  int dataPos;	// Index position in the bucket/data table
	int	status;		// EMPTY or FILLED

	unsigned long	ipAddr;				// Network format of IP addr
	char	macStr[MAC_STR_ARR];	// Presentation format of MAC addr

	struct * dataElement nextHash;	// Index position of the next in the hash
	struct * dataElement prevHash;	// Index position of the prev in the hash
	struct * dataElement nextData;	// Index position of the next in the data
	struct * dataElement prevData;	// Index position of the prev in the data
};

struct dataElement * dataTable;
struct dataElement ** hashTable;

# ifdef DEBUG
int displayHash(int pos);	// Display the hash position + collisions
int displayBuckets();			// Display the bucket
# endif /* DEBUG */

int hashFunction (unsigned long ipAddr);

int hasElement (unsigned long ipAddr);
int findElement (unsigned long ipAddr, int * locn);
int findEmptyDataPos (unsigned long ipAddr, int * locn);
int lastHashElement (int hashPos, int * locn);

int insertHash (unsigned long ipAddr);	// Insert to hash table
int lookupHash (unsigned long ipAddr);	// Lookup a value from hash table

# endif /* ifndef DATATABLE */
