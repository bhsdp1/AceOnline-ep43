// RTPRouter.cpp : implementation file
//

#include "stdafx.h"
#include "RTPRouter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#include "DbgOut.h"
//#undef TRACE
//#define TRACE DbgOut
static UINT m_nTimerRTPRouter;
//CRTPRouter* pRouter;
void CALLBACK TimerProcInRTPRouter(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime ); 

/////////////////////////////////////////////////////////////////////////////


CRTPRouter::CRTPRouter()
{
}
CRTPRouter::CRTPRouter(CNATSocket* pNAT)
{
	m_pNAT = pNAT;
	m_bFirstRTP = TRUE;
	m_iPacketSize = 36/sizeof(char);
	m_iRTPPort = 4010;
	nWrite = 0;
	m_iTCPCount = 0;
	m_pUDP = NULL;
	
//	pRouter = this;

	m_pPacket = NULL;
	m_nPacketLength = 0;
	m_nCurrentLength = 0;

	Index = 0;

   
}

CRTPRouter::~CRTPRouter()
{
//	KillTimer(NULL,m_nTimerRTPRouter);
	KillTimer(m_pNAT->m_hWnd,20 );
	if(m_pUDP) 
		delete m_pUDP;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CRTPRouter, CAsyncSocket)
	//{{AFX_MSG_MAP(CRTPRouter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CRTPRouter member functions

