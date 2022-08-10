#if !defined(AFX_RTPROUTER_H__CA926BDE_EC62_4286_8181_8C0315EBB992__INCLUDED_)
#define AFX_RTPROUTER_H__CA926BDE_EC62_4286_8181_8C0315EBB992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RTPRouter.h : header file
//
///////////////////////////////
#pragma warning(disable:4786)
#include <queue>
using namespace std;

#include "winpacket.h"
//////////////////////////by cje 020724

#include "NATSocket1.h"
#include "RouteSocket.h"
/////////////////////////////////////////////////////////////////////////////
// CRTPRouter command target


class CNATSocket;
class CRouteSocket;

class CRTPRouter : public CAsyncSocket
{
enum MessageTypes {
	_ROUTERTP= 0x10,
		_ROUTEOK ,
		_GWADDR ,
		_REQUDPSTATUS ,
		_RESUDPSTATUS ,
		_HANGUP ,
		_ALIVE = 0x16}; //by cje ROUTE
	enum ProtocolTypes {
		_TCP= 0x20,
			_UDP= 0x21
	}; //by cje ROUTE    010601
	enum UDPSTATUS {
		_UNITCP = 0x01,
			_BITCP  = 0x02
	}; //by cje ROUTE    010601

struct ROUTEMSG{
	MessageTypes type;
	union{
		ProtocolTypes ptype;
		struct ADDRESS{
			unsigned long RtpPort;
			char RtpAddr[20];
		} endpointAddr;
		UDPSTATUS     UdpStatus;
	};
};


// Attributes
public:

// Operations
public:
	CRTPRouter();
	CRTPRouter(CNATSocket* pNAT);
	virtual ~CRTPRouter();

// Overrides
public:
	void SendAlive();
	BOOL CheckUDPStatus();
	BOOL SendGWAddr(CString addr, unsigned port);
	unsigned GetRouteSockPort();
	void		HangUp();
	BOOL		SendRequest();
	CString		m_strProxyAddr;
	unsigned	m_iProxyRTPPort;

//////////////////////////////////////////////////
	char* 			m_pPacket;
	USHORT			m_nPacketLength;
	USHORT			m_nCurrentLength;
	USHORT			AddBlock(char *pBlock, int nLength);
	UINT			Index;
 
	queue<CSendPacket*> m_qSendPacket;	// Send packet queue
	int				Write(char* pData, int size);
//////////////////////////////////////////////////by cje 020724
	
protected:
	CRouteSocket*      m_pUDP;    //by cje 010601
//	CAsyncSocket*      m_pUDP;    //by cje 010601
	CNATSocket*		   m_pNAT;  
	char		RouteBuf[50000];
	int			nWrite;
	BOOL		m_bFirstRTP;
	int         m_iPacketSize;
	unsigned    m_iRTPPort;
	int			m_iTCPCount;
	unsigned short		nlen;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTPRouter)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRTPRouter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTPROUTER_H__CA926BDE_EC62_4286_8181_8C0315EBB992__INCLUDED_)
