#include "stdafx.h"
#include "NPlayPCBangModule.h"
#include "PreGlobal.h"
#include "PreIOCP.h"
#include "AtumError.h"
#include "TickManager.h"
#include "PreIOCPSocket.h"

#define	NPLAY_DEFINE_PORT					29021				// PC방 게임 서버 Port

#define	NPLAY_ACEONLINE_GAMECODE			125		// AceOnline GameCode
#define	NPLAY_OVERLAP_LOGIN_DISCONNECT		2		// 중복 로그인시 기존 연결의 강제접속 해지 여부
													// (1:접속 해제하지 않음, 2:접속해제)

#define	NPLAY_DEFINE_GSCONNECT				100		// 게임서버연결 요청/응답
#define	NPLAY_DEFINE_HEALTHCHECK			110		// 게임서버연결 요청/응답
#define	NPLAY_DEFINE_CHECKPREMIUM			120		// PC방 결제정보 확인 요청
#define	NPLAY_DEFINE_CHECKPREMIUM2			121		// PC방 결제정보 확인 요청2
#define	NPLAY_DEFINE_GAMESTART				200		// 게임스타트
#define	NPLAY_DEFINE_CHANGE_GAME_SERVER		210		// 게임서버 변경
#define	NPLAY_DEFINE_CHANGE_GAME_END		300		// 게임종료
#define	NPLAY_DEFINE_ALL_DISCONNECT			310		// 전체종료
#define	NPLAY_DEFINE_IS_ALIVE				400		// 살아있냐?
#define	NPLAY_DEFINE_ALARM_EXPIRE			500		// 잔여시간 통보
#define	NPLAY_DEFINE_TIME_EXPIRE			600		// 잔여시간 만료 통보
#define	NPLAY_DEFINE_BOQPTS_CHANGED			700		// 상품변경 BOQPTS_PTYPE_CHANGED

#define	NPLAY_TIME_HEALTHCHECK_ERROR_TIME	100		// (100초)
#define	NPLAY_TIME_HEALTHCHECK_TIME			120		// (120초->2분)

#define NPLAY_RECT_CODE_OK					0		// 처리 결과
#define NPLAY_RECT_CODE_OVERLAP_LOGIN		9		// 중복 로그인
#define NPLAY_REQ_KEY						482243	// 페킷 유효성 검사 Key

#define NPLAY_PCBANG_NO						0		// pc방 가맹점 여부
#define NPLAY_PREMIUM_NO					0		// 프리미엄 결제 여부

#define NPLAY_ALIVE_TRUE					1		// 접속중이다
#define NPLAY_ALIVE_FALSE					2		// 비접속중이다

#define NPLAY_HEALTHCHECK_WAIT				0		// 헬스체크 대기시간
#define NPLAY_HEALTHCHECK_SEND				1		// 헬스체크 보내야 할 시간
#define NPLAY_HEALTHCHECK_RECEIVE_DELAY		2		// 헬스체크 리턴 응답이 없음
#define NPLAY_HEALTHCHECK_DISCONNECT		3		// 연결 끊김


#define INIT_MSG_WITH_BUFFER_NPLAY(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)	\
	BYTE _BUFFER[sizeof(_MSG_STRUCT)];											\
	memset(_BUFFER, 0x00, sizeof(_MSG_STRUCT));									\
	_MSG_STRUCT *_VAR_NAME = (_MSG_STRUCT*)(_BUFFER);


#define IS_STRSUM(SOR, DES, RESULT)									\
	if( strlen(SOR) == strlen(DES) )								\
	{																\
		if ( 0 == strncmp(SOR, DES, strlen(SOR)) )	RESULT = TRUE;	\
		else RESULT = FALSE;										\
	}																\
	else RESULT = FALSE;


const char* GetPacketStr(MessageType_t type)
{
	switch(type)
	{
	case NPLAY_DEFINE_GSCONNECT:			return "NPLAY_DEFINE_GSCONNECT";
	case NPLAY_DEFINE_HEALTHCHECK:			return "NPLAY_DEFINE_HEALTHCHECK";
	//case NPLAY_DEFINE_CHECKPREMIUM:		return "NPLAY_DEFINE_CHECKPREMIUM";
	case NPLAY_DEFINE_CHECKPREMIUM2:		return "NPLAY_DEFINE_CHECKPREMIUM2";
	case NPLAY_DEFINE_GAMESTART:			return "NPLAY_DEFINE_GAMESTART";
	//case NPLAY_DEFINE_CHANGE_GAME_SERVER:	return "NPLAY_DEFINE_CHANGE_GAME_SERVER";
	case NPLAY_DEFINE_CHANGE_GAME_END:		return "NPLAY_DEFINE_CHANGE_GAME_END";
	case NPLAY_DEFINE_ALL_DISCONNECT:		return "NPLAY_DEFINE_ALL_DISCONNECT";
	case NPLAY_DEFINE_IS_ALIVE:			return "NPLAY_DEFINE_IS_ALIVE";
	case NPLAY_DEFINE_ALARM_EXPIRE:		return "NPLAY_DEFINE_ALARM_EXPIRE";
	case NPLAY_DEFINE_TIME_EXPIRE:			return "NPLAY_DEFINE_TIME_EXPIRE";
	case NPLAY_DEFINE_BOQPTS_CHANGED:		return "NPLAY_DEFINE_BOQPTS_CHANGED";
	}
	return "NO SUCH MESSAGE TYPE";
}
//////////////////////////////////////////////////////////////////////
// CNPlayPCBangModule
//////////////////////////////////////////////////////////////////////
CNPlayPCBangModule::CNPlayPCBangModule(HWND i_hWnd, DWORD i_nTickInterval /*= SPTOMP_TICK_INTERVAL*/, char* i_szNPlayServerIP_Real /*= ""*/, char* i_szNPlayServerIP_BackUp/* = ""*/)
:CWinSocket(i_hWnd, WM_ASYNC_EVENT_BY_NPLAY, WM_PACKET_NOTIFY_BY_NPLAY, FALSE, i_szNPlayServerIP_Real, i_szNPlayServerIP_BackUp), CTickManager(i_nTickInterval)
{
	CTickManager::InitTickManager();
	m_bConnectNPlayServer = FALSE;		// PC방 서버에 접속 여부
	m_vectNPlayConnectClientList.clear();
	memset(&m_atimeGsConnectFailTime, 0x00, sizeof(m_atimeGsConnectFailTime));
	m_nGsConnectFailCount = NULL;
	memset(&m_atimeHealthCheckTime , 0x00, sizeof(m_atimeHealthCheckTime));
	m_bHealthCheckReturn = FALSE;
	InitializeCriticalSection(&m_criticalSection);
}

