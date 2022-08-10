// PreServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "PreGlobal.h"
#include "PreIOCP.h"
#include "PreIOCPSocket.h"
#include "VMemPool.h"
#include "SPToMPWinSocket.h"		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 

// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 임시용
// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(121.134.114.)

// start 2011-06-22 by hskim, 사설 서버 방지
// 공용으로 쓰기위해 PreGlobal.h 로 이동
//#ifdef  S_AUTHENTICATION_SERVER_HSKIM		// 2011-01-26 by hskim, 인증 서버 구현
//#define MasangPreServerIP		"c3.eyehs.kr"		// 테스트용 URL
//#define MasangPreServerPort		14999
//#else
//#define MasangPreServerIP		"61.39.170.131"
//#define MasangPreServerPort		15100
//#endif
// end 2011-06-22 by hskim, 사설 서버 방지

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
    wsprintf(szTemp, "PreServer Crash !! : Create dump file (Exception 0x%08x arised)", pExceptionInfo->ExceptionRecord->ExceptionCode);

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
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 	// TODO: Place code here.
	g_pPreGlobal = new CPreGlobal;

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif
// end 2012-04-20 by hskim 미니덤프 기능 추가
	
	if(FALSE == g_pPreGlobal->InitGlobal("Pre Server"))
	{
		return FALSE;
	}

	// set config root path
	g_pPreGlobal->SetConfigRootPath();
	g_pPreGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "PreSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());

	g_pPreGlobal->WriteSystemLogEX(TRUE, "Pre Server Start\r\n");

	if(FALSE == CIOCP::SocketInit())
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain SocketInit error!!\r\n");
		return FALSE;
	}

	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_PRESERVER_SESSION * 10);
	CVMemPool::vmPoolAddObject(sizeof(CSendPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
	CVMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
	if(CVMemPool::vmPoolInit() == FALSE)
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] WinaMain vmPoolInit error!!\r\n");
		return FALSE;
	}

	g_pPreGlobal->WndRegisterClass(hInstance, IDI_PRESERVER, _T("Pre Server"));
	// Perform application initialization:
	if (!g_pPreGlobal->InitInstance (hInstance, nCmdShow, _T("Pre Server"), _T("Pre Server")))
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

	g_pPreGlobal->WriteSystemLogEX(TRUE, "Pre Server End\r\n\r\n\r\n");

	SAFE_DELETE(g_pPreGlobal);
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
// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 아래와 같이 수정 함, g_pPreGlobal->InitServerSocket() 처리를 MasangPreServer 로 연결 후 처리하게 변경함
// 			if(g_pPreGlobal->LoadConfiguration())
// 			{
// 				if(g_pPreGlobal->InitServerSocket())
// 				{
// 					g_pPreGlobal->SetMainWndHandle(hWnd);
// 					g_pPreGlobal->StartTimerTraffic();
// 					g_pPreGlobal->StartTimerAliveCheck();
// 				}
// 			}
// 			else
// 			{
// 				MessageBox(hWnd, "LoadConfiguration Error", "ERROR", MB_OK);
// 				exit(0);
// 			}			
			
			// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
			g_pPreGlobal->SetMainWndHandle(hWnd);

			if(FALSE == g_pPreGlobal->LoadConfiguration())
			{
 				MessageBox(hWnd, "LoadConfiguration Error", "ERROR", MB_OK);
 				exit(0);
				break;
			}
