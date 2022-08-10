// AtumLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumLauncher.h"
#include "AtumLauncherDlg.h"
#include <iostream>
#include <strstream>
#include <fstream>
#include "dbgout_c.h"
#include "ftpdownload.h"
#include <direct.h>
#include "PreServerIPDlg.h"
#include "md5_lib_src.h"
#include "LoginSuccessDlg.h"
#include "Wininet.h"
#include "FTP\FTPManager.h"
#include "ZipArchive/ZipArchive.h"
// ysw
#include <afxhtml.h>
#include "MGameDecryption.h"
#include "AtumError.h"
#include "HttpManager.h"					// 2007-01-08 by cmkwon
#include "screenkeyboarddlg.h"				// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 -
#include "DlgGuardAgreement.h"				// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 

#include "VTCGuardManager.h"				// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

// 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]
#if defined(S_HACK_SHIELD_AUTO_UPDATE_HSKIM)
#include "ConfigHackShield.h"

#pragma comment(lib, "HSUpChk.lib")
#pragma comment(lib, "version.lib")
#include "Security/HSUpChk.h"
#endif
// 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NOTICE_FILE_NAME "notice.txt"
#define DELFILELIST_FILE_NAME "deletefilelist.txt"

#define STRING_SERVER_GROUP_NAME_DELIMIT				" "
#define TICKGAP_NETWORK_STATE_WORST_PING_TICK			1500		// 2007-06-21 by cmkwon, 평균 Ping 속도를 리턴하도록 수정함

struct SWINDOW_DEGREE
{
	char	*szWindowDegreeName;
	int		nCX;
	int		nCY;
	int		nDegree;
};

// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 추가함
SWINDOW_DEGREE g_pWindowDegreeList[] =
{
	//{STRMSG_WINDOW_DEGREE_1024x768_LOW,		1024, 768, 0},
	//{STRMSG_WINDOW_DEGREE_1024x768_MEDIUM,	1024, 768, 1},
	{STRMSG_WINDOW_DEGREE_1024x768_HIGH,	1024, 768, 2},

	//{STRMSG_WINDOW_DEGREE_W1280x720_LOW,	1280, 720, 0},
	//{STRMSG_WINDOW_DEGREE_W1280x720_MEDIUM,	1280, 720, 1},
	{STRMSG_WINDOW_DEGREE_1280x720_HIGH,	1280, 720, 2},
	
	//{STRMSG_WINDOW_DEGREE_W1280x800_LOW,	1280, 800, 0},
	//{STRMSG_WINDOW_DEGREE_W1280x800_MEDIUM,	1280, 800, 1},
	{STRMSG_WINDOW_DEGREE_1280x800_HIGH,	1280, 800, 2},
	
	//{STRMSG_WINDOW_DEGREE_1280x960_LOW,		1280, 960, 0},
	//{STRMSG_WINDOW_DEGREE_1280x960_MEDIUM,	1280, 960, 1},
	{STRMSG_WINDOW_DEGREE_1280x960_HIGH,	1280, 960, 2},
	
	//{STRMSG_WINDOW_DEGREE_1280x1024_LOW,	1280, 1024, 0},
	//{STRMSG_WINDOW_DEGREE_1280x1024_MEDIUM,	1280, 1024, 1},
	{STRMSG_WINDOW_DEGREE_1280x1024_HIGH,	1280, 1024, 2},
	
	// 2008-02-11 by cmkwon, 해상도 추가(1440x900) - 
	//{STRMSG_WINDOW_DEGREE_1440x900_LOW,		1440, 900, 0},
	//{STRMSG_WINDOW_DEGREE_1440x900_MEDIUM,	1440, 900, 1},
	{STRMSG_WINDOW_DEGREE_1440x900_HIGH,	1440, 900, 2},

	//{STRMSG_WINDOW_DEGREE_W1600x900_LOW,	1600, 900, 0},
	//{STRMSG_WINDOW_DEGREE_W1600x900_MEDIUM,	1600, 900, 1},
	{STRMSG_WINDOW_DEGREE_1600x900_HIGH,	1600, 900, 2},
	
	//{STRMSG_WINDOW_DEGREE_1600x1200_LOW,	1600, 1200, 0},
	//{STRMSG_WINDOW_DEGREE_1600x1200_MEDIUM,	1600, 1200, 1},
	{STRMSG_WINDOW_DEGREE_1600x1200_HIGH,	1600, 1200, 2},

	//{STRMSG_WINDOW_DEGREE_1680x1050_LOW		,	1680, 1050, 0},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	//{STRMSG_WINDOW_DEGREE_1680x1050_MEDIUM	,	1680, 1050, 1},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	{STRMSG_WINDOW_DEGREE_1680x1050_HIGH	,	1680, 1050, 2},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 

	//{STRMSG_WINDOW_DEGREE_1920x1080_LOW		,	1920, 1080, 0},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	//{STRMSG_WINDOW_DEGREE_1920x1080_MEDIUM	,	1920, 1080, 1},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	{STRMSG_WINDOW_DEGREE_1920x1080_HIGH	,	1920, 1080, 2},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 

	//{STRMSG_WINDOW_DEGREE_1920x1200_LOW		,	1920, 1200, 0},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	//{STRMSG_WINDOW_DEGREE_1920x1200_MEDIUM	,	1920, 1200, 1},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	//{STRMSG_WINDOW_DEGREE_1920x1200_HIGH	,	1920, 1200, 2},		// 2009-10-16 by cmkwon, 지원 해상도 추가(1680x1050,1920x1080,1920x1200) - 
	{STRMSG_WINDOW_DEGREE_2560x1080_HIGH	,	2560, 1080, 2},
	{STRMSG_WINDOW_DEGREE_2560x1440_HIGH	,	2560, 1440, 2},
	{STRMSG_WINDOW_DEGREE_3440x1440_HIGH	,	3440, 1440, 2},
	{STRMSG_WINDOW_DEGREE_3840x2160_HIGH	,	3840, 2160, 2},
	{STRMSG_WINDOW_DEGREE_5120x2160_HIGH	,	5120, 2160, 2},

	//{ "Borderless (low)", 0, 0, 0 },
	//{ "Borderless (medium)", 0, 0, 1 },
	//{ "Borderless (high)", 0, 0, 2 },

	{NULL, 0, 0, 0}		// 2007-12-28 by cmkwon, 끝을 구분하기 위해
};



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

#define ICON_IMAGE_ID		IDB_ICON_VTC

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherDlg dialog
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 인자 수정됨
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){   return 0;}
CAtumLauncherDlg::CAtumLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAtumLauncherDlg::IDD, pParent), m_ctrlServerList(IDB_SERVERUSER_VTC, IDB_LISTBG_VTC, ICON_IMAGE_ID)
{
	//{{AFX_DATA_INIT(CAtumLauncherDlg)
	m_nServer = -1;
	m_szAccountName = _T("");
	m_szPassword = _T("");
	m_staticNumFileCtrl = _T("");
	m_nWindowDegree = 0;
	m_ctlbWindowMode = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pUpdateWinsocket = 0;
	m_nOldSel = 0;
//	m_pFieldWinsocket = 0;
	m_bControlEnabled = TRUE;

	m_StaticBrushBlack.CreateSolidBrush(RGB(0, 0, 0));
	m_StaticBrushGray.CreateSolidBrush(RGB(23, 23, 23));
	m_ListBrushGray.CreateSolidBrush(RGB(212, 208, 200));
	
	m_listBrush.CreateSolidBrush(RGB(29,29,40));



	m_progressBk.CreateSolidBrush(RGB(79,79,79));	
	m_progressBar.CreateSolidBrush(RGB(99, 123, 246));	

	m_pUpdateFTPManager	= NULL;
	m_pHost = NULL;
	m_pHttpManager		= NULL;				// 2007-01-08 by cmkwon
	SetFTPUpdateState(UPDATE_STATE_INIT);


	strcpy(m_szLocalIP, "127.0.0.1");

	m_nBirthYear		= 0;				// 2007-06-05 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-18 by cmkwon, 네트워크 상태 체크 
	m_nMaxNetworkCheckCount		= 5;
	m_nCurNetworkCheckCount		= -1;
	m_dwNetworkCheckSendTick	= 0;
	m_dwSumPacketTickGap		= 0;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-07 by cmkwon, 베트남 런처 인터페이스 수정 - 초기화 위치 변경
	m_Cur_Percent				= 0;
	m_SelectFlag				= FALSE;	
	m_bCancelFlag				= FALSE;
	m_bProcessingVersionUpdate	= FALSE;
	m_bShowPreServerIPDlg		= TRUE;

	// 2007-09-11 by cmkwon, 베트남 화면키보드 구현 -
	m_pScreenKeyboardDlg			= NULL;
	m_pInputEditFromScreenKeyboard	= NULL;
	m_bHideScreenKeyboardByScreenKeyboardWindow	= FALSE; // 2007-09-18 by cmkwon, 화상키보드 수정 - 

	m_vectServerGroupList.clear();		// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 

	m_bGuardAgreementReg		= FALSE;	// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 

	m_pVTCGuard = NULL;		// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
}

void CAtumLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_REMEMBER_ID, m_ctlBtnRememberID);
	DDX_Control(pDX, IDC_COMBO_SERVER_LIST, m_comboServerList);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_ctrlEditAccount);
	DDX_Control(pDX, IDC_LIST, m_ctrlServerList);					// 2006-05-02 by ispark
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_szPassword);
	DDV_MaxChars(pDX, m_szPassword, 14);
	DDX_Text(pDX, IDC_DOWNLOAD_FILENUM, m_staticNumFileCtrl);
	DDX_CBIndex(pDX, IDC_COMBO_WINDOW_DEGREE_LAUNCHER, m_nWindowDegree);
	DDX_Check(pDX, IDC_CHECK_WINDOWS_MODE, m_ctlbWindowMode);
	DDX_Control(pDX, IDGO, m_KbcGO);
	DDX_Control(pDX, IDJOIN, m_kbcBtnJoin);
	DDX_Control(pDX, IDC_BTN_HOMEPAGE, m_bmpBtnHomepage);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAtumLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CAtumLauncherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDJOIN, OnJoin)
	ON_BN_CLICKED(IDSECRET, OnSecret)
	ON_BN_CLICKED(IDEND, OnEnd)
	ON_BN_CLICKED(IDDECA, OnDeca)
	ON_BN_CLICKED(IDBATTAL, OnBattal)
	ON_BN_CLICKED(IDSHARIN, OnSharin)
	ON_BN_CLICKED(IDPHILON, OnPhilon)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST, OnSelchangeList)
	ON_BN_CLICKED(IDCAN, OnCan)
	ON_BN_CLICKED(IDMIN, OnMin)
	ON_EN_SETFOCUS(IDC_EDIT_ACCOUNT, OnSetfocusEditAccount)
	ON_EN_SETFOCUS(IDC_EDIT_PASSWORD, OnSetfocusEditPassword)
	ON_BN_CLICKED(IDC_BTN_VIEW_SCREEN_KEYBOARD, OnBtnViewScreenKeyboard)
	ON_BN_CLICKED(IDC_BTN_HOMEPAGE, OnBtnHomepage)
	ON_BN_CLICKED(IDGO, OnOk)
	ON_BN_CLICKED(IDC_CHECK_WINDOWS_MODE, OnCheckWindowsMode)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PACKET_NOTIFY, OnSocketNotify)
	ON_MESSAGE(WM_ASYNC_EVENT, OnAsyncSocketMessage)
	ON_MESSAGE(WM_DOWNLOAD_GAMEFILES_DONE, OnDownLoadGamefilesDone)

	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_ERROR, OnUpdateFileDownloadError)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_INIT, OnUpdateFileDownloadInit)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_PROGRESS, OnUpdateFileDownloadProgress)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_OK, OnUpdateFileDownloadOK)
	ON_MESSAGE(WM_UPDATE_VTCGUARD_OK, OnUpdateVTCGuardOK)		// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherDlg message handlers


BOOL CAtumLauncherDlg::ReadNoticeFile()
{
	char	szLine[10000];
	CString strTemp;

	m_NoticeEdit = "";

	FILE		*fp;
#ifdef _ATUM_DEVELOP
	fp = fopen(m_szExecutePathReg + NOTICE_FILE_NAME, "r");
#else
	fp = fopen(NOTICE_FILE_NAME, "r");
#endif

	if (fp == NULL)
	{
		//szLine[0] = '\0';
		//strcpy(temp, m_NoticeEdit);
		//m_NoticeEdit.Format("%s%s",temp, szLine);
	}
	else
	{
		while(!feof(fp))
		{
			memset(szLine, 0x00, sizeof(szLine));
			if(fgets(szLine, 10000, fp)==NULL)
			{
				break;
			}

			szLine[strlen(szLine)-1] = 0;
			strTemp = m_NoticeEdit;
			m_NoticeEdit.Format("%s\r\n%s", strTemp, szLine);
		}
		fclose(fp);
	}

	// 2007-09-07 by cmkwon, 사용하지 않는 것이므로 컨트롤을 삭제함
	//GetDlgItem(IDC_NOTICE)->SetWindowText(m_NoticeEdit);
	return TRUE;
}

	#define EXE2_LAUNCHER_BG_SIZE_X					800
	#define EXE2_LAUNCHER_BG_SIZE_Y					536


	#define EXE2_BG_TITLE_BAR_SIZE_X				EXE2_LAUNCHER_BG_SIZE_X	// 타이틀바 이미지 Width
	#define EXE2_BG_TITLE_BAR_SIZE_Y				22						// 타이틀바 Height

	#define EXE2_BG_BACKGROUND_IMAGE_SIZE_X			EXE2_LAUNCHER_BG_SIZE_X									// 배경 이미지 Width
	#define EXE2_BG_BACKGROUND_IMAGE_SIZE_Y			(EXE2_LAUNCHER_BG_SIZE_Y - EXE2_BG_TITLE_BAR_SIZE_Y)	// 배경 이미지 Height

#define EXE2_BG_WEBCONTROL_POS_X				19		// 웹브라우저 컨트롤 위치 X
#define EXE2_BG_WEBCONTROL_POS_Y				67		// 웹브라우저 컨트롤 위치 Y
#define EXE2_BG_WEBCONTROL_WIDTH				519		// 웹브라우저 컨트롤 Width
#define EXE2_BG_WEBCONTROL_HEIGHT				421		// 웹브라우저 컨트롤 Height
	
#define EXE2_BG_RESOLUTION_COMBOBOX_POS_X		574		// 해상도 선택 Combo Box 위치 X
#define EXE2_BG_RESOLUTION_COMBOBOX_POS_Y		473		// 해상도 선택 Combo Box 위치 Y
#define EXE2_BG_RESOLUTION_COMBOBOX_WIDTH		200		// 해상도 선택 Combo Box Width
	#define EXE2_BG_RESOLUTION_COMBOBOX_HEIGHT		16		// 해상도 선택 Combo Box Height

#if defined(S_LAUNCHER_USE_ID_PASSWORD_HSKIM)
	#define EXE2_BG_WINDOWSMODE_CHECKBOX_POS_X		680		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box 위치 X
	#define EXE2_BG_WINDOWSMODE_CHECKBOX_POS_Y		454		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box 위치 Y

	#define EXE2_BG_ACCOUNTNAME_EDIT_POS_X			653		// AccountName Edit Box 위치 X
	#define EXE2_BG_ACCOUNTNAME_EDIT_POS_Y			326		// AccountName Edit Box 위치 Y
	#define EXE2_BG_ACCOUNTNAME_EDIT_WIDTH			117		// AccountName Edit Box Width
	#define EXE2_BG_ACCOUNTNAME_EDIT_HEIGHT			15		// AccountName Edit Box Height

	#define EXE2_BG_PASSWORD_EDIT_POS_X				653		// Password Edit Box 위치 X
	#define EXE2_BG_PASSWORD_EDIT_POS_Y				349		// Password Edit Box 위치 Y
	#define EXE2_BG_PASSWORD_EDIT_WIDTH				117		// Password Edit Box Width
	#define EXE2_BG_PASSWORD_EDIT_HEIGHT			15		// Password Edit Box Height

	#define EXE2_BG_GAMESTART_BTN_POS_X				544		// GameStart Button 위치 X
	#define EXE2_BG_GAMESTART_BTN_POS_Y				397		// GameStart Button 위치 Y
	#define EXE2_BG_GAMESTART_BTN_WIDTH				256		// GameStart Button Width
	#define EXE2_BG_GAMESTART_BTN_HEIGHT			57		// GameStart Button Height

#ifndef INTECOM_VIET
	#define EXE2_BG_REMEMBERID_CHECKBOX_POS_X		680		// 2013-03-25 by jhseol, 게임포지 영국 ID 체크버튼 위치 수정 기존 위치(689)에서 -20 만큼
#else
	#define EXE2_BG_REMEMBERID_CHECKBOX_POS_X		680		// Remember ID CheckBox 위치 X
#endif
	#define EXE2_BG_REMEMBERID_CHECKBOX_POS_Y		382		// Remember ID CheckBox 위치 Y
	#define EXE2_BG_REMEMBERID_CHECKBOX_WIDTH		13		// Remember ID CheckBox Width
	#define EXE2_BG_REMEMBERID_CHECKBOX_HEIGHT		13		// Remember ID CheckBox Height
#else
	#define EXE2_BG_WINDOWSMODE_CHECKBOX_POS_X		690		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box 위치 X
	#define EXE2_BG_WINDOWSMODE_CHECKBOX_POS_Y		456		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box 위치 Y

		#define EXE2_BG_ACCOUNTNAME_EDIT_POS_X			666		// AccountName Edit Box 위치 X
		#define EXE2_BG_ACCOUNTNAME_EDIT_POS_Y			518		// AccountName Edit Box 위치 Y
		#define EXE2_BG_ACCOUNTNAME_EDIT_WIDTH			117		// AccountName Edit Box Width
		#define EXE2_BG_ACCOUNTNAME_EDIT_HEIGHT			15		// AccountName Edit Box Height

		#define EXE2_BG_PASSWORD_EDIT_POS_X				666		// Password Edit Box 위치 X
		#define EXE2_BG_PASSWORD_EDIT_POS_Y				534		// Password Edit Box 위치 Y
		#define EXE2_BG_PASSWORD_EDIT_WIDTH				117		// Password Edit Box Width
		#define EXE2_BG_PASSWORD_EDIT_HEIGHT			15		// Password Edit Box Height	

	#define EXE2_BG_GAMESTART_BTN_POS_X				544		// GameStart Button 위치 X
	#define EXE2_BG_GAMESTART_BTN_POS_Y				307		// GameStart Button 위치 Y
	#define EXE2_BG_GAMESTART_BTN_WIDTH				256		// GameStart Button Width
	#define EXE2_BG_GAMESTART_BTN_HEIGHT			143		// GameStart Button Height

	#define EXE2_BG_REMEMBERID_CHECKBOX_POS_X		666		// Remember ID CheckBox 위치 X
	#define EXE2_BG_REMEMBERID_CHECKBOX_POS_Y		480		// Remember ID CheckBox 위치 Y
	#define EXE2_BG_REMEMBERID_CHECKBOX_WIDTH		13		// Remember ID CheckBox Width
	#define EXE2_BG_REMEMBERID_CHECKBOX_HEIGHT		13		// Remember ID CheckBox Height
	#endif

#define EXE2_BG_WINDOWSMODE_CHECKBOX_WIDTH		13		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box Width
#define EXE2_BG_WINDOWSMODE_CHECKBOX_HEIGHT		13		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box Height

#define EXE2_BG_SERVERLIST_BOX_POS_X			570		// ServerList Box 위치 X
#define EXE2_BG_SERVERLIST_BOX_POS_Y			130		// ServerList Box 위치 Y
#define EXE2_BG_SERVERLIST_BOX_WIDTH			208		// ServerList Box Width
#define EXE2_BG_SERVERLIST_BOX_HEIGHT			30		// ServerList Box Height

#define EXE2_BG_SERVERLIST_ITEM_BG_WIDTH		208		// ServerList Item Background Width
#define EXE2_BG_SERVERLIST_ITEM_BG_HEIGHT		30		// ServerList Item Background Height

#define EXE2_BG_SERVERLIST_ITEM_ICON_POS_X		6		// ServerList Item Icon 위치 X
#define EXE2_BG_SERVERLIST_ITEM_ICON_POS_Y		9		// ServerList Item Icon 위치 Y
#define EXE2_BG_SERVERLIST_ITEM_ICON_WIDTH		16		// ServerList Item Icon Width
#define EXE2_BG_SERVERLIST_ITEM_ICON_HEIGHT		18		// ServerList Item Icon Height

#define EXE2_BG_MINIMIZED_BTN_POS_X				750		// Minimized Button 위치 X
		#define EXE2_BG_MINIMIZED_BTN_POS_Y				0		// Minimized Button 위치 Y
