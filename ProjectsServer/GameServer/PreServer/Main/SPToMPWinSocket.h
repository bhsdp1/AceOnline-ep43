// SPToMPWinSocket.h: interface for the CSPToMPWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPTOMPWINSOCKET_H__D89CA12F_A5C7_4AB8_8C65_5879AA2B7E42__INCLUDED_)
#define AFX_SPTOMPWINSOCKET_H__D89CA12F_A5C7_4AB8_8C65_5879AA2B7E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"
#include "TickManager.h"		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리

// start 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
// event types
#define SPTOMP_TICK_EVENT_SESSION_TIMEOUT		0

#define SPTOMP_DELAY_TIME_SEC_TIMEOUT			2 * 60		// (1분) (단위 : 초)
// end 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리

///////////////////////////////////////////////////////////////////////////////
/// \class		class CSPToMPWinSocket : public CWinSocket
///
/// \brief		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
/// \author		cmkwon
/// \version	
/// \date		2008-02-22 ~ 2008-02-22
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CPreIOCP;

class CSPToMPWinSocket : public CWinSocket, public CTickManager		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
{
public:
	CSPToMPWinSocket(HWND i_hWnd, DWORD i_nTickInterval = SPTOMP_TICK_INTERVAL);		// 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
	virtual ~CSPToMPWinSocket();

	void SetPreIOCP(CPreIOCP *i_pPreIOCP);

	ProcessResult Process_PP_CONNECT_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_PP_CONNECT_DO_CLOSE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_PAUTH_CONNECT_OK(const char* pPacket, int nLength, int &nBytesUsed);			// 2011-01-26 by hskim, 인증 서버 구현
	ProcessResult Process_PAUTH_CONNECT_FAIL(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-01-26 by hskim, 인증 서버 구현
	ProcessResult Process_PAUTH_CONNECT_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-06-22 by hskim, 사설 서버 방지

	// start 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리
	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);
	// end 2011-07-21 by hskim, 인증 서버 구현 - 접속후 Timeout 처리

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CPreIOCP		*m_pPreIOCP3;
};

#endif // !defined(AFX_SPTOMPWINSOCKET_H__D89CA12F_A5C7_4AB8_8C65_5879AA2B7E42__INCLUDED_)
