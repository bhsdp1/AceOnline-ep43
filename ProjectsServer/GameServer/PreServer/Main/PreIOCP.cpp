// PreIOCP.cpp: implementation of the CPreIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpManager.h"
#include "PreIOCP.h"
#include "PreIOCPSocket.h"
#include "Wininet.h"
#include "config.h"
#include "PreGlobal.h"
#include "AtumError.h"
#include "AccountBlockManager.h"
#include "SecurityManager.h"					// 2011-06-22 by hskim, 사설 서버 방지
#include "ShutdownTarget.h"				// 2012-07-11 by hskim, 선택적 셧다운
#include "SelectiveShutdown.h"			// 2012-07-11 by hskim, 선택적 셧다운

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAccountInfo::SetBirthday(int i_nBirthday)
/// \brief		// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 함수 추가
/// \author		cmkwon
/// \date		2007-06-28 ~ 2007-06-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAccountInfo::SetBirthday(int i_nBirthday)
{
	if(0 >= i_nBirthday)
	{
		return;
	}

	switch(g_pPreGlobal->GetLanguageType())
	{
	case LANGUAGE_TYPE_DEFAULT:		// 2011-11-03 by shcho, yedang 셧다운제 구현 - 구분을 위해 필요함
		{
			int nMonth  = 0;
			int nDay	= 0;
			// int nYear	= 0;
			int nYear	= i_nBirthday;

			// START 2011-11-03 by shcho, yedang 셧다운제 구현 - 4자리도 판별 할 수 있게 한다.
			if(i_nBirthday>=10000) // 4자리이상 이면
			{
				nYear	= max(1900, i_nBirthday/10000);
				nMonth	= max(1, (i_nBirthday/100)%100);
				nDay	= max(1, i_nBirthday%100);	
			}
			// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 4자리도 판별 할 수 있게 한다.

			atBirthday.SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
		}
		break;
	default:
		{
			atBirthday.SetDateTime(i_nBirthday, 1, 1, 0, 0, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreIOCP::CPreIOCP(int nPort, char *szLocalIP)
:CIOCP(0, SIZE_MAX_PRESERVER_SESSION, nPort, szLocalIP, ST_PRE_SERVER)
{
	CPreIOCPSocket::ms_pPreIOCP		= this;
	
	CPreIOCPSocket * pPreIOCPSocket = new CPreIOCPSocket[m_dwArrayClientSize];
	m_pArrayIOCPSocket = pPreIOCPSocket;
	for(int i = 0; i < m_dwArrayClientSize; i++)
	{
		m_ArrayClient[i] = &pPreIOCPSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}

	m_pAtumDBManager		= NULL;
	m_pPreTickManager		= NULL;
	m_bPauseService			= FALSE;
	m_pAccountBlockManager	= NULL;

	m_AllServerGroupVector.clear();
	m_OpeningMovieStandard.Reset();		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
}

CPreIOCP::~CPreIOCP()
{
	ListenerClose();
	IOCPClean();
	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CPreIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}

	m_AccountInfoMap.lock();
	mtmapAccountInfoPtr::iterator itrAccountInfo = m_AccountInfoMap.begin();
	for (; itrAccountInfo != m_AccountInfoMap.end(); itrAccountInfo++)
	{
		SAFE_DELETE(itrAccountInfo->second);
	}
	m_AccountInfoMap.clear();
	m_AccountInfoMap.unlock();

	// 2004-10-19 by cmkwon
	//m_mapVersionOld2New.clear();
	m_mtmapVersionOld2New.lock();
	m_mtmapVersionOld2New.clear();
	m_mtmapVersionOld2New.unlock();

	vector<CServerGroup*>::iterator itrServerGroup = m_AllServerGroupVector.begin();
	for (; itrServerGroup != m_AllServerGroupVector.end(); itrServerGroup++)
	{
		SAFE_DELETE(*itrServerGroup);
	}
	m_AllServerGroupVector.clear();

	SAFE_DELETE(m_pAccountBlockManager);
	SAFE_DELETE(m_pPreTickManager);
	SAFE_DELETE(m_pAtumDBManager);
}

BOOL CPreIOCP::IOCPInit(void)
{
/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	char szServerName[SIZE_MAX_STRING_128] = { 0, };
	sprintf(szServerName, "PreServer");		// 프리서버는 하나만 실행이 된다
	if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
		return FALSE;
	}
	// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 10000 \r\n");

	// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가
	if(g_pPreGlobal->GetOnlyChoiceServer())
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "====> [OnlyChoiceServer]\r\n");
		// 다른것은 처리 할 필요 없다
		return CIOCP::IOCPInit();
	}


#ifdef S_AUTO_UPDATE_VERSION_BY_PRESVR_HSSON
	// PreServer 시작 시 자동 버젼 업데이트 갱신
	this->AutoUpdateVersionByPreSvr();
#else
	// PreServer 시작 시 자동 버젼 업데이트 갱신, .cfg 파일에서 로딩하는 부분 주석 처리
	DBGOUT("\r\nGetting Update Versions from Local File...\r\n");
	// get version history
	if (CreateVersionInfoFromLocalFile() == FALSE)
	{
		return FALSE;
	}
	WriteVersionInfoToLocalFile();
#endif


	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 11000 updated version files !\r\n");

	// set current client version
	// 2004-10-19 by cmkwon
//	if (!m_mapVersionOld2New.empty())
//	{
//		m_LatestClientVersion = m_mapVersionOld2New.rbegin()->second;
//	}
	m_mtmapVersionOld2New.lock();
	if (!m_mtmapVersionOld2New.empty())
	{
		m_LatestClientVersion = m_mtmapVersionOld2New.rbegin()->second;
	}
	m_mtmapVersionOld2New.unlock();

#ifdef _DEBUG
	if (g_pPreGlobal->m_bIgnoreClientVersionUpdate)
	{
		DBGOUT("!!! IGNORING CLIENT VERSION UPDATE !!!\r\n\r\n");
	}
#endif

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 12000 arranged version list !\r\n");


#ifdef S_TEMP_PRESERVER_MAKE_HSKIM
#else // S_TEMP_PRESERVER_MAKE_HSKIM

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon
	CODBCStatement odbcStmt;

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//	BOOL bRet = odbcStmt.Init((LPCSTR)g_pGlobalGameServer->GetODBCDSN(), (LPCSTR)g_pGlobalGameServer->GetODBCUID(), (LPCSTR)g_pGlobalGameServer->GetODBCPASSWORD());
	BOOL bRet = odbcStmt.Init(g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), g_pGlobalGameServer->GetDBServerDatabaseName(), (char*)g_pGlobalGameServer->GetODBCUID(), (char*)g_pGlobalGameServer->GetODBCPASSWORD(), g_pGlobalGameServer->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CPreIOCP::IOCPInit_) !! %s,%d %s : Wrong DB Password \r\n"		// 2012-09-24 by jhseol, 소스정리 - 주석 정리 ID / PW 미출력
			, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), g_pGlobalGameServer->GetDBServerDatabaseName());	// 2012-09-24 by jhseol, 소스정리 - 주석 정리 ID / PW 미출력

		MessageBox(NULL, "DB connect CPreIOCP::IOCPInit()", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 13000 connected to db server !\r\n");

	// 2007-01-29 by cmkwon, DB Table 초기화 관련 처리
	bRet = this->InitDBTable(&odbcStmt);
	if (!bRet)
	{
		MessageBox(NULL, "InitDBTable()", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 14000 called CPreIOCP::InitDBTable# \r\n");


	// Init DBManager
	m_pAtumDBManager = new CAtumPreDBManager(this);
	if(m_pAtumDBManager->Init() == FALSE)
	{
		MessageBox(NULL, "m_pAtumDBManager initialize fail", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 15000 CAtumPreDBManager::Init# !\r\n");

	// Init TickManager
	m_pPreTickManager = new CPreTickManager(this);
	if (m_pPreTickManager->InitTickManager() == FALSE)
	{
		MessageBox(NULL, "m_pPreTickManager initialize fail", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 16000 CPreTickManager::InitTickManager# !\r\n");

	m_pAccountBlockManager = new CAccountBlockManager(m_pAtumDBManager);

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 17000 CAccountBlockManager::CAccountBlockManager# !\r\n");

	// 막힌 계정 로딩
	LoadBlockedAccounts();

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 18000 CPreIOCP::LoadBlockedAccounts# !\r\n");

	// server group 정보 로딩
	vector<CServerGroup*>::iterator itr = g_pPreGlobal->m_AllServerGroupVectorForLoading.begin();
	while (g_pPreGlobal->m_AllServerGroupVectorForLoading.end() != itr)
	{
		m_AllServerGroupVector.push_back(*itr);
		itr++;
	}
	// 다 로딩한 후에는 바로 삭제함
	g_pPreGlobal->m_AllServerGroupVectorForLoading.clear();

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 19000 arranged ServerGroupList !\r\n");


	// start 2011-12-21 by hskim, EP4 [동영상 1회 재생]
#ifdef S_EP4_HSKIM		// ON/OFF 기능 구현
	bRet = this->LoadSystemEvent(&odbcStmt);
	if( !bRet )
	{
		MessageBox(NULL, "LoadSystemEvent()", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}
#endif
	// end 2011-12-21 by hskim, EP4 [동영상 1회 재생]

	odbcStmt.Clean();

	// 2012-07-11 by hskim, 선택적 셧다운
	if( NULL != g_pPreGlobal->m_pSelectiveShutdown )
	{
		g_pPreGlobal->m_pSelectiveShutdown->SetPreIOCP(this);
	}
	// end 2012-07-11 by hskim, 선택적 셧다운

#endif // S_TEMP_PRESERVER_MAKE_HSKIM

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	g_pPreGlobal->WriteSystemLogEX(TRUE, "  [Notify] CPreIOCP::IOCPInit# 20000 end !\r\n");

	return CIOCP::IOCPInit();
}

void CPreIOCP::IOCPClean(void)
{
	CIOCP::IOCPClean();

	if (m_pAtumDBManager)
	{
		m_pAtumDBManager->Clean();
		SAFE_DELETE(m_pAtumDBManager);
	}

	if (m_pPreTickManager != NULL)
	{
		m_pPreTickManager->CleanTickManager();
		SAFE_DELETE(m_pPreTickManager);
	}
}

SThreadInfo *CPreIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);

	// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가 - NULL 체크 추가
	if(NULL == pTInfo
		&& m_pAtumDBManager)
	{
		pTInfo = m_pAtumDBManager->CheckDBThread(i_dwThreadIdToExclude);
	}
	if(pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
		BYTE SendBuf[256];
		INIT_MSG(MSG_PM_SERVER_THREAD_STATUS, T_PM_SERVER_THREAD_STATUS, pSend, SendBuf);
		pSend->dwThreadId = pTInfo->dwThreadId;
		STRNCPY_MEMSET(pSend->szThreadComment, pTInfo->GetThreadCheckComment(pTInfo->enThreadCheckType), SIZE_MAX_THREAD_COMMENT);
		pSend->dwTimeGap = timeGetTime() - pTInfo->dwLastUseStartTick;
		SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_PM_SERVER_THREAD_STATUS));

		INIT_MSG(MSG_PA_ADMIN_CHANGE_SERVER_STATE, T_PA_ADMIN_CHANGE_SERVER_STATE, msgChangeSS, SendBuf);
		msgChangeSS->ServerID = SERVER_ID(m_szLocalIPAddress, m_nListenerPort);
		msgChangeSS->ServerType = ST_PRE_SERVER;
		msgChangeSS->ServerState = SERVER_STATE_ABNORMAL;
		SendMessageToAdminTool(SendBuf, MSG_SIZE(MSG_PA_ADMIN_CHANGE_SERVER_STATE));		// 2006-05-18 by cmkwon, 버그 수정
	}
	return pTInfo;
}

#define COMMAND_LINE_BUFF 512

BOOL CPreIOCP::CreateVersionInfoFromLocalFile()
{
	// reset version list
	// 2004-10-19 by cmkwon
	//m_mapVersionOld2New.clear();
	m_mtmapVersionOld2New.lock();
	m_mtmapVersionOld2New.clear();

	// set config root path
	g_pPreGlobal->SetConfigRootPath();

	string versionFileName = string(CONFIG_ROOT) + string("versions.cfg");

	ifstream	fin;
	char		buff[COMMAND_LINE_BUFF];
	const char	*token;
	char		seps[] = " \t";

	fin.open(versionFileName.c_str());

	if (!fin.is_open())
	{
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, COMMAND_LINE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, COMMAND_LINE_BUFF).eof();
		if (bFlagFileEnd == TRUE)
		{
			break;
		}

		token = strtok(buff, seps);

		if (token == NULL						// ignore blank lines
			|| strncmp(token, "#", 1) == 0		// ignore comments
			|| strncmp(token, "$", 1) == 0)		// ignore user defined variables
		{
			continue;
		}

		if (stricmp(token, "ClientVersion") == 0)
		{
			VersionInfo tmpOldVersion;
			VersionInfo tmpNewVersion;

			char *token1 = strtok(NULL, seps);
			if (token1 == NULL)
			{
				char *msg = "Loading Version File Failed. ClientVersion\n";
				DBGOUT(msg);
				return FALSE;
			}

			char *token2 = strtok(NULL, seps);
			if (token2 == NULL)
			{
				char *msg = "Loading Version File Failed. ClientVersion\n";
				DBGOUT(msg);
				return FALSE;
			}

			tmpOldVersion.SetVersion(token1);
			tmpNewVersion.SetVersion(token2);

			// 2004-10-19 by cmkwon
			//m_mapVersionOld2New.insertEZ(tmpOldVersion, tmpNewVersion);
			m_mtmapVersionOld2New.insertLock(tmpOldVersion, tmpNewVersion);
		}
		else if (stricmp(token, "LauncherVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. LauncherVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestLauncherVersion.SetVersion(token);
		}
		else if (stricmp(token, "DeleteFileListVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. DeleteFileListVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestDeleteFileListVersion.SetVersion(token);
		}
		else if (stricmp(token, "NoticeVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. NoticeVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestNoticeVersion.SetVersion(token);
		}
	}

	fin.close();

#ifdef _DEBUG
	mtmapVersionInfo::iterator itr(m_mtmapVersionOld2New.begin());
	for (; itr != m_mtmapVersionOld2New.end(); itr++)
	{
		VersionInfo tmpOldVersion = itr->first;
		VersionInfo tmpNewVersion = itr->second;
		DBGOUT("  Client Version: %10s -> %10s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
	}
	DBGOUT("  Latest Client Version: %s\n", m_mtmapVersionOld2New.rbegin()->second.GetVersionString());
	DBGOUT("  Client Reinstall Version: %s\n", g_pPreGlobal->GetClientReinstallVersion().GetVersionString());
	DBGOUT("\n  Launcher Version: %s\n", m_LatestLauncherVersion.GetVersionString());
	DBGOUT("  Delete File List Version: %s\n", m_LatestDeleteFileListVersion.GetVersionString());
	DBGOUT("  Notice Version: %s\n\n", m_LatestNoticeVersion.GetVersionString());
	g_pPreGlobal->PrintAllowedList();
#endif

	m_mtmapVersionOld2New.unlock();

	return TRUE;
}

BOOL CPreIOCP::WriteVersionInfoToLocalFile()
{
	string versionFileName = string(CONFIG_ROOT) + string("versions.cfg");

	// 지우고 새로 저장
	DeleteFile(versionFileName.c_str());

	// 파일 생성
	HANDLE hFile;
	hFile = CreateFile(versionFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
							CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( hFile == INVALID_HANDLE_VALUE)
	{
		// error
		int nErr = GetLastError();
		SetLastError(0);

		DBGOUT("ERROR: Cannot VersionFile\r\n");

		return FALSE;
	}

	// 파일 쓰기
	DWORD nWritten;
	char lineBuf[512];

	// ClientVersion list
	m_mtmapVersionOld2New.lock();
	mtmapVersionInfo::iterator itr = m_mtmapVersionOld2New.begin();
	for(; m_mtmapVersionOld2New.end() != itr; itr++)
	{
		sprintf(lineBuf, "ClientVersion\t\t\t%s\t%s\r\n", ((VersionInfo&)itr->first).GetVersionString(), itr->second.GetVersionString());
		WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);		
	}
	m_mtmapVersionOld2New.unlock();

	// blank lines
	sprintf(lineBuf, "\r\n");
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// LauncherVersion
	sprintf(lineBuf, "LauncherVersion\t\t\t%s\r\n", m_LatestLauncherVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// DeleteFileListVersion
	sprintf(lineBuf, "DeleteFileListVersion\t%s\r\n", m_LatestDeleteFileListVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// NoticeVersion
	sprintf(lineBuf, "NoticeVersion\t\t\t%s\r\n", m_LatestNoticeVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// 파일 닫기
	CloseHandle(hFile);

	return TRUE;
}

// 해당 버전이 있는지 확인하고, 업데이트할 버전을 리턴함
BOOL CPreIOCP::GetNextUpdateVersion(VersionInfo *o_pNextVersion, VersionInfo *i_pVersion)
{
	m_mtmapVersionOld2New.lock();
	mtmapVersionInfo::iterator itr = m_mtmapVersionOld2New.find(*i_pVersion);
	if (itr == m_mtmapVersionOld2New.end())
	{
		m_mtmapVersionOld2New.unlock();
		return FALSE;
	}
	
	*o_pNextVersion = itr->second;
	m_mtmapVersionOld2New.unlock();
	
	return TRUE;
}

BOOL CPreIOCP::GetLatestUpdateVersion(VersionInfo *o_pLatestVersion)
{
	m_mtmapVersionOld2New.lock();
	if (m_mtmapVersionOld2New.empty())
	{
		m_mtmapVersionOld2New.unlock();
		return FALSE;
	}

	*o_pLatestVersion = m_mtmapVersionOld2New.rbegin()->second;
	m_mtmapVersionOld2New.unlock();

	return TRUE;
}

BOOL CPreIOCP::IsLogined(char* accountName)
{
	CAccountInfo * pAccInfo = GetAccountInfoPtr(accountName);

	if (pAccInfo == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CPreIOCP::GetAccountInfo(CAccountInfo *o_pAccountInfo, const char* accountName)
{
	m_AccountInfoMap.lock();
	CAccountInfo *ptmAccInfo = GetAccountInfoPtr(accountName);
	if(ptmAccInfo)
	{
		*o_pAccountInfo = *ptmAccInfo;
	}	
	m_AccountInfoMap.unlock();

	if(NULL == ptmAccInfo)
	{
		return FALSE;
	}

	return TRUE;
}

CAccountInfo *CPreIOCP::GetAccountInfoPtr(const char *i_szAccountName)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-18 by cmkwon, 소문자로 변환
	char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
	MEMSET_ZERO(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
	STRNCPY_MEMSET(szLowercaseAccName, i_szAccountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(szLowercaseAccName);

	return m_AccountInfoMap.findLock(szLowercaseAccName);
}

BOOL CPreIOCP::InsertAccountInfo(char* accountName, CAccountInfo* accountInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-18 by cmkwon, 소문자로 변환
	char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
	MEMSET_ZERO(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
	STRNCPY_MEMSET(szLowercaseAccName, accountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(szLowercaseAccName);

	BOOL bRet = m_AccountInfoMap.insertLock(szLowercaseAccName, accountInfo);

	if (bRet)
	{
		CServerGroup *pServerGroup = GetServerGroup(accountInfo->CurrentServerGroup);
		if (pServerGroup != NULL)
		{
			pServerGroup->m_setLoginedAccount.insertLock(szLowercaseAccName);
		}

		// 2012-07-11 by hskim, 선택적 셧다운
		if( NULL != g_pPreGlobal->m_pSelectiveShutdown )
		{
			if( TRUE == accountInfo->bSelectiveShutdownTarget )
			{
				CShutdownTarget TargeInfo(accountName, accountInfo->SelectiveShutdownInfo);

				g_pPreGlobal->m_pSelectiveShutdown->AddTarget(TargeInfo);
			}
		}
		// end 2012-07-11 by hskim, 선택적 셧다운
	}

	return bRet;
}

BOOL CPreIOCP::DeleteAccountInfo(char* accountName)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-18 by cmkwon, 소문자로 변환
	char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
	MEMSET_ZERO(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
	STRNCPY_MEMSET(szLowercaseAccName, accountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(szLowercaseAccName);

	m_AccountInfoMap.lock();
	CAccountInfo *pAccountInfo = GetAccountInfoPtr(szLowercaseAccName);
	if (pAccountInfo == NULL)
	{
		m_AccountInfoMap.unlock();
		return FALSE;
	}

	char szTmpServerGroupName[SIZE_MAX_SERVER_NAME];
	STRNCPY_MEMSET(szTmpServerGroupName, pAccountInfo->CurrentServerGroup, SIZE_MAX_SERVER_NAME);

	// 2012-07-11 by hskim, 선택적 셧다운
	if( NULL != g_pPreGlobal->m_pSelectiveShutdown )
	{
		if( TRUE == pAccountInfo->bSelectiveShutdownTarget )
		{
			CShutdownTarget TargeInfo(accountName, pAccountInfo->SelectiveShutdownInfo);
			g_pPreGlobal->m_pSelectiveShutdown->DelTarget(TargeInfo);
		}
	}
	// end 2012-07-11 by hskim, 선택적 셧다운

	int nDeleted = m_AccountInfoMap.erase(szLowercaseAccName);
	SAFE_DELETE(pAccountInfo);

	if (nDeleted!=0)
	{
		CServerGroup *pServerGroup = GetServerGroup(szTmpServerGroupName);
		if (pServerGroup != NULL)
		{
			pServerGroup->m_setLoginedAccount.deleteLock(szLowercaseAccName);
		}
	}
	m_AccountInfoMap.unlock();

	return (nDeleted!=0);
}

void CPreIOCP::DeleteAccountInfoByServerGroupName(const char *i_szServerGroup)
{
	m_AccountInfoMap.lock();
	{	
		mtmapAccountInfoPtr::iterator itrAccInfo = m_AccountInfoMap.begin();
		while( itrAccInfo != m_AccountInfoMap.end())
		{
			CAccountInfo *pAccInfo = itrAccInfo->second;
			if(0 == strnicmp(pAccInfo->CurrentServerGroup, i_szServerGroup, SIZE_MAX_SERVER_NAME))
			{
				m_AccountInfoMap.erase(itrAccInfo++);
				SAFE_DELETE(pAccInfo);
				continue;
			}

			itrAccInfo++;
		}
	}
	m_AccountInfoMap.unlock();
}

// 2010-11 by dhjin, 아라리오 채널링 로그인.
INT	CPreIOCP::GetOtherPublisherConncectCnt(eCONNECT_PUBLISHER i_eOtherPublisherConncect, char * i_pServerName)
{
#ifdef S_ARARIO_HSSON
	int Cnt = 0;
	mt_auto_lock mta(&m_AccountInfoMap);

	mtmapAccountInfoPtr::iterator itrAccInfo = m_AccountInfoMap.begin();
	for(; itrAccInfo != m_AccountInfoMap.end(); itrAccInfo++)
	{
		CAccountInfo *pAccInfo = itrAccInfo->second;
		if(i_eOtherPublisherConncect == pAccInfo->eOtherPublisherConncect &&
			0 == strnicmp(pAccInfo->CurrentServerGroup, i_pServerName, SIZE_MAX_SERVER_NAME))
		{
			Cnt++;
		}
	}

	return Cnt;
#endif // #ifdef S_ARARIO_HSSON
	
	return 0;
}

void CPreIOCP::DBG_PrintAccountMap()
{
	if (m_AccountInfoMap.size() >= 10)
	{
		return;
	}

	DBGOUT("\nAccount Info...\n");
	m_AccountInfoMap.lock();
	mtmapAccountInfoPtr::iterator itr = m_AccountInfoMap.begin();

	for (; itr != m_AccountInfoMap.end(); itr++)
	{
		DBGOUT("  ID(%s), State(%d), PrivateIP(%s)\n", (itr->first).c_str(), (itr->second)->ClientState, (itr->second)->PrivateClientIP);
	}
	m_AccountInfoMap.unlock();
	DBGOUT("\n");
}

void CPreIOCP::LoadBlockedAccounts()
{
	BOOL	ret;

	SQLHENV		henv = SQL_NULL_HENV;
	SQLHDBC		hdbc = SQL_NULL_HDBC;
	SQLHSTMT	hstmt = SQL_NULL_HSTMT;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//	ret = SQLConnect(hdbc, g_pGlobalGameServer->GetODBCDSN(), SQL_NTS, g_pGlobalGameServer->GetODBCUID(), SQL_NTS, g_pGlobalGameServer->GetODBCPASSWORD(), SQL_NTS);

	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
#if defined(DB_SERVER_MYSQL)
	sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
		, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), (char*)g_pGlobalGameServer->GetODBCUID(), (char*)g_pGlobalGameServer->GetODBCPASSWORD(), g_pGlobalGameServer->GetDBServerDatabaseName());
#else
	sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
		, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), (char*)g_pGlobalGameServer->GetODBCUID(), (char*)g_pGlobalGameServer->GetODBCPASSWORD(), g_pGlobalGameServer->GetDBServerDatabaseName());
#endif
	ret = SQLDriverConnect(hdbc, g_pGlobalGameServer->GetMainWndHandle(), (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CPreIOCP::LoadBlockedAccounts_) !! %s,%d %s %s %s\r\n"
			, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), g_pGlobalGameServer->GetDBServerDatabaseName(), (CHAR*)g_pGlobalGameServer->GetODBCUID(), (CHAR*)g_pGlobalGameServer->GetODBCPASSWORD());
		return;
	}

	
	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	// lock
	mt_auto_lock mtA(m_pAccountBlockManager->GetmapBLOCKED_ACCOUNT_INFOPtr());
	m_pAccountBlockManager->ResetAccountBlockManager();

	/*[Stored Query Definition]************************************************
	-- 2005-06-02 by cmkwon
	-- 계정 블럭 리스트 로딩
	--			2007-01-10 by cmkwon, td_BlockedAccounts 테이블에 BlockedReasonForOnlyAdmin 필드 추가함
	CREATE PROCEDURE atum_Load_BlockAccounts
	AS
		-- 모두 로딩한다
		SELECT AccountName, BlockedType, StartDate, EndDate, AdminAccountName, BlockedReason, BlockedReasonForOnlyAdmin
			FROM td_BlockedAccounts
	GO
	**************************************************************************/

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0276, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_STMT, hstmt,"CPreIOCP::LoadBlockedAccounts Failed!\n", TRUE);
		return;
	}

	if(SQL_NO_DATA != ret)
	{
		SQLINTEGER arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
		SBLOCKED_ACCOUNT_INFO	tmBlockedInfo;
		SQL_TIMESTAMP_STRUCT	arrSqlTime[2];

		SQLBindCol(hstmt, 1, SQL_C_CHAR, tmBlockedInfo.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME,			&arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_LONG, &tmBlockedInfo.enBlockedType, 0,									&arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &arrSqlTime[0], 0,											&arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[1], 0,											&arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, tmBlockedInfo.szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_CHAR, tmBlockedInfo.szBlockedReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON,	&arrCB[6]);
		SQLBindCol(hstmt, 7, SQL_C_CHAR, tmBlockedInfo.szBlockedReasonForOnlyAdmin, SIZE_MAX_BLOCKED_ACCOUNT_REASON,	&arrCB[7]);	// 2007-01-10 by cmkwon

		MEMSET_ZERO(&tmBlockedInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
		MEMSET_ZERO(arrSqlTime, sizeof(arrSqlTime[0])*2);
		while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			tmBlockedInfo.atimeStartTime	= arrSqlTime[0];
			tmBlockedInfo.atimeEndTime		= arrSqlTime[1];

			// 2010-02-22 by cmkwon, 계정블럭시 대문자포함 계정명 문제 해결(K0002462) - 
			_strlwr(tmBlockedInfo.szBlockedAccountName);

			m_pAccountBlockManager->AddBlockedAccountList(&tmBlockedInfo);

			MEMSET_ZERO(&tmBlockedInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
			MEMSET_ZERO(arrSqlTime, sizeof(arrSqlTime[0])*2);
		}
	}
	///////////////////////////////////////////////////////////////////////////////
	// lock된것을 해제한다.
	mtA.auto_unlock_cancel();

	SQLFreeStmt(hstmt, SQL_CLOSE);
	// cleanup odbc resources
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
#ifndef _DEBUG
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
#endif
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

// start 2011-12-21 by hskim, EP4 [동영상 1회 재생]
BOOL CPreIOCP::LoadSystemEvent(CODBCStatement *i_pODBCStmt)
{
	if( FALSE == CAtumDBHelper::DBLoadSystemEventInfo(i_pODBCStmt , &m_OpeningMovieStandard, FALSE) )
	{
		return FALSE;
	}	

	return TRUE;
}
// end 2011-12-21 by hskim, EP4 [동영상 1회 재생]

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::InitDBTable(CODBCStatement *i_pODBCStmt)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPreIOCP::InitDBTable(CODBCStatement *i_pODBCStmt)
{
//	char szQuery[1024];

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, 접속 중인 ServerGroupID 필드를 모두 초기화 한다.
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	sprintf(szQuery, "UPDATE td_Account SET ConnectingServerGroupID = 0");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0087);
//	BOOL bRet = i_pODBCStmt->ExecuteQuery(szQuery);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0087));
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();		// error
		return FALSE;
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;
}

BOOL CPreIOCP::BlockAccount(SBLOCKED_ACCOUNT_INFO *i_pBlockedAccountInfo, CPreIOCPSocket *i_pSocketToNotify, BOOL i_bDisconnectUser/*=TRUE*/)
{
	if (i_bDisconnectUser)
	{
		CAccountInfo tmAccountInfo;
		BOOL bRet = GetAccountInfo(&tmAccountInfo, i_pBlockedAccountInfo->szBlockedAccountName);
		if(bRet)
		{// 연결된 상태

			DisconnectAccount(&tmAccountInfo, ERR_PROTOCOL_ACCOUNT_BLOCKED, i_pSocketToNotify);
		}
	}

	m_pAccountBlockManager->AddBlockedAccountList(i_pBlockedAccountInfo);
	
	SBLOCKED_ACCOUNT_INFO *pDBQuery	= new SBLOCKED_ACCOUNT_INFO;
	*pDBQuery						= *i_pBlockedAccountInfo;
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_BlockUser, i_pSocketToNotify, pDBQuery);
	
	return TRUE;
}

// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - UnblockAccount() 함수 인자 수정
//BOOL CPreIOCP::UnblockAccount(const char *i_szAccountName, CPreIOCPSocket *i_pSocketToNotify)
BOOL CPreIOCP::UnblockAccount(MSG_PA_ADMIN_UNBLOCK_ACCOUNT *i_pUnblockAccount, CPreIOCPSocket *i_pSocketToNotify)
{
// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 아래와 같이 수정
// 	m_pAccountBlockManager->DeleteBlockedAccountList(i_szAccountName);
// 
// 	char *szAccountName = new char[SIZE_MAX_ACCOUNT_NAME];
// 	STRNCPY_MEMSET(szAccountName, i_szAccountName, SIZE_MAX_ACCOUNT_NAME);
// 	m_pAtumDBManager->MakeAndEnqueueQuery(QT_UnblockUser, i_pSocketToNotify, szAccountName);

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 블럭 상태의 계정 체크 추가
	if(FALSE == m_pAccountBlockManager->DeleteBlockedAccountList(i_pUnblockAccount->szBlockedAccountName))
	{
		return FALSE;
	}

	SBLOCKED_ACCOUNT_INFO *pDBQuery	= new SBLOCKED_ACCOUNT_INFO;
	*pDBQuery						= *i_pUnblockAccount;
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_UnblockUser, i_pSocketToNotify, pDBQuery);
	return TRUE;
}

BOOL CPreIOCP::DisconnectAccount(CAccountInfo *i_pAccountInfo, Err_t i_nError, CPreIOCPSocket *i_pSocketToNotify)
{
	if (i_pAccountInfo == NULL)
	{
		return FALSE;
	}

	CServerGroup *pServerGroup = GetServerGroup(i_pAccountInfo->CurrentServerGroup);
	if (pServerGroup == NULL)
	{
		DBGOUT("  Error: No Such Server Group!");
		return FALSE;
	}

	CPreIOCPSocket *pFieldServerSocket = (CPreIOCPSocket*)pServerGroup->m_FieldServerInfo.pSocket;
	if (NULL == pFieldServerSocket
		|| FALSE == pFieldServerSocket->IsUsing())
	{
		DBGOUT("  Error: Not Available Field Server Socket!");
		return FALSE;
	}
	pFieldServerSocket->SendErrorMessage(T_PA_ADMIN_DISCONNECT_USER, i_nError, i_pAccountInfo->CurrentClientIndex, 0, i_pAccountInfo->AccountName);

	// Account 정보 지우기
	DeleteAccountInfo(i_pAccountInfo->AccountName);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CPreIOCP::On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, MSG_FP_CONNECT_AUTH_USER *i_pMsg)
/// \brief		
/// \author		cmkwon
/// \date		2004-10-19 ~ 2004-10-19
/// \warning	
///
/// \param		
/// \return		nErrorNum
///////////////////////////////////////////////////////////////////////////////
int CPreIOCP::On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, MSG_FP_CONNECT_AUTH_USER *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);

	DBGOUT("%s AUTH_USER: %s %s %d %s\n", GetTimeString(string()), i_pMsg->AccountName
		, i_pMsg->FieldServerID.IP , i_pMsg->AuthType
		, (pAccountInfo!=NULL)?STRMSG_S_P2PRENOTIFY_0001:STRMSG_S_P2PRENOTIFY_0002);

	if (NULL == pAccountInfo)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGINED)	// state가 LOGINED인지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
	}
	else if (pAccountInfo->CurrentFieldServerID != i_pMsg->FieldServerID)
	{
		nRetErrorNum = ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED;
	}
	else if (strncmp(i_pMsg->PrivateIP, pAccountInfo->PrivateClientIP, SIZE_MAX_IPADDRESS) != 0)
	{
		nRetErrorNum = ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED;
	}
	else
	{// 인증 성공

		// client index 저장
		pAccountInfo->CurrentClientIndex = i_pMsg->ClientIndex;
		*o_AccountInfo = *pAccountInfo;

		nRetErrorNum = 0;
	}
	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_NOTIFY_WARP(MSG_FP_EVENT_NOTIFY_WARP *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);

	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGINED)	// state가 LOGINED인지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
	}
	else
	{
		//strncpy(pAccountInfo->CurrentFieldServerIP, pMsgRecvWarp->FieldServerIP, SIZE_MAX_IPADDRESS);
		pAccountInfo->CurrentFieldServerID = i_pMsg->FieldServerID;
		
		nRetErrorNum = 0;
	}
	
	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_ENTER_SELECT_SCREEN(CP_State *o_pState, MSG_FP_EVENT_ENTER_SELECT_SCREEN *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGINED && pAccountInfo->ClientState != CP_GAMESTARTED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		STRNCPY_MEMSET(pAccountInfo->CurrentCharacterName, "", SIZE_MAX_CHARACTER_NAME);
		pAccountInfo->CurrentCharacterUniqueNumber		= 0;
		pAccountInfo->CurrentMapChannel.MapIndex		= 0;
		pAccountInfo->CurrentMapChannel.ChannelIndex	= 0;
		pAccountInfo->ClientState						= CP_FIELD_LOGINED;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_GAME_STARTED(CP_State *o_pState, MSG_FP_EVENT_GAME_STARTED *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_FIELD_LOGINED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		STRNCPY_MEMSET(pAccountInfo->CurrentCharacterName, i_pMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pAccountInfo->CurrentCharacterUniqueNumber	= i_pMsg->CharacterUniqueNumber;
		pAccountInfo->CurrentMapChannel				= i_pMsg->MapChannelIndex;
		pAccountInfo->ClientState					= CP_GAMESTARTED;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_MAP_CHANGED(CP_State *o_pState, MSG_FP_EVENT_MAP_CHANGED *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_GAMESTARTED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
		
		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		// 맵 업데이트
		pAccountInfo->CurrentMapChannel = i_pMsg->MapChannelIndex;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE(MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE *i_pMsg)
{
		this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	DBGOUT("FIELDSERVER_CHANGE: %s %s %s\n", i_pMsg->AccountName
		, i_pMsg->FieldServerID.GetString(string())
		, (pAccountInfo!=NULL)?STRMSG_S_P2PRENOTIFY_0001:STRMSG_S_P2PRENOTIFY_0002);


	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGINED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;		
	}
	else
	{// 

		//strncpy(pAccountInfo->CurrentFieldServerIP, pMsgRecvNotifyFieldServer->FieldServerIP, SIZE_MAX_IPADDRESS);
		pAccountInfo->CurrentFieldServerID = i_pMsg->FieldServerID;

		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}



CServerGroup* CPreIOCP::GetServerGroup(const char * szServerGruopName)
{
	vector<CServerGroup*>::iterator itr = m_AllServerGroupVector.begin();
	for (; itr != m_AllServerGroupVector.end(); itr++)
	{
		if (strnicmp((*itr)->m_ServerGroupName , szServerGruopName, SIZE_MAX_SERVER_NAME) == 0)
		{
			return *itr;
		}
	}

	return NULL;
}

void CPreIOCP::ResetServerGroup(const char *i_szServerGroupName, BOOL i_bExcludeIMServer/*=FALSE*/)
{
	CServerGroup *pSGroup = GetServerGroup(i_szServerGroupName);
	if(pSGroup)
	{
		pSGroup->ResetServerGroup(i_bExcludeIMServer);				
	}

	this->DeleteAccountInfoByServerGroupName(i_szServerGroupName);
}


void CPreIOCP::SendServerInfoForAdmin(CPreIOCPSocket *i_pSocket)
{
// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 아래와 같이 수정 함
// 	MEX_SERVER_INFO_FOR_ADMIN *pMexServerInfo = NULL;
// 	int offset = 0;
// 
// 	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_SERVER_INFO_OK, T_PA_ADMIN_GET_SERVER_INFO_OK, msgServerInfoOK, msgServerInfoOKBuf);
// 	msgServerInfoOK->NumOfServers = 0;
// 
// 	offset = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
// 	vector<CServerGroup*>::iterator itr = m_AllServerGroupVector.begin();
// 	for (; itr != m_AllServerGroupVector.end(); itr++)
// 	{
// 		CServerGroup *pServerGroup = *itr;
// 		if(pServerGroup->m_bEnableServerGroup)
// 		{
// 			pMexServerInfo = (MEX_SERVER_INFO_FOR_ADMIN*)(msgServerInfoOKBuf+offset);
// 			MEMSET_ZERO(pMexServerInfo, sizeof(MEX_SERVER_INFO_FOR_ADMIN));
// 
// 			STRNCPY_MEMSET(pMexServerInfo->ServerName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
// 			pMexServerInfo->ServerType		= ST_IM_SERVER;
// 			pMexServerInfo->ServerID		= pServerGroup->m_IMServerInfo.serverID;
// 			pMexServerInfo->UserCount		= pServerGroup->GetUserCount();
// 			pMexServerInfo->ServerState		= (pServerGroup->m_IMServerInfo.IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);
// 			offset += sizeof(MEX_SERVER_INFO_FOR_ADMIN);
// 			msgServerInfoOK->NumOfServers++;
// 
// 			FieldServerInfo *pFieldServerInfo = &pServerGroup->m_FieldServerInfo;
// 			pMexServerInfo = (MEX_SERVER_INFO_FOR_ADMIN*)(msgServerInfoOKBuf+offset);
// 			MEMSET_ZERO(pMexServerInfo, sizeof(MEX_SERVER_INFO_FOR_ADMIN));
// 
// 			STRNCPY_MEMSET(pMexServerInfo->ServerName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
// 			pMexServerInfo->ServerType		= ST_FIELD_SERVER;
// 			pMexServerInfo->ServerID		= pFieldServerInfo->serverID;
// 			pMexServerInfo->UserCount		= 0;	// 필드 서버는 알 수 없음
// 			pMexServerInfo->ServerState		= (pFieldServerInfo->IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);
// 			offset += sizeof(MEX_SERVER_INFO_FOR_ADMIN);
// 			msgServerInfoOK->NumOfServers++;
// 		}		
// 	}
// 
// 	i_pSocket->SendAddData(msgServerInfoOKBuf, offset);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) -
	int offset = 0;

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_SERVER_INFO_OK, T_PA_ADMIN_GET_SERVER_INFO_OK, msgServerInfoOK, msgServerInfoOKBuf);
	msgServerInfoOK->NumOfServerGroups		= 0;

	offset = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
	vector<CServerGroup*>::iterator itr = m_AllServerGroupVector.begin();
	for (; itr != m_AllServerGroupVector.end(); itr++)
	{
		CServerGroup *pServerGroup = *itr;
		if(FALSE == pServerGroup->m_bEnableServerGroup)
		{
			continue;
		}
		if(offset + sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN) > SIZE_MAX_PACKET)
		{
			i_pSocket->SendAddData(msgServerInfoOKBuf, offset);
			msgServerInfoOK->NumOfServerGroups	= 0;
			offset								= MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
		}

		MEX_SERVER_GROUP_INFO_FOR_ADMIN *pMexServGInfo = (MEX_SERVER_GROUP_INFO_FOR_ADMIN*)(msgServerInfoOKBuf+offset);
		MEMSET_ZERO(pMexServGInfo, sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN));

		STRNCPY_MEMSET(pMexServGInfo->ServerName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
		pMexServGInfo->IMServerID			= pServerGroup->m_IMServerInfo.serverID;
		pMexServGInfo->UserCount			= pServerGroup->GetUserCount();
		pMexServGInfo->IMServerState		= (pServerGroup->m_IMServerInfo.IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);

		pMexServGInfo->FieldServerID		= pServerGroup->m_FieldServerInfo.serverID;
		pMexServGInfo->FieldServerState		= (pServerGroup->m_FieldServerInfo.IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);
		pMexServGInfo->LimitUserCount		= pServerGroup->m_LimitGroupUserCounts;
		pMexServGInfo->LockCreateCharacterForNewAccount	= pServerGroup->m_bLockCreateCharacterForNewAccount;
		offset += sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN);
		msgServerInfoOK->NumOfServerGroups++;
	}

	if(0 < msgServerInfoOK->NumOfServerGroups)
	{
		i_pSocket->SendAddData(msgServerInfoOKBuf, offset);
	}
}


BOOL CPreIOCP::SendErrorToFieldServer(char *i_szServerGroupName
									  , SERVER_ID i_FieldServerid
									  , MessageType_t msgType
									  , Err_t err
									  , int errParam1/*=0*/
									  , int errParam2/*=0*/
									  , char* errMsg/*=NULL*/
									  , BOOL bCloseConnection/*=FALSE*/)
{	
	CServerGroup *pServerGroup = this->GetServerGroup(i_szServerGroupName);
	if (pServerGroup == NULL)
	{// 서버 그룹이 없다
		DBGOUT("  Error: No Such Server Group(%s)!", i_szServerGroupName);		
		return FALSE;
	}

	CPreIOCPSocket *pPISocket = (CPreIOCPSocket*)pServerGroup->m_FieldServerInfo.pSocket;
	if (pPISocket == NULL 
		|| FALSE == pPISocket->IsUsing())
	{
		DBGOUT("  Error: Not Available Field Server Socket!");
		return FALSE;
	}
	
	pPISocket->SendErrorMessage(msgType, err
		, errParam1, errParam2, errMsg, bCloseConnection);	
	return TRUE;
}


void CPreIOCP::SendMessageToAllFieldServer(BYTE *i_pBuff, int i_nSize)
{
	vector<CServerGroup*>::iterator itr(m_AllServerGroupVector.begin());
	for(; itr != m_AllServerGroupVector.end(); itr++)
	{
		(*itr)->SendMessageToFieldServer(i_pBuff, i_nSize);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPreIOCP::SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPreIOCP::SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize)
{
	vector<CServerGroup*>::iterator itr(m_AllServerGroupVector.begin());
	for(; itr != m_AllServerGroupVector.end(); itr++)
	{
		(*itr)->SendMsgToIMServer(i_pBuff, i_nSize);
	}
}


void CPreIOCP::OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{
	if(0 == pDateTime->Minute%10)
	{// 10분에 한번씩 처리

		this->CheckOnlyLauncherLoginedList(pDateTime);
	}

	if(0 == pDateTime->Minute%5)
	{
		this->StoredLogTotalUser();
	}	
}


void CPreIOCP::CheckOnlyLauncherLoginedList(ATUM_DATE_TIME *pDateTime)
{
	m_AccountInfoMap.lock();
	{
		mtmapAccountInfoPtr::iterator itr(m_AccountInfoMap.begin());
		while (itr != m_AccountInfoMap.end())
		{
			CAccountInfo *pAccInfo = itr->second;
			if(CP_LOGINED == pAccInfo->ClientState
				&& 10 < pDateTime->GetTimeDiffTimeInMinutes(pAccInfo->LauncherLoginTime))
			{// LauncherLogin후 10분이 지났다

				CServerGroup *pServerGroup = GetServerGroup(pAccInfo->CurrentServerGroup);
				if (pServerGroup != NULL)
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2006-10-18 by cmkwon, 소문자로 변환
					char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
					MEMSET_ZERO(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
					STRNCPY_MEMSET(szLowercaseAccName, pAccInfo->AccountName, SIZE_MAX_ACCOUNT_NAME);
					_strlwr(szLowercaseAccName);
					
					pServerGroup->m_setLoginedAccount.deleteLock(szLowercaseAccName);
				}
				m_AccountInfoMap.erase(itr++);
				SAFE_DELETE(pAccInfo);
				continue;
			}
			itr++;
		}
	}
	m_AccountInfoMap.unlock();
}


void CPreIOCP::StoredLogTotalUser(void)
{
	MSG_DB_TOTAL_USER *pTotalUser = new MSG_DB_TOTAL_USER;
	STRNCPY_MEMSET(pTotalUser->szServerGroup, "AllServer", SIZE_MAX_SERVER_NAME);
	pTotalUser->nMGameServerID	= 0;
	pTotalUser->nUserCounts		= 0;
#ifdef S_ARARIO_HSSON
	pTotalUser->nUserCountsOtherPublisherConncect	= 0;		 // 2010-11 by dhjin, 아라리오 채널링 로그인
#endif
	vector<CServerGroup*>::iterator itr(m_AllServerGroupVector.begin());
	for(; itr != m_AllServerGroupVector.end(); itr++)
	{		
		CServerGroup *pSGroup = *itr;
		if(pSGroup->m_bEnableServerGroup)
		{
			pTotalUser->nUserCounts += pSGroup->GetUserCount();

			MSG_DB_TOTAL_USER *pSGUser = new MSG_DB_TOTAL_USER;
			STRNCPY_MEMSET(pSGUser->szServerGroup, pSGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
			pSGUser->nMGameServerID	= pSGroup->m_nMGameServerID;
			pSGUser->nUserCounts	= pSGroup->GetUserCount();
#ifdef S_ARARIO_HSSON
			pSGUser->nUserCountsOtherPublisherConncect	= this->GetOtherPublisherConncectCnt(CONNECT_PUBLISHER_NHN_JPN, pSGroup->m_ServerGroupName);		// 2010-11 by dhjin, 아라리오 채널링 로그인, 우선은 아라리오를 위한 형식으로 구성됨. 차후 구조 만들 필요가 있으면 만들도록
			m_pAtumDBManager->MakeAndEnqueueQuery(QT_LogTotalUser, NULL, (void*)pSGUser);
			pTotalUser->nUserCountsOtherPublisherConncect += pSGUser->nUserCountsOtherPublisherConncect;			// 2010-11 by dhjin, 아라리오 채널링 로그인
#else
			m_pAtumDBManager->MakeAndEnqueueQuery(QT_LogTotalUser, NULL, (void*)pSGUser);
#endif
		}
	}

	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LogTotalUser, NULL, (void*)pTotalUser);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::ExtAuthODBCInit(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPreIOCP::ExtAuthODBCInit(void)
{
	if(FALSE == g_pPreGlobal->IsExternalAuthentication())
	{
		return FALSE;
	}

	mt_auto_lock mtA(m_odbcExtAuth.GetLockPtr());
	if(m_odbcExtAuth.IsReadyToUse())
	{
		return TRUE;
	}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
//	return m_odbcExtAuth.Init(g_pPreGlobal->GetExtAuthODBCDSN(), g_pPreGlobal->GetExtAuthODBCUID(), g_pPreGlobal->GetExtAuthODBCPASSWORD());
	return m_odbcExtAuth.Init(g_pPreGlobal->GetExtAuthDBServerIP(), g_pPreGlobal->GetExtAuthDBServerPort(), g_pPreGlobal->GetExtAuthDBServerDatabaseName(), (char*)g_pPreGlobal->GetExtAuthODBCUID(), (char*)g_pPreGlobal->GetExtAuthODBCPASSWORD(), g_pPreGlobal->GetMainWndHandle());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPreIOCP::ExtAuthODBCReset(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPreIOCP::ExtAuthODBCReset(void)
{
	m_odbcExtAuth.Clean();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::ExtAuthODBCAuthentication(char *i_szAccName, char *i_szPwd)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPreIOCP::ExtAuthODBCAuthentication(char *i_szAccName, char *i_szPwd)
{
	if(FALSE == g_pPreGlobal->IsExternalAuthentication())
	{
		return FALSE;
	}

	mt_auto_lock mtA(m_odbcExtAuth.GetLockPtr());
	if(FALSE == m_odbcExtAuth.IsReadyToUse())
	{
		if(FALSE == this->ExtAuthODBCInit())
		{
			// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
			g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CPreIOCP::ExtAuthODBCAuthentication_) !! %s,%d %s %s %s\r\n"
				, g_pGlobalGameServer->GetExtAuthDBServerIP(), g_pGlobalGameServer->GetExtAuthDBServerPort(), g_pGlobalGameServer->GetExtAuthDBServerDatabaseName(), (char*)g_pGlobalGameServer->GetExtAuthODBCUID(), (char*)g_pGlobalGameServer->GetExtAuthODBCPASSWORD());
			return FALSE;
		}
	}
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_ExternalAuthentication
	-- Desc: 2006-04-27 by cmkwon, 추가 함
	--		   외부 DB 인증 프로시저 - 이것은 없어도 되지만 내부에서 테스트 하기 위해 필요한 모듈임
	--		   실제로 이 모듈은 상대 회사에서 구현해 주어야 한다.
	--====
	CREATE PROCEDURE atum_ExternalAuthentication
		@i_AccountName				VARCHAR(20),	
		@i_Password					VARCHAR(35)
	AS
		DECLARE @AccountUID INT
		
		SET @AccountUID = (SELECT AccountUniqueNumber 
								FROM td_Account
								WHERE AccountName = @i_AccountName AND Password = @i_Password)
		
		IF (@AccountUID IS NULL)
			BEGIN
					RETURN 0
			END

		RETURN 1
	GO	
	**************************************************************************/
	SQLHSTMT	hstmt = m_odbcExtAuth.GetSTMTHandle();
	BOOL		bAuth = FALSE;

	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bAuth, 0,								NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
	BOOL ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call atum_ExternalAuthentication(?, ?)}", SQL_NTS);

	if (ret != SQL_SUCCESS
		&& ret != SQL_SUCCESS_WITH_INFO)
	{
		if(ret != SQL_NO_DATA)
		{// 쿼리 실패

			m_odbcExtAuth.ProcessLogMessagesForStmt("@CPreIOCP::ExtAuthODBCAuthentication_ Failed!\r\n");
			this->ExtAuthODBCReset();
		}
		m_odbcExtAuth.FreeStatement();	// close cursor
		return FALSE;
	}
	m_odbcExtAuth.FreeStatement();	// close cursor

	if(FALSE == bAuth)
	{
		return FALSE;
	}
	
	return TRUE;
}

// PreServer 시작 시 자동 버젼 업데이트 갱신
BOOL CPreIOCP::PreGetVersionInfoFromFTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
{
	HINTERNET hInternet;
	HINTERNET hFtpConnect;
	HINTERNET hDir;
	WIN32_FIND_DATA pDirInfo;
	char pDirString[SIZE_MAX_FTP_FILE_PATH];

	hInternet = InternetOpen("Atum Pre Server", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		// check: error
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] InternetOpen ERROR!!!");
		return FALSE;
	}

	hFtpConnect = InternetConnect(hInternet, g_pPreGlobal->GetUploadFTPIP(), g_pPreGlobal->GetUploadFTPPort(),
		g_pPreGlobal->GetUploadFTPAccount(), g_pPreGlobal->GetUploadFTPPassword(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpConnect == NULL)
	{
		// check: error
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] InternetConnect ERROR!!!");
		return FALSE;
	}

	char currDir[128];
	DWORD currDirLen = 128;
	if (FtpGetCurrentDirectory(hFtpConnect, currDir, &currDirLen))
	{
	}

	// change directory
	if (FALSE == FtpSetCurrentDirectory(hFtpConnect, (LPCSTR)g_pPreGlobal->m_szClientFTPUpdateUploadDir))
	{
		// check: error
		DWORD err = GetLastError();
		DWORD len = 128;
		char errString[128];
		InternetGetLastResponseInfo(&err, errString, &len);
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] %s Change Directory ERROR!", errString);		
		return FALSE;
	}

	// Find files and update UpdateFileListVector
	sprintf(pDirString, "%s", "*");
	if (!(hDir = FtpFindFirstFile(hFtpConnect, pDirString, &pDirInfo, 0, 0) ) )
	{
		// check: error
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] FTPFile Not Find ");
		return FALSE;
	}
	else
	{
		if (strcmp(pDirInfo.cFileName, ".") != 0 && strcmp(pDirInfo.cFileName, "..") != 0 )
		{
			if (pDirInfo.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				// zip file 이름 해석하여 버전 넣기
				const int SIZE_BUFF = 512;
				char		buff[SIZE_BUFF];
				char		seps[] = "_zip";
				STRNCPY_MEMSET(buff, pDirInfo.cFileName, SIZE_BUFF);

				// from version
				char *token1 = strtok(buff, seps);
				// to version
				char *token2 = strtok(NULL, seps);

				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);

				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
				if (pVersion == NULL)
				{
					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
				}
				else
				{
					if (*pVersion < NewVersion)
					{
						*pVersion = NewVersion;
					}
				}
			}
			else
			{
				// check: error! directory가 존재하면 아니 됨!
				g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] directory Error ");
				return FALSE;
			}
		}

		while(TRUE)
		{
			if (!InternetFindNextFile (hDir, &pDirInfo))
			{
				int		nErr = GetLastError();
				SetLastError(0);

				if (ERROR_NO_MORE_FILES == nErr)
				{
					DBGOUT("No more files here\n");
					break;
				}
				else
				{
					// check: error
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] FindNextFile ");
					return FALSE;
				}
			}
			else
			{
				if (strcmp(pDirInfo.cFileName, ".") == 0 || strcmp(pDirInfo.cFileName, "..") == 0 )
				{
					continue;
				}

				if (pDirInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					// check: error! directory가 존재하면 아니 됨!
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] FILE_ATTRIBUTE_DIRECTORY ");
					return FALSE;
				}

				// zip file 이름 해석하여 버전 넣기
				const int SIZE_BUFF = 512;
				char		buff[SIZE_BUFF];
				char		seps[] = "_zip";
				STRNCPY_MEMSET(buff, pDirInfo.cFileName, SIZE_BUFF);

				// from version
				char *token1 = strtok(buff, seps);
				// to version
				char *token2 = strtok(NULL, seps);

				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);

				if(FALSE == OldVersion.IsValidVersionInfo()
					|| FALSE == NewVersion.IsValidVersionInfo()
					|| OldVersion >= NewVersion)
				{// 2006-12-28 by cmkwon, 유효한 업데이트 파일이 아니다

					char szErr[1024];
					sprintf(szErr, "Invalid update version file(%s/%s) !!", g_pPreGlobal->m_szClientFTPUpdateUploadDir, pDirInfo.cFileName);
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] %s ", szErr);
					continue;
				}

				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
				if (pVersion == NULL)
				{
					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
				}
				else
				{
					if (*pVersion < NewVersion)
					{
						*pVersion = NewVersion;
					}
				}
			}
		}
	}

	if (o_pMapVersionOld2New->empty())
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] No Version File in FTP Server");
		return FALSE;
	}

	char *buffer = new char[DOWNLOAD_BUFFER_SIZE];
	DWORD amount_read = 0;
	int	total_read = 0;
	BOOL ret = FALSE;

	///////////////////////////////////////////////////////////////////////////
	// get launcher version
	sprintf(pDirString, "%s/lversion.ver", (LPCSTR)g_pPreGlobal->m_szLauncherFileUploadPath);
	HINTERNET hLauncherVersionFile
		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);

	amount_read = 0;
	total_read = 0;

	ret = FALSE;
	while(TRUE)
	{
		ret = InternetReadFile (hLauncherVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
		if (!ret)
		{
			DWORD err;
			char errMsg[1024];
			DWORD errMsgLen = 1024;
			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
			g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] %s ", errMsg);
			return FALSE;
		}

		if(0 == amount_read)
		{
			break;
		}
		total_read += amount_read;
	}

	memcpy(buffer + total_read, "\0", 1);

	o_pLauncherVersion->SetVersion(buffer);

	// close file handle
	InternetCloseHandle(hLauncherVersionFile);

	sprintf(pDirString, "%s/dversion.ver", (LPCSTR)g_pPreGlobal->m_szDeleteFileListUploadPath);
	HINTERNET hDelFileListVersionFile
		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);

	amount_read = 0;
	total_read = 0;

	ret = FALSE;
	while(ret)
	{
		ret = InternetReadFile (hDelFileListVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
		if(FALSE == ret)
		{// 2006-06-30 by cmkwon
			DWORD err;
			char errMsg[1024];
			DWORD errMsgLen = 1024;
			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
			g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] %s ", errMsg);
			return FALSE;
		}
		if(0 == amount_read)
		{// 2006-06-30 by cmkwon
			break;
		}
		total_read += amount_read;
	}

	memcpy(buffer + total_read, "\0", 1);
	if(0 < total_read)
	{
		o_pDeleteFileListVersion->SetVersion(buffer);
	}

	// close file handle
	InternetCloseHandle(hDelFileListVersionFile);

	// delete buffer
	delete buffer;

	// close handles
	InternetCloseHandle(hDir);
	InternetCloseHandle(hFtpConnect);
	InternetCloseHandle(hInternet);

