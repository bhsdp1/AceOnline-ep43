// SCVoIP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SCVoIP.h"
#include "SCVoIPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPApp

BEGIN_MESSAGE_MAP(CSCVoIPApp, CWinApp)
	//{{AFX_MSG_MAP(CSCVoIPApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPApp construction

CSCVoIPApp::CSCVoIPApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hMonoInstance		= NULL;
	m_pSCVoIPDlg	= NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSCVoIPApp object

CSCVoIPApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPApp initialization

BOOL CSCVoIPApp::InitInstance()
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
	SetRegistryKey(_T("Masang Soft"));

	if(__argc != 7)	// 2006-12-25 by cmkwon, 변경(5->7), 인자 두개 추가함
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////
	// EXEFile param1 param2 param3 param4 param5 param6
	//   param1 : VoIP Agent WindowText		<== ex> ACEonlineVoIP_R
	//   param2 : VoIP Client WindowText	<== ex> ACEonline_R
	//   param3 : PCCallAgent IP
	//   param4 : PCCallAgent Port
	//   param5 : VDServer IP
	//   param6 : VDServer Port
	m_pSCVoIPDlg = new CSCVoIPDlg;
	m_pSCVoIPDlg->m_strVoIPAgentName.Format("%s", __argv[1]);		// 2006-12-25 by cmkwon, 추가함
	m_pSCVoIPDlg->m_strVoIPClientName.Format("%s", __argv[2]);		// 2006-12-25 by cmkwon, 추가함
	m_pSCVoIPDlg->m_strPCCallAgentIP.Format("%s", __argv[3]);
	m_pSCVoIPDlg->m_nPCCallAgentPort		= atoi(__argv[4]);
	m_pSCVoIPDlg->m_strVDServerIP.Format("%s", __argv[5]);
	m_pSCVoIPDlg->m_nVDServerPort			= atoi(__argv[6]);

	DbgOut("Parameter VoIPAgent(%s) VoIPClient(%s), PCCallAgent(IP[%15s] Port[%5d]), VDServer(IP[%15s] Port[%5d])\n"
		, m_pSCVoIPDlg->m_strVoIPAgentName, m_pSCVoIPDlg->m_strVoIPClientName
		, m_pSCVoIPDlg->m_strPCCallAgentIP, m_pSCVoIPDlg->m_nPCCallAgentPort
		, m_pSCVoIPDlg->m_strVDServerIP, m_pSCVoIPDlg->m_nVDServerPort);

	if ((m_hMonoInstance = ::CreateMutex(NULL, TRUE, m_pSCVoIPDlg->m_strVoIPAgentName)) == NULL)
    {
        AfxMessageBox ("ERROR : \nCan not get synchronisation.", MB_TOPMOST | MB_ICONSTOP);
        return FALSE;
    }

	if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
		// 이미 실행 되어있음
		HWND hWndSelf	= ::FindWindow(NULL, m_pSCVoIPDlg->m_strVoIPAgentName);
		HWND hWndServer	= ::FindWindow(NULL, m_pSCVoIPDlg->m_strVoIPClientName);
		if(NULL == hWndSelf || NULL == hWndServer)
		{			
			return FALSE;
		}
		
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_BothCall, AV_EV_EXECUTE_OK, 0);
		copyData.dwData = avMsg;
		::SendMessage(hWndServer, WM_COPYDATA, (WPARAM)hWndSelf, (LPARAM)&copyData);
		return FALSE;
    }

	m_pMainWnd = m_pSCVoIPDlg;
	m_pSCVoIPDlg->Create(IDD_DIALOG_ATUM_VOIP);
	
	/*
	//-- CMKWON 2003-05-06 
	ReadProfile();
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
	//*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}

int CSCVoIPApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_hMonoInstance)
	{
		CloseHandle(m_hMonoInstance);
		m_hMonoInstance = NULL;
	}
	return CWinApp::ExitInstance();
}
