#if !defined(AFX_INCOMMINGDLG_H__87EA186D_126F_4D01_A541_54B72A8DFE63__INCLUDED_)
#define AFX_INCOMMINGDLG_H__87EA186D_126F_4D01_A541_54B72A8DFE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IncommingDlg.h : header file
//
#include "SCDialer1Ctl.h"

/////////////////////////////////////////////////////////////////////////////
// CIncommingDlg dialog

class CIncommingDlg : public CDialog
{
// Construction
public:
	CIncommingDlg(CWnd* pParent = NULL);   // standard constructor
	CSCDialer1Ctrl *m_pMainCtrl;

// Dialog Data
	//{{AFX_DATA(CIncommingDlg)
	enum { IDD = IDD_DIALOG_CALL };
	CStatic	m_ctrlCallNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIncommingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIncommingDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCOMMINGDLG_H__87EA186D_126F_4D01_A541_54B72A8DFE63__INCLUDED_)
