// CNPlayPCBangModule.h: interface for the CNPlayPCBangModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASPCBNAGMANAGER_H__F54CA42B_41BD_4644_A648_5B5A0E907EBC__INCLUDED_)
#define AFX_ASPCBNAGMANAGER_H__F54CA42B_41BD_4644_A648_5B5A0E907EBC__INCLUDED_
  
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(push, 1)
struct NET_Packet_PCBANG
{
	WORD	wLen;
	WORD	wReqType;	
};


//////////// 게임서버 연결 요청 ////////////
typedef struct _BOQPTS_GS_CONNECT
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	BYTE  ConnectFlag;
	
	WORD	RetCode;
}BOQPTS_GS_CONNECT, PBOQPTS_GS_CONNECT;

//////////// 서버 정상 동작 체크 요청 ////////////
typedef struct _BOQPTS_HEALTH_CHECK
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	
	WORD	RetCode;
} BOQPTS_HEALTH_CHECK, *PBOQPTS_HEALTH_CHECK;

//////////// PC방 결제정보 확인 요청 ////////////
typedef struct _BOQPTS_CHECKPREMIUM
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD ClientIP;
	WORD	ClientPort;
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	char    UserID[50+1];
	
	BYTE	PremiumType;
	BYTE	BillMethod;
	DWORD	RemainTime;
	WORD	RetCode;
} BOQPTS_CHECKPREMIUM, *PBOQPTS_CHECKPREMIUM;

//////////// PC방 결제정보 확인 요청2 ////////////
typedef struct _BOQPTS_CHECKPREMIUM2
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD ClientIP;
	WORD	ClientPort;
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	char    UserID[50+1];
	
	BYTE	PremiumType;
	BYTE	BillMethod;
	DWORD	RemainTime;
	BYTE	SameGameUser;
	DWORD	PCBangNo;
	WORD	RetCode;
} BOQPTS_CHECKPREMIUM2, *PBOQPTS_CHECKPREMIUM2;

//////////// 시간차감 시작 요청 ////////////
typedef struct _BOQPTS_GAMESTART
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD ClientIP;
	WORD	ClientPort;
	BYTE   DisconnectFlag;
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	char    UserID[50+1];
	char    CharacterID[30+1];
	BYTE	PremiumType;
	BYTE	BillMethod;
	DWORD	RemainTime;
	BYTE	SameGameUser;
	DWORD	PCBangNo;
	WORD	RetCode;
} BOQPTS_GAMESTART, *PBOQPTS_GAMESTART;

//////////// 게임서버 변경 요청////////////
typedef struct _BOQPTS_CHANGE_GAMESERVER
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	BYTE   Status;
	WORD  GameServerNoB;
	WORD  GameServerNoA;
	DWORD WorldNo;
	WORD  GameNo;
	DWORD UserNo;
	char    UserID[50+1];
	
	WORD	RetCode;
} BOQPTS_CHANGE_GAMESERVER, *PBOQPTS_CHANGE_GAMESERVER;

//////////// 시간차감 종료 요청 //////////// 
typedef struct _BOQPTS_GAMEEND
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD WorldNo;
	WORD  GameNo;
	char    UserID[50+1];
	
	WORD	RetCode;
} BOQPTS_GAMEEND, *PBOQPTS_GAMEEND;
//////////// 게임서버 내 모든 사용자 종료 요청 ////////////
typedef struct _BOQPTS_ALL_DISCONNECT
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	WORD  GameServerNo;
	DWORD WorldNo;
	WORD  GameNo;
	
	WORD	RetCode;
}BOQPTS_ALL_DISCONNECT, *PBOQPTS_ALL_DISCONNECT;

//////////// 접속상태 확인 요청 //////////// 
typedef struct _BOQPTS_ISALIVE
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD WorldNo;
	WORD  GameNo;
	char    UserID[50+1];
	
	WORD	RetCode;
} BOQPTS_ISALIVE, *PBOQPTS_ISALIVE;

//////////// 잔여시간 통보 //////////// 
typedef struct _BOQPTS_ALARM_EXPIRE
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD WorldNo;
	DWORD Minutes;
	char    UserID[50+1];
	
} BOQPTS_ALARM_EXPIRE, *PBOQPTS_ALARM_EXPIRE;

//////////// 잔여시간 만료 통보 //////////// 
typedef struct _BOQPTS_TIME_EXPIRED
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD WorldNo;
	char    UserID[50+1];
	
} BOQPTS_TIME_EXPIRED, *PBOQPTS_TIME_EXPIRED;

//////////// 상품 속성 변경 통보//////////// 
typedef struct _BOQPTS_PTYPE_CHANGED
{
	WORD  ReqLen;
	WORD  ReqType;
	DWORD ReqKey;
	
	DWORD UserNo;
	DWORD WorldNo;
	char    UserID[50+1];
	BYTE   PremiumTypeB;
	BYTE   PremiumTypeA;
} BOQPTS_PTYPE_CHANGED, *PBOQPTS_PTYPE_CHANGED;

