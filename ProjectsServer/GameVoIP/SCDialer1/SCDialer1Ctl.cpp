// SCDialer1Ctl.cpp : Implementation of the CSCDialer1Ctrl ActiveX Control class.

#include "stdafx.h"
#include "SCDialer1.h"
#include "SCDialer1Ctl.h"
#include "SCDialer1Ppg.h"
#include "Code2.h"
#include "iphlpapi.h"
#include "RTPRouter.h"
#include "WindowsRegistry.h"
#include "IncommingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSCDialer1Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSCDialer1Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSCDialer1Ctrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE( UM_MSGSOCKET, OnSocketMessageSCDialer1 )
	ON_MESSAGE( UM_NATRESULT, OnNATResult )
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE( UM_WCASOCKET, OnSocketMessageWCA )
	ON_MESSAGE(WM_SERIAL_SIGNAL, OnRecvFromSerialPort)
	ON_MESSAGE(WM_NATANALYZER_RESULT, OnNATAnalyzerResult)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSCDialer1Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSCDialer1Ctrl)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PCCallAgentIP", m_strSCDialer1ServerIP, OnPCCallAgentIPChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PCCallAgentPort", m_lSCDialer1Port, OnPCCallAgentPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "UserID", m_strUserID, OnUserIDChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PhoneNumber", m_strPhoneNumber, OnPhoneNumberChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PCN", m_strUserPhoneNumber, OnPCNChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "AutoAcceptCall", m_bAutoAcceptCall, OnAutoAcceptCallChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "EnableTelBox", m_bEnableTelBox, OnEnableTelBoxChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "ProxyMode", m_lProxyMode, OnProxyModeChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "Status", m_nState, OnStatusChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PhoneBookList", m_strPhoneBookList, OnPhoneBookListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "AuthType", m_nAuthType, OnAuthTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "DialButtonClicked", m_bCall, OnDialButtonClickedChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "MyStatus", m_nMyStatus, OnMyStatusChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "DTMFWave", m_bDTMFWave, OnDTMFWaveChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "RingWave", m_bRingWave, OnRingWaveChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "InBand", m_bInBand, OnInBandChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "LKey", m_strLKey, OnLKeyChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "WebCallAgentIP", m_strWebCallAgentIP, OnWebCallAgentIPChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "WebCallAgentPort", m_nWebCallAgentPort, OnWebCallAgentPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "CallType", m_nCallType, OnCallTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "DeviceType", m_nDeviceType, OnDeviceTypeChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "SerialPort", m_nSerialPort, OnSerialPortChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "AutoRetryLoginInterval", m_nAutoRetryLoginInterval, OnAutoRetryLoginIntervalChanged, VT_I2)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "PlayDevice", m_strPlayDevice, OnPlayDeviceChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "RecordDevice", m_strRecordDevice, OnRecordDeviceChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "WaveDevice", m_strWaveDevice, OnWaveDeviceChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "NATServerPort", m_nNATServerPort, OnNATServerPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "NATFireTalkPort", m_nNATFireTalkPort, OnNATFireTalkPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer1Ctrl, "ManualRoute", m_bManualRoute, OnManualRouteChanged, VT_BOOL)
	DISP_FUNCTION(CSCDialer1Ctrl, "Hangup", FireHangup, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CSCDialer1Ctrl, "RegisterServer", FireRegisterServer, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CSCDialer1Ctrl, "UnRegisterServer", FireUnRegisterServer, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "AcceptCall", FireAcceptCall, VT_BOOL, VTS_I2)
	DISP_FUNCTION(CSCDialer1Ctrl, "SetLicenseKey", SetLicenseKey, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "SpkVolumeUp", SpkVolumeUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "SpkVolumeDown", SpkVolumeDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "MicVolumeUp", MicVolumeUp, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "MicVolumeDown", MicVolumeDown, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "PushButton", PushButton, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "IsRegistered", IsRegistered, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "RetrievePhoneBook", RetrievePhoneBook, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetAudioDeviceList", GetAudioDeviceList, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "Forwarding", Forwarding, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "SendVideoInfo", SendVideoInfo, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "SendMessage", SendText, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "SerialMessage", SerialMessage, VT_I4, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "TimeOutTone", TimeOutTone, VT_I4, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "Dial", FireDial, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "ReDial", FireReDial, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetVersion", GetVersion, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "SendVideoPause", SendVideoPause, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "SetKeepAliveSerial", SetKeepAliveSerial, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "MakeConference", MakeConference, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "JoinConference", JoinConference, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "InviteConference", InviteConference, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "CloseConference", CloseConference, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "StatusConference", StatusConference, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CSCDialer1Ctrl, "LeaveConference", LeaveConference, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetSupportedVoiceCodec", GetSupportedVoiceCodec, VT_I4, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "SetSupportedVoiceCodec", SetSupportedVoiceCodec, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetSupportedVideoCodec", GetSupportedVideoCodec, VT_I4, VTS_NONE)
	DISP_FUNCTION(CSCDialer1Ctrl, "SetSupportedVideoCodec", SetSupportedVideoCodec, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "SetVoiceCodec", SetVoiceCodec, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetVoiceCodec", GetVoiceCodec, VT_I4, VTS_NONE)	
	DISP_FUNCTION(CSCDialer1Ctrl, "SetSpkVolume", SetSpkVolume, VT_I4, VTS_I4)
	DISP_FUNCTION(CSCDialer1Ctrl, "GetSpkVolume", GetSpkVolume, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CSCDialer1Ctrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSCDialer1Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSCDialer1Ctrl)
	EVENT_CUSTOM("EvError", FireEvError, VTS_I2  VTS_BSTR)
	EVENT_CUSTOM("EvAlert", FireEvAlert, VTS_NONE)
	EVENT_CUSTOM("EvReceiveCall", FireEvReceiveCall, VTS_BSTR)
	EVENT_CUSTOM("EvHangup", FireEvHangup, VTS_NONE)
	EVENT_CUSTOM("EvGetPeerIP", FireEvGetPeerIP, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvConnected", FireEvConnected, VTS_NONE)
	EVENT_CUSTOM("EvUnRegistration", FireEvUnRegistration, VTS_I2)
	EVENT_CUSTOM("EvAntena", FireEvAntena, VTS_I2)
	EVENT_CUSTOM("EvPushButton", FireEvPushButton, VTS_BSTR)
	EVENT_CUSTOM("EvReceiveButton", FireEvReceiveButton, VTS_BSTR)
	EVENT_CUSTOM("EvVolume", FireEvVolume, VTS_I2  VTS_I4)
	EVENT_CUSTOM("EvStatus", FireEvStatus, VTS_I2)
	EVENT_CUSTOM("EvRetrievePhoneBook", FireEvRetrievePhoneBook, VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("EvRegistration", FireEvRegistration, VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvWCAInfo", FireEvWCAInfo, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvRetrieveAudioDeviceList", FireEvRetrieveAudioDeviceList, VTS_BSTR)
	EVENT_CUSTOM("EvSuccess", FireEvSuccess, VTS_I2)
	EVENT_CUSTOM("EvRetrieveSearchUser", FireEvRetrieveSearchUser, VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvForwarding", FireEvForwarding, VTS_I2  VTS_BSTR)
	EVENT_CUSTOM("EvVideoStartInfo", FireEvVideoStartInfo, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvNotice", FireEvNotice, VTS_I2  VTS_BSTR)
	EVENT_CUSTOM("EvReceiveMessage", FireEvReceiveMessage, VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("EvProgress", FireEvProgress, VTS_I2  VTS_I2)
	EVENT_CUSTOM("EvSerialMessage", FireEvSerialMessage, VTS_I2)
	EVENT_CUSTOM("EvSerialConnect", FireEvSerialConnect, VTS_NONE)
	EVENT_CUSTOM("EvSerialDisconnect", FireEvSerialDisconnect, VTS_NONE)
	EVENT_CUSTOM("EvHookOn", FireEvHookOn, VTS_I2)
	EVENT_CUSTOM("EvHookOff", FireEvHookOff, VTS_I2)
	EVENT_CUSTOM("EvVideoCreateInfo", FireEvVideoCreateInfo, VTS_I4)
	EVENT_CUSTOM("EvVideoPause", FireEvVideoPause, VTS_I4)
	EVENT_CUSTOM("EvMakeConference", FireEvMakeConference, VTS_BSTR  VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvJoinConference", FireEvJoinConference, VTS_BSTR  VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvInviteConference", FireEvInviteConference, VTS_BSTR  VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSCDialer1Ctrl, 1)
	PROPPAGEID(CSCDialer1PropPage::guid)
END_PROPPAGEIDS(CSCDialer1Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCDialer1Ctrl, "SCDialer1.SCDialer1Ctrl.1",
	0xe43303fe, 0xbde8, 0x44e2, 0x87, 0xbf, 0x25, 0x9d, 0x92, 0xf, 0x3a, 0xdd)

/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSCDialer1Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSCDialer1 =
		{ 0xe2fa309f, 0x2de, 0x4a35, { 0xb9, 0x77, 0xb9, 0xb7, 0xd3, 0x5, 0xc2, 0xa7 } };

const IID BASED_CODE IID_DSCDialer1Events =
		{ 0x843459f6, 0xb01b, 0x4067, { 0xb1, 0x40, 0xdf, 0xcc, 0xb7, 0x57, 0x7d, 0xab } };

/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSCDialer1OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSCDialer1Ctrl, IDS_SCDialer1, _dwSCDialer1OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::CSCDialer1CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCDialer1Ctrl

BOOL CSCDialer1Ctrl::CSCDialer1CtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_SCDialer1,
			IDB_SCDialer1,
			afxRegApartmentThreading,
			_dwSCDialer1OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::CSCDialer1Ctrl - Constructor

CSCDialer1Ctrl::CSCDialer1Ctrl()
{
	InitializeIIDs(&IID_DSCDialer1, &IID_DSCDialer1Events);

	// TODO: Initialize your control's instance data here.
	m_nState = -1;
	m_323Connection = NULL;

	m_nRTPmode = 64;
	m_uCreatePort = BASERTPPORT;
	m_bCaller = FALSE;
	m_pDataSocketSCDialer1 = NULL;
	m_pDataSocketWCA = NULL;
	m_bRegistered = FALSE;
	m_bLicense = FALSE;
//	m_bCall = FALSE;
	m_bProgressRegistration = FALSE;
	m_bProgressPhonebook = FALSE;
	m_bProgressSignUp = FALSE; // SignUp, Modify
	m_bProgressSearchUser = FALSE; // SearchUser
	m_bProgressBilling = FALSE;
	m_lMiraCallVersion = 0;
	m_bInternalCall = FALSE; //by cje 011206
	m_lSearchUser = 0;
	m_timeConnected = NULL;
	m_strLogin = _T("");
	m_strNotLogin = _T("");
	m_lPhonebookCount = 0;
	m_nRegisterMode = 0; // normal login
	m_bReTryToLogin = TRUE; // _REREGISTRATION 메세지를 받았을 경우는 ReTry 를 하지 않는다.
	m_strTryToLoginIP = _T("");
	m_nProductIDWCA = -1;
	m_nProductID = -1;
	m_strPrevPhoneNumber = _T("");
	m_nPrevCallType = -1;
	m_bAnalyzed = FALSE;	//by cje 020827
	m_pNATAnalyzer = new CNATAnalyzer(); //by cje 020827
	m_nNetworkType = _OPENINTERNET;
	m_nRealNetworkType = _OPENINTERNET;
	m_nPeerNetworkType = _OPENINTERNET;
	m_bFireTalkExist = FALSE;
	m_nRTPTransferType = _NORMAL;
	m_bReAnalyzed = FALSE;
	m_uMediaCount = MAXMEDIACOUNT; //기본 2 pc2phone일때 1로 변경되도록.
//	m_uMediaCount = 1;
	m_bActiveConfirm = FALSE;

	for(int i=0; i< m_uMediaCount;i++)
	{
		m_pNATSoc[i] = NULL;
		m_MediaInfo[i].MediaPort = BASERTPPORT + 2*i;
		m_MediaInfo[i].bSet = FALSE;
		m_PeerMediaInfo[i].bSet = FALSE;
		m_PeerMediaInfo[i].MediaPort = 0;
		m_bMediaActiveConfirm[i] = FALSE;
	}
	m_bNATResulted = FALSE;
	m_bIsPh2PC = FALSE;
	m_nEvConnectedCount = 0; // EvConnected 가 한번만 발생하도록..
	m_lBillingPID = -987564321;
	m_nInterval = 1000;
	m_bIsHookOff = FALSE;

	memset(&m_msg, 0x00, sizeof(MESSAGE));
	memset(&m_msgDIAL, 0x00, sizeof(MESSAGE));
	memset(&m_msgOK, 0x00, sizeof(MESSAGE));
	memset(&m_msgFireTalkAddr, 0x00, sizeof(MESSAGE));
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::~CSCDialer1Ctrl - Destructor

CSCDialer1Ctrl::~CSCDialer1Ctrl()
{
	// TODO: Cleanup your control's instance data here.
	delete m_pNATAnalyzer; //by cje 020827
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::OnDraw - Drawing function

void CSCDialer1Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::DoPropExchange - Persistence support

void CSCDialer1Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	PX_String(pPX, "UserID", m_strUserID, _T(""));
	PX_String(pPX, "PCCallAgentIP", m_strSCDialer1ServerIP, _T(""));
	PX_Long(pPX, "PCCallAgentPort", m_lSCDialer1Port, 8002); 
	PX_String(pPX, "WebCallAgentIP", m_strWebCallAgentIP, _T(""));
	PX_Long(pPX, "WebCallAgentPort", m_nWebCallAgentPort, 8000);
	PX_Short(pPX, "CallType", m_nCallType, 0);
	PX_String(pPX, "PhoneNumber", m_strPhoneNumber, _T(""));
	PX_String(pPX, "PCN", m_strUserPhoneNumber, _T(""));
	PX_Bool(pPX, "AutoAcceptCall", m_bAutoAcceptCall, TRUE);  // Show MessageBox
	PX_String(pPX, "PhoneBookList", m_strPhoneBookList, _T(""));
	PX_Long( pPX, _T("ProxyMode"), m_lProxyMode, 64 );
	PX_Bool(pPX, _T("EnableTelBox"), m_bEnableTelBox, FALSE);
	PX_Short(pPX, _T("AuthType"), m_nAuthType, 0); // 0 : PCN, 1 : ID, 2 : E-Mail, 3 : Name, 4 : Unknown
	PX_Bool(pPX, _T("DialButtonClicked"), m_bCall, FALSE);
	PX_Bool(pPX, _T("RingWave"), m_bRingWave, TRUE);
	PX_Bool(pPX, _T("DTMFWave"), m_bDTMFWave, TRUE);
	PX_Bool(pPX, _T("InBand"), m_bInBand, FALSE);
	PX_String(pPX, _T("LKey"), m_strLKey, _T(""));
	PX_Short(pPX, _T("DeviceType"), m_nDeviceType, 0);
	PX_Short(pPX, _T("SerialPort"), m_nSerialPort, 1);
	PX_Short(pPX, _T("AutoRetryLoginInterval"), m_nAutoRetryLoginInterval, 0);
	PX_Long( pPX, _T("NATServerPort"), m_nNATServerPort, 7999 );
	PX_Long( pPX, _T("NATFireTalkPort"), m_nNATFireTalkPort, 7998 );
	PX_Bool(pPX, _T("ManualRoute"), m_bManualRoute, FALSE);
	// TODO: Call PX_ functions for each persistent custom property.

	/*
	TRACE("UserID :%s\n", m_strUserID);
	TRACE("PCCallAgentIP : %s\n" ,m_strSCDialer1ServerIP);
	TRACE("PCCallAgentPort: %d\n",m_lSCDialer1Port); 
	TRACE("WebCallAgentIP :%s\n", m_strWebCallAgentIP);
	TRACE("WebCallAgentPort :%d\n", m_nWebCallAgentPort);
	TRACE("CallType :%d\n", m_nCallType);
	TRACE("PhoneNumber :%s\n", m_strPhoneNumber);
	TRACE("PCN :%s\n", m_strUserPhoneNumber);
	TRACE("AutoAcceptCall :%d\n", m_bAutoAcceptCall);  // Show MessageBox
	TRACE("PhoneBookList :%s\n", m_strPhoneBookList);
	TRACE("ProxyMode :%d\n", m_lProxyMode );
	TRACE("EnableTelBox :%d\n", m_bEnableTelBox);
	TRACE("AuthType %d\n", m_nAuthType); // 0 : PCN, 1 : ID, 2 : E-Mail, 3 : Name, 4 : Unknown
	TRACE("DialButtonClicked %d\n", m_bCall);
	TRACE("RingWave %d\n", m_bRingWave);
	TRACE("DTMFWave %d\n", m_bDTMFWave);
	TRACE("InBand %d\n", m_bInBand);
	TRACE("LKey :%s\n", m_strLKey);
	TRACE("DeviceType %d\n", m_nDeviceType);
	TRACE("SerialPort %d\n", m_nSerialPort);
	TRACE("AutoRetryLoginInterval %d\n", m_nAutoRetryLoginInterval);
	TRACE("NATServerPort %d\n", m_nNATServerPort);
	TRACE("NATFireTalkPort %d\n", m_nNATFireTalkPort);
	TRACE("ManualRoute %d\n", m_bManualRoute);
	//*/	
	
	if(m_lProxyMode < _SIMPLE || m_lProxyMode > _BITCPROUTE)
		m_lProxyMode = _DIRECT;
	m_nRTPmode = m_lProxyMode;     /* RTPMODE   
											_DIRECT = 0x40,  (64) 
											_UNIUDPROUTE,	(65)
											_BIUDPROUTE,	(66)
											_UNITCPROUTE,	(67)
											_BITCPROUTE,	(68)
											_NOROUTEMODE    = 0x45 
								  */

}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::OnResetState - Reset control to default state

void CSCDialer1Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl::AboutBox - Display an "About" box to the user

void CSCDialer1Ctrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SCDialer1);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1Ctrl message handlers

void CSCDialer1Ctrl::OnPCCallAgentIPChanged() 
{
	// TODO: Add notification handler code
	m_pNATAnalyzer->SetServerInfo(m_hWnd, (char*)(LPCTSTR)m_strSCDialer1ServerIP, m_nNATServerPort); // by cje 020827
	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnPCCallAgentPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnUserIDChanged() 
{
	// TODO: Add notification handler code
	if(m_bCall)
		return;

	SetModifiedFlag();
}

long CSCDialer1Ctrl::OnSocketMessageWCA(WPARAM wParam, LPARAM lParam)
{
	CString tmp;
	switch( WSAGETSELECTEVENT( lParam ))
	{
	case FD_CONNECT:
		TRACE("\nFD_CONNECT\n");
		if( m_pDataSocketWCA ){
			if( m_pDataSocketWCA->OnConnected( lParam ) == 0 ) // if error then -1
			{				
				WCALogIn();
			}
		}
		break;
	case FD_READ:
		m_nCallType = 1;
		memset(&m_msg, 0x00, sizeof(m_msg)); // 2003.11.27 by gomeisa
		TRACE("[OnSocketMessageWCA] [FD_READ] m_nCallType = %d\n", m_nCallType);
		if( m_pDataSocketWCA )
		{
			m_pDataSocketWCA->Receive( (char*)&m_msg, sizeof( m_msg ) );

			TRACE("\n[Protocol] [R] [WCA] %d,%d,%s\n", m_msg.Type, m_msg.Data.dialmsg.RtpPort, m_msg.Data.dialmsg.PhoneNumber);
			switch(m_msg.Type)
			{
			case _ALIVE:
				if(m_nCallType == 1 && m_bCall && m_pDataSocketWCA)
				{
					MESSAGE msg;
					memset(&msg, 0x00, sizeof(msg));
					msg.Type = _ALIVE;
					m_pDataSocketWCA->Send((char*)&msg, sizeof(msg));
				}
				break;
			case _OK:
				m_msg.Type = _REGOK; // WebCallient 에서의  _REQPROXY; // PCCA 와 WCA 의 message type 의 차이로 
				if(m_pDataSocketWCA) m_pDataSocketWCA->Send( (char*)&m_msg, sizeof(m_msg) );		
				SetTimer(2, 10000, NULL); // alive timer
				break;
			case _ROUTERTP: // WebCallient 에서의 _RESPROXY 값: // PCCA 와 WCA 의 message type 의 차이로 
				m_bFireTalkExist = TRUE;
				if(m_pNATSoc[0])
				{
					for(int i=0; i< m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetProxyAddr(m_msg.Data.routemsg.ProxyServerIP, (unsigned)m_msg.Data.routemsg.ProxyServerPort);
					}
					//m_pNATAnalyzer->SetFireTalkInfo(m_msg.Data.routemsg.ProxyServerIP, (unsigned)m_nNATFireTalkPort); //by cje 020725
					m_pNATAnalyzer->SetFireTalkInfo(m_msg.Data.routemsg.ProxyServerIP, (unsigned)m_msg.Data.routemsg.ProxyServerPort); //by cje 021126 (위의건 버그인듯)
					//if(m_pNATSoc[0]->rtpMode >= _UNITCPROUTE)
					if(m_pNATSoc[0]->rtpMode >= _UNIUDPROUTE) //이미 중계모드로 설정이 된상태라면..by cje 030114
					{
						for(int i=0; i<m_uMediaCount;i++)
						{
							m_pNATSoc[i]->SetLocalRTPPort();
							m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
						}
						SendRouteMsg();
					}
					else //자동 모드일 경우..
					{
						if(m_bAnalyzed) //by cje 020725
						{
							for(int i=0; i<m_uMediaCount;i++)
							{
								m_pNATSoc[i]->SetLocalRTPPort();
								m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
							}

							if(m_nNetworkType <= _FULLCONE)
							{
								if(m_pNATAnalyzer->GetNetworkType() == _NAT_CONE)
								{
									for(int i=0; i<m_uMediaCount;i++)
										m_pNATSoc[i]->SetGlobalRTPPort();
								}
								else
								{
									SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
								}
							}
							else 
							{
								for(int i=0; i<m_uMediaCount;i++)
								{
									m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
									if(m_pNATSoc[i]->rtpMode < _UNIUDPROUTE) 
										m_pNATSoc[i]->rtpMode = _UNIUDPROUTE;
									//if(m_pNATSoc[i]->rtpMode < _UNITCPROUTE) 
									//	m_pNATSoc[i]->rtpMode = _UNITCPROUTE; //by cje 021126
								}
								SendRouteMsg();
							}
						}
						else
						{
							if(m_nRTPmode <=_DIRECT)
								m_pNATAnalyzer->ExcuteAnalysis();
							else
							{
								for(int i=0; i<m_uMediaCount;i++)
								{
									m_pNATSoc[i]->SetLocalRTPPort(); //NATSoc Creation
									m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
								}
								SendRouteMsg();
							}
						}
					}
				}
				break;
			case _ROUTEOK: // WebCallient 에서의 _NOPROXY: // PCCA 와 WCA 의 message type 의 차이로 
				m_bFireTalkExist = FALSE;
				if(m_pNATSoc[0])
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->rtpMode = _DIRECT;
						//	AfxMessageBox("NoProxy");
						m_pNATSoc[i]->SetLocalRTPPort();  //by cje 010508
						m_pNATSoc[i]->SetGlobalRTPPort(); //by cje 020104
					}
				}
				break;
			case _RETRIEVE: // WebCallient 에서의 _ROUTEOK: // PCCA 와 WCA 의 message type 의 차이로 
				//connect to proxy & get addr/port
				if(m_pNATSoc[0])
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						if(!(m_pNATSoc[i]->ConnectProxy()))
						{}//AfxMessageBox("Set the Proxy Address First!");
					}
				}
				//NAT클래스에서 모든 로직이 처리..				
				break;			
			case _ALERT:
				FireEvAlert();
				m_nState = _ALERTINGSTATE;
				FireEvStatus(m_nState);

				// PC2Phone 사용시 PCCallAgent 에 자신의 상태를 알린다
				m_nMyStatus = m_nState;
				OnMyStatusChanged();
				
#ifdef	_LOCAL_PLAY_G7231
				if(m_bRingWave)
				{
					m_323Connection->LocalPlayG7231(0, TRUE);
					TRACE("\n [_ALERT from WCA] RingWave Play Start1 ");
				}
#endif
				break;
			case _STARTRTP:
				{
					StartRTP();
					break;
				}
			case _CONNECTED:
				if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
				{
					FireEvConnected();
					m_nEvConnectedCount++;
				}
				
				m_nState = _DIALINGSTATE;
				FireEvStatus(m_nState);

				// PC2Phone 사용시 PCCallAgent 에 자신의 상태를 알린다
				m_nMyStatus = m_nState;
				OnMyStatusChanged();
#ifdef	_LOCAL_PLAY_G7231
//				if(m_bRingWave)
				{
					// Ringback tone 종료
					m_323Connection->LocalPlayG7231(0, FALSE);
					TRACE("\n [CDialerBase] RingWave Stop (OnSocketConnected 1)");
				}
#endif
				PlaySound( NULL, NULL, 0 );  // 만약에 대비한 처리.
				//		method : start timer ... view call time
//				m_startTime = CTime::GetCurrentTime();
				
				SetTimer( 4, 5000, NULL );
				
				// by gomeisa 11.07
				KillTimer(8);
				
				break;
			case _HANGUP:
//				FireEvHangup();
				if(m_nState != _READYSTATE && m_nState != _ERRORSTATE)
				{
					if( !m_323Connection->EndRTPSession(t_Audio, 0) )
					{
						AfxMessageBox("Fail to end channel");
					}
#ifdef	_LOCAL_PLAY_G7231
//					if(m_bRingWave)
					{
						m_323Connection->LocalPlayG7231(0, FALSE);
						TRACE("\n [CDialerBase] RingWave Stop (OnSocketConnected 2)");
					}
#endif
					PlaySound(NULL, NULL, 0);
					// 1 : _HANGUP 을 받았을 때
					FireHangup(1); //by cje
					TRACE("FireEvHangup\n");

					// 아래의 내용은 FireHangup() 에서 실행됨..
//					m_nState = _READYSTATE;
//					FireEvStatus(m_nState);
//					FireEvHangup();
					
				}
				m_nMyStatus = _READYSTATE;
				OnMyStatusChanged();
				KillTimer(4); // billing
				KillTimer(5); // billing
				KillTimer(8);
				break;
			case _ERROR:
				{
					m_bCaller = FALSE;
					m_bCall = FALSE;
					m_bInternalCall = FALSE; //by cje 011206
					
					if(m_nState >= _DIALBLOCK && m_nState != _ERRORSTATE)
					{
						if( !m_323Connection->EndRTPSession(t_Audio, 0) )
						{
							AfxMessageBox("Fail to end channel");
						}
#ifdef	_LOCAL_PLAY_G7231
						{
							m_323Connection->LocalPlayG7231(0, FALSE);
							TRACE("\n [CDialerBase] RingWave Stop (OnSocketError)");
						}
#endif
						PlaySound(NULL, NULL, 0);
						if(m_pNATSoc[0])  //by cje
						{
							KillTimer(32); 
							for(int i=0; i<m_uMediaCount;i++)
							{
								if(m_pNATSoc[i]->rtpMode >= _UNIUDPROUTE) 
									m_pNATSoc[i]->CloseProxy();  //by cje ROUTE
								if(m_pNATSoc[i]->m_bNATCreated)
									m_pNATSoc[i]->Close();
							//	delete m_pNATSoc[i];
							//	m_pNATSoc[i] = NULL;
							}
							SetTimer(40, 500 , NULL) ; 
						}
					}
					m_nState = _ERRORSTATE;
					if(m_msg.Data.ErrorInfo == INCONF)
					{	
						m_323Connection->SerialPhoneMessage(36);
						SetTimer(33, 3000, NULL);
					}	// Send Message to Serial Phone
					
					if(m_msg.Data.ErrorInfo == REFUSE) // Already Logined
					{
						CStringArray array;
						int count = ParseData(m_msg.Data.dialmsg.PhoneNumber, ';', &array);
						if(count > 1)
						{
							FireEvError(m_msg.Data.ErrorInfo, array.GetAt(0));
							m_strPCCallAgentIPAlreadyLogined = array.GetAt(1); // 자신이 로그인 되어 있던 PCCA IP
						}
						else
							FireEvError(m_msg.Data.ErrorInfo, m_msg.Data.dialmsg.PhoneNumber); 
					}
					else
						FireEvError(m_msg.Data.ErrorInfo, m_msg.Data.dialmsg.PhoneNumber);  //proxyproblem은 어떻게 처리? by cje
					
					//				FireEvError(m_msg.Data.ErrorInfo, array.GetAt(0));  //proxyproblem은 어떻게 처리? by cje
					// 등록되지 않은 PCN 이나 Busy 상태일 경우에 PC2Phone 으로 전환시킬때
					// 아래가 실행되면 _READYSTATE 가 표시 된다.
					m_bProgressRegistration = FALSE; // INVALID_ID, INVALID_PWD, REFUSE 일 경우에 REGISTRATION 과정이 끝났음을 알린다.
					
					if(m_msg.Data.ErrorInfo != NO_ANSWER || m_msg.Data.ErrorInfo != INCONF)
					{
						m_nState = _READYSTATE;
						SetTimer(14, 1500, NULL);
						// PC2Phone 으로 forwarding 하는 소스를 여기에 추가하도록 한다.
					}				
				}
				m_nMyStatus = _READYSTATE;
				OnMyStatusChanged();
				KillTimer(4); // billing
				KillTimer(5); // billing
				KillTimer(8);

				break;
			}
		}
		break;
	case FD_CLOSE:
		{
			KillTimer(2); // keepalive for WCA
			KillTimer(4); // billing
			KillTimer(5); // billing
			KillTimer(8); // web2phone dial timeout
			if(m_bCall && m_nCallType == 1 /*web2phone*/)
			{
				if( !m_323Connection->EndRTPSession(t_Audio, 0) ) // error...add later
				{
				}
				FireEvHangup();
				if(m_pNATSoc[0])  //by cje
				{
					KillTimer(32);
					for(int i=0; i<m_uMediaCount;i++)
					{
						if(m_pNATSoc[i]->rtpMode >= _UNIUDPROUTE) 
							m_pNATSoc[i]->CloseProxy();  //by cje ROUTE
						if(m_pNATSoc[i]->m_bNATCreated)
							m_pNATSoc[i]->Close();
				//		delete m_pNATSoc[i];
				//		m_pNATSoc[i] = NULL;
					}
					SetTimer(40, 500 , NULL) ; 
				}	
			}
			if( m_pDataSocketWCA )
				m_pDataSocketWCA->Close();
#ifdef	_LOCAL_PLAY_G7231
	//		if(m_bRingWave)
			{
				m_323Connection->LocalPlayG7231(0, FALSE);
				TRACE("\n [CDialerBase] RingWave Stop (FireHangup)\n");
			}
#endif
			PlaySound(NULL, NULL, 0);
			m_nState = _READYSTATE;
			FireEvStatus(m_nState);
			break;
		}
	}
	return 0L;
}

long CSCDialer1Ctrl::OnSocketMessageSCDialer1(WPARAM wParam, LPARAM lParam)
{
	int i;
	CString tmp;
	CString strTemp;
	switch( WSAGETSELECTEVENT( lParam )){
	case FD_CONNECT:
		if( m_pDataSocketSCDialer1 )
		{
			KillTimer(15);
			if( m_pDataSocketSCDialer1->OnConnected( lParam ) == 0 ) // if error then -1
			{
				if(!GetIPAddress(LocalGWIP,LocalIP)) //by cje 020909 연결된후에 해야 문제가 없다..
				{
					sockaddr_in localaddr;
					int addrlen;
					m_pDataSocketSCDialer1->GetSockName((sockaddr *)&localaddr, &addrlen);
					strcpy(LocalIP,inet_ntoa(localaddr.sin_addr));
				}
				tmp.Format("IP;%s (%s)", LocalIP, LocalGWIP);
				FireEvReceiveMessage("[Serial]", tmp);
					
				if(m_bProgressSignUp)
				{
					CStringArray arraySignUp;
					CString strSignUp;

					if(ParseData(m_strSignUp, ';', &arraySignUp) == 4)
					{
						MESSAGE newmsg;
						memset(&newmsg, 0x00, sizeof(MESSAGE));
						newmsg.Type = _SIGNUP;
						newmsg.Data.signupmsg.step = 0;
						strSignUp.Format("%s;%s", arraySignUp.GetAt(0), arraySignUp.GetAt(1));
						strcpy(newmsg.Data.signupmsg.UserInfo, strSignUp);
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));

						newmsg.Data.signupmsg.step = 1;
						strSignUp.Format("%s;%s;%s", arraySignUp.GetAt(2), arraySignUp.GetAt(3), arraySignUp.GetAt(4));
						strcpy(newmsg.Data.signupmsg.UserInfo, strSignUp);
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
					}
				}
				else
				{
//					Registration();
					m_msg.Type = _REQPROXY; //by cje 020902
					if(m_pDataSocketSCDialer1) 
						m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );
				}
			}
			else if( m_pDataSocketSCDialer1->OnConnected( lParam ) == -1 )
			{
				if(m_bProgressSignUp)
				{
					m_bProgressSignUp = FALSE;
					FireEvProgress(_SIGNUPING, 0);
				}
				else
				{
					m_bCall = FALSE;
					m_nState = _ERRORSTATE;
					FireEvError(NETWORK_ERROR, _T("PCCallAgent connection fail"));
					KillTimer(16);
					if(m_nAutoRetryLoginInterval)
						SetTimer(16, m_nAutoRetryLoginInterval * 1000, NULL);
					m_bProgressRegistration = FALSE;
					m_nState = _READYSTATE;
					SetTimer(14, 1500, NULL);
				}
			}
		}
		break;
	case FD_READ:
//		m_nCallType = 0;
//		TRACE("[OnSocketMessageSCDialer1] [FD_READ] m_nCallType = %d\n", m_nCallType);
		if(NULL == m_pDataSocketSCDialer1 
			|| FALSE == m_pDataSocketSCDialer1->IsConnecting())
		{
			break;
		}

		int nReted;
		nReted = m_pDataSocketSCDialer1->Receive( (char*)&m_msg, sizeof( m_msg ));
		if(nReted > 0)
		{
//			DbgOut("	MessageType(%3d), Connected(%d) Bytes(%d)\n", m_msg.Type, m_pDataSocketSCDialer1->IsConnecting(), nReted);

			switch(m_msg.Type)
			{
			case _SIGNUP:
				if(m_bProgressSignUp)
				{
					if(m_msg.Data.signupmsg.step == 1)
					{
						FireEvSuccess(_SIGNUPING);
					}
					else if(m_msg.Data.signupmsg.step == 2)
					{
						FireEvSuccess(_MODIFYING);
					}
					else
						FireEvError(DBERROR, _T("")); // SignUp, Modify 는 DB error 외엔 없을 듯..
				}
				m_pDataSocketSCDialer1->Close();
				m_bProgressSignUp = FALSE;
				break;
				// 2002.11.16 by gomeisa
			case _ALIVE:
				{
					if(m_bRegistered && m_pDataSocketSCDialer1)
					{
						// 20020221 버전부터는 _PREREGISTRATION 에 버전 정보 송신함
			//			msg.Data.updatemsg.version = m_lMiraCallVersion;
						KillTimer(1);
						MESSAGE msg;
						memset(&msg, 0x00, sizeof(msg));
						msg.Type = _ALIVE;
						m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(msg));
						SetTimer(1, ALIVETIME, NULL);
						TRACE("Send _ALIVE message after receive _ALIVE from server\n");

					}
					else
						KillTimer(1);
					break;
				}
			case _PREREGISTRATION:
				{
					CStringArray arrayServerInfo;
					int count = ParseData(m_msg.Data.registrationmsg.UserPhoneNumber, ';', &arrayServerInfo);
					
					if(count >= 6) // IP ==> WCAIP;WCAPort;WCAPID;BillingIP;BillingPort;BillingPID     Port ==> N/A
					{
						char szTemp[255];
						PHOSTENT pHostEntry; 
						struct in_addr inAddr;
						
						CString strWebCallAgentIP = arrayServerInfo.GetAt(0);
						m_nWebCallAgentPort = atoi(arrayServerInfo.GetAt(1));
						
						if(!strWebCallAgentIP.IsEmpty() && strWebCallAgentIP.GetAt(0) >= '0' && strWebCallAgentIP.GetAt(0) <= '9')
							m_strWebCallAgentIP = strWebCallAgentIP;
						else
						{
							if ( ( pHostEntry = gethostbyname( strWebCallAgentIP ) ) == NULL ) //호스트 이름로 호스트 entry 얻기
								wsprintf( szTemp, "WebCallAgent IP incorrect : %d", WSAGetLastError() );
							else memcpy( &inAddr, pHostEntry->h_addr, 4 );
							
							m_strWebCallAgentIP = inet_ntoa(inAddr);
						}
						
						m_nProductIDWCA = atoi(arrayServerInfo.GetAt(2));
						m_lBillingPID = atol(arrayServerInfo.GetAt(5));
					}
					else
					{
						m_strWebCallAgentIP = _T("");
						m_nWebCallAgentPort = 8000;
					}
					FireEvWCAInfo(m_msg.Data.registrationmsg.UserPhoneNumber, m_msg.Data.registrationmsg.version);
				}
				break;
			case _REREGISTRATION:
				m_bReTryToLogin = FALSE;
				m_strTryToLoginIP.Format("%s", m_msg.Data.registrationmsg.UserPhoneNumber);
				break;
			case _REGOK:
//				AfxMessageBox("regok");
				m_bProgressRegistration = FALSE; // Finish to register to server.
				m_bRegistered = TRUE;
				m_nState = _READYSTATE;
				SetTimer(1, ALIVETIME, NULL);
				FireEvRegistration(m_msg.Data.updatemsg.version, m_msg.Data.updatemsg.Url);
				m_bProgressPhonebook = FALSE;
				FireEvStatus(m_nState);
				break;
			case _OK: // make a call 의 경우
				{
					memcpy(&m_msgOK,&m_msg,sizeof(MESSAGE)); 
					m_msg.Type = _REQPROXY; //by cje 020912 통화전에 FireTalk를 다시한번 확인한다.
					if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );
					break;
				}
			case _ALERT: // make a call 의 경우
				FireEvAlert();
				m_nState = _ALERTINGSTATE;
				FireEvStatus(m_nState);
#ifdef	_LOCAL_PLAY_G7231
				m_323Connection->LocalPlayG7231(0, TRUE);
				TRACE("\n [_ALERT from PCCA(%s)] RingWave Play Start2 ", m_strPhoneNumber);
#endif
				m_bCall = TRUE;
				break;
			case _DIAL: // receive a call 의 경우
			{
				m_uMediaCount = MAXMEDIACOUNT; //기본 2 pc2phone일때 1로 변경되도록.
				TRACE("_DIAL with addr %s:%d\n",m_msg.Data.dialmsg.PhoneNumber,m_msg.Data.dialmsg.RtpPort);
				memcpy(&m_msgDIAL,&m_msg,sizeof(MESSAGE));
				m_nState = _ALERTINGSTATE; // READY보다 큰값..
				FireEvReceiveMessage("[Server]", "ProxyMode");
				m_msg.Type = _REQPROXY; //by cje 020912 통화전에 FireTalk를 다시한번 확인한다.
				if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );

				break;
			}
			case _STARTRTP:  // make a call 의 경우
#ifdef	_LOCAL_PLAY_G7231
				m_323Connection->LocalPlayG7231(0, FALSE);
				TRACE("\n [CDialerBase] RingWave Stop (OnSocketConnected 3)");
#endif
				PlaySound( NULL, NULL, 0 );
				FireEvGetPeerIP(m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);
				TRACE("_STARTRTP %s:%d\n",m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);
				StartRTP();
				TRACE("m_nRTPTransferType : %d\n",m_nRTPTransferType);
				break;
			case _CONNECTED: // receive call 의 경우 : callee 입장일때
				KillTimer(8); //
				m_nState = _DIALINGSTATE;				
#ifdef	_LOCAL_PLAY_G7231
//				if(m_bRingWave)
				{
					m_323Connection->LocalPlayG7231(0, FALSE);
					TRACE("\n [CDialerBase] RingWave Stop (OnSocketConnected 4)");
				}
#endif
				if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
				{
					FireEvConnected();
					m_nEvConnectedCount++;
				}
				FireEvStatus(m_nState);
				TRACE("m_nRTPTransferType : %d\n",m_nRTPTransferType);
				break;
			case _NONFASTSTART:
				// by gomeisa 2002.11.04 : non-faststart ph2pc 지원을 위한..
				// _DIAL 메세지를 보낼때 ip / port 정보를 보내야 하는데 Non faststart 일 경우는 정보가 없으므로 ip 대신 "ph2pc" 로 보낸다
				if(!m_strPeerIP.Compare("ph2pc"))
				{
					m_bIsPh2PC = TRUE;
					CStringArray arrayData;
					//Caller의 IP와 PCN이 함께 넘어온다.. Svr의 Dial 함수에서 세팅..
					int count = ParseData(m_msg.Data.nonfaststartmsg.RTPAddr, ';', &arrayData);
					
					if(count > 0)
						m_strPeerIP = arrayData.GetAt(0);
					if(count > 1)
						m_strCallerPCN = arrayData.GetAt(1);
					m_nState = _DIALINGSTATE;
					FireEvStatus(m_nState);
					
					PlaySound( NULL, NULL, 0 );
					FireEvGetPeerIP(m_strPeerIP/*m_msg.Data.dialmsg.PhoneNumber*//*IP Address*/, m_msg.Data.nonfaststartmsg.RtpPort);
					m_nPeerPort = m_msg.Data.nonfaststartmsg.RtpPort;

					char CallerAddr[40];
					UINT PeerPort[MAXMEDIACOUNT];
					//StartRTPSession에 사용할 상대방 주소..
					
					//StartRTPSession
					if(m_bInternalCall)
					{
						strcpy(CallerAddr, m_strPeerIP);
						PeerPort[0] = m_nPeerPort;
						for(i=1; i<m_uMediaCount;i++)
							PeerPort[i] = m_PeerMediaInfo[i].MediaPort;
					}
					else
					{
						if((m_pNATSoc[0]->rtpMode != _BITCPROUTE)&&(m_pNATSoc[0]->rtpMode != _BIUDPROUTE))
						{
							strcpy(CallerAddr, m_strPeerIP);
							PeerPort[0] = m_nPeerPort;
							for(i=1; i<m_uMediaCount;i++)
								PeerPort[i] = m_PeerMediaInfo[i].MediaPort;
						}
						else if(m_pNATSoc[0]->rtpMode == _BITCPROUTE)
						{
							//상대방의 주소를 프록시에게 보낸다
							BOOL rvalue;
							rvalue =m_pNATSoc[0]->SendGWAddr(m_strPeerIP,m_nPeerPort);
							TRACE("SendGWAddr1 %s:%d, rvalue %d\n",m_strPeerIP,m_nPeerPort, rvalue);
							
							//return이 FALSE이면 문제가된다..
							
							//udp->TCP
							
							strcpy(CallerAddr, "127.0.0.1");
							PeerPort[0] = m_pNATSoc[0]->GetRouteSockPort();
							for(i=1; i<m_uMediaCount;i++)
							{
								m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
								PeerPort[i] = m_pNATSoc[i]->GetRouteSockPort();
							}
						}
						else  // BIUDPROUTEstartrtpsession to
						{
							BOOL rvalue;
							rvalue = m_pNATSoc[0]->SendGWAddr(m_strPeerIP,m_nPeerPort);  //by cje 010705
							TRACE("SendGWAddr1 %s:%d, rvalue %d\n",m_strPeerIP,m_nPeerPort, rvalue);
							
							//strcpy(CallerAddr, (char*)(LPCTSTR)m_pNATSoc[0]->GetProxyAddr());
							//PeerPort[0] = m_uCalleeRTPPort;
							//for(i=1; i<m_uMediaCount;i++)
							//{
							//	m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
							//	PeerPort[i] = m_MediaInfo[i].MediaPort; //m_pNATSoc[i]->GetProxyPort()와 같은 값.
							//}
							strcpy(CallerAddr,"127.0.0.1");
							PeerPort[0] = m_pNATSoc[0]->GetLocalRTPPort();
							for(i=1; i<m_uMediaCount;i++)
							{
								m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
								PeerPort[i] = m_pNATSoc[i]->GetLocalRTPPort();
							}//by cje 021126
						}
						
					}
					
					tmp.Format("StartRTPSession to %s:%d\n",CallerAddr, PeerPort[0]);
					TRACE(tmp);
					if( !m_323Connection->StartRTPSession( CallerAddr, PeerPort[0], t_Audio, 0) ){
						// 2 : StartRTPSession 을 실패했을 때
						FireHangup(2);
						AfxMessageBox("SoundCard already used by another application.");
					}
					else // ph2pc 에서 callee 가 H.323 CONNECT 를 줄 수 있도록 하기 위한 메세지
					{
						CString strTemp;
						strTemp.Format("DesInfo;%s:%d", CallerAddr, PeerPort[0]);
						if(m_uMediaCount > 1)
						{
							for(i=1; i< m_uMediaCount;i++)
							{
								if(i == 1) 
								{
									FireEvVideoStartInfo(CallerAddr, PeerPort[i]);
									strTemp.Format("DesInfo;%s:%d/%d", CallerAddr, PeerPort[0], PeerPort[i]);
								}
							}
						}
						FireEvReceiveMessage("[Serial]", strTemp);
						
						m_timeConnected = CTime::GetCurrentTime();
						if(m_pDataSocketSCDialer1)
						{
							MESSAGE newmsg;
							memset(&newmsg, 0x00, sizeof(MESSAGE));
							newmsg.Type = _COMPLETE;
							KillTimer(8);
							m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
							if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
							{
								FireEvConnected();
								m_nEvConnectedCount++;
							}
						}
					}
					
				}
				break;
			case _HANGUP:
				TRACE("rcv _HANGUP\n");

				if(m_nState != _READYSTATE && m_nState != _ERRORSTATE)
				{
//					if( !m_pMcWrap->EndChannel() )
					if( !m_323Connection->EndRTPSession(t_Audio, 0) )
					{
						AfxMessageBox("Fail to end channel");
					}
#ifdef	_LOCAL_PLAY_G7231
//					if(m_bRingWave)
					{
						m_323Connection->LocalPlayG7231(0, FALSE);
						TRACE("\n [CDialerBase] RingWave Stop (OnSocketConnected 5)");
					}
#endif
					PlaySound(NULL, NULL, 0);
					// 1 : _HANGUP 을 받았을 때
					FireHangup(1); //by cje
					TRACE("FireEvHangup\n");
				}
				KillTimer(8);
				break;
			case _ERROR:
				TRACE("\nReceive from PCCA _ERROR = %d / %s, m_nState = %d\n", m_msg.Data.ErrorInfo, m_msg.Data.moneymsg.Money, m_nState);
				m_bCaller = FALSE;
				m_bCall = FALSE;
				m_bInternalCall = FALSE; //by cje 011206

				KillTimer(8); // pc2pc call timeout
				if(m_nState >= _DIALBLOCK && m_nState != _ERRORSTATE)
				{
					if( !m_323Connection->EndRTPSession(t_Audio, 0) )
					{
						AfxMessageBox("Fail to end channel");
					}
#ifdef	_LOCAL_PLAY_G7231
					{
						m_323Connection->LocalPlayG7231(0, FALSE);
						TRACE("\n [CDialerBase] RingWave Stop (OnSocketError) from PCCA");
					}
#endif
					PlaySound(NULL, NULL, 0);
					if(m_pNATSoc[0])  //by cje
					{
						KillTimer(32); 
						for(int i=0; i<m_uMediaCount;i++)
						{
							if(m_pNATSoc[i]->rtpMode >= _UNIUDPROUTE) 
								m_pNATSoc[i]->CloseProxy();  //by cje ROUTE
							if(m_pNATSoc[i]->m_bNATCreated)
								m_pNATSoc[i]->Close();
					//		delete m_pNATSoc[i];
					//		m_pNATSoc[i] = NULL;
						}
						SetTimer(40, 500 , NULL) ; 
					}
				}
				m_nState = _ERRORSTATE;
				if(m_msg.Data.ErrorInfo == INCONF)
				{	
					m_323Connection->SerialPhoneMessage(36);
					SetTimer(33, 3000, NULL);
				}	// Send Message to Serial Phone

				if(m_msg.Data.ErrorInfo == REFUSE) // Already Logined
				{
					CStringArray array;
					int count = ParseData(m_msg.Data.dialmsg.PhoneNumber, ';', &array);
					if(count > 1)
					{
						FireEvError(m_msg.Data.ErrorInfo, array.GetAt(0));
						m_strPCCallAgentIPAlreadyLogined = array.GetAt(1); // 자신이 로그인 되어 있던 PCCA IP
					}
					else
						FireEvError(m_msg.Data.ErrorInfo, m_msg.Data.dialmsg.PhoneNumber); 
				}
				else if(m_msg.Data.ErrorInfo == NO_ANSWER)
				{
					FireEvError(m_msg.Data.ErrorInfo, (char*)(&m_msg.Data.ErrorInfo+1) );
					TRACE("[FireEvError] No Answer in _ERROR %s\n", (&m_msg.Data.ErrorInfo+1));
				}
				else
				{
					// 2003.08.11 by gomeisa : invalid id / pwd 인 경우, 로그인 안한 상태에서 no permission 인 경우는 
					// m_bReTryToLogin 을 FALSE 로 만들어야 FD_CLOSE 시 재로그인 시도 안함
					if(m_msg.Data.ErrorInfo ==	INVALID_ID || m_msg.Data.ErrorInfo == INVALID_PWD || (!m_bRegistered && m_msg.Data.ErrorInfo == NOPERMISION) || m_msg.Data.ErrorInfo == SECURITYDENIED)
					{
						m_bReTryToLogin = FALSE;
						m_strTryToLoginIP = "Server";
					}
					FireEvError(m_msg.Data.ErrorInfo, m_msg.Data.dialmsg.PhoneNumber);  //proxyproblem은 어떻게 처리? by cje
				}

//				FireEvError(m_msg.Data.ErrorInfo, array.GetAt(0));  //proxyproblem은 어떻게 처리? by cje
				// 등록되지 않은 PCN 이나 Busy 상태일 경우에 PC2Phone 으로 전환시킬때
				// 아래가 실행되면 _READYSTATE 가 표시 된다.
				m_bProgressRegistration = FALSE; // INVALID_ID, INVALID_PWD, REFUSE 일 경우에 REGISTRATION 과정이 끝났음을 알린다.

				if(m_msg.Data.ErrorInfo != NO_ANSWER || m_msg.Data.ErrorInfo != INCONF)
				{
					m_nState = _READYSTATE;
					SetTimer(14, 1500, NULL);
					// PC2Phone 으로 forwarding 하는 소스를 여기에 추가하도록 한다.
				}

				break;
			case _RESPROXY:
				{
					m_bFireTalkExist = TRUE;
					m_pNATAnalyzer->SetFireTalkInfo(m_msg.Data.routemsg.ProxyServerIP, (unsigned)m_nNATFireTalkPort); //by cje 020827
					//FireTalk의 주소를 저장해야한다..나중에 NATSoc에 전달해야함...
					memcpy((void*)&m_msgFireTalkAddr,&m_msg,sizeof(MESSAGE));
					if(!m_bAnalyzed)
						m_pNATAnalyzer->ExcuteAnalysis(); //by cje 020902
					else if(m_nState >= _DIALBLOCK && m_nState != _ERRORSTATE)
					{
						if(m_bCaller)
							OnSocketOK();
						else
							OnSocketDial();
					}
					else
						Registration();
				}
				break;
			case _NOPROXY:
				{
					m_bFireTalkExist = FALSE;
					if(m_nState >= _DIALBLOCK)
					{	
						if(m_bCaller)
							OnSocketOK();
						else
							OnSocketDial();
					}
					else
					{
						m_nNetworkType = _NOINFO;
						Registration();										
					}
				}
				break;
			case _ROUTEOK:
				TRACE("_ROUTEOK\n");
				if(!m_pNATSoc[0])
				{
					// 3 : NAT Socket 이 NULL 일 때
					FireHangup(3);
					break;
				}
				for(i=0; i<m_uMediaCount;i++)
				{
					if(!(m_pNATSoc[i]->ConnectProxy())) 
						TRACE("Set the Proxy Address First!");
				}
				break;
			case _PHONEBOOK:
				{
					CStringArray arrayPhoneBookList;
					CString strResult;
					strResult.Format("%s", m_msg.Data.phonebookmsg.PhoneBook);

//					CString strLogin;
//					CString strNotLogin;
					
					int count = ParseData(m_strPhoneBookList, ';', &arrayPhoneBookList);

					m_bProgressPhonebook = FALSE;
					if(count != (int)m_msg.Data.phonebookmsg.count)
					{
#ifdef _DEBUG
							CString str; str.Format("not match : m_strPhoneBookList=%d, count=%d", count, m_msg.Data.phonebookmsg.count);
							DBGOUT("[pb] %s\n", str);
#endif
						FireEvRetrievePhoneBook(_T(""), _T(""));
						m_strLogin.Empty(); // 초기화
						m_strNotLogin.Empty(); // 초기화
						m_lPhonebookCount = 0; // 초기화
						return -1L;
					}

					for(int i = 0; i < strResult.GetLength(); i++)
					{
						if(strResult.GetAt(i) == 'Y')
						{
							m_strLogin += arrayPhoneBookList.GetAt(m_lPhonebookCount + i);
#ifdef _DEBUG
							DBGOUT("[pb] strLogin=%s is yes\n", arrayPhoneBookList.GetAt(m_lPhonebookCount + i));
#endif
							m_strLogin += ";";
						}
						else
						{
							m_strNotLogin += arrayPhoneBookList.GetAt(m_lPhonebookCount + i);
#ifdef _DEBUG
							DBGOUT("[pb] strLogin=%s is no\n", arrayPhoneBookList.GetAt(m_lPhonebookCount + i));
#endif
							m_strNotLogin += ";";
						}
					}
					m_lPhonebookCount += strResult.GetLength();

					if(m_lPhonebookCount == (long)m_msg.Data.phonebookmsg.count)
					{
#ifdef _DEBUG
							CString str; str.Format("success count=%d", count);// AfxMessageBox(str);
							DBGOUT("[pb] %s\n", str);
#endif

						FireEvRetrievePhoneBook(m_strLogin, m_strNotLogin);
						m_strLogin.Empty(); // 초기화
						m_strNotLogin.Empty(); // 초기화
						m_lPhonebookCount = 0; // 초기화
						FireEvSuccess(_PHONEBOOKING);
					}
				}
				break;
			case _SEARCHUSER:
				if(!m_bProgressSearchUser)
					break;

				if(m_lSearchUser < (long)m_msg.Data.searchusermsg.type)
				{
					m_strSearchUserList += m_msg.Data.searchusermsg.UserInfo;
					m_strSearchUserList += ";";
					m_lSearchUser++;
				}
				else
				{
					FireEvRetrieveSearchUser(m_msg.Data.searchusermsg.type, m_strSearchUserList);
					m_lSearchUser = 0;
					m_strSearchUserList.Empty();
					m_bProgressSearchUser = FALSE;
					FireEvProgress(_SEARCHUSERING, 0);
				}
				break;
			case _FORWARDING:
				if(!m_bCall)
					break;
				FireEvForwarding((short)m_msg.Data.forwardingmsg.type, m_msg.Data.forwardingmsg.Forwarding);				
				break;
			case _VIDEOMSG:
				if(!m_bRegistered)
					break;
				for(i=1 ;i<m_uMediaCount;i++)
				{
					if(!m_PeerMediaInfo[i].bSet)
					{
						m_PeerMediaInfo[i].MediaPort = m_msg.Data.startrtpmsg.RtpPort;
						m_PeerMediaInfo[i].bSet = TRUE;
						break;
					}
					
				}
				break;
			case _NOTICE:
				FireEvNotice((short)m_msg.Data.noticemsg.Type, m_msg.Data.noticemsg.Message);
				break;
			case _ASK_ACTIVE_CONFIRM: //by cje 021112  passive side
				TRACE("OnReceived _ASK_ACTIVE_CONFIRM\n");
				if(!m_bActiveConfirm)
				{
					TRACE("Send _RETRY_ACTIVE_CONFIRM \n");
					m_msg.Type = _RETRY_ACTIVE_CONFIRM;
					m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );//ask retry
				}
				else
				{	
					//close NATSocket
					for(i=0 ; i<m_uMediaCount; i++)
					{
						if(m_pNATSoc[i]->m_bNATCreated)
							m_pNATSoc[i]->Close();
						m_pNATSoc[i]->m_bNATCreated = FALSE;
					}
					//CreateRTPSession
					if(!m_323Connection->CreateRTPSession(m_pNATSoc[0]->GetLocalRTPPort(), t_Audio, 0)) 
					{
						m_msg.Type = _ERROR;
						if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&m_msg, sizeof(MESSAGE));
						// 4 : CreateRTPSession 이 실패했을 때
						FireHangup(4);
						AfxMessageBox("m_323Connection Create fail");
						break;
					}
					//일단은 Video 1개만 적용하는 것이므로
					FireEvVideoCreateInfo((long)m_pNATSoc[1]->GetLocalRTPPort());

					//send _ACTIVE_CONFIRM_OK
					TRACE("Send _ACTIVE_CONFIRM_OK \n");
					m_msg.Type = _ACTIVE_CONFIRM_OK;
					m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );//ask retry
				}
				break;
			case _RETRY_ACTIVE_CONFIRM: //active side
				TRACE("OnReceived _RETRY_ACTIVE_CONFIRM \n");
				//SendActiveConfirm
				if(m_pNATSoc[0])
				{
					for(i=0; i < m_uMediaCount ; i++)
						m_pNATSoc[i]->SendActiveConfirm();
				}
				
				//SetTimer
				SetTimer(31,100,NULL);
				break;
			case _ACTIVE_CONFIRM_OK: //active side
				TRACE("OnReceived _ACTIVE_CONFIRM_OK \n");
				
				//Close NATSocket
				for(i=0 ; i<m_uMediaCount; i++)
				{
					if(m_pNATSoc[i]->m_bNATCreated)
						m_pNATSoc[i]->Close();
					m_pNATSoc[i]->m_bNATCreated = FALSE;
				}
				
				//CreateRTPSession
				if(!m_323Connection->CreateRTPSession(m_pNATSoc[0]->GetLocalRTPPort(), t_Audio, 0)) 
				{
					m_msg.Type = _ERROR;
					if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&m_msg, sizeof(MESSAGE));
					// 4 : CreateRTPSession 이 실패했을 때
					FireHangup(4);
					AfxMessageBox("m_323Connection Create fail");
					break;
				}
				FireEvVideoCreateInfo((long)m_pNATSoc[1]->GetLocalRTPPort());
				
				//StartRTPSession
				if( !m_323Connection->StartRTPSession( (char*)(LPCTSTR)m_pNATSoc[0]->m_strPassivePeerIP, m_pNATSoc[0]->m_uPassivePeerPort, t_Audio, 0) ){
					// 2 : StartRTPSession 을 실패했을 때
					FireHangup(2);
					AfxMessageBox("SoundCard already used by another application.");
					break;
				}
				FireEvVideoStartInfo(m_pNATSoc[1]->m_strPassivePeerIP,m_pNATSoc[1]->m_uPassivePeerPort);
				strTemp.Format("DesInfo;%s:%d/%d", m_pNATSoc[0]->m_strPassivePeerIP, m_pNATSoc[0]->m_uPassivePeerPort, m_pNATSoc[1]->m_uPassivePeerPort);
				FireEvReceiveMessage("[Serial]", strTemp);

				//send _PASSIVE_START
				TRACE("Send _PASSIVE_START \n");
				m_msg.Type = _PASSIVE_START;
				m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );//ask retry
				
				break;
			case _PASSIVE_START: //passive side
				TRACE("OnReceived _PASSIVE_START \n");
				//StartRTPSession
				if( !m_323Connection->StartRTPSession( (char*)(LPCTSTR)m_pNATSoc[0]->m_strActivePeerIP, m_pNATSoc[0]->m_uActivePeerPort, t_Audio, 0) ){
					// 2 : StartRTPSession 을 실패했을 때
					FireHangup(2);
					AfxMessageBox("SoundCard already used by another application.");
					break;
				}
				FireEvVideoStartInfo(m_pNATSoc[1]->m_strActivePeerIP,m_pNATSoc[1]->m_uActivePeerPort);
				
				strTemp.Format("DesInfo;%s:%d/%d", m_pNATSoc[0]->m_strActivePeerIP, m_pNATSoc[0]->m_uActivePeerPort, m_pNATSoc[1]->m_uActivePeerPort);
				FireEvReceiveMessage("[Serial]", strTemp);

				break;
			case _TEXT:
				{
					CString strTemp;
					char buf[512];
					memset(buf, 0x00, 512);

					if(m_msg.Data.textmsg.length > MAXDATALENGTH)
					{
						memcpy(buf, m_msg.Data.textmsg.Message, MAXDATALENGTH);
						m_pDataSocketSCDialer1->Receive(buf+MAXDATALENGTH, m_msg.Data.textmsg.length - MAXDATALENGTH);						
					}
					else
					{
						memcpy(buf, m_msg.Data.textmsg.Message, m_msg.Data.textmsg.length);
					}
					CStringArray array;
					int count = ParseData(buf, ';', &array);
					if(count < 2)
					 	break;
					strTemp.Format("%s", buf);
					if(strTemp.GetLength() == m_msg.Data.textmsg.length)
					{
						FireEvReceiveMessage(array.GetAt(0), strTemp.Right(strTemp.GetLength() - array.GetAt(0).GetLength() - 1));
//						DbgOut("[_TEXT] FireEvReceiveMessage(%s, %s)\n", array.GetAt(0), strTemp.Right(strTemp.GetLength() - array.GetAt(0).GetLength() - 1));
					}
					else
					{
//						DbgOut("[_TEXT] message length error : src %d - result %d\n", m_msg.Data.textmsg.length, strTemp.GetLength());
					}
					
/*					
					if(m_msg.Data.textmsg.length > (unsigned)m_strText.GetLength())
					{
						strTemp.Format("%s", m_msg.Data.textmsg.Message);
						CStringArray array;
						int count = ParseData(strTemp, ';', &array);
						if(count != 2)
							break;
						m_strText += array.GetAt(1);
						if((unsigned)m_strText.GetLength() >= m_msg.Data.textmsg.length)
						{
							FireEvReceiveMessage(array.GetAt(0), m_strText);
//							DbgOut("\n[Text] FireEvReceiveMessage(%s, %s)\n", array.GetAt(0), m_strText);
							m_strText.Empty();
							break;
						}
					}	
					//*/
					break;
				}
			case _VIDEOPAUSE:
				FireEvVideoPause(m_msg.Data.videopausemsg.Flag);
				break;
			case _MONEY:
				{
					m_nInterval = (short)m_msg.Data.moneymsg.Interval;
					// 통화중이라면 KillTimer(4, 5) 후에 SetTimer(5) 한다.
					KillTimer(4);
					KillTimer(5);
					CString strMoney; strMoney.Format("%s;%s", "Money", m_msg.Data.moneymsg.Money);
					FireEvReceiveMessage("[Server]", strMoney);
					if(m_bCall && m_nCallType == 1 && m_lBillingPID == 987654321)
					{
						SetTimer(5, m_nInterval*1000, NULL);
					}
				}
				break;
			case _UPDATE:
				{
					CString strMsg;
					strMsg.Format("update;%d;%s", m_msg.Data.updatemsg.version, m_msg.Data.updatemsg.Url);
					FireEvReceiveMessage("[Server]", strMsg);
				}
				break;
			case _RESMAKECONFERENCE:
				{
					FireEvMakeConference(m_msg.Data.resmakeconferencemsg.FireTalkIP, m_msg.Data.resmakeconferencemsg.FireTalkPort, "");
					break;
				}
			case _RESJOINCONFERENCE:
				{
					FireEvJoinConference(m_msg.Data.resmakeconferencemsg.FireTalkIP, m_msg.Data.resmakeconferencemsg.FireTalkPort, "");
					break;
				}
			case _REQINVITECONFERENCE:
				{
					FireEvInviteConference(m_msg.Data.reqinviteconferencemsg.InviteUserID, "");
					break;
				}
			}
		}
		break;
	case FD_CLOSE:
		/*
		전화 중일 경우에 RTP 처리에 대한 부분이 없음 2002.04.26
		Web2MiraCall, Web2Phone 통화를 종료해야함..
		*/
		KillTimer(8); // pc2pc timeout
		if(m_nState != _READYSTATE)
		{
			// 5 : FD_CLOSE 이벤트 발생시
			FireHangup(5);
		}
		m_bRegistered = FALSE;
		// _REREGISTRATION 을 받았을 때만 m_bReTryToLogin 값이 FALSE 로 바뀜.
		FireEvUnRegistration((m_bReTryToLogin ? 1 : 2));  // 0 : No Retry, 1 : Retry, 2 : No Retry because login somewhere
		if(!m_bReTryToLogin)
		{
			FireEvError(LOGINSOMEWHERE, m_strTryToLoginIP);
		}
		// FD_CLOSE 가 다른 곳에서 연결되어 끊어진 것이 아니라면 자동 로그인 옵션을 체크해서 타이머 시작
		else if(m_nAutoRetryLoginInterval)
		{
			KillTimer(16);
			SetTimer(16, m_nAutoRetryLoginInterval * 1000, NULL);
		}

		m_bReTryToLogin = TRUE; 
		m_strTryToLoginIP = _T("");
