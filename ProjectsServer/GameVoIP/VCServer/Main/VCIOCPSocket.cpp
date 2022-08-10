// VCIOCPSocket.cpp: implementation of the CVCIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCIOCPSocket.h"
#include "VCGlobal.h"
#include "VoIPProtocol.h"
#include "Room.h"
#include "RoomManager.h"
#include "VCIOCP.h"


CVCIOCP *CVCIOCPSocket::ms_pVCIOCP = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCIOCPSocket::CVCIOCPSocket()
{
// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
// 	m_pJoinedRoom		= NULL;
// 	m_dwChannelIndex	= UINT_MAX;
// 	memset(m_szUserName, 0x00, SIZE_MAX_USER_NAME);
// 
// 	m_bTalking				= FALSE;
// 	m_dwLastTickRecvdAudio	= 0;

	this->ResetVCIOCPSocket();		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
}

CVCIOCPSocket::~CVCIOCPSocket()
{

}

void CVCIOCPSocket::SetJoinedRoom(CRoom *pRoom, DWORD i_ChannIdx)
{
	m_pJoinedRoom		= pRoom;
	m_dwChannelIndex	= i_ChannIdx;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CVCIOCPSocket::GetRoomChannelIndex(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CVCIOCPSocket::GetRoomChannelIndex(void)
{
	return m_dwChannelIndex;
}

void CVCIOCPSocket::ResetVCIOCPSocket(void)
{
	if(m_pJoinedRoom)
	{		
//		m_pJoinedRoom->DeleteMemberFromRoom(this);
	}

	m_pJoinedRoom		= NULL;
	m_dwChannelIndex	= UINT_MAX;
	memset(m_szUserName, 0x00, SIZE_MAX_USER_NAME);

	m_bTalking				= FALSE;
	m_dwLastTickRecvdAudio	= 0;

	m_VoIPSockTy		= VOIP_SOCK_TYPE_UNKNOWN;
	m_VoIPState			= VOIP_STATE_UNKNOWN;
}

BOOL CVCIOCPSocket::OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
//	BYTE					SendBuf[SIZE_MAX_PACKET];
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
		// from SCVoIP
		case T_S2C_CONNECT:
// 			{
// 				this->SendAddMessageType(T_C2S_CONNECT_OK);
// 				procRes = RES_RETURN_TRUE;
// 			}
			procRes = _Process_S2C_CONNECT(pPacket, nLength, nBytesUsed);
			break;
		case T_S2C_SOCKET_ALIVE:
			{
				this->OnRecvdAlivePacket();
				procRes = RES_RETURN_TRUE;
			}
			break;
		case T_S2C_CREATE_ROOM:
			procRes = _Process_S2C_CREATE_ROOM(pPacket, nLength, nBytesUsed);
			break;
		case T_S2C_JOIN_ROOM:
			procRes = _Process_S2C_JOIN_ROOM(pPacket, nLength, nBytesUsed);
			break;
		case T_S2C_GOOUT_ROOM:
			procRes = _Process_S2C_GOOUT_ROOM(pPacket, nLength, nBytesUsed);
			break;

		case T_S2C_REQUEST_TOKEN:
			procRes = _Process_S2C_REQUEST_TOKEN(pPacket, nLength, nBytesUsed);
			break;
		case T_S2C_CHANGE_TALKING_STATE:		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
			procRes = _Process_S2C_CHANGE_TALKING_STATE(pPacket, nLength, nBytesUsed);
			break;

		case T_S2C_RTP_PACKET:
			procRes = _Process_S2C_RTP_PACKET(pPacket, nLength, nBytesUsed);
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
				sprintf(szSystemLog, "[Error] CNPCIOCPSocket::OnRecvdPacketNPCServer invalid protocol type, RecvType[%#04X]\r\n", nRecvType);
				g_pVoIPGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				procRes = RES_PACKET_ERROR;
			}
		}	// switch_end

		///////////////////////////////////////////////////////////////////////////////
		// MSG 결과 처리
		switch(procRes)
		{
		case RES_BREAK:			// 경미한 에러들, 연결을 끊지 않음, do nothing, 그냥 남아있는 다음 packet을 처리한다
		case RES_RETURN_TRUE:	// 정상, do nothing, 그냥 남아있는 다음 packet을 처리한다
			break;
		case RES_RETURN_FALSE:	// 심각한 오류 이므로 FALSE를 리턴하여 Socket을 종료한다
			return FALSE;			
		case RES_PACKET_ERROR:	// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
			return TRUE;
		}
	}	// while_end

	return TRUE;
}

