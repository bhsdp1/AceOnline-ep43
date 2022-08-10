// ProjectAtum.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "AtumApplication.h"
#include "MGameDecryption.h"
#include <ShuttleChild.h>

// 2010-09-29 by jskim, 덤프 남기도록 수정 
#include "dbgHelp.h"
#pragma comment(lib, "dbghelp.lib")

#include "CustomOptimizer.h"

#include "DarkCrash.h" // 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

 #define  SET_CRT_DEBUG_FIELD(a) \
                 _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))


static BOOL PreventSetUnhandledExceptionFilter()
{
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));
	if (hKernel32 == NULL) return FALSE;
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if (pOrgEntry == NULL) return FALSE;

#ifdef _M_IX86
	// Code for x86:
	// 33 C0                xor         eax,eax  
	// C2 04 00             ret         4 
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };
#elif _M_X64
	// 33 C0                xor         eax,eax 
	// C3                   ret  
	unsigned char szExecute[] = { 0x33, 0xC0, 0xC3 };
#else
#error "The following code only works for x86 and x64!"
#endif

	SIZE_T bytesWritten = 0;
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),
		pOrgEntry, szExecute, sizeof(szExecute), &bytesWritten);
	return bRet;
}


LONG __stdcall Exception_Minidump(_EXCEPTION_POINTERS* pExceptionInfo)
{
    char fileName[MAX_PATH];
    GetModuleFileName(NULL, fileName, sizeof(fileName));
    char* ext = strrchr(fileName, '.');
    strcpy(ext ? ext : fileName+strlen(fileName), ".dmp");

#ifndef _CRASH_HANDLER
    char temp[256];
    wsprintf(temp, "An unknown error occured! Please report this crash to the DreamACE Staff.\nOn request upload your: %s, so the developers can fix this issue!\nErrorcode: 0x%08x",fileName, pExceptionInfo->ExceptionRecord->ExceptionCode); //2015-03-04 by St0rmy, changed exception text
    MessageBox(NULL, temp, "[DreamACE] Unknown Error!", MB_OK);//2015-03-04 by St0rmy, changed msgbox title
#endif

    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessID = GetCurrentProcessId();
    HANDLE hFile = CreateFile(fileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = pExceptionInfo;
    eInfo.ClientPointers = FALSE;

    MiniDumpWriteDump(hProcess, dwProcessID, hFile, MiniDumpNormal, pExceptionInfo ? &eInfo : NULL, NULL, NULL);

	// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송
#ifdef SC_DARK_CRASH_HSSON
	Exception::CDarkCrash::OnPlayMiniDump( pExceptionInfo );
	char* pc = new char[1024];
	Exception::CDarkCrash::OutInfo( pc );
	//OutputDebugString( pc );
	
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_PARTY_ACCEPT_INVITE;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, pc, strlen(pc));
	g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + strlen(pc));

	delete pc;
#endif // SC_DARK_CRASH_HSSON
	// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송
#ifdef _CRASH_HANDLER
	char buff[512];
	sprintf(buff, "CrashReporter.exe -code 0x%08x -accountname %s -mapnumber %d -charnumber %d -dmpfile %s", pExceptionInfo->ExceptionRecord->ExceptionCode, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.AccountName, g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber, fileName);


	DWORD dwExitCode;
	PROCESS_INFORMATION pi;

	STARTUPINFO si = { sizeof(si) };
	ZeroMemory(&si, sizeof(si));

	CreateProcess(NULL, buff, NULL, NULL, 0, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
#endif


    return EXCEPTION_EXECUTE_HANDLER;
}
// end 2010-09-29 by jskim, 덤프 남기도록 수정 
HANDLE g_hMutexMonoInstance = NULL;
HANDLE g_hMutexMonoInstanceTemp = NULL;
#ifdef _WARRIOR_ANTICHEAT
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char title[80];
	std::string str(title);
	//Improved by st0rmy (its also now dynamic)
	const string entries[] = { "Cheat Engine", "ArtMoney", "AI Robot", "Macro" };
	GetWindowText(hwnd, title, sizeof(title));
	str = title;
	for (int i = 0; i < sizeof(entries) / sizeof(*entries); i++)
	{
		//size_t found = str.find(entries[i]);
		if(str.find(entries[i]) != std::string::npos)
			return FALSE;
	}
	return TRUE;
	/*std::string stk("Cheat Engine");

	std::string stk1("ArtMoney"); //added
	std::string stk2("AI Robot"); //added
	std::string stk3("Macro"); //added

	GetWindowText(hwnd, title, sizeof(title));
	str = title;

	size_t found = str.find(stk);

	size_t found1 = str.find(stk1); //added
	size_t found2 = str.find(stk2); //added
	size_t found3 = str.find(stk3); //added

	if (found != std::string::npos) {
		return FALSE;
	}

	if (found1 != std::string::npos) { //added
		return FALSE;
	}
	if (found2 != std::string::npos) { //added
		return FALSE;
	}
	if (found3 != std::string::npos) { //added
		return FALSE;
	}
	return TRUE;*/
}

