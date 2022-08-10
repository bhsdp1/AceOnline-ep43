// VDIOCPSocket.cpp: implementation of the CVDIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VDIOCPSocket.h"
#include "VDGlobal.h"
#include "VoIPProtocol.h"
#include "VDIOCP.h"


CVDIOCP		*CVDIOCPSocket::ms_pVDIOCP = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVDIOCPSocket::CVDIOCPSocket()
{
	this->ResetVDIOCPSocket();		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
}

CVDIOCPSocket::~CVDIOCPSocket()
{

}

void CVDIOCPSocket::ResetVDIOCPSocket(void)
{
	m_VoIPSockTy		= VOIP_SOCK_TYPE_UNKNOWN;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	m_VoIPState			= VOIP_STATE_UNKNOWN;			// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	m_VCServerInfo.ResetVCServerInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CVDIOCPSocket::IsValidVCServer(BOOL i_bCheckRegistered/*=TRUE*/)
/// \brief		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
/// \author		cmkwon
/// \date		2008-09-30 ~ 2008-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CVDIOCPSocket::IsValidVCServer(BOOL i_bCheckRegistered/*=TRUE*/)
{
	if(VOIP_SOCK_TYPE_VCServer != this->GetVoIPSockType())
	{
		return FALSE;
	}

	if(i_bCheckRegistered
		&& VOIP_STATE_VD_VCServer_Registered != this->GetVoIPState())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CVDIOCPSocket::OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP/*=""*/, int nPeerPort/*=0*/, SThreadInfo *i_pThreadInfo/*=NULL*/)
{
	int						nBytesUsed		= 0;
	MessageType_t			nRecvType		= 0;
	int						nRecvTypeSize	= 0;
	ProcessResult			procRes			= RES_RETURN_FALSE;
	int						tmpSeq;
	MessageType_t			prevMsgType		= 0;

///////////////////////////////////////////////////////////////////////////////
// UDP 처리 루틴
	if(m_bUDPFlag == TRUE)
	{
		return FALSE;
	}
// UDP 처리 루틴 End
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// TCP 처리 루틴
	if(m_bPeerSequenceNumberInitFlag == FALSE)
	{
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
		m_bPeerSequenceNumberInitFlag = TRUE;
	}
	else
	{
		if(m_byPeerSequenceNumber != nSeq)
		{
			// Protocl Error 처리
			// - 받은 패킷의 Sequence Number가 유효하지 않음
			// Error Code : ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER
			SendErrorMessage(0, 0);
			Close(0x15002);
			return FALSE;
		}
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
	}

	while(this->IsUsing() && nBytesUsed < nLength)
	{
		procRes		= RES_RETURN_FALSE;
		nRecvType	= *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed	+= SIZE_FIELD_TYPE_HEADER;

		switch(nRecvType)
		{
		////////////////////////////////////////////////////////////////////////
		// from VCServer
		case T_C2D_CONNECT:
// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
// 			{
// 				m_VoIPSockTy	= VOIP_SOCK_TYPE_VCServer;				
// 				m_VoIPState		= VOIP_STATE_VD_VCServer_Connected;
// 				this->SendAddMessageType(T_D2C_CONNECT_OK);
// 				procRes = RES_RETURN_TRUE;
// 			}
			procRes = _Process_C2D_CONNECT(pPacket, nLength, nBytesUsed);
			break;
		case T_C2D_SOCKET_ALIVE:
			{
				this->OnRecvdAlivePacket();
				procRes = RES_RETURN_TRUE;
			}
			break;
		case T_C2D_REGISTER_SELF:
			procRes = _Process_C2D_REGISTER_SELF(pPacket, nLength, nBytesUsed);
			break;
		case T_C2D_VCSERVER_STATUS_INFO:
			procRes = _Process_C2D_VCSERVER_STATUS_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_C2D_CREATE_ROOM_NOTIFY:
			procRes = _Process_C2D_CREATE_ROOM_NOTIFY(pPacket, nLength, nBytesUsed);
			break;
		case T_C2D_DELETE_ROOM_NOTIFY:
			procRes = _Process_C2D_DELETE_ROOM_NOTIFY(pPacket, nLength, nBytesUsed);
			break;

		////////////////////////////////////////////////////////////////////////
		// from SCVoIP
		case T_S2D_CONNECT:
// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
// 			{
// 				m_VoIPSockTy	= VOIP_SOCK_TYPE_SCVoIP;
// 				m_VoIPState		= VOIP_STATE_VD_VCServer_Connected;
// 				this->SendAddMessageType(T_D2S_CONNECT_OK);
// 				procRes = RES_RETURN_TRUE;
// 			}
			procRes = _Process_S2D_CONNECT(pPacket, nLength, nBytesUsed);
			break;
		case T_S2D_GET_VCSERVER_CONNECT_INFO:
			procRes = _Process_S2D_GET_VCSERVER_CONNECT_INFO(pPacket, nLength, nBytesUsed);
			break;

		////////////////////////////////////////////////////////////////////////
		// Unknown Message
		default:
			{
				// Protocol Error 처리(Close 처리함)
				// - Client로 부터 받은 Field Type이 유효하지 않다
				// Error Code : ERR_PROTOCOL_INVALID_PROTOCOL_TYPE
				SendErrorMessage(0, 0, 0, 0, NULL, TRUE);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CVDIOCPSocket::OnRecvdPacketNormalServer invalid protocol type, RecvType[%#04X]\r\n", nRecvType);
				g_pVoIPGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				procRes = RES_PACKET_ERROR;
			}
		}	// switch_end

		///////////////////////////////////////////////////////////////////////////////
		// MSG 결과 처리
		switch(procRes)
		{
		case RES_RETURN_FALSE:	// 심각한 오류 이므로 FALSE를 리턴하여 Socket을 종료한다
		case RES_PACKET_ERROR:	// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
			return FALSE;
		}
	}// while_end

	return TRUE;
}

void CVDIOCPSocket::OnConnect(void)
{
	// TODO: Place code here.
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pVoIPGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	CIOCPSocket::OnConnect();
}

void CVDIOCPSocket::OnClose(int reason)
{
	// TODO: Place code here.
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket  Closed SocketIndex[%3d] PeerIP[%15s] Port[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort, reason, reason);
	g_pVoIPGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	switch(this->GetVoIPSockType())
	{
	case VOIP_SOCK_TYPE_VCServer:
		{
			ms_pVDIOCP->DeleteVCServerList(this);
		}
		break;
	}


	this->ResetVDIOCPSocket();	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	CIOCPSocket::OnClose(30);
}

BOOL CVDIOCPSocket::OnError(int errCode)
{
	switch(errCode)
	{
	case ERROR_PORT_UNREACHABLE:
		{	// UDP 전송일때 상대방의 Port가 열려있지 않을때 발생하는 에러
			// Monster 정보를 모두 초기화, Read를 다시 한번 호출

			MessageBox(NULL, "UDP Error", "Error", MB_OK);
		}
		return TRUE;
	}
	return FALSE;
}


void CVDIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err
									 , int errParam1/*=0*/, int errParam2/*=0*/
									 , char* errMsg/*=NULL*/, BOOL bCloseConnection/*=FALSE*/)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "  SendErrorMsg to IP(%15s) ==> ErrType(0x%6X) MsgType(0x%6X) Param1(%4d) Param2(%4d) Msg(%s)\r\n"
		, m_szPeerIP, err, msgType
		, errParam1, errParam2, errMsg);
	g_pVDGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	BYTE SendBuf[1024];
	INIT_MSG(MSG_ERROR, T_ERROR, pSError, SendBuf);
	pSError->MsgType			= msgType;
	pSError->ErrorCode			= err;	
	pSError->ErrParam1			= errParam1;
	pSError->ErrParam2			= errParam2;
	pSError->CloseConnection	= bCloseConnection;
	this->SendAddData(SendBuf, MSG_SIZE(MSG_ERROR));
}





