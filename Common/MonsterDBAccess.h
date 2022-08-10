#ifndef __MONSTER_DB_ACCESS__
#define __MONSTER_DB_ACCESS__

#define MAXDBMSGBUFLEN   255

#include "stdafx.h"

#include <sql.h>
#include <sqlext.h>
#include "AtumParam.h"

typedef enum
{
	MONSTER_LOAD_TYPE_SIZE_FOR_SERVER = 0,
	MONSTER_LOAD_TYPE_SIZE_FOR_CLIENT = 1
} MonsterLoadType;

class CLocalization;
class CMonsterDBAccess
{
public:
	CMonsterDBAccess();
	~CMonsterDBAccess();

	BOOL GetAllMonsters(ez_map<int, MONSTER_INFO> &mapMonster, ez_map<INT, ITEM*> *pMapItemInfo, MonsterLoadType loadType, CLocalization *i_pLocalization);
	BOOL MonsterMoveTypeCheck(MONSTER_INFO *pMon);
	BOOL CheckMPOption(MONSTER_INFO *pMonInfo);

private:
	SQLHENV		henv;
	SQLHDBC		hdbc;
	SQLHSTMT	hstmt;
};

#endif
