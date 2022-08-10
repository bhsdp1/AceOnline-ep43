#ifndef _ATUM_PRE_TICK_MANAGER_H_
#define _ATUM_PRE_TICK_MANAGER_H_

#include "TickManager.h"

class CPreIOCP;

class CPreTickManager : public CTickManager
{
public:
	CPreTickManager(CPreIOCP *i_pPreIOCP, DWORD i_nTickInterval = 1000);
	virtual ~CPreTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

public:
	CPreIOCP *m_pPreIOCP1;
};

#endif // _ATUM_PRE_TICK_MANAGER_H_
