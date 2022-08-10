//7A7A7A584023252521217E6667766D65696C3200580000000000020001000000323ED20438C7000050C3000050C30000000000000040A7E3A36BC90100006E5C758ACA017176785A5E24242421217E7A766D657473724200
#include "zwave_sdk_linker.h"

static HMODULE g_hStcDll = NULL;

typedef BOOL	(WINAPI* PZSWAVE_InitializeW)(LPCWSTR path);
typedef BOOL	(WINAPI* PZSWAVE_InitializeA)(LPCSTR path);
typedef BOOL	(WINAPI* PZSWAVE_Uninitialize)();
typedef DWORD	(WINAPI* PZSWAVE_GetDBConnectionCount)();
typedef BOOL	(WINAPI* PZSWAVE_GetLibraryPathW)(LPWSTR path, SIZE_T size);
typedef BOOL	(WINAPI* PZSWAVE_GetLibraryPathA)(LPSTR path, SIZE_T size);
typedef int	(WINAPI* PZSWAVE_GetGroupCount)();
typedef int	(WINAPI* PZSWAVE_GetGroupDataCount)(int index);
typedef HANDLE	(WINAPI* PZSWAVE_OpenAt)(int gi, int di, SIZE_T bs);
typedef HANDLE	(WINAPI* PZSWAVE_Open)(SIZE_T bs, DWORD type);
typedef DWORD	(WINAPI* PZSWAVE_WaitForStop)(DWORD t);
typedef DWORD	(WINAPI* PZSWAVE_WaitForState)(LONG state, DWORD t);
typedef BOOL	(WINAPI* PZSWAVE_Close)(HANDLE h);
typedef BOOL	(WINAPI* PZSWAVE_GetDataAt)(HANDLE h, int no, PACKETDATA data);
typedef BOOL	(WINAPI* PZSWAVE_Verify)(HANDLE h, LPCBYTE data, SIZE_T size);
typedef int	(WINAPI* PZSWAVE_VerifyExA)(HANDLE h, CPACKETDATA data, SIZE_T size, LPSTR msg, SIZE_T msgSize);
typedef int	(WINAPI* PZSWAVE_VerifyExW)(HANDLE h, CPACKETDATA data, SIZE_T size, LPWSTR msg, SIZE_T msgSize);
typedef BOOL	(WINAPI* PZSWAVE_SetVerifyValueA)(HANDLE h, LPCSTR v);
typedef BOOL	(WINAPI* PZSWAVE_SetVerifyValueW)(HANDLE h, LPCWSTR v);
typedef BOOL	(WINAPI* PZSWAVE_DebugInfoA)(HANDLE h, PZWAVE_DEBUG_INFOA info);
typedef BOOL	(WINAPI* PZSWAVE_DebugInfoW)(HANDLE h, PZWAVE_DEBUG_INFOW info);
typedef LONG	(WINAPI* PZSWAVE_GetState)();
typedef BOOL	(WINAPI* PZSWAVE_SetState)(LONG s);
typedef int	(WINAPI* PZSWAVE_GetDataCount)(HANDLE s);