CNPlayPCBangModule::~CNPlayPCBangModule()
{
	DeleteCriticalSection(&m_criticalSection);
}

void CNPlayPCBangModule::NPlayCreateSocket()
{
	CreateSocket();
	m_bConnectNPlayServer = FALSE;		// PC방 서버에 접속 여부
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPlayPCBangModule::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
/// \brief		
/// \author		jhseol
/// \date		2012-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPlayPCBangModule::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int				nBytesUsed	= 0;
	int				nMsgSize	= 0;
	WORD			nPacketLen;
	WORD			nType;
	ProcessResult	procRes		= RES_BREAK;
	
	nPacketLen = ntohs(*(WORD*)pPacket);
	nType = ntohs(*(WORD*)(pPacket + sizeof(WORD)));
	
	switch(nType)
	{
	case NPLAY_DEFINE_GSCONNECT:		// 게임서버연결 요청/응답
		procRes = Process_GsConnect(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_HEALTHCHECK:		// 게임서버 체크
		procRes = Process_HealthCHeck(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_CHECKPREMIUM2:	// PC방 결제정보 확인 요청2
		procRes = Process_CheckPremium2(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_GAMESTART:		// 게임스타트
		procRes = Process_GameStart(pPacket, nPacketLen, nBytesUsed);					
		break;
	case NPLAY_DEFINE_CHANGE_GAME_END:	// 게임종료
		procRes = Process_ChangeGameEnd(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_ALL_DISCONNECT:	// 전체종료
		procRes = Process_AllDisConnect(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_IS_ALIVE:		// 살아있나?
		procRes = Process_IsAlive(pPacket, nPacketLen, nBytesUsed);
		break;
	case NPLAY_DEFINE_ALARM_EXPIRE:	// 잔여시간 통보
		procRes = Process_AlarmExpire(pPacket, nPacketLen, nBytesUsed);			
		break;
	case NPLAY_DEFINE_TIME_EXPIRE:		// 잔여시간 만료 통보
		procRes = Process_TimeExpire(pPacket, nPacketLen, nBytesUsed);						
		break;
	case NPLAY_DEFINE_BOQPTS_CHANGED:	// 상품변경 BOQPTS_PTYPE_CHANGED
		procRes = Process_BoqptsChanged(pPacket, nPacketLen, nBytesUsed);									
		break;
		
	default:
		{	// check: unknown message type --> error!
			g_pPreGlobal->WriteSystemLogEX(TRUE, "[Error] CNPlayPCBangModule::OnRecvdPacket_ Unknown Message Type error !!, LastMsgType(%d:%s) ThisMsgType(%d)\r\n"
				, m_msgTyLastPacket, GetPacketStr(m_msgTyLastPacket), nType);
			return TRUE;
		}
	}
	
	m_msgTyLastPacket	= nType;
	return TRUE;
}

BOOL CNPlayPCBangModule::OnCloseSocket(int nErrorCode)
{
	CloseSocket(0x00001);
	m_PeerSocketType	= ST_INVALID_TYPE;
	m_bConnectNPlayServer = FALSE;
	PostNotify(WS_CLOSED, 0, (char*)this);
	return	TRUE;
}

BOOL CNPlayPCBangModule::NPlayServerConnect()
{
	return GsConnect();
}

// 게임서버연결 요청/응답
BOOL CNPlayPCBangModule::GsConnect()
{
	if( TRUE == m_bConnectNPlayServer )		// 이미 연결되 있는데 다시 연결하는 경우
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::GsConnect, m_bConnectNPlayServer is TRUE\r\n");
		return FALSE;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_GS_CONNECT, NPLAY_DEFINE_GSCONNECT, pSendMsg, SendBuf);

	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_GS_CONNECT));
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_GSCONNECT);	// (100:게임서버 연결요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 필요하지 않은 경우 0으로 설정	
	pSendMsg->GameServerNo		= htons(0);	// 게임서버 식별자 번호
	pSendMsg->WorldNo			= htons(0);	// 게임월드 식별자 번호
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);//htons(125);	// 열혈협객  GameNo : 120
	pSendMsg->RetCode			= htons(0);	// 응답코드(0:접속허용, 1:허용되지 않은 IP, >50:처리실패(50초과의 에러코드는 에러코드집 참조.))

	if (NPLAY_CONNECT_SERVER_EMPTY == g_pPreGlobal->GetNPlayConnectServer())
	{
		pSendMsg->ConnectFlag		= 1;	// 게임서버가 정상적으로 또는 Crash 후 기동되어 최초로 GS_CONNECT를 보낼 때는 1, 끊어진 연결을 복구 하는 경우에는 2 설정.
	}
	else
	{
		pSendMsg->ConnectFlag		= 2;	// 게임서버가 정상적으로 또는 Crash 후 기동되어 최초로 GS_CONNECT를 보낼 때는 1, 끊어진 연결을 복구 하는 경우에는 2 설정.
	}
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_GS_CONNECT), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::GsConnect, Packet Write Fail\r\n");
		return FALSE;
	}
	return TRUE;
}


