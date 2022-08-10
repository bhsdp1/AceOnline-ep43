// VDGlobal.h: interface for the CVDGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VDGLOBAL_H__00B90D44_963E_4514_AF6E_AD42AF410E2A__INCLUDED_)
#define AFX_VDGLOBAL_H__00B90D44_963E_4514_AF6E_AD42AF410E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VoIPGlobal.h"


class CVDGlobal;
//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
extern CVDGlobal		*g_pVDGlobal;		// 전역변수관리를 위한 전역클래스포인터

class CVDIOCP;
class CVDGlobal : public CVoIPGlobal  
{
public:
	CVDGlobal();
	virtual ~CVDGlobal();



	///////////////////////////////////////////////////////////////////////////
	// virtual Function	
	virtual BOOL InitServerSocket(void);						// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);							// 소켓관련 종료 함수
	virtual BOOL LoadConfiguration(void);						// Server 설정을 위한 Configuration Load

protected:
	

};

#endif // !defined(AFX_VDGLOBAL_H__00B90D44_963E_4514_AF6E_AD42AF410E2A__INCLUDED_)
