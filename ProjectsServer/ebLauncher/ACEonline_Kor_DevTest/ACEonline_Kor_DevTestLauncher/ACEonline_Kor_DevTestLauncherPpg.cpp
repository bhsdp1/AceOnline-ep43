// ACEonline_Kor_DevTestLauncherPpg.cpp : Implementation of the CACEonline_Kor_DevTestLauncherPropPage property page class.

#include "stdafx.h"
#include "ACEonline_Kor_DevTestLauncher.h"
#include "ACEonline_Kor_DevTestLauncherPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACEonline_Kor_DevTestLauncherPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CACEonline_Kor_DevTestLauncherPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CACEonline_Kor_DevTestLauncherPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CACEonline_Kor_DevTestLauncherPropPage, "ACEONLINEKORDEVTESTLAUNCHER.ACEonlineKorDevTestLauncherPropPage.1",
	0x1ee05e9e, 0x81e1, 0x4237, 0xb4, 0x48, 0x45, 0xf8, 0x1d, 0xf0, 0xc5, 0xde)


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherPropPage::CACEonline_Kor_DevTestLauncherPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CACEonline_Kor_DevTestLauncherPropPage

BOOL CACEonline_Kor_DevTestLauncherPropPage::CACEonline_Kor_DevTestLauncherPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACEONLINE_KOR_DEVTESTLAUNCHER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherPropPage::CACEonline_Kor_DevTestLauncherPropPage - Constructor

CACEonline_Kor_DevTestLauncherPropPage::CACEonline_Kor_DevTestLauncherPropPage() :
	COlePropertyPage(IDD, IDS_ACEONLINE_KOR_DEVTESTLAUNCHER_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CACEonline_Kor_DevTestLauncherPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherPropPage::DoDataExchange - Moves data between page and properties

void CACEonline_Kor_DevTestLauncherPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CACEonline_Kor_DevTestLauncherPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherPropPage message handlers
