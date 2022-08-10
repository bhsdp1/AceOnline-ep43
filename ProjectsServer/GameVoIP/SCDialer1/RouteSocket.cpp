// RouteSocket.cpp : implementation file
//

#include "stdafx.h"
#include "RouteSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRouteSocket

CRouteSocket::CRouteSocket()
{
}

CRouteSocket::~CRouteSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRouteSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CRouteSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRouteSocket member functions

void CRouteSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
//	nRead = ReceiveFrom(RouteBuf+2, sizeof(RouteBuf)-2, 0,0,0);
	nRead = ReceiveFrom(RouteBuf, sizeof(RouteBuf), 0,0,0);
	TRACE("ROUTESOCKET nRead: [%d]\n",nRead);
	if(nRead != SOCKET_ERROR)
	{
//		nlen = (unsigned short)nRead;
//		memcpy(RouteBuf,&nlen,2);
//		m_pRTPRouter->Send(RouteBuf,nRead+2,0);
		m_pRTPRouter->Write(RouteBuf,nRead);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

CRouteSocket::CRouteSocket(CRTPRouter *pRouter)
{
	m_pRTPRouter = pRouter;
}
