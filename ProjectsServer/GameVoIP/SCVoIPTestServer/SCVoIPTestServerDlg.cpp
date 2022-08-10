// SCVoIPTestServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCVoIPTestServer.h"
#include "SCVoIPTestServerDlg.h"
#include "DlgInput.h"
#include "VoIPProtocol.h"

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
// CSCVoIPTestServerDlg dialog

CSCVoIPTestServerDlg::CSCVoIPTestServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCVoIPTestServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCVoIPTestServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hWndSCVoIPTestServerClient	= NULL;
}

void CSCVoIPTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCVoIPTestServerDlg)
	DDX_Control(pDX, IDC_EDIT_STATUS, m_editStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSCVoIPTestServerDlg, CDialog)
	//{{AFX_MSG_MAP(CSCVoIPTestServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_EXECUTE, OnBtnExecute)
	ON_BN_CLICKED(IDC_BTN_CALLER_PC2PC, OnBtnCallerPc2pc)
	ON_BN_CLICKED(IDC_BTN_CALL_END, OnBtnCallEnd)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_1to1_REGISTER, OnBTN1to1REGISTER)
	ON_BN_CLICKED(IDC_BTN_NtoN_CREATEROOM, OnBTNNtoNCREATEROOM)
	ON_BN_CLICKED(IDC_BTN_NtoN_JOINROOM, OnBTNNtoNJOINROOM)
	ON_BN_CLICKED(IDC_BTN_TALK, OnBtnTalk)
	ON_BN_CLICKED(IDC_BTN_1to1_SET_VOL, OnBTN1to1SETVOL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPTestServerDlg message handlers

BOOL CSCVoIPTestServerDlg::OnInitDialog()
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
	SetWindowText("VoIP_T_Server");	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSCVoIPTestServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSCVoIPTestServerDlg::OnPaint() 
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
HCURSOR CSCVoIPTestServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSCVoIPTestServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CSCVoIPTestServerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSCVoIPTestServerDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	AV_MSG_WM_COPYDATA	avMsg(pCopyDataStruct->dwData);

	switch(avMsg.nMsgType)
	{
	case AV_EV_CLIENT_ERROR:
		{
			short nErrCode = HIWORD(pCopyDataStruct->dwData);
			switch(nErrCode)
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
			case 31: 
				m_editStatus.SetWindowText("Already login");
				break;
			case 32:
				m_editStatus.SetWindowText("Incorrect pwd");
				break;
			case 35:
				m_editStatus.SetWindowText("Not Login");
				break;
			case 36: 
				break;
			case 50:
				m_editStatus.SetWindowText("Using Serial Port");
				break;
			case 51:
				m_editStatus.SetWindowText("Invalid Serial Port");
				break;
			}
		}
		break;
	case AV_EV_CLIENT_STATUS:
		{
			short	nStatus= HIWORD(pCopyDataStruct->dwData);
			switch(nStatus)
			{
			case -1:
				m_editStatus.SetWindowText("Not login");
			case 0:
				m_editStatus.SetWindowText("Ready");				
				break;
			case 1:
				m_editStatus.SetWindowText("Connecting");				
				break;
			case 2:
				m_editStatus.SetWindowText("Ringing");
				break;
			case 3:
				m_editStatus.SetWindowText("Speaking");
				break;
			case 4:
				m_editStatus.SetWindowText("Error");
				break;
			default:
				m_editStatus.SetWindowText("Unknown Status");
				break;
			}
		}
		break;
	case AV_EV_EXECUTE_OK:
		{
			m_hWndSCVoIPTestServerClient = pWnd->m_hWnd;
			m_editStatus.SetWindowText("Atum VoIP Executing");
		}
		break;
	default:
		{
		}
	}
	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CSCVoIPTestServerDlg::OnBtnExecute() 
{
	// TODO: Add your control notification handler code here
	// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(121.134.114.)
#ifdef _DEBUG
	ShellExecute(GetSafeHwnd(), "open", "./../../Bin/GameVoIP/Debug/SCVoIP.exe", "VoIP_Client VoIP_T_Server 61.39.170.131 15152 61.39.170.131 15150", ".", SW_SHOW);
#else
	ShellExecute(GetSafeHwnd(), "open", "SCVoIP.exe", "VoIP_Client VoIP_T_Server 61.39.170.131 15152 61.39.170.131 15150", ".", SW_SHOW);
#endif
	
}

