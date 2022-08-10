// SelectiveShutdown.cpp: implementation of the CSelectiveShutdown class.
// 2012-07-11 by hskim, 선택적 셧다운
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectiveShutdown.h"
#include "ShutdownTarget.h"
#include "PreIOCP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_30MIN_AGO		30
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_10MIN_AGO		50
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_5MIN_AGO		55
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_3MIN_AGO		57
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_1MIN_AGO		59
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_RUN					0

CSelectiveShutdown::CSelectiveShutdown(DWORD i_nTickInterval /*= SELECTIVE_SHUTDOWN_TICK_INTERVAL*/)
: CTickManager(i_nTickInterval)
{
	m_pPreIOCP = NULL;
	m_mtVecTargetList.clear();
}

CSelectiveShutdown::~CSelectiveShutdown()
{
}

void CSelectiveShutdown::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
	return ;
}

void CSelectiveShutdown::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSelectiveShutdown::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{	
}

void CSelectiveShutdown::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSelectiveShutdown::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	if( STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_30MIN_AGO == pDateTime->Minute || 
		STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_10MIN_AGO == pDateTime->Minute || 
		STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_5MIN_AGO == pDateTime->Minute ||
		STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_3MIN_AGO == pDateTime->Minute ||
		STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_1MIN_AGO == pDateTime->Minute )
	{
		int iNoticeTime = 0;
		mt_auto_lock mtLock(&m_mtVecTargetList);
		mtvectShutdownTarget::iterator itr = m_mtVecTargetList.begin();

		while( itr != m_mtVecTargetList.end() )
		{
			if( TRUE == CheckShutdownStatus(itr->m_bServiceBlock, 1) )
			{
				switch( pDateTime->Minute )
				{
				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_30MIN_AGO :
					iNoticeTime = 30;
					break;

				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_10MIN_AGO :
					iNoticeTime = 10;
					break;

				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_5MIN_AGO :
					iNoticeTime = 5;
					break;

				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_3MIN_AGO :
					iNoticeTime = 3;
					break;

				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_NOTICE_1MIN_AGO :	
					iNoticeTime = 1;
					break;

				default :
					break;
				}

				if( NULL != m_pPreIOCP && 0 != iNoticeTime )
				{
					m_pPreIOCP->SelectiveShutdownNotice(itr->m_szAccountName, iNoticeTime);
					
				}
			}

			itr++;
		}
	}

	if( STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_RUN == pDateTime->Minute )
	{
		mt_auto_lock mtLock(&m_mtVecTargetList);
		mtvectShutdownTarget::iterator itr = m_mtVecTargetList.begin();

		mtvectShutdownTarget m_tempVecTargetList;
		m_tempVecTargetList.clear();

		while( itr != m_mtVecTargetList.end() )
		{
			m_tempVecTargetList.push_back(*itr);
			itr++;
		}		

		mtvectShutdownTarget::iterator itr2 = m_tempVecTargetList.begin();
		while( itr2 != m_tempVecTargetList.end() )
		{
			if( TRUE == CheckShutdownStatus(itr2->m_bServiceBlock, 0) )
			{
				switch( pDateTime->Minute )
				{
				case STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_RUN :

					// 셧다운 적용

					if( NULL != m_pPreIOCP )
					{
						m_pPreIOCP->SelectiveShutdownProgress(itr2->m_szAccountName);
					}
					
					break;

				default :
					break;
				}
			}

			itr2++;
		}		
	}
}

void CSelectiveShutdown::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CSelectiveShutdown::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{
}

BOOL CSelectiveShutdown::Init()
{
	return CTickManager::InitTickManager();
}

BOOL CSelectiveShutdown::SetPreIOCP(CPreIOCP *pPreIOCP)
{
	m_pPreIOCP = pPreIOCP;
	
	return TRUE;
}

CShutdownTarget *CSelectiveShutdown::FindTarget(CShutdownTarget Target)
{
	mt_auto_lock mtLock(&m_mtVecTargetList);

	mtvectShutdownTarget::iterator itr = m_mtVecTargetList.begin();
	while( itr != m_mtVecTargetList.end() )
	{
		if( 0 == strncmp(itr->m_szAccountName, Target.m_szAccountName, SIZE_MAX_ACCOUNT_NAME) )
		{
			return &*itr;
		}

		itr++;
	}

	return NULL;
}

BOOL CSelectiveShutdown::AddTarget(CShutdownTarget Target)
{
	mt_auto_lock mtLock(&m_mtVecTargetList);

	CShutdownTarget *pFindTarget = FindTarget(Target);
	if( NULL != pFindTarget )
	{
		pFindTarget->SetTarget(&Target);
	}
	else
	{
		m_mtVecTargetList.push_back(Target);
	}

	return TRUE;
}

BOOL CSelectiveShutdown::DelTarget(CShutdownTarget Target)
{
	mt_auto_lock mtLock(&m_mtVecTargetList);

	mtvectShutdownTarget::iterator itr = m_mtVecTargetList.begin();
	while( itr != m_mtVecTargetList.end() )
	{
		if( 0 == strncmp(itr->m_szAccountName, Target.m_szAccountName, SIZE_MAX_ACCOUNT_NAME) )
		{
			itr = m_mtVecTargetList.erase(itr);
			
			continue;
		}

		itr++;
	}

	return TRUE;
}

BOOL CSelectiveShutdown::CheckShutdownStatus(char ServiceBlock[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO])
{
	int nHour = 0;
	ATUM_DATE_TIME tmpDateTime;
	tmpDateTime.SetCurrentDateTime();

	if( nHour < 0 )
	{
		return FALSE;
	}

	nHour = tmpDateTime.Hour % 24;

	if( '1' == ServiceBlock[nHour] )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CSelectiveShutdown::CheckShutdownStatus(BOOL m_bServiceBlock[SIZE_MAX_SHUTDOWN_SERVICE_BLOCK], int nAddHour /*=0*/)
{
	int nNextHour = 0;
	ATUM_DATE_TIME tmpDateTime;
	tmpDateTime.SetCurrentDateTime();

	if( nNextHour < 0 )
	{
		return FALSE;
	}

	nNextHour = (tmpDateTime.Hour + nAddHour) % 24;

	if( TRUE == m_bServiceBlock[nNextHour] )
	{
		return FALSE;
	}

	return TRUE;
}