ProcessResult CNPlayPCBangModule::Process_GsConnect(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_GSCONNECT,
		BOQPTS_GS_CONNECT, pRMsg);

	if( TRUE == m_bConnectNPlayServer)		// 이미 연결되 있는데 또 받은 경우
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_GsConnect, m_bConnectNPlayServer is TRUE\r\n");
		return RES_BREAK;
	}

	if( NPLAY_REQ_KEY != ntohl(pRMsg->ReqKey) || NPLAY_RECT_CODE_OK != ntohs(pRMsg->RetCode) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_GsConnect, [NPLAY_REQ_KEY != ReqKey] or [NPLAY_RECT_CODE_OK != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
		return RES_BREAK;
	}
	m_bConnectNPlayServer = TRUE;		// PC방 서버에 접속 여부
	SendHealthCheck();
	return RES_RETURN_TRUE;
}


// 게임서버 정상상태
void CNPlayPCBangModule::SendHealthCheck()
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendHealthCheck, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_HEALTH_CHECK, NPLAY_DEFINE_HEALTHCHECK, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_HEALTH_CHECK));
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_HEALTHCHECK);	// (110: PTS 정상 동작 체크 요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 필요하지 않은 경우 0으로 설정	
	pSendMsg->GameServerNo		= htons(0);	// 게임서버 식별자 번호		
	pSendMsg->WorldNo			= htons(0);	// 게임월드 식별자 번호
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120
	pSendMsg->RetCode			= htons(0);	// 응답코드(0:정상 동작), 
	// 즉 PTS에서 응답이 가면 정상으로 판단하고 
	// 응답이 가지 않는 경우 또는 요청을 보낼 수 없는 경우 PTS가 문제가 있다고 판단한다.
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_HEALTH_CHECK), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendHealthCheck, Packet Write Fail\r\n");
		return;
	}
	m_atimeHealthCheckTime.SetCurrentDateTime();
	m_bHealthCheckReturn = FALSE;
}


ProcessResult CNPlayPCBangModule::Process_HealthCHeck(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_HEALTHCHECK,
		BOQPTS_HEALTH_CHECK, pRMsg);
	
	if( FALSE == m_bConnectNPlayServer)
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_HealthCHeck, m_bConnectNPlayServer is FALSE \r\n");
		return RES_BREAK;
	}
	
	if( NPLAY_REQ_KEY != ntohl(pRMsg->ReqKey) || NPLAY_RECT_CODE_OK != ntohs(pRMsg->RetCode) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_HealthCHeck, [NPLAY_REQ_KEY != ReqKey] or [NPLAY_RECT_CODE_OK != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
		return RES_BREAK;
	}
	m_bHealthCheckReturn = TRUE;
	return RES_RETURN_TRUE;
}


// PC방 결제정보 확인 요청2
void CNPlayPCBangModule::SendCheckPremium2(char* pAccountName, char* pUserIp, WORD wClientPort)
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendCheckPremium2, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_CHECKPREMIUM2, NPLAY_DEFINE_CHECKPREMIUM2, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_CHECKPREMIUM2));		
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_CHECKPREMIUM2);	// 요청 구분(121: PC방 결제정보 확인 요청2)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 
	// 필요하지 않은 경우 0으로 설정	
	
	pSendMsg->UserNo			= htonl(0);	// 사용자의 고유번호 (아이디별 SeqNo) , 없는 경우 0 설정
	pSendMsg->ClientIP			= htonl(inet_addr(pUserIp));	// 사용자의 게임 클라이언트가 동작중인 PC의 IP주소		
	pSendMsg->ClientPort		= htons(wClientPort);			// 사용자의 게임 클라이언트가 Binding된 포트
	
	pSendMsg->GameServerNo		= htons(0);	// 게임서버 식별자 번호		
	pSendMsg->WorldNo			= htons(0);	// 게임월드 식별자 번호
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120		
	strncpy(pSendMsg->UserID, pAccountName, sizeof(pSendMsg->UserID));	// User ID
	
	pSendMsg->PremiumType		= 0;	// 프리미엄등급(0:No 프리미엄, 1:1등급, 2:2등급…)
	pSendMsg->BillMethod		= 0;	// 티켓 유형(1:정액제, 2:정량제,시간 정량제)
	pSendMsg->RemainTime		= 0;	// 잔여시간(잔여시간 분 단위)
	pSendMsg->SameGameUser		= 0;	// 현재 PC방에서 같은 게임을 하는 사용자수(0~255 사이)
	pSendMsg->PCBangNo			= 0;	// 현재 PC방의 유니크번호
	pSendMsg->RetCode			= 0;	// 응답코드(0:성공, 9:중복로그인,  >50:Failed)
	// *50초과의 에러코드는 에러코드집 참조.
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_CHECKPREMIUM2), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendCheckPremium2, Packet Write Fail\r\n");
	}
}


