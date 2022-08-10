// AtumAdminToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumAdminTool.h"
#include "AtumAdminToolDlg.h"
#include "AtumProtocol.h"
#include "AtumError.h"
#include "PWDDlg.h"
#include "AtumAdminTool.h"
#include "SetLanguageDlg.h"
#include "ExpViewerDlg.h"
#include "SCGuildAdminDlg.h"
#include "xortestdlg.h"			// 2007-10-24 by cmkwon, 서버 정보 암호화 - 해더파일 추가
#include "PetitionManagementDlg.h"	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
#include "eventmonstermanagementdlg.h"		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
#include "cashshopmanagementdlg.h"			// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
#include "wrankingmanagement.h"				// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
#include "StaisticsMoneyDlg.h"
// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

#include "ArenaCharSend.h" // 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 퀴리만들기



#define WM_TIMER_FOR_SEND_ALIVE_PACKET		101

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolDlg dialog

CAtumAdminToolDlg::CAtumAdminToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAtumAdminToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAtumAdminToolDlg)
	m_UID = _T("");
	m_PWD = _T("");
	m_ctl_strLanguageString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pUserAdminDlg = NULL;
	m_pServerAdminDlg = NULL;
	m_pBadUserAdminDlg = NULL;
	m_pLogAdminDlg = NULL;
	m_pHappyHourEventAdminDlg = NULL;
	m_pExpViewer				= NULL;
	m_pGuildAdminDlg			= NULL;		// 2006-03-07 by cmkwon
	m_szServerName = "";
	m_pItemEventDlg	= NULL;
	m_pAdminPreSocket = NULL;
	m_pStrategyPointDlg = NULL;
	m_pXOREncodeDlg			= NULL;			// 2007-10-24 by cmkwon, 서버 정보 암호화 - 생성자 초기화
	m_pPetitionDlg			= NULL;			// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	m_pOutPostDlg			= NULL;
	m_pEventMonsterDlg		= NULL;			// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 

	m_pSCMonthlyArmorEventDlg = NULL;		// 2013-04-18 by jhseol,bckim 이달의 아머

	// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
	m_pStatisticsMoneyDlg	= NULL;
	// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

	m_pCArenaCharSendDlg	= NULL;	// 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 퀴리만들기

	m_pRenewalStrategyPointDlg	= NULL;	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전 툴 추가

	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
	m_pAdminAutoNoticeDlg	= NULL;

	m_pCashShopManagementDlg	= NULL;		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	m_pWRankingManagementDlg	= NULL;		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 


	MEMSET_ZERO(m_szManagerAccountName, SIZE_MAX_ACCOUNT_NAME);		// 2006-04-15 by cmkwon
	m_usManagerAccountType		= 0;								// 2006-04-15 by cmkwon

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
	// 2007-07-06 by cmkwon, SCAdminTool에서 OnlyServerAdmin관련 수정
	m_UID = _T(SCADMINTOOL_ONLY_SERVER_ADMIN_ACCOUNT_NAME);
	m_PWD = _T(SCADMINTOOL_ONLY_SERVER_ADMIN_PASSWORD);
#endif
}

CAtumAdminToolDlg::~CAtumAdminToolDlg()
{
	SAFE_DELETE(m_pAdminPreSocket);

	SAFE_DELETE(m_pUserAdminDlg);
	SAFE_DELETE(m_pServerAdminDlg);
	SAFE_DELETE(m_pBadUserAdminDlg);
	SAFE_DELETE(m_pLogAdminDlg);
	SAFE_DELETE(m_pHappyHourEventAdminDlg);
	SAFE_DELETE(m_pExpViewer);
	SAFE_DELETE(m_pGuildAdminDlg);
	SAFE_DELETE(m_pItemEventDlg);
	SAFE_DELETE(m_pStrategyPointDlg);
	SAFE_DELETE(m_pXOREncodeDlg);				// 2007-10-24 by cmkwon, 서버 정보 암호화 - 소멸자에서 삭제
	SAFE_DELETE(m_pPetitionDlg);				// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	SAFE_DELETE(m_pOutPostDlg);
	SAFE_DELETE(m_pEventMonsterDlg);			// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 

	SAFE_DELETE(m_pSCMonthlyArmorEventDlg);		// 2013-04-18 by jhseol,bckim 이달의 아머
	
	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
	SAFE_DELETE(m_pAdminAutoNoticeDlg);

	SAFE_DELETE(m_pCashShopManagementDlg);		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	SAFE_DELETE(m_pWRankingManagementDlg);		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

	// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
	SAFE_DELETE(m_pStatisticsMoneyDlg);
	// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가
}

void CAtumAdminToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAtumAdminToolDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER, m_ComboSelectServer);
	DDX_Text(pDX, IDC_EDIT_UID, m_UID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_PWD);
	DDX_Text(pDX, IDC_EDIT_LANGUAGE, m_ctl_strLanguageString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAtumAdminToolDlg, CDialog)
	//{{AFX_MSG_MAP(CAtumAdminToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_USER_TOOL, OnButtonUserTool)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SERVER, OnButtonServerTool)
	ON_BN_CLICKED(IDC_BUTTON_LOG_TOOL, OnButtonLogTool)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_SHOT_VIEWER, OnButtonScreenShotViewer)
	ON_COMMAND(ID_MENU_TRAY_EXIT, OnMenuTrayExit)
	ON_COMMAND(ID_MENU_TRAY_OPEN, OnMenuTrayOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LOCALIZATION, OnBtnLocalization)
	ON_BN_CLICKED(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL, OnButtonHappyHourEventTool)
	ON_BN_CLICKED(IDC_BTN_EXP_VIEWER, OnBtnExpViewer)
	ON_BN_CLICKED(IDC_BTN_GUILD_TOOL, OnBtnGuildTool)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_EVENT, OnButtonItemEvent)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT, OnButtonStrategypoint)
	ON_BN_CLICKED(IDC_BTN_XOR_ENCODE, OnBtnXorEncode)
	ON_BN_CLICKED(IDC_BTN_PETITION_SYTEM, OnBtnPetitionSytem)
	ON_BN_CLICKED(IDC_BUTTON_OUTPOST, OnButtonOutpost)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_MONSTER, OnButtonEventMonster)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_NOTICE, OnButtonAutoNotice)
	ON_BN_CLICKED(IDC_BTN_CASHSHOP_MANAGEMENT, OnBtnCashshopManagement)
	ON_BN_CLICKED(IDC_BTN_WRANKING_MANAGEMENT, OnBtnWrankingManagement)
	ON_BN_CLICKED(IDC_BTN_INFINITY, OnBtnInfinity)
	ON_BN_CLICKED(IDC_BTN_STATISTICS_MONEY, OnBtnStatisticsMoney)
	ON_BN_CLICKED(IDC_BTN_ARENA_CHAR_SEND, OnBtnArenaCharSend)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MONTHLY_ARMOR_EVENT, OnButtonMonthlyArmorEvent)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRE_PACKET_NOTIFY, OnSocketNotifyPre)
	ON_MESSAGE(WM_PRE_ASYNC_EVENT, OnAsyncSocketMessage)
//	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolDlg message handlers

BOOL CAtumAdminToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if(!IS_VALID_LANGUAGE_TYPE(this->m_nLanguageType))
	{
		this->m_nLanguageType = LANGUAGE_TYPE_DEFAULT;
	}
	if(!m_Localization.LoadConfiguration((LPSTR)(LPCSTR)m_strLocalizationDirectoryPath, m_nLanguageType))
	{
		MessageBox("Load Localization files fail !!\n\n Reset Localization files directory path.");
		this->m_nLanguageType = LANGUAGE_TYPE_DEFAULT;
	}
	SetLanguageString(this->m_nLanguageType);

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(FALSE);
// 2005-10-11 by cmkwon, 로그 검색까지 가능하도록 수정
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_UID)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCREEN_SHOT_VIEWER)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow(FALSE);

	// 2013-04-18 by jhseol,bckim 이달의 아머
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(FALSE);
	// End. 2013-04-18 by jhseol,bckim 이달의 아머

#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifdef


	int nComboSelIndex = 0;
	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ComboSelectServer.AddString(g_arrGameServers[i].ServerName);
		if (m_szServerName == g_arrGameServers[i].ServerName)
		{
			nComboSelIndex = i;
		}
	}

	m_ComboSelectServer.SetCurSel(nComboSelIndex);

	EnableToolControls(FALSE);

	SetTimer(0, 100, NULL);

