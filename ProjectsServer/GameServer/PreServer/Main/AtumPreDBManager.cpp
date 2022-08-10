#include "stdafx.h"
#include "AtumPreDBManager.h"
#include "PreIOCPSocket.h"
#include "PreIOCP.h"
#include "md5_lib_src.h"
#include "AtumError.h"
#include "PreGlobal.h"
#include "GenericHTTPClient.h"		// 2008-10-08 by cmkwon, �븸 Netpower_Tpe �ܺ����� ���� - 
#include "AccountBlockManager.h"

CAtumPreDBManager::CAtumPreDBManager(CPreIOCP *pPreIOCP)
:CAtumDBManager(COUNT_PRE_SERVER_DBWORKER)
{
	m_pPreIOCP2 = pPreIOCP;

	if(g_pPreGlobal->IsExternalAuthentication())
	{
		m_bExtAuthDBConnect		= TRUE;
	}

	if(g_pPreGlobal->InMasangServer())
	{// 2006-05-21 by cmkwon
		m_dwCountDBWorker	= 3;
	}
}

BOOL CAtumPreDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog ����
{
	switch(dbquery.enumQuryType)
	{
	case QT_PRE_Login:			QP_PRE_Login(dbquery, hstmt, hstmt_extAuth);		break;
	case QT_BlockUser:			QP_BlockUser(dbquery, hstmt);		break;
	case QT_UnblockUser:		QP_UnblockUser(dbquery, hstmt);		break;
	case QT_MGameLogin:			QP_MGameLogin(dbquery, hstmt);		break;
	case QT_LogTotalUser:		QP_LogTotalUser(dbquery, hstmt);	break;
	case QT_InsertGlogAccount:  QP_InsertGlogAccount(dbquery, hstmt); break;
	case QT_UpdateAccountLastGameEndDate: QP_UpdateAccountLastGameEndDate(dbquery, hstmt); break;		// 2012-01-13 by hskim, EP4 [������ 1ȸ ���]

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

// QT_PRE_Login
void CAtumPreDBManager::QP_PRE_Login(DB_QUERY q, SQLHSTMT &hstmt, SQLHSTMT &hstmt_extAuth)
{
	MSG_PC_CONNECT_LOGIN	*pRMsg = (MSG_PC_CONNECT_LOGIN*)q.pQueryParam;
	CAccountInfo			accInfo;


	_strlwr(pRMsg->AccountName);			// 2006-06-02 by cmkwon, �ҹ��ڷ� ��ȯ

	if(g_pPreGlobal->IsExternalAuthentication() || g_pPreGlobal->GetIsExtAuthUseSOAP())
	{// 2006-04-27 by cmkwon, �ܺ� DB ���� ���� ó�� �߰�
		INT		nExtAccountIDNum = 0;

		// 2008-10-08 by cmkwon, �븸 2�ܰ� ���� �ý��� ���� ����(email->uid) -
		char szOriginalAccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];
		STRNCPY_MEMSET(szOriginalAccountName, pRMsg->AccountName, SIZE_MAX_ORIGINAL_ACCOUNT_NAME);

		// 2010-04-26 by cmkwon, ���þ� Innva ����/���� �ý��� ���� - 
		if(FALSE == this->ExternalAuthentication(&accInfo, pRMsg, hstmt, hstmt_extAuth, q.pIOCPSocket->GetPeerIP4()))
		{
			((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_COMMON_LOGIN_FAILED);
		}
		else
		{

			// 2012-10-07 by hskim, �޽� ����ġ
 			/*[Stored Query Definition]************************************************
			CREATE PROCEDURE [dbo].[atum_PreServerUpdateLastLoginDate]
				@i_accUID			INT
			WITH EXECUTE AS 'proexe'	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ���� ���Ȱ��� ���� ���� ���� �ϱ�(SQL Server 2005�̻� �����ϴ� ���) - 
			AS
			BEGIN
				DECLARE @NowDate				DATETIME; SET @NowDate = GETDATE();
				DECLARE @DiffMin				INT
				DECLARE @RestExperienceCount	INT

				DECLARE @MinTime				INT; SET @MinTime = 300			-- �⺻ �� ����	(�ּ� ���� ��	: [300, 5�ð�])
				DECLARE @MaxCount				INT; SET @MaxCount = 3600		-- �⺻ �� ����	(�ִ� ���� ����	: [3600, 2�д� 1����, �� 5��, 3600����)

				UPDATE dbo.td_account SET LastLoginDate = @NowDate WHERE AccountUniqueNumber = @i_accUID

				-- ���� ���̺��� ���� �ִ��� Ȯ���Ŀ� ������ ����
				SELECT @RestExperienceCount = RestExperienceCount FROM dbo.td_AdditionalExperience WITH (NOLOCK) WHERE AccountUniqueNumber = @i_accUID

				IF( @RestExperienceCount IS NULL )
				BEGIN 
					EXEC [dbo].[atum_Set_AddExp_RestExperienceCount] @i_AccountUniqueNumber = @i_accUID, @i_RestExperienceCount = 0;

					RETURN
				END

				SELECT @DiffMin = DATEDIFF(minute, LastGameEndDate, @NowDate) FROM dbo.td_account WITH (NOLOCK) WHERE [AccountUniqueNumber] = @i_accUID;

				IF( @DiffMin IS NULL )
				BEGIN
					RETURN
				END

				IF( @DiffMin >= @MinTime )
				BEGIN
					SET @RestExperienceCount = @RestExperienceCount + (@DiffMin / 2)

					IF( @RestExperienceCount > @MaxCount )
					BEGIN
						SET @RestExperienceCount = @MaxCount
					END

					EXEC [dbo].[atum_Set_AddExp_RestExperienceCount] @i_AccountUniqueNumber = @i_accUID, @i_RestExperienceCount = @RestExperienceCount;
				END
			END
 			**************************************************************************/
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &accInfo.AccountUniqueNumber, 0, NULL);
			SQLExecDirect(hstmt, PROCEDURE_080822_0266, SQL_NTS);
			SQLMoreResults(hstmt);
 			SQLFreeStmt(hstmt, SQL_CLOSE);
			// end 2012-10-07 by hskim, �޽� ����ġ

			((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_NO_ERROR);
		}
		SAFE_DELETE(pRMsg);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-11 by cmkwon, ���þ� Innova_Rus ���� �ý��� ���� - Usp_Do_Login() ���� ���� �����ÿ��� �������� üũ�� �����ϵ��� ����
	//							Innova���� Usp_Do_Login() ������ atum2_db_account.dbo.td_account ���̺��� AccountName, Password�� �߰� �ϰ�, Password ����ÿ��� Innova���� �������ֱ�� ��.
	RETCODE ret;
	SQLINTEGER arrCB[10] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ��¥�� ������ �α����� ��¥�ε��� ���� �ϳ� �߰�
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &arrCB[0]);
 	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0265, SQL_NTS);
 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
 	{
 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_PreServerGetAccountInfo Failed!\r\n", TRUE);
 		SQLFreeStmt(hstmt, SQL_CLOSE);
 		q.pIOCPSocket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_DB_EXECUTION_FAILED);
 		SAFE_DELETE(pRMsg);
 		return;
 	}
	if(ret == SQL_NO_DATA)
	{// ���� ���� (���̵� ����)

		SQLFreeStmt(hstmt, SQL_CLOSE);
		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_DB_NO_SUCH_ACCOUNT);
		SAFE_DELETE(pRMsg);
		return;
	}

 	char szMD5String[SIZE_MAX_PASSWORD_MD5_STRING];			MEMSET_ZERO(szMD5String, SIZE_MAX_PASSWORD_MD5_STRING);
 	MD5::MD5Binary2String(pRMsg->Password, szMD5String);
	SQL_TIMESTAMP_STRUCT		tmSqlTime;		MEMSET_ZERO(&tmSqlTime, sizeof(tmSqlTime));
	SQL_TIMESTAMP_STRUCT		tmSqlTime2;		MEMSET_ZERO(&tmSqlTime2, sizeof(tmSqlTime2));
	SQL_TIMESTAMP_STRUCT		tmSqlTime3;		MEMSET_ZERO(&tmSqlTime3, sizeof(tmSqlTime3));		// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ��¥�� ������ �α����� ��¥	

	int nBirthYear = 0;		// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������) - ����⵵(�߱��� ��������)�� ������
 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &accInfo.AccountUniqueNumber, 0,						&arrCB[1]);
 	SQLBindCol(hstmt, 2, SQL_C_USHORT, &accInfo.AccountType, 0,								&arrCB[2]); 
	SQLBindCol(hstmt, 3, SQL_C_CHAR, accInfo.PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &tmSqlTime, 0,									&arrCB[4]);
 	SQLBindCol(hstmt, 5, SQL_C_LONG, &accInfo.GameContinueTimeInSecondOfToday, 0,			&arrCB[5]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &tmSqlTime2, 0,									&arrCB[6]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 7, SQL_C_LONG, &nBirthYear, 0,										&arrCB[7]);		// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������)
	SQLBindCol(hstmt, 8, SQL_C_CHAR, accInfo.SecondaryPassword, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[8]);	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - SecondaryPassword �������� �߰�
	SQLBindCol(hstmt, 9, SQL_C_TIMESTAMP, &tmSqlTime3, 0,	&arrCB[9]);	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ��¥�� ������ �α����� ��¥Ȯ���� ����	
 	
 	while( SQL_NO_DATA != (ret = SQLFetch(hstmt)) );
 	SQLFreeStmt(hstmt, SQL_CLOSE);
	accInfo.AccountRegisteredDate	= tmSqlTime;
	accInfo.LastGameEndDate			= tmSqlTime2;			// 2006-11-15 by cmkwon
	accInfo.SetBirthday(nBirthYear);						// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������) - 

	if(FALSE == IS_VALID_UNIQUE_NUMBER(accInfo.AccountUniqueNumber))
	{// 2006-06-02 by cmkwon, AccountUID üũ - ���� ���� (���̵� ����)
		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_DB_NO_SUCH_ACCOUNT);
		SAFE_DELETE(pRMsg);
		return;
	}

	if(0 != stricmp(szMD5String, accInfo.PasswordFromDB))
	{// 2006-06-02 by cmkwon, ��� üũ
 		char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];
 		BYTE byPass[SIZE_MAX_PASSWORD_MD5];
		MD5 md5_instance; 
 		char szTmPassword[1024];		MEMSET_ZERO(szTmPassword, 1024); 		
 		wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, accInfo.PasswordFromDB);
 		md5_instance.MD5Encode(szTmPassword, byPass);
 		MD5::MD5Binary2String(byPass, szEncodedString);
 		if (0 != stricmp(szEncodedString, szMD5String))
		{
	 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_COMMON_LOGIN_FAILED);
			SAFE_DELETE(pRMsg);
			return;
		}
	}

	// start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ��¥�� ������ �α����� ��¥	
	// ���� Ÿ�� �� ���� ���ο��� ���� �� ó���Ѵ�.	
	ATUM_DATE_TIME	CheckDateTime;
	CheckDateTime.SetCurrentDateTime();

	if(CheckDateTime.Year == tmSqlTime3.year) //�⵵�� ���ƾ��ϰ�.
	{
		if(CheckDateTime.Month == tmSqlTime3.month) //�޵� ���ƾ� �Ѵ�.
		{
			if(CheckDateTime.Day != tmSqlTime3.day) //��¥�� ���� ������ ��ƾ�� ó������ �ʾƾ��Ѵ�.
			{
				SQLRETURN retEv = 0;
				SQLINTEGER tmpNum = 1;
				SQLINTEGER tmpnum1= 0;
				SQLINTEGER AccountUniqueNumber = accInfo.AccountUniqueNumber;
				SQLINTEGER	arrcb1[3] = {SQL_NTS,SQL_NTS,SQL_NTS}; 

				retEv = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG,	 SQL_INTEGER, 0,							0,	&tmpNum,				0, &arrcb1[0]); // ����� ����Ʈ�� �����
				retEv = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG,	 SQL_INTEGER, 0,							0,	&AccountUniqueNumber,	0, &arrcb1[1]);
				retEv = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG,	 SQL_INTEGER, 0,							0,	&tmpnum1,				0, &arrcb1[2]); // ó�� ������ ������ 0���� �Ѵ�.
				
				ret = SQLExecDirect(hstmt, PROCEDURE_LOGINITEMEVENT_110825_0001, SQL_NTS);
				if( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
				{
					ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_InsertLoginItemEvent Failed!\n", TRUE);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					// return; ������ ���� ���ƾ� �Ѵ�. ������ �����ϰ� �ȴ�.
				}
			}
		}
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &accInfo.AccountUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0266, SQL_NTS);
	ret = SQLMoreResults(hstmt);
 	SQLFreeStmt(hstmt, SQL_CLOSE);

	((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_NO_ERROR);
	SAFE_DELETE(pRMsg);

// 2006-06-02 by cmkwon
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_PreServerLogin
// 		@accname	varchar(20),
// 		@passwd		varchar(35)
// 	AS
// 		-- ������ �α��� �ð� ����
// 		UPDATE Account
// 		SET LastLoginDate = GetDate()
// 		WHERE AccountName = @accname
// 
// 		SELECT AccountUniqueNumber, AccountType
// 		FROM td_account
// 		WHERE AccountName = @accname AND Password = @passwd
// 	GO
// 	**************************************************************************/
// 	_strlwr(pRMsg->AccountName);
// 	char szMD5String[SIZE_MAX_PASSWORD_MD5_STRING];
// 	RETCODE ret;
// 	SQLINTEGER arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
// 	MD5::MD5Binary2String(pRMsg->Password, szMD5String);
// 	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &arrCB[1]);
// 	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, szMD5String, 0, &arrCB[2]);
// 
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_PreServerLogin(?, ?)}", SQL_NTS);
// 	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_PreServerLogin Failed!\r\n", TRUE);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		q.pIOCPSocket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_DB_EXECUTION_FAILED);
// 		SAFE_DELETE(pRMsg);
// 		return;
// 	}
// 
// 	if (ret == SQL_NO_DATA)
// 	{// ���� ���� (���̵� ����)
// 
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, 0, 0, ERR_COMMON_LOGIN_FAILED);
// 		SAFE_DELETE(pRMsg);
// 		return;
// 	}
// 
// 	// ù �� ° '������ �α��� �ð� ����' �κ� �ѱ�
// 	ret = SQLMoreResults(hstmt);
// 
// 	// bind columns
// 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &nAccountUniqueNumber, 0, NULL);
// 	SQLBindCol(hstmt, 2, SQL_C_USHORT, &nAccountType, 0, NULL);
// 
// 	// AccountUniqueNumber, AccountType ��ȯ
// 	ret = SQLFetch(hstmt);
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 
// 	// ���ڵ����� ���� ��츦 ���
// 	if (ret == SQL_NO_DATA)
// 	{
// 		char szPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];
// 		char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];
// 		BYTE byPass[SIZE_MAX_PASSWORD_MD5];
// 
// 		char pTmpSql[1024];
// 		_strlwr(pRMsg->AccountName);
// 		sprintf(pTmpSql, "SELECT AccountUniqueNumber, AccountType, Password \
// 							FROM td_Account	 \
// 							WHERE AccountName = \'%s\'", pRMsg->AccountName);
// 
// 		ret = SQLExecDirect(hstmt, (UCHAR*)pTmpSql, SQL_NTS);
// 
// 		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
// 			ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_PreServerLogin Failed!\r\n", TRUE);
// 			SQLFreeStmt(hstmt, SQL_CLOSE);
// 			q.pIOCPSocket->SendErrorMessage(T_PRE_DB, ERR_DB_EXECUTION_FAILED);
// 			SAFE_DELETE(pRMsg);
// 			return;
// 		}
// 
// 		SQLBindCol(hstmt, 1, SQL_C_ULONG, &nAccountUniqueNumber, 0, NULL);
// 		SQLBindCol(hstmt, 2, SQL_C_USHORT, &nAccountType, 0, NULL);
// 		SQLBindCol(hstmt, 3, SQL_C_CHAR, szPasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &arrCB[3]);
// 
// 		ret = SQLFetch(hstmt);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 
// 		MD5 md5_instance;
// 
// 		char szTmPassword[1024];			// 2006-05-22 by cmkwon, ����� �߰� ��Ʈ�� ����
// 		MEMSET_ZERO(szTmPassword, 1024);
// 		wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, szPasswordFromDB);
// 		md5_instance.MD5Encode(szTmPassword, byPass);
// 		MD5::MD5Binary2String(byPass, szEncodedString);
// 
// 		if (strcmp(szEncodedString, szMD5String) == 0)
// 		{
// 			ret = SQL_SUCCESS;
// 		}
// 		else
// 		{
// 			ret = SQL_NO_DATA;
// 		}
// 	}
// 
// 	if (ret != SQL_NO_DATA)
// 	{
// 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, nAccountUniqueNumber, nAccountType, ERR_NO_ERROR);
// 	}
// 	else
// 	{
// 		// ���� ���� (���̵� ���ų�, �н����尡 Ʋ��)
// 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, 0, 0, ERR_COMMON_LOGIN_FAILED);
// 	}
//	SAFE_DELETE(pRMsg);
}


