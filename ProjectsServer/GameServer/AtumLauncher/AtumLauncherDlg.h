// AtumLauncherDlg.h : header file
//

#if !defined(AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_)
#define AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateWinSocket.h"
#include "FtpDownload.h"
#include "resource.h"
#include "FTP\ProgressCtrlX.h"
#include "FTP\KbcButton.h"
#include "ListBoxEBX.h"
#include "comboboxebx.h"
#include "IExplore.h"



// 2008-12-17 by ckPark 러시아 런쳐
#include "MyEdit.h"
#include "MyCheck.h"
#include "MyComboBox.h"
// end 2008-12-17 by ckPark 러시아 런쳐




///////////////////////////////////////////////////////////////////////////////
// 2005-08-22 by cmkwon
//	MGame 본섭 PreServer			: 218.38.136.192
//	MGame 테섭 PreServer			: 218.38.136.201
//	MasangSoft 내부(엠게임용) 테섭	: 121.134.114.145		// 2007-01-03 by cmkwon
//	MasangSoft 내부(일본_JPN)		: 121.134.114.144		// 2007-01-03 by cmkwon
//	MasangSoft 내부(개발_Kor)		: 121.134.114.140		// 2007-01-03 by cmkwon

// 2007-02-13 by cmkwon, LocalizationDefineCommon.h로 이동함
//
//// Main PreServer IPs
//#define MAIN_PRE_SERVER_IP_0			218
//#define MAIN_PRE_SERVER_IP_1			38
//#define MAIN_PRE_SERVER_IP_2			136
//#define MAIN_PRE_SERVER_IP_3			192
//#define REGISTRY_BASE_PATH				"SpaceCowboy"
//#define CLIENT_EXEUTE_FILE_NAME			"SpaceCowboy.atm"
//#define URL_REGISTER_PAGE				"Sign_up.htm"
//#define	SG_BOX_FONT_FACENAME			"Times New Roman"			// 2007-02-12 by cmkwon, 글씨체
//#define	SG_BOX_FONT_CHARSET				ANSI_CHARSET				// 2007-02-12 by cmkwon, 캐릭터셋
//#define	SG_BOX_FONT_WEIGHT				FW_NORMAL					// 2007-02-12 by cmkwon, 글씨체 두깨
//#ifdef _MGAME_TEST_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//	
//	#define MAIN_PRE_SERVER_IP_0		218
//	#define MAIN_PRE_SERVER_IP_1		38
//	#define MAIN_PRE_SERVER_IP_2		136
//	#define MAIN_PRE_SERVER_IP_3		201
//	#define REGISTRY_BASE_PATH				"SpaceCowboy_Test"
//	#define CLIENT_EXEUTE_FILE_NAME			"SpaceCowboy_Test.atm"
//	#define URL_REGISTER_PAGE				"Sign_up.htm"
//#endif
//#ifdef _MASANG15_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//
//	// 2007-01-03 by cmkwon, 121.134.114.144
//	#define MAIN_PRE_SERVER_IP_0		121
//	#define MAIN_PRE_SERVER_IP_1		134
//	#define MAIN_PRE_SERVER_IP_2		114
//	#define MAIN_PRE_SERVER_IP_3		144
//	#define REGISTRY_BASE_PATH				"SpaceCowboy"
//	#define CLIENT_EXEUTE_FILE_NAME			"SpaceCowboy.atm"
//	#define URL_REGISTER_PAGE				"Sign_up.htm"
//#endif
//#ifdef _MASANG51_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//	#undef SG_BOX_FONT_FACENAME				// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_CHARSET					// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_WEIGHT					// 2007-02-12 by cmkwon	
//
//	#define	SG_BOX_FONT_FACENAME					"굴림"					// 2007-02-12 by cmkwon, 글씨체
//	#define	SG_BOX_FONT_CHARSET					ANSI_CHARSET			// 2007-02-12 by cmkwon, 캐릭터셋
//	#define	SG_BOX_FONT_WEIGHT						FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
//
//	// 2007-01-03 by cmkwon, 121.134.114.140
//	#define MAIN_PRE_SERVER_IP_0		121
//	#define MAIN_PRE_SERVER_IP_1		134
//	#define MAIN_PRE_SERVER_IP_2		114
//	#define MAIN_PRE_SERVER_IP_3		140
//	#define REGISTRY_BASE_PATH				"SpaceCowboy(Masang51)"
//	#define CLIENT_EXEUTE_FILE_NAME			"SpaceCowboy.atm"
//	#define URL_REGISTER_PAGE				"Sign_up.htm"
//#endif
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// 2006-06-12 by cmkwon
//#ifdef _GLOBAL_ENG_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//	#undef SG_BOX_FONT_FACENAME				// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_CHARSET					// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_WEIGHT					// 2007-02-12 by cmkwon	
//
//	#define	SG_BOX_FONT_FACENAME					"Times New Roman"		// 2007-02-12 by cmkwon, 글씨체
//	#define	SG_BOX_FONT_CHARSET					ANSI_CHARSET			// 2007-02-12 by cmkwon, 캐릭터셋
//	#define	SG_BOX_FONT_WEIGHT						FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
//
//#ifdef _TEST_SERVER					// 2006-06-12 by cmkwon, 미국 Gala-Net
//	// 2006-06-12 by cmkwon, 69.90.214.114
//	#define MAIN_PRE_SERVER_IP_0		69
//	#define MAIN_PRE_SERVER_IP_1		90
//	#define MAIN_PRE_SERVER_IP_2		214
//	#define MAIN_PRE_SERVER_IP_3		114
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Eng)_Test"
//	#define CLIENT_EXEUTE_FILE_NAME		"SpaceCowboy_Test.atm"
//	#define URL_REGISTER_PAGE			"reg.asp"				// 2006-04-05 by cmkwon, 수정함
//#else
//	// 2006-05-19 by cmkwon, 69.90.214.106
//	#define MAIN_PRE_SERVER_IP_0		69
//	#define MAIN_PRE_SERVER_IP_1		90
//	#define MAIN_PRE_SERVER_IP_2		214
//	#define MAIN_PRE_SERVER_IP_3		106
//	#define REGISTRY_BASE_PATH				"SpaceCowboy(Eng)"
//	#define CLIENT_EXEUTE_FILE_NAME			"SpaceCowboy.atm"
//	#define URL_REGISTER_PAGE				"sign_up.htm?viewtype=read"
//#endif// end_#ifdef _TEST_SERVER
//#endif// end_#ifdef _GLOBAL_ENG_SERVER
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// 2006-06-12 by cmkwon
//#ifdef _VTC_VIET_SERVER				// 2006-03-23 by cmkwon, 베트남 VTC
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//	#undef SG_BOX_FONT_FACENAME				// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_CHARSET					// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_WEIGHT					// 2007-02-12 by cmkwon	
//
//	#define	SG_BOX_FONT_FACENAME					"Times New Roman"		// 2007-02-12 by cmkwon, 글씨체
//	#define	SG_BOX_FONT_CHARSET					VIETNAMESE_CHARSET		// 2007-02-12 by cmkwon, 캐릭터셋
//	#define	SG_BOX_FONT_WEIGHT						FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
//
//#ifdef _TEST_SERVER					// 2006-03-23 by cmkwon, 베트남 VTC
//	// 2006-06-09 by cmkwon, 210.245.12.5: VTC-Intecom TestServer
//	// 2006-09-08 by cmkwon, 변경(210.245.12.5-->222.255.15.54)
//	#define MAIN_PRE_SERVER_IP_0		222
//	#define MAIN_PRE_SERVER_IP_1		255
//	#define MAIN_PRE_SERVER_IP_2		15
//	#define MAIN_PRE_SERVER_IP_3		54
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Viet)_Test"
//	#define CLIENT_EXEUTE_FILE_NAME		"SpaceCowboy_Test.atm"
//	#define URL_REGISTER_PAGE			"reg.asp"				// 2006-04-05 by cmkwon, 수정함
//#else
//	// 2006-08-17 by cmkwon, 222.255.15.37: VTC-Intecom PreServer
//	#define MAIN_PRE_SERVER_IP_0		222
//	#define MAIN_PRE_SERVER_IP_1		255
//	#define MAIN_PRE_SERVER_IP_2		15
//	#define MAIN_PRE_SERVER_IP_3		37
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Viet)"
//	#define CLIENT_EXEUTE_FILE_NAME		"SpaceCowboy.atm"
//	#define URL_REGISTER_PAGE			"reg.asp"				// 2006-04-05 by cmkwon, 수정함
//#endif// end_#ifdef _VTC_VIET_TEST_SERVER
//#endif
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// 2006-09-05 by cmkwon
//#ifdef _KOREA_SERVER_2
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef CLIENT_EXEUTE_FILE_NAME
//	#undef URL_REGISTER_PAGE
//	#undef SG_BOX_FONT_FACENAME				// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_CHARSET					// 2007-02-12 by cmkwon
//	#undef SG_BOX_FONT_WEIGHT					// 2007-02-12 by cmkwon	
//
//	#define	SG_BOX_FONT_FACENAME					"굴림"					// 2007-02-12 by cmkwon, 글씨체
//	#define	SG_BOX_FONT_CHARSET					ANSI_CHARSET			// 2007-02-12 by cmkwon, 캐릭터셋
//	#define	SG_BOX_FONT_WEIGHT						FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
//
//#ifdef _TEST_SERVER					// 2006-03-23 by cmkwon, 베트남 VTC
//	// 2006-09-05 by cmkwon, 임시로 본섭과 같이 해 놓는다 - 218.145.57.83
//	// 2006-12-22 by cmkwon, 211.189.116.11 <=예당온라인
//	#define MAIN_PRE_SERVER_IP_0		211
//	#define MAIN_PRE_SERVER_IP_1		189
//	#define MAIN_PRE_SERVER_IP_2		116
//	#define MAIN_PRE_SERVER_IP_3		11
//	#define REGISTRY_BASE_PATH			"ACEonline_Test"
//	#define CLIENT_EXEUTE_FILE_NAME		"ACEonline_Test.atm"
//	#define URL_REGISTER_PAGE			"reg.asp"				// 2006-04-05 by cmkwon, 수정함
//#else
//	// 2006-09-05 by cmkwon, 218.145.57.83
//	// 2006-12-22 by cmkwon, 211.189.116.13 <=예당온라인
//	#define MAIN_PRE_SERVER_IP_0		211
//	#define MAIN_PRE_SERVER_IP_1		189
//	#define MAIN_PRE_SERVER_IP_2		116
//	#define MAIN_PRE_SERVER_IP_3		13
//	#define REGISTRY_BASE_PATH			"ACEonline"
//	#define CLIENT_EXEUTE_FILE_NAME		"ACEonline.atm"
//	#define URL_REGISTER_PAGE			"reg.asp"				// 2006-04-05 by cmkwon, 수정함
//#endif// end_#ifdef _KOREA_SERVER_2
//#endif






