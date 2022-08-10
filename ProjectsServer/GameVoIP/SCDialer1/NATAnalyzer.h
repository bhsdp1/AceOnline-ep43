// NATAnalyzer.h: interface for the CNATAnalyzer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NATANALYZER_H__FBD61870_19C6_49E3_8715_80B672BEF120__INCLUDED_)
#define AFX_NATANALYZER_H__FBD61870_19C6_49E3_8715_80B672BEF120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Protocol_FireTalk_II.h"
#include "NATAnalyzerSock.h"

typedef enum 
{
	_N_NULL					= 0,	// 처음 생성시 상태
	_N_READY				= 1,	// 서버와 FireTalk II의 IP와 Port가 설정되고 메세지 윈도우 핸들도 이상이 없는 상태
	_N_A_SEND				= 2,	// NAT 타입(cone or other)을 확익하기위해 NAT_A UDP 패킷을 서버로 보낸 상태
	_N_A_ACK_B_SEND			= 3,	// NAT_A패킷에 대한 응답을 받고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
//	_N_A_ACK_B_ACKfromS		= 4,	// NAT_A패킷에 대한 응답을 받고 NAT_B UDP 패킷에 대한 응답을 서버로 부터 받은 상태
//	_N_A_ACK_B_ACKfromF		= 5,	// NAT_A패킷에 대한 응답을 받고 NAT_B UDP 패킷에 대한 응답을 FireTalk II로 부터 받은 상태

	_N_A_NOACK_B_SEND		= 10,	// NAT_A패킷에 대한 응답을 받지 못하고 NAT_B UDP 패킷을 서버와 FireTalk II로 보낸 상태
//	_N_A_NOACK_B_ACKfromS	= 11,	// NAT_A패킷에 대한 응답을 받지 못하고 NAT_B UDP 패킷에 대한 응답을 서버로 부터 받은 상태
//	_N_A_NOACK_B_ACKfromF	= 12,	// NAT_A패킷에 대한 응답을 받지 못하고 NAT_B UDP 패킷에 대한 응답을 FireTalk II로 부터 받은 상태

	_N_RESULT				= 20	// NAT 타입 분석을 완료 하였다
	
} NATAnalyzerStatus;

class CNATAnalyzerSock;
class CNATAnalyzer  
{
public:
	CNATAnalyzer();
	virtual ~CNATAnalyzer();

public:
	// 멤버 변수
	CNATAnalyzerSock*	m_pAnalyzerSock;		// NAT 타입을 분석하기 위해 사용되는 UDP 소켓
	HWND				m_hMsgWnd;				// NAT 타입이 확인되면 메세지를 보낼 윈도우 핸들
	char				m_strServerIP[16];		// NAT 타입을 분석하기 위해 사용되는 서버 A IP
	UINT				m_nServerPort;			// NAT 타입을 분석하기 위해 사용되는 서버 A Port
	char				m_strFireTalkIP[16];	// NAT 타입을 분석하기 위해 사용되는 서버 A IP
	UINT				m_nFireTalkPort;		// NAT 타입을 분석하기 위해 사용되는 서버 A Port
	BOOL				m_bReady;				// NAT 타입을 분석할 준비가 되었는지의 플래그(서버의 IP와 Port 세팅, 메세지 보낼 윈도우 핸들)		
	
	BOOL				m_bServerReady;			// by cje 020614
	BOOL				m_bFireTalkReady;
	
	int					m_nSendtoServerCount;	// 서버로 전송된 UDP 패킷에 대한 응답이 없을 경우 재전송하는 count
//	int					m_nSendtoFireTalkCount;	// FireTalk II로 전송된 UDP 패킷에 대한 응답이 없을 경우 재전송하는 count
	int					m_nSendBtoServerCount;	// Server로 전송된 UDP 패킷에 대한 응답이 없을 경우 재전송하는message B count
	NATErrorInfos			m_eErrorInfo;			// 마지막 에러 정보
	BOOL				m_bMMTimerSetFlag;		
	UINT				m_nMMTimerID;
	NetworkType			m_eNetworkType;			// 확인된 Network Type
	UINT				m_nPortNum;				// 

protected:
	NATAnalyzerStatus	m_Status;				// 분석 단계

public:
	// 멤버 함수
	NATAnalyzerStatus GetStatus();
	void SetStatus(NATAnalyzerStatus status);
	NATErrorInfos GetErrorInfos();
	void SetErrorInfos(NATErrorInfos err);
	void GetErrorInfosString(char* str, NATErrorInfos err = ERR_NOTHING);
	NetworkType GetNetworkType(){ return m_eNetworkType;}
	BOOL SetServersInfo(HWND hWnd, char* serverIP, UINT serverPort, char* fireTalkIP, UINT fireTalkPort);

	BOOL SetServerInfo(HWND hWnd, char* serverIP, UINT serverPort); //by cje 020614
	BOOL SetFireTalkInfo(char* fireTalkIP, UINT fireTalkPort);
	
	BOOL ExcuteAnalysis();
	void SetResendTimer();
	void KillResendTimer();
	BOOL NetworkTypeResult(NetworkType type);
};

#endif // !defined(AFX_NATANALYZER_H__FBD61870_19C6_49E3_8715_80B672BEF120__INCLUDED_)