void CVCIOCPSocket::OnConnect(void)
{
	// TODO: Place code here.
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pVoIPGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	CIOCPSocket::OnConnect();
}

void CVCIOCPSocket::OnClose(int reason)
{
	// TODO: Place code here.
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket  Closed SocketIndex[%3d] PeerIP[%15s] Port[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort, reason, reason);
	g_pVoIPGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	if(m_pJoinedRoom)
	{
		CRoom *pTmRoom = m_pJoinedRoom;
		m_pJoinedRoom = NULL;

		ms_pVCIOCP->OnDeleteMemberFromRoom(this, pTmRoom);
	}	

	CIOCPSocket::OnClose(30);
}

BOOL CVCIOCPSocket::OnError(int errCode)
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

void CVCIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err
									 , int errParam1/*=0*/, int errParam2/*=0*/
									 , char* errMsg/*=NULL*/, BOOL bCloseConnection/*=FALSE*/)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "  SendErrorMsg to IP(%15s) ==> MsgType(0x%6X) ErrType(0x%6X) Param1(%4d) Param2(%4d) Msg(%s)\r\n"
		, m_szPeerIP, msgType, err
		, errParam1, errParam2, errMsg);
	g_pVCGlobal->WriteSystemLog(szSystemLog);
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


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CVCIOCPSocket::_Process_S2C_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
/// \author		cmkwon
/// \date		2008-09-30 ~ 2008-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CVCIOCPSocket::_Process_S2C_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{
	// no body
	
	m_VoIPSockTy		= VOIP_SOCK_TYPE_SCVoIP;
	m_VoIPState			= VOIP_STATE_VC_Connected_SCVoIP;
		
	this->SendAddMessageType(T_C2S_CONNECT_OK);

	return RES_RETURN_TRUE;
}

ProcessResult CVCIOCPSocket::_Process_S2C_CREATE_ROOM(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2C_CREATE_ROOM		*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_S2C_CREATE_ROOM);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2C_CREATE_ROOM, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x16000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_S2C_CREATE_ROOM*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	char szLog[512];
	sprintf(szLog, "	RecvCreateRoom SCVoIP(%15s:%4d) RoomID(%I64d) UserName(%s) ConcurrentTalkingCounts(%d)\r\n"
		, this->GetPeerIP(), this->GetPeerPort()
		, pRecvMsg->createRoomID, pRecvMsg->szUserName, pRecvMsg->nConcurrentTalkingCount);
	g_pVCGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);

	///////////////////////////////////////////////////////////////////////////////
	//
	strncpy(m_szUserName, pRecvMsg->szUserName, SIZE_MAX_USER_NAME);

	///////////////////////////////////////////////////////////////////////////////
	// VDServer로 전송한다
	BYTE SendBuf[128];
	INIT_MSG(MSG_C2D_CREATE_ROOM_NOTIFY, T_C2D_CREATE_ROOM_NOTIFY, pSendMsg, SendBuf);	
	pSendMsg->ClientIndex				= this->m_nClientArrayIndex;
	pSendMsg->CreateRoomID				= pRecvMsg->createRoomID;
	pSendMsg->nConcurrentTalkingCount	= pRecvMsg->nConcurrentTalkingCount;
	strncpy(pSendMsg->szUserName, pRecvMsg->szUserName, SIZE_MAX_USER_NAME);
	g_pVCGlobal->Send2VDServer(SendBuf, MSG_SIZE(MSG_C2D_CREATE_ROOM_NOTIFY));
	
	return RES_RETURN_TRUE;
}

