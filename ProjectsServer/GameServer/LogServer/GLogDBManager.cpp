// GLogDBManager.cpp: implementation of the CGLogDBManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLogDBManager.h"
#include "LogGlobal.h"
#include "AtumDBLogWriter.h"
#include "AtumLogDBManager.h"		// 2011-12-12 by hskim, GLog 2차

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLogDBManager::CGLogDBManager()
:CAtumDBManager(COUNT_LOG_SERVER_DBWORKER)
{
	if(g_pLogGlobal->InMasangServer())
	{// 2006-05-21 by cmkwon
		m_dwCountDBWorker	= 3;
	}
}

CGLogDBManager::~CGLogDBManager()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGLogDBManager::InitGLogDB()
/// \brief		
/// \author		shcho, GLogDB 관련 -
/// \date		2010-06-01 ~ 2010-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGLogDBManager::InitGLogDB()
{
	BOOL bRet = m_OdbcStmt.Init(g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), "GLog", (CHAR*)g_pGlobalGameServer->GetODBCUID(), (CHAR*)g_pGlobalGameServer->GetODBCPASSWORD(), g_pGlobalGameServer->GetMainWndHandle());
	if(FALSE == bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CAtumDBManager::Init_GLogDB) !! %s,%d %s %s \r\n"
			, g_pGlobalGameServer->GetDBServerIP(), g_pGlobalGameServer->GetDBServerPort(), (CHAR*)g_pGlobalGameServer->GetODBCUID(), (CHAR*)g_pGlobalGameServer->GetODBCPASSWORD());
		return FALSE;
	}
	
	
	return TRUE;
}