/*
DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
  PCONTEXT ctx = ep->ContextRecord;

  ctx->Ebx = -1; // Not running VPC
  ctx->Eip += 4; // skip past the "call VPC" opcodes
  return EXCEPTION_CONTINUE_EXECUTION;
  // we can safely resume execution since we skipped faulty instruction
}

// High level language friendly version of IsInsideVPC()
bool IsInsideVPC()
{
  bool rc = false;

  __try
  {
    _asm push ebx
    _asm mov  ebx, 0 // It will stay ZERO if VPC is running
    _asm mov  eax, 1 // VPC function number

    // call VPC 
    _asm __emit 0Fh
    _asm __emit 3Fh
    _asm __emit 07h
    _asm __emit 0Bh

    _asm test ebx, ebx
    _asm setz [rc]
    _asm pop ebx
  }
  // The except block shouldn't get triggered if VPC is running!!
  __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))
  {
  }

  return rc;
}

bool IsInsideVMWare()
{
  bool rc = true;

  __try
  {
    __asm
    {
      push   edx
      push   ecx
      push   ebx

      mov    eax, 'VMXh'
      mov    ebx, 0 // any value but not the MAGIC VALUE
      mov    ecx, 10 // get VMWare version
      mov    edx, 'VX' // port number

      in     eax, dx // read port
                     // on return EAX returns the VERSION
      cmp    ebx, 'VMXh' // is it a reply from VMWare?
      setz   [rc] // set return value

      pop    ebx
      pop    ecx
      pop    edx
    }
  }
  __except(EXCEPTION_EXECUTE_HANDLER)
  {
    rc = false;
  }

  return rc;
}
*/
int CheckHardwareBreakpoints()
{
    unsigned int NumBps = 0;

    // This structure is key to the function and is the 
    // medium for detection and removal
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT)); 
    
    // The CONTEXT structure is an in/out parameter therefore we have
    // to set the flags so Get/SetThreadContext knows what to set or get.
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS; 
    
    // Get a handle to our thread
    HANDLE hThread = GetCurrentThread();

    // Get the registers
    if(GetThreadContext(hThread, &ctx) == 0)
        return -1;

    // Now we can check for hardware breakpoints, its not 
    // necessary to check Dr6 and Dr7, however feel free to
    if(ctx.Dr0 != 0)
        ++NumBps; 
    if(ctx.Dr1 != 0)
           ++NumBps; 
    if(ctx.Dr2 != 0)
           ++NumBps; 
    if(ctx.Dr3 != 0)
        ++NumBps;
        
    return NumBps;
}
bool MemoryBreakpointDebuggerCheck()
{
     unsigned char *pMem = NULL;
     SYSTEM_INFO sysinfo = {0}; 
     DWORD OldProtect = 0;
     void *pAllocation = NULL; // Get the page size for the system 
 
    GetSystemInfo(&sysinfo); // Allocate memory 
 
    pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize, 
                        MEM_COMMIT | MEM_RESERVE, 
                         PAGE_EXECUTE_READWRITE); 
        
    if (pAllocation == NULL)
        return false; 
    
    // Write a ret to the buffer (opcode 0xc3)
    pMem = (unsigned char*)pAllocation;
    *pMem = 0xc3; 
    
    // Make the page a guard page         
    if (VirtualProtect(pAllocation, sysinfo.dwPageSize, 
                    PAGE_EXECUTE_READWRITE | PAGE_GUARD, 
                    &OldProtect) == 0)
    {
        return false;
    } 
    
    __try
    {
        __asm
        {
            mov eax, pAllocation
            // This is the address we'll return to if we're under a debugger
            push MemBpBeingDebugged
            jmp eax // Exception or execution, which shall it be :D?
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // The exception occured and no debugger was detected
        VirtualFree(pAllocation, NULL, MEM_RELEASE);
        return false;
    }     
    
    __asm{MemBpBeingDebugged:}
    VirtualFree(pAllocation, NULL, MEM_RELEASE);
    return true;
}

bool CheckForCCBreakpointXor55(void* pMemory,  size_t SizeToCheck)
 {
     unsigned char *pTmp = (unsigned char*)pMemory;
    unsigned char tmpchar = 0;
        
    for (size_t i = 0; i < SizeToCheck; i++)
     {
        tmpchar = pTmp[i];
        if( 0x99 == (tmpchar ^ 0x55) ) // 0xCC xor 0x55 = 0x99
            return true;
     } 

    return false;
 }