ProcessResult CNPlayPCBangModule::Process_CheckPremium2(const char* pPacket, int nLength, int &nBytesUsed)
{
	//g_pPreGlobal->WriteSystemLogEX(TRUE, " >>>>>>> Process_CheckPremium2 recv 확인용 \r\n");

	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_CHECKPREMIUM2,
		BOQPTS_CHECKPREMIUM2, pRMsg);

	//g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_CheckPremium2, recv OK ReqKey [%d], RetCode [%d]\r\n", ntohl(pRMsg->ReqKey), ntohs(pRMsg->RetCode));
	if ( NPLAY_REQ_KEY == ntohl(pRMsg->ReqKey) && ( NPLAY_RECT_CODE_OK == ntohs(pRMsg->RetCode) || NPLAY_RECT_CODE_OVERLAP_LOGIN == ntohs(pRMsg->RetCode) ) )
	{
		NPLAY_CONNECT_CLIENT_INFO ClientInfo;
		
		STRNCPY_MEMSET(ClientInfo.AccountName, pRMsg->UserID, sizeof(ClientInfo.AccountName));
		//ClientInfo.ClientIP = ntohl(pRMsg->ClientIP);
		
		ClientInfo.PremiumInfo.ReqLen		= ntohs(pRMsg->ReqLen);
		ClientInfo.PremiumInfo.ReqType		= ntohs(pRMsg->ReqType);
		ClientInfo.PremiumInfo.ReqKey		= ntohl(pRMsg->ReqKey);
		
		ClientInfo.PremiumInfo.UserNo		= ntohl(pRMsg->UserNo);
		ClientInfo.PremiumInfo.ClientIP		= ntohl(pRMsg->ClientIP);
		ClientInfo.PremiumInfo.ClientPort	= ntohs(pRMsg->ClientPort);
		ClientInfo.PremiumInfo.GameServerNo	= ntohs(pRMsg->GameServerNo);
		ClientInfo.PremiumInfo.WorldNo		= ntohl(pRMsg->WorldNo);
		ClientInfo.PremiumInfo.GameNo		= ntohs(pRMsg->GameNo);
		STRNCPY_MEMSET(ClientInfo.PremiumInfo.UserID, pRMsg->UserID, sizeof(ClientInfo.PremiumInfo.UserID));
		
		ClientInfo.PremiumInfo.PremiumType	= (pRMsg->PremiumType);
		ClientInfo.PremiumInfo.BillMethod	= (pRMsg->BillMethod);
		ClientInfo.PremiumInfo.RemainTime	= ntohl(pRMsg->RemainTime);
		ClientInfo.PremiumInfo.SameGameUser	= (pRMsg->SameGameUser);
		ClientInfo.PremiumInfo.PCBangNo		= ntohl(pRMsg->PCBangNo);
		ClientInfo.PremiumInfo.RetCode		= ntohs(pRMsg->RetCode);
		
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_CheckPremium2, Recevie OK [AccountName:%s] [PCBangNo:%d] [PremiunType:%d]\r\n", ClientInfo.AccountName, ClientInfo.PremiumInfo.PCBangNo, ClientInfo.PremiumInfo.PremiumType);
		
		EnterCriticalSection(&m_criticalSection);
		m_vectNPlayConnectClientList.push_back(ClientInfo);
		LeaveCriticalSection(&m_criticalSection);
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_CheckPremium2, m_vectNPlayConnectClientList Push OK [AccountName:%s]\r\n", ClientInfo.AccountName);
		return RES_RETURN_TRUE;
	}
	g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_CheckPremium2, [NPLAY_REQ_KEY != ReqKey] AND [NPLAY_RECT_CODE_OK != RetCode] OR [NPLAY_RECT_CODE_OVERLAP_LOGIN != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
	return RES_BREAK;
}



// 시간차감 시작 요청
void CNPlayPCBangModule::SendGameStart(BOQPTS_CHECKPREMIUM2* pCheckPreMium2)
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendGameStart, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_GAMESTART, NPLAY_DEFINE_GAMESTART, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_GAMESTART));		
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_GAMESTART);	// 요청 구분(200: 시간차감 시작 요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 
	// 필요하지 않은 경우 0으로 설정	
	
	pSendMsg->UserNo			= htonl(pCheckPreMium2->UserNo);	// 사용자의 고유번호 (아이디별 SeqNo) , 없는 경우 0 설정
	pSendMsg->ClientIP			= htonl(pCheckPreMium2->ClientIP);			// 사용자의 게임 클라이언트가 동작중인 PC의 IP주소		
	pSendMsg->ClientPort		= htons(pCheckPreMium2->ClientPort);			// 사용자의 게임 클라이언트가 Binding된 포트
	
	pSendMsg->DisconnectFlag	= NPLAY_OVERLAP_LOGIN_DISCONNECT;	// 중복 로그인시 기존 연결의 강제접속 해지 여부
	// (1:접속 해제하지 않음, 2:접속해제)
	
	pSendMsg->GameServerNo		= htons(0);	// 게임서버 식별자 번호		
	pSendMsg->WorldNo			= htonl(pCheckPreMium2->WorldNo);	// 게임월드 식별자 번호
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120
	
	memcpy(pSendMsg->UserID, pCheckPreMium2->UserID, sizeof(pSendMsg->UserID));				// User ID
	memset(pSendMsg->CharacterID, 0x00, sizeof(pSendMsg->CharacterID));			// 이용자 캐릭터명		
	
	pSendMsg->PremiumType		= (pCheckPreMium2->PremiumType);	// 프리미엄등급(0:No 프리미엄, 1:1등급, 2:2등급…)
	pSendMsg->BillMethod		= (pCheckPreMium2->BillMethod);	// 티켓 유형(1:정액제, 2:정량제,시간 정량제)
	pSendMsg->RemainTime		= htonl(pCheckPreMium2->RemainTime);	// 잔여시간(잔여시간 분 단위)
	pSendMsg->SameGameUser		= (pCheckPreMium2->SameGameUser);	// 현재 PC방에서 같은 게임을 하는 사용자수(0~255 사이)
	pSendMsg->PCBangNo			= htonl(pCheckPreMium2->PCBangNo);		// 현재 PC방의 유니크번호
	pSendMsg->RetCode			= htons(pCheckPreMium2->RetCode);		// 응답코드(0:성공, 9:중복로그인,  >50:Failed)
	// *50초과의 에러코드는 에러코드집 참조.
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_GAMESTART), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendGameStart, Packet Write Fail [AccountName:%s]\r\n", pCheckPreMium2->UserID);
	}
}


