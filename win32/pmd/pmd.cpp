/*!
 *  \file pmd.cpp
 *  \brief Promiscuous Mode Detector - Windows version
 *  \ingroup win32backend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
 *  All Rights Reserved. <BR>
 */

/*!
 *  \defgroup win32backend Backend Functionality - Windows Implementation
 *  \brief Primary functions for the Windows platform.
 *  These functions use the Windows API and therefore require he MSVC compiler.
 */

// pmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>
#include <conio.h>
#include <objbase.h>
#include <rpcsal.h>
#include <objbase.h>
#include <msxml6.h>
#include <atlbase.h>
#include <iostream>
#include <iomanip>
#include <windows.h>
// headers needed to use WLAN APIs 
#include <wlanapi.h>

#pragma comment(lib,"Wlanapi.lib")

/*!
 *  \class PMD
 *  \brief Promiscuous Mode Detector class using Windows API functions.
 *  \date 2008 March 24
 */
class PMD {

	DWORD dClientVersion, dNegotiatedVersion;
	HANDLE hClientHandle;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	DWORD dSelectedInterfaceIndex;
	PWLAN_CONNECTION_ATTRIBUTES pCurrentConnInfo;
	DWORD	dwSize;
	INT iReqestedOp, iContinue;

	public:
                /*!
                 *  Default constructor.
                 */
		PMD();
                /*!
                 *  Default destructor.
                 */
		~PMD(){};
                /*!
                 *  Checking function for WLAN devices in Monitor mode.
                 */
		DWORD pmcheckwlan();
	private:
                /*!
                 *  Create the connection to the device(s).
                 */
		DWORD createConnect();
                /*!
                 *  Populate list of WLAN interfaces.
                 */
		DWORD setInterfaceList();
                /*!
                 *  Retrieve capabilities of an interface card.
                 */
		DWORD getInterfaceCapability();
                /*!
                 *  Check interface for monitor mode flag.
                 */
		DWORD queryInterface();
};

PMD::PMD () {
	// pro constructor here
}

/*!
 *  \fn DWORD PMD::createConnect()
 *  \ingroup win32backend
 *  \private
 *  \brief Creates a connection with the WLAN device(s).
 *  \retval ERROR_SUCCESS : WlanOpenHandle successful.
 *  \retval int : Error code from WlanOpenHandle.
 */
DWORD PMD::createConnect () {
	
	DWORD rtn;
	dClientVersion = 1;
	dNegotiatedVersion = 0;

	// create file handler
	rtn = WlanOpenHandle (dClientVersion, NULL, &dNegotiatedVersion, &hClientHandle);
	if (rtn != ERROR_SUCCESS) {
		printf ("WlanOpenHandle\n");
		scanf_s ("%d", &iContinue);
		return rtn;
	}
	return rtn;
}

/*!
 *  \fn DWORD PMD::setInterfaceList()
 *  \ingroup win32backend
 *  \private
 *  \brief Populates an interface list to be used later.
 *  \retval ERROR_SUCCESS : WlanEnumInterfaces successful.
 *  \retval int : Error code from WlanEnumInterfaces.
 */
DWORD PMD::setInterfaceList () {

	DWORD rtn;
	pInterfaceList = NULL;

	rtn = WlanEnumInterfaces (hClientHandle, NULL, &pInterfaceList);
	if (rtn != ERROR_SUCCESS) {
		printf ("WlanEnumInterfaces: %d\n", rtn);
		scanf_s ("%d", &iContinue);
		return rtn;
	}
	return rtn;
}

/*!
 *  \fn DWORD PMD::getInterfaceCapability()
 *  \ingroup win32backend
 *  \private
 *  \brief Retrieves the interface's capabilities.
 *  \retval ERROR_SUCCESS : WlanGetInterfaceCapability successful.
 *  \retval int : Error code from WlanGetInterfaceCapability.
 */
DWORD PMD::getInterfaceCapability () {
	
	PWLAN_INTERFACE_CAPABILITY pCapability = NULL;
	WLAN_INTERFACE_TYPE interfaceType;
	DWORD rtn;

	__try {
		
		rtn = WlanGetInterfaceCapability (
					hClientHandle,
					&pInterfaceList->InterfaceInfo[pInterfaceList->dwIndex].InterfaceGuid,
					NULL,
					&pCapability);
		
		if ( rtn != ERROR_SUCCESS ) {
			__leave;
		}
		interfaceType = pCapability->interfaceType;
		//printf ("%d\n", interfaceType); 
	}
	__finally
    {
        // clean up
        if (hClientHandle != NULL)
        {
            WlanCloseHandle(
                hClientHandle, 
                NULL            // reserved
                );
        }
    }
	return rtn;
}

