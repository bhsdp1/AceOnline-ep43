// VCServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "VCGlobal.h"
#include "VCIOCP.h"
#include "VMemPool.h"
#include "VoIPProtocol.h"
#include "C2DWinSocket.h"


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	g_pVCGlobal = new CVCGlobal;
	if(FALSE == g_pVCGlobal->InitGlobal("VoIP Call Server"))
	{
		return FALSE;
	}

	// LoadConfiguration
	if(FALSE == g_pVCGlobal->LoadConfiguration())
	{
		return FALSE;
	}

	g_pVCGlobal->GetSystemLogManagerPtr()->InitLogManger(TRUE
		, "VCSystem", (char*)(string(g_pVCGlobal->GetConfigRootPath()) + "../log/SystemLogVoIP/").c_str());

	char szSystemLog[256];
	sprintf(szSystemLog, "VoIP Call Server Start\r\n\r\n");
	g_pVCGlobal->WriteSystemLog(szSystemLog);

	if(FALSE == CIOCP::SocketInit())
	{
		return FALSE;
	}

	CVMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_VCSERVER_SESSION * 50);
	if(CVMemPool::vmPoolInit() == FALSE)
	{
		return FALSE;
	}

	g_pVCGlobal->WndRegisterClass(hInstance, IDI_VCSERVER, "VC Server");
	if (FALSE == g_pVCGlobal->InitInstance (hInstance, nCmdShow, "VC Server", "VC Server"))
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

	sprintf(szSystemLog, "VoIP Call Server End\r\n\r\n\r\n");
	DBGOUT(szSystemLog);
	g_pVCGlobal->WriteSystemLog(szSystemLog);

	SAFE_DELETE(g_pVCGlobal);
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
			g_pVCGlobal->CreateC2DWinSocket(hWnd);
			if(FALSE == g_pVCGlobal->ConnectC2DWinSocket(g_pVCGlobal->GetVDServerIP(), g_pVCGlobal->GetVDServerPort()))
			{
				MessageBox(hWnd, "ConnectC2DWinSocket Error", "ERROR", MB_OK);
			}						
		}
		break;
	case WM_C2D_ASYNC_EVENT:
		{
			g_pVCGlobal->OnAsyncEventC2DWinSocket(wParam, lParam);
		}
		break;
	case WM_C2D_PACKET_NOTIFY:
		{
			g_pVCGlobal->OnPacketNotifyC2DWinSocket(wParam);
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
			g_pVCGlobal->OnTimer(wParam);
		}
		break;
	case WM_DESTROY:
		{			
			g_pVCGlobal->EndServerSocket();
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
   return 0;
}