/*
			// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
			char szServerName[SIZE_MAX_STRING_128] = { 0, };
			sprintf(szServerName, "PreServer_%s", g_pPreGlobal->GetServerGroupName());
			SetWindowText(hWnd, szServerName);
			// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
#ifdef S_AUTHENTICATION_SERVER_HSKIM		// 2011-01-26 by hskim, 인증 서버 구현
			// _ATUM_AUTHENTICATION_SERVER 정의시 기존 인증서버시 사용 안함 (Authentication Server 프로젝트로 따로 구축)
#else
			if(0 == stricmp(MasangPreServerIP, g_pPreGlobal->GetPublicIPLocal()))				
			{// 2008-02-22 by cmkwon, 자신이 MasangPreServer 이면 바로 서비스를 시작한다.

				// start 2011-06-22 by hskim, 사설 서버 방지
				if(NULL != g_pPreGlobal->GetGIOCP() ) 
				{
					break;
				}
				// end 2011-06-22 by hskim, 사설 서버 방지

				if(FALSE == g_pPreGlobal->InitServerSocket())
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] WndProc InitServerSocket_ error !!");
				}
				else
				{
					g_pPreGlobal->StartTimerTraffic();
					g_pPreGlobal->StartTimerAliveCheck();
				}
				break;
			}
#endif

			// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
			if(FALSE == g_pPreGlobal->createPreToAllServer(hWnd))
			{
				g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] createPreToAllServer_ error !!");
				exit(0);
				break;
			}
			if(FALSE == g_pPreGlobal->connectBySPToMPWinSoc(MasangPreServerIP, MasangPreServerPort))
			{
// start 2011-07-26 by hskim, 인증 서버 구현
//////////
// 기존
// 				g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] connectBySPToMPWinSoc_ error !! IP(%s) Port(%d)", MasangPreServerIP, MasangPreServerPort);
// 				exit(0);
//				break;

//////////
// 수정
#ifdef S_AUTHENTICATION_SERVER_2ND_HSKIM
 				g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] connectBySPToMPWinSoc_ error !! IP(%s) Port(%d)", MasangPreServerIP, MasangPreServerPort);
 				exit(0);
				break;
#else
				// 도메인등 접속 실패 시 서버 바로 실행

				if(NULL != g_pPreGlobal->GetGIOCP() ) 
				{
					break;
				}

				if(FALSE == g_pPreGlobal->InitServerSocket())
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] WndProc InitServerSocket_ error !!");
				}
				else
				{
					g_pPreGlobal->StartTimerTraffic();
					g_pPreGlobal->StartTimerAliveCheck();
				}
				break;
#endif
// end 2011-07-26 by hskim, 인증 서버 구현
			}

			// 2012-10-05 by jhseol, NPlay PC방 모듈 - 객체 생성 및 NPlay 서버 Connect
#ifdef S_NPLAY_PCBANG_MODULE_JHSEOL
			if( 0 != stricmp("", g_pPreGlobal->GetNPlayServerIP_Real() ) )
			{
				if( FALSE == g_pPreGlobal->createNPlayPCBangModule(hWnd, g_pPreGlobal->GetNPlayServerIP_Real(), g_pPreGlobal->GetNPlayServerIP_BackUP() ) )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] createNPlayPCBangModule_ error !!");
				}
				if( FALSE == g_pPreGlobal->connectByNPlayPCBangModuleWinSoc(g_pPreGlobal->GetNPlayServerIP_Real() ,NPLAY_SERVER_PORT) )
				{
					g_pPreGlobal->WriteSystemLogEX(TRUE, "[ERROR] connectByNPlayPCBangModuleWinSoc_ Connect Failed !!");
				}
			}
			// end 2012-10-05 by jhseol, NPlay PC방 모듈 - 객체 생성 및 NPlay 서버 Connect
#endif
			
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
					g_pPreGlobal->CheckServerThread();
					g_pPreGlobal->CalculateIOCPTraffic();
				}
				break;
			case TIMERID_ALIVE_CHECK:
				{
					// 2005-01-04 by cmkwon, 다음주 업데이트에 수정해야함, 런처에서 AlivePacket을 전송하지 않는다
					g_pPreGlobal->CheckClientAlive();
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			g_pPreGlobal->EndServerSocket();
			PostQuitMessage(0);
		}
		break;
	case WM_ASYNC_EVENT:	// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
		{
			g_pPreGlobal->OnAsyncEventOfSPToMPWinSoc(wParam, lParam);
		}
		break;
	case WM_PACKET_NOTIFY:	// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
		{
			g_pPreGlobal->OnPacketNotifyOfSPToMPWinSoc(LOWORD(wParam), HIWORD(wParam));
		}
		break;
		// 2012-10-05 by jhseol, NPlay pc방 모듈
	case WM_ASYNC_EVENT_BY_NPLAY:
		{
			g_pPreGlobal->OnAsyncEventOfNPlayPCBangModuleWinSoc(wParam, lParam);
		}
		break;
	case WM_PACKET_NOTIFY_BY_NPLAY:
		{
			g_pPreGlobal->OnPacketNotifyOfNPlayPCBangModuleWinSoc(LOWORD(wParam), HIWORD(wParam));
		}
		break;
		// end 2012-10-05 by jhseol, NPlay pc방 모듈
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
   return 0;
}
