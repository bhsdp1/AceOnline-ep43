// SPToMPWinSocket.cpp: implementation of the CSPToMPWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPToMPWinSocket.h"
#include "PreGlobal.h"
#include "PreIOCP.h"
#include "AtumError.h"
#include "SecurityManager.h"		// 2011-06-22 by hskim, 사설 서버 방지
#include "TickManager.h"		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPToMPWinSocket::CSPToMPWinSocket(HWND i_hWnd, DWORD i_nTickInterval /*= SPTOMP_TICK_INTERVAL*/)		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
:CWinSocket(i_hWnd, WM_ASYNC_EVENT, WM_PACKET_NOTIFY, TRUE), CTickManager(i_nTickInterval)		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
{
	CTickManager::InitTickManager();		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
}

CSPToMPWinSocket::~CSPToMPWinSocket()
{

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSPToMPWinSocket::SetPreIOCP(CPreIOCP *i_pPreIOCP)
/// \brief		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
/// \author		cmkwon
/// \date		2008-02-22 ~ 2008-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSPToMPWinSocket::SetPreIOCP(CPreIOCP *i_pPreIOCP)
{
	m_pPreIOCP3		= i_pPreIOCP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSPToMPWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
/// \brief		
/// \author		cmkwon
/// \date		2008-02-22 ~ 2008-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSPToMPWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int				nBytesUsed	= 0;
	int				nMsgSize	= 0;
	MessageType_t	nType;
	ProcessResult	procRes		= RES_BREAK;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket + nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		switch(nType)
		{
		case T_PP_CONNECT_OK:
			procRes = Process_PP_CONNECT_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_PP_CONNECT_DO_CLOSE:
			procRes = Process_PP_CONNECT_DO_CLOSE(pPacket, nLength, nBytesUsed);
			break;
		case T_PAUTH_CONNECT_LOGIN_OK:		// 2011-01-26 by hskim, 인증 서버 구현
			procRes = Process_PAUTH_CONNECT_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_PAUTH_CONNECT_LOGIN_FAIL:	// 2011-01-26 by hskim, 인증 서버 구현
			procRes = Process_PAUTH_CONNECT_FAIL(pPacket, nLength, nBytesUsed);
			break;
		case T_PAUTH_CONNECT_LOGIN_SHUTDOWN:	// 2011-06-22 by hskim, 사설 서버 방지
			procRes = Process_PAUTH_CONNECT_SHUTDOWN(pPacket, nLength, nBytesUsed);
			break;
		case T_ERROR:
			procRes = Process_ERROR(pPacket, nLength, nBytesUsed);
			break;
		default:
			{	// check: unknown message type --> error!
				g_pPreGlobal->WriteSystemLogEX(TRUE, "[Error] CSPToMPWinSocket::OnRecvdPacket_ Unknown Message Type error !!, LastMsgType(0x%04X:%s) CurMsgType(0x%04X:%s)\r\n"
					, m_msgTyLastPacket, GetProtocolTypeString(m_msgTyLastPacket), nType, GetProtocolTypeString(nType));
				return TRUE;
			}
		}

		m_msgTyLastPacket	= nType;
	}
	return TRUE;
}

ProcessResult CSPToMPWinSocket::Process_PP_CONNECT_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_PP_CONNECT_OK,
											MSG_PP_CONNECT_OK, pRMsg);

	// start 2011-06-22 by hskim, 사설 서버 방지
	if(NULL != g_pPreGlobal->GetGIOCP() ) { return RES_RETURN_TRUE; }
	// end 2011-06-22 by hskim, 사설 서버 방지

	if(FALSE == g_pPreGlobal->InitServerSocket())
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] CSPToMPWinSocket::Process_PP_CONNECT_OK_ InitServerSocket_ error !!");
		return RES_BREAK;
	}
	g_pPreGlobal->StartTimerTraffic();
	g_pPreGlobal->StartTimerAliveCheck();

	return RES_RETURN_TRUE;
}

ProcessResult CSPToMPWinSocket::Process_PP_CONNECT_DO_CLOSE(const char* pPacket, int nLength, int &nBytesUsed)
{
	this->CloseSocket();
	return RES_RETURN_TRUE;
}

ProcessResult CSPToMPWinSocket::Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_ERROR,
											MSG_ERROR, pRMsg);
	char szErrString[1024];
	MEMSET_ZERO(szErrString, 1024);
	if(0 < pRMsg->StringLength)
	{
		STRNCPY_MEMSET(szErrString, pPacket + nBytesUsed, 1024);
		nBytesUsed += pRMsg->StringLength;
	}

