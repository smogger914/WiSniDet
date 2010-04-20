
# ifndef __MAIN_DEFINED__
# define __MAIN_DEFINED__

# include <winsock2.h>			/// include winsock2.h BEFORE windows.h
# include <WS2tcpip.h>
# include <WSPiApi.h>
# include <stdio.h>

# include "pmd.h"				/// contains windows.h #include


# pragma comment (lib, "ws2_32.lib")

# define BUFFER_SIZE 		100
# define SERVER_PORT 		"5316"
# define CONFIG_FILE 		"pmd.conf"
# define CONFIG_FILE_WIN32 	"C:\\Users\\ko\\AppData\\Local\\Temp\\pmd.conf"

# define SENDAYES			"1234567890"
# define SENDANO			"123"
# define SENDAFAIL			"1234567"
# define SENDAFOUND			"1234567890123"

# endif