//	if(!m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T("SpaceCowboy Admin Tool"), NULL, IDR_MENU_TRAY))
//	{
//		return -1;
//	}
//	m_TrayIcon.SetIcon(IDR_MAINFRAME);

	this->SetTimer(WM_TIMER_FOR_SEND_ALIVE_PACKET, 60000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAtumAdminToolDlg::OnPaint() 
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
HCURSOR CAtumAdminToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAtumAdminToolDlg::OnButtonUserTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif

	// TODO: Add your control notification handler code here
	if (m_pUserAdminDlg != NULL)
	{
		SAFE_DELETE(m_pUserAdminDlg);
	}	
	if (m_pUserAdminDlg == NULL)
	{
		m_pUserAdminDlg = new CSCUserAdminDlg(&m_Localization);
		m_pUserAdminDlg->Create(IDD_SCUSERADMIN_DIALOG, this);
	}

	m_pUserAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonServerTool() 
{

#ifndef _ATUM_ONLY_SERVER_ADMIN_TOOL
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif// _ATUM_ADMIN_RELEASE_end_ifdef
#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef

	// TODO: Add your control notification handler code here
	if (m_pServerAdminDlg != NULL)
	{
		SAFE_DELETE(m_pServerAdminDlg);
	}

	if (m_pServerAdminDlg == NULL)
	{
		m_pServerAdminDlg = new CSCServerAdminDlg;
		m_pServerAdminDlg->Create(IDD_DIALOG_SERVER_ADMIN_TOOL, this);
	}

	m_pServerAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonLogTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
	// TODO: Add your control notification handler code here
	if (m_pLogAdminDlg != NULL)
	{
		SAFE_DELETE(m_pLogAdminDlg);
	}

	if (m_pLogAdminDlg == NULL)
	{
		m_pLogAdminDlg = new CSCLogAdminDlg(this);
		m_pLogAdminDlg->Create(IDD_DIALOG_LOG_ADMIN_TOOL, this);
	}

	m_pLogAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonBadUserTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
	// TODO: Add your control notification handler code here
	if (m_pBadUserAdminDlg != NULL)
	{
		SAFE_DELETE(m_pBadUserAdminDlg);
	}

	if (m_pBadUserAdminDlg == NULL)
	{
		m_pBadUserAdminDlg = new CSCBadUserAdminDlg;
		m_pBadUserAdminDlg->Create(IDD_DIALOG_BAD_USER_ADMIN_TOOL, this);
	}

	m_pBadUserAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_UID == "")
	{
		MessageBox(STRERR_S_SCADMINTOOL_0004);
		GetDlgItem(IDC_EDIT_UID)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_UID))->SetSel(0, -1);
		return;
	}

	if (m_PWD == "")
	{
		MessageBox(STRERR_S_SCADMINTOOL_0002);
		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetSel(0, -1);
		return;
	}

	if (m_ComboSelectServer.GetCurSel() != -1)
	{
		m_pServerInfo4Admin = &g_arrGameServers[m_ComboSelectServer.GetCurSel()];
		m_ComboSelectServer.GetLBText(m_ComboSelectServer.GetCurSel(), m_szServerName);
	}
	else
	{
		m_pServerInfo4Admin = NULL;
	}

	if (FALSE == ConnectServer())
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-15 by cmkwon
	STRNCPY_MEMSET(m_szManagerAccountName, m_UID, SIZE_MAX_ACCOUNT_NAME);
	m_usManagerAccountType			= 0;

	((CAtumAdminToolApp*)AfxGetApp())->WriteProfile();
}

BOOL CAtumAdminToolDlg::ConnectServer()
{
	if(m_pAdminPreSocket)
	{
		MessageBox("Aready Connecting !!");
		return FALSE;
	}
	// Initialize winsock 2.0
	CWinSocket::SocketInit();

	// Make socket instance & connect
	m_pAdminPreSocket = new CSCAdminPreWinSocket("CAtumAdminToolDlg's PreServer Socket", this, GetSafeHwnd());

	if (!m_pAdminPreSocket->Connect(m_pServerInfo4Admin->ServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		MessageBox(STRERR_S_SCADMINTOOL_0005);
		SAFE_DELETE(m_pAdminPreSocket);
		EndDialog(-1);
		return FALSE;
	}

	return TRUE;
}

void CAtumAdminToolDlg::EnableToolControls(BOOL i_bEnable)
{
	GetDlgItem(IDC_STATIC_TOOLS)->EnableWindow(i_bEnable);
// 2007-07-06 by cmkwon, 중국은 운영자는 ServerAdmin만 접근 가능 - 아래와 같이 수정함
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//#ifndef _ATUM_ONLY_SERVER_ADMIN_TOOL
//	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//// 2005-10-11 by cmkwon, 로그 검색까지 가능하도록 수정
////	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef

// 2007-11-21 by cmkwon, 중국도 나머지 나라들과 GM 권한을 동일하게 처리한다. - 
//#if defined(_CHN_S1)		// 2007-07-06 by cmkwon, 중국은 운영자는 ServerAdmin만 접근 가능
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//	if(FALSE == i_bEnable
//		|| this->IsManagerAdministrator())
//	{
//		GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
//	}
//#elif !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)
//	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
//#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef
#if !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)	
	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
// 2012-01-07 by khkim, 베트남 GM 권한수정
#ifdef S_MANAGER_ADMIN_HSSON
	if(RACE_GAMEMASTER == m_usManagerAccountType)
	{
		GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
		GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
	}
#endif
// end 2012-01-07 by khkim, 베트남 GM 권한수정
	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BTN_PETITION_SYTEM)->EnableWindow(i_bEnable);		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_EVENT_MONSTER)->EnableWindow(i_bEnable);		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	GetDlgItem(IDC_BUTTON_AUTO_NOTICE)->EnableWindow(i_bEnable);		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
	GetDlgItem(IDC_BTN_CASHSHOP_MANAGEMENT)->EnableWindow(i_bEnable);	// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	GetDlgItem(IDC_BTN_WRANKING_MANAGEMENT)->EnableWindow(i_bEnable);	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
	GetDlgItem(IDC_BTN_STATISTICS_MONEY)->EnableWindow(i_bEnable);
	// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

	// 2013-04-18 by jhseol,bckim 이달의 아머
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 이벤트 리로드
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(i_bEnable);
#else
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(FALSE);
#endif
	// End. 2013-04-18 by jhseol,bckim 이달의 아머

