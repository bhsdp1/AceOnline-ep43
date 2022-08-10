// Atum.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Atum.h"
#include "AtumDlg.h"
#include "MGameDecryption.h"
#include "AtumError.h"
#include "AtumParam.h"			// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -
#include "RegistryControl.h"	// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtumApp

BEGIN_MESSAGE_MAP(CAtumApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumApp construction

CAtumApp::CAtumApp()
:CWinApp(REGISTRY_BASE_PATH)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumApp object

CAtumApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAtumApp initialization

BOOL CAtumApp::InitInstance()
{
// 2008-03-03 by cmkwon, manifest 적용 - 
//	AfxEnableControlContainer();
	InitCommonControls();    // 2008-03-03 by cmkwon, manifest 적용 - initialize common control library

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 - 함수 호출
	UpdateEXE1FilePathResitry();

	// XP ServicePack2 Disable
	DisableXPServicePack2();	
	SetRegistryKey(STRMSG_REG_STRING_REGISTRYKEY_NAME);

// 2007-03-29 by cmkwon, 필요없음
//#ifdef _MGAME_RELEASE
//	///////////////////////////////////////////////////////////////////////////////
//	// 2004-06-21 cmkwon 엠게임 웹에서 파라미터 받기<총 7개>
//	//	param1 : INET			==> 인터넷을 통해 실행 되었는지의 여부, SpaceCowboy는 항상 INET
//	//	param2 : 3850			==> 인크립션 시드값
//	//	param3 : 엠게임 계정	==> 엠게임에 로그인한 계정
//	//	param4 : 계정 인크립션된 문자열	==> 엠게임 계정을 param2로 인크립션한 문자열
//	//	param5 : 비번 인크립션된 문자열	==> 엠게임 비번을 인크립션한 문자열, 시드값은 param2와 다르다
//	//	param6 : 성별			==> 남자 = 1, 여자 = 2
//	//	param7 : 출생년도		==> 출생년도
//	if(__argc != 8)
//	{
//		DBGOUT("[Error] Parameter Count Error, Count(%d)\n", __argc);
//
//		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
//		return FALSE;
//	}
//
//	int nEnSeed=0;
//	STRNCPY_MEMSET(g_szMGameExcuteType, __argv[1], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameSeed, __argv[2], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameID, __argv[3], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameEncryptedID, __argv[4], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameEncryptedPWD, __argv[5], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameSEX, __argv[6], MGAME_MAX_PARAM_STRING_SIZE);
//	STRNCPY_MEMSET(g_szMGameYear, __argv[7], MGAME_MAX_PARAM_STRING_SIZE);
//
//
//	///////////////////////////////////////////////////////////////////////////////
//	// Mutex Check
//	HANDLE hMutex = NULL;	
//	hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, (LPTSTR)g_szMGameEncryptedPWD);  // 암호화된패스워드값을 넣는다
//	if(hMutex == NULL)
//	{
//		DBGOUT("[Error] Mutex Error\n");
//		return FALSE;
//	}	
//	ReleaseMutex(hMutex);
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// ExcuteType Check
//	if(strcmp(g_szMGameExcuteType, "INET"))
//	{
//		DBGOUT("[Error] Excute Type Error, Type(%s)\n", g_szMGameExcuteType);
//		return FALSE;
//	}
//
//	nEnSeed = atoi(g_szMGameSeed);
//	char szDecryptedID[MGAME_MAX_PARAM_STRING_SIZE];	
//	STRNCPY_MEMSET(szDecryptedID, (char *)decrypt((unsigned char*)g_szMGameEncryptedID, nEnSeed), MGAME_MAX_PARAM_STRING_SIZE);
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// ID Check
//	if(strcmp(g_szMGameID, szDecryptedID))
//	{
//		DBGOUT("[Error] Decryption ID Error, DecryptedID(%s)\n", szDecryptedID);
//		return FALSE;
//	}
//
//	DBGOUT("%s %s %s %s %s %s %s\n"
//		, __argv[1], __argv[2], __argv[3], __argv[4]
//		, __argv[5], __argv[6], __argv[7]);
//	//
//	///////////////////////////////////////////////////////////////////////////////
//#endif // _MGAME_RELEASE

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-02 by cmkwon
	Err_t errCode = GSetExcuteParameterList(__argc, __argv);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Set Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}

	SEXCUTE_PARAMETER exeParam;		MEMSET_ZERO(&exeParam, sizeof(exeParam));
	exeParam.i_nExcuteFileType		= EXCUTE_FILE_TYPE_SC_EXE;
	errCode = GCheckExcuteParameterList(&exeParam);
	if(ERR_NO_ERROR != errCode)
	{
		DbgOut("Check Excute Parameter error !! Error = %s(0x%X)\r\n", GetErrorString(errCode), errCode);
		ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
		return FALSE;
	}
	//
	///////////////////////////////////////////////////////////////////////////////
	

	CAtumDlg dlg;
	m_pMainWnd = &dlg;
	ReadProfile();
//	ReadCrocessProfile();

	// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -
	if(0 != strncmp(exeParam.o_szPreServerIP0, "", SIZE_MAX_IPADDRESS))
	{
		dlg.m_szMainPreServerIP = exeParam.o_szPreServerIP0;
	}

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void AddXPServicePack2Regestry(HKEY i_hKey, char *i_szExeDir, char *i_szExeFileName, char *i_szRegName)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-13 by cmkwon
	// 1. 예외 목록에 삽입할 문자열을 만든다.
	char szName1[_MAX_PATH] = {0};
	sprintf(szName1, "%s\\%s", i_szExeDir, i_szExeFileName);		

	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-13 by cmkwon
	// 2. 삽입할 문자열의 값을 만든다.
	// 들어갈 값은 이미 xp 깔린 컴퓨터에서 테스트 하고 레지스트리에 기록된  값을 그대로 쓴다.
	// FullPath:*:Enabled:RegName
	char szValue1[_MAX_PATH] = {0};
	sprintf(szValue1, "%s:Enabled:%s", szName1, i_szRegName);		

	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-13 by cmkwon
	// 4. 문자열이 레지스트리에 이미 있는가 확인한다
	BYTE byBuffer1[_MAX_PATH];
	DWORD dwType1, dwSize1 = _MAX_PATH;
	char szValue_old1[_MAX_PATH] = {0};
	if (RegQueryValueEx(i_hKey, szName1, NULL, &dwType1, byBuffer1, &dwSize1) == ERROR_SUCCESS )
	{
		// 이미 있다면
		sprintf( szValue_old1, "%s", (char*)byBuffer1);

		// 만약 있다면 값을 읽어서 Enabled 인지 Disabled인지 확인 한다

		char *pdest;
		pdest = strstr(szValue_old1, "Disabled" );

		if( pdest != NULL )
		{
			//만약 Disabled 이라면 Enabled로 변경해서 값을 바꿔준다
			RegSetValueEx(i_hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
		}
		// Enabled 이라면 그냥 통과
	}
	else
	{
		// 없으면
		RegSetValueEx(i_hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
	}

}

void CAtumApp::DisableXPServicePack2()
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-24 by cmkwon, VoIP관련 파일을 시스템폴더로 복사한다.
	char szSystemPath[MAX_PATH];
	if(GetSystemDirectory(szSystemPath, MAX_PATH))
	{// 2007-07-24 by cmkwon, 시스템폴더 접근 권한 체크
		// 2007-06-25 by cmkwon, SCVoIP ActiveX 컨트롤 등록
		char szGameRootFullPath[MAX_PATH];
		char szSCVoIPFullPath[MAX_PATH];
		BOOL bReted = FALSE;
		GetCurrentDirectory(MAX_PATH, szGameRootFullPath);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-07-24 by cmkwon, VoIP관련 파일을 시스템폴더로 복사한다.
		// 2008-10-23 by cmkwon, 등록에 필요한 파일 복사를 먼저 하도록 위치 변경
		char szSourceFilePath[MAX_PATH];
		char szTargetFilePath[MAX_PATH];
		sprintf(szTargetFilePath, "%s\\%s", szSystemPath, "Sx5363.ini");
		sprintf(szSourceFilePath, "%s\\Res-Voip\\%s", szGameRootFullPath, "Sx5363.ini");
		CopyFile(szSourceFilePath, szTargetFilePath, TRUE);		// 기존 파일이 있으면 복사 실패
		sprintf(szTargetFilePath, "%s\\%s", szSystemPath, "SX5363S.DLL");
		sprintf(szSourceFilePath, "%s\\Res-Voip\\%s", szGameRootFullPath, "SX5363S.DLL");
		CopyFile(szSourceFilePath, szTargetFilePath, TRUE);		// 기존 파일이 있으면 복사 실패
		sprintf(szTargetFilePath, "%s\\%s", szSystemPath, "RV32RTP.dll");
		sprintf(szSourceFilePath, "%s\\Res-Voip\\%s", szGameRootFullPath, "RV32RTP.dll");
		CopyFile(szSourceFilePath, szTargetFilePath, TRUE);		// 기존 파일이 있으면 복사 실패
		SetLastError(0);

		sprintf(szSCVoIPFullPath, "%s\\Res-Voip\\%s", szGameRootFullPath, "SCDialer1.ocx");
		bReted = RegisterServer(szSCVoIPFullPath, FALSE);
		if(FALSE == bReted)
		{
			DbgOut("[Error] SCVoIP ActiveX control register fail !!, Path(%s)\r\n", szSCVoIPFullPath);
		}
		SetLastError(0);

		sprintf(szSCVoIPFullPath, "%s\\Res-Voip\\%s", szGameRootFullPath, "SCDialer2.ocx");
		bReted = RegisterServer(szSCVoIPFullPath, FALSE);
		if(FALSE == bReted)
		{
			DbgOut("[Error] SCVoIP ActiveX control register fail !!, Path(%s)\r\n", szSCVoIPFullPath);
		}
		SetLastError(0);
	}

	HKEY hKey;

	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
				"SYSTEM\\ControlSet001\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile\\AuthorizedApplications\\List", 
				0L, 
				KEY_ALL_ACCESS, 
				&hKey) == ERROR_SUCCESS )
	{// 레지스트리가 존재 한다면


		// 1. 게임 설치 디렉토리를 얻는다.
		HKEY hSubKey = NULL;
		
		BYTE byBuffer[_MAX_PATH];
		DWORD dwType, dwSize = _MAX_PATH;
		// 게임 마다 레지스트리에  디렉토리 정보가 틀릴 수도 있다.

		///////////////////////////////////////////////////////////////////////////////
		// 2007-03-13 by cmkwon, 게임이 설치되어 있는 경로가 저장된 레지스트리를 검색한다.
		char szFullPathReg[_MAX_PATH]={0};
		sprintf(szFullPathReg, "SOFTWARE\\%s", REGISTRY_BASE_PATH);
// 2007-03-13 by cmkwon, 아래와 같이 조합하는 것으로 수정함
//		if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, FULL_PATH_REGISTRY, 0L, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS )
		if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szFullPathReg, 0L, KEY_ALL_ACCESS, &hSubKey))
		{// 2007-03-13 by cmkwon, 지정한 경로의 레지스트리가 존재하지 않는다
			return;
		}

		if (ERROR_SUCCESS != RegQueryValueEx(hSubKey, "InstallPath", NULL, &dwType, byBuffer, &dwSize))
		{// 2007-03-13 by cmkwon, 지정한 필드의 레지스트리가 존재하지 않는다
			RegCloseKey(hSubKey);
			return;
		}
		RegCloseKey(hSubKey);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-03-13 by cmkwon, 실행파일의 FullPath를 가져온다
		char szExecDir[_MAX_PATH] = {0}, szExecPath[_MAX_PATH] = {0};
		sprintf(szExecPath, "%s", (char*)byBuffer);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-03-13 by cmkwon, 실행 파일의 경로 Directory를 가져온다
		// 처음 실행될 파일이 같이 있는 경우 이미 알고 있는 이름이므로 
		char *pdest;
		int nCount;
		pdest = strstr(szExecPath, EXE_1_FILE_NAME );
		if(NULL == pdest)
		{// 2007-05-17 by cmkwon, 레지스트리 내용에서 실행파일 이름 검색 실패
			return;
		}
		nCount = pdest - szExecPath;
		STRNCPY_MEMSET( szExecDir, szExecPath, nCount);
		TRACE("\n%s\n", szExecDir );

		///////////////////////////////////////////////////////////////////////////////
		// 2007-03-13 by cmkwon, Launcher 등록
		AddXPServicePack2Regestry(hKey, szExecDir, LAUNCHER_FILE_NAME, "GameExe2");

		///////////////////////////////////////////////////////////////////////////////
		// 2007-03-13 by cmkwon, VoIP 등록
		AddXPServicePack2Regestry(hKey, szExecDir, "Res-Voip\\SCVoIP.exe", "GameVoIP");

