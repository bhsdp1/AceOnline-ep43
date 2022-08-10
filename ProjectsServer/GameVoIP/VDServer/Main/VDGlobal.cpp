// VDGlobal.cpp: implementation of the CVDGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VDGlobal.h"
#include "VDIOCP.h"
#include "Config.h"

//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
CVDGlobal		*g_pVDGlobal = NULL;		// 전역변수관리를 위한 전역클래스포인터


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVDGlobal::CVDGlobal()
{
	if(g_pVDGlobal)
	{
		return;
	}
	g_pVDGlobal = this;

	
}

CVDGlobal::~CVDGlobal()
{
	this->EndServerSocket();
	g_pVoIPGlobal = NULL;			// 상위 클래스의 전역변수를 초기화 한다
}


BOOL CVDGlobal::InitServerSocket(void)
{
	// TODO: Place code here.
	
	DBGOUT("Starting VDServer...\n");
	if(NULL != m_pGIOCP){		return FALSE;}

	m_pGIOCP = new CVDIOCP(this->GetPortListening(), this->GetIPLocal());
	if(m_pGIOCP->IOCPInit() == FALSE)
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] VDServer IOCPInit Error\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR", MB_OK);
		return FALSE;
	}
	
	CVoIPGlobal::InitServerSocket();
	return TRUE;
}

BOOL CVDGlobal::EndServerSocket(void)
{
	CVoIPGlobal::EndServerSocket();

	// TODO: Place code here.
	
	return FALSE;
}

BOOL CVDGlobal::LoadConfiguration(void)
{
	// set config root path
	this->SetConfigRootPath();

	DBGOUT("========================================\n");
	DBGOUT("Loading Server Informations...\n");

	const int SIZE_BUFF = 512;
	string stringGlobalConfigFileName = string(this->GetConfigRootPath()) + string(GLOBAL_CONFIG_FILE_NAME);
	string stringConfigFileName = string(this->GetConfigRootPath()) + string(VD_SERVER_CONFIG_FILE_NAME);
	
	char buff[SIZE_BUFF];
	const char *token;
	char seps[] = " \t";
	char seps_servergroup[] = ".";

	
	ifstream fin;
	CConfig config;

	if (!config.LoadUserDefinedVariables(stringGlobalConfigFileName.c_str()))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
			, stringGlobalConfigFileName);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(stringConfigFileName.c_str()))
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error1\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	fin.open(stringConfigFileName.c_str());

	if (! fin.is_open())
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Can't open config file\r\n");
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
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed2.\n";
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
		else
		{
			// configuration file error!
			assert(0);
		}
	}


	DBGOUT("========================================\n");

	fin.close();

	return TRUE;
}
