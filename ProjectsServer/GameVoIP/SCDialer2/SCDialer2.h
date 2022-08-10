#if !defined(AFX_SCDIALER2_H__D982B6C4_8E2B_4844_AA03_DDF42245CDC3__INCLUDED_)
#define AFX_SCDIALER2_H__D982B6C4_8E2B_4844_AA03_DDF42245CDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer2.h : main header file for SCDIALER2.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCDialer2App : See SCDialer2.cpp for implementation.

class CSCDialer2App : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDIALER2_H__D982B6C4_8E2B_4844_AA03_DDF42245CDC3__INCLUDED)
