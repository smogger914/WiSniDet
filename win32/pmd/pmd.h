/*!
 *  \file pmd.h
 *  \brief Promiscuous Mode Detector - public header file - Windows version
 *  \ingroup win32backend
 *  \author Kenneth Ko
 *
 *  Copyright (c) 2010 Kenneth Ko <BR>
 *  All Rights Reserved. <BR>
 */

#ifndef __PMD_DEFINED__
#define __PMD_DEFINED__


#include <conio.h>
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

#endif