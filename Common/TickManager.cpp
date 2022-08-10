#include "stdafx.h"
#include "TickManager.h"
#include "IOCP.h"
#include "AtumTime.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTickManager::CTickManager(DWORD i_nTickInterval)
{
	m_nPrevDateTime.Reset();
	m_nTickInterval = i_nTickInterval;
}

CTickManager::~CTickManager()
{
}

BOOL CTickManager::InitTickManager()
{
	return InitThread();
}

void CTickManager::CleanTickManager()
{
	CleanThread();
}

DWORD CTickManager::ThreadWorker()
{
	while (!GetShutDownFlag())
	{
		DWORD ret = SleepAndWaitForEvent(m_nTickInterval);
		if (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT)
		{
			// error
			break;
		}

		// shutdown
		if (GetShutDownFlag())
		{
			break;
		}

		ATUM_DATE_TIME currentDateTime;
		currentDateTime.SetCurrentDateTime();

		ProcessTickEvent(&currentDateTime);

		DoEveryTickWork(&currentDateTime);

		if(currentDateTime.Month != m_nPrevDateTime.Month)
		{
			DoMonthlyWork(&currentDateTime);
		}

		if (currentDateTime.Day != m_nPrevDateTime.Day)
		{
			DoDailyWork(&currentDateTime);
		}

		if (currentDateTime.Hour != m_nPrevDateTime.Hour)
		{
			DoHourlyWork(&currentDateTime);
		}

		if (currentDateTime.Minute != m_nPrevDateTime.Minute)
		{
			DoMinutelyWork(&currentDateTime);
		}

		if (currentDateTime.Second != m_nPrevDateTime.Second)
		{
			DoSecondlyWork(&currentDateTime);
		}

		m_nPrevDateTime = currentDateTime;
	} // end while

	return 5;
}

void CTickManager::ProcessTickEvent(ATUM_DATE_TIME *i_pDateTime)
{
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>::iterator itrMain;
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>::iterator itrEnd;

	vector<TICK_EVENT> vecTickEvent;		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	m_multimapTickEvent.lock();

	itrEnd = m_multimapTickEvent.upper_bound(*i_pDateTime);
	if (itrEnd == m_multimapTickEvent.begin())
	{
		m_multimapTickEvent.unlock();
		return;
	}

	for (itrMain = m_multimapTickEvent.begin(); itrMain != itrEnd; itrMain++)
	{
		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
		//
		// 기존
		//
		// DoTickEvent(i_pDateTime, &itrMain->second);

		/////////
		// 수정

		vecTickEvent.push_back(itrMain->second);

		// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
		///////////////////////////////////////////////////////////////////////////////////////
	}

	m_multimapTickEvent.erase(m_multimapTickEvent.begin(), itrEnd);
	m_multimapTickEvent.unlock();

	// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	for(int i=0; i<vecTickEvent.size(); i++)
	{
		DoTickEvent(i_pDateTime, &vecTickEvent[i]);
	}
	// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
}

void CTickManager::InsertTickEvent(INT i_TickEventType, ATUM_DATE_TIME *i_pStartTime, INT i_nTickEventParam1 /* = 0 */, INT i_nTickEventParam2 /* = 0 */, void *i_vpTickEventParam1 /* = NULL */, void *i_vpTickEventParam2 /* = NULL */)
{
	TICK_EVENT tmpEvent;
	tmpEvent.TickEventType		= i_TickEventType;
	tmpEvent.nTickEventParam1	= i_nTickEventParam1;
	tmpEvent.nTickEventParam2	= i_nTickEventParam2;
	tmpEvent.vpTickEventParam1	= i_vpTickEventParam1;
	tmpEvent.vpTickEventParam2	= i_vpTickEventParam2;

	m_multimapTickEvent.lock();

	m_multimapTickEvent.insertNoLock(*i_pStartTime, tmpEvent);

	m_multimapTickEvent.unlock();

//#ifdef _DEBUG
//	DBGOUT("타이머 시작 %s\r\n", i_pStartTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
//#endif
}

void CTickManager::InsertTickEvent(INT i_TickEventType, INT i_nTimeIntervalInSeconds, INT i_nTickEventParam1 /* = 0 */, INT i_nTickEventParam2 /* = 0 */, void *i_vpTickEventParam1 /* = NULL */, void *i_vpTickEventParam2 /* = NULL */)
{
	ATUM_DATE_TIME tmpDateTime;
	tmpDateTime.SetCurrentDateTime();
	tmpDateTime.AddDateTime(0, 0, 0, 0, 0, i_nTimeIntervalInSeconds);

	InsertTickEvent(i_TickEventType, &tmpDateTime, i_nTickEventParam1, i_nTickEventParam2, i_vpTickEventParam1, i_vpTickEventParam2);
}
