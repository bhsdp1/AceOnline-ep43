#include "stdafx.h"
#include "config.h"

#ifdef	_ATUM_SERVER
#include "global.h"
#else // _ATUM_SERVER
#define DBGOUT(x)
#define g_pGlobal->WriteSystemLog(x)
#endif // _ATUM_SERVER

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
		if (0 == strncmp(token, "$$", 2))
		{// 2007-09-05 by cmkwon, 새로운 형식("$$"로 시작)을 추가함 - "$$"로 시작할 경우는 뒤의 스트일을 붙여서 처리한다
			char key[CONFIG_SIZE_BUFF];
			STRNCPY_MEMSET(key, token, CONFIG_SIZE_BUFF);

			char value[CONFIG_SIZE_BUFF];
			MEMSET_ZERO(value, CONFIG_SIZE_BUFF);
			while(TRUE)
			{
				token = (char*)GetToken((char*)::strtok(NULL, seps));
				if(NULL == token)
				{
					break;
				}
				int nLen = strlen(value);
				if(nLen + strlen(token) + 1 >= CONFIG_SIZE_BUFF)
				{
					break;
				}
				sprintf(&(value[nLen]), "%s ", token);				
			}
			m_mapUserDefinedVariables.insert(pair<string, string>(key, value));
		}
		else if (strncmp(token, "$", 1) == 0)
		{
			char key[CONFIG_SIZE_BUFF];
			char value[CONFIG_SIZE_BUFF];

			STRNCPY_MEMSET(key, token, CONFIG_SIZE_BUFF);

			token = (char*)GetToken((char*)::strtok(NULL, seps));
			if (token != NULL)
			{
				STRNCPY_MEMSET(value, token, CONFIG_SIZE_BUFF);
			}
			else
			{
				STRNCPY_MEMSET(value, "", CONFIG_SIZE_BUFF);
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

	if(0 == strncmp(token, "$", 1)
		|| 0 == strncmp(token, "#", 1))
	{// 2008-09-24 by cmkwon, global.cfg 형식 수정 - 정의한 것이 없다면 그냥 NULL을 리턴하게 수정 함
		return NULL;
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

D3DXVECTOR3 CConfig::getPosition(char *strToken)
{
	D3DXVECTOR3 ret(0, 0, 0);
	const char *tmpToken;

	tmpToken = GetToken(::strtok(strToken, " \t(),"));
	if (tmpToken == NULL) return ret;
	ret.x = atoi(tmpToken);

	tmpToken = GetToken(::strtok(strToken, " \t(),"));
	if (tmpToken == NULL) return ret;
	ret.y = atoi(tmpToken);

	tmpToken = GetToken(::strtok(strToken, " \t(),"));
	if (tmpToken == NULL) return ret;
	ret.z = atoi(tmpToken);

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CConfig::GetTokenList(vectstring *o_pTokenList, char *strToken, const char *strDelimit)
/// \brief		// 2007-09-05 by cmkwon, 새로운 형식("$$"로 시작)을 추가함 -
/// \author		cmkwon
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CConfig::GetTokenList(vectstring *o_pTokenList, char *strToken, const char *strDelimit)
{
	o_pTokenList->clear();

	char *token = ::strtok(strToken, strDelimit);
	while(token)
	{
		o_pTokenList->push_back(token);
		
		token = ::strtok(NULL, strDelimit);
	}
}
