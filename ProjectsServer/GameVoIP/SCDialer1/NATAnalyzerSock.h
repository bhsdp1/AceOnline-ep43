#if !defined(AFX_NATANALYZERSOCK_H__5EC3EA5A_6404_461F_89EF_51BF8C6EE001__INCLUDED_)
#define AFX_NATANALYZERSOCK_H__5EC3EA5A_6404_461F_89EF_51BF8C6EE001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NATAnalyzerSock.h : header file
//

#define WM_NATANALYZER_RESULT		WM_USER+150

class CNATAnalyzer;
/////////////////////////////////////////////////////////////////////////////
// CNATAnalyzerSock command target
class CNATAnalyzerSock : public CAsyncSocket
{
// Attributes
public:
	char			m_Buffer[128];
	UINT			m_nOpenPort;
	char			m_strPeerIP[16];		// 네트워크 타입을 분석하기위한 상대방 IP Address
	UINT			m_nPeerPort;			// 네트워크 타입을 분석하기위한 상대방 Port
	CNATAnalyzer*	m_pNATAnalyer;
	char			m_strRecvIP[16];
	UINT			m_nRecvPort;

// Operations
public:
	CNATAnalyzerSock(CNATAnalyzer* pAnalyzer);
	virtual ~CNATAnalyzerSock();

	
	BOOL LocalOpenPort(UINT port);
	void SetPeerInfo(char* ip, UINT port);
	int SendData(char* pData, int nLength);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNATAnalyzerSock)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CNATAnalyzerSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NATANALYZERSOCK_H__5EC3EA5A_6404_461F_89EF_51BF8C6EE001__INCLUDED_)
