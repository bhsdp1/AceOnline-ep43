// GlobalGameServer.cpp: implementation of the CGlobalGameServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalGameServer.h"
#include "IOCP.h"
#include "ODBCStatement.h"
#include "md5_lib_src.h"
#include "AtumDBManager.h"

char *ArrCommandList[] = {
	// Field Server
	STRCMD_CS_COMMAND_ITEMINFObyKIND_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyKIND_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_HELP,
	STRCMD_CS_COMMAND_QUESTINFO_HELP,
	STRCMD_CS_COMMAND_QUESTDEL_HELP,	
	STRCMD_CS_COMMAND_QUESTCOMPLETION_HELP,			// 2005-12-07 by cmkwon
	STRCMD_CS_COMMAND_STATINIT_HELP,
	STRCMD_CS_COMMAND_PARTYINFO_HELP,
	STRCMD_CS_COMMAND_GAMETIME_HELP,
	STRCMD_CS_COMMAND_STRINGLEVEL_HELP,
	STRCMD_CS_COMMAND_SKILLALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTWEAPON_HELP,
	STRCMD_CS_COMMAND_ITEMDELALL_HELP,
	STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_HELP,
	STRCMD_CS_COMMAND_ITEMDROP_HELP,
	STRCMD_CS_COMMAND_USERSINFOTOTAL_HELP,
	STRCMD_CS_COMMAND_DEBUGPRINTDBG_HELP,
	STRCMD_CS_COMMAND_DEBUGSETPARAMF_HELP,
	STRCMD_CS_COMMAND_BULLETCHARGE_HELP,
	STRCMD_CS_COMMAND_REPAIRALL_HELP,
	STRCMD_CS_COMMAND_REPAIRbyPARAM_HELP,
	STRCMD_CS_COMMAND_USERNORMALIZE_HELP,
	STRCMD_CS_COMMAND_USERSPECIALIZE_HELP,
	STRCMD_CS_COMMAND_POWERUP_HELP,
	STRCMD_CS_COMMAND_VARIABLESET_HELP,
	STRCMD_CS_COMMAND_LEVELSET_HELP,
	STRCMD_CS_COMMAND_PARTNERLEVELSET_HELP,		// 2011-09-05 by hskim, 파트너 시스템 2차
	STRCMD_CS_COMMAND_DEBUGPRINTMSGF_HELP,
	STRCMD_CS_COMMAND_GAMEEVENT_HELP,
	STRCMD_CS_COMMAND_PREMEUM_HELP,
// 2008-02-14 by cmkwon, 도시점령전 명령어 제거
//	STRCMD_CS_COMMAND_CITYWAR_HELP,
	STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP,
	STRCMD_CS_COMMAND_BONUSSTAT_HELP,	

// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 
//	STRCMD_CS_COMMAND_COMEONINFL_HELP,				// 2006-07-27 by cmkwon
	STRCMD_CS_COMMAND_COMEONINFL_HELP2,				// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 

	STRCMD_CS_COMMAND_ITEMINMAP_HELP,				// 2006-07-27 by cmkwon
	STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_HELP,		// 2006-08-09 by dhjin
	STRCMD_CS_COMMAND_BONUSSTAT_POINT_HELP,
	STRCMD_CS_COMMAND_PCBANGUSERCOUNT_HELP,
	STRMSG_CS_COMMAND_WARPOINT_HELP,
	STRCMD_CS_COMMAND_WAPPOINTINMAP_HELP,		// 2007-11-05 by cmkwon, WP 지급 명령어 구현 -
	STRCMD_CS_COMMAND_ITEMALLUSER_HELP,			// 2008-02-20 by cmkwon, 명령어 추가(접속 중인 유저 모두에게 아이템 지급 - 

	// IM Server
	STRCMD_CS_COMMAND_DEBUGSETPARAMI_HELP,
	STRCMD_CS_COMMAND_REGISTERADMIN_HELP,
	STRCMD_CS_COMMAND_DEBUGPRINTMSGI_HELP,
	STRCMD_CS_COMMAND_SERVERDOWN_HELP,
	STRCMD_CS_COMMAND_GUILDINFO_HELP,
	STRCMD_CS_COMMAND_WEATHERSET_HELP,
	STRCMD_CS_COMMAND_CONPOINT_HELP,

	NULL
};

// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 Command List
char *ArrGameMasterCommandList[] = {
	// Field Server
	STRCMD_CS_COMMAND_MOVE_HELP,
	STRCMD_CS_COMMAND_COORDINATE_HELP,
	STRCMD_CS_COMMAND_LIST_HELP,
	STRCMD_CS_COMMAND_USERSEND_HELP,
	STRCMD_CS_COMMAND_INFObyNAME_HELP,
	STRCMD_CS_COMMAND_MONSUMMON_HELP,
	STRCMD_CS_COMMAND_USERSINFOperMAP_HELP,
	STRCMD_CS_COMMAND_CHANNELINFO_HELP,
	STRCMD_CS_COMMAND_USERINVINCIBILITY_HELP,
	STRCMD_CS_COMMAND_USERINVISIABLE_HELP,
	STRCMD_CS_COMMAND_STEALTH_HELP,
	STRCMD_CS_COMMAND_RETURNALL_HELP,
	STRCMD_CS_COMMAND_CHANGEINFL_HELP,				// 2006-02-08 by cmkwon
	STRCMD_CS_COMMAND_STRATEGYPOINTINFO_HELP,
	STRCMD_CS_COMMAND_OBSERVER_REG_HELP,
	STRMSG_CS_COMMAND_WATCH_START_INFO_HELP,
	STRMSG_CS_COMMAND_WATCH_END_INFO_HELP,
	STRMSG_CS_COMMAND_ARENA_TEAM_INFO_HELP,
	STRCMD_CS_COMMAND_ARENAFORBID_HELP,			// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - /? 결과리스트에 추가
	STRCMD_CS_COMMAND_ARENAFORBIDRelease_HELP,	// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - /? 결과리스트에 추가
	STRCMD_CS_COMMAND_UsableSpeakerItem_HELP,	// 2007-08-27 by cmkwon, 추가
	STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_HELP,		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -
	STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_HELP,	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -

	STRCMD_CS_COMMAND_TEAMARENALEAVE_HELP, 		// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_TARGETARENALEAVE_HELP, 	// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_ARENAEVENT_HELP, 			// 2008-03-10 by dhjin, 아레나 통합 - 
	STRCMD_CS_COMMAND_ARENAEVENTRELEASE_HELP,	// 2008-03-10 by dhjin, 아레나 통합 -
	STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_HELP,	// 2008-08-18 by dhjin, 세력마크이벤트
	STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_HELP,	// 2008-08-18 by dhjin, 세력마크이벤트
	STRCMD_CS_COMMAND_PCBANGRELOADTIME_HELP,	// 2008-08-25 by dhjin, 태국 PC방 IP정보 로딩
	STRCMD_CS_COMMAND_KICK_HELP,				// 2008-09-09 by cmkwon, "/kick" 명령어 추가 - 

	STRCMD_CS_COMMAND_INFINITY_NEXT_SCENE,		// 2010. 06. 04 by hsLee 인티피니 필드 2차 난이도 조절. (GM 명령어 추가. /nextscene(다음 시네마 씬 호출.) )

	// IM Server
	STRCMD_CS_COMMAND_WHO_HELP,
	STRCMD_CS_COMMAND_GOUSER_HELP,
	STRCMD_CS_COMMAND_COMEON_HELP,
	STRCMD_CS_COMMAND_GUILDCOMEON_HELP,
	STRCMD_CS_COMMAND_GUILDSEND_HELP,
	STRCMD_CS_COMMAND_CHATFORBID_HELP,
	STRCMD_CS_COMMAND_CHATFORBIDRELEASE_HELP,
	STRCMD_CS_COMMAND_VIEWCALLGM_HELP,				// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_BRINGCALLGM_HELP,				// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_PrepareShutdown_HELP,			// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게) - 
	STRCMD_CS_COMMAND_STARTCALLGM_HELP,				// 2007-11-19 by cmkwon, 진정시스템 업데이트 - /? 결과에 추가
	STRCMD_CS_COMMAND_ENDCALLGM_HELP,				// 2007-11-19 by cmkwon, 진정시스템 업데이트 - /? 결과에 추가
	NULL
};

char *ArrCommandListforUser[] = {
	STRCMD_CS_COMMAND_MENT_HELP,
	STRCMD_CS_COMMAND_WHOAREYOU_HELP,
	STRCMD_CS_COMMAND_CHATPTOPFLAG_HELP,
	STRCMD_CS_COMMAND_COMMANDLIST_HELP,
	STRCMD_CS_COMMAND_CALLGM_HELP,					// 2006-05-09 by cmkwon
	STRCMD_CS_COMMAND_PLAYTIME_HELP,
	STRCMD_CS_COMMAND_SIGNBOARD_HELP,
	STRCMD_CS_COMMAND_SIGNBOARD_DEL_HELP,
	STRCMD_CS_COMMAND_INFLDITRIBUTION_HELP,			// 2006-02-08 by cmkwon
	STRCMD_CS_COMMAND_EntrancePermission_HELP,
	STRCMD_CS_COMMAND_EntrancePermissionDeny_HELP,
	STRCMD_CS_COMMAND_SUBLEADER1_HELP,			// 2007-10-06 by dhjin
	STRCMD_CS_COMMAND_SUBLEADER2_HELP,			// 2007-10-06 by dhjin

	// 2006-08-24 by cmkwon, 클라이언트에서만 사용하는 명령어 리스트
	STRCMD_C_COMMAND_CALL_HELP,
	STRCMD_C_COMMAND_PARTYCALL_HELP,
	STRCMD_C_COMMAND_PARTYCALLEND_HELP,
	STRCMD_C_COMMAND_GUILDCALL_HELP,
	STRCMD_C_COMMAND_GUILDCALLEND_HELP,
	STRCMD_C_COMMAND_CALLEND_HELP,
	STRCMD_C_COMMAND_BATTLE_HELP,
	STRCMD_C_COMMAND_SURRENDER_HELP,
	STRCMD_C_COMMAND_PARTYBATTLE_HELP,
	STRCMD_C_COMMAND_GUILDCOMBAT_HELP,
	STRCMD_C_COMMAND_GUILDSURRENDER_HELP,
	STRCMD_C_COMMAND_NAME_HELP,
	STRCMD_C_COMMAND_INITCHAT_HELP,
	STRCMD_C_COMMAND_REFUSEBATTLE_HELP,
	STRCMD_C_COMMAND_REFUSETRADE_HELP,
	
	NULL
};

#ifdef S_BONUS_KILL_SYSTEM_RENEWAL
// 2015-06-12 Future, Bonus Kill Count Reward Items
// Format:
// { ItemNum, ItemCount }
const KILL_REWARD_ITEM BonusKillSystemRewardItemNUMs[] = {
	{ 7038810 , 1 },
	{ 7901800 , 2 },
	{ 7007480 , 5 },
	{ -1, -1 }	// Keep this last element to determine the end of the array!!!
};

