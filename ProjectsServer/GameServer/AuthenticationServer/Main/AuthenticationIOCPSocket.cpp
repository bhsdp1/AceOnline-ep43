// AuthenticationIOCPSocket.cpp: implementation of the CAuthenticationIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuthenticationIOCPSocket.h"
#include "AuthenticationIOCP.h"
#include "AuthenticationGlobal.h"
#include "AtumError.h"

CAuthenticationIOCP		*CAuthenticationIOCPSocket::ms_pAuthenticationIOCP = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuthenticationIOCPSocket::CAuthenticationIOCPSocket()
{
	m_PeerSocketType	= ST_INVALID_TYPE;
}

CAuthenticationIOCPSocket::~CAuthenticationIOCPSocket()
{
}

BOOL CAuthenticationIOCPSocket::OnRecvdPacketAuthenticationServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	int				nBytesUsed		= 0;
	MessageType_t	nRecvType		= 0;
	ProcessResult	procRes			= RES_BREAK;
	int				tmpSeq;
	MessageType_t	nOldRecvType	= 0;			// 2007-04-02 by cmkwon

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
			SendErrorMessage(T_PRE_IOCP, ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER);
			Close(0x11000, TRUE);
			return FALSE;
		}
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
	} // end TCP 처리 루틴


	while(this->IsUsing() && nBytesUsed < nLength)
	{
		nRecvType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		if(i_pThreadInfo)
		{
			i_pThreadInfo->dwSocketIndex = m_nClientArrayIndex;
			i_pThreadInfo->dwMessageType = nRecvType;
		}


#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nRecvType, m_szPeerIP, m_PeerSocketType, GGetexchangeMsgPrintLevel());
#endif

		switch(nRecvType)
		{
		//////////////////////////////////////////////////////////////////
		// AuthenticationServer <-- PreServer	
		case T_PAUTH_CONNECT_LOGIN :
				   procRes = Process_PAUTH_CONNECT_LOGIN(pPacket, nLength, nBytesUsed);
			break;	

		///////////////////////////////////////////////////////////////////////////////
		// 2006-08-04 by cmkwon
		default:
			{
				// Protocol Error 처리(Close 처리함)
				// - Client로 부터 받은 Message Type이 유효하지 않다
				SendErrorMessage(T_PRE_IOCP, ERR_PROTOCOL_INVALID_PROTOCOL_TYPE, 0, 0, NULL, TRUE);

				char	szSystemLog[1024];
				sprintf(szSystemLog, "[Error] CAuthenticationIOCPSocket::OnRecvdPacketAuthenticationServer invalid protocol type, OldRecvType(%#04X), CurRecvType[%s(%#04X)]", nOldRecvType, GetProtocolTypeString(nRecvType), nRecvType);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
		}	// end switch

		// MSG 결과 처리
		if (procRes == RES_BREAK)
		{
			// 경미한 에러들. 연결을 끊지 않는다.
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_PACKET_ERROR)
		{
			// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
			return TRUE;
		}
		else if (procRes == RES_RETURN_TRUE)
		{
			// 정상
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_RETURN_FALSE)
		{
			return FALSE;
		}

		nOldRecvType = nRecvType;						// 2007-04-02 by cmkwon, 추가함
	}	// end while

	return TRUE;
}

void CAuthenticationIOCPSocket::OnConnect(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType		= ST_INVALID_TYPE;

	CIOCPSocket::OnConnect();
}

