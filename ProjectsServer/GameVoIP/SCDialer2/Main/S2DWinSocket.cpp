// S2DWinSocket.cpp: implementation of the CS2DWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCDialer2.h"
#include "S2DWinSocket.h"
#include "VoIPProtocol.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CS2DWinSocket::CS2DWinSocket(HWND hWnd, UINT wmSock/*=WM_S2D_ASYNC_EVENT*/, UINT wmNotify/*=WM_S2D_PACKET_NOTIFY*/)
	: CWinSocket(hWnd, wmSock, wmNotify, FALSE)
{

}

CS2DWinSocket::~CS2DWinSocket()
{

}


BOOL CS2DWinSocket::OnCloseSocket(int nErrorCode)
{
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////

	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CS2DWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CS2DWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
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
		case T_D2S_CONNECT_OK:		// nobody
			{				
			}			
			break;
		case T_D2S_GET_VCSERVER_CONNECT_INFO_OK:
			{
				nRecvMsgSize += sizeof(MSG_D2S_GET_VCSERVER_CONNECT_INFO_OK);				
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
				sprintf(szSystemLog, "[Error] CN2FSocket::OnRecvdPacket Unknown Message Type, %d (0x%08X)\r\n"
					, RecvMsgType, RecvMsgType);
				DbgOut(szSystemLog);
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