#pragma pack(pop)

struct NPLAY_CONNECT_CLIENT_INFO
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];			// 사용자 아이디
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];			// 서버 그룹 이름
 	BOQPTS_CHECKPREMIUM2	PremiumInfo;					// 프리미엄 정보
};

typedef vector<NPLAY_CONNECT_CLIENT_INFO>		vectNPlayConnectClientList;


#include "WinSocket.h"
#include "TickManager.h"

class CPreIOCP;

//////////////////////////////////////////////////////////////////////
// class CNPlayPCBangModule
class CNPlayPCBangModule : public CWinSocket, public CTickManager
{
public:
	CNPlayPCBangModule(HWND i_hWnd, DWORD i_nTickInterval = SPTOMP_TICK_INTERVAL, char* i_szNPlayServerIP_Real = "", char* i_szNPlayServerIP_BackUp = "");
	virtual ~CNPlayPCBangModule();
	
	void NPlayCreateSocket();
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);
	virtual BOOL OnCloseSocket(int nErrorCode);

	BOOL NPlayServerConnect();
	BOOL IsConnected();
	void VectNPlayConnectClientList_Login(char* i_AccountName, char* i_ClientIP, int i_ClientPort);
	void VectNPlayConnectClientList_Pop(char* i_AccountName);
	void VectNPlayConnectClientList_SendPremiuminfo(char* i_AccountName, char* i_ServerGroupName);
	void VectNPlayConnectClientList_GameStart(char* i_AccountName, char* i_ServerGroupName);
	void VectNPlayConnectClientList_GameEnd(char* i_AccountName);
	void VectNPlayConnectClientList_SendAllDisConnectByServername(char* i_srvername);


protected:
	//////////////////////// Receive 패킷 //////////////////////// 
	ProcessResult Process_GsConnect			(const char* pPacket, int nLength, int &nBytesUsed);		// 게임서버연결 요청/응답
	ProcessResult Process_HealthCHeck		(const char* pPacket, int nLength, int &nBytesUsed);		// 게임서버연결 요청/응답
	ProcessResult Process_CheckPremium2		(const char* pPacket, int nLength, int &nBytesUsed);		// PC방 결제정보 확인 요청2
	ProcessResult Process_GameStart			(const char* pPacket, int nLength, int &nBytesUsed);		// 게임스타트
	ProcessResult Process_ChangeGameEnd		(const char* pPacket, int nLength, int &nBytesUsed);		// 게임종료
	ProcessResult Process_AllDisConnect		(const char* pPacket, int nLength, int &nBytesUsed);		// 전체종료
	ProcessResult Process_IsAlive			(const char* pPacket, int nLength, int &nBytesUsed);		// 살아있나?
	ProcessResult Process_AlarmExpire		(const char* pPacket, int nLength, int &nBytesUsed);		// 잔여시간 통보
	ProcessResult Process_TimeExpire		(const char* pPacket, int nLength, int &nBytesUsed);		// 잔여시간 만료 통보
	ProcessResult Process_BoqptsChanged		(const char* pPacket, int nLength, int &nBytesUsed);		// 상품변경 BOQPTS_PTYPE_CHANGED

	//////////////////////// Send 패킷 ////////////////////////
	// 게임서버연결 요청/응답
	BOOL GsConnect();
	// 게임서버 정상상태
	void SendHealthCheck();
	// PC방 결제정보 확인 요청2
	void SendCheckPremium2(char* pAccountName, char* pUserIp, WORD wClientPort);
	// 시간차감 시작 요청
	void SendGameStart(BOQPTS_CHECKPREMIUM2* pCheckPreMium2);
	// 시간차감 종료 요청
	void SendGameEnd(char* pAccountName);
	// 게임서버 내 모든 사용자 종료 요청
	void SendAllDisConnect();
	// 접속상태 확인 요청
	void SendIsAlive(char* pAccountName);

	int HealthCheckTimer();
	void SendPremiuminfoToField(char* i_AccountName, char* i_UserGameServerName, BOOL i_PremiumApply);
	void SendResidualTimeToField(char* i_AccountName, char* i_UserGameServerName, DWORD ResidualTime);
	
	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);

	BOOL m_bConnectNPlayServer;		// PC방 서버에 접속 여부
	vectNPlayConnectClientList	m_vectNPlayConnectClientList;

	BYTE				m_nGsConnectFailCount;
	ATUM_DATE_TIME		m_atimeGsConnectFailTime;
	ATUM_DATE_TIME		m_atimeHealthCheckTime;
	BOOL				m_bHealthCheckReturn;

	CRITICAL_SECTION	m_criticalSection;

private:

};

#endif