BOOL CGLogDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog 보완
{
	switch(dbquery.enumQuryType)
	{
	case QT_FL_GLOG_BUY_CASH_ITEM:					QP_FL_GLOG_BUY_CASH_ITEM(dbquery, hstmt_GLog);							break;
	case QT_FL_GLOG_CHARACTER_PLAY_TIME:			QP_FL_GLOG_CHARACTER_PLAY_TIME(dbquery, hstmt_GLog);						break;
	case QT_FL_GLOG_CONNECT_TOTAL_USER_COUNT:		QP_FL_GLOG_CONNECT_TOTAL_USER_COUNT(dbquery, hstmt_GLog);				break;
	case QT_FL_GLOG_CONNECT_ZONE_USER_COUNT:		QP_FL_GLOG_CONNECT_ZONE_USER_COUNT(dbquery, hstmt_GLog);					break;
	case QT_FL_GLOG_EVENT_PARTICIPATION_RATE:		QP_FL_GLOG_EVENT_PARTICIPATION_RATE(dbquery, hstmt_GLog);					break;

	// start 2011-12-12 by hskim, GLog 2차
	case QT_FL_GLOG_TB_CONNECT_USER:				QP_FL_GLOG_TB_CONNECT_USER(dbquery, hstmt_GLog);							break;
	case QT_FL_GLOG_TB_ITEM_BUY:					QP_FL_GLOG_TB_ITEM_BUY(dbquery, hstmt_GLog);								break;
	case QT_FL_GLOG_TB_CONCURRENT_USER:				QP_FL_GLOG_TB_CONCURRENT_USER(dbquery, hstmt_GLog);						break;
	case QT_FL_GLOG_TB_USER_PLAYTIME:				QP_FL_GLOG_TB_USER_PLAYTIME(dbquery, hstmt_GLog);						break;
	case QT_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc:	QP_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc(dbquery, hstmt_GLog); break;
	case QT_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc:	QP_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc(dbquery, hstmt_GLog); break;
	// end 2011-12-12 by hskim, GLog 2차

 	// start 2012-10-08 by khkim, GLog 
 	case QT_FL_LOG_ACCOUNTCONNECT:					QP_PL_LOG_ACCOUNTCONNECT(dbquery, hstmt_GLog);							break;	
 	case QT_FL_LOG_CHARDATA:						QP_FL_LOG_CHARDATA		(dbquery, hstmt_GLog);							break;
 	case QT_FL_LOG_ITEM:							QP_FL_LOG_ITEM			(dbquery, hstmt_GLog);							break;
 	case QT_FL_LOG_ITEMSTATE:						QP_FL_LOG_ITEMSTATE		(dbquery, hstmt_GLog);							break;
 	case QT_FL_LOG_SERVER:							QP_PL_LOG_SERVER		(dbquery, hstmt_GLog);							break;
 	// end 2012-10-08 by khkim, GLog
		
	default:
		{
			g_pGlobal->WriteSystemLogEX(TRUE, STRMSG_080904_0001, dbquery.enumQuryType, GetDBQueryTypeString(dbquery.enumQuryType));
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CGLogDBManager::ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumLogDBManager::ProcessDinamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQuryType, GetDBQueryTypeString(i_qQuery->enumQuryType));
	return FALSE;
}

SQL_TIMESTAMP_STRUCT CGLogDBManager::GetDBCurrentdatetime()
{
	SYSTEMTIME Curtime;
	GetLocalTime(&Curtime);
 	SQL_TIMESTAMP_STRUCT cDate;
 	cDate.year = Curtime.wYear;
 	cDate.month = Curtime.wMonth;
 	cDate.day = Curtime.wDay;
 	cDate.hour = Curtime.wHour;
 	cDate.minute = Curtime.wMinute;
 	cDate.second = Curtime.wSecond;
 	cDate.fraction = Curtime.wMilliseconds * 1000000;

	return cDate;
}

void CGLogDBManager::QP_FL_GLOG_BUY_CASH_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_BUY_ITEM *pRMsg = (MSG_FL_LOG_ITEM_BUY_ITEM*)q.pQueryParam;
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_BUY_CASH_ITEM MSG_FL_LOG_ITEM_BUY_ITEM IS NULL !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.GLog_BuyCashItem_Insert
		@i_AccountName			[nvarchar] (128),		-- 계정 이름
		@i_GamePublisher		[nvarchar] (128),		-- 게임서비스사 이름
		@i_GameName				[nvarchar] (128),		-- 게임 이름
		@i_CharacterName		[nvarchar] (128),		-- 구매한 케릭터 이름
		@i_CharacterLevel		[int],					-- 구매한 케릭터 레벨
		@i_ItemName				[nvarchar] (128),		-- 구매한 아이템 이름
		@i_CashCost				[int],					-- 구매한 아이템 개당 유료 가격
		@i_ItemCount			[int],					-- 구매한 아이템 수
		@i_BuyDate				[datetime]				-- 구매 날짜
	AS
		INSERT INTO GLog.dbo.GLog_BuyCashItem([AccountName], [GamePublisher], [GameName], [CharacterName], [CharacterLevel], [ItemName], [CashCost], [ItemCount], [BuyDate])
			VALUES(@i_AccountName, @i_GamePublisher, @i_GameName, @i_CharacterName, @i_CharacterLevel, @i_ItemName, @i_CashCost, @i_ItemCount, @i_BuyDate)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->AccountName, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, "ACEOnline", 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->CharacterName, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterLevel, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->ItemName, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CashItemPrice, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0001, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_BuyCashItem_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_CHARACTER_PLAY_TIME(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_GAME_END *pRMsg = (MSG_FL_LOG_GAME_END*)q.pQueryParam;
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_BUY_CASH_ITEM MSG_FL_LOG_GAME_END IS NULL !!\r\n");
		return;
	}
	SQL_TIMESTAMP_STRUCT cLoginDate;
	cLoginDate.year = pRMsg->LoginDate.Year;
	cLoginDate.month = pRMsg->LoginDate.Month;
	cLoginDate.day = pRMsg->LoginDate.Day;
	cLoginDate.hour = pRMsg->LoginDate.Hour;
	cLoginDate.minute = pRMsg->LoginDate.Minute;
	cLoginDate.second = pRMsg->LoginDate.Second;
	cLoginDate.fraction = 0;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.GLog_CharacterPlayTime_Insert
		@i_AccountName				[nvarchar] (128),		-- 계정 이름
		@i_GamePublisher			[nvarchar] (128),		-- 게임서비스사 이름
		@i_GameName					[nvarchar] (128),		-- 게임 이름
		@i_GameServerName			[nvarchar] (128),		-- 게임서버 이름
		@i_CharacterName			[nvarchar] (128),		-- 종료한 케릭터 이름
		@i_LoginDate				[datetime],				-- 케릭터 접속 날짜
		@i_LogoutDate				[datetime],				-- 케릭터 종료 날짜
		@i_TotalPlayTime			[bigint],				-- 케릭터 생성일부터 지금까지 플레이 시간
		@i_PlayExp					[int],					-- 케릭터 접속부터 종료까지 획득 경험치
		@i_TotalExp					[bigint],				-- 케릭터 생성일부터 지금까지 획득 경험치
		@i_Race						[nvarchar] (128),		-- 케릭터 종족
		@i_Class					[nvarchar] (128),		-- 케릭터 종류
		@i_Level					[int],					-- 케릭터 종료 시 레벨
		@i_MostStayedInZoneName		[nvarchar] (128)		-- 케릭터가 접속하여 종료시까지 제일 많이 머물렀던 지역 이름
	AS
		INSERT INTO GLog.dbo.GLog_CharacterPlayTime([AccountName], [GamePublisher], [GameName], [GameServerName], [CharacterName], [LoginDate], [LogoutDate], [TotalPlayTime], [PlayExp], [TotalExp], [Race], [Class], [Level], [MostStayedInZoneName])
			VALUES(@i_AccountName, @i_GamePublisher, @i_GameName, @i_GameServerName, @i_CharacterName, @i_LoginDate, @i_LogoutDate,	@i_TotalPlayTime, @i_PlayExp, @i_TotalExp, @i_Race, @i_Class, @i_Level, @i_MostStayedInZoneName)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->AccountName, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, "ACEOnline", 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->CharacterName, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&cLoginDate, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->TotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PlayExp, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->TotalExp, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->CharacterRace, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->CharacterClass, 0, NULL);
	INT GLogTypeLevel = pRMsg->CharacterLevel;
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &GLogTypeLevel, 0, NULL);
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->MostStayedInZoneName, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0002, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_CharacterPlayTime_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
void CGLogDBManager::QP_FL_GLOG_CONNECT_TOTAL_USER_COUNT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER_INFO_TOTAL *pRMsg = (MSG_FL_LOG_SERVER_INFO_TOTAL*)q.pQueryParam;
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_CONNECT_TOTAL_USER_COUNT MSG_FL_LOG_SERVER_INFO_TOTAL IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.GLog_ConnectTotalUserCount_Insert
		@i_Date				[datetime],					-- 저장 시간
		@i_GamePublisher	[nvarchar] (128),			-- 게임서비스사 이름
		@i_GameName			[nvarchar] (128),			-- 게임 이름
		@i_GameServerName	[nvarchar] (128),			-- 게임서버 이름
		@i_TotalUserCount	[int]						-- 최대 동접자 수
	AS
		INSERT INTO GLog.dbo.GLog_ConnectTotalUserCount([Date],	[GamePublisher], [GameName], [GameServerName], [TotalUserCount])
			VALUES(@i_Date,	@i_GamePublisher, @i_GameName, @i_GameServerName, @i_TotalUserCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, "ACEOnline", 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0003, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_ConnectTotalUserCount_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
void CGLogDBManager::QP_FL_GLOG_CONNECT_ZONE_USER_COUNT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER_INFO_MAP *pRMsg = (MSG_FL_LOG_SERVER_INFO_MAP*)q.pQueryParam;
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_CONNECT_ZONE_USER_COUNT MSG_FL_LOG_SERVER_INFO_MAP IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.GLog_ConnectZoneUserCount_Insert
		@i_Date				[datetime],					-- 저장 시간
		@i_GamePublisher	[nvarchar] (128),			-- 게임서비스사 이름
		@i_GameName			[nvarchar] (128),			-- 게임 이름
		@i_GameServerName	[nvarchar] (128),			-- 게임서버 이름
		@i_ZoneName			[nvarchar] (128),			-- 게임 지역 이름
		@i_ZoneUserCount	[int]						-- 해당 게임 지역에 최대 동접자 수		
	AS
		INSERT INTO GLog.dbo.GLog_ConnectZoneUserCount([Date],	[GamePublisher], [GameName], [GameServerName], [ZoneName], [ZoneUserCount])
			VALUES(@i_Date,	@i_GamePublisher, @i_GameName, @i_GameServerName, @i_ZoneName, @i_ZoneUserCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, "ACEOnline", 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, pRMsg->MapName, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0004, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_ConnectZoneUserCount_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
void CGLogDBManager::QP_FL_GLOG_EVENT_PARTICIPATION_RATE(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_EVENT_PARTICIPATION_RATE *pRMsg = (MSG_FL_LOG_EVENT_PARTICIPATION_RATE*)q.pQueryParam;
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_EVENT_PARTICIPATION_RATE MSG_FL_LOG_EVENT_PARTICIPATION_RATE IS NULL !!\r\n");
		return;
	}
	SQL_TIMESTAMP_STRUCT cEventStartDate;
	cEventStartDate.year = pRMsg->StartTime.Year;
	cEventStartDate.month = pRMsg->StartTime.Month;
	cEventStartDate.day = pRMsg->StartTime.Day;
	cEventStartDate.hour = pRMsg->StartTime.Hour;
	cEventStartDate.minute = pRMsg->StartTime.Minute;
	cEventStartDate.second = pRMsg->StartTime.Second;
	cEventStartDate.fraction = 0;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.GLog_EventParticipationRate_Insert
		@i_GamePublisher			[nvarchar] (128),			-- 게임서비스사 이름 
		@i_GameName					[nvarchar] (128),			-- 게임 이름
		@i_GameServerName			[nvarchar] (128),			-- 게임서버 이름     
		@i_StartDate				[datetime],					-- 이벤트 시작 날짜
		@i_EndDate					[datetime],					-- 이벤트 종료 날짜
		@i_ParticipationCount		[int],						-- 이벤트 참여 총인원수
		@i_ParticipationRate		[smallint],					-- 이벤트 참여율 [0%~100%]
		@i_Description				[nvarchar] (512)			-- 이벤트 설명	
	AS
		INSERT INTO GLog.dbo.GLog_EventParticipationRate([GamePublisher], [GameName], [GameServerName], [StartDate], [EndDate], [ParticipationCount], [ParticipationRate], [Description])
			VALUES(@i_GamePublisher, @i_GameName, @i_GameServerName, @i_StartDate, @i_EndDate, @i_ParticipationCount, @i_ParticipationRate, @i_Description)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, "ACEOnline", 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 128, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&cEventStartDate, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ParticipationCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->ParticipationRate, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GLOG_EVENT_DESCRIPTION, 0, pRMsg->Description, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0005, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_EventParticipationRate_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

// start 2011-12-12 by hskim, GLog 2차
void CGLogDBManager::QP_FL_GLOG_TB_CONNECT_USER(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME	*pRMsg = (QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME*)q.pQueryParam;

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_TB_CONNECT_USER QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE [dbo].[GLog_TB_CONNECT_USER_Insert]
		@i_AccountName				VARCHAR(20),
		@i_GamePublisher			NVARCHAR(100),
		@i_GameServer				NVARCHAR(100)
	AS
	-- YD 보안 코드
	BEGIN
		SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
		BEGIN TRAN
		
		DECLARE @RtnDate			DATETIME
		SET @RtnDate = (SELECT TOP 1 [Date] from [GLog].[dbo].[GLog_Local_UserConnectInfo] ORDER BY [Date])
		
		IF (@RtnDate IS NOT NULL )
		BEGIN
			-- 여기서 시간 확인 후 날이 지났으면 Insert 처리
			DECLARE @NowDate		DATETIME
			SET @NowDate = getdate()

			IF DATEDIFF(d, @NowDate, @RtnDate) != 0
			BEGIN
				DECLARE @UniqueCount	INT
				DECLARE @TotalCount		INT
				DECLARE @NewCount		INT
				DECLARE @DeleteCount	INT
				
				SET @UniqueCount = (SELECT count(*) from [GLog].[dbo].[GLog_Local_UserConnectInfo] WHERE DATEDIFF(d, @RtnDate, [Date]) = 0)
				SET @TotalCount = (SELECT sum(CurrentCount) from [GLog].[dbo].[GLog_Local_UserConnectInfo] WHERE DATEDIFF(d, @RtnDate, [Date]) = 0)
				SET @NewCount = (SELECT sum(NewCount) from [GLog].[dbo].[GLog_Local_UserConnectInfo] WHERE DATEDIFF(d, @RtnDate, [Date]) = 0)
				SET @DeleteCount = (SELECT sum(DeleteCount) from [GLog].[dbo].[GLog_Local_UserConnectInfo] WHERE DATEDIFF(d, @RtnDate, [Date]) = 0)
				
				INSERT INTO [GLog].[dbo].[GLog_TB_CONNECT_USER]
					([c_date], [c_world], [c_unique], [c_new], [c_times], [c_del], [c_gameserver], [c_publisher])
						VALUES (convert(varchar(19), @RtnDate, 112), 0, @UniqueCount, @NewCount, @TotalCount, @DeleteCount, @i_GameServer, @i_GamePublisher)
				DELETE FROM [GLog].[dbo].[GLog_Local_UserConnectInfo] WHERE DATEDIFF(d, @RtnDate, [Date]) = 0
			END
		END

		-- 현재 테이블에 있는지 확인하고 없으면 추가 있으면 업데이트 한다
		
		DECLARE @RtnID			VARCHAR(40)
		SET @RtnID = (SELECT [ID] from [GLog].[dbo].[GLog_Local_UserConnectInfo] where [ID] = @i_AccountName)
		
		IF (@RtnID IS NOT NULL )
			BEGIN
				-- 업데이트
				UPDATE [GLog].[dbo].[GLog_Local_UserConnectInfo] SET [CurrentCount] = [CurrentCount] + 1 WHERE [ID] = @i_AccountName;
			END
		ELSE
			BEGIN
				-- 추가
				INSERT INTO [GLog].[dbo].[GLog_Local_UserConnectInfo] ([Date], [ID], [CurrentCount])
					VALUES (getdate(), @i_AccountName, 1)
			END
		
		COMMIT TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED;	
	END
	GO
	**************************************************************************/

	RETCODE ret;
	SQLINTEGER arrCB[3] = { SQL_NTS, SQL_NTS, SQL_NTS };

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0,pRMsg->szAccountName, 0, &arrCB[0]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GAME_PUBLISHER_NAME, 0, g_pLogGlobal->GetGamePublisherName(), 0, &arrCB[1]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, g_pLogGlobal->GetServerGroupName(), 0, &arrCB[2]);

	ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0002, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_FL_GLOG_TB_CONNECT_USER Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_TB_ITEM_BUY(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_BUY_ITEM *pRMsg = (MSG_FL_LOG_ITEM_BUY_ITEM*)q.pQueryParam;

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_TB_ITEM_BUY MSG_FL_LOG_ITEM_BUY_ITEM IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROC [dbo].[GLog_TB_ITEM_BUY_Insert]
		@i_ib_date			DATETIME,
		@i_ib_totalCost		INT,
		@i_ib_CashCost		INT,
		@i_us_no			INT,
		@i_us_id			VARCHAR(255),
		@i_ib_good_id		INT,
		@i_ib_good_name		VARCHAR(128),
		@i_ib_publisher		VARCHAR(100),
		@i_ib_charName		VARCHAR(100),
		@i_ib_charLevel		INT,
		@i_ib_itemCount		INT,
		@i_ib_gameserver	VARCHAR(100)
	AS
	-- YD 보안 코드
	BEGIN
		INSERT INTO [GLog].[dbo].[GLog_TB_ITEM_BUY] ([ib_date], [ib_totalCost], [ib_CashCost], [us_no], [us_id], [ib_good_id], [ib_good_name], [ib_publisher], [ib_charName], [ib_charLevel], [ib_itemCount], [ib_gameserver])
			VALUES(@i_ib_date, @i_ib_totalCost, @i_ib_CashCost, @i_us_no, @i_us_id, @i_ib_good_id, @i_ib_good_name, @i_ib_publisher, @i_ib_charName, @i_ib_charLevel, @i_ib_itemCount, @i_ib_gameserver)
	END
	GO
	**************************************************************************/

	int iCount = 1;
	int iTotalCost = pRMsg->CashItemPrice * pRMsg->ChangeCount;

	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &iTotalCost, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &iTotalCost, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ITEM_NAME, 0, pRMsg->ItemName, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterLevel, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0003, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_TB_ITEM_BUY_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_TB_CONCURRENT_USER(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER_INFO_TOTAL *pRMsg = (MSG_FL_LOG_SERVER_INFO_TOTAL*)q.pQueryParam;

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_TB_CONCURRENT_USER MSG_FL_LOG_SERVER_INFO_TOTAL IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROC [dbo].[GLog_TB_CONCURRENT_USER_Insert]
		@i_cu_world			TINYINT,
		@i_cu_date			SMALLDATETIME,
		@i_cu_count			INT,
		@i_cu_publisher		VARCHAR(100),
		@i_cu_gameServer	VARCHAR(100)
	AS
	-- YD 보안 코드
	BEGIN
		INSERT INTO [GLog].[dbo].[GLog_TB_CONCURRENT_USER] ([cu_world], [cu_date], [cu_count], [cu_publisher], [cu_gameServer])
			VALUES(@i_cu_world, @i_cu_date, @i_cu_count, @i_cu_publisher, @i_cu_gameServer)
	END
	GO
	**************************************************************************/

	int iCount = 1;
	BYTE byWorld = 0;

	ATUM_DATE_TIME	CurrentDate;
	CurrentDate.SetCurrentDateTime();
	char szCurrentDate[SIZE_MAX_SQL_DATETIME_STRING] = { 0, };
	CurrentDate.GetSQLDateTimeString(szCurrentDate, SIZE_MAX_SQL_DATETIME_STRING);

	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byWorld, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szCurrentDate, 0,	NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientCount, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0004, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.GLog_TB_CONCURRENT_USER_Insert Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_TB_USER_PLAYTIME(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_GAME_END *pRMsg = (MSG_FL_LOG_GAME_END*)q.pQueryParam;

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_GLOG_TB_USER_PLAYTIME MSG_FL_LOG_GAME_END IS NULL !!\r\n");
		return;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROC [dbo].[GLog_TB_USER_PLAYTIME_Insert]
		@i_up_login		DATETIME,
		@i_up_logout	DATETIME,
		@i_up_time		INT,
		@i_us_no		INT,
		@i_up_charID	INT,
		@i_up_country	CHAR(1),
		@i_up_race		CHAR(1),
		@i_up_class		CHAR(1),
		@i_up_exp		INT,
		@i_up_publisher		VARCHAR(100),
		@i_up_gameServer	VARCHAR(100)
	AS
	-- YD 보안 코드
	BEGIN
		INSERT INTO [GLog].[dbo].[GLog_TB_USER_PLAYTIME] ([up_login], [up_logout], [up_time], [us_no], [up_charID], [up_country], [up_race], [up_class], [up_exp], [up_publisher], [up_gameServer])
			VALUES(@i_up_login, @i_up_logout, @i_up_time, @i_us_no, @i_up_charID, @i_up_country, @i_up_race, @i_up_class, @i_up_exp, @i_up_publisher, @i_up_gameServer)
	END
	GO
	**************************************************************************/

	int iCount = 1;

	ATUM_DATE_TIME	CurrentDate;
	CurrentDate.SetCurrentDateTime();
	char szCurrentDate[SIZE_MAX_SQL_DATETIME_STRING] = { 0, };
	CurrentDate.GetSQLDateTimeString(szCurrentDate, SIZE_MAX_SQL_DATETIME_STRING);

	if( FALSE == pRMsg->LoginDate.IsValidATUM_DATE_TIME() )
	{
		pRMsg->LoginDate.SetCurrentDateTime();
	}

	SQL_TIMESTAMP_STRUCT cLoginDate;
	cLoginDate.year = pRMsg->LoginDate.Year;
	cLoginDate.month = pRMsg->LoginDate.Month;
	cLoginDate.day = pRMsg->LoginDate.Day;
	cLoginDate.hour = pRMsg->LoginDate.Hour;
	cLoginDate.minute = pRMsg->LoginDate.Minute;
	cLoginDate.second = pRMsg->LoginDate.Second;
	cLoginDate.fraction = 0;

	char byCountry[2] = { '0', 0 };
	char byRace[2] = { '0', 0 };
	char byClass[2] = { '0', 0 };

	if( TRUE == IS_BGEAR(pRMsg->UnitKind) ) byClass[0] = '1';		// 0 = 없음		1 = B기어		2 = M기어		3 = A기어		4 = I기어
	if( TRUE == IS_MGEAR(pRMsg->UnitKind) ) byClass[0] = '2';		// 0 = 없음		1 = B기어		2 = M기어		3 = A기어		4 = I기어
	if( TRUE == IS_AGEAR(pRMsg->UnitKind) ) byClass[0] = '3';		// 0 = 없음		1 = B기어		2 = M기어		3 = A기어		4 = I기어
	if( TRUE == IS_IGEAR(pRMsg->UnitKind) ) byClass[0] = '4';		// 0 = 없음		1 = B기어		2 = M기어		3 = A기어		4 = I기어

	if( INFLUENCE_TYPE_NORMAL == pRMsg->InfluenceType ) byCountry[0] = '0' + INFLUENCE_TYPE_NORMAL;
	if( INFLUENCE_TYPE_VCN == pRMsg->InfluenceType ) byCountry[0] = '0' + INFLUENCE_TYPE_VCN;
	if( INFLUENCE_TYPE_ANI == pRMsg->InfluenceType ) byCountry[0] = '0' + INFLUENCE_TYPE_ANI;
	if( INFLUENCE_TYPE_RRP == pRMsg->InfluenceType ) byCountry[0] = '0' + INFLUENCE_TYPE_RRP;

	if( (RACE_BATTALUS & pRMsg->Race) ) byRace[0] = '1';				// 바탈러스
	if( (RACE_DECA & pRMsg->Race) ) byRace[0] = '2';					// 데카
	if( (RACE_PHILON & pRMsg->Race) ) byRace[0] = '3';					// 필론
	if( (RACE_SHARRINE & pRMsg->Race) ) byRace[0] = '4';				// 샤린

	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&cLoginDate, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, 23, 3, (SQLPOINTER)&(this->GetDBCurrentdatetime()), 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->PlayTime, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 1, 0, byCountry, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 1, 0, byRace, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 1, 0, byClass, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &pRMsg->PlayExp, 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetGamePublisherName(), 0, NULL);
	SQLBindParameter(hstmt, iCount++, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, g_pLogGlobal->GetServerGroupName(), 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0005, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@dbo.QP_FL_GLOG_TB_USER_PLAYTIME Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_GLOG_USER_INFO_COUNT_INC	*pRMsg = (QPARAM_GLOG_USER_INFO_COUNT_INC*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER arrCB[2] = { SQL_NTS, SQL_NTS };

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE [dbo].[GLog_TB_USER_LocalUserConnectInfo_DelCount_Inc]
		@i_AccountName				VARCHAR(20)
	AS
		SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
		BEGIN TRAN

		DECLARE @RtnID			VARCHAR(40)
		SET @RtnID = (SELECT [ID] from [GLog].[dbo].[GLog_Local_UserConnectInfo] where [ID] = @i_AccountName)
		
		IF (@RtnID IS NOT NULL )
		BEGIN
			-- 업데이트
			UPDATE [GLog].[dbo].[GLog_Local_UserConnectInfo] SET [DeleteCount] = [DeleteCount] + 1 WHERE [ID] = @i_AccountName;
		END
		
		COMMIT TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED;	
	GO
	**************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0,pRMsg->szAccountName, 0, &arrCB[0]);

	ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0007, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"QP_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME	*pRMsg = (QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER arrCB[2] = { SQL_NTS, SQL_NTS };

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE [dbo].[GLog_TB_USER_LocalUserConnectInfo_NewCount_Inc]
		@i_AccountName				VARCHAR(20)
	AS
	-- YD 보안 코드
	BEGIN
		SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
		BEGIN TRAN

		DECLARE @RtnID			VARCHAR(40)
		SET @RtnID = (SELECT [ID] from [GLog].[dbo].[GLog_Local_UserConnectInfo] where [ID] = @i_AccountName)

		IF (@RtnID IS NOT NULL )
			BEGIN
				-- 업데이트
				UPDATE [GLog].[dbo].[GLog_Local_UserConnectInfo] SET [NewCount] = 1 WHERE [ID] = @i_AccountName;
			END
		ELSE
			BEGIN
				-- 추가
				INSERT INTO [GLog].[dbo].[GLog_Local_UserConnectInfo] ([Date], [ID], [CurrentCount], [NewCount])
					VALUES (getdate(), @i_AccountName, 0, 1)
			END
		
		COMMIT TRAN
		SET TRANSACTION ISOLATION LEVEL READ COMMITTED;	
	END
	GO
	**************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0,pRMsg->szAccountName, 0, &arrCB[0]);

	ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_2ND_111212_0006, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"QP_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
// end 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 by khkim, GLog 
void CGLogDBManager::QP_PL_LOG_ACCOUNTCONNECT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ACCOUNTCONNECT *pRMsg = (MSG_FL_LOG_ACCOUNTCONNECT*)q.pQueryParam;
	SQLINTEGER arrCB[4] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_PL_LOG_ACCOUNTCONNECT !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************

	CREATE PROCEDURE GLog_ImportAcconutConnect
	@i_LOGTYPE			SMALLINT,
	@i_USERSERIAL		BIGINT,
	@i_IP				BIGINT,
	@i_GAMESERVERID		INT
	AS
	INSERT INTO GLog_AccountConnect(LogType, UserSerial, IP, GameServerID)
	VALUES (
	GETDATE(),
	@i_LOGTYPE,
	@i_USERSERIAL,	
	@i_IP,
	@i_GAMESERVERID
	)	
	GO

	**************************************************************************/
	SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_SHORT,		SQL_SMALLINT,	0,0,&pRMsg->LogType,		0, &arrCB[0]);
	SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->UserSerial,		0, &arrCB[1]);
	SQLBindParameter(hstmt,3,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->IP,				0, &arrCB[2]);
	SQLBindParameter(hstmt,4,SQL_PARAM_INPUT,SQL_C_LONG,		SQL_INTEGER,	0,0,&pRMsg->GameServerID,	0, &arrCB[3]);

	// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
#ifdef S_KOR_TEST_GLOG_CHARDATA	// - 쓰레기 값 추적
	{
		g_pGlobal->WriteSystemLogEX(TRUE,"[GLOG_DEBUG] Insert Data CGLogDBManager::QP_PL_LOG_ACCOUNTCONNECT - LogType(%d), UserSerial(%I64d), IP(%I64d), GameServerID(%d)\r\n",
			pRMsg->LogType,	pRMsg->UserSerial,	pRMsg->IP,	pRMsg->GameServerID);
	}
#endif 
	// end 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_3ND_121011_0001, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_PL_LOG_ACCOUNTCONNECT Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_LOG_CHARDATA(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_CHARDATA *pRMsg = (MSG_FL_LOG_CHARDATA*)q.pQueryParam;
	SQLINTEGER arrCB[9] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_LOG_CHARDATA !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************

	CREATE PROCEDURE GLog_ImportCharData
	@i_LOGTYPE			SMALLINT,
	@i_USERSERIAL		BIGINT,
	@i_CHARSERIAL		BIGINT,
	@i_CLASS			SMALLINT,
	@i_LV				SMALLINT,
	@i_EXP				BIGINT,
	@i_GAMEMONEY		BIGINT,
	@i_PLAYTIME			BIGINT,
	@i_GAMESERVERID		INT
	AS
	INSERT INTO GLog_CharData(LogType, UserSerial, CharSerial, Class, Lv, Exp, GameMoney, Playtime, GameServerID)
	VALUES (
	GETDATE(),
	@i_LOGTYPE,
	@i_USERSERIAL,
	@i_CHARSERIAL,
	@i_CLASS,
	@i_LV,
	@i_EXP,
	@i_GAMEMONEY,	
	@i_PLAYTIME,
	@i_GAMESERVERID
	)
	GO

	**************************************************************************/
	
	SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_SHORT,		SQL_SMALLINT,	0,0,&pRMsg->LogType,		0, &arrCB[0]);
	SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->UserSerial,		0, &arrCB[1]);
	SQLBindParameter(hstmt,3,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->CharSerial,		0, &arrCB[2]);
	SQLBindParameter(hstmt,4,SQL_PARAM_INPUT,SQL_C_SHORT,		SQL_SMALLINT,	0,0,&pRMsg->Class,			0, &arrCB[3]);
	SQLBindParameter(hstmt,5,SQL_PARAM_INPUT,SQL_C_SHORT,		SQL_SMALLINT,	0,0,&pRMsg->Lv,				0, &arrCB[4]);
	SQLBindParameter(hstmt,6,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->Exp,			0, &arrCB[5]);
	SQLBindParameter(hstmt,7,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->GameMoney,		0, &arrCB[6]); 
	SQLBindParameter(hstmt,8,SQL_PARAM_INPUT,SQL_C_SBIGINT,	SQL_BIGINT,		0,0,&pRMsg->Playtime,		0, &arrCB[7]);
	SQLBindParameter(hstmt,9,SQL_PARAM_INPUT,SQL_C_LONG,		SQL_INTEGER,	0,0,&pRMsg->GameServerID,	0, &arrCB[8]);


