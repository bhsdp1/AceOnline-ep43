// VTCThread.cpp: implementation of the CVTCThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTCThread.h"
#include "AtumApplication.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVTCThread::CVTCThread()
{

}

CVTCThread::~CVTCThread()
{

}

DWORD CVTCThread::Run()
{
	Sleep( GUARD_TIME );
	while( !m_bThreadMustStop )	// 2015-07-08 Future, added proper Shutdown of threads
	{
 		if(g_pD3dApp->m_fnVTCDLLGetStatus())
 		{	
			//MessageBox(NULL,"VTC m_fnVTCDLLGetStatus","m_fnVTCDLLGetStatus",MB_OK);
 		}
 		else
 		{
 			g_pD3dApp->m_dwGameState = _ENDGAME;
 			//MessageBox(NULL,"VTC Exit","Exit",MB_OK);
 			g_pD3dApp->m_fnVTCDLLExit();
 		}
		Sleep( GUARD_TIME );
	}
	m_bRunning = FALSE;
	return 0;
}