#define SERVER_DECA			0x00
#define SERVER_BATTALUS		0x01
#define SERVER_SHARRINE		0x02
#define SERVER_PHILON		0x03

#define GAME_MODE_WINDOW		0
#define GAME_MODE_FULLSCREEN	1

// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 필요 없음
//// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 값을 중간에 추가함
//// 2007-07-24 by cmkwon, 런처에서 800*600 해상도 삭제 - 콤보박스 리스트 인데스 필요 없음
////#define WINDOW_DEGREE_800x600_LOW		0
////#define WINDOW_DEGREE_800x600_MEDIUM	1
////#define WINDOW_DEGREE_800x600_HIGH		2
//#define WINDOW_DEGREE_1024x768_LOW		3
//#define WINDOW_DEGREE_1024x768_MEDIUM	4
//#define WINDOW_DEGREE_1024x768_HIGH		5
//#define WINDOW_DEGREE_W1280x720_LOW		6		// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 
//#define WINDOW_DEGREE_W1280x720_MEDIUM	7		// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 
//#define WINDOW_DEGREE_W1280x720_HIGH	8		// 2007-08-23 by cmkwon, Wide 해상도 1280x720(16:9) 추가 - 
//#define WINDOW_DEGREE_W1280x800_LOW		9		// 2007-05-02 by cmkwon, Wide 추가 - 1280x800(16:10)
//#define WINDOW_DEGREE_W1280x800_MEDIUM	10		// 2007-05-02 by cmkwon, Wide 추가
//#define WINDOW_DEGREE_W1280x800_HIGH	11		// 2007-05-02 by cmkwon, Wide 추가
//#define WINDOW_DEGREE_1280x960_LOW		12
//#define WINDOW_DEGREE_1280x960_MEDIUM	13
//#define WINDOW_DEGREE_1280x960_HIGH		14
//#define WINDOW_DEGREE_1280x1024_LOW		15
//#define WINDOW_DEGREE_1280x1024_MEDIUM	16
//#define WINDOW_DEGREE_1280x1024_HIGH	17
//#define WINDOW_DEGREE_W1600x900_LOW		18		// 2007-05-02 by cmkwon, Wide 추가 - 1600x900(16:9)
//#define WINDOW_DEGREE_W1600x900_MEDIUM	19		// 2007-05-02 by cmkwon, Wide 추가
//#define WINDOW_DEGREE_W1600x900_HIGH	20		// 2007-05-02 by cmkwon, Wide 추가
//#define WINDOW_DEGREE_1600x1200_LOW		21
//#define WINDOW_DEGREE_1600x1200_MEDIUM	22
//#define WINDOW_DEGREE_1600x1200_HIGH	23

