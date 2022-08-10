// LogServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
//#include "LogServerDlg.h"
#include "resource.h"		// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
#include "LogIOCP.h"
#include "VMemPool.h"
#include "LogGlobal.h"

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")


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
    wsprintf(szTemp, "LogServer Crash !! : Create dump file (Exception 0x%08x arised)", pExceptionInfo->ExceptionRecord->ExceptionCode);

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
#endif
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Place code here.
	g_pLogGlobal = new CLogGlobal;

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif
// end 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef ARENA
	if(FALSE == g_pLogGlobal->InitGlobal("Arena Log Server"))
#else
	if(FALSE == g_pLogGlobal->InitGlobal("Log Server"))
#endif //ARENA
	{
		return FALSE;
	}

	// set config root path
	g_pLogGlobal->LoadConfiguration();
#ifdef ARENA
	g_pLogGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "ArenaLogSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());
#else
	g_pLogGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "LogSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());
#endif //ARENA
	char szSystemLog[256];
	sprintf(szSystemLog, "Log Server Start\r\n\r\n");
	g_pLogGlobal->WriteSystemLog(szSystemLog);

	if(FALSE == CIOCP::SocketInit())
	{
		g_pLogGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain SocketInit error!!\r\n");
		return FALSE;
	}

	CIOCP::SocketInit();
	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_LOGSERVER_SESSION * 100);
	if(CVMemPool::vmPoolInit() == FALSE)
	{
		g_pLogGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain vmPoolInit error!!\r\n");
		return FALSE;
	}
#ifdef ARENA
	g_pLogGlobal->WndRegisterClass(hInstance, IDI_LOGSERVER, "Arena Log Server");
#else
	g_pLogGlobal->WndRegisterClass(hInstance, IDI_LOGSERVER, "Log Server");
#endif //ARENA
	// Perform application initialization:
#ifdef ARENA
	if (FALSE == g_pLogGlobal->InitInstance (hInstance, nCmdShow, "Arena Log Server", "Arena Log Server"))
#else
	if (FALSE == g_pLogGlobal->InitInstance (hInstance, nCmdShow, "Log Server", "Log Server"))
#endif //ARENA
	{
		return FALSE;
	}

	// Main message loop:
	MSG			msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CVMemPool::vmPoolClean();
	CIOCP::SocketClean();

	g_pLogGlobal->WriteSystemLogEX(TRUE, "Log Server End\r\n\r\n\r\n");

	SAFE_DELETE(g_pLogGlobal);
	return msg.wParam;
}

// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
/*			// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
			char szServerName[SIZE_MAX_STRING_128] = { 0, };
			sprintf(szServerName, "LogServer_%s", g_pLogGlobal->GetServerGroupName());
			SetWindowText(hWnd, szServerName);
			// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
			if(FALSE == g_pLogGlobal->InitServerSocket())
			{
				g_pLogGlobal->WriteSystemLogEX(TRUE, "[ERROR] LogServer call InitServerSocket() error !!\r\n");			
			}
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
	case WM_DESTROY:
		{
			g_pLogGlobal->EndServerSocket();
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

