# ifndef MODPING
# define MODPING

/*
 *			P I N G . C
 *
 * Using the InterNet Control Message Protocol (ICMP) "ECHO" facility,
 * measure round-trip-delays and packet loss across network paths.
 *
 * Author -
 *	Mike Muuss
 *	U. S. Army Ballistic Research Laboratory
 *	December, 1983
 * Modified at Uc Berkeley
 *
 * Changed argument to inet_ntoa() to be struct in_addr instead of u_long
 * DFM BRL 1992
 *
 * Modified for returning values rather than printing to stdout
 * SCU K1Ko 2009
 *
 * Status -
 *	Public Domain.  Distribution Unlimited.
 *
 * Bugs -
 *	More statistics could always be gathered.
 *	This program has to run SUID to ROOT to access the ICMP socket.
 */

/*
 *  File:         modPing.c
 *  Description:  - Original Ping code from 1983, modified
 *                - Added in-line comments for modifications to properly
 *                  compile with modern gcc
 *                - Included get_avg_time() function
 *								- Return desired values with helper functions
 *  Website:      http://www.ping127001.com/pingpage.htm
 *  Date:         2009 October 25
 */
 
/*
 *	HOW TO USE
 *
 *
 *	# include <stdio.h>
 *	int main () {
 *		printf ("average ping times: %d\n", avgPing ("192.168.1.1"));
 * 	}
 */


/* Included for Debian compilation  */
#include <stdlib.h>
#include <string.h>
#include <signal.h>
/***/

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

// Added the below
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

#define	MAXWAIT		2	/* max time to wait for response, sec. */
#define	MAXPACKET	4096	/* max packet size */
#define VERBOSE		1	/* verbose flag */
#define QUIET		2	/* quiet flag */
#define FLOOD		4	/* floodping flag */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

//u_char	packet[MAXPACKET];
char	packet[MAXPACKET];
int	i, pingflags, options;
extern	int errno;

int s;			/* Socket file descriptor */
struct hostent *hp;	/* Pointer to host info */
struct timezone tz;	/* leftover */

struct sockaddr whereto;/* Who to ping */
int datalen;		/* How much data */

/*
static char usage[] =
"Usage:  ping [-dfqrv] host [packetsize [count [preload]]]\n";
*/

char *hostname;
char hnamebuf[MAXHOSTNAMELEN];

/*
int npackets = 10;              // amount of pings to send 
int preload = 0;		// number of packets to "preload" 
int ntransmitted = 0;		// sequence # for outbound packets = #sent 
int ident;

int nreceived = 0;		// # of packets we got back 
long timing = 0;
long tmin = 999999999;
long tmax = 0;
long tsum = 0;		        // sum of all times, for doing average 
long tavg = 0;
*/

//void finish(), catcher();
//void pinger(), pr_pack(char *, int, struct sockaddr_in *);
//char *inet_ntoa();


int avgPing(char *argv);
void catcher();
void pinger();
char * pr_type (register int);
int pr_pack (char * , int , struct sockaddr_in *);
u_short in_cksum (u_short *, int);
void tvsub (register struct timeval *, register struct timeval *);
void finish();
long get_avg_time();


# endif /* MODPING */
