// IMServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "IMGlobal.h"
#include "IMIOCP.h"
#include "IMIOCPSocket.h"
#include <fstream>
#include "PreIMWinsocket.h"
#include "config.h"
#include "VMemPool.h"

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
    wsprintf(szTemp, "IMServer Crash !! : Create dump file (Exception 0x%08x arised)", pExceptionInfo->ExceptionRecord->ExceptionCode);

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
	g_pIMGlobal = new CIMGlobal;

// 2012-04-20 by hskim 미니덤프 기능 추가
#ifdef S_MINI_DUMP_HSKIM
	SetUnhandledExceptionFilter(&ExceptionHandler);
#endif
// end 2012-04-20 by hskim 미니덤프 기능 추가

#ifdef ARENA
	if(FALSE == g_pIMGlobal->InitGlobal("Arena IM Server"))
#else
	if(FALSE == g_pIMGlobal->InitGlobal("IM Server"))
#endif //ARENA
	{
		return FALSE;
	}

 	// TODO: Place code here.
	MSG			msg;

	// set config root path
	g_pIMGlobal->SetConfigRootPath();
#ifdef ARENA
	g_pIMGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "ArenaIMSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());
#else
	g_pIMGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE, "IMSystem", (char*)(string(CONFIG_ROOT) + "../log/SystemLog/").c_str());
#endif //ARENA
	char szSystemLog[256];
	sprintf(szSystemLog, "IM Server Start\r\n\r\n");
	g_pIMGlobal->WriteSystemLog(szSystemLog);

	if(FALSE == CIOCP::SocketInit())
	{
		return FALSE;
	}
	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_IMSERVER_SESSION * 10);
	CVMemPool::vmPoolAddObject(sizeof(CSendPacket), 1000);
	CVMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);	

	if(CVMemPool::vmPoolInit() == FALSE)
	{
		return FALSE;
	}
#ifdef ARENA
	g_pIMGlobal->WndRegisterClass(hInstance, IDI_IMSERVER, _T("Arena IM Server"));
#else
	g_pIMGlobal->WndRegisterClass(hInstance, IDI_IMSERVER, _T("IM Server"));
#endif //ARENA
	// Perform application initialization:
#ifdef ARENA
	if (FALSE == g_pIMGlobal->InitInstance (hInstance, nCmdShow, _T("Arena IM Server"), _T("Arena IM Server")))
#else
	if (FALSE == g_pIMGlobal->InitInstance (hInstance, nCmdShow, _T("IM Server"), _T("IM Server")))
