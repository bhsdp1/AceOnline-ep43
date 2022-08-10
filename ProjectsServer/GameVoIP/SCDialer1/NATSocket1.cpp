// NATSocket1.cpp : implementation file
//

#include "stdafx.h"
#include "NATSocket1.h"
#include "Protocol_FireTalk_II.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNATSocket
//static CClient* pClient;
/*static HWND m_handle;
static CNATSocket* pNAT;
static UINT m_nTimerID;
*/
//void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime ); 

CNATSocket::CNATSocket()
{
	m_bSendDial = FALSE;
	m_bRcvdUDP = FALSE;
	m_iReCreation = 0;
	udp_error = 0;
	m_bProxyConnected = FALSE;
	srand((unsigned)time(NULL)); // by cje 010402
	rtpMode = _DIRECT;
	m_pRouter = NULL;
	m_bNATCreated = FALSE;
	TRACE("NATCreated FALSE2\n");
	m_bSetProxy = FALSE;
	m_bPassivePeerSet = FALSE;
}
CNATSocket::CNATSocket(HWND hWnd ,unsigned int port, long mode, int mediaIndex,CString strUserID)
{
	
	m_bSendDial = FALSE;
	m_uCreatePort = port;
	m_bRcvdUDP = FALSE;
	m_iReCreation = 0;
	udp_error = 0;
	m_bProxyConnected = FALSE;
	srand((unsigned)time(NULL)); // by cje 010402
	rtpMode = _DIRECT;
	if(mode >=64 && mode <=69)
		rtpMode = (RTPMODE)mode;
	m_hWnd = hWnd;
/*	m_handle = hWnd;
	pNAT = this;
*/
	m_pRouter = NULL;
	m_bNATCreated = FALSE;
	TRACE("NATCreated FALSE3\n");
	m_bSetProxy = FALSE;
//	m_bJAPANLOCAL = FALSE;
	m_iMediaIndex =mediaIndex;
	m_bPassivePeerSet = FALSE;
	m_strUserID = strUserID;
}
CNATSocket::~CNATSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CNATSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CNATSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CNATSocket member functions
void CNATSocket::SetLocalRTPPort(unsigned int Port)
{
	m_uCreatePort = Port;
}
void CNATSocket::SetLocalRTPPort()
{
	CString tmp;
	m_cagent_addr1.sin_family = AF_INET;
	m_cagent_addr1.sin_port = htons(m_uServerPort);
	m_cagent_addr1.sin_addr.S_un.S_addr = inet_addr((LPCTSTR)m_strSetverIP);

//	m_cagent_addr2.sin_family = AF_INET;
//	m_cagent_addr2.sin_port = htons(m_uServerPort);
//	m_cagent_addr2.sin_addr.S_un.S_addr = inet_addr((LPCTSTR)m_strSetverIP);

	m_uCreatePort = BASERTPPORT;
	for(int i = 0; i < 20; )
	{
		if(Create(m_uCreatePort,SOCK_DGRAM,FD_READ,NULL)==0)
		{
			int e;
			e = GetLastError();
			CString tmp;
			tmp.Format("error num : %d\n",e);

			m_uCreatePort += 2;
			i++;
		}
		else
		{
			m_bNATCreated = TRUE;
			TRACE("NATCreated TRUE1\n");
			break;
		}

		if(i==20) 
		{m_uCreatePort = 0;
//		AfxMessageBox("make m_uCreatePort 0");
		}

	}

//	tmp.Format("create udp socket on port:%d  \n",m_uCreatePort);
//	strDebug += tmp;
//	AfxMessageBox(strDebug);
}

unsigned int CNATSocket::GetLocalRTPPort()
{
	return m_uCreatePort;
}

unsigned int CNATSocket::GetGlobalRTPPort()
{
	return rtpport_Global;
}
void CNATSocket::SetGlobalRTPPort(unsigned int Port)
{
	rtpport_Global = Port;
}
void CNATSocket::SetGlobalRTPPort()
{
//	AfxMessageBox("still OK2");	
	msg_rtp.type = _SEEK_PUBLIC_INFO;

	TRACE("\nTry SetGlobalRTPPort\n");
	if(SOCKET_ERROR ==SendTo((char*)&msg_rtp, sizeof(msg_rtp),
		(SOCKADDR*)&m_cagent_addr1  ,sizeof(sockaddr_in),0  ) 
		)
	{
		int errnum = WSAGetLastError();
		udp_error = 1;
//		strDebug += "send to CA Error  \n ";
//		CString tmp;
//		tmp.Format("sendto err %d\n",errnum);
//		AfxMessageBox(tmp);
//		TRACE(tmp);
	}
	else
	{
		strDebug += "send to CA OK \n";
		TRACE("send to CA OK\n");
	}
//	CString tmp;
//	tmp.Format("m_bSenddial : %d\n" ,m_bSendDial);
//	AfxMessageBox(tmp);
//	m_nTimerID = SetTimer(NULL,NULL, 5000, TimerProc);
	SetTimer(m_hWnd,30,3000,NULL);	
}