ProcessResult CVCIOCPSocket::_Process_S2C_JOIN_ROOM(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2C_JOIN_ROOM		*pRecvMsg;
	int						nRecvTypeSize;
	
	nRecvTypeSize = sizeof(MSG_S2C_JOIN_ROOM);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2C_JOIN_ROOM, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x16000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_S2C_JOIN_ROOM*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;
		
	if(FALSE == ms_pVCIOCP->On_S2C_JOIN_ROOM(pRecvMsg, this))
	{// SendError, RoomID가 없음
		
		SendErrorMessage(T_S2C_JOIN_ROOM, VOIPERR_NtoN_NO_SUCH_ROOMID);
		return RES_BREAK;
	}

	if(NULL == m_pJoinedRoom
		|| this->m_dwChannelIndex >= m_pJoinedRoom->m_dwMaxChannelCounts)
	{// Error
		
		SendErrorMessage(T_S2C_JOIN_ROOM, VOIPERR_NtoN_NO_SUCH_ROOMID);
		return RES_BREAK;
	}

	m_VoIPState		= VOIP_STATE_VC_Joined_Room;	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	char szLog[512];
	sprintf(szLog, "	RecvJoinRoom SCVoIP(%15s:%4d) RoomID(%I64d) UserName(%s)\r\n"
		, this->GetPeerIP(), this->GetPeerPort()
		, pRecvMsg->joinRoomID, pRecvMsg->szUserName);
	g_pVCGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);


	///////////////////////////////////////////////////////////////////////////////
	//
	strncpy(m_szUserName, pRecvMsg->szUserName, SIZE_MAX_USER_NAME);

	BYTE SendBuf[1024];
	///////////////////////////////////////////////////////////////////////////////
	// JoinRoom 성공 메세지를 전송
	INIT_MSG(MSG_C2S_JOIN_ROOM_OK, T_C2S_JOIN_ROOM_OK, pSMsg, SendBuf);
	pSMsg->nChannelIndex = this->m_dwChannelIndex;
	this->SendAddData(SendBuf, MSG_SIZE(MSG_C2S_JOIN_ROOM_OK));

	///////////////////////////////////////////////////////////////////////////////
	// 이미 Room에 참여중인 UserList를 전송한다
	INIT_MSG(MSG_C2S_JOINED_USERLIST, T_C2S_JOINED_USERLIST, pSMsg1, SendBuf);
	pSMsg1->byUserCounts = 0;
	JoinedUserInfo *pUserInfo = (JoinedUserInfo*)(SendBuf + MSG_SIZE(MSG_C2S_JOINED_USERLIST));
	m_pJoinedRoom->LockRoom();
	for(int i=0; i < m_pJoinedRoom->m_dwMaxChannelCounts; i++)
	{
		if(m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i]
			&& VOIP_STATE_VC_Joined_Room == m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i]->GetVoIPState()	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 체크 추가
			&& m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i] != this)
		{
			pUserInfo[pSMsg1->byUserCounts].nChannelIndex = m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i]->m_dwChannelIndex;
			strncpy(pUserInfo[pSMsg1->byUserCounts].szUserName, m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i]->GetUserName(), SIZE_MAX_USER_NAME);
			pUserInfo[pSMsg1->byUserCounts].bIsTalking		= m_pJoinedRoom->m_ArrayIOCPSocketPtrJoinedUser[i]->GetEnableTalking();		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 말하기 권한 플래그 전달
			pSMsg1->byUserCounts++;
		}
	}
	m_pJoinedRoom->UnlockRoom();
	this->SendAddData(SendBuf, MSG_SIZE(MSG_C2S_JOINED_USERLIST) + pSMsg1->byUserCounts*sizeof(JoinedUserInfo));

	///////////////////////////////////////////////////////////////////////////////
	// 이미 참여 중인 User들에게 정보를 전송한다.
	INIT_MSG(MSG_C2S_JOIN_USER, T_C2S_JOIN_USER, pSMsg2, SendBuf);
	pSMsg2->nChannelIndex = this->m_dwChannelIndex;
	strncpy(pSMsg2->szUserName, this->GetUserName(), SIZE_MAX_USER_NAME);
	pSMsg2->bIsTalking		= this->GetEnableTalking();		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	m_pJoinedRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_JOIN_USER), this);
	return RES_RETURN_TRUE;
}

ProcessResult CVCIOCPSocket::_Process_S2C_GOOUT_ROOM(const char* pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == m_pJoinedRoom)
	{
		return RES_BREAK;
	}

	m_VoIPState		= VOIP_STATE_VC_Connected_SCVoIP;	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	CRoom *pTmRoom = m_pJoinedRoom;
	m_pJoinedRoom = NULL;

	char szLog[512];
	sprintf(szLog, "	RecvGoOutRoom SCVoIP(%15s:%4d) RoomID(%I64d) UserName(%s) UserCounts(%d)\r\n"
		, this->GetPeerIP(), this->GetPeerPort()
		, pTmRoom->m_idRoomID, this->GetUserName()
		, pTmRoom->GetMembersCount());
	g_pVCGlobal->WriteSystemLog(szLog);
	DBGOUT(szLog);

	ms_pVCIOCP->OnDeleteMemberFromRoom(this, pTmRoom);	
	return RES_RETURN_TRUE;
}

