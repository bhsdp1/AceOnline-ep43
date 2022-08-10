                            // GlobalGameServer.h: interface for the CGlobalGameServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALGAMESERVER_H__A4741983_FEBC_498A_878A_A082B9852FD1__INCLUDED_)
#define AFX_GLOBALGAMESERVER_H__A4741983_FEBC_498A_878A_A082B9852FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "SocketHeader.h"
#include "AtumParam.h"
#include "Localization.h"


//////////////////////////////////////////////////////////////////////////
// Configuration File Path
#ifdef ARENA
#define GLOBAL_CONFIG_FILE_PATH					(string(CONFIG_ROOT)+string("./ArenaGlobal.cfg")).c_str()
#else
#define GLOBAL_CONFIG_FILE_PATH					(string(CONFIG_ROOT)+string("./global.cfg")).c_str()
#endif
#define DGA_CONFIG_FILE_PATH					(string(CONFIG_ROOT)+string("./dga.cfg")).c_str()
#define IM_SERVER_CONFIG_FILE_PATH				(string(CONFIG_ROOT)+string("./im.cfg")).c_str()
#define FIELD_SERVER_CONFIG_FILE_PATH			(string(CONFIG_ROOT)+string("./field.cfg")).c_str()
#define NPC_SERVER_CONFIG_FILE_PATH				(string(CONFIG_ROOT)+string("./npc.cfg")).c_str()
#define HAPPYHOUR_EVENT_CONFIG_FILE_PATH		(string(CONFIG_ROOT)+string("./happyHourEvent.cfg")).c_str()
#define LOCALIZATION_CONFIG_DIRECTORY_PATH		(string(CONFIG_ROOT)+string("/localization")).c_str()
#define AUTHENTICATION_CONFIG_FILE_PATH					(string(CONFIG_ROOT)+string("./authentication.cfg")).c_str()		// 2011-01-26 by hskim, 인증 서버 구현

// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 아래와 같이 디렉토리만 지정
// // 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - 아래와 같이 수정됨, '/' 사용 불가, '\' 사용 해야 함
// //#define SECURITY_HACKSHIELD_DIRECTORY_PATH		(string(CONFIG_ROOT)+string("./Security/HackShield/HackShield.crc")).c_str()		// 2006-06-05 by cmkwon
// #define SECURITY_HACKSHIELD_DIRECTORY_PATH		(string(CONFIG_ROOT)+string(".\\Security\\HackShield\\HackShield.crc")).c_str()		// 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - 아래와 같이 수정됨, '/' 사용 불가, '\' 사용 해야 함
#define SECURITY_HACKSHIELD_DIRECTORY_PATH		(string(CONFIG_ROOT)+string(".\\Security\\HackShield")).c_str()		// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 디렉토리만 설정함
#define SECURITY_XIGNCODE_DIRECTORY_PATH		(string(CONFIG_ROOT)+string("Security\\xigncode\\")).c_str()		// 2008-11-28 by cmkwon, 대만 Netpower_Tpe XignCode(게임가드) 적용 - 
#define SECURITY_NPROTECT_DIRECTORY_PATH		(string(CONFIG_ROOT)+string("Security\\nProtect\\")).c_str()		// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 - 
#define SECURITY_XTRAP_DIRECTORY_PATH			(string(CONFIG_ROOT)+string("Security\\XTrap")).c_str()			// 2009-10-06 by cmkwon, 베트남 게임 가드 X-TRAP으로 변경 - 

#define	PCBANG_IPADDR_FILE_PATH					(string(CONFIG_ROOT)+string("./PCBangIPList.cfg")).c_str()						// 2006-08-14 by dhjin
#define	RESOBJ_DIRECTORY_PATH					(string(CONFIG_ROOT)+string("./../map/Res-Obj")).c_str()							// 2007-05-28 by cmkwon
#define	RESTEX_DIRECTORY_PATH					(string(CONFIG_ROOT)+string("./../map/Res-Tex")).c_str()						// 2007-07-18 by cmkwon, omi.tex도 체크섬을 체크 루틴 추가
#define	RESEXE_DIRECTORY_PATH					(string(CONFIG_ROOT)+string("./../map/Res-EXE")).c_str()						// 2008-09-17 by cmkwon, 클라이언트 실행파일도 체크섬 체크 추가 - 


// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
#define CONFIG_NAME_DB_SERVER_IP						"DBServerIP"
#define CONFIG_NAME_DB_SERVER_PORT						"DBServerPort"
#define CONFIG_NAME_DB_SERVER_DATABASE_NAEE				"DBServerDatabaseName"
#define CONFIG_NAME_LOGDB_DB_SERVER_IP					"LOGDB_DBServerIP"
#define CONFIG_NAME_LOGDB_DB_SERVER_PORT				"LOGDB_DBServerPort"
#define CONFIG_NAME_LOGDB_DB_SERVER_DATABASE_NAEE		"LOGDB_DBServerDatabaseName"
#define CONFIG_NAME_EXT_AUTH_DB_SERVER_IP				"ExtAuthDBServerIP"
#define CONFIG_NAME_EXT_AUTH_DB_SERVER_PORT				"ExtAuthDBServerPort"
#define CONFIG_NAME_EXT_AUTH_DB_SERVER_DATABASE_NAEE	"ExtAuthDBServerDatabaseName"
// 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리 - grobal.cfg, Pre.cfg에 SOAP관련 정보 추가
#define CONFIG_NAME_EXT_AUTH_SOAP_URL					"ExtAuthSOAPURL"
#define CONFIG_NAME_EXT_AUTH_SOAP_GAMEID				"ExtAuthSOAP"
// END 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리 - grobal.cfg, Pre.cfg에 SOAP관련 정보 추가

//////////////////////////////////////////////////////////////////////////
// Timer Variables
const UINT TIMERID_TRAFFIC						= 100;					// Traffic 모니터 Timer
const UINT TIMERID_ALIVE_CHECK					= 101;					// 서버에서 Alive Check Timer
const UINT TIMERID_RECONNECT					= 102;
const UINT TIMERID_SERVER_GROUP_INFO			= 103;
const UINT TIMERGAP_TRAFFIC						= 10000;				// 10초(10*1000)
const UINT TIMERGAP_ALIVE_CHECK					= 60000;				// 60초(60*1000)
const UINT TIMERGAP_RECONNECT					= 5000;					// 5초
const UINT TIMERGAP_LOGSYSTEM					= 300000;				// 5분(5*60*1000)
const UINT TIMERGAP_SERVER_GROUP_INFO			= 30000;				// 30초
const UINT TIMERGAP_GLOGSYSTEM					= 550000;				// 10분 (10*60*1000 - 여분)		// 2011-12-12 by hskim, GLog 2차 // 2010-06-01 by shcho, GLogDB 관련		
const UINT TIMERGAP_TRIGGER_SYSTEM				= 60000;				// 60초(60 * 1000)	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

// 2008-03-03 by cmkwon, SIZE_MAX_ODBC_CONN_STRING 정의 위치 변경 - AtumParam.h 로 위치 이동 함 
//#define SIZE_MAX_ODBC_CONN_STRING				128

class CGlobalGameServer;
//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
extern CGlobalGameServer *	g_pGlobalGameServer;		// 전역변수관리를 위한 전역클래스포인터
extern char	*				ArrCommandList[];			// Command List
extern char	*				ArrCommandListforUser[];	// 일반 유저를 위한 Command List
extern char	*				ArrGameMasterCommandList[];	// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 Command List

#ifdef S_BONUS_KILL_SYSTEM_RENEWAL
#include <BonusKillSystem.h>			// 2015-06-16 by Future, Bonus Kill System Definitions
extern const KILL_REWARD_ITEM BonusKillSystemRewardItemNUMs[];	// 2015-06-12 Future, Bonus Kill Count Reward Items
#endif //  S_BONUS_KILL_SYSTEM_RENEWAL

//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 윈도우 메세지 프로시저
int GDeleteNewLine(char *str);


typedef ez_set<string>			ezsetString;

struct SServerGroupForMame
{
	char		szServerGroupName[SIZE_MAX_SERVER_NAME];
	int			nServerIDforMGame;
};
typedef vector<SServerGroupForMame>			vectorServerGroupForMGame;

///////////////////////////////////////////////////////////////////////////////
/// \class		CGlobalGameServer
///
/// \brief		전역변수를 클래스로 만들어 처리를 위해
/// \author		cmkwon
/// \version	
/// \date		2004-03-18 ~ 2004-03-18
/// \warning	객체는 하나만 만들어야 한다.
///////////////////////////////////////////////////////////////////////////////
class CIOCP;
class CGlobalGameServer : public CGlobal
{
public:
	CGlobalGameServer();
	virtual ~CGlobalGameServer();

	///////////////////////////////////////////////////////////////////////////
	// public Property
	void SetInstanceHandle(HINSTANCE i_hInstance);
	HINSTANCE GetInstanceHandle(void);
	void SetMainWndHandle(HWND i_hWnd);
	HWND GetMainWndHandle(void);
	UINT GetTimerIDTraffic(void);
	UINT GetTimerIDAliveCheck(void);
	UINT GetTimerIDReconnect(void);
		
