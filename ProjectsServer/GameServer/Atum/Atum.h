// Atum.h : main header file for the ATUM application
//

#if !defined(AFX_ATUM_H__098F59D8_423F_4329_BA81_37158816D7DF__INCLUDED_)
#define AFX_ATUM_H__098F59D8_423F_4329_BA81_37158816D7DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAtumApp:
// See Atum.cpp for the implementation of this class
//

class CAtumApp : public CWinApp
{
public:
	CAtumApp();
	void ReadProfile();
	void WriteProfile();
	void ReadCrocessProfile();
	void DisableXPServicePack2();

	// 2007-09-01 by cmkwon, 웹런처(WebLauncher) 시스템 수정 -
	BOOL UpdateEXE1FilePathResitry(void);

	BOOL RegisterServer(LPCTSTR pszDllName, BOOL bUnregister);		// 2007-06-25 by cmkwon, SCVoIP ActiveX 컨트롤 등록

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAtumApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUM_H__098F59D8_423F_4329_BA81_37158816D7DF__INCLUDED_)
