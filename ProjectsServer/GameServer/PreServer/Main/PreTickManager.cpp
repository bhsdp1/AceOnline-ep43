#include "stdafx.h"
#include "PreTickManager.h"
#include "PreIOCP.h"

CPreTickManager::CPreTickManager(CPreIOCP *i_pPreIOCP, DWORD i_nTickInterval /* = 1000 */)
: CTickManager(i_nTickInterval), m_pPreIOCP1(i_pPreIOCP)
{
}

CPreTickManager::~CPreTickManager()
{
}

void CPreTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CPreTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CPreTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CPreTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
//	DBGOUT("DoMinutelyWork: %s\r\n", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
	m_pPreIOCP1->OnDoMinutelyWorkIOCP(pDateTime);
}

void CPreTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
//	DBGOUT("DoSecondlyWork: %s\r\n", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
}
