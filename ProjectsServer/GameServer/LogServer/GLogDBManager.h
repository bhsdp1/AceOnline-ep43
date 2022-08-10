// GLogDBManager.h: interface for the CGLogDBManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOGDBMANAGER_H__0D10233B_B919_4E3A_94FA_BD9DD1E4091E__INCLUDED_)
#define AFX_GLOGDBMANAGER_H__0D10233B_B919_4E3A_94FA_BD9DD1E4091E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumDBManager.h"

class CGLogDBManager : public CAtumDBManager  
{
public:
	CGLogDBManager();
	virtual ~CGLogDBManager();

	BOOL InitGLogDB();		// 2010-06-01 by shcho, GLogDB 관련 -
	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완
	virtual BOOL ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

private:
	void QP_FL_GLOG_BUY_CASH_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_CHARACTER_PLAY_TIME(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_CONNECT_TOTAL_USER_COUNT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_CONNECT_ZONE_USER_COUNT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_EVENT_PARTICIPATION_RATE(DB_QUERY q, SQLHSTMT hstmt);		//GLog_Event_Insert	만들어야함.

	// start 2011-12-12 by hskim, GLog 2차
	void QP_FL_GLOG_TB_CONNECT_USER(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_TB_ITEM_BUY(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_TB_CONCURRENT_USER(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_TB_USER_PLAYTIME(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc(DB_QUERY q, SQLHSTMT hstmt);
	// end 2011-12-12 by hskim, GLog 2차

	// start 2012-10-08 by khkim, GLog
	void QP_PL_LOG_ACCOUNTCONNECT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_CHARDATA(DB_QUERY q, SQLHSTMT hstmt); 
	void QP_FL_LOG_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEMSTATE(DB_QUERY q, SQLHSTMT hstmt);
	void QP_PL_LOG_SERVER(DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-10-08 by khkim, GLog

	SQL_TIMESTAMP_STRUCT GetDBCurrentdatetime();			// DB 데이타형인 datetime에 값 대입을 위해 현재 시간을 가져온다.

private:
	CODBCStatement			m_OdbcStmt;
};

#endif // !defined(AFX_GLOGDBMANAGER_H__0D10233B_B919_4E3A_94FA_BD9DD1E4091E__INCLUDED_)
