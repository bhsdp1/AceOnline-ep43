// NATAnalyzer.cpp: implementation of the CNATAnalyzer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "CDlg.h"
#include "NATAnalyzer.h"
#include <Mmsystem.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNATAnalyzer::CNATAnalyzer()
{
	m_pAnalyzerSock			= NULL;
	m_hMsgWnd				= NULL;
	memset(m_strServerIP, 0x00, sizeof(m_strServerIP));
	m_nServerPort			= 0;
	memset(m_strFireTalkIP, 0x00, sizeof(m_strFireTalkIP));
	m_nFireTalkPort			= 0;
	m_bReady				= FALSE;	
	m_Status				= _N_NULL;
	m_nSendtoServerCount	= 0;
//	m_nSendtoFireTalkCount	= 0;
	m_nSendBtoServerCount	= 0;
	m_eErrorInfo			= ERR_NOTHING;
	m_bMMTimerSetFlag		= FALSE;
	m_nMMTimerID			= 0;
	m_eNetworkType			= _NULL_TYPE;
	m_nPortNum				= 1026;
}

CNATAnalyzer::~CNATAnalyzer()
{
	if(m_pAnalyzerSock)
	{
		m_pAnalyzerSock->Close();
		delete m_pAnalyzerSock;
		m_pAnalyzerSock = NULL;
	}
}

NATAnalyzerStatus CNATAnalyzer::GetStatus()
{ 
	return m_Status;
}

void CNATAnalyzer::SetStatus(NATAnalyzerStatus status)
{	
	m_Status = status;
	TRACE("\n Status %d", m_Status);
}

NATErrorInfos CNATAnalyzer::GetErrorInfos()
{
	return m_eErrorInfo;
}
void CNATAnalyzer::SetErrorInfos(NATErrorInfos err)
{
	m_eErrorInfo = err;
}
void CNATAnalyzer::GetErrorInfosString(char* str, NATErrorInfos err)
{
	NATErrorInfos tmp;

	if(err == ERR_NOTHING && m_eErrorInfo == ERR_NOTHING)
	{
		strcpy(str, "The error does not exist !");
		return;
	}
	else if(err == ERR_NOTHING)
	{
		tmp = m_eErrorInfo;
	}
	else
	{
		tmp = err;
	}

	switch(tmp)
	{
	case ERR_NO_RESPONSE_SERVER:
		{
			sprintf(str, "The Server does not reply !\n IP[%s], Port[%d]", m_strServerIP, m_nServerPort);
		}
		break;
	case ERR_No_RESPOnSE_FIRETALK:
		{
			sprintf(str, "The FireTalk II does not reply !\n IP[%s], Port[%d]", m_strFireTalkIP, m_nFireTalkPort);
		}
		break;
	}
}

BOOL CNATAnalyzer::SetServersInfo(HWND hWnd, char* serverIP, UINT serverPort, char* fireTalkIP, UINT fireTalkPort)
{
	if(!IsWindow(hWnd) || strcmp(serverIP, "") == 0 || serverPort <= 0 || strcmp(fireTalkIP, "") == 0 || fireTalkPort <= 0 
		|| strcmp(serverIP, fireTalkIP) == 0 || serverPort == fireTalkPort)
	{
		return FALSE;
	}

	m_hMsgWnd		= hWnd;
	strcpy(m_strServerIP, serverIP);
	m_nServerPort	= serverPort;
	strcpy(m_strFireTalkIP, fireTalkIP);
	m_nFireTalkPort	= fireTalkPort;	
	m_bReady		= TRUE;
	SetStatus(_N_READY);

	return TRUE;
}
BOOL CNATAnalyzer::SetServerInfo(HWND hWnd, char* serverIP, UINT serverPort)
{
	if(!IsWindow(hWnd) || strcmp(serverIP, "") == 0 || serverPort <= 0 )
	{
		return FALSE;
	}

	m_hMsgWnd		= hWnd;
	strcpy(m_strServerIP, serverIP);
	m_nServerPort	= serverPort;
	m_bServerReady		= TRUE;
	if(m_bFireTalkReady && m_bServerReady)
	{
		m_bReady = TRUE;
		SetStatus(_N_READY);
	}

	return TRUE;
}
BOOL CNATAnalyzer::SetFireTalkInfo(char* fireTalkIP, UINT fireTalkPort)
{
	if( strcmp(fireTalkIP, "") == 0 || fireTalkPort <= 0 )
	{
		return FALSE;
	}

	strcpy(m_strFireTalkIP, fireTalkIP);
	m_nFireTalkPort	= fireTalkPort;	
	m_bFireTalkReady		= TRUE;
	if(m_bFireTalkReady && m_bServerReady)
	{
		m_bReady = TRUE;
		SetStatus(_N_READY);
	}
	return TRUE;
}
BOOL CNATAnalyzer::ExcuteAnalysis()
{
	if(!m_bReady && GetStatus() != _N_READY)
	{
		if(GetStatus() != _NULL_TYPE)
		{
			AfxMessageBox("Wait a moment !!");
		}
		return FALSE;
	}
	
	if(m_pAnalyzerSock)
	{
		m_pAnalyzerSock->Close();
		delete m_pAnalyzerSock;
		m_pAnalyzerSock = NULL;
	}

	m_pAnalyzerSock = new CNATAnalyzerSock(this);
	
	//if(!m_pAnalyzerSock->LocalOpenPort(m_nPortNum)){ return FALSE;}
	if(!m_pAnalyzerSock->LocalOpenPort(unsigned( rand()*30000 / (RAND_MAX+1.0)+20000 )))
	{ return FALSE;}  //by cje 020614

	

	// NAT 분석 1단계로 서버로 Data에 FireTalk IP와 Port를 넣어 서버로 UDP 패킷을 보낸다
	UDPMSG msg;
	msg.type = _ANALYZE_NAT_A;
	strcpy(msg.Data.natAmsg.strRouteIP, m_strFireTalkIP);
	msg.Data.natAmsg.uRoutePort = m_nFireTalkPort;	

	m_pAnalyzerSock->SetPeerInfo(m_strServerIP, m_nServerPort);
	SetStatus(_N_A_SEND);
	if(!m_pAnalyzerSock->SendData((char*)&msg, sizeof(UDPMSG)))
	{
		return FALSE;
	}	
	SetResendTimer();
	m_nSendtoServerCount++;

	
	return TRUE;
}