PZSWAVE_InitializeW				gZSWAVE_InitializeW				= NULL;
PZSWAVE_InitializeA				gZSWAVE_InitializeA				= NULL;
PZSWAVE_Uninitialize				gZSWAVE_Uninitialize				= NULL;
PZSWAVE_GetDBConnectionCount	gZSWAVE_GetDBConnectionCount	= NULL;
PZSWAVE_GetLibraryPathW			gZSWAVE_GetLibraryPathW			= NULL;
PZSWAVE_GetLibraryPathA			gZSWAVE_GetLibraryPathA			= NULL;
PZSWAVE_GetGroupCount			gZSWAVE_GetGroupCount			= NULL;
PZSWAVE_GetGroupDataCount		gZSWAVE_GetGroupDataCount		= NULL;
PZSWAVE_OpenAt					gZSWAVE_OpenAt					= NULL;
PZSWAVE_Open						gZSWAVE_Open						= NULL;
PZSWAVE_WaitForStop				gZSWAVE_WaitForStop				= NULL;
PZSWAVE_WaitForState				gZSWAVE_WaitForState				= NULL;
PZSWAVE_Close						gZSWAVE_Close						= NULL;
PZSWAVE_GetDataAt					gZSWAVE_GetDataAt					= NULL;
PZSWAVE_Verify					gZSWAVE_Verify					= NULL;
PZSWAVE_VerifyExA					gZSWAVE_VerifyExA					= NULL;
PZSWAVE_VerifyExW					gZSWAVE_VerifyExW					= NULL;
PZSWAVE_SetVerifyValueA			gZSWAVE_SetVerifyValueA			= NULL;
PZSWAVE_SetVerifyValueW			gZSWAVE_SetVerifyValueW			= NULL;
PZSWAVE_DebugInfoA				gZSWAVE_DebugInfoA				= NULL;
PZSWAVE_DebugInfoW				gZSWAVE_DebugInfoW				= NULL;
PZSWAVE_GetState					gZSWAVE_GetState					= NULL;
PZSWAVE_SetState					gZSWAVE_SetState					= NULL;
PZSWAVE_GetDataCount				gZSWAVE_GetDataCount				= NULL;


BOOL GetProcAddressEx(HMODULE module, PVOID ptr, LPCSTR name)
{
	FARPROC proc;
	proc = GetProcAddress(module, name);
	if(proc)
	{
		*(FARPROC *) ptr = proc;
		return TRUE;
	}

	return FALSE;
}

BOOL ZSWAVE_LinkW(LPCWSTR pszlibdllpath)
{
	if ( g_hStcDll ) return FALSE;

	if ( pszlibdllpath != NULL )
	{
		wchar_t szT[MAX_PATH];
		wcscpy(szT, pszlibdllpath);
		if(szT[wcslen(szT) -1] != L'\\') wcscat(szT, L"\\");
		wcscat(szT, L"zwave_sdk_server_lib.dll");
		g_hStcDll = LoadLibraryW(szT);	

	}
	else
	{
		wchar_t szT[MAX_PATH];
		GetModuleFileNameW(NULL, szT, MAX_PATH);
		wchar_t *p = wcsrchr(szT, '\\');
		*(p+1) = 0x00;
		wcscat(szT, L"zwave_sdk_server_lib.dll");
		g_hStcDll = LoadLibraryW(szT);	
	}

	if ( g_hStcDll == NULL ) return FALSE;

	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_InitializeW,			"ZSWAVE_InitializeW")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_InitializeA,			"ZSWAVE_InitializeA")) goto $CLEAR;			
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_Uninitialize,			"ZSWAVE_Uninitialize")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDBConnectionCount,	"ZSWAVE_GetDBConnectionCount")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetLibraryPathW,		"ZSWAVE_GetLibraryPathW")) goto $CLEAR;		
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetLibraryPathA,		"ZSWAVE_GetLibraryPathA")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetGroupCount,			"ZSWAVE_GetGroupCount")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetGroupDataCount,		"ZSWAVE_GetGroupDataCount")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_OpenAt,					"ZSWAVE_OpenAt")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_Open,					"ZSWAVE_Open")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_WaitForStop,			"ZSWAVE_WaitForStop")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_WaitForState,			"ZSWAVE_WaitForState")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_Close,					"ZSWAVE_Close")) goto $CLEAR;					
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDataAt,				"ZSWAVE_GetDataAt")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_Verify,					"ZSWAVE_Verify")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_VerifyExA,				"ZSWAVE_VerifyExA")) goto $CLEAR;				
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_VerifyExW,				"ZSWAVE_VerifyExW")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_SetVerifyValueA,		"ZSWAVE_SetVerifyValueA")) goto $CLEAR;		
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_SetVerifyValueW,		"ZSWAVE_SetVerifyValueW")) goto $CLEAR;		
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_DebugInfoA,				"ZSWAVE_DebugInfoA")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_DebugInfoW,			"ZSWAVE_DebugInfoW")) goto $CLEAR;
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetState,				"ZSWAVE_GetState")) goto $CLEAR;			
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_SetState,				"ZSWAVE_SetState")) goto $CLEAR;				
	if ( !GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDataCount,			"ZSWAVE_GetDataCount")) goto $CLEAR;			

	return TRUE;
