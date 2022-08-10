// SCDialer1Ppg.cpp : Implementation of the CSCDialer1PropPage property page class.

#include "stdafx.h"
#include "SCDialer1.h"
#include "SCDialer1Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSCDialer1PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSCDialer1PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSCDialer1PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCDialer1PropPage, "SCDialer1.SCDialer1PropPage.1",
	0xda49297e, 0xf55, 0x45cb, 0xa5, 0x84, 0xc1, 0xb2, 0x47, 0x9d, 0x15, 0xfb)



/////////////////////////////////////////////////////////////////////////////
// CSCDialer1PropPage::CSCDialer1PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCDialer1PropPage

BOOL CSCDialer1PropPage::CSCDialer1PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SCDialer1_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1PropPage::CSCDialer1PropPage - Constructor

CSCDialer1PropPage::CSCDialer1PropPage() :
	COlePropertyPage(IDD, IDS_SCDialer1_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSCDialer1PropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1PropPage::DoDataExchange - Moves data between page and properties

void CSCDialer1PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSCDialer1PropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer1PropPage message handlers