void CRTPRouter::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("\r\nCRTPRouter::OnReceive");
	int nRead;


	if(m_nPacketLength == 0 )
	{
		nRead = Receive(RouteBuf, 4);  //by cje 010601
	}
	else
	{
//		TRACE("\nProxyPort[%d] Try Receive length[%d - %d]\n",m_iProxyRTPPort,m_nPacketLength,m_nCurrentLength);
		nRead = Receive(RouteBuf, m_nPacketLength-m_nCurrentLength);  //by cje 010601
	}
	
	TRACE("\nProxyPort[%d] Received Data Size [%d]\n", m_iProxyRTPPort,nRead);

	if(nRead != SOCKET_ERROR)
	{
		USHORT nCurrent = AddBlock(RouteBuf,nRead);
		if(nCurrent == m_nPacketLength)
		{
			if( nCurrent != sizeof(ROUTEMSG))
			{
				if(m_bFirstRTP) 
				{
					m_bFirstRTP = FALSE;
					m_iRTPPort = m_pNAT->GetLocalRTPPort();
				}
				
				/*				for(int i=0; i< nRead ; i= i+nlen+2)
				{
				memcpy(&nlen,&RouteBuf[i], 2);
				TRACE(" RTP len : [%d]\n" ,nlen);
				if(m_pUDP) m_pUDP->SendTo(&RouteBuf[i+2], (int)nlen, m_iRTPPort, "127.0.0.1" ,0);
				}
				*/				
				if(m_pUDP) m_pUDP->SendTo(m_pPacket, (int)nCurrent, m_iRTPPort, "127.0.0.1" ,0);
			}
			else if(sizeof(ROUTEMSG) == nCurrent)  //by cje 010601
			{
				ROUTEMSG msg;
				//				memmove(&msg, &RouteBuf, sizeof(ROUTEMSG));
				memmove(&msg, m_pPacket, sizeof(ROUTEMSG));
				/*
				*	
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
				 */
				TRACE("\n[Protocol] [R] [NAT] [RTPRouter] type=%d, protocoltype=%d, port=%d, ip=%s, status=%d\n", msg.type, msg.ptype, msg.endpointAddr.RtpPort, msg.endpointAddr.RtpAddr, msg.UdpStatus);
				switch(msg.type)
				{
				case _ROUTEOK :
					//			AfxMessageBox("RouteOK");
					m_iProxyRTPPort = msg.endpointAddr.RtpPort;
					//			if(m_pNAT->rtpMode >= _UNITCPROUTE)
					TRACE("\nProxyRTPPort Set[%d]\n",m_iProxyRTPPort);
					if(m_pNAT->rtpMode == _UNITCPROUTE)
					{
						if(m_pUDP) m_pUDP->SendTo(&msg, sizeof(msg),m_iProxyRTPPort,m_pNAT->GetProxyAddr() , 0);
						Sleep(100);
						//				AfxMessageBox("CheckUDPStatus");
						CheckUDPStatus();
						
					}
					else
					{
						//m_pNAT->SendDialMsg(m_iProxyRTPPort);
						m_pNAT->SetRTPRoutePort(m_iProxyRTPPort); //by cje 021126
					}
					break;
				case _RESUDPSTATUS :
					if(msg.UdpStatus == _BITCP)
					{ 
						//				m_pNAT->m_bTCPSend = TRUE;
						m_pNAT->rtpMode = _BITCPROUTE;
					}
					else if (msg.UdpStatus == _UNITCP)
					{
						//				m_pNAT->m_bTCPSend = FALSE;
						m_pNAT->rtpMode = _UNITCPROUTE;
					}
					//			AfxMessageBox("RCV_RESUDPSTATUS");
					//m_pNAT->SendDialMsg(m_iProxyRTPPort);
					m_pNAT->SetRTPRoutePort(m_iProxyRTPPort);	//by cje 021126				 
				}
			}
			m_nCurrentLength = 0 ;
			m_nPacketLength = 0 ;
			delete m_pPacket;
			m_pPacket = NULL;
		}
		else if(nCurrent >= m_nPacketLength)  //by cje 020823 nCurrent가 더크다는 것은 잘못된 패킷..
		{
			m_nCurrentLength = 0 ;
			m_nPacketLength = 0 ;
			delete m_pPacket;
			m_pPacket = NULL;
		}
	}
	else
	{
		TRACE("\nReceive Error[%d]\n",GetLastError());
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CRTPRouter::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode)
	{
		TRACE("\nConnection To Proxy Fail.\nPlease try calling again after a while\nIf you see this again, ask your service provider.\n");
		m_pNAT->OnProxyDisconnected(); //by cje 020826
	}
	else
	{
		if(!m_pNAT->m_bProxyConnected)
		{
			m_pNAT->m_bProxyConnected = TRUE;
			TRACE("\nConnection To Proxy Success\n");
			SendRequest();
			//m_nTimerRTPRouter = SetTimer(NULL,NULL, 30000, TimerProcInRTPRouter);
			m_nTimerRTPRouter = SetTimer(m_pNAT->m_hWnd, 20, 30000, NULL);
		}
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

BOOL CRTPRouter::SendRequest()
{
	ROUTEMSG msg;
	msg.type = _ROUTERTP;
//	if(m_pNAT->m_bTCP) //by cje 010601
	if(m_pNAT->rtpMode >= _UNITCPROUTE)
	{
		msg.ptype = _TCP;
	}
	else
	{
		msg.ptype = _UDP;
	}
	if(m_pNAT->m_strUserID.GetLength() < 20)
		strcpy(msg.endpointAddr.RtpAddr, m_pNAT->m_strUserID);
	else
		strncpy(msg.endpointAddr.RtpAddr,m_pNAT->m_strUserID, 20);

	m_pUDP= new CRouteSocket(this);
//	m_pUDP= new CAsyncSocket;
	m_pUDP->Create(0, SOCK_DGRAM, FD_READ,NULL);  //by cje 010601
//	AfxMessageBox("m_pUDPCreate");
//	if(SOCKET_ERROR != Send(&msg, sizeof(msg)))
	if(SOCKET_ERROR != Write((char*)&msg, sizeof(msg)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CRTPRouter::HangUp()
{
//	AfxMessageBox("send hangup");
	ROUTEMSG msg;
	msg.type = _HANGUP;
//	Send(&msg,sizeof(msg));
	Write((char*)&msg,sizeof(msg));

	if(m_pUDP)
	{
		m_pUDP->Close();
//		m_pUDP->m_pRTPRouter->Close(); 
//		delete m_pUDP->m_pRTPRouter; 
//		m_pUDP->m_pRTPRouter = NULL; 
		delete m_pUDP;
		m_pUDP= NULL;
	}
}

void CRTPRouter::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pNAT) 
	{
		KillTimer(m_pNAT->m_hWnd,20 );
		m_pNAT->OnProxyDisconnected();  //by cje 010801
	}
	
//	KillTimer(NULL,m_nTimerRTPRouter);
	CAsyncSocket::OnClose(nErrorCode);
}


unsigned CRTPRouter::GetRouteSockPort()
{
	unsigned port;
	CString tmp;
	if(m_pUDP)
	{
		if(m_pUDP->GetSockName(tmp,port))
		{
			return port;
		}
	}
	return 0;
}

BOOL CRTPRouter::SendGWAddr(CString addr, unsigned int port)
{
	ROUTEMSG msg;
	msg.type = _GWADDR;
	strcpy(msg.endpointAddr.RtpAddr, addr);
	msg.endpointAddr.RtpPort = port;
//	if(SOCKET_ERROR == Send(&msg, sizeof(ROUTEMSG)))
	if(SOCKET_ERROR == Write((char*)&msg, sizeof(ROUTEMSG)))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CRTPRouter::CheckUDPStatus()
{
	ROUTEMSG msg;
	msg.type = _REQUDPSTATUS;
//	if(SOCKET_ERROR == Send(&msg, sizeof(msg)))
	if(SOCKET_ERROR == Write((char*)&msg, sizeof(msg)))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}
/*
void CALLBACK TimerProcInRTPRouter(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
//	AfxMessageBox("on timer RTPRouter");
	enum MessageTypes {
		_ALIVE = 0x16}; //by cje ROUTE
	enum ProtocolTypes {
		_TCP= 0x20,
		_UDP= 0x21
	}; //by cje ROUTE    010601
	enum UDPSTATUS {
		_UNITCP = 0x01,
		_BITCP  = 0x02
	}; //by cje ROUTE    010601

	struct ROUTEMSG {
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
	ROUTEMSG msg;
	msg.type = _ALIVE;
//	pRouter->Send(&msg,sizeof(msg));
	pRouter->Write((char*)&msg,sizeof(msg));
}
*/
void CRTPRouter::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	while(!m_qSendPacket.empty())
	{
		CSendPacket* ps = m_qSendPacket.front();
		int nLength = ps->GetLength();
		char *pBlock = ps->GetPacket();
		if( nLength > 0 && pBlock )
		{
			int nSend = Send(pBlock, nLength, 0);

			
			if (SOCKET_ERROR != nSend)
				{
//					TRACE("\n ClientSession - Send: %d",nLength);
					ps->PopPacket();
					delete pBlock;
					
				}
				else if(GetLastError() == WSAEWOULDBLOCK)
				{
					AsyncSelect();				
					break;
				}else
				{
					TRACE("\n Send Error");
					break;
				}
		}	
		m_qSendPacket.pop();
		delete ps;
	}	
	
	
	CAsyncSocket::OnSend(nErrorCode);
}
USHORT CRTPRouter::AddBlock(char *pBlock, int nLength)
{
	if( m_nPacketLength == 0 )
	{	// header에서 packet size만 빼낸다.
		memcpy( (void*)&m_nPacketLength, pBlock, sizeof(USHORT));
		nLength -= sizeof(USHORT);
		pBlock += sizeof(USHORT);
		Index++;
		
	}
	char *p = new char[nLength+m_nCurrentLength];
	if(m_nCurrentLength > 0 )
	{
		memcpy( p, m_pPacket, m_nCurrentLength );
		delete m_pPacket;
	}
	memcpy( p + m_nCurrentLength, pBlock, nLength );
	m_pPacket = p;
	m_nCurrentLength += nLength;

//	TRACE("\n[%d] ClientSession(%x) - Receive Data Length: %d  / %d",Index,this,m_nCurrentLength, m_nPacketLength );
	return	m_nCurrentLength;
}

int CRTPRouter::Write(char *pData, int size)
{
	CSendPacket* ps = new CSendPacket((char*)pData,size);
	m_qSendPacket.push(ps);
	AsyncSelect();
	return 0;
}

void CRTPRouter::SendAlive()
{
	ROUTEMSG msg;
	msg.type = _ALIVE;
	Write((char*)&msg,sizeof(ROUTEMSG));	
}

int CRTPRouter::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	ROUTEMSG msg;
	memset(&msg, 0x00, sizeof(msg));
	memcpy(&msg, lpBuf, sizeof(msg));
	TRACE("\n[SCDialer1] [Protocol] [Send] [NAT] [RTPRouter] type=%d, protocoltype=%d, port=%d, ip=%s, status=%d\n", msg.type, msg.ptype, msg.endpointAddr.RtpPort, msg.endpointAddr.RtpAddr, msg.UdpStatus);
*/	
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}