$CLEAR:
	FreeLibrary(g_hStcDll);
	g_hStcDll = NULL;
	return FALSE;
}

BOOL ZSWAVE_LinkA(LPCSTR pszlibdllpath)
{
	if ( pszlibdllpath != NULL )
	{
		WCHAR libPath[MAX_PATH + 1];
		if (!MultiByteToWideChar(CP_ACP, 0, pszlibdllpath, -1, libPath, MAX_PATH))
		{
			return FALSE;
		} 
		else return ZSWAVE_LinkW(libPath);
	}
	return ZSWAVE_LinkW(NULL);
}


BOOL ZSWAVE_UnLink()
{
	if(g_hStcDll)
	{
		FreeLibrary(g_hStcDll);
		g_hStcDll = NULL;
	}

	return TRUE;
}

BOOL WINAPI ZSWAVE_InitializeW(LPCWSTR path)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_InitializeW(path);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_InitializeA(LPCSTR path)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_InitializeA(path);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_Uninitialize()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Uninitialize();
	else return FALSE;
}

DWORD WINAPI ZSWAVE_GetDBConnectionCount()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDBConnectionCount();
	else return FALSE;
}

BOOL WINAPI ZSWAVE_GetLibraryPathW(LPWSTR path, SIZE_T size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetLibraryPathW(path, size);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_GetLibraryPathA(LPSTR path, SIZE_T size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetLibraryPathA(path, size);
	else return FALSE;
}

int WINAPI ZSWAVE_GetGroupCount()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetGroupCount();
	else return FALSE;
}

int	WINAPI ZSWAVE_GetGroupDataCount(int index)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetGroupDataCount(index);
	else return FALSE;
}

HANDLE WINAPI ZSWAVE_OpenAt(int gi, int di, SIZE_T bs)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_OpenAt(gi, di, bs);
	else return FALSE;
}

HANDLE WINAPI ZSWAVE_Open(SIZE_T bs, DWORD type)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Open(bs, type);
	else return FALSE;
}

DWORD WINAPI ZSWAVE_WaitForStop(DWORD t)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_WaitForStop(t);
	else return FALSE;
}

DWORD WINAPI ZSWAVE_WaitForState(LONG state, DWORD t)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_WaitForState(state, t);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_Close(HANDLE h)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Close(h);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_GetDataAt(HANDLE h, int no, PACKETDATA data)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDataAt(h, no, data);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_Verify(HANDLE h, LPCBYTE data, SIZE_T size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Verify(h, data, size);
	else return FALSE;
}

int	WINAPI ZSWAVE_VerifyExA(HANDLE h, CPACKETDATA data, SIZE_T size, LPSTR msg, SIZE_T msgSize)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_VerifyExA(h, data, size, msg, msgSize);
	else return FALSE;
}

int WINAPI ZSWAVE_VerifyExW(HANDLE h, CPACKETDATA data, SIZE_T size, LPWSTR msg, SIZE_T msgSize)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_VerifyExW(h, data, size, msg, msgSize);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_SetVerifyValueA(HANDLE h, LPCSTR v)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_SetVerifyValueA(h, v);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_SetVerifyValueW(HANDLE h, LPCWSTR v)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_SetVerifyValueW(h, v);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_DebugInfoA(HANDLE h, PZWAVE_DEBUG_INFOA info)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_DebugInfoA(h, info);
	else return FALSE;
}

BOOL WINAPI ZSWAVE_DebugInfoW(HANDLE h, PZWAVE_DEBUG_INFOW info)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_DebugInfoW(h, info);
	else return FALSE;
}

LONG WINAPI ZSWAVE_GetState()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetState();
	else return FALSE;
}

BOOL WINAPI ZSWAVE_SetState(LONG s)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_SetState(s);
	else return FALSE;
}
int	WINAPI ZSWAVE_GetDataCount(HANDLE s)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDataCount(s);
	else return FALSE;
}