void CSCVoIPTestServerDlg::OnBtnCallerPc2pc() 
{
	// TODO: Add your control notification handler code here
	if (NULL == m_hWndSCVoIPTestServerClient)
	{
		return;
	}

	CDlgInput	dlg(_1to1Call, 1, this);
	if(dlg.DoModal() == IDOK)
	{
		MSG_MT_DIAL		msgMTDial;
		memset(&msgMTDial, 0x00, sizeof(MSG_MT_DIAL));
		msgMTDial.nCalleeAccountUniqueNumber = dlg.m_clt_nAccountUniqueNumber;

		COPYDATASTRUCT	copyData;			
		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
		AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_MT_DIAL, 0);
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = &msgMTDial;
		copyData.cbData = sizeof(MSG_MT_DIAL);
		::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
	}
}

//void CSCVoIPTestServerDlg::OnBtnCallerPc2phone() 
//{
//	// TODO: Add your control notification handler code here
//	if (NULL == m_hWndSCVoIPTestServerClient)
//	{
//		return;
//	}
//
//	CDlgInput	dlg;
//	if(dlg.DoModal() == IDOK)
//	{
//		if(dlg.m_strPeerPCN.IsEmpty() == FALSE)
//		{
//			MSG_MT_DIAL		msgMTDial;
//			memset(&msgMTDial, 0x00, sizeof(MSG_MT_DIAL));
//			msgMTDial.bPC2Phone = TRUE;
//			strncpy(msgMTDial.AccountName, dlg.m_strPeerPCN, SIZE_MAX_ACCOUNT_NAME);
//
//			COPYDATASTRUCT	copyData;			
//			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//			AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_MT_DIAL, 0);
//			memcpy(&copyData.dwData, &avMsg, sizeof(copyData.dwData));
//			copyData.lpData = &msgMTDial;
//			copyData.cbData = sizeof(MSG_MT_DIAL);
//			::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
//		}
//	}
//}

void CSCVoIPTestServerDlg::OnBtnCallEnd() 
{
	// TODO: Add your control notification handler code here
	if (NULL == m_hWndSCVoIPTestServerClient)
	{
		return;
	}
			
//	::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);	
}

void CSCVoIPTestServerDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (NULL == m_hWndSCVoIPTestServerClient)
	{
		return;
	}
			
	COPYDATASTRUCT	copyData;			
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	AV_MSG_WM_COPYDATA avMsg(_BothCall, AV_MT_CLIENT_END, 0);
	copyData.dwData = (DWORD)avMsg;
	copyData.lpData = NULL;
	copyData.cbData = 0;
	::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}

BOOL CSCVoIPTestServerDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnExit();
	m_hWndSCVoIPTestServerClient = NULL;
	return CDialog::DestroyWindow();
}

void CSCVoIPTestServerDlg::OnBTN1to1REGISTER() 
{
	// TODO: Add your control notification handler code here

	CDlgInput	dlg(_1to1Call, 0, this);
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_ctl_szCharacterName.IsEmpty() == FALSE)
		{
			MSG_MT_REGISTER	msg;
			memset(&msg, 0x00, sizeof(MSG_MT_REGISTER));
			msg.nAccountUniqueNumber	= dlg.m_clt_nAccountUniqueNumber;
			strcpy(msg.szCharacterName, dlg.m_ctl_szCharacterName);
			
			COPYDATASTRUCT	copyData;			
			memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
			AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_MT_REGISTER, 0);
			copyData.dwData = (DWORD)avMsg;
			copyData.lpData = &msg;
			copyData.cbData = sizeof(msg);
			::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
		}
	}
}

