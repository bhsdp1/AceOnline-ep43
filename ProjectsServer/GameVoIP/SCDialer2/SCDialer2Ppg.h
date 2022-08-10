#if !defined(AFX_SCDIALER2PPG_H__CC534187_C332_453A_8EC3_D36EB7DB7EF5__INCLUDED_)
#define AFX_SCDIALER2PPG_H__CC534187_C332_453A_8EC3_D36EB7DB7EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer2Ppg.h : Declaration of the CSCDialer2PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSCDialer2PropPage : See SCDialer2Ppg.cpp.cpp for implementation.

class CSCDialer2PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSCDialer2PropPage)
	DECLARE_OLECREATE_EX(CSCDialer2PropPage)

// Constructor
public:
	CSCDialer2PropPage();

// Dialog Data
	//{{AFX_DATA(CSCDialer2PropPage)
	enum { IDD = IDD_PROPPAGE_SCDIALER2 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSCDialer2PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDIALER2PPG_H__CC534187_C332_453A_8EC3_D36EB7DB7EF5__INCLUDED)
