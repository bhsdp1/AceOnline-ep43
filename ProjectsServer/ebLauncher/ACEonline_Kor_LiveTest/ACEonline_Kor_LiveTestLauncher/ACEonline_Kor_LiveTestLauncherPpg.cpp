// ACEonline_Kor_LiveTestLauncherPpg.cpp : Implementation of the CACEonline_Kor_LiveTestLauncherPropPage property page class.

#include "stdafx.h"
#include "ACEonline_Kor_LiveTestLauncher.h"
#include "ACEonline_Kor_LiveTestLauncherPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CACEonline_Kor_LiveTestLauncherPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CACEonline_Kor_LiveTestLauncherPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CACEonline_Kor_LiveTestLauncherPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CACEonline_Kor_LiveTestLauncherPropPage, "ACEONLINEKORLIVETESTLAUNCHER.ACEonlineKorLiveTestLauncherPropPage.1",
	0xd35cb825, 0xd4a, 0x4b4b, 0x81, 0xbb, 0x82, 0x98, 0x21, 0x7b, 0x1b, 0x2a)


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherPropPage::CACEonline_Kor_LiveTestLauncherPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CACEonline_Kor_LiveTestLauncherPropPage

BOOL CACEonline_Kor_LiveTestLauncherPropPage::CACEonline_Kor_LiveTestLauncherPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACEONLINE_KOR_LIVETESTLAUNCHER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherPropPage::CACEonline_Kor_LiveTestLauncherPropPage - Constructor

CACEonline_Kor_LiveTestLauncherPropPage::CACEonline_Kor_LiveTestLauncherPropPage() :
	COlePropertyPage(IDD, IDS_ACEONLINE_KOR_LIVETESTLAUNCHER_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CACEonline_Kor_LiveTestLauncherPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherPropPage::DoDataExchange - Moves data between page and properties

void CACEonline_Kor_LiveTestLauncherPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CACEonline_Kor_LiveTestLauncherPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherPropPage message handlers