void CSCVoIPTestServerDlg::OnBTNNtoNCREATEROOM() 
{
	// TODO: Add your control notification handler code here
	CDlgInput	dlg(_NtoNCall, 0, this);
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_ctl_szCharacterName.IsEmpty() == FALSE)
		{			
			if(dlg.m_bPartyCallFlag)
			{// Party CreateRoom
				
				MSG_MT_PARTY_CREATE_ROOM	msg;
				memset(&msg, 0x00, sizeof(MSG_MT_PARTY_CREATE_ROOM));
				msg.nAccountUniqueNumber	= dlg.m_clt_nAccountUniqueNumber;
				strcpy(msg.szCharacterName, dlg.m_ctl_szCharacterName);
				msg.llPartyID				= dlg.m_ctl_dwPartyID;
				
				AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_PARTY_CREATE_ROOM, 0);				
				COPYDATASTRUCT	copyData;
				memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
				copyData.dwData = (DWORD)avMsg;
				copyData.lpData = &msg;
				copyData.cbData = sizeof(msg);

				::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
			}
			else
			{// Guild CreateRoom
				
				MSG_MT_GUILD_CREATE_ROOM	msg;
				memset(&msg, 0x00, sizeof(MSG_MT_GUILD_CREATE_ROOM));
				msg.nAccountUniqueNumber	= dlg.m_clt_nAccountUniqueNumber;
				strcpy(msg.szCharacterName, dlg.m_ctl_szCharacterName);
				msg.uiGuildUniqueNumber		= dlg.m_ctl_dwPartyID;
				
				AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_GUILD_CREATE_ROOM, 0);
				COPYDATASTRUCT	copyData;
				memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
				copyData.dwData = (DWORD)avMsg;
				copyData.lpData = &msg;
				copyData.cbData = sizeof(msg);

				::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
			}
		}
	}
}

void CSCVoIPTestServerDlg::OnBTNNtoNJOINROOM() 
{
	// TODO: Add your control notification handler code here
	CDlgInput	dlg(_NtoNCall, 0, this);
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_ctl_szCharacterName.IsEmpty() == FALSE)
		{			
			if(dlg.m_bPartyCallFlag)
			{// Party JoinRoom

				COPYDATASTRUCT	copyData;			
				memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));

				MSG_MT_PARTY_JOIN_ROOM	msg;
				memset(&msg, 0x00, sizeof(MSG_MT_PARTY_JOIN_ROOM));
				msg.nAccountUniqueNumber	= dlg.m_clt_nAccountUniqueNumber;
				strcpy(msg.szCharacterName, dlg.m_ctl_szCharacterName);
				msg.llPartyID				= dlg.m_ctl_dwPartyID;
				
				AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_PARTY_JOIN_ROOM, 0);
				copyData.dwData = (DWORD)avMsg;
				copyData.lpData = &msg;
				copyData.cbData = sizeof(msg);

				::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
			}
			else
			{// Guild JoinRoom

				COPYDATASTRUCT	copyData;			
				memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));

				MSG_MT_GUILD_JOIN_ROOM	msg;
				memset(&msg, 0x00, sizeof(MSG_MT_GUILD_JOIN_ROOM));
				msg.nAccountUniqueNumber	= dlg.m_clt_nAccountUniqueNumber;
				strcpy(msg.szCharacterName, dlg.m_ctl_szCharacterName);
				msg.uiGuildUniqueNumber		= dlg.m_ctl_dwPartyID;
				
				AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_GUILD_JOIN_ROOM, 0);
				copyData.dwData = (DWORD)avMsg;
				copyData.lpData = &msg;
				copyData.cbData = sizeof(msg);

				::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
			}			
		}
	}
}



void CSCVoIPTestServerDlg::OnBtnTalk() 
{
	// TODO: Add your control notification handler code here

	CWnd *pBtnTalk = GetDlgItem(IDC_BTN_TALK);

	CString strTalk;
	pBtnTalk->GetWindowText(strTalk);

	COPYDATASTRUCT	copyData;
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
	if(strTalk.Compare("말하기") == 0)
	{
		pBtnTalk->SetWindowText("그만하기");
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_TALK, 0);
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = NULL;
		copyData.cbData = 0;
	}
	else
	{
		pBtnTalk->SetWindowText("말하기");
		AV_MSG_WM_COPYDATA avMsg(_NtoNCall, AV_MT_TALK_CANCEL, 0);
		copyData.dwData = (DWORD)avMsg;
		copyData.lpData = NULL;
		copyData.cbData = 0;
	}
	::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);	
}

void CSCVoIPTestServerDlg::OnBTN1to1SETVOL() 
{
	// TODO: Add your control notification handler code here
	
	COPYDATASTRUCT	copyData;			
	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));

	AV_MSG_WM_COPYDATA avMsg(_1to1Call, AV_MT_SET_SPK_VOLUME, 0);
	copyData.dwData = (DWORD)avMsg;
	
	::SendMessage(m_hWndSCVoIPTestServerClient, WM_COPYDATA, (WPARAM)GetSafeHwnd(), (LPARAM)&copyData);
}
