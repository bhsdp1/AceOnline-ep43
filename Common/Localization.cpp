// Localization.cpp: implementation of the CLocalization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Localization.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalization::CLocalization()
{
	m_nLoadLanguageType		= LANGUAGE_TYPE_UNKNOWN;
}

CLocalization::~CLocalization()
{

}

void CLocalization::ResetLocalization(void)
{
	m_nLoadLanguageType		= LANGUAGE_TYPE_UNKNOWN;
	m_vectLocalBuildingNPC.clear();
	m_vectLocalItem.clear();
	m_vectLocalMapInfo.clear();
	m_vectLocalMonster.clear();
	m_vectLocalRareItemInfo.clear();	
	m_vectLocalLuckyMachine.clear();	// 2009-03-03 by dhjin, 럭키머신 수정안
	m_vectLocalHPAction.clear();	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	m_vectLocalCinema.clear();		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::IsKoreanLanguageType(void)
/// \brief		한글인지의 여부 - 한글이라면 현지화 처리를 하지 않도록 한다.
/// \author		cmkwon
/// \date		2005-07-01 ~ 2005-07-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::IsDefaultLanguageType(void)
{
	if(m_nLoadLanguageType == LANGUAGE_TYPE_DEFAULT)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CLocalization::LoadConfiguration(char *i_szLocalizationFilePath, int i_nReqLanguage)
{
	if(FALSE == IS_VALID_LANGUAGE_TYPE(i_nReqLanguage))
	{
		return FALSE;
	}
	m_nLoadLanguageType	 = i_nReqLanguage;

	if(IsDefaultLanguageType())
	{
		return TRUE;
	}

	char szFullPath[1024];
	wsprintf(szFullPath, "%s\\ti_BuildingNPC.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadBuildingNPC(szFullPath))
	{
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_Item.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadItem(szFullPath))
	{
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_MapInfo.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadMapInfo(szFullPath))
	{
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_Monster.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadMonster(szFullPath))
	{
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_RareItemInfo.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadRareItemInfo(szFullPath))
	{
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_LuckyMachine.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadLuckyMachine(szFullPath))
	{// 2009-03-03 by dhjin, 럭키머신 수정안
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_HPAction.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadHPAction(szFullPath))
	{// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_Cinema.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadCinema(szFullPath))
	{// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
		return FALSE;
	}
	wsprintf(szFullPath, "%s\\ti_OperatorAction.cfg", i_szLocalizationFilePath);
	if(FALSE == LoadPetOperatorActionInfo(szFullPath))
	{// 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가 - *.cfg파일 추가
		return FALSE;
	}
	return TRUE;
}

BOOL CLocalization::LoadBuildingNPC(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;
	char	szError[1024];

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			sprintf(szError, "[Error] LoadBuildingNPC() Data error\r\n");
			DbgOut(szError);
			return FALSE;
		}
		SLOCAL_ti_BuildingNPC tmBuildingNPC;
		MEMSET_ZERO(&tmBuildingNPC, sizeof(tmBuildingNPC));
		tmBuildingNPC.BuildingIndex = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			sprintf(szError, "[Error] LoadBuildingNPC() error, NPCName is not exist!\r\n");
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_NPC_NAME <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadBuildingNPC NPCName Length overflow !!, BuildingIndex[%8d], NPCNameLen[%d]\r\n"
				, tmBuildingNPC.BuildingIndex, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmBuildingNPC.szNPCName, token, SIZE_MAX_NPC_NAME);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			sprintf(szError, "[Error] LoadBuildingNPC() error, GreetingTalk is not exist!\r\n");
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_BUILDING_NPC_TALK_STRING <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadBuildingNPC Building NPC Talk Length overflow !!, BuildingIndex[%8d], NPCTalkLen[%d]\r\n"
				, tmBuildingNPC.BuildingIndex, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmBuildingNPC.szGreetingTalk, token, SIZE_MAX_BUILDING_NPC_TALK_STRING);
		
		SLOCAL_ti_BuildingNPC *pBuildingNPC = FindBuildingNPC(tmBuildingNPC.BuildingIndex);
		if(pBuildingNPC)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadBuildingNPC overlapping error, BuildingIndex[%8d], NPCName[%s]\r\n"
				, pBuildingNPC->BuildingIndex, pBuildingNPC->szNPCName);
			sprintf(szError, "[Error] LoadBuildingNPC overlapping error, BuildingIndex[%8d], NPCName[%s]\r\n"
				, tmBuildingNPC.BuildingIndex, tmBuildingNPC.szNPCName);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalBuildingNPC.push_back(tmBuildingNPC);
		}
	}// end_while (TRUE)	
	fin.close();
	
	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalBuildingNPC.empty();
}
BOOL CLocalization::LoadItem(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token){						return FALSE;}
		
		///////////////////////////////////////////////////////////////////////////////
		// ItemNum 을 가져온다.
		SLOCAL_ti_Item tmItem;
		MEMSET_ZERO(&tmItem, sizeof(tmItem));
		tmItem.ItemNum = atoi(token);
		if(0 == tmItem.ItemNum){				return FALSE;}
		
		///////////////////////////////////////////////////////////////////////////////
		// ItemName 을 가져온다
		token = strtok(NULL, seps);
		if(NULL == token)
		{
			// 2008-10-16 by cmkwon, 수정
			char	szError[1024];
			sprintf(szError, "[Error] LoadItem ItemName isn't !!, RareCode[%8d]\r\n"
				, tmItem.ItemNum);
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_ITEM_NAME <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadItem ItemName Length overflow !!, ItemNum[%8d], ItemNameLen[%d]\r\n"
				, tmItem.ItemNum, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmItem.szItemName, token, SIZE_MAX_ITEM_NAME);

		///////////////////////////////////////////////////////////////////////////////
		// Item Description 을 가져온다.
		token = strtok(NULL, seps);
		if(token)
		{// Description 은 없을 수도 있다.
			if(SIZE_MAX_ITEM_DESCRIPTION <= strlen(token))
			{
				char	szError[1024];
				sprintf(szError, "[Error] LoadItem ItemDescription Length overflow !!, ItemNum[%8d], ItemDescriptionLen[%d]\r\n"
					, tmItem.ItemNum, strlen(token));
				DbgOut(szError);
			}

			STRNCPY_MEMSET(tmItem.szDescription, token, SIZE_MAX_ITEM_DESCRIPTION);
		}

		SLOCAL_ti_Item *pItem = FindItem(tmItem.ItemNum);
		if(pItem)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadItem overlapping error, ItemNum[%8d], ItemName[%s]\r\n"
				, pItem->ItemNum, pItem->szItemName);
			sprintf(szError, "[Error] LoadItem overlapping error, ItemNum[%8d], ItemName[%s]\r\n"
				, tmItem.ItemNum, tmItem.szItemName);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalItem.push_back(tmItem);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalItem.empty();
}
BOOL CLocalization::LoadMapInfo(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_MapInfo tmMapInfo;
		MEMSET_ZERO(&tmMapInfo, sizeof(tmMapInfo));
		tmMapInfo.MapIndex = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			// 2008-10-16 by cmkwon, 수정
			char	szError[1024];
			sprintf(szError, "[Error] LoadMapInfo MapName isn't !!, MapIndex[%8d]\r\n"
				, tmMapInfo.MapIndex);
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_MAP_NAME <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadMapInfo MapName Length overflow !!, MapIndex[%8d], MapNameLen[%d]\r\n"
				, tmMapInfo.MapIndex, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmMapInfo.szMapName, token, SIZE_MAX_MAP_NAME);

		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Map Description 을 가져온다.
		token = strtok(NULL, seps);
		if(token)
		{// Description 은 없을 수도 있다.
			if(SIZE_MAX_MAP_DESCRIPTION <= strlen(token))
			{
				char	szError[1024];
				sprintf(szError, "[Error] LoadItem MapDescription Length overflow !!, MapIndex[%8d], MapDescriptionLen[%d]\r\n"
					, tmMapInfo.MapIndex, strlen(token));
				DbgOut(szError);
			}
			
			STRNCPY_MEMSET(tmMapInfo.szDescription, token, SIZE_MAX_MAP_DESCRIPTION);
		}

		SLOCAL_ti_MapInfo *pMapInfo = FindMapInfo(tmMapInfo.MapIndex);
		if(pMapInfo)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadMapInfo overlapping error, MapIndex[%8d] MapName[%s]\r\n"
				, pMapInfo->MapIndex, pMapInfo->szMapName);
			sprintf(szError, "[Error] LoadMapInfo overlapping error, MapIndex[%8d] MapName[%s]\r\n"
				, tmMapInfo.MapIndex, tmMapInfo.szMapName);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalMapInfo.push_back(tmMapInfo);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalMapInfo.empty();
}
BOOL CLocalization::LoadMonster(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_Monster tmMonster;
		MEMSET_ZERO(&tmMonster, sizeof(tmMonster));
		tmMonster.MonsterUnitKind = atoi(token);

		token = strtok(NULL, seps);
		if(token)
		{// 2006-03-16 by cmkwon, Monster Name이 꼭 필요 없는 몬스터도 있으므로 없어도 정상적으로 처리해야한다.
			if(SIZE_MAX_MONSTER_NAME <= strlen(token))
			{
				char	szError[1024];
				sprintf(szError, "[Error] LoadMonster MonsterName Length overflow !!, MonsterUnitKind[%8d], MonsterNameLen[%d]\r\n"
					, tmMonster.MonsterUnitKind, strlen(token));
				DbgOut(szError);
			}
			STRNCPY_MEMSET(tmMonster.szMonsterName, token, SIZE_MAX_MONSTER_NAME);
		}

		SLOCAL_ti_Monster *pMonster = FindMonster(tmMonster.MonsterUnitKind);
		if(pMonster)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadMonster overlapping error, MonsterUnitKind[%8d] MonsterName[%s]\r\n"
				, pMonster->MonsterUnitKind, pMonster->szMonsterName);
			sprintf(szError, "[Error] LoadMonster overlapping error, MonsterUnitKind[%8d] MonsterName[%s]\r\n"
				, tmMonster.MonsterUnitKind, tmMonster.szMonsterName);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalMonster.push_back(tmMonster);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalMonster.empty();
}
BOOL CLocalization::LoadRareItemInfo(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_RareItemInfo tmRareItemInfo;
		MEMSET_ZERO(&tmRareItemInfo, sizeof(tmRareItemInfo));
		tmRareItemInfo.RareCodeNum = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			// 2008-10-16 by cmkwon, 수정
			char	szError[1024];
			sprintf(szError, "[Error] LoadRareItemInfo RareName isn't !!, RareCode[%8d]\r\n"
				, tmRareItemInfo.RareCodeNum);
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_RARE_FIX_NAME <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadRareItemInfo RareName Length overflow !!, RareCode[%8d], RareNameLen[%d]\r\n"
				, tmRareItemInfo.RareCodeNum, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmRareItemInfo.szRareName, token, SIZE_MAX_RARE_FIX_NAME);

		SLOCAL_ti_RareItemInfo *plRareInfo = FindRareItemInfo(tmRareItemInfo.RareCodeNum);
		if(plRareInfo)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadRareItemInfo overlapping error, RareCode[%8d] RareName\r\n"
				, plRareInfo->RareCodeNum, plRareInfo->szRareName);
			DbgOut(szError);
			sprintf(szError, "[Error] LoadRareItemInfo overlapping error, RareCode[%8d] RareName\r\n"
				, tmRareItemInfo.RareCodeNum, tmRareItemInfo.szRareName);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalRareItemInfo.push_back(tmRareItemInfo);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalRareItemInfo.empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::LoadLuckyMachine(char *i_szFileFullPath)
