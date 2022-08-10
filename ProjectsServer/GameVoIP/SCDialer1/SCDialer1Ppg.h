#if !defined(AFX_SCDialer1PPG_H__E59D71F6_1C04_4695_ADDE_C5A2B572D025__INCLUDED_)
#define AFX_SCDialer1PPG_H__E59D71F6_1C04_4695_ADDE_C5A2B572D025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// SCDialer1Ppg.h : Declaration of the CSCDialer1PropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CSCDialer1PropPage : See SCDialer1Ppg.cpp.cpp for implementation.

class CSCDialer1PropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSCDialer1PropPage)
	DECLARE_OLECREATE_EX(CSCDialer1PropPage)

// Constructor
public:
	CSCDialer1PropPage();

// Dialog Data
	//{{AFX_DATA(CSCDialer1PropPage)
	enum { IDD = IDD_PROPPAGE_SCDialer1 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CSCDialer1PropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCDialer1PPG_H__E59D71F6_1C04_4695_ADDE_C5A2B572D025__INCLUDED)
