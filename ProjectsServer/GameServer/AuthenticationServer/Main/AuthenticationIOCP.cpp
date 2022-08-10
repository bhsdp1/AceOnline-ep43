// AuthenticationIOCP.cpp: implementation of the CAuthenticationIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuthenticationIOCP.h"
#include "AuthenticationIOCPSocket.h"
#include "Wininet.h"
#include "config.h"
#include "AuthenticationGlobal.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuthenticationIOCP::CAuthenticationIOCP(int nPort, char *szLocalIP)
:CIOCP(0, SIZE_MAX_AUTHSERVER_SESSION, nPort, szLocalIP, ST_AUTHENTICATION_SERVER)
{
	CAuthenticationIOCPSocket::ms_pAuthenticationIOCP		= this;
	
	CAuthenticationIOCPSocket * pAuthenticationIOCPSocket = new CAuthenticationIOCPSocket[m_dwArrayClientSize];
	m_pArrayIOCPSocket = pAuthenticationIOCPSocket;
	for(int i = 0; i < m_dwArrayClientSize; i++)
	{
		m_ArrayClient[i] = &pAuthenticationIOCPSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}

	m_pAtumDBManager		= NULL;
	m_pAuthenticationTickManager		= NULL;
}

CAuthenticationIOCP::~CAuthenticationIOCP()
{
	ListenerClose();
	IOCPClean();
	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CAuthenticationIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}

	SAFE_DELETE(m_pAuthenticationTickManager);
	SAFE_DELETE(m_pAtumDBManager);
}

BOOL CAuthenticationIOCP::IOCPInit(void)
{
	// 2009-10-13 by cmkwon, AuthenticationServer 시스템 로그 추가 - 
	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationIOCP::IOCPInit# 10000 \r\n");

	///////////////////////////////////////////////////////////////////////////////
	// Init DBManager
	m_pAtumDBManager = new CAtumAuthenticationDBManager(this);
	if(m_pAtumDBManager->Init() == FALSE)
	{
		MessageBox(NULL, "m_pAtumDBManager initialize fail", "ERROR - SpaceCowboy AuthenticationServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, AuthenticationServer 시스템 로그 추가 - 
	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationIOCP::IOCPInit# 15000 CAtumAuthenticationDBManager::Init# !\r\n");

	// Init TickManager
	m_pAuthenticationTickManager = new CAuthenticationTickManager(this);
	if (m_pAuthenticationTickManager->InitTickManager() == FALSE)
	{
		MessageBox(NULL, "m_pAuthenticationTickManager initialize fail", "ERROR - SpaceCowboy AuthenticationServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, AuthenticationServer 시스템 로그 추가 - 
	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationIOCP::IOCPInit# 16000 CAuthenticationTickManager::InitTickManager# !\r\n");

	// 2009-10-13 by cmkwon, AuthenticationServer 시스템 로그 추가 - 
	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationIOCP::IOCPInit# 20000 end !\r\n");

	CIOCP::IOCPInit();

	return TRUE;
}

void CAuthenticationIOCP::IOCPClean(void)
{
	CIOCP::IOCPClean();

	if (m_pAtumDBManager)
	{
		m_pAtumDBManager->Clean();
		SAFE_DELETE(m_pAtumDBManager);
	}

	if (m_pAuthenticationTickManager != NULL)
	{
		m_pAuthenticationTickManager->CleanTickManager();
		SAFE_DELETE(m_pAuthenticationTickManager);
	}
}

SThreadInfo *CAuthenticationIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
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
		msgChangeSS->ServerType = ST_AUTHENTICATION_SERVER;
		msgChangeSS->ServerState = SERVER_STATE_ABNORMAL;
		SendMessageToAdminTool(SendBuf, MSG_SIZE(MSG_PA_ADMIN_CHANGE_SERVER_STATE));		// 2006-05-18 by cmkwon, 버그 수정
	}
	return pTInfo;
}

void CAuthenticationIOCP::OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{	
}