/// \brief		// 2009-03-03 by dhjin, 럭키머신 수정안
/// \author		dhjin
/// \date		2009-03-03 ~ 2009-03-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::LoadLuckyMachine(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_LuckyMachine tmLuckyMachine;
		MEMSET_ZERO(&tmLuckyMachine, sizeof(SLOCAL_ti_LuckyMachine));
		tmLuckyMachine.MachineNum = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadLuckyMachine desc isn't !!, MachineNum[%8d]\r\n"
				, tmLuckyMachine.MachineNum);
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_LUCKYMACHINE_DESCRIPTION <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadLuckyMachine desc Length overflow !!, MachineNum[%8d], MachineDescLen[%d]\r\n"
				, tmLuckyMachine.MachineNum, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmLuckyMachine.szDescription, token, SIZE_MAX_LUCKYMACHINE_DESCRIPTION);

		SLOCAL_ti_LuckyMachine *pLuckyMachine = FindLuckyMachine(tmLuckyMachine.MachineNum);
		if(pLuckyMachine)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadRareItemInfo overlapping error, RareCode[%8d] RareName\r\n"
				, pLuckyMachine->MachineNum, pLuckyMachine->szDescription);
			DbgOut(szError);
			sprintf(szError, "[Error] LoadRareItemInfo overlapping error, RareCode[%8d] RareName\r\n"
				, tmLuckyMachine.MachineNum, tmLuckyMachine.szDescription);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalLuckyMachine.push_back(tmLuckyMachine);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalLuckyMachine.empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::LoadHPAction(char *i_szFileFullPath)
