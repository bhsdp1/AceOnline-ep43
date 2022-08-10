// SCVoIPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCVoIP.h"
#include "SCVoIPDlg.h"
#include "DbgOut_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPDlg dialog

CSCVoIPDlg::CSCVoIPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCVoIPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCVoIPDlg)
	m_strPCN = _T("");
	m_strPhoneNumber = _T("");
	m_strPWD = _T("1111");
	m_strStatus = _T("Not Login");
	m_nCallType = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_szCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	m_hWndAtumVoIPServer	= NULL;	
	m_nGameVoIPCallType		= _BothCall;

	m_bIsPartyCall			= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
}

void CSCVoIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCVoIPDlg)
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	DDX_Control(pDX, IDC_EDIT_PHONENUMBER, m_editPhoneNumber);
	DDX_Control(pDX, IDC_BUTTON_DIAL, m_buttonDial);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_buttonLogin);
	DDX_Text(pDX, IDC_EDIT_PCN, m_strPCN);
	DDX_Text(pDX, IDC_EDIT_PHONENUMBER, m_strPhoneNumber);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPWD);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_strStatus);
	DDX_Control(pDX, IDC_SCDIALER1CTRL1, m_ctrlSCDialer1);
	DDX_Control(pDX, IDC_SCDIALER2CTRL1, m_ctrlSCDialer2);
	DDX_Radio(pDX, IDC_RADIO_PC, m_nCallType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSCVoIPDlg, CDialog)
	//{{AFX_MSG_MAP(CSCVoIPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_DIAL, OnButtonDial)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON_10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON_0, OnButton0)
	ON_BN_CLICKED(IDC_BUTTON_11, OnButton11)
	ON_BN_CLICKED(IDC_RADIO_PH, OnRadioPh)
	ON_WM_COPYDATA()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPDlg message handlers

BOOL CSCVoIPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_ctrlSCDialer1.SetPCCallAgentIP(m_strPCCallAgentIP);
	m_ctrlSCDialer1.SetPCCallAgentPort(m_nPCCallAgentPort);
	
	// 2002.11.31 까지의 라이센스
	m_ctrlSCDialer1.SetLicenseKey("0204001013151325102200120012122400120113122408201325102210221325102404180915111715210713142000120921001214261428101604101218081403090309");
	m_ctrlSCDialer1.SetAutoAcceptCall(FALSE);

	this->SetWindowText(m_strVoIPAgentName);			// 2006-12-25 by cmkwon, 실행 인자로 받은것을 설정한다
// 2006-12-25 by cmkwon, 위와 같이 실행 인자로 받은것을 설정
//	SetWindowText(WINDOWTEXT_ATUM_CLIENT);
	SetTimer(TIMERID_ATUM_VOIP_SERVER_CHECK, 1000, NULL);
	
	m_hWndAtumVoIPServer = ::FindWindow(NULL, m_strVoIPClientName);		// 2006-12-25 by cmkwon, 수정(WINDOWTEXT_ATUM_SERVER=>m_strVoIPClientName)
	if(m_hWndAtumVoIPServer)
	{
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_BothCall, AV_EV_EXECUTE_OK, 0);
		copyData.dwData = (DWORD)avMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}

	m_ctrlSCDialer2.SetStrVDServerIP(this->m_strVDServerIP);
	m_ctrlSCDialer2.SetUsVDServerPort(this->m_nVDServerPort);

	// 자동 로그인
	//OnButtonLogin();	// 040701, cmkwon
	
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CSCVoIPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSCVoIPDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSCVoIPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSCVoIPDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	DestroyWindow();
	//CDialog::OnOK();
}

void CSCVoIPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
	//CDialog::OnCancel();
}

void CSCVoIPDlg::OnButtonLogin() 
{
	// TODO: Add your control notification handler code here
	if(m_ctrlSCDialer1.IsRegistered())
	{
		m_ctrlSCDialer1.UnRegisterServer();
	}
	else
	{
		UpdateData();
		if(strncmp(m_szCharacterName, "", SIZE_MAX_CHARACTER_NAME) == 0)
		{
			return;
		}

		CString strPCN;
		strPCN.Format("%s,%s,", m_strPCN, m_strPWD);
		m_ctrlSCDialer1.SetUserID(m_strPCN);
		m_ctrlSCDialer1.SetPcn(strPCN);
		m_ctrlSCDialer1.RegisterServer(0);
	}
}