ProcessResult CVDIOCPSocket::_Process_C2D_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{
	// no body

	if(VOIP_SOCK_TYPE_UNKNOWN != this->GetVoIPSockType()
		|| VOIP_STATE_UNKNOWN != this->GetVoIPState())
	{
		g_pVoIPGlobal->WriteSystemLogEX(TRUE, "[Error] VCServer connect fail !!, VCServerIP(%15s), SockType(%d) State(%d)\r\n", this->GetPeerIP(), this->GetVoIPSockType(), this->GetVoIPState());
		return RES_BREAK;			
	}
	
	m_VoIPSockTy	= VOIP_SOCK_TYPE_VCServer;				
	m_VoIPState		= VOIP_STATE_VD_VCServer_Connected;
	this->SendAddMessageType(T_D2C_CONNECT_OK);
	return RES_RETURN_TRUE;
}

ProcessResult CVDIOCPSocket::_Process_C2D_REGISTER_SELF(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_C2D_REGISTER_SELF	*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_C2D_REGISTER_SELF);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_C2D_REGISTER_SELF, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x15000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_C2D_REGISTER_SELF*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	if(FALSE == this->IsValidVCServer(FALSE))
	{
		g_pVoIPGlobal->WriteSystemLogEX(TRUE, "[Error] VCServer fail to register !!, VCServerIP(%15s), SockType(%d) State(%d)\r\n", this->GetPeerIP(), this->GetVoIPSockType(), this->GetVoIPState());
		return RES_BREAK;
	}

	m_VCServerInfo.m_idServerID			= pRecvMsg->vcServerID;
	m_VCServerInfo.m_dwMaxRoomCounts	= pRecvMsg->dwMaxRoomCounts;
	m_VCServerInfo.m_dwMaxUserCounts	= pRecvMsg->dwMaxUserCounts;
	m_VCServerInfo.m_dwMaxBandwidth		= max(1*1024*1024*8, pRecvMsg->dwMaxBandwidth);	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 최소 1 MBytes

	if(TRUE == ms_pVDIOCP->IsSearchVCServer(this))
	{
		g_pVoIPGlobal->WriteSystemLogEX(TRUE, "[Error] VCServer fail to register !!, VCServerIP(%15s)\r\n", this->GetPeerIP());
		return RES_BREAK;
	}

	char szLog[512];
	sprintf(szLog, "	VCServer registered IP(%15s) Port(%4d) MaxRoomCounts(%4d) MaxUserCounts(%4d) MaxBandwidth(%6d)\r\n"
		, this->GetPeerIP(), m_VCServerInfo.m_idServerID.usPort, m_VCServerInfo.m_dwMaxRoomCounts
		, m_VCServerInfo.m_dwMaxUserCounts, m_VCServerInfo.m_dwMaxBandwidth);
	g_pVDGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);

	ms_pVDIOCP->AddVCServerList(this);	
	
	m_VoIPState		= VOIP_STATE_VD_VCServer_Registered;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	this->SendAddMessageType(T_D2C_REGISTER_SELF_OK);

	return RES_RETURN_TRUE;
}