// 2005-06-02 by cmkwon, void CPreIOCP::LoadBlockedAccounts()���� ó��
//// QT_LoadBlockedAccount
//void CAtumPreDBManager::QP_LoadBlockedAccount(DB_QUERY q, SQLHSTMT hstmt)
//{
//	// lock
//	m_pPreIOCP2->m_setBlockedAccounts.lock();
//
//	BOOL	ret;
//	char	szBlockedAccount[SIZE_MAX_ACCOUNT_NAME];
//	SDWORD	cb1;
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"SELECT * FROM td_BlockedAccounts", SQL_NTS);
//
//	cb1 = SQL_NTS;
//	SQLBindCol(hstmt, 1, SQL_C_CHAR, szBlockedAccount, SIZE_MAX_ACCOUNT_NAME, &cb1);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
//	{
//		// unlock
//		m_pPreIOCP2->m_setBlockedAccounts.unlock();
//		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"QP_LoadBlockedAccount() Failed!\n", TRUE);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	// initialize to 0x00
//	memset(szBlockedAccount, 0x00, SIZE_MAX_ACCOUNT_NAME);
//	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
//	{
//		m_pPreIOCP2->m_setBlockedAccounts.insertLock(szBlockedAccount);
//
//		// initialize to 0x00
//		memset(szBlockedAccount, 0x00, SIZE_MAX_ACCOUNT_NAME);
//	}
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	// unlock
//	m_pPreIOCP2->m_setBlockedAccounts.unlock();
//}

