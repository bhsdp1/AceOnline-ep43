// SelectiveShutdown.h: interface for the CSelectiveShutdown class.
// 2012-07-11 by hskim, 선택적 셧다운
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTIVESHUTDOWN_H__D27E22BF_030E_485B_8BD1_0C13E4062891__INCLUDED_)
#define AFX_SELECTIVESHUTDOWN_H__D27E22BF_030E_485B_8BD1_0C13E4062891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

class CPreIOCP;
class CShutdownTarget;

typedef mt_vector<CShutdownTarget>				mtvectShutdownTarget;		// 2008-12-22 by dhjin, 전쟁 보상 추가안

class CSelectiveShutdown : public CTickManager
{
public:
	CSelectiveShutdown(DWORD i_nTickInterval = SELECTIVE_SHUTDOWN_TICK_INTERVAL);
	virtual ~CSelectiveShutdown();

public:
	BOOL Init();

	BOOL SetPreIOCP(CPreIOCP *pPreIOCP);

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);

public:
	CShutdownTarget *FindTarget(CShutdownTarget Target);
	BOOL AddTarget(CShutdownTarget Target);
	BOOL DelTarget(CShutdownTarget Target);

	BOOL CheckShutdownStatus(char ServiceBlock[SIZE_MAX_SHUTDOWN_SERVICE_BLOCK]);
	BOOL CheckShutdownStatus(BOOL m_bServiceBlock[SIZE_MAX_SHUTDOWN_SERVICE_BLOCK], int nAddHour=0);

private:
	CPreIOCP *m_pPreIOCP;
	mtvectShutdownTarget m_mtVecTargetList;
};

#endif // !defined(AFX_SELECTIVESHUTDOWN_H__D27E22BF_030E_485B_8BD1_0C13E4062891__INCLUDED_)
