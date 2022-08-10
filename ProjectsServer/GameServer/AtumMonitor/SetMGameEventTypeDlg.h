#if !defined(AFX_SETMGAMEEVENTTYPEDLG_H__39056585_84C9_468E_9197_919718696FA2__INCLUDED_)
#define AFX_SETMGAMEEVENTTYPEDLG_H__39056585_84C9_468E_9197_919718696FA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetMGameEventTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetMGameEventTypeDlg dialog

class CSetMGameEventTypeDlg : public CDialog
{
// Construction
public:
	CSetMGameEventTypeDlg(MGAME_EVENT_t i_event, CWnd* pParent = NULL);   // standard constructor

	MGAME_EVENT_t			m_enEventType;
// Dialog Data
	//{{AFX_DATA(CSetMGameEventTypeDlg)
	enum { IDD = IDD_DLG_SET_MGAME_EVENT_TYPE };
	CComboBox	m_ctlCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetMGameEventTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetMGameEventTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETMGAMEEVENTTYPEDLG_H__39056585_84C9_468E_9197_919718696FA2__INCLUDED_)