// QT_BlockUser
void CAtumPreDBManager::QP_BlockUser(DB_QUERY q, SQLHSTMT hstmt)
{
	SBLOCKED_ACCOUNT_INFO *pBlockedAccInfo = (SBLOCKED_ACCOUNT_INFO*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	-- 2005-06-02 by cmkwon
	-- ���� ���� ����Ʈ�� �߰� Ȥ�� ���� Update
	--			2007-01-10 by cmkwon, td_BlockedAccounts ���̺��� BlockedReasonForOnlyAdmin �ʵ� �߰���
	CREATE PROCEDURE atum_Insert_BlockedAccounts
		@i_AccountName			VARCHAR(20),
		@i_BlockedType			INT,
		@i_StartDate			VARCHAR(30),
		@i_EndDate				VARCHAR(30),
		@i_AdminAccountName		VARCHAR(20),
		@i_BlockedReason		VARCHAR(200),			-- ������ ���� ���� ����
		@i_BlockedReasonForOnlyAdmin	VARCHAR(200)	-- 2007-01-10 by cmkwon, �߰��� - �����ڰ� ���� ��������
	AS	
		DECLARE @blockedTy INT
		SET @blockedTy = (SELECT BlockedType FROM td_BlockedAccounts WHERE AccountName = @i_AccountName)
		
		IF (@blockedTy IS NULL)
			BEGIN
				INSERT INTO td_BlockedAccounts
					VALUES (@i_AccountName, @i_BlockedType, @i_StartDate, @i_EndDate, @i_AdminAccountName, @i_BlockedReason, @i_BlockedReasonForOnlyAdmin)
			END
		ELSE
			BEGIN
				UPDATE td_BlockedAccounts
					SET BlockedType = @i_BlockedType, StartDate = @i_StartDate
							, EndDate = @i_EndDate, AdminAccountName = @i_AdminAccountName, BlockedReason = @i_BlockedReason, BlockedReasonForOnlyAdmin = @i_BlockedReasonForOnlyAdmin
						WHERE AccountName = @i_AccountName
			END	
	GO
	**************************************************************************/

	SQLINTEGER arrCB[8] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockedAccountName, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pBlockedAccInfo->enBlockedType, 0, &arrCB[2]);
	char tmpStartTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pBlockedAccInfo->atimeStartTime.GetSQLDateTimeString(tmpStartTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpStartTimeBuf, 0, &arrCB[3]);
	char tmpEndTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pBlockedAccInfo->atimeEndTime.GetSQLDateTimeString(tmpEndTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpEndTimeBuf, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockAdminAccountName, 0, &arrCB[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, pBlockedAccInfo->szBlockedReasonForUser, 0, &arrCB[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, pBlockedAccInfo->szBlockedReasonForOnlyAdmin, 0, &arrCB[7]);		// 2007-01-10 by cmkwon

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0267, SQL_NTS);

	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_BlockUser() Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pBlockedAccInfo);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - �α׸� �߰��Ѵ�.
	if(FALSE == this->LogInsertBlockUnblock(hstmt, pBlockedAccInfo, T1_FL_LOG_BLOCK_ACCOUNT))
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[Error] Insert log error!!, AccountName(%s) BlockType(%d) Period(%s~%s) AdminAccName(%s) ReasonForUser(%s) ReasonForAdmin(%s)\r\n"
			, pBlockedAccInfo->szBlockedAccountName, pBlockedAccInfo->enBlockedType, pBlockedAccInfo->atimeStartTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
			, pBlockedAccInfo->atimeEndTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), pBlockedAccInfo->szBlockAdminAccountName
			, pBlockedAccInfo->szBlockedReasonForUser, pBlockedAccInfo->szBlockedReasonForOnlyAdmin);
	}

	// 2008-04-04 by cmkwon, �� �޽����� ������ ����
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_BLOCK_ACCOUNT_OK, T_PA_ADMIN_BLOCK_ACCOUNT_OK, msgBlockOK, msgBlockOKBuf);
	*msgBlockOK = *pBlockedAccInfo;
	m_pPreIOCP2->SendMessageToAdminTool(msgBlockOKBuf, MSG_SIZE(MSG_PA_ADMIN_BLOCK_ACCOUNT_OK));

	// Notify
	if (q.pIOCPSocket
		&& q.pIOCPSocket->IsUsing())
	{
		// 2008-01-31 by cmkwon, ���� ����/���� ���ɾ�� ������ �ý��� ���� - �Ʒ��� ���� SocketType ���� ó��
		if(ST_FIELD_SERVER == q.pIOCPSocket->m_PeerSocketType)
		{
			// 2008-01-31 by cmkwon, ���� ����/���� ���ɾ�� ������ �ý��� ���� - FieldServer �� ��� ����
			INIT_MSG_WITH_BUFFER(MSG_FP_ADMIN_BLOCKACCOUNT_OK, T_FP_ADMIN_BLOCKACCOUNT_OK, pSMsgOK, SendBuf);
			pSMsgOK->ErrCode	= ERR_NO_ERROR;
			STRNCPY_MEMSET(pSMsgOK->BlockedAccName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->AdminFieldServerClientIndex	= pBlockedAccInfo->AdminFieldServerClientIndex;
			STRNCPY_MEMSET(pSMsgOK->AdminAccName, pBlockedAccInfo->szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->atimeEndTime					= pBlockedAccInfo->atimeEndTime;
			q.pIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FP_ADMIN_BLOCKACCOUNT_OK));
		}
	}
	SAFE_DELETE(pBlockedAccInfo);
}