#endif //ARENA
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CVMemPool::vmPoolClean();
	CIOCP::SocketClean();

	sprintf(szSystemLog, "IM Server End\r\n\r\n\r\n");
	g_pIMGlobal->WriteSystemLog(szSystemLog);

	SAFE_DELETE(g_pIMGlobal);
	return msg.wParam;
}




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
			SetLastError(0);
			if(g_pIMGlobal->LoadConfiguration())
			{
/*				// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
				char szServerName[SIZE_MAX_STRING_128] = { 0, };
				sprintf(szServerName, "IMServer_%s", g_pIMGlobal->GetServerGroupName());
				SetWindowText(hWnd, szServerName);
				// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
				g_pIMGlobal->CreateIM2PreWinSokcet(hWnd);
				g_pIMGlobal->GetIM2PreWinSocket()->Connect(g_pIMGlobal->GetIPPreServer(), g_pIMGlobal->GetPortPreServer());
			}
			else
			{
				MessageBox(hWnd, "LoadConfiguration Error", "ERROR", MB_OK);
			}
		}
		break;
	case WM_PREIM_ASYNC_EVENT:
		{
			if(g_pIMGlobal->GetIM2PreWinSocket()){ g_pIMGlobal->GetIM2PreWinSocket()->OnAsyncEvent(lParam);}
		}
		break;
	case WM_PREIM_PACKET_NOTIFY:
		{
			switch(LOWORD(wParam))
			{
			case CWinSocket::WS_ERROR:
				{
				}
				break;
			case CWinSocket::WS_CONNECTED:
				{
					g_pIMGlobal->EndTimerReconnect();
					if(HIWORD(wParam) == FALSE)
					{
						char	szSystemLog[256];
						sprintf(szSystemLog, "[Error] WndProc(), Can't connect to PreServer[%15s:%4d]\r\n",
							g_pIMGlobal->GetIPPreServer(), g_pIMGlobal->GetPortPreServer());
						g_pIMGlobal->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);

						g_pIMGlobal->StartTimerReconnect();
					}
					else
					{
						// 2007-12-18 by cmkwon, 시스템 로그 추가
						g_pIMGlobal->WriteSystemLogEX(TRUE, STRMSG_S_I2PRECONNECT_0000);

						if(g_pIMGlobal->InitServerSocket())
						{
							g_pIMGlobal->StartTimerTraffic();
							g_pIMGlobal->StartTimerAliveCheck();

							// send MSG_IP_CONNECT_IM_CONNECT
							INIT_MSG_WITH_BUFFER(MSG_IP_CONNECT_IM_CONNECT, T_IP_CONNECT_IM_CONNECT, pIMConnect, buf);
							strncpy(pIMConnect->ServerGroupName, g_pIMGlobal->GetServerGroupName(), SIZE_MAX_SERVER_NAME);
// 2006-05-10 by cmkwon, 아래와 같이 수정함 - IP도 포함시켜서 전송
//							pIMConnect->IMServerListenPort	= g_pIMGlobal->GetPortListening();
// 2006-05-23 by cmkwon, 아래와 같이 수정함
//							pIMConnect->IMServerID.SetValue(g_pIMGlobal->GetIPLocal(), g_pIMGlobal->GetPortListening());
							pIMConnect->IMServerID.SetValue(g_pIMGlobal->GetPublicIPLocal(), g_pIMGlobal->GetPortListening());
							g_pIMGlobal->GetIM2PreWinSocket()->Write(buf, MSG_SIZE(MSG_IP_CONNECT_IM_CONNECT));
						}
					}
				}
				break;
			case CWinSocket::WS_CLOSED:
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, STRMSG_S_I2PRECONNECT_0001,
						g_pIMGlobal->GetIPPreServer(), g_pIMGlobal->GetPortPreServer());
					g_pIMGlobal->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);

					g_pIMGlobal->EndServerSocket();
					g_pIMGlobal->StartTimerReconnect();
				}
				break;
			}
		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case TIMERID_TRAFFIC:
				{
					g_pIMGlobal->CheckServerThread();
					g_pIMGlobal->CalculateIOCPTraffic();
				}
				break;
			case TIMERID_ALIVE_CHECK:
				{
					//////////////////////////////////////////////////////
					// PreServer로 Alive 패킷을 전송한다
					if(g_pIMGlobal->GetIM2PreWinSocket() 
						&& g_pIMGlobal->GetIM2PreWinSocket()->IsConnected())
					{
						int nSendType = T_IP_CONNECT_ALIVE;
						g_pIMGlobal->GetIM2PreWinSocket()->Write((char*)&nSendType, SIZE_FIELD_TYPE_HEADER);

						g_pIMGlobal->StartTimerSendServerGroupInfo();
					}
					g_pIMGlobal->CheckClientAlive();
				}
				break;
			case TIMERID_RECONNECT:
				{					
					g_pIMGlobal->EndTimerReconnect();
					if(g_pIMGlobal->GetIM2PreWinSocket())
					{
						g_pIMGlobal->GetIM2PreWinSocket()->Connect(g_pIMGlobal->GetIPPreServer(), g_pIMGlobal->GetPortPreServer());
					}
				}
			case TIMERID_SERVER_GROUP_INFO:
				{
					if(g_pIMGlobal->GetIM2PreWinSocket() 
						&& g_pIMGlobal->GetIM2PreWinSocket()->IsConnected()
						&& g_pIMGlobal->GetGIOCP() 
						&& g_pIMGlobal->GetGIOCP()->GetListeningFlag())
					{
						INIT_MSG_WITH_BUFFER(MSG_IP_GET_SERVER_GROUP_INFO_ACK, T_IP_GET_SERVER_GROUP_INFO_ACK, pSACK, SendBuf);
						strncpy(pSACK->ServerGroupName, g_pIMGlobal->GetServerGroupName(), SIZE_MAX_SERVER_NAME);
						pSACK->IMCurrentUserCounts	= ((CIMIOCP*)g_pIMGlobal->GetGIOCP())->m_MapAccountUniqueNumber.size();
						pSACK->IMMaxUserCounts		= ((CIMIOCP*)g_pIMGlobal->GetGIOCP())->m_uiIMMaxTotalUserCounts;
						g_pIMGlobal->GetIM2PreWinSocket()->Write(SendBuf, MSG_SIZE(MSG_IP_GET_SERVER_GROUP_INFO_ACK));
					}
				}
				break;
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
			g_pIMGlobal->EndServerSocket();
			g_pIMGlobal->DestroyIM2PreWinSocket();
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


