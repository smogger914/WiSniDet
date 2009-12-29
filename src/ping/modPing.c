
#include "modPing.h"

/*
 *	HOW TO USE
 *
 *
 *	# include <stdio.h>
 *	int main () {
 *		printf ("average ping times: %d\n", avgPing ("192.168.1.1"));
 * 	}
 */

char usage[] =
"Usage:	ping [-dfqrv] host [packetsize [count [preload]]]\n";

int npackets = 10;		// amount of pings to send
int preload = 0;			// number of packets to preload
int ntransmitted = 0;	// sequence # for outbound packets = #sent
int ident;

int nreceived = 0;		// # of packets we got back
long timing = 0;
long tmin = 999999999;
long tmax = 0;
long tsum = 0;				// sum of all times, for doing average
long tavg = 0;

int mytmp = 0;
int mystart = 0;			// ntransmitted - mystart = #sent
int nsent = 0;				// #sent
int prevrecv = 0;			// did we receive previous ping

/*
 * 			M A I N
 */

/*
 *	RETURNS
 *		-1				Unreachable destination
 *		> -1			Average ping time to destination
 */

int avgPing(char *argv)
{

	int rv;
	struct pollfd fds;




	nreceived = 0;		// # of packets we got back
	tmax = 0;
	tsum = 0;				// sum of all times, for doing average
	tavg = 0;

	mytmp = 0;
	// !!! v below v !!!
	mystart = ntransmitted;	// ntransmitted - mystart = #sent
	nsent = ntransmitted - mystart;
	prevrecv = 0;
	
	
	

	struct sockaddr_in from;
	struct sockaddr_in *to = (struct sockaddr_in *) &whereto;
	int on = 1;
	struct protoent *proto;

	bzero((char *)&whereto, sizeof(struct sockaddr) );
	to->sin_family = AF_INET;
	//to->sin_addr.s_addr = inet_addr(argv);
	inet_pton(to->sin_family, argv, &to->sin_addr); // inet_addr deprecated
	if(to->sin_addr.s_addr != (unsigned)-1) {
		strcpy(hnamebuf, argv);
		hostname = hnamebuf;
	} else {
		hp = gethostbyname(argv);
		if (hp) {
			to->sin_family = hp->h_addrtype;
			bcopy(hp->h_addr, (caddr_t)&to->sin_addr, hp->h_length);
			hostname = hp->h_name;
		} else {
			//printf("%s: unknown host %s\n", argv[0], av[0]);
			// Return 2: Unknown host
			return 2;
		}
	}

	datalen = 64-8;
	if (datalen > MAXPACKET) {
		fprintf(stderr, "ping: packet size too large\n");
		exit(1);
	}
	if (datalen >= sizeof(struct timeval))	/* can we time 'em? */
		timing = 1;

	ident = getpid() & 0xFFFF;

	if ((proto = getprotobyname("icmp")) == NULL) {
		fprintf(stderr, "icmp: unknown protocol\n");
		exit(10);
	}

	if ((s = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0) {
		perror("ping: socket");
		exit(5);
	}
	if (options & SO_DEBUG) {
		if(pingflags & VERBOSE)
			printf("...debug on.\n");
		setsockopt(s, SOL_SOCKET, SO_DEBUG, &on, sizeof(on));
	}
	if (options & SO_DONTROUTE) {
		if(pingflags & VERBOSE)
			printf("...no routing.\n");
		setsockopt(s, SOL_SOCKET, SO_DONTROUTE, &on, sizeof(on));
	}

	if(to->sin_family == AF_INET) {
	/*
		printf("PING %s (%s): %d data bytes\n", hostname,
		  inet_ntoa(to->sin_addr), datalen);
	*/
	/* DFM */
	
	} else {
	/*
		printf("PING %s: %d data bytes\n", hostname, datalen );
	*/
	}
	setlinebuf( stdout );


	signal( SIGINT, finish );
	signal(SIGALRM, catcher);

	/* fire off them quickies */
	for(i=0; i < preload; i++) {
		pinger();
printf ("returned from pinger\n");
  }

	if(!(pingflags & FLOOD)) {
		catcher();	/* start things going */
printf ("returned from catcher\n");
  }

	for (;;) {
		int len = sizeof (packet);
		//int fromlen = sizeof (from);
		socklen_t fromlen = sizeof (from); // signedness error if only int type
		int cc;
		struct timeval timeout;
		int fdmask = 1 << s;

		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

		if(pingflags & FLOOD) {
                //printf ("pinger inside\n");
			pinger();
printf ("return from pinger (in for)\n");
                        /* added (fd_set *) */
			if( select(32, (fd_set *)&fdmask, 0, 0, &timeout) == 0)
				continue;
		}

// below
printf ("1 + %d\n", mytmp);

if (mytmp == 0) {
if (nreceived == 0) {
	printf ("have no received\n");
	printf ("	sent %d\n", nsent);
}



fds.fd = s;
fds.events = POLLIN;
fds.revents = 0;

rv = poll (&fds, 1, 10000);

if (rv == -1) {
	perror ("poll");
}
else if (rv == 0) {
	printf ("timeout!\n");
}
else {


		if ((cc=recvfrom(s, packet, len, 0, 
                    /* typecasted (struct sockaddr *) */
                    (struct sockaddr *)&from, &fromlen)) < 0) {

// above

			if( errno == EINTR )
				continue;
			perror("ping: recvfrom");
			continue;
		}

prevrecv = 1;
printf ("2!\n");
}
		if (pr_pack( packet, cc, &from ) == -1) {
			//printf ("out of sight out of mind\n");
			return -1; // unreachable destination
		}
		if (npackets && nreceived >= npackets) {
                  //      printf ("%d\n", get_avg_time());
      return get_avg_time(); // Once all pings finished
			//finish();
    }
}
else {
	printf ("avg_time: %ld\n", get_avg_time());
	return get_avg_time();
}

	}
	/*NOTREACHED*/
}

int rusrs() {
	
	printf ("r u srs\n");
	return 1;
}

/*
 * 			C A T C H E R
 * 
 * This routine causes another PING to be transmitted, and then
 * schedules another SIGALRM for 1 second from now.
 * 
 * Bug -
 * 	Our sense of time will slowly skew (ie, packets will not be launched
 * 	exactly at 1-second intervals).  This does not affect the quality
 *	of the delay and loss statistics.
 */
void catcher()
{
	int waittime;
	nsent = ntransmitted - mystart;

	pinger();
/*
printf ("returned from pinger (in catcher)\n");
printf ("	trans + pack = %d + %d\n", nsent, npackets);
printf ("	prevrecv? %d\n", prevrecv);
*/
	if (npackets == 0 || nsent < npackets) {
		mytmp = 0;
		alarm(1);
	}
	else {
		if (nreceived) {
			waittime = 2 * tmax / 1000;
			if (waittime == 0)
				waittime = 1;
		} 
		else {
				waittime = MAXWAIT;
		}

		printf ("nreceived is %d\n", nreceived);
		printf ("ntransmitted is %d\n", nsent);
		printf ("nreceived is %d!\n", nreceived);
		printf ("waittime! is %d\n", waittime);
		printf ("want to return\n");

		rusrs();
		//return;
		//signal(SIGALRM, finish);
		//alarm(waittime);

		if (prevrecv == 0) {
			printf ("this is the end\n");
			return;
		}

		prevrecv = 0;
		mytmp = 1;
	}
}

/*
 * 			P I N G E R
 * 
 * Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 */
void pinger()
{
	static u_char outpack[MAXPACKET];
	register struct icmp *icp = (struct icmp *) outpack;
	int i, cc;
	register struct timeval *tp = (struct timeval *) &outpack[8];
	register u_char *datap = &outpack[8+sizeof(struct timeval)];

	nsent = ntransmitted - mystart;

	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
//	icp->icmp_seq = ntransmitted++;
	ntransmitted++;
	icp->icmp_seq = nsent++;
	icp->icmp_id = ident;		/* ID */

	cc = datalen+8;			/* skips ICMP portion */

	if (timing)
		gettimeofday( tp, &tz );

	for( i=8; i<datalen; i++)	/* skip 8 for time */
		*datap++ = i;

	/* Compute ICMP checksum here */
	icp->icmp_cksum = in_cksum( (u_short *)icp, cc );

	/* cc = sendto(s, msg, len, flags, to, tolen) */
	i = sendto( s, outpack, cc, 0, &whereto, sizeof(struct sockaddr) );

	if( i < 0 || i != cc )  {
		if( i<0 )  perror("sendto");
		//printf("ping: wrote %s %d chars, ret=%d\n",
	//		hostname, cc, i );
		fflush(stdout);
	}
	if(pingflags == FLOOD) {
		putchar('.');
		fflush(stdout);
	}
}

/*
 * 			P R _ T Y P E
 *
 * Convert an ICMP "type" field to a printable string.
 */
char * pr_type( register int t )
{
	static char *ttab[] = {
		"Echo Reply",
		"ICMP 1",
		"ICMP 2",
		"Dest Unreachable",
		"Source Quench",
		"Redirect",
		"ICMP 6",
		"ICMP 7",
		"Echo",
		"ICMP 9",
		"ICMP 10",
		"Time Exceeded",
		"Parameter Problem",
		"Timestamp",
		"Timestamp Reply",
		"Info Request",
		"Info Reply"
	};

	if( t < 0 || t > 16 )
		return("OUT-OF-RANGE");

	return(ttab[t]);
}

/*
 *			P R _ P A C K
 *
 * Print out the packet, if it came from us.  This logic is necessary
 * because ALL readers of the ICMP socket get a copy of ALL ICMP packets
 * which arrive ('tis only fair).  This permits multiple copies of this
 * program to be run without having intermingled output (or statistics!).
 */
int pr_pack( char * buf, int cc, struct sockaddr_in * from )
{
	char * buf1;
	uint32_t tmp;
	struct ip *ip;
	register struct icmp *icp;
	//register long *lp = (long *) packet;
	//register int i;
	struct timeval tv;
	struct timeval *tp;
	int hlen;
        long triptime;

	from->sin_addr.s_addr = ntohl( from->sin_addr.s_addr );
	gettimeofday( &tv, &tz );

	ip = (struct ip *) buf;
	hlen = ip->ip_hl << 2;
	if (cc < hlen + ICMP_MINLEN) {
		if (pingflags & VERBOSE) {
			tmp = ntohl (from->sin_addr.s_addr);	
			inet_ntop (AF_INET, &tmp, buf1, sizeof(buf1));
			printf("packet too short (%d bytes) from %s\n", cc,
                                /* added s_addr field for ntohl */
				buf1); 
                                /* DFM */
		}
		return 0;
	}
	cc -= hlen;
	icp = (struct icmp *)(buf + hlen);
	if( (!(pingflags & QUIET)) && icp->icmp_type != ICMP_ECHOREPLY )  {
		
		//printf("%d bytes from %s: icmp_type=%d (%s) icmp_code=%d\n",
                  /* added s_addr field for ntohl */
		//  cc, inet_ntoa(ntohl(from->sin_addr.s_addr)),
		//  icp->icmp_type, pr_type(icp->icmp_type), icp->icmp_code);
		
                  /*DFM*/

                /* Check if icmp_type is ICMP_UNREACH */
                if (icp->icmp_type == ICMP_UNREACH) {
                  //printf ("destination unreachable\n");
printf ("pr_pack unreachable\n");
									return -1;
                  signal(SIGALRM, finish);
                }

		if (pingflags & VERBOSE) {
			/*
			for( i=0; i<12; i++)
				printf("x%2.2x: x%8.8x\n", i*sizeof(long),
				  *lp++);
			*/
		}
		return 0;
	}
	if( icp->icmp_id != ident )
		return 1;			/* 'Twas not our ECHO */

	if (timing) {
		tp = (struct timeval *)&icp->icmp_data[0];
		tvsub( &tv, tp );
		triptime = tv.tv_sec*1000
                            +(tv.tv_usec/1000);
		tsum += triptime;
		if( triptime < tmin )
			tmin = triptime;
		if( triptime > tmax )
			tmax = triptime;
	}

	if(!(pingflags & QUIET)) {
		if(pingflags != FLOOD) {
			/*
			printf("%d bytes from %s: icmp_seq=%d", cc,
			  inet_ntoa(from->sin_addr),
			  icp->icmp_seq );	
			*/
				/* DFM */
			if (timing) {
			/*
				printf(" time=%ld ms\n", triptime );
			*/
        //printf ("avg_time: %ld\n", get_avg_time());
      }
			else
				putchar('\n');
		} else {
			putchar('b');
			fflush(stdout);
		}
	}
	nreceived++;
	return 0;
}


/*
 *			I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *
 */
u_short in_cksum(u_short * addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

/*
 * 			T V S U B
 * 
 * Subtract 2 timeval structs:  out = out - in.
 * 
 * Out is assumed to be >= in.
 */
void tvsub( register struct timeval * out, register struct timeval * in )
{
	if( (out->tv_usec -= in->tv_usec) < 0 )   {
		out->tv_sec--;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

/*
 *			F I N I S H
 *
 * Print out statistics, and give up.
 * Heavily buffered STDIO is used here, so that all the statistics
 * will be written with 1 sys-write call.  This is nice when more
 * than one copy of the program is running on a terminal;  it prevents
 * the statistics output from becomming intermingled.
 */
void finish()
{
	nsent = ntransmitted - mystart;
        /* Added \ to form \n */
	putchar('\n');
	fflush(stdout);
	/*
	printf("\n----%s PING Statistics----\n", hostname );
	printf("%d packets transmitted, ", ntransmitted );
	printf("%d packets received, ", nreceived );
	*/
	if (nsent) {
		if( nreceived > nsent)
			printf("-- somebody's printing up packets!");
		else {
			printf("%d%% packet loss !!", 
			  (int) ((( nsent-nreceived)*100) /
			  nsent));
		}
	}
	printf("\n");
        /* tsum is total time of all packets travel */
	if (nreceived && timing)
	/*
	    printf("round-trip (ms)  min/avg/max = %ld/%ld/%ld\n",
	
		tmin,
		tsum / nreceived,
		tmax );
	*/
	fflush(stdout);

	exit(7);
}

long get_avg_time() {

  if (nreceived && timing) 
    tavg = tsum / (nreceived);
  return tavg;
}


