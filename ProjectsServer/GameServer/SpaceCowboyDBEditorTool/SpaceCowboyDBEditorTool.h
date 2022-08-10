// SpaceCowboyDBEditorTool.h : main header file for the SPACECOWBOYDBEDITORTOOL application
//

#if !defined(AFX_SPACECOWBOYDBEDITORTOOL_H__C7AC4910_D69A_4E4E_BF43_5876DBA0EA1E__INCLUDED_)
#define AFX_SPACECOWBOYDBEDITORTOOL_H__C7AC4910_D69A_4E4E_BF43_5876DBA0EA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolApp:
// See SpaceCowboyDBEditorTool.cpp for the implementation of this class
//

class CSpaceCowboyDBEditorToolApp : public CWinApp
{
public:
	CSpaceCowboyDBEditorToolApp();

	void WriteProfile();
	void ReadProfile();

	CString						m_szAccountNameInputReg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceCowboyDBEditorToolApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpaceCowboyDBEditorToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPACECOWBOYDBEDITORTOOL_H__C7AC4910_D69A_4E4E_BF43_5876DBA0EA1E__INCLUDED_)