//		DbgOut("[SCDialer1] EvUnRegistration in FD_CLOSE\n");
		m_bProgressRegistration = FALSE; // 등록 진행 중에 또 다른 등록 명령을 막는다.
		m_bProgressPhonebook = FALSE; // 폰북 정보 가져 오는 중에 또 다른 폰북 명령을 막는다.
		m_bProgressSignUp = FALSE; // SignUp, Modify
		m_bProgressSearchUser = FALSE; // SearchUser
		m_bProgressBilling = FALSE;

		KillTimer(1); // _ALIVE
#ifdef	_LOCAL_PLAY_G7231
//		if(m_bRingWave)
		{
			m_323Connection->LocalPlayG7231(0, FALSE);
			TRACE("\n [CDialerBase] RingWave Stop (FireHangup)");
		}
#endif
		PlaySound(NULL, NULL, 0);
		break;
	}

	return 0L;
}

int CSCDialer1Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pNATAnalyzer->SetServerInfo(m_hWnd, (char*)(LPCTSTR)m_strSCDialer1ServerIP, m_nNATServerPort); // by cje 020827	

	OSVERSIONINFO	os;
	memset(&os, 0x00, sizeof(OSVERSIONINFO));
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&os);
	
	if(os.dwPlatformId != VER_PLATFORM_WIN32_NT)
	{
//		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		
//		TRACE("\n[OnCreate] SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)");
	}
	else
	{
		if(m_nDeviceType != 0) // AudioDevice 가 아니면
		{
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);		
//			TRACE("\n[OnCreate] SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)");
		}	
		else
		{
			SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		
//			TRACE("\n[OnCreate] SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)");
		}
	}

	// TODO: Add your specialized creation code here
	m_pDataSocketSCDialer1 = new DataSocket;
	m_pDataSocketWCA = new DataSocket(UM_WCASOCKET);

	if(m_nDeviceType == 0)			// Sound Card
	{
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
		m_323Connection = new CH323Connection(this, t_G7231, t_NULL, m_bInBand, 2, 10, GetSafeHwnd(), FALSE, 1); // Multimedia control construct
	}
	else// if(m_nDeviceType == 1)		// Serial Phone
	{
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
		m_323Connection = new CH323Connection(this, t_G7231, t_NULL, m_bInBand, 2, 10, GetSafeHwnd(), TRUE, m_nSerialPort); // Multimedia control construct
	}
	if(m_323Connection)
	{
		m_323Connection->SetKeepAliveSerial(m_strKeepAlive);
	}

	// 2002.12.10 by gomeisa : GetAudioDevice 메소드를 호출해야만 FireEvRetrieveAudioDeviceList 가 밸생되도록 수정한다.