#define EXE2_BG_MINIMIZED_BTN_WIDTH				23		// Minimized Button Width
#define EXE2_BG_MINIMIZED_BTN_HEIGHT			22		// Minimized Button Height
		#define EXE2_BG_CANCEL_BTN_POS_X				773		// Cancel Button 위치 X
		#define EXE2_BG_CANCEL_BTN_POS_Y				0		// Cancel Button 위치 Y
	#define EXE2_BG_CANCEL_BTN_WIDTH				23		// Cancel Button Width
	#define EXE2_BG_CANCEL_BTN_HEIGHT				22		// Cancel Button Height

	#define EXE2_BG_UPDATE_PROGRESS_BAR_POS_X		17		// Update Progress Bar 위치 X
	#define EXE2_BG_UPDATE_PROGRESS_BAR_POS_Y		517		// Update Progress Bar 위치 Y = (EXE2_LAUNCHER_BG_SIZE_Y - 23)
	#define EXE2_BG_UPDATE_PROGRESS_BAR_WIDTH		766		// Update Progress Bar Width = (EXE2_LAUNCHER_BG_SIZE_X - 2*EXE2_BG_UPDATE_PROGRESS_BAR_POS_X)
	#define EXE2_BG_UPDATE_PROGRESS_BAR_HEIGHT		8		// Update Progress Bar Height

	#define EXE2_BG_DOWNLOAD_FILE_STATIC_POS_X		20		// Download file info static control 위치 X
	#define EXE2_BG_DOWNLOAD_FILE_STATIC_POS_Y		500		// Download file info static control 위치 Y
	#define EXE2_BG_DOWNLOAD_FILE_STATIC_WIDTH		200		// Download file info static control Width
	#define EXE2_BG_DOWNLOAD_FILE_STATIC_HEIGHT		13		// Download file info static control Height
	#define EXE2_BG_DOWNLOAD_FILE_FONT_SIZE			12		// Download file info static control font Size
	#define EXE2_BG_DOWNLOAD_FILE_FONT_WEIGHT		FW_NORMAL	// Download file info static control font 굵기

	#define EXE2_BG_DOWNLOAD_FILE_FONT_COLOR		RGB(255, 255, 255)	// Download file info static control font Color

	#define EXE2_BG_UPDATE_INFO_STATIC_POS_X		130		// Update info static control 위치 X
	#define EXE2_BG_UPDATE_INFO_STATIC_POS_Y		500		// Update info static control 위치 Y
	#define EXE2_BG_UPDATE_INFO_STATIC_WIDTH		290		// Update info static control Width
	#define EXE2_BG_UPDATE_INFO_STATIC_HEIGHT		13		// Update info static control Height
	#define EXE2_BG_UPDATE_INFO_FONT_SIZE			12		// Update info static control font Size
	#define EXE2_BG_UPDATE_INFO_FONT_WEIGHT			FW_NORMAL	// Update info static control font 굵기

	#define EXE2_BG_UPDATE_INFO_FONT_COLOR			RGB(255, 255, 255)	// Update info static control font Color

BOOL CAtumLauncherDlg::OnInitDialog()
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

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 
#if defined(_DEFINED_GAMEFORGE4D_)
	if(FALSE == this->m_bGuardAgreementReg)
	{
		CDlgGuardAgreement GuardDlg;
		if(IDOK != GuardDlg.DoModal())
		{
			OnOK();
			return FALSE;
		}
		this->m_bGuardAgreementReg	= TRUE;
		((CAtumLauncherApp*)AfxGetApp())->WriteProfile();
	}
#endif

#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 -
	m_pScreenKeyboardDlg	= new CScreenKeyboardDlg(this);
	m_pScreenKeyboardDlg->Create(IDD_DLG_SCREEN_KEYBOARD, this);
	// 2007-09-18 by cmkwon, 화상키보드 수정 - 패스워드 입력시에만 활성화 되도록 일단 숨긴다
	//m_pScreenKeyboardDlg->ShowWindow(SW_SHOW);
	m_pScreenKeyboardDlg->ShowWindow(SW_HIDE);
#endif

	// 2007-09-11 by cmkwon, 베트남 화면키보드 구현 - 요청시까지 버튼은 보여지지 안는다
	//GetDlgItem(IDC_BTN_VIEW_SCREEN_KEYBOARD)->ShowWindow(SW_SHOW);		


	// 작업 표시줄에 프로그램명 표시
	AfxGetApp()->m_pMainWnd->SetWindowText(STRMSG_WINDOW_TEXT);
	// 작업 표시줄에 아이콘 표시
	ModifyStyle(0,WS_SYSMENU);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	///////////////////////////////////////////////////////////////////////////
	// TODO: Add extra initialization here

	CWinSocket::SocketInit();		// Initialize winsock 2.0

	this->MoveWindow2Center();	// 2007-09-07 by cmkwon, 베트남 런처 인터페이스 수정 - 화면 중앙으로 위치
	RECT rtBG;
	this->GetClientRect(&rtBG);

	// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 지원 해상도 리스트 초기화
	this->InitSupportedWindowResolutionList();		
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-07 by cmkwon, 해상도 정보를 각 나라별 언어로 설정하기 위해
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER);
	if(pComboBox)
	{
		this->InsertWindowDegreeList(pComboBox, m_ctlbWindowMode);



		int nIdx = this->FindWindowDegreeComboBoxIndex(pComboBox, (LPSTR)(LPCSTR)m_csWindowsResolutionReg);
		nIdx = max(0, nIdx);
		pComboBox->SetCurSel(nIdx);

	}
	
    m_fontServerGroupListBox.CreateFont(16, 0, 0, 0, SG_BOX_FONT_WEIGHT, 0, FALSE, FALSE, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , SG_BOX_FONT_FACENAME);    // "System" Font는 대표적인 Fixed Font임다.
    GetDlgItem(IDC_LIST)->SetFont(&m_fontServerGroupListBox);

	
	m_bitmapBtnCancel.AutoLoad(IDCAN, this);
	m_bitmapBtnMin.AutoLoad(IDMIN, this);
// 2008-01-08 by cmkwon, Yedang-Global 프로젝트에 추가
//#if defined(_GLOBAL_ENG_SERVER)			// 2006-09-07 by cmkwon, 수정(#ifdef _VTC_VIET_SERVER --> #ifndef _GLOBAL_ENG_SERVER)
//	m_KbcGO.SetBmpButtonImage(IDB_GOBTN, RGB(0,0,255));
//#elif defined(_KOREA_SERVER_2)		// 2006-10-02 by cmkwon
#if defined(S_LAUNCHER_USE_ID_PASSWORD_HSKIM)		// 2006-10-02 by cmkwon
	//m_KbcGO.SetBmpButtonImage(IDB_GO_VTC, RGB(0,0,255));
	m_KbcGO.SetBmpButtonImage(IDB_GOBTN, RGB(0,0,255));
#else
	m_KbcGO.SetBmpButtonImage(IDB_GO_S2, RGB(0,0,255));
#endif

	m_KbcGO.SetToolTipText("Game Start");
	m_kbcBtnJoin.SetBmpButtonImage(IDB_JOINBTN, RGB(0,0,255));
	m_kbcBtnJoin.SetToolTipText("Join");


	///////////////////////////////////////////////////////////////////////////////
	// 컨트롤 위치 설정하기 ----> Start

	// 2007-09-07 by cmkwon, WebBrowser control
	char strWebAddress[1024];
	//STRNCPY_MEMSET(strWebAddress, LAUNCHER_WEB_URL, 1024);
#ifdef S_ACCESS_INTERNAL_SERVER_HSSON
	//strWebAddress		= TESTSERVER_LAUNCHER_WEB_URL;			// 2006-08-04 by cmkwon, 테섭 Launcher 웹페이지
	//STRNCPY_MEMSET(strWebAddress, TESTSERVER_LAUNCHER_WEB_URL, 1024);
#endif

	RECT rt;
	rt.left		= EXE2_BG_WEBCONTROL_POS_X;
	rt.top		= EXE2_BG_WEBCONTROL_POS_Y;
	rt.right	= EXE2_BG_WEBCONTROL_POS_X + EXE2_BG_WEBCONTROL_WIDTH;
	rt.bottom	= EXE2_BG_WEBCONTROL_POS_Y + EXE2_BG_WEBCONTROL_HEIGHT;

	// 해상도 선택 ComboBox
	GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->MoveWindow(EXE2_BG_RESOLUTION_COMBOBOX_POS_X, EXE2_BG_RESOLUTION_COMBOBOX_POS_Y, EXE2_BG_RESOLUTION_COMBOBOX_WIDTH, EXE2_BG_RESOLUTION_COMBOBOX_HEIGHT);

	// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - Windows Mode Check Box 위치 크기 설정
	GetDlgItem(IDC_CHECK_WINDOWS_MODE)->MoveWindow(EXE2_BG_WINDOWSMODE_CHECKBOX_POS_X, EXE2_BG_WINDOWSMODE_CHECKBOX_POS_Y, EXE2_BG_WINDOWSMODE_CHECKBOX_WIDTH, EXE2_BG_WINDOWSMODE_CHECKBOX_HEIGHT);

	GetDlgItem(IDC_CHARACTER_NAME)->ShowWindow(SW_HIDE);



	// AccountName Edit Box, Password Edit Box
	GetDlgItem(IDC_EDIT_ACCOUNT)->MoveWindow(EXE2_BG_ACCOUNTNAME_EDIT_POS_X, EXE2_BG_ACCOUNTNAME_EDIT_POS_Y, EXE2_BG_ACCOUNTNAME_EDIT_WIDTH, EXE2_BG_ACCOUNTNAME_EDIT_HEIGHT);
	GetDlgItem(IDC_EDIT_PASSWORD)->MoveWindow(EXE2_BG_PASSWORD_EDIT_POS_X, EXE2_BG_PASSWORD_EDIT_POS_Y, EXE2_BG_PASSWORD_EDIT_WIDTH, EXE2_BG_PASSWORD_EDIT_HEIGHT);

#if defined(S_LAUNCHER_USE_ID_PASSWORD_HSKIM) || defined(_DEBUG)	// 2007-09-07 by cmkwon, 한국 예타임만 Release만 입력이 없다
	GetDlgItem(IDC_EDIT_ACCOUNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(SW_SHOW);
#endif


	// 서버리스트 List Box
	GetDlgItem(IDC_LIST)->MoveWindow(EXE2_BG_SERVERLIST_BOX_POS_X, EXE2_BG_SERVERLIST_BOX_POS_Y, EXE2_BG_SERVERLIST_BOX_WIDTH, EXE2_BG_SERVERLIST_BOX_HEIGHT);
	GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);		
	m_ctrlServerList.SetItemHeight(30);
	m_ctrlServerList.SetItemBGImageSize(EXE2_BG_SERVERLIST_ITEM_BG_WIDTH, EXE2_BG_SERVERLIST_ITEM_BG_HEIGHT);
	m_ctrlServerList.SetItemIconImage(EXE2_BG_SERVERLIST_ITEM_ICON_POS_X, EXE2_BG_SERVERLIST_ITEM_ICON_POS_Y, EXE2_BG_SERVERLIST_ITEM_ICON_WIDTH, EXE2_BG_SERVERLIST_ITEM_ICON_HEIGHT);
	m_ServerList = (CListBoxEBX *)GetDlgItem(IDC_LIST);

	// Minimized Button, Cancel Button
	GetDlgItem(IDMIN)->MoveWindow(EXE2_BG_MINIMIZED_BTN_POS_X, EXE2_BG_MINIMIZED_BTN_POS_Y, EXE2_BG_MINIMIZED_BTN_WIDTH, EXE2_BG_MINIMIZED_BTN_HEIGHT);
	GetDlgItem(IDCAN)->MoveWindow(EXE2_BG_CANCEL_BTN_POS_X, EXE2_BG_CANCEL_BTN_POS_Y, EXE2_BG_CANCEL_BTN_WIDTH, EXE2_BG_CANCEL_BTN_HEIGHT);

	// Game Start Button
	GetDlgItem(IDGO)->MoveWindow(EXE2_BG_GAMESTART_BTN_POS_X, EXE2_BG_GAMESTART_BTN_POS_Y, EXE2_BG_GAMESTART_BTN_WIDTH, EXE2_BG_GAMESTART_BTN_HEIGHT);

	// 2007-09-07 by cmkwon, 사용하지 않는 버튼임
	//// Join Button
	//GetDlgItem(IDJOIN)->MoveWindow(nPosX, nPosY, IMAGE_JOIN_BUTTON_X_SIZE, IMAGE_JOIN_BUTTON_Y_SIZE);

	// Update Progress Bar
	m_progressCtrl.MoveWindow(EXE2_BG_UPDATE_PROGRESS_BAR_POS_X, EXE2_BG_UPDATE_PROGRESS_BAR_POS_Y, EXE2_BG_UPDATE_PROGRESS_BAR_WIDTH, EXE2_BG_UPDATE_PROGRESS_BAR_HEIGHT);

	m_progressCtrl.SetGradientColors(RGB(255, 0, 0), RGB(0, 0, 255));



	// Download File Version Num
	GetDlgItem(IDC_DOWNLOAD_FILENUM)->MoveWindow(EXE2_BG_DOWNLOAD_FILE_STATIC_POS_X, EXE2_BG_DOWNLOAD_FILE_STATIC_POS_Y, EXE2_BG_DOWNLOAD_FILE_STATIC_WIDTH, EXE2_BG_DOWNLOAD_FILE_STATIC_HEIGHT);
	long wndStyle = ::GetWindowLong(GetDlgItem(IDC_DOWNLOAD_FILENUM)->m_hWnd, GWL_STYLE);
    ::SetWindowLong(GetDlgItem(IDC_DOWNLOAD_FILENUM)->m_hWnd, GWL_STYLE, wndStyle | SS_CENTERIMAGE);	// 취득한 윈도우 수직중앙(SS_CENTERIMAGE)속성을 추가
	m_fontDownloadFileNum.CreateFont(EXE2_BG_DOWNLOAD_FILE_FONT_SIZE, 0, 0, 0, EXE2_BG_DOWNLOAD_FILE_FONT_WEIGHT, 0, FALSE, FALSE, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , SG_BOX_FONT_FACENAME);    // "System" Font는 대표적인 Fixed Font임다.
	GetDlgItem(IDC_DOWNLOAD_FILENUM)->SetFont(&m_fontDownloadFileNum);

	// Download File Info
	GetDlgItem(IDC_FILE_INFO)->MoveWindow(EXE2_BG_UPDATE_INFO_STATIC_POS_X, EXE2_BG_UPDATE_INFO_STATIC_POS_Y, EXE2_BG_UPDATE_INFO_STATIC_WIDTH, EXE2_BG_UPDATE_INFO_STATIC_HEIGHT);
	wndStyle = ::GetWindowLong(GetDlgItem(IDC_FILE_INFO)->m_hWnd, GWL_STYLE);
    ::SetWindowLong(GetDlgItem(IDC_FILE_INFO)->m_hWnd, GWL_STYLE, wndStyle | SS_CENTERIMAGE);	// 취득한 윈도우 수직중앙(SS_CENTERIMAGE)속성을 추가
	m_fontFileInfo.CreateFont(EXE2_BG_UPDATE_INFO_FONT_SIZE, 0, 0, 0, EXE2_BG_UPDATE_INFO_FONT_WEIGHT, 0, FALSE, FALSE, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , SG_BOX_FONT_FACENAME);    // "System" Font는 대표적인 Fixed Font임다.
	GetDlgItem(IDC_FILE_INFO)->SetFont(&m_fontFileInfo);

	
#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-27 by cmkwon, Homepage가기 버튼 추가(베트남 VTC-Intecom 요청) - 
	GetDlgItem(IDC_BTN_HOMEPAGE)->MoveWindow(93, 40, 129, 20);
	GetDlgItem(IDC_BTN_HOMEPAGE)->ShowWindow(SW_SHOW);
	m_bmpBtnHomepage.LoadBitmaps(IDB_HOMEPAGE_UP, IDB_HOMEPAGE_DOWN, IDB_HOMEPAGE_FOCUS);
#endif


	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 
	m_ctlBtnRememberID.ShowWindow(SW_SHOW);
	m_ctlBtnRememberID.MoveWindow(EXE2_BG_REMEMBERID_CHECKBOX_POS_X, EXE2_BG_REMEMBERID_CHECKBOX_POS_Y, EXE2_BG_REMEMBERID_CHECKBOX_WIDTH, EXE2_BG_REMEMBERID_CHECKBOX_HEIGHT);

	if(FALSE == m_szAccountNameReg.IsEmpty())
	{
		m_ctlBtnRememberID.SetCheck(TRUE);
	}

	CDC *pDC = GetDC();
	int nXScreen = EXE2_LAUNCHER_BG_SIZE_X;
	int nYScreen = EXE2_LAUNCHER_BG_SIZE_Y;
	m_BackGround.CreateCompatibleBitmap(pDC, nXScreen, nYScreen);
		
	CDC		memDCBackGround;	
	memDCBackGround.CreateCompatibleDC(pDC);
	CBitmap	*pOldBitmapBackGround = memDCBackGround.SelectObject(&m_BackGround);
	memDCBackGround.PatBlt(0,0,nXScreen,nYScreen,BLACKNESS);						// 배경을 검은색으로 초기화
	
	CDC tmMemDC;
	tmMemDC.CreateCompatibleDC(pDC);


	CBitmap tmBitmap;
	CBitmap *pTmOldBitmap;

	BitmapRgn(IDB_BG_VTC_MASK, RGB( 255,255,255 ),0, 0);

	// 타이틀바 그리기(Title Bar)
	tmBitmap.LoadBitmap(IDB_TITLE);
	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
	memDCBackGround.BitBlt(0, 0, EXE2_BG_TITLE_BAR_SIZE_X, EXE2_BG_TITLE_BAR_SIZE_Y, &tmMemDC, 0, 0, SRCCOPY);
	tmMemDC.SelectObject(pTmOldBitmap);
	tmBitmap.DeleteObject();

	// 배경 화면 그리기(Background)
	
	
	tmBitmap.LoadBitmap(IDB_BG_VTC);
	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
	memDCBackGround.BitBlt(0, EXE2_BG_TITLE_BAR_SIZE_Y,EXE2_BG_BACKGROUND_IMAGE_SIZE_X, EXE2_BG_BACKGROUND_IMAGE_SIZE_Y,&tmMemDC,0,0,SRCPAINT);			
	tmMemDC.SelectObject(pTmOldBitmap);
	tmBitmap.DeleteObject();

	memDCBackGround.SelectObject(pOldBitmapBackGround);

	// 배경 화면 만들기 ----> End
	///////////////////////////////////////////////////////////////////////////////

	DisableControls();
	SetPrivateIP();

	// timeout이 발생하면 프리 서버에 연결
	SetTimer(TIMERID_CONNECT_PRESERVER, 200, NULL);
	SetTimer(TIMERID_SEND_ALIVE_PACKET, 30000, NULL);
	SetTimer(TIMERID_NETWORK_STATE_CHECK, 1000, NULL);		// 2007-06-18 by cmkwon, 네트워크 상태 체크

#ifndef _DEBUG
	DeleteFile("AtumLauncher_dbg.exe");
#endif

	ShowWindow(SW_SHOW);
	this->GetPublicLocalIP(m_szLocalIP);		// 2006-05-07 by cmkwon
	return TRUE;  // return TRUE  unless you set the focus to a control

