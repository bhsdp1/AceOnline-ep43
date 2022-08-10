// VoIPGlobal.cpp: implementation of the CVoIPGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VoIPGlobal.h"
#include "ThreadCheck.h"
#include "IOCP.h"

//////////////////////////////////////////////////////////////////////
// Global 변수
//////////////////////////////////////////////////////////////////////
CVoIPGlobal				*g_pVoIPGlobal = NULL;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVoIPGlobal::CVoIPGlobal()
{
	if(g_pVoIPGlobal)
	{
		return;
	}
	g_pVoIPGlobal = this;		// 상위 클래스에서 사용 할 수 있도록 전역변수에 자신을 할당한다.

	m_hInstanceHandle			= NULL;
	m_hMainWndHandle			= NULL;
	m_nTimerIDTraffic			= 0;
	m_nTimerIDAliveCheck		= 0;
	m_nTimerIDReconnect			= 0;

	memset(m_szConfigRootPath, 0x00, MAX_PATH);
	memset(m_szIPLocal, 0x00, SIZE_MAX_IPADDRESS);
	m_nPortListening			= 0;
	
	m_pGIOCP					= NULL;
}

CVoIPGlobal::~CVoIPGlobal()
{
	this->EndServerSocket();
	g_pGlobal = NULL;			// 상위 클래스의 전역변수를 초기화 한다
}

void CVoIPGlobal::SetConfigRootPath(void)
{
	memset(m_szConfigRootPath, 0x00, MAX_PATH);	
	strncpy(m_szConfigRootPath, "./", 128);				// Default Path를 미리 설정해둠

	const int	SIZE_BUFF = 512;
	char		buff[SIZE_BUFF];
	const char	*token;
	char		seps[] = " \t";
	ifstream	fin;

	fin.open("./config_root.cfg");
	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		char	szDir[256];
		GetCurrentDirectory(256, szDir);
		sprintf(szError, "\r\n[WARNING] Cannot find 'config_root.cfg': set default path '%s', Current Directory'%s'\r\n\r\n", m_szConfigRootPath, szDir);
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

		strncpy(m_szConfigRootPath, token, 128);
	}

	DBGOUT("\r\nSET CONFIG ROOT PATH '%s'\r\n\r\n", m_szConfigRootPath);
}

char *CVoIPGlobal::GetConfigRootPath(void)
{
	return m_szConfigRootPath;
}

HINSTANCE CVoIPGlobal::GetInstanceHandle(void)
{
	return m_hInstanceHandle;
}

HWND CVoIPGlobal::GetMainWndHandle(void)
{
	return m_hMainWndHandle;
}

UINT CVoIPGlobal::GetTimerIDTraffic(void)
{
	return m_nTimerIDTraffic;
}

UINT CVoIPGlobal::GetTimerIDAliveCheck(void)
{
	return m_nTimerIDAliveCheck;
}

UINT CVoIPGlobal::GetTimerIDReconnect(void)
{
	return m_nTimerIDReconnect;
}

void CVoIPGlobal::SetIPLocal(char *i_szIP)
{
	memcpy(m_szIPLocal, i_szIP, SIZE_MAX_IPADDRESS);
}
char *CVoIPGlobal::GetIPLocal(void)
{
	return m_szIPLocal;
}

void CVoIPGlobal::SetPublicIPLocal(char *i_szIP)
{
	memcpy(m_szPublicIPLocal, i_szIP, SIZE_MAX_IPADDRESS);
}
char *CVoIPGlobal::GetPublicIPLocal(void)
{
	return m_szPublicIPLocal;
}

void CVoIPGlobal::SetPortListening(int i_nPort)
{
	m_nPortListening = i_nPort;
}
int CVoIPGlobal::GetPortListening(void)
{
	return m_nPortListening;
}

CIOCP *CVoIPGlobal::GetGIOCP(void)
{
	return m_pGIOCP;
}

BOOL CVoIPGlobal::InitServerSocket(void)
{
	return FALSE;
}