//	FireEvRetrieveAudioDeviceList(m_323Connection->CheckAudioDevice());
	//----------------- 2002.12.10 by gomeisa ---------------->

	// 최초에는 Not login 으로 표시되게끔한다.
//	m_nState = _READYSTATE;
	FireEvStatus(m_nState);

	return 0;
}

BOOL CSCDialer1Ctrl::Registration()
{
	// union 구조체가 max 50 이므로 Encryption 되었을 때는 문제가 있으므로..
/*	if(m_strUserPhoneNumber.GetLength() < 50)
	{
		m_msg.Type = _REGISTRATION;
		strcpy( m_msg.Data.registrationmsg.UserPhoneNumber, m_strUserPhoneNumber);
		m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );
	}
	else
	{
*/		
		CStringArray arrayPCN;
		CString strPCN = _T("");
		CString strPWD = _T("");

		int count = 0;

		// encrytion 되지 않았으면 1219 버전. 타회사 SCDialer1 제공시에는 encode 없이 값이 온다.
		if(m_strUserPhoneNumber.Find(",", 0) != -1) // encryption 이 안되었을 경우 구분자는 ','
		{
			count = ParseData(m_strUserPhoneNumber, ',', &arrayPCN);
			CCode2 code;
			if(count >= 2)
			{
				strPWD = code.Encode(arrayPCN.GetAt(1));
				strPCN = code.Encode(arrayPCN.GetAt(0));
			}
		}
		else
		{
			count = ParseData(m_strUserPhoneNumber, ';', &arrayPCN); // encryption 되었을 경우 구분자는 ';'
			if(count >= 2)
			{
				strPWD = arrayPCN.GetAt(1);
				strPCN = arrayPCN.GetAt(0);
			}
		}

		if( !m_pDataSocketSCDialer1 || !m_pDataSocketSCDialer1->IsConnecting() || count >= 4)
		{
			m_bProgressRegistration = FALSE;
			return FALSE; 
		}

		// 2002.10.31 by gomeisa : 모듈 버전은 직접 판단한다.
/*		if(count >= 3)
		{
			m_lMiraCallVersion = atoi(arrayPCN.GetAt(2)); // _PREREGISTRATION 에서 Server로 전송 --> _PREREGISTRATION 에서 전송 2002.02.21
			// 타 회사에 SCDialer1 모듈 제공시는 SCDialer1 version 표기.
		}
*/
		CStringArray arrayVersion;
		int versioncount = ParseData(GetVersion(), ';', &arrayVersion);
		if(versioncount == 3)
		{
			m_lMiraCallVersion = atoi(arrayVersion.GetAt(1));
		}
		CString NetInfo;
		NetInfo.Format(";%s;%d",LocalGWIP,m_nNetworkType);
		strPCN +=NetInfo;//by cje 020902;

		if(count >= 2)
		{
			m_msg.Type = _PREREGISTRATION; // PWD
			// 2002.02.21  MiraCall version 전송. 이전 버전은 pid 를 전송했지만 pid 는 Server 에서 관리
			m_msg.Data.registrationmsg.version = m_lMiraCallVersion;
			strcpy(m_msg.Data.registrationmsg.UserPhoneNumber, strPWD);
			m_pDataSocketSCDialer1->Send((char*)&m_msg, sizeof(m_msg));

//------------------------------------------------------------------------------------>
			char	buf[512];
			memset(buf, 0x00, 512);
			MESSAGE *pNewmsg;			

			pNewmsg = (MESSAGE*)buf;
			
			if(m_nRegisterMode == 1) // 2002.05.21 force to logout and login
			{
				pNewmsg->Type = _REREGISTRATION;
				m_nRegisterMode = 0; // 초기화
			}
			else
				pNewmsg->Type = _REGISTRATION; // PCN
			// PCN, ID, EMAIL, NAME 중 어떤 것으로 로긴할 것인지를 세팅할 부분
			strcpy(pNewmsg->Data.registrationmsg.UserPhoneNumber, strPCN);

			// 2002.11.22 by gomeisa : AuthType + data_length *10 의 값으로 전달된다.
			if(strPCN.GetLength() > MAXDATALENGTH)
			{
				pNewmsg->Data.registrationmsg.version = m_nAuthType + strPCN.GetLength()*10;
				m_pDataSocketSCDialer1->Send(buf, sizeof(MESSAGE) + pNewmsg->Data.textmsg.length - MAXDATALENGTH);
			}
			else
			{
				pNewmsg->Data.registrationmsg.version = m_nAuthType;
				m_pDataSocketSCDialer1->Send(buf, sizeof(MESSAGE));
			}
/*
	char	buf[512];
	memset(buf, 0x00, 512);
	MESSAGE *pNewmsg;

	pNewmsg = (MESSAGE*)buf;
	pNewmsg->Type = _TEXT;
	CString strMessage; strMessage.Format("%s", Message);
	CString strDestination; strDestination.Format("%s", Destination);

	if(strDestination.IsEmpty())
	{
		if(m_bCall) // Destination 이 없을 경우 통화중인지 체크하고 m_strPhoneNumber 가 destination.
			strDestination = m_strPhoneNumber;
		else
			return FALSE; // destination 이 없음.
	}
	
	if((strMessage.GetLength() + strDestination.GetLength() + 1) < 200)
	{
		strDestination += ";" + strMessage;
		pNewmsg->Data.textmsg.length = strDestination.GetLength(); // length 는 실제 message 길이만 준다.
		strcpy(pNewmsg->Data.textmsg.Message, strDestination);
		
		if(pNewmsg->Data.textmsg.length > MAXDATALENGTH)
		{
		}
		else
		{
		}		
//		DbgOut("\n[Text]1 Send : %s\n", pNewmsg->Data.textmsg.Message);
		return TRUE;
	}
*/
		}
		else if(count >=1) // PWD가 없는 경우로 판단, unknown user일 경우
		{
			m_msg.Type = _REGISTRATION;
			m_msg.Data.registrationmsg.version = m_nAuthType;
			strcpy(m_msg.Data.registrationmsg.UserPhoneNumber, strPCN);
			m_pDataSocketSCDialer1->Send((char*)&m_msg, sizeof(m_msg));
		}
		
/*	}*/
		

	return TRUE;
}

