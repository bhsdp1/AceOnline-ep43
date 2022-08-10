// IncommingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCDialer1.h"
#include "IncommingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIncommingDlg dialog

CIncommingDlg::CIncommingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIncommingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIncommingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}

void CIncommingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIncommingDlg)
	DDX_Control(pDX, IDC_STATIC_CALLNUMBER, m_ctrlCallNumber);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIncommingDlg, CDialog)
	//{{AFX_MSG_MAP(CIncommingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIncommingDlg message handlers

BOOL CIncommingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
/*	CSCDialer1Ctrl *m_pMainCtrl = (CSCDialer1Ctrl*)AfxGetMainWnd();
	CString strtemp;
	strtemp = m_pMainCtrl->strCaller;
	SetDlgItemText(IDC_STATIC_CALLNUMBER, strtemp);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
