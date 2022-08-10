#if !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHER_H__DCBD7817_3324_45BF_AAB8_CDE010FB0B05__INCLUDED_)
#define AFX_ACEONLINE_KOR_DEVTESTLAUNCHER_H__DCBD7817_3324_45BF_AAB8_CDE010FB0B05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonline_Kor_DevTestLauncher.h : main header file for ACEONLINE_KOR_DEVTESTLAUNCHER.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherApp : See ACEonline_Kor_DevTestLauncher.cpp for implementation.

class CACEonline_Kor_DevTestLauncherApp : public COleControlModule
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

#endif // !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHER_H__DCBD7817_3324_45BF_AAB8_CDE010FB0B05__INCLUDED)
