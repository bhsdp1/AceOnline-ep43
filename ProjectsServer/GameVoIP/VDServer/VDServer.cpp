// VDServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "VDGlobal.h"
#include "VDIOCP.h"
#include "VMemPool.h"



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// TODO: Place code here.
	g_pVDGlobal = new CVDGlobal;
	if(FALSE == g_pVDGlobal->InitGlobal("VoIP Distribute Server"))
	{
		return FALSE;
	}

	// LoadConfiguration
	if(FALSE == g_pVDGlobal->LoadConfiguration())
	{
		return FALSE;
	}

	g_pVDGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE
		, "VDSystem", (char*)(string(g_pVDGlobal->GetConfigRootPath()) + "../log/SystemLogVoIP/").c_str());

	char szSystemLog[256];
	sprintf(szSystemLog, "VoIP Distribute Server Start\r\n\r\n");
	g_pVDGlobal->WriteSystemLog(szSystemLog);

	if(FALSE == CIOCP::SocketInit())
	{
		return FALSE;
	}

	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_VDSERVER_SESSION * 10);
	if(CVMemPool::vmPoolInit() == FALSE)
	{
		return FALSE;
	}

	g_pVDGlobal->WndRegisterClass(hInstance, IDI_VDSERVER, "VD Server");
	if (FALSE == g_pVDGlobal->InitInstance (hInstance, nCmdShow, "VD Server", "VD Server"))
	{
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// Main message loop
	MSG			msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CVMemPool::vmPoolClean();
	CIOCP::SocketClean();

	sprintf(szSystemLog, "VoIP Distribute Server End\r\n\r\n\r\n");
	DBGOUT(szSystemLog);
	g_pVDGlobal->WriteSystemLog(szSystemLog);

	SAFE_DELETE(g_pVDGlobal);
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
			::SetLastError(0);

			if(FALSE == g_pVDGlobal->InitServerSocket())
			{
				MessageBox(hWnd, "InitServerSocket Error", "ERROR", MB_OK);
			}
			else
			{
				g_pVDGlobal->StartTimerTraffic();
				g_pVDGlobal->StartTimerAliveCheck();
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
	case WM_TIMER:
		{
			switch(wParam)
			{
			case TIMERID_TRAFFIC:
				{
					g_pVDGlobal->CheckServerThread();
					g_pVDGlobal->CalculateIOCPTraffic();
				}
				break;
			case TIMERID_ALIVE_CHECK:
				{					
					g_pVDGlobal->CheckClientAlive();
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			g_pVDGlobal->EndServerSocket();
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}