void CSCVoIPDlg::OnButtonDial() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_ctrlSCDialer1.GetDialButtonClicked())
	{
		m_ctrlSCDialer1.Hangup(10);
	}
	else if(!m_strPhoneNumber.IsEmpty())
	{
		m_ctrlSCDialer1.SetCallType(m_nCallType);
		m_ctrlSCDialer1.SetPhoneNumber(m_strPhoneNumber);
		m_ctrlSCDialer1.Dial();
		m_strPrevPhoneNumber = m_strPhoneNumber;
	}
}

void CSCVoIPDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
/*	m_serial.Close();
	if(m_pSocVoice)
	{
		m_serial.m_bStartSocket = FALSE;
		m_pSocVoice->Close();
		delete m_pSocVoice;
		m_pSocVoice = NULL;
	}

	if(m_pSocListener)
		m_pSocListener->Close();
*/
}

BOOL CSCVoIPDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	KillTimer(TIMERID_ATUM_VOIP_SERVER_CHECK);
	return CDialog::DestroyWindow();
}

void CSCVoIPDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMER_RESET_1to1_STATUS:
		KillTimer(TIMER_RESET_1to1_STATUS);
		OnEvStatusMiradialerctrl1(m_ctrlSCDialer1.GetStatus());		
		break;
	case TIMER_RESET_NtoN_STATUS:
		{
			KillTimer(TIMER_RESET_NtoN_STATUS);
			
			m_ctrlSCDialer2.GoOutRoom();
			OnEvStatusScdialer2ctrl1(m_ctrlSCDialer2.GetNStatus());
		}
		break;
	case TIMERID_ATUM_VOIP_END:
		{
			KillTimer(TIMERID_ATUM_VOIP_END);
			OnOK();
		}		
		return;
	case TIMERID_ATUM_VOIP_SERVER_CHECK:
		{
			
			HWND hWndServer = ::FindWindow(NULL, m_strVoIPClientName);
			if(NULL == hWndServer)
			{	
				if(m_ctrlSCDialer1.IsRegistered())
				{
					m_ctrlSCDialer1.Hangup(10);
					m_ctrlSCDialer1.UnRegisterServer();
				}
				OnOK();
				return;
			}

			///////////////////////////////////////////////////////////////////////////////
			// 2007-07-24 by cmkwon, 실행시 설정과 다를 경우 수시로 설정한다.
			CWinApp *pWinApp = AfxGetApp();
			if(IsWindowVisible() != pWinApp->m_nCmdShow)
			{
				ShowWindow(pWinApp->m_nCmdShow);
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CSCVoIPDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSCVoIPDlg)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 1 /* EvError */, OnEvErrorMiradialerctrl1, VTS_I2 VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 2 /* EvAlert */, OnEvAlertMiradialerctrl1, VTS_NONE)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 3 /* EvReceiveCall */, OnEvReceiveCallMiradialerctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 4 /* EvHangup */, OnEvHangupMiradialerctrl1, VTS_NONE)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 6 /* EvConnected */, OnEvConnectedMiradialerctrl1, VTS_NONE)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 7 /* EvUnRegistration */, OnEvUnRegistrationMiradialerctrl1, VTS_I2)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 10 /* EvReceiveButton */, OnEvReceiveButtonMiradialerctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 11 /* EvVolume */, OnEvVolumeMiradialerctrl1, VTS_I2 VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 12 /* EvStatus */, OnEvStatusMiradialerctrl1, VTS_I2)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 13 /* EvRetrievePhoneBook */, OnEvRetrievePhoneBookMiradialerctrl1, VTS_BSTR VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 14 /* EvRegistration */, OnEvRegistrationMiradialerctrl1, VTS_I4 VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 15 /* EvWCAInfo */, OnEvWCAInfoMiradialerctrl1, VTS_BSTR VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 16 /* EvRetrieveAudioDeviceList */, OnEvRetrieveAudioDeviceListMiradialerctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 9 /* EvPushButton */, OnEvPushButtonMiradialerctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER1CTRL1, 22 /* EvReceiveMessage */, OnEvReceiveMessageMiradialerctrl1, VTS_BSTR VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 1 /* EvStatus */, OnEvStatusScdialer2ctrl1, VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 2 /* EvError */, OnEvErrorScdialer2ctrl1, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 3 /* EvOtherJoinedRoom */, OnEvOtherJoinedRoomScdialer2ctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 4 /* EvOtherGoOutedRoom */, OnEvOtherGoOutedRoomScdialer2ctrl1, VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 5 /* EvTalk */, OnEvTalkScdialer2ctrl1, VTS_BSTR VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 6 /* EvNotify */, OnEvNotifyScdialer2ctrl1, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 7 /* EvChangedTalkingState */, OnEvChangedTalkingStateScdialer2ctrl1, VTS_BSTR VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 8 /* EvUserJoinedRoom */, OnEvUserJoinedRoomScdialer2ctrl1, VTS_BSTR VTS_I4)
	ON_EVENT(CSCVoIPDlg, IDC_SCDIALER2CTRL1, 9 /* EvVolume */, OnEvVolumeScdialer2ctrl1, VTS_I2 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSCVoIPDlg::OnEvHangupMiradialerctrl1() 
{
	// TODO: Add your control notification handler code here
	m_strPhoneNumber.Empty();
	UpdateData(FALSE);

	CString strBtnString;
	m_buttonDial.GetWindowText(strBtnString);

	// 2005-10-06 by cmkwon, 통화 종료가 한번만 발생하도록 수정
	if(0 == strBtnString.Compare("Dial"))
	{
		return;
	}
	m_buttonDial.SetWindowText("Dial");

	m_nGameVoIPCallType		= _BothCall;
	m_bIsPartyCall			= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 

	MSG_EV_GOOUT_USER msg;
	memset(&msg, 0x00, sizeof(msg));
	strcpy(msg.szCharacterName, "상대방");	
	AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_GOOUT_USER, 0);
	COPYDATASTRUCT	copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	copyData.dwData = (DWORD)avMsg;
	copyData.lpData = &msg;
	copyData.cbData = sizeof(msg);	
	::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}

