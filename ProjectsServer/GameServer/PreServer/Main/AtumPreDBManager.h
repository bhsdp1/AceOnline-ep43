#ifndef __ATUM_PRE_DB_MANAGER_H__
#define __ATUM_PRE_DB_MANAGER_H__

#include "AtumDBManager.h"


struct MSG_DB_TOTAL_USER
{
	char		szServerGroup[SIZE_MAX_SERVER_NAME];
	int			nMGameServerID;
	int			nUserCounts;
#ifdef S_ARARIO_HSSON
	int			nUserCountsOtherPublisherConncect;		// 2010-11 by dhjin, 아라리오 채널링 로그인.
#endif
};

struct QPARAM_GLOG_INSERT_ACCOUNT
{// 2010-06-01 by shcho, GLogDB 관련 -
	char		szAccountName[SIZE_MAX_ACCOUNT_NAME];
};

// start 2012-01-13 by hskim, EP4 [동영상 1회 재생]
struct QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE
{
	UID32_t		AccountUniqueNumber;
};
// end 2012-01-13 by hskim, EP4 [동영상 1회 재생]

class CPreIOCP;
class CAtumPreDBManager : public CAtumDBManager
{
public:
	CAtumPreDBManager(CPreIOCP *pPreIOCP);

	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완

	// For PreServer
	void QP_PRE_Login(DB_QUERY q, SQLHSTMT &hstmt, SQLHSTMT &hstmt_extAuth);
	void QP_MGameLogin(DB_QUERY q, SQLHSTMT hstmt);
	void QP_BlockUser(DB_QUERY q, SQLHSTMT hstmt);
	void QP_UnblockUser(DB_QUERY q, SQLHSTMT hstmt);
//	void QP_LoadBlockedAccount(DB_QUERY q, SQLHSTMT hstmt);
	void QP_LogTotalUser(DB_QUERY q, SQLHSTMT hstmt);

	// 2010-06-01 by shcho, GLogDB 관련 -
	void QP_InsertGlogAccount(DB_QUERY q, SQLHSTMT hstmt);

	// start 2011-12-12 by hskim, GLog 2차
	BOOL IsExistAccount(char *pAccountName, SQLHSTMT &hstmt);
	// end 2011-12-12 by hskim, GLog 2차

	// start 2012-01-13 by hskim, EP4 [동영상 1회 재생]
	void QP_UpdateAccountLastGameEndDate(DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-01-13 by hskim, EP4 [동영상 1회 재생]

	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CAtumPreDBManager::LogInsertBlockUnblock() 추가
	BOOL LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, BYTE i_byLogType);

	// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
	BOOL ExternalAuthentication(CAccountInfo *o_pAccInfo, MSG_PC_CONNECT_LOGIN *i_pLogin, SQLHSTMT &hstmt, SQLHSTMT &hstmt_extAuth, BOOL i_bAuthenticateCheck=TRUE, UINT i_nClientIP=0);

	BOOL ExecuteExtAuth(char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy=EN_DBCONN_EXT_AUTH);
	BOOL ExecuteExtAuth_GalaNet(INT *o_pnExtAccountIDNum, char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy=EN_DBCONN_EXT_AUTH);
	BOOL ExecuteExtAuth2(char *i_szAccName, char *i_szPwd, char *i_szWebLoginAuthKey, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy=EN_DBCONN_EXT_AUTH);
	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	virtual BOOL ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

public:
	CPreIOCP	*m_pPreIOCP2;
};

#endif