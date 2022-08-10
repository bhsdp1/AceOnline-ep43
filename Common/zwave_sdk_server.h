#ifndef ZWAVE_SDK_SERVER_H
#define ZWAVE_SDK_SERVER_H

#if	(defined(_DEBUG)) || \
	(defined(_MT)&&defined(_DLL)) || \
	(!defined(_MT)&&!defined(_DLL))
#pragma comment(linker, "/NODEFAULTLIB:libc")
#endif

#if _MSC_VER == 1310
#pragma comment(lib, "zwave_sdk_server_2003.lib")
#elif _MSC_VER == 1400
#pragma comment(lib, "zwave_sdk_server.lib")
#elif _MSC_VER == 1200
#pragma comment(lib, "zwave_sdk_server_6.lib")
#else
#error Unsupported compiler.
#endif

#define ZWAVE_STATE_STOP 0x0001
#define ZWAVE_STATE_RUN 0x0002
#define ZWAVE_STATE_P 0x0004
#define ZWAVE_STATE_Q 0x0008

enum Z_SCAN_TYPE_LIST
{
	Z_SCAN_TYPE_BASE = 100,

	// 더미 일회용 실행코드 (내부에서 사용)
	Z_SCAN_TYPE_ANTI_DUMMY,
	// 논클라이언트 탐지 일회용 실행코드
	Z_SCAN_TYPE_ANTI_NONECLIENTBOT,
	// 치트엔진 탐지 일회용 실행코드
	Z_SCAN_TYPE_ANTI_CHEAT_ENGINE
};

	
// 결과값
enum Z_RETURN
{
	// 잘못된 패킷
	Z_RTN_ERROR = -1,
	// 정상적인 클라이언트
	Z_RTN_NONE, 
	// 논클라이언트 봇 클라이언트
	Z_RTN_NONCLIENT,
	// 치트엔진 탐지
	Z_RTN_BLACK_CODE,
	// 치트엔진 추정 탐지
	Z_RTN_SUSPICIOUS,
	// 매크로 탐지
	Z_RTN_USERDEFINED,
	// 사용자 정의

	// 패킷 재전송
	Z_RTN_RESEND
};

typedef struct _ZWAVE_DEBUG_INFOA
{
	// 전체 생성시 패킷 사이즈
	int							make_BlockSize;

	// 전체 패킷중 코드 블럭 사이즈
	int							code_BlockSize;

	// 파일의 전체 크기
	int							code_FileSize;

	// 전체 파일사이즈 / 코드블럭사이즈 = 코드 사이즈 블럭 갯수
	int							code_HashCount;

	// File open Handle
	FILE						*z_fp;

	// 코드 파일명
	char						szCodeFile[MAX_PATH];

	// 리턴값
	char						szCodeResult[MAX_PATH];

	// 함수명
	char						szFunction[MAX_PATH];

	// 사용하는 테이블
	int							code_Index_Table;

	// 사용하는 레코드 번호
	int							code_Database_Index;
} ZWAVE_DEBUG_INFOA, *PZWAVE_DEBUG_INFOA;

typedef struct _ZWAVE_DEBUG_INFOW
{
	// 전체 생성시 패킷 사이즈
	int							make_BlockSize;

	// 전체 패킷중 코드 블럭 사이즈
	int							code_BlockSize;

	// 파일의 전체 크기
	int							code_FileSize;

	// 전체 파일사이즈 / 코드블럭사이즈 = 코드 사이즈 블럭 갯수
	int							code_HashCount;

	// File open Handle
	FILE						*z_fp;

	// 코드 파일명
	WCHAR						szCodeFile[MAX_PATH];

	// 리턴값
	WCHAR						szCodeResult[MAX_PATH];
	
	// 함수명
	WCHAR						szFunction[MAX_PATH];

	// 사용하는 테이블
	int							code_Index_Table;

	// 사용하는 레코드 번호
	int							code_Database_Index;
} ZWAVE_DEBUG_INFOW, *PZWAVE_DEBUG_INFOW;

typedef char *PACKETDATA;
typedef const char *CPACKETDATA;