#endif // S_BONUS_KILL_SYSTEM_RENEWAL

CGlobalGameServer *		g_pGlobalGameServer = NULL;


///////////////////////////////////////////////////////////////////////////////
/// \fn			int GDeleteNewLine(char *str)
/// \brief
/// \author		cmkwon
/// \date		2004-03-19 ~ 2004-03-19
/// \warning
///
/// \param
/// \return
///////////////////////////////////////////////////////////////////////////////
int GDeleteNewLine(char *str)
{
	char *ret = NULL;
	int len = strlen(str);
	int outlen = 0;

	for (int i = 0; (i < len && outlen < 128); i++)
	{
		if (str[i] != '\r' && str[i] != '\n')
		{
			str[outlen++] = str[i];
		}
	}

	str[outlen] = '\0';

	return outlen;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlobalGameServer::CGlobalGameServer()
{
	if(g_pGlobalGameServer)
	{
		return;
	}
	srand( timeGetTime() );			// 2007-04-10 by cmkwon, Random Number를 위해서

	g_pGlobalGameServer = this;		// 상위 클래스 사용을 위해 전역변수에 자신을 할당한다.

	//////////////////////////////////////////////////////////////////////////
	// 윈도우 관련 멤버 변수
	m_hInstanceHandle			= NULL;
	m_hMainWndHandle			= NULL;

	//////////////////////////////////////////////////////////////////////////
	// 윈도우 타이버 관련 멤버 변수
	m_nTimerIDTraffic			= 0;
	m_nTimerIDAliveCheck		= 0;
	m_nTimerIDReconnect			= 0;

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	memset(m_szIPLocal, 0x00, SIZE_MAX_IPADDRESS);
	memset(m_szIPPreServer, 0x00, SIZE_MAX_IPADDRESS);
	m_nPortListening			= 0;
	m_nPortPreServer			= 0;

	m_pGIOCP					= NULL;

	MEMSET_ZERO(m_szODBCDSN, SIZE_MAX_ODBC_CONN_STRING);
	MEMSET_ZERO(m_szODBCUID, SIZE_MAX_ODBC_CONN_STRING);
	MEMSET_ZERO(m_szODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);

	// 2007-12-03 by cmkwon, 게임 로그 DB 따로 구축 하기 버그 수정 - 초기화
	MEMSET_ZERO(m_szLogDBODBCDSN, SIZE_MAX_ODBC_CONN_STRING);			// LogDB_ODBC_DSN
	MEMSET_ZERO(m_szLogDBODBCUID, SIZE_MAX_ODBC_CONN_STRING);			// LogDB_ODBC_UID
	MEMSET_ZERO(m_szLogDBODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);		// LogDB_ODBC_PASSWD

	m_enMGameEventType			= MGAME_EVENT_NO_EVENT;

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	memset(m_szServerGroupName, 0x00, SIZE_MAX_SERVER_NAME);

	m_vectorServerGroupForMGame.reserve(20);
	m_nMGameServerID1			= 10061;

	m_bIsTestServer				= FALSE;

	m_nLanguageType				= LANGUAGE_TYPE_UNKNOWN;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	m_bIsExternalAuthentication		= FALSE;
	MEMSET_ZERO(m_szExtAuthODBCDSN, SIZE_MAX_ODBC_CONN_STRING);
	MEMSET_ZERO(m_szExtAuthODBCUID, SIZE_MAX_ODBC_CONN_STRING);
	MEMSET_ZERO(m_szExtAuthODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	MEMSET_ZERO(m_szDBServerIP, SIZE_MAX_ODBC_CONN_STRING);
	m_nDBServerPort					= 0;
	MEMSET_ZERO(m_szDBServerDatabaseName, SIZE_MAX_ODBC_CONN_STRING);
	
	MEMSET_ZERO(m_szLogDBDBServerIP, SIZE_MAX_ODBC_CONN_STRING);
	m_nLogDBDBServerPort					= 0;
	MEMSET_ZERO(m_szLogDBDBServerDatabaseName, SIZE_MAX_ODBC_CONN_STRING);
	
	MEMSET_ZERO(m_szExtAuthDBServerIP, SIZE_MAX_ODBC_CONN_STRING);
	m_nExtAuthDBServerPort					= 0;
	MEMSET_ZERO(m_szExtAuthDBServerDatabaseName, SIZE_MAX_ODBC_CONN_STRING);

	// 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리 - 초기화
	m_IsSOAPUse = FALSE;
    // end 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리 - 초기화
}

CGlobalGameServer::~CGlobalGameServer()
{
	EndServerSocket();
	g_pGlobal = NULL;
}

void CGlobalGameServer::SetInstanceHandle(HINSTANCE i_hInstance)
{
	m_hInstanceHandle = i_hInstance;
}

HINSTANCE CGlobalGameServer::GetInstanceHandle(void)
{
	return m_hInstanceHandle;
}

void CGlobalGameServer::SetMainWndHandle(HWND i_hWnd)
{
	m_hMainWndHandle = i_hWnd;
}

HWND CGlobalGameServer::GetMainWndHandle(void)
{
	return m_hMainWndHandle;
}

UINT CGlobalGameServer::GetTimerIDTraffic(void)
{
	return m_nTimerIDTraffic;
}

UINT CGlobalGameServer::GetTimerIDAliveCheck(void)
{
	return m_nTimerIDAliveCheck;
}

UINT CGlobalGameServer::GetTimerIDReconnect(void)
{
	return m_nTimerIDReconnect;
}

void CGlobalGameServer::SetIPLocal(char *i_szIP)
{
	memcpy(m_szIPLocal, i_szIP, SIZE_MAX_IPADDRESS);
}
char *CGlobalGameServer::GetIPLocal(void)
{
	return m_szIPLocal;
}

void CGlobalGameServer::SetPublicIPLocal(char *i_szIP)
{
	memcpy(m_szPublicIPLocal, i_szIP, SIZE_MAX_IPADDRESS);
}
char *CGlobalGameServer::GetPublicIPLocal(void)
{
	return m_szPublicIPLocal;
}

void CGlobalGameServer::SetIPPreServer(char *i_szIP)
{
	memcpy(m_szIPPreServer, i_szIP, SIZE_MAX_IPADDRESS);
}
char *CGlobalGameServer::GetIPPreServer(void)
{
	return m_szIPPreServer;
}

void CGlobalGameServer::SetPortListening(int i_nPort)
{
	m_nPortListening = i_nPort;
}
int CGlobalGameServer::GetPortListening(void)
{
	return m_nPortListening;
}

void CGlobalGameServer::SetPortPreServer(int i_nPort)
{
	m_nPortPreServer = i_nPort;
}
int CGlobalGameServer::GetPortPreServer(void)
{
	return m_nPortPreServer;
}

CIOCP *CGlobalGameServer::GetGIOCP(void)
{
	return m_pGIOCP;
}

CSystemLogManager * CGlobalGameServer::GetPtrFieldGameLogManager(void)
{
	return &m_FieldGameLogManager;
}

void CGlobalGameServer::SetMGameEventType(MGAME_EVENT_t i_enEventType)
{
	m_enMGameEventType = i_enEventType;
}

// start 2011-12-12 by hskim, GLog 2차
char *CGlobalGameServer::GetServiceCountry(void)
{
	static char szCountry[SIZE_MAX_SERVICE_COUNTRY] = { 0, };

	STRNCPY_MEMSET(szCountry, SERVICE_COUNTRY_STRING, SIZE_MAX_SERVICE_COUNTRY);
	
	return szCountry;
}
// end 2011-12-12 by hskim, GLog 2차

void CGlobalGameServer::SetServerGroupName(char *i_szServerGroupName)
{
	STRNCPY_MEMSET(m_szServerGroupName, i_szServerGroupName, SIZE_MAX_SERVER_NAME);
}

char *CGlobalGameServer::GetServerGroupName(void)
{
	return m_szServerGroupName;
}

BOOL CGlobalGameServer::IsTestServer(void)
{
	return m_bIsTestServer;
}

BOOL CGlobalGameServer::InMasangServer(void)
{
	return TRUE;
}

BOOL CGlobalGameServer::InitServerSocket(void)
{
	return FALSE;
}

BOOL CGlobalGameServer::EndServerSocket(void)
{
	char szSystemLog[512];
	sprintf(szSystemLog, "CGlobalGameServer::EndServerSocket_1\r\n");
	DBGOUT(szSystemLog);
	this->WriteSystemLog(szSystemLog);

	if(m_hMainWndHandle)
	{
		if(m_nTimerIDTraffic)
		{
			KillTimer(m_hMainWndHandle, TIMERID_TRAFFIC);
			m_nTimerIDTraffic = 0;
		}
		if(m_nTimerIDAliveCheck)
		{
			KillTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK);
			m_nTimerIDAliveCheck = 0;
		}
		if(m_nTimerIDReconnect)
		{
			KillTimer(m_hMainWndHandle, TIMERID_RECONNECT);
			m_nTimerIDReconnect = 0;
		}
	}

	if(m_pGIOCP && m_pGIOCP->GetListeningFlag())
	{
		m_pGIOCP->IOCPClean();
	}
	SAFE_DELETE(m_pGIOCP);
	return FALSE;
}

void CGlobalGameServer::PrintPreprocessorDefinitions(void)
{
DBGOUT("\n=== PREPROCESSOR DEFINITIONS ===========\n");

#ifdef _DEBUG
	DBGOUT("    _DEBUG defined\n");
#else // _DEBUG
	DBGOUT("    _DEBUG NOT defined\n");
#endif // _DEBUG

#ifdef _ATUM_DEVELOP
	DBGOUT("    _ATUM_DEVELOP defined\n");
#else // _ATUM_DEVELOP
	DBGOUT("    _ATUM_DEVELOP NOT defined\n");
#endif // _ATUM_DEVELOP

#ifdef _ATUM_CLIENT
	DBGOUT("    _ATUM_CLIENT defined\n");
#else // _ATUM_CLIENT
	DBGOUT("    _ATUM_CLIENT NOT defined\n");
#endif // _ATUM_CLIENT

#ifdef _ATUM_FIELD_SERVER
	DBGOUT("    _ATUM_FIELD_SERVER defined\n");
#else // _ATUM_FIELD_SERVER
	DBGOUT("    _ATUM_FIELD_SERVER NOT defined\n");
#endif // _ATUM_FIELD_SERVER

#ifdef _ATUM_VERSION_DIFF
	DBGOUT("    _ATUM_VERSION_DIFF defined\n");
	MessageBox(NULL, "_ATUM_VERSION_DIFF defined!", "check", MB_OK);
#else // _ATUM_VERSION_DIFF
	DBGOUT("    _ATUM_VERSION_DIFF NOT defined\n");
#endif // _ATUM_VERSION_DIFF

DBGOUT("========================================\n\n");
}

void CGlobalGameServer::WndRegisterClass(HINSTANCE hInstance, UINT IDI_Icon, char *szClass)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Icon));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= _T(szClass);
	wcex.hIconSm		= NULL;

	::RegisterClassEx(&wcex);
}

