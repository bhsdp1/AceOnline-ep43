#if !defined(AFX_ACEONLINE_KOR_LIVETESTLAUNCHERPPG_H__0EF546A2_54C3_4FE9_A1E5_3D018B8361A2__INCLUDED_)
#define AFX_ACEONLINE_KOR_LIVETESTLAUNCHERPPG_H__0EF546A2_54C3_4FE9_A1E5_3D018B8361A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonline_Kor_LiveTestLauncherPpg.h : Declaration of the CACEonline_Kor_LiveTestLauncherPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_LiveTestLauncherPropPage : See ACEonline_Kor_LiveTestLauncherPpg.cpp.cpp for implementation.

class CACEonline_Kor_LiveTestLauncherPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CACEonline_Kor_LiveTestLauncherPropPage)
	DECLARE_OLECREATE_EX(CACEonline_Kor_LiveTestLauncherPropPage)

// Constructor
public:
	CACEonline_Kor_LiveTestLauncherPropPage();

// Dialog Data
	//{{AFX_DATA(CACEonline_Kor_LiveTestLauncherPropPage)
	enum { IDD = IDD_PROPPAGE_ACEONLINE_KOR_LIVETESTLAUNCHER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CACEonline_Kor_LiveTestLauncherPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINE_KOR_LIVETESTLAUNCHERPPG_H__0EF546A2_54C3_4FE9_A1E5_3D018B8361A2__INCLUDED)
