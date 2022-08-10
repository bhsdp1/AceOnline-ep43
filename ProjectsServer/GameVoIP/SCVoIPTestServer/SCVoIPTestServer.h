// SCVoIPTestServer.h : main header file for the SCVoIPTestServer application
//

#if !defined(AFX_SCVoIPTestServer_H__963929A5_8603_4D17_A250_F9ED97D8F710__INCLUDED_)
#define AFX_SCVoIPTestServer_H__963929A5_8603_4D17_A250_F9ED97D8F710__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPTestServerApp:
// See SCVoIPTestServer.cpp for the implementation of this class
//

class CSCVoIPTestServerApp : public CWinApp
{
public:
	CSCVoIPTestServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCVoIPTestServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSCVoIPTestServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCVoIPTestServer_H__963929A5_8603_4D17_A250_F9ED97D8F710__INCLUDED_)
