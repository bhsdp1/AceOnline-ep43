// SCVoIPTestServerDlg.h : header file
//

#if !defined(AFX_SCVoIPTestServerDLG_H__C135D51C_9FAB_4431_80BA_44C250C176DA__INCLUDED_)
#define AFX_SCVoIPTestServerDLG_H__C135D51C_9FAB_4431_80BA_44C250C176DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSCVoIPTestServerDlg dialog

class CSCVoIPTestServerDlg : public CDialog
{
// Construction
public:
	HWND		m_hWndSCVoIPTestServerClient;
	CSCVoIPTestServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSCVoIPTestServerDlg)
	enum { IDD = IDD_SCVoIPTestServer_DIALOG };
	CEdit	m_editStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCVoIPTestServerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSCVoIPTestServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBtnExecute();
	afx_msg void OnBtnCallerPc2pc();
	afx_msg void OnBtnCallEnd();
	afx_msg void OnBtnExit();
	afx_msg void OnBTN1to1REGISTER();
	afx_msg void OnBTNNtoNCREATEROOM();
	afx_msg void OnBTNNtoNJOINROOM();
	afx_msg void OnBtnTalk();
	afx_msg void OnBTN1to1SETVOL();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCVoIPTestServerDLG_H__C135D51C_9FAB_4431_80BA_44C250C176DA__INCLUDED_)
