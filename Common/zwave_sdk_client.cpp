//7A7A7A584023252521217E6667766D65696C3200580000000000020001000000B791AC415815010070110100591501009A010000004039C5EFC9C9010000003EC1E8CA017176785A5E24242421217E7A766D657473724200
#include <windows.h>
#include "zwave_sdk_client.h"
#include "zwave_sdk_client_stub0.inc"
#include "zwave_sdk_client_stub1.inc"
#include "zwave_sdk_client_stub2.inc"
#include "zwave_sdk_client_stub3.inc"
#include "zwave_sdk_client_stub4.inc"

typedef struct _XIGNCODE_STUB_ST
{
	PVOID GetModuleHandleA;
	PVOID GetProcAddress;
	PVOID InterlockedExchange;
	PVOID InterlockedCompareExchange;

	PVOID LoadLibraryA;
	PVOID HeapAlloc;
	PVOID HeapFree;
	PVOID GetProcessHeap;
	PVOID VirtualAlloc;
	PVOID VirtualFree;
	PVOID VirtualProtect;
} XIGNCODE_STUB_ST, *PXIGNCODE_STUB_ST;

typedef BOOL (WINAPI *LinkThunkT)(PULONG, PVOID *, PXIGNCODE_STUB_ST, PVOID);
typedef BOOL (WINAPI *UnlinkThunkT)(PULONG, PVOID *, PXIGNCODE_STUB_ST);
typedef FARPROC (WINAPI *FindThunkT)(PULONG, PVOID *, PXIGNCODE_STUB_ST, LPCSTR);

typedef BOOL (__cdecl *ZCWAVE_InitT)();
typedef BOOL (__cdecl *ZCWAVE_CleanupT)();
typedef BOOL (__cdecl *ZCWAVE_SysEnterT)(LPCTSTR, DWORD);
typedef BOOL (__cdecl *ZCWAVE_SysExitT)();
typedef BOOL (__cdecl *ZCWAVE_ProbeT)(CPACKETDATA, PACKETDATA, SIZE_T);
typedef HANDLE (__cdecl *ZCWAVE_CreateCodeBoxT)();
typedef BOOL (__cdecl *ZCWAVE_CloseCodeBoxT)(HANDLE h);
typedef BOOL (__cdecl *ZCWAVE_ProbeCodeBoxT)(HANDLE h, CPACKETDATA data, PACKETDATA buf, SIZE_T size);

static ULONG g_thunkState;
static PVOID g_thunkContext;
static BOOL g_initialized = FALSE;
static PVOID g_vmem = NULL;
static ULONG g_vmemPreambleSize = 0;

#ifdef GetPtr
#undef GetPtr
#endif

#define GetPtr(b, o) (PVOID)((ULONG_PTR) b + o)

static void InitST(PXIGNCODE_STUB_ST st)
{
	st->GetModuleHandleA = GetModuleHandleA;
	st->GetProcAddress = GetProcAddress;
	st->InterlockedExchange = InterlockedExchange;
	st->InterlockedCompareExchange = InterlockedCompareExchange;
	st->LoadLibraryA = LoadLibraryA;
	st->HeapAlloc = HeapAlloc;
	st->HeapFree = HeapFree;
	st->GetProcessHeap = GetProcessHeap;
	st->VirtualAlloc = VirtualAlloc;
	st->VirtualFree = VirtualFree;
	st->VirtualProtect = VirtualProtect;
}

static BOOL LinkThunk()
{
	g_vmem = VirtualAlloc(NULL
						, XIGNCODE_BIN_DATA_SIZE + 0x3000
						, MEM_COMMIT | MEM_RESERVE
						, PAGE_EXECUTE_READWRITE);
	if(!g_vmem)
		return FALSE;

	g_vmemPreambleSize = (GetTickCount() % 4096) & 0xfffff000;

	PVOID p = GetPtr(g_vmem, g_vmemPreambleSize);

	for(ULONG i=0; i<XIGNCODE_BIN_DATA_CNT; ++i)
	{
		memcpy(p, XIGNCODE_BIN_DATA_ARR[i], XIGNCODE_BIN_DATA_ARR_SIZE[i]);
		p = GetPtr(p, XIGNCODE_BIN_DATA_ARR_SIZE[i]);
	}

	XIGNCODE_STUB_ST st;
	InitST(&st);
	BYTE *D = (BYTE *) GetPtr(g_vmem, g_vmemPreambleSize);
	LinkThunkT F = (LinkThunkT) (D + *(DWORD*)(D + 20));
	BOOL ret = F(&g_thunkState, &g_thunkContext, &st, D);
	if(!ret)
	{
		VirtualFree(g_vmem, 0, MEM_DECOMMIT | MEM_RELEASE);
		g_vmem = NULL;
	}

	return ret;
}

