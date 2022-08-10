#include "stdafx.h"
#include "AuthenticationTickManager.h"
#include "AuthenticationIOCP.h"

CAuthenticationTickManager::CAuthenticationTickManager(CAuthenticationIOCP *i_pAuthenticationIOCP, DWORD i_nTickInterval /* = 1000 */)
: CTickManager(i_nTickInterval), m_pAuthenticationIOCP1(i_pAuthenticationIOCP)
{
}

CAuthenticationTickManager::~CAuthenticationTickManager()
{
}

void CAuthenticationTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CAuthenticationTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CAuthenticationTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CAuthenticationTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CAuthenticationTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
}