void CSCVoIPDlg::OnEvReceiveCallMiradialerctrl1(LPCTSTR Caller) 
{
	// TODO: Add your control notification handler code here
	m_buttonDial.SetWindowText("Hangup");

	m_ctrlSCDialer1.AcceptCall(TRUE);
}

void CSCVoIPDlg::OnEvAlertMiradialerctrl1() 
{
	// TODO: Add your control notification handler code here
 	m_buttonDial.SetWindowText("Hangup");	
}

void CSCVoIPDlg::OnEvErrorMiradialerctrl1(short ErrCode, LPCTSTR ErrMsg) 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.Hangup(ErrCode*100);

	switch(ErrCode)
	{
	case 1:
		m_editStatus.SetWindowText("Invalid product");
		break;
	case 2:
		m_editStatus.SetWindowText("Invalid PCN");
		break;
	case 3:
		m_editStatus.SetWindowText("Invalid Number");
		break;
	case 4:
		m_editStatus.SetWindowText("Server Error");
		break;
	case 5:
		m_editStatus.SetWindowText("No Free Channel");
		break;
	case 6:
		m_editStatus.SetWindowText("Network Error");
		break;
	case 7:
		m_editStatus.SetWindowText("No bandwidth");
		break;
	case 12:
		m_editStatus.SetWindowText("No Permission");
		break;
	case 17:
		m_editStatus.SetWindowText("Busy.. Try again");
		break;
	case 30:
		m_editStatus.SetWindowText("No Answer");
		break;
	case 31: // Refuse 가 오는 경우는 이미 등록된 PCN 일경우, 상대방이 서비스 이용 유보 상태, Caller 가 서버에 등록되지 않았을 때(이경우는 전화 시도도 안하므로 발생 하지 않겠지?)
		m_editStatus.SetWindowText("Already login");
		break;
	case 32:
		m_editStatus.SetWindowText("Incorrect pwd");
		break;
	case 35:
		m_editStatus.SetWindowText("Not Login");
		break;
	case 36: // Login by somewhere
		break;
	case 50:
		m_editStatus.SetWindowText("Using Serial Port");
		break;
	case 51:
		m_editStatus.SetWindowText("Invalid Serial Port");
		break;
	}
	
	if(NULL != m_hWndAtumVoIPServer)
	{
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_CLIENT_ERROR, ErrCode);
		copyData.dwData = (DWORD)avMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}	

	SetTimer(TIMER_RESET_1to1_STATUS, 3000, NULL);
}

void CSCVoIPDlg::OnEvConnectedMiradialerctrl1() 
{
	// TODO: Add your control notification handler code here
	m_buttonDial.SetWindowText("Hangup");
}

void CSCVoIPDlg::OnEvRegistrationMiradialerctrl1(long Date, LPCTSTR URL) 
{
	// TODO: Add your control notification handler code here

	char	szString[256];
	sprintf(szString, "set;username;%s", m_szCharacterName);
	m_ctrlSCDialer1.SendMessage("[Server]", szString);

	m_buttonLogin.SetWindowText("Login status");
	UpdateData();


//	DBGOUT("OnEvRegistrationMiradialerctrl1		==> %s\n", szString);
	COPYDATASTRUCT copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_REGISTERED, 0);
	copyData.dwData = (DWORD)avMsg;
	::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}