// QT_UnblockUser
void CAtumPreDBManager::QP_UnblockUser(DB_QUERY q, SQLHSTMT hstmt)
{
// 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - �Ʒ��� ���� ����
// 	char *szAccountName = (char*)q.pQueryParam;
	SBLOCKED_ACCOUNT_INFO *pBlockedAccInfo = (SBLOCKED_ACCOUNT_INFO*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	-- ���� ���� ����Ʈ���� ����
	CREATE PROCEDURE atum_Delete_BlockedAccounts
		@i_AccountName			VARCHAR(20)
	AS
		DELETE FROM td_BlockedAccounts
			WHERE AccountName = @i_AccountName	
	GO
	**************************************************************************/

	SQLINTEGER cb = SQL_NTS;
	// 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - �Ʒ��� ���� ����
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, szAccountName, 0, &cb);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockedAccountName, 0, &cb);

	// 2008-01-29 by cmkwon, T_A: ���� ���� ���� ���� ���� - ������� �ʴ� �ڵ���
	//char szQuery[512];
	//sprintf(szQuery, "DELETE FROM td_BlockedAccounts WHERE AccountName LIKE \'%s\'", szAccountName);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0268, SQL_NTS);

	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_UnblockUser() Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pBlockedAccInfo);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - �α׸� �߰��Ѵ�.
	if(FALSE == this->LogInsertBlockUnblock(hstmt, pBlockedAccInfo, T1_FL_LOG_UNBLOCK_ACCOUNT))
	{
		g_pPreGlobal->WriteSystemLogEX(TRUE, "[Error] Insert log error!!, AccountName(%s) BlockType(%d) Period(%s~%s) AdminAccName(%s) ReasonForUser(%s) ReasonForAdmin(%s)\r\n"
			, pBlockedAccInfo->szBlockedAccountName, pBlockedAccInfo->enBlockedType, pBlockedAccInfo->atimeStartTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
			, pBlockedAccInfo->atimeEndTime.GetDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), pBlockedAccInfo->szBlockAdminAccountName
			, pBlockedAccInfo->szBlockedReasonForUser, pBlockedAccInfo->szBlockedReasonForOnlyAdmin);
	}

	// 2008-04-04 by cmkwon, �� �޽����� ������ ���� �ؾ� �Ѵ�.
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK, T_PA_ADMIN_UNBLOCK_ACCOUNT_OK, msgUnblockOK, msgUnblockOKBuf);
	STRNCPY_MEMSET(msgUnblockOK->AccountName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
	m_pPreIOCP2->SendMessageToAdminTool(msgUnblockOKBuf, MSG_SIZE(MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK));
	
	// Notify
	if (q.pIOCPSocket
		&& q.pIOCPSocket->IsUsing())
	{
		// 2008-01-31 by cmkwon, ���� ����/���� ���ɾ�� ������ �ý��� ���� - �Ʒ��� ���� SocketType ���� ó��
		if(ST_FIELD_SERVER == q.pIOCPSocket->m_PeerSocketType)
		{
			// 2008-01-31 by cmkwon, ���� ����/���� ���ɾ�� ������ �ý��� ���� - FieldServer �� ��� ����
			INIT_MSG_WITH_BUFFER(MSG_FP_ADMIN_UNBLOCKACCOUNT_OK, T_FP_ADMIN_UNBLOCKACCOUNT_OK, pSMsgOK, SendBuf);
			pSMsgOK->ErrCode	= ERR_NO_ERROR;
			STRNCPY_MEMSET(pSMsgOK->UnblockedAccName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->AdminFieldServerClientIndex	= pBlockedAccInfo->AdminFieldServerClientIndex;
			STRNCPY_MEMSET(pSMsgOK->AdminAccName, pBlockedAccInfo->szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME);
			q.pIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FP_ADMIN_UNBLOCKACCOUNT_OK));
		}
	}
	SAFE_DELETE(pBlockedAccInfo);
}

// QT_MGameLogin
void CAtumPreDBManager::QP_MGameLogin(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_PC_CONNECT_LOGIN	*pRMsg = (MSG_PC_CONNECT_LOGIN*)q.pQueryParam;
	CAccountInfo			accInfo;
	SQLHSTMT				hstmt_ext = 0;
	ExternalAuthentication(&accInfo, pRMsg, hstmt, hstmt_ext, FALSE);
	if(FALSE == IS_VALID_UNIQUE_NUMBER(accInfo.AccountUniqueNumber))
	{
		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_COMMON_LOGIN_FAILED);
	}
	else
	{
		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_NO_ERROR);
	}
	
	SAFE_DELETE(pRMsg);
	return;