BOOL CGlobalGameServer::InitInstance(HINSTANCE hInstance, int nCmdShow, char *szClass, char *szWindowText)
{
   HWND hWnd;

   g_pGlobalGameServer->SetInstanceHandle(hInstance); // Store instance handle in our global variable
   hWnd = CreateWindow(_T(szClass), _T(szWindowText), WS_OVERLAPPEDWINDOW,
      350, 150, 400, 120, NULL, NULL, hInstance, NULL);		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행

   if (!hWnd)
   {
      return FALSE;
   }

   g_pGlobalGameServer->SetMainWndHandle(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // Preprocessor Definitions를 출력, 확인 위해
   PrintPreprocessorDefinitions();

   return TRUE;
}

void CGlobalGameServer::SetConfigRootPath(void)
{
	// Default Path를 미리 설정해둠
	STRNCPY_MEMSET(CONFIG_ROOT, "./", 1024);

	const int SIZE_BUFF = 1024;
	char		buff[SIZE_BUFF];
	const char	*token;
	char		seps[] = "\r\n";
	ifstream	fin;

	fin.open("./config_root.cfg");
	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		char	szDir[1024];
		GetCurrentDirectory(1024, szDir);
		sprintf(szError, "\r\n[WARNING] Cannot find 'config_root.cfg': set default path '%s', Current Directory'%s'\r\n\r\n", CONFIG_ROOT, szDir);
		DBGOUT(szError);
		return;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		token = strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		STRNCPY_MEMSET(CONFIG_ROOT, token, 128);
	}

	if('\\' != CONFIG_ROOT[strlen(CONFIG_ROOT)-1]
		&& '/' != CONFIG_ROOT[strlen(CONFIG_ROOT)-1])
	{
		strcat(CONFIG_ROOT, "/");
	}

	DBGOUT("\r\nSET CONFIG ROOT PATH '%s'\r\n\r\n", CONFIG_ROOT);
}


