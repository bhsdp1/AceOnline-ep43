// NPCTickManager.h: interface for the CNPCTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCTICKMANAGER_H__E9D91658_B931_40F7_BBFD_DE11FCE6A94A__INCLUDED_)
#define AFX_NPCTICKMANAGER_H__E9D91658_B931_40F7_BBFD_DE11FCE6A94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"
#include "NPCIOCPSocket.h"

#define NPC_DEFAULT_TICK_INTERVAL			500		// 0.5 second

class CNPCTickManager : public CTickManager  
{
public:
	CNPCTickManager(CNPCIOCP *i_pNPCIOCP, DWORD i_nTickInterval = NPC_DEFAULT_TICK_INTERVAL);
	virtual ~CNPCTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

public:
	CNPCIOCP		*m_pNPCIOCP2;
};

#endif // !defined(AFX_NPCTICKMANAGER_H__E9D91658_B931_40F7_BBFD_DE11FCE6A94A__INCLUDED_)
