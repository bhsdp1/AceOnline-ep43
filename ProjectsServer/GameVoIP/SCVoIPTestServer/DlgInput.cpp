// DlgInput.cpp : implementation file
//

#include "stdafx.h"
#include "SCVoIPTestServer.h"
#include "DlgInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInput dialog


CDlgInput::CDlgInput(AVCallType i_GameVoIPCallType, int nUseType, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInput)
	m_ctl_szCharacterName = _T("");
	m_clt_nAccountUniqueNumber = 0;
	m_ctl_dwPartyID = 0;
	//}}AFX_DATA_INIT

	m_GameVoIPCallType = i_GameVoIPCallType;
	m_nUseType = nUseType;
}


void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInput)
	DDX_Text(pDX, IDC_EDIT_CHARACTER, m_ctl_szCharacterName);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_clt_nAccountUniqueNumber);
	DDX_Text(pDX, IDC_EDIT_PARTYID, m_ctl_dwPartyID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialog)
	//{{AFX_MSG_MAP(CDlgInput)
	ON_BN_CLICKED(IDC_RADIO_GUILD, OnRadioGuild)
	ON_BN_CLICKED(IDC_RADIO_PARTY, OnRadioParty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInput message handlers

BOOL CDlgInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetWindowText("SpaceCowboy VoIP");
	::SendMessage(GetDlgItem(IDC_RADIO_PARTY)->GetSafeHwnd(), BM_SETCHECK, 1, 0);
	m_bPartyCallFlag = TRUE;
	if(_1to1Call == m_GameVoIPCallType)
	{
		GetDlgItem(IDC_STATIC_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTYID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_PARTY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_GUILD)->ShowWindow(SW_HIDE);
		if(1 == m_nUseType)
		{// 콜을 할때

			GetDlgItem(IDC_STATIC_1)->SetWindowText("상대방 AccountUniqueNumber :");
			GetDlgItem(IDC_STATIC_2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_CHARACTER)->ShowWindow(SW_HIDE);			
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInput::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}

void CDlgInput::OnRadioParty() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_3)->SetWindowText("PartyID :");
	m_bPartyCallFlag = TRUE;
}

void CDlgInput::OnRadioGuild() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_3)->SetWindowText("GuildUniqueNumber :");
	m_bPartyCallFlag = FALSE;
}