ProcessResult CNPlayPCBangModule::Process_GameStart(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_GAMESTART,
		BOQPTS_GAMESTART, pRMsg);
	//g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_GameStart, recv OK AccountName[%s], ReqKey [%d], RetCode [%d]\r\n", pRMsg->UserID, ntohl(pRMsg->ReqKey), ntohs(pRMsg->RetCode));
	if ( NPLAY_REQ_KEY == ntohl(pRMsg->ReqKey) && ( NPLAY_RECT_CODE_OK == ntohs(pRMsg->RetCode) || NPLAY_RECT_CODE_OVERLAP_LOGIN == ntohs(pRMsg->RetCode) ) )
	{
		// ReqKey 가 같고 ReCode가 0 또는 9 이면 정상임으로 여기서 종료 한다.
		return RES_RETURN_TRUE;
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_GameStart, [NPLAY_REQ_KEY != ReqKey] AND [NPLAY_RECT_CODE_OK != RetCode] OR [NPLAY_RECT_CODE_OVERLAP_LOGIN != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
		EnterCriticalSection(&m_criticalSection);
		if( 0 < m_vectNPlayConnectClientList.size() )
		{
			vectNPlayConnectClientList::iterator	itr = m_vectNPlayConnectClientList.begin();
			while( itr != m_vectNPlayConnectClientList.end() )
			{
				BOOL StrSumCheckResult;
				IS_STRSUM(itr->AccountName, pRMsg->UserID, StrSumCheckResult);
				if( TRUE == StrSumCheckResult )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_GameStart, SendPremiuminfoToField() [AccountName:%s] [PremiunType:0]\r\n", itr->AccountName);
					SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, FALSE);
					break;
				}
				itr++;
			}
			g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_GameStart, [AccountName:%s] Fin\r\n", pRMsg->UserID);
		}
		else
		{
			g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_GameStart, m_vectNPlayConnectClientList = 0\r\n");
		}
		LeaveCriticalSection(&m_criticalSection);
		return RES_RETURN_TRUE;
	}
	//g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Notify] CNPlayPCBangModule::Process_GameStart, SendGameStart Packet Recevie OK [AccountName:%s] [RetCode:%d]\r\n", pRMsg->UserID, ntohs(pRMsg->RetCode));
}


// 시간차감 종료 요청
void CNPlayPCBangModule::SendGameEnd(char* pAccountName)
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendGameEnd, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_GAMEEND, NPLAY_DEFINE_CHANGE_GAME_END, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_GAMEEND));		
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_CHANGE_GAME_END);	// 요청 구분(300: 시간차감 종료 요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 
	
	pSendMsg->UserNo			= htonl(0);	// 사용자의 고유번호 (아이디별 SeqNo), 없는 경우 0 설정// 필요하지 않은 경우 0으로 설정	
	pSendMsg->WorldNo			= 0;	// 게임월드 식별자 번호(월드 구분이 없는 경우 0 으로 설정)		
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120
	
	strncpy(pSendMsg->UserID, pAccountName, 50);				// User ID		
	pSendMsg->UserID[50]		= NULL;
	
	pSendMsg->RetCode			= 0;	// 응답코드(0:성공, <> 0:실패)		
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_GAMEEND), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendGameEnd, Packet Write Fail\r\n");
	}
}


ProcessResult CNPlayPCBangModule::Process_ChangeGameEnd(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_CHANGE_GAME_END,
		BOQPTS_GAMEEND, pRMsg);
	if( NPLAY_REQ_KEY == ntohl(pRMsg->ReqKey) && NPLAY_RECT_CODE_OK == ntohs(pRMsg->RetCode) )
	{
		return RES_RETURN_TRUE;
	}
	//g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Notify] CNPlayPCBangModule::Process_ChangeGameEnd, OK [AccountName:%s]\r\n", pRMsg->UserID);
	g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_ChangeGameEnd, [NPLAY_REQ_KEY != ReqKey] or [NPLAY_RECT_CODE_OK != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
	return RES_BREAK;
}



// 게임서버 내 모든 사용자 종료 요청
void CNPlayPCBangModule::SendAllDisConnect()
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendAllDisConnect, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_ALL_DISCONNECT, NPLAY_DEFINE_ALL_DISCONNECT, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_ALL_DISCONNECT));		
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_ALL_DISCONNECT);	// 요청 구분(310: 게임서버 내 모든 사용자 종료 요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 
	
	pSendMsg->GameServerNo		= htons(0);	// 게임서버 식별자 번호(GameServerNo <> 0:해당 게임서버이용자만 disconnect 함)
	pSendMsg->WorldNo			= 0;	// 게임 월드명(WorldNo <> 0 : 해당 월드의 이용자만 disconnect 함)
	//	GameServerNo와 WorldNo의 AND 조건으로 disconnect 함.	
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120				
	pSendMsg->RetCode			= 0;	// 응답코드(0:성공, <> 0:실패)		
	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_ALL_DISCONNECT), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendAllDisConnect, Packet Write Fail\r\n");
	}
}