ProcessResult CVDIOCPSocket::_Process_C2D_VCSERVER_STATUS_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_C2D_VCSERVER_STATUS_INFO	*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_C2D_VCSERVER_STATUS_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_C2D_VCSERVER_STATUS_INFO, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x15000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_C2D_VCSERVER_STATUS_INFO*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(FALSE == this->IsValidVCServer())
	{// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		return RES_BREAK;
	}

	m_VCServerInfo.m_dwCurRoomCounts	= pRecvMsg->dwCurRoomCounts;
	m_VCServerInfo.m_dwCurUserCounts	= pRecvMsg->dwCurUserCounts;
	m_VCServerInfo.m_dwCurBandwidth		= pRecvMsg->dwCurBandwidth;	
	m_VCServerInfo.m_dwBandwidthPercent = (m_VCServerInfo.m_dwCurBandwidth*100)/m_VCServerInfo.m_dwMaxBandwidth;

	return RES_RETURN_TRUE;
}

ProcessResult CVDIOCPSocket::_Process_C2D_CREATE_ROOM_NOTIFY(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_C2D_CREATE_ROOM_NOTIFY	*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_C2D_CREATE_ROOM_NOTIFY);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_C2D_CREATE_ROOM_NOTIFY, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x15000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_C2D_CREATE_ROOM_NOTIFY*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(FALSE == this->IsValidVCServer())
	{// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		SendErrorMessage(T_C2D_CREATE_ROOM_NOTIFY, VOIPERR_NtoN_NO_SUCH_WELLSUITED_VCSERVER, this->GetVoIPSockType(), this->GetVoIPState());
		return RES_BREAK;
	}

	if(ms_pVDIOCP->FindVCServer(pRecvMsg->CreateRoomID))
	{// SendError, Already Exist RoomID
		
		SendErrorMessage(T_C2D_CREATE_ROOM_NOTIFY, VOIPERR_NtoN_ALREADY_EXISTS_ROOMID);
		return RES_BREAK;
	}

	char szLog[512];
	sprintf(szLog, "	CreateRoom VCServer(%15s:%4d) RoomID(%I64d) UserName(%s)\r\n"
		, this->GetPeerIP(), m_VCServerInfo.m_idServerID.usPort
		, pRecvMsg->CreateRoomID, pRecvMsg->szUserName);
	g_pVDGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);

	m_VCServerInfo.m_mtvectorRoomIDList.lock();
	m_VCServerInfo.PutRoomID(pRecvMsg->CreateRoomID);
	m_VCServerInfo.m_mtvectorRoomIDList.unlock();

	BYTE SendBuf[128];
	INIT_MSG(MSG_D2C_CREATE_ROOM_NOTIFY_OK, T_D2C_CREATE_ROOM_NOTIFY_OK, pSendMsg, SendBuf);
	pSendMsg->ClientIndex				= pRecvMsg->ClientIndex;
	pSendMsg->CreateRoomID				= pRecvMsg->CreateRoomID;
	pSendMsg->nConcurrentTalkingCount	= pRecvMsg->nConcurrentTalkingCount;
	strncpy(pSendMsg->szUserName, pRecvMsg->szUserName, SIZE_MAX_USER_NAME);
	
	this->SendAddData(SendBuf, MSG_SIZE(MSG_D2C_CREATE_ROOM_NOTIFY_OK));
	return RES_RETURN_TRUE;
}

