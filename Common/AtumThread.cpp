#include "stdafx.h"
#include "AtumThread.h"

#include <process.h>

CAtumThread::CAtumThread()
{
	// 초기화
	m_hWorkerThread = NULL;
	m_bShutDownFlag = FALSE;
	m_dwThreadID = 0;
	m_hAtumThreadEvent = NULL;
}

CAtumThread::~CAtumThread()
{
	CleanThread();
}

DWORD WINAPI __WorkerThread(LPVOID lpParam)
{
	if (lpParam == NULL)
	{
		return 0x100;
	}

	return ((CAtumThread*)lpParam)->ThreadWorkerWrapper();
}

BOOL CAtumThread::InitThread()
{
	if (m_hWorkerThread != NULL)
	{
		return FALSE;
	}

	// 초기화
	m_hWorkerThread = NULL;
	m_bShutDownFlag = FALSE;
	m_dwThreadID = 0;

	// create event
	m_hAtumThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// begin thread
	m_hWorkerThread = chBEGINTHREADEX(NULL, 0, __WorkerThread, (LPVOID)this, 0, &m_dwThreadID);
	if (m_hWorkerThread == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAtumThread::CleanThread()
{
	if (m_hWorkerThread == NULL)
	{
		return FALSE;
	}

	DWORD dwRet;

	// Set ShutDownFlag
	m_bShutDownFlag = TRUE;

	// set evnet
	SetEvent(m_hAtumThreadEvent);

	// Wait for threads to quit
	dwRet = WaitForSingleObject(m_hWorkerThread, INFINITE);

	if(WAIT_OBJECT_0 == dwRet)
	{
		// 모든 Thread가 signaled 상태가 됨, 모두 정상 종료
	}
	else if(WAIT_ABANDONED_0 == dwRet)
	{
		// 모든 Thread가 signaled 상태가 됨, 적어도 하나 이상의 object가 버려짐
	}
	else if(WAIT_TIMEOUT == dwRet)
	{
		// 모든 Thread가 signaled 상태가 되지 못함, 설정된 Time을 벗어남
	}
	else
	{
		// 리턴이 WAIT_FAILED임
		int nError = GetLastError();
	}

	// Close event handle
	if (m_hAtumThreadEvent != NULL)
	{
		CloseHandle(m_hAtumThreadEvent);
	}

	// close thread handle
	if (m_hWorkerThread != NULL)
	{
		CloseHandle(m_hWorkerThread);
	}

	// 초기화
	m_hWorkerThread = NULL;
	m_bShutDownFlag = FALSE;
	m_dwThreadID = 0;
	m_hAtumThreadEvent = NULL;

	return TRUE;
}

DWORD CAtumThread::ThreadWorkerWrapper()
{
	DWORD dwRet = ThreadWorker();

	return dwRet;
}