#if !defined(AFX_SCDialer1CTL_H__3558F4A1_1F00_4D10_B2F2_75C405A19156__INCLUDED_)
#define AFX_SCDialer1CTL_H__3558F4A1_1F00_4D10_B2F2_75C405A19156__INCLUDED_

#include "file_ver.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer1Ctl.h : Declaration of the CSCDialer1Ctrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl : See SCDialer1Ctl.cpp for implementation.
#include "DataSocket.h"
#include "DialerDefine.h"
//#include "../MCLib/MCLibConnection.h"
#include "H323Connection.h" // by cje
#include "NATSocket1.h" 
#include "NATAnalyzer.h" //by cje 020829
#include "RAD_RTP\INCLUDE\rvcommon.h"	// Added by ClassView

#define ALIVETIME 20000
#define MAXMEDIACOUNT 2

class CNATSocket;

class CSCDialer1Ctrl : public COleControl
{
	DECLARE_DYNCREATE(CSCDialer1Ctrl)

// Constructor
public:
	CSCDialer1Ctrl();
	CString strCaller; 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCDialer1Ctrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CSCDialer1Ctrl();

	DECLARE_OLECREATE_EX(CSCDialer1Ctrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CSCDialer1Ctrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSCDialer1Ctrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CSCDialer1Ctrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CSCDialer1Ctrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg long OnSocketMessageSCDialer1( WPARAM wParam, LPARAM lParam );
	afx_msg long OnSocketMessageWCA(WPARAM wParam, LPARAM lParam);
	afx_msg long OnNATResult( WPARAM wParam, LPARAM lParam );  //by cje 010719
	afx_msg long OnRecvFromSerialPort(WPARAM wParam, LPARAM lParam);
	afx_msg long OnNATAnalyzerResult(WPARAM wParam, LPARAM lParam);		//by cje 020725
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CSCDialer1Ctrl)
	CString m_strSCDialer1ServerIP;
	afx_msg void OnPCCallAgentIPChanged();
	long m_lSCDialer1Port;
	afx_msg void OnPCCallAgentPortChanged();
	CString m_strUserID;
	afx_msg void OnUserIDChanged();
	CString m_strPhoneNumber;
	afx_msg void OnPhoneNumberChanged();
	CString m_strUserPhoneNumber;
	afx_msg void OnPCNChanged();
	BOOL m_bAutoAcceptCall;
	afx_msg void OnAutoAcceptCallChanged();
	BOOL m_bEnableTelBox;
	afx_msg void OnEnableTelBoxChanged();
	long m_lProxyMode;
	afx_msg void OnProxyModeChanged();
	short m_nState;
	afx_msg void OnStatusChanged();
	CString m_strPhoneBookList;
	afx_msg void OnPhoneBookListChanged();
	short m_nAuthType;
	afx_msg void OnAuthTypeChanged();
	BOOL m_bCall;
	afx_msg void OnDialButtonClickedChanged();
	short m_nMyStatus;
	afx_msg void OnMyStatusChanged();
	BOOL m_bDTMFWave;
	afx_msg void OnDTMFWaveChanged();
	BOOL m_bRingWave;
	afx_msg void OnRingWaveChanged();
	BOOL m_bInBand;
	afx_msg void OnInBandChanged();
	CString m_strLKey;
	afx_msg void OnLKeyChanged();
	CString m_strWebCallAgentIP;
	afx_msg void OnWebCallAgentIPChanged();
	long m_nWebCallAgentPort;
	afx_msg void OnWebCallAgentPortChanged();
	short m_nCallType;
	afx_msg void OnCallTypeChanged();
	short m_nDeviceType;
	afx_msg void OnDeviceTypeChanged();
	short m_nSerialPort;
	afx_msg void OnSerialPortChanged();
	short m_nAutoRetryLoginInterval;
	afx_msg void OnAutoRetryLoginIntervalChanged();
	CString m_strPlayDevice;
	afx_msg void OnPlayDeviceChanged();
	CString m_strRecordDevice;
	afx_msg void OnRecordDeviceChanged();
	CString m_strWaveDevice;
	afx_msg void OnWaveDeviceChanged();
	long m_nNATServerPort;
	afx_msg void OnNATServerPortChanged();
	long m_nNATFireTalkPort;
	afx_msg void OnNATFireTalkPortChanged();
	BOOL m_bManualRoute;
	afx_msg void OnManualRouteChanged();
	afx_msg BOOL FireHangup(short Mode);
	afx_msg BOOL FireRegisterServer(short Mode);
	afx_msg BOOL FireUnRegisterServer();
	afx_msg BOOL FireAcceptCall(short IsAccept);
	afx_msg BOOL SetLicenseKey(LPCTSTR LicenseKey);
	afx_msg void SpkVolumeUp();
	afx_msg void SpkVolumeDown();
	afx_msg void MicVolumeUp();
	afx_msg void MicVolumeDown();
	afx_msg void PushButton(LPCTSTR button);
	afx_msg BOOL IsRegistered();
	afx_msg BOOL RetrievePhoneBook();
	afx_msg void GetAudioDeviceList();
	afx_msg void Forwarding(short Type, LPCTSTR Destination);
	afx_msg void SendVideoInfo(LPCTSTR IP, long Port);
	afx_msg BOOL SendText(LPCTSTR Destination, LPCTSTR Message);
	afx_msg long SerialMessage(long dwMsg);
	afx_msg long TimeOutTone();
	afx_msg void FireDial();
	afx_msg void FireReDial();
	afx_msg BSTR GetVersion();
	afx_msg void SendVideoPause(long Flag);
	afx_msg void SetKeepAliveSerial(LPCTSTR KeepAlive);
	afx_msg void MakeConference(short AttendantCount, LPCTSTR ConferenceInfo);
	afx_msg void JoinConference(LPCTSTR ConferenceID, LPCTSTR ConferenceInfo);
	afx_msg void InviteConference(LPCTSTR Callee);
	afx_msg void CloseConference();
	afx_msg void StatusConference(short Status, LPCTSTR StatusInfo);
	afx_msg void LeaveConference();
	afx_msg long GetSupportedVoiceCodec();
	afx_msg void SetSupportedVoiceCodec(long nVoiceCodec);
	afx_msg long GetSupportedVideoCodec();
	afx_msg void SetSupportedVideoCodec(long nVideoCodec);
	afx_msg void SetVoiceCodec(long nVoiceCodec);
	afx_msg long GetVoiceCodec();
	afx_msg long SetSpkVolume(long i_nVolumeValueOfPercent);
	afx_msg long GetSpkVolume();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CSCDialer1Ctrl)
	void FireEvError(short ErrCode, LPCTSTR ErrMsg)
		{FireEvent(eventidEvError,EVENT_PARAM(VTS_I2  VTS_BSTR), ErrCode, ErrMsg);}
	void FireEvAlert()
		{FireEvent(eventidEvAlert,EVENT_PARAM(VTS_NONE));}
	void FireEvReceiveCall(LPCTSTR Caller)
		{FireEvent(eventidEvReceiveCall,EVENT_PARAM(VTS_BSTR), Caller);}
	void FireEvHangup()
		{FireEvent(eventidEvHangup,EVENT_PARAM(VTS_NONE));}
	void FireEvGetPeerIP(LPCTSTR PeerIP, long PeerPort)
		{FireEvent(eventidEvGetPeerIP,EVENT_PARAM(VTS_BSTR  VTS_I4), PeerIP, PeerPort);}
	void FireEvConnected()
		{FireEvent(eventidEvConnected,EVENT_PARAM(VTS_NONE));}
	void FireEvUnRegistration(short RetryRegisterServer)
		{FireEvent(eventidEvUnRegistration,EVENT_PARAM(VTS_I2), RetryRegisterServer);}
	void FireEvAntena(short Antena)
		{FireEvent(eventidEvAntena,EVENT_PARAM(VTS_I2), Antena);}
	void FireEvPushButton(LPCTSTR Num)
		{FireEvent(eventidEvPushButton,EVENT_PARAM(VTS_BSTR), Num);}
	void FireEvReceiveButton(LPCTSTR Num)
		{FireEvent(eventidEvReceiveButton,EVENT_PARAM(VTS_BSTR), Num);}
	void FireEvVolume(short Type, long Value)
		{FireEvent(eventidEvVolume,EVENT_PARAM(VTS_I2  VTS_I4), Type, Value);}
	void FireEvStatus(short Status)
		{FireEvent(eventidEvStatus,EVENT_PARAM(VTS_I2), Status);}
	void FireEvRetrievePhoneBook(LPCTSTR Login, LPCTSTR NotLogin)
		{FireEvent(eventidEvRetrievePhoneBook,EVENT_PARAM(VTS_BSTR  VTS_BSTR), Login, NotLogin);}
	void FireEvRegistration(long Date, LPCTSTR URL)
		{FireEvent(eventidEvRegistration,EVENT_PARAM(VTS_I4  VTS_BSTR), Date, URL);}
	void FireEvWCAInfo(LPCTSTR IP, long Port)
		{FireEvent(eventidEvWCAInfo,EVENT_PARAM(VTS_BSTR  VTS_I4), IP, Port);}
	void FireEvRetrieveAudioDeviceList(LPCTSTR AudioDeviceList)
		{FireEvent(eventidEvRetrieveAudioDeviceList,EVENT_PARAM(VTS_BSTR), AudioDeviceList);}
	void FireEvSuccess(short SuccessCode)
		{FireEvent(eventidEvSuccess,EVENT_PARAM(VTS_I2), SuccessCode);}
	void FireEvRetrieveSearchUser(long Count, LPCTSTR SearchUserList)
		{FireEvent(eventidEvRetrieveSearchUser,EVENT_PARAM(VTS_I4  VTS_BSTR), Count, SearchUserList);}
	void FireEvForwarding(short Type, LPCTSTR Destination)
		{FireEvent(eventidEvForwarding,EVENT_PARAM(VTS_I2  VTS_BSTR), Type, Destination);}
	void FireEvVideoStartInfo(LPCTSTR IP, long Port)
		{FireEvent(eventidEvVideoStartInfo,EVENT_PARAM(VTS_BSTR  VTS_I4), IP, Port);}
	void FireEvNotice(short Type, LPCTSTR Message)
		{FireEvent(eventidEvNotice,EVENT_PARAM(VTS_I2  VTS_BSTR), Type, Message);}
	void FireEvReceiveMessage(LPCTSTR Source, LPCTSTR Message)
		{FireEvent(eventidEvReceiveMessage,EVENT_PARAM(VTS_BSTR  VTS_BSTR), Source, Message);}
	void FireEvProgress(short ProgressCode, short Type)
		{FireEvent(eventidEvProgress,EVENT_PARAM(VTS_I2  VTS_I2), ProgressCode, Type);}
	void FireEvSerialMessage(short Message)
		{FireEvent(eventidEvSerialMessage,EVENT_PARAM(VTS_I2), Message);}
	void FireEvSerialConnect()
		{FireEvent(eventidEvSerialConnect,EVENT_PARAM(VTS_NONE));}
	void FireEvSerialDisconnect()
		{FireEvent(eventidEvSerialDisconnect,EVENT_PARAM(VTS_NONE));}
	void FireEvHookOn(short bHookSwitch)
		{FireEvent(eventidEvHookOn,EVENT_PARAM(VTS_I2), bHookSwitch);}
	void FireEvHookOff(short bHookSwitch)
		{FireEvent(eventidEvHookOff,EVENT_PARAM(VTS_I2), bHookSwitch);}
	void FireEvVideoCreateInfo(long createport)
		{FireEvent(eventidEvVideoCreateInfo,EVENT_PARAM(VTS_I4), createport);}
	void FireEvVideoPause(long Flag)
		{FireEvent(eventidEvVideoPause,EVENT_PARAM(VTS_I4), Flag);}
	void FireEvMakeConference(LPCTSTR ServerIP, long ServerPort, LPCTSTR ConferenceInfo)
		{FireEvent(eventidEvMakeConference,EVENT_PARAM(VTS_BSTR  VTS_I4  VTS_BSTR), ServerIP, ServerPort, ConferenceInfo);}
	void FireEvJoinConference(LPCTSTR ServerIP, long ServerPort, LPCTSTR ConferenceInfo)
		{FireEvent(eventidEvJoinConference,EVENT_PARAM(VTS_BSTR  VTS_I4  VTS_BSTR), ServerIP, ServerPort, ConferenceInfo);}
	void FireEvInviteConference(LPCTSTR Caller, LPCTSTR ConferenceInfo)
		{FireEvent(eventidEvInviteConference,EVENT_PARAM(VTS_BSTR  VTS_BSTR), Caller, ConferenceInfo);}
	void FireEvSelectedVoiceCodec(long nVoiceCodec)
		{FireEvent(eventidEvSelectedVoiceCodec,EVENT_PARAM(VTS_I4), nVoiceCodec);}
	void FireEvSelectedVideoCodec(long nVideoCodec)
		{FireEvent(eventidEvSelectedVideoCodec,EVENT_PARAM(VTS_I4), nVideoCodec);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	CString GetMyIP();

	CString m_strLogin;
	CString m_strNotLogin;
	long m_lPhonebookCount;
//	CString m_strMyIP;
	DataSocket* m_pDataSocketSCDialer1;
	DataSocket* m_pDataSocketWCA;
	CH323Connection* m_323Connection;  //by cje
	MESSAGE m_msg;
//	short m_nState; // Property로 선언
	CNATSocket*				m_pNATSoc[MAXMEDIACOUNT]; // by cje
	MEDIAPORTINFO			m_MediaInfo[MAXMEDIACOUNT];
	MEDIAPORTINFO			m_PeerMediaInfo[MAXMEDIACOUNT];
	BOOL					m_bNATResulted;
	UINT					m_uMediaCount; // 1:pc2phone, 2:pc2pc
	CNATAnalyzer*			m_pNATAnalyzer; //by cje 020827
	BOOL					m_bAnalyzed;
	MESSAGE					m_msgFireTalkAddr;
	BOOL					m_bReAnalyzed;
	
	BOOL					m_bActiveConfirm;  //by cje 021112
	BOOL				    m_bMediaActiveConfirm[MAXMEDIACOUNT]	;
	
	BOOL					m_bFireTalkExist;
	unsigned long			m_nRealNetworkType; //분석에의한 타입값을 보존하기위함..
	unsigned long			m_nNetworkType;
	unsigned long			m_nPeerNetworkType;
	unsigned long			m_nRTPTransferType;	
	
	long					m_nRTPmode;
	unsigned			m_uCreatePort;
	unsigned			m_uProxyRTPPort;
	BOOL					m_bCaller;
	unsigned			m_uCalleeRTPPort; //Caller에게 알려줄 포트
	CString				m_strCallerPCN;
	BOOL				m_bRegistered; 

public:
	BOOL Registration();
public:
	CString m_strKeepAlive;
	BOOL m_bIsHookOff;
	short m_nInterval;
	void SendMoneyData();
	long m_lBillingPID;
	int m_nPrevCallType;
	short m_nEvConnectedCount;
	BOOL m_bIsPh2PC;
	CString GetPathRegistered(CString strCLSID);
	BOOL GetModuleVersion(LPTSTR lpDestFile, LPTSTR strVersion, LPTSTR strBuild);

	BOOL GetIPAddress(char* GatewayIP,char* LocalIP);
	char LocalGWIP[20] ; //by cje 020907
	char LocalIP[20] ; //by cje 020907
	MESSAGE m_msgOK;
	MESSAGE m_msgDIAL;
	void OnSocketDial();
	void OnSocketOK();

	CString m_strPrevPhoneNumber;
	void WCALogIn();
	void WCAConnect(LPCTSTR ip, long port);
	CString m_strText;
	CTime m_timeConnected;
	CString m_strSearchUserList;
	long m_lSearchUser;
	CString m_strSignUp;
	long m_lMiraCallVersion;
	BOOL m_bProgressRegistration; // 등록 진행 중에 또 다른 등록 명령을 막는다.
	BOOL m_bProgressPhonebook; // 폰북 정보 가져 오는 중에 또 다른 폰북 명령을 막는다.
	BOOL m_bProgressSignUp; // SignUp, Modify
	BOOL m_bProgressSearchUser; // SearchUser
	BOOL m_bProgressBilling;
	short m_nRegisterMode; // 0 : normal login [_REGISTRATION], 1 : logout and login [_UNREGISTRATION]
	BOOL m_bReTryToLogin; // EvUnRegistration 이 재시도를 유발하는 것인지 아닌지를 결정해준다.
	CString m_strTryToLoginIP; // 다른 곳에서 로긴을 시도후 성공하면 끊어지는 쪽에서 상대방의 IP 정보를 준다.
	CString m_strPCCallAgentIPAlreadyLogined; // 이미 로그인이 되었었던 PCCA IP. 여기로 다시 _REREGISTRATION 메세지를 보낸다.

	UINT m_nProductID;
	UINT m_nProductIDWCA;
//	BOOL m_bCall;
	BOOL m_bLicense;
	BOOL m_bInternalCall; //by cje 같은 NAT내의 콜일때 구분.
	int SendStartRTPMsg(unsigned port);
	int CSCDialer1Ctrl::OnReceivedDial(unsigned int port);
	void StartRTP();
	void SendDialMsg(unsigned rtpport);
	void SendRouteMsg();
	int ParseData(CString str, char sep, CStringArray* array);

	long				m_nSupportedVoiceCodec; 
	long				m_nSupportedVideoCodec;
	
	long				m_nVoiceCodec; 
	long				m_nVideoCodec; 
	
	long SelectVoiceCodec(long voicecodec); 
	void SelectVideoCodec(long videocodec); 
	long VoiceCodecInfo(long voicdecode); 
	void SetVideoCodec(long nVoiceCode); 
	long GetVideoCodec() ;

	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
	void OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent);
	
	UINT m_nPeerPort;
	CString m_strPeerIP;
	enum {
	//{{AFX_DISP_ID(CSCDialer1Ctrl)
	dispidPCCallAgentIP = 1L,
	dispidPCCallAgentPort = 2L,
	dispidUserID = 3L,
	dispidPhoneNumber = 4L,
	dispidPCN = 5L,
	dispidAutoAcceptCall = 6L,
	dispidEnableTelBox = 7L,
	dispidProxyMode = 8L,
	dispidStatus = 9L,
	dispidPhoneBookList = 10L,
	dispidAuthType = 11L,
	dispidDialButtonClicked = 12L,
	dispidMyStatus = 13L,
	dispidDTMFWave = 14L,
	dispidRingWave = 15L,
	dispidInBand = 16L,
	dispidLKey = 17L,
	dispidWebCallAgentIP = 18L,
	dispidWebCallAgentPort = 19L,
	dispidCallType = 20L,
	dispidDeviceType = 21L,
	dispidSerialPort = 22L,
	dispidAutoRetryLoginInterval = 23L,
	dispidPlayDevice = 24L,
	dispidRecordDevice = 25L,
	dispidWaveDevice = 26L,
	dispidNATServerPort = 27L,
	dispidNATFireTalkPort = 28L,
	dispidManualRoute = 29L,
	dispidHangup = 30L,
	dispidRegisterServer = 31L,
	dispidUnRegisterServer = 32L,
	dispidAcceptCall = 33L,
	dispidSetLicenseKey = 34L,
	dispidSpkVolumeUp = 35L,
	dispidSpkVolumeDown = 36L,
	dispidMicVolumeUp = 37L,
	dispidMicVolumeDown = 38L,
	dispidPushButton = 39L,
	dispidIsRegistered = 40L,
	dispidRetrievePhoneBook = 41L,
	dispidGetAudioDeviceList = 42L,
	dispidForwarding = 43L,
	dispidSendVideoInfo = 44L,
	dispidSendMessage = 45L,
	dispidSerialMessage = 46L,
	dispidTimeOutTone = 47L,
	dispidDial = 48L,
	dispidReDial = 49L,
	dispidGetVersion = 50L,
	dispidSendVideoPause = 51L,
	dispidSetKeepAliveSerial = 52L,
	dispidMakeConference = 53L,
	dispidJoinConference = 54L,
	dispidInviteConference = 55L,
	dispidCloseConference = 56L,
	dispidStatusConference = 57L,
	dispidLeaveConference = 58L,
	dispidGetSupportedVoiceCodec = 59L,
	dispidSetSupportedVoiceCodec = 60L,
	dispidGetSupportedVideoCodec = 61L,
	dispidSetSupportedVideoCodec = 62L,
	dispidSetVoiceCodec = 63L,
	dispidGetVoiceCodec = 64L,
	dispidSetSpkVolume = 65L,
	dispidGetSpkVolume = 66L,
	eventidEvError = 1L,
	eventidEvAlert = 2L,
	eventidEvReceiveCall = 3L,
	eventidEvHangup = 4L,
	eventidEvGetPeerIP = 5L,
	eventidEvConnected = 6L,
	eventidEvUnRegistration = 7L,
	eventidEvAntena = 8L,
	eventidEvPushButton = 9L,
	eventidEvReceiveButton = 10L,
	eventidEvVolume = 11L,
	eventidEvStatus = 12L,
	eventidEvRetrievePhoneBook = 13L,
	eventidEvRegistration = 14L,
	eventidEvWCAInfo = 15L,
	eventidEvRetrieveAudioDeviceList = 16L,
	eventidEvSuccess = 17L,
	eventidEvRetrieveSearchUser = 18L,
	eventidEvForwarding = 19L,
	eventidEvVideoStartInfo = 20L,
	eventidEvNotice = 21L,
	eventidEvReceiveMessage = 22L,
	eventidEvProgress = 23L,
	eventidEvSerialMessage = 24L,
	eventidEvSerialConnect = 25L,
	eventidEvSerialDisconnect = 26L,
	eventidEvHookOn = 27L,
	eventidEvHookOff = 28L,
	eventidEvVideoCreateInfo = 29L,
	eventidEvVideoPause = 30L,
	eventidEvMakeConference = 31L,
	eventidEvJoinConference = 32L,
	eventidEvInviteConference = 33L,
	eventidEvSelectedVoiceCodec = 34L,
	eventidEvSelectedVideoCodec = 35L,
	//}}AFX_DISP_ID
	};
private:
	CFileVersionInfo m_info;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDialer1CTL_H__3558F4A1_1F00_4D10_B2F2_75C405A19156__INCLUDED)
