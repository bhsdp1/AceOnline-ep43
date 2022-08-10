// ThreadCheck.h: interface for the CThreadCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADCHECK_H__2B0EE6F6_46EA_4BE1_8465_36AA5FDE16B2__INCLUDED_)
#define AFX_THREADCHECK_H__2B0EE6F6_46EA_4BE1_8465_36AA5FDE16B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mt_stl.h"

#define SIZE_MAX_THREAD_COMMENT					64

typedef enum
{
	THREAD_CHECK_TYPE_UNKNOWN		= 0,
	THREAD_CHECK_TYPE_IOCP_WORKER	= 1,
	THREAD_CHECK_TYPE_IOCP_WRITER	= 2,
	THREAD_CHECK_TYPE_IOCP_LISTENER	= 3,
	THREAD_CHECK_TYPE_DB_WORKER		= 4
} EN_THREAD_CHECK_t;

typedef struct
{
	DWORD				dwThreadId;
	EN_THREAD_CHECK_t	enThreadCheckType;
	BOOL				bThreadUseFlag;
	DWORD				dwLastUseStartTick;
	DWORD				dwSocketIndex;
	DWORD				dwMessageType;
	DWORD				dwCharacterUID;
	int					nQueryArraySize;
	BOOL				bPrintOut;
	int					nParam1;				// 2007-02-21 by cmkwon
	int					nParam2;				// 2007-02-21 by cmkwon

	static char *GetThreadCheckComment(EN_THREAD_CHECK_t i_enThreadType);
} SThreadInfo;

class CThreadCheck
{
public:
	CThreadCheck();
	virtual ~CThreadCheck();

	BOOL GetSettingCompletionFlag(void){			return m_bSettingCompletionFlag;}
	SThreadInfo *GetThreadInfo(DWORD threadId);
	BOOL AddThreadInfo(SThreadInfo threadInfo);
	SThreadInfo * CheckThreadInfo(DWORD i_dwThreadIdToExclude);
	void SetCheckThreadCounts(int nThreadCounts){	m_nSettingThreadCounts = nThreadCounts;};

protected:
	int							m_nSettingThreadCounts;
	BOOL						m_bSettingCompletionFlag;
	DWORD						m_dwCheckTick;
	mt_vector<SThreadInfo>		m_ThreadInfoDB;
};

#endif // !defined(AFX_THREADCHECK_H__2B0EE6F6_46EA_4BE1_8465_36AA5FDE16B2__INCLUDED_)
