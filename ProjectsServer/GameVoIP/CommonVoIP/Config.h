#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <fstream>
#include <string>
#include <map>

using namespace std;

#define CONFIG_SIZE_BUFF	512

class CConfig
{
public:
	CConfig();

	BOOL LoadUserDefinedVariables(const char *configFile);
	const char* strtok(char* strToken, const char* strDelimit);
	char* getQuotedString(char *strToken, const char *strDelimit);

private:
	const char* GetToken(char* token);

private:
	map<string, string, greater<string> >	m_mapUserDefinedVariables;
};

#endif // __CONFIG_H__