ProcessResult CNPlayPCBangModule::Process_AllDisConnect(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_ALL_DISCONNECT,
		BOQPTS_ALL_DISCONNECT, pRMsg);
	if( NPLAY_REQ_KEY == ntohl(pRMsg->ReqKey) && NPLAY_RECT_CODE_OK == ntohs(pRMsg->RetCode) )
	{
		//g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Notify] CNPlayPCBangModule::Process_AllDisConnect, OK \r\n");
		return RES_RETURN_TRUE;
	}
	g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::Process_AllDisConnect, [NPLAY_REQ_KEY != ReqKey] or [NPLAY_RECT_CODE_OK != RetCode], [RecCode:%d]\r\n", ntohs(pRMsg->RetCode));
	return RES_BREAK;
}



// 접속상태 확인 요청
void CNPlayPCBangModule::SendIsAlive(char* pAccountName)
{
	if( FALSE == m_bConnectNPlayServer )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendIsAlive, m_bConnectNPlayServer is FALSE\r\n");
		return;
	}
	
	WORD bIsAlive = NPLAY_ALIVE_FALSE;

	INIT_MSG_WITH_BUFFER_NPLAY(BOQPTS_ISALIVE, NPLAY_DEFINE_IS_ALIVE, pSendMsg, SendBuf);
	pSendMsg->ReqLen			= htons(sizeof(BOQPTS_ISALIVE));		
	pSendMsg->ReqType			= htons(NPLAY_DEFINE_IS_ALIVE);	// 요청 구분(400: 접속상태 확인 요청)
	pSendMsg->ReqKey			= htonl(NPLAY_REQ_KEY);	// 요청 패킷 고유번호, Async 통신을 위해서 각 요청 패킷에 유니크하게 부여하는 번호, 
	
	pSendMsg->UserNo			= htonl(0);	// 사용자의 고유번호 (아이디별 SeqNo) , 없는 경우 0 설정		
	pSendMsg->WorldNo			= htonl(0);	// 게임월드 식별자 번호(월드 구분이 없는 경우 0 으로 설정)		
	//	GameServerNo와 WorldNo의 AND 조건으로 disconnect 함.		
	pSendMsg->GameNo			= htons(NPLAY_ACEONLINE_GAMECODE);	// 열혈협객  GameNo : 120		
	strncpy(pSendMsg->UserID, pAccountName, 50);				// User ID				

	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator	itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, pAccountName, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				bIsAlive = NPLAY_ALIVE_TRUE;
				break;
			}
			itr++;
		}
		pSendMsg->RetCode			= htons(bIsAlive);	// 응답코드(0:성공, <> 0:실패), 게임서버에 존재하는 이용자에 대해서만 응답코드를 1로 준다.
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Noyify] CNPlayPCBangModule::SendIsAlive, m_vectNPlayConnectClientList Size = 0\r\n");
		pSendMsg->RetCode			= htons(bIsAlive);	// 응답코드(0:성공, <> 0:실패), 게임서버에 존재하는 이용자에 대해서만 응답코드를 1로 준다.
	}
	LeaveCriticalSection(&m_criticalSection);
	g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Noyify] CNPlayPCBangModule::SendIsAlive, [AccountName:%s] Alive Check(%d)\r\n", pAccountName, bIsAlive);

	if( FALSE == Write((char *)SendBuf, sizeof(BOQPTS_ISALIVE), TRUE) )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Error] CNPlayPCBangModule::SendIsAlive, Packet Write Fail\r\n");
	}
}


ProcessResult CNPlayPCBangModule::Process_IsAlive(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_IS_ALIVE,
		BOQPTS_ISALIVE, pRMsg);

	SendIsAlive(pRMsg->UserID);
	return RES_RETURN_TRUE;
}


ProcessResult CNPlayPCBangModule::Process_AlarmExpire(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_ALARM_EXPIRE,
		BOQPTS_ALARM_EXPIRE, pRMsg);

	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator	itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, pRMsg->UserID, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				SendResidualTimeToField(itr->AccountName, itr->ServerGroupName, htonl(pRMsg->Minutes));
				break;
			}
			itr++;
		}
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_AlarmExpire, m_vectNPlayConnectClientList SZIE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
	//T_FP_PCBANG_USER_PREMIUM_RESIDUAL_TIME
	return RES_RETURN_TRUE;
}



ProcessResult CNPlayPCBangModule::Process_TimeExpire(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_TIME_EXPIRE,
		BOQPTS_TIME_EXPIRED, pRMsg);

	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator	itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, pRMsg->UserID, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				itr->PremiumInfo.PremiumType = FALSE;
				g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_TimeExpire, SendPremiuminfoToField() [AccountName:%s] [PremiunType:%d]\r\n", itr->AccountName, itr->PremiumInfo.PremiumType);
				SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, FALSE);
				SendResidualTimeToField(itr->AccountName, itr->ServerGroupName, 0);
				break;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_TimeExpire, [AccountName:%s] Fin\r\n", pRMsg->UserID);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_TimeExpire, m_vectNPlayConnectClientList SZIE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
	return RES_BREAK;
}