//BOOL CGlobalGameServer::WriteGameLog(USHORT msgType, MSG_FL_LOG* log, char* szAction, ofstream* file)
//{
//	if(m_FieldGameLogManager.GetFileHandle() == NULL)
//	{
//		return FALSE;
//	}
//	char szLogLineBuffer[SIZE_MAX_A_LOG_LINE];
//	char CurrTime[128], ConnTime[128];
//	struct tm *today;
//	DWORD	dwFileSize = 0;
//
//	today = localtime( &log->CurrentTime );
//	if (today == 0) { return FALSE; }
//	strftime(CurrTime, 128, "%Y-%m-%d %H:%M:%S", today );
//
//	today = localtime( &log->ConnectTime );
//	if (today == 0) { return FALSE; }
//	strftime(ConnTime, 128, "%Y-%m-%d %H:%M:%S", today );
//
//	sprintf(szLogLineBuffer, "[%s|%s|%s|%s|%s|%s|%d|%d|%d|%d|%d|%s]\r\n",
//			GetSZLogType(msgType),
//			CurrTime,
//			ConnTime,
//			log->ClientIP,
//			log->AccountName,
//			log->CharacterName,
//			log->MapChannelIndex.MapIndex,
//			log->MapChannelIndex.ChannelIndex,
//			(int)log->PosVector.x,
//			(int)log->PosVector.y,
//			(int)log->PosVector.z,
//			szAction
//			);
//
//	return m_FieldGameLogManager.WriteSystemLog(szLogLineBuffer, FALSE);
//}

const char* CGlobalGameServer::GetSZLogType(USHORT type)
{
//	switch(type)
//	{
//	case T_FL_LOG_ITEM:
//		return "ITEM";
//	case T_FL_LOG_SKILL:
//		return "SKILL";
//	case T_FL_LOG_LEVEL:
//		return "LEVEL";
//	case T_FL_LOG_EXP:
//		return "EXP";
//	case T_FL_LOG_STAT:
//		return "STAT";
//	case T_FL_LOG_GAME_END:
//		return "GAME_END";
//	case T_FL_LOG_GAME_START:
//		return "GAME_START";
//	case T_FL_LOG_SERVER_INFO_MAP:
//		return "CONNECTION_INFO_MAP";
//	case T_FL_LOG_SERVER_INFO_TOTAL:
//		return "CONNECTION_INFO_TOTAL";
//	default:
//		DbgOut("Unknown Type[0x%X]\r\n", type);
//		return "UNKNOWN";
//	}
	return NULL;
}

BOOL CGlobalGameServer::StartTimerTraffic(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDTraffic)
	{
		return FALSE;
	}

	m_nTimerIDTraffic = ::SetTimer(m_hMainWndHandle, TIMERID_TRAFFIC, TIMERGAP_TRAFFIC, NULL);
	return TRUE;
}

void CGlobalGameServer::EndTimerTraffic(void)
{
	if(m_nTimerIDTraffic && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_TRAFFIC);
	}
	m_nTimerIDTraffic = 0;
}

BOOL CGlobalGameServer::StartTimerAliveCheck(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDAliveCheck)
	{
		return FALSE;
	}

	m_nTimerIDAliveCheck = ::SetTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK, TIMERGAP_ALIVE_CHECK, NULL);
	return TRUE;
}
void CGlobalGameServer::EndTimerAliveCheck(void)
{
	if(m_nTimerIDAliveCheck && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK);
	}
	m_nTimerIDAliveCheck = 0;
}


