// MTCriticalSection.cpp: implementation of the CMTCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTCriticalSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMTCriticalSection::CMTCriticalSection()
{
	this->ResetMTCriticalSection();
	::InitializeCriticalSection(&m_criticalSection);
}

CMTCriticalSection::CMTCriticalSection(CRITICAL_SECTION *i_pCritSec)
{
	this->ResetMTCriticalSection();
	m_pCriticalSection = i_pCritSec;
}

CMTCriticalSection::~CMTCriticalSection()
{
	if(NULL == m_pCriticalSection)
	{
		::DeleteCriticalSection(&m_criticalSection);
	}
	
	this->ResetMTCriticalSection();
}

void CMTCriticalSection::ResetMTCriticalSection(void)
{
	m_pCriticalSection		= NULL;
	memset(&m_criticalSection, 0x00, sizeof(CRITICAL_SECTION));
}

void CMTCriticalSection::Enter(void)
{
	if(NULL == m_pCriticalSection)
	{
		::EnterCriticalSection(&m_criticalSection);
	}
	else
	{
		::EnterCriticalSection(m_pCriticalSection);
	}
}

void CMTCriticalSection::Leave(void)
{
	if(NULL == m_pCriticalSection)
	{
		::LeaveCriticalSection(&m_criticalSection);
	}
	else
	{
		::LeaveCriticalSection(m_pCriticalSection);
	}
}


#if (_WIN32_WINNT >= 0x0400)
/*
BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

Return Values
	- If the critical section is successfully entered or the current
	thread already owns the critical section, the return value is nonzero.
	- If another thread already owns the critical section,
	the return value is zero.
 */
BOOL CMTCriticalSection::Try(void)
{
	if(NULL == m_pCriticalSection)
	{
		return ::TryEnterCriticalSection(&m_criticalSection);
	}
	
	return ::TryEnterCriticalSection(m_pCriticalSection);		
}
#endif// (_WIN32_WINNT >= 0x0400)_endif