ProcessResult CVCIOCPSocket::_Process_S2C_REQUEST_TOKEN(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2C_REQUEST_TOKEN	*pRecvMsg;
	int						nRecvTypeSize;
		
	nRecvTypeSize		= sizeof(MSG_S2C_REQUEST_TOKEN);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2C_REQUEST_TOKEN, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x16000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_S2C_REQUEST_TOKEN*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(NULL == m_pJoinedRoom)
	{
		SendErrorMessage(T_S2C_REQUEST_TOKEN, VOIPERR_NtoN_NO_SUCH_ROOM);
		return RES_BREAK;
	}

	if(FALSE != pRecvMsg->byIsTalk)
	{		
		if(FALSE == m_pJoinedRoom->GetTalkingToken(this))
		{
			SendErrorMessage(T_S2C_REQUEST_TOKEN, VOIPERR_NtoN_FAIL_REQUEST_TOKEN
				, m_pJoinedRoom->GetConcurrentTalkingCount(), 0, NULL, FALSE);
			return RES_BREAK;
		}		
	}
// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
//	this->SetTalking(pRecvMsg->byIsTalk);		
	this->SetEnableTalking(pRecvMsg->byIsTalk);		

	///////////////////////////////////////////////////////////////////////////////
	// 말하기 플래그 데이타를 다른 유저들에게 전송한다	
	BYTE SendBuf[128];
	INIT_MSG(MSG_C2S_REQUEST_TOKEN_OK, T_C2S_REQUEST_TOKEN_OK, pSMsg, SendBuf);
	pSMsg->byIsTalk			= pRecvMsg->byIsTalk;
	pSMsg->nChannelIndex	= this->m_dwChannelIndex;
	strncpy(pSMsg->szUserName, this->GetUserName(), SIZE_MAX_USER_NAME);
	m_pJoinedRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_REQUEST_TOKEN_OK));
	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CVCIOCPSocket::_Process_S2C_CHANGE_TALKING_STATE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
/// \author		cmkwon
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CVCIOCPSocket::_Process_S2C_CHANGE_TALKING_STATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2C_CHANGE_TALKING_STATE	*pRecvMsg;
	int						nRecvTypeSize;
		
	nRecvTypeSize		= sizeof(MSG_S2C_CHANGE_TALKING_STATE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2C_CHANGE_TALKING_STATE, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x16000);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_S2C_CHANGE_TALKING_STATE*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(NULL == m_pJoinedRoom)
	{
		SendErrorMessage(T_S2C_CHANGE_TALKING_STATE, VOIPERR_NtoN_NO_SUCH_ROOM);
		return RES_BREAK;
	}

	if(TALKING_STATE_START_TALKING == pRecvMsg->talkingState
		&& FALSE == this->GetEnableTalking())
	{// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
		SendErrorMessage(T_S2C_CHANGE_TALKING_STATE, VOIPERR_NtoN_CANNOT_TALKING, pRecvMsg->talkingState, this->GetEnableTalking());
		return RES_BREAK;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 말하기 상태 변경을 다른 유저들에게 전송한다	
	BYTE SendBuf[1024];
	INIT_MSG(MSG_C2S_CHANGE_TALKING_STATE_OK, T_C2S_CHANGE_TALKING_STATE_OK, pSMsg, SendBuf);
	pSMsg->talkingState		= pRecvMsg->talkingState;
	pSMsg->nChannelIndex	= this->m_dwChannelIndex;
	strncpy(pSMsg->szUserName, this->GetUserName(), SIZE_MAX_USER_NAME);
	m_pJoinedRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_CHANGE_TALKING_STATE_OK));
	return RES_RETURN_TRUE;
}