// 2013-01-03 by khkim, GLog (버그 확인용)
#ifdef S_KOR_TEST_GLOG_CHARDATA
//	if(pRMsg->UserSerial < 0 || pRMsg->UserSerial > 7000000 )	// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
	{
		// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
//		g_pGlobal->WriteSystemLogEX(TRUE,"[DEBUG] GLog #200 [%d,%I64d,%I64d,%d,%d,%I64d,%I64d,%I64d,%d]\r\n ", 
		g_pGlobal->WriteSystemLogEX(TRUE,"[GLOG_DEBUG] Insert Data CGLogDBManager::QP_FL_LOG_CHARDATA - LogType(%d), UserSerial(%I64d), CharSerial(%I64d), Class(%d), Lv(%d), Exp(%I64d),  GameMoney(%I64d), Playtime(%I64d),GameServerID(%d)\r\n",
		// end 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
			pRMsg->LogType,		pRMsg->UserSerial,	pRMsg->CharSerial,
			pRMsg->Class,		pRMsg->Lv,			pRMsg->Exp, 
			pRMsg->GameMoney,	pRMsg->Playtime,	pRMsg->GameServerID);
	}
#endif 
// end 2013-01-03 by khkim, GLog (버그 확인용)		

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_3ND_121011_0002, SQL_NTS);
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
// 2013-01-03 by khkim, GLog (버그 확인용)
#ifdef S_KOR_TEST_GLOG_CHARDATA
		g_pGlobal->WriteSystemLogEX(TRUE,"[DEBUG] GLog #201 [%d,%I64d,%I64d,%d,%d,%I64d,%I64d,%I64d,%d]\r\n ", 
			pRMsg->LogType,		pRMsg->UserSerial,	pRMsg->CharSerial,
			pRMsg->Class,		pRMsg->Lv,			pRMsg->Exp, 
			pRMsg->GameMoney,	pRMsg->Playtime,	pRMsg->GameServerID);
