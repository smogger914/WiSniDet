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

# include "stdafx.h"
# include "pmd.h"

PMD::PMD () {
	// pro constructor here
	pCurrentConnInfo = NULL;
	pInterfaceList = NULL;
	hClientHandle = NULL;
	dClientVersion = 1;
	dNegotiatedVersion = 0;
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
	dNegotiatedVersion = 0;
	dClientVersion = 1;

	// create file handler
	rtn = WlanOpenHandle (dClientVersion, NULL, &dNegotiatedVersion, &hClientHandle);
	if (rtn != ERROR_SUCCESS) {
		printf ("WlanOpenHandle\n");
		//scanf_s ("%d", &iContinue);
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
		//scanf_s ("%d", &iContinue);
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
    }

	/*
	 *	Cleanup process for local handles
	 */
	/*
	if (pCapability != NULL)
	{
		printf ("\pCapability not null\n");
		WlanFreeMemory(pCapability);
	}
	
	if (interfaceType != NULL)
	{
		printf ("\interfaceType not null\n");
		WlanFreeMemory((VOID*)&interfaceType);			/// segfault?
	}
	*/

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

	DWORD rtn = 0;
	ULONG ulSize = 0;
	dNegotiatedVersion = 0;
	dClientVersion = 1;

	/*
	rtn = WlanOpenHandle(dClientVersion, NULL, &dNegotiatedVersion, &hClientHandle);
	if(rtn != ERROR_SUCCESS) {

		printf("Error occured in WlanOpenHandle: %d\n", rtn);
		return rtn;
	}
	*/

	// get connected AP info from WLAN API
	dwSize = 0;
	//pCurrentConnInfo = NULL;
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

		printf("Not connected to any AP\n");
		return rtn;
	} 
	else if(rtn != ERROR_SUCCESS)
	{
		printf("Error occured in WlanQueryInterface: %d\n", rtn);
		return rtn;
	}


	if (mode == DOT11_OPERATION_MODE_NETWORK_MONITOR) {
		//printf ("monitor\n");
        rtn = 1;
		printf ("Found dwIndex, %d\n", pInterfaceList->dwIndex);
	}
	else if (mode == DOT11_OPERATION_MODE_EXTENSIBLE_STATION) {
		//printf ("extensible station\n");
        rtn = 0;
	}
	/*
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_NETWORK_MONITOR);
	printf ("dot11 %llu\n", DOT11_OPERATION_MODE_EXTENSIBLE_STATION);
    printf ("Returning : %d\n" , rtn);
	*/


	return rtn;
}

/*!
 *  \fn DWORD PMD::pmcheckwlan()
 *  \ingroup win32backend
 *  \public
 *  \brief Public function to encapsulate the checking of all WLAN interfaces.
 *  \retval 1 : At least one monitor mode card found.
 *  \retval 0 : No monitor mode cards found.
 *  \retval -1 : The process is broken.
 *  \retval -2 : Monitor mode found but could not be stopped.
 *  \retval -3 : Monitor mode found and stopped.
 */
DWORD PMD::pmcheckwlan() {
	
	DWORD rtn = -1;

	createConnect();
	setInterfaceList();

	
	for (pInterfaceList->dwIndex = 0; pInterfaceList->dwIndex < pInterfaceList->dwNumberOfItems; pInterfaceList->dwIndex++)
	{
		getInterfaceCapability();
		
		rtn = queryInterface();
		
		if (rtn == 1) {
			if (stopMonitorMode() == ERROR_SUCCESS) {
				printf ("Monitor mode stopped~\n");
				rtn = -3;
			}
			else  {
				printf ("Monitor mode failed to stop!\n");
				rtn = -2;
			}
			cleanup();
          	return rtn;
        }
		
	}
	
	cleanup();
	return rtn;
}

/*!
 *  \fn DWORD PMD::stopMonitorMode()
 *  \ingroup win32backend
 *  \private
 *  \brief Changes monitor mode flag on the interface back to managed mode.
 *  \retval 0 : Successful exit.
 *	\retval 1 : Someerror.
 */

DWORD PMD::stopMonitorMode() {

	DWORD rtn = 0;
	ULONG ulSize = 0;
	dNegotiatedVersion = 0;
	dClientVersion = 1;

	// get connected AP info from WLAN API
	dwSize = 0;

	
	ULONG pulresult = DOT11_OPERATION_MODE_EXTENSIBLE_STATION;
	PULONG mode = &pulresult;

	printf ("I want to change dwIndex, %d\n", pInterfaceList->dwIndex);
	rtn = WlanSetInterface(hClientHandle,
					&pInterfaceList->InterfaceInfo[pInterfaceList->dwIndex].InterfaceGuid,
					wlan_intf_opcode_current_operation_mode,
					sizeof(mode),
					mode,
					NULL);

	if(rtn == ERROR_INVALID_STATE) {

		// that means not connected to any AP
		rtn = ERROR_SUCCESS;

		printf("Not connected to any AP\n");
		return rtn;
	} 
	else if(rtn != ERROR_SUCCESS)
	{
		switch (rtn) {
		case ERROR_ACCESS_DENIED:
			printf ("WlanSetInterface [ERROR_ACCESS_DENIED]\n");
			break;
		case ERROR_GEN_FAILURE:
			printf ("WlanSetInterface [ERROR_GEN_FAILURE]\n");
			break;
		case ERROR_INVALID_HANDLE:
			printf ("WlanSetInterface [ERROR_INVALID_HANDLE]\n");
			break;
		case ERROR_INVALID_PARAMETER:
			printf ("WlanSetInterface [ERROR_INVALID_PARAMETER]\n");
			break;
		/*
		case RPC_STATUS:
			printf ("WlanSetInterface [RPC_STATUS]\n");
			break;
		*/
		default:
			printf ("WlanSetInterface [SHOULD_NOT]\n");
			break;
		}

		printf("Error occured in WlanSetInterface: %d\n", rtn);
		return rtn;
	}
	

	return rtn;
}

/*!
 *  \fn DWORD PMD::cleanup()
 *  \ingroup win32backend
 *  \private
 *  \brief Cleans up the allocated memory before exiting a function.
 */
VOID PMD::cleanup() {

	if (hClientHandle != NULL) {
		printf ("\thClientHandle not null\n");
        WlanCloseHandle(
			hClientHandle, 
			NULL            // reserved
		);
    }
	if (pInterfaceList != NULL) {
		printf ("\tpInterfaceList not null \n");
		WlanFreeMemory(pInterfaceList);
	}
	
	if (pCurrentConnInfo != NULL)
	{
		printf ("\tpCurrentConnInfo not null\n");
		WlanFreeMemory(pCurrentConnInfo);
	}/*
	*/
}

