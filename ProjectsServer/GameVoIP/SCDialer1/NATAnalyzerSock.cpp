// NATAnalyzerSock.cpp : implementation file
//

#include "stdafx.h"
//#include "CDlg.h"
#include "NATAnalyzerSock.h"
#include "NATAnalyzer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNATAnalyzerSock

CNATAnalyzerSock::CNATAnalyzerSock(CNATAnalyzer* pAnalyzer)
{
	m_nOpenPort		= 0;
	memset(m_strPeerIP, 0x00, sizeof(m_strPeerIP));
	m_nPeerPort		= 0;
	m_pNATAnalyer	= pAnalyzer;
	memset(m_strRecvIP, 0x00, sizeof(m_strRecvIP));
	m_nRecvPort		= 0;
}

CNATAnalyzerSock::~CNATAnalyzerSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CNATAnalyzerSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CNATAnalyzerSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CNATAnalyzerSock member functions

void CNATAnalyzerSock::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int nReturn;
	CString strIP;
	UINT nPort;

	if(nErrorCode){ return;}

	nReturn = ReceiveFrom(m_Buffer, sizeof(m_Buffer), strIP, nPort);
	if(nReturn != sizeof(UDPMSG))
	{
		if(nReturn == SOCKET_ERROR)
		{
			m_pNATAnalyer->KillResendTimer();
			if(m_pNATAnalyer->GetStatus() == _N_A_SEND)
			{
				m_pNATAnalyer->m_eErrorInfo = ERR_NO_RESPONSE_SERVER;
			}
			else
			{
				m_pNATAnalyer->m_eErrorInfo = ERR_No_RESPOnSE_FIRETALK;
			}
			m_pNATAnalyer->NetworkTypeResult(_NULL_TYPE);
		}
		return;
	}

	UDPMSG msg;
	memcpy(&msg, m_Buffer, sizeof(UDPMSG));

	switch(msg.type)
	{
	case _ANALYZE_NAT_A_ACK:
		{
//			if(strcmp(msg.Data.natAackmsg.strRouteIP, m_pNATAnalyer->m_strServerIP) == 0 
//				&& msg.Data.natAackmsg.uRoutePort == m_pNATAnalyer->m_nServerPort && m_pNATAnalyer->GetStatus() == _N_A_SEND) 
			if( m_pNATAnalyer->GetStatus() == _N_A_SEND) // by cje
			{
				m_pNATAnalyer->KillResendTimer();
			
				char		lpszHostName[255];
				char		szTemp[255];
				PHOSTENT	pHostEntry;
				char		szIP[20];
				
				if(gethostname(lpszHostName,255) != 0 ) //호스트 이름 얻기
				{	wsprintf(szTemp, "gethostname() error : %d", WSAGetLastError());}
				else 
				{
					CString rSocketAddress;
					UINT rSocketPort; 
					m_pNATAnalyer->m_pAnalyzerSock->GetSockName(rSocketAddress, rSocketPort);
					
					if ( ( pHostEntry = gethostbyname( lpszHostName ) ) == NULL ) //호스트 이름로 호스트 entry 얻기
					{	wsprintf( szTemp, "gethostbyname() error : %d", WSAGetLastError() );}
					else
					{						
						for(int i= 0; pHostEntry->h_addr_list[i]; i++)
						{
							wsprintf(szIP, "%d.%d.%d.%d", (BYTE)pHostEntry->h_addr_list[i][0], (BYTE)pHostEntry->h_addr_list[i][1],
								(BYTE)pHostEntry->h_addr_list[i][2], (BYTE)pHostEntry->h_addr_list[i][3]);
							if(strcmp(msg.Data.natAackmsg.strRouteIP, szIP) == 0 && msg.Data.natAackmsg.uRoutePort == rSocketPort) 
							{
								//Open Internet...
								m_pNATAnalyer->NetworkTypeResult(_PUBLIC_TYPE);
								break;
							}
							else
							{
								if(!pHostEntry->h_addr_list[i+1])
								{
									// cmkwon 20020710
									m_pNATAnalyer->NetworkTypeResult(_NAT_CONE);
									break;
								}								
							}
						}					
					}
				} 
				
			}
		}
		break;
	case _ANALYZE_NAT_B_ACK:
		{
			switch(m_pNATAnalyer->GetStatus())
			{				
			case _N_A_ACK_B_SEND:
				// NAT_A패킷에 대한 응답을 받고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
				{
					//m_pNATAnalyer->SetStatus(_N_A_ACK_B_ACK);
					m_pNATAnalyer->KillResendTimer();
					TRACE("\n from Server Recv IP[%s], Port[%d]", msg.Data.natBackmsg.strRouteIP, msg.Data.natBackmsg.uRoutePort);
					m_pNATAnalyer->NetworkTypeResult(_NAT_CONE);
					
				}
				break;
			case _N_A_NOACK_B_SEND:
				// NAT_A패킷에 대한 응답을 받지 못하고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
				{
					//m_pNATAnalyer->SetStatus(_N_A_NOACK_B_ACK);
					m_pNATAnalyer->KillResendTimer();
					m_pNATAnalyer->NetworkTypeResult(_NAT_SYMETRIC);	
				}
				break;
			}
		}
		break;
	default:
		{
			TRACE("Error : CNATAnalyzerSock::OnReceive type[%d] error", msg.type);
		}
	}	
	
	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CNATAnalyzerSock::LocalOpenPort(UINT port)
{
	if(port <= 0){ return FALSE;}

	BOOL quit = TRUE;
	
	do
	{
		if(Create(port, SOCK_DGRAM))
		{
			m_nOpenPort = port;
			quit = FALSE;
		}
		else
		{
			port+=2;
			quit++;
		}

	}while(quit && quit < 20);

	return ~quit;
}

void CNATAnalyzerSock::SetPeerInfo(char* ip, UINT port)
{
	strcpy(m_strPeerIP, ip);
	m_nPeerPort		= port;
}

int CNATAnalyzerSock::SendData(char* pData, int nLength)
{
	if(strcmp(m_strPeerIP, "") == 0 || m_nPeerPort <= 0)
	{
		return FALSE;
	}

	int Re = SendTo(pData, nLength, m_nPeerPort, m_strPeerIP);
	if(SOCKET_ERROR == Re){ return FALSE;}

	return Re;
}