void CAuthenticationIOCPSocket::OnClose(int reason)
{
	char szSystemLog[1024];

	// 2009-03-19 by cmkwon, 시스템 로그 추가 - m_PeerSocketType 정보 추가
	sprintf(szSystemLog, "Socket Closed  SocketIndex[%3d] SocketType[%d] PeerIP[%15s] Port[%4d] MaxWriteBufCounts[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_PeerSocketType, m_szPeerIP, m_nPeerPort, m_nMaxWriteBufCounts, reason, reason);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType = ST_INVALID_TYPE;

	CIOCPSocket::OnClose(30);
}

ProcessResult CAuthenticationIOCPSocket::Process_PAUTH_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize	= 0;
	MSG_PATUM_CONNECT		*pRecvMsg = NULL;

	nRecvTypeSize = sizeof(MSG_PATUM_CONNECT);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_PAUTH_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DATA);
		Close(0x16001, TRUE);
		return RES_RETURN_FALSE;
	}

	pRecvMsg = (MSG_PATUM_CONNECT *)(new char[sizeof(MSG_PATUM_CONNECT)]);
	memcpy(pRecvMsg, pPacket+nBytesUsed, nRecvTypeSize);
	nBytesUsed += nRecvTypeSize;

	if(strnicmp(((MSG_PATUM_CONNECT*)pRecvMsg)->szGameName, "", SIZE_MAX_ACCOUNT_NAME) == 0)
	{
		SendErrorMessage(T_PAUTH_CONNECT_LOGIN, ERR_PROTOCOL_EMPTY_ACCOUNTNAME);
		SAFE_DELETE(pRecvMsg);
		return RES_BREAK;
	}

	// 관련 처리
	QPARAM_AUTHENTICATION_ACCOUNT *pQMsg = new QPARAM_AUTHENTICATION_ACCOUNT;

	pQMsg->bResult = FALSE;
	STRNCPY_MEMSET(pQMsg->szGameName, pRecvMsg->szGameName, SIZE_MAX_AUTH_GAMENAME);
	STRNCPY_MEMSET(pQMsg->szServerPublicIP, GetPeerIP(), SIZE_MAX_IPADDRESS);
	STRNCPY_MEMSET(pQMsg->szServerPrivateIP, pRecvMsg->szServerIP, SIZE_MAX_IPADDRESS);
	pQMsg->nServerPort = pRecvMsg->nServerPort;
	STRNCPY_MEMSET(pQMsg->szCurrentVer, pRecvMsg->szCurrentVer, SIZE_MAX_AUTH_CURRENTVER);
	pQMsg->nLanguageType = pRecvMsg->nLanguageType;
	pQMsg->byTestServer = pRecvMsg->byTestServer;
	pQMsg->byUseExternalAuthentication = pRecvMsg->byUseExternalAuthentication;
	pQMsg->nPreServerGroupCnts = pRecvMsg->nPreServerGroupCnts;
	pQMsg->nEnableGameServerGroupCnts = pRecvMsg->nEnableGameServerGroupCnts;
	pQMsg->bAccept = FALSE;
	pQMsg->bShutdown = FALSE;
	MEMSET_ZERO(pQMsg->szAcceptComment, SIZE_MAX_AUTH_ACCEPT_COMMENT);
	MEMSET_ZERO(&pQMsg->AuthAccount, sizeof(SSERVER_AUTHENTICATION_ACCOUNT));

	// SQL 문의
	ms_pAuthenticationIOCP->m_pAtumDBManager->ExecuteQuery(QT_AUTH_CONFIRM, this, (void*)pQMsg);

	// SQL 결과 LOG 저장
	ms_pAuthenticationIOCP->m_pAtumDBManager->ExecuteQuery(QT_AUTH_INSERT_CONFIRM_LOG, this, (void*)pQMsg);

	if( TRUE != pQMsg->bResult )
	{
		SendErrorMessage(T_PAUTH_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_AUTHENTICATION_SERVER_LOGIN_DB);
		SAFE_DELETE(pQMsg);
		SAFE_DELETE(pRecvMsg);
		Close(0x16001, TRUE);
		return RES_RETURN_FALSE;
	}

	if( TRUE == pQMsg->bAccept )
	{
		if( TRUE == pQMsg->bShutdown )
		{
			INIT_MSG_WITH_BUFFER(MSG_PATUM_CONNECT_SHUTDOWN, T_PAUTH_CONNECT_LOGIN_SHUTDOWN, pSendMsg, SendBuf);
			SendAddData(SendBuf, MSG_SIZE(MSG_PATUM_CONNECT_SHUTDOWN));
		}
		else
		{
		INIT_MSG_WITH_BUFFER(MSG_PATUM_CONNECT_OK, T_PAUTH_CONNECT_LOGIN_OK, pSendMsg, SendBuf);
		SendAddData(SendBuf, MSG_SIZE(MSG_PATUM_CONNECT_OK));
		}
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_PATUM_CONNECT_FAIL, T_PAUTH_CONNECT_LOGIN_FAIL, pSendMsg, SendBuf);
		SendAddData(SendBuf, MSG_SIZE(MSG_PATUM_CONNECT_FAIL));
	}

	SAFE_DELETE(pQMsg);
	SAFE_DELETE(pRecvMsg);
	return RES_RETURN_TRUE;
}

// error 메세지를 전송
void CAuthenticationIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1, int errParam2, char* errMsg, BOOL bCloseConnection)
{
	MSG_ERROR	*pMsgError;
	BYTE		SendBuf[SIZE_MAX_PACKET];

	char szSystemLog[256];
	sprintf(szSystemLog, "  SendErrorMsg to ClientIndex[%3d] %15s(%4d) ==> %s(%#04x) AT %s Param1(%d) Param2(%d) Msg(%s)\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort,
		GetErrorString(err), err, GetProtocolTypeString(msgType), errParam1, errParam2, errMsg);
	g_pGlobal->WriteSystemLog(szSystemLog);
#ifdef _DEBUG
//	if (err != ERR_PROTOCOL_INVALID_MAP_EVENT_INFO)
	{
		DBGOUT(szSystemLog);
	}
#endif

	*(MessageType_t*)SendBuf = T_ERROR;
	pMsgError = (MSG_ERROR*)(SendBuf+SIZE_FIELD_TYPE_HEADER);
	pMsgError->CloseConnection = bCloseConnection;
	pMsgError->ErrorCode = err;
	pMsgError->MsgType = msgType;
	pMsgError->ErrParam1 = errParam1;
	pMsgError->ErrParam2 = errParam2;
	if (errMsg == NULL || 0 == strncmp(errMsg, "", SIZE_MAX_ERROR_STRING))
	{
		pMsgError->StringLength = 0;
	}
	else
	{
		pMsgError->StringLength = (USHORT)(strlen(errMsg) + 1);
		STRNCPY_MEMSET((char*)SendBuf + MSG_SIZE(MSG_ERROR), errMsg, SIZE_MAX_ERROR_STRING);
	}
	SendAddData(SendBuf, MSG_SIZE(MSG_ERROR) + pMsgError->StringLength);
}
