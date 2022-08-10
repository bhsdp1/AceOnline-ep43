// AtumDlg.h : header file
//

#if !defined(AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_)
#define AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PreUpdateWinSocket.h"
#include "AtumProtocol.h"

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
//#define FULL_PATH_REGISTRY				"SOFTWARE\\SpaceCowboy"		// Windows XP SP2 보안 설정
//#define EXE_1_FILE_NAME				"SpaceCowboy.exe"
//#define LAUNCHER_FILE_NAME				"Launcher.atm"
//
//#ifdef _MGAME_TEST_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//	#define MAIN_PRE_SERVER_IP_0		218
//	#define MAIN_PRE_SERVER_IP_1		38
//	#define MAIN_PRE_SERVER_IP_2		136
//	#define MAIN_PRE_SERVER_IP_3		201
//	#define REGISTRY_BASE_PATH			"SpaceCowboy_Test"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\SpaceCowboy_Test"		//
//	#define EXE_1_FILE_NAME			"SpaceCowboy_Test.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher_Test.atm"
//#endif
//#ifdef _MASANG15_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//	// 2007-01-03 by cmkwon, 121.134.114.144
//	#define MAIN_PRE_SERVER_IP_0		121
//	#define MAIN_PRE_SERVER_IP_1		134
//	#define MAIN_PRE_SERVER_IP_2		114
//	#define MAIN_PRE_SERVER_IP_3		144
//	#define REGISTRY_BASE_PATH			"SpaceCowboy"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\SpaceCowboy"		//
//	#define EXE_1_FILE_NAME			"SpaceCowboy.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher.atm"
//#endif
//#ifdef _MASANG51_SERVER
//	#undef MAIN_PRE_SERVER_IP_0
//	#undef MAIN_PRE_SERVER_IP_1
//	#undef MAIN_PRE_SERVER_IP_2
//	#undef MAIN_PRE_SERVER_IP_3
//	#undef REGISTRY_BASE_PATH
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//	// 2007-01-03 by cmkwon, 121.134.114.140
//	#define MAIN_PRE_SERVER_IP_0		121
//	#define MAIN_PRE_SERVER_IP_1		134
//	#define MAIN_PRE_SERVER_IP_2		114
//	#define MAIN_PRE_SERVER_IP_3		140
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Masang51)"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\SpaceCowboy"		//
//	#define EXE_1_FILE_NAME				"SpaceCowboy.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher.atm"
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
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//#ifdef _TEST_SERVER					// 2006-06-12 by cmkwon, 미국 Gala-Net
//	// 2006-06-12 by cmkwon, 69.90.214.114
//	#define MAIN_PRE_SERVER_IP_0		69
//	#define MAIN_PRE_SERVER_IP_1		90
//	#define MAIN_PRE_SERVER_IP_2		214
//	#define MAIN_PRE_SERVER_IP_3		114
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Eng)_Test"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Masang Soft\\SpaceCowboy(Eng)_Test"	//
//	#define EXE_1_FILE_NAME			"SpaceCowboy_Test.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher_Test.atm"
//#else
//	// 2006-05-19 by cmkwon, 69.90.214.106
//	#define MAIN_PRE_SERVER_IP_0		69
//	#define MAIN_PRE_SERVER_IP_1		90
//	#define MAIN_PRE_SERVER_IP_2		214
//	#define MAIN_PRE_SERVER_IP_3		106
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Eng)"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Masang Soft\\SpaceCowboy(Eng)"	//
//	#define EXE_1_FILE_NAME			"SpaceCowboy.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher.atm"
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
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//#ifdef _TEST_SERVER					// 2006-03-23 by cmkwon, 베트남 VTC
//	// 2006-06-09 by cmkwon, 210.245.12.5: VTC-Intecom TestServer
//	// 2006-09-08 by cmkwon, 변경(210.245.12.5-->222.255.15.54)
//	#define MAIN_PRE_SERVER_IP_0		222
//	#define MAIN_PRE_SERVER_IP_1		255
//	#define MAIN_PRE_SERVER_IP_2		15
//	#define MAIN_PRE_SERVER_IP_3		54
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Viet)_Test"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Masang Soft\\SpaceCowboy(Viet)_Test"	//
//	#define EXE_1_FILE_NAME			"SpaceCowboy_Test.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher_Test.atm"
//#else
//	// 2006-08-17 by cmkwon, 222.255.15.37: VTC-Intecom PreServer
//	#define MAIN_PRE_SERVER_IP_0		222
//	#define MAIN_PRE_SERVER_IP_1		255
//	#define MAIN_PRE_SERVER_IP_2		15
//	#define MAIN_PRE_SERVER_IP_3		37
//	#define REGISTRY_BASE_PATH			"SpaceCowboy(Viet)"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Masang Soft\\SpaceCowboy(Viet)"	//
//	#define EXE_1_FILE_NAME			"SpaceCowboy.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher.atm"
//#endif// end_#ifdef _TEST_SERVER
//#endif// end_#ifdef _VTC_VIET_SERVER
//
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
//	#undef FULL_PATH_REGISTRY
//	#undef EXE_1_FILE_NAME
//	#undef LAUNCHER_FILE_NAME
//
//#ifdef _TEST_SERVER
//	// 2006-09-05 by cmkwon, 임시로 본섭과 같이 해 놓는다 - 218.145.57.83
//	// 2006-12-22 by cmkwon, 211.189.116.11 <=예당온라인
//	#define MAIN_PRE_SERVER_IP_0		211
//	#define MAIN_PRE_SERVER_IP_1		189
//	#define MAIN_PRE_SERVER_IP_2		116
//	#define MAIN_PRE_SERVER_IP_3		11
//	#define REGISTRY_BASE_PATH			"ACEonline_Test"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Yedang Online\\ACEonline_Test"	//
//	#define EXE_1_FILE_NAME			"ACEonline_Test.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher_Test.atm"
//#else
//	// 2006-09-05 by cmkwon, 218.145.57.83
//	// 2006-12-22 by cmkwon, 211.189.116.13 <=예당온라인
//	#define MAIN_PRE_SERVER_IP_0		211
//	#define MAIN_PRE_SERVER_IP_1		189
//	#define MAIN_PRE_SERVER_IP_2		116
//	#define MAIN_PRE_SERVER_IP_3		13
//	#define REGISTRY_BASE_PATH			"ACEonline"
//	#define FULL_PATH_REGISTRY			"SOFTWARE\\Yedang Online\\ACEonline"	//
//	#define EXE_1_FILE_NAME			"ACEonline.exe"
//	#define LAUNCHER_FILE_NAME			"Launcher.atm"
//#endif// end_#ifdef _TEST_SERVER
//#endif// end_#ifdef _KOREA_SERVER_2

// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 
#define TIMERID_SEND_ALIVE_PACKET		3


/////////////////////////////////////////////////////////////////////////////
// CAtumDlg dialog
class CHttpManager;
class CAtumDlg : public CDialog
{
// Construction
public:
	CAtumDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAtumDlg)
	enum { IDD = IDD_ATUM_DIALOG };
	CProgressCtrl	m_progressCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CPreUpdateWinSocket	*m_pPreUpdateWinsocket;
	VersionInfo			m_CurrentLauncherVersion;
	VersionInfo			m_UpdatingLauncherVersion;		// 2007-01-06 by cmkwon
	CString				m_szCrocessSuffix;
	CString				m_szMainPreServerIP;

	CHttpManager		*m_pHttpManager;				// 2007-01-05 by cmkwon
	vectSGAME_SERVER_GROUP		m_vectGameServerGroupList;			// 2007-05-15 by cmkwon

	BOOL GetLauncherFileFTP(MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo);
	BOOL GetLauncherFileHTTP(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo);
	void ExecLauncher();
	void ExecLauncherCrocess();

	int AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAtumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);

	afx_msg LONG OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam);	

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_)