/// \brief		// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::LoadHPAction(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_HPAction tmHPAction;
		MEMSET_ZERO(&tmHPAction, sizeof(SLOCAL_ti_HPAction));
		tmHPAction.HPActionUID = atoi(token);
		
		token = strtok(NULL, seps);
		if(NULL == token)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadHPAction HPTalk isn't !!, HPActionNum[%8d]\r\n"
				, tmHPAction.HPActionUID);
			DbgOut(szError);
			return FALSE;
		}

		if(SIZE_MAX_HPTALK_DESCRIPTION <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadHPAction HPTalk Length overflow !!, HPActionNum[%8d], HPTalkLen[%d]\r\n"
				, tmHPAction.HPActionUID, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmHPAction.HPTalk, token, SIZE_MAX_HPTALK_DESCRIPTION);

		SLOCAL_ti_HPAction *pHPAction = FindHPAction(tmHPAction.HPActionUID);
		if(pHPAction)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadHPAction overlapping error, HPActionNum[%8d] HPTalk\r\n"
				, pHPAction->HPActionUID, pHPAction->HPTalk);
			DbgOut(szError);
			sprintf(szError, "[Error] LoadHPAction overlapping error, HPActionNum[%8d] HPTalk\r\n"
				, pHPAction->HPActionUID, pHPAction->HPTalk);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalHPAction.push_back(tmHPAction);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalHPAction.empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::LoadCinema(char *i_szFileFullPath)
