#if !defined(AFX_NATSOCKET1_H__BDB14C93_28B7_48E5_98DE_32677F5B3B5D__INCLUDED_)
#define AFX_NATSOCKET1_H__BDB14C93_28B7_48E5_98DE_32677F5B3B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NATSocket1.h : header file
//
#include "Protocol_FireTalk_II.h"
//#include "DialerBase.h"
#include "RTPRouter.h"  //by cje ROUTE
/////////////////////////////////////////////////////////////////////////////
// CNATSocket command target
#define UM_NATRESULT WM_USER +123
enum RTPMODE {  
		_SIMPLE = 0x3f,	
		_DIRECT = 0x40, 
		_UNIUDPROUTE,
		_BIUDPROUTE,
		_UNITCPROUTE, 
		_BITCPROUTE,
		_NOROUTEMODE    = 0x45 
		};


/*#define NATSERVERPORT1			7999
#define NATSERVERPORT2			7998
#define NATPROXYPORT1			7997
#define NATPROXYPORT2			7996
*/
#define BASERTPPORT			4010



//class CClient;
class CRTPRouter;
class CNATSocket : public CAsyncSocket
{
// Attributes


public:

// Operations
public:
	CNATSocket();
//	CNATSocket(CClient* pointer,unsigned int port,  BOOL bRoute, BOOL bTCP, BOOL bTCPSend);
//  	CNATSocket(HWND hWnd,unsigned int port,  BOOL bRoute, BOOL bTCP, BOOL bTCPSend);
    	CNATSocket(HWND hWnd,unsigned int port,  long mode, int mediaIndex,
			CString strUserID);
    
	virtual ~CNATSocket();

	void SetServerAddr(LPCTSTR Address, unsigned int Port);

	void SendToGateway(LPCTSTR Address, const unsigned int Port);
	unsigned int GetGlobalRTPPort();
	void         SetGlobalRTPPort();
	void         SetGlobalRTPPort(unsigned int Port);
	unsigned int GetLocalRTPPort();
	void		 SetLocalRTPPort();
	void		 SetLocalRTPPort(unsigned int Port);
	

// Overrides
public:
	CString m_strUserID;
	void SetRTPRoutePort(unsigned int uRoutePort);
	void SendNATMapAlive();
	void SendActiveConfirm();
	void SendActiveConfirm(CString peerip, unsigned int peerport);
	void SendAlive();
	void OnProxyDisconnected();
	void SetJapanLocalCall(BOOL japanlocal);
	void SetProxyAddr(CString proxyaddr, unsigned int proxyport);
	BOOL SendGWAddr(CString addr, unsigned port);
	unsigned GetRouteSockPort();
//	CClient*        GetClient();
	BOOL			m_bProxyConnected;
//	BOOL			m_bJAPANLOCAL;
	void			CloseProxy();
	void			SendDialMsg(unsigned rtpport);
	CString			GetProxyAddr();
	unsigned int	GetProxyPort();
	BOOL			ConnectProxy();
//	BOOL						m_bTCP;
//	BOOL						m_bROUTE;
//	BOOL						m_bTCPSend;
	RTPMODE			rtpMode;
	BOOL			m_bNATCreated;
	BOOL						m_bSetProxy;
	CString			m_strActivePeerIP;
	unsigned int	m_uActivePeerPort;
	CString			m_strPassivePeerIP; //by cje 021112
	unsigned int	m_uPassivePeerPort;
    BOOL			m_bPassivePeerSet;
	
	unsigned int	m_uRTPRoutePort; //by cje 021126
	unsigned int	m_uLocalRTPPort;
		
	HWND						m_hWnd; //by cje 010719	
	int							m_iMediaIndex;
	CRTPRouter *				m_pRouter;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNATSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CNATSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
	
private:
 
	BOOL						m_bSendDial;
	int							udp_error;
	CString						m_strSetverIP;
	unsigned int				m_uServerPort;
	UDPMSG						msg_rtp;
	unsigned int				rtpport_Global,rtcpport_Global;	
	unsigned int				m_uCreatePort;
	struct sockaddr_in			m_cagent_addr1,m_cagent_addr2; 
	BOOL						m_bRcvdUDP; 
	unsigned int 				m_iOutside1, m_iOutside2;
	CString						strDebug; 
	unsigned int				m_iReCreation ; //   UDP 家南 积己冉荐
//	CClient *					m_pClient;
	CString						m_strFireTalkIP;
	unsigned int				m_uFireTalkPort;
	char						m_msgBuf[50000];
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NATSOCKET1_H__BDB14C93_28B7_48E5_98DE_32677F5B3B5D__INCLUDED_)