void CSCDialer1Ctrl::OnPhoneNumberChanged() 
{
	// TODO: Add notification handler code
	if(m_bCall)
		return;

	SetModifiedFlag();
}


//BOOL CSCDialer1Ctrl::FireDial(LPCTSTR PhoneNumber) 
void CSCDialer1Ctrl::FireDial() 
{
	// TODO: Add your dispatch handler code here

	if(!IsRegistered())
	{
		if(m_nDeviceType != 0)// Serial phone
		{
			m_323Connection->SerialPhoneMessage(37);		// Send/Redial DTMF 취소 메세지
		}
		FireEvError(NOTLOGIN, _T(""));
		return ;
	}
	if(!strlen(m_strPhoneNumber) || strlen(m_strPhoneNumber) > 40)
	{
		FireEvError(INVALID_NUMBER, _T(""));
		return ;
	}
	m_strPrevPhoneNumber = m_strPhoneNumber;
	m_nPrevCallType = m_nCallType;
	m_bCall = TRUE;
	m_bCaller = TRUE;
	m_nState = _DIALBLOCK;
	FireEvStatus(m_nState);
	m_nEvConnectedCount = 0;// EvConnected 가 한번만 발생하도록..

	m_uMediaCount = MAXMEDIACOUNT; //기본 2 pc2phone일때 1로 변경되도록.

	if(m_nCallType == 1) // pc2phone
	{
		m_uMediaCount = 1; //기본 2 pc2phone일때 1로 변경되도록.
//		if(m_nDeviceType != 0) // Serial Phone
			m_323Connection->LocalPlayG7231(4, TRUE);							// Send Button Play

		// PC2Phone 사용시 PCCallAgent 에 자신의 상태를 알린다
		m_nMyStatus = m_nState;
		OnMyStatusChanged();

		WCAConnect(m_strWebCallAgentIP, m_nWebCallAgentPort);
		KillTimer(8);
		SetTimer(8, 120000, NULL);
		return ;
	}
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	msg.Type = _LOGIN;

	msg.Data.loginmsg.Product = m_nProductID;
//////////////////////////////////////////////	사설망의 경우 내부 ip끼리의 음성통화 지원(라우팅없이)
	strcpy( msg.Data.loginmsg.UserID, m_strPhoneNumber+";");
	strcat( msg.Data.loginmsg.UserID, LocalIP );	 //by cje 020909
 /////////////////by cje 011206
	if(m_pDataSocketSCDialer1) 
	{
		m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(msg));
		KillTimer(8);
		SetTimer(8, 120000, NULL);
	}


	return ;
}

void CSCDialer1Ctrl::OnPCNChanged() 
{
	// TODO: Add notification handler code
	if(m_bCall)
		return;

	SetModifiedFlag();
}

/*
1 : _HANGUP 을 받았을 때
2 : StartRTPSession 을 실패했을 때
3 : NAT Socket 이 NULL 일 때
4 : CreateRTPSession 이 실패했을 때
5 : FD_CLOSE 이벤트 발생시
6 : Unregistration from Server
7 : PC2PC 에서 전화를 Accept 하지 않고 끊음
8 : FireTalk연결상태에서 통화중에 FireTalk와의 연결이 끊어졌을때..
    또는 FireTalk Connection Failure/Refused
9 : Client 종료 (close)
10 : 수화기를 내려놓을 때
11 : 전화번호가 틀릴 때
12 : 장치에러
13 : 모듈에서 에러가 났을때..
14 : 이미 dialbuttonclicked 가 true 일때..
15 : error from billing
16 : 장치가 없는 경우
*/
BOOL CSCDialer1Ctrl::FireHangup(short nMode) 
{
	// TODO: Add your dispatch handler code here
	if((CTime::GetCurrentTime().GetTime() - m_timeConnected.GetTime()) < 2)
	{
//		DbgOut("\n[SCDialer1] Call Duration less than 2 sec, Sleep(3000)\n");
		Sleep(3000);
	}
	m_nEvConnectedCount = 0; // EvConnected 가 한번만 발생하도록..
	m_bCaller = FALSE;

	m_bInternalCall = FALSE; //by cje 011206
	m_nRTPTransferType = _NORMAL; //by cje 020912
	m_bReAnalyzed = FALSE;
	m_bNATResulted = FALSE;
	m_bActiveConfirm = FALSE;
	KillTimer(31); //by cje 021126
	KillTimer(4); // billing
	KillTimer(5); // billing
	KillTimer(8); // billing

	DBGOUT("\n[FireHangup] Mode = %d, m_nCallType = %d, m_nStatus = %d\n", nMode, m_nCallType, m_nState);
	
	m_323Connection->EndRTPSession(t_Audio, 0);
	if(m_nDeviceType != 0 ){	m_323Connection->SerialPhoneMessage(35);}	// Send Message to Serial Phone

	if(m_nState >= _DIALBLOCK && m_nState != _ERRORSTATE)
	{
		PlaySound(NULL, NULL, 0);
		/*
		// cmkwon 20020805
		if( !m_323Connection->EndRTPSession(t_Audio, 0) )
		{
			AfxMessageBox("Fail to end channel");
		}
		//*/
		MESSAGE msg;
		memset(&msg, 0x00, sizeof(MESSAGE));
		msg.Type = _HANGUP;
		msg.Data.mystatusmsg.status = nMode;
		if(m_nCallType == 0)
		{
			if(m_pDataSocketSCDialer1) 
				m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(msg));
		}
		else
		{
			if(m_pDataSocketWCA) 
				m_pDataSocketWCA->Send((char*)&msg, sizeof(msg));
		}

#ifdef	_LOCAL_PLAY_G7231
//		if(m_bRingWave)
		{
			m_323Connection->LocalPlayG7231(0, FALSE);
			TRACE("\nRingWave Stop (FireHangup)");
		}
#endif
		if(m_pNATSoc[0])  //by cje
		{
			KillTimer(32); 
			for(int i=0; i<m_uMediaCount;i++)
			{
				if(m_pNATSoc[i]->rtpMode >= _UNIUDPROUTE) 
					m_pNATSoc[i]->CloseProxy();  //by cje ROUTE
				if(m_pNATSoc[i]->m_bNATCreated)
					m_pNATSoc[i]->Close();
		//		delete m_pNATSoc[i];
		//		m_pNATSoc[i] = NULL;
			}	
			SetTimer(40, 500 , NULL) ; 

		}
		for(int i=0 ; i<m_uMediaCount;i++)
		{
			m_MediaInfo[i].bSet=FALSE;
			m_PeerMediaInfo[i].bSet=FALSE;
			m_MediaInfo[i].MediaPort = BASERTPPORT + 2*i;
			m_PeerMediaInfo[i].MediaPort = 0;
			m_bMediaActiveConfirm[i] = FALSE;

		}

		m_nState = _READYSTATE;
		if(m_nCallType == 1) // PC2Phone
		{
			// PC2Phone 사용시 PCCallAgent 에 자신의 상태를 알려야 하는데 
			// 시리얼 폰의 경우 상대방이 끊은 경우는 아직 수화기를 들고 있는 상태이므로 1로
			// 세팅을 하고 AudioDevice 인 경우는 여기서 0 으로 한다.

			// 2003.05.16 by gomeisa : 시리얼폰을 연결해서 사용하더래도 키보드로 전화를 걸고 Hangup 버튼을
			// 직접 눌러 전화를 끊을 수가 있으므로 Hook off 상태인지를 체크한뒤 0 값 세팅 한다.
			if(m_nDeviceType == 0) // AudioDevice
				m_nMyStatus = 0;
			else
			{
				if(m_bIsHookOff)
					m_nMyStatus = 1; // 
				else
					m_nMyStatus = 0;
			}

			OnMyStatusChanged();
		}
		FireEvStatus(m_nState);
	}

	FireEvHangup();
	m_bCall = FALSE;
	m_nCallType = 0;
	
	return TRUE;
}

BOOL CSCDialer1Ctrl::FireRegisterServer(short Mode) 
{
	// TODO: Add your dispatch handler code here
	if(m_bProgressRegistration || m_bRegistered) // 현재 등록이 진행중인 상태라면 또는 이미 등록이 되었다면
		return FALSE;

	if(!m_bLicense)
	{
		SetLicenseKey(m_strLKey);
	}
	
	TRACE("\n =====FireRegisterServer : SetLicenseKey");

	if(!m_bLicense)
	{
		AfxMessageBox("SCDialer1 : UnLicensed usage");
		m_bProgressRegistration = FALSE;
		return FALSE;
	}
	if(m_pDataSocketSCDialer1) 
	{
		if(m_strSCDialer1ServerIP.IsEmpty())
		{
			FireEvError(NETWORK_ERROR, _T("PCCallAgent connection fail"));
			m_bProgressRegistration = FALSE;
		}
		else
		{
			TRACE("\n =====FireRegisterServer : Connect Socket");
			m_nRegisterMode = Mode;
			m_pDataSocketSCDialer1->Connect(GetSafeHwnd(), m_strSCDialer1ServerIP, m_lSCDialer1Port);
			SetTimer(15, 15000, NULL); // Login Timeout 설정
			m_bProgressRegistration = TRUE;
		}
	}
	return TRUE;
}

BOOL CSCDialer1Ctrl::FireUnRegisterServer() 
{
	// TODO: Add your dispatch handler code here
	// 6 : Unregistration from Server
	FireHangup(6);

	if(m_pDataSocketSCDialer1) 
		m_pDataSocketSCDialer1->Close();

	KillTimer(15);
	FireEvUnRegistration(0); // 0 : No Retry, 1 : Retry, 2 : No Retry because login somewhere
	m_bProgressRegistration = FALSE;
	m_bRegistered = FALSE;
	m_bAnalyzed = FALSE;  //by cje 021015

	return TRUE;
}

void CSCDialer1Ctrl::OnAutoAcceptCallChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}


BOOL CSCDialer1Ctrl::FireAcceptCall(short IsAccept) 
{
	// TODO: Add your dispatch handler code here
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));

	// 20020807 cmkwon
	// Callee일 경우는 무시
	// Caller일 경우만 정상적으로 처리
	if(m_bCaller){ return TRUE;}
	
	if(!IsAccept) // Callee 가 전화를 받지 않겠다고 함...(전화 수화기를 들지 않거나 Message창에서 취소 버튼을 누름)
	{ 
		msg.Type = _ERROR;
		if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
		// 7 : PC2PC 에서 전화를 Accept 하지 않고 끊음
		FireHangup(7);
		return -1L;
	}

	if(m_nState == _ALERTINGSTATE)
	{
		SendStartRTPMsg(m_uCalleeRTPPort);
	}
// -------------------------------------------------------------------------------------->	
	return TRUE;
}