/// \brief		2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 -
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::LoadCinema(char *i_szFileFullPath) {
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;
	
	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...
		
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}
	
	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables
			
			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}
		
		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return FALSE;
		}
		SLOCAL_ti_Cinema tmCinema;
		MEMSET_ZERO(&tmCinema, sizeof(SLOCAL_ti_Cinema));
		tmCinema.CinemaNum = atoi(token);
		
		token = strtok(NULL, seps);
		if(NULL == token)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadCinema CinemaOrder isn't !!, CinemaNum[%8d]\r\n"
				, tmCinema.CinemaNum);
			DbgOut(szError);
			return FALSE;
		}
		tmCinema.CinemaOrder = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadCinema StartTime isn't !!, CinemaNum[%8d]\r\n"
				, tmCinema.CinemaNum);
			DbgOut(szError);
			return FALSE;
		}
		tmCinema.EffectStartTime = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadCinema CinemaTalk isn't !!, CinemaNum[%8d]\r\n"
				, tmCinema.CinemaNum);
			DbgOut(szError);
			return FALSE;
		}
		if(SIZE_MAX_CINEMATALK_DESCRIPTION <= strlen(token))
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadCinema CinemaTalk Length overflow !!, CinemaNum[%8d], CinemaTalkLen[%d]\r\n"
				, tmCinema.CinemaNum, strlen(token));
			DbgOut(szError);
		}
		STRNCPY_MEMSET(tmCinema.CinemaTalk, token, SIZE_MAX_CINEMATALK_DESCRIPTION);
		
		SLOCAL_ti_Cinema *pCinema = FindCinema(tmCinema.CinemaNum, tmCinema.CinemaOrder, tmCinema.EffectStartTime);
		if(pCinema)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadCinema overlapping error, CinemaNum[%8d], CinemaOrder[%8d], StartTime[%8d] \r\n"
				, pCinema->CinemaNum, pCinema->CinemaOrder, pCinema->EffectStartTime);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalCinema.push_back(tmCinema);
		}
	}// end_while (TRUE)
	fin.close();
	
	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalCinema.empty();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::LoadPetOperatorActionInfo(char *i_szFileFullPath)