BOOL CVoIPGlobal::EndServerSocket(void)
{
	char szSystemLog[512];
	sprintf(szSystemLog, "CVoIPGlobal::EndServerSocket_1\r\n");
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
	
	return TRUE;
}


void CVoIPGlobal::WndRegisterClass(HINSTANCE hInstance, UINT IDI_Icon, char *szClass)
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

BOOL CVoIPGlobal::InitInstance(HINSTANCE hInstance, int nCmdShow, char *szClass, char *szWindowText)
{
   HWND hWnd;
   m_hInstanceHandle = hInstance; // Store instance handle in our global variable
   hWnd = CreateWindow(_T(szClass), _T(szWindowText), WS_OVERLAPPEDWINDOW,
      200, 100, 300, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   m_hMainWndHandle = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

BOOL CVoIPGlobal::StartTimerTraffic(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDTraffic)
	{
		return FALSE;
	}

	m_nTimerIDTraffic = ::SetTimer(m_hMainWndHandle, TIMERID_TRAFFIC, TIMERGAP_TRAFFIC, NULL);
	return TRUE;
}

void CVoIPGlobal::EndTimerTraffic(void)
{
	if(m_nTimerIDTraffic && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_TRAFFIC);
	}
	m_nTimerIDTraffic = 0;
}

BOOL CVoIPGlobal::StartTimerAliveCheck(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDAliveCheck)
	{
		return FALSE;
	}

	m_nTimerIDAliveCheck = ::SetTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK, TIMERGAP_ALIVE_CHECK, NULL);
	return TRUE;
}
void CVoIPGlobal::EndTimerAliveCheck(void)
{
	if(m_nTimerIDAliveCheck && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_ALIVE_CHECK);
	}
	m_nTimerIDAliveCheck = 0;
}


BOOL CVoIPGlobal::StartTimerReconnect(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDReconnect)
	{
		return FALSE;
	}

	m_nTimerIDReconnect = ::SetTimer(m_hMainWndHandle, TIMERID_RECONNECT, TIMERGAP_RECONNECT, NULL);
	return TRUE;
}
void CVoIPGlobal::EndTimerReconnect(void)
{
	if(m_nTimerIDReconnect && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_RECONNECT);
	}
	m_nTimerIDReconnect = 0;
}

void CVoIPGlobal::CheckServerThread(void)
{
	if(NULL == m_pGIOCP){		return;}

	DWORD dwExcludeThreadID = 0;	
	SThreadInfo *pSTInfo = m_pGIOCP->CheckIOCPThread(0);
	while (pSTInfo)
	{
		char szSysLog[256];
		sprintf(szSysLog, "[ERROR] deadlock : ThreadName(%10s), ThreadId(%5d), TimeGap(%5d) StartTick(%5d) SocketIndex(%4d) MessageType[%#04x]\r\n"
			, pSTInfo->GetThreadCheckComment(pSTInfo->enThreadCheckType)
			, pSTInfo->dwThreadId
			, timeGetTime() - pSTInfo->dwLastUseStartTick
			, pSTInfo->dwLastUseStartTick
			, pSTInfo->dwSocketIndex
			, pSTInfo->dwMessageType);
		CGlobal::ms_SystemLogManager.WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);

		pSTInfo = m_pGIOCP->CheckIOCPThread(pSTInfo->dwThreadId);
	}	
}

void CVoIPGlobal::CalculateIOCPTraffic(void)
{
	if(NULL == m_pGIOCP
		|| m_pGIOCP->GetListeningFlag() == FALSE)
	{
		return;
	}

	m_pGIOCP->CalcTotalTrafficInfo();					// Bandwidth 계산
}

void CVoIPGlobal::CheckClientAlive(void)
{
	if(NULL == m_pGIOCP
		|| m_pGIOCP->GetListeningFlag() == FALSE){	return;}

	m_pGIOCP->ClientCheck();
}