// 2006-06-02 by cmkwon
// 	UID32_t					nAccountUniqueNumber = INVALID_UNIQUE_NUMBER;
// 	USHORT					nAccountType = 0;
// 	int						nMGameEventType = 0;
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// MGameEvent ó��
// 	if(MGAME_EVENT_OPEN_BETA_ATTENDANCE == g_pPreGlobal->m_enMGameEventType
// 		&& g_pPreGlobal->IsExistAccountNameForMGameEvent(pRMsg->AccountName))
// 	{
// 		nMGameEventType = g_pPreGlobal->m_enMGameEventType;
// 	}
// 
// 	RETCODE ret;
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_PreServerMGameLogin
// 		@i_AccountName				VARCHAR(20),
// 		@i_AccountNameForSearch		VARCHAR(64),
// 		@i_Sex						TINYINT,			-- 1: ��, 2: ��
// 		@i_BirthYear				INT,				-- ����
// 		@i_MGameEventType			INT,
// 		@o_AccountType				SMALLINT	OUTPUT
// 	AS
// 		DECLARE @AccountUniqueNumber INT
// 		
// 		SET @AccountUniqueNumber = (
// 			SELECT AccountUniqueNumber
// 			FROM td_account
// 			WHERE AccountName LIKE @i_AccountName
// 		)
// 
// 		-- ���� �����ڴ� ���� �߰�
// 		IF (@AccountUniqueNumber IS NULL)
// 			BEGIN
// 				INSERT INTO td_Account(AccountName, Sex, BirthYear)
// 				VALUES(@i_AccountName, @i_Sex, @i_BirthYear)
// 			END
// 
// 		-- ������ �α��� �ð� ����
// 		UPDATE td_Account
// 		SET LastLoginDate = GetDate()
// 		WHERE AccountName LIKE @i_AccountName
// 
// 		SET @AccountUniqueNumber = (
// 			SELECT AccountUniqueNumber
// 			FROM td_account
// 			WHERE AccountName LIKE @i_AccountName
// 		)
// 		
// 		SET @o_AccountType = (
// 			SELECT AccountType
// 			FROM td_Account
// 			WHERE AccountUniqueNumber = @AccountUniqueNumber
// 		)
// 
// 		RETURN @AccountUniqueNumber
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nAccountUniqueNumber, 0, NULL);
// 	SDWORD pcbNTS = SQL_NTS;
// 	_strlwr(pRMsg->AccountName);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &pcbNTS);
// 
// 	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER
// 		, 0, GetSqlPattern(pRMsg->AccountName, spbuf), 0, &pcbNTS);	
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->MGameSEX, 0, NULL);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->MGameYear, 0, NULL);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMGameEventType, 0, NULL);
// 	SQLBindParameter(hstmt, 7, SQL_PARAM_OUTPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &nAccountType, 0, NULL);
// 
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call dbo.atum_PreServerMGameLogin(?, ?, ?, ?, ?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
// 	{
// 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_PreServerMGameLogin Failed!\r\n", TRUE);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		q.pIOCPSocket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_DB_EXECUTION_FAILED);
// 		SAFE_DELETE(pRMsg);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
// 
// 	if (FALSE == IS_VALID_UNIQUE_NUMBER(nAccountUniqueNumber))
// 	{
// 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, nAccountUniqueNumber, nAccountType, ERR_NO_ERROR);
// 	}
// 	else
// 	{
// 		// ���� ���� (���̵� ���ų�, �н����尡 Ʋ��)
// 		((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, 0, 0, ERR_COMMON_LOGIN_FAILED);
// 	}
// 
// 	SAFE_DELETE(pRMsg);
}


void CAtumPreDBManager::QP_LogTotalUser(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_DB_TOTAL_USER	*pRMsg = (MSG_DB_TOTAL_USER*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};		// 2010-11 by dhjin, �ƶ󸮿� ä�θ� �α���.

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_total_user
		@i_ServerGroupName			VARCHAR(20),	
		@i_MGameServerID			INT,
		@i_UserCount				INT,
		@i_nUserCountsOtherPublisherConncect				INT
	AS
		INSERT INTO atum_log_total_user
		VALUES (GetDate(), @i_ServerGroupName, @i_MGameServerID, @i_UserCount, @i_nUserCountsOtherPublisherConncect)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0,
		pRMsg->szServerGroup, 0, &arrCB[0]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nMGameServerID, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nUserCounts, 0, &arrCB[2]);

#ifdef S_ARARIO_HSSON
	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nUserCountsOtherPublisherConncect, 0, &arrCB[3]);		// 2010-11 by dhjin, �ƶ󸮿� ä�θ� �α���.
#else
	int nUserCountsOtherPublisherConncect = 0;
	SQLBindParameter(hstmt, 4,SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nUserCountsOtherPublisherConncect, 0, &arrCB[3]);		// 2010-11 by dhjin, �ƶ󸮿� ä�θ� �α���.
#endif

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0269, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_log_insert_total_user Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