ProcessResult CNPlayPCBangModule::Process_BoqptsChanged(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, NPLAY_DEFINE_BOQPTS_CHANGED,
		BOQPTS_PTYPE_CHANGED, pRMsg);

	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator	itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, pRMsg->UserID, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				if( itr->PremiumInfo.PremiumType == pRMsg->PremiumTypeA )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_BoqptsChanged, [AccountName:%s] [Before, After PremiunType Is Same]\r\n", itr->AccountName);
				}
				else
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_BoqptsChanged, SendPremiuminfoToField() [AccountName:%s] [PremiunType:%d->%d]\r\n", itr->AccountName, itr->PremiumInfo.PremiumType, pRMsg->PremiumTypeA);
					itr->PremiumInfo.PremiumType = pRMsg->PremiumTypeA;
					if( NPLAY_PREMIUM_NO < itr->PremiumInfo.PremiumType )
					{
						SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, TRUE);
					}
					else
					{
						SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, FALSE);
					}
				}
				break;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_BoqptsChanged, [AccountName:%s] Fin\r\n", pRMsg->UserID);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::Process_BoqptsChanged, m_vectNPlayConnectClientList SZIE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
	return RES_RETURN_TRUE;
}

int CNPlayPCBangModule::HealthCheckTimer()
{
	if( FALSE == m_bConnectNPlayServer)
	{
		return NPLAY_HEALTHCHECK_DISCONNECT;
	}

	ATUM_DATE_TIME CurrentTime;
	memset(&CurrentTime, 0x00, sizeof(CurrentTime));
	CurrentTime.SetCurrentDateTime();
	if(CurrentTime.GetTimeInSeconds() - m_atimeHealthCheckTime.GetTimeInSeconds() >= NPLAY_TIME_HEALTHCHECK_TIME )
	{
		if( FALSE == m_bHealthCheckReturn )
		{
			return NPLAY_HEALTHCHECK_RECEIVE_DELAY;
		}
		return NPLAY_HEALTHCHECK_SEND;
	}
	return NPLAY_HEALTHCHECK_WAIT;
}


void CNPlayPCBangModule::SendPremiuminfoToField(char* i_AccountName, char* i_UserGameServerName, BOOL i_PremiumApply)
{
	CPreIOCP* PreIOCP = NULL;
	PreIOCP = (CPreIOCP*)g_pPreGlobal->GetGIOCP();
	if( NULL == PreIOCP )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, " [ERROR] CNPlayPCBangModule::SendPremiuminfoToField : Not Find PreIOCP!! \r\n");
		return;
	}
	
	CServerGroup* UserConnectCServerGroup = NULL;
	UserConnectCServerGroup = PreIOCP->GetServerGroup(i_UserGameServerName);
	if( NULL == UserConnectCServerGroup)
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, " [ERROR] CNPlayPCBangModule::SendPremiuminfoToField : [AccountName:%d][ServerGroupName:%s] Not Find UserConnectCServerGroup!! \r\n", i_AccountName, i_UserGameServerName);
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FP_PCBANG_USER_PREMIUM_INFO, T_FP_PCBANG_USER_PREMIUM_INFO, pPremiumInfo, SendBuf);
	STRNCPY_MEMSET(pPremiumInfo->AccountName, i_AccountName, sizeof(pPremiumInfo->AccountName));
	pPremiumInfo->PremiumApply	= i_PremiumApply;
	UserConnectCServerGroup->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FP_PCBANG_USER_PREMIUM_INFO));
}


void CNPlayPCBangModule::SendResidualTimeToField(char* i_AccountName, char* i_UserGameServerName, DWORD ResidualTime)
{
	CPreIOCP* PreIOCP = NULL;
	PreIOCP = (CPreIOCP*)g_pPreGlobal->GetGIOCP();
	if( NULL == PreIOCP )
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, " [ERROR] CNPlayPCBangModule::Process_AlarmExpire : Not Find PreIOCP!! \r\n");
		return;
	}
	
	CServerGroup* UserConnectCServerGroup = NULL;
	UserConnectCServerGroup = PreIOCP->GetServerGroup(i_UserGameServerName);
	if( NULL == UserConnectCServerGroup)
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, " [ERROR] CNPlayPCBangModule::Process_AlarmExpire : [AccountName:%d][ServerGroupName:%s] Not Find UserConnectCServerGroup!! \r\n", i_AccountName, i_UserGameServerName);
		return;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_FP_PCBANG_USER_PREMIUM_RESIDUAL_TIME, T_FP_PCBANG_USER_PREMIUM_RESIDUAL_TIME, pPremiumInfo, SendBuf);
	STRNCPY_MEMSET(pPremiumInfo->AccountName, i_AccountName, sizeof(pPremiumInfo->AccountName));
	pPremiumInfo->ResidualTime	= ResidualTime;
	
	if( 10 <= ResidualTime )
	{
		if( 0 == ResidualTime % 10 )
		{
			UserConnectCServerGroup->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FP_PCBANG_USER_PREMIUM_RESIDUAL_TIME));
		}
	}
	else
	{
		UserConnectCServerGroup->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FP_PCBANG_USER_PREMIUM_RESIDUAL_TIME));
	}
}


void CNPlayPCBangModule::VectNPlayConnectClientList_Login(char* i_AccountName, char* i_ClientIP, int i_ClientPort)
{
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Nptify] CNPlayPCBangModule::VectNPlayConnectClientList_Login, SendCheckPremium2() [AccountName:%s]\r\n", i_AccountName);
	SendCheckPremium2(i_AccountName, i_ClientIP, i_ClientPort);
}

void CNPlayPCBangModule::VectNPlayConnectClientList_Pop(char* i_AccountName)
{
	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, i_AccountName, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_Pop, m_vectNPlayConnectClientList Pop OK [AccountName:%s]\r\n", itr->AccountName);
				m_vectNPlayConnectClientList.erase(itr);
				continue;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_Pop, [AccountName:%s] Fin\r\n", i_AccountName);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_Pop, m_vectNPlayConnectClientList SZIE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
}