#define MAX_UDP_RESEND_COUNT	2

void CALLBACK TimerProcResend(UINT uiID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CNATAnalyzer* pAnalyzer = NULL;
	pAnalyzer = (CNATAnalyzer*)dwUser;

	if(pAnalyzer && pAnalyzer->m_bMMTimerSetFlag)
	{
		switch(pAnalyzer->GetStatus())
		{
		case _N_A_SEND:
			// NAT 타입(cone or other)을 확익하기위해 NAT_A UDP 패킷을 서버로 보낸 상태
			{
				if(pAnalyzer->m_nSendtoServerCount < MAX_UDP_RESEND_COUNT)
				{
					UDPMSG msg;
					msg.type = _ANALYZE_NAT_A;
					strcpy(msg.Data.natAmsg.strRouteIP, pAnalyzer->m_strFireTalkIP);
					msg.Data.natAmsg.uRoutePort = pAnalyzer->m_nFireTalkPort;
					pAnalyzer->m_pAnalyzerSock->SendData((char*)&msg, sizeof(UDPMSG));
					pAnalyzer->m_nSendtoServerCount++;
				}
				else
				{
					pAnalyzer->KillResendTimer();
					
					UDPMSG msg;
					msg.type = _ANALYZE_NAT_B;
					
					pAnalyzer->SetStatus(_N_A_NOACK_B_SEND);
//					pAnalyzer->m_pAnalyzerSock->SetPeerInfo(pAnalyzer->m_strFireTalkIP, pAnalyzer->m_nFireTalkPort);
					pAnalyzer->m_pAnalyzerSock->SendData((char*)&msg, sizeof(UDPMSG));					
					pAnalyzer->SetResendTimer();
//					pAnalyzer->m_nSendtoFireTalkCount++;
					pAnalyzer->m_nSendBtoServerCount++;
						

				}
			}
			break;
		case _N_A_ACK_B_SEND:
			// NAT_A패킷에 대한 응답을 받고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
		case _N_A_NOACK_B_SEND:
			// NAT_A패킷에 대한 응답을 받지 못하고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
			{
				if(pAnalyzer->m_nSendBtoServerCount < MAX_UDP_RESEND_COUNT)
				{
					UDPMSG msg;
					msg.type = _ANALYZE_NAT_B;
										
//					pAnalyzer->m_pAnalyzerSock->SetPeerInfo(pAnalyzer->m_strFireTalkIP, pAnalyzer->m_nFireTalkPort);
					pAnalyzer->m_pAnalyzerSock->SendData((char*)&msg, sizeof(UDPMSG));
//					pAnalyzer->m_nSendtoFireTalkCount++;
					pAnalyzer->m_nSendBtoServerCount++;
				}
				else
				{
					pAnalyzer->KillResendTimer();
					pAnalyzer->NetworkTypeResult(_NOTUSE_UDP);
				}
			}
			break;
		}		
	}
	else
	{
		timeKillEvent(uiID);
	}
}

void CNATAnalyzer::SetResendTimer()
{
	if(m_bMMTimerSetFlag == FALSE && m_nMMTimerID == 0)
	{					
		m_nMMTimerID = timeSetEvent(1000,1, &TimerProcResend, (DWORD)this,TIME_PERIODIC);
		m_bMMTimerSetFlag = 1;
		m_nSendtoServerCount = 0;
//		m_nSendtoFireTalkCount = 0;
		m_nSendBtoServerCount = 0;
	}
}
void CNATAnalyzer::KillResendTimer()
{
	if(m_bMMTimerSetFlag && m_nMMTimerID)
	{
		timeKillEvent(m_nMMTimerID);
	}
	m_bMMTimerSetFlag = FALSE;
	m_nMMTimerID = 0;
	m_nSendtoServerCount = 0;
//	m_nSendtoFireTalkCount = 0;	
	m_nSendBtoServerCount = 0;
}

BOOL CNATAnalyzer::NetworkTypeResult(NetworkType type)
{
	if(type == _NULL_TYPE)		// error
	{
		SendMessage(m_hMsgWnd, WM_NATANALYZER_RESULT, 0, (LPARAM)this);
		return FALSE;
	}

	if(GetStatus() != _N_RESULT)
	{
		SetStatus(_N_RESULT);
		m_eNetworkType = type;

		SendMessage(m_hMsgWnd, WM_NATANALYZER_RESULT, 1, (LPARAM)this);

		return TRUE;
	}
	
	return FALSE;
}