// 2007-03-13 by cmkwon, 위와 같이 함수로 만들고 음성 통신 프로그램도 등록한다.
//		///////////////////////////////////////////////////////////////////////////////		
//		// 2. 예외 목록에 삽입할 문자열을 만든다.
//		// 스페이스카우보이 등록해야할게 1개( Launcher.atm )  있으므로
//		char szName1[_MAX_PATH] = {0};
//		sprintf(szName1, "%s\\%s", szExecDir, LAUNCHER_FILE_NAME);		
//		TRACE("\n%s\n", szName1);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2007-03-13 by cmkwon
//		// 3. 삽입할 문자열의 값을 만든다.
//		// 들어갈 값은 이미 xp 깔린 컴퓨터에서 테스트 하고 레지스트리에 기록된  값을 그대로 쓴다.
//		// :*:Enabled:SpaceCowboyLauncher
//		char szValue1[_MAX_PATH] = {0};
//		sprintf(szValue1, "%s%s", szName1, ":*:Enabled:SpaceCowboyLauncher" );		
//		TRACE("\n%s\n", szValue1);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2007-03-13 by cmkwon
//		// 4. 문자열이 레지스트리에 이미 있는가 확인한다
//		BYTE byBuffer1[_MAX_PATH];
//		DWORD dwType1, dwSize1 = _MAX_PATH;
//		char szValue_old1[_MAX_PATH] = {0};
//		if (RegQueryValueEx(hKey, szName1, NULL, &dwType1, byBuffer1, &dwSize1) == ERROR_SUCCESS )
//		{
//			// 이미 있다면
//			sprintf( szValue_old1, "%s", (char*)byBuffer1);
//
//			// 만약 있다면 값을 읽어서 Enabled 인지 Disabled인지 확인 한다
//
//			char *pdest;
//			pdest = strstr(szValue_old1, "Disabled" );
//
//			if( pdest != NULL )
//			{
//				//만약 Disabled 이라면 Enabled로 변경해서 값을 바꿔준다
//				RegSetValueEx(hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
//			}
//			// Enabled 이라면 그냥 통과
//		}
//		else
//		{
//			// 없으면
//			RegSetValueEx(hKey, szName1, 0L, REG_SZ, (BYTE *)szValue1, strlen(szValue1)+1);
//		}
	}

}




