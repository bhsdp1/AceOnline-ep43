// AccountBlockManager.h: interface for the CAccountBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOUNTBLOCKMANAGER_H__C7D37397_CC0A_4A89_982B_8492E4D39B0E__INCLUDED_)
#define AFX_ACCOUNTBLOCKMANAGER_H__C7D37397_CC0A_4A89_982B_8492E4D39B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCStatement.h"

typedef mt_map<string, SBLOCKED_ACCOUNT_INFO>			mtmapBLOCKED_ACCOUNT_INFO;

class CAtumPreDBManager;
class CPreIOCPSocket;
class CAccountBlockManager  
{
public:
	CAccountBlockManager(CAtumPreDBManager *i_pDBManager);
	~CAccountBlockManager();

	void ResetAccountBlockManager(void);

	mtmapBLOCKED_ACCOUNT_INFO * GetmapBLOCKED_ACCOUNT_INFOPtr(void);

	BOOL AddBlockedAccountList(SBLOCKED_ACCOUNT_INFO *i_pBlockedInfo);
	BOOL DeleteBlockedAccountList(const char *i_szAccountName);			// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - DeleteBlockedAccountList() 수정
	BOOL IsCheckBlockedAccountInfoByAccountName(SBLOCKED_ACCOUNT_INFO *o_pBlockedInfo, char *i_szAccName, CPreIOCPSocket *i_pNotifySock=NULL);
protected:

	mtmapBLOCKED_ACCOUNT_INFO	m_mtmapBlockedAccountList;
	CAtumPreDBManager			*m_pPreDBManager;
};

#endif // !defined(AFX_ACCOUNTBLOCKMANAGER_H__C7D37397_CC0A_4A89_982B_8492E4D39B0E__INCLUDED_)