ProcessResult CVCIOCPSocket::_Process_S2C_RTP_PACKET(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_S2C_RTP_PACKET		*pRecvMsg;
	int						nRecvTypeSize;
	
	BYTE *pRecvRTPData	= (BYTE*)pPacket + nBytesUsed + sizeof(MSG_S2C_RTP_PACKET);
	pRecvMsg			= (MSG_S2C_RTP_PACKET*)(pPacket+nBytesUsed);	
	nRecvTypeSize		= sizeof(MSG_S2C_RTP_PACKET) + pRecvMsg->usRTPLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_S2C_RTP_PACKET, VOIPERR_PROTOCOL_INVAILD_LENGTH);
		Close(0x16000);
		return RES_RETURN_FALSE;
	}	
	nBytesUsed += nRecvTypeSize;

	if(VOIP_STATE_VC_Joined_Room != this->GetVoIPState())
	{// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		return RES_BREAK;
	}

	if(NULL == m_pJoinedRoom)
	{
		SendErrorMessage(T_S2C_RTP_PACKET, VOIPERR_NtoN_NO_SUCH_ROOM);
		return RES_BREAK;
	}
	else if(m_pJoinedRoom->GetMembersCount() <= 1
		|| FALSE == this->GetEnableTalking())	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 기존( || FALSE == this->GetTalking()) )
	{
		return RES_BREAK;
	}

	m_dwLastTickRecvdAudio	= timeGetTime();

	BYTE SendBuf[512];
	///////////////////////////////////////////////////////////////////////////////
	// 전송 받은 RTP 데이타를 다른 유저들에게 전송한다	
	INIT_MSG(MSG_C2S_RTP_PACKET, T_C2S_RTP_PACKET, pSMsg, SendBuf);	
	pSMsg->dwChannelIndex	= this->m_dwChannelIndex;
	pSMsg->byMediaType		= pRecvMsg->byMediaType;
	pSMsg->usRTPLength		= pRecvMsg->usRTPLength;
	memcpy(SendBuf+MSG_SIZE(MSG_C2S_RTP_PACKET), pRecvRTPData, pRecvMsg->usRTPLength);
	m_pJoinedRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_RTP_PACKET) + pSMsg->usRTPLength, this);

	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CVCIOCPSocket::SetEnableTalking(BOOL i_bFlag)
/// \brief		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
//void CVCIOCPSocket::SetTalking(BOOL i_bFlag)
void CVCIOCPSocket::SetEnableTalking(BOOL i_bFlag)
{
	m_bTalking		= i_bFlag;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CVCIOCPSocket::GetEnableTalking(void)
/// \brief		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
//BOOL CVCIOCPSocket::GetTalking(void)
BOOL CVCIOCPSocket::GetEnableTalking(void)
{
	return m_bTalking;
}

// 2008-10-15 by cmkwon, 말하기 기능 수정 - 필요 없는 함수임
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CVCIOCPSocket::IsTalking(void)
// /// \brief		
// /// \author		cmkwon
// /// \date		2005-10-05 ~ 2005-10-05
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CVCIOCPSocket::IsTalking(void)
// {
// 	if(NULL == m_pJoinedRoom
// 		|| FALSE == m_bTalking)
// 	{
// 		return FALSE;
// 	}
// 
// // 2008-10-15 by cmkwon, 말하기 기능 수정 - 묵음처리 기능 추가로 아래의 기능은 제거 해야함. 10초 동안 RTP 패킷이 오지 않으면 말하기 권한을 제거하는 기능
// // 	int tickGap = timeGetTime() - m_dwLastTickRecvdAudio;
// // 	if(tickGap > 10000)
// // 	{
// // 		this->m_bTalking		= FALSE;
// // 		
// // 		///////////////////////////////////////////////////////////////////////////////
// // 		// 말하기 권한 FALSE로 설정한다.
// // 		BYTE SendBuf[128];
// // 		INIT_MSG(MSG_C2S_REQUEST_TOKEN_OK, T_C2S_REQUEST_TOKEN_OK, pSMsg, SendBuf);
// // 		pSMsg->byIsTalk			= FALSE;
// // 		pSMsg->nChannelIndex	= this->m_dwChannelIndex;
// // 		strncpy(pSMsg->szUserName, this->GetUserName(), SIZE_MAX_USER_NAME);
// // 		m_pJoinedRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_REQUEST_TOKEN_OK));
// // 		return FALSE;
// // 	}
// 
// 	return TRUE;
// }