///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumApp::UpdateEXE1FilePathResitry(void)
/// \brief		// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -
/// \author		cmkwon
/// \date		2007-08-31 ~ 2007-08-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumApp::UpdateEXE1FilePathResitry(void)
{
	char szGameRootDirPath[MAX_PATH]	= {0x00,};	
	GetCurrentDirectory(MAX_PATH, szGameRootDirPath);

	char szGameRootFullPath[MAX_PATH]	= {0x00,};	
	if('\\' == szGameRootDirPath[strlen(szGameRootDirPath)-1])
	{
		sprintf(szGameRootFullPath, "%s%s", szGameRootDirPath, EXE_1_FILE_NAME);
	}
	else
	{
		sprintf(szGameRootFullPath, "%s\\%s", szGameRootDirPath, EXE_1_FILE_NAME);
	}
	
	CRegistryControl regCtrl;
	
	// 2007-09-12 by cmkwon, 설치 경로도 REGISTRY_BASE_PATH 와 같도록 수정한다
	char szRegKeyPath[MAX_PATH]	= {0x00,};	
	sprintf(szRegKeyPath, "SOFTWARE\\%s", REGISTRY_BASE_PATH);
	if(FALSE == regCtrl.RegistryControlInit(HKEY_LOCAL_MACHINE, szRegKeyPath, TRUE))
	{
		return FALSE;
	}

	if(FALSE == regCtrl.WriteString(WEB_START_REGISTRY_VALUE_NAME, szGameRootFullPath))
	{
		return FALSE;
	}

	return TRUE;
}