///////////////////////////////////////////////////////////////////////////////	
// 2007-09-07 by cmkwon, 베트남 런처 인터페이스 수정 - 위와 같이 수정 정리함
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-05-07 by cmkwon, 해상도 정보를 각 나라별 언어로 설정하기 위해
//	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER);
//	if(pComboBox)
//	{
//// 2007-07-24 by cmkwon, 런처에서 800*600 해상도 삭제 - 콤보박스에 추가하지 않는다
////		pComboBox->AddString(STRMSG_WINDOW_DEGREE_800x600_LOW);
////		pComboBox->AddString(STRMSG_WINDOW_DEGREE_800x600_MEDIUM);	
////		pComboBox->AddString(STRMSG_WINDOW_DEGREE_800x600_HIGH);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1024x768_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1024x768_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1024x768_HIGH);
//
//		// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x720_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x720_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x720_HIGH);
//
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x800_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x800_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1280x800_HIGH);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x960_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x960_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x960_HIGH);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x1024_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x1024_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1280x1024_HIGH);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1600x900_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1600x900_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_W1600x900_HIGH);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1600x1200_LOW);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1600x1200_MEDIUM);
//		pComboBox->AddString(STRMSG_WINDOW_DEGREE_1600x1200_HIGH);
//		pComboBox->SetCurSel(max(0, m_nWindowDegreeReg-3));			// 2007-07-24 by cmkwon, 런처에서 800*600 해상도 삭제 - 레지스트리갑은 기존것을 그대로 사용한다.
//	}
//	
//    m_fontServerGroupListBox.CreatePointFont(90, "굴림체");    // "System" Font는 대표적인 Fixed Font임다.
//    GetDlgItem(IDC_LIST)->SetFont(&m_fontServerGroupListBox);
//
//    m_fontNoticeBox.CreateFont(16, 0, 0, 0, SG_BOX_FONT_WEIGHT, 0,
//							  FALSE, FALSE, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
//							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//							  FIXED_PITCH , SG_BOX_FONT_FACENAME);    // "System" Font는 대표적인 Fixed Font임다.
//    GetDlgItem(IDC_NOTICE)->SetFont(&m_fontNoticeBox);
//
//	m_Cur_Percent=0;
//	m_SelectFlag = FALSE;
//
//	
//	m_bitmapBtnCancel.AutoLoad(IDCAN, this);
//	m_bitmapBtnMin.AutoLoad(IDMIN, this);
//#if defined(_GLOBAL_ENG_SERVER)			// 2006-09-07 by cmkwon, 수정(#ifdef _VTC_VIET_SERVER --> #ifndef _GLOBAL_ENG_SERVER)
//	m_KbcGO.SetBmpButtonImage(IDB_GOBTN, RGB(0,0,255));
//#elif defined(_KOREA_SERVER_2)		// 2006-10-02 by cmkwon
//	m_KbcGO.SetBmpButtonImage(IDB_GO_S2, RGB(0,0,255));
//#elif defined(_CHN_S1)				// 2007-03-13 by cmkwon
//	m_KbcGO.SetBmpButtonImage(IDB_GO_CHN, RGB(0,0,255));
//#else
//	m_KbcGO.SetBmpButtonImage(IDB_GO_VTC, RGB(0,0,255));
//#endif
//
//	m_KbcGO.SetToolTipText("Game Start");
//	m_kbcBtnJoin.SetBmpButtonImage(IDB_JOINBTN, RGB(0,0,255));
//	m_kbcBtnJoin.SetToolTipText("Join");
//
//
//	//GetDlgItem(IDC_CHARACTER_NAME)->SetWindowText(g_szAccountName);
////////////////////////////////////////////////////////////////////////////
//// 2006-05-02 by ispark, 웹페이지
//	char* strWebAddress = LAUNCHER_WEB_URL;
//#ifdef _TEST_SERVER
//	strWebAddress		= TESTSERVER_LAUNCHER_WEB_URL;			// 2006-08-04 by cmkwon, 테섭 Launcher 웹페이지
//#endif
////	HWND hWnd = GetDlgItem(IDC_WEB)->m_hWnd;
//
//// 2007-09-07 by cmkwon, 아래와 같이 수정함
////	m_pHost = new Host(m_hWnd,strWebAddress,NULL,NULL,NULL);
////	RECT rt;
////	rt.left = DEF_HOST_L;
////	rt.top = DEF_HOST_T;
////	rt.right = DEF_HOST_R;
////	rt.bottom = DEF_HOST_B;
////	m_pHost->ReSizeRect(rt);
//	RECT rt;
//	rt.left		= DEF_HOST_L;
//	rt.top		= DEF_HOST_T;
//	rt.right	= DEF_HOST_R;
//	rt.bottom	= DEF_HOST_B;
//	m_pHost		= new Host(m_hWnd,strWebAddress,NULL,NULL,NULL, &rt);
//
//
/////////////////////////////////////////////////////////////////////////////////
//// 배경 화면 만들기
//	// 2006-05-02 by ispark, 런처 배경 화면 변경
//
//	CDC *pDC = GetDC();
//// 2004-11-08 by cmkwon
////	int nXScreen = GetSystemMetrics(SM_CXSCREEN);
////	int nYScreen = GetSystemMetrics(SM_CYSCREEN);
//	int nXScreen = DEF_SCREEN_X;
//	int nYScreen = DEF_SCREEN_Y;
//	m_BackGround.CreateCompatibleBitmap(pDC, nXScreen, nYScreen);
//		
//	CDC		memDCBackGround;	
//	memDCBackGround.CreateCompatibleDC(pDC);
//	CBitmap	*pOldBitmapBackGround = memDCBackGround.SelectObject(&m_BackGround);
//	memDCBackGround.PatBlt(0,0,nXScreen,nYScreen,BLACKNESS);						// 배경을 검은색으로 초기화
//	
//	CDC tmMemDC;
//	int nPosX, nPosY;	
//	
//	// Gear 그리기
//	CBitmap tmBitmap;
//	CBitmap *pTmOldBitmap;
////	tmBitmap.LoadBitmap(IDB_GEAR);		
//	tmMemDC.CreateCompatibleDC(pDC);
////	CBitmap *pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = nXScreen - IMAGE_GEAR_X_SIZE;
//	nPosY = nYScreen - (IMAGE_GEAR_Y_SIZE + 50);
////	memDCBackGround.BitBlt(nPosX, nPosY, IMAGE_GEAR_X_SIZE, IMAGE_GEAR_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
////	tmMemDC.SelectObject(pTmOldBitmap);
////	tmBitmap.DeleteObject();
//
//	// ServerList 그리기
////	tmBitmap.LoadBitmap(IDB_SERVERLIST);
////	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = (nXScreen - IMAGE_SERVERLIST_X_SIZE)/2;
//	nPosY = (nYScreen - IMAGE_SERVERLIST_Y_SIZE)/2;
//	int nServerListPosX = nPosX;
//	int nServerListPosY = nPosY;
////	memDCBackGround.BitBlt(nPosX, nPosY, IMAGE_SERVERLIST_X_SIZE, IMAGE_SERVERLIST_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
////	tmMemDC.SelectObject(pTmOldBitmap);
////	tmBitmap.DeleteObject();
//
//	// 타이틀바 그리기
//	tmBitmap.LoadBitmap(IDB_TITLE);
//	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = 0;
//	nPosY = 0;
//	memDCBackGround.BitBlt(nPosX, nPosY, IMAGE_TITLE_X_SIZE, IMAGE_TITLE_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
//	tmMemDC.SelectObject(pTmOldBitmap);
//	tmBitmap.DeleteObject();
//
//
//// 7/13/2006 by dgwoo, NEW UpdateBar 그리기
//#ifndef _GLOBAL_ENG_SERVER			// 2006-09-07 by cmkwon, 수정(#ifdef _VTC_VIET_SERVER --> #ifndef _GLOBAL_ENG_SERVER)
//	tmBitmap.LoadBitmap(IDB_BG_VTC);
//	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = 0;
//	nPosY = IMAGE_TITLE_Y_SIZE;
//	memDCBackGround.BitBlt(nPosX,nPosY,DEF_SCREEN_X,DEF_SCREEN_Y,&tmMemDC,0,0,SRCCOPY);
//	tmMemDC.SelectObject(pTmOldBitmap);
//	tmBitmap.DeleteObject();
//#else
//	tmBitmap.LoadBitmap(IDB_CENTER);
//	pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
//	nPosX = 0;
//	nPosY = nYScreen - IMAGE_CENTERBAR_Y_SIZE;
//	memDCBackGround.BitBlt(nPosX, nPosY, IMAGE_CENTERBAR_X_SIZE, IMAGE_CENTERBAR_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
//	tmMemDC.SelectObject(pTmOldBitmap);
//	tmBitmap.DeleteObject();
//#endif
//
//
////	if(nXScreen > IMAGE_UPDATEBAR_X_SIZE * 2)
////	{
////		// UpdateCenterBar 그리기
////		tmBitmap.LoadBitmap(IDB_BARCENTER);
////		pTmOldBitmap = tmMemDC.SelectObject(&tmBitmap);
////		nPosX = (nXScreen - IMAGE_UPDATEBAR_X_SIZE)/2;
////		nPosY = nYScreen - IMAGE_UPDATEBAR_Y_SIZE;
////		memDCBackGround.BitBlt(nPosX, nPosY, IMAGE_UPDATEBAR_X_SIZE, IMAGE_UPDATEBAR_Y_SIZE, &tmMemDC, 0, 0, SRCCOPY);
////		tmMemDC.SelectObject(pTmOldBitmap);
////		tmBitmap.DeleteObject();
////	}
//
//	memDCBackGround.SelectObject(pOldBitmapBackGround);
////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// 컨트롤 위치 설정하기
//	
//	// 해상도 설정 ComboBox
//// 7/12/2006 by dgwoo
//	GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->MoveWindow(DEF_DEGREE_L, DEF_DEGREE_T, 
//															 DEF_DEGREE_R, DEF_DEGREE_B);
//
//	// CharacterName Static
//	nPosX = nServerListPosX + 80;
//	nPosY = nServerListPosY + 5;
//	GetDlgItem(IDC_CHARACTER_NAME)->MoveWindow(nPosX, nPosY, 124, 14);
//	GetDlgItem(IDC_CHARACTER_NAME)->SetWindowText(g_szMGameID);
//#if defined(_KOREA_SERVER_2)		// 2006-10-02 by cmkwon
//	#if defined(_DEBUG)
//		GetDlgItem(IDC_EDIT_ACCOUNT)->MoveWindow(DEF_ACCOUNT_L_S2, DEF_ACCOUNT_T_S2, DEF_ACCOUNT_R_S2, DEF_ACCOUNT_B_S2);
//		GetDlgItem(IDC_EDIT_ACCOUNT)->ShowWindow(SW_SHOW);
//		GetDlgItem(IDC_EDIT_PASSWORD)->MoveWindow(DEF_PASSWORD_L_S2, DEF_PASSWORD_T_S2, DEF_ACCOUNT_R, DEF_ACCOUNT_B);
//		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(SW_SHOW);
//	#endif
//// 2007-03-13 by cmkwon, 나머지는 모두 아뒤/비번 입력 컨트롤 보여주기
////// 2007-03-06 by cmkwon, 추가함(|| defined(_MASANG15_SERVER))
////#elif defined(_DEBUG) || defined(_MASANG15_SERVER) || defined(_MASANG51_SERVER) || defined(_GLOBAL_ENG_SERVER) || defined(_VTC_VIET_SERVER) || defined(_KOREA_SERVER_2) || defined(_CHN_S1)
//#else
//	GetDlgItem(IDC_EDIT_ACCOUNT)->MoveWindow(DEF_ACCOUNT_L, DEF_ACCOUNT_T, DEF_ACCOUNT_R, DEF_ACCOUNT_B);
//	GetDlgItem(IDC_EDIT_ACCOUNT)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_EDIT_PASSWORD)->MoveWindow(DEF_PASSWORD_L, DEF_PASSWORD_T, DEF_ACCOUNT_R, DEF_ACCOUNT_B);
//	GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(SW_SHOW);
//#endif
//
//
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 서버리스트 List
//	GetDlgItem(IDC_LIST)->MoveWindow(DEF_SERVERLIST_L, DEF_SERVERLIST_T, DEF_SERVERLIST_R,DEF_SERVERLIST_B);
//	GetDlgItem(IDC_COMBO_SERVER_LIST)->MoveWindow(DEF_SERVERLIST_L, DEF_SERVERLIST_T, DEF_SERVERLIST_R, DEF_SERVERLIST_B);
//#ifndef _GLOBAL_ENG_SERVER			// 2006-09-07 by cmkwon, 수정(#ifdef _VTC_VIET_SERVER --> #ifndef _GLOBAL_ENG_SERVER)
//	GetDlgItem(IDC_LIST)->ShowWindow(SW_SHOW);		
//	GetDlgItem(IDC_COMBO_SERVER_LIST)->ShowWindow(SW_HIDE);
//	m_ctrlServerList.SetItemHeight(25);
//	m_ServerList = (CListBoxEBX *)GetDlgItem(IDC_LIST);
//#else
//	GetDlgItem(IDC_LIST)->ShowWindow(SW_HIDE);		
//	GetDlgItem(IDC_COMBO_SERVER_LIST)->ShowWindow(SW_SHOW);
//	m_ServerList = (CComboBoxEBX *)GetDlgItem(IDC_COMBO_SERVER_LIST);
//#endif
//
//	
//	// 공지사항 EditBox
//	nPosX = nServerListPosX + 245;
//	nPosY = nServerListPosY + 49;
//	GetDlgItem(IDC_NOTICE)->MoveWindow(nPosX, nPosY, 382, 166);
//	GetDlgItem(IDC_NOTICE)->ShowWindow(SW_HIDE);
//
//	// Min Button
////	nPosX = nServerListPosX + 612;
////	nPosY = nServerListPosY;
////	GetDlgItem(IDMIN)->MoveWindow(nPosX, nPosY, 16, 20);
//	nPosX = 752;
//	nPosY = 0;
//	GetDlgItem(IDMIN)->MoveWindow(nPosX, nPosY, 16, 20);
//
//	// Cancel Button
////	nPosX = nServerListPosX + 628;
////	nPosY = nServerListPosY;
////	GetDlgItem(IDCAN)->MoveWindow(nPosX, nPosY, 16, 20);
//	nPosX = 772;
//	nPosY = 0;
//	GetDlgItem(IDCAN)->MoveWindow(nPosX, nPosY, 16, 20);
//
//	// GO Button
//// 2006-05-02 by ispark
////	nPosX = nServerListPosX + 54;
////	nPosY = nServerListPosY + 216;
//#if defined(_KOREA_SERVER_2)
//	GetDlgItem(IDGO)->MoveWindow(DEF_GO_L_S2, DEF_GO_T_S2, IMAGE_GO_BUTTON_X_SIZE_S2, IMAGE_GO_BUTTON_Y_SIZE_S2);
//#elif defined(_CHN_S1)				// 2007-03-13 by cmkwon
//	GetDlgItem(IDGO)->MoveWindow(DEF_GO_L_CHN, DEF_GO_T_CHN, IMAGE_GO_BUTTON_X_SIZE_CHN, IMAGE_GO_BUTTON_Y_SIZE_CHN);
//#else
//	GetDlgItem(IDGO)->MoveWindow(DEF_GO_L, DEF_GO_T, IMAGE_GO_BUTTON_X_SIZE, IMAGE_GO_BUTTON_Y_SIZE);
//#endif
//	// 2005-11-10 by cmkwon, Join Button
//	nPosX = nServerListPosX + 265;
//	nPosY = nServerListPosY + 226;
//	GetDlgItem(IDJOIN)->MoveWindow(nPosX, nPosY, IMAGE_JOIN_BUTTON_X_SIZE, IMAGE_JOIN_BUTTON_Y_SIZE);
//
//	// Update Progress
//// 7/12/2006 by dgwoo
//	nPosX = 26;	
//	nPosY = nYScreen - 23;
//	m_progressCtrl.MoveWindow(nPosX, nPosY, nXScreen - (nPosX*2), 7);
//
//	// Download File Version Num
//// 7/12/2006 by dgwoo
//	GetDlgItem(IDC_DOWNLOAD_FILENUM)->MoveWindow(DEF_FILEVERSION_L, DEF_FILEVERSION_T, DEF_FILEVERSION_R, DEF_FILEVERSION_B);
//
//	// Download File Info
//// 7/12/2006 by dgwoo
//	GetDlgItem(IDC_FILE_INFO)->MoveWindow(DEF_FILEINFO_L, DEF_FILEVERSION_T, DEF_FILEINFO_R, DEF_FILEVERSION_B);
////
/////////////////////////////////////////////////////////////////////////////////
//
//
//	m_progressCtrl.SetGradientColors(RGB(255, 0, 0), RGB(0, 0, 255));
//
//	// Initialize winsock 2.0
//	CWinSocket::SocketInit();
//
//	// set default values & settings
//	m_bCancelFlag = FALSE;
//	m_bProcessingVersionUpdate = FALSE;
//	m_bShowPreServerIPDlg = TRUE;
////	m_FtpDownload.SetMainDlg(this);
//	DisableControls();
//	SetPrivateIP();
//
//// 2006-05-03 by cmkwon, 공지사항은 런처의 웹화면에서 처리한다.
//// 	// 일단 Notice 로딩 - 없음 말고... 나중에 notice를 다시 다운 받으니 상관없음...
//// 	if (!ReadNoticeFile())
//// 	{
//// 		//AtumMessageBox("Notice File Error!");
//// 		//EndDialog(-1);
//// 		//return FALSE;
//// 	}
//
//	// timeout이 발생하면 프리 서버에 연결
//	SetTimer(TIMERID_CONNECT_PRESERVER, 200, NULL);
//	SetTimer(TIMERID_SEND_ALIVE_PACKET, 30000, NULL);
//	SetTimer(TIMERID_NETWORK_STATE_CHECK, 1000, NULL);		// 2007-06-18 by cmkwon, 네트워크 상태 체크
//
//#ifndef _DEBUG
//	DeleteFile("AtumLauncher_dbg.exe");
//#endif
//
//	int nXScreen1 = GetSystemMetrics(SM_CXSCREEN);
//	int nYScreen1 = GetSystemMetrics(SM_CYSCREEN);
//
//	CRect rect((nXScreen1-800)/2, (nYScreen1-600)/2, (nXScreen1-800)/2+800, (nYScreen1-600)/2+DEF_SCREEN_Y);
//	this->MoveWindow(&rect);
//	ShowWindow(SW_SHOW);
//
//	this->GetPublicLocalIP(m_szLocalIP);		// 2006-05-07 by cmkwon
//	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CAtumLauncherDlg::ConnectPreServer()
{
// Pre Server IP 창 띄우기
//	char preServerIP[SIZE_MAX_IPADDRESS];

// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - 버퍼 크키가 더 필요 하다.
//	char preServerIP[128];
	char preServerIP[1024];		// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - 
#ifdef _ATUM_DEVELOP
	char ExecutableBinary[256];
	char ExecutePath[256];
	CPreServerIPDlg	dlg(m_szPreServerIPReg, m_szExecutableBinaryReg, m_szExecutePathReg,
						m_szPreServerIPHistoryReg, m_szExecuteBinHistoryReg, m_szExecutePathHistoryReg);
	dlg.m_bWindowMode = ((m_nWindowModeReg==GAME_MODE_WINDOW)?TRUE:FALSE);

// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 처리 필요하지 않음
//	// window mode이면 해상도 combo box를 disable시킴
//	if (m_nWindowModeReg == GAME_MODE_WINDOW)
//	{
//		GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(FALSE);
//	}
//	else
//	{
//		GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow();
//	}

	if (m_bShowPreServerIPDlg)
	{
		if (dlg.DoModal() != IDOK)
		{
			return FALSE;
		}
	}

	STRNCPY_MEMSET(preServerIP, dlg.m_preserver_ip, 128);
	m_szPreServerIPReg = dlg.m_preserver_ip;
	STRNCPY_MEMSET(ExecutableBinary, dlg.m_executable_bin, 256);
	m_szExecutableBinaryReg = dlg.m_executable_bin;
	STRNCPY_MEMSET(ExecutePath, dlg.m_execute_path, 256);
	m_szExecutePathReg = dlg.m_execute_path;
	m_szPreServerIPHistoryReg = dlg.m_szPreServerIPHistory;
	m_szExecuteBinHistoryReg = dlg.m_szExecuteBinHistory;
	m_szExecutePathHistoryReg = dlg.m_szExecutePathHistory;
	m_nWindowModeReg = ((dlg.m_bWindowMode==TRUE)?GAME_MODE_WINDOW:GAME_MODE_FULLSCREEN);
	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 추가한것
	if(GAME_MODE_WINDOW == m_nWindowModeReg)
	{
		this->m_ctlbWindowMode		= TRUE;




		this->CheckDlgButton(IDC_CHECK_WINDOWS_MODE, 1);
	}
	((CAtumLauncherApp*)AfxGetApp())->WriteProfile();

	m_bUpdateClientFile = dlg.m_bUpdateClientFile;

#else
	// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - domain 으로 설정되면 길이가 길어질수 있다.
	//STRNCPY_MEMSET(preServerIP, m_szPreServerIPReg, SIZE_MAX_IPADDRESS);
	STRNCPY_MEMSET(preServerIP, m_szPreServerIPReg, 1024);		// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - 
//	m_szPreServerIPReg = m_szPreServerIPReg;	// BULLSHIT!
#endif


	// Make socket instance & connect
	m_pUpdateWinsocket = new CUpdateWinSocket(GetSafeHwnd());
	// 2004-11-19 by cmkwon
	//if (!m_pUpdateWinsocket->Connect(GGetIPByName(preServerIP, preServerIP), PRE_SERVER_PORT))

	int nPreServerPort = PRE_SERVER_PORT;		// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 포트도 변결 할 수 있게 수정


// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 지정된 포트로 연결 시도
//	if (!m_pUpdateWinsocket->Connect(preServerIP, PRE_SERVER_PORT))
	if (!m_pUpdateWinsocket->Connect(preServerIP, nPreServerPort))
	{
 		int err = GetLastError();
//		AtumMessageBox("서버가 실행되어 있지 않거나 점검 중입니다.\n\n테스트 시간은 주말 24시간,\n\n평일 오전10시부터 오후24시까지입니다.");
 		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0004);
		SAFE_DELETE(m_pUpdateWinsocket);
		EndDialog(-1);
		return FALSE;
	}
	m_PreSD = m_pUpdateWinsocket->GetSocketDescriptor();

	return TRUE;
}

void CAtumLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(SC_MAXIMIZE == nID)
	{// 최대화를 막는다.

		return;
	}
	
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

void CAtumLauncherDlg::OnPaint()
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
		CPaintDC PaintDC (this);
		CDC		dcMem;
		BITMAP	stBitmap;

		dcMem.CreateCompatibleDC(&PaintDC);
		CBitmap *OldBitmap = dcMem.SelectObject(&m_BackGround);

		m_BackGround.GetObject(sizeof(BITMAP), &stBitmap);
		PaintDC.BitBlt(0, 0, stBitmap.bmWidth, stBitmap.bmHeight, &dcMem, 0, 0, SRCCOPY);

		

			

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAtumLauncherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


struct sort_MEX_SERVER_GROUP_INFO_FOR_LAUNCHER_By_Crowdedness
{
	bool operator()(MEX_SERVER_GROUP_INFO_FOR_LAUNCHER op1, MEX_SERVER_GROUP_INFO_FOR_LAUNCHER op2)
	{
		return op1.Crowdedness < op2.Crowdedness;				// 오른차순 정렬
	}
};


