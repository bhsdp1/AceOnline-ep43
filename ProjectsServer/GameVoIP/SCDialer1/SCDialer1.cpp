// SCDialer1.cpp : Implementation of CSCDialer1App and DLL registration.

#include "stdafx.h"
#include "SCDialer1.h"
#include <Comcat.h>
#include <ObjSafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSCDialer1App NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xb69cacb, 0xafbd, 0x487e, { 0x91, 0xc4, 0xf7, 0x23, 0x78, 0x31, 0x53, 0x9 } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

const GUID CDECL BASED_CODE _DSCDialer1Ctrl = 
		{ 0xe43303fe, 0xbde8, 0x44e2, { 0x87, 0xbf, 0x25, 0x9d, 0x92, 0xf, 0x3a, 0xdd } };


STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
HRESULT CreateComponentCategory (CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory (REFCLSID clsId, CATID catId);
HRESULT UnRegisterCLSIDInCategory (REFCLSID clsId, CATID catId);

////////////////////////////////////////////////////////////////////////////
// CSCDialer1App::InitInstance - DLL initialization

BOOL CSCDialer1App::InitInstance()
{
	// CG: The following block was added by the Windows Sockets component.
	{
		if (!AfxSocketInit())
		{
			AfxMessageBox(CG_IDS_SOCKETS_INIT_FAILED);
			return FALSE;
		}

	}
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CSCDialer1App::ExitInstance - DLL termination

int CSCDialer1App::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
/*	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
*/
	HRESULT hRes;
	AFX_MANAGE_STATE(_afxModuleAddrThis);
	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);
	hRes = CreateComponentCategory (CATID_SafeForInitializing,
		L"Controls safely initializable from persistent data!");
	if (FAILED (hRes)) {
		return (hRes);
	}

	hRes = RegisterCLSIDInCategory (_DSCDialer1Ctrl, CATID_SafeForInitializing);
	if (FAILED (hRes)) {
		return (hRes);
	}

	// Now make safe for scription.

	hRes = CreateComponentCategory (CATID_SafeForScripting, L"Controls safely scriptable!");
	if (FAILED (hRes)) {
		return (hRes);
	}

	hRes = RegisterCLSIDInCategory (_DSCDialer1Ctrl, CATID_SafeForScripting);
	if (FAILED (hRes)) {
		return (hRes);
	}

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
/*
	HRESULT hRes;
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	// Unregister the component categories which were registered for safe initialization &
	// safe scripting.

	hRes = UnRegisterCLSIDInCategory ( _DSCDialer1Ctrl, CATID_SafeForInitializing);
	if (FAILED (hRes)) {
		return (hRes);
	}

	hRes = UnRegisterCLSIDInCategory ( _DSCDialer1Ctrl, CATID_SafeForScripting);
	if (FAILED (hRes)) {
		return (hRes);
	}
	return NOERROR;
*/

}

/////////////////////////////////////////////////////////////////////////////
// Component Category creation

HRESULT CreateComponentCategory (CATID catid, WCHAR *catDescription)
{
	ICatRegister *pCatReg = NULL;
	HRESULT hRes = S_OK;

	// Create instance of ICatRegister interface

	hRes = CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER,
		IID_ICatRegister, (void **)&pCatReg);

	if (FAILED (hRes)) {
		return hRes;
	}

	// Make sure that HKCR\Component Categories\{..catid..} key is regstered
	
	// Definition CATEGORYINFO structure as defined in comcat.h files.
	//	typedef struct  tagCATEGORYINFO
	//	{
	//	CATID catid;
	//	LCID lcid;
	//	OLECHAR szDescription[ 128 ];
	//	}

	CATEGORYINFO catInfo;
	catInfo.catid = catid;
	catInfo.lcid = 0x0409;		// English
	
	// Check the size of decsription. It shouldn't have more than 127 characters.

	int len = wcslen (catDescription);
	if (len > 127) {
		len = 127;
	}
	
	wcsncpy (catInfo.szDescription, catDescription, len);
		
	// Add the null termination to string.

	catInfo.szDescription[len] = '\0';

	// Register the category.

	hRes = pCatReg->RegisterCategories (1, &catInfo);

	// Release the interface.

	pCatReg->Release ();

	return (hRes);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Register control's clsid in component categories

HRESULT RegisterCLSIDInCategory (REFCLSID clsId, CATID catId)
{
	HRESULT hRes = S_OK;

	ICatRegister *pCatReg = NULL;

	// Create instance of ICatRegister interface.

	hRes = CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER,
		IID_ICatRegister, (void **)&pCatReg);
	if (FAILED (hRes)) {
		return (hRes);
	}

	CATID regCatId[1];
	regCatId[0] = catId;
	
	// Register the class as implementing one or more component categories

	hRes = pCatReg->RegisterClassImplCategories (clsId, 1, regCatId);

	// release ICatRegister interface.

	if (pCatReg != NULL) {
		pCatReg->Release ();
	}

	return (hRes);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Component category unregistration

HRESULT UnRegisterCLSIDInCategory (REFCLSID clsId, CATID catId)
{
	HRESULT hRes = S_OK;
	ICatRegister *pCatReg = NULL;

	// Create instance of the ICatRegister interface.

	hRes = CoCreateInstance (CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_INPROC_SERVER,
		IID_ICatRegister, (void **)&pCatReg);
	if (FAILED (hRes)) {
		return (hRes);
	}

	CATID regCatId[1];
	regCatId[0] = catId;

	// Remove one or more implemented category identifiers from a class

	hRes = pCatReg->UnRegisterClassImplCategories (clsId, 1, regCatId);

	// Release ICatRegister interface.

	if (pCatReg != NULL) {
		pCatReg->Release ();
	}

	return (hRes);
}