// 게임 시작 버튼이 눌러졌을 때 control들을 disable시키기 위함
#define TIMERID_CONNECT_PRESERVER		1
#define TIMERID_ENABLE_CONTROL			2
#define TIMERID_SEND_ALIVE_PACKET		3
#define TIMERID_NETWORK_STATE_CHECK		4		// 2007-06-18 by cmkwon, 네트워크 상태 체크

typedef enum
{	
	UPDATE_STATE_INIT			= 0,
	UPDATE_STATE_CONNECTING		= 1,
	UPDATE_STATE_DOWNLOADING	= 2,
	UPDATE_STATE_DOWNLOADED		= 3
} FTP_UPDATE_STATE;

// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 
struct SSERVER_GROUP_FOR_LAUNCHER
{
	int		nIndex;
	char	szServerGroupName[SIZE_MAX_SERVER_NAME];
};
typedef vector<SSERVER_GROUP_FOR_LAUNCHER>		vectSSERVER_GROUP_FOR_LAUNCHER;

typedef vector<DEVMODE>			vectDEVMODE;		// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - 

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherDlg dialog
class CFTPManager;
class CHttpManager;
class CVTCGuardManager;			// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
class CScreenKeyboardDlg;		// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 -
class CAtumLauncherDlg : public CDialog
{
// Construction
public:
	CAtumLauncherDlg(CWnd* pParent = NULL);	// standard constructor