#ifdef _DEBUG
	for (ez_map<VersionInfo, VersionInfo>::iterator itr = o_pMapVersionOld2New->begin(); itr != o_pMapVersionOld2New->end(); itr++)
	{
		VersionInfo tmpOldVersion = itr->first;
		VersionInfo tmpNewVersion = itr->second;
		DBGOUT("  Client Version: %s -> %s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
	}
	DBGOUT("  Latest Client Version: %s\n", o_pMapVersionOld2New->rbegin()->second.GetVersionString());
	DBGOUT("\n  Launcher Version: %s\n", o_pLauncherVersion->GetVersionString());
	DBGOUT("  Delete File List Version: %s\n", o_pDeleteFileListVersion->GetVersionString());
	DBGOUT("  Notice Version: %s\n\n", o_pNoticeVersion->GetVersionString());
#endif

	return TRUE;
}

// PreServer 시작 시 자동 버젼 업데이트 갱신
BOOL CPreIOCP::PreGetVersionInfoByHTTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
{
	char			szRemoteFileName[SIZE_MAX_FTP_FILE_PATH];
	char			szTempFileName[SIZE_MAX_FTP_FILE_PATH];;
	CHttpManager	httpMan;
	Err_t			errCode;
	
//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szLauncherFileDownloadHttpPath);
//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szDeleteFileListDownloadHttpPath);
//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szNoticeFileDownloadHttpPath);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Delete file list version
	STRNCPY_MEMSET(szTempFileName, "tmdversion.ver", SIZE_MAX_FTP_FILE_PATH);
	sprintf(szRemoteFileName, "%s/dversion.ver", (LPSTR)(LPCSTR)GetOnlyPathWithoutFileName((CString)g_pPreGlobal->m_szDeleteFileListDownloadHttpPath));
	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
		, szRemoteFileName, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			MEMSET_ZERO(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
 			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	const int	SIZE_BUFF = 1024;
	char		buffer[SIZE_BUFF];
	{
		ifstream	finD;

		finD.open(szTempFileName);
		if (false == finD.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}
		MEMSET_ZERO(buffer, SIZE_BUFF);
		finD.getline(buffer, SIZE_BUFF);
		if(7 > strlen(buffer))
		{
			buffer[SIZE_BUFF-1] = '\0';
			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
			return FALSE;
		}
		o_pDeleteFileListVersion->SetVersion(buffer);
		finD.close();
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Launcher File Version
	STRNCPY_MEMSET(szTempFileName, "tmlversion.ver", SIZE_MAX_FTP_FILE_PATH);
	sprintf(szRemoteFileName, "%s/lversion.ver", (LPSTR)(LPCSTR)GetOnlyPathWithoutFileName((CString)g_pPreGlobal->m_szLauncherFileDownloadHttpPath));
	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
		, szRemoteFileName, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			MEMSET_ZERO(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	{
		ifstream finl;
		finl.open(szTempFileName);
		if (false == finl.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}
		MEMSET_ZERO(buffer, SIZE_BUFF);
		finl.getline(buffer, SIZE_BUFF);
		if(7 > strlen(buffer))
		{
			buffer[SIZE_BUFF-1] = '\0';
			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
			return FALSE;
		}
		o_pLauncherVersion->SetVersion(buffer);
		finl.close();
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Version list file
	STRNCPY_MEMSET(szTempFileName, "tmversionlist.ver", SIZE_MAX_FTP_FILE_PATH);
	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
		, g_pPreGlobal->m_szVersionListFileDownloadHttpPath, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			MEMSET_ZERO(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	{
		ifstream finV;
		finV.open(szTempFileName);
		if (false == finV.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}

		bool bFileEndFlag = false;
		do
		{
			MEMSET_ZERO(buffer, SIZE_BUFF);
			finV.getline(buffer, SIZE_BUFF);
			bFileEndFlag = finV.eof();
		
			char		buffLine[SIZE_BUFF];
			char		seps[] = "_zip";
			STRNCPY_MEMSET(buffLine, buffer, SIZE_BUFF);
			if(0 == strnicmp(buffLine, "#", 1))
			{// 2007-01-09 by cmkwon, 주석임
				continue;
			}

			// from version
			char *token1 = strtok(buffLine, seps);
			// to version
			char *token2 = strtok(NULL, seps);

			if(token1 && token2)
			{
				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);
				if(OldVersion.IsValidVersionInfo()
					&& NewVersion.IsValidVersionInfo()
					&& OldVersion < NewVersion)
				{
					VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
					if (pVersion == NULL)
					{
						o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
					}
					else
					{
						if (*pVersion < NewVersion)
						{
							*pVersion = NewVersion;
						}
					}
				}
			}

		} while(false == bFileEndFlag);
		finV.close();
	}

	return TRUE;
}

CString CPreIOCP::GetOnlyPathWithoutFileName(CString & i_szFilePath)
{
	if (i_szFilePath.ReverseFind('/') != -1)
	{
		return i_szFilePath.Left(i_szFilePath.ReverseFind('/'));
	}

	return ""; 
}

BOOL CPreIOCP::AutoUpdateVersionByPreSvr()
{
	// PreServer 시작 시 자동 버젼 업데이트 갱신
	STRNCPY_MEMSET(m_szDownloadHttpServerIP, g_pPreGlobal->GetRandomDownloadHttpIP(), SIZE_MAX_FTP_URL);
	mt_auto_lock mtV(&m_mtmapVersionOld2New);
	ez_map<VersionInfo, VersionInfo> mapVersionOld2New;
	
	switch(g_pPreGlobal->GetAutoUpdateServerType())
	{
	case AU_SERVER_TYPE_HTTP:
		{
			if(FALSE == PreGetVersionInfoByHTTP(&mapVersionOld2New, &m_LatestLauncherVersion, &m_LatestDeleteFileListVersion, &m_LatestNoticeVersion))
			{
				return FALSE;
			}
		}
		break;
	default:
		{
			if (FALSE == PreGetVersionInfoFromFTP(&mapVersionOld2New, &m_LatestLauncherVersion, &m_LatestDeleteFileListVersion, &m_LatestNoticeVersion))
			{
				return FALSE;
			}
		}
	}
	mtV.auto_unlock_cancel();
	
	m_mtmapVersionOld2New.clear();
	ez_map<VersionInfo, VersionInfo>::iterator itr = mapVersionOld2New.begin();
	for(; itr != mapVersionOld2New.end(); itr++)
	{
		m_mtmapVersionOld2New.insertLock(itr->first, itr->second);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-09 by cmkwon
	if (false == m_mtmapVersionOld2New.empty())
	{// 2007-01-09 by cmkwon, 마지막 버젼 설정
		m_LatestClientVersion		= m_mtmapVersionOld2New.rbegin()->second;
	}
	
	///////////////////////////////////////////////////////////////////////////////	
	// write versions
	this->WriteVersionInfoToLocalFile();

	return TRUE;
}

// 2012-07-11 by hskim, 선택적 셧다운
BOOL CPreIOCP::SelectiveShutdownProgress(char *pAccountName)
{
	CAccountInfo tmPreAccountInfo;
	m_AccountInfoMap.lock();
	BOOL bReted = GetAccountInfo(&tmPreAccountInfo, pAccountName);

	if(bReted)
	{
		DeleteAccountInfo(pAccountName);
	
		SendErrorToFieldServer(tmPreAccountInfo.CurrentServerGroup, tmPreAccountInfo.CurrentFieldServerID
			, T_PC_CONNECT_LOGIN, ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT
			, tmPreAccountInfo.CurrentClientIndex, 0, pAccountName);

		g_pPreGlobal->WriteSystemLogEX(TRUE, "[Notify] Apply SelectiveShutdown [%s:%s]\r\n", pAccountName, tmPreAccountInfo.SelectiveShutdownInfo);
	}

	m_AccountInfoMap.unlock();

	return TRUE;
}

BOOL CPreIOCP::SelectiveShutdownNotice(char *pAccountName, int nMin)
{
	UID32_t nAccountUID;
	CAccountInfo tmPreAccountInfo;
	m_AccountInfoMap.lock();
	BOOL bReted = GetAccountInfo(&tmPreAccountInfo, pAccountName);
	
	if(bReted)
	{
		nAccountUID = tmPreAccountInfo.AccountUniqueNumber;

		m_AccountInfoMap.unlock();

		INIT_MSG_WITH_BUFFER(MSG_IP_SELECTIVE_SHUTDOWN_NOTICE, T_IP_SELECTIVE_SHUTDOWN_NOTICE, pSMsg, SendBuf);
		pSMsg->AccountUID = nAccountUID;
		strncpy(pSMsg->m_szAccountName, pAccountName, sizeof(pSMsg->m_szAccountName));
		pSMsg->nRemainingTime = nMin;
		SendMsgToAllIMServer(SendBuf, MSG_SIZE(MSG_IP_SELECTIVE_SHUTDOWN_NOTICE));
	}
	else
	{
		m_AccountInfoMap.unlock();
	}

	return TRUE;
}
// end 2012-07-11 by hskim, 선택적 셧다운