void CSCVoIPDlg::OnEvUnRegistrationMiradialerctrl1(short RetryRegisterServer) 
{
	// TODO: Add your control notification handler code here
	m_buttonLogin.SetWindowText("Logout status");
	m_editStatus.SetWindowText("Not login");
}

void CSCVoIPDlg::OnEvStatusMiradialerctrl1(short Status) 
{
	// TODO: Add your control notification handler code here
	BOOL bNotifyMsg = TRUE;
	switch(Status)
	{
	case -1:
		m_strStatus = "Not login";
	case 0:
		if(m_ctrlSCDialer1.m_hWnd && m_ctrlSCDialer1.IsRegistered())
			m_strStatus = "Ready";
		else
			m_strStatus = "Not Login";
		break;
	case 1:

		// 2005-10-06 by cmkwon, 통화 종료가 한번만 발생하도록 수정
		if(0 == m_strStatus.Compare("Connecting"))
		{
			bNotifyMsg = FALSE;
		}
		m_strStatus = "Connecting";
		m_buttonDial.SetWindowText("Hangup");
		break;
	case 2:
		m_strStatus = "Ringing";
		break;
	case 3:
		// 2005-10-06 by cmkwon, 통화 시작이 한번만 발생하도록 수정
		if(0 == m_strStatus.Compare("Speaking"))
		{
			bNotifyMsg = FALSE;
		}
		m_strStatus = "Speaking";
		break;
	case 4:
		m_strStatus = "Error";
		break;
	default:
		m_strStatus = "Unknown Status";
		break;
	}

	DBGOUT("[SCVoIP.exe] SCDialer1 Status(%s)\n", m_strStatus);
	if(m_editStatus)
		UpdateData(FALSE);
	
	if(m_hWndAtumVoIPServer
		&& bNotifyMsg)
	{
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_CLIENT_STATUS, Status);
		copyData.dwData = (DWORD)avMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

void CSCVoIPDlg::OnEvReceiveButtonMiradialerctrl1(LPCTSTR Num) 
{
	// TODO: Add your control notification handler code here
	
}

void CSCVoIPDlg::OnEvVolumeMiradialerctrl1(short Type, long Value) 
{
	// TODO: Add your control notification handler code here

	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
	COPYDATASTRUCT copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_GET_SPK_VOLUME_OK, Value);
	copyData.dwData = (DWORD)avMsg;
	::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}

void CSCVoIPDlg::OnEvRetrievePhoneBookMiradialerctrl1(LPCTSTR Login, LPCTSTR NotLogin) 
{
	// TODO: Add your control notification handler code here
	
}

void CSCVoIPDlg::OnEvWCAInfoMiradialerctrl1(LPCTSTR IP, long Port) 
{
	// TODO: Add your control notification handler code here
	// 빌링 서버 정보를 입력하게 된다.
}

void CSCVoIPDlg::OnEvRetrieveAudioDeviceListMiradialerctrl1(LPCTSTR AudioDeviceList) 
{
	// TODO: Add your control notification handler code here
	
}

void CSCVoIPDlg::OnEvPushButtonMiradialerctrl1(LPCTSTR Num) 
{
	// TODO: Add your control notification handler code here
	m_strPhoneNumber += Num;
	m_editPhoneNumber.SetWindowText(m_strPhoneNumber);
}

void CSCVoIPDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("1");
}

void CSCVoIPDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("2");	
}

void CSCVoIPDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("3");	
}

void CSCVoIPDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("4");	
}

void CSCVoIPDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("5");	
}

void CSCVoIPDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("6");	
}

void CSCVoIPDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("7");	
}

void CSCVoIPDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("8");	
}

void CSCVoIPDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("9");	
}

void CSCVoIPDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("*");	
}

void CSCVoIPDlg::OnButton0() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("0");	

	m_ctrlSCDialer1.SendMessage("030348762395", "Stop");
}

void CSCVoIPDlg::OnButton11() 
{
	// TODO: Add your control notification handler code here
	m_ctrlSCDialer1.PushButton("#");	
}

void CSCVoIPDlg::OnRadioPh() 
{
	// TODO: Add your control notification handler code here
	m_nCallType = 1;
}


void CSCVoIPDlg::OnEvReceiveMessageMiradialerctrl1(LPCTSTR Source, LPCTSTR Message) 
{
	// TODO: Add your control notification handler code here
	
}

void CSCVoIPDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

