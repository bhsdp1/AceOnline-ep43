#ifndef __ATUM_AUTHENTICATION_DB_MANAGER_H__
#define __ATUM_AUTHENTICATION_DB_MANAGER_H__

#include "AtumDBManager.h"

class CAuthenticationIOCP;
class CAtumAuthenticationDBManager : public CAtumDBManager
{
public:
	CAtumAuthenticationDBManager(CAuthenticationIOCP *pAuthenticationIOCP);
	
	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완

	void QP_Confirm(DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertConfirmLog(DB_QUERY q, SQLHSTMT hstmt);

	BOOL CheckAuthenticationIP(char *pPeerIP, char *pPeerNetmask, char *pDBIP);		// 추후 위치 변경 (네트워크 유틸 통합?)

	virtual BOOL ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);
	
public:
	CAuthenticationIOCP	*m_pAuthenticationIOCP2;
};

#endif
