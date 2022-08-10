// SCDialer2Ppg.cpp : Implementation of the CSCDialer2PropPage property page class.

#include "stdafx.h"
#include "SCDialer2.h"
#include "SCDialer2Ppg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSCDialer2PropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSCDialer2PropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSCDialer2PropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSCDialer2PropPage, "SCDIALER2.SCDialer2PropPage.1",
	0x1c732466, 0x5632, 0x4104, 0xaa, 0x45, 0x9c, 0xc9, 0x7, 0xa2, 0x8e, 0x2)


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2PropPage::CSCDialer2PropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSCDialer2PropPage

BOOL CSCDialer2PropPage::CSCDialer2PropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SCDIALER2_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2PropPage::CSCDialer2PropPage - Constructor

CSCDialer2PropPage::CSCDialer2PropPage() :
	COlePropertyPage(IDD, IDS_SCDIALER2_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSCDialer2PropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2PropPage::DoDataExchange - Moves data between page and properties

void CSCDialer2PropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSCDialer2PropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSCDialer2PropPage message handlers
