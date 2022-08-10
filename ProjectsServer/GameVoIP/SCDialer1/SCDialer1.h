#if !defined(AFX_SCDialer1_H__61BAB5C0_3CF1_4FB7_A633_3063D2516502__INCLUDED_)
#define AFX_SCDialer1_H__61BAB5C0_3CF1_4FB7_A633_3063D2516502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer1.h : main header file for SCDialer1.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCDialer1App : See SCDialer1.cpp for implementation.

class CSCDialer1App : public COleControlModule
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

#endif // !defined(AFX_SCDialer1_H__61BAB5C0_3CF1_4FB7_A633_3063D2516502__INCLUDED)
