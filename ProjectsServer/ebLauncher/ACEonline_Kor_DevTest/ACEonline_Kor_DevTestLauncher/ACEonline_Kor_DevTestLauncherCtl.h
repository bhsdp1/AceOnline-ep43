#if !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHERCTL_H__5BAC2215_3AE9_498F_856B_28F0B917A0D4__INCLUDED_)
#define AFX_ACEONLINE_KOR_DEVTESTLAUNCHERCTL_H__5BAC2215_3AE9_498F_856B_28F0B917A0D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ACEonline_Kor_DevTestLauncherCtl.h : Declaration of the CACEonline_Kor_DevTestLauncherCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CACEonline_Kor_DevTestLauncherCtrl : See ACEonline_Kor_DevTestLauncherCtl.cpp for implementation.

class CACEonline_Kor_DevTestLauncherCtrl : public COleControl
{
	DECLARE_DYNCREATE(CACEonline_Kor_DevTestLauncherCtrl)

// Constructor
public:
	CACEonline_Kor_DevTestLauncherCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACEonline_Kor_DevTestLauncherCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL
	
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업
	HANDLE	m_hMutexMonoInstance;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업 - 뮤텍스 작업

// Implementation
protected:
	~CACEonline_Kor_DevTestLauncherCtrl();

	DECLARE_OLECREATE_EX(CACEonline_Kor_DevTestLauncherCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CACEonline_Kor_DevTestLauncherCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CACEonline_Kor_DevTestLauncherCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CACEonline_Kor_DevTestLauncherCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CACEonline_Kor_DevTestLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CACEonline_Kor_DevTestLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	afx_msg short SetGameArgument(LPCTSTR szGameId, LPCTSTR szGamePassWord, LPCTSTR szSeed, LPCTSTR szGameType, LPCTSTR szAuthenticationKey, LPCTSTR szBirthday);
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CACEonline_Kor_DevTestLauncherCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CACEonline_Kor_DevTestLauncherCtrl)
		// NOTE: ClassWizard will add and remove enumeration elements here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISP_ID
	};

private:
	// 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
	CString m_strACEonlineInstallPath;
	// end 2012-08-13 by mspark, 에이스 온라인 ActiveX 작업
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACEONLINE_KOR_DEVTESTLAUNCHERCTL_H__5BAC2215_3AE9_498F_856B_28F0B917A0D4__INCLUDED)
