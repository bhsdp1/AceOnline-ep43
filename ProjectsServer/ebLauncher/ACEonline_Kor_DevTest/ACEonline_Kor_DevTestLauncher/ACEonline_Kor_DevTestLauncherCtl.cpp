// ACEonline_Kor_DevTestLauncherCtl.cpp : Implementation of the CACEonline_Kor_DevTestLauncherCtrl ActiveX Control class.

#include "stdafx.h"
#include "ACEonline_Kor_DevTestLauncher.h"
#include "ACEonline_Kor_DevTestLauncherCtl.h"
#include "ACEonline_Kor_DevTestLauncherPpg.h"
#include "DbgOut_C.h"	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACEonline_Kor_DevTestLauncherCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CACEonline_Kor_DevTestLauncherCtrl, COleControl)
	//{{AFX_MSG_MAP(CACEonline_Kor_DevTestLauncherCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CACEonline_Kor_DevTestLauncherCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CACEonline_Kor_DevTestLauncherCtrl)
	// NOTE - ClassWizard will add and remove dispatch map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	DISP_FUNCTION(CACEonline_Kor_DevTestLauncherCtrl, "SetGameArgument", SetGameArgument, 
				VT_I2, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CACEonline_Kor_DevTestLauncherCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CACEonline_Kor_DevTestLauncherCtrl, COleControl)
	//{{AFX_EVENT_MAP(CACEonline_Kor_DevTestLauncherCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CACEonline_Kor_DevTestLauncherCtrl, 1)
	PROPPAGEID(CACEonline_Kor_DevTestLauncherPropPage::guid)
END_PROPPAGEIDS(CACEonline_Kor_DevTestLauncherCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CACEonline_Kor_DevTestLauncherCtrl, "ACEONLINEKORDEVTESTLAUNCHER.ACEonlineKorDevTestLauncherCtrl.1",
	0x63b871d4, 0xa1b9, 0x4788, 0xb0, 0x6d, 0x6e, 0xfb, 0x9d, 0xb1, 0xe1, 0xac)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CACEonline_Kor_DevTestLauncherCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DACEonline_Kor_DevTestLauncher =
		{ 0x15343484, 0x240f, 0x47f5, { 0x9d, 0xfb, 0x6b, 0x50, 0x4d, 0xd3, 0xf1, 0xa1 } };