#endif
// end 2013-01-03 by khkim, GLog (버그 확인용)

		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"QP_FL_LOG_CHARDATA Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_LOG_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM *pRMsg = (MSG_FL_LOG_ITEM*)q.pQueryParam;
	SQLINTEGER arrCB[7] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };

	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_LOG_ITEM !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************

	CREATE PROCEDURE GLog_ImportItemLog
	@i_LOGTYPE			SMALLINT,
	@i_ITEMSERIAL		BIGINT,
	@i_COUNT1			BIGINT,
	@i_COUNT2			BIGINT,
	@i_CHARSERIAL		BIGINT,
	@i_CHARLV			INT,
	@i_GAMESERVERID		INT
	AS
	INSERT INTO GLog_ItemLog(LogDate, LogType, ItemSerial, Count1, Count2, CharSerial, CharLv, GameServerID)
	VALUES(
	GETDATE(),		
	@i_LOGTYPE,		
	@i_ITEMSERIAL,		
	@i_COUNT1,			
	@i_COUNT2,		
	@i_CHARSERIAL,		
	@i_CHARLV,	 
	@i_GAMESERVERID	
	)
	GO

	**************************************************************************/
	
	SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_SHORT,			SQL_SMALLINT,		0,0,&pRMsg->LogType,		0, &arrCB[0]);
	SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->ItemSerial,		0, &arrCB[1]);
	SQLBindParameter(hstmt,3,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->Count1,			0, &arrCB[2]);
	SQLBindParameter(hstmt,4,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->Count2,			0, &arrCB[3]);
	SQLBindParameter(hstmt,5,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->CharSerial,		0, &arrCB[4]);
	SQLBindParameter(hstmt,6,SQL_PARAM_INPUT,SQL_C_LONG,			SQL_INTEGER,		0,0,&pRMsg->CharLv,			0, &arrCB[5]);
	SQLBindParameter(hstmt,7,SQL_PARAM_INPUT,SQL_C_LONG,			SQL_INTEGER,		0,0,&pRMsg->GameServerID,	0, &arrCB[6]);

	// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