	char *GetPublicLocalIP(char *o_szLocalIP);
	char				m_szLocalIP[SIZE_MAX_IPADDRESS];		// 2006-05-07 by cmkwon


	// 2004-07-05, cmkwon
	CBitmapButton		m_bitmapBtnCancel;
	CBitmapButton		m_bitmapBtnMin;
	CBitmapButton		m_bmpBtnHomepage;		// 2007-09-27 by cmkwon, Homepage가기 버튼 추가(베트남 VTC-Intecom 요청) - 멤버 변수 추가


	CBitmap				m_BackGround;
	CKbcButton			m_KbcGO;
	CKbcButton			m_kbcBtnJoin;

	CFTPManager			*m_pUpdateFTPManager;
	FTP_UPDATE_STATE	m_FTPUpdateState;
	MSG_PC_CONNECT_UPDATE_INFO	m_msg_PC_CONNECT_UPDATE_INFO;

	CHttpManager		*m_pHttpManager;
	CVTCGuardManager	*m_pVTCGuard;			// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

	void SetFTPUpdateState(FTP_UPDATE_STATE i_state){	m_FTPUpdateState = i_state;};


	CFont				m_fontServerGroupListBox;
	CFont				m_fontNoticeBox;

	CFont				m_fontDownloadFileNum;
	CFont				m_fontFileInfo;

// Dialog Data
	//{{AFX_DATA(CAtumLauncherDlg)
	enum { IDD = IDD_ATUMLAUNCHER_DIALOG };
	CButton	m_ctlBtnRememberID;
	CComboBoxEBX	m_comboServerList;
	
	CEdit		m_ctrlEditAccount;