#endif // END - #if !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)	


	GetDlgItem(IDC_BUTTON_SERVER)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(i_bEnable);

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(!i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_UID))->SetReadOnly(i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetReadOnly(i_bEnable);	
}


/******************************************************************************
**
**	계정 타입별 메뉴 권한 부여.
**
**	Create Info : 2010. 09. 06. by hsLee.
**
*******************************************************************************/
void CAtumAdminToolDlg :: EnableToolControls ( USHORT usAccountType )
{

	EnableToolControls ( TRUE );

	switch ( usAccountType )
	{
		case RACE_MONITOR :

			GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow( TRUE );
			GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_PETITION_SYTEM)->EnableWindow( FALSE );		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
			GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EVENT_MONSTER)->EnableWindow( FALSE );		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
			GetDlgItem(IDC_COMBO_SERVER)->EnableWindow ( FALSE );
			GetDlgItem(IDC_BUTTON_AUTO_NOTICE)->EnableWindow( FALSE );		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
			GetDlgItem(IDC_BTN_CASHSHOP_MANAGEMENT)->EnableWindow( FALSE );	// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
			GetDlgItem(IDC_BTN_WRANKING_MANAGEMENT)->EnableWindow( FALSE );	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
			GetDlgItem(IDC_BUTTON_SERVER)->EnableWindow( FALSE );
			// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
			GetDlgItem(IDC_BTN_STATISTICS_MONEY)->EnableWindow(FALSE);
			// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

			break;

		default :
			break;
	}
	
	

}

LONG CAtumAdminToolDlg::OnSocketNotifyPre(WPARAM wParam, LPARAM lParam)
{
	CSCAdminWinSocket *pSCAdminWinSocket = (CSCAdminWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
				INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_CONNECT, T_PA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				STRNCPY_MEMSET(msgConnect->UID, (LPCSTR)m_UID, SIZE_MAX_ACCOUNT_NAME);
				STRNCPY_MEMSET(msgConnect->PWD, (LPCSTR)m_PWD, SIZE_MAX_PASSWORD);
				m_pAdminPreSocket->Write(msgConnectBuf, MSG_SIZE(MSG_PA_ADMIN_CONNECT));		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈 (추가 기존 버그 수정)
			}
			else
			{
				// 연결 실패
				MessageBox(STRERR_S_SCADMINTOOL_0006);
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				switch(msgType)
				{
				case T_PA_ADMIN_CONNECT_OK:
					{
						MSG_PA_ADMIN_CONNECT_OK *msgConnectOK
							= (MSG_PA_ADMIN_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

// 2006-04-15 by cmkwon
//						if (!msgConnectOK->AuthOK)
						if (0 == msgConnectOK->AccountType0)
						{
							// 인증 실패
							MessageBox("Fail PreServer1 certification!!");
							EndDialog(-1);
						}
						else
						{// 인증 성공
							
							m_usManagerAccountType	= msgConnectOK->AccountType0;	// 2006-04-15 by cmkwon, 접속자 권한 설정

							//EnableToolControls(TRUE);

							EnableToolControls ( m_usManagerAccountType );
							// 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 마상 내부가 아니면 사용 불가
#ifdef S_ARENA_EX_1ST_JHSEOL
							if(msgConnectOK->IsMasang)
							{
								GetDlgItem(IDC_BTN_ARENA_CHAR_SEND)->ShowWindow(TRUE);	// 그냥 수정 EnableWindow() -> ShowWindow()
							}
#endif
							// end 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 마상 내부가 아니면 사용 불가
						}
					}
					break;
				case T_ERROR:
					{
						MSG_ERROR *pRecvMsg;
						pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						char buf[128];
						Err_t error = pRecvMsg->ErrorCode;

						//DBGOUT("ERROR %s(%#04X) received from %s[%s]\n", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_PRE_SERVER", m_pUpdateWinsocket->m_szPeerIP);

						switch (error)
						{
						case ERR_PROTOCOL_INVALID_PROTOCOL_TYPE:
							{
								MessageBox(STRERR_S_SCADMINTOOL_0008);
								OnCancel();
							}
							break;
						default:
							{
								sprintf(buf, "ERROR: %s(%#04X)", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								MessageBox(buf);
								OnCancel();
							}
							break;
						}
					}
				default:
					{
					}
					break;
				}
			}

			SAFE_DELETE(pPacket);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			// 2008-01-17 by cmkwon, T_A: PreServer와 연결 종료시 종료 되도록 처리
			char buf[1024];
			sprintf(buf, "Socket closed by PreServer !!");
			MessageBox(buf);
			OnCancel();
		}
		break;

	}	// end of switch

	return 0;
}