#ifdef S_KOR_TEST_GLOG_CHARDATA	// - 쓰레기 값 추적
	{
		g_pGlobal->WriteSystemLogEX(TRUE,"[GLOG_DEBUG] Insert Data CGLogDBManager::QP_FL_LOG_ITEM - LogType(%d), ItemSerial(%I64d), Count1(%I64d), Count2(%I64d), CharSerial(%I64d), CharLv(%d), GameServerID(%d)\r\n",
			pRMsg->LogType,	pRMsg->ItemSerial,	pRMsg->Count1,		pRMsg->Count2,		pRMsg->CharSerial,	pRMsg->CharLv,		pRMsg->GameServerID);
	}
#endif 
	// end 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_3ND_121011_0003, SQL_NTS);
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_FL_LOG_ITEM Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_FL_LOG_ITEMSTATE(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEMSTATE *pRMsg = (MSG_FL_LOG_ITEMSTATE*)q.pQueryParam;
	SQLINTEGER arrCB[6] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_FL_LOG_ITEMSTATE !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************

	CREATE PROCEDURE GLog_ImportItemStateLog
	@i_LOGTYPE			SMALLINT,
	@i_ITEMSERIAL		BIGINT,
	@i_COUNT			BIGINT,
	@i_CHARSERIAL		BIGINT,
	@i_CHARLV			INT,
	@i_GAMESERVERID		INT
	AS
	INSERT INTO GLog_ItemStateLog(LogDate, LogType, ItemSerial, Count, CharSerial, CharLv, GameServerID)
	VALUES(
	GETDATE(),
	@i_LOGTYPE,
	@i_ITEMSERIAL,
	@i_COUNT,
	@i_CHARSERIAL,
	@i_CHARLV,
	@i_GAMESERVERID
	)
	GO

	**************************************************************************/
	
	SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_SHORT,			SQL_SMALLINT,		0,0,&pRMsg->LogType,		0, &arrCB[0]);
	SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->ItemSerial,		0, &arrCB[1]);
	SQLBindParameter(hstmt,3,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->Count,			0, &arrCB[2]);
	SQLBindParameter(hstmt,4,SQL_PARAM_INPUT,SQL_C_SBIGINT,		SQL_BIGINT,			0,0,&pRMsg->CharSerial,		0, &arrCB[3]);
	SQLBindParameter(hstmt,5,SQL_PARAM_INPUT,SQL_C_LONG,			SQL_INTEGER,		0,0,&pRMsg->CharLv,			0, &arrCB[4]);
	SQLBindParameter(hstmt,6,SQL_PARAM_INPUT,SQL_C_LONG,			SQL_INTEGER,		0,0,&pRMsg->GameServerID,	0, &arrCB[5]);

	// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