LONG CAtumLauncherDlg::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_pUpdateWinsocket)
	{
		return 0L;
	}

	switch(LOWORD(wParam))
	{
	case CUpdateWinSocket::WS_ERROR:
		{
		}
		break;
	case CUpdateWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				m_pUpdateWinsocket->WriteMessageType(T_PC_CONNECT_GET_SERVER_GROUP_LIST);

				INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_SINGLE_FILE_VERSION_CHECK, T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK, msgSFVer, msgSFVerBuf);
				// delete file list version
				msgSFVer->DeleteFileListVersion[0] = m_CurrentDelFileListVersion.GetVersion()[0];
				msgSFVer->DeleteFileListVersion[1] = m_CurrentDelFileListVersion.GetVersion()[1];
				msgSFVer->DeleteFileListVersion[2] = m_CurrentDelFileListVersion.GetVersion()[2];
				msgSFVer->DeleteFileListVersion[3] = m_CurrentDelFileListVersion.GetVersion()[3];
				// notice version
				msgSFVer->NoticeVersion[0] = m_CurrentNoticeVersion.GetVersion()[0];
				msgSFVer->NoticeVersion[1] = m_CurrentNoticeVersion.GetVersion()[1];
				msgSFVer->NoticeVersion[2] = m_CurrentNoticeVersion.GetVersion()[2];
				msgSFVer->NoticeVersion[3] = m_CurrentNoticeVersion.GetVersion()[3];

				m_pUpdateWinsocket->Write((char*)msgSFVerBuf, MSG_SIZE(MSG_PC_CONNECT_SINGLE_FILE_VERSION_CHECK));
			}
			else
			{
//				AtumMessageBox("서버가 실행되어 있지 않거나 점검 중입니다.\n\n테스트 시간은 주말 24시간,\n\n평일 오전10시부터 오후24시까지입니다.");
				AtumMessageBox(STRERR_S_ATUMLAUNCHER_0005);
				EndDialog(-1);
			}
		}
		break;
	case CUpdateWinSocket::WS_CLOSED:
		{
			SAFE_DELETE(m_pUpdateWinsocket);
#ifdef _DEBUG
			AtumMessageBox(STRERR_S_ATUMLAUNCHER_0006);
#endif

			OnCancel();
		}
		break;
	case CUpdateWinSocket::WS_RECEIVED:
		{
			char * pPacket = NULL;
			int len,nType;
			m_pUpdateWinsocket->Read(&pPacket, len);

			if(pPacket)
			{
				nType = 0;
				memcpy(&nType, pPacket, SIZE_FIELD_TYPE_HEADER);

				switch(nType)
				{
				case T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK:
					{
// 2006-05-03 by cmkwon, 공지사항은 런처의 웹화면에서 처리한다.
// 						// Notice 로딩
// 						if (!ReadNoticeFile())
// 						{
// 							AtumMessageBox(STRERR_S_ATUMLAUNCHER_0007);
// 							EndDialog(-1);
// 							return FALSE;
// 						}
//						DbgOut("	2007-07-13 by cmkwon, T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK\r\n");	// 2007-07-13 by cmkwon, 테스트 

						/////////////////////////////////////////////
						// Send Client Version
						INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_VERSION, T_PC_CONNECT_VERSION, msgVersion, msgVersionBuff);
						msgVersion->ClientVersion[0] = m_CurrentVersion.GetVersion()[0];
						msgVersion->ClientVersion[1] = m_CurrentVersion.GetVersion()[1];
						msgVersion->ClientVersion[2] = m_CurrentVersion.GetVersion()[2];
						msgVersion->ClientVersion[3] = m_CurrentVersion.GetVersion()[3];
						m_pUpdateWinsocket->Write(msgVersionBuff, MSG_SIZE(MSG_PC_CONNECT_VERSION));

						// 일단 찍어두기
						SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0000);
					}
					break;
				case T_PC_CONNECT_SINGLE_FILE_UPDATE_INFO:
					{
						MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pMsgUpdate
							= (MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						// 릴리즈용
						VersionInfo TmpDelVerion(pMsgUpdate->NewDeleteFileListVersion);
						if (TmpDelVerion != m_CurrentDelFileListVersion)
						{
							// delete file 처리
							if (m_bUpdateClientFile)
							{
								///////////////////////////////////////////////////////////////////////////////
								// 2007-01-08 by cmkwon, Http Update 처리 추가
								switch(pMsgUpdate->nAutoUpdateServerType)
								{
								case AU_SERVER_TYPE_HTTP:
									{
										ProcessDeleteFileListByHttp(pMsgUpdate);
									}
									break;
								default:
									{
										if (!ProcessDeleteFileList(pMsgUpdate))
										{
											// 방화벽 등의 이유로 FTP에 접근하지 못하는 유저가 존재할 수 있으므로,
											// 실패해도 그냥 넘긴다. 20041030, kelovon
											//OnCancel();
										}
									}
								}
							}
							m_CurrentDelFileListVersion = TmpDelVerion;
						}
// 2006-05-03 by cmkwon, 공지사항은 런처의 웹화면에서 처리한다.
// 						VersionInfo TmpNoticeVerion(pMsgUpdate->NewNoticeVersion);
// 						if (TmpNoticeVerion != m_CurrentNoticeVersion)
// 						{
// 							// notice.txt를 다운로드받음
// 							if (!ProcessNoticeFile(pMsgUpdate))
// 							{
// 								// 방화벽 등의 이유로 FTP에 접근하지 못하는 유저가 존재할 수 있으므로,
// 								// 실패해도 그냥 넘긴다. 20041030, kelovon
// 								//OnCancel();
// 							}
// 							m_CurrentNoticeVersion = TmpNoticeVerion;
// 						}
						
						// 레지스트리에 쓰기
						((CAtumLauncherApp*)AfxGetApp())->WriteProfile();

// 2006-05-03 by cmkwon, 공지사항은 런처의 웹화면에서 처리한다.
// 						// Notice 로딩
// 						if (!ReadNoticeFile())
// 						{
// 							AtumMessageBox(STRERR_S_ATUMLAUNCHER_0007);
// 							EndDialog(-1);
// 							return FALSE;
// 						}

//						DbgOut("	2007-07-13 by cmkwon, T_PC_CONNECT_SINGLE_FILE_UPDATE_INFO\r\n");	// 2007-07-13 by cmkwon, 테스트 
						/////////////////////////////////////////////
						// Send Client Version
						INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_VERSION, T_PC_CONNECT_VERSION, msgVersion, msgVersionBuff);
						msgVersion->ClientVersion[0] = m_CurrentVersion.GetVersion()[0];
						msgVersion->ClientVersion[1] = m_CurrentVersion.GetVersion()[1];
						msgVersion->ClientVersion[2] = m_CurrentVersion.GetVersion()[2];
						msgVersion->ClientVersion[3] = m_CurrentVersion.GetVersion()[3];
						m_pUpdateWinsocket->Write(msgVersionBuff, MSG_SIZE(MSG_PC_CONNECT_VERSION));
						// 일단 찍어두기
						SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0000);
					}
					break;
				case T_PC_CONNECT_VERSION_OK:
					{
//						DbgOut("	2007-07-13 by cmkwon, T_PC_CONNECT_VERSION_OK\r\n");	// 2007-07-13 by cmkwon, 테스트 

// start 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
						SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0014);
						m_pVTCGuard		= new CVTCGuardManager;

						if( NULL != m_pVTCGuard )
						{
							m_pVTCGuard->Update(TRUE, this->GetSafeHwnd());
						}
#else
						m_bProcessingVersionUpdate = TRUE;
						EnableControls();
#endif	// SERVICE_TYPE_VIETNAMESE_SERVER_1
// end 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

						NTStartNetworkCheck();		// 2007-06-18 by cmkwon, 네트워크 상태 체크

#ifndef _DEBUG
						DeleteFile("AtumLauncher_dbg.exe");
#endif
					}
					break;
				case T_PC_CONNECT_REINSTALL_CLIENT:
					{
						m_bProcessingVersionUpdate = TRUE;
						MSG_PC_CONNECT_REINSTALL_CLIENT *pReinstall
							= (MSG_PC_CONNECT_REINSTALL_CLIENT*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						VersionInfo currentVersion;
						currentVersion.SetVersion(pReinstall->LatestVersion[0]
													, pReinstall->LatestVersion[1]
													, pReinstall->LatestVersion[2]
													, pReinstall->LatestVersion[3]);

#ifdef _ATUM_DEVELOP
						if (!m_bUpdateClientFile)
						{

							/////////////////////////////////////////////
							// Send Client Version
							INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_VERSION, T_PC_CONNECT_VERSION, msgVersion, msgVersionBuff);
							msgVersion->ClientVersion[0] = pReinstall->LatestVersion[0];
							msgVersion->ClientVersion[1] = pReinstall->LatestVersion[1];
							msgVersion->ClientVersion[2] = pReinstall->LatestVersion[2];
							msgVersion->ClientVersion[3] = pReinstall->LatestVersion[3];
							m_pUpdateWinsocket->Write(msgVersionBuff, MSG_SIZE(MSG_PC_CONNECT_VERSION));
							EnableControls();
							NTStartNetworkCheck();		// 2007-06-18 by cmkwon, 네트워크 상태 체크
							SetProgressGroupText("update completed");
							break;
						}
#endif
						CString tmpMsg;
						tmpMsg.Format(STRMSG_S_050930, STRMSG_S_GAMEHOMEPAGE_DOMAIN);
						tmpMsg += currentVersion.GetVersionString();
						AtumMessageBox(tmpMsg);
// 2005-09-30 by cmkwon
//						string tmpMsg = STRERR_S_ATUMLAUNCHER_0008;
//						tmpMsg += currentVersion.GetVersionString();
//						AtumMessageBox(tmpMsg.c_str());
						OnCancel();
					}
					break;
				case T_PC_CONNECT_UPDATE_INFO:
					{
						MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo
							= (MSG_PC_CONNECT_UPDATE_INFO*)(pPacket + SIZE_FIELD_TYPE_HEADER);

#ifdef _ATUM_DEVELOP
						if (!m_bUpdateClientFile)
						{
							/////////////////////////////////////////////
							// Send Client Version
							INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_VERSION, T_PC_CONNECT_VERSION, msgVersion, msgVersionBuff);
							msgVersion->ClientVersion[0] = pMsgUpdateInfo->UpdateVersion[0];
							msgVersion->ClientVersion[1] = pMsgUpdateInfo->UpdateVersion[1];
							msgVersion->ClientVersion[2] = pMsgUpdateInfo->UpdateVersion[2];
							msgVersion->ClientVersion[3] = pMsgUpdateInfo->UpdateVersion[3];
							m_pUpdateWinsocket->Write(msgVersionBuff, MSG_SIZE(MSG_PC_CONNECT_VERSION));
							EnableControls();
							NTStartNetworkCheck();		// 2007-06-18 by cmkwon, 네트워크 상태 체크
							SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0000);
							break;
						}
#endif

						// download update file
						switch(pMsgUpdateInfo->nAutoUpdateServerType)
						{
						case AU_SERVER_TYPE_HTTP:
							{
								if(FALSE == DownloadUpdateFileByHttp(pMsgUpdateInfo))
								{
									AtumMessageBox(STRERR_S_ATUMLAUNCHER_0009);
									ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
									exit(0);
								}
							}
							break;
						default:
							{
								if(FALSE == DownloadUpdateFile(pMsgUpdateInfo))
								{
									AtumMessageBox(STRERR_S_ATUMLAUNCHER_0009);
									ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
									exit(0);
								}
							}
						}
						m_msg_PC_CONNECT_UPDATE_INFO = *pMsgUpdateInfo;

// 2004-07-06, cmkwon OnUpdateFileDownloadOK()함수로 이동						
//						// extract update file
//						ExtractUpdateFile(pMsgUpdateInfo);
//
//						// update completed
//						VersionInfo OldVersion(pMsgUpdateInfo->OldVersion);
//						VersionInfo UpdateVersion(pMsgUpdateInfo->UpdateVersion);
//						char strBuffer[256];
//						sprintf(strBuffer, "update completed(%s -> %s)", OldVersion.GetVersionString(),
//							UpdateVersion.GetVersionString());
//						SetProgressGroupText(strBuffer);
//
//						// set current cliet version
//						m_CurrentVersion = UpdateVersion;
//
//						// On DownLoadGamefilesDone
//						OnDownLoadGamefilesDone(NULL, NULL);

					}
					break;
				case T_PC_CONNECT_LOGIN_OK:
					{
						MSG_PC_CONNECT_LOGIN_OK *pConnOK;
						pConnOK = (MSG_PC_CONNECT_LOGIN_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);


						char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];
						BYTE byPass[SIZE_MAX_PASSWORD_MD5];
						MD5 MD5_instance;
						
						char szTmPassword[1024];			// 2006-05-22 by cmkwon, 비번에 추가 스트링 적용
						MEMSET_ZERO(szTmPassword, 1024);
						wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, m_szPassword);
						MD5_instance.MD5Encode(szTmPassword, byPass);
						MD5_instance.MD5Binary2String(byPass, szEncodedString);

						// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 2단계 계정을 설정한다.
						m_szAccountName	= pConnOK->AccountName;

						char cmdLine[1024];		MEMSET_ZERO(cmdLine, 1024);
						//_chdir(".\\down");
						CString szTmpWindowDegree;
						// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 수정함
						int nCX, nCY, nDegree;
						if(FALSE == this->FindWindowResolutionByWindowDegree(&nCX, &nCY, &nDegree, (LPSTR)(LPCSTR)m_csWindowsResolutionReg))
						{
							nCX		= 1024;		nCY		= 768;		nDegree		= 2;	// 2007-12-28 by cmkwon, 기본값
						}
						szTmpWindowDegree.Format("%d %d %d", nCX, nCY, nDegree);

						DBGOUT(" Resolution ==> %s\r\n", szTmpWindowDegree);

						///////////////////////////////////////////////////////////////////////////////
						// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
						char szAppPath[1024];		MEMSET_ZERO(szAppPath, 1024);
						char szCmdParam[1024];		MEMSET_ZERO(szCmdParam, 1024);
						if (IsWindowsVersionOrGreater(6, 0, 0))
						{
							STRNCPY_MEMSET(szAppPath, CLIENT_EXEUTE_FILE_NAME, 1024);
						}
						else
						{
							STRNCPY_MEMSET(szAppPath, CLIENT_EXEUTE_FILE_NAME_XP, 1024);
						}
						

#ifndef _ATUM_DEVELOP
						// Release용
						CString szAccountNameLow = g_szMGameID;
						szAccountNameLow.MakeLower();
// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
// 						sprintf(cmdLine, "%s %s %d %s %d %s %s %d %s INET %s %s %s %s"
// 							, CLIENT_EXEUTE_FILE_NAME
// 							, pConnOK->FieldServerIP, pConnOK->FieldServerPort
// 							, pConnOK->IMServerIP, pConnOK->IMServerPort
// 							, szAccountNameLow, szEncodedString
// 							, m_nWindowModeReg, szTmpWindowDegree,
// 							(LPCSTR)g_szMGameSeed, (LPCSTR)g_szMGameID, (LPCSTR)g_szMGameEncryptedID, (LPCSTR)g_szMGameEncryptedPWD);
						///////////////////////////////////////////////////////////////////////////////
						// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
						sprintf(szCmdParam, "%s %d %s %d %s %s %d %s INET %s %s %s %s %d"		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
							, pConnOK->FieldServerIP, pConnOK->FieldServerPort
							, pConnOK->IMServerIP, pConnOK->IMServerPort
							, szAccountNameLow, szEncodedString
							, m_nWindowModeReg, szTmpWindowDegree,
							(LPCSTR)g_szMGameSeed, (LPCSTR)g_szMGameID, (LPCSTR)g_szMGameEncryptedID, (LPCSTR)g_szMGameEncryptedPWD, pConnOK->OpeningMoviePlay);		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
						sprintf(cmdLine, "%s %s", szAppPath, szCmdParam);
 						sprintf(szCmdParam, "%s %d %s %d %s %s %d %s %d", pConnOK->FieldServerIP, pConnOK->FieldServerPort, pConnOK->IMServerIP, pConnOK->IMServerPort, m_szAccountName, szEncodedString, m_nWindowModeReg, szTmpWindowDegree, pConnOK->OpeningMoviePlay);		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
						sprintf(cmdLine, "%s %s", szAppPath, szCmdParam);
#else
						// 개발용
// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
// 						sprintf(cmdLine, "%s %s %d %s %d %s %s %d %s DEVELOP",
// 							m_szExecutableBinaryReg,
// 							pConnOK->FieldServerIP, pConnOK->FieldServerPort,
// 							pConnOK->IMServerIP, pConnOK->IMServerPort,
// 							m_szAccountName + m_szCrocessSuffix, szEncodedString,
// 							m_nWindowModeReg, szTmpWindowDegree);
						///////////////////////////////////////////////////////////////////////////////
						// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
						sprintf(szCmdParam, "%s %d %s %d %s %s %d %s %d DEVELOP", pConnOK->FieldServerIP, pConnOK->FieldServerPort, pConnOK->IMServerIP, pConnOK->IMServerPort, m_szAccountName + m_szCrocessSuffix, szEncodedString, m_nWindowModeReg, szTmpWindowDegree, pConnOK->OpeningMoviePlay);		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
						sprintf(cmdLine, "%s %s", szAppPath, szCmdParam);

#endif// end_#ifndef _ATUM_DEVELOP


#ifdef _ATUM_DEVELOP
						if (m_szExecutableBinaryReg.IsEmpty())
						{
							char buf[128];
							sprintf(buf, "%s %d %s %d %s %s %d %s %d DEVELOP",		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
												pConnOK->FieldServerIP, pConnOK->FieldServerPort,
												pConnOK->IMServerIP, pConnOK->IMServerPort,
												m_szAccountName, szEncodedString, m_nWindowModeReg, szTmpWindowDegree, pConnOK->OpeningMoviePlay);		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
							CLoginSuccessDlg ldlg(buf);
							if (ldlg.DoModal() != IDOK)
							{
								return FALSE;
							}
							m_bProcessingVersionUpdate = TRUE;
							OnCancel();
							break;
						}