void CNPlayPCBangModule::VectNPlayConnectClientList_SendPremiuminfo(char* i_AccountName, char* i_ServerGroupName)
{
	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, i_AccountName, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				STRNCPY_MEMSET(itr->ServerGroupName, i_ServerGroupName, sizeof(itr->ServerGroupName));
				g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_PremiunCheck, SendPremiuminfoToField() [AccountName:%s] [PremiunType:%d]\r\n", itr->AccountName, itr->PremiumInfo.PremiumType);
				if( NPLAY_PREMIUM_NO < itr->PremiumInfo.PremiumType )
				{
					SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, TRUE);
				}
				else
				{
					SendPremiuminfoToField(itr->AccountName, itr->ServerGroupName, FALSE);
				}
				break;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_PremiunCheck, [AccountName:%s] Fin\r\n", i_AccountName);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_PremiunCheck, m_vectNPlayConnectClientList SIZE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
}


void CNPlayPCBangModule::VectNPlayConnectClientList_GameStart(char* i_AccountName, char* i_ServerGroupName)
{
	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, i_AccountName, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				STRNCPY_MEMSET(itr->ServerGroupName, i_ServerGroupName, SIZE_MAX_SERVER_NAME);
				if( NPLAY_PCBANG_NO < itr->PremiumInfo.PCBangNo )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameStart, SendGameStart() [AccountName:%s] Start PCBang [Premium:%d] \r\n", itr->AccountName, itr->PremiumInfo.PremiumType);
					SendGameStart(&itr->PremiumInfo);
				}
				break;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameStart, [AccountName:%s] Fin\r\n", i_AccountName);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameStart, m_vectNPlayConnectClientList SIZE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
}

void CNPlayPCBangModule::VectNPlayConnectClientList_GameEnd(char* i_AccountName)
{
	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->AccountName, i_AccountName, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				if( NPLAY_PCBANG_NO < itr->PremiumInfo.PCBangNo )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameEnd, SendGameEnd() [AccountName:%s] Is PCBang SendGameEnd() Request\r\n", i_AccountName);
					SendGameEnd(itr->AccountName);
				}
				break;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameEnd, [AccountName:%s] Not Fin\r\n", i_AccountName);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_GameEnd, m_vectNPlayConnectClientList SIZE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
}

void CNPlayPCBangModule::VectNPlayConnectClientList_SendAllDisConnectByServername(char* i_srvername)
{
	EnterCriticalSection(&m_criticalSection);
	if( 0 < m_vectNPlayConnectClientList.size() )
	{
		vectNPlayConnectClientList::iterator itr = m_vectNPlayConnectClientList.begin();
		while( itr != m_vectNPlayConnectClientList.end() )
		{
			BOOL StrSumCheckResult;
			IS_STRSUM(itr->ServerGroupName, i_srvername, StrSumCheckResult);
			if( TRUE == StrSumCheckResult )
			{
				if( NPLAY_PCBANG_NO < itr->PremiumInfo.PCBangNo )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_SendAllDisConnectByServername, SendGameEnd() [AccountName:%s] Is PCBang SendGameEnd() Request\r\n", itr->AccountName);
					SendGameEnd(itr->AccountName);
				}
				g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_SendAllDisConnectByServername, m_vectNPlayConnectClientList.erase() [AccountName:%s] [ServerName:%s]\r\n", itr->AccountName, itr->ServerGroupName);
				m_vectNPlayConnectClientList.erase(itr);
				continue;
			}
			itr++;
		}
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_SendAllDisConnectByServername, [ServerName:%s] Fin 0\r\n", i_srvername);
	}
	else
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE,"  [Notify] CNPlayPCBangModule::VectNPlayConnectClientList_SendAllDisConnectByServername, m_vectNPlayConnectClientList SIZE = 0\r\n");
	}
	LeaveCriticalSection(&m_criticalSection);
}

BOOL CNPlayPCBangModule::IsConnected()
{
	return m_bConnectNPlayServer;
}

void CNPlayPCBangModule::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
}

void CNPlayPCBangModule::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPlayPCBangModule::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPlayPCBangModule::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPlayPCBangModule::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	int tmCheck = HealthCheckTimer();
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CNPlayPCBangModule::DoMinutelyWork, HealthCheckTimer() = %d\r\n", tmCheck);
	switch(tmCheck)
	{
	case NPLAY_HEALTHCHECK_SEND:
		{
			SendHealthCheck();
		}
		break;
	case NPLAY_HEALTHCHECK_RECEIVE_DELAY:
	case NPLAY_HEALTHCHECK_DISCONNECT:
		{
			CloseSocket(0x00001);
			m_bConnectNPlayServer = FALSE;
			CreateSocket();

			if( NPLAY_CONNECT_SERVER_REAL == g_pPreGlobal->GetNPlayConnectServer() )
			{
				g_pPreGlobal->SetNPlayConnectServer(NPLAY_CONNECT_SERVER_BACKUP);
				this->Connect(g_pPreGlobal->GetNPlayServerIP_BackUP(), NPLAY_SERVER_PORT);
			}
			else if( NPLAY_CONNECT_SERVER_BACKUP == g_pPreGlobal->GetNPlayConnectServer() )
			{
				g_pPreGlobal->SetNPlayConnectServer(NPLAY_CONNECT_SERVER_REAL);
				this->Connect(g_pPreGlobal->GetNPlayServerIP_Real(), NPLAY_SERVER_PORT);
			}
		}
		break;
	}
}

void CNPlayPCBangModule::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CNPlayPCBangModule::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}