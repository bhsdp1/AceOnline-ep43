// AtumLauncher.h : main header file for the ATUMLAUNCHER application
//

#if !defined(AFX_ATUMLAUNCHER_H__EA010DDF_7F31_402F_B1D6_FF7991CF5DE0__INCLUDED_)
#define AFX_ATUMLAUNCHER_H__EA010DDF_7F31_402F_B1D6_FF7991CF5DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherApp:
// See AtumLauncher.cpp for the implementation of this class
//

class CAtumLauncherApp : public CWinApp
{
public:
	CAtumLauncherApp();
	void ReadProfile();
	void WriteProfile();
	void ReadCrocessProfile();
	void DisableXPServicePack2();

	// 2012-10-21 by mspark, 게임 종료 후 프로세스에 남아있는 ACEonline.atm 제거
	void RemainingProcessRemove();
	// end 2012-10-21 by mspark, 게임 종료 후 프로세스에 남아있는 ACEonline.atm 제거

	HANDLE					m_hMutexMonoInstance;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumLauncherApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAtumLauncherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMLAUNCHER_H__EA010DDF_7F31_402F_B1D6_FF7991CF5DE0__INCLUDED_)