// 2010-06-01 by shcho, GLogDB ���� -
void CAtumPreDBManager::QP_InsertGlogAccount(DB_QUERY q, SQLHSTMT hstmt)
{// QT_InsertGlogAccount
	QPARAM_GLOG_INSERT_ACCOUNT	*pRMsg = (QPARAM_GLOG_INSERT_ACCOUNT*)q.pQueryParam;
	
	RETCODE ret;
	SQLINTEGER arrCB[4] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};

	/*[Stored Query Definition]************************************************
	ALTER PROCEDURE dbo.atum_InsertGlogAccount
		@i_AccountName				VARCHAR(20),
		@i_GamePublisher			NVARCHAR(128),		-- ���Ӽ��񽺻� �̸�
		@i_GameServerIP				NVARCHAR(64)			-- ���Ӽ���IP (�������� ǥ�ø� ����(ex:�ѱ�,�̱����))
	AS
		--AccountUID �޾ƿ��� �ۺ����ſ� �̸��� �˻��Ѵ�.
		DECLARE @AccountName	 	NVARCHAR(128)
		SET @AccountName = (SELECT AccountName FROM GLog.dbo.GLog_Account WITH (NOLOCK) WHERE AccountName = @i_AccountName and GamePublisher = @i_GamePublisher)
	
	  
		-- ���� �����ڴ� ���� �߰�
		IF (@AccountName IS NULL)
		BEGIN
			--// 2010-06-01 by shcho, GLogDB ���� - ���񽺻� �̸�, IP�� �޾ƿ;� �Ѵ�.	
			INSERT GLog.dbo.GLog_Account (AccountName, GamePublisher, GameName, GameServerIP, Birthday, Sex, RegDate ) 
			SELECT @i_AccountName, @i_GamePublisher, 'AceOnline', @i_GameServerIP,  CAST(BirthYear AS NVARCHAR(64)), Sex-1, RegisteredDate
			FROM atum2_db_account.dbo.td_account
			WHERE AccountName = @i_AccountName
		END
			  
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0,pRMsg->szAccountName, 0, &arrCB[0]);
	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,	128, 0, g_pPreGlobal->GetGamePublisherName(), 0, &arrCB[1]);
	SQLBindParameter(hstmt, 3,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,	64,	0, g_pPreGlobal->GetPublicIPLocal(), 0, &arrCB[2]);

	ret = SQLExecDirect(hstmt, PROCEDURE_GLOG_100601_0006, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@QP_InsertGlogAccount Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}

// start 2011-12-12 by hskim, GLog 2��
BOOL CAtumPreDBManager::IsExistAccount(char *pAccountName, SQLHSTMT &hstmt)
{
	CAccountInfo AccountInfo;
	SDWORD pcbNTS = SQL_NTS;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pAccountName, 0, NULL);			

	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0258, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"CAtumPreDBManager::IsExistAccount PROCEDURE_080827_0258 Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &AccountInfo.AccountUniqueNumber, 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_USHORT, &AccountInfo.AccountType, 0, NULL);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, AccountInfo.PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &pcbNTS);

	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(FALSE == IS_VALID_UNIQUE_NUMBER(AccountInfo.AccountUniqueNumber))
	{
		return FALSE;
	}

	return TRUE;
}
// end 2011-12-12 by hskim, GLog 2��

// start 2012-01-13 by hskim, EP4 [������ 1ȸ ���]
void CAtumPreDBManager::QP_UpdateAccountLastGameEndDate(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE *pRMsg = (QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE*)q.pQueryParam;
	
	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROC [dbo].[atum_Update_AccountLastGameEndDate]
		@i_AccountUniqueNumber		INT
	AS
	-- YD ���� �ڵ�
	BEGIN
		UPDATE [dbo].[td_Account] SET [LastGameEndDate] = getdate() WHERE AccountUniqueNumber = @i_AccountUniqueNumber
	END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_111222_0002, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"QP_UpdateAccountLastGameEndDate Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		SAFE_DELETE(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	SAFE_DELETE(pRMsg);
}
// end 2012-01-13 by hskim, EP4 [������ 1ȸ ���]

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, BYTE i_byLogType)
/// \brief		// 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - CAtumPreDBManager::LogInsertBlockUnblock() �߰�
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, BYTE i_byLogType)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- Name: atum_log_insert_block_unblock
	-- Desc: // 2008-01-30 by cmkwon, ���� ���� �α� ����� ���� - ���� ����/����/���� ���� �α� �߰��ϱ�
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_insert_block_unblock
		@i_LogType				TINYINT,
		@i_AccountName			VARCHAR(20),
		@i_BlockedType			INT,
		@i_StartDate			VARCHAR(30),
		@i_EndDate				VARCHAR(30),
		@i_AdminAccountName		VARCHAR(20),
		@i_BlockedReason		VARCHAR(200),			-- ������ ���� ���� ����
		@i_BlockedReasonForOnlyAdmin	VARCHAR(200)	-- 2007-01-10 by cmkwon, �߰��� - �����ڰ� ���� ��������
	AS
		INSERT INTO dbo.atum_log_blockedAccount
		VALUES (@i_LogType, GetDate(), @i_AccountName, @i_BlockedType, @i_StartDate, @i_EndDate, @i_AdminAccountName, @i_BlockedReason, @i_BlockedReasonForOnlyAdmin)
	GO
	**************************************************************************/

	SQLINTEGER arrCB[9] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char tmpStartTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	char tmpEndTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	i_pBlockAccInfo->atimeStartTime.GetSQLDateTimeString(tmpStartTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	i_pBlockAccInfo->atimeEndTime.GetSQLDateTimeString(tmpEndTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byLogType, 0,													&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pBlockAccInfo->szBlockedAccountName, 0,		&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pBlockAccInfo->enBlockedType, 0,									&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpStartTimeBuf, 0,						&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpEndTimeBuf, 0,							&arrCB[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pBlockAccInfo->szBlockAdminAccountName, 0,		&arrCB[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, i_pBlockAccInfo->szBlockedReasonForUser, 0,		&arrCB[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, i_pBlockAccInfo->szBlockedReasonForOnlyAdmin, 0,	&arrCB[8]);		// 2007-01-10 by cmkwon

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0270, SQL_NTS);
	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_log_insert_block_unblock_ Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ExternalAuthentication(CAccountInfo *o_pAccInfo, MSG_PC_CONNECT_LOGIN *i_pLogin, SQLHSTMT hstmt, SQLHSTMT hstmt_extAuth, BOOL i_bAuthenticateCheck/*=TRUE*/)
/// \brief		
///				// 2010-04-26 by cmkwon, ���þ� Innva ����/���� �ý��� ���� - 
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ExternalAuthentication(CAccountInfo *o_pAccInfo, MSG_PC_CONNECT_LOGIN *i_pLogin, SQLHSTMT &hstmt, SQLHSTMT &hstmt_extAuth, BOOL i_bAuthenticateCheck/*=TRUE*/, UINT i_nClientIP/*=0*/)
{
	// 2008-04-25 by cmkwon, ���� ���/���� �߰��� �� �߰� �Ǿ�� �ϴ� ���� - [���-�ɼ�] �߰� ���ÿ� �ܺ� ���� ó�� ��ƾ�� �߰�

	char szJuminNum[SIZE_MAX_JUMIN_NUMBER]={NULL,};		// 2007-06-29 by cmkwon, td_account ���̺��� �ֹι�ȣ�����ϱ�
	if(i_bAuthenticateCheck)
	{
		char szMD5String[SIZE_MAX_PASSWORD_MD5_STRING];
		MD5::MD5Binary2String(i_pLogin->Password, szMD5String);
		BOOL bExtAuth = FALSE;
		switch(g_pPreGlobal->GetLanguageType())
		{
		case LANGUAGE_TYPE_DEFAULT:
		case LANGUAGE_TYPE_ENGLISH:
			{
				bExtAuth = this->ExecuteExtAuth2(i_pLogin->AccountName, szMD5String, i_pLogin->WebLoginAuthKey, hstmt_extAuth);
			}
			break;
		default:
			{
				bExtAuth = this->ExecuteExtAuth(i_pLogin->AccountName, szMD5String, hstmt_extAuth);
			}
		}
		
		if(FALSE == bExtAuth)
		{// 2006-04-27 by cmkwon, �ܺ� �������� ������

			/*if(FALSE == IS_MASANG_IP(i_pLogin->PrivateIP))
			{// 2006-04-28 by cmkwon, �������Ʈ �系 IP�� �ƴϸ� ���� ����
				return FALSE;
			}*/

			// 2007-10-08 by cmkwon
			g_pPreGlobal->WriteSystemLogEX(TRUE, "[Notify] MasangIP authentication !!, AccountName(%s) WebAuthKey(%s) IP(%s)\r\n"
													, i_pLogin->AccountName, i_pLogin->WebLoginAuthKey, i_pLogin->PrivateIP);

//			char szQuery[1024];
			SDWORD pcbNTS = SQL_NTS;

			// 2008-07-02 by cmkwon, MySQL ���� ���� - 
			//wsprintf(szQuery, "SELECT AccountUniqueNumber, AccountType, Password FROM td_Account WITH (NOLOCK) \
			//					WHERE AccountName = \'%s\'", i_pLogin->AccountName);
// 2008-08-27 by cmkwon, �������� Procedure�� ���� - 
//			wsprintf(szQuery, QUERY_080702_0258, i_pLogin->AccountName);
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pLogin->AccountName, 0, NULL);			
			RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0258, SQL_NTS);
			if ( ret!=SQL_SUCCESS
				&& ret!=SQL_SUCCESS_WITH_INFO )
			{
				ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"CAtumPreDBManager::ExternalAuthentication_ Failed!\r\n", TRUE);
				SQLFreeStmt(hstmt, SQL_CLOSE);
				return FALSE;
			}
			SQLBindCol(hstmt, 1, SQL_C_ULONG, &o_pAccInfo->AccountUniqueNumber, 0, NULL);
			SQLBindCol(hstmt, 2, SQL_C_USHORT, &o_pAccInfo->AccountType, 0, NULL);
			SQLBindCol(hstmt, 3, SQL_C_CHAR, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &pcbNTS);
			ret = SQLFetch(hstmt);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			if(0 == strncmp(o_pAccInfo->PasswordFromDB, "", SIZE_MAX_PASSWORD_MD5_STRING))
			{// 2007-10-08 by cmkwon, DB �н����忡 ������ ���� ���� - ���� �Ұ�
				return FALSE;
			}

			if(FALSE == IS_VALID_UNIQUE_NUMBER(o_pAccInfo->AccountUniqueNumber))
			{// 2006-04-28 by cmkwon, ������ ����
				return FALSE;
			}
			if(0 != strnicmp(szMD5String, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING))
			{// 2006-04-28 by cmkwon, MD5 �н����� ��

				char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];		MEMSET_ZERO(szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);
				BYTE byPass[SIZE_MAX_PASSWORD_MD5];
				MD5 md5_instance;

				char szTmPassword[1024];			// 2006-05-22 by cmkwon, ����� �߰� ��Ʈ�� ����
				MEMSET_ZERO(szTmPassword, 1024);
				wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, o_pAccInfo->PasswordFromDB);
				md5_instance.MD5Encode(szTmPassword, byPass);
				MD5::MD5Binary2String(byPass, szEncodedString);

				if (0 != strnicmp(szEncodedString, szMD5String, SIZE_MAX_PASSWORD_MD5_STRING))
				{
					return FALSE;
				}
			}
		}
	}

	int						nMGameEventType = 0;
	///////////////////////////////////////////////////////////////////////////////
	// MGameEvent ó��
	if(MGAME_EVENT_OPEN_BETA_ATTENDANCE == g_pPreGlobal->m_enMGameEventType
		&& g_pPreGlobal->IsExistAccountNameForMGameEvent(i_pLogin->AccountName))
	{
		nMGameEventType = g_pPreGlobal->m_enMGameEventType;
	}

	// start 2011-12-12 by hskim, GLog 2��
