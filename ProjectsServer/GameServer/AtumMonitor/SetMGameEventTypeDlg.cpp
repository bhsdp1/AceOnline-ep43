// SetMGameEventTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atummonitor.h"
#include "SetMGameEventTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetMGameEventTypeDlg dialog


CSetMGameEventTypeDlg::CSetMGameEventTypeDlg(MGAME_EVENT_t i_event, CWnd* pParent /*=NULL*/)
	: CDialog(CSetMGameEventTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetMGameEventTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_enEventType = i_event;
}


void CSetMGameEventTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetMGameEventTypeDlg)
	DDX_Control(pDX, IDC_COMBO_EVENT_TYPE, m_ctlCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetMGameEventTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CSetMGameEventTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetMGameEventTypeDlg message handlers

BOOL CSetMGameEventTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	switch(m_enEventType)
	{
	case MGAME_EVENT_NO_EVENT:
		m_ctlCombo.SelectString(-1, "MGAME_EVENT_NO_EVENT");
		break;
	case MGAME_EVENT_OPEN_BETA_ATTENDANCE:
		m_ctlCombo.SelectString(-1, "MGAME_EVENT_OPEN_BETA_ATTENDANCE");
		break;
	default:
		{
		}
	}
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetMGameEventTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	switch(m_ctlCombo.GetCurSel())
	{
	case 0:
		m_enEventType = MGAME_EVENT_NO_EVENT;
		break;
	case 1:
		m_enEventType = MGAME_EVENT_OPEN_BETA_ATTENDANCE;
		break;
	}

	CDialog::OnOK();
}