//	g_pPreGlobal->WriteSystemLogEX(TRUE, "임시테스트용 CSPToMPWinSocket::Process_ERROR_ MsgType(0x%04X:%s) ErrorCode(%d) Param1(%d) Param2(%d) ErrString(%d:%s)\r\n"
//		, pRMsg->MsgType, GetProtocolTypeString(pRMsg->MsgType), pRMsg->ErrorCode, pRMsg->ErrParam1, pRMsg->ErrParam2
//		, pRMsg->StringLength, szErrString);

	switch(pRMsg->MsgType)
	{
	case T_PP_CONNECT:
		{
			this->CloseSocket();
		}
		break;
	default:
		{
			this->CloseSocket();
			g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] CSPToMPWinSocket::Process_ERROR_ MsgType(0x%04X:%s) ErrorCode(%d) Param1(%d) Param2(%d) ErrString(%d:%s)\r\n"
				, pRMsg->MsgType, GetProtocolTypeString(pRMsg->MsgType), pRMsg->ErrorCode, pRMsg->ErrParam1, pRMsg->ErrParam2
				, pRMsg->StringLength, szErrString);
		}
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버 구현
ProcessResult CSPToMPWinSocket::Process_PAUTH_CONNECT_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_PAUTH_CONNECT_LOGIN_OK,
		MSG_PATUM_CONNECT_OK, pRMsg);

	g_pPreGlobal->m_pSecurityManager->SetAuthentication(TRUE);		// 인증 성공

	if(NULL != g_pPreGlobal->GetGIOCP() ) { return RES_RETURN_TRUE; }

	if(FALSE == g_pPreGlobal->InitServerSocket())
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] CSPToMPWinSocket InitServerSocket_ error !!");

		return RES_BREAK;
	}

	g_pPreGlobal->StartTimerTraffic();
	g_pPreGlobal->StartTimerAliveCheck();

	this->CloseSocket();

	return RES_RETURN_TRUE;
}

ProcessResult CSPToMPWinSocket::Process_PAUTH_CONNECT_FAIL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_PAUTH_CONNECT_LOGIN_FAIL,
		MSG_PATUM_CONNECT_FAIL, pRMsg);

	// 인증 실패시에는 관련 로그 안남김

	this->CloseSocket();

	exit(0);

	return RES_RETURN_TRUE;
}

// start 2011-06-22 by hskim, 사설 서버 방지
ProcessResult CSPToMPWinSocket::Process_PAUTH_CONNECT_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_PAUTH_CONNECT_LOGIN_SHUTDOWN,
		MSG_PATUM_CONNECT_SHUTDOWN, pRMsg);
	
	this->CloseSocket();
	
	g_pPreGlobal->WriteSystemLogEX(TRUE, STRMSG_AUTHENTICATION_PRESERVER_SHUTDOWN_LOG, 64);		// 로그 출력
	g_pPreGlobal->m_pSecurityManager->SetTimer_InfinityMemoryLeak(SM_DELAY_TIME_SEC_INTERVAL);
		
	return RES_RETURN_TRUE;
}
// end 2011-06-22 by hskim, 사설 서버 방지

// start 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
void CSPToMPWinSocket::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif

	switch (pTickEvent->TickEventType)
	{
	case SPTOMP_TICK_EVENT_SESSION_TIMEOUT:
		{
			this->CloseSocket();

			g_pPreGlobal->WriteSystemLogEX(TRUE, "[Notify] CSPToMPWinSocket Timeout !!\r\n");

#ifdef S_AUTHENTICATION_SERVER_2ND_HSKIM
#else
			if(NULL != g_pPreGlobal->GetGIOCP() ) 
			{
				break;
			}
			
			if(FALSE == g_pPreGlobal->InitServerSocket())
			{
				g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] CSPToMPWinSocket::DoTickEvent InitServerSocket_ error !!\r\n");
				
				break;
			}

			g_pPreGlobal->StartTimerTraffic();
			g_pPreGlobal->StartTimerAliveCheck();
#endif
		}
		break;
		
	default:
		break;
	}
	
	return;
}

void CSPToMPWinSocket::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSPToMPWinSocket::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSPToMPWinSocket::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSPToMPWinSocket::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSPToMPWinSocket::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSPToMPWinSocket::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}
// end 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리