	CListBoxEBX		m_ctrlServerList;			// 2006-05-02 by ispark
	CProgressCtrlX	m_progressCtrl;
	int		m_nServer;
	CString	m_szAccountName;
	CString	m_szPassword;
	CString	m_staticNumFileCtrl;	
	int		m_nWindowDegree;
	BOOL	m_ctlbWindowMode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumLauncherDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CUpdateWinSocket	*m_pUpdateWinsocket;
//	CFieldWinSocket		*m_pFieldWinsocket;
	SOCKET				m_PreSD;
	SOCKET				m_FieldSD;
	VersionInfo			m_CurrentVersion;
	VersionInfo			m_CurrentDelFileListVersion;
	VersionInfo			m_CurrentNoticeVersion;
//	CFtpDownload		m_FtpDownload;
	BOOL				m_bCancelFlag;
	BOOL				m_bProcessingVersionUpdate;
	char				m_PrivateIP[SIZE_MAX_IPADDRESS];
	BOOL				m_bShowPreServerIPDlg;
	BOOL				m_bUpdateClientFile;

	// config data in registry
	CString				m_szPreServerIPReg;
	CString				m_szExecutableBinaryReg;
	CString				m_szExecutePathReg;
	CString				m_szPreServerIPHistoryReg;
	CString				m_szExecuteBinHistoryReg;
	CString				m_szExecutePathHistoryReg;
	CString				m_szAccountNameReg;
	CString				m_szPasswordReg;
	//int					m_nServerGroupReg;
	CString				m_strServerGroupName;
	CString				m_szCrocessSuffix;
	int					m_nWindowModeReg;	// GAME_MODE_FULLSCREEN(1) or GAME_MODE_WINDOW(0)
// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 아래의 스트링 변수로 수정 함
//	int					m_nWindowDegreeReg;
	CString				m_csWindowsResolutionReg;		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 해상도 저장 시스템 수정


	BOOL				m_bGuardAgreementReg;		// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 

	//ysw
	CString				m_NoticeEdit;
	int					m_Cur_Rect;
	DWORD				m_Cur_Percent;
	BOOL				m_SelectFlag;
	BOOL				m_bControlEnabled;		// control의 enable 상태를 저장
	CBrush				m_StaticBrushBlack;
	CBrush				m_StaticBrushGray;
	CBrush				m_ListBrushGray;
	CBrush				m_listBrush;

	CBrush				m_progressBk;
	CBrush				m_progressBar;

	Host				*m_pHost;				// 2006-05-02 by ispark, 웹페이지
	int					m_nOldSel;				// 7/13/2006 by dgwoo list Old Select item
	CListBoxEBX				*m_ServerList;			// 7/13/2006 by dgwoo combo & list box handler

	int					m_nBirthYear;			// 2007-06-05 by cmkwon, 출생년도

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-06-18 by cmkwon, 네트워크 상태 체크 
	int					m_nMaxNetworkCheckCount;		// 2007-06-18 by cmkwon, 테스트 횟수
	int					m_nCurNetworkCheckCount;		// 2007-06-18 by cmkwon, 현재 테스트 진행 횟수
	DWORD				m_dwNetworkCheckSendTick;		// 2007-06-18 by cmkwon, 테스트 패킷을 전송한 Tick
	DWORD				m_dwSumPacketTickGap;			// 2007-06-18 by cmkwon, 각 패킷 전송 후 받은 시간의 합
	BOOL NTStartNetworkCheck(void);
	int NTOnReceivedNetworkCheckOK(int i_nCheckCount);
	int NTGetPingAverageTime(void);
	BOOL NTCheckTimeOver(DWORD *o_pdwPingTime);


//	BOOL SetCurrentClientVersion();
	char* GetFileName(char *szFullPathName, char *szFileName);
	BOOL Tokenizer(CList <CString, CString&>& lsString, CString strMsg, TCHAR devide);
	int CreateDirectory(CString strParent, CList<CString, CString&> &lsDir);
	BOOL ConnectPreServer();

// 2009-01-30 by cmkwon, 러시아 Innova 런처 시스템(프로스트) 수정 - 아래와 같이 수정함.
//	void ExecGame(char *cmdLine);
	void ExecGame(char *cmdLine, char *i_szAppPath=NULL, char *i_szCmdParam=NULL);
	void ExecGameCrocess(char *cmdLine);