BOOL CGlobalGameServer::StartTimerReconnect(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDReconnect)
	{
		return FALSE;
	}

	m_nTimerIDReconnect = ::SetTimer(m_hMainWndHandle, TIMERID_RECONNECT, TIMERGAP_RECONNECT, NULL);
	return TRUE;
}
void CGlobalGameServer::EndTimerReconnect(void)
{
	if(m_nTimerIDReconnect && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_RECONNECT);
	}
	m_nTimerIDReconnect = 0;
}

const char* GetProtocolTypeString (MessageType_t msgType);

void CGlobalGameServer::CheckServerThread(void)
{
	if(NULL == m_pGIOCP){		return;}

	DWORD dwExcludeThreadID = 0;	
	SThreadInfo *pSTInfo = m_pGIOCP->CheckIOCPThread(0);
	while (pSTInfo)
	{
		pSTInfo->bPrintOut = TRUE;
		char szSysLog[1024];
		switch(pSTInfo->enThreadCheckType)
		{
		case THREAD_CHECK_TYPE_DB_WORKER:
			{
				sprintf(szSysLog, "[ERROR] deadlock : ThreadType(%10s), ThreadId(%5d), TimeGap(%5d) StartTick(%10d) DBThreadIndex(%4d) QueryType[%20s(%#04x)] QueryArraySize(%4d) CharacterUID(%6d), Param1(%2d) Param2(%d)\r\n"
					, pSTInfo->GetThreadCheckComment(pSTInfo->enThreadCheckType), pSTInfo->dwThreadId
					, timeGetTime() - pSTInfo->dwLastUseStartTick
					, pSTInfo->dwLastUseStartTick
					, pSTInfo->dwSocketIndex
					, GetDBQueryTypeString((EnumQueryType)pSTInfo->dwMessageType)
					, pSTInfo->dwMessageType
					, pSTInfo->nQueryArraySize
					, pSTInfo->dwCharacterUID
					, pSTInfo->nParam1, pSTInfo->nParam2);		// 2007-02-21 by cmkwon
			}
			break;
		default:
			{
				sprintf(szSysLog, "[ERROR] deadlock : ThreadType(%10s), ThreadId(%5d), TimeGap(%5d) StartTick(%10d) SocketIndex(%4d) MessageType[%20s(%#04x)] Parma1(%2d) Param2(%d)\r\n"
					, pSTInfo->GetThreadCheckComment(pSTInfo->enThreadCheckType), pSTInfo->dwThreadId
					, timeGetTime() - pSTInfo->dwLastUseStartTick
					, pSTInfo->dwLastUseStartTick
					, pSTInfo->dwSocketIndex
					, GetProtocolTypeString(pSTInfo->dwMessageType)
					, pSTInfo->dwMessageType
					, pSTInfo->nParam1, pSTInfo->nParam2);		// 2007-02-21 by cmkwon
			}
		}		
		CGlobal::ms_SystemLogManager.WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);

		pSTInfo = m_pGIOCP->CheckIOCPThread(pSTInfo->dwThreadId);
	}
}

void CGlobalGameServer::CalculateIOCPTraffic(void)
{
	if(NULL == m_pGIOCP
		|| m_pGIOCP->GetListeningFlag() == FALSE)
	{
		return;
	}

	m_pGIOCP->CalcTotalTrafficInfo();					// Bandwidth 계산
}

void CGlobalGameServer::CheckClientAlive(void)
{
	if(NULL == m_pGIOCP
		|| m_pGIOCP->GetListeningFlag() == FALSE){	return;}

	m_pGIOCP->ClientCheck();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			USHORT CGlobalGameServer::AuthAdminToolUser(const char *i_pUID, const char *i_pPWD)
/// \brief		
///				// 2006-04-15 by cmkwon, 리턴값의 자료형 변경 - BOOL ==> USHORT
/// \author		cmkwon
/// \date		2006-04-15 ~ 2006-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
USHORT CGlobalGameServer::AuthAdminToolUser(const char *i_pUID, const char *i_pPWD)
{
	CODBCStatement ODBCStmt;
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//ODBCStmt.Init((char*)GetODBCDSN(), (char*)GetODBCUID(), (char*)GetODBCPASSWORD());
	BOOL bRet = ODBCStmt.Init(this->GetDBServerIP(), this->GetDBServerPort(), this->GetDBServerDatabaseName(), (char*)this->GetODBCUID(), (char*)this->GetODBCPASSWORD(), this->GetMainWndHandle());
	if(FALSE == bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CGlobalGameServer::AuthAdminToolUser_) !! %s,%d %s %s %s\r\n"
			, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), g_pGlobalGameServer->GetDBServerDatabaseName(), (CHAR*)g_pGlobalGameServer->GetODBCUID(), (CHAR*)g_pGlobalGameServer->GetODBCPASSWORD());
		return 0;
	}

	char szPWDToEncode[SIZE_MAX_PASSWORD];
	STRNCPY_MEMSET(szPWDToEncode, i_pPWD, SIZE_MAX_PASSWORD);

	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];
	USHORT	usRetRace = 0;			// 2006-04-15 by cmkwon

	MD5_instance.MD5Encode((char*)szPWDToEncode, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);