BOOL CSCVoIPDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	AV_MSG_WM_COPYDATA	avMsg(pCopyDataStruct->dwData);

	if(AV_MT_CLIENT_END == avMsg.nMsgType
		|| AV_MT_UNREGISTER == avMsg.nMsgType)
	{
		if(m_ctrlSCDialer1.IsRegistered())
		{
			if(m_ctrlSCDialer1.GetDialButtonClicked())
			{
				m_ctrlSCDialer1.Hangup(10);
			}
			m_ctrlSCDialer1.UnRegisterServer();
		}
		m_ctrlSCDialer2.GoOutRoom();
		m_nGameVoIPCallType		= _BothCall;
		m_bIsPartyCall			= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 


		if(AV_MT_CLIENT_END == avMsg.nMsgType)
		{
			SetTimer(TIMERID_ATUM_VOIP_END, 200, NULL);
		}

		return CDialog::OnCopyData(pWnd, pCopyDataStruct);
	}

	
	BOOL bReted = FALSE;
	///////////////////////////////////////////////////////////////////////////////
	// 1. 1:1 통화 처리
	// 2. 편대가 통화 처리
	switch(avMsg.nCallType)
	{
	case _1to1Call:
		{
			if(_BothCall == m_nGameVoIPCallType
				|| m_nGameVoIPCallType == avMsg.nCallType)
			{
				bReted = Process1to1Message(avMsg.nMsgType, avMsg.nMsgParam, pCopyDataStruct);
			}			
		}		
		break;
	case _NtoNCall:
		{
			if(_BothCall == m_nGameVoIPCallType
				|| m_nGameVoIPCallType == avMsg.nCallType)
			{
				bReted = ProcessNtoNMessage(avMsg.nMsgType, avMsg.nMsgParam, pCopyDataStruct);
			}
		}
		break;
	}
	if(FALSE == bReted)
	{
		DbgOut("[Error] Unknow Message GameVoIPCallType(%d) CallType(%d) MsgType(%3d) MsgParam(%4d) cbData(0x%8X) lpData(0x%X)\n"
			, m_nGameVoIPCallType, avMsg.nCallType, avMsg.nMsgType, avMsg.nMsgParam
			, pCopyDataStruct->cbData, pCopyDataStruct->lpData);

		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA tmMsg((AVCallType)avMsg.nCallType, AV_EV_CLIENT_ERROR, _SC2_NORMAL_ERROR);
		copyData.dwData = (DWORD)tmMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

BOOL CSCVoIPDlg::Process1to1Message(BYTE i_nMsgType, USHORT i_nMsgParam, COPYDATASTRUCT* pCopyDataStruct)
{
	COPYDATASTRUCT copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));		

	switch(i_nMsgType)
	{
	case AV_MT_REGISTER:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_REGISTER)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}
			
			m_nGameVoIPCallType = _1to1Call;
			m_bIsPartyCall		= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 

			MSG_MT_REGISTER *pMsgRegister = (MSG_MT_REGISTER*)pCopyDataStruct->lpData;
			m_strPCN.Format("%d", pMsgRegister->nAccountUniqueNumber);
			UpdateData(FALSE);
			strncpy(m_szCharacterName, pMsgRegister->szCharacterName, SIZE_MAX_CHARACTER_NAME);
			if(m_ctrlSCDialer1.IsRegistered())
			{
				AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_REGISTERED, 0);
				copyData.dwData = (DWORD)avMsg;
				::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
			}
			else
			{
				OnButtonLogin();
			}
		}
		break;
	case AV_MT_DIAL:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_DIAL)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}

			MSG_MT_DIAL	*pMsgDial = (MSG_MT_DIAL*)pCopyDataStruct->lpData;
			char	szText[128];
			m_buttonDial.GetWindowText(szText, sizeof(szText));
			if (strcmp(szText, "Dial") == 0)
			{
				m_strPhoneNumber.Format("%d", pMsgDial->nCalleeAccountUniqueNumber);
				UpdateData(FALSE);					
				OnButtonDial();
			}
		}
		break;
	case AV_MT_GET_SPK_VOLUME:		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
		{
			BYTE bySpkVolOfPercent = (BYTE)m_ctrlSCDialer1.GetSpkVolume();

			COPYDATASTRUCT copyData;
			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
			AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_EV_GET_SPK_VOLUME_OK, bySpkVolOfPercent);
			copyData.dwData = (DWORD)avMsg;
			::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
		}
		break;
	case AV_MT_SET_SPK_VOLUME:		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
		{
//			DbgOut("081021-2 CSCVoIPDlg::Process1to1Message %d, %d\r\n", i_nMsgType, i_nMsgParam);
			m_ctrlSCDialer1.SetSpkVolume(i_nMsgParam);
		}
		break;
	default:
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CSCVoIPDlg::ProcessNtoNMessage(BYTE i_nMsgType, USHORT i_nMsgParam, COPYDATASTRUCT* pCopyDataStruct)
{
	COPYDATASTRUCT tmCopyData;
	memset(&tmCopyData, 0x00, sizeof(COPYDATASTRUCT));		

	switch(i_nMsgType)
	{
	case AV_MT_PARTY_CREATE_ROOM:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_PARTY_CREATE_ROOM)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}
			MSG_MT_PARTY_CREATE_ROOM *pRvMsg = (MSG_MT_PARTY_CREATE_ROOM*)pCopyDataStruct->lpData;
			m_nGameVoIPCallType	= _NtoNCall;
			m_bIsPartyCall		= TRUE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 

			m_ctrlSCDialer2.CreateRoomEx(pRvMsg->szCharacterName, (long*)&pRvMsg->llPartyID, pRvMsg->nConcurrentTalkingCount);
		}
		break;
	case AV_MT_PARTY_JOIN_ROOM:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_PARTY_JOIN_ROOM)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}
			MSG_MT_PARTY_JOIN_ROOM *pRvMsg = (MSG_MT_PARTY_JOIN_ROOM*)pCopyDataStruct->lpData;
			m_nGameVoIPCallType	= _NtoNCall;
			m_bIsPartyCall		= TRUE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 

			m_ctrlSCDialer2.JoinRoom(pRvMsg->szCharacterName, (long*)&pRvMsg->llPartyID);
		}
		break;
	case AV_MT_GUILD_CREATE_ROOM:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_GUILD_CREATE_ROOM)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}
			MSG_MT_GUILD_CREATE_ROOM *pRvMsg = (MSG_MT_GUILD_CREATE_ROOM*)pCopyDataStruct->lpData;
			m_nGameVoIPCallType	= _NtoNCall;
			m_bIsPartyCall		= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
			
			LONGLONG tmRoomID = MAKEUINT64(pRvMsg->uiGuildUniqueNumber, 0xFFFF);
			m_ctrlSCDialer2.CreateRoomEx(pRvMsg->szCharacterName, (long*)&tmRoomID, pRvMsg->nConcurrentTalkingCount);
		}
		break;
	case AV_MT_GUILD_JOIN_ROOM:
		{
			if(pCopyDataStruct->cbData != sizeof(MSG_MT_GUILD_JOIN_ROOM)
				|| NULL == pCopyDataStruct->lpData)
			{
				return FALSE;
			}
			MSG_MT_GUILD_JOIN_ROOM *pRvMsg = (MSG_MT_GUILD_JOIN_ROOM*)pCopyDataStruct->lpData;
			m_nGameVoIPCallType	= _NtoNCall;
			m_bIsPartyCall		= FALSE;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
			
			LONGLONG tmRoomID = MAKEUINT64(pRvMsg->uiGuildUniqueNumber, 0xFFFF);
			m_ctrlSCDialer2.JoinRoom(pRvMsg->szCharacterName, (long*)&tmRoomID);
		}
		break;
 	case AV_MT_TALK:
		{
			m_ctrlSCDialer2.SetAudioSendPause(FALSE);
		}
		break;
	case AV_MT_TALK_CANCEL:
		{
			m_ctrlSCDialer2.SetAudioSendPause(TRUE);
		}
		break;
	case AV_MT_GET_SPK_VOLUME:		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
		{
			BYTE bySpkVolOfPercent = (BYTE)m_ctrlSCDialer2.GetSpkVolume();

			COPYDATASTRUCT copyData;
			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
			AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_GET_SPK_VOLUME_OK, bySpkVolOfPercent);
			copyData.dwData = (DWORD)avMsg;
			::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
		}
		break;
	case AV_MT_SET_SPK_VOLUME:		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
		{
			m_ctrlSCDialer2.SetSpkVolume(i_nMsgParam);
		}
		break;
	default:
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CSCVoIPDlg::OnEvStatusScdialer2ctrl1(long i_nStatus) 
{
	// TODO: Add your control notification handler code here
	if(NULL == this->GetSafeHwnd()){		return;}

	char szStatus[512];
	memset(szStatus, 0x00, 512);
	switch(i_nStatus)
	{
	case _SC2_ERROR_STATUS:
		{
			sprintf(szStatus,"_SC2_ERROR_STATUS");
		}
		break;
	case _SC2_READY_STATUS:
		{
			sprintf(szStatus,"_SC2_READY_STATUS");
		}
		break;
	case _SC2_CONNECTING_VDSERVER_STATUS:
		{
			sprintf(szStatus,"_SC2_CONNECTING_VDSERVER_STATUS");
		}
		break;
	case _SC2_CONNECTED_VDSERVER_STATUS:
		{
			sprintf(szStatus,"_SC2_CONNECTED_VDSERVER_STATUS");
		}
		break;
	case _SC2_CONNECTING_VCSERVER_STATUS:
		{
			sprintf(szStatus,"_SC2_CONNECTING_VCSERVER_STATUS");
		}
		break;
	case _SC2_CONNECTED_VCSERVER_STATUS:
		{
			sprintf(szStatus,"_SC2_CONNECTED_VCSERVER_STATUS");
		}
		break;
	case _SC2_CREATED_ROOM_STATUS:
		{
			sprintf(szStatus,"_SC2_CREATED_ROOM_STATUS");

			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 권한을 자동으로 요청한다.
			if(m_bIsPartyCall)
			{
				m_ctrlSCDialer2.SetAudioSendPause(FALSE);
			}
		}
		break;
	case _SC2_JOINED_ROOM_STATUS:
		{
			sprintf(szStatus,"_SC2_JOINED_ROOM_STATUS");

			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 권한을 자동으로 요청한다.
			if(m_bIsPartyCall)
			{
				m_ctrlSCDialer2.SetAudioSendPause(FALSE);
			}
		}
		break;
	default:
		{
			sprintf(szStatus,"Unknown Status(%d)", i_nStatus);		
		}
	}
	DBGOUT("[SCVoIP.exe] SCDialer2 Status(%s)\n", szStatus);
	if(GetDlgItem(IDC_EDIT_STATUS2))
	{
		GetDlgItem(IDC_EDIT_STATUS2)->SetWindowText(szStatus);
	}

	if(NULL != m_hWndAtumVoIPServer)
	{
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_CLIENT_STATUS, (USHORT)i_nStatus);
		copyData.dwData = (DWORD)avMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);

		if(_SC2_CREATED_ROOM_STATUS == i_nStatus)
		{
			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
			avMsg.nCallType	= _NtoNCall;
			avMsg.nMsgType	= AV_EV_CREATE_ROOM_OK;
			avMsg.nMsgParam	= 0;
			copyData.dwData = (DWORD)avMsg;
			::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
		}
		else if(_SC2_JOINED_ROOM_STATUS == i_nStatus)
		{
			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
			avMsg.nCallType	= _NtoNCall;
			avMsg.nMsgType	= AV_EV_JOIN_ROOM_OK;
			avMsg.nMsgParam	= 0;
			copyData.dwData = (DWORD)avMsg;
			::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
		}
	}
}