#endif
						if (m_szCrocessSuffix == "")
						{
							// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
							//ExecGame(cmdLine);
							ExecGame(cmdLine, szAppPath, szCmdParam);
						}
						else
						{
							// 크로세스를 통해 실행
							ExecGameCrocess(cmdLine);
						}

						OnCancel();
					}
					break;
				case T_PC_CONNECT_GET_SERVER_GROUP_LIST_OK:
					{
						MSG_PC_CONNECT_GET_SERVER_GROUP_LIST_OK *pServerListOK
							= (MSG_PC_CONNECT_GET_SERVER_GROUP_LIST_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						if (pServerListOK->NumOfServerGroup <= 0)
						{
							AtumMessageBox(STRERR_S_ATUMLAUNCHER_0010);
							OnCancel();
							return 0;
						}

						MEX_SERVER_GROUP_INFO_FOR_LAUNCHER *pArrMexServerGroup
							= ((MEX_SERVER_GROUP_INFO_FOR_LAUNCHER*)(pPacket + MSG_SIZE(MSG_PC_CONNECT_GET_SERVER_GROUP_LIST_OK)));

// 2005-01-01 by cmkwon, 서버군 채널을 추가하면서 sorting을 제거함						
//						///////////////////////////////////////////////////////////////////////////////
//						// 서버군이 2개이상이면 혼잡도 순으로 정렬 한다
//						if(pServerListOK->NumOfServerGroup >= 2)
//						{							
//							sort(&pArrMexServerGroup[0], &pArrMexServerGroup[pServerListOK->NumOfServerGroup]
//								, sort_MEX_SERVER_GROUP_INFO_FOR_LAUNCHER_By_Crowdedness());
//						}

						BOOL	bSelectedServerGroupName = FALSE;
						int		nIndexMinCrowdednessServerGroup = -1;
						int		nMinCrowdedness = 100;
						int		nCrowdedIndex = -1;
						///////////////////////////////////////////////////////////////////////////////
						// 서버군을 리스트에 추가한다
						for (int i = 0; i < pServerListOK->NumOfServerGroup; i++)
						{
							MEX_SERVER_GROUP_INFO_FOR_LAUNCHER *pMexServerGroup
								= &pArrMexServerGroup[i];

							///////////////////////////////////////////////////////////////////////////////
							// 혼잡도 정보를 ServerGroupName에 추가한다
							CString strTemp;
							COLORREF	fontColor;

							///////////////////////////////////////////////////////////////////////////////
							// 혼잡도 정보가 추가된 ServerGroupName을 List에 추가한다
							// 2006-05-02 by ispark, 콤보로 변경
							if(0 >= pMexServerGroup->Crowdedness)
							{
								strTemp.Format(STRERR_S_ATUMLAUNCHER_0011, pMexServerGroup->ServerGroupName, STRING_SERVER_GROUP_NAME_DELIMIT);
								fontColor = RGB(180, 180, 180);
								nCrowdedIndex = -1; 
							}
							else
							{
								nCrowdedIndex = min(9, (int)(pMexServerGroup->Crowdedness)/10);
								strTemp.Format("%-16s%s", pMexServerGroup->ServerGroupName, STRING_SERVER_GROUP_NAME_DELIMIT);
								fontColor = RGB(255, 255, 255);
							}

							///////////////////////////////////////////////////////////////////////////////
							// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - m_vectServerGroupList 에 추가한다.
							SSERVER_GROUP_FOR_LAUNCHER tmSerGroup;
							tmSerGroup.nIndex	= i;
							STRNCPY_MEMSET(tmSerGroup.szServerGroupName, pMexServerGroup->ServerGroupName, SIZE_MAX_SERVER_NAME);
							m_vectServerGroupList.push_back(tmSerGroup);
							
							m_ServerList->InsertItem(i, strTemp, pMexServerGroup->Crowdedness, DEF_COL, fontColor, nCrowdedIndex);
							///////////////////////////////////////////////////////////////////////////////
							// 이전에 접속한 ServerGroupName에 비교한다
							if(FALSE == m_strServerGroupName.IsEmpty()
								&& 0 == strnicmp((LPSTR)(LPCSTR)m_strServerGroupName, pMexServerGroup->ServerGroupName, SIZE_MAX_SERVER_NAME)
								&& pMexServerGroup->Crowdedness > 0)
							{// Crowdedness가 0이면 서버 점검 상태임

								// 7/13/2006 by dgwoo
								m_ServerList->SetCurSel(i);
								m_nOldSel					= i;		// 2007-06-21 by cmkwon, 서버군 선택 버그 수정 - 선택된 index를 설정한다.
								bSelectedServerGroupName = TRUE;
							}

							if(pMexServerGroup->Crowdedness > 0
								&& pMexServerGroup->Crowdedness < nMinCrowdedness)
							{
								nIndexMinCrowdednessServerGroup = i;
								nMinCrowdedness = pMexServerGroup->Crowdedness;
							}
						}
						
						if(FALSE == bSelectedServerGroupName)
						{
							if(nIndexMinCrowdednessServerGroup == -1)
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0012);
								OnCancel();
							}
							// 7/13/2006 by dgwoo
							m_ServerList->SetCurSel(nIndexMinCrowdednessServerGroup);
							m_nOldSel					= nIndexMinCrowdednessServerGroup;		// 2007-06-21 by cmkwon, 서버군 선택 버그 수정 - 선택된 index를 설정한다.
						}						
					}
					break;

				case T_PC_CONNECT_NETWORK_CHECK_OK:		// 2007-06-18 by cmkwon, 네트워크 상태 체크
					{
						MSG_PC_CONNECT_NETWORK_CHECK_OK *pRMsg = (MSG_PC_CONNECT_NETWORK_CHECK_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						this->NTOnReceivedNetworkCheckOK(pRMsg->nCheckCount);
						m_ServerList->UpdateNetworkState(this->NTGetPingAverageTime());		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
						m_ServerList->Invalidate();											// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
					}
					break;

				case T_PC_CONNECT_LOGIN_BLOCKED:
					{
						MSG_PC_CONNECT_LOGIN_BLOCKED *pLoginBlocked
							= (MSG_PC_CONNECT_LOGIN_BLOCKED*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						char szTemp[1024];
						wsprintf(szTemp, STRMSG_S_050506
							, pLoginBlocked->szAccountName, pLoginBlocked->szBlockedReasonForUser
							, pLoginBlocked->atimeStart.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING))
							, pLoginBlocked->atimeEnd.GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
						AtumMessageBox(szTemp);
						OnCancel();
					}
					break;
				case T_ERROR:
					{
						MSG_ERROR *pRecvMsg;
						pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						char buf[1024];
						Err_t error = pRecvMsg->ErrorCode;

						DBGOUT(STRERR_S_ATUMLAUNCHER_0013, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_PRE_SERVER", m_pUpdateWinsocket->m_szPeerIP);

						switch (error)
						{
						case ERR_COMMON_LOGIN_FAILED:
							{
								AtumMessageBox(STRMSG_060824_0000);
								GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
								((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetSel(0, -1);
							}
							break;
						case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0015);
								OnCancel();
							}
							break;
						case ERR_PROTOCOL_EMPTY_ACCOUNTNAME:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0016);
								GetDlgItem(IDC_EDIT_ACCOUNT)->SetFocus();
								((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->SetSel(0, -1);
							}
							break;
						case ERR_PROTOCOL_DUPLICATE_LOGIN:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0017);
								OnCancel();
							}
							break;
						case ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0018);
								//OnCancel();
							}
							break;
						case ERR_PROTOCOL_IM_SERVER_NOT_ALIVE:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0019);
								//OnCancel();
							}
							break;
						case ERR_COMMON_SERVICE_TEMPORARILY_PAUSED:
							{
								static BOOL bMsgBox = FALSE;
								if(FALSE == bMsgBox)
								{
									bMsgBox = TRUE;
									AtumMessageBox(STRERR_S_ATUMLAUNCHER_0020);
									OnCancel();
								}
							}
							break;
						case ERR_PROTOCOL_LIMIT_GROUP_USER_COUNT:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0021);
								OnCancel();
							}
							break;
						case ERR_PROTOCOL_ACCOUNT_BLOCKED:
							{
								char szErrString[512];
								memset(szErrString, 0x00, 512);
								memcpy(szErrString, ((BYTE*)pRecvMsg) + sizeof(MSG_ERROR), pRecvMsg->StringLength);
								sprintf(buf, STRERR_S_ATUMLAUNCHER_0022, szErrString);
								AtumMessageBox(buf);
								OnCancel();
							}
							break;
						case ERR_COMMON_INVALID_CLIENT_VERSION:
							{
								AtumMessageBox(STRERR_061011_0000);		// 2006-10-11 by cmkwon, 수정(STRERR_S_ATUMLAUNCHER_0023-->STRERR_061011_0000)
								OnCancel();
							}
							break;
						case ERR_DB_NO_SUCH_ACCOUNT:		// 2006-09-19 by cmkwon, 등록되지 않은 계정임
							{
								AtumMessageBox(STRMSG_060824_0000);
							}
							break;
						case ERR_PERMISSION_DENIED:			// 2006-09-27 by cmkwon, 접근 권한이 없습니다.
							{
								AtumMessageBox(STRMSG_060927_0000);		// 2006-09-27 by cmkwon, "현재 서비스 점검 중입니다. 자세한 사항은 홈페이지를 참고하세요."
								OnCancel();
							}
							break;
						case ERR_NO_SEARCH_CHARACTER:		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
						case ERR_DB_EXECUTION_FAILED:		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
							{
								AtumMessageBox(STRMSG_080430_0001);		// 2008-04-30 by cmkwon, "선택하신 서버는 신규 캐릭터 생성이 제한된 서버 입니다."
							}
							break;
						// 2012-07-11 by hskim, 선택적 셧다운
						case ERR_PROTOCOL_SELECTIVE_SHUTDOWN_NOT_ALLOWED_TIME:
							{
								AtumMessageBox(STRERR_S_ATUMLAUNCHER_0037);
								OnCancel();								
							}
							break;
						// end 2012-07-11 by hskim, 선택적 셧다운

						default:
							{
								sprintf(buf, STRERR_S_ATUMLAUNCHER_0024, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								AtumMessageBox(buf);
								OnCancel();
							}
							break;
						}
					}
				default:
					{
					}
				}

				SAFE_DELETE(pPacket);
			}
		}
		break;
	}

	return 0;
}

BOOL CAtumLauncherDlg::DownloadUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo)
{
	if(m_pUpdateFTPManager){		return FALSE;}

	VersionInfo OldVersion(pMsgUpdateInfo->OldVersion);
	VersionInfo UpdateVersion(pMsgUpdateInfo->UpdateVersion);
	char szDownLoadFileName[256];
// 2005-12-23 by cmkwon
//	sprintf(szDownLoadFileName, "%s/%s_%s.zip",	pMsgUpdateInfo->FtpUpdateDir, OldVersion.GetVersionString(), UpdateVersion.GetVersionString());
	sprintf(szDownLoadFileName, "%s/%s_%s.zip",	pMsgUpdateInfo->FtpUpdateDownloadDir, OldVersion.GetVersionString(), UpdateVersion.GetVersionString());

	DbgOut("Updating game data file(%s) From %s:%d by FTP\r\n", szDownLoadFileName, pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort);

	m_pUpdateFTPManager = new CFTPManager;
	
	BOOL bRet = m_pUpdateFTPManager->ConnectToServer(pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort,
									pMsgUpdateInfo->FtpAccountName, pMsgUpdateInfo->FtpPassword);
	if (!bRet)
	{
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0025);		
		return FALSE;
	}

	HINTERNET hFile;
	int nFileSize = m_pUpdateFTPManager->GetFileSize(szDownLoadFileName, hFile);
	if (nFileSize == -1)
	{
		DbgOut(STRERR_S_ATUMLAUNCHER_0026, szDownLoadFileName);
// 2006-05-26 by cmkwon, FTP Server 옵션에 List 보여주기 설정이 안되어 있을 수도 있다
// 		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0027);
// 		return FALSE;
		nFileSize		= 1000*1024*1024;			// 2008-11-27 by cmkwon, 기본 1000MBytes로 설정함. // 2006-05-26 by cmkwon, 기본 100MBytes로 설정함
	}

	m_progressCtrl.SetRange32(0, nFileSize);
	SetProgressGroupText((LPCSTR)(CString("updating from ") + OldVersion.GetVersionString()
							+ " to " + UpdateVersion.GetVersionString()));
	
	SetFTPUpdateState(UPDATE_STATE_DOWNLOADING);
	bRet = m_pUpdateFTPManager->DownloadFile(szDownLoadFileName, NULL, NULL, GetSafeHwnd());
	if (!bRet)
	{
		delete m_pUpdateFTPManager;
		m_pUpdateFTPManager = NULL;
		this->SetFTPUpdateState(UPDATE_STATE_INIT);
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0028);
		return FALSE;
	}

	return TRUE;
}

void CAtumLauncherDlg::ExtractUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo)
{
	VersionInfo OldVersion(pMsgUpdateInfo->OldVersion);
	VersionInfo UpdateVersion(pMsgUpdateInfo->UpdateVersion);
	char szUpdateFileName[256];
	sprintf(szUpdateFileName, "%s_%s.zip", OldVersion.GetVersionString(),
				UpdateVersion.GetVersionString());

	// extract zip file
	CZipArchive ZipOut;
	FILE* fp = NULL;
	errno_t no = fopen_s(&fp, szUpdateFileName, "rb");
	//if (ZipOut.FileExists(szUpdateFileName) != 1)
	if (no!=0)
	{
		fclose(fp);
		// error
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0029);
		exit(0);
	}
	fclose(fp);

	ZipOut.Open(szUpdateFileName, CZipArchive::zipOpen);

	// 2010-04-28 by cmkwon, 런처 스트링 오류 수정(K0002816) - 
	SetProgressGroupText((LPCSTR)(CString("extracting ") + szUpdateFileName));
	m_progressCtrl.SetRange32(0, ZipOut.GetCount());
	for (int i = 0; i < ZipOut.GetCount(); i++)
	{
		//if (!ZipOut.IsFileDirectory(i))
		if (!(ZipOut.GetFileInfo(i))->IsDirectory())
		{
			CZipFileHeader zipFileHeader;
			ZipOut.GetFileInfo(zipFileHeader, i);
			CString targetFileName = zipFileHeader.GetFileName();
			DWORD fileAttr = GetFileAttributes(targetFileName);
			if (fileAttr & FILE_ATTRIBUTE_READONLY != 0)
			{
				fileAttr = fileAttr & ~FILE_ATTRIBUTE_READONLY;
				SetFileAttributes(targetFileName, fileAttr);
			}
		}

		ZipOut.ExtractFile(i, "./", true);
		m_progressCtrl.SetPos(i+1);
	}

	ZipOut.Close();

	DeleteFile(szUpdateFileName);

	return;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::DownloadUpdateFileByHttp(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-08 ~ 2007-01-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::DownloadUpdateFileByHttp(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo)
{
	if(m_pHttpManager){						return FALSE;}

	VersionInfo OldVersion(pMsgUpdateInfo->OldVersion);
	VersionInfo UpdateVersion(pMsgUpdateInfo->UpdateVersion);
	char szDownLoadFileName[SIZE_MAX_FTP_FILE_PATH];
	char szDownLoadFileFullPath[SIZE_MAX_FTP_FILE_PATH];

	sprintf(szDownLoadFileName, "%s_%s.zip", OldVersion.GetVersionString(), UpdateVersion.GetVersionString());
	sprintf(szDownLoadFileFullPath, "%s/%s", pMsgUpdateInfo->FtpUpdateDownloadDir, szDownLoadFileName);

	DbgOut("Updating game data file(%s) From %s:%d by Http\r\n", szDownLoadFileName, pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort);

	m_pHttpManager		= new CHttpManager;

	SetFTPUpdateState(UPDATE_STATE_DOWNLOADING);
	Err_t errCode = m_pHttpManager->DownloadFileByHttp(pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort, szDownLoadFileFullPath, szDownLoadFileName, TRUE, this->GetSafeHwnd());
	if (ERR_NO_ERROR != errCode)
	{
		SAFE_DELETE(m_pHttpManager);
		this->SetFTPUpdateState(UPDATE_STATE_INIT);
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0028);
		return FALSE;
	}

	SetProgressGroupText((LPCSTR)(CString("updating from ") + OldVersion.GetVersionString()	+ " to " + UpdateVersion.GetVersionString()));
	return TRUE;
}

// WPARAM: Socket descriptor
LONG CAtumLauncherDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	if ((SOCKET)wParam == m_PreSD)
		m_pUpdateWinsocket->OnAsyncEvent(lParam);
/*	else if ((SOCKET)wParam == m_FieldSD)
		m_pFieldWinsocket->OnAsyncEvent(lParam);*/

	return 0;
}

LONG CAtumLauncherDlg::OnDownLoadGamefilesDone(WPARAM wParam, LPARAM lParam)
{
	if (m_bCancelFlag)
	{
		return -1;
	}

	CString cstrVersion = m_CurrentVersion.GetVersionString();
	SetFileNumText((char*)(LPCTSTR)("VER " + cstrVersion));

	///////////////////////////////////////////////////////////////////////////
	// 다운로드 완료후 버전 수정 후 다시 CONNECT_VERSION 전송
	((CAtumLauncherApp*)AfxGetApp())->WriteProfile();

	/////////////////////////////////////////////
	// Send Version
	INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_VERSION, T_PC_CONNECT_VERSION, msgVersion, msgVersionBuff);
	msgVersion->ClientVersion[0] = m_CurrentVersion.GetVersion()[0];
	msgVersion->ClientVersion[1] = m_CurrentVersion.GetVersion()[1];
	msgVersion->ClientVersion[2] = m_CurrentVersion.GetVersion()[2];
	msgVersion->ClientVersion[3] = m_CurrentVersion.GetVersion()[3];
	m_pUpdateWinsocket->Write(msgVersionBuff, MSG_SIZE(MSG_PC_CONNECT_VERSION));

	return 0;
}

void CAtumLauncherDlg::OnDestroy()
{
	CDialog::OnDestroy();

	SAFE_DELETE(m_pUpdateWinsocket);

	CWinSocket::SocketClean();
}

void CAtumLauncherDlg::OnOk()
{
	// TODO: Add your control notification handler code here

	if(m_SelectFlag)
	{
		UpdateData();
		// 7/13/2006 by dgwoo
		if (m_ServerList->GetCurSel() == LB_ERR
			|| m_ServerList->GetCount() < 1)
		{
			AtumMessageBox(STRMSG_S_ATUMLAUNCHER_0001);
			return;
		}

		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 -
		CString csWDegree;
		GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->GetWindowText(csWDegree);
		if(csWDegree.IsEmpty())
		{
			AtumMessageBox(STRMSG_071228_0001);
			return;
		}
		if(FALSE == this->IsDlgButtonChecked(IDC_CHECK_WINDOWS_MODE))
		{
			m_nWindowModeReg		= GAME_MODE_FULLSCREEN;
		}
		else
		{
			m_nWindowModeReg		= GAME_MODE_WINDOW;
		}

		SendLogin(LOGIN_TYPE_DIRECT);
// 2007-03-06 by cmkwon, 엠게임 소스 제거로 필요 없음
//		//////////////////////////////////////////////////////////////////////
//		// Send Login
//#if defined(_ATUM_DEVELOP) || defined(_MASANG15_SERVER) || defined(_MASANG51_SERVER) || defined(_GLOBAL_ENG_SERVER) || defined(_VTC_VIET_SERVER) || defined(_KOREA_SERVER_2)
//		SendLogin(LOGIN_TYPE_DIRECT);
//#else
//		SendLogin(LOGIN_TYPE_MGAME);
//#endif// end_ATUM_DEVELOP

//		m_nServerGroupReg = m_nServer;
		m_szAccountName.MakeLower();
		m_szAccountNameReg = m_szAccountName;
		m_szPasswordReg = m_szPassword;
		m_csWindowsResolutionReg	= csWDegree;	// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 -



		((CAtumLauncherApp*)AfxGetApp())->WriteProfile();

		// 게임 시작 버튼 여러번 누르는 거 방지
		DisableControls();
		SetTimer(TIMERID_ENABLE_CONTROL, 2000, NULL);
	}
	else
	{
		AfxMessageBox(STRMSG_S_ATUMLAUNCHER_0001);
	}
}

BOOL CAtumLauncherDlg::SendLogin(BYTE i_nLoginType)
{
	INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_LOGIN, T_PC_CONNECT_LOGIN, msgLogin, msgLoginBuf);

	msgLogin->LoginType = i_nLoginType;
	if (i_nLoginType == LOGIN_TYPE_DIRECT)
	{
		// 크로세스 인증용
		// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 
		//STRNCPY_MEMSET(msgLogin->AccountName, m_szAccountName + m_szCrocessSuffix, SIZE_MAX_ACCOUNT_NAME);
		STRNCPY_MEMSET(msgLogin->AccountName, m_szAccountName + m_szCrocessSuffix, SIZE_MAX_ORIGINAL_ACCOUNT_NAME);
	}
	else if (i_nLoginType == LOGIN_TYPE_MGAME)
	{		
		CString szTmpMGameID	= g_szMGameID;
		szTmpMGameID.MakeLower();
		m_szAccountName			= szTmpMGameID;
		// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 
		//STRNCPY_MEMSET(msgLogin->AccountName, szTmpMGameID, SIZE_MAX_ACCOUNT_NAME);
		STRNCPY_MEMSET(msgLogin->AccountName, szTmpMGameID, SIZE_MAX_ORIGINAL_ACCOUNT_NAME);
	}
	else
	{
		return FALSE;
	}

	if (i_nLoginType == LOGIN_TYPE_DIRECT)
	{
		MD5 MD5_instance;
		char szTmPassword[1024];			// 2006-05-22 by cmkwon, 비번에 추가 스트링 적용
		MEMSET_ZERO(szTmPassword, 1024);
		wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, m_szPassword);
		MD5_instance.MD5Encode(szTmPassword, msgLogin->Password);
	}
	else if (i_nLoginType == LOGIN_TYPE_MGAME)
	{
		MEMSET_ZERO(msgLogin->Password, SIZE_MAX_PASSWORD_MD5);
	}
	else
	{
		return FALSE;
	}

///////////////////////////////////////////////////////////////////////////////
// 2006-10-02 by cmkwon

