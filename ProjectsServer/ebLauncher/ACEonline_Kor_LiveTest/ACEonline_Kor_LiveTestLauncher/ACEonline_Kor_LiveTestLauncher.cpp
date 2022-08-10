// ACEonline_Kor_LiveTestLauncher.cpp : Implementation of CACEonline_Kor_LiveTestLauncherApp and DLL registration.

#include "stdafx.h"
#include "ACEonline_Kor_LiveTestLauncher.h"
#include "comcat.h"		// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid); 
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장


CACEonline_Kor_LiveTestLauncherApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x452556bc, 0x696c, 0x406f, { 0x90, 0xc1, 0x8d, 0xc5, 0xd1, 0x2b, 0x57, 0x6c } };

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장
const CATID CATID_SafeForScripting =
{ 0x7dd95801, 0x9882, 0x11cf, { 0x9f, 0xa9, 0x00, 0xaa, 0x00, 0x6c, 0x42, 0xc4 } };
const CATID CATID_SafeForInitializing =
{ 0x7dd95801, 0x9882, 0x11cf, { 0x9f, 0xa9, 0x00, 0xaa, 0x00, 0x6c, 0x42, 0xc4 } };

const GUID CDECL BASED_CODE _ctlid = 
{0x9077d378, 0x35fa, 0x4e59, 0x99, 0xb8, 0x9e, 0x13, 0x79, 0xb2, 0x4c, 0x55};
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherApp::InitInstance - DLL initialization

BOOL CACEonline_Kor_LiveTestLauncherApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherApp::ExitInstance - DLL termination

int CACEonline_Kor_LiveTestLauncherApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

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

	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장
	if(FAILED(CreateComponentCategory(CATID_SafeForScripting, L"Controls that are safely scriptable")))
		return ResultFromScode(SELFREG_E_CLASS);
	
	if(FAILED(CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely Initializing from persistent data")))
		return ResultFromScode(SELFREG_E_CLASS);
	
	if(FAILED(RegisterCLSIDInCategory(_ctlid, CATID_SafeForScripting)))
		return ResultFromScode(SELFREG_E_CLASS);
	
	if(FAILED(RegisterCLSIDInCategory(_ctlid, CATID_SafeForInitializing)))
		return ResultFromScode(SELFREG_E_CLASS);
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장

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

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;
	
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICatRegister,
		(void**)&pcr);
	if (FAILED(hr))
		return hr;
	
	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english
	
	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is
	int len = wcslen(catDescription);
	if (len>127)
		len = 127;
	wcsncpy(catinfo.szDescription, catDescription, len);
	// Make sure the description is null terminated
	catinfo.szDescription[len] = '\0';
	
	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();
	
	return hr;
}

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister* pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICatRegister,
		(void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Register this category as being "implemented" by
		// the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}
	
	if (pcr != NULL)
		pcr->Release();
	
	return hr;
}
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 안정성 보장