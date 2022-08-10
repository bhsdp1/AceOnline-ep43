#if !defined(AFX_SCDIALER2CTL_H__317B2030_6E5F_4119_BD76_63BB1CA26513__INCLUDED_)
#define AFX_SCDIALER2CTL_H__317B2030_6E5F_4119_BD76_63BB1CA26513__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer2Ctl.h : Declaration of the CSCDialer2Ctrl ActiveX Control class.
#include "VoIPParam.h"
#include "SocketHeader.h"
#include "VoIPProtocol.h"


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl : See SCDialer2Ctl.cpp for implementation.
class CS2DWinSocket;
class CS2CWinSocket;
class CWinSocket;
class CAVManager;
class CSCDialer2Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSCDialer2Ctrl)

// Constructor
public:
	CSCDialer2Ctrl();

	void ResetSCDialer2Ctrl(void);

	void SetCtlStatus(Status_SCDialer2 i_nStatus, BOOL i_bNotifyEvent=TRUE);

	void OnSocketError(MSG_ERROR *i_pError, CWinSocket *i_pSocket);	

	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 ¼öÁ¤ - 
	void OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent);

public:
	int						m_nCtlStatus;
	int						m_nCtlError;

	CAVManager				*m_pAVManager;
	CS2DWinSocket			*m_pS2DWinSocket;
	CS2CWinSocket			*m_pS2CWinSocket;
	
	BOOL					m_bCreateRoomFlag;
	char					m_szVCServerIP[SIZE_MAX_IPADDRESS];
	USHORT					m_usVCServerPort;


public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCDialer2Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSCDialer2Ctrl();

	DECLARE_OLECREATE_EX(CSCDialer2Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSCDialer2Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSCDialer2Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSCDialer2Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSCDialer2Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnS2DAsyncEventWinSocket(SOCKET i_hSocket, LPARAM i_lParam);
	afx_msg void OnS2DPacketNotifyWinSocket(WPARAM i_wParam);
	afx_msg void OnS2CAsyncEventWinSocket(SOCKET i_hSocket, LPARAM i_lParam);
	afx_msg void OnS2CPacketNotifyWinSocket(WPARAM i_wParam);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSCDialer2Ctrl)
	CString m_strVDServerIP;
	afx_msg void OnStrVDServerIPChanged();
	long m_usVDServerPort;
	afx_msg void OnUsVDServerPortChanged();
	long m_nStatus;
	afx_msg void OnStatusChanged();
	BOOL m_audioSendPause;
	afx_msg void OnAudioSendPauseChanged();
	afx_msg long JoinRoom(LPCTSTR szUserName, long FAR* pn64RoomID);
	afx_msg void GoOutRoom();
	afx_msg long CreateRoom(LPCTSTR szUserName, long FAR* pn64RoomID);
	afx_msg long CreateRoomEx(LPCTSTR i_szUserName, long FAR* i_pn64RoomID, long i_nConcurrentTalkingCount);
	afx_msg long SetSpkVolume(long i_nVolumeValueOfPercent);
	afx_msg long GetSpkVolume();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CSCDialer2Ctrl)
	void FireEvStatus(long i_nStatus)
		{FireEvent(eventidEvStatus,EVENT_PARAM(VTS_I4), i_nStatus);}
	void FireEvError(long i_nErrorNum, long i_nErrParam1, long i_nErrParam2, LPCTSTR i_szErrString)
		{FireEvent(eventidEvError,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4  VTS_BSTR), i_nErrorNum, i_nErrParam1, i_nErrParam2, i_szErrString);}
	void FireEvOtherJoinedRoom(LPCTSTR szJoinedUserName)
		{FireEvent(eventidEvOtherJoinedRoom,EVENT_PARAM(VTS_BSTR), szJoinedUserName);}
	void FireEvOtherGoOutedRoom(LPCTSTR szGoOutedUserName)
		{FireEvent(eventidEvOtherGoOutedRoom,EVENT_PARAM(VTS_BSTR), szGoOutedUserName);}
	void FireEvTalk(LPCTSTR i_szUserName, long i_bIsTalk)
		{FireEvent(eventidEvTalk,EVENT_PARAM(VTS_BSTR  VTS_I4), i_szUserName, i_bIsTalk);}
	void FireEvNotify(long i_nNotifyCode, long i_nParam1, long i_nParam2, LPCTSTR i_szString)
		{FireEvent(eventidEvNotify,EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4  VTS_BSTR), i_nNotifyCode, i_nParam1, i_nParam2, i_szString);}
	void FireEvChangedTalkingState(LPCTSTR UserName, long TalkingState)
		{FireEvent(eventidEvChangedTalkingState,EVENT_PARAM(VTS_BSTR  VTS_I4), UserName, TalkingState);}
	void FireEvUserJoinedRoom(LPCTSTR UserName, long EnableTalking)
		{FireEvent(eventidEvUserJoinedRoom,EVENT_PARAM(VTS_BSTR  VTS_I4), UserName, EnableTalking);}
	void FireEvVolume(short Type, long Value)
		{FireEvent(eventidEvVolume,EVENT_PARAM(VTS_I2  VTS_I4), Type, Value);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CSCDialer2Ctrl)
	dispidStrVDServerIP = 1L,
	dispidUsVDServerPort = 2L,
	dispidNStatus = 3L,
	dispidAudioSendPause = 4L,
	dispidJoinRoom = 5L,
	dispidGoOutRoom = 6L,
	dispidCreateRoom = 7L,
	dispidCreateRoomEx = 8L,
	dispidSetSpkVolume = 9L,
	dispidGetSpkVolume = 10L,
	eventidEvStatus = 1L,
	eventidEvError = 2L,
	eventidEvOtherJoinedRoom = 3L,
	eventidEvOtherGoOutedRoom = 4L,
	eventidEvTalk = 5L,
	eventidEvNotify = 6L,
	eventidEvChangedTalkingState = 7L,
	eventidEvUserJoinedRoom = 8L,
	eventidEvVolume = 9L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDIALER2CTL_H__317B2030_6E5F_4119_BD76_63BB1CA26513__INCLUDED)
