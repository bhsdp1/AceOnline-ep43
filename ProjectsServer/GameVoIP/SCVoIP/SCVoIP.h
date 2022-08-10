// SCVoIP.h : main header file for the SCVoIP application
//

#if !defined(AFX_SCVoIP_H__275EA954_BCA2_458D_BDC8_1F961F04862E__INCLUDED_)
#define AFX_SCVoIP_H__275EA954_BCA2_458D_BDC8_1F961F04862E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "DbgOut_c.h"
/////////////////////////////////////////////////////////////////////////////
// CSCVoIPApp:
// See SCVoIP.cpp for the implementation of this class
//
class CSCVoIPDlg;
class CSCVoIPApp : public CWinApp
{
private:
	HANDLE			m_hMonoInstance;
	CSCVoIPDlg	*m_pSCVoIPDlg;

public:
	CSCVoIPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCVoIPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSCVoIPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCVoIP_H__275EA954_BCA2_458D_BDC8_1F961F04862E__INCLUDED_)
