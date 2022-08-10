// SpaceCowboyDBEditorTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include "WinSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp

BEGIN_MESSAGE_MAP(CSpaceCowboyDBEditorToolApp, CWinApp)
	//{{AFX_MSG_MAP(CSpaceCowboyDBEditorToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp construction

CSpaceCowboyDBEditorToolApp::CSpaceCowboyDBEditorToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpaceCowboyDBEditorToolApp object

CSpaceCowboyDBEditorToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp initialization

BOOL CSpaceCowboyDBEditorToolApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWinSocket::SocketInit();
// 2006-06-07 by cmkwon
// 	char szLocalIP[SIZE_MAX_IPADDRESS];
// 	GGetLocalIP(szLocalIP, IP_TYPE_PUBLIC);
// 	if(FALSE == IS_SCADMINTOOL_CONNECTABLE_IP(szLocalIP))
// 	{
// 		AfxMessageBox("This computer can't excute this program !!");
// 		return FALSE;
// 	}

	SetRegistryKey(STRMSG_REG_STRING_REGISTRYKEY_NAME);

	CSpaceCowboyDBEditorToolDlg dlg;
	m_pMainWnd = &dlg;
	ReadProfile();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, 서버 정보 암호화 - 암호화 되어 있는 데이터를 복호화 한다
	GDecryptGameServerInfoByXOR();

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CSpaceCowboyDBEditorToolApp::WriteProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\SpaceCowboy\AtumAdminTool\Configuration
	CSpaceCowboyDBEditorToolDlg *pDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();

	if(pDlg)
	{
#ifndef _ATUM_ADMIN_RELEASE
		WriteProfileString(_T("Configuration"), _T("UID"), pDlg->m_valueEditID);
		WriteProfileString(_T("Configuration"), _T("PWD"), pDlg->m_valueEditPW);
#endif// _ATUM_ADMIN_RELEASE_end_ifndef
		WriteProfileString(_T("Configuration"), _T("Server"), pDlg->m_szServerName);
		WriteProfileInt(_T("Configuration"), _T("LanguageType"), pDlg->m_nLanguageType);
		WriteProfileString(_T("Configuration"), _T("LocalizationDirectory"), pDlg->m_strLocalizationDirectoryPath);

		// CSCUserAdminToolDlg
		WriteProfileString(_T("Configuration"), _T("AccountNameInput"), m_szAccountNameInputReg);
	}
}

void CSpaceCowboyDBEditorToolApp::ReadProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\AtumAdminTool\Configuration
	CSpaceCowboyDBEditorToolDlg *pDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();

	if(pDlg)
	{
#ifndef _ATUM_ADMIN_RELEASE
		pDlg->m_valueEditID = GetProfileString( _T("Configuration"), _T("UID"), "");
		pDlg->m_valueEditPW = GetProfileString( _T("Configuration"), _T("PWD"), "");
#endif// _ATUM_ADMIN_RELEASE_end_ifndef
		pDlg->m_szServerName = GetProfileString( _T("Configuration"), _T("Server"), "");
		pDlg->m_nLanguageType = GetProfileInt(_T("Configuration"), _T("LanguageType"), LANGUAGE_TYPE_DEFAULT);
		pDlg->m_strLocalizationDirectoryPath = GetProfileString( _T("Configuration"), _T("LocalizationDirectory"), "./localization");

		// CSCUserAdminToolDlg
		m_szAccountNameInputReg = GetProfileString( _T("Configuration"), _T("AccountNameInput"), "");
	}
}

int CSpaceCowboyDBEditorToolApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	CWinSocket::SocketClean();

	return CWinApp::ExitInstance();
}