#ifdef S_KOR_TEST_GLOG_CHARDATA	// - 쓰레기 값 추적
	{
		g_pGlobal->WriteSystemLogEX(TRUE,"[GLOG_DEBUG] Insert Data CGLogDBManager::QP_FL_LOG_ITEMSTATE - LogType(%d), ItemSerial(%I64d), Count(%I64d), CharSerial(%I64d), CharLv(%d), GameServerID(%d)\r\n",
			pRMsg->LogType,	pRMsg->ItemSerial,	pRMsg->Count,	pRMsg->CharSerial,	pRMsg->CharLv,		pRMsg->GameServerID);
	}
#endif 
	// end 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_3ND_121011_0004, SQL_NTS);
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
// 2013-01-03 by khkim, GLog (버그 확인용)
#ifdef S_KOR_TEST_GLOG_CHARDATA
		g_pGlobal->WriteSystemLogEX(TRUE,"[DEBUG] GLog #202 [%d,%I64d,%I64d,%I64d,%d,%d]\r\n", 
			pRMsg->LogType,		pRMsg->ItemSerial,	pRMsg->Count,
			pRMsg->CharSerial,	pRMsg->CharLv,		pRMsg->GameServerID);
#endif
// end 2013-01-03 by khkim, GLog (버그 확인용)

		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_FL_LOG_ITEMSTATE Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

