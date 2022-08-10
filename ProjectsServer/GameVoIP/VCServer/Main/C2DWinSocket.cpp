// C2DWinSocket.cpp: implementation of the CC2DWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C2DWinSocket.h"
#include "VCGlobal.h"
#include "VoIPProtocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CC2DWinSocket::CC2DWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify)
	: CWinSocket(hWnd, wmSock, wmNotify, FALSE)
{
	m_pVCIOCP1		= NULL;
}

CC2DWinSocket::~CC2DWinSocket()
{

}

void CC2DWinSocket::SetVCIOCP(CVCIOCP *i_pIOCP)
{
	m_pVCIOCP1 = i_pIOCP;
}


BOOL CC2DWinSocket::OnCloseSocket(int nErrorCode)
{
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////

	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CC2DWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CC2DWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int					nBytesUsed	= 0;
	MessageType_t		RecvMsgType	= 0;
	int					nRecvMsgSize = 0;
	ProcessResult		procRes		= RES_RETURN_FALSE;


	while(nBytesUsed < nLength)
	{
		procRes		= RES_BREAK;
		RecvMsgType = *(MessageType_t*)(pPacket+nBytesUsed);
		nRecvMsgSize = SIZE_FIELD_TYPE_HEADER;
		switch(RecvMsgType)
		{
		// T0_DC_CONNECT
		case T_D2C_CONNECT_OK:			// No body
			break;
		case T_D2C_REGISTER_SELF_OK:	// No body
			break;

		// T0_D2C_ROOM
		case T_D2C_CREATE_ROOM_NOTIFY_OK:
			{
				nRecvMsgSize += sizeof(MSG_D2C_CREATE_ROOM_NOTIFY_OK);
			}
			break;

		case T_ERROR:
			{
				nRecvMsgSize += sizeof(MSG_ERROR);
			}
			break;
		
		////////////////////////////////////////////////////////////////////////
		// Unknown Message	
		default:
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CC2DWinSocket::OnRecvdPacket Unknown Message Type, %d (0x%08X)\r\n"
					, RecvMsgType, RecvMsgType);
				g_pVoIPGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				procRes = RES_PACKET_ERROR;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		// 받은 메세지 체크
		if(nLength-(nBytesUsed+nRecvMsgSize) < 0){				procRes = RES_PACKET_ERROR;}

		///////////////////////////////////////////////////////////////////////////////
		// MSG 결과 처리
		switch(procRes)
		{
		case RES_RETURN_FALSE:	// 심각한 오류 이므로 FALSE를 리턴하여 Socket을 종료한다
		case RES_PACKET_ERROR:	// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다.
			return FALSE;
		}
		
		char *pTmMsg = new char[nRecvMsgSize];
		memcpy(pTmMsg, pPacket+nBytesUsed, nRecvMsgSize);
		EnterCriticalSection(&m_criticalRecvMessageQueue);
		m_queueRecvMessage.push(pTmMsg);
		LeaveCriticalSection(&m_criticalRecvMessageQueue);

		nBytesUsed +=nRecvMsgSize;
	}// while_end
	
	if(false == m_queueRecvMessage.empty())
	{
		PostNotify(WS_RECEIVED); 
	}

	return TRUE;
}