	void SetIPLocal(char *i_szIP);
	char *GetIPLocal(void);
	void SetPublicIPLocal(char *i_szIP);
	char *GetPublicIPLocal(void);
	void SetIPPreServer(char *i_szIP);
	char *GetIPPreServer(void);
	void SetPortListening(int i_nPort);
	int GetPortListening(void);
	void SetPortPreServer(int i_nPort);
	int GetPortPreServer(void);	
	CIOCP *GetGIOCP(void);
	CSystemLogManager * GetPtrFieldGameLogManager(void);
	int GetMGameServerID(void){				return m_nMGameServerID1;}
	int GetLanguageType(void){				return m_nLanguageType;}
	char *GetServiceCountry(void);		// 2011-12-12 by hskim, GLog 2차

	void SetMGameEventType(MGAME_EVENT_t i_enEventType);
	void SetServerGroupName(char *i_szServerGroupName);
	char *GetServerGroupName(void);

	BOOL IsTestServer(void);
	BOOL InMasangServer(void);

	//////////////////////////////////////////////////////////////////////////
	// Method
	void PrintPreprocessorDefinitions(void);
	void WndRegisterClass(HINSTANCE hInstance, UINT IDI_Icon, char *szClass);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, char *szClass, char *szWindowText);
	void SetConfigRootPath(void);
//	BOOL WriteGameLog(USHORT msgType, MSG_FL_LOG* log, char* szAction, ofstream* file = NULL);
	const char* GetSZLogType(USHORT type);
	BOOL StartTimerTraffic(void);
	void EndTimerTraffic(void);
	BOOL StartTimerAliveCheck(void);
	void EndTimerAliveCheck(void);
	BOOL StartTimerReconnect(void);	
	void EndTimerReconnect(void);

	void CheckServerThread(void);
	void CalculateIOCPTraffic(void);
	void CheckClientAlive(void);

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
//	UCHAR* GetODBCDSN() { return m_szODBCDSN; }

	UCHAR* GetODBCUID() { return m_szODBCUID; }
	UCHAR* GetODBCPASSWORD() { return m_szODBCPASSWORD; }

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
//	UCHAR* GetLogDBODBCDSN(void) {			return m_szLogDBODBCDSN; }		// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - CGlobalGameServer::GetLogDBODBCDSN() 추가

	UCHAR* GetLogDBODBCUID(void) {			return m_szLogDBODBCUID; }		// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - CGlobalGameServer::GetLogDBODBCUID() 추가
	UCHAR* GetLogDBODBCPASSWORD(void) {		return m_szLogDBODBCPASSWORD; }	// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - CGlobalGameServer::GetLogDBODBCPASSWORD() 추가

	// 2006-04-27 by cmkwon
	BOOL IsExternalAuthentication(void){		return m_bIsExternalAuthentication;}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