int CSCDialer1Ctrl::ParseData(CString str, char sep, CStringArray *array)
{
	int start = 0;
	int step = 0;
	int count = 0;
	array->RemoveAll();

	if(str.Right(1) != sep)
		str += sep;

	do
	{
		step = str.Find(sep, start);
		array->Add(str.Mid(start, step - start));
		start = step + 1;
		count++;
		if(count > 10000)
		{
			count = 0;
			break;
		}
	}while(start < str.GetLength());

	return count;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCDialer1Ctrl::OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
/// \brief		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
/// \author		cmkwon
/// \date		2008-10-22 ~ 2008-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCDialer1Ctrl::OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
{
	FireEvVolume(i_bIsSpkDevice, i_byVolPercent);
}

long CSCDialer1Ctrl::OnNATResult( WPARAM wParam, LPARAM lParam )
{
	// wParam : MediaIndex : 미디어 번호(채널넘버) : audio 0, video 1
	// lParam : 미디어 포트넘버 
	CString tmp;
	int port;
	int i;
	port = (int)lParam;
	if(!m_bNATResulted)
	{
		
		if(port == 0) //FireTalk에 중계요청..(UDP패킷 수신에러발생시)
		{
			tmp.Format("NAT Result port 0 with rtpMode %d\n", m_pNATSoc[0]->rtpMode);
			TRACE(tmp);
			m_bNATResulted = TRUE;
			SendRouteMsg();
		}
		else if(port >0)//포트 매핑 또는 FireTalk로 부터 받은 포트 넘버..통화 진행
		{
			m_MediaInfo[(int)wParam].MediaPort= port;
			m_MediaInfo[(int)wParam].bSet = TRUE;
			m_bNATResulted = TRUE;
			for(i=0; i<m_uMediaCount; i++)
				if(m_bNATResulted) m_bNATResulted = m_MediaInfo[i].bSet;
			if(m_bNATResulted) //모든미디어의 포트를 얻으면 진행하도록..
			{
				if(m_bCaller)
				{
					tmp.Format("SendDialMsg  with port %d & rtpMode %d\n", port, m_pNATSoc[0]->rtpMode);
					TRACE(tmp);
					SendDialMsg((unsigned int)m_MediaInfo[0].MediaPort); //첫번째 미디어의 포트를 넘긴다..
				}
				else
				{
					tmp.Format("OnReceivedDial  with port %d & rtpMode %d\n", port, m_pNATSoc[0]->rtpMode);
					TRACE(tmp);
					OnReceivedDial((unsigned int)m_MediaInfo[0].MediaPort);
				}
			}
		}
		else if(port == -1)//UDP패킷 통신 문제발생 && FireTalk부재시..
		{
			m_bNATResulted = TRUE;
			TRACE("[SCDialer1] You may not hear the otherside's voice\n");
			for(i=0; i<m_uMediaCount; i++)
				m_pNATSoc[i]->rtpMode = _DIRECT;
			
			if(m_bCaller)
			{
				SendDialMsg(BASERTPPORT);
			}
			else
			{
				OnReceivedDial(BASERTPPORT);
			}
		}
	}
	
	if(port == -2) //FireTalk연결상태에서 통화중에 FireTalk와의 연결이 끊어졌을때..
	{
		// 8 : FireTalk연결상태에서 통화중에 FireTalk와의 연결이 끊어졌을때..
		FireHangup(8);
	}
	else if(port == -3) //passive일때
	{
		m_bMediaActiveConfirm[(int)wParam]= TRUE;
		m_bActiveConfirm = TRUE;
		for(int i = 0; i< m_uMediaCount ; i++)
			if(m_bActiveConfirm) m_bActiveConfirm = m_bMediaActiveConfirm[i];
		
/*		if(m_uActiveConfirmCount <= m_uMediaCount)
		{
			if(0 == (int)wParam) //audio일때
			{
				if(!m_323Connection->CreateRTPSession(m_pNATSoc[0]->GetLocalRTPPort(), t_Audio, 0)) 
				{
					m_msg.Type = _ERROR;
					if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&m_msg, sizeof(MESSAGE));
					FireHangup();
					AfxMessageBox("m_323Connection Create fail");
				}
				if( !m_323Connection->StartRTPSession( (char*)(LPCTSTR)m_pNATSoc[0]->m_strActivePeerIP, m_pNATSoc[0]->m_uActivePeerPort, t_Audio, 0) ){
					FireHangup();
					AfxMessageBox("SoundCard already used by another application.");
				}
			}
			else if(1 == (int)wParam)
			{
				//일단은 Video 1개만 적용하는 것이므로
				FireEvVideoCreateInfo((long)m_pNATSoc[(int)wParam]->GetLocalRTPPort());
				FireEvVideoStartInfo(m_pNATSoc[(int)wParam]->m_strActivePeerIP,m_pNATSoc[(int)wParam]->m_uActivePeerPort);
			}
		}
*/	}
	return 0;
}// by cje 010719

void CSCDialer1Ctrl::SendRouteMsg()
{
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	TRACE("SendRouteMsg with m_nCallType(%d)\n",m_nCallType);
	if(m_nCallType == 1 && m_pDataSocketWCA)
	{
		msg.Type = _REGISTRATION; // WCA 의 _ROUTERTP 메세지 (PCCA 와 WCA 의 차이에 의한)
		m_pDataSocketWCA->Send( (char*)&msg, sizeof(msg) );
	}
	else if(m_nCallType == 0 && m_pDataSocketSCDialer1) 
	{
		msg.Type = _ROUTERTP;
		m_pDataSocketSCDialer1->Send( (char*)&msg, sizeof(msg) );
	}
}

void CSCDialer1Ctrl::SendDialMsg(unsigned int rtpport)//rtpport :상대방에게 전달할 첫번째미디어포트(audio)
{
	int i;
	m_nState = _DIALBLOCK;
	FireEvStatus(m_nState);
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	msg.Type = _DIAL;

	CString strNewType; //by cje 020912
	if(m_bReAnalyzed) 
		strNewType.Format("%d",m_nNetworkType);

	msg.Data.dialmsg.RtpPort = rtpport;
//	DbgOut("\n SendPort[%d]", msg.Data.dialmsg.RtpPort);

	m_uProxyRTPPort = rtpport; //by cje 010629
	m_strPhoneNumber.Remove( '-' ); 
	m_strPhoneNumber.Remove( ' ' );

//	if(!m_bInternalCall && m_nCallType != 1/*pc2phone 일경우는 m_strPhoneNumber 만 보낸다*/)
	if(!m_bInternalCall ) //pc2pc,pc2phone모두 중계시에는 strProxyAddr가 필요하다 by cje 020829
	{
		if(!m_pNATSoc[0])
		{
			// 3 : NAT Socket 이 NULL 일 때
			FireHangup(3);
			return;
		}
		CString strProxyAddr;
		
		
		////////////////////////////////////
//		if(m_nRTPTransferType != _PASSIVE && m_pNATSoc[0]->m_bNATCreated) 
//commented by cje 021127
		if(m_nRTPTransferType == _ACTIVE && m_pNATSoc[0]->m_bNATCreated) 
			for(int i=0; i<m_uMediaCount;i++)
				m_pNATSoc[i]->AsyncSelect(0); //by cje 010319 no more notification.

		if(m_pNATSoc[0]->rtpMode >= _UNIUDPROUTE)
		{
			strProxyAddr = m_pNATSoc[0]->GetProxyAddr();
			strcpy( msg.Data.dialmsg.PhoneNumber, m_strPhoneNumber+";"+strProxyAddr + ((m_bReAnalyzed && m_nCallType == 0) ? ";" + strNewType : ""));
		}
		else //NAT_CONE일때...by cje 020725
		{
			strcpy( msg.Data.dialmsg.PhoneNumber, m_strPhoneNumber + ((m_bReAnalyzed && m_nCallType == 0) ? ";;" + strNewType : ""));
		}
		
		m_uCreatePort = m_pNATSoc[0]->GetLocalRTPPort();
		if(m_nRTPTransferType == _NORMAL) //by cje 021112
		{
			//by cje 021126
			if(m_pNATSoc[0]->rtpMode != _UNIUDPROUTE && m_pNATSoc[0]->rtpMode != _BIUDPROUTE)
			{
				for(int i=0; i<m_uMediaCount;i++)
				{
					if(m_pNATSoc[i]->m_bNATCreated)
						m_pNATSoc[i]->Close();
					m_pNATSoc[i]->m_bNATCreated = FALSE;
				}
				if(!m_323Connection->CreateRTPSession(m_uCreatePort, t_Audio, 0)) 
				{
					if(m_nCallType == 0) //pc2pc
					{
						msg.Type = _ERROR;
						if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
						// 4 : CreateRTPSession 이 실패했을 때
						FireHangup(4);
						AfxMessageBox("m_323Connection Create fail");
						return ;
					}
				}
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						//m_pNATSoc[i]->GetLocalRTPPort() 를 이용하여 CreateChannel
						//현재는 i=1일때만 event발생하도록..
						if(i == 1) 
							FireEvVideoCreateInfo((long)m_pNATSoc[i]->GetLocalRTPPort());
					}
				}
			}
			else // _UNIUDPROUTE || _BIUDPROUTE
			{
				if(m_pNATSoc[0]->rtpMode == _UNIUDPROUTE)
					SetTimer(32, 30000, NULL);
				if(!m_323Connection->CreateRTPSession(m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+ 2, t_Audio, 0)) 
				{
					if(m_nCallType == 0) //pc2pc
					{
						msg.Type = _ERROR;
						if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
						// 4 : CreateRTPSession 이 실패했을 때
						FireHangup(4);
						AfxMessageBox("m_323Connection Create fail");
						return ;
					}
				}
				m_pNATSoc[0]->m_uLocalRTPPort = m_323Connection->GetLocalAudioPort(0);

				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						//m_pNATSoc[i]->GetLocalRTPPort() 를 이용하여 CreateChannel
						//현재는 i=1일때만 event발생하도록..
						if(i == 1) 
						{
							FireEvVideoCreateInfo((long)m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+2*(i+1));
							m_pNATSoc[i]->m_uLocalRTPPort = m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+2*(i+1); 
							//실제로는 vMira에서 생성한 포트를 넘겨줘야 한다. by cje 021126
						}
					}
				}
			}
			
		}
		else if( m_nRTPTransferType == _ACTIVE)
		{
			//??
		}
		
	}
	else
	{
		strcpy( msg.Data.dialmsg.PhoneNumber, m_strPhoneNumber + ((m_bReAnalyzed && m_nCallType == 0) ? ";;" + strNewType : ""));
	}
	CString tmp;
	tmp.Format("Send Dial with phonenumber %s\n",msg.Data.dialmsg.PhoneNumber);
	TRACE(tmp);
	if(m_nCallType == 1)
	{
		if(m_pDataSocketWCA) 
		{
			m_pDataSocketWCA->Send((char*)&msg, sizeof(msg));
			CString strTemp;
			CStringArray arrayTemp;
			if(ParseData(msg.Data.dialmsg.PhoneNumber, ';', &arrayTemp) > 1)
				strTemp.Format("SrcInfo;%s:%d", arrayTemp.GetAt(1), msg.Data.dialmsg.RtpPort);
			else
				strTemp.Format("SrcInfo;%d", msg.Data.dialmsg.RtpPort);

			FireEvReceiveMessage("[Serial]", strTemp);
		}
	}
	else if(m_nCallType == 0)
	{
		if(m_pDataSocketSCDialer1) 
		{
			
			m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(msg)); //_DIAL
	
			CString strTemp;
			CStringArray arrayTemp;
			if(ParseData(msg.Data.dialmsg.PhoneNumber, ';', &arrayTemp) > 1)
				strTemp.Format("SrcInfo;%s:%d", arrayTemp.GetAt(1), msg.Data.dialmsg.RtpPort);
			else
				strTemp.Format("SrcInfo;%d", msg.Data.dialmsg.RtpPort);

			FireEvReceiveMessage("[Serial]", strTemp);
			if(m_uMediaCount >1)
			{
				for(i=1 ; i< m_uMediaCount; i++	)
				{
					if(m_bInternalCall)//실제 생성된 video 포트를 전달해야한다.
						SendVideoInfo("127.0.0.1",BASERTPPORT + 2*i); //IPAddr은 다른곳에서 처리하도록.
					else
						SendVideoInfo("127.0.0.1",m_MediaInfo[i].MediaPort); //IPAddr은 다른곳에서 처리하도록.
				}//통화시에 서버에 m_strPhoneNumber가 Dial에서 설정되기 때문에 미리 먼저 보내준다.
			}
			
		}
	}
}

void CSCDialer1Ctrl::StartRTP()
{
	CString tmp;
	int i;
	if(m_bInternalCall)
	{
		PlaySound( NULL, NULL, 0 );
//		Sleep(1000);//

		tmp.Format("StartRTPSession to %s:%d\n" ,m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);
		TRACE(tmp);
		
		if( !m_323Connection->StartRTPSession( m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort, t_Audio, 0) ){
			// 2 : StartRTPSession 을 실패했을 때
			FireHangup(2);
			AfxMessageBox("SoundCard already used by another application.");
		}
		else
		{
			CString strTemp;
			strTemp.Format("DesInfo;%s:%d", m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);

			if(m_uMediaCount > 1)
			{
				for( i=1; i< m_uMediaCount;i++)
				{
					//m_pNATSoc[i]->GetLocalRTPPort() 를 이용하여 CreateChannel
					//현재는 i=1일때만 event발생하도록..
					if(i == 1) 
					{
						FireEvVideoStartInfo(m_msg.Data.startrtpmsg.RtpAddr, m_PeerMediaInfo[i].MediaPort);
						strTemp.Format("DesInfo;%s:%d/%d", m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort, m_PeerMediaInfo[i].MediaPort);
					}
				}
			}

			FireEvReceiveMessage("[Serial]", strTemp);

			m_timeConnected = CTime::GetCurrentTime();
			if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc))
			{
				MESSAGE newmsg;
				memset(&newmsg, 0x00, sizeof(MESSAGE));
				newmsg.Type = _COMPLETE;
				m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
				KillTimer(8);
				if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
				{
					FireEvConnected();
					m_nEvConnectedCount++;
				}
			}
		}		
	}
	else
	{
		if(!m_pNATSoc[0])
		{
			// 3 : NAT Socket 이 NULL 일 때
			FireHangup(3);
			return;
		}
		
		tmp.Format("StartRTP with rtpMode %d\n",m_pNATSoc[0]->rtpMode);
		TRACE(tmp);
		PlaySound( NULL, NULL, 0 );

		if(m_nRTPTransferType == _NORMAL)
		{
			if((m_pNATSoc[0]->rtpMode != _BITCPROUTE)&&(m_pNATSoc[0]->rtpMode != _BIUDPROUTE))
			{
				tmp.Format("StartRTPSession to %s:%d\n" ,m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);
				TRACE(tmp);
				
				if( !m_323Connection->StartRTPSession( m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort, t_Audio, 0) ){
					// 2 : StartRTPSession 을 실패했을 때
					FireHangup(2);
					AfxMessageBox("SoundCard already used by another application.");
				}
				else
				{
					CString strTemp;
					strTemp.Format("DesInfo;%s:%d", m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort);

					if(m_uMediaCount > 1)
					{
						for(i=1; i< m_uMediaCount;i++)
						{
							if(i == 1) 
							{
								FireEvVideoStartInfo(m_msg.Data.startrtpmsg.RtpAddr, m_PeerMediaInfo[i].MediaPort);
								strTemp.Format("DesInfo;%s:%d/%d", m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort, m_PeerMediaInfo[i].MediaPort);
							}
						}
					}
					FireEvReceiveMessage("[Serial]", strTemp);

					m_timeConnected = CTime::GetCurrentTime();
					if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc))
					{
						MESSAGE newmsg;
						memset(&newmsg, 0x00, sizeof(MESSAGE));
						newmsg.Type = _COMPLETE;
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
						KillTimer(8);
						if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
						{
							FireEvConnected();
							m_nEvConnectedCount++;
						}
					}
				}

			}
			else if(m_pNATSoc[0]->rtpMode == _BITCPROUTE)
			{
				//상대방의 RTP주소를 프록시에게 보낸다
				BOOL rvalue;
				rvalue = m_pNATSoc[0]->SendGWAddr(m_msg.Data.startrtpmsg.RtpAddr,m_msg.Data.startrtpmsg.RtpPort);
				TRACE("SendGWAddr %s:%d, rvalue %d\n",m_msg.Data.startrtpmsg.RtpAddr,m_msg.Data.startrtpmsg.RtpPort, rvalue);
				//return이 FALSE이면 문제가된다..
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_msg.Data.startrtpmsg.RtpAddr,m_PeerMediaInfo[i].MediaPort);
						if(i == 1) 
							FireEvVideoStartInfo("127.0.0.1", m_pNATSoc[i]->GetRouteSockPort());
					}
				}		
				
				
				tmp.Format("StartRTPSession to %s:%d\n" ,"127.0.0.1", m_pNATSoc[0]->GetRouteSockPort());
				TRACE(tmp);
				if( !m_323Connection->StartRTPSession( "127.0.0.1", m_pNATSoc[0]->GetRouteSockPort(), t_Audio, 0) ){
					// 2 : StartRTPSession 을 실패했을 때
					FireHangup(3);
					AfxMessageBox("SoundCard already used by another application.");
				}
				else
				{
					CString strTemp;
					strTemp.Format("DesInfo;%s:%d", "127.0.0.1", m_pNATSoc[0]->GetRouteSockPort());
					FireEvReceiveMessage("[Serial]", strTemp);

					m_timeConnected = CTime::GetCurrentTime();
					if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc))
					{
						MESSAGE newmsg;
						memset(&newmsg, 0x00, sizeof(MESSAGE));
						newmsg.Type = _COMPLETE;
						KillTimer(8);
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
						if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
						{
							FireEvConnected();
							m_nEvConnectedCount++;
						}
					}
				}
			}
			else  // BIUDPROUTE
			{
				BOOL rvalue;
				rvalue =	m_pNATSoc[0]->SendGWAddr(m_msg.Data.startrtpmsg.RtpAddr,m_msg.Data.startrtpmsg.RtpPort); 
				TRACE("SendGWAddr %s:%d, rvalue %d\n",m_msg.Data.startrtpmsg.RtpAddr,m_msg.Data.startrtpmsg.RtpPort, rvalue);
				//by cje 010705  
				//			Sleep(1000);
/*				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_msg.Data.startrtpmsg.RtpAddr,m_PeerMediaInfo[i].MediaPort);
						if(i == 1) 
							FireEvVideoStartInfo(m_pNATSoc[i]->GetProxyAddr(), m_MediaInfo[i].MediaPort);
					}
				}		
				
				tmp.Format("StartRTPSession to %s:%d\n" ,m_pNATSoc[0]->GetProxyAddr(), m_uProxyRTPPort);
				TRACE(tmp);	
				
				if( !m_323Connection->StartRTPSession( (char*)(LPCTSTR)m_pNATSoc[0]->GetProxyAddr(), m_uProxyRTPPort, t_Audio, 0)){
					FireHangup();
					AfxMessageBox("SoundCard already used by another application.");
				}
				else
				{
					m_timeConnected = CTime::GetCurrentTime();
					if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc)
					{
						MESSAGE newmsg;
						newmsg.Type = _COMPLETE;
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
						FireEvConnected();
					}
				}
*/
				//by cje 021126
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_msg.Data.startrtpmsg.RtpAddr,m_PeerMediaInfo[i].MediaPort);
						if(i == 1) 
							FireEvVideoStartInfo("127.0.0.1", m_pNATSoc[i]->GetLocalRTPPort());
					}
				}		
				
				tmp.Format("StartRTPSession to %s:%d\n" ,"127.0.0.1", m_pNATSoc[0]->GetLocalRTPPort());
				TRACE(tmp);	
				
				if( !m_323Connection->StartRTPSession( "127.0.0.1", m_pNATSoc[0]->GetLocalRTPPort(), t_Audio, 0)){
					// 2 : StartRTPSession 을 실패했을 때
					FireHangup(2);
					AfxMessageBox("SoundCard already used by another application.");
				}
				else
				{
					CString strTemp;
					strTemp.Format("DesInfo;%s:%d", "127.0.0.1", m_pNATSoc[0]->GetLocalRTPPort());
					FireEvReceiveMessage("[Serial]", strTemp);

					m_timeConnected = CTime::GetCurrentTime();
					if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc)
					{
						MESSAGE newmsg;
						memset(&newmsg, 0x00, sizeof(MESSAGE));
						newmsg.Type = _COMPLETE;
						m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
						KillTimer(8);
						if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
						{
							FireEvConnected();
							m_nEvConnectedCount++;
						}
					}
				}
			}
		}
		else if(m_nRTPTransferType == _ACTIVE)
		{
			for(int i = 0; i < m_uMediaCount ; i++) 
			{
				if(i == 0)
					m_pNATSoc[i]->SendActiveConfirm(m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort); 
				else
					m_pNATSoc[i]->SendActiveConfirm(m_msg.Data.startrtpmsg.RtpAddr, m_PeerMediaInfo[i].MediaPort); 
			}
			SetTimer(31,100, NULL);

/*			if( !m_323Connection->StartRTPSession( m_msg.Data.startrtpmsg.RtpAddr, m_msg.Data.startrtpmsg.RtpPort, t_Audio, 0) ){
				FireHangup();
				AfxMessageBox("SoundCard already used by another application.");
			}	
			else
			{
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						if(i == 1) 
							FireEvVideoStartInfo(m_msg.Data.startrtpmsg.RtpAddr, m_PeerMediaInfo[i].MediaPort);
					}
				}	
				m_timeConnected = CTime::GetCurrentTime();
				if(m_nCallType == 0 && m_pDataSocketSCDialer1) // pc2pc 일 경우는 _COMPLETE 을 보낸다. (궁극적으로는 ph2pc)
				{
					MESSAGE newmsg;
					newmsg.Type = _COMPLETE;
					m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
					FireEvConnected();
				}
			}
*/
		}
		else if(m_nRTPTransferType == _PASSIVE)
		{
			//아무것도 하지 않는다..받은 채널정보는 그대로 무시한다.
		}
	}
	if(m_nCallType == 0) //pc2pc
	{
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _CONNECTED; // Caller 가 보냄
		KillTimer(8);
		if(m_pDataSocketSCDialer1) 
		{
			m_nState = _DIALINGSTATE;
			m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(MESSAGE));
			if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
			{
				FireEvConnected();// Caller 에서는 여기서 
				m_nEvConnectedCount++;
			} 
			FireEvStatus(m_nState);
		}
	}
}