BOOL WINAPI ZSWAVE_InitializeW(LPCWSTR path);
BOOL WINAPI ZSWAVE_InitializeA(LPCSTR path);
BOOL WINAPI ZSWAVE_Uninitialize();

DWORD WINAPI ZSWAVE_GetDBConnectionCount();
BOOL WINAPI ZSWAVE_GetLibraryPathW(LPWSTR path, SIZE_T size);
BOOL WINAPI ZSWAVE_GetLibraryPathA(LPSTR path, SIZE_T size);
int WINAPI ZSWAVE_GetGroupCount();
int WINAPI ZSWAVE_GetGroupDataCount(int index);
HANDLE WINAPI ZSWAVE_OpenAt(int gi, int di, SIZE_T bs);
HANDLE WINAPI ZSWAVE_Open(SIZE_T bs, DWORD type);
DWORD WINAPI ZSWAVE_WaitForStop(DWORD t);
BOOL WINAPI ZSWAVE_Close(HANDLE h);
BOOL WINAPI ZSWAVE_GetDataAt(HANDLE h, int no, PACKETDATA data);
BOOL WINAPI ZSWAVE_Verify(HANDLE h, CPACKETDATA data, SIZE_T size);
int WINAPI ZSWAVE_VerifyExA(HANDLE h, CPACKETDATA data, SIZE_T size, LPSTR msg, SIZE_T msgSize);
int WINAPI ZSWAVE_VerifyExW(HANDLE h, CPACKETDATA data, SIZE_T size, LPWSTR msg, SIZE_T msgSize);
BOOL WINAPI ZSWAVE_SetVerifyValueA(HANDLE h, LPCSTR v);
BOOL WINAPI ZSWAVE_SetVerifyValueW(HANDLE h, LPCWSTR v);
BOOL WINAPI ZSWAVE_DebugInfoA(HANDLE h, PZWAVE_DEBUG_INFOA info);
BOOL WINAPI ZSWAVE_DebugInfoW(HANDLE h, PZWAVE_DEBUG_INFOW info);
LONG WINAPI ZSWAVE_GetState();
BOOL WINAPI ZSWAVE_SetState(LONG s);
int WINAPI ZSWAVE_GetDataCount(HANDLE h);

#ifdef _UNICODE
#define ZWAVE_DEBUG_INFO ZWAVE_DEBUG_INFOW
#define ZSWAVE_Initialize ZSWAVE_InitializeW
#define ZSWAVE_GetLibraryPath ZSWAVE_GetLibraryPathW
#define ZSWAVE_VerifyEx ZSWAVE_VerifyExW
#define ZSWAVE_SetVerifyValue ZSWAVE_SetVerifyValueW
#define ZSWAVE_DebugInfo ZSWAVE_DebugInfoW
#else
#define ZWAVE_DEBUG_INFO ZWAVE_DEBUG_INFOA
#define ZSWAVE_Initialize ZSWAVE_InitializeA
#define ZSWAVE_GetLibraryPath ZSWAVE_GetLibraryPathA
#define ZSWAVE_VerifyEx ZSWAVE_VerifyExA
#define ZSWAVE_SetVerifyValue ZSWAVE_SetVerifyValueA
#define ZSWAVE_DebugInfo ZSWAVE_DebugInfoA
#endif

#ifndef ZWAVE_ERROR_CODE
#define ZWAVE_STATUS_INVALID_HANDLE				0xE0190001
#define ZWAVE_STATUS_CODE_NOT_FOUND				0xE0190002
#define ZWAVE_STATUS_NOT_INITIALIZED			0xE0190003
#define ZWAVE_STATUS_UNICODE_CONVERSION_FAIL	0xE0190004
#define ZWAVE_STATUS_INVALID_PARAMETER			0xE0190005
#define ZWAVE_STATUS_INVALID_DATABASE			0xE0190006
#define ZWAVE_STATUS_INSUFFICIENT_RESOURCE		0xE0190007
#define ZWAVE_STATUS_UNHANDLED_EXCEPTION		0xE0190008
#define ZWAVE_STATUS_INVALID_DLL				0xE0190009
#endif

#endif