// 2012-07-11 by hskim, 선택적 셧다운

	// 7/13/2006 by dgwoo
	int nCrowdedness = m_ServerList->GetCrowdedness(m_ServerList->GetCurSel());
	if(0 == nCrowdedness)
	{
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0030);
		return FALSE;
	}

	CString szSelectedServerName;

	if(FALSE == this->FindServerGroupName(&szSelectedServerName, m_ServerList->GetCurSel()))
	{// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 인덱스로 서버그룹 이름을 검색한다.
		AtumMessageBox("Invalid ServerGroup");
		return FALSE;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// ServerGroupName을 설정한다
	m_strServerGroupName = szSelectedServerName;
	STRNCPY_MEMSET(msgLogin->FieldServerGroupName, szSelectedServerName, SIZE_MAX_SERVER_NAME);
	STRNCPY_MEMSET(msgLogin->PrivateIP, m_PrivateIP, SIZE_MAX_IPADDRESS);
// 2007-06-05 by cmkwon, 필요없음 - 아래와 같이 수정함
//	if (i_nLoginType == LOGIN_TYPE_DIRECT)
//	{
//		msgLogin->MGameSEX = 0;
//		msgLogin->MGameYear = 0;
//	}
//	else if (i_nLoginType == LOGIN_TYPE_MGAME)
//	{
//		msgLogin->MGameSEX = atoi(g_szMGameSEX);
//		msgLogin->MGameYear = atoi(g_szMGameYear);
//	}
//	else
//	{
//		return FALSE;
//	}
	msgLogin->MGameSEX		= 0;							// 2007-06-05 by cmkwon, 성별 - 모름=0, 남자=1, 여자=2
	msgLogin->MGameYear		= this->m_nBirthYear;			// 2007-06-05 by cmkwon

// 2008-02-11 by cmkwon, Yedang_Global_Eng 외부 인증 처리 - Yedang_Global_Eng 에 패스워드 인증을 WebLoginAuthKey 를 사용함, Password 는 마상에서만 사용함
#ifndef SERVICE_TYPE_VIETNAMESE_SERVER_1
	// 2007-07-03 by cmkwon, 중국 Yetime 비밀 번호 MD5 하지 않음 - 임시로 WebLoginAuthKey 필드 사용함
	STRNCPY_MEMSET(msgLogin->WebLoginAuthKey, (LPCSTR)m_szPassword, SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY);	
#endif

	return m_pUpdateWinsocket->Write(msgLoginBuf, MSG_SIZE(MSG_PC_CONNECT_LOGIN));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/*=NULL*/, UINT nType/*=MB_OK*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/*=NULL*/, UINT nType/*=MB_OK*/)
{
// 2008-05-09 by cmkwon, EXE1, EXE2 에서 태국어 깨지는 문제 해결
//#ifdef SERVICE_TYPE_VIETNAMESE_SERVER_1
#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
	WCHAR wcText[1024];
	MEMSET_ZERO(wcText, sizeof(wcText));
	WCHAR wcCaption[1024];
	MEMSET_ZERO(wcCaption, sizeof(wcCaption));
	// 2008-05-09 by cmkwon, CodePage 정의 추가 - 
	//MultiByteToWideChar(1258, 0, lpszText, strlen(lpszText)+1, wcText, sizeof(wcText)/sizeof(wcText[0]));
	MultiByteToWideChar(CODE_PAGE, 0, lpszText, strlen(lpszText)+1, wcText, sizeof(wcText)/sizeof(wcText[0]));

	char	szCaption[1024] = STRMSG_WINDOW_TEXT;
	if(lpszCaption)
	{
		strncpy(szCaption, lpszCaption, 1023);
	}
	// 2008-05-09 by cmkwon, CodePage 정의 추가 - 
	//MultiByteToWideChar(1258, 0, szCaption, strlen(szCaption)+1, wcCaption, sizeof(wcCaption)/sizeof(wcCaption[0]));
	MultiByteToWideChar(CODE_PAGE, 0, szCaption, strlen(szCaption)+1, wcCaption, sizeof(wcCaption)/sizeof(wcCaption[0]));
	return ::MessageBoxW(GetSafeHwnd(), wcText, wcCaption, nType);
#else

	if(lpszCaption)
	{
		return MessageBox(lpszText, lpszCaption, nType);
	}
	return MessageBox(lpszText, STRMSG_WINDOW_TEXT, nType);	
#endif	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::ProcessDeleteFileListByHttp(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-08 ~ 2007-01-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::ProcessDeleteFileListByHttp(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo)
{
	char			szDelFileName[SIZE_MAX_FTP_FILE_PATH];
	CHttpManager	httpMan;
	Err_t			errCode;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, 임시 파일 이름
	STRNCPY_MEMSET(szDelFileName, "tmDelete.txt", SIZE_MAX_FTP_FILE_PATH);
	errCode = httpMan.DownloadFileByHttp(pUpdateInfo->FtpIP, pUpdateInfo->FtpPort, pUpdateInfo->DeleteFileListDownloadPath, szDelFileName);
	if(ERR_NO_ERROR != errCode)
	{
		return FALSE;
	}

#ifdef _DEBUG
		char	szDir[512];
		GetCurrentDirectory(512, szDir);
#endif

	///////////////////////////////////////////////////////////////////////////////		
	// 처리
	ifstream isDel;
	isDel.open(szDelFileName);
	if(false == isDel.is_open())
	{
		return FALSE;
	}
	char delFileLine[SIZE_MAX_FTP_FILE_PATH];
	bool bFileEndFlag = false;
	do
	{
		isDel.getline(delFileLine, SIZE_MAX_FTP_FILE_PATH);
		bFileEndFlag = isDel.eof();
		if(bFileEndFlag)
		{
			break;
		}

		// 0x0D(CR)를 제거하지 못한 경우 제거함
		if (delFileLine[strlen(delFileLine)-1] == 0x0D)
		{
			delFileLine[strlen(delFileLine)-1] = '\0';
		}

		if (delFileLine[0] == '#')
		{// comment임

			continue;
		}

		::DeleteFile(delFileLine);
		SetLastError(0);				// 2007-01-08 by cmkwon
	}while(false == bFileEndFlag);
	isDel.close();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, 임시 파일을 삭제한다
	::DeleteFile(szDelFileName);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLauncherDlg::MoveWindow2Center(void)
/// \brief		// 2007-09-07 by cmkwon, 베트남 런처 인터페이스 수정 -
/// \author		cmkwon
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLauncherDlg::MoveWindow2Center(void)
{
	int nWidth	= EXE2_LAUNCHER_BG_SIZE_X + 6;
	int nHeight	= EXE2_LAUNCHER_BG_SIZE_Y + 6 ;

	LONG x,y;
	x=LONG((GetSystemMetrics(SM_CXSCREEN)-nWidth)/2); 
	y=LONG((GetSystemMetrics(SM_CYSCREEN)-nHeight)/2);
	
	MoveWindow(x, y, nWidth, nHeight, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLauncherDlg::PushCharFromScreenKeyboard(char i_cPushChar)
/// \brief		// 2007-09-11 by cmkwon, 베트남 화면키보드 구현 -
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLauncherDlg::PushCharFromScreenKeyboard(char i_cPushChar)
{
	if(NULL == m_pScreenKeyboardDlg)
	{
		return;
	}

	if(NULL == m_pInputEditFromScreenKeyboard)
	{
		m_pInputEditFromScreenKeyboard = GetDlgItem(IDC_EDIT_ACCOUNT);
	}

	CString strCurText, strResultText;
	m_pInputEditFromScreenKeyboard->GetWindowText(strCurText);
	strResultText.Format("%s%c", strCurText, i_cPushChar);
	m_pInputEditFromScreenKeyboard->SetWindowText(strResultText);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLauncherDlg::DeleteCharFromScreenKeyboard(void)
/// \brief		// 2007-09-11 by cmkwon, 베트남 화면키보드 구현 -
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLauncherDlg::DeleteCharFromScreenKeyboard(void)
{
	if(NULL == m_pScreenKeyboardDlg)
	{
		return;
	}

	if(NULL == m_pInputEditFromScreenKeyboard)
	{
		m_pInputEditFromScreenKeyboard = GetDlgItem(IDC_EDIT_ACCOUNT);
	}

	CString strCurText;
	m_pInputEditFromScreenKeyboard->GetWindowText(strCurText);
	if(strCurText.IsEmpty())
	{
		return;
	}
	strCurText.Delete(strCurText.GetLength()-1);
	m_pInputEditFromScreenKeyboard->SetWindowText(strCurText);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLauncherDlg::HideScreenKeyboardByScreenKeyboardWindow(void)
/// \brief		// 2007-09-18 by cmkwon, 화상키보드 수정 - 
/// \author		cmkwon
/// \date		2007-09-18 ~ 2007-09-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLauncherDlg::HideScreenKeyboardByScreenKeyboardWindow(void)
{
	m_bHideScreenKeyboardByScreenKeyboardWindow	= TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::FindWindowResolutionByWindowDegree(int *o_pnCX, int *o_pnCY, int *o_pnDegree, char *i_szWindowDegreeName)
/// \brief		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - CAtumLauncherDlg::FindWindowResolutionByWindowDegree() 추가
/// \author		cmkwon
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::FindWindowResolutionByWindowDegree(int *o_pnCX, int *o_pnCY, int *o_pnDegree, char *i_szWindowDegreeName)
{
	*o_pnCX		= 0;
	*o_pnCY		= 0;
	*o_pnDegree	= 0;

	for(int i=0; g_pWindowDegreeList[i].szWindowDegreeName != NULL; i++)
	{
		if(0 == strncmp(g_pWindowDegreeList[i].szWindowDegreeName, i_szWindowDegreeName, SIZE_MAX_WINDOW_DEGREE_NAME))
		{
			*o_pnCX		= g_pWindowDegreeList[i].nCX;
			*o_pnCY		= g_pWindowDegreeList[i].nCY;
			*o_pnDegree	= g_pWindowDegreeList[i].nDegree;
			return TRUE;
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::InsertWindowDegreeList(CComboBox *i_pComboBox, BOOL i_bWindowsMode)
/// \brief		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - CAtumLauncherDlg::InsertWindowDegreeList() 추가
/// \author		cmkwon
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::InsertWindowDegreeList(CComboBox *i_pComboBox, BOOL i_bWindowsMode)
{
	i_pComboBox->ResetContent();	// 2007-12-28 by cmkwon, 모든 아이템 초기화

	// 2013-07-30 by jhseol, 멀티해상도 지원 - 전체 해상도 확인
#ifdef S_MULTI_RESOLUTION_JHSEOL	// - 전체 해상도 확인
	int nCXScreen = GetSystemMetrics(SM_CXMAXTRACK);
	int nCYScreen = GetSystemMetrics(SM_CYMAXTRACK);
#else
	int nCXScreen = GetSystemMetrics(SM_CXSCREEN);
	int nCYScreen = GetSystemMetrics(SM_CYSCREEN);
#endif
	// end 2013-07-30 by jhseol, 멀티해상도 지원 - 전체 해상도 확인


	int nInsertedCnts = 0;
	for(int i=0; g_pWindowDegreeList[i].szWindowDegreeName != NULL; i++)
	{
		// 2013-07-30 by jhseol, 멀티해상도 지원 - 전체 해상도 안에 포함되는 해상도만 등록
#ifdef S_MULTI_RESOLUTION_JHSEOL	// - 전체 해상도 안에 포함되는 해상도만 등록
		if( FALSE == i_bWindowsMode
			|| (nCXScreen >= g_pWindowDegreeList[i].nCX && nCYScreen >= g_pWindowDegreeList[i].nCY) )
		{
			i_pComboBox->AddString(g_pWindowDegreeList[i].szWindowDegreeName);
			nInsertedCnts++;
		}
#else
		if(FALSE == this->IsSupportedResolution(g_pWindowDegreeList[i].nCX, g_pWindowDegreeList[i].nCY))
		{// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - 지원하지 않는 해상도는 보여주지 안는다
			continue;
		}

		if( FALSE == i_bWindowsMode
			|| (nCXScreen >= g_pWindowDegreeList[i].nCX	&& nCYScreen >= g_pWindowDegreeList[i].nCY) )
		{
			i_pComboBox->AddString(g_pWindowDegreeList[i].szWindowDegreeName);

			nInsertedCnts++;
		}
#endif
	}
	return nInsertedCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::FindWindowDegreeComboBoxIndex(CComboBox *i_pComboBox, char *i_szWindowDegreeName)
/// \brief		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - CAtumLauncherDlg::FindWindowDegreeComboBoxIndex() 추가
/// \author		cmkwon
/// \date		2007-12-28 ~ 2007-12-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::FindWindowDegreeComboBoxIndex(CComboBox *i_pComboBox, char *i_szWindowDegreeName)
{
	int nItemCnts = i_pComboBox->GetCount();
	for(int i=0; i < nItemCnts; i++)
	{
		CString csWDegree;
		i_pComboBox->GetLBText(i, csWDegree);



		if(0 == csWDegree.Compare(i_szWindowDegreeName))
		{
			return i;
		}
	}

	return -1;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::InitSupportedWindowResolutionList(void)
/// \brief		// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - CAtumLauncherDlg::InitSupportedWindowResolutionList() 추가
/// \author		cmkwon
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::InitSupportedWindowResolutionList(void)
{
 	///////////////////////////////////////////////////////////////////////////////
 	// 2008-06-12 by cmkwon, 임시 체크용, 지원 해상도 로그 파일로 저장 - 필요 없을경우 주석으로 처리
// 	BOOL bSaveFile	= TRUE;
// 	CSystemLogManager resultLog;
// 	if(FALSE == resultLog.InitLogManger(TRUE, "LauncherLog", "./"))
// 	{
// 		bSaveFile	= FALSE;
// 	}

	DEVMODE devMode;
	INT32 modeExist;
	for (int i=0; ;i++) 
	{
		modeExist = EnumDisplaySettings(NULL, i, &devMode);
		if (!modeExist) 
		{
			break;
		}

		
// 2008-06-12 by cmkwon, 임시 체크용, 지원 해상도 로그 파일로 저장
// 		if(bSaveFile)
// 		{
// 			// 2008-05-23 by cmkwon, 체크용
// 			//DbgOut("Resolution Idx(%3d) %4d x %4d , BitsPerPel(%2d) Frequency(%3d)\r\n", i, devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
// 			char szResult[2048];
// 			MEMSET_ZERO(szResult, 2048);
// 			sprintf(szResult, "Resolution Idx(%3d) %4d x %4d , BitsPerPel(%2d) Frequency(%3d)\r\n", i, devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
// 			resultLog.WriteSystemLog(szResult);
// 		}

// 2008-10-31 by cmkwon, 런처(Luncher)에서 해상도 체크시 픽셀만 체크로 수정(색상비트는 체크하지 않음) - 체크 필요 없음
//		// 2008-06-12 by cmkwon, Win98, Win98ME 에서 해상도 1개도 나오지 않는 문제 수정(K0000227) - win98, win98ME 에서는 dmDisplayFrequency 이 0으로 리턴되고 있음.
//		if( 32 != devMode.dmBitsPerPel
//			|| (0 != devMode.dmDisplayFrequency && 60 != devMode.dmDisplayFrequency) )
//		{
//			continue;
//		}

		m_vectSupportedResolutionList.push_back(devMode);
	}

	return m_vectSupportedResolutionList.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::IsSupportedResolution(int i_nWidth, int i_nHeight)
/// \brief		// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - CAtumLauncherDlg::IsSupportedResolution() 추가
/// \author		cmkwon
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::IsSupportedResolution(int i_nWidth, int i_nHeight)
{
	int nCnts = m_vectSupportedResolutionList.size();
	if (i_nWidth == 0 && i_nHeight == 0)
	{
		return TRUE;
	}

	for(int i=0; i < nCnts; i++)
	{
		DEVMODE *pDevMode = &(m_vectSupportedResolutionList[i]);
		if(pDevMode->dmPelsWidth == i_nWidth
			&& pDevMode->dmPelsHeight == i_nHeight)
		{
			return TRUE;
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::FindServerGroupName(CString *o_pcsServerGroupName, int i_nFindIndex)
/// \brief		// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - CAtumLauncherDlg::FindServerGroupName() 추가
/// \author		cmkwon
/// \date		2008-02-14 ~ 2008-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::FindServerGroupName(CString *o_pcsServerGroupName, int i_nFindIndex)
{
	o_pcsServerGroupName->Empty();
	int i = 0;
	for(i=0; i < m_vectServerGroupList.size(); i++)
	{
		if(i_nFindIndex == m_vectServerGroupList[i].nIndex)
		{
			*o_pcsServerGroupName = m_vectServerGroupList[i].szServerGroupName;
			return TRUE;
		}
	}
	return FALSE;
}





/*
BOOL CAtumLauncherDlg::SetCurrentClientVersion()
{
	const int SIZE_BUFF = 512;
	char buff[SIZE_BUFF];
	char *token;
	char seps[] = " \t";

	ifstream fin;

	CString cstrVersionFile = CLIENT_DOWNLOAD_DIR;
	cstrVersionFile += CLIENT_VERSION_FILE_NAME;
	fin.open((LPCTSTR)cstrVersionFile);

	if (! fin.is_open())
	{
#ifdef _ATUM_DEVELOP
		m_CurrentVersion.SetVersion(0, 0, 0, 0);
		return TRUE;
#else // _ATUM_DEVELOP
		return FALSE;
#endif // _ATUM_DEVELOP
	}

	fin.getline(buff, SIZE_BUFF);

	token = strtok(buff, seps);

	m_CurrentVersion.SetVersion(token);

	fin.close();

	return TRUE;
}
*/


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::NTStartNetworkCheck(void)
/// \brief		// 2007-06-18 by cmkwon, 네트워크 상태 체크 
/// \author		cmkwon
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::NTStartNetworkCheck(void)
{
	if(NULL == m_pUpdateWinsocket
		|| FALSE == m_pUpdateWinsocket->IsConnected()
		|| -1 != m_nCurNetworkCheckCount)
	{
		return FALSE;
	}


	// 2007-06-18 by cmkwon, 초기화
	m_dwSumPacketTickGap		= 0;
	m_dwNetworkCheckSendTick	= timeGetTime();

	INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_NETWORK_CHECK, T_PC_CONNECT_NETWORK_CHECK, pNetCheck, SendBuf);
	pNetCheck->nCheckCount		= m_nCurNetworkCheckCount;	
	m_pUpdateWinsocket->Write(SendBuf, MSG_SIZE(MSG_PC_CONNECT_NETWORK_CHECK));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::NTOnReceivedNetworkCheckOK(int i_nCheckCount)
/// \brief		// 2007-06-18 by cmkwon, 네트워크 상태 체크
/// \author		cmkwon
/// \date		2007-06-18 ~ 2007-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::NTOnReceivedNetworkCheckOK(int i_nCheckCount)
{
	if(NULL == m_pUpdateWinsocket
		|| FALSE == m_pUpdateWinsocket->IsConnected()
		|| m_nCurNetworkCheckCount != i_nCheckCount)
	{
		return NTGetPingAverageTime();
	}
	
	DWORD dwCurTick				= timeGetTime();

	m_nCurNetworkCheckCount++;
	m_dwSumPacketTickGap		+= dwCurTick - m_dwNetworkCheckSendTick;

	Sleep(500);		// 2007-06-19 by cmkwon, 약간의 시간차를 둔다

	m_dwNetworkCheckSendTick	= timeGetTime();

	if(m_nMaxNetworkCheckCount <= m_nCurNetworkCheckCount)
	{
		return NTGetPingAverageTime();
	}
	INIT_MSG_WITH_BUFFER(MSG_PC_CONNECT_NETWORK_CHECK, T_PC_CONNECT_NETWORK_CHECK, pNetCheck, SendBuf);
	pNetCheck->nCheckCount		= m_nCurNetworkCheckCount;	
	m_pUpdateWinsocket->Write(SendBuf, MSG_SIZE(MSG_PC_CONNECT_NETWORK_CHECK));

	return NTGetPingAverageTime();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumLauncherDlg::NTGetPingAverageTime(void)
/// \brief		// 2007-06-18 by cmkwon, 네트워크 상태 체크
/// \author		cmkwon
/// \date		2007-06-19 ~ 2007-06-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumLauncherDlg::NTGetPingAverageTime(void)
{
	if(0 >= m_nCurNetworkCheckCount)
	{// 2007-06-19 by cmkwon, 시작상태가 아니거나 처음 패킷을 받지 못한 상태 일경우

		return 0;		// 2007-06-21 by cmkwon, 평균 Ping 속도를 리턴하도록 수정함 - 
	}

	DWORD dwAverageTick = m_dwSumPacketTickGap/m_nCurNetworkCheckCount;

// 2007-06-21 by cmkwon, 평균 Ping 속도를 리턴하도록 수정함
//	const int nBestStateTick	= 100;
//	const int nWorstStateTick	= 300;
//	if(dwAverageTick <= nBestStateTick)
//	{
//		nRetState = 100;
//	}
//	else if(dwAverageTick >= nWorstStateTick)
//	{
//		nRetState = 1;
//	}
//	else
//	{
//		nRetState = 100 - ( 100 * (dwAverageTick-nBestStateTick) )/(nWorstStateTick - nBestStateTick);
//	}
//	

// 2007-07-13 by cmkwon, 필요 없음
//	DbgOut("	 AverageTickGap[%8d] <== Count(%d), SumTickGap(%8d)\r\n"
//		, dwAverageTick, m_nCurNetworkCheckCount, m_dwSumPacketTickGap);

	return dwAverageTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLauncherDlg::NTCheckTimeOver(DWORD *o_pdwPingTime)
/// \brief		// 2007-06-18 by cmkwon, 네트워크 상태 체크
/// \author		cmkwon
/// \date		2007-06-19 ~ 2007-06-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLauncherDlg::NTCheckTimeOver(DWORD *o_pdwPingTime)
{
	if(0 > m_nCurNetworkCheckCount
		|| m_nMaxNetworkCheckCount <= m_nCurNetworkCheckCount)
	{
		return FALSE;
	}

	DWORD dwCurTick				= timeGetTime();
	if(TICKGAP_NETWORK_STATE_WORST_PING_TICK > dwCurTick - m_dwNetworkCheckSendTick)
	{// 2007-06-19 by cmkwon, 처리가 필요 없으므로 그냥 현재 상태를 리턴
		*o_pdwPingTime = NTGetPingAverageTime();
	}
	else
	{
		// 2007-06-19 by cmkwon, 현재 패킷을 초기화 하고 다음 패킷 전송
		*o_pdwPingTime = NTOnReceivedNetworkCheckOK(m_nCurNetworkCheckCount);
	}

	return TRUE;
}


char* CAtumLauncherDlg::GetFileName(char *szFullPathName, char *szFileName)
{
	char *token;
	char seps[] = "/";
	int len = strlen(szFullPathName);

	strcpy(szFileName, szFullPathName);

	token = strtok(szFileName, seps);
	len -= strlen(token);

	while( len > 7 )
	{
		token = strtok(NULL, seps);
		len -= strlen(token);
	}

	STRNCPY_MEMSET(szFileName, token, SIZE_MAX_FTP_FILE_PATH);

	return szFileName;
}

// devide문자를 경계로 리스트에 넣는다.
BOOL CAtumLauncherDlg::Tokenizer(CList <CString, CString&>& lsString, CString strMsg, TCHAR devide)
{
	if( strMsg.IsEmpty() ) return FALSE;
	int nStart = 0;
	int pos;
	if( strMsg[strMsg.GetLength()-1] != devide )
		strMsg += devide;
	pos = strMsg.Find( devide, nStart );
	if( pos == -1)
		lsString.AddTail( strMsg );
	while( (pos = strMsg.Find( devide, nStart )) != -1 )	{
		lsString.AddTail( strMsg.Mid( nStart, pos - nStart ) );
		nStart = pos+1;
	}
	return TRUE;
}

int CAtumLauncherDlg::CreateDirectory(CString strParent, CList<CString, CString&> &lsDir)
{
	POSITION pos = lsDir.GetHeadPosition();
	strParent += lsDir.GetNext(pos);
	if(pos)// 마지막은 디렉토리가 아닌 복사할 파일
	{
		CFileFind finder;
		if(finder.FindFile(strParent))
		{

		    finder.FindNextFile();
			if(!finder.IsDirectory())
			{
				DeleteFile(strParent); // 파일로 존재하면 지운다.
				if(!::CreateDirectory(strParent,NULL))
					return -1;
			}
		} else
			if(!::CreateDirectory(strParent,NULL))
				return -1;

		lsDir.GetPrev(pos);
		lsDir.RemoveAt(pos);
		strParent += '/';
		if(CreateDirectory(strParent,lsDir)==-1)
			return -1;
	}
	return 0;

}

void CAtumLauncherDlg::SetProgressRange(int up)
{
	m_progressCtrl.SetRange32(0, up);
}

void CAtumLauncherDlg::SetProgressPos(int pos)
{
	m_progressCtrl.SetPos(pos);
}

void CAtumLauncherDlg::SetFileNumText(char* str)
{
	CWnd *pWndStatus = GetDlgItem(IDC_DOWNLOAD_FILENUM);
// 2007-09-11 by cmkwon, 아래와 같이 수정함
//	InvalidateRect(CRect(90,579,190,592),true);
	InvalidateRect(CRect(EXE2_BG_DOWNLOAD_FILE_STATIC_POS_X,EXE2_BG_DOWNLOAD_FILE_STATIC_POS_Y,EXE2_BG_DOWNLOAD_FILE_STATIC_POS_X + EXE2_BG_DOWNLOAD_FILE_STATIC_WIDTH,EXE2_BG_DOWNLOAD_FILE_STATIC_POS_Y+EXE2_BG_DOWNLOAD_FILE_STATIC_HEIGHT),true);
	
	pWndStatus->SetWindowText(str);
}

void CAtumLauncherDlg::SetProgressGroupText(const char* str)
{
	//ysw : 삭제
	CWnd *pWndStatus = GetDlgItem(IDC_FILE_INFO);
	// 7/10/2006 by dgwoo 글씨에 잔상이 남는것을 막기위한 부분.

// 2007-09-11 by cmkwon, 아래와 같이 수정함
//	InvalidateRect(CRect(190,579,480, 592),true);
	InvalidateRect(CRect(EXE2_BG_UPDATE_INFO_STATIC_POS_X,EXE2_BG_UPDATE_INFO_STATIC_POS_Y,EXE2_BG_UPDATE_INFO_STATIC_POS_X+EXE2_BG_UPDATE_INFO_STATIC_WIDTH, EXE2_BG_UPDATE_INFO_STATIC_POS_Y+EXE2_BG_UPDATE_INFO_STATIC_HEIGHT),true);
	
	CPaintDC dc(this);
	CBitmap *oldbitmap;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	oldbitmap = (CBitmap *)memDC.SelectObject(&m_BackGround);
	dc.BitBlt(EXE2_BG_UPDATE_INFO_STATIC_POS_X,EXE2_BG_UPDATE_INFO_STATIC_POS_Y,EXE2_BG_UPDATE_INFO_STATIC_WIDTH,EXE2_BG_UPDATE_INFO_STATIC_HEIGHT, &memDC,EXE2_BG_UPDATE_INFO_STATIC_POS_X,EXE2_BG_UPDATE_INFO_STATIC_POS_Y,SRCCOPY);
	memDC.SelectObject(oldbitmap);
	memDC.DeleteDC();
	
	WCHAR wcText[1024]; 
	MEMSET_ZERO(wcText, sizeof(wcText)); 
	// 2008-05-09 by cmkwon, CodePage 정의 추가 - 
	//MultiByteToWideChar(1258, 0, str, strlen(str)+1, wcText, sizeof(wcText)/sizeof(wcText[0])); 
	MultiByteToWideChar(CODE_PAGE, 0, str, strlen(str)+1, wcText, sizeof(wcText)/sizeof(wcText[0])); 
	::SetWindowTextW(pWndStatus->GetSafeHwnd(), wcText);
}


void CAtumLauncherDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	m_bCancelFlag = TRUE;
	if (!m_bProcessingVersionUpdate)
	{
//		AtumMessageBox("Operation Canceled!");
	}
// start 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
	if(m_pVTCGuard)
	{
		m_pVTCGuard->SetUpdateThreadCancelFlag(TRUE);
		Sleep(100);
		m_pVTCGuard->ThreadEnd(1000);
		SAFE_DELETE(m_pVTCGuard);
	}														
// end 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

	CDialog::OnCancel();
}

void CAtumLauncherDlg::DisableControls()
{
	m_bControlEnabled = FALSE;

	GetDlgItem(IDC_EDIT_ACCOUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDGO)->EnableWindow(FALSE);
	m_KbcGO.SetButtonDisable();
	GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(FALSE);
	GetDlgItem(IDC_LIST)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(FALSE);	// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정
	GetDlgItem(IDC_CHECK_WINDOWS_MODE)->EnableWindow(FALSE);			// 2008-01-17 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정

	m_ctlBtnRememberID.EnableWindow(FALSE);			// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 체크박스 위치

	//ysw : 삭제
//	GetDlgItem(IDC_DECA_SERVER)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BATTALUS_SERVER)->EnableWindow(FALSE);
//	GetDlgItem(IDC_SHARRINE_SERVER)->EnableWindow(FALSE);
//	GetDlgItem(IDC_PHILON_SERVER)->EnableWindow(FALSE);


	SendDlgItemMessage(IDOK, BM_SETSTYLE, BS_PUSHBUTTON, (LONG)TRUE);
	SendDlgItemMessage(IDCANCEL, BM_SETSTYLE, BS_DEFPUSHBUTTON, (LONG)TRUE);
	GetDlgItem(IDCANCEL)->SetFocus();
}

void CAtumLauncherDlg::EnableControls()
{
//	DbgOut("	2007-07-13 by cmkwon, 111\r\n");	// 2007-07-13 by cmkwon, 테스트 
	m_bControlEnabled = TRUE;

	int nLower, nUpper;
	m_progressCtrl.GetRange(nLower, nUpper);
	m_progressCtrl.SetPos(nUpper);

	GetDlgItem(IDC_EDIT_ACCOUNT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDGO)->EnableWindow(TRUE);
	m_KbcGO.SetButtonEnable();
// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정
//	if (m_nWindowModeReg == GAME_MODE_WINDOW)
//	{
//		GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(FALSE);
//	}
//	else
//	{
//		GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(TRUE);
//	}
	GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER)->EnableWindow(TRUE);	// 2008-01-03 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정
	GetDlgItem(IDC_CHECK_WINDOWS_MODE)->EnableWindow(TRUE);				// 2008-01-17 by cmkwon, 윈도우모드 상태 저장하기 - 버그 수정
	GetDlgItem(IDC_LIST)->EnableWindow(TRUE);

	m_ctlBtnRememberID.EnableWindow();			// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 체크박스 위치

	GetDlgItem(IDC_EDIT_ACCOUNT)->GetWindowText(m_szAccountNameReg);
	if (m_szAccountNameReg == "")
	{
		GetDlgItem(IDC_EDIT_ACCOUNT)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_PASSWORD))->SetSel(0, -1);
	}
#ifdef _DEBUG
	//	- beta service에서는 서버 선택 불가능함, 12.16.

//ysw : 삭제
//	GetDlgItem(IDC_DECA_SERVER)->EnableWindow(TRUE);
//	GetDlgItem(IDC_BATTALUS_SERVER)->EnableWindow(TRUE);
//	GetDlgItem(IDC_SHARRINE_SERVER)->EnableWindow(TRUE);
//	GetDlgItem(IDC_PHILON_SERVER)->EnableWindow(TRUE);
	//*/
#endif
	// reset default button
	SendDlgItemMessage(IDCANCEL, BM_SETSTYLE, BS_PUSHBUTTON, (LONG)TRUE);
	SendDlgItemMessage(IDOK, BM_SETSTYLE, BS_DEFPUSHBUTTON, (LONG)TRUE);
//	DbgOut("	2007-07-13 by cmkwon, 222\r\n");	// 2007-07-13 by cmkwon, 테스트 
}

void CAtumLauncherDlg::SetPrivateIP()
{
	// 자신의 IP Address를 구한다
	char	host[100];
	HOSTENT	*p;
	char	ip[SIZE_MAX_IPADDRESS];

	gethostname(host, 100);
	if(p = gethostbyname(host))
	{
		sprintf(ip, "%d.%d.%d.%d", (BYTE)p->h_addr_list[0][0], (BYTE)p->h_addr_list[0][1],(BYTE)p->h_addr_list[0][2], (BYTE)p->h_addr_list[0][3]);
		STRNCPY_MEMSET(m_PrivateIP, ip, SIZE_MAX_IPADDRESS);
	}
}

BOOL CAtumLauncherDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DestroyWindow();
}

void CAtumLauncherDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default


	if (nIDEvent == TIMERID_CONNECT_PRESERVER)
	{
		KillTimer(TIMERID_CONNECT_PRESERVER);

		BOOL ret = ConnectPreServer();

		if (ret)
		{
			// succeeded
			m_SelectFlag = TRUE;
//			m_nServerGroupReg = SERVER_DECA;
			m_nServer = SERVER_DECA;

//ysw : 삭제
			// Set default radio button
/*			int ServerGroup = IDC_DECA_SERVER;
			if ( m_nServerGroupReg == SERVER_DECA )
			{
				ServerGroup = IDC_DECA_SERVER;
			}
			else if ( m_nServerGroupReg == SERVER_BATTALUS )
			{
				ServerGroup = IDC_BATTALUS_SERVER;
			}
			else if ( m_nServerGroupReg == SERVER_SHARRINE )
			{
				ServerGroup = IDC_SHARRINE_SERVER;
			}
			else if ( m_nServerGroupReg == SERVER_PHILON )
			{
				ServerGroup = IDC_PHILON_SERVER;
			}
*/
		#ifdef _DEBUG
//			CButton* pBtn= (CButton* )GetDlgItem(ServerGroup);
//			pBtn->SetCheck( TRUE );
		#else
//			CButton* pBtn= (CButton* )GetDlgItem(IDC_BATTALUS_SERVER);
//			pBtn->SetCheck( TRUE );

		//ysw : 삭제
//			GetDlgItem(IDC_DECA_SERVER)->EnableWindow(FALSE);
//			GetDlgItem(IDC_BATTALUS_SERVER)->EnableWindow(FALSE);
//			GetDlgItem(IDC_SHARRINE_SERVER)->EnableWindow(FALSE);
//			GetDlgItem(IDC_PHILON_SERVER)->EnableWindow(FALSE);
		#endif

			CString txt = m_CurrentVersion.GetVersionString();
			SetFileNumText((char*)(LPCTSTR)("VER " + txt));

			// set account name
// 2005-08-22 by cmkwon, for jpn alpha test
// 2007-03-06 by cmkwon, for debug
//#if defined(_DEBUG) || (  !defined(_MASANG15_SERVER) && (!defined(_MASANG51_SERVER)) && (!defined(_GLOBAL_ENG_SERVER)) && (!defined(_VTC_VIET_SERVER)) && (!defined(_KOREA_SERVER_2))  )

// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 아래와 같이 수정 함
// #if defined(_DEBUG)
// 			GetDlgItem(IDC_EDIT_ACCOUNT)->SetWindowText(m_szAccountNameReg);
// 			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(m_szPasswordReg);
// 			((CEdit*)GetDlgItem(IDC_EDIT_ACCOUNT))->SetSel(0, -1);
// #endif

#if defined(_DEBUG)
			// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 
			GetDlgItem(IDC_EDIT_ACCOUNT)->SetWindowText(m_szAccountNameReg);
			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText(m_szPasswordReg);

#else
			// 2008-06-17 by cmkwon, WinnerOnline_Tha Launcher에서 이전 접속 계정 기억하기(K0000243) - 저장된 AccountName 을 설정한다.
			GetDlgItem(IDC_EDIT_ACCOUNT)->SetWindowText(m_szAccountNameReg);
#endif

		}
		else
		{
			// failed
			EndDialog(-1);
			return;
		}
	}
	else if (nIDEvent == TIMERID_ENABLE_CONTROL)
	{
		KillTimer(nIDEvent);
		EnableControls();
	}
	else if(nIDEvent == TIMERID_SEND_ALIVE_PACKET)
	{
		if(m_pUpdateWinsocket
			&& m_pUpdateWinsocket->IsConnected())
		{
			m_pUpdateWinsocket->WriteMessageType(T_PC_CONNECT_ALIVE);
		}
	}
	else if(TIMERID_NETWORK_STATE_CHECK == nIDEvent)
	{// 2007-06-18 by cmkwon, 네트워크 상태 체크

		DWORD dwPingGapTick = 0;
		if(this->NTCheckTimeOver(&dwPingGapTick))
		{
			// 2007-06-18 by cmkwon, 네트워크 상태 체크 - 화면에 표시
			m_ServerList->UpdateNetworkState(this->NTGetPingAverageTime());		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
			m_ServerList->Invalidate();											// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
		}
	}

	CDialog::OnTimer(nIDEvent);
}


//ysw : 추가
void CAtumLauncherDlg::OnJoin()
{
	// TODO: Add your control notification handler code here
	CString strTmp;
	strTmp.Format("%s/%s", STRMSG_S_GAMEHOMEPAGE_DOMAIN, URL_REGISTER_PAGE);
	ShellExecute(NULL, "open", strTmp, NULL, NULL, SW_SHOWNORMAL);
}

void CAtumLauncherDlg::OnSecret()
{
	// TODO: Add your control notification handler code here
	WinExec("explorer.exe http://www.atumonline.com/7_members/login.asp ", SW_SHOW);
}

void CAtumLauncherDlg::OnEnd()
{
	// TODO: Add your control notification handler code here
	CDialog::DestroyWindow();
}

void CAtumLauncherDlg::OnDeca()
{
	// TODO: Add your control notification handler code here
	m_SelectFlag = TRUE;
	m_nServer = SERVER_DECA;
}

void CAtumLauncherDlg::OnBattal()
{
	// TODO: Add your control notification handler code here

}

void CAtumLauncherDlg::OnSharin()
{
	// TODO: Add your control notification handler code here

}

void CAtumLauncherDlg::OnPhilon()
{
	// TODO: Add your control notification handler code here

}

#define CUR_BAR_X		170
#define CUR_BAR_Y		41
#define CUR_BAR_WIDTH	258
#define CUR_BAR_HEIGHT	20

/*--------------------------------------------------------------------
	 DrawProgressBar
--------------------------------------------------------------------*/
int CAtumLauncherDlg::DrawProgressBar()
{
	CClientDC	dc(this);
	CDC			MemDC;
	CBitmap		* pOldBmp;
	CBrush		BlueBrush(RGB(0, 84, 166));
	CBrush		GrayBrush(RGB(109, 207, 246));

	CRect		ProRect;
	CBitmap		Percent;

	char		 Junk[10];
	TEXTMETRIC	 TextMetric;

	MemDC.CreateCompatibleDC(&dc);
	Percent.CreateCompatibleBitmap(&dc, CUR_BAR_WIDTH, CUR_BAR_HEIGHT);
	pOldBmp = (CBitmap *)MemDC.SelectObject(&Percent);

	ProRect.SetRect(0, 0, CUR_BAR_WIDTH, CUR_BAR_HEIGHT);
	MemDC.FillRect(&ProRect, &GrayBrush);
	ProRect.SetRect(0, 0, m_Cur_Rect , CUR_BAR_HEIGHT);
	MemDC.FillRect(&ProRect, &BlueBrush);

	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(0, 0, 0));
	MemDC.GetTextMetrics(&TextMetric);

	wsprintf(Junk, "%d%%", m_Cur_Percent);
	MemDC.TextOut(CUR_BAR_WIDTH/2 - (TextMetric.tmAveCharWidth * lstrlen(Junk)/2),
				CUR_BAR_HEIGHT/2 - (TextMetric.tmHeight/2), (LPCSTR)Junk, lstrlen(Junk));

	dc.BitBlt(CUR_BAR_X, CUR_BAR_Y, CUR_BAR_WIDTH, CUR_BAR_HEIGHT, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(&pOldBmp);

	return	true;
}


/*----------------------------------------------------------------------------*
*			Set_Cur_Percent
*-----------------------------------------------------------------------------*/
void CAtumLauncherDlg::Set_Cur_Percent(DWORD CurSize)
{
	m_Cur_Percent = CurSize;
	if(m_Cur_Percent > 100) m_Cur_Percent = 100;

	m_Cur_Rect = (int)( (m_Cur_Percent*CUR_BAR_WIDTH)/100 );
}

void CAtumLauncherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);

	// fake windows into thinking your clicking on the caption, does not
	// maximizeon double click
	
	// 마우스로 끌어서 윈도우 이동
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

//	if(m_FTPUpdateState == UPDATE_STATE_DOWNLOADING)
//	{
//		AtumMessageBox("현재 최신 업데이트가 진행 중 입니다.\n\n업데이트 완료 후 시작 버튼을 클릭해 주세요.");
//	}
}


UINT CAtumLauncherDlg::OnNcHitTest(CPoint point)
{
     UINT nHitTest = CDialog::OnNcHitTest( point );
     // also fake windows out, but this maximizes the window
     // when you double click on it.
     return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
}


HBRUSH CAtumLauncherDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor)
	{
	case CTLCOLOR_EDIT:
		{		
			if((GetDlgItem(IDC_EDIT_ACCOUNT)->m_hWnd == pWnd->m_hWnd)
				|| (GetDlgItem(IDC_EDIT_PASSWORD)->m_hWnd == pWnd->m_hWnd))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(0, 0, 0));
				pDC->SetTextColor(RGB(255, 255, 255));
				return m_StaticBrushBlack;
			}
		}
		break;
		
	case CTLCOLOR_STATIC:
		{
			if(GetDlgItem(IDC_FILE_INFO)->m_hWnd == pWnd->m_hWnd)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(EXE2_BG_UPDATE_INFO_FONT_COLOR);		
				return (HBRUSH)GetStockObject(NULL_BRUSH);
			}
			else if(GetDlgItem(IDC_DOWNLOAD_FILENUM)->m_hWnd == pWnd->m_hWnd)
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(EXE2_BG_DOWNLOAD_FILE_FONT_COLOR);		
				return (HBRUSH)GetStockObject(NULL_BRUSH);
			}
			else if(GetDlgItem(IDC_CHARACTER_NAME)->m_hWnd == pWnd->m_hWnd )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(0, 0, 0));
				pDC->SetTextColor(RGB(255, 255, 255));		
				return m_StaticBrushGray;
			}
		}
		break;
	case CTLCOLOR_LISTBOX:
		{
			if(GetDlgItem(IDC_LIST)->m_hWnd == pWnd->m_hWnd )
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetBkColor(RGB(29, 29, 40));
				pDC->SetTextColor(RGB(189,194,198));
				return m_listBrush;
			}
		}
		break;
	default:
		{			
		}
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
//void CAtumLauncherDlg::ExecGame(char *cmdLine)
void CAtumLauncherDlg::ExecGame(char *cmdLine, char *i_szAppPath/*=NULL*/, char *i_szCmdParam/*=NULL*/)
{
#ifdef _ATUM_DEVELOP
	SetCurrentDirectory(m_szExecutePathReg);
#endif

// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
	m_pVTCGuard->Run(i_szAppPath, i_szCmdParam);

	return ;
#endif	// SERVICE_TYPE_VIETNAMESE_SERVER_1
// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

// start 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]
#ifdef S_HACK_SHIELD_AUTO_UPDATE_HSKIM
	// 2012-09-13 by jhseol, 캐나다 핵쉴드 제거
#ifdef S_HACK_SHIELD_REMOVE_JHSEOL
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "CANADA_HACK_SHIELD");
	if( NULL == hMutex )
	{
	DWORD dwRet = 0;
	char szFullFilePath[MAX_PATH]={ 0, };
	char szFolderGamePath[MAX_PATH]={ 0, };
	char szHackShieldPath[MAX_PATH]={ 0, };
	char szDir[_MAX_DIR] = { 0, };
	char szDrive[_MAX_DRIVE] = { 0, };
	AHNHS_EXT_ERRORINFO HsExtError = { 0, };

	ZeroMemory(&HsExtError,sizeof(HsExtError));

	strncpy(HsExtError.szServer, GAME_GUARD_MONITORING_SERVER_IP, MAX_PATH);
	strncpy(HsExtError.szUserId, m_szAccountName, MAX_PATH);
	strncpy(HsExtError.szGameVersion, m_CurrentVersion.GetVersionString(), SIZE_MAX_VERSION_STRING);

	::GetModuleFileName(NULL, szFullFilePath, MAX_PATH);

	_splitpath(szFullFilePath, szDrive, szDir, NULL, NULL);
	_makepath(szFolderGamePath, szDrive, szDir, NULL, NULL);

	sprintf(szHackShieldPath, "%s\\HShield", szFolderGamePath);

	dwRet = _AhnHS_HSUpdateEx(
		szHackShieldPath,
		1000 * 600,		// 600초 = 10 분 
		HACKSHIELD_GAME_CODE,
		AHNHSUPDATE_CHKOPT_HOSTFILE | AHNHSUPDATE_CHKOPT_GAMECODE, 
		HsExtError,
		1000* 20);		// 20초

	if ( dwRet != ERROR_SUCCESS) 
	{ 
		AtumMessageBox(STRERR_S_ATUMLAUNCHER_0035);

		return ;
	}
	}
#endif	// #ifdef S_HACK_SHIELD_REMOVE_JHSEOL
	// end 2012-09-13 by jhseol, 캐나다 핵쉴드 제거
#endif
// end 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]

	UINT ret = WinExec(cmdLine, SW_SHOWNORMAL);
	DbgOut("EXE 2 CommandLine : %s, RetCode(%d)\r\n", cmdLine, ret);		// 2007-05-16 by cmkwon
	if ( ret <= 31 )	// exec failed...
	{
		switch (ret)
		{
		case 0:						// The system is out of memory or resources.
			AtumMessageBox(STRERR_S_ATUMLAUNCHER_0031);
			break;
		case ERROR_BAD_FORMAT:		// The .exe file is invalid.
			AtumMessageBox(STRERR_S_ATUMLAUNCHER_0032);
			break;
		case ERROR_FILE_NOT_FOUND:	// The specified file was not found.
#ifdef _ATUM_DEVELOP
			AtumMessageBox(CString("File not Found!\nbin: ") + cmdLine
				+ "\npath: " + m_szExecutePathReg);
#else
			AtumMessageBox(STRERR_S_ATUMLAUNCHER_0033);
#endif
			break;
		case ERROR_PATH_NOT_FOUND:	// The specified path was not found.
			AtumMessageBox(STRERR_S_ATUMLAUNCHER_0034);
			break;
		default:
			break;
		}
	}
}

void CAtumLauncherDlg::ExecGameCrocess(char *cmdLine)
{
	DWORD dwExitCode;
	PROCESS_INFORMATION pi;

	STARTUPINFO si = {sizeof(si)};
	ZeroMemory(&si,sizeof(si));

	CreateProcess(
		NULL,				// name of executable module
		cmdLine,			// command line string
		NULL,				//LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
		0,					//BOOL bInheritHandles,                      // handle inheritance option
		0,					//DWORD dwCreationFlags,                     // creation flags
		NULL,				//LPVOID lpEnvironment,                      // new environment block
		NULL,				//LPCTSTR lpCurrentDirectory,                // current directory name
		&si,				//LPSTARTUPINFO lpStartupInfo,               // startup information
		&pi					//LPPROCESS_INFORMATION lpProcessInformation // process information
		);


	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_OBJECT_0) {
		// Process 종료
		//AtumMessageBox("Process Terminated!");
	}

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );
}

