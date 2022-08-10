#include "stdafx.h"
#include "config.h"
#include "Global.h"


CConfig::CConfig()
{
}

BOOL CConfig::LoadUserDefinedVariables(const char *configFile)
{
	// 여러 개의 config file을 로딩할 수 있으므로 reset하지 않는다! by kelovon, 20030703
	// reset variables
	//m_mapUserDefinedVariables.clear();

	char		*token;
	char		buff[CONFIG_SIZE_BUFF];
	const char	seps[] = " \t=";
	ifstream	fin;

	fin.open(configFile);

	if (!fin.is_open())
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		char	szDir[512];
		GetCurrentDirectory(512, szDir);
		sprintf(szError, "[Error] CConfig::LoadUserDefinedVariables open() error, LastError[%d] ConfigFileName[%s] CurrentDirectory[%s]\r\n"
			, nErr, configFile, szDir);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, CONFIG_SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, CONFIG_SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		// 라인 검사, ';'로 끝나는 라인은 ';'을 NULL로 바꾸고 뒷 부분은 무시한다.
		char *pLineEnd = strstr(buff, ";");
		if (pLineEnd != NULL)
		{
			pLineEnd[0] = NULL;
		}

		token = ::strtok(buff, seps);

		// ignore blank lines
		if (token == NULL)
		{
			continue;
		}

		// ignore comments
		if (strncmp(token, "#", 1) == 0)
		{
			continue;
		}

		// user defined variable
		if (strncmp(token, "$", 1) == 0)
		{
			char key[CONFIG_SIZE_BUFF];
			char value[CONFIG_SIZE_BUFF];

			strncpy(key, token, CONFIG_SIZE_BUFF);

			token = (char*)GetToken((char*)::strtok(NULL, seps));
			if (token != NULL)
			{
			strncpy(value, token, CONFIG_SIZE_BUFF);
			}
			else
			{
				strncpy(value, "", CONFIG_SIZE_BUFF);
			}


			m_mapUserDefinedVariables.insert(pair<string, string>(key, value));
		}
	}

	fin.close();
/*
#ifdef _DEBUG
	if (!m_mapUserDefinedVariables.empty())
	{
		DBGOUT("[User Defined Variables]\n");
		map<string, string, greater<string> >::iterator itr = m_mapUserDefinedVariables.begin();
		while(m_mapUserDefinedVariables.end() != itr)
		{
			DBGOUT("  %s = %s\n", itr->first.c_str(), itr->second.c_str());
			itr++;
		}
	}
	else
	{
		DBGOUT("NO User Defined Variables...\n");
	}
#endif
//*/
	return TRUE;
}

const char* CConfig::GetToken(char* token)
{
	if (token == NULL) return NULL;

	map<string, string, greater<string> >::iterator itr = m_mapUserDefinedVariables.find(token);

	if (itr != m_mapUserDefinedVariables.end())
	{
		return itr->second.c_str();
	}

	return token;
}

const char* CConfig::strtok(char* strToken, const char* strDelimit)
{
	return GetToken(::strtok(strToken, strDelimit));
}

char* CConfig::getQuotedString(char *strToken, const char* strDelimit)
{
	char *pTmpToken = NULL;

	pTmpToken = ::strtok(strToken, "\"");
	pTmpToken = ::strtok(strToken, "\"");

	return pTmpToken;
}