//	UCHAR* GetExtAuthODBCDSN(void) {			return m_szExtAuthODBCDSN; }

	UCHAR* GetExtAuthODBCUID(void) {			return m_szExtAuthODBCUID; }
	UCHAR* GetExtAuthODBCPASSWORD(void) {		return m_szExtAuthODBCPASSWORD; }


	USHORT AuthAdminToolUser(const char *i_pUID, const char *i_pPWD);

	BOOL LoadStringList(ezsetString *o_psetStirng, const char *i_szFilePath, BOOL i_bPrintFlag=FALSE);

	// 욕설 및 금지 아이디
	BOOL LoadAbuseAndProhibitedName();
	BOOL CheckProhibitedName(const char *i_szName);

	// MGameEvent 계정/캐릭터명 로딩
	BOOL LoadAccountNameAndCharacterNameForMGameEvent(void);
	BOOL IsExistAccountNameForMGameEvent(char *i_szAccountName);

	// 
	BOOL InsertServerGroupforMGame(char *i_szServerGroupName, int i_nServerID);
	int GetServerIDforMGame(char *i_szServerGroupName);
	char *GetServerGroupNameByMGameServerID(int i_nMServerID);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	char *GetDBServerIP(void);
	int GetDBServerPort(void);
	char *GetDBServerDatabaseName(void);
	char *GetLogDBDBServerIP(void);
	int GetLogDBDBServerPort(void);
	char *GetLogDBDBServerDatabaseName(void);
	char *GetExtAuthDBServerIP(void);
	int GetExtAuthDBServerPort(void);
	char *GetExtAuthDBServerDatabaseName(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리
	BOOL GetIsExtAuthUseSOAP(void); // SOAP 사용 여부 FALSE이면 사용 안함
	char *GetExtAuthSOAPUrlString(void);
	// m_szSOAPUrlString[SIZE_MAX_URL_STRNIG];	// URL 스트링
	char *GetExtAuthSOAPGameID(void);
	//char				m_szSOAPGameID[SIZE_MAX_GAME_ID];		// GameID 사용여부
	// END 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
	vector<string>			m_AllowedToolIPList;
	void AddAllowedToolIP(const char *i_szAllowedToolIP);
	BOOL CheckAllowedToolIP(char *i_szToolIP);

	int GetServerID()		{ return m_nMGameServerID1; }		// 2013-03-13 by hskim, 웹 캐시 상점
public:	
	///////////////////////////////////////////////////////////////////////////
	// virtual Function	
	virtual BOOL InitServerSocket(void);						// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);							// 소켓관련 종료 함수
	virtual BOOL LoadConfiguration(void){	return FALSE;};		// Server 설정을 위한 Configuration Load

protected:
	//////////////////////////////////////////////////////////////////////////
	// 윈도우 관련 멤버 변수
	HINSTANCE			m_hInstanceHandle;				// 실행된 인스턴스 핸들
	HWND				m_hMainWndHandle;				// 실행된 메인 윈도우 핸들

	//////////////////////////////////////////////////////////////////////////
	// 윈도우 타이버 관련 멤버 변수
	UINT				m_nTimerIDTraffic;				// 모든 서버에서 Bandwidth 계산을 위한 TimerID
	UINT				m_nTimerIDAliveCheck;			// 모든 서버에서 클라이언트의 Alive 체크를 위한 TimerID
	UINT				m_nTimerIDReconnect;			// 서버(IMServer, FieldServer, NPCServer)에서 상위 서버와 접속이 종료 되었을때 재접속을 위한 TimerID

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	char				m_szIPLocal[SIZE_MAX_IPADDRESS];				// All (Pre, IM, Field, NPC)
	char				m_szPublicIPLocal[SIZE_MAX_IPADDRESS];			// All (Pre, IM, Field, NPC)// 2006-05-23 by cmkwon
	char				m_szIPPreServer[SIZE_MAX_IPADDRESS];			// two (IM, Field)
	int					m_nPortListening;								// one (Field)
	int					m_nPortPreServer;								// two (IM, Field)
	CSystemLogManager	m_FieldGameLogManager;
	UCHAR				m_szODBCDSN[SIZE_MAX_ODBC_CONN_STRING];			// ODBC_DSN
	UCHAR				m_szODBCUID[SIZE_MAX_ODBC_CONN_STRING];			// ODBC_UID
	UCHAR				m_szODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_PASSWD

	// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - CGlobalGameServer 에 멤버 변수 추가
	UCHAR				m_szLogDBODBCDSN[SIZE_MAX_ODBC_CONN_STRING];			// LogDB_ODBC_DSN
	UCHAR				m_szLogDBODBCUID[SIZE_MAX_ODBC_CONN_STRING];			// LogDB_ODBC_UID
	UCHAR				m_szLogDBODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];		// LogDB_ODBC_PASSWD

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon, 외부 DB 인증 관련
	BOOL					m_bIsExternalAuthentication;						// 외부 DB 인증 플래그
	UCHAR					m_szExtAuthODBCDSN[SIZE_MAX_ODBC_CONN_STRING];			// EXT_AUTH_ODBC_DSN
	UCHAR					m_szExtAuthODBCUID[SIZE_MAX_ODBC_CONN_STRING];			// EXT_AUTH_ODBC_UID
	UCHAR					m_szExtAuthODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];		// EXT_AUTH_ODBC_PASSWD

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	char				m_szDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nDBServerPort;
	char				m_szDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	char				m_szLogDBDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nLogDBDBServerPort;
	char				m_szLogDBDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	char				m_szExtAuthDBServerIP[SIZE_MAX_ODBC_CONN_STRING];
	int					m_nExtAuthDBServerPort;
	char				m_szExtAuthDBServerDatabaseName[SIZE_MAX_ODBC_CONN_STRING];


	char				m_szServerGroupName[SIZE_MAX_SERVER_NAME];		// one (IM)
	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리
	BOOL				m_IsSOAPUse;			// SOAP 사용 여부 FALSE이면 사용 안함
	char				m_szSOAPUrlString[SIZE_MAX_URL_STRNIG];	// URL 스트링
	char				m_szSOAPGameID[SIZE_MAX_GAME_ID];		// GameID 사용여부
	// END 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리

	vectorServerGroupForMGame		m_vectorServerGroupForMGame;		// Log, Field
	int					m_nMGameServerID1;								// Log, Field


	ezsetString			m_setAbuseWords;
	ezsetString			m_setProhibitedNames;

	ezsetString			m_setAccountNameForMGameEvent;
	ezsetString			m_setCharacterNameForMGameEvent;

	BOOL				m_bIsTestServer;								// TestServer 여부

	int					m_nLanguageType;

	CIOCP *				m_pGIOCP;


public:
	CLocalization		m_Localization;
	MGAME_EVENT_t		m_enMGameEventType;							// MGameEvent 
};

typedef mt_set<UID32_t>		mtsetUID32;

#endif // !defined(AFX_GLOBALGAMESERVER_H__A4741983_FEBC_498A_878A_A082B9852FD1__INCLUDED_)