const IID BASED_CODE IID_DACEonline_Kor_DevTestLauncherEvents =
		{ 0x3fd39c9e, 0xd7a7, 0x4aa5, { 0xbc, 0xd8, 0x61, 0x49, 0xc6, 0x19, 0xd0, 0x82 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwACEonline_Kor_DevTestLauncherOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CACEonline_Kor_DevTestLauncherCtrl, IDS_ACEONLINE_KOR_DEVTESTLAUNCHER, _dwACEonline_Kor_DevTestLauncherOleMisc)

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
#define GAME_PATH			"SOFTWARE\\ACEonline_Kor_DevTest"
#define GAME_REG_VAL		"ExecutePath"
#define GAME_EXEC			"ACEonline.exe"
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::CACEonline_Kor_DevTestLauncherCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CACEonline_Kor_DevTestLauncherCtrl

BOOL CACEonline_Kor_DevTestLauncherCtrl::CACEonline_Kor_DevTestLauncherCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACEONLINE_KOR_DEVTESTLAUNCHER,
			IDB_ACEONLINE_KOR_DEVTESTLAUNCHER,
			afxRegApartmentThreading,
			_dwACEonline_Kor_DevTestLauncherOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::CACEonline_Kor_DevTestLauncherCtrl - Constructor

CACEonline_Kor_DevTestLauncherCtrl::CACEonline_Kor_DevTestLauncherCtrl()
{
	InitializeIIDs(&IID_DACEonline_Kor_DevTestLauncher, &IID_DACEonline_Kor_DevTestLauncherEvents);

	// TODO: Initialize your control's instance data here.
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	m_strACEonlineInstallPath = "";
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	m_hMutexMonoInstance = NULL;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::~CACEonline_Kor_DevTestLauncherCtrl - Destructor

CACEonline_Kor_DevTestLauncherCtrl::~CACEonline_Kor_DevTestLauncherCtrl()
{
	// TODO: Cleanup your control's instance data here.
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	if(m_hMutexMonoInstance)
	{
		::CloseHandle(m_hMutexMonoInstance);
		m_hMutexMonoInstance = NULL;
	}
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::OnDraw - Drawing function

void CACEonline_Kor_DevTestLauncherCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::DoPropExchange - Persistence support

void CACEonline_Kor_DevTestLauncherCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::OnResetState - Reset control to default state

void CACEonline_Kor_DevTestLauncherCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl::AboutBox - Display an "About" box to the user

void CACEonline_Kor_DevTestLauncherCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ACEONLINE_KOR_DEVTESTLAUNCHER);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl message handlers

// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
short CACEonline_Kor_DevTestLauncherCtrl::SetGameArgument(LPCTSTR szGameId, LPCTSTR szAuthenticationKey, LPCTSTR szSeed, LPCTSTR szGameType, LPCTSTR szGamePassWord1, LPCTSTR szGamePassWord2) 
{
	// TODO: Add your dispatch handler code here
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	char szEncryptMutexName[1024];
	sprintf(szEncryptMutexName, "%s%s", szAuthenticationKey, szSeed);
	m_hMutexMonoInstance = CreateMutex(NULL, TRUE, szEncryptMutexName);
	if(NULL == m_hMutexMonoInstance)
	{
		MessageBox("CreateMutex Error", "ACEonlineLauncher", MB_OK);
		return FALSE;
	}
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업

	m_strACEonlineInstallPath.Format("%s", GAME_PATH);

	DWORD dwReturn = 0;
	HKEY hKey = NULL;
	DWORD dwType = REG_SZ;
	char cACEonlineInstallFullNameBuffer[256];
	DWORD dwBufferSize = sizeof(cACEonlineInstallFullNameBuffer);
	
	dwReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T(m_strACEonlineInstallPath),0,KEY_READ,&hKey);
	
	if(dwReturn == ERROR_SUCCESS)
	{		
		dwReturn = ::RegQueryValueEx(hKey, TEXT(GAME_REG_VAL), NULL, &dwType, (LPBYTE)cACEonlineInstallFullNameBuffer, &dwBufferSize );
		
		::RegCloseKey(hKey);
	}
	else
	{
		return FALSE;
	}
	
	{		
		char szParameter[MAX_PATH];
		memset(szParameter, 0x00, MAX_PATH);
		
		wsprintf(szParameter,"%s %s %s %s %s %s", 
			szGameId, szAuthenticationKey, szSeed, szGameType, szGamePassWord1, szGamePassWord2);			
		
		SHELLEXECUTEINFO sExecInfo;		
		memset(&sExecInfo, 0x00, sizeof(SHELLEXECUTEINFO));
		
		sExecInfo.cbSize		= sizeof(SHELLEXECUTEINFO);
		sExecInfo.hwnd			= AfxGetMainWnd()->GetSafeHwnd();
		sExecInfo.lpFile		= GAME_EXEC;
		sExecInfo.lpParameters	= szParameter;
		sExecInfo.lpDirectory	= (char*)cACEonlineInstallFullNameBuffer;
		sExecInfo.hProcess		= NULL;
		sExecInfo.nShow			= SW_SHOWNORMAL;
		sExecInfo.fMask			= SEE_MASK_FLAG_DDEWAIT|SEE_MASK_FLAG_NO_UI;
		
		OSVERSIONINFO osv;
		
		// 운영체제 버전 조사
		osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		GetVersionEx(&osv);
		
		if(osv.dwMajorVersion  <= 5)
		{
			// 비스타, 7.0이상
			sExecInfo.lpVerb		= "open";
		}
		else
		{
			// 비스타, 윈도우7
			sExecInfo.lpVerb		= "runas";
		}
		
		
		if(FALSE == ShellExecuteEx(&sExecInfo))
		{
			DbgOut("ShellExecuteEx FALSE[%s\\%s]\n", (char*)cACEonlineInstallFullNameBuffer, GAME_EXEC);
			return 0;
		}
	}
	
	return 1;
}
// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업