LONG CAtumAdminToolDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	m_pAdminPreSocket->OnAsyncEvent(lParam);

	return 0;
}


void CAtumAdminToolDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	m_pAdminPreSocket->CloseSocket();
	SAFE_DELETE(m_pAdminPreSocket);
	SAFE_DELETE(m_pUserAdminDlg);
	EnableToolControls(FALSE);
}

void CAtumAdminToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_UID != "")
		{
			GetDlgItem(IDC_EDIT_PWD)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetSel(0, -1);
		}
	}
	else if(nIDEvent == WM_TIMER_FOR_SEND_ALIVE_PACKET)
	{
		if(m_pAdminPreSocket
			&& m_pAdminPreSocket->IsConnected())
		{
			// 2005-01-02 by cmkwon, 임시 방편으로 다른것을 보낸다
			m_pAdminPreSocket->WriteMessageType(T_PM_CONNECT_ALIVE);
		}

		if(m_pUserAdminDlg)
		{
			m_pUserAdminDlg->OnTimerForSendAlivePacket();
		}

		if(m_pServerAdminDlg)
		{
			m_pServerAdminDlg->OnTimerForSendAlivePacket();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CAtumAdminToolDlg::OnButtonScreenShotViewer() 
{
	// TODO: Add your control notification handler code here
	char buff[40];
	GetCurrentDirectory(40, buff);
	UINT ret = WinExec("Vista.exe", SW_SHOWNORMAL);
}


//LONG CAtumAdminToolDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
//{
//	return m_TrayIcon.OnTrayNotification(wParam, lParam);
//}

BOOL CAtumAdminToolDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	m_TrayIcon.RemoveIcon();

	if(m_pAdminPreSocket){		m_pAdminPreSocket->CloseSocket();}

	if(m_pUserAdminDlg){		m_pUserAdminDlg->DestroyWindow();}
	if(m_pServerAdminDlg){		m_pServerAdminDlg->DestroyWindow();}
	if(m_pBadUserAdminDlg){		m_pBadUserAdminDlg->DestroyWindow();}
	if(m_pLogAdminDlg){			m_pLogAdminDlg->DestroyWindow();}
	if(m_pHappyHourEventAdminDlg){	m_pHappyHourEventAdminDlg->DestroyWindow();}
	if(m_pExpViewer){				m_pExpViewer->DestroyWindow();}
	if(m_pItemEventDlg){		m_pItemEventDlg->DestroyWindow();}
	if(m_pStrategyPointDlg){	m_pStrategyPointDlg->DestroyWindow();}
	if(m_pXOREncodeDlg){			m_pXOREncodeDlg->DestroyWindow();}		// 2007-10-24 by cmkwon, 서버 정보 암호화 - CAtumAdminToolDlg::DestroyWindow() 함수에서 종료 처리
	if(m_pPetitionDlg){				m_pPetitionDlg->DestroyWindow();}		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	if(m_pOutPostDlg){			m_pOutPostDlg->DestroyWindow();}
	if(m_pEventMonsterDlg){		m_pEventMonsterDlg->DestroyWindow();}		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 

	if(m_pRenewalStrategyPointDlg){m_pRenewalStrategyPointDlg->DestroyWindow();}	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전 툴 추가


	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
	if(m_pAdminAutoNoticeDlg){	m_pAdminAutoNoticeDlg->DestroyWindow();}

	if(m_pCashShopManagementDlg){	m_pCashShopManagementDlg->DestroyWindow();}		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
	if(m_pWRankingManagementDlg){	m_pWRankingManagementDlg->DestroyWindow();}		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	
	return CDialog::DestroyWindow();
}

void CAtumAdminToolDlg::OnMenuTrayExit() 
{
	// TODO: Add your command handler code here
	OnOK();
}

void CAtumAdminToolDlg::OnMenuTrayOpen() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_SHOW);
}


void CAtumAdminToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
//	if(nID == SC_MINIMIZE)
//	{
//		ShowWindow(SW_HIDE);
//	}
//	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAtumAdminToolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CAtumAdminToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAtumAdminToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(WM_TIMER_FOR_SEND_ALIVE_PACKET);
}