void CSCVoIPDlg::OnEvErrorScdialer2ctrl1(long i_nErrorNum, long i_nErrParam1, long i_nErrParam2, LPCTSTR i_szErrString) 
{
	// TODO: Add your control notification handler code here
	if(NULL == this->GetSafeHwnd()){		return;}

	BOOL bResetSCDialer2 = TRUE;
	char szError[512];
	memset(szError, 0x00, 512);
	switch(i_nErrorNum)
	{
	case _SC2_NO_ERROR:
		{
			sprintf(szError,"_SC2_NO_ERROR");
		}
		break;
	case _SC2_NORMAL_ERROR:
		{
			sprintf(szError,"_SC2_NORMAL_ERROR");
		}
		break;
	case _SC2_INVALID_STATUS_ERROR:
		{
			sprintf(szError,"_SC2_INVALID_STATUS_ERROR");
		}
		break;
	case _SC2_INVALID_PARAM_ERROR:
		{
			sprintf(szError,"_SC2_INVALID_PARAM_ERROR");
		}
		break;
	case _SC2_INVALID_ROOMID_ERROR:
		{
			sprintf(szError,"_SC2_INVALID_ROOMID_ERROR");
		}
		break;
	case _SC2_CONNECT_VDSERVER_ERROR:
		{
			sprintf(szError,"_SC2_CONNECT_VDSERVER_ERROR");
		}
		break;
	case _SC2_CONNECT_VCSERVER_ERROR:
		{
			sprintf(szError,"_SC2_CONNECT_VCSERVER_ERROR");
		}
		break;
	case _SC2_VOIP_SERVER_ERROR:
		{
			sprintf(szError,"_SC2_VOIP_SERVER_ERROR");
			if(VOIPERR_NtoN_FAIL_REQUEST_TOKEN == i_nErrParam1
				|| VOIPERR_NtoN_CANNOT_TALKING == i_nErrParam1)		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
			{
				bResetSCDialer2 = FALSE;
			}
		}
		break;
	default:
		{
			sprintf(szError,"Unknown Error(%d)", i_nErrorNum);		
		}
	}

	DBGOUT("[SCVoIP.exe] SCDialer2 Error(%s), ErrorCode(0x%X)\n", szError, i_nErrParam1);
	if(GetDlgItem(IDC_EDIT_ERROR2))
	{		
		GetDlgItem(IDC_EDIT_ERROR2)->SetWindowText(szError);
	}

	if(NULL != m_hWndAtumVoIPServer)
	{
		COPYDATASTRUCT copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_CLIENT_ERROR, (USHORT)i_nErrorNum);
		copyData.dwData = (DWORD)avMsg;
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}

	if(bResetSCDialer2)
	{
		SetTimer(TIMER_RESET_NtoN_STATUS, 2000, NULL);
	}
}

