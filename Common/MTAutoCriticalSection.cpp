// MTAutoCriticalSection.cpp: implementation of the CMTAutoCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MTAutoCriticalSection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMTAutoCriticalSection::CMTAutoCriticalSection()
{
}

CMTAutoCriticalSection::CMTAutoCriticalSection(CRITICAL_SECTION *i_pCritSec)
: CMTCriticalSection(i_pCritSec)
{
	this->Enter();
}

CMTAutoCriticalSection::~CMTAutoCriticalSection()
{
	if(m_pCriticalSection)
	{
		this->Leave();
	}
}


void CMTAutoCriticalSection::UnlockMTAutoCriticalSection(void)
{
	if(m_pCriticalSection)
	{
		this->Leave();
	}

	m_pCriticalSection = NULL;
}
