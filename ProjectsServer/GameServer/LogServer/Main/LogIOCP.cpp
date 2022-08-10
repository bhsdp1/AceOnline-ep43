// LogIOCP.cpp: implementation of the CLogIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
//#include "LogGlobal.h"		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
#include "LogIOCP.h"
#include "LogIOCPSocket.h"
#include "IOCP.h"
#include "AtumLogDBManager.h"
#include "GLogDBManager.h"				// 2010-06-01 by shcho, GLogDB 관련 -
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){   return 0;}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogIOCP::CLogIOCP(int nPort, char *szLocalIP)
:CIOCP(0, SIZE_MAX_LOGSERVER_SESSION, nPort, szLocalIP, ST_LOG_SERVER)
{
	CLogIOCPSocket::ms_pLogIOCP		= this;

	CLogIOCPSocket * pLogIOCPSocket = new CLogIOCPSocket[m_dwArrayClientSize];
	m_pArrayIOCPSocket = pLogIOCPSocket;
	for(int i = 0; i < m_dwArrayClientSize; i++)
	{
		m_ArrayClient[i] = &pLogIOCPSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}
}

CLogIOCP::~CLogIOCP()
{
	ListenerClose();
	IOCPClean();

	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CLogIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}
}

BOOL CLogIOCP::IOCPInit()
{
/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	char szServerName[SIZE_MAX_STRING_128] = { 0, };
	sprintf(szServerName, "LogServer_%s", g_pLogGlobal->GetServerGroupName());
	if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
		return FALSE;
	}
	// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
	m_pAtumLogDBManager = new CAtumLogDBManager(this);
	if(m_pAtumLogDBManager->Init() == FALSE)
	{
		MessageBox(NULL, "m_pAtumLogDBManager initialize fail", "ERROR - SpaceCowboy LogServer", NULL);
		return FALSE;
	}

#if defined(S_GLOG_HSSON) || defined(S_GLOG_2ND_HSKIM) || defined(S_GLOG_3ND_KHK)		// 2012-10-19 by jhseol, Glog 3차 포함
	// 2010-06-01 by shcho, GLogDB 관련 -
	m_pGLogDBManager = new CGLogDBManager();
	if(m_pGLogDBManager->Init() == FALSE
		|| m_pGLogDBManager->InitGLogDB() == FALSE)
	{
		MessageBox(NULL, "m_pGLogDBManager initialize fail", "ERROR - SpaceCowboy LogServer", NULL);
		return FALSE;
	}
#endif // S_GLOG_HSSON

#ifdef ARENA
	this->m_GameLogManager.InitLogManger(TRUE, "ArenaLogGameLog", (char*)(string(CONFIG_ROOT) + "../log/GameLog/").c_str());
#else
	this->m_GameLogManager.InitLogManger(TRUE, "LogGameLog", (char*)(string(CONFIG_ROOT) + "../log/GameLog/").c_str());
#endif

	return CIOCP::IOCPInit();
}

void CLogIOCP::IOCPClean()
{
	CIOCP::IOCPClean();

	if (m_pAtumLogDBManager)
	{// 2006-05-19 by cmkwon
		m_pAtumLogDBManager->Clean();
		SAFE_DELETE(m_pAtumLogDBManager);
	}

	this->m_GameLogManager.Clean();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLogIOCP::WriteGameLog(char *i_szLogString)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-23 ~ 2006-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLogIOCP::WriteGameLog(char *i_szLogString)
{
	m_GameLogManager.WriteSystemLog(i_szLogString);
}