/// \brief		// 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가 - *.cfg파일 추가
/// \author		shcho
/// \date		2010-12-03 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::LoadPetOperatorActionInfo(char *i_szFileFullPath)
{
	char		buff[1024];
	const char	*token;
	char		seps[] = "\t";
	ifstream	fin;
	BOOL		bRet = TRUE;

	fin.open(i_szFileFullPath);
	if (!fin.is_open())
	{// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, i_szFileFullPath);
		DbgOut(szError);
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, sizeof(buff));
		BOOL bFlagFileEnd = fin.getline(buff, sizeof(buff)).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}
		
		token = strtok(buff, seps);
		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		int nLanguageType = atoi(token);
		if(m_nLoadLanguageType != nLanguageType)
		{// 요청한 언어와 다르다
			continue;
		}

		token = strtok(NULL, seps);
		if(NULL == token){						return FALSE;}
		
		///////////////////////////////////////////////////////////////////////////////
		// ItemNum 을 가져온다.
		SLOCAL_ti_OperatorInfo tmoperInfo;
		MEMSET_ZERO(&tmoperInfo, sizeof(tmoperInfo));
		tmoperInfo.Itemnum = atoi(token);
		if(0 == tmoperInfo.Itemnum){				return FALSE;}
		
		///////////////////////////////////////////////////////////////////////////////
		// FunctionIndex 을 가져온다
		token = strtok(NULL, seps);
		if(NULL == token)
		{
			// 2008-10-16 by cmkwon, 수정
			char	szError[1024];
			sprintf(szError, "[Error] PetOperatorAction ItemName isn't !!, RareCode[%8d]\r\n"
				, tmoperInfo.Itemnum);
			DbgOut(szError);
			return FALSE;
		}
		tmoperInfo.FunctionIndex = atoi(token);

		///////////////////////////////////////////////////////////////////////////////
		// Operator Description 을 가져온다.
		token = strtok(NULL, seps);
		if(token)
		{// Description 은 없을 수도 있다.
			if(SIZE_MAX_PET_OPERATOR_DESCRIPTION <= strlen(token))
			{
				char	szError[1024];
				sprintf(szError, "[Error] LoadItem PetOperatorDescription Length overflow !!, ItemNum[%8d], PetOperatorDescriptionLen[%s]\r\n"
					, tmoperInfo.Itemnum, strlen(token));
				DbgOut(szError);
			}

			STRNCPY_MEMSET(tmoperInfo.ActionDesc, token, SIZE_MAX_PET_OPERATOR_DESCRIPTION);
		}

		SLOCAL_ti_OperatorInfo *pOperatorInfo = FindOperatorAction(tmoperInfo.Itemnum,tmoperInfo.FunctionIndex);
		if(pOperatorInfo)
		{
			char	szError[1024];
			sprintf(szError, "[Error] LoadItem overlapping error, ItemNum[%8d], FunctionIndex[%d]\r\n"
				, pOperatorInfo->Itemnum, pOperatorInfo->FunctionIndex);
			sprintf(szError, "[Error] LoadItem overlapping error, ItemNum[%8d], FunctionIndex[%d]\r\n"
				, tmoperInfo.Itemnum, tmoperInfo.FunctionIndex);
			DbgOut(szError);
			bRet = FALSE;
		}
		else
		{
			m_vectLocalOperatorInfo.push_back(tmoperInfo);
		}
	}// end_while (TRUE)
	fin.close();

	if(FALSE == bRet)
	{
		return FALSE;
	}
	return !m_vectLocalOperatorInfo.empty();
}

