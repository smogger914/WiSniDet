
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

	struct sockaddr_in serverAddr; 		/// server address
	struct sockaddr_in clientAddr; 		/// client address
	int sockfd;							/// socket file descriptor 
	char buf[BUFFER_SIZE];				/// message buffer
	int proPort;						/// protocol port
	int len = sizeof (struct sockaddr);	/// size of sockaddr

	WSADATA wsaData;
	WSAStartup (0x0202, &wsaData);		/// windows socket startup
	
	memset ((char *) &clientAddr, 0, sizeof (clientAddr));
	clientAddr.sin_family = AF_INET;	/// address family for client
	clientAddr.sin_addr.s_addr = INADDR_ANY;
	clientAddr.sin_port = htons ((u_short) SERVER_PORT); /// server port set

	serverAddr.sin_family = AF_INET;


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
	char ip[BUFFER_SIZE];
	FILE * fp;
	if ( (fp = freopen ("D:\\ko\\Desktop\\tests.txt", "w+", stdout) ) == NULL )
		exit (-1);

	DWORD rtn = 0;
	PMD pminstance;

	while (1) {
		rtn = pminstance.pmcheckwlan();
		if (rtn) {
			notifyController (1, ip);
		}
		else {
			notifyController (0, ip);
		}
		Sleep(5);
		break;
	}
	
	printf ("baller\n");

	return rtn;
}
