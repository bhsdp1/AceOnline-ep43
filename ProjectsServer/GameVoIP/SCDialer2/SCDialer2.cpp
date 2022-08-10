// SCDialer2.cpp : Implementation of CSCDialer2App and DLL registration.

#include "stdafx.h"
#include "SCDialer2.h"
#include "WinSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSCDialer2App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xbc2bf882, 0x699e, 0x4d9d, { 0xb0, 0xad, 0x8d, 0xae, 0x58, 0xb4, 0xd5, 0x35 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CSCDialer2App::InitInstance - DLL initialization

BOOL CSCDialer2App::InitInstance()
{
	CWinSocket::SocketInit();

	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSCDialer2App::ExitInstance - DLL termination

int CSCDialer2App::ExitInstance()
{
	// TODO: Add your own module termination code here.
	
	CWinSocket::SocketClean();
	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
