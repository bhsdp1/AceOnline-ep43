#if !defined(AFX_ACEONLINE_KOR_LIVETESTLAUNCHER_H__8E609966_BE5C_45DC_A9AD_43F20059CDBE__INCLUDED_)
#define AFX_ACEONLINE_KOR_LIVETESTLAUNCHER_H__8E609966_BE5C_45DC_A9AD_43F20059CDBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonline_Kor_LiveTestLauncher.h : main header file for ACEONLINE_KOR_LIVETESTLAUNCHER.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherApp : See ACEonline_Kor_LiveTestLauncher.cpp for implementation.

class CACEonline_Kor_LiveTestLauncherApp : public COleControlModule
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

#endif // !defined(AFX_ACEONLINE_KOR_LIVETESTLAUNCHER_H__8E609966_BE5C_45DC_A9AD_43F20059CDBE__INCLUDED)