inline bool Int2DCheck()
{
    __try
    {
        __asm
        {
            int 0x2d
            xor eax, eax
            add eax, 2
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return false;
    }
    
    return true;
}

inline void ErasePEHeaderFromMemory()
{
    DWORD OldProtect = 0;
    
    // Get base address of module
    char *pBaseAddr = (char*)GetModuleHandle(NULL);

    // Change memory protection
    VirtualProtect(pBaseAddr, 4096, // Assume x86 page size
            PAGE_READWRITE, &OldProtect);

    // Erase the header
    ZeroMemory(pBaseAddr, 4096);
}
#endif
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CAtumApplication pD3dApp;
#ifdef _WARRIOR_ANTICHEAT
	BOOL chk = EnumWindows(EnumWindowsProc, NULL);

	if(chk == FALSE) {
			MessageBox(NULL, "Hacking tool detected! \n Closing program.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
	} 
	
	 if(IsDebuggerPresent())
		{
			MessageBox(NULL, "Hacking tool detected! \n Closing program.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
/*		if(IsInsideVPC())
		{
			MessageBox(NULL, "Using VirtualPC is not allowed! \n Please run DreamACE outside of VirtualPC.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		if(IsInsideVMWare())
		{
			MessageBox(NULL, "Using VMWare is not allowed! \n Please run DreamACE outside of VMWare", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
*/		if(MemoryBreakpointDebuggerCheck())
		{
			MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		if(CheckHardwareBreakpoints() != 0)
		{
			MessageBox(NULL, "Debug software detected! \n Close your debug software and Login Again.", "DreamACE Anti-Cheat", MB_OK);
			exit(1);
			return E_FAIL;
		}
		HKEY hSubKey = NULL;
		if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Cheat Engine", 0L, KEY_ALL_ACCESS, &hSubKey))
		{// warriorsw checking cheat engine
		MessageBox(NULL, "Cheat Engine not allowed! \n Remove Cheat Engine, Restart Computer and Login again.", "DreamACE Anti-Cheat", MB_OK);
		exit(1);
		return E_FAIL;
		}
		//endof 2015-03-03
#endif



	pD3dApp.b_CanadaMutex = true;// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
	g_cCustomOptimizer.AutoLauncher();

	// 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리
	// 2010-09-29 by jskim, 덤프 남기도록 수정 
//	SetUnhandledExceptionFilter(Exception_Minidump);
	// end 2010-09-29 by jskim, 덤프 남기도록 수정 
	if(GAMEGUARD_TYPE != USE_GAMEGUARD_AHNLAB_HACKSHIELD)
	{		
		SetUnhandledExceptionFilter(Exception_Minidump);		
	}
	// END 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리


	// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
#ifdef C_CANADA_HACKSHEILD_JHAHN
	{
		HANDLE hMutexTemp = NULL;	
			
		hMutexTemp = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, "CANADA_HACK_SHIELD");
		if(hMutexTemp == NULL)
		{
			pD3dApp.b_CanadaMutex = true;
		}	
		else
		{
			pD3dApp.b_CanadaMutex = false;
		}
		
		
		CloseHandle(hMutexTemp);
		//end 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
		
	}
#endif
	
	
	if(g_hMutexMonoInstance)
		return 0;

	g_hMutexMonoInstance = CreateMutex(NULL, TRUE, WINDOWTEXT_NAME_CLIENT);

	if(NULL == g_hMutexMonoInstance)
		return 0;

#ifdef MULTI_LOADER_HSSON

#else // MULTI_LOADER_HSSON
	if(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", WINDOWTEXT_NAME_CLIENT, MB_OK);
		return 0;
	}
#endif // MULTI_LOADER_HSSON

	// 2007-07-26 by bhsohn 예당 윈도우 모드만 되는 버그 수정
	//if(__argc != 12 && __argc != 16 && __argc != 11 && __argc != 14)
	if(__argc != 13 && __argc != 17 && __argc != 12 && __argc != 16)
	{
		DBGOUT("[Error] Parameter Count Error, Count(%d)(%s)\n", __argc, lpCmdLine);
		return FALSE;
	}

	char szTemp[20];


	if( __argc == 13 )
	{
#ifdef MULTI_LOADER_HSSON
		sscanf(lpCmdLine,"%s %d %s %d %s %s %d %d %d %d %d %s", pD3dApp.m_strFieldIP, &pD3dApp.m_nFieldPort, pD3dApp.m_strChatIP, &pD3dApp.m_nChatPort, pD3dApp.m_strUserID,
				pD3dApp.m_strUserPassword, &pD3dApp.m_IsFullMode, &pD3dApp.m_nWidth, &pD3dApp.m_nHeight, &pD3dApp.m_bDegree,&pD3dApp.m_bMovePlayer, szTemp);
		
		if( strcmp( szTemp, "DEVELOP" ) != 0 )
			return 0;
#else // MULTI_LOADER_HSSON
		return 0;
#endif // MULTI_LOADER_HSSON
	}
	else if(__argc == 12)	// 2005-08-05 by cmkwon, for JPN release
	{
		sscanf(lpCmdLine,"%s %d %s %d %s %s %d %d %d %d %d", pD3dApp.m_strFieldIP, &pD3dApp.m_nFieldPort, pD3dApp.m_strChatIP, &pD3dApp.m_nChatPort, pD3dApp.m_strUserID,
				pD3dApp.m_strUserPassword, &pD3dApp.m_IsFullMode, &pD3dApp.m_nWidth, &pD3dApp.m_nHeight, &pD3dApp.m_bDegree, &pD3dApp.m_bMovePlayer);
	}
	else
	{
		int nSeed = 0;
		char szAccount[SIZE_MAX_ACCOUNT_NAME];
		char szEncAccount[MGAME_MAX_PARAM_STRING_SIZE];
		char szEncPassword[MGAME_MAX_PARAM_STRING_SIZE];

		sscanf(lpCmdLine,"%s %d %s %d %s %s %d %d %d %d %d %s %d %s %s %s ", pD3dApp.m_strFieldIP, &pD3dApp.m_nFieldPort, pD3dApp.m_strChatIP, &pD3dApp.m_nChatPort,
						pD3dApp.m_strUserID, pD3dApp.m_strUserPassword, &pD3dApp.m_IsFullMode, &pD3dApp.m_nWidth, &pD3dApp.m_nHeight, &pD3dApp.m_bDegree,
						&pD3dApp.m_bMovePlayer,	szTemp, &nSeed, szAccount, szEncAccount, szEncPassword);

		// Mutex Check
		HANDLE hMutex = NULL;	
		hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, (LPTSTR)szEncPassword);  // 암호화된패스워드값을 넣는다
		if(hMutex == NULL)
		{
			DBGOUT("[Error] Mutex Error\n");
			return FALSE;
		}	
		// 2006-10-18 by ispark, ReleaseMutex() -> CloseHandle()로 변경
		CloseHandle(hMutex);
		
		// ExcuteType Check
		if(strcmp(szTemp, "INET"))
		{
			DBGOUT("[Error] Excute Type Error, Type(%s)\n", szTemp);
			return FALSE;
		}

		char szDecryptedID[MGAME_MAX_PARAM_STRING_SIZE];	
		strncpy(szDecryptedID, (char *)decrypt((unsigned char*)szEncAccount, nSeed), MGAME_MAX_PARAM_STRING_SIZE);
		
		// ID Check
		if(strcmp(szAccount, szDecryptedID))
		{
			DBGOUT("[Error] Decryption ID Error, DecryptedID(%s)\n", szDecryptedID);
			return FALSE;
		}

	}

#ifdef ONLY_FULL_WINDOW_HSSON
		pD3dApp.m_IsFullMode = TRUE;
#endif // ONLY_FULL_WINDOW_HSSON

// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전

		if (pD3dApp.b_CanadaMutex)
		{
			// 2009-01-22 by bhsohn Xign Code시, Sleep(3000)추가
			if(!pD3dApp.StartGameGuard())
			{
				pD3dApp.CloseGameGuard();
				return FALSE;
			}		
			// end 2009-01-22 by bhsohn Xign Code시, Sleep(3000)추가
		}
		// 2013-05-03 by bhsohn 핵쉴드 Dump 안나오는 현상 처리		
		if(GAMEGUARD_TYPE == USE_GAMEGUARD_AHNLAB_HACKSHIELD)
		{		
			SetUnhandledExceptionFilter(Exception_Minidump);
#ifdef _ENHANCED_EXCEPTIONS
			if(PreventSetUnhandledExceptionFilter())
				DBGOUT("PreventSetUnhandledExceptionFilter() success!");
#endif
		}

	DBGOUT("FullMode = %d\n",pD3dApp.m_IsFullMode);
	if( FAILED( pD3dApp.Create( hInstance ) ) )
		return 0;

	if(!pD3dApp.SetGameGuardHWND())
	{
		pD3dApp.CloseGameGuard();
		return FALSE;
	}	

	g_input.InitInput();

	INT nResult;

	nResult = pD3dApp.Run(); //Run Main App

	pD3dApp.CloseGameGuard();

	CloseHandle(g_hMutexMonoInstance);

	if(pD3dApp.m_bShutDown && strlen(pD3dApp.m_strMsgLastError))
	{
		MessageBox(NULL, pD3dApp.m_strMsgLastError,STRMSG_WINDOW_TEXT, MB_OK);
	}

	return nResult;
}