SLOCAL_ti_BuildingNPC * CLocalization::FindBuildingNPC(INT i_nBuildingIndex)
{
	for(int i=0; i < m_vectLocalBuildingNPC.size(); i++)
	{
		if(m_vectLocalBuildingNPC[i].BuildingIndex == i_nBuildingIndex)
		{
			return &m_vectLocalBuildingNPC[i];
		}
	}
	return NULL;
}

SLOCAL_ti_Item * CLocalization::FindItem(INT i_nItemNum)
{
	for(int i=0; i < m_vectLocalItem.size(); i++)
	{
		if(m_vectLocalItem[i].ItemNum == i_nItemNum)
		{
			return &m_vectLocalItem[i];
		}
	}
	return NULL;
}

SLOCAL_ti_MapInfo * CLocalization::FindMapInfo(INT i_nMapIndex)
{
	for(int i=0; i < m_vectLocalMapInfo.size(); i++)
	{
		if(m_vectLocalMapInfo[i].MapIndex == i_nMapIndex)
		{
			return &m_vectLocalMapInfo[i];
		}
	}
	return NULL;
}

SLOCAL_ti_Monster * CLocalization::FindMonster(INT i_nMonsterUnitKind)
{
	for(int i=0; i < m_vectLocalMonster.size(); i++)
	{
		if(m_vectLocalMonster[i].MonsterUnitKind == i_nMonsterUnitKind)
		{
			return &m_vectLocalMonster[i];
		}
	}
	return NULL;
}

SLOCAL_ti_RareItemInfo * CLocalization::FindRareItemInfo(INT i_nRareCodeNum)
{
	for(int i=0; i < m_vectLocalRareItemInfo.size(); i++)
	{
		if(m_vectLocalRareItemInfo[i].RareCodeNum == i_nRareCodeNum)
		{
			return &m_vectLocalRareItemInfo[i];
		}
	}
	return NULL;
}

// 2009-03-03 by dhjin, 럭키머신 수정안
SLOCAL_ti_LuckyMachine * CLocalization::FindLuckyMachine(INT i_nMachineNum)
{
	for(int i=0; i < m_vectLocalLuckyMachine.size(); i++)
	{
		if(m_vectLocalLuckyMachine[i].MachineNum == i_nMachineNum)
		{
			return &m_vectLocalLuckyMachine[i];
		}
	}
	return NULL;
}

// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
SLOCAL_ti_HPAction * CLocalization::FindHPAction(INT i_HPActionUID)
{
	for(int i=0; i < m_vectLocalHPAction.size(); i++)
	{
		if(m_vectLocalHPAction[i].HPActionUID == i_HPActionUID)
		{
			return &m_vectLocalHPAction[i];
		}
	}
	return NULL;
}

