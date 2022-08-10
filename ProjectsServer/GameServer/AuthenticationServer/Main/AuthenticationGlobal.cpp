// AuthenticationGlobal.cpp: implementation of the CAuthenticationGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AuthenticationGlobal.h"
#include "AuthenticationIOCP.h"
#include "AuthenticationIOCPSocket.h"
#include "Config.h"
#include "AtumSJ.h"

CAuthenticationGlobal *		g_pAuthenticationGlobal = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAuthenticationGlobal::CAuthenticationGlobal()
{
	if(g_pAuthenticationGlobal)
	{
		return;
	}

	g_pAuthenticationGlobal = this;

	// reset all member variables
	Reset();
}

CAuthenticationGlobal::~CAuthenticationGlobal()
{
	// reset all member variables
	Reset();

	g_pGlobalGameServer = NULL;
}

void CAuthenticationGlobal::Reset()
{
}

BOOL CAuthenticationGlobal::InitServerSocket(void)
{
	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 아래와 같이 수정
	//g_pAuthenticationGlobal->WriteSystemLogEX(TRUE, "Starting AuthenticationServer... \r\n");

	if(NULL != m_pGIOCP){		return FALSE;}

	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 
	this->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationGlobal::InitServerSocket# Starting... \r\n");

	m_pGIOCP = new CAuthenticationIOCP(m_nPortListening, m_szIPLocal);
	if(m_pGIOCP->IOCPInit() == FALSE)
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] AuthenticationServer IOCPInit Error\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR", MB_OK);
		return FALSE;
	}

	CGlobalGameServer::InitServerSocket();

	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 
	this->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationGlobal::InitServerSocket# Started \r\n");

	return TRUE;
}

BOOL CAuthenticationGlobal::EndServerSocket(void)
{
	// 2009-03-19 by cmkwon, 시스템 로그 추가 - 
	this->WriteSystemLogEX(TRUE, "  [Notify] CAuthenticationGlobal::EndServerSocket# \r\n");

	CGlobalGameServer::EndServerSocket();


	return TRUE;
}

BOOL CAuthenticationGlobal::LoadConfiguration(void)
{
	// set config root path
	this->SetConfigRootPath();

	DBGOUT("========================================\n");
	DBGOUT("Loading Server Informations...\n");

	const int SIZE_BUFF = 512;
	string configFileName = string(CONFIG_ROOT) + string("pre_authentication.cfg");
	char buff[SIZE_BUFF];
	char buffBackup[SIZE_BUFF];		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char *token;
	char seps[] = " \t";
	char seps_servergroup[] = ".";

	//char tmpIP[SIZE_MAX_IPADDRESS];
	//int	 tmpPort;

	/////////////////////////////////////////////
	// load MapWorkspace file...
	ifstream fin;
	CConfig config;

	if (!config.LoadUserDefinedVariables(AUTHENTICATION_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
			, GLOBAL_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(configFileName.c_str()))
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CAuthenticationIOCP::LoadAllServers Config Initialization Error1\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	fin.open(configFileName.c_str());

	if (! fin.is_open())
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CAuthenticationIOCP::LoadAllServers Can't open config file\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		STRNCPY_MEMSET(buffBackup, buff, SIZE_BUFF);		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

		token = config.strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		if(stricmp(token, "LocalIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalPublicIPAddress") == 0)
		{// 2006-05-23 by cmkwon,
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetPublicIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalListenPort") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortListening(atoi(token));
			}
		}
		else if(stricmp(token, "ODBCUID") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCUID\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				STRNCPY_MEMSET((char*)m_szODBCUID, token, SIZE_MAX_ODBC_CONN_STRING);
				BYTE byEncodedBinary[1024];
				MEMSET_ZERO(byEncodedBinary, 1024);
				if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					MEMSET_ZERO(m_szODBCUID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCUID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}
			}
		}
		else if(stricmp(token, "ODBCPASSWD") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCPASSWD\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				STRNCPY_MEMSET((char*)m_szODBCPASSWORD, token, SIZE_MAX_ODBC_CONN_STRING);
				BYTE byEncodedBinary[1024];
				MEMSET_ZERO(byEncodedBinary, 1024);
				if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					MEMSET_ZERO(m_szODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCPASSWORD, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}
			}
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CAuthenticationGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_IP, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			STRNCPY_MEMSET(m_szDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_PORT))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CAuthenticationGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_PORT, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			m_nDBServerPort = atoi(token);			
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_DATABASE_NAEE))
		{// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CAuthenticationGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_DATABASE_NAEE, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			STRNCPY_MEMSET(m_szDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else
		{
			// configuration file error!
			return FALSE;
		}
	}

	DBGOUT("========================================\n");

	fin.close();

	return TRUE;
}