void CNATSocket::SendToGateway(LPCTSTR Address, const unsigned int Port)
{
	struct sockaddr_in		m_gateway_addr;
	m_gateway_addr.sin_family			= AF_INET;
	m_gateway_addr.sin_port				= htons((unsigned short)Port);
	m_gateway_addr.sin_addr.S_un.S_addr = inet_addr((LPCTSTR)Address);
	if(SOCKET_ERROR !=SendTo((char*)&msg_rtp, sizeof(msg_rtp),
		(SOCKADDR*)&m_gateway_addr  ,sizeof(sockaddr_in) ,0 )  //gw로 전송
		)
	{
//		CString str;
//		str.Format("send to gw port :%d\n",(unsigned short)Port);
//		strDebug +=str;
//		AfxMessageBox(str);
	}
		//clarent gateway는 sending port = receiving port -2
		
}

void CNATSocket::SetServerAddr(LPCTSTR Address, unsigned int Port)
{
	m_strSetverIP = Address;
	m_uServerPort = Port;
}


void CNATSocket::OnReceive(int nErrorCode) 
{

	// TODO: Add your specialized code here and/or call the base class
	CString tmp;
//AfxMessageBox("Receive_OK");
	int rcvlen;
	rcvlen = ReceiveFrom(m_msgBuf,sizeof(m_msgBuf),m_strActivePeerIP,m_uActivePeerPort,0);
	if(SOCKET_ERROR == rcvlen)
	{
		//udp packet unreachable인 경우도 해당됨
		int err =WSAGetLastError();
		TRACE("\nNATSocket_RcvError:%d (ip=%s, port=%d)\n",err,m_strActivePeerIP,m_uActivePeerPort);
		if(m_bSendDial) return;
		m_bSendDial = TRUE;
		udp_error = 1;
		strDebug += "FD_READ Rcv error \n";
		Close(); // by cje 010319
		m_bNATCreated = FALSE;
		rtpport_Global = 0 ;
		rtpMode  = _UNITCPROUTE;
		if(m_bSetProxy)
		{
			TRACE("SendMessage port0-3\n");
			SendMessage(m_hWnd,UM_NATRESULT, (WPARAM)m_iMediaIndex,(LPARAM)0);
		}
		else
		{
			SendMessage(m_hWnd,UM_NATRESULT, (WPARAM)m_iMediaIndex,(LPARAM)-1);
		}
	}
	else
	{	

		//메시지 내에서 매핑된 주소만 얻으면 된다...
		//Cone일때만 여기가 수행된다.
		if(rcvlen == sizeof(UDPMSG))
		{
			if(!m_bRcvdUDP) //by cje 021126 맨위에서 여기로 이동
			{
				KillTimer(m_hWnd,30);
				TRACE("killtimer\n");
				m_bRcvdUDP = TRUE;
			}
			memcpy((void*)&msg_rtp,m_msgBuf,sizeof(UDPMSG));
			if(msg_rtp.type != _ACTIVE_CONFIRM)
			{
				if(m_bSendDial) return;
				m_bSendDial = TRUE;
				rtpport_Global = msg_rtp.Data.natBackmsg.uRoutePort;
				SendDialMsg(rtpport_Global);
			}
			else
			{
//				Close();
//				m_bNATCreated = FALSE;
				SendMessage(m_hWnd,UM_NATRESULT, (WPARAM)m_iMediaIndex,(LPARAM)-3);
			}
		}
		else //by cje 021126 _UNIUDPROUTE 또는 _BIUDPROUTE일때만 여기가 실행된다.
		{
			if( 0 == m_strActivePeerIP.Compare(m_strFireTalkIP)  && m_uActivePeerPort == m_uRTPRoutePort)
			{
				//FireTalk로 부터 받은 패킷
				SendTo(m_msgBuf,rcvlen,m_uLocalRTPPort,"127.0.0.1");
			}
			else
			{
				//FireTalk로 보낼 패킷
				SendTo(m_msgBuf,rcvlen,m_uRTPRoutePort,m_strFireTalkIP);
			}
		}
		
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}
void CNATSocket::SetProxyAddr(CString proxyaddr, unsigned int proxyport)
{
	m_strFireTalkIP = proxyaddr;
	m_uFireTalkPort = proxyport;
	m_bSetProxy = TRUE;
}
BOOL CNATSocket::ConnectProxy()
{
	m_pRouter = new CRTPRouter(this);
	m_pRouter->Create();

	BOOL bNodelay = TRUE;
    m_pRouter->SetSockOpt(TCP_NODELAY, (void*)&bNodelay, sizeof(BOOL),IPPROTO_TCP);

	CString strtmp;
	strtmp.Format("proxy : %s:%d",m_strFireTalkIP, m_uFireTalkPort);
//	AfxMessageBox(strtmp);
	if(m_bSetProxy)
	{
		m_pRouter->Connect(m_strFireTalkIP, m_uFireTalkPort);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


unsigned int CNATSocket::GetProxyPort()
{
	if(m_pRouter)
	{
		return (unsigned int)m_pRouter->m_iProxyRTPPort;  //중계할 포트
	}
	else
	{
		return 0;
	}
}

CString CNATSocket::GetProxyAddr()
{
	return m_strFireTalkIP;
}

void CNATSocket::SendDialMsg(unsigned int rtpport)
{

	TRACE("m_bNATCreated : %d\n", m_bNATCreated);

//	if(m_bNATCreated) AsyncSelect(0);
	if(rtpMode == _BIUDPROUTE || rtpMode == _UNIUDPROUTE)
	{
		UDPMSG msg;
//		AfxMessageBox("route_udp");
		SendTo(&msg, sizeof(UDPMSG), GetProxyPort(),m_strFireTalkIP , 0); //TCP일때는 error..
		GetLastError();
		KillTimer(m_hWnd,32); 
//		Close();
//		m_bNATCreated = FALSE;
//		TRACE("NATCreated FALSE6\n");
//by cje 021126
	}
	SendMessage(m_hWnd,UM_NATRESULT, (WPARAM)m_iMediaIndex,(LPARAM)rtpport);
}

void CNATSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CNATSocket::CloseProxy()
{
	if(m_pRouter)
	{
		if(m_bProxyConnected)
		{
			m_pRouter->HangUp();
			m_pRouter->Close();
		}
	//	delete m_pRouter;
	//	m_pRouter = NULL;
	}
}
unsigned CNATSocket::GetRouteSockPort()
{
	if(m_pRouter)
	{
		return m_pRouter->GetRouteSockPort();
	}
	else
	{
		return 0;
	}
}

BOOL CNATSocket::SendGWAddr(CString addr, unsigned int port)
{
	if(m_pRouter)
	{
		if(m_pRouter->SendGWAddr(addr, port))
		{
			return TRUE;
		}
	}
	return FALSE;
	
}
/*void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
	KillTimer(hwnd,idEvent);
	TRACE("killtimer2");
//	AfxMessageBox("timeout");
	pNAT->rtpMode  = _UNITCPROUTE;
	pNAT->Close();
	pNAT->m_bNATCreated = FALSE;
	TRACE("NATCreated FALSE7\n");
	if(pNAT->m_bSetProxy)
	{
		TRACE("SensMessage port0-1\n");
		SendMessage(m_handle,UM_NATRESULT, (WPARAM)pNAT->m_iMediaIndex,(LPARAM)0);
	}
	else
	{
		SendMessage(m_handle,UM_NATRESULT, (WPARAM)pNAT->m_iMediaIndex,(LPARAM)-1);
	}
}
*/

void CNATSocket::SetJapanLocalCall(BOOL japanlocal) // it's for JENS (BIUDPROUTE)
{
//	m_bJAPANLOCAL = japanlocal;
}
void CNATSocket::OnProxyDisconnected()  //by cje 010801
{
	m_bProxyConnected = FALSE;
	SendMessage(m_hWnd,UM_NATRESULT, (WPARAM)m_iMediaIndex,(LPARAM)-2);//hangup
}

void CNATSocket::SendAlive()
{
	if(m_pRouter) m_pRouter->SendAlive();
}

void CNATSocket::SendActiveConfirm(CString peerip, unsigned int peerport)
{
	m_strPassivePeerIP = peerip;
	m_uPassivePeerPort = peerport;
	m_bPassivePeerSet = TRUE;
	UDPMSG msgActive;
	msgActive.type = _ACTIVE_CONFIRM;
	TRACE("SendActiveConfirm to %s:%d\n",m_strPassivePeerIP,m_uPassivePeerPort);
	SendTo(&msgActive,sizeof(UDPMSG),m_uPassivePeerPort,m_strPassivePeerIP);
}

void CNATSocket::SendActiveConfirm()
{
	if(m_bPassivePeerSet)
	{
		UDPMSG msgActive;
		msgActive.type = _ACTIVE_CONFIRM;
		TRACE("SendActiveConfirm to %s:%d\n",m_strPassivePeerIP,m_uPassivePeerPort);
		SendTo(&msgActive,sizeof(UDPMSG),m_uPassivePeerPort,m_strPassivePeerIP);
	}
}

void CNATSocket::SendNATMapAlive()
{
	if(m_bNATCreated)
	{
		UDPMSG msgAlive;
		SendTo(&msgAlive,sizeof(UDPMSG),GetProxyPort(), m_strFireTalkIP);
	}
}

void CNATSocket::SetRTPRoutePort(unsigned int uRoutePort)
{
	m_uRTPRoutePort = uRoutePort;
	SendDialMsg(uRoutePort);
}
