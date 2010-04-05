
/*!
 *  \file main.cpp
 *  \brief Core Functions - Windows version
 *  \ingroup win32backend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
 *  All Rights Reserved. <BR>
 */

# include "stdafx.h"
# include "main.h"

/*!
 *  \fn int ipFromFile (char * ip)
 *  \ingroup cbackend
 *  \brief Retrieves IP address from config file.
 *  \param[out] ip char * : IP address of the server.
 *  \return int : 0 means successful read.
 */
int ipFromFile (char * ip) {

	FILE * fd;
	fd = fopen (CONFIG_FILE_WIN32, "r");

	if (fgets (ip, BUFFER_SIZE, fd) == NULL) {
		fprintf (stderr, "Config file empty\n");
		exit(1);
	}
	if (ip[strlen(ip)-1] == '\n')
		ip[strlen(ip)-1] = '\0';

	fclose (fd);
	return 0;
}

/*!
 *  \fn int notifyController (int yesno, char * SERVER_IP)
 *  \ingroup cbackend
 *  \brief Sends UDP packet to controller [ server ] of current status.
 *  \param yesno int : 1 for promiscuous / monitor found. 0 for none.
 *  \param SERVER_IP char * : Presentation format of the server's IP address.
 *  \return int : 0 signifies success.
 */
int notifyController(int yesno, char * SERVER_IP) {

	int sockfd;								/// socket file descriptor 
	char buf[BUFFER_SIZE];					/// message buffer
	WSADATA wsaData;						/// used for WSAStartup
	struct addrinfo hints, *p, *servinfo;	/// addrinfo stuff
	int rv, numbytes;						/// used for return codes

	/*!
	 *	Create the message packet to send
	 */
	if (yesno == 1) {
		strncpy (buf, "1234567890", sizeof (buf));
	}
	else {
		strncpy (buf, "123", sizeof(buf));
	}

	/*!
	 *	Open the windows connection
	 *	\param 0x0101 : Version of WinSock to use.
	 *	\param w : Structure of WSADATA to send.
	 */
	if (WSAStartup (0x0101 , &wsaData) != 0) {	
		fprintf (stderr, "Error opening the Windows connection.\n");
		exit(1);
	}

	ZeroMemory (&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (rv = getaddrinfo (SERVER_IP, SERVER_PORT, &hints, &servinfo)) != 0) {
		perror ("notifyController [getaddrinfo]");
		WSACleanup();
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket (p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror ("notifyController [socket]");
			WSACleanup();
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf (stderr, "Client: Failed to bind socket\n");
		return 2;
	}

	if ((numbytes = sendto (sockfd, buf, strlen(buf), 0, p->ai_addr,
							p->ai_addrlen)) == -1) {
		perror ("talker: sendto");
		WSACleanup();
		exit(1);
	}

	return 0;
}



/*!
 *  \fn int main (int argc, _TCHAR* argv[])
 *  \ingroup win32backend
 *  \brief Core functionality for the Windows implementation.
 *  \param argc int : Number of inputs.
 *  \param argv _TCHAR *[] : Array of the inputs.
 *  \retval 0 : Program successfully exited.
 */
int main(int argc, _TCHAR* argv[])
{
	FreeConsole();
	struct sockaddr_in sa;
	char ip[BUFFER_SIZE];
	FILE * fp;
	if ( (fp = freopen ("D:\\ko\\Desktop\\tests.txt", "w+", stdout) ) == NULL )
		exit (-1);

	DWORD rtn = 0;
	PMD pminstance;

	ipFromFile(ip);
	if (inet_pton (AF_INET, ip, &(sa.sin_addr)) != 1) {
		fprintf (stderr, "No IP address found in config file. Terminating.\n");
		exit(5);
	}
	printf ("Server IP address: %s\n", ip);

	while (1) {
		rtn = pminstance.pmcheckwlan();
		if (rtn) {
			notifyController (1, ip);
		}
		else {
			notifyController (0, ip);
		}
		Sleep(5);
	}
	
	printf ("baller\n");

	return rtn;
}