ProcessResult CVDIOCPSocket::_Process_C2D_DELETE_ROOM_NOTIFY(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_C2D_DELETE_ROOM_NOTIFY	*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_C2D_DELETE_ROOM_NOTIFY);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_C2D_DELETE_ROOM_NOTIFY, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x15000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_C2D_DELETE_ROOM_NOTIFY*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(FALSE == this->IsValidVCServer())
	{// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		SendErrorMessage(T_C2D_DELETE_ROOM_NOTIFY, VOIPERR_NtoN_NO_SUCH_WELLSUITED_VCSERVER, this->GetVoIPSockType(), this->GetVoIPState());
		return RES_BREAK;
	}

	char szLog[512];
	sprintf(szLog, "	DeleteRoom VCServer(%15s:%4d) RoomID(%I64d)\r\n"
		, this->GetPeerIP(), m_VCServerInfo.m_idServerID.usPort
		, pRecvMsg->DeleteRoomID);
	g_pVDGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);

	m_VCServerInfo.PopRoomID(pRecvMsg->DeleteRoomID);	
	return RES_RETURN_TRUE;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CVDIOCPSocket::_Process_S2D_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
/// \author		cmkwon
/// \date		2008-09-30 ~ 2008-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CVDIOCPSocket::_Process_S2D_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{
	// no body

	if(VOIP_SOCK_TYPE_UNKNOWN != this->GetVoIPSockType()
		|| VOIP_STATE_UNKNOWN != this->GetVoIPState())
	{
		g_pVoIPGlobal->WriteSystemLogEX(TRUE, "[Error] SCVoIP connect fail !!, SCVoIP(%15s), SockType(%d) State(%d)\r\n", this->GetPeerIP(), this->GetVoIPSockType(), this->GetVoIPState());
		return RES_BREAK;			
	}

	m_VoIPSockTy	= VOIP_SOCK_TYPE_SCVoIP;
	m_VoIPState		= VOIP_STATE_VD_VCServer_Connected;

	this->SendAddMessageType(T_D2S_CONNECT_OK);
	
	return RES_RETURN_TRUE;
}

ProcessResult CVDIOCPSocket::_Process_S2D_GET_VCSERVER_CONNECT_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2D_GET_VCSERVER_CONNECT_INFO	*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_S2D_GET_VCSERVER_CONNECT_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2D_GET_VCSERVER_CONNECT_INFO, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x15000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_S2D_GET_VCSERVER_CONNECT_INFO*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	BOOL bCreatRoomFlag = pRecvMsg->bCreateRoomFlag;
	CVDIOCPSocket *pVCServer = ms_pVDIOCP->FindVCServer(pRecvMsg->connectRoomID);
	if(pRecvMsg->bCreateRoomFlag)
	{// Room을 등록한다

		if(pVCServer)
		{// 이미 Room이 존재 하므로 CreatRoomFlag를 FALSE로 설정한다

			bCreatRoomFlag = FALSE;
		}
		else
		{
			bCreatRoomFlag = TRUE;
			pVCServer = ms_pVDIOCP->FindWellSuitedVCServer(pRecvMsg->connectRoomID);
		}
	}
	
	if(NULL == pVCServer
		|| FALSE == pVCServer->IsValidVCServer())	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	{// Error 전송(등록된 VCServer가 없거나 적당한 VCServer가 없다
		
		SendErrorMessage(T_S2D_GET_VCSERVER_CONNECT_INFO, VOIPERR_NtoN_NO_SUCH_WELLSUITED_VCSERVER, ms_pVDIOCP->m_mtvectorVCServerList.size());
		return RES_BREAK;
	}	

	BYTE SendBuf[256];
	INIT_MSG(MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK, T_D2S_GET_VCSERVER_CONNECT_INFO_OK, pSendMsg, SendBuf);
	pSendMsg->connectVCServerID = pVCServer->m_VCServerInfo.m_idServerID;
	pSendMsg->bCreateRoomFlag	= bCreatRoomFlag;

	this->SendAddData(SendBuf, MSG_SIZE(MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK));
	return RES_RETURN_TRUE;
}