int CSCDialer1Ctrl::OnReceivedDial(unsigned int port)
{
	CString  tmp;
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	TRACE("OnReceivedDial m_nPeerPort : %d\n",m_nPeerPort);
	m_uCalleeRTPPort = port;

#ifdef	_LOCAL_PLAY_G7231
		if(m_bRingWave)
		{
			m_323Connection->LocalPlayG7231(0, TRUE);
			TRACE("\n [_DIAL from PCCA] RingWave Play Start 3 ");
		}
#endif
	
	FireEvReceiveCall(m_strCallerPCN); // after ringwave ringing occurs

	// 아래를 수행하면 전화 안 받겠다고 NO 버튼을 눌렀을때도 OnReceiveDial 로 넘어와서
	// m_bCall = TRUE; 가 되기 때문에 case _DIAL 에서 바를로 return이 되버린다.
//	m_bCall = TRUE;

	if(m_bAutoAcceptCall)
	{
		
		strCaller.Format("Call from %s. Accept?", m_strCallerPCN);
	
		//MessageBox 형태의 Incommingcall 박스를 Dlg 형태로 바꿈.
		CIncommingDlg m_IncommingDlg;
		if(IDCANCEL ==  m_IncommingDlg.DoModal())
		{
			msg.Type = _ERROR;
			if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
			// 7 : PC2PC 에서 전화를 Accept 하지 않고 끊음
			FireHangup(10);
			return -1L;
		}

//by cje
		tmp.Format("SendStartRTPMsg with Callee(Caller)RTPport %d\n", m_uCalleeRTPPort);
		TRACE(tmp);
		SendStartRTPMsg(m_uCalleeRTPPort);

	}
	return 0 ;

}
int CSCDialer1Ctrl::SendStartRTPMsg(unsigned int port)
{
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	CString tmp;
	int i;
	msg.Type = _STARTRTP;
	msg.Data.startrtpmsg.RtpPort = port;
	PlaySound(NULL, NULL, NULL); // 이것을 해야 전화를 받을 때(without TelBox) device 에러 없음

	CString strNewType; //by cje 020912
	if(m_bReAnalyzed) 
		strNewType.Format("%d",m_nNetworkType);
	CString strLocalIP;
	strLocalIP.Format(LocalIP);

	if(m_bInternalCall)
	{
		strcpy(msg.Data.startrtpmsg.RtpAddr, strLocalIP + ((m_bReAnalyzed && m_nCallType == 0) ? ";" + strNewType : ""));
		tmp.Format("SendStartRTP with IP:%s, Port:%d\n",msg.Data.startrtpmsg.RtpAddr,msg.Data.startrtpmsg.RtpPort );
		TRACE(tmp);
		if(m_uMediaCount >1)
		{
			for(i=1 ; i< m_uMediaCount; i++	) //이미 생성되어있는 video 포트를 전달해야..
				SendVideoInfo("127.0.0.1",BASERTPPORT + 2*i); //IPAddr은 다른곳에서 처리하도록.
		}
		
		if(m_pDataSocketSCDialer1) 
		{
			m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
		}
	}
	else
	{
		// by gomeisa
		if(!m_pNATSoc[0])
		{
			// 3 : NAT Socket 이 NULL 일 때
			FireHangup(3);
			return -1L;
		}
		m_uCreatePort  = m_pNATSoc[0]->GetLocalRTPPort();  //by cje 010508
		
		if(m_pNATSoc[0]->rtpMode >= _UNITCPROUTE)
		{
			m_uCreatePort = BASERTPPORT;  
		}
		m_uCreatePort = m_pNATSoc[0]->GetLocalRTPPort();
		TRACE("m_nRTPTransferType On SendStartRTP : %d\n", m_nRTPTransferType);
		if(m_nRTPTransferType == _NORMAL) //by cje 021112
		{
			if(m_pNATSoc[0]->rtpMode != _UNIUDPROUTE && m_pNATSoc[0]->rtpMode != _BIUDPROUTE)
			{
				
				for(int i=0; i<m_uMediaCount;i++)
				{
					if(m_pNATSoc[i]->m_bNATCreated)
						m_pNATSoc[i]->Close();
					m_pNATSoc[i]->m_bNATCreated = FALSE;
				}
				tmp.Format("CreateRTPSession with port %d\n",m_uCreatePort);
				TRACE(tmp);
				if( !m_323Connection->CreateRTPSession(m_uCreatePort, t_Audio, 0) ) 
				{
					msg.Type = _ERROR;
					if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
					// 4 : CreateRTPSession 이 실패했을 때
					FireHangup(4);
					AfxMessageBox("m_323Connection Create fail");
					return -1L;
				}
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						if(i == 1) 
							FireEvVideoCreateInfo((long)m_pNATSoc[i]->GetLocalRTPPort());
					}
				}
			}
			else // _UNIUDPROUTE || _BIUDPROUTE 일때는 m_pNATSoc을 닫지 않는다.
			{
				if(m_pNATSoc[0]->rtpMode == _UNIUDPROUTE)
					SetTimer(32, 30000, NULL);
				if( !m_323Connection->CreateRTPSession(m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+ 2, t_Audio, 0) ) 
				{
					msg.Type = _ERROR;
					if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
					// 4 : CreateRTPSession 이 실패했을 때
					FireHangup(4);
					AfxMessageBox("m_323Connection Create fail");
					return -1L;
				}
				m_pNATSoc[0]->m_uLocalRTPPort = m_323Connection->GetLocalAudioPort(0);

				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						if(i == 1) 
						{
							FireEvVideoCreateInfo((long)m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+2*(i+1));
							m_pNATSoc[i]->m_uLocalRTPPort = m_pNATSoc[m_uMediaCount-1]->GetLocalRTPPort()+2*(i+1); 
							//실제로는 vMira에서 생성한 포트를 넘겨줘야 한다. by cje 021126
						}
					}
				}
			}
		}
		else if(m_nRTPTransferType == _ACTIVE)
		{
			//??아무것도 하지 않는다..
		}

		// 2003.10.02 by gomeisa : m_pNATSoc[0] 가 NULL 일 경우 발생해서 수정함.
		if(m_pNATSoc[0] && m_pNATSoc[0]->rtpMode >= _UNIUDPROUTE)
		{
			strcpy(msg.Data.startrtpmsg.RtpAddr, (LPTSTR)(LPCTSTR)(m_pNATSoc[0]->GetProxyAddr()+";"+"USEPROXY" + ((m_bReAnalyzed && m_nCallType == 0) ? ";" + strNewType : ""))); // set this value in SCDialer1 Server
		}
		else
		{
//			strcpy( msg.Data.startrtpmsg.RtpAddr, (LPTSTR)(LPCTSTR)m_strMyIP );
			strcpy( msg.Data.startrtpmsg.RtpAddr, strLocalIP + ((m_bReAnalyzed && m_nCallType == 0) ? ";;" + strNewType : ""));
		}

		tmp.Format("SendStartRTP with IP:%s, Port:%d\n",msg.Data.startrtpmsg.RtpAddr,msg.Data.startrtpmsg.RtpPort );
		TRACE(tmp);
		if(m_uMediaCount >1)
		{
			for(i=1 ; i< m_uMediaCount; i++	)
				SendVideoInfo("127.0.0.1",m_MediaInfo[i].MediaPort); //IPAddr은 다른곳에서 처리하도록.
		}
		
		if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
	}

	// by gomeisa 2002.11.04 : non-faststart phone2pc 를 위해
	if(m_strPeerIP.Compare("ph2pc"))
	{
		m_bIsPh2PC = FALSE;
		m_nState = _DIALINGSTATE;
		FireEvStatus(m_nState);
		
		PlaySound( NULL, NULL, 0 );
		FireEvGetPeerIP(m_strPeerIP/*m_msg.Data.dialmsg.PhoneNumber*//*IP Address*/, m_msg.Data.dialmsg.RtpPort);
		
		char CallerAddr[40];
		UINT PeerPort[MAXMEDIACOUNT];
		//StartRTPSession에 사용할 상대방 주소..
		
		//StartRTPSession
		if(m_bInternalCall)
		{
			strcpy(CallerAddr, m_strPeerIP);
			PeerPort[0] = m_nPeerPort;
			for(i=1; i<m_uMediaCount;i++)
				PeerPort[i] = m_PeerMediaInfo[i].MediaPort;
		}
		else
		{
			if(m_nRTPTransferType != _PASSIVE)
			{
				if((m_pNATSoc[0]->rtpMode != _BITCPROUTE)&&(m_pNATSoc[0]->rtpMode != _BIUDPROUTE))
				{
					strcpy(CallerAddr, m_strPeerIP);
					PeerPort[0] = m_nPeerPort;
					for(i=1; i<m_uMediaCount;i++)
						PeerPort[i] = m_PeerMediaInfo[i].MediaPort;
				}
				else if(m_pNATSoc[0]->rtpMode == _BITCPROUTE)
				{
					//상대방의 주소를 프록시에게 보낸다
					BOOL rvalue;
					rvalue =m_pNATSoc[0]->SendGWAddr(m_strPeerIP,m_nPeerPort);
					TRACE("SendGWAddr1 %s:%d, rvalue %d\n",m_strPeerIP,m_nPeerPort, rvalue);
					
					//return이 FALSE이면 문제가된다..
					
					//udp->TCP
					
					strcpy(CallerAddr, "127.0.0.1");
					PeerPort[0] = m_pNATSoc[0]->GetRouteSockPort();
					for(i=1; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
						PeerPort[i] = m_pNATSoc[i]->GetRouteSockPort();
					}
				}
				else  // BIUDPROUTEstartrtpsession to
				{
					BOOL rvalue;
					rvalue = m_pNATSoc[0]->SendGWAddr(m_strPeerIP,m_nPeerPort);  //by cje 010705
					TRACE("SendGWAddr1 %s:%d, rvalue %d\n",m_strPeerIP,m_nPeerPort, rvalue);
					
				/*	strcpy(CallerAddr, (char*)(LPCTSTR)m_pNATSoc[0]->GetProxyAddr());
					PeerPort[0] = m_uCalleeRTPPort;
					for(i=1; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
						PeerPort[i] = m_MediaInfo[i].MediaPort; //m_pNATSoc[i]->GetProxyPort()와 같은 값.
					}
				*/	strcpy(CallerAddr, "127.0.0.1");
					PeerPort[0] = m_pNATSoc[0]->GetLocalRTPPort();
					for(i=1; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SendGWAddr(m_strPeerIP,m_PeerMediaInfo[i].MediaPort);
						PeerPort[i] = m_pNATSoc[i]->GetLocalRTPPort();
					}
					//by cje 021126
				}
			}
			else
			{
				//아무것도 하지 않는다..
			}
		}
		
		if(m_nRTPTransferType == _NORMAL)
		{
			tmp.Format("StartRTPSession to %s:%d\n",CallerAddr, PeerPort[0]);
			TRACE(tmp);
			if( !m_323Connection->StartRTPSession( CallerAddr, PeerPort[0], t_Audio, 0) ){
				// 2 : StartRTPSession 을 실패했을 때
				FireHangup(2);
				AfxMessageBox("SoundCard already used by another application.");
			}
			else // ph2pc 에서 callee 가 H.323 CONNECT 를 줄 수 있도록 하기 위한 메세지
			{
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						if(i == 1) 
							FireEvVideoStartInfo(CallerAddr, PeerPort[i]);
					}
				}
				CString strTemp;
				strTemp.Format("DesInfo;%s:%d/%d", CallerAddr, PeerPort[0], PeerPort[i]);
				FireEvReceiveMessage("[Serial]", strTemp);

				m_timeConnected = CTime::GetCurrentTime();
				if(m_pDataSocketSCDialer1)
				{
					MESSAGE newmsg;
					memset(&newmsg, 0x00, sizeof(MESSAGE));
					newmsg.Type = _COMPLETE;
					m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
					KillTimer(8);
					if(!m_nEvConnectedCount) // EvConnected 가 한번만 발생하도록..
					{
						FireEvConnected();
						m_nEvConnectedCount++;
					}
				}
			}
		}
		else if (m_nRTPTransferType == _ACTIVE)
		{
			for(int i = 0; i < m_uMediaCount ; i++) 
			{
					m_pNATSoc[i]->SendActiveConfirm(CallerAddr, PeerPort[i]); 
			}
			SetTimer(31,100, NULL);
			/*			
			tmp.Format("StartRTPSession to %s:%d\n",CallerAddr, PeerPort[0]);
			TRACE(tmp);
			if( !m_323Connection->StartRTPSession( CallerAddr, PeerPort[0], t_Audio, 0) ){
				FireHangup();
				AfxMessageBox("SoundCard already used by another application.");
			}
			else // ph2pc 에서 callee 가 H.323 CONNECT 를 줄 수 있도록 하기 위한 메세지
			{
				if(m_uMediaCount > 1)
				{
					for(i=1; i< m_uMediaCount;i++)
					{
						if(i == 1) 
							FireEvVideoStartInfo(CallerAddr, PeerPort[i]);
					}
				}
				
				m_timeConnected = CTime::GetCurrentTime();
				if(m_pDataSocketSCDialer1)
				{
					MESSAGE newmsg;
					newmsg.Type = _COMPLETE;
					m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
					FireEvConnected();
				}
			}
*/
		}
	}
	else
	{
//		AfxMessageBox("non-faststart : send");
		m_bIsPh2PC = TRUE;
	}
	return 0;
}

void CSCDialer1Ctrl::OnDestroy() 
{
	OSVERSIONINFO	os;
	memset(&os, 0x00, sizeof(OSVERSIONINFO));
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&os);

	if(os.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		
	}
//	TRACE("\n[OnDestory] SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)");
	
	COleControl::OnDestroy();
	
	// TODO: Add your message handler code here
/*	FireHangup(); //by cje
	KillTimer(1); // _ALIVE
*/
	if(m_bCall)
	{
		// 9 : Client 종료 (close)
		FireHangup(9);
	}

	if(IsRegistered())
		FireUnRegisterServer();

	if(m_pDataSocketSCDialer1)
	{
		delete m_pDataSocketSCDialer1;
		m_pDataSocketSCDialer1 = NULL;
	}
	if(m_pDataSocketWCA)
	{
		delete m_pDataSocketWCA;
		m_pDataSocketWCA = NULL;
	}

	if(m_pNATSoc[0])
	{
		for(int i=0; i<m_uMediaCount;i++)
		{
			delete m_pNATSoc[i];
			m_pNATSoc[i] = NULL;
		}
	}

	if(m_323Connection)
	{
		delete m_323Connection;
		m_323Connection = NULL;
	}

}

void CSCDialer1Ctrl::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("\n[OnClose in CSCDialer1Ctrl]");
	
	CWnd::OnClose();
}

BOOL CSCDialer1Ctrl::SetLicenseKey(LPCTSTR LicenseKey) 
{
	// TODO: Add your dispatch handler code here
/*	CString str;
	str.Format("%s", LicenseKey);
	if(str.Find(";", 0) == -1) // 이전 버전과의 호환을 위해서 추가 1219
	{
		CCode2 code;
		str = code.Decode(LicenseKey);
	}

	CStringArray arrayData;

	if(ParseData(str, ';', &arrayData) !=2)
	{
		m_bLicense = FALSE;
		return FALSE;
	}

	m_nProductID = atoi(arrayData.GetAt(0));

	if(!arrayData.GetAt(1).CompareNoCase("webcallgomeisa0907"))
		m_bLicense = TRUE;
	else 
		m_bLicense = FALSE;

	return TRUE;
*/
	CString str = LicenseKey;
	CStringArray arrayData;

	if(str.Find(";", 0) == -1) // 신버전 1219
	{
		CCode2 code;
		str = code.Decode(LicenseKey);

		if(ParseData(str, ';', &arrayData) !=4)
		{
			m_bLicense = FALSE;
			return FALSE;
		}

		int nKey = arrayData.GetAt(0).Find('K') + 1;
		int nPID = arrayData.GetAt(0).Find('P') + 1;
		int nDate = arrayData.GetAt(0).Find('D') + 1;

		m_nProductID = atoi(arrayData.GetAt(nPID));
		
		if(!arrayData.GetAt(nKey).CompareNoCase("urimong0907webcall") && CTime::GetCurrentTime().Format("%Y%m%d") < arrayData.GetAt(nDate))
			m_bLicense = TRUE;
		else 
			m_bLicense = FALSE;
	}
	else         // 구버전 (MiraCall 에만 적용됨)
	{
		if(ParseData(str, ';', &arrayData) !=2)
		{
			m_bLicense = FALSE;
			return FALSE;
		}

		m_nProductID = atoi(arrayData.GetAt(0));

		if(!arrayData.GetAt(1).CompareNoCase("webcallgomeisa0907"))
			m_bLicense = TRUE;
		else 
			m_bLicense = FALSE;
	}

	return TRUE;
}

void CSCDialer1Ctrl::OnEnableTelBoxChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnProxyModeChanged() 
{
	// TODO: Add notification handler code

	CString strTemp;
	strTemp.Format("NetworkType;%d", m_lProxyMode);
	FireEvReceiveMessage("[Serial]", strTemp);
	if(m_lProxyMode == 0 ) //VideoRoute가 체크해제됐을때.
	{
		CString strMsg; strMsg.Format("NetworkType;%d", m_nRealNetworkType);
		SendText("[Server]", strMsg);

		return;
	}
	else if( m_lProxyMode == 1) // VideoRoute가 체크됐을때.
	{
		SendText("[Server]", "NetworkType;4");
		return;
	}


	if(m_lProxyMode < _SIMPLE || m_lProxyMode > _BITCPROUTE)
		m_lProxyMode = _DIRECT;
	m_nRTPmode = m_lProxyMode;    

	TRACE("\n[OnProxyModeChanged : %d]",m_nRTPmode);

	for(int i=0; i<m_uMediaCount;i++) //by cje 030228추가
	{
		if(m_pNATSoc[i])
			m_pNATSoc[i]->rtpMode = (RTPMODE)m_nRTPmode;
	}

	m_nNetworkType = m_nRealNetworkType; //분석에 의한 실제 네트워크타입.
	if(m_nRTPmode >= _UNIUDPROUTE) m_nNetworkType = _RESTRICTED;
	if(m_nRTPmode >= _UNITCPROUTE) m_nNetworkType = _UDPBLOCK;
	m_bReAnalyzed = TRUE;

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnStatusChanged() 
{
	// TODO: Add notification handler code
	if(m_bCall)
		return;

	SetModifiedFlag();
}

void CSCDialer1Ctrl::SpkVolumeUp() 
{
	// TODO: Add your dispatch handler code here
	if(m_323Connection && !m_nDeviceType)
		m_323Connection->ShowVolumeConfig(0);
}

void CSCDialer1Ctrl::SpkVolumeDown() 
{
	// TODO: Add your dispatch handler code here
	if(m_323Connection && !m_nDeviceType)
		m_323Connection->ShowVolumeConfig(0);
}

void CSCDialer1Ctrl::MicVolumeUp() 
{
	// TODO: Add your dispatch handler code here
	if(m_323Connection && !m_nDeviceType)
		m_323Connection->ShowVolumeConfig(1);
}

void CSCDialer1Ctrl::MicVolumeDown() 
{
	// TODO: Add your dispatch handler code here
	if(m_323Connection && !m_nDeviceType)
		m_323Connection->ShowVolumeConfig(1);
}

void CSCDialer1Ctrl::PushButton(LPCTSTR button) 
{
	// TODO: Add your dispatch handler code here
	m_strPhoneNumber += button;
	FireEvPushButton(button);
	// by gomeisa 2003.01.29 usbphone 에서도 
	if(m_nDeviceType != 0 || m_bDTMFWave)		// Serial Phone
	{
		WPARAM wP;
		unsigned short int tmp;
		if(strcmp(button, "*") == 0){ tmp = 10;}
		else if(strcmp(button, "#") == 0){ tmp = 11;}
		else if(strcmp(button, "B") == 0){ tmp = 12;}
		else{ tmp = atoi(button);}
		wP = MAKEWPARAM(3,tmp);
		//m_323Connection->LocalPlayG7231(5, TRUE);

		m_323Connection->LocalPlayG7231(wP, TRUE);
	}
	
	if( m_nState != _DIALINGSTATE || m_nCallType != 1) // PC2Phone
		return;
	if( !m_pDataSocketWCA ) return;
	if( !m_pDataSocketWCA->IsConnecting() ) return;
	
	if(!m_bInBand)
	{
		memset(&m_msg, 0x00, sizeof(m_msg));
		m_msg.Type = _BUTTON;
		strcpy( m_msg.Data.Button, button );
		m_pDataSocketWCA->Send( (char*)&m_msg, sizeof(MESSAGE) ); // size : type + string[20]
	}
	else
	{
		if(strcmp(button, "*") == 0){ m_323Connection->SendDTMF("10");}
		else if(strcmp(button, "#") == 0){ m_323Connection->SendDTMF("11");}
		else{ m_323Connection->SendDTMF(button);}		
	}	
}

void CSCDialer1Ctrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	int i;

	switch(nIDEvent)
	{
	case 1: // PCCallAgent alive
		if(m_bRegistered && m_pDataSocketSCDialer1)
		{
			// 20020221 버전부터는 _PREREGISTRATION 에 버전 정보 송신함
//			msg.Data.updatemsg.version = m_lMiraCallVersion;
			msg.Type = _ALIVE;
			m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(msg));
		}
		else
			KillTimer(1);
		break;
	case 2: // WebCallAgent alive
		msg.Type = _ALIVE;
		if(m_nCallType == 1 && m_bCall && m_pDataSocketWCA)
		{
			m_pDataSocketWCA->Send((char*)&msg, sizeof(msg));
		}
		// 2002.11.16 by gomeisa : WCA 에 pc2phone 을 끊었음에도 Alive 가 전달되는 버그가 있어서 여기서 막아둠
		else
		{
			KillTimer(2);
			if(m_pDataSocketWCA && m_pDataSocketWCA->IsConnecting())
			{
				m_pDataSocketWCA->Close();
			}
		}

		break;
	case 4: // billing 시작
		KillTimer(4);
		if(m_bCall && m_nCallType == 1 && m_lBillingPID == 987654321)
		{
			SendMoneyData();
		}
		break;
	case 5:
		if(m_bCall && m_nCallType == 1 && m_lBillingPID == 987654321)		
			SendMoneyData();
		else
			KillTimer(5);
		break;
	case 8: // pc2phone/pc2pc WCA connect timout
		KillTimer(8);
		if(m_nState == _DIALINGSTATE)
			return;
		FireHangup(1);
		m_nState = _ERROR;
		FireEvStatus(m_nState);
		m_nState = _READYSTATE;
		SetTimer(14, 1500, NULL);
		FireEvError(OVER_TIME, _T("Server connection fail/not received"));
		TRACE("[FireEvError] No Answer in Timer : 8\n");
		break;
	case 14: // Error state 에서 다른 state 로 전환시켜줌.
		KillTimer(14);
		FireEvStatus(m_nState);
		break;
	case 15:  // PCCA login timeout
		KillTimer(15);
		m_nState = _ERRORSTATE;
		FireEvError(NETWORK_ERROR, _T("PCCallAgent connection fail"));
		m_bProgressRegistration = FALSE;
		m_nState = _READYSTATE;
		SetTimer(14, 1500, NULL);
		KillTimer(16);
		if(m_nAutoRetryLoginInterval)
			SetTimer(16, m_nAutoRetryLoginInterval * 1000, NULL);
		break;
	case 16: // AutoRetryLogin
		KillTimer(16);
		if(!IsRegistered() && m_nAutoRetryLoginInterval && m_bReTryToLogin)
		{
			FireRegisterServer(0);
		}
		break;
	case 20: //ALIVE to FireTalk
		{
			if(m_pNATSoc[0])
			{
				for(i=0;i<m_uMediaCount;i++)
				{
					m_pNATSoc[i]->SendAlive();
				}
			}
		}
		break;
	case 30:
		{
			KillTimer(30);
			TRACE("killtimer30");
			if(m_pNATSoc[0])
			{
				KillTimer(32); 
				for(i=0;i<m_uMediaCount;i++)
				{
					m_pNATSoc[i]->rtpMode  = _UNITCPROUTE;
					m_pNATSoc[i]->Close();
					m_pNATSoc[i]->m_bNATCreated = FALSE;
					TRACE("NATCreated FALSE7\n");
					if(m_pNATSoc[i]->m_bSetProxy)
					{
						TRACE("SensMessage port0-1\n");
						SendMessage(UM_NATRESULT, (WPARAM)m_pNATSoc[i]->m_iMediaIndex,(LPARAM)0);
					}
					else
					{
						SendMessage(UM_NATRESULT, (WPARAM)m_pNATSoc[i]->m_iMediaIndex,(LPARAM)-1);
					}
				}
			}
			
		}
		break;
	case 31 :
		{
			//ACTIVE Confirm확인 루틴..
			KillTimer(31);
			TRACE("OnTimer31 Send _ASK_ACTIVE_CONFIRM\n");
			m_msg.Type = _ASK_ACTIVE_CONFIRM;
			m_pDataSocketSCDialer1->Send( (char*)&m_msg, sizeof(m_msg) );//ask retry
			
		}
		break;
	case 32 :
		for(i=0; i< m_uMediaCount;i++)
		{
			m_pNATSoc[i]->SendNATMapAlive();
		}		
		break;
	case 33 :
		{
			KillTimer(33);
			if(m_323Connection)
			{
				m_323Connection->EndAllRTPSession();
				//m_323Connection->SetBusyTone(FALSE);
			}
		}
		break;
	case 40 :
		{
			KillTimer(40); 
			if(m_pNATSoc[0])  //by cje
			{
				for(int i=0; i<m_uMediaCount;i++)
				{
					if(m_pNATSoc[i]->m_pRouter)
					{
						delete m_pNATSoc[i]->m_pRouter ; 
					}
					delete m_pNATSoc[i];
					m_pNATSoc[i] = NULL;
				}	
			}
		}
		break;
	}

	COleControl::OnTimer(nIDEvent);
}

BOOL CSCDialer1Ctrl::IsRegistered() 
{
	// TODO: Add your dispatch handler code here

	return m_bRegistered;
}

void CSCDialer1Ctrl::OnPhoneBookListChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

BOOL CSCDialer1Ctrl::RetrievePhoneBook() 
{
	// TODO: Add your dispatch handler code here
	if(m_pDataSocketSCDialer1 && !m_bProgressPhonebook)
	{
		CStringArray arrayPhoneBook;
		CString strTemp;
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _PHONEBOOK;
		
		m_bProgressPhonebook = TRUE;

		int count = ParseData(m_strPhoneBookList, ';', &arrayPhoneBook);
		newmsg.Data.phonebookmsg.count = arrayPhoneBook.GetSize();

		for(int i = 1; i <= count; i++)
		{
			strTemp += arrayPhoneBook.GetAt(i-1);
			strTemp += ";";
			if(!(i%3))
			{
				strcpy(newmsg.Data.phonebookmsg.PhoneBook, strTemp);
				m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
#ifdef _DEBUG
				DBGOUT("[PhoneBook] [Send] count=%d, data=%s\n", count, strTemp);
#endif
				strTemp.Empty();
			}
		}
		if(!strTemp.IsEmpty())
		{
			strcpy(newmsg.Data.phonebookmsg.PhoneBook, strTemp);
			m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
#ifdef _DEBUG
			DBGOUT("[PhoneBook] [Send] count=%d, data=%s\n", count, strTemp);
#endif
		}
		return TRUE;
	}
	FireEvRetrievePhoneBook(_T(""), _T(""));
#ifdef _DEBUG
	DBGOUT("[PhoneBook] [Error] m_bProgressPhoneBook=%s\n", m_bProgressPhonebook ? "TRUE" : "FALSE");
#endif

	m_strLogin.Empty(); // 초기화
	m_strNotLogin.Empty(); // 초기화
	m_lPhonebookCount = 0; // 초기화
	
	return FALSE;
}

void CSCDialer1Ctrl::OnAuthTypeChanged() 
{
	// TODO: Add notification handler code
	if(m_bRegistered)
		return;

	SetModifiedFlag(); 
}


void CSCDialer1Ctrl::OnDialButtonClickedChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnMyStatusChanged() 
{
	// TODO: Add notification handler code
	if(m_bRegistered)
	{
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _MYSTATUS;
		newmsg.Data.mystatusmsg.status = m_nMyStatus;
		if(m_bCall && m_nCallType == 1)
			strcpy(newmsg.Data.mystatusmsg.MyStatus, m_strPhoneNumber);
		else
			strcpy(newmsg.Data.mystatusmsg.MyStatus, "");
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
		
	}

	SetModifiedFlag();
}

void CSCDialer1Ctrl::GetAudioDeviceList() 
{
	// TODO: Add your dispatch handler code here
	if(m_323Connection)
		FireEvRetrieveAudioDeviceList(m_323Connection->CheckAudioDevice());
	else
		FireEvRetrieveAudioDeviceList(_T(""));
}

