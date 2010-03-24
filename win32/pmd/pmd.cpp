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

class PMD {

	DWORD dClientVersion, dNegotiatedVersion;
	HANDLE hClientHandle;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	DWORD dSelectedInterfaceIndex;
	PWLAN_CONNECTION_ATTRIBUTES pCurrentConnInfo;
	DWORD	dwSize;
	INT iReqestedOp, iContinue;

	public:
		PMD();								// constructor
		~PMD(){};							// destructor
		DWORD pmcheckwlan();				// checking function for wlan devices
	private:
		DWORD createConnect();				// create the connection to the device
		DWORD setInterfaceList();			// populate list of wlan interfaces
		DWORD getInterfaceCapability();		// retrieve capabilitites --> why?
		DWORD queryInterface();				// check for monitor or not
};

PMD::PMD () {
	// pro constructor here
}

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
	}
	else if (mode == DOT11_OPERATION_MODE_EXTENSIBLE_STATION) {
		printf ("extensible station\n");
	}
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_NETWORK_MONITOR);
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_EXTENSIBLE_STATION);
	return rtn;
}

DWORD PMD::pmcheckwlan() {
	
	DWORD rtn = 0;

	createConnect();
	setInterfaceList();

	for (pInterfaceList->dwIndex = 0; pInterfaceList->dwIndex < pInterfaceList->dwNumberOfItems; pInterfaceList->dwIndex++)
	{
		getInterfaceCapability();
		queryInterface();
	}
	
	
	return rtn;
}

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
