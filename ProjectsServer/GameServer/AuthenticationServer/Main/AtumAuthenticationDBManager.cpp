#include "stdafx.h"
#include "AtumAuthenticationDBManager.h"
#include "AuthenticationIOCPSocket.h"
#include "AuthenticationIOCP.h"
#include "md5_lib_src.h"
#include "AtumError.h"
#include "AuthenticationGlobal.h"

CAtumAuthenticationDBManager::CAtumAuthenticationDBManager(CAuthenticationIOCP *pAuthenticationIOCP)
:CAtumDBManager(COUNT_AUTHENTICATION_SERVER_DBWORKER)
{
	m_pAuthenticationIOCP2 = pAuthenticationIOCP;
}

BOOL CAtumAuthenticationDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog 보완
{
	switch(dbquery.enumQuryType)
	{
	case QT_AUTH_CONFIRM : 

		QP_Confirm(dbquery, hstmt);

		break;

	case QT_AUTH_INSERT_CONFIRM_LOG :

		QP_InsertConfirmLog(dbquery, hstmt);

		break;

	default:
		{
			// error: no such DB query type
			DBGOUT("Error! No such DB query type! %d\n", dbquery.enumQuryType);
			return FALSE;
		}
		break;
	}

	return TRUE;
}

void CAtumAuthenticationDBManager::QP_Confirm(DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: account
	-- Desc: server account
	--			2011-01-26 by hskim
	--====
	USE [db_authentication_server]
	GO
	-- 개체:  Table [dbo].[account]
	SET ANSI_NULLS ON
	GO
	SET QUOTED_IDENTIFIER ON
	GO
	CREATE TABLE [dbo].[account](
		[UniqueNumber] [bigint] IDENTITY(1,1) NOT NULL,
		[GameName] [nchar](30) NOT NULL,
		[ServerIP] [nchar](20) NOT NULL,
		[ServerNetmask] [nchar](20) NOT NULL,
		[Publisher] [nchar](35) NOT NULL,
		[Allow] [tinyint] NOT NULL,
		[Shutdown] [Tinyint] NOT NULL,
		[ResourceFileName] [nchar](254) NULL,
	 CONSTRAINT [PK_account] PRIMARY KEY CLUSTERED 
	(
		[UniqueNumber] ASC
	)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
	) ON [PRIMARY]
	**************************************************************************/

	QPARAM_AUTHENTICATION_ACCOUNT	*pRMsg = (QPARAM_AUTHENTICATION_ACCOUNT*)q.pQueryParam;

	RETCODE ret;
  	SQLINTEGER arrCB = { SQL_NTS };

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, pRMsg->szGameName, 0, &arrCB);

	ret  = SQLExecDirect(hstmt, PROCEDURE_AUTHENTICATION_100701_0001, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"[Error]CAtumAuthenticationDBManager::QP_Confirm@QP_Select Authentication Account Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		return ;
	}

	SSERVER_AUTHENTICATION_ACCOUNT	BuffAuthAccount;
	MEMSET_ZERO(&BuffAuthAccount, sizeof(SSERVER_AUTHENTICATION_ACCOUNT));

	int nObjects = 0;

	SQLINTEGER arrCB2[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &BuffAuthAccount.UniqueNumber, 0, &arrCB2[0]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, BuffAuthAccount.szGameName, SIZE_MAX_AUTH_GAMENAME, &arrCB2[1]);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, BuffAuthAccount.szServerIP, SIZE_MAX_IPADDRESS, &arrCB2[2]);
	SQLBindCol(hstmt, 4, SQL_C_CHAR, BuffAuthAccount.szServerNetmask, SIZE_MAX_IPADDRESS, &arrCB2[3]);
	SQLBindCol(hstmt, 5, SQL_C_CHAR, BuffAuthAccount.szPublisher, SIZE_MAX_AUTH_PUBLISHER, &arrCB2[4]);
	SQLBindCol(hstmt, 6, SQL_C_TINYINT, &BuffAuthAccount.iAllow, 0, &arrCB2[5]);
	SQLBindCol(hstmt, 7, SQL_C_TINYINT, &BuffAuthAccount.iShutdown, 0, &arrCB2[6]);
	SQLBindCol(hstmt, 8, SQL_C_CHAR, BuffAuthAccount.szResourceFileName, SIZE_MAX_AUTH_RESOURCE_FILE_NAME, &arrCB2[7]);

	STRNCPY_MEMSET(pRMsg->szAcceptComment, STRMSG_AUTHENTICATION_ACCEPT_COMMENT_NOT_REGISTER, SIZE_MAX_AUTH_ACCEPT_COMMENT);

	while( (ret = SQLFetch(hstmt)) != SQL_NO_DATA )
	{
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumAuthenticationDBManager::QP_Confirm# error !! ObjectCount(%d) ret(0x%X) \r\n", nObjects, ret);

			SQLFreeStmt(hstmt, SQL_CLOSE);
			STRNCPY_MEMSET(pRMsg->szAcceptComment, STRMSG_AUTHENTICATION_ACCEPT_COMMENT_DB_ERROR, SIZE_MAX_AUTH_ACCEPT_COMMENT);

			return ;
		}

		nObjects = nObjects + 1;
		
		// 확인 및 처리
		if( CheckAuthenticationIP(BuffAuthAccount.szServerIP, BuffAuthAccount.szServerNetmask, pRMsg->szServerPublicIP) )
		{
			memcpy((char *)&pRMsg->AuthAccount, (char *)&BuffAuthAccount, sizeof(SSERVER_AUTHENTICATION_ACCOUNT));
			
			if( BuffAuthAccount.iAllow == 1 )
			{
				pRMsg->bAccept = TRUE;
				
				if( 1 == BuffAuthAccount.iShutdown )
				{
					pRMsg->bShutdown = TRUE;
					STRNCPY_MEMSET(pRMsg->szAcceptComment, STRMSG_AUTHENTICATION_ACCEPT_COMMENT_SHUTDOWN, SIZE_MAX_AUTH_ACCEPT_COMMENT);
				}
				else

				{
				STRNCPY_MEMSET(pRMsg->szAcceptComment, STRMSG_AUTHENTICATION_ACCEPT_COMMENT_OK, SIZE_MAX_AUTH_ACCEPT_COMMENT);
			}
			}
			else
			{
				STRNCPY_MEMSET(pRMsg->szAcceptComment, STRMSG_AUTHENTICATION_ACCEPT_COMMENT_BLOCKED, SIZE_MAX_AUTH_ACCEPT_COMMENT);
			}

			break;
		}

		MEMSET_ZERO(&BuffAuthAccount, sizeof(SSERVER_AUTHENTICATION_ACCOUNT));
	}

	pRMsg->bResult = TRUE;
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return ;
}

void CAtumAuthenticationDBManager::QP_InsertConfirmLog(DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: log_aceonline
	-- Desc: inserts log
	--			2011-01-26 by hskim
	--====
	USE [db_authentication_server]
	GO
	--개체:  Table [dbo].[log_aceonline]
	SET ANSI_NULLS ON
	GO
	SET QUOTED_IDENTIFIER ON
	GO
	CREATE TABLE [dbo].[log_aceonline](
		[UniqueNumber] [int] IDENTITY(1,1) NOT NULL,
		[Date] [datetime] NOT NULL,
		[PublicIP] [nchar](20) NULL,
		[PrivateIP] [nchar](20) NULL,
		[Accept] [tinyint] NULL,
		[AcceptComment] [nchar](254) NULL,
		[Publisher] [nchar](35) NULL,
		[Sender] [nchar](30) NULL,
		[CurrentVersion] [nchar](30) NULL,
		[LanguageType] [int] NULL,
		[TestServer] [tinyint] NULL,
		[ListeningPort] [int] NULL,
		[ExternalAuthentication] [tinyint] NULL,
		[PreServerGroupCnts] [int] NULL,
		[EnableGameServerGroupCnts] [int] NULL,
	CONSTRAINT [PK_log_access] PRIMARY KEY CLUSTERED 
	(
	[UniqueNumber] ASC
	)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
	) ON [PRIMARY]
	**************************************************************************/
	QPARAM_AUTHENTICATION_ACCOUNT	*pRMsg = (QPARAM_AUTHENTICATION_ACCOUNT*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER arrCB[14] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_IPADDRESS, 0, pRMsg->szServerPublicIP, 0, &arrCB[0]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_IPADDRESS, 0, pRMsg->szServerPrivateIP, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_C_TINYINT, 0, 0, &pRMsg->bAccept, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_C_TINYINT, 0, 0, &pRMsg->bShutdown, 0, &arrCB[3]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_AUTH_ACCEPT_COMMENT, 0, pRMsg->szAcceptComment, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_AUTH_PUBLISHER, 0, pRMsg->AuthAccount.szPublisher, 0, &arrCB[5]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_AUTH_GAMENAME, 0, pRMsg->szGameName, 0, &arrCB[6]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_AUTH_CURRENTVER, 0, pRMsg->szCurrentVer, 0, &arrCB[7]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nLanguageType, 0, &arrCB[8]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_C_TINYINT, 0, 0, &pRMsg->byTestServer, 0, &arrCB[9]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_INTEGER, 0, 0, &pRMsg->nServerPort, 0, &arrCB[10]);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_C_TINYINT, 0, 0, &pRMsg->byUseExternalAuthentication, 0, &arrCB[11]);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nPreServerGroupCnts, 0, &arrCB[12]);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nEnableGameServerGroupCnts, 0, &arrCB[13]);

	ret  = SQLExecDirect(hstmt, PROCEDURE_AUTHENTICATION_100701_0002, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_Authentication Account Insert Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAuthenticationDBManager::ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAuthenticationDBManager::ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - AuthenticationServer는 사용하는 쿼리가 없음
	//	switch(i_qQuery->enumQuryType)
	//	{
	//	case :
	//	}
	
	// error: no such DB query type
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumAuthenticationDBManager::ProcessDinamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQuryType, GetDBQueryTypeString(i_qQuery->enumQuryType));
	return FALSE;
}

BOOL CAtumAuthenticationDBManager::CheckAuthenticationIP(char *pPeerIP, char *pPeerNetmask, char *pDBIP)
{
	unsigned long ulAccountPeerIP = 0, ulAccountPeerNetmask = 0, ulAccountDBIP = 0;
	
	ulAccountPeerIP = inet_addr(pPeerIP);
	ulAccountPeerNetmask = inet_addr(pPeerNetmask);
	ulAccountDBIP = inet_addr(pDBIP);

	if( (ulAccountPeerIP & ulAccountPeerNetmask) == (ulAccountDBIP & ulAccountPeerNetmask) )
	{
		return TRUE;
	}
	
	return FALSE;
}