void CSCDialer1Ctrl::OnDTMFWaveChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnRingWaveChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnInBandChanged() 
{
	// TODO: Add notification handler code
	if(m_323Connection)
	{
		m_323Connection->SetInBandDTMF(m_bInBand);
	}

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnLKeyChanged() 
{
	// TODO: Add notification handler code
	SetLicenseKey(m_strLKey);

	SetModifiedFlag();
}

void CSCDialer1Ctrl::Forwarding(short Type, LPCTSTR Destination) 
{
	// TODO: Add your dispatch handler code here
	if(m_pDataSocketSCDialer1 && m_bRegistered)
	{
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _FORWARDING;
		newmsg.Data.forwardingmsg.type = Type;
		strcpy(newmsg.Data.forwardingmsg.Forwarding, Destination);
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
	}
}

void CSCDialer1Ctrl::SendVideoInfo(LPCTSTR IP, long Port) 
{
	// TODO: Add your dispatch handler code here
//	if(m_pDataSocketSCDialer1 && m_bCall && m_bRegistered && m_nCallType == 0/*PC2PC*/)
	if(m_pDataSocketSCDialer1 && m_bCall && m_bRegistered /*PC2PC*/)
	{
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _VIDEOMSG;
		newmsg.Data.forwardingmsg.type = Port;
		strcpy(newmsg.Data.forwardingmsg.Forwarding, IP);
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
	}	
}

BOOL CSCDialer1Ctrl::SendText(LPCTSTR Destination, LPCTSTR Message) 
{
	// TODO: Add your dispatch handler code here
	if(!strcmp(Destination, "[Serial]"))
	{
		if(!m_323Connection)
			return FALSE;
		
		if(!strcmp(Message, "enable"))
		{
			m_323Connection->LogEnable();
		}
		else if(!strcmp(Message, "disable"))
		{
			m_323Connection->LogDisable();
		}
		else if(!strcmp(Message, "record_start"))
		{
			m_323Connection->LogRecordStart();
		}
		else if(!strcmp(Message, "record_stop"))
		{
			m_323Connection->LogRecordStop();
		}
		else if(!strcmp(Message, "play_start"))
		{
			m_323Connection->LogPlayStart();
		}
		else if(!strcmp(Message, "play_stop"))
		{
			m_323Connection->LogPlayStop();
		}
		return TRUE;
	}

	if(!m_pDataSocketSCDialer1 || !m_bRegistered/* || !m_bCall*/) // Call 중이 아니라도 채팅은 할 수 있도록..
		return FALSE;

	char	buf[512];
	memset(buf, 0x00, 512);
	MESSAGE *pNewmsg;

	pNewmsg = (MESSAGE*)buf;
	pNewmsg->Type = _TEXT;
	CString strMessage; strMessage.Format("%s", Message);
	CString strDestination; strDestination.Format("%s", Destination);

	if(strDestination.IsEmpty())
	{
		if(m_bCall) // Destination 이 없을 경우 통화중인지 체크하고 m_strPhoneNumber 가 destination.
			strDestination = m_strPhoneNumber;
		else // 전화 중이 아닐 경우에 test 관련된 메세지가 도착할 수 있다.
		{
			return FALSE; // destination 이 없음.
		}
	}
	
	if(!strDestination.CompareNoCase("[Server]"))
	{
		CStringArray arrayTemp;
		int count = ParseData(Message, ';', &arrayTemp);
		if(count > 1)
		{
			if(!arrayTemp.GetAt(0).CompareNoCase("update"))
			{
				MESSAGE newmsg;
				memset(&newmsg, 0x00, sizeof(MESSAGE));
				newmsg.Type = _UPDATE;
				newmsg.Data.updatemsg.version = 0;

				int count2 = 0;
				CStringArray arrayPCN;
				CString strPCN, strPWD;
				
				// encrytion 되지 않았으면 1219 버전. 타회사 SCDialer1 제공시에는 encode 없이 값이 온다.
				if(m_strUserPhoneNumber.Find(",", 0) != -1) // encryption 이 안되었을 경우 구분자는 ','
				{
					count2 = ParseData(m_strUserPhoneNumber, ',', &arrayPCN);
					CCode2 code;
					if(count2 >= 2)
					{
						strPWD = arrayPCN.GetAt(1);
						strPCN = arrayPCN.GetAt(0);
					}
				}

				CString strMsg; strMsg.Format("%s;%s;%s", arrayTemp.GetAt(1), strPCN, strPWD);
				strcpy(newmsg.Data.updatemsg.Url, strMsg);
				m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
//				DbgOut("\n[_UPDATE]1 Send : %s\n", newmsg.Data.updatemsg.Url);
				return TRUE;
			}
		}
	}
	if((strMessage.GetLength() + strDestination.GetLength() + 1) < 200)
	{
		strDestination += ";" + strMessage;
		pNewmsg->Data.textmsg.length = strDestination.GetLength(); // length 는 실제 message 길이만 준다.
		strcpy(pNewmsg->Data.textmsg.Message, strDestination);
		
		if(pNewmsg->Data.textmsg.length > MAXDATALENGTH)
		{
			m_pDataSocketSCDialer1->Send(buf, sizeof(MESSAGE) + pNewmsg->Data.textmsg.length - MAXDATALENGTH);
		}
		else
		{
			m_pDataSocketSCDialer1->Send(buf, sizeof(MESSAGE));
		}		
//		DbgOut("\n[Text]1 Send : %s\n", pNewmsg->Data.textmsg.Message);
		return TRUE;
	}

	/*
	int size = sizeof(newmsg.Data.textmsg.Message) - strDestination.GetLength() - 2;

	int step = 0, start = 0, end = 0;
	step = strMessage.GetLength() / size;
	end = strMessage.GetLength() % size;

	for(int i = 0; i < step; i++)
	{
		CString strTemp;
		strTemp.Format("%s;%s", strDestination, strMessage.Mid(start, size));
		start += size;
		strcpy(newmsg.Data.textmsg.Message, strTemp);
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
//		DbgOut("\n[Text]2 Send : %s\n", newmsg.Data.textmsg.Message);
	}
	if(end)
	{
		CString strTemp;
		strTemp.Format("%s;%s", strDestination, strMessage.Mid(start, end));
		strcpy(newmsg.Data.textmsg.Message, strTemp);
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
//		DbgOut("\n[Text]3 Send : %s\n", newmsg.Data.textmsg.Message);
	}

  //*/
	return TRUE;
}

void CSCDialer1Ctrl::OnWebCallAgentIPChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnWebCallAgentPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnCallTypeChanged() 
{
	// TODO: Add notification handler code
	if(m_bCall)
		return;

	SetModifiedFlag();
}

void CSCDialer1Ctrl::WCAConnect(LPCTSTR ip, long port)
{
	if( !m_pDataSocketWCA )
		m_pDataSocketWCA = new DataSocket(UM_WCASOCKET);
	if( m_pDataSocketWCA->IsConnecting() ) 
		return;
	m_pDataSocketWCA->Connect( GetSafeHwnd(), ip, port );
	
	m_uMediaCount = 1;
	for(int i = 0; i < m_uMediaCount ; i++) 
	{
		if(!m_pNATSoc[i])
			m_pNATSoc[i] = new CNATSocket(GetSafeHwnd() ,m_MediaInfo[i].MediaPort, 
								m_nRTPmode, i,	m_strUserID); //by cje 010504
		if(m_nNetworkType == _FULLCONE) //by cje 02120
		{
			if(m_bManualRoute) m_pNATSoc[i]->rtpMode = _BIUDPROUTE;
		}
		else if(m_nNetworkType == _RESTRICTED) 
		{
			if(m_nRTPmode < _UNIUDPROUTE) 
				m_pNATSoc[i]->rtpMode = _UNIUDPROUTE; //by cje 021017
			if(m_bManualRoute)
				m_pNATSoc[i]->rtpMode = _BITCPROUTE; //by cje 030318
				//m_pNATSoc[i]->rtpMode = _UNITCPROUTE; //by cje 021130
		}
		else if(m_nNetworkType == _UDPBLOCK) 
		{
			if(m_nRTPmode < _UNITCPROUTE)
				m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
			if(m_bManualRoute)
				m_pNATSoc[i]->rtpMode = _BITCPROUTE; //by cje 030318
		}
		if(!m_bFireTalkExist) m_pNATSoc[i]->rtpMode = _DIRECT;
	}
	m_pNATSoc[0]->SetServerAddr(ip,(unsigned int)m_nNATServerPort); // 20020417  넘겨받은 ip 의 값으로 바뀌어야 함...!!!!
	TRACE("\nWCAConnect End with Networktype:%d, m_bManualRoute : %d  RouteMode :%d,\n",
		m_nNetworkType,m_bManualRoute,m_pNATSoc[0]->rtpMode);
}

void CSCDialer1Ctrl::WCALogIn()
{
	if( !m_pDataSocketWCA ) return;
	if( !m_pDataSocketWCA->IsConnecting() ) return;

	m_msg.Type = _LOGIN;
	m_msg.Data.loginmsg.Product = m_nProductIDWCA; // PREReigistration 에서 받은 값을 입력한다.
	int count = 0;
	CStringArray array;
	count = ParseData(m_strUserPhoneNumber, ',', &array);
	if(count > 0)
		strcpy( m_msg.Data.loginmsg.UserID, array.GetAt(0));
	else
		strcpy(m_msg.Data.loginmsg.UserID, _T(""));

	m_pDataSocketWCA->Send( (char*)&m_msg, sizeof(m_msg)); 
}

/*
	Serial Device Signal ( lParam )
	Serial Device	:	Control
	0 - 12			|	0 - 12			==> DTMF
	13				|	13				==> Hook switch on
	14				|	14				==> Send
	15				|	15				==> Home Page
	16				|	16				==> Phone Book List
	17				|	17				==> Shopping Mall Homepage
	85				|	18				==> Hook On
	170				|	19				==> Hook Off	
	50				|	20				==> Alive Message from SerialPhone to PC(Hoon On State)
	51				|	21				==> Alive Message from SerialPhone to PC(Hoon Off State)
	22				|	22				==> Hook switch off
	없음			|	23				==> Serial device connect Undefine Message
					|	24				==> Redial(MiraPhone) -- cmkwon 20021108
	없음			|	30				==> Serial device connect OK
	없음			|	31				==> Serial device disconnect(통화 도중 Serial device 제거)
	없음			|	32				==> Serial device connect error(Port가 사용중입니다.)
	없음			|	33				==> Serial device connect error(Port가 유효하지 않습니다.)
	없음			|	34				==>	Received Dial Message(PCtoPC로 전화가 온경우)
	없음			|	35				==>	Received Hanup Message(PCtoPC로 전화가 와서 Ringing중에 전화를 건사람이 전화를 끊을때 처리를 위해)
	없음			|	36				==>	Received Busy Message(PCtoPC로 전화를 걸때 상대방이 통화중일때 Busy Tone을 발생하기 위해서)
	없음			|	52				==> Serial device is invalid (Device 가 invalid 하다.)
//*/
long CSCDialer1Ctrl::OnRecvFromSerialPort(WPARAM wParam, LPARAM lParam)
{
	if(m_nDeviceType == 0 || !m_323Connection){	return 0;}
	
	char	temp[10];	
	switch(lParam)
	{
/*	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
		{
			itoa(lParam, temp, 10);
			PushButton(temp);
		}
		break;
	case 10:	PushButton("*");			break;
	case 11:	PushButton("0");			break;
	case 12:	PushButton("#");			break;
*/
	case 1:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			strcpy(temp, "*");

		PushButton(temp);
		break;
	case 2:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(7, temp, 10);

		PushButton(temp);
		break;
	case 3:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(4, temp, 10);

		PushButton(temp);
		break;
	case 4:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(1, temp, 10);

		PushButton(temp);
		break;
	case 5:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(0, temp, 10);

		PushButton(temp);
		break;
	case 6:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(8, temp, 10);

		PushButton(temp);
		break;
	case 7:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(5, temp, 10);

		PushButton(temp);
		break;
	case 8:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			itoa(2, temp, 10);

		PushButton(temp);
		break;
	case 9:
		if(m_nDeviceType == 1)
			itoa(lParam, temp, 10);
		else if(m_nDeviceType == 2)
			strcpy(temp, "#");

		PushButton(temp);
		break;
	case 10:
		if(m_nDeviceType == 1)
			strcpy(temp, "*");
		else if(m_nDeviceType == 2)
			itoa(9, temp, 10);

		PushButton(temp);
		break;
	case 11:
		if(m_nDeviceType == 1)
			strcpy(temp, "0");
		else if(m_nDeviceType == 2)
			itoa(6, temp, 10);

		PushButton(temp);
		break;
	case 12:
		if(m_nDeviceType == 1)
			strcpy(temp, "#");
		else if(m_nDeviceType == 2)
			itoa(3, temp, 10);

		PushButton(temp);
		break;
	case 13:	FireEvHookOff(TRUE);
		m_bIsHookOff = TRUE;
		if(m_nState == _READYSTATE)
		{
			m_nMyStatus = _DIALBLOCK;
			OnMyStatusChanged();
		}
		else if(m_nState == _CONNECTED)  // by gomeisa 2002.11.08 전화가 왔을 때 수화기를 들면 ringback tone 을 없앰 (Phone2PC 에서 음성 채널이 늦게 열려 링소리가 나는 것을 제거하기위해
		{
			m_323Connection->LocalPlayG7231(0, FALSE);
		}
		break;
	case 14: case 15: case 16: case 17:
		// Function Key	
		{
			if(m_nDeviceType == 2)
			{
				if(lParam == 17)
				{
					lParam = 14;
				}
				else if(lParam == 14)
				{
					lParam = 24;
				}
			}			
			
			FireEvSerialMessage(lParam);
		}
		break;
	case 18:	FireEvHookOn(FALSE);	
		m_bIsHookOff = FALSE;
		m_nMyStatus = _READYSTATE;
		OnMyStatusChanged();
		break;
	case 19:	FireEvHookOff(FALSE);	
		m_bIsHookOff = TRUE;
		if(m_nState == _READYSTATE)
		{
			m_nMyStatus = _DIALBLOCK;
			OnMyStatusChanged();
		}
		else  if(m_nState == _CONNECTED)// by gomeisa 2002.11.08 전화가 왔을 때 수화기를 들면 ringback tone 을 없앰 (Phone2PC 에서 음성 채널이 늦게 열려 링소리가 나는 것을 제거하기위해
		{
			m_323Connection->LocalPlayG7231(0, FALSE);
		}
		break;
	case 20: case 21: m_323Connection->SerialPhoneMessage(lParam);	break;		// Send Message to Serial Phone
	case 22:	FireEvHookOn(TRUE);	
		m_bIsHookOff = FALSE;
		m_nMyStatus = _READYSTATE;
		OnMyStatusChanged();
		break;
	case 30:	FireEvSerialConnect();		break;
	case 31:	FireEvSerialDisconnect();	break;
	case 32:	FireEvError(SERIAL_DEVICE_USEING, _T(""));	break;
	case 33:	FireEvError(SERIAL_INVALID_PORT, _T(""));	break;
	case 52:	FireEvError(SERIAL_INVALID_DEVICE, _T("")); break;
		// log
	case 10000:
		{
			CString strMsg; strMsg.Format("%d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
			break;
		}
	case 10001:
		{
			CString strMsg; strMsg.Format("Try to port : %d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
			break;
		}
	case 10002:
		{
			CString strMsg; strMsg.Format("Open fail port : %d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
			break;
		}	
	case 10003: // m_lRxSerialData
		{
			CString strMsg; strMsg.Format("RxSerialData;%d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
		}
		break;
	case 10004: // m_lTxSerialData
		{
			CString strMsg; strMsg.Format("TxSerialData;%d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
		}
		break;
	case 10005: // m_lRxRTPData
		{
			CString strMsg; strMsg.Format("RxRTPData;%d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
		}
		break;
	case 10006: // m_lTxRTPData
		{
			CString strMsg; strMsg.Format("TxRTPData;%d", wParam);
			FireEvReceiveMessage("[Serial]", strMsg);
		}
		break;

	default:
		{
		}
	}
	return TRUE;
}

void CSCDialer1Ctrl::OnDeviceTypeChanged() 
{
	// TODO: Add notification handler code

	TRACE("\n[OnDeviceTypeChanged : DeviceType = %d] ", m_nDeviceType);
	if(m_nDeviceType == 0) // 일반 AudioDevice 라면
	{
//		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		
		TRACE("SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)");
	}
	else
	{
		OSVERSIONINFO	os;
		memset(&os, 0x00, sizeof(OSVERSIONINFO));
		os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&os);
		
		if(os.dwPlatformId != VER_PLATFORM_WIN32_NT)
		{
//			SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);		
			TRACE("SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS)");
		}
		else
		{
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);		
			TRACE("SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)");
		}
	}
	
	if(m_323Connection)
	{
		delete m_323Connection;
		m_323Connection = NULL;
	}
	
	if(m_nDeviceType == 0)			// Sound Card
	{
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
		m_323Connection = new CH323Connection(this, t_G7231, t_NULL, m_bInBand, 2, 10, GetSafeHwnd(), FALSE, 1); // Multimedia control construct
	}
	else// if(m_nDeviceType == 1)		// Serial Phone
	{
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
		m_323Connection = new CH323Connection(this, t_G7231, t_NULL, m_bInBand, 2, 10, GetSafeHwnd(), TRUE, m_nSerialPort); // Multimedia control construct
	}

	if(m_323Connection)
	{
		m_323Connection->SetKeepAliveSerial(m_strKeepAlive);
	}

	SetModifiedFlag();
}

long CSCDialer1Ctrl::SerialMessage(long dwMsg) 
{
	// TODO: Add your dispatch handler code here

	if(m_nDeviceType == 0) // Serial Phone 일 경우만
		return -1;
	m_323Connection->SerialPhoneMessage(dwMsg);			// Send Message to Serial Phone
	return 0;
}

void CSCDialer1Ctrl::OnSerialPortChanged() 
{
	// TODO: Add notification handler code
	if(m_nDeviceType != 0)		// Serial Phone
	{
		if(m_323Connection)
		{
			delete m_323Connection;
			m_323Connection = NULL;
		}

		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
		m_323Connection = new CH323Connection(this, t_G7231, t_NULL, m_bInBand, 2, 10, GetSafeHwnd(), TRUE, m_nSerialPort); // Multimedia control construct

		if(m_323Connection)
		{
			m_323Connection->SetKeepAliveSerial(m_strKeepAlive);
		}
	}
	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnAutoRetryLoginIntervalChanged() 
{
	// TODO: Add notification handler code
	if(m_nAutoRetryLoginInterval < 0)
		m_nAutoRetryLoginInterval = 0;
	else if(m_nAutoRetryLoginInterval >= 60)
		m_nAutoRetryLoginInterval = 60;

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnPlayDeviceChanged() 
{
	// TODO: Add notification handler code
	if(m_323Connection)
	{
		m_323Connection->SetPlaySoundDevice(m_strPlayDevice);
	}

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnRecordDeviceChanged() 
{
	// TODO: Add notification handler code
	if(m_323Connection)
	{
		m_323Connection->SetRecordSoundDevice(m_strRecordDevice);
	}

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnWaveDeviceChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}
long CSCDialer1Ctrl::OnNATAnalyzerResult(WPARAM wParam, LPARAM lParam)
{
	char str[256];
	CNATAnalyzer* pAnalyzer = (CNATAnalyzer*)lParam;

	if(!pAnalyzer){ return 0;}
	
	m_bAnalyzed= TRUE;
	
	if(wParam)
	{
		switch(pAnalyzer->GetNetworkType())
		{
		case _PUBLIC_TYPE: //직접통화
			{
				strcpy(str, "Type : Public Open Internet");
				m_nNetworkType = _OPENINTERNET;
				
			}
			break;
		case _NAT_CONE: //포트 매핑후 전달...직접통화
			{
				strcpy(str, "Type : Cone");
				m_nNetworkType = _FULLCONE;
			}
			break;
		case _NAT_SYMETRIC: //UDP 중계(단방향)
			{
				strcpy(str, "Type : Symetric");
				m_nNetworkType = _RESTRICTED;
//				if(m_nRTPmode < _UNIUDPROUTE) m_nRTPmode = _UNIUDPROUTE;
			}
			break;
		case _NOTUSE_UDP:  //TCP중계.. 일단은 _UNITCPROUTE
			{
				strcpy(str, "Type : Firewall");
				m_nNetworkType = _UDPBLOCK;	
//				if(m_nRTPmode < _UNITCPROUTE) m_nRTPmode = _UNITCPROUTE;
			}
			break;
		default:
			{
				strcpy(str, "Type : default");
			}
		}		
	}
	else
	{
		m_nNetworkType = _UDPBLOCK; //by cje 030319
		pAnalyzer->GetErrorInfosString(str);
		
	}//에러발생시 처리는????/ by cje 020902
	
	m_nRealNetworkType = m_nNetworkType; //실제 분석에 의한 타입.(저장)
	
	if(m_nRTPmode >= _UNIUDPROUTE && m_nNetworkType < _RESTRICTED) //by cje 030319
		m_nNetworkType = _RESTRICTED;
	if(m_nRTPmode >= _UNITCPROUTE && m_nNetworkType < _UDPBLOCK) 
		m_nNetworkType = _UDPBLOCK;
	
	if(m_nState >= _DIALBLOCK)  //by cje 020912
	{
		m_bReAnalyzed = TRUE;
		if(m_bCaller)
		{
			if(m_nCallType == 1) //PC2Phone
			{
				for(int i=0; i< m_uMediaCount;i++)
					m_pNATSoc[i]->SetLocalRTPPort();
				if(m_nNetworkType == _OPENINTERNET)
				{
					for(int i=0; i< m_uMediaCount;i++)
						m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
					SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
				}
				else if(m_nNetworkType == _FULLCONE)
				{
					for(int i=0; i< m_uMediaCount;i++)
						m_pNATSoc[i]->SetGlobalRTPPort(); 
				}
				else if(m_nNetworkType >= _RESTRICTED)
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
						if(m_pNATSoc[i]->rtpMode < _UNIUDPROUTE) 
							m_pNATSoc[i]->rtpMode = _UNIUDPROUTE;
//						if(m_pNATSoc[i]->rtpMode < _UNITCPROUTE) 
//							m_pNATSoc[i]->rtpMode = _UNITCPROUTE; //by cje 021126
					}
					SendRouteMsg();
				}
			}
			else
				OnSocketOK();
		}
		else
			OnSocketDial();
	}
	else
		Registration();

	return 0;
}

long CSCDialer1Ctrl::TimeOutTone() 
{
	// TODO: Add your dispatch handler code here
	m_323Connection->LocalPlayG7231(6, TRUE);
	return 0;
}

void CSCDialer1Ctrl::FireReDial() 
{
	// TODO: Add your dispatch handler code here
	m_strPhoneNumber = m_strPrevPhoneNumber;
	m_nCallType = m_nPrevCallType;
	FireDial();
}

BOOL CSCDialer1Ctrl::GetIPAddress(char *GateWayIP,char* LocalIP)
{
	memset(LocalGWIP,0x00,sizeof(LocalGWIP));
	memset(LocalIP,0x00,sizeof(LocalIP));
	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	DWORD     AdapterInfoSize, Err;
	int GateWayFind = TRUE;
	
	AdapterInfoSize = 0;
	if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != ERROR_SUCCESS)
	{
		if (Err != ERROR_BUFFER_OVERFLOW)
		{
//			char b[100];
//			sprintf (b, "GetAdaptersInfo sizing failed with error %d\n", Err);
//			AfxMessageBox (b);            
			return FALSE;
		}
	}
	
	// Allocate memory from sizing information
	if ((pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize)) 
		== NULL)
	{
//		char b[100];
//		sprintf(b, "[Gateway] Memory allocation error\n");
//		AfxMessageBox ( b);
		return FALSE;
	}
	
	// Get actual adapter information
	if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	{
		
		return FALSE;
	}
	
	pAdapt = pAdapterInfo;
	
	while (GateWayFind)
	{
		if (pAdapt->GatewayList.IpAddress.String == NULL || *(pAdapt->GatewayList.IpAddress.String) == NULL)
		{
			pAdapt = pAdapt->Next;
			if(pAdapt == NULL)
			{
				if(pAdapterInfo)
				{
					GlobalFree(pAdapterInfo);
				}
				return FALSE;

			}
		}
		else
		{
//			*GateWayIP = inet_addr(pAdapt->GatewayList.IpAddress.String);
			strcpy(LocalIP,pAdapt->IpAddressList.IpAddress.String);
			strcpy(GateWayIP, pAdapt->GatewayList.IpAddress.String);
//			GateWayFind = NULL;
			pAdapt = pAdapt->Next;
			if(pAdapt == NULL || 0 != strcmp(LocalIP,GateWayIP)){
				GateWayFind = FALSE;
//				CString ipout;
//				ipout.Format("LocalIP:%s\nGateWayIP:%s",LocalIP,GateWayIP);
//				AfxMessageBox(ipout);
			}
		}
	}
	if(pAdapterInfo)
	{
		GlobalFree(pAdapterInfo);
	}
  
	return TRUE;
}

void CSCDialer1Ctrl::OnSocketOK()
{
	TRACE("_OK\n");
	m_bCall = TRUE;
	int i;
	if(m_msgOK.Data.loginmsg.Product == _INTERNAL)//by cje 011206
	{
		m_bInternalCall = TRUE;
		CString tmp;
		tmp.Format("CreateRTPSession with port %d\n", BASERTPPORT);
		TRACE(tmp);
		m_323Connection->CreateRTPSession(BASERTPPORT, t_Audio, 0);
		if(m_uMediaCount > 1)
		{
			for(i=1; i< m_uMediaCount;i++)
			{
				if(i == 1) 
					FireEvVideoCreateInfo((long)(BASERTPPORT + 2*i));
			}
		}
		SendDialMsg(m_323Connection->GetLocalAudioPort(0));
	}
	else
	{
		m_nPeerNetworkType = m_msgOK.Data.loginmsg.Product;//서버에서 _OK에 넣어서 보낸다	
		TRACE("\nPeerNetworkType : %d",m_nPeerNetworkType);
		if(!m_pNATSoc[0])
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i] = new CNATSocket(GetSafeHwnd() , m_MediaInfo[i].MediaPort,
					m_nRTPmode, i, m_strUserID); //by cje 010504
			}
		}
		for(int i=0; i<m_uMediaCount;i++)
		{
			m_pNATSoc[i]->SetServerAddr(m_strSCDialer1ServerIP,(unsigned int)m_nNATServerPort); //by cje 010416
			m_pNATSoc[i]->SetProxyAddr(m_msgFireTalkAddr.Data.routemsg.ProxyServerIP, (unsigned)m_msgFireTalkAddr.Data.routemsg.ProxyServerPort);
			if(!m_bFireTalkExist) 
				m_pNATSoc[i]->rtpMode = _DIRECT;
		}
		
		
		if(m_nNetworkType == _UDPBLOCK )
		{
			//TCP중계
			for(int i=0; i<m_uMediaCount;i++)
			{
				if(m_pNATSoc[i]->rtpMode < _UNITCPROUTE) 
					m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
				if(m_bManualRoute)
					m_pNATSoc[i]->rtpMode = _BITCPROUTE; //by cje 030318
			}
			SendRouteMsg();
		}
		else if(m_nNetworkType == _OPENINTERNET)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_nPeerNetworkType == _RESTRICTED || m_nPeerNetworkType == _FULLCONE)//passive
			{
				m_nRTPTransferType = _PASSIVE;
				SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
			}
			else
			{
				SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
			}
		}
		else if(m_nNetworkType == _FULLCONE)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_nPeerNetworkType == _OPENINTERNET)
			{
				//active RTP
				m_nRTPTransferType = _ACTIVE;
				SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
			}
			else 
			{
				//passive RTP
				if(m_nPeerNetworkType <= _RESTRICTED)
					m_nRTPTransferType = _PASSIVE;
				if(m_nRTPTransferType == _NORMAL && m_bManualRoute) //by cje 021212
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
						m_pNATSoc[i]->rtpMode = _BIUDPROUTE;
					}	
					SendRouteMsg();
				}
				else
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(); //이후 OnNATResult에서 받은 포트값을 SendDialMsg로 전달
					}
				}

			}
		}
		else if(m_nNetworkType == _RESTRICTED)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_nPeerNetworkType >= _RESTRICTED || m_pNATSoc[0]->rtpMode >= _UNIUDPROUTE)
			{
				//Normal.._UNIUDP
				for(int i=0; i<m_uMediaCount;i++)
				{
					m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
					if(m_pNATSoc[i]->rtpMode < _UNIUDPROUTE) 
						m_pNATSoc[i]->rtpMode = _UNIUDPROUTE;
					
					if(m_bManualRoute)//by cje 021130
						m_pNATSoc[i]->rtpMode = _BITCPROUTE; // by cje 030318
						//m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
				}
				SendRouteMsg();
			}
			else
			{
				//active RTP
				m_nRTPTransferType = _ACTIVE;
				SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
			}
		}
		// 2003.11.05 by gomeisa : m_nNetworkType 이 없으면 그냥 시도해라..!!!!  FT 없는 상태에서
		// PC2PC 할 경우 두번째 콜에서는 m_nNetworkType 이 NULL 값이 되어서 수정함..