#ifdef S_GLOG_2ND_HSKIM		// ON/OFF ��� ����

	if( FALSE == IsExistAccount(i_pLogin->AccountName, hstmt) )
	{
		if( NULL != m_pPreIOCP2 ) 
		{
			CServerGroup *ServerGroup = m_pPreIOCP2->GetServerGroup(i_pLogin->FieldServerGroupName);

			if( ServerGroup != NULL )
			{
				ServerInfo *ServerInfo = &ServerGroup->m_FieldServerInfo;

				if( TRUE == ServerInfo->IsActive )
				{
					INIT_MSG_WITH_BUFFER(MSG_FP_GLOG_CONNECT_USER_NEW, T_FP_GLOG_CONNECT_USER_NEW, pSendGLogMsg, pSendGLogBuf);
					STRNCPY_MEMSET(pSendGLogMsg->AccountName, i_pLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
					ServerGroup->SendMessageToFieldServer(pSendGLogBuf, MSG_SIZE(MSG_FP_GLOG_CONNECT_USER_NEW));
				}
			}
		}
	}
#endif
	// end 2011-12-12 by hskim, GLog 2��

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_PreServerExtAuthLogin
	-- Desc: 2006-04-27 by cmkwon, �߰���
	--		   �ܺ� DB ���� �α��� ó�� - ��� üũ�� ����, ������ ������ ���� �߰�, AccountUniqueNumber�� �����Ѵ�.
	--		2006-11-15 by cmkwon, Select �˻� �ʵ� �߰�(td_account���̺��� �߰��� �ʵ� - GameContinueTimeInSecondOfToday, LastGameEndDate)
	--		2007-06-29 by cmkwon, td_account ���̺��� �ֹι�ȣ�����ϱ� - �ֹι�ȣ ���� �߰�
	--		// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - atum_PreServerExtAuthLogin ���ν��� ����
	--====
	CREATE PROCEDURE dbo.atum_PreServerExtAuthLogin
		@i_AccountName				VARCHAR(20),
		@i_Sex						TINYINT,			-- 1: ��, 2: ��
		@i_BirthYear				INT,				-- ����
		@i_MGameEventType			INT,
		@i_JuminNumber				VARCHAR(20)			-- 2007-06-29 by cmkwon, td_account ���̺��� �ֹι�ȣ�����ϱ� - �߰��� ����
	AS
		DECLARE @AccountUID INT
		SET @AccountUID = (SELECT AccountUniqueNumber FROM td_Account WITH (NOLOCK) WHERE AccountName = @i_AccountName)

		-- ���� �����ڴ� ���� �߰�
		IF (@AccountUID IS NULL)
		BEGIN
			INSERT INTO td_Account(AccountName, Sex, BirthYear, MGameEventType, JuminNumber)
				VALUES(@i_AccountName, @i_Sex, @i_BirthYear, @i_MGameEventType, @i_JuminNumber)
		END
		ELSE
		BEGIN
			-- 2007-03-30 by cmkwon
			-- SQLMoreResults()�� ������ ���߱� ���ؼ� �߰��� �ڵ���
			UPDATE td_Account
				SET Sex = Sex
				WHERE AccountName = @i_AccountName
		END
		
		-- ������ �α��� �ð� ����
		UPDATE td_Account
			SET LastLoginDate = GetDate()
			WHERE AccountName = @i_AccountName	
		
		-- // 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - SecondaryPassword �������� �߰�
		SELECT AccountUniqueNumber, AccountType, Password, RegisteredDate, GameContinueTimeInSecondOfToday, LastGameEndDate, SecondaryPassword
			FROM td_Account WHERE AccountName = @i_AccountName
	GO
	**************************************************************************/
	SQLINTEGER arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pLogin->AccountName, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_pLogin->MGameSEX, 0,						&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pLogin->MGameYear, 0,							&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMGameEventType, 0,								&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_JUMIN_NUMBER, 0, szJuminNum, 0,				&arrCB[5]);	// 2007-06-29 by cmkwon, td_account ���̺��� �ֹι�ȣ�����ϱ�

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0271, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_PreServerExtAuthLogin Failed!\n\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}

	arrCB[0]= arrCB[1]= arrCB[2]= arrCB[3]= arrCB[4]= arrCB[5]= arrCB[6]= arrCB[7]= SQL_NTS;
	char szPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	MEMSET_ZERO(szPasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING);
	SQL_TIMESTAMP_STRUCT		tmSqlTime;		MEMSET_ZERO(&tmSqlTime, sizeof(tmSqlTime));
	SQL_TIMESTAMP_STRUCT		tmSqlTime2;		MEMSET_ZERO(&tmSqlTime2, sizeof(tmSqlTime2));
 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &o_pAccInfo->AccountUniqueNumber, 0,						&arrCB[1]);
 	SQLBindCol(hstmt, 2, SQL_C_USHORT, &o_pAccInfo->AccountType, 0,								&arrCB[2]); 
	SQLBindCol(hstmt, 3, SQL_C_CHAR, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &tmSqlTime, 0,										&arrCB[4]);
 	SQLBindCol(hstmt, 5, SQL_C_LONG, &o_pAccInfo->GameContinueTimeInSecondOfToday, 0,			&arrCB[5]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &tmSqlTime2, 0,										&arrCB[6]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 7, SQL_C_CHAR, o_pAccInfo->SecondaryPassword, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[7]);	// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - SecondaryPassword �������� �߰�

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - �Ʒ��� ���� ����
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-03-30 by cmkwon
// 	ret = SQLMoreResults(hstmt);		// 2007-03-30 by cmkwon, ���� ���� �߰� ó��
// 	ret = SQLMoreResults(hstmt);		// 2007-03-30 by cmkwon, ������ �α��� �ð� ���� ó��
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-03-30 by cmkwon, ���� ���� ��������
//	ret= SQLFetch(hstmt);
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//	o_pAccInfo->AccountRegisteredDate	= tmSqlTime;
//	o_pAccInfo->LastGameEndDate			= tmSqlTime2;		// 2006-11-15 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		o_pAccInfo->AccountRegisteredDate	= tmSqlTime;
		o_pAccInfo->LastGameEndDate			= tmSqlTime2;		// 2006-11-15 by cmkwon
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return IS_VALID_UNIQUE_NUMBER(o_pAccInfo->AccountUniqueNumber);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth(char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-17 ~ 2006-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ExecuteExtAuth(char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
{
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_ExternalAuthentication
	-- Desc: 2006-04-27 by cmkwon, �߰� ��
	--		   �ܺ� DB ���� ���ν��� - �̰��� ��� ������ ���ο��� �׽�Ʈ �ϱ� ���� �ʿ��� �����
	--		   ������ �� ����� ��� ȸ�翡�� ������ �־�� �Ѵ�.
	--====
	CREATE PROCEDURE atum_ExternalAuthentication
		@i_AccountName				VARCHAR(20),	
		@i_Password					VARCHAR(35)
	AS
		DECLARE @AccountUID INT
		
		SET @AccountUID = (SELECT AccountUniqueNumber 
								FROM td_Account
								WHERE AccountName = @i_AccountName AND Password = @i_Password)
		
		IF (@AccountUID IS NULL)
			BEGIN
					RETURN 0
			END

		RETURN 1
	GO	
	**************************************************************************/
	BOOL		bAuth = FALSE;

	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bAuth, 0,								NULL);
	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
	BOOL ret = SQLExecDirect(hstmt_extAuth, (UCHAR*)"{ ? = call atum_ExternalAuthentication(?, ?)}", SQL_NTS);
	if (ret != SQL_SUCCESS
		&& ret != SQL_SUCCESS_WITH_INFO)
	{
		if(ret != SQL_NO_DATA)
		{// ���� ����
			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@atum_ExternalAuthentication Failed!!\r\n", TRUE);
		}
		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
		return FALSE;
	}

	ret = SQLMoreResults(hstmt_extAuth);
	ret = SQLMoreResults(hstmt_extAuth);
	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
	if(FALSE == bAuth)
	{
		return FALSE;
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth_GalaNet(INT *o_pnExtAccountIDNum, char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-22 ~ 2006-05-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ExecuteExtAuth_GalaNet(INT *o_pnExtAccountIDNum, char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
{
	*o_pnExtAccountIDNum	= 0;		// 2006-05-22 by cmkwon, �ʱ�ȭ

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: G_AUTHENTICATE
	-- Desc: 2006-04-27 by cmkwon, �߰� ��
	--		   Galanet�� 
	--		@o_ExtAccountIDNum = -1 ==> ��ī��Ʈ���ܻ���
	--		@o_ExtAccountIDNum =  0 ==> ������ �������� �ȰŸ� ��й�ȣ�� ���� ������
	--		@o_ExtAccountIDNum =  �� �̿��� �� ==> ���� ����, ������ ���� IDNumber
	--====
	CREATE PROCEDURE G_AUTHENTICATE
		@i_AccountName			VARCHAR(20),	
		@i_Password				VARCHAR(35),
		@i_GameName				VARCHAR(10),
		@o_ExtAccountIDNum		INT				OUTPUT
	AS		
	GO	
	**************************************************************************/
	char *szExtAuthGameName  = EXT_AUTH_GAME_NAME; 
	SQLINTEGER arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, szExtAuthGameName, 0,					NULL);
	SQLBindParameter(hstmt_extAuth, 4, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, o_pnExtAccountIDNum, 0,					NULL);
	BOOL ret = SQLExecDirect(hstmt_extAuth, PROCEDURE_080822_0273, SQL_NTS);
	if (ret != SQL_SUCCESS
		&& ret != SQL_SUCCESS_WITH_INFO)
	{
		if(ret != SQL_NO_DATA)
		{// ���� ����
			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@G_AUTHENTICATE Failed!!\r\n", TRUE);
		}
		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
		return FALSE;
	}

	ret = SQLMoreResults(hstmt_extAuth);
	ret = SQLMoreResults(hstmt_extAuth);
	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
	if(0 == *o_pnExtAccountIDNum
		|| -1 == *o_pnExtAccountIDNum)
	{
		return FALSE;
	}
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth2(char *i_szAccName, char *i_szPwd, char *i_szWebLoginAuthKey, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
/// \brief		
/// \author		cmkwon
/// \date		2007-03-30 ~ 2007-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ExecuteExtAuth2(char *i_szAccName, char *i_szPwd, char *i_szWebLoginAuthKey, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
{
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_ExternalAuthentication2
	-- Desc: 2007-03-30 by cmkwon, �߰� ��
	--		   �ܺ� DB ���� ���ν��� - �̰��� ��� ������ ���ο��� �׽�Ʈ �ϱ� ���� �ʿ��� �����
	--		   ������ �� ����� ��� ȸ�翡�� ������ �־�� �Ѵ�.
	--====
	CREATE PROCEDURE atum_ExternalAuthentication2
		@o_RetCode					INT				OUTPUT,		-- 2007-03-30 by cmkwon
		@i_AccountName				VARCHAR(20),				-- ������
		@i_Password					VARCHAR(35),				-- �н�����
		@i_WebLoginAuthKey			VARCHAR(20)					-- ������Ű
	AS
		SET @o_RetCode		= 0

		DECLARE @AccountUID INT	
		SET @AccountUID = (SELECT AccountUniqueNumber FROM td_Account WITH(NOLOCK) WHERE AccountName = @i_AccountName AND Password = @i_Password)
		
		IF (@AccountUID IS NULL)
		BEGIN
			SET @o_RetCode		= 1
			RETURN
		END

		SET @o_RetCode		= 0
		RETURN
	GO
	**************************************************************************/
	int		nRetCode = 1;		// 2008-01-23 by cmkwon, �⺻���� ���� ���·� �����Ѵ�.
	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetCode, 0,								NULL);
	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
	SQLBindParameter(hstmt_extAuth, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY, 0, i_szWebLoginAuthKey, 0,		NULL);
	BOOL ret = SQLExecDirect(hstmt_extAuth, PROCEDURE_080822_0274, SQL_NTS);
	if (ret != SQL_SUCCESS
		&& ret != SQL_SUCCESS_WITH_INFO)
	{
		if(ret != SQL_NO_DATA)
		{// ���� ����
			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@atum_ExternalAuthentication2 Failed!!\r\n", TRUE);
		}
		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
		return FALSE;
	}

	ret = SQLMoreResults(hstmt_extAuth);
	ret = SQLMoreResults(hstmt_extAuth);
	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
	if(0 != nRetCode)
	{// 2007-03-30 by cmkwon, ���ϰ��� 0�� �ƴϸ� ���� ������
		return FALSE;
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ProcessDinamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - PreServer�� ����ϴ� ������ ����
	//	switch(i_qQuery->enumQuryType)
	//	{
	//	case :
	//	}
	
	// error: no such DB query type
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumPreDBManager::ProcessDinamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQuryType, GetDBQueryTypeString(i_qQuery->enumQuryType));
	return FALSE;
}