static BOOL UnlinkThunk()
{
	XIGNCODE_STUB_ST st;
	InitST(&st);
	BYTE *D = (BYTE *) GetPtr(g_vmem, g_vmemPreambleSize);
	UnlinkThunkT F = (UnlinkThunkT) (D + *(DWORD*)(D + 24));
	BOOL ret = F(&g_thunkState, &g_thunkContext, &st);
	if(!ret)
		return ret;

	VirtualFree(g_vmem, 0, MEM_DECOMMIT | MEM_RELEASE);
	g_vmem = NULL;
	return ret;
}

static FARPROC FindThunk(LPCSTR name)
{
	XIGNCODE_STUB_ST st;
	InitST(&st);
	BYTE *D = (BYTE *) GetPtr(g_vmem, g_vmemPreambleSize);
	FindThunkT F = (FindThunkT) (D + *(DWORD*)(D + 28));
	return F(&g_thunkState, &g_thunkContext, &st, name);
}

BOOL ZCWAVE_Probe(CPACKETDATA data, PACKETDATA buf, SIZE_T size)
{
	if(!g_initialized)
		return FALSE;

	ZCWAVE_ProbeT F;
	F = (ZCWAVE_ProbeT) FindThunk("ZCWAVE_Probe");
	if(!F)
		return FALSE;

	return F(data, buf, size);
}

BOOL ZCWAVE_SysEnter(LPCTSTR path, DWORD opt)
{
	if(!LinkThunk())
		return FALSE;

	g_initialized = TRUE;

	ZCWAVE_SysEnterT F;
	F = (ZCWAVE_SysEnterT) FindThunk("ZCWAVE_SysEnter");
	if(!F)
		return FALSE;

	return F(path, opt);
}

BOOL ZCWAVE_SysExit()
{
	if(!g_initialized)
		return FALSE;

	ZCWAVE_SysExitT F;
	F = (ZCWAVE_SysExitT) FindThunk("ZCWAVE_SysExit");
	if(!F)
		return FALSE;

	BOOL ret = F();
	UnlinkThunk();
	g_initialized = FALSE;
	return ret;
}

BOOL ZCWAVE_Init()
{
	if(!g_initialized)
		return FALSE;

	ZCWAVE_InitT F;
	F = (ZCWAVE_InitT) FindThunk("ZCWAVE_Init");
	if(!F)
		return FALSE;

	return F();
}

BOOL ZCWAVE_Cleanup()
{
	if(!g_initialized)
		return FALSE;

	ZCWAVE_CleanupT F;
	F = (ZCWAVE_CleanupT) FindThunk("ZCWAVE_Cleanup");
	if(!F)
		return FALSE;

	BOOL ret = F();
	return ret;
}

HANDLE ZCWAVE_CreateCodeBox()
{
	if(!g_initialized)
		return FALSE;

	ZCWAVE_CreateCodeBoxT F;
	F = (ZCWAVE_CreateCodeBoxT) FindThunk("ZCWAVE_CreateCodeBox");
	if(!F)
		return FALSE;

	return F();
}

BOOL ZCWAVE_CloseCodeBox(HANDLE h)
{
 	if(!g_initialized)
		return FALSE;

	ZCWAVE_CloseCodeBoxT F;
	F = (ZCWAVE_CloseCodeBoxT) FindThunk("ZCWAVE_CloseCodeBox");
	if(!F)
		return FALSE;

	return F(h);
}

BOOL ZCWAVE_ProbeCodeBox(HANDLE h, CPACKETDATA data, PACKETDATA buf, SIZE_T size)
{
  	if(!g_initialized)
		return FALSE;

	ZCWAVE_ProbeCodeBoxT F;
	F = (ZCWAVE_ProbeCodeBoxT) FindThunk("ZCWAVE_ProbeCodeBox");
	if(!F)
		return FALSE;

	return F(h, data, buf, size);
}