const char _szDllRegSvr[] = "DllRegisterServer";
const char _szDllUnregSvr[] = "DllUnregisterServer";

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD CAtumApp::RegisterServer(LPCTSTR pszDllName,BOOL bUnregister)
/// \brief		// 2007-06-25 by cmkwon, SCVoIP ActiveX 컨트롤 등록
/// \author		cmkwon
/// \date		2007-06-25 ~ 2007-06-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumApp::RegisterServer(LPCTSTR pszDllName,BOOL bUnregister)
{
    BOOL	bRet = TRUE;
    HRESULT (STDAPICALLTYPE * lpDllEntryPoint)(void);

    LPCSTR pszDllEntryPoint = (bUnregister)?_szDllUnregSvr:_szDllRegSvr;

    // Initialize OLE.
    if (FAILED(OleInitialize(NULL))) return FALSE;

    SetErrorMode(SEM_FAILCRITICALERRORS); // Make sure LoadLib fails.

    // Load the library.
    HINSTANCE hLib = LoadLibraryEx(pszDllName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (hLib < (HINSTANCE)HINSTANCE_ERROR)
    {
		DbgOut("The specified module(%s) could not be found\r\n", pszDllName);
		OleUninitialize();
		return FALSE;
    }

    // Find the entry point.
    (FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, pszDllEntryPoint);

    if (lpDllEntryPoint == NULL)
    {
	    FreeLibrary(hLib);
		OleUninitialize();
		return FALSE;
    }

    // Call the entry point.
    if (FAILED((*lpDllEntryPoint)()))
    {
        FreeLibrary(hLib);
		OleUninitialize();
		return FALSE;

    }

    FreeLibrary(hLib);
    OleUninitialize();
    return TRUE;
}

void CAtumApp::WriteProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	CAtumDlg *pDlg = (CAtumDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		GSaveExe1VersionInfo(&pDlg->m_CurrentLauncherVersion, STRMSG_VERSION_INFO_FILE_NAME);
		WriteProfileString(_T("Configuration"), _T("LauncherVersion"), pDlg->m_CurrentLauncherVersion.GetVersionString());
	}
}

