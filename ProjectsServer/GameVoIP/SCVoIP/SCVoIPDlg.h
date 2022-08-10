// SCVoIPDlg.h : header file
//
//{{AFX_INCLUDES()
#include "scdialer1.h"
#include "SCDialer2.h"
//}}AFX_INCLUDES

#if !defined(AFX_SCVoIPDLG_H__CF1B91E6_DCFA_40BB_8A36_E525707248AF__INCLUDED_)
#define AFX_SCVoIPDLG_H__CF1B91E6_DCFA_40BB_8A36_E525707248AF__INCLUDED_

#include "VoIPProtocol.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TIMERID_ATUM_VOIP_END			0x10
#define TIMERID_ATUM_VOIP_SERVER_CHECK	0x11

#define TIMER_RESET_1to1_STATUS			101
#define TIMER_RESET_NtoN_STATUS			102
/////////////////////////////////////////////////////////////////////////////
// CSCVoIPDlg dialog

class CSCVoIPDlg : public CDialog
{
// Construction
public:
	CSCVoIPDlg(CWnd* pParent = NULL);	// standard constructor

	char					m_szCharacterName[SIZE_MAX_CHARACTER_NAME];
	HWND					m_hWndAtumVoIPServer;
	CString					m_strPrevPhoneNumber;
	
	CString					m_strPCCallAgentIP;
	int						m_nPCCallAgentPort;
	CString					m_strVDServerIP;
	int						m_nVDServerPort;

	int						m_nGameVoIPCallType;
	BOOL					m_bIsPartyCall;			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	
	CString					m_strVoIPAgentName;			// 2006-12-25 by cmkwon, 자신의 WindowText
	CString					m_strVoIPClientName;		// 2006-12-25 by cmkwon, 게임의 WindowText

	BOOL Process1to1Message(BYTE i_nMsgType, USHORT i_nMsgParam, COPYDATASTRUCT* pCopyDataStruct);
	BOOL ProcessNtoNMessage(BYTE i_nMsgType, USHORT i_nMsgParam, COPYDATASTRUCT* pCopyDataStruct);	

// Dialog Data
	//{{AFX_DATA(CSCVoIPDlg)
	enum { IDD = IDD_DIALOG_ATUM_VOIP };
	CEdit	m_editStatus;
	CEdit		m_editPhoneNumber;
	CButton		m_buttonDial;
	CButton		m_buttonLogin;
	CString		m_strPCN;
	CString		m_strPhoneNumber;
	CString		m_strPWD;
	CString		m_strStatus;
	CSCDialer1	m_ctrlSCDialer1;
	CSCDialer2	m_ctrlSCDialer2;
	int		m_nCallType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCVoIPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSCVoIPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonLogin();
	afx_msg void OnButtonDial();
	afx_msg void OnButtonClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton0();
	afx_msg void OnButton11();
	afx_msg void OnEvErrorMiradialerctrl1(short ErrCode, LPCTSTR ErrMsg);
	afx_msg void OnEvAlertMiradialerctrl1();
	afx_msg void OnEvReceiveCallMiradialerctrl1(LPCTSTR Caller);
	afx_msg void OnEvHangupMiradialerctrl1();
	afx_msg void OnEvConnectedMiradialerctrl1();
	afx_msg void OnEvUnRegistrationMiradialerctrl1(short RetryRegisterServer);
	afx_msg void OnEvReceiveButtonMiradialerctrl1(LPCTSTR Num);
	afx_msg void OnEvVolumeMiradialerctrl1(short Type, long Value);
	afx_msg void OnEvStatusMiradialerctrl1(short Status);
	afx_msg void OnEvRetrievePhoneBookMiradialerctrl1(LPCTSTR Login, LPCTSTR NotLogin);
	afx_msg void OnEvRegistrationMiradialerctrl1(long Date, LPCTSTR URL);
	afx_msg void OnEvWCAInfoMiradialerctrl1(LPCTSTR IP, long Port);
	afx_msg void OnEvRetrieveAudioDeviceListMiradialerctrl1(LPCTSTR AudioDeviceList);
	afx_msg void OnRadioPh();
	afx_msg void OnEvPushButtonMiradialerctrl1(LPCTSTR Num);
	afx_msg void OnEvReceiveMessageMiradialerctrl1(LPCTSTR Source, LPCTSTR Message);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnEvStatusScdialer2ctrl1(long i_nStatus);
	afx_msg void OnEvErrorScdialer2ctrl1(long i_nErrorNum, long i_nErrParam1, long i_nErrParam2, LPCTSTR i_szErrString);
	afx_msg void OnEvOtherJoinedRoomScdialer2ctrl1(LPCTSTR szJoinedUserName);
	afx_msg void OnEvOtherGoOutedRoomScdialer2ctrl1(LPCTSTR szGoOutedUserName);
	afx_msg void OnEvTalkScdialer2ctrl1(LPCTSTR i_szUserName, long i_bIsTalk);
	afx_msg void OnEvNotifyScdialer2ctrl1(long i_nNotifyCode, long i_nParam1, long i_nParam2, LPCTSTR i_szString);
	afx_msg void OnEvChangedTalkingStateScdialer2ctrl1(LPCTSTR UserName, long TalkingState);
	afx_msg void OnEvUserJoinedRoomScdialer2ctrl1(LPCTSTR UserName, long EnableTalking);
	afx_msg void OnEvVolumeScdialer2ctrl1(short Type, long Value);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCVoIPDLG_H__CF1B91E6_DCFA_40BB_8A36_E525707248AF__INCLUDED_)
