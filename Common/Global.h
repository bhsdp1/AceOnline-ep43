#ifndef _GROBAL_VARIABEL_H_
#define _GROBAL_VARIABEL_H_

#include "SystemLogManager.h"

class CGlobal;


///////////////////////////////////////////////////////////////////////////////
// Global Variable
///////////////////////////////////////////////////////////////////////////////
extern CGlobal *			g_pGlobal;


//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 윈도우 메세지 프로시저


///////////////////////////////////////////////////////////////////////////////
/// \class		CGlobal
///
/// \brief		전역 변수를 관리하기 위한 클래스
/// \author		cmkwon
/// \version	
/// \date		2004-03-18 ~ 2004-03-18
/// \warning	객체를 하나만 생성해야함.
///////////////////////////////////////////////////////////////////////////////
class CGlobal
{
public:
	CGlobal();
	virtual ~CGlobal();

	///////////////////////////////////////////////////////////////////////////
	// Property
	CSystemLogManager * GetSystemLogManagerPtr(void);
	
	virtual BOOL InitGlobal(char * i_szServerName);
	BOOL CreateDuplicateRun(char * i_szServerName);		// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행

	///////////////////////////////////////////////////////////////////////////
	// public Method
	static BOOL CheckWriteLog(char *szLogMsg);		// 2011-06-22 by hskim, 사설 서버 방지
	static BOOL WriteSystemLog(char *szLogMsg, BOOL bTimeHeaderFlag = TRUE);
	static BOOL WriteSystemLogEX(BOOL bPrintDBGOUT, const char* pFormat, ...);

protected:		

	HANDLE							m_hMutexMonoInstance;

	//////////////////////////////////////////////////////////////////////////
	// 로그 관련 멤버 변수
	static CSystemLogManager 		ms_SystemLogManager;			// 모든 서버에서 시스템 관련 로그를 저장하기 위한 멤버변수

};


#endif	// _GROBAL_VARIABEL_H_endif