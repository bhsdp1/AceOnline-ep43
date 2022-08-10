#if !defined(AFX_ROUTESOCKET_H__A4D30CA6_F2BD_43E6_9233_EAC9B03B0C9A__INCLUDED_)
#define AFX_ROUTESOCKET_H__A4D30CA6_F2BD_43E6_9233_EAC9B03B0C9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RouteSocket.h : header file
//


#include "RTPRouter.h"
/////////////////////////////////////////////////////////////////////////////
// CRouteSocket command target
class CRTPRouter;
class CRouteSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CRouteSocket();
	virtual ~CRouteSocket();

// Overrides
public:
	CRouteSocket(CRTPRouter* pRouter);
	CRTPRouter* m_pRTPRouter;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRouteSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRouteSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	int nRead;
	unsigned short nlen;
	char RouteBuf[50000];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTESOCKET_H__A4D30CA6_F2BD_43E6_9233_EAC9B03B0C9A__INCLUDED_)