SLOCAL_ti_Cinema * CLocalization::FindCinema(CinemaNum_t i_CinemaNum, CinemaOrder_t i_CinemaOrder, MSec_t i_StartTime)
{// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
	for(int i=0; i < m_vectLocalCinema.size(); i++)
	{
		if(m_vectLocalCinema[i].CinemaNum == i_CinemaNum
			&& m_vectLocalCinema[i].CinemaOrder == i_CinemaOrder
			&& m_vectLocalCinema[i].EffectStartTime == i_StartTime)
		{
			return &m_vectLocalCinema[i];
		}
	}
	return NULL;
}
// 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가 - 찾는 함수
SLOCAL_ti_OperatorInfo * CLocalization::FindOperatorAction(INT i_nItemNum, INT i_functionIndex)
{
	for(int i=0; i < m_vectLocalOperatorInfo.size(); i++)
	{
		if(m_vectLocalOperatorInfo[i].Itemnum == i_nItemNum
			&& m_vectLocalOperatorInfo[i].FunctionIndex == i_functionIndex)
		{
			return &m_vectLocalOperatorInfo[i];
		}
	}
	return NULL;
}
// END 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가 - 찾는 함수 

BOOL CLocalization::BuildingNPCLocalization(INT i_BuindingIndex, char *o_szNPCName, char *o_szGreetingTalk)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_BuildingNPC *pLocal = FindBuildingNPC(i_BuindingIndex);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::BuildingNPCLocalization non-existing error, Building(%d), NPCName(%s)\r\n",
			i_BuindingIndex, o_szNPCName);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szNPCName, pLocal->szNPCName, SIZE_MAX_NPC_NAME);
	STRNCPY_MEMSET(o_szGreetingTalk, pLocal->szGreetingTalk, SIZE_MAX_BUILDING_NPC_TALK_STRING);
	return TRUE;
}

BOOL CLocalization::ItemLocalization(INT i_ItemNum, char *o_szItemName, char *o_szDescription/*=NULL*/)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_Item *pLocal = FindItem(i_ItemNum);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::ItemLocalization non-existing error, ItemNum(%d), ItemName(%s)\r\n",
			i_ItemNum, o_szItemName);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szItemName, pLocal->szItemName, SIZE_MAX_ITEM_NAME);
	if(o_szDescription)
	{	
		STRNCPY_MEMSET(o_szDescription, pLocal->szDescription, SIZE_MAX_ITEM_DESCRIPTION);
	}
	return TRUE;
}

BOOL CLocalization::MapInfoLocalization(INT i_MapIndex, char *o_szMapName, char *o_szMapDescription/*=NULL*/)		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_MapInfo *pLocal = FindMapInfo(i_MapIndex);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::MapInfoLocalization non-existing error, MapIndex(%d), MapName(%s)\r\n",
			i_MapIndex, o_szMapName);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szMapName, pLocal->szMapName, SIZE_MAX_MAP_NAME);	
	if(0 < strlen(pLocal->szDescription))
	{// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가	
		STRNCPY_MEMSET(o_szMapDescription, pLocal->szDescription, SIZE_MAX_MAP_DESCRIPTION);
	}
	return TRUE;
}
BOOL CLocalization::MonsterLocalization(INT i_MonsterUnitKind, char *o_szMonsterName)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_Monster *pLocal = FindMonster(i_MonsterUnitKind);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::MonsterLocalization non-existing error, MonsterUnitKind(%d) MonsterName(%s)\r\n",
			i_MonsterUnitKind, o_szMonsterName);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szMonsterName, pLocal->szMonsterName, SIZE_MAX_MONSTER_NAME);
	return TRUE;
}
BOOL CLocalization::RareItemInfoLocalization(INT i_RareCodeNum, char *o_szRareName)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_RareItemInfo *pLocal = FindRareItemInfo(i_RareCodeNum);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::RareItemInfoLocalization non-existing error, RareCodeNum(%d), RareName(%s)\r\n",
			i_RareCodeNum, o_szRareName);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szRareName, pLocal->szRareName, SIZE_MAX_RARE_FIX_NAME);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-03 by dhjin, 럭키머신 수정안