/*!
 *  \fn DWORD PMD::queryInterface()
 *  \ingroup win32backend
 *  \private
 *  \brief Queries the interface for monitor mode flag.
 *  \retval 0 : Extensible Station mode detected.
 *  \retval 1 : Monitor mode detected.
 */
DWORD PMD::queryInterface() {

	DWORD rtn;
	TCHAR szCurrentESSID[256];
	TCHAR szCurrentMACAddressStr[256]; 
	INT iCurrentRSSI;
	ULONG ulSize = 0;

	dClientVersion = 1;
	dNegotiatedVersion = 0;
	rtn = WlanOpenHandle(dClientVersion, NULL, &dNegotiatedVersion, &hClientHandle);
	if(rtn != ERROR_SUCCESS) {

		printf("Error occured in WlanOpenHandle: %d\n", rtn);
		return rtn;
	}

	// get connected AP info from WLAN API
	dwSize = 0;
	pCurrentConnInfo = NULL;
	rtn = WlanQueryInterface(hClientHandle,
					&pInterfaceList->InterfaceInfo[pInterfaceList->dwIndex].InterfaceGuid,
					wlan_intf_opcode_current_connection,
					NULL,
					&dwSize,
					(PVOID *) &pCurrentConnInfo,
					NULL);

	PULONG pulresult = 0; 
	rtn = WlanQueryInterface(hClientHandle,
					&pInterfaceList->InterfaceInfo[pInterfaceList->dwIndex].InterfaceGuid,
					wlan_intf_opcode_current_operation_mode,
					NULL,
					&ulSize,
					(PVOID *)&pulresult,
					NULL);

	ULONG mode = *pulresult;


	if(rtn == ERROR_INVALID_STATE) {

		// that means not connected to any AP
		rtn = ERROR_SUCCESS;

		if(pCurrentConnInfo != NULL)
		{
			WlanFreeMemory((PVOID) pCurrentConnInfo);
		}

		// close handle
		WlanCloseHandle(hClientHandle, NULL);

		printf("Not connected to any AP\n");

		return rtn;
	} 
	else if(rtn != ERROR_SUCCESS)
	{
		if(pCurrentConnInfo != NULL)
		{
			WlanFreeMemory((PVOID) pCurrentConnInfo);
		}

		// close handle
		WlanCloseHandle(hClientHandle, NULL);

		printf("Error occured in WlanQueryInterface: %d\n", rtn);

		return rtn;
	}


	if (mode == DOT11_OPERATION_MODE_NETWORK_MONITOR) {
		printf ("monitor\n");
                rtn = 1;
	}
	else if (mode == DOT11_OPERATION_MODE_EXTENSIBLE_STATION) {
		printf ("extensible station\n");
                rtn = 0;
	}
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_NETWORK_MONITOR);
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_EXTENSIBLE_STATION);
        printf ("Returning : %d\n" , rtn);
	return rtn;
}

/*!
 *  \fn DWORD PMD::pmcheckwlan()
 *  \ingroup win32backend
 *  \public
 *  \brief Public function to encapsulate the checking of all WLAN interfaces.
 *  \retval 1 : At least one monitor mode card found.
 *  \retval 0 : No monitor mode cards found.
 */
DWORD PMD::pmcheckwlan() {
	
	DWORD rtn = 0;

	createConnect();
	setInterfaceList();

	for (pInterfaceList->dwIndex = 0; pInterfaceList->dwIndex < pInterfaceList->dwNumberOfItems; pInterfaceList->dwIndex++)
	{
		getInterfaceCapability();
		rtn = queryInterface();
                if (rtn == 1) {
                  return rtn;
                }
	}
	
	
	return rtn;
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

	FILE * fp;
	if ( (fp = freopen ("D:\\ko\\Desktop\\tests.txt", "w+", stdout) ) == NULL )
		exit (-1);

	DWORD rtn = 0;

	PMD pminstance;
	pminstance.pmcheckwlan();
	Sleep(5000);
	
	printf ("baller\n");

	return rtn;
}