//	char szSQLQuery[512];
// 2006-04-15 by cmkwon, 아래와 같이 수정함
// 	sprintf(szSQLQuery, "select accountname from td_account\
// 							where accountname like \'%s\' and (password like \'%s\' OR password like \'%s\')\
// 							and (accounttype & %d = %d OR accounttype & %d = %d)"
// 							, i_pUID, i_pPWD, szEncodedString, (INT)RACE_OPERATION, (INT)RACE_OPERATION, (INT)RACE_GAMEMASTER, (INT)RACE_GAMEMASTER);

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szSQLQuery, "select accounttype from td_account WITH (NOLOCK)\
// 							where accountname like \'%s\' and (password like \'%s\' OR password like \'%s\')"
// 							, i_pUID, i_pPWD, szEncodedString);
 
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szSQLQuery, QUERY_080702_0074, i_pUID, i_pPWD, szEncodedString);
//
//	BOOL bRet = ODBCStmt.ExecuteQuery(szSQLQuery);
	SQLHSTMT hstmt = ODBCStmt.GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, (LPSTR)i_pUID, 0,					&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)i_pPWD, 0,				&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)szEncodedString, 0,	&arrCB2[3]);
	bRet = ODBCStmt.ExecuteQuery((char*)(PROCEDURE_080827_0074));
	if (!bRet)
	{
		ODBCStmt.FreeStatement();
		ODBCStmt.Clean();
		return usRetRace;
	}

	SQLINTEGER arrCB = SQL_NTS;
	SQLBindCol(ODBCStmt.m_hstmt, 1, SQL_C_SHORT, &usRetRace, 0,		&arrCB);	
	while ( (bRet = SQLFetch(ODBCStmt.m_hstmt)) != SQL_NO_DATA)
	{
		/*if(FALSE == COMPARE_RACE(usRetRace, RACE_OPERATION|RACE_GAMEMASTER))*/
		if(FALSE == COMPARE_RACE(usRetRace, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR))
		{// 2006-04-15 by cmkwon, 관리자/GM이 아니면 0으로 초기화 한다.
			usRetRace = 0;
		}
	}
	ODBCStmt.FreeStatement();
	ODBCStmt.Clean();

	return usRetRace;
}

BOOL CGlobalGameServer::LoadStringList(ezsetString *o_psetStirng
									   , const char *i_szFilePath
									   , BOOL i_bPrintFlag/*=FALSE*/)
{
	const int SIZE_BUFF = 512;
	char buff[SIZE_BUFF];

	ifstream fin;
	fin.open(i_szFilePath);

	if (! fin.is_open())
	{
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE && strcmp(buff, "") == 0)
		{
			break;
		}

		if (buff == NULL
			|| strncmp(buff, "#", 1) == 0
			|| strncmp(buff, "$", 1) == 0
			|| 0 == strcmp(buff, ""))
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		_strlwr(buff);
		o_psetStirng->insertEZ(buff);
		if(i_bPrintFlag)
		{			
			DBGOUT("	%s\r\n", buff);
		}
	}
	fin.close();

	return TRUE;
}

BOOL CGlobalGameServer::LoadAbuseAndProhibitedName(void)
{
	///////////////////////////////////////////////////////////////////////////
	// 욕설 로딩
	///////////////////////////////////////////////////////////////////////////
	if(FALSE == LoadStringList(&m_setAbuseWords, (string(CONFIG_ROOT)+string("./AFList.txt")).c_str()))
	{
// 2004-12-14 by cmkwon 
//		MessageBox(NULL, "욕설 로딩 실패", "Error", MB_OK);
	}

	///////////////////////////////////////////////////////////////////////////
	// 금지 계정 로딩
	///////////////////////////////////////////////////////////////////////////
	if(FALSE == LoadStringList(&m_setProhibitedNames, (string(CONFIG_ROOT)+string("./prohibited_names.cfg")).c_str(), TRUE))
	{
// 2004-12-14 by cmkwon
//		MessageBox(NULL, "금지 계정 로딩 실패", "Error", MB_OK);
	}

	return TRUE;
}

