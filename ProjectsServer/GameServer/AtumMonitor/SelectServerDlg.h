#if !defined(AFX_SELECTSERVERDLG_H__14C97BA6_D054_4955_BF93_D292B8F39235__INCLUDED_)
#define AFX_SELECTSERVERDLG_H__14C97BA6_D054_4955_BF93_D292B8F39235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// selectserverdlg.h : header file
//

#include "AtumParam.h"

extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

/////////////////////////////////////////////////////////////////////////////
// CSelectServerDlg dialog

class CSelectServerDlg : public CDialog
{
// Construction
public:
	CSelectServerDlg(CString i_strPath, int i_nLangType, CString i_strServerName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectServerDlg)
	enum { IDD = IDD_DIALOG_SELECT_SERVER };
	CEdit	m_ctl_editLocalizationPath;
	CComboBox	m_ComboServerList;
	CString	m_ctl_strLocalizationPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	GAME_SERVER_INFO_FOR_ADMIN m_GameServerInfoForAdmin;
	
	int								m_nLanguageT;
	CString							m_strServerName;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectServerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnSelectPath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSERVERDLG_H__14C97BA6_D054_4955_BF93_D292B8F39235__INCLUDED_)