/// \author		dhjin
/// \date		2009-03-03 ~ 2009-03-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::LuckyMachineLocalization(INT i_MachineNum, char *o_szDescription/*=NULL*/)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}

	SLOCAL_ti_LuckyMachine *pLocal = FindLuckyMachine(i_MachineNum);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::LuckyMachineLocalization non-existing error, MachineNum(%d), Desc(%s)\r\n",
			i_MachineNum, o_szDescription);
		DbgOut(szErr);
		return FALSE;
	}

	STRNCPY_MEMSET(o_szDescription, pLocal->szDescription, SIZE_MAX_LUCKYMACHINE_DESCRIPTION);		// 2009-05-12 by cmkwon, 럭키머신 설명 잘리는 버그 수정 - 
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::HPActionLocalization(INT i_HPActionUID, char *o_szHPTalk/*=NULL*/)
/// \brief		// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::HPActionLocalization(INT i_HPActionUID, char *o_szPreHPTalk/*=NULL*/, char *o_szHPTalk/*=NULL*/)	// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}
	
	SLOCAL_ti_HPAction *pLocal = FindHPAction(i_HPActionUID);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::HPActionLocalization non-existing error, HPActionNum(%d), HPTalk(%s)\r\n",
			i_HPActionUID, o_szHPTalk);
		DbgOut(szErr);
		return FALSE;
	}
	
	if(0 < strlen(pLocal->PreHPTalk)) {
		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
		STRNCPY_MEMSET(o_szPreHPTalk, pLocal->PreHPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
	}
	if(0 < strlen(pLocal->HPTalk)) {
		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
		STRNCPY_MEMSET(o_szHPTalk, pLocal->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLocalization::CinemaLocalization(CinemaNum_t i_CinemaNum, CinemaOrder_t i_CinemaOrder, MSec_t i_StartTime, char *o_szCinemaTalk/*=NULL*/)
/// \brief		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - *.cfg파일 추가
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLocalization::CinemaLocalization(CinemaNum_t i_CinemaNum, CinemaOrder_t i_CinemaOrder, MSec_t i_StartTime, char *o_szCinemaTalk/*=NULL*/) {

	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}
	
	SLOCAL_ti_Cinema *pLocal = FindCinema(i_CinemaNum, i_CinemaOrder, i_StartTime);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::CinemaLocalization non-existing error, CinemaNum(%d), CinemaOrder(%d), StartTime(%d), CinemaTalk(%s)\r\n",
			i_CinemaNum, i_CinemaOrder, i_StartTime, o_szCinemaTalk);
		DbgOut(szErr);
		return FALSE;
	}
	
	if(0 < strlen(pLocal->CinemaTalk)) {
		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
		STRNCPY_MEMSET(o_szCinemaTalk, pLocal->CinemaTalk, SIZE_MAX_CINEMATALK_DESCRIPTION);
	}
	
	return TRUE;
}

// 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가	
BOOL CLocalization::PetOperatorActionInfoLocalization(INT i_nItemNum, INT i_functionIndex, char* o_szActionDesc)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2005-07-01 by cmkwon, 한글은 DB의내용을 그대로 사용 - DB의 데이타는 한글이어야 한다.
	if(this->IsDefaultLanguageType()){			return TRUE;}
	
	SLOCAL_ti_OperatorInfo *pLocal = FindOperatorAction(i_nItemNum, i_functionIndex);
	if(NULL == pLocal)
	{
		char szErr[1024];
		sprintf(szErr, "[Error] CLocalization::OperatorAction non-existing error, ItemNum(%d), FunctionIndex(%d), CinemaTalk(%s)\r\n",
			i_nItemNum, i_functionIndex, o_szActionDesc );
		DbgOut(szErr);
		return FALSE;
	}
	
	if(0 < strlen(pLocal->ActionDesc)) {
		// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
		STRNCPY_MEMSET(o_szActionDesc, pLocal->ActionDesc, SIZE_MAX_PET_OPERATOR_DESCRIPTION);
	}
	
	return TRUE;
}
// END 2010-12-03 by shcho, 펫 시스템 관련 스트링 로컬리제이션 추가