//		else if(m_nNetworkType == _NOINFO) //FireTalk가 없어서 네트워크 분석을 못한 상태..
		else //FireTalk가 없어서 네트워크 분석을 못한 상태..
		{
			//그냥 직접통화 시도.
			m_nNetworkType = _NOINFO;
			SendDialMsg(m_pNATSoc[0]->GetLocalRTPPort());
		}
	}
	
	
}
void CSCDialer1Ctrl::OnSocketDial() 
{
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	int i;
	// 10.12  이미 통화중일 때는 통화중임을 Server에 알린다. 결국 Alert message가 상대방에게 가지 않음.				
	if(m_bCall)
	{
		msg.Type = _ERROR;
		msg.Data.ErrorInfo = INCONF;
		if(m_pDataSocketSCDialer1)
			m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
		// 아래 사항을 수행하면 기존의 콜도 끊기게 되므로... 2002.12.23 by gomeisa
		//FireHangup();
		//m_bCall = FALSE;
		return;
	}
	msg.Type = _ALERT;
	if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
	m_nState = _ALERTINGSTATE;
	FireEvStatus(m_nState);
	m_bCall = TRUE;				
	
	CStringArray arrayData;
	//Caller의 IP와 PCN이 함께 넘어온다.. Svr의 Dial 함수에서 세팅..
	int count = ParseData(m_msgDIAL.Data.dialmsg.PhoneNumber, ';', &arrayData);
	
	if(count > 0)
		m_strPeerIP = arrayData.GetAt(0);
	if(count > 1)
		m_strCallerPCN = arrayData.GetAt(1);
	if(count > 2)  //by cje
	{
		if("0" == arrayData.GetAt(2)) m_bInternalCall = TRUE;
		else if("1" == arrayData.GetAt(2)) m_nPeerNetworkType = _OPENINTERNET;
		else if("2" == arrayData.GetAt(2)) m_nPeerNetworkType = _FULLCONE;
		else if("3" == arrayData.GetAt(2)) m_nPeerNetworkType = _RESTRICTED;
		else if("4" == arrayData.GetAt(2)) m_nPeerNetworkType = _UDPBLOCK;
		else if("6" == arrayData.GetAt(2)) m_nPeerNetworkType = _NOINFO;
	}
	m_nPeerPort = m_msgDIAL.Data.dialmsg.RtpPort;
	
	if(m_bInternalCall)//by cje 011206
	{
		CString tmp;
		tmp.Format("CreateRTPSession with port %d\n",BASERTPPORT);
		TRACE(tmp);
		PlaySound(NULL, NULL, NULL);
		Sleep(10);
		if( !m_323Connection->CreateRTPSession(BASERTPPORT, t_Audio, 0) ) 
		{
			msg.Type = _ERROR;
			if(m_pDataSocketSCDialer1) m_pDataSocketSCDialer1->Send((char*)&msg, sizeof(MESSAGE));
			// 4 : CreateRTPSession 이 실패했을 때
			FireHangup(4);
			AfxMessageBox("m_323Connection Create fail");
			return ;
		}
		if(m_uMediaCount > 1)
		{
			for(i=1; i< m_uMediaCount;i++)
			{
				if(i == 1) 
					FireEvVideoCreateInfo((long)(BASERTPPORT + 2*i));
			}
		}
		OnReceivedDial(m_323Connection->GetLocalAudioPort(0));
	}
	else
	{

		if(!m_strPeerIP.Compare("ph2pc")) // added by cje 030331
			m_uMediaCount = 1;

		if(!m_pNATSoc[0])
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i] = new CNATSocket(GetSafeHwnd() , m_MediaInfo[i].MediaPort
					,m_nRTPmode, i, m_strUserID); //by cje 010504
			}
		}
		for(int i=0; i<m_uMediaCount;i++)
		{
			m_pNATSoc[i]->SetServerAddr(m_strSCDialer1ServerIP,(unsigned int)m_nNATServerPort); //by cje 010416
			m_pNATSoc[i]->SetProxyAddr(m_msgFireTalkAddr.Data.routemsg.ProxyServerIP, (unsigned)m_msgFireTalkAddr.Data.routemsg.ProxyServerPort);
			if(!m_bFireTalkExist) m_pNATSoc[i]->rtpMode = _DIRECT;
		}
		

		TRACE("OnSocketDial_EXTERNAL_m_nNetworkType(%d)_m_nPeerNetworkType(%d)\n",m_nNetworkType,m_nPeerNetworkType);
		if(m_nNetworkType == _UDPBLOCK )
		{
			//TCP중계
			for(int i=0; i<m_uMediaCount;i++)
			{
				if(m_pNATSoc[i]->rtpMode < _UNITCPROUTE) 
					m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
				if(m_bManualRoute)
					m_pNATSoc[i]->rtpMode = _BITCPROUTE; // by cje 030318
			}
			SendRouteMsg();
		}
		else if(m_nNetworkType == _OPENINTERNET)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_nPeerNetworkType == _RESTRICTED || m_nPeerNetworkType == _FULLCONE)//passive
			{
				m_nRTPTransferType = _PASSIVE;
				OnReceivedDial(m_pNATSoc[0]->GetLocalRTPPort());
			}
			else
			{
				OnReceivedDial(m_pNATSoc[0]->GetLocalRTPPort());
			}
		}
		else if(m_nNetworkType == _FULLCONE)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_strPeerIP.Compare("ph2pc"))
			{
				m_bIsPh2PC = FALSE;
				if(m_nPeerNetworkType <= _FULLCONE)
				{
					//active RTP
					m_nRTPTransferType = _ACTIVE;
					OnReceivedDial(m_pNATSoc[0]->GetLocalRTPPort());
				}
				else 
				{
					//passive RTP
					if(m_nPeerNetworkType == _RESTRICTED)
						m_nRTPTransferType = _PASSIVE;
					if(m_nRTPTransferType == _NORMAL && m_bManualRoute) //by cje 021212
					{
						for(int i=0; i<m_uMediaCount;i++)
						{
							m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
							m_pNATSoc[i]->rtpMode = _BIUDPROUTE;
						}	
						SendRouteMsg();
					}
					else
					{
						for(int i=0; i<m_uMediaCount;i++)
						{
							m_pNATSoc[i]->SetGlobalRTPPort(); //이후 OnNATResult에서 받은 포트값을 SendDialMsg로 전달
						}
					}
				}
			}
			else //ph2pc 021118
			{
				m_bIsPh2PC = TRUE;
				if(m_bManualRoute) //by cje 021212
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
						m_pNATSoc[i]->rtpMode = _BIUDPROUTE;
					}	
					SendRouteMsg();
				}
				else
				{
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(); //이후 OnNATResult에서 받은 포트값을 SendDialMsg로 전달
					}
				}
			}
		}
		else if(m_nNetworkType == _RESTRICTED)
		{
			for(int i=0; i<m_uMediaCount;i++)
			{
				m_pNATSoc[i]->SetLocalRTPPort();
			}
			if(m_nPeerNetworkType >= _RESTRICTED || m_pNATSoc[0]->rtpMode >= _UNIUDPROUTE)
			{
				//Normal.._UNIUDP
				
				for(int i=0; i<m_uMediaCount;i++)
				{
					m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
					if(m_pNATSoc[i]->rtpMode < _UNIUDPROUTE) 
						m_pNATSoc[i]->rtpMode = _UNIUDPROUTE;
					if(m_bManualRoute) //by cje 021130
						m_pNATSoc[i]->rtpMode = _BITCPROUTE; //by cje 030318
						//m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
				}
				SendRouteMsg();
			}
			else
			{
				if(m_strPeerIP.Compare("ph2pc")) //pc2pc
				{//active RTP
					m_bIsPh2PC = FALSE;
					m_nRTPTransferType = _ACTIVE;
					OnReceivedDial(m_pNATSoc[0]->GetLocalRTPPort());
				}
				else //ph2pc 021118 앞으로는 peer가 UDPBLOCK로 오기때문에 여기는 실행도지 않는다.
				{
					m_bIsPh2PC = TRUE;
					for(int i=0; i<m_uMediaCount;i++)
					{
						m_pNATSoc[i]->SetGlobalRTPPort(m_MediaInfo[i].MediaPort);
//						if(m_pNATSoc[i]->rtpMode < _UNIUDPROUTE) 
//							m_pNATSoc[i]->rtpMode = _UNIUDPROUTE;
						if(m_pNATSoc[i]->rtpMode < _UNITCPROUTE) 
							m_pNATSoc[i]->rtpMode = _UNITCPROUTE;
					}
					TRACE("OnSocketDial_ph2pc_RESTRICTED_SendRouteMsg\n");
					SendRouteMsg();	
				}
			}
		}
		// 2003.11.05 by gomeisa : m_nNetworkType 이 없으면 그냥 시도해라..!!!!  FT 없는 상태에서
		// PC2PC 할 경우 두번째 콜에서는 m_nNetworkType 이 NULL 값이 되어서 수정함..
//		else if(m_nNetworkType == _NOINFO) //FireTalk가 없어서 네트워크 분석을 못한 상태..
		else //FireTalk가 없어서 네트워크 분석을 못한 상태..
		{
			//그냥 직접통화 시도.
			m_nNetworkType = _NOINFO; // 2003.11.05
			OnReceivedDial(m_pNATSoc[0]->GetLocalRTPPort());
		}
	}
	if(m_nDeviceType != 0){	m_323Connection->SerialPhoneMessage(34);}	// Send Message to Serial Phone
}

BSTR CSCDialer1Ctrl::GetVersion() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	char strFilePath[100] = "";
	char strVersion[50] = "";
	char strBuildNo[50] = "";
	strcpy(strFilePath, GetPathRegistered("E43303FE-BDE8-44e2-87BF-259D920F3ADD"));
	GetModuleVersion(strFilePath, strVersion, strBuildNo);

	strResult.Format("%s;%s;%s", strVersion, strBuildNo, strFilePath);

	return strResult.AllocSysString();
}

CString CSCDialer1Ctrl::GetPathRegistered(CString strCLSID)
{
    CString strPathRegistered;
    CWindowsRegistry MyRegistry;
    CString strKeyName = "\\HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\CLSID\\{" + strCLSID +"}";

    // Open Registry (note: the base key "LOCAL_MACHINE" is part of the KeyName)
    LONG nResult=MyRegistry.OpenKey(strKeyName, NULL, KEY_READ);
    if (nResult == ERROR_SUCCESS) {
        // Loop on all subkeys
        TCHAR lpszSubKey[FWR_MAXIMUM_KEY_SIZE];
        TCHAR lpszImagePathSubKey[FWR_MAXIMUM_KEY_SIZE];
        BOOL bFound = FALSE;
        for (int i=0; MyRegistry.EnumerateSubKeys(i, lpszSubKey)==TRUE && !bFound; i++) {
            char *lpszImagePath;
			if(strcmp(lpszSubKey, "InprocServer32"))
				continue;
            strcpy(lpszImagePathSubKey, lpszSubKey);
            strcat(lpszImagePathSubKey, "\\");
            if (MyRegistry.QueryValue(lpszImagePathSubKey,&lpszImagePath) == ERROR_SUCCESS) 
			{
				strPathRegistered.Format("%s", lpszImagePath);

                if (lpszImagePath) 
				{
                    delete [] lpszImagePath;
                }
				break;
            }
       }
    }

    return strPathRegistered;
}

BOOL CSCDialer1Ctrl::GetModuleVersion(LPTSTR lpDestFile, LPTSTR strVersion, LPTSTR strBuild)
{
	m_info.ReadVersionInfo(lpDestFile);
	strcpy(strVersion, m_info.GetVersionInfo(2));
	strcpy(strBuild, m_info.GetVersionInfo(11));
	
	return TRUE;
}

void CSCDialer1Ctrl::OnNATServerPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer1Ctrl::OnNATFireTalkPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}


void CSCDialer1Ctrl::SendVideoPause(long Flag) 
{
	// TODO: Add your dispatch handler code here
	if(m_pDataSocketSCDialer1 && m_bCall && m_bRegistered && !m_nCallType/*PC2PC*/ && !m_bIsPh2PC)
	{
		MESSAGE newmsg;
		memset(&newmsg, 0x00, sizeof(MESSAGE));
		newmsg.Type = _VIDEOPAUSE;
		newmsg.Data.videopausemsg.Flag = Flag;
		strcpy(newmsg.Data.videopausemsg.Pause, "");
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
	}		
}

void CSCDialer1Ctrl::OnManualRouteChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

CString CSCDialer1Ctrl::GetMyIP()
{
	char lpszHostName[255];
	char szTemp[255];
	PHOSTENT pHostEntry; 
	struct in_addr inAddr;
	if(gethostname(lpszHostName,255) != 0 ) //호스트 이름 얻기
		wsprintf(szTemp, "gethostname() error : %d", WSAGetLastError());
	else 
	{
		if ( ( pHostEntry = gethostbyname( lpszHostName ) ) == NULL ) //호스트 이름로 호스트 entry 얻기
			wsprintf( szTemp, "gethostbyname() error : %d", WSAGetLastError() );
		else memcpy( &inAddr, pHostEntry->h_addr, 4 );
	} 

	return inet_ntoa(inAddr);
}

void CSCDialer1Ctrl::SendMoneyData()
{
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _MONEY;
	strcpy(newmsg.Data.moneymsg.Money, m_strPhoneNumber);
	if(m_pDataSocketSCDialer1)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::SetKeepAliveSerial(LPCTSTR KeepAlive) 
{
	// TODO: Add your dispatch handler code here
	m_strKeepAlive = KeepAlive;
	if(m_323Connection)
		m_323Connection->SetKeepAliveSerial(m_strKeepAlive);
}

void CSCDialer1Ctrl::MakeConference(short AttendantCount, LPCTSTR ConferenceInfo) 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _REQMAKECONFERENCE;
	newmsg.Data.reqmakeconferencemsg.AttendantCount = AttendantCount;
	strcpy(newmsg.Data.reqmakeconferencemsg.MakeConferenceInfo, ConferenceInfo);
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::JoinConference(LPCTSTR ConferenceID, LPCTSTR ConferenceInfo) 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _REQJOINCONFERENCE;
	strcpy(newmsg.Data.reqjoinconferencemsg.JoinRoomID, ConferenceID);
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::InviteConference(LPCTSTR Callee) 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _REQINVITECONFERENCE;
	strcpy(newmsg.Data.reqinviteconferencemsg.InviteUserID, Callee);
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::CloseConference() 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _CLOSECONFERENCE;
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::StatusConference(short Status, LPCTSTR StatusInfo) 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _STATUSCONFERENCE;
	newmsg.Data.statusconferencemsg.Status = Status;
	strcpy(newmsg.Data.statusconferencemsg.StatusInfo, StatusInfo);
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

void CSCDialer1Ctrl::LeaveConference() 
{
	// TODO: Add your dispatch handler code here
	MESSAGE newmsg;
	memset(&newmsg, 0x00, sizeof(MESSAGE));
	newmsg.Type = _LEAVECONFERENCE;
	if(m_pDataSocketSCDialer1 && m_bRegistered)
		m_pDataSocketSCDialer1->Send((char*)&newmsg, sizeof(newmsg));
}

long CSCDialer1Ctrl::GetSupportedVoiceCodec()
{
	return 0;
}
void CSCDialer1Ctrl::SetSupportedVoiceCodec(long nVoiceCodec)
{

}
long CSCDialer1Ctrl::GetSupportedVideoCodec()
{
	return 0;
}
void CSCDialer1Ctrl::SetSupportedVideoCodec(long nVideoCodec)
{

}
void CSCDialer1Ctrl::SetVoiceCodec(long nVoiceCodec)
{

}
long CSCDialer1Ctrl::GetVoiceCodec()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
long CSCDialer1Ctrl::SetSpkVolume(long i_nVolumeValueOfPercent) 
{
	// TODO: Add your dispatch handler code here
//	DbgOut("081021-3 CSCDialer1Ctrl::SetSpkVolume %d\r\n", i_nVolumeValueOfPercent);

	BOOL bRet = FALSE;
	if(NULL == m_323Connection)
	{
		return FALSE;
	}
	bRet	= m_323Connection->SetPlayVolumeW(i_nVolumeValueOfPercent);	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
long CSCDialer1Ctrl::GetSpkVolume() 
{
	// TODO: Add your dispatch handler code here
	BYTE byVolOfPercent = 0;
	if(NULL == m_323Connection)
	{
		return byVolOfPercent;
	}

	m_323Connection->GetPlayVolumeW(&byVolOfPercent);		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	return byVolOfPercent;
}
