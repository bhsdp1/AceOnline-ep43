// AuthenticationServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "AuthenticationGlobal.h"
#include "AuthenticationIOCP.h"
#include "AuthenticationIOCPSocket.h"
#include "VMemPool.h"

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")
#endif

LONG __stdcall ExceptionHandler(_EXCEPTION_POINTERS* pExceptionInfo)
{
    char fileName[MAX_PATH];
    GetModuleFileName(NULL, fileName, sizeof(fileName));
    char* ext = strrchr(fileName, '.');

#ifdef S_SERVER_CRASH_FILENAME_ADD_INFO
	// 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
	ATUM_DATE_TIME	CurrentTime(TRUE);
	sprintf(ext ? ext : fileName+strlen(fileName), "_%s.dmp", CurrentTime.GetFileDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
	// end 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
#else
    strcpy(ext ? ext : fileName+strlen(fileName), ".dmp");
#endif

    char szTemp[256];
    wsprintf(szTemp, "AuthenticationServer Crash !! : Create dump file (Exception 0x%08x arised)", pExceptionInfo->ExceptionRecord->ExceptionCode);

    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessID = GetCurrentProcessId();
    HANDLE hFile = CreateFile(fileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = pExceptionInfo;
    eInfo.ClientPointers = FALSE;

    MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpWithFullMemory, pExceptionInfo ? &eInfo : NULL, NULL, NULL);

// 2012-10-18 by mspark, 게임포지 서버 충돌 시 메시지 박스 보이지 않는 작업
#ifdef S_SERVER_CRASH_MSGBOX_REMOVE_MSPARK
	//MessageBox(NULL, szTemp, "ERROR", MB_TOPMOST | MB_ICONSTOP);
#else
	MessageBox(NULL, szTemp, "ERROR", MB_TOPMOST | MB_ICONSTOP);
#endif
// end 2012-10-18 by mspark, 게임포지 서버 충돌 시 메시지 박스 보이지 않는 작업

	return EXCEPTION_EXECUTE_HANDLER;
}
// end 2012-04-20 by hskim 미니덤프 기능 추가

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 	// TODO: Place code here.
	g_pAuthenticationGlobal = new CAuthenticationGlobal;

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif
// end 2012-04-20 by hskim 미니덤프 기능 추가

	if(FALSE == g_pAuthenticationGlobal->InitGlobal("Authentication Server"))
	{
		return FALSE;
	}

	// set config root path
	g_pAuthenticationGlobal->SetConfigRootPath();
	g_pAuthenticationGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "AuthenticationSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());

	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "Authentication Server Start\r\n");

	if(FALSE == CIOCP::SocketInit())
	{
		g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain SocketInit error!!\r\n");
		return FALSE;
	}

	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_AUTHSERVER_SESSION * 10);
	CVMemPool::vmPoolAddObject(sizeof(CSendPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
	CVMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
	if(CVMemPool::vmPoolInit() == FALSE)
	{
		g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain vmPoolInit error!!\r\n");
		return FALSE;
	}

	g_pAuthenticationGlobal->WndRegisterClass(hInstance, IDI_AUTHENTICATIONSERVER, _T("Authentication Server"));
	// Perform application initialization:
	if (!g_pAuthenticationGlobal->InitInstance (hInstance, nCmdShow, _T("Authentication Server"), _T("Authentication Server")))
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////
	// Main message loop:
	MSG			msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CVMemPool::vmPoolClean();
	CIOCP::SocketClean();

	g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "Authentication Server End\r\n\r\n\r\n");

	SAFE_DELETE(g_pAuthenticationGlobal);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			SetLastError(0);

			g_pAuthenticationGlobal->SetMainWndHandle(hWnd);

			if(FALSE == g_pAuthenticationGlobal->LoadConfiguration())
			{
 				MessageBox(hWnd, "LoadConfiguration Error", "ERROR", MB_OK);
 				exit(0);
				break;
			}

			if(FALSE == g_pAuthenticationGlobal->InitServerSocket())
			{
				g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "[ERROR] WndProc InitServerSocket_ error !!");
			}
			else
			{
				g_pAuthenticationGlobal->StartTimerTraffic();
				g_pAuthenticationGlobal->StartTimerAliveCheck();
			}

			break;
		}
		break;

	case WM_COMMAND:
		{
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		{
			switch(wParam)
			{
			case TIMERID_TRAFFIC:
				{
					g_pAuthenticationGlobal->CheckServerThread();
					g_pAuthenticationGlobal->CalculateIOCPTraffic();
				}
				break;
			case TIMERID_ALIVE_CHECK:
				{
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		{
			g_pAuthenticationGlobal->EndServerSocket();
			PostQuitMessage(0);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
   return 0;
}
