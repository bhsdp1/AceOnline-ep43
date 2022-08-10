#if !defined(AFX_DLGINPUT_H__2ECED914_8BE4_4FBF_9886_3C295618798F__INCLUDED_)
#define AFX_DLGINPUT_H__2ECED914_8BE4_4FBF_9886_3C295618798F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInput.h : header file
//

#include "VoIPProtocol.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog

class CDlgInput : public CDialog
{
// Construction
public:
	CDlgInput(AVCallType i_GameVoIPCallType, int nUseType, CWnd* pParent = NULL);   // standard constructor


	int				m_GameVoIPCallType;
	int				m_nUseType;
	BOOL			m_bPartyCallFlag;

// Dialog Data
	//{{AFX_DATA(CDlgInput)
	enum { IDD = IDD_DIALOG_INPUT };
	CString	m_ctl_szCharacterName;
	int		m_clt_nAccountUniqueNumber;
	DWORD	m_ctl_dwPartyID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInput)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioGuild();
	afx_msg void OnRadioParty();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINPUT_H__2ECED914_8BE4_4FBF_9886_3C295618798F__INCLUDED_)