	// Delete File List, Notice.txt 처리
	BOOL ProcessDeleteFileList(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo);
	BOOL ProcessNoticeFile(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo);

	BOOL SendLogin(BYTE i_nLoginType);	// LOGIN_TYPE_XXX

	int AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Http Update
	BOOL ProcessDeleteFileListByHttp(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-07 by cmkwon, 베트남 런처 인터페이스 수정 - 
	void MoveWindow2Center(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 - 
	CScreenKeyboardDlg		*m_pScreenKeyboardDlg;
	CWnd					*m_pInputEditFromScreenKeyboard;
	BOOL					m_bHideScreenKeyboardByScreenKeyboardWindow;	// 2007-09-18 by cmkwon, 화상키보드 수정 - 
	void PushCharFromScreenKeyboard(char i_cPushChar);
	void DeleteCharFromScreenKeyboard(void);
	void HideScreenKeyboardByScreenKeyboardWindow(void);		// 2007-09-18 by cmkwon, 화상키보드 수정 -

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 -
	BOOL FindWindowResolutionByWindowDegree(int *o_pnCX, int *o_pnCY, int *o_pnDegree, char *i_szWindowDegreeName);
	int InsertWindowDegreeList(CComboBox *i_pComboBox, BOOL i_bWindowsMode);
	int FindWindowDegreeComboBoxIndex(CComboBox *i_pComboBox, char *i_szWindowDegreeName);

	// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - 
	vectDEVMODE				m_vectSupportedResolutionList;
	int InitSupportedWindowResolutionList(void);
	BOOL IsSupportedResolution(int i_nWidth, int i_nHeight);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 
	vectSSERVER_GROUP_FOR_LAUNCHER	m_vectServerGroupList;
	BOOL FindServerGroupName(CString *o_pcsServerGroupName, int i_nFindIndex);

	CString					m_strEventURL;		// 2008-10-16 by cmkwon, Gameforge4D(Eng,Deu) Launcher Registry event 구현 - 런처 이벤트 URL

	CString					m_csSessionKey;		// 2008-12-18 by cmkwon, 일본 Arario 런처 수정 - CAtumLauncherDlg에 변수 추가
public:
	// display download progress & status
	void SetProgressRange(int up);
	void SetProgressPos(int pos);
	void SetFileNumText(char* str);
	void SetProgressGroupText(const char* str);

	void DisableControls();
	void EnableControls();

	void SetPrivateIP();

	BOOL DownloadUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	void ExtractUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	BOOL DownloadUpdateFileByHttp(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/,int nx,int ny);
	BOOL BitmapRgn( LPCTSTR resource, COLORREF TansColor ,int nx,int ny);
	BOOL BitmapRgn( UINT resource, COLORREF TansColor ,int nx,int ny);

	//ysw : 추가
	BOOL	ReadNoticeFile();
	int		DrawProgressBar();
	void	Set_Cur_Percent(DWORD CurSize);

	// UpdateData overloading
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE)
	{
		BOOL bRet = FALSE;

		m_szAccountName.MakeLower();
		m_szAccountNameReg.MakeLower();

		bRet = CDialog::UpdateData(bSaveAndValidate);

		m_szAccountName.MakeLower();
		m_szAccountNameReg.MakeLower();

		return bRet;
	}

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAtumLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJoin();
	afx_msg void OnSecret();
	afx_msg void OnEnd();
	afx_msg void OnDeca();
	afx_msg void OnBattal();
	afx_msg void OnSharin();
	afx_msg void OnPhilon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeList();
	afx_msg void OnCan();
	afx_msg void OnMin();
	afx_msg void OnSetfocusEditAccount();
	afx_msg void OnSetfocusEditPassword();
	afx_msg void OnBtnViewScreenKeyboard();
	afx_msg void OnBtnHomepage();
	afx_msg void OnCheckWindowsMode();
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDownLoadGamefilesDone(WPARAM wParam, LPARAM lParam);

	afx_msg LONG OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam);	
	afx_msg LONG OnUpdateVTCGuardOK(WPARAM wParam, LPARAM lParam);		// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

	// ysw
	afx_msg UINT OnNcHitTest( CPoint point );  // <- 여기에 추가

   	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_)
