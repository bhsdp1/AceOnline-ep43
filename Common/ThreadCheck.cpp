// ThreadCheck.cpp: implementation of the CThreadCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadCheck.h"
#include "mmsystem.h"

char *SThreadInfo::GetThreadCheckComment(EN_THREAD_CHECK_t i_enThreadType)
{
	switch(i_enThreadType)
	{
	case THREAD_CHECK_TYPE_IOCP_WORKER:
		return "THREAD_CHECK_TYPE_IOCP_WORKER";
	case THREAD_CHECK_TYPE_IOCP_WRITER:
		return "THREAD_CHECK_TYPE_IOCP_WRITER";
	case THREAD_CHECK_TYPE_IOCP_LISTENER:
		return "THREAD_CHECK_TYPE_IOCP_LISTENER";
	case THREAD_CHECK_TYPE_DB_WORKER:
		return "THREAD_CHECK_TYPE_DB_WORKER";
	}
	
	return "THREAD_CHECK_TYPE_UNKNOWN";
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadCheck::CThreadCheck()
{
	m_nSettingThreadCounts		= 0;
	m_bSettingCompletionFlag	= FALSE;
	m_dwCheckTick				= 30000;	// 30√ 
	m_ThreadInfoDB.reserve(10);
}

CThreadCheck::~CThreadCheck()
{
}

SThreadInfo *CThreadCheck::GetThreadInfo(DWORD threadId)
{
	if(FALSE == m_bSettingCompletionFlag){		return FALSE;}

	mt_vector<SThreadInfo>::iterator itr = m_ThreadInfoDB.begin();
	while (itr != m_ThreadInfoDB.end())
	{
		if(itr->dwThreadId == threadId)
		{
			return &*itr;
		}
		itr++;
	}
	return NULL;
}

BOOL CThreadCheck::AddThreadInfo(SThreadInfo threadInfo)
{	
	m_ThreadInfoDB.lock();
	if(m_bSettingCompletionFlag
		|| GetThreadInfo(threadInfo.dwThreadId))
	{
		m_ThreadInfoDB.unlock();
		return FALSE;
	}
	m_ThreadInfoDB.push_back(threadInfo);
	if(m_nSettingThreadCounts == m_ThreadInfoDB.size())
	{
		m_bSettingCompletionFlag = TRUE;
	}
	m_ThreadInfoDB.unlock();
	return TRUE;
}

SThreadInfo * CThreadCheck::CheckThreadInfo(DWORD i_dwThreadIdToExclude)
{
	if(FALSE == m_bSettingCompletionFlag){	return NULL;}
	
	BOOL	bCheckValid = (i_dwThreadIdToExclude==0)?TRUE:FALSE;
	DWORD	dwCurTick = timeGetTime();
	mt_vector<SThreadInfo>::iterator itr = m_ThreadInfoDB.begin();
	while (itr != m_ThreadInfoDB.end())
	{
		if(bCheckValid)
		{
			if(itr->dwThreadId != i_dwThreadIdToExclude
				&& itr->bThreadUseFlag
				&& dwCurTick - itr->dwLastUseStartTick > m_dwCheckTick)
			{
				return &*itr;
			}
		}
		else if(itr->dwThreadId == i_dwThreadIdToExclude)
		{
			bCheckValid = TRUE;
		}
		itr++;
	}
	return NULL;
}