void CSCVoIPDlg::OnEvOtherJoinedRoomScdialer2ctrl1(LPCTSTR szJoinedUserName) 
{
	// TODO: Add your control notification handler code here
	if(NULL != m_hWndAtumVoIPServer)
	{
		MSG_EV_JOIN_USER msg;
		memset(&msg, 0x00, sizeof(msg));
		strncpy(msg.szCharacterName, szJoinedUserName, SIZE_MAX_CHARACTER_NAME);
		
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_JOIN_USER, 0);
		COPYDATASTRUCT	copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msg;
		copyData.cbData = sizeof(msg);
		
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

void CSCVoIPDlg::OnEvOtherGoOutedRoomScdialer2ctrl1(LPCTSTR szGoOutedUserName) 
{
	// TODO: Add your control notification handler code here
	if(NULL != m_hWndAtumVoIPServer)
	{
		MSG_EV_GOOUT_USER msg;
		memset(&msg, 0x00, sizeof(msg));
		strncpy(msg.szCharacterName, szGoOutedUserName, SIZE_MAX_CHARACTER_NAME);
		
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_GOOUT_USER, 0);
		COPYDATASTRUCT	copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msg;
		copyData.cbData = sizeof(msg);
		
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

void CSCVoIPDlg::OnEvTalkScdialer2ctrl1(LPCTSTR i_szUserName, long i_bIsTalk) 
{
	// TODO: Add your control notification handler code here

	if(NULL != m_hWndAtumVoIPServer)
	{
		MSG_EV_TALK_USER msg;
		memset(&msg, 0x00, sizeof(msg));
		strncpy(msg.szCharacterName, i_szUserName, SIZE_MAX_CHARACTER_NAME);
		msg.bTalkFlag = i_bIsTalk;
		
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_TALK_USER, 0);
		COPYDATASTRUCT	copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msg;
		copyData.cbData = sizeof(msg);
		
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

void CSCVoIPDlg::OnEvNotifyScdialer2ctrl1(long i_nNotifyCode, long i_nParam1, long i_nParam2, LPCTSTR i_szString) 
{
	// TODO: Add your control notification handler code here
	if(NULL == m_hWndAtumVoIPServer)
	{
		return;
	}
	
	AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_NOTIFY, i_nNotifyCode);
	COPYDATASTRUCT	copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	copyData.dwData = (DWORD)avMsg;
	::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}

void CSCVoIPDlg::OnEvChangedTalkingStateScdialer2ctrl1(LPCTSTR UserName, long TalkingState) 
{
	// TODO: Add your control notification handler code here
	if(NULL != m_hWndAtumVoIPServer)
	{
		MSG_EV_CHANGED_TALKING_STATE msg;
		memset(&msg, 0x00, sizeof(msg));
		strncpy(msg.szCharacterName, UserName, SIZE_MAX_CHARACTER_NAME);
		msg.nTakingState = TalkingState;
		
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_CHANGED_TALKING_STATE, 0);
		COPYDATASTRUCT	copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msg;
		copyData.cbData = sizeof(msg);
		
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

void CSCVoIPDlg::OnEvUserJoinedRoomScdialer2ctrl1(LPCTSTR UserName, long EnableTalking) 
{
	// TODO: Add your control notification handler code here
	if(NULL != m_hWndAtumVoIPServer)
	{
		MSG_EV_JOIN_USER msg;
		memset(&msg, 0x00, sizeof(msg));
		strncpy(msg.szCharacterName, UserName, SIZE_MAX_CHARACTER_NAME);
		msg.EnableTalking		= EnableTalking;		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
		
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_JOIN_USER, 0);
		COPYDATASTRUCT	copyData;
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msg;
		copyData.cbData = sizeof(msg);
		
		::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}	
}

void CSCVoIPDlg::OnEvVolumeScdialer2ctrl1(short Type, long Value) 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 처음 볼륨을 전달한다.
	COPYDATASTRUCT copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_EV_GET_SPK_VOLUME_OK, Value);
	copyData.dwData = (DWORD)avMsg;
	::SendMessage(m_hWndAtumVoIPServer, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}