void CAtumAdminToolDlg::OnBtnLocalization() 
{
	// TODO: Add your control notification handler code here

	CSetLanguageDlg dlg(this->m_strLocalizationDirectoryPath, this->m_nLanguageType);
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}
	
	m_Localization.ResetLocalization();
	
	if(FALSE == m_Localization.LoadConfiguration((LPSTR)(LPCSTR)dlg.m_ctl_strLocalPath, dlg.m_nLanguageType))
	{
		MessageBox("Setting Localization files directory fail !!\n\nRetry.");
		return;
	}
	this->m_strLocalizationDirectoryPath	= dlg.m_ctl_strLocalPath;
	this->m_nLanguageType					= dlg.m_nLanguageType;
	((CAtumAdminToolApp*)AfxGetApp())->WriteProfile();
		
	this->SetLanguageString(this->m_nLanguageType);
	UpdateData(FALSE);
}

void CAtumAdminToolDlg::SetLanguageString(int i_nLanguageType)
{
	// 2008-04-25 by cmkwon, 지원 언어에 독일어 추가 - GET_LANGUAGE_TYPE_STRING() 함수로 처리
	m_ctl_strLanguageString = GET_LANGUAGE_TYPE_STRING(i_nLanguageType);

	UpdateData(FALSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::IsManagerAdministrator(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-15 ~ 2006-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::IsManagerAdministrator(void)
{
#ifdef S_MANAGER_ADMIN_HSSON
	return COMPARE_RACE(m_usManagerAccountType, RACE_OPERATION | RACE_GAMEMASTER);
#else 
	return COMPARE_RACE(m_usManagerAccountType, RACE_OPERATION);
#endif	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::SendMsgToPreServer(BYTE *i_pbyData, int i_nDataLen)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::SendMsgToPreServer(BYTE *i_pbyData, int i_nDataLen)
{
	if(NULL == m_pAdminPreSocket
		|| FALSE == m_pAdminPreSocket->IsConnected())
	{
		return FALSE;
	}

	return m_pAdminPreSocket->Write(i_pbyData, i_nDataLen);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::SendMsgTypeToPreServer(MessageType_t i_nMsgTy)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::SendMsgTypeToPreServer(MessageType_t i_nMsgTy)
{
	if(NULL == m_pAdminPreSocket
		|| FALSE == m_pAdminPreSocket->IsConnected())
	{
		return FALSE;
	}
	
	return m_pAdminPreSocket->WriteMessageType(i_nMsgTy);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-25 ~ 2009-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::IsConnectedToPreServer(void)
{
	if(NULL == m_pAdminPreSocket)
	{
		return FALSE;
	}
	
	return m_pAdminPreSocket->IsConnected();
}

void CAtumAdminToolDlg::OnButtonHappyHourEventTool() 
{
	// TODO: Add your control notification handler code here
	
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
	// TODO: Add your control notification handler code here
	if (m_pHappyHourEventAdminDlg != NULL)
	{
		SAFE_DELETE(m_pHappyHourEventAdminDlg);
	}

	if (m_pHappyHourEventAdminDlg == NULL)
	{
		m_pHappyHourEventAdminDlg = new CSCHappyHourEventAdminDlg(this);
		m_pHappyHourEventAdminDlg->Create(IDD_DIALOG_HAPPY_HOUR_EVENT_ADMIN, this);
	}

	m_pHappyHourEventAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnBtnExpViewer() 
{
	// TODO: Add your control notification handler code here
	if(NULL == m_pExpViewer)
	{
		m_pExpViewer = new CExpViewerDlg;
		m_pExpViewer->Create(IDD_DLG_EXP_VIEWER, this);
	}

	m_pExpViewer->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnBtnGuildTool() 
{
	// TODO: Add your control notification handler code here
	if (m_pGuildAdminDlg != NULL)
	{
		SAFE_DELETE(m_pGuildAdminDlg);
	}

	if (m_pGuildAdminDlg == NULL)
	{
		m_pGuildAdminDlg = new CSCGuildAdminDlg(&m_Localization, this);
		m_pGuildAdminDlg->Create(IDD_DLG_GUILD_ADMIN, this);
	}

	m_pGuildAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonItemEvent() 
{
	// TODO: Add your control notification handler code here
	if (m_pItemEventDlg != NULL)
	{
		SAFE_DELETE(m_pItemEventDlg);
	}

	if (m_pItemEventDlg == NULL)
	{
		m_pItemEventDlg = new CSCItemEventDlg(this);
		m_pItemEventDlg->Create(IDD_DIALOG_ITEM_EVENT_ADMIN, this);
	}

	m_pItemEventDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonStrategypoint() 
{
	// TODO: Add your control notification handler code here

// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전략포인트전 정보 검색은 가능, 수정 불가
// 	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 전략포인트 소환 설정 처리
// 	if(FALSE == this->IsManagerAdministrator())
// 	{
// 		AfxMessageBox("You are not have permission !!");
// 		return;
// 	}

	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전 툴 추가
#ifdef S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL
	if (m_pRenewalStrategyPointDlg != NULL)
	{
		SAFE_DELETE(m_pRenewalStrategyPointDlg);
	}
	
	if (m_pRenewalStrategyPointDlg == NULL)
	{
		m_pRenewalStrategyPointDlg = new CRenewalStrategyPointAdminDlg(this, m_pAdminPreSocket);
		m_pRenewalStrategyPointDlg->Create(IDD_DIALOG_RENEWAL_STRATEGYPOINT, this);
	}
	
	m_pRenewalStrategyPointDlg->ShowWindow(SW_SHOW);	
#else
	if (m_pStrategyPointDlg != NULL)
	{
		SAFE_DELETE(m_pStrategyPointDlg);
	}

	if (m_pStrategyPointDlg == NULL)
	{
		m_pStrategyPointDlg = new CSCStrategyPointAdminDlg(this);
		m_pStrategyPointDlg->Create(IDD_DIALOG_STRATEGYPOINT_ADMIN_TOOL, this);
	}

	m_pStrategyPointDlg->ShowWindow(SW_SHOW);	
#endif	// #ifdef S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전 툴 추가
}

void CAtumAdminToolDlg::OnBtnXorEncode() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, 서버 정보 암호화 - CAtumAdminToolDlg::OnBtnXorEncode() 에서 윈도우 생성 및 보여주기
	if(NULL == m_pXOREncodeDlg)
	{
		m_pXOREncodeDlg = new CXORTestDlg;
		m_pXOREncodeDlg->Create(IDD_DLG_XOR_TEST, this);
	}

	m_pXOREncodeDlg->ShowWindow(SW_SHOW);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnBtnPetitionSytem() 
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CAtumAdminToolDlg::OnBtnPetitionSytem() 추가
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnPetitionSytem() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	if(NULL == m_pPetitionDlg)
	{
		m_pPetitionDlg = new CPetitionManagementDlg;
		m_pPetitionDlg->Create(IDD_DLG_PETITION_MANAGEMENT, this);
	}

	m_pPetitionDlg->ShowWindow(SW_SHOW);	
}

void CAtumAdminToolDlg::OnButtonOutpost() 
{
	// TODO: Add your control notification handler code here
// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 전진기지전 정보 검색은 가능, 수정 불가
// 	if(FALSE == this->IsManagerAdministrator())
// 	{
// 		AfxMessageBox("You are not have permission !!");
// 		return;
// 	}

	if (m_pOutPostDlg != NULL)
	{
		SAFE_DELETE(m_pOutPostDlg);
	}

	if (m_pOutPostDlg == NULL)
	{
		m_pOutPostDlg = new CSCOutPostDlg(this);
		m_pOutPostDlg->Create(IDD_DLG_OUTPOST, this);
	}

	m_pOutPostDlg->ShowWindow(SW_SHOW);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnButtonEventMonster()
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CAtumAdminToolDlg::OnButtonEventMonster() 추가
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnButtonEventMonster() 
{
	// TODO: Add your control notification handler code here
	
	if (m_pEventMonsterDlg != NULL)
	{
		SAFE_DELETE(m_pEventMonsterDlg);
	}

	if (m_pEventMonsterDlg == NULL)
	{
		m_pEventMonsterDlg = new CEventMonsterManagementDlg(this);
		m_pEventMonsterDlg->Create(IDD_DLG_EVENT_MONSTER_MANAGEMENT, this);
	}

	m_pEventMonsterDlg->ShowWindow(SW_SHOW);	
}



// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - Admin Auto Notice Management
void CAtumAdminToolDlg::OnButtonAutoNotice() 
{
	if (m_pAdminAutoNoticeDlg != NULL)
	{
		SAFE_DELETE(m_pAdminAutoNoticeDlg);
	}
	
	if (m_pAdminAutoNoticeDlg == NULL)
	{
		m_pAdminAutoNoticeDlg = new CAdminAutoNoticeDlg(this);
		if(FALSE == m_pAdminAutoNoticeDlg->Create(IDD_DIALOG_ADMIN_AUTO_NOTICE, this))
		{
			return;
		}
	}
	
	m_pAdminAutoNoticeDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnBtnCashshopManagement()
/// \brief		// 2009-01-28 by cmkwon, 캐쉬샾 수정(추천탭,신상품 추가) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnCashshopManagement()
{
	if (m_pCashShopManagementDlg != NULL)
	{
		SAFE_DELETE(m_pCashShopManagementDlg);
	}
	
	if (m_pCashShopManagementDlg == NULL)
	{
		m_pCashShopManagementDlg = new CCashShopManagementDlg(this);
		if(FALSE == m_pCashShopManagementDlg->Create(IDD_DLG_CASHSHOP_MANAGEMENT, this))
		{
			return;
		}
	}
	
	m_pCashShopManagementDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnWrankingManagement() 
{
	// TODO: Add your control notification handler code here
	if (m_pWRankingManagementDlg != NULL)
	{
		SAFE_DELETE(m_pWRankingManagementDlg);
	}
	
	if (m_pWRankingManagementDlg == NULL)
	{
		m_pWRankingManagementDlg = new CWRankingManagement(this);
		if(FALSE == m_pWRankingManagementDlg->Create(IDD_DLG_WRK_MANAGEMENT, this))
		{
			return;
		}
	}
	
	m_pWRankingManagementDlg->ShowWindow(SW_SHOW);	
}

void CAtumAdminToolDlg::OnBtnInfinity()
{// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋
	// TODO: Add your control notification handler code here
	if(FALSE == this->IsManagerAdministrator()) {
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}
	
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL)) {
		return;
	}

	if(IDCANCEL == AfxMessageBox("Do you want to initialize all Infinity belonging information of the server?", MB_OKCANCEL)) {
		return;
	}

	CODBCStatement * m_pODBCStmt = new CODBCStatement;
	if (!m_pODBCStmt->Init(this->m_pServerInfo4Admin->DBIP, this->m_pServerInfo4Admin->DBPort, this->m_pServerInfo4Admin->DBName, this->m_pServerInfo4Admin->DBUID, this->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())) {
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return;
	}
	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0531));
	if (!bRet) {
		m_pODBCStmt->FreeStatement();		// cleanup
		
		AfxMessageBox("Infinity Initialize error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup	
	SAFE_DELETE(m_pODBCStmt);

	AfxMessageBox("Success !!");
}

// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
void CAtumAdminToolDlg::OnBtnStatisticsMoney() 
{
	// TODO: Add your control notification handler code here
	if (m_pStatisticsMoneyDlg != NULL)
	{
		SAFE_DELETE(m_pStatisticsMoneyDlg);
	}	
	if (m_pStatisticsMoneyDlg == NULL)
	{
		m_pStatisticsMoneyDlg = new CStaisticsMoneyDlg(this);
		m_pStatisticsMoneyDlg->Create(IDD_DIALOG_STATISTICS_MONEY, this);
	}
	
	m_pStatisticsMoneyDlg->ShowWindow(SW_SHOW);

}
// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가

// 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 퀴리만들기
void CAtumAdminToolDlg::OnBtnArenaCharSend() 
{
	// TODO: Add your control notification handler code here
	if (m_pCArenaCharSendDlg != NULL)
	{
		SAFE_DELETE(m_pCArenaCharSendDlg);
	}	
	if (m_pCArenaCharSendDlg == NULL)
	{
		m_pCArenaCharSendDlg = new CArenaCharSend(this);
		m_pCArenaCharSendDlg->Create(IDD_DIALOG_ARENA_CHAR_SEND, this);
	}
	
	m_pCArenaCharSendDlg->ShowWindow(SW_SHOW);
	
}
// end 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 퀴리만들기


// 2013-04-30 by bckim. 베트남 운영툴관련 처리ExchangeAccountDBName 
void CAtumAdminToolDlg::ExchangeAccountDBName(CString *szTmpQuery)
{
#if defined(INTECOM_VIET)
	if( m_pServerInfo4Admin != NULL )
	{
		if ( strlen(m_pServerInfo4Admin->DBIP) == strlen("117.103.194.75") )
		{
			if( 0 == strcmp( m_pServerInfo4Admin->DBIP, "117.103.194.75") )
			{
				CString strBeforeAccount = "atum2_db_account";
				CString strAfterAccount = "db02.atum2_db_account";
				szTmpQuery->Replace( (LPCTSTR)strBeforeAccount, (LPCTSTR)strAfterAccount );
				// 			DBGOUT(*szTmpQuery);
				// 			DBGOUT("\r\n");
			}
		}
	}
#endif
}
// End. 2013-04-30 by bckim. 베트남 운영툴관련 처리 

void CAtumAdminToolDlg::OnButtonMonthlyArmorEvent() 
{
	// TODO: Add your control notification handler code here
	if (m_pSCMonthlyArmorEventDlg != NULL)
	{
		SAFE_DELETE(m_pSCMonthlyArmorEventDlg);
	}	
	if (m_pSCMonthlyArmorEventDlg == NULL)
	{
		m_pSCMonthlyArmorEventDlg = new SCMonthlyArmorEventDlg(this);
		m_pSCMonthlyArmorEventDlg->Create(IDD_DLG_MONTHLY_ARMOR_EVENT, this);
	}
	
	m_pSCMonthlyArmorEventDlg->ShowWindow(SW_SHOW);
}