void CAtumApp::ReadProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	CAtumDlg *pDlg = (CAtumDlg*)AfxGetMainWnd();

	if(pDlg)
	{
// 2007-08-31 by cmkwon, 필요 없는 부분인것 같음 - CAtumDlg::CAtumDlg() 에서 할당 되고 있음
//		char szTemp[16];
//		sprintf(szTemp, "%d.%d.%d.%d"
//			, MAIN_PRE_SERVER_IP_0, MAIN_PRE_SERVER_IP_1
//			, MAIN_PRE_SERVER_IP_2, MAIN_PRE_SERVER_IP_3);
//		pDlg->m_szMainPreServerIP = szTemp;

		///////////////////////////////////////////////////////////////////////////////
		// 2007-05-09 by cmkwon, 레지스트리에 저장할 데이터를 파일로 저장하기 시스템
		VersionInfo lVer;
		Err_t errCode = GLoadExe1VersionInfo(&lVer, STRMSG_VERSION_INFO_FILE_NAME);
		if(ERR_NO_ERROR != errCode
			|| FALSE == lVer.IsValidVersionInfo())
		{// 2007-05-09 by cmkwon, 레지스트리파일이 없거나 버전 정보가 없을때
			lVer.SetVersion((char*)(LPCSTR)GetProfileString( _T("Configuration"), _T("LauncherVersion"), "0.0.0.0"));
		}

		pDlg->m_CurrentLauncherVersion = lVer;		
	}
}

void CAtumApp::ReadCrocessProfile()
{
	CAtumDlg *pDlg = (CAtumDlg*)AfxGetMainWnd();

	if(pDlg)
	{
		pDlg->m_szCrocessSuffix = "";
	}
	else
	{
		return;
	}

	HKEY hKey = NULL;
    TCHAR szSuffix[16];
    DWORD dwBufLen = 16;
    LONG lRet;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("software\\Gameinfinity\\Install game info.\\c34"), 0, KEY_QUERY_VALUE,
		&hKey) == ERROR_SUCCESS)
	{
		lRet = RegQueryValueEx(hKey,
							TEXT("identifier"),
							NULL,
							NULL,
							(LPBYTE)szSuffix,
							&dwBufLen);

		RegCloseKey(hKey);
		pDlg->m_szCrocessSuffix = szSuffix;
	}
	else
	{
		return;
	}
}
