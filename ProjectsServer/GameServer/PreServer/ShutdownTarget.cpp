// ShutdownTarget.cpp: implementation of the CShutdownTarget class.
// 2012-07-11 by hskim, 선택적 셧다운
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShutdownTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShutdownTarget::CShutdownTarget()
{
	Clear();
}

CShutdownTarget::CShutdownTarget(char *i_pAccountName, char *i_pServiceBlock)
{
	SetTarget(i_pAccountName, i_pServiceBlock);
}

CShutdownTarget::~CShutdownTarget()
{
}

void CShutdownTarget::Clear()
{
	memset(m_szAccountName, 0, sizeof(m_szAccountName));
	memset(m_bServiceBlock, 0, sizeof(m_bServiceBlock));
}

void CShutdownTarget::SetTarget(char *i_pAccountName, char *i_pServiceBlock)
{
	Clear();
	
	STRNCPY_MEMSET(m_szAccountName, i_pAccountName, SIZE_MAX_ACCOUNT_NAME);

	for(int i=0; i<SIZE_MAX_SHUTDOWN_SERVICE_BLOCK; i++)
	{
		if( '1' == i_pServiceBlock[i] )
		{
			m_bServiceBlock[i] = TRUE;
		}
	}
}

void CShutdownTarget::SetTarget(CShutdownTarget *pTarget)
{
	STRNCPY_MEMSET(m_szAccountName, pTarget->m_szAccountName, SIZE_MAX_ACCOUNT_NAME);
	memcpy(m_bServiceBlock, pTarget->m_bServiceBlock, sizeof(pTarget->m_bServiceBlock));
}

BOOL CShutdownTarget::IsShutdown(int iHour)
{
	if( iHour >= SIZE_MAX_SHUTDOWN_SERVICE_BLOCK )
	{
		return TRUE;
	}

	return (TRUE != m_bServiceBlock[iHour]);
}