BOOL CAtumLauncherDlg::ProcessDeleteFileList(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo)
{
#ifdef _DEBUG
	VersionInfo DVersion(pUpdateInfo->NewDeleteFileListVersion);
	DBGOUT(STRMSG_S_ATUMLAUNCHER_0002, DVersion.GetVersionString());
#endif

	HINTERNET hInternet;
	HINTERNET hFtpConnect;
	HINTERNET hDir;
	HINTERNET hDelFile;
	WIN32_FIND_DATA pDirInfo;
	int nFileSize;

	hInternet = InternetOpen("Atum Pre Server", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		// check: error
		// 2005-01-13 by cmkwon, DeleteFile, NoticeFile FTP 연결 관련 오류는 메시지박스 삭제
		//AtumMessageBox("InternetOpen ERROR!");
		DbgOut("InternetOpen ERROR: %d", GetLastError());
		return FALSE;
	}

	DbgOut("Updating delete file list(%s) From %s:%d \r\n", pUpdateInfo->DeleteFileListDownloadPath, pUpdateInfo->FtpIP, pUpdateInfo->FtpPort);

	hFtpConnect = InternetConnect(hInternet, pUpdateInfo->FtpIP, pUpdateInfo->FtpPort, pUpdateInfo->FtpAccountName, pUpdateInfo->FtpPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpConnect == NULL)
	{
		// check: error
		// 2005-01-13 by cmkwon, DeleteFile, NoticeFile FTP 연결 관련 오류는 메시지박스 삭제
		//AtumMessageBox("InternetConnect ERROR!");
		DbgOut("InternetConnect ERROR: %d\r\n", GetLastError());
		return FALSE;
	}

	if ( !(hDir = FtpFindFirstFile (hFtpConnect, pUpdateInfo->DeleteFileListDownloadPath, &pDirInfo, 0, 0) ) )
	{
		DWORD dwLastErr = GetLastError();
		if (dwLastErr == ERROR_NO_MORE_FILES)
		{
			DbgOut("No more files here\r\n");
			return FALSE;
		}
		else if (dwLastErr == ERROR_INTERNET_EXTENDED_ERROR)
		{
			DWORD dwError;
			TCHAR szErr[2048]; DWORD dwErrLen = 2048;
			if (InternetGetLastResponseInfo(&dwError, szErr, &dwErrLen))
			{
				CString szErrStr;
				szErrStr.Format("DFile Error: %s\r\n", szErr);
				// 2005-01-13 by cmkwon, DeleteFile, NoticeFile FTP 연결 관련 오류는 메시지박스 삭제
				//AtumMessageBox(szErrStr);
				DbgOut("%s\n", szErrStr);
			}
			return FALSE;
		}
		else
		{
			// check: error
			CString szErrStr;
			szErrStr.Format("DFile Error: %d", GetLastError());
			// 2005-01-13 by cmkwon, DeleteFile, NoticeFile FTP 연결 관련 오류는 메시지박스 삭제
			//AtumMessageBox(szErrStr);
			DbgOut("%s\n", szErrStr);
			return FALSE;
		}
	}
	else
	{
		nFileSize = pDirInfo.nFileSizeLow;
	}

	// GUI 설정
	SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0003);
	m_progressCtrl.SetRange32(0, nFileSize);

	////////////////////////////
	// file buffer
	string delFileBuffer = "";

	////////////////////////////
	// open remote file
	hDelFile = FtpOpenFile(hFtpConnect, pUpdateInfo->DeleteFileListDownloadPath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
	char *buffer = new char[DOWNLOAD_BUFFER_SIZE];
	DWORD amount_read = DOWNLOAD_BUFFER_SIZE;
	UINT total_read = 0;

	while (TRUE)
	{
		if (!InternetReadFile (hDelFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read))
		{
			// error
			// 2005-01-13 by cmkwon, DeleteFile, NoticeFile FTP 연결 관련 오류는 메시지박스 삭제
			//AtumMessageBox("Reading file failed");
			return FALSE;
		}

		if(0 == amount_read)
		{// 2006-06-30 by cmkwon
			break;
		}

		delFileBuffer.append(buffer, amount_read);

		total_read += amount_read;
		m_progressCtrl.SetPos(total_read);
	}

	delFileBuffer += "\r\n";

	///////////////////////////
	// 처리
	istrstream isDel(delFileBuffer.c_str());
	char delFileLine[SIZE_MAX_FTP_FILE_PATH];
	int isOffset = 0;
	while(isOffset < nFileSize)
	{
		if (isDel.getline(delFileLine, SIZE_MAX_FTP_FILE_PATH).eof())
		{
			break;
		}

		isOffset += (strlen(delFileLine) + 1);

		// 0x0D를 제거하지 못한 경우 제거함
		if (delFileLine[strlen(delFileLine)-1] == 0x0D)
		{
			delFileLine[strlen(delFileLine)-1] = '\0';
		}

		if (delFileLine[0] == '#')
		{
			// comment임
			continue;
		}

#ifdef _DEBUG
		char	szDir[512];
		GetCurrentDirectory(512, szDir);
#endif

		DeleteFile(delFileLine);
	}

	// delete buffer
	delete buffer;

	// close resources
	InternetCloseHandle(hDelFile);
	InternetCloseHandle(hDir);
	InternetCloseHandle(hFtpConnect);
	InternetCloseHandle(hInternet);

	return TRUE;
}