BOOL CGlobalGameServer::LoadAccountNameAndCharacterNameForMGameEvent(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// AccountName 로딩
	if(FALSE == LoadStringList(&m_setAccountNameForMGameEvent, (string(CONFIG_ROOT)+string("./AccountName_for_MGameEvent.cfg")).c_str()))
	{
// 2004-12-14 by cmkwon
//		MessageBox(NULL, "MGameEvent를 위한 AccountName 로딩 실패", "Error", MB_OK);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// AccountName 로딩
	if(FALSE == LoadStringList(&m_setCharacterNameForMGameEvent, (string(CONFIG_ROOT)+string("./CharacterName_for_MGameEvent.cfg")).c_str()))
	{
// 2004-12-14 by cmkwon
//		MessageBox(NULL, "MGameEvent를 위한 CharacterName 로딩 실패", "Error", MB_OK);
	}
	return TRUE;
}

BOOL CGlobalGameServer::IsExistAccountNameForMGameEvent(char *i_szAccountName)
{
	char szBuff[SIZE_MAX_ACCOUNT_NAME];
	STRNCPY_MEMSET(szBuff, i_szAccountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(szBuff);

	return m_setAccountNameForMGameEvent.find(szBuff) != m_setAccountNameForMGameEvent.end();
}

BOOL CGlobalGameServer::CheckProhibitedName(const char *i_szName)
{
	char buff[SIZE_MAX_CHARACTER_NAME];
	STRNCPY_MEMSET(buff, i_szName, SIZE_MAX_CHARACTER_NAME);
	_strlwr(buff);

	if (m_setProhibitedNames.find(buff) != m_setProhibitedNames.end())
	{
		return FALSE;
	}

	ez_set<string>::iterator itr = m_setAbuseWords.begin();
	while (m_setAbuseWords.end() != itr)
	{
		const char *pAbuseWord = (*itr).c_str();

		if (0 < strlen(pAbuseWord)
			&& strstr(i_szName, pAbuseWord) != NULL)
		{
			return FALSE;
		}

		itr++;
	}

	return TRUE;
}


BOOL CGlobalGameServer::InsertServerGroupforMGame(char *i_szServerGroupName, int i_nServerID)
{
	int nSize = m_vectorServerGroupForMGame.size();
	for(int i=0; i < nSize; i++)
	{
		if(0 == stricmp(m_vectorServerGroupForMGame[i].szServerGroupName, i_szServerGroupName))
		{			
			return FALSE;
		}
	}

	SServerGroupForMame tmGroup;
	STRNCPY_MEMSET(tmGroup.szServerGroupName, i_szServerGroupName, SIZE_MAX_SERVER_NAME);
	tmGroup.nServerIDforMGame	= i_nServerID;

	m_vectorServerGroupForMGame.push_back(tmGroup);
	return TRUE;
}

int CGlobalGameServer::GetServerIDforMGame(char *i_szServerGroupName)
{
	int nSize = m_vectorServerGroupForMGame.size();
	for(int i=0; i < nSize; i++)
	{
		if(0 == stricmp(m_vectorServerGroupForMGame[i].szServerGroupName, i_szServerGroupName))
		{
			return m_vectorServerGroupForMGame[i].nServerIDforMGame;
		}
	}

	return 10061;
}

char *CGlobalGameServer::GetServerGroupNameByMGameServerID(int i_nMServerID)
{
	int nSize = m_vectorServerGroupForMGame.size();
	for(int i=0; i < nSize; i++)
	{
		if(i_nMServerID == m_vectorServerGroupForMGame[i].nServerIDforMGame)
		{
			return m_vectorServerGroupForMGame[i].szServerGroupName;
		}
	}

	return "UNKNOWN";
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetDBServerIP(void)
{
	return m_szDBServerIP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGlobalGameServer::GetDBServerPort(void)
{
	return m_nDBServerPort;		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetDBServerDatabaseName(void)
{
	return m_szDBServerDatabaseName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetLogDBDBServerIP(void)
{
	return m_szLogDBDBServerIP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGlobalGameServer::GetLogDBDBServerPort(void)
{
	return m_nLogDBDBServerPort;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetLogDBDBServerDatabaseName(void)
{
	return m_szLogDBDBServerDatabaseName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetExtAuthDBServerIP(void)
{
	return m_szExtAuthDBServerIP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGlobalGameServer::GetExtAuthDBServerPort(void)
{
	return m_nExtAuthDBServerPort;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
/// \author		cmkwon
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CGlobalGameServer::GetExtAuthDBServerDatabaseName(void)
{
	return m_szExtAuthDBServerDatabaseName;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGlobalGameServer::AddAllowedToolIP(char *i_szAllowedToolIP)
/// \brief		// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
/// \author		cmkwon
/// \date		2008-06-05 ~ 2008-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGlobalGameServer::AddAllowedToolIP(const char *i_szAllowedToolIP)
{
	if(NULL == i_szAllowedToolIP
		|| 0 == strcmp("", i_szAllowedToolIP))
	{// 2008-06-05 by cmkwon, 유효하지 않은 IP
		return;
	}

	this->m_AllowedToolIPList.push_back(i_szAllowedToolIP);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGlobalGameServer::CheckAllowedToolIP(char *i_szToolIP)
/// \brief		// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
/// \author		cmkwon
/// \date		2008-06-05 ~ 2008-06-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGlobalGameServer::CheckAllowedToolIP(char *i_szToolIP)
{
	if(IS_SCADMINTOOL_CONNECTABLE_IP(i_szToolIP))
	{
		g_pGlobalGameServer->WriteSystemLogEX(TRUE, "  [Notify] CheckAllowedToolIP ==> Allowed (%s)\r\n", i_szToolIP);
		return TRUE;
	}

	for (int i = 0; i < m_AllowedToolIPList.size(); i++)
	{
		if (0 == strncmp(m_AllowedToolIPList[i].c_str(), i_szToolIP, strlen(m_AllowedToolIPList[i].c_str())))
		{		
			g_pGlobalGameServer->WriteSystemLogEX(TRUE, "  [Notify] CheckAllowedToolIP ==> Allowed (%s)\r\n", i_szToolIP);
			return TRUE;
		}
	}

	g_pGlobalGameServer->WriteSystemLogEX(TRUE, "  [Notify] CheckAllowedToolIP ==> Not Allowed (%s) !!\r\n", i_szToolIP);
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGlobalGameServer::CheckAllowedToolIP(char *i_szToolIP)
/// \brief		// 2010-11-29 by shcho, 아르헨티나 외부인증 변경 SOAP 처리 - grobal.cfg 정보 가져오기
/// \author		shcho
/// \date		2010-11-29 ~ 2010-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

BOOL CGlobalGameServer::GetIsExtAuthUseSOAP(void) // SOAP 사용 여부 FALSE이면 사용 안함
{
	return m_IsSOAPUse;
}
char *CGlobalGameServer::GetExtAuthSOAPUrlString(void) // SOAP URL 스트링
{
	return m_szSOAPUrlString;
}
char *CGlobalGameServer::GetExtAuthSOAPGameID(void) // 사용할 게임 ID
{
	return m_szSOAPGameID;
}