void CGLogDBManager::QP_PL_LOG_SERVER(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER *pRMsg = (MSG_FL_LOG_SERVER*)q.pQueryParam;
	SQLINTEGER arrCB[2] = { SQL_NTS, SQL_NTS };
	
	if(NULL == pRMsg)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CGLogDBManager::QP_PL_LOG_SERVER !!\r\n");
		return;
	}
	/*[Stored Query Definition]************************************************

	CREATE PROCEDURE GLog_ImportServerLog
	@i_LOGTYPE			SMALLINT,
	@i_GAMESERVERID		INT
	AS
	INSERT INTO GLog_ServerLogLogType, GameServerID)
	VALUES(
	GETDATE(),
	@i_LOGTYPE,
	@i_GAMESERVERID			
	)
	GO

	**************************************************************************/
	
	SQLBindParameter(hstmt,1,SQL_PARAM_INPUT,SQL_C_SHORT,		SQL_SMALLINT,		0,0,&pRMsg->LogType,		0, &arrCB[0]);
	SQLBindParameter(hstmt,2,SQL_PARAM_INPUT,SQL_C_LONG,		SQL_INTEGER,		0,0,&pRMsg->GameServerID,	0, &arrCB[1]);
	
	// 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
#ifdef S_KOR_TEST_GLOG_CHARDATA	// - 쓰레기 값 추적
	{
		g_pGlobal->WriteSystemLogEX(TRUE,"[GLOG_DEBUG] Insert Data CGLogDBManager::QP_PL_LOG_SERVER - LogType(%d), GameServerID(%d)\r\n",
			pRMsg->LogType,		pRMsg->GameServerID);
	}
#endif 
	// end 2013-05-30 by jhseol, GLog 시스템 보완 - 쓰레기 값 추적
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_3ND_121011_0005, SQL_NTS);
	
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_PL_LOG_SERVER Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
// end 2012-10-08 by khkim, GLog