// SCDialer2Ctl.cpp : Implementation of the CSCDialer2Ctrl ActiveX Control class.

#include "stdafx.h"
#include "SCDialer2.h"
#include "SCDialer2Ctl.h"
#include "SCDialer2Ppg.h"
#include "VoIPProtocol.h"
#include "S2DWinSocket.h"
#include "S2CWinSocket.h"
#include "AudioCodec.h"
#include "AVManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERID_S2C_SEND_ALIVEPACKET				100
#define TIMERGAP_S2C_SEND_ALIVEPACKET				10000		// 10초

#define TIMERID_RESET_CONTROL						101
#define TIMERGAP_RESET_CONTROL						1000		// 5초


IMPLEMENT_DYNCREATE(CSCDialer2Ctrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSCDialer2Ctrl, COleControl)
	//{{AFX_MSG_MAP(CSCDialer2Ctrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_S2D_ASYNC_EVENT, OnS2DAsyncEventWinSocket)
	ON_MESSAGE(WM_S2D_PACKET_NOTIFY, OnS2DPacketNotifyWinSocket)
	ON_MESSAGE(WM_S2C_ASYNC_EVENT, OnS2CAsyncEventWinSocket)
	ON_MESSAGE(WM_S2C_PACKET_NOTIFY, OnS2CPacketNotifyWinSocket)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSCDialer2Ctrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSCDialer2Ctrl)
	DISP_PROPERTY_NOTIFY(CSCDialer2Ctrl, "strVDServerIP", m_strVDServerIP, OnStrVDServerIPChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CSCDialer2Ctrl, "usVDServerPort", m_usVDServerPort, OnUsVDServerPortChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer2Ctrl, "nStatus", m_nStatus, OnStatusChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CSCDialer2Ctrl, "AudioSendPause", m_audioSendPause, OnAudioSendPauseChanged, VT_BOOL)
	DISP_FUNCTION(CSCDialer2Ctrl, "JoinRoom", JoinRoom, VT_I4, VTS_BSTR VTS_PI4)
	DISP_FUNCTION(CSCDialer2Ctrl, "GoOutRoom", GoOutRoom, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSCDialer2Ctrl, "CreateRoom", CreateRoom, VT_I4, VTS_BSTR VTS_PI4)
	DISP_FUNCTION(CSCDialer2Ctrl, "CreateRoomEx", CreateRoomEx, VT_I4, VTS_BSTR VTS_PI4 VTS_I4)
	DISP_FUNCTION(CSCDialer2Ctrl, "SetSpkVolume", SetSpkVolume, VT_I4, VTS_I4)
	DISP_FUNCTION(CSCDialer2Ctrl, "GetSpkVolume", GetSpkVolume, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSCDialer2Ctrl, COleControl)
	//{{AFX_EVENT_MAP(CSCDialer2Ctrl)
	EVENT_CUSTOM("EvStatus", FireEvStatus, VTS_I4)
	EVENT_CUSTOM("EvError", FireEvError, VTS_I4  VTS_I4  VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvOtherJoinedRoom", FireEvOtherJoinedRoom, VTS_BSTR)
	EVENT_CUSTOM("EvOtherGoOutedRoom", FireEvOtherGoOutedRoom, VTS_BSTR)
	EVENT_CUSTOM("EvTalk", FireEvTalk, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvNotify", FireEvNotify, VTS_I4  VTS_I4  VTS_I4  VTS_BSTR)
	EVENT_CUSTOM("EvChangedTalkingState", FireEvChangedTalkingState, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvUserJoinedRoom", FireEvUserJoinedRoom, VTS_BSTR  VTS_I4)
	EVENT_CUSTOM("EvVolume", FireEvVolume, VTS_I2  VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSCDialer2Ctrl, 1)
	PROPPAGEID(CSCDialer2PropPage::guid)
END_PROPPAGEIDS(CSCDialer2Ctrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCDialer2Ctrl, "SCDIALER2.SCDialer2Ctrl.1",
	0xadaa5f78, 0xb23, 0x4c82, 0xb4, 0xd, 0x9a, 0x54, 0x7b, 0x2f, 0x90, 0x49)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSCDialer2Ctrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSCDialer2 =
		{ 0x3d96ca2e, 0xe0c3, 0x4b45, { 0x80, 0xfd, 0xb, 0x86, 0xc7, 0x93, 0x73, 0x89 } };
const IID BASED_CODE IID_DSCDialer2Events =
		{ 0x6033d1d6, 0x6a74, 0x4f8e, { 0x80, 0x7b, 0xa4, 0xeb, 0x85, 0x26, 0x34, 0x46 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSCDialer2OleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSCDialer2Ctrl, IDS_SCDIALER2, _dwSCDialer2OleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::CSCDialer2CtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCDialer2Ctrl

BOOL CSCDialer2Ctrl::CSCDialer2CtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SCDIALER2,
			IDB_SCDIALER2,
			afxRegApartmentThreading,
			_dwSCDialer2OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::CSCDialer2Ctrl - Constructor

CSCDialer2Ctrl::CSCDialer2Ctrl()
{
	InitializeIIDs(&IID_DSCDialer2, &IID_DSCDialer2Events);

	// TODO: Initialize your control's instance data here.

	m_nCtlStatus		= _SC2_READY_STATUS;
	this->m_nStatus		= this->m_nCtlStatus;
	m_nCtlError			= _SC2_NO_ERROR;

	m_pAVManager		= NULL;
	m_pS2DWinSocket		= NULL;
	m_pS2CWinSocket		= NULL;	

	m_bCreateRoomFlag	= FALSE;
	memset(m_szVCServerIP, 0x00, SIZE_MAX_IPADDRESS);
	m_usVCServerPort	= 0;
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::~CSCDialer2Ctrl - Destructor

CSCDialer2Ctrl::~CSCDialer2Ctrl()
{
	// TODO: Cleanup your control's instance data here.
	
	SAFE_DELETE(m_pAVManager);
	SAFE_DELETE(m_pS2DWinSocket);
	SAFE_DELETE(m_pS2CWinSocket);
}

void CSCDialer2Ctrl::ResetSCDialer2Ctrl(void)
{
	m_pS2CWinSocket->CloseSocket();
	m_pS2DWinSocket->CloseSocket();
	this->SetCtlStatus(_SC2_READY_STATUS, FALSE);
	m_nCtlError = _SC2_NO_ERROR;
}

/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::OnDraw - Drawing function

void CSCDialer2Ctrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::DoPropExchange - Persistence support

void CSCDialer2Ctrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl::OnResetState - Reset control to default state

void CSCDialer2Ctrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



void CSCDialer2Ctrl::SetCtlStatus(Status_SCDialer2 i_nStatus, BOOL i_bNotifyEvent/*=TRUE*/)
{
	this->m_nCtlStatus	= i_nStatus;
	this->m_nStatus		= this->m_nCtlStatus;
	
	if(i_bNotifyEvent)
	{
		FireEvStatus(this->m_nCtlStatus);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2Ctrl message handlers

void CSCDialer2Ctrl::OnS2DAsyncEventWinSocket(SOCKET i_hSocket, LPARAM i_lParam)
{
	if(m_pS2DWinSocket)
	{
		m_pS2DWinSocket->OnAsyncEvent(i_lParam);
	}
}

void CSCDialer2Ctrl::OnS2DPacketNotifyWinSocket(WPARAM i_wParam)
{
	if(NULL == m_pS2DWinSocket){				return;}


	BYTE		SendBuf[1024];
	USHORT		usNotifyType = LOWORD(i_wParam);
	USHORT		usNotifyParam = HIWORD(i_wParam);
	
	switch(usNotifyType)
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if(usNotifyParam)
			{
				this->SetCtlStatus(_SC2_CONNECTED_VDSERVER_STATUS);
			
				m_pS2DWinSocket->WriteMessageType(T_S2D_CONNECT);
			}
			else
			{
				m_nCtlError = _SC2_CONNECT_VDSERVER_ERROR;
				FireEvError(m_nCtlError, 0, 0, NULL);

				this->SetCtlStatus(_SC2_ERROR_STATUS);
			}
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			m_nCtlError = _SC2_CONNECT_VCSERVER_ERROR;
			FireEvError(m_nCtlError, 0, 0, NULL);
			
			this->SetCtlStatus(_SC2_ERROR_STATUS);
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{			
			char			*pPacket = NULL;
			int				nReadLen = 0;
			MessageType_t	nReadType = 0;

			while (TRUE)
			{
				m_pS2DWinSocket->Read(&pPacket, nReadLen);
				if(NULL == pPacket){						break;}

				nReadType = *(MessageType_t*)pPacket;				
				switch(nReadType)
				{
				case T_D2S_CONNECT_OK:
					{
						INIT_MSG(MSG_S2D_GET_VCSERVER_CONNECT_INFO, T_S2D_GET_VCSERVER_CONNECT_INFO, pSendMsg, SendBuf);
						pSendMsg->connectRoomID		= m_pAVManager->GetRoomID();
						pSendMsg->bCreateRoomFlag	= m_bCreateRoomFlag;
						m_pS2DWinSocket->Write(SendBuf, MSG_SIZE(MSG_S2D_GET_VCSERVER_CONNECT_INFO));
					}
					break;
				case T_D2S_GET_VCSERVER_CONNECT_INFO_OK:
					{
						////////////////////////////////////////////////////////////////////////////////
						// VDServer와는 연결을 종료한다.
						m_pS2DWinSocket->CloseSocket(0x1000);

						MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK *pMsg 
							= (MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						
						m_bCreateRoomFlag = pMsg->bCreateRoomFlag;		// 생성 여부가 다시 할당된다
						strncpy(m_szVCServerIP, Inet_n2a(pMsg->connectVCServerID.unionIP), SIZE_MAX_IPADDRESS);
						m_usVCServerPort = pMsg->connectVCServerID.usPort;
						
						if(FALSE == m_pS2CWinSocket->Connect(m_szVCServerIP, m_usVCServerPort))
						{
							m_nCtlError = _SC2_CONNECT_VCSERVER_ERROR;
							char szTemp[256];
							sprintf(szTemp, "OnS2DPacketNotifyWinSocket S2CWinSocketError SCServerIP(%16s) SCServerPort(%d)\n"
								, m_szVCServerIP, m_usVCServerPort);
							FireEvError(m_nCtlError, 0, 0, szTemp);
						}
					}
					SAFE_DELETE(pPacket);
					return;

				case T_ERROR:
					{
						MSG_ERROR *pRErr = (MSG_ERROR*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						this->OnSocketError(pRErr, m_pS2DWinSocket);
					}
					break;

				////////////////////////////////////////////////////////////////////////
				// Unknown Message	
				default:
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, "[Error] CSCDialer2Ctrl::OnS2DPacketNotifyWinSocket Unknown Message Type ==> %d (0x%08X)\r\n"
							, nReadType, nReadType);
						DbgOut(szSystemLog);
					}
				}

				SAFE_DELETE(pPacket);
			}
		}
		break;
	}
}

void CSCDialer2Ctrl::OnSocketError(MSG_ERROR *i_pError, CWinSocket *i_pSocket)
{

	int nCtlError = _SC2_NO_ERROR;
	char szErrString[256];
	memset(szErrString, 0x00, 256);

//	DbgOut("	CSCDialer2Ctrl::OnSocketError ==> ErrorType(0x%6X) MsgType(0x%6X), CloseFlag(%d) Param1(%4d) Param2(%4d) ErrString(%s)\r\n"
//		, i_pError->ErrorCode, i_pError->MsgType, i_pError->CloseConnection
//		, i_pError->ErrParam1, i_pError->ErrParam2, szErrString);

	switch(i_pError->ErrorCode)
	{
	case VOIPERR_NtoN_FAIL_REQUEST_TOKEN:
		{
			m_pAVManager->SetAudioSendPauseFlag(TRUE);		// 말하기 권한 요청 실패
			FireEvNotify(i_pError->ErrorCode, i_pError->ErrParam1, i_pError->ErrParam2, NULL);
		}
		return;
	case VOIPERR_NtoN_CANNOT_TALKING:		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 별다른 에러가 아님
		{

		}
		return;
	}
	m_nCtlError		= _SC2_VOIP_SERVER_ERROR;
	

	FireEvError(m_nCtlError, i_pError->ErrorCode, i_pError->ErrParam1, szErrString);
	if(i_pError->CloseConnection)
	{
		i_pSocket->CloseSocket(0x1001);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCDialer2Ctrl::OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
/// \brief		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
/// \author		cmkwon
/// \date		2008-10-22 ~ 2008-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCDialer2Ctrl::OnVolume(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
{
	FireEvVolume(i_bIsSpkDevice, i_byVolPercent);
}



void CSCDialer2Ctrl::OnS2CAsyncEventWinSocket(SOCKET i_hSocket, LPARAM i_lParam)
{
	if(m_pS2CWinSocket)
	{
		m_pS2CWinSocket->OnAsyncEvent(i_lParam);
	}
}

void CSCDialer2Ctrl::OnS2CPacketNotifyWinSocket(WPARAM i_wParam)
{
	if(NULL == m_pS2CWinSocket){				return;}

	BYTE		SendBuf[1024];
	USHORT		usNotifyType = LOWORD(i_wParam);
	USHORT		usNotifyParam = HIWORD(i_wParam);

	switch(usNotifyType)
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if(usNotifyParam)
			{
				this->SetCtlStatus(_SC2_CONNECTED_VCSERVER_STATUS);
			
				m_pS2CWinSocket->WriteMessageType(T_S2C_CONNECT);
			}
			else
			{
				m_nCtlError = _SC2_CONNECT_VCSERVER_ERROR;
				FireEvError(m_nCtlError, 0, 0, NULL);

				this->SetCtlStatus(_SC2_ERROR_STATUS);
			}
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			m_nCtlError = _SC2_CONNECT_VCSERVER_ERROR;
			FireEvError(m_nCtlError, 0, 0, NULL);
			
			this->SetCtlStatus(_SC2_ERROR_STATUS);
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			char			*pPacket = NULL;
			int				nReadLen = 0;
			MessageType_t	nReadType = 0;

			while (TRUE)
			{
				m_pS2CWinSocket->Read(&pPacket, nReadLen);
				if(NULL == pPacket){						break;}

				nReadType = *(MessageType_t*)pPacket;
				switch(nReadType)
				{
				case T_C2S_CONNECT_OK:
					{
						if(m_bCreateRoomFlag)
						{
							INIT_MSG(MSG_S2C_CREATE_ROOM, T_S2C_CREATE_ROOM, pSendMsg, SendBuf);
							pSendMsg->createRoomID				= m_pAVManager->GetRoomID();
							strncpy(pSendMsg->szUserName, m_pAVManager->GetUserName(), SIZE_MAX_USER_NAME);
							pSendMsg->nConcurrentTalkingCount	= m_pAVManager->GetConcurrentTalkingCount();
							m_pS2CWinSocket->Write(SendBuf, MSG_SIZE(MSG_S2C_CREATE_ROOM));
						}
						else
						{
							INIT_MSG(MSG_S2C_JOIN_ROOM, T_S2C_JOIN_ROOM, pSendMsg, SendBuf);
							pSendMsg->joinRoomID	= m_pAVManager->GetRoomID();
							strncpy(pSendMsg->szUserName, m_pAVManager->GetUserName(), SIZE_MAX_USER_NAME);
							m_pS2CWinSocket->Write(SendBuf, MSG_SIZE(MSG_S2C_JOIN_ROOM));
						}						
					}
					break;
				case T_C2S_CREATE_ROOM_OK:
					{
						MSG_C2S_CREATE_ROOM_OK *pMsg = (MSG_C2S_CREATE_ROOM_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						DBGOUT("	Recv CreateRoomOK SelfChannelIndex[%d]\r\n", pMsg->nChannelIndex);
						
						m_pAVManager->SetSelfChannelIndex(pMsg->nChannelIndex);
						this->SetCtlStatus(_SC2_CREATED_ROOM_STATUS);

						m_pAVManager->AudioStart();

					}
					break;
				case T_C2S_JOIN_ROOM_OK:
					{
						MSG_C2S_JOIN_ROOM_OK *pMsg = (MSG_C2S_JOIN_ROOM_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						DBGOUT("	Recv JoinRoomOK SelfChannelIndex[%d]\r\n", pMsg->nChannelIndex);
						
						m_pAVManager->SetSelfChannelIndex(pMsg->nChannelIndex);
						this->SetCtlStatus(_SC2_JOINED_ROOM_STATUS);

						m_pAVManager->AudioStart();
					}
					break;
				case T_C2S_JOINED_USERLIST:
					{						
						MSG_C2S_JOINED_USERLIST *pMsg	= (MSG_C2S_JOINED_USERLIST*)(pPacket+SIZE_FIELD_TYPE_HEADER);
//						DbgOut("081022	Recv JoinedUserList UserCounts(%d)\r\n", pMsg->byUserCounts);

						JoinedUserInfo *pUserInfo		= (JoinedUserInfo*)(pPacket+MSG_SIZE(MSG_C2S_JOINED_USERLIST));
						for(int i=0; i < pMsg->byUserCounts; i++)
						{
//							DbgOut("081022		UserInf UserName[%10s] ChannelIndex[%d]\r\n", pUserInfo[i].szUserName, pUserInfo[i].nChannelIndex);
							m_pAVManager->OtherJoinedRoom(pUserInfo[i].szUserName, pUserInfo[i].nChannelIndex);
							// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
							//FireEvOtherJoinedRoom(pUserInfo[i].szUserName);
							FireEvUserJoinedRoom(pUserInfo[i].szUserName, pUserInfo[i].bIsTalking);	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
						}
					}
					break;
				case T_C2S_JOIN_USER:
					{
						MSG_C2S_JOIN_USER *pMsg = (MSG_C2S_JOIN_USER*)(pPacket+SIZE_FIELD_TYPE_HEADER);						
//						DbgOut("081022	Recv JoineUser UserName[%10s] ChannelIndex[%d]\r\n", pMsg->szUserName, pMsg->nChannelIndex);

						m_pAVManager->OtherJoinedRoom(pMsg->szUserName, pMsg->nChannelIndex);
						// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
						//FireEvOtherJoinedRoom(pMsg->szUserName);
						FireEvUserJoinedRoom(pMsg->szUserName, pMsg->bIsTalking);	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
					}
					break;
				case T_C2S_GOOUT_USER:
					{
						MSG_C2S_GOOUT_USER *pMsg = (MSG_C2S_GOOUT_USER*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						DBGOUT("	Recv GoOutUser UserName[%10s] ChannelIndex[%d]\r\n", pMsg->szUserName, pMsg->nChannelIndex);

						m_pAVManager->OtherGoOutedRoom(pMsg->nChannelIndex);
						FireEvOtherGoOutedRoom(pMsg->szUserName);
					}
					break;
				case T_C2S_REQUEST_TOKEN_OK:
					{
						MSG_C2S_REQUEST_TOKEN_OK *pMsg = (MSG_C2S_REQUEST_TOKEN_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						if(m_pAVManager->GetSelfChannelIndex() == pMsg->nChannelIndex)
						{// 자신의 정보

							m_pAVManager->SetAudioSendPauseFlag(!pMsg->byIsTalk);
						}
						FireEvTalk(pMsg->szUserName, pMsg->byIsTalk);	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 자신의 정보도 전달한다.
					}
					break;
				case T_C2S_CHANGE_TALKING_STATE_OK:		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
					{
						MSG_C2S_CHANGE_TALKING_STATE_OK *pMsg = (MSG_C2S_CHANGE_TALKING_STATE_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						if(m_pAVManager->GetSelfChannelIndex() != pMsg->nChannelIndex)
						{// 다른사람의 말하기 시작/종료 변경 정보를 이벤트로 발생시킨다.
							FireEvChangedTalkingState(pMsg->szUserName, pMsg->talkingState);
						}
					}
					break;
				case T_C2S_LOST_TOKEN:
					{
						MSG_C2S_LOST_TOKEN *pMsg = (MSG_C2S_LOST_TOKEN*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						if(m_pAVManager->GetSelfChannelIndex() == pMsg->nChannelIndex)
						{// 자신의 정보

							m_pAVManager->SetAudioSendPauseFlag(TRUE);
							FireEvNotify(VOIPERR_NtoN_LOST_TOKEN, 0, 0, NULL);
						}
						FireEvTalk(pMsg->szUserName, FALSE);	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 자신의 정보도 전달한다.
					}
					break;
				case T_C2S_RTP_PACKET:
					{						
						MSG_C2S_RTP_PACKET *pMsg = (MSG_C2S_RTP_PACKET*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						m_pAVManager->RecvRTPData(pMsg->dwChannelIndex
							, pPacket+MSG_SIZE(MSG_C2S_RTP_PACKET), pMsg->usRTPLength, pMsg->byMediaType);
					}
					break;
				
				case T_ERROR:
					{
						MSG_ERROR *pRErr = (MSG_ERROR*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						this->OnSocketError(pRErr, m_pS2CWinSocket);
					}
					break;


				////////////////////////////////////////////////////////////////////////
				// Unknown Message	
				default:
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, "[Error] CSCDialer2Ctrl::OnS2CPacketNotifyWinSocket Unknown Message Type ==> %d (0x%08X)\r\n"
							, nReadType, nReadType);
						DbgOut(szSystemLog);
					}
				}

				SAFE_DELETE(pPacket);
			}
		}
		break;
	}
}

//void CSCDialer2Ctrl::OnS2CError(MSG_ERROR *i_pError)
//{	
//	BOOL bFireEvErrorFlag = TRUE;
//	char szErrString[256];
//	memset(szErrString, 0x00, 256);
////	switch(i_pError->ErrorCode)
////	{
////	case :
////		break;
////	case :
////		break;
////	default:
////		{
////		}
////	}
//	
//	DBGOUT("	CSCDialer2Ctrl::OnS2CError ==> ErrorType(0x%6X) MsgType(0x%6X), CloseFlag(%d) Param1(%4d) Param2(%4d) ErrString(%s)\r\n"
//		, i_pError->ErrorCode, i_pError->MsgType, i_pError->CloseConnection
//		, i_pError->ErrParam1, i_pError->ErrParam2, szErrString);
//
//	if(bFireEvErrorFlag)
//	{
//		FireEvError(i_pError->ErrorCode, i_pError->ErrParam1, i_pError->ErrParam2, szErrString);
//	}
//	
//	if(i_pError->CloseConnection)
//	{
//		m_pS2CWinSocket->CloseSocket(0x1002);
//	}
//}





void CSCDialer2Ctrl::OnStrVDServerIPChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

void CSCDialer2Ctrl::OnUsVDServerPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}
//long CSCDialer2Ctrl::CreateRoom(LPCTSTR szUserName, long FAR* pn64RoomID) 
//{
//	// TODO: Add your dispatch handler code here	
//	if(FALSE == strncmp(szUserName, "", SIZE_MAX_USER_NAME)
//		|| 0 == *(RoomID_t*)pn64RoomID)
//	{
//		m_nCtlError = _SC2_INVALID_PARAM_ERROR;
//		char szTemp[256];
//		sprintf(szTemp, "CreateRoom ParamError UserName(%10s) RoomID(%I64)\n"
//			, szUserName, *(RoomID_t*)pn64RoomID);
//		FireEvError(m_nCtlError, 0, 0, szTemp);
//		return 0;
//	}
//	else if (_SC2_READY_STATUS != m_nCtlStatus)
//	{
//		m_nCtlError = _SC2_INVALID_STATUS_ERROR;
//		char szTemp[256];
//		sprintf(szTemp, "CreateRoom StatusError UserName(%10s) RoomID(%I64)\n"
//			, szUserName, *(RoomID_t*)pn64RoomID);
//		FireEvError(m_nCtlError, 0, 0, szTemp);
//		return 0;
//	}
//	else if(NULL == m_pS2DWinSocket
//		|| FALSE == m_pS2DWinSocket->Connect(m_strVDServerIP, m_usVDServerPort))
//	{
//		m_nCtlError = _SC2_CONNECT_VDSERVER_ERROR;
//		char szTemp[256];
//		sprintf(szTemp, "CreateRoom S2DWinSocketError UserName(%10s) RoomID(%I64)\n"
//			, szUserName, *(RoomID_t*)pn64RoomID);
//		FireEvError(m_nCtlError, 0, 0, szTemp);
//		return 0;
//	}
//
//	m_bCreateRoomFlag	= TRUE;
//	this->SetCtlStatus(_SC2_CONNECTING_VDSERVER_STATUS, FALSE);
//	m_pAVManager->SetUserName((char*)szUserName);
//	m_pAVManager->SetRoomID(*(RoomID_t*)pn64RoomID);
//
//	return TRUE;
//}
//
long CSCDialer2Ctrl::CreateRoom(LPCTSTR szUserName, long FAR* pn64RoomID) 
{
	// TODO: Add your dispatch handler code here	
	return this->CreateRoomEx(szUserName, pn64RoomID, TRUE);
}

long CSCDialer2Ctrl::CreateRoomEx(LPCTSTR i_szUserName, long FAR* i_pn64RoomID, long i_nConcurrentTalkingCount) 
{
	// TODO: Add your dispatch handler code here
	if(FALSE == strncmp(i_szUserName, "", SIZE_MAX_USER_NAME)
		|| 0 == *(RoomID_t*)i_pn64RoomID)
	{
		m_nCtlError = _SC2_INVALID_PARAM_ERROR;
		char szTemp[256];
		sprintf(szTemp, "CreateRoom ParamError UserName(%10s) RoomID(%I64)\n"
			, i_szUserName, *(RoomID_t*)i_pn64RoomID);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}
	else if (_SC2_READY_STATUS != m_nCtlStatus)
	{
		m_nCtlError = _SC2_INVALID_STATUS_ERROR;
		char szTemp[256];
		sprintf(szTemp, "CreateRoom StatusError UserName(%10s) RoomID(%I64)\n"
			, i_szUserName, *(RoomID_t*)i_pn64RoomID);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}
	else if(NULL == m_pS2DWinSocket
		|| FALSE == m_pS2DWinSocket->Connect(m_strVDServerIP, m_usVDServerPort))
	{
		m_nCtlError = _SC2_CONNECT_VDSERVER_ERROR;
		char szTemp[256];
		sprintf(szTemp, "CreateRoom S2DWinSocketError UserName(%10s) RoomID(%I64)\n"
			, i_szUserName, *(RoomID_t*)i_pn64RoomID);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}

	m_bCreateRoomFlag	= TRUE;
	this->SetCtlStatus(_SC2_CONNECTING_VDSERVER_STATUS, FALSE);
	m_pAVManager->SetUserName((char*)i_szUserName);
	m_pAVManager->SetConcurrentTalkingCount(i_nConcurrentTalkingCount);
	m_pAVManager->SetRoomID(*(RoomID_t*)i_pn64RoomID);
	return TRUE;
}

long CSCDialer2Ctrl::JoinRoom(LPCTSTR szUserName, long FAR* pn64RoomID) 
{
	// TODO: Add your dispatch handler code here
	if(FALSE == strncmp(szUserName, "", SIZE_MAX_USER_NAME)
		|| 0 == *(RoomID_t*)pn64RoomID)
	{
		m_nCtlError = _SC2_INVALID_PARAM_ERROR;
		char szTemp[256];
		sprintf(szTemp, "JoinRoom ParamError UserName(%10s) RoomID(%I64)\n"
			, szUserName, *(RoomID_t*)pn64RoomID);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}
	else if (_SC2_READY_STATUS != m_nCtlStatus)
	{
		m_nCtlError = _SC2_INVALID_STATUS_ERROR;
		char szTemp[256];
		sprintf(szTemp, "JoinRoom StatusError UserName(%10s) RoomID(%I64)\n"
			, szUserName, *(RoomID_t*)pn64RoomID);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}
	else if(FALSE == m_pS2DWinSocket->Connect(m_strVDServerIP, m_usVDServerPort))
	{
		m_nCtlError = _SC2_CONNECT_VDSERVER_ERROR;
		char szTemp[256];
		sprintf(szTemp, "JoinRoom S2DWinSocketError VDServerIP(%16s) VDServerPort(%d)\n"
			, m_strVDServerIP, m_usVDServerPort);
		FireEvError(m_nCtlError, 0, 0, szTemp);
		return 0;
	}

	m_bCreateRoomFlag	= FALSE;
	this->SetCtlStatus(_SC2_CONNECTING_VDSERVER_STATUS, FALSE);
	m_pAVManager->SetUserName((char*)szUserName);
	m_pAVManager->SetRoomID(*(RoomID_t*)pn64RoomID);
	return TRUE;
}

void CSCDialer2Ctrl::GoOutRoom() 
{
	// TODO: Add your dispatch handler code here
	///////////////////////////////////////////////////////////////////////////////
	// Audio Recoding, Playing을 멈춘다
	m_pAVManager->AudioStop();
	Sleep(200);

	if(_SC2_CREATED_ROOM_STATUS <= m_nCtlStatus
		&& m_pS2CWinSocket->IsConnected())
	{
		///////////////////////////////////////////////////////////////////////////////
		// VCServer로 전송한다
		m_pS2CWinSocket->WriteMessageType(T_S2C_GOOUT_ROOM);
		Sleep(200);
	}

	this->ResetSCDialer2Ctrl();
}

void CSCDialer2Ctrl::OnStatusChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}

int CSCDialer2Ctrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	ASSERT_ASSERT(m_pS2DWinSocket == NULL);
	ASSERT_ASSERT(m_pS2CWinSocket == NULL);
	ASSERT_ASSERT(m_pAVManager == NULL);
	
	m_pS2DWinSocket = new CS2DWinSocket(this->GetSafeHwnd());
	m_pS2CWinSocket = new CS2CWinSocket(this->GetSafeHwnd());
	m_pAVManager = new CAVManager(this, m_pS2CWinSocket, SIZE_MAX_CHANNEL_COUNT, t_G7231Low, 4, 16);

	SetTimer(TIMERID_S2C_SEND_ALIVEPACKET, TIMERGAP_S2C_SEND_ALIVEPACKET, NULL);
	return 0;
}



void CSCDialer2Ctrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMERID_S2C_SEND_ALIVEPACKET:
		{
			if(_SC2_CONNECTED_VCSERVER_STATUS <= m_nCtlStatus
				&& m_pS2CWinSocket->IsConnected())
			{
				m_pS2CWinSocket->WriteMessageType(T_S2C_SOCKET_ALIVE);
			}
		}
		break;
	case TIMERID_RESET_CONTROL:
		{
			KillTimer(TIMERID_RESET_CONTROL);
			
			this->ResetSCDialer2Ctrl();
			FireEvStatus(m_nCtlStatus);
			FireEvError(m_nCtlError, 0, 0, NULL);
		}
		break;
	}

	COleControl::OnTimer(nIDEvent);
}

BOOL CSCDialer2Ctrl::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	KillTimer(TIMERID_S2C_SEND_ALIVEPACKET);

	if(m_pAVManager)
	{
		m_pAVManager->AudioEnd();
	}
	if(m_pS2DWinSocket
		&& m_pS2DWinSocket->IsConnected())
	{
		m_pS2DWinSocket->CloseSocket();
	}

	if(m_pS2CWinSocket
		&& m_pS2CWinSocket->IsConnected())
	{
		m_pS2CWinSocket->CloseSocket();
	}
	
	SAFE_DELETE(m_pAVManager);
	SAFE_DELETE(m_pS2DWinSocket);
	SAFE_DELETE(m_pS2CWinSocket);	
	return COleControl::DestroyWindow();
}

void CSCDialer2Ctrl::OnAudioSendPauseChanged() 
{
	// TODO: Add notification handler code

	if(m_pAVManager)
	{
		m_pAVManager->SetAndSendAudioSendPauseFlag(m_audioSendPause);
	}
	SetModifiedFlag();
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
long CSCDialer2Ctrl::SetSpkVolume(long i_nVolumeValueOfPercent) 
{
	// TODO: Add your dispatch handler code here

	
	BOOL bRet = FALSE;
	if(NULL == m_pAVManager)
	{
		return FALSE;
	}
	bRet	= m_pAVManager->SetPlayVolumeW(i_nVolumeValueOfPercent);	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
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
long CSCDialer2Ctrl::GetSpkVolume() 
{
	// TODO: Add your dispatch handler code here

	BYTE byVolOfPercent = 0;
	if(NULL == m_pAVManager)
	{
		return byVolOfPercent;
	}
	
	m_pAVManager->GetPlayVolumeW(&byVolOfPercent);		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	return byVolOfPercent;
}
