// VTCGuardManager.cpp: implementation of the CVTCGuardManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumError.h"
#include "atumlauncher.h"
#include "VTCGuardManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Security/Guard4Launcher.h"

#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)

#if defined(_DEBUG)
#pragma comment(lib, "vt_launcher_mtd.lib")
//#define  PHIDOI_VTC_GUARD_SERVER_URL	"http://vtcguardtest.pd.vtc.vn/PhiDoi"		// 정상적으로 동작 안함
#define  PHIDOI_VTC_GUARD_SERVER_URL	"http://vtcguard.phidoi.vtc.vn/PhiDoi"
#else
#pragma comment(lib, "vt_launcher_mt.lib")
#define  PHIDOI_VTC_GUARD_SERVER_URL	"http://vtcguard.phidoi.vtc.vn/PhiDoi"
#endif

#endif	// SERVICE_TYPE_VIETNAMESE_SERVER_1

DWORD WINAPI UpdateThread(LPVOID lpParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVTCGuardManager::CVTCGuardManager()
{
	m_hUpdateThread				= NULL;
	m_bUpdateThreadCancelFlag		= FALSE;
	m_hUpdateWindow					= 0;

	ZeroMemory(m_szVTCAutoUpdateMsg, sizeof(m_szVTCAutoUpdateMsg));
	ZeroMemory(m_szFolderGame, sizeof(m_szFolderGame));
}

CVTCGuardManager::~CVTCGuardManager()
{
	if( m_hUpdateThread )
	{
		::CloseHandle(m_hUpdateThread);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HANDLE CVTCGuardManager::GetUpdateThreadHandle(void)
{
	return m_hUpdateThread;
}

void CVTCGuardManager::SetUpdateThreadCancelFlag(BOOL i_bCancelFlag)
{
	m_bUpdateThreadCancelFlag		= i_bCancelFlag;
}

BOOL CVTCGuardManager::ThreadEnd(DWORD i_dwWaitTimems/*=1000*/)
{
	BOOL bRet = TRUE;
	HANDLE hThread = this->GetUpdateThreadHandle();
	if(hThread)
	{
		DWORD dwRet;
		dwRet = WaitForSingleObject(hThread, i_dwWaitTimems);
		if(WAIT_OBJECT_0 != dwRet)
		{
			bRet = FALSE;

			// 리턴이 WAIT_FAILED임
			int nError = GetLastError();
			SetLastError(0);
		}
		Sleep(100);

		::CloseHandle(hThread);
		m_hUpdateThread		= NULL;
	}

	return bRet;
}

Err_t CVTCGuardManager::Update(BOOL i_bUseThread/*=FALSE*/, HWND i_hUpdateWindow/*=0*/)
{
	if(i_bUseThread)
	{
		m_hUpdateWindow		= i_hUpdateWindow;
		
		unsigned thrdaddr;
		m_hUpdateThread = chBEGINTHREADEX(NULL, 0, UpdateThread, (LPVOID)this, 0, &thrdaddr);

		if(NULL == m_hUpdateThread)
		{
			return FALSE;
		}

		return ERR_NO_ERROR;
	}

	return _Update();
}

Err_t CVTCGuardManager::_Update(void)
{
	char pszFullFileName[MAX_PATH]={ 0 };
	DWORD dwResult = 0;
	::ZeroMemory((PBYTE)pszFullFileName, MAX_PATH);
	dwResult = ::GetModuleFileName(NULL, pszFullFileName, MAX_PATH);

	char dir[_MAX_DIR] = { 0, }, drive[_MAX_DRIVE] = { 0, };
	_splitpath(pszFullFileName, drive, dir, NULL, NULL);
	_makepath(m_szFolderGame, drive, dir, NULL, NULL);

	char *szPath = m_szFolderGame;

#if defined(PHIDOI_VTC_GUARD_SERVER_URL)
	vt_Autoupdate(PHIDOI_VTC_GUARD_SERVER_URL, szPath, m_szVTCAutoUpdateMsg);
#endif

	return ERR_NO_ERROR;
}

void CVTCGuardManager::Run(char *i_szAppPath, char *i_szCmdParam)
{
	char VTCGuardParam[512] = { 0 };

	sprintf(VTCGuardParam, "%s\\%s#%s#KJSHDF8932N3B2M3NBCPS8DHLO63G9FV", m_szFolderGame, i_szAppPath, i_szCmdParam);

#if defined(PHIDOI_VTC_GUARD_SERVER_URL)
	vt_run(VTCGuardParam, m_szVTCAutoUpdateMsg);
#endif
}

DWORD WINAPI UpdateThread(LPVOID lpParam)
{
	CVTCGuardManager *pVTCGuardManager = (CVTCGuardManager*)lpParam;

	Err_t err = pVTCGuardManager->_Update();

	pVTCGuardManager->PostMessage2UpdateWindow(WM_UPDATE_VTCGUARD_OK, 0, 0);

	return 0x20;
}

void CVTCGuardManager::PostMessage2UpdateWindow(UINT i_uiMsgTy, WPARAM i_wParam, LPARAM i_lParam)
{
	if(m_hUpdateWindow)
	{
		::PostMessage(m_hUpdateWindow, i_uiMsgTy, i_wParam, i_lParam);
	}
}