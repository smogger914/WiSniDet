# ifndef DATATABLE
# define DATATABLE 

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <time.h>

# ifndef DEBUG
# 	define DEBUG			1		// Debug?
# endif /* ifndef DEBUG */

# define ARRAY_SIZE		256	// Maximum size of data table array
# define HASH_SIZE 		512 // Maximum size of hash table size
# define IP_STR_LEN		64	// Length of IP in presentation format
# define MAC_STR_LEN	64	// Length of MAC in presentation format

# define EMPTY				0
# define FILLED				1

struct dataElement {
  int hashPos;	// Index position in the hash table
  int dataPos;	// Index position in the bucket/data table
	int	status;		// EMPTY or FILLED

	char	ipStr[IP_STR_LEN];		// Presentation format of IP addr
	char	macStr[MAC_STR_LEN];	// Presentation format of MAC addr

	struct * dataElement nextHash;	// Index position of the next in the hash
	struct * dataElement prevHash;	// Index position of the prev in the hash
	struct * dataElement nextData;	// Index position of the next in the data
	struct * dataElement prevHash;	// Index position of the prev in the data
};

struct dataElement * dataTable;
struct dataElement ** hashTable;

# ifdef DEBUG
int displayHash(int pos);	// Display the hash position + collisions
int displayBuckets();			// Display the bucket
# endif /* DEBUG */

int insertHash (int pos, int val);	// Insert to hash table
int 

# endif /* ifndef DATATABLE */
