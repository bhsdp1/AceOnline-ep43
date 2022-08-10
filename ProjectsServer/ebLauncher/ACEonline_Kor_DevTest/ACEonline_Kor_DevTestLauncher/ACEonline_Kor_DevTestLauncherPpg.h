#if !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHERPPG_H__30304F3F_004C_4A44_A991_F414FA32B2FA__INCLUDED_)
#define AFX_ACEONLINE_KOR_DEVTESTLAUNCHERPPG_H__30304F3F_004C_4A44_A991_F414FA32B2FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonline_Kor_DevTestLauncherPpg.h : Declaration of the CACEonline_Kor_DevTestLauncherPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherPropPage : See ACEonline_Kor_DevTestLauncherPpg.cpp.cpp for implementation.

class CACEonline_Kor_DevTestLauncherPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CACEonline_Kor_DevTestLauncherPropPage)
	DECLARE_OLECREATE_EX(CACEonline_Kor_DevTestLauncherPropPage)

// Constructor
public:
	CACEonline_Kor_DevTestLauncherPropPage();

// Dialog Data
	//{{AFX_DATA(CACEonline_Kor_DevTestLauncherPropPage)
	enum { IDD = IDD_PROPPAGE_ACEONLINE_KOR_DEVTESTLAUNCHER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CACEonline_Kor_DevTestLauncherPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHERPPG_H__30304F3F_004C_4A44_A991_F414FA32B2FA__INCLUDED)
