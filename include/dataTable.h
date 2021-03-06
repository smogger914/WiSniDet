# ifndef DATATABLE
# define DATATABLE 

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#include "modPing.h"

# define DEBUG					1
# ifndef DEBUG
# 	define DEBUG				1							// Debug?
# endif /* ifndef DEBUG */

# define DATA_SIZE			256						// Maximum size of data table array
# define HASH_SIZE 			512 					// Maximum size of hash table size
# define IP_STR_LEN			64						// Length of IP in presentation format
																			// Use this for strn* functions
# define MAC_STR_LEN		64						// Length of MAC in presentation format
																			// Use this for strn* functions
# define IP_STR_ARR			IP_STR_LEN+1	// IP strength length + null
																			// Make arrays this size in the struct
# define MAC_STR_ARR		MAC_STR_LEN+1	// MAC string length + null
																			// Make arrays this size in the struct

# define EMPTY					0
# define FILLED					1
# define OFF						0
# define ON							1
# define NO							0
# define YES						1
# define FALSE					0
# define TRUE						1

# define PING_BASELINE	0
# define PING_ONOFF			1
# define DEAUTH_SENT		2
# define DEAUTH_RECV		3	

struct dataElement {
  int hashPos;	// Index position in the hash table
  int dataPos;	// Index position in the bucket/data table
	int	status;		// EMPTY or FILLED

	int pbaseline;		// PING_BASELINE, baseline ping stored
	int ponoff;				// PING_ONOFF, do you allow pings
	int deauthSent;		// DEAUTH_SENT, did you send a deauth
	int deauthRecv;		// DEAUTH_RECV, did you recv a deauth

	unsigned long	ipAddr;				// Network format of IP addr
	

	struct dataElement * nextHash;	// Index position of the next in the hash
	struct dataElement * prevHash;	// Index position of the prev in the hash
	struct dataElement * nextData;	// Index position of the next in the data
	struct dataElement * prevData;	// Index position of the prev in the data
};

struct dataElement * dataTable;
struct dataElement ** hashTable;

# ifdef DEBUG
int displayHash(int pos);	// Display the hash position + collisions
int displayBuckets();			// Display the bucket
# endif /* DEBUG */

int hashFunction (short af, unsigned long ipaddr);

int hasElement (unsigned long ipAddr);
int findElement (unsigned long ipAddr, int * locn);
int findEmptyDataPos (unsigned long ipAddr, int * locn);
int lastHashElement (int hashPos, int * locn);

int insertHash (short af, struct in_addr inaddr);	// Insert to hash table
int lookupHash (short af, struct in_addr inaddr, int searching, int *returning);
								
int pingBaseline (short af, struct in_addr inaddr, int * result); 	
																												// Handles both 
																												// PING_ONOFF and
																												// PING_BASELINE

int getIpAddr (short af, struct in_addr inaddr, unsigned long * ipAddr);


# endif /* ifndef DATATABLE */