BOOL CAtumLauncherDlg::ProcessNoticeFile(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo)
{
	BOOL bRet = FALSE;
	CFTPManager noticeFTPManager;

	DbgOut("Updating notice file(%s) From %s:%d \r\n", pUpdateInfo->NoticeFileDownloadPath, pUpdateInfo->FtpIP, pUpdateInfo->FtpPort);

	bRet = noticeFTPManager.ConnectToServer(pUpdateInfo->FtpIP, pUpdateInfo->FtpPort, pUpdateInfo->FtpAccountName, pUpdateInfo->FtpPassword);
	if (!bRet)
	{
		return FALSE;
	}

	// Find file info
	WIN32_FIND_DATA fileInfo;
	HINTERNET hFile = noticeFTPManager.GetFileInfo(pUpdateInfo->NoticeFileDownloadPath, &fileInfo);
	if (hFile == NULL)
	{
		return FALSE;
	}

	// set file length
	int nFileSize = fileInfo.nFileSizeLow;

	// GUI 설정
	SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0004);
	m_progressCtrl.SetRange32(0, nFileSize);

	// 다운로드
	bRet = noticeFTPManager.DownloadFile(pUpdateInfo->NoticeFileDownloadPath, "notice.txt", &m_progressCtrl);
	if (!bRet)
	{
		return FALSE;
	}

	noticeFTPManager.CloseConnection();

	return TRUE;
}

void CAtumLauncherDlg::OnSelchangeList()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	int nSel = m_ctrlServerList.GetCurSel();
	if(m_ctrlServerList.GetServerCheck(nSel))
	{
		m_ctrlServerList.SetCurSel(nSel);
		m_nOldSel = nSel;
	}
	else
	{
		m_ctrlServerList.SetCurSel(m_nOldSel);
	}
}

void CAtumLauncherDlg::OnCan()
{
	// TODO: Add your control notification handler code here

	if(UPDATE_STATE_DOWNLOADING == m_FTPUpdateState
		|| UPDATE_STATE_DOWNLOADED == m_FTPUpdateState)
	{
		if(m_pUpdateFTPManager)
		{
			m_pUpdateFTPManager->m_bDownloadThreadCancelFlag = TRUE;
			if(m_pUpdateFTPManager->m_hDownloadThread)
			{
				DWORD dwRet;
				dwRet = WaitForSingleObject(m_pUpdateFTPManager->m_hDownloadThread, INFINITE);
				if(WAIT_OBJECT_0 != dwRet)
				{
					// 리턴이 WAIT_FAILED임
					int nError = GetLastError();
					SetLastError(0);
				}
				CloseHandle(m_pUpdateFTPManager->m_hDownloadThread);
				m_pUpdateFTPManager->m_hDownloadThread = NULL;				
				Sleep(100);
			}
			SAFE_DELETE(m_pHost);
			delete m_pUpdateFTPManager;
			m_pUpdateFTPManager = NULL;
			this->SetFTPUpdateState(UPDATE_STATE_INIT);
		}

		if(m_pHttpManager)
		{// 2007-01-08 by cmkwon
			m_pHttpManager->ThreadEnd(1000);
			SAFE_DELETE(m_pHttpManager);
			this->SetFTPUpdateState(UPDATE_STATE_INIT);
		}

		VersionInfo OldVersion(m_msg_PC_CONNECT_UPDATE_INFO.OldVersion);
		VersionInfo UpdateVersion(m_msg_PC_CONNECT_UPDATE_INFO.UpdateVersion);
		char szUpdateFileName[256];
		sprintf(szUpdateFileName, "%s_%s.zip", OldVersion.GetVersionString(),
			UpdateVersion.GetVersionString());
		DeleteFile(szUpdateFileName);
	}
	CDialog::OnCancel();
}

BOOL CAtumLauncherDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (m_bControlEnabled)
		{
			OnOk();
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}



LONG CAtumLauncherDlg::OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam)
{
	if(UPDATE_STATE_DOWNLOADING != m_FTPUpdateState){		return FALSE;}

	char szErrString[1024];
	char szErrAddStr[1024];
	MEMSET_ZERO(szErrString, 1024);
	MEMSET_ZERO(szErrAddStr, 1024);
// 2007-01-05 by cmkwon, 아래와 같이 ERR_XXX로 수정함
//	switch(wParam)
//	{
//	case DOWNLOAD_ERR_FTP_CONNECT:
//		{
//			strcpy(szErrAddStr, STRERR_S_ATUMEXE_0006);
//		}
//		break;
//	case DOWNLOAD_ERR_FTP_OPENFILE:
//		{
//			strcpy(szErrAddStr, STR_DOWNLOAD_ERR_FTPCONNECT);
//		}
//		break;
//	case DOWNLOAD_ERR_CREATE_LOCAL_FILE:
//		{
//			strcpy(szErrAddStr, STR_DOWNLOAD_ERR_CREATE_LOCAL_FILE);
//		}
//		break;
//	case DOWNLOAD_ERR_READ_REMOTE_FILE:
//		{
//			strcpy(szErrAddStr, STR_DOWNLOAD_ERR_READ_REMOTE_FILE);
//		}
//		break;
//	default:
//		{
//			wsprintf(szErrAddStr, "Nomal download error(%d) !!", wParam);
//		}
//	}
	switch(wParam)
	{
	case ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER:
		{
			strcpy(szErrAddStr, STRERR_S_ATUMEXE_0006);
		}
		break;
	case ERR_LOCAL_FILE_CREATE_FAIL:
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0001);
		}
		break;
	case ERR_UPDATE_FILE_NOT_FOUND:
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0000);
		}
		break;
	case ERR_UPDATE_FILE_DOWNLOADING_FAIL:	
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0002);
		}
		break;
	default:
		{
			wsprintf(szErrAddStr, "Nomal download error(%d) !!", wParam);
		}
	}

	wsprintf(szErrString, STRMSG_060526_0001
		, STRMSG_S_GAMEHOMEPAGE_DOMAIN, szErrAddStr);
	AtumMessageBox(szErrString);
	OnCancel();
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			LONG CAtumLauncherDlg::OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-08 ~ 2007-01-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CAtumLauncherDlg::OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam)
{
	m_progressCtrl.SetRange32(0, wParam);
	m_progressCtrl.SetPos(0);
	return TRUE;
}

LONG CAtumLauncherDlg::OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam)
{
	if(UPDATE_STATE_DOWNLOADING != m_FTPUpdateState){		return FALSE;}

	m_progressCtrl.SetPos(wParam);
	return TRUE;
}

LONG CAtumLauncherDlg::OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam)
{
	if(UPDATE_STATE_DOWNLOADING != m_FTPUpdateState){		return FALSE;}
	this->SetFTPUpdateState(UPDATE_STATE_DOWNLOADED);

	if(m_pUpdateFTPManager)
	{
		if(m_pUpdateFTPManager->m_hDownloadThread)
		{
			DWORD dwRet;
			dwRet = WaitForSingleObject(m_pUpdateFTPManager->m_hDownloadThread, 1000);
			if(WAIT_OBJECT_0 != dwRet)
			{
				// 리턴이 WAIT_FAILED임
				int nError = GetLastError();
				SetLastError(0);
			}
			CloseHandle(m_pUpdateFTPManager->m_hDownloadThread);
			m_pUpdateFTPManager->m_hDownloadThread = NULL;

			Sleep(100);
		}

		delete m_pUpdateFTPManager;
		m_pUpdateFTPManager = NULL;
		this->SetFTPUpdateState(UPDATE_STATE_INIT);
	}
	if(m_pHttpManager)
	{
		m_pHttpManager->ThreadEnd(1000);
		SAFE_DELETE(m_pHttpManager);		
		this->SetFTPUpdateState(UPDATE_STATE_INIT);
	}

	// extract update file
	ExtractUpdateFile(&m_msg_PC_CONNECT_UPDATE_INFO);
	
	// update completed
	VersionInfo OldVersion(m_msg_PC_CONNECT_UPDATE_INFO.OldVersion);
	VersionInfo UpdateVersion(m_msg_PC_CONNECT_UPDATE_INFO.UpdateVersion);
	char strBuffer[256];
	sprintf(strBuffer, STRMSG_S_ATUMLAUNCHER_0005, OldVersion.GetVersionString(),
		UpdateVersion.GetVersionString());
	SetProgressGroupText(strBuffer);
	
	// set current cliet version
	m_CurrentVersion = UpdateVersion;
	
	// On DownLoadGamefilesDone
	OnDownLoadGamefilesDone(NULL, NULL);

	memset(&m_msg_PC_CONNECT_UPDATE_INFO, 0x00, sizeof(MSG_PC_CONNECT_UPDATE_INFO));

	return TRUE;
}
// start 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
LONG CAtumLauncherDlg::OnUpdateVTCGuardOK(WPARAM wParam, LPARAM lParam)
{
	SetProgressGroupText(STRMSG_S_ATUMLAUNCHER_0015);

	m_bProcessingVersionUpdate = TRUE;
	EnableControls();
	
	return TRUE;
}
// end 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

void CAtumLauncherDlg::OnMin() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_MINIMIZE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLauncherDlg::OnBtnHomepage() 
/// \brief		// 2007-09-27 by cmkwon, Homepage가기 버튼 추가(베트남 VTC-Intecom 요청) - 
/// \author		cmkwon
/// \date		2007-09-27 ~ 2007-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLauncherDlg::OnBtnHomepage() 
{
	// TODO: Add your control notification handler code here

	ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CAtumLauncherDlg::GetPublicLocalIP(char *o_szLocalIP)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-10 ~ 2006-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CAtumLauncherDlg::GetPublicLocalIP(char *o_szLocalIP)
{
	GGetLocalIP(o_szLocalIP, IP_TYPE_PUBLIC);
	return o_szLocalIP;
}

void CAtumLauncherDlg::OnSetfocusEditAccount() 
{
	// TODO: Add your control notification handler code here
	if(NULL == m_pScreenKeyboardDlg)
	{
		return;
	}

	m_pInputEditFromScreenKeyboard = GetDlgItem(IDC_EDIT_ACCOUNT);	
}

void CAtumLauncherDlg::OnSetfocusEditPassword() 
{
	// TODO: Add your control notification handler code here
	
	if(NULL == m_pScreenKeyboardDlg)
	{
		return;
	}

	m_pInputEditFromScreenKeyboard = GetDlgItem(IDC_EDIT_PASSWORD);	

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-18 by cmkwon, 화상키보드 수정 - 화상키보드 윈도우에서 Hide 시킬경우 처리를 위해
	if(FALSE == m_bHideScreenKeyboardByScreenKeyboardWindow)
	{
		// 2007-09-18 by cmkwon, 화상키보드 수정 - 패스워드 입력시에만 활성화 된다.
		m_pScreenKeyboardDlg->ShowWindow(SW_SHOW);
	}
	m_bHideScreenKeyboardByScreenKeyboardWindow	= FALSE;
}

void CAtumLauncherDlg::OnBtnViewScreenKeyboard() 
{
	// TODO: Add your control notification handler code here

	if(NULL == m_pScreenKeyboardDlg)
	{
		return;
	}
	
	// 2007-09-11 by cmkwon, 베트남 화면키보드 구현 - 화면 키보드를 보여준다
	m_pScreenKeyboardDlg->ShowWindow(SW_SHOW);
}

void CAtumLauncherDlg::OnCheckWindowsMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_WINDOW_DEGREE_LAUNCHER);
	if(NULL == pComboBox)
	{
		return;
	}

	CString csBeforeWDegree;
	pComboBox->GetWindowText(csBeforeWDegree);

	this->InsertWindowDegreeList(pComboBox, m_ctlbWindowMode);



	int nIdx = this->FindWindowDegreeComboBoxIndex(pComboBox, (LPSTR)(LPCSTR)csBeforeWDegree);
	nIdx = max(0, nIdx);
	pComboBox->SetCurSel(nIdx);
}

BOOL CAtumLauncherDlg::BitmapRgn( LPCTSTR resource, COLORREF TansColor ,int nx,int ny)
{
	HBITMAP			m_hBack;
	HINSTANCE hInstance = AfxGetInstanceHandle();
	
	HANDLE handle = ::LoadImage( hInstance, resource, IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADFROMFILE );
   	
	if ( !handle ) return FALSE;
	
	m_hBack = (HBITMAP)handle;
	::SetWindowRgn( m_hWnd, BitmapToRegion( m_hBack, TansColor, TansColor, nx,ny) ,TRUE);
	
	return TRUE;
}

BOOL CAtumLauncherDlg::BitmapRgn( UINT resource, COLORREF TansColor,int nx,int ny )
{
	HBITMAP			m_hBack;
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_hBack = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(resource));
	::SetWindowRgn( m_hWnd, BitmapToRegion( m_hBack, TansColor, TansColor , nx,ny),TRUE);
	
	return TRUE;
}
HRGN CAtumLauncherDlg::BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/,int nx,int ny)
{
	HRGN hRgn = NULL;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, nx, ny, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
					#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}

	return hRgn;
}