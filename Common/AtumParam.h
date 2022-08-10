///////////////////////////////////////////////////////////////////////////////
//		Ä³¸¯ÅÍ, ¸ó½ºÅÍ, ¾ÆÀÌÅÛ ÆÄ¶ó¹ÌÅÍ Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMPARAM_H)
#define AFX_ATUMPARAM_H

#include <math.h>
#include <d3dx9math.h>
#include "XOR_Lib.h"		// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡ÇÔ
#include <stdio.h>
#include "DebugAssert.h"
#include <string>
#include <vector>
#include <SQLTYPES.H>
#include "DefineGlobal.h"					// 2007-03-14 by cmkwon
#include "LocalizationDefineCommon.h"		// 2006-09-05 by cmkwon
#include "AtumDateTime.h"
#include "mt_stl.h"							// 2007-01-09 by cmkwon

#if defined(_ATUM_SERVER)
//#include "GlobalFunc.h"						// 2008-06-26 by cmkwon, float -> int Çüº¯È¯ ÇÔ¼ö Ãß°¡ - 
#endif

using namespace std;

// 2008-04-25 by cmkwon, Áö¿ø ¾ð¾î/¼­ºñ½º Ãß°¡½Ã ²À Ãß°¡ µÇ¾î¾ß ÇÏ´Â »çÇ× - [¾ð¾î-ÇÊ¼ö] Ãß°¡ ¾ð¾î Á¤ÀÇ Ãß°¡
#define LANGUAGE_TYPE_UNKNOWN			-1
#define LANGUAGE_TYPE_DEFAULT			0
#define LANGUAGE_TYPE_ENGLISH			1
#define LANGUAGE_TYPE_VIETNAMESE		2	

#define LANGUAGE_TYPE_LAST				LANGUAGE_TYPE_VIETNAMESE		// 2009-03-17 by cmkwon, Áö¿ø ¾ð¾î Ãß°¡(Æú¶õµå¾î, ½ºÆäÀÎ¾î) - 
// 2008-04-11 by cmkwon, Áö¿ø ¾ð¾î Ãß°¡ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤
#define IS_VALID_LANGUAGE_TYPE(_lang)	((_lang) >= LANGUAGE_TYPE_DEFAULT && (_lang) <= LANGUAGE_TYPE_LAST)	// 2008-04-11 by cmkwon, Áö¿ø ¾ð¾î Ãß°¡ - 

extern char *GET_LANGUAGE_TYPE_STRING(int i_nLangTy);	// 2008-04-25 by cmkwon, Áö¿ø ¾ð¾î¿¡ µ¶ÀÏ¾î Ãß°¡ - Gameforge4D_Deu Ãß°¡

// Country String

#ifdef SERVICE_TYPE_ENGLISH_SERVER_1
#define SERVICE_COUNTRY_STRING		"Canada"
#endif

#ifdef SERVICE_TYPE_VIETNAMESE_SERVER_1
#define SERVICE_COUNTRY_STRING		"Vietnam"
#endif

#if defined(SERVICE_TYPE_DREAMACE) || defined(SERVICE_TYPE_ETERNAL_SKIES)
#define SERVICE_COUNTRY_STRING		"Global"
#endif



///////////////////////////////////////////////////////////////////////////////
// Atum Type Á¤ÀÇ
typedef INT64		TimeUnit_t;				// milli-seconds
typedef SHORT		Stat_t;					// °¢ ½ºÅÝ °ª
typedef BYTE		TimerEventType;			// Å¸ÀÌ¸Ó ÀÌº¥Æ® Å¸ÀÔ, TE_TYPE_XXX
typedef UINT64		PartyID_t;				// ÆÄÆ¼ ¾ÆÀÌµð, PartyID(PartyUnqiueNumber + CreationTime)
typedef BYTE		BitFlag8_t;				// 8 bit flag
typedef USHORT		BitFlag16_t;			// 16 bit flag
typedef UINT		BitFlag32_t;			// 32 bit flag
typedef ULONGLONG	BitFlag64_t;			// 64 bit flag
typedef double		Experience_t;			// °æÇèÄ¡
#define COMPARE_BIT_FLAG(VAR, MASK)	(((VAR) & (MASK)) != 0)
#define SET_BIT_FLAG(VAR, MASK)		{(VAR) |= (MASK);}
#define CLEAR_BIT_FLAG(VAR, MASK)	{(VAR) &= ~(MASK);}
typedef short		Prob256_t;				// È®·ü [0, 256)
#define PROB256_MAX_VALUE	255
typedef short		Prob100_t;				// È®·ü [0, 100]
// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
// #define PROB100_MAX_VALUE	99	// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
#define PROB100_MAX_VALUE	100
typedef short		Prob1000_t;				// È®·ü [0, 1000)
#define PROB1000_MAX_VALUE	999
typedef short		Prob10K_t;				// È®·ü [0, 10000)
#define PROB10K_MAX_VALUE	9999
typedef int			Prob100K_t;				// È®·ü [0, 100000)
#define PROB100K_MAX_VALUE	99999
typedef int			Prob1000K_t;			// È®·ü [0, 1000000)
#define PROB1000K_MAX_VALUE	999999
typedef USHORT ChannelIndex_t;
typedef USHORT MapIndex_t;
const char *Int2String(int n, string &str);	// convert integer to string
#define INT2STRING(x)	Int2String(x, string())
typedef USHORT	Err_t;
typedef vector<UINT>		vectUINT;		// 2009-09-17 by cmkwon, B±â¾î Ã¼ÇÁ ÇÏÇâ - 

///////////////////////////////////////////////////////////////////////////////
// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 
#define MILLISECOND_TO_SECOND(ms)			((int)((ms)/1000))		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - MilliSecond ¸¦ Second ·Î º¯°æÇÏ´Â µðÆÄÀÎ Ãß°¡
#define Prob256_TO_Prob10K(pro)				(((pro)==PROB256_MAX_VALUE) ? PROB10K_MAX_VALUE : (Prob10K_t)((pro)*39.0625f))	// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 256 È®·üÀ» 10K È®·ü·Î º¯°æ
#define MAX_DELAY_TIME_AT_EVENT_MONSTER		600		// 2008-04-17 by cmkwon, 600 ÃÊ = 10 ºÐ

typedef enum
{
	MGAME_EVENT_NO_EVENT				= 0,
	MGAME_EVENT_OPEN_BETA_ATTENDANCE	= 1,			// ¿ÀÇÂº£Å¸ Ãâ¼® ÀÌº¥Æ®
} MGAME_EVENT_t;

typedef USHORT	UID16_t;	// 2 Bytes Unique IDentifier, 0ÀÌ°Å³ª INVALID_UID16ÀÌ¸é Àß¸øµÈ UIDÀÓ, ±âÁ¸ÀÇ UniqueNumber¿¡ ÇØ´ç, 20040722, kelovon
typedef UINT32	UID32_t;	// 4 Bytes Unique IDentifier, 0ÀÌ°Å³ª INVALID_UID32ÀÌ¸é Àß¸øµÈ UIDÀÓ, ±âÁ¸ÀÇ UniqueNumber¿¡ ÇØ´ç, 20040722, kelovon
typedef UINT64	UID64_t;	// 8 Bytes Unique IDentifier, 0ÀÌ°Å³ª INVALID_UID64ÀÌ¸é Àß¸øµÈ UIDÀÓ, ±âÁ¸ÀÇ UniqueNumber¿¡ ÇØ´ç, 20040722, kelovon
#define INVALID_UID16		((UID16_t)~0)
#define INVALID_UID32		((UID32_t)~0)
#define INVALID_UID64		((UID64_t)~0)
#define INVALID_GUILD_UID	0				// 2005-02-04 by cmkwon

// Atum Defined SQL_C_XXX
#define SQL_C_PROB256		SQL_C_SHORT
#define SQL_C_PROB100		SQL_C_SHORT
#define SQL_C_PROB1000		SQL_C_SHORT
#define SQL_C_PROB10K		SQL_C_SHORT
#define SQL_C_PROB100K		SQL_C_LONG	
#define SQL_C_PROB1000K		SQL_C_LONG	
#define SQL_C_UID16			SQL_C_USHORT
#define SQL_C_UID32			SQL_C_ULONG
#define SQL_C_UID64			SQL_C_UBIGINT


// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ´ÏÆ¼ °ü·Ã Type Á¤ÀÇ
typedef INT			MSec_t;				// MilliSecond ´ÜÀ§ Ç¥Çö
typedef INT			HPActionUID_t;		// HPActionUID	Type
typedef INT			HPActionIdx_t;		// HPActionIndex Type
typedef BYTE		HPValueRate_t;		// HPValueRate Type
typedef INT			ItemIdx_t;			// ItemIndex Type
typedef BYTE		HPHitRate_t;		// HPHitRate Type
typedef INT			HPUseCount_t;		// HPUseCount Type	
typedef BYTE		HPTalkCondition_t;	// HPTalkCondition Type
typedef BYTE		HPTalkImportance_t; // HPTalkImportance Type
typedef char		HPTalk_t;			// HPTalk Type
typedef float		MonHP_t;			// MonsterHP Type
typedef	INT			CinemaNum_t;		// CinemaNum Type
typedef	INT			CinemaOrder_t;		// CinemaOrder Type
typedef	INT			RevisionNum_t;		// RevisionNum Type
typedef	USHORT		RevisionHP_t;		// RevisionHP Type
typedef	USHORT		RevisionDP_t;		// RevisionDP Type
typedef	INT			RevisionWeapon1_t;	// RevisionStandardWeapon Type
typedef	INT			RevisionWeapon2_t;	// RevisionAdvancedWeapon Type
typedef INT			EffectIdx_t;		// EffectIndex Type
typedef FLOAT		Position_t;			// Position Type
typedef INT			EffectPlayCount_t;	// EffectPlayCount Type
typedef short		ObjectIdx_t;		// ObjectIndex Type
typedef INT			ObjectNum_t;		// ObjectNumber Type
typedef INT			MonIdx_t;			// MonsterIdx Type
typedef BYTE		InfiMonTargetType_t;	// InfinityMonster Target Type
typedef BYTE		InfiMonTargetCount_t;	// InfinityMonster TargetCount Type
typedef INT			InfiModeUID_t;		// InfinityModeUID Type
typedef BYTE		ResetCycle_t;		// ResetCycle Type
typedef BYTE		EntranceCount_t;	// EntranceCount Type
typedef INT			Minute_t;			// Minute ´ÜÀ§ Ç¥Çö
typedef BYTE		Lv_t;				// Level Type
typedef BYTE		AdmissionCount_t;	// AdmissionCount Type
typedef UID64_t		ImputeUID_t;		// ImputeUID Type
typedef UID64_t		InfinityCreateUID_t;		// InfinityCreateUID Type
typedef BYTE		ChangeTarget_t;		// ChangeTarget Type
typedef UID32_t		KillCnt_t;			// KillCount Type
typedef UID32_t		DeathCnt_t;			// DeathCount Type
typedef UID32_t		TenderUID_t;		// TenderUID Type
typedef INT			ItemNum_t;			// ItemNum Type		// 2010-01-26 by cmkwon, Ä³½¬ ¾ÆÀÌÅÛ ÇÑÁ¤ÆÇ¸Å ½Ã½ºÅÛ ±¸Çö - Å¸ÀÔ Á¤ÀÇ Ãß°¡(±âÁ¸ UID32_t)
typedef BYTE		DiceCnt_t;			// DiceCount Type
typedef char		CinemaTalk_t;		// CinemaTalk Type
typedef	float		ParamValue_t;		// Item ParamValue Type
typedef	UID32_t		InfinityShopUID_t;	// InfinityShopUID Type			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ »óÁ¡
typedef UID32_t		InfinityShopItemCnt_t;	// InfinityShopItemCount Type // 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ »óÁ¡
typedef USHORT		DestParam_t;		// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø) - // DestParam Type	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
typedef INT			InvokingDestParamID_t;	// InvokingDestParamID Type	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
typedef UID64_t		ItemUID_t;			// ItemUID Type		// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
typedef char		FileName_t;			// FileName Type		// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) -
typedef INT			SummonCnt_t;		// SummonCount Type		// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) -
typedef INT			Distance_t;			// Distance Type		// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) -
typedef INT			QuestIndex_t;		// QuestIndex Type		// 2011-03-09 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ Äù½ºÆ® ÀÎµ¦½º ¼³Á¤ Ãß°¡
typedef	BYTE		KeyMonster_t;		// KeyMonster Type		// 2011-04-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
typedef INT			CustomIdx_t;		// CustomIdx Type		// 2011-04-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡
typedef INT			Penalty_t;			// Penalty Type			// 2011-06-14 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÆÐ³ÎÆ¼ ±â´É Ãß°¡ (HP ¹× ½Ã°£ µ¿½Ã Áö¿øÀ» À§ÇØ)					

// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
typedef INT			MapTriggerID_t;			// MapTriggerID Type
typedef INT			FunctionID_t;			// FunctionID Type
typedef INT			CrystalGroupID_t;		// CrystalGroupID Type
typedef INT			DestroyGroupID_t;		// DestroyGroupID Type
typedef INT			EventID_t;				// EventID Type
typedef SHORT		SequenceNumber_t;		// SequenceNumber Type
typedef INT			NGCInflWarGroupID_t;	// NGCInflWarGroupID Type
typedef INT			SummonMosterGroupID_t;	// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ - ¼ÒÈ¯ ¸ó½ºÅÍ ±×·ì ID

typedef enum
{
	MAP_TRIGGER_NONE			= 0,		// ¾øÀ½
	MAP_TRIGGER_CRYSTAL			= 1,		// Å©¸®½ºÅ» ½Ã½ºÅÛ
	MAP_TRIGGER_NGC_INFL_WAR	= 2			// NGC °ÅÁ¡Àü
	,MAP_TRIGGER_NGC_OUTPOST	= 3			// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ - ÀüÁø±âÁöÀü

} MapTriggerType_t;						// MapTriggerType Type
// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

// start 2012-01-16 by hskim, Åë°è - È­ÆÐ
typedef INT64		StatisticsMoneyUnit_t;	// Money Type			

typedef enum
{
	STATISTICS_SPI = 0,
	STATISTICS_WAR_POINT,
	STATISTICS_SYSTEM_ORB,
	STATISTICS_QUASAR_ORB,
	STATISTICS_CLUSTER_ORB,
	STATISTICS_GALAXY_ORG,
	STATISTICS_UNIVERSE_ORB,

	STATISTICS_KIND_END

} StatisticsMoneyKind;

#define STATISTICS_MONEY_FUNCTION_ADD	0		// È¹µæ
#define STATISTICS_MONEY_FUNCTION_SUB	1		// »ç¿ë
#define STATISTICS_MONEY_FUNCTION_MAX	2		// ÀüÃ¼ Function ¼ýÀÚ

struct SSTATISTICS_MONEY_UNIT
{
	StatisticsMoneyUnit_t System;
	StatisticsMoneyUnit_t Quasar;
	StatisticsMoneyUnit_t Cluster;
	StatisticsMoneyUnit_t Galaxy;
	StatisticsMoneyUnit_t Universe;
	StatisticsMoneyUnit_t SPI;

	void operator+=(const SSTATISTICS_MONEY_UNIT &MoneyOrg)
	{
		this->System += MoneyOrg.System;
		this->Quasar += MoneyOrg.Quasar;
		this->Cluster += MoneyOrg.Cluster;
		this->Galaxy += MoneyOrg.Galaxy;
		this->Universe += MoneyOrg.Universe;
		this->SPI += MoneyOrg.SPI;
	}

	void operator-=(const SSTATISTICS_MONEY_UNIT &MoneyOrg)
	{
		this->System -= MoneyOrg.System;
		this->Quasar -= MoneyOrg.Quasar;
		this->Cluster -= MoneyOrg.Cluster;
		this->Galaxy -= MoneyOrg.Galaxy;
		this->Universe -= MoneyOrg.Universe;
		this->SPI -= MoneyOrg.SPI;
	}
};
// end 2012-01-16 by hskim, Åë°è - È­ÆÐ

// start 2011-04-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡
typedef enum
{
	CINEMA_MONSTER_INDEX_CREATE				= 0,		// ¸ó½ºÅÍ »ý¼º
	CINEMA_MONSTER_INDEX_DESTORY			= 1,		// ¸ó½ºÅÍ ÆÄ±«
	CINEMA_MONSTER_INDEX_CHANGE				= 2,		// ¸ó½ºÅÍ ±³Ã¼
	CINEMA_MONSTER_INDEX_REGEN_ON			= 3,		// ¸ó½ºÅÍ ¸®Á¨ ON ¼³Á¤
	CINEMA_MONSTER_INDEX_REGEN_OFF			= 4			// ¸ó½ºÅÍ ¸®Á¨ OFF ¼³Á¤
} eCINEMA_MONSTER_INDEX_TYPE;
// end 2011-04-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡

// 2010-01-18 by cmkwon, ¾ÆÀÌÅÛ »ç¿ë½Ã Parameter Áßº¹ Ã¼Å© ½Ã½ºÅÛ ±¸Çö - ¾Æ·¡¿¡ ÀÖ´ø°ÍÀ» ¿©±â À§ÂÊÀ¸·Î ¿Å±è
typedef vector<INT>				vectINT;					// 2008-01-31 by cmkwon, ½Ã°£Á¦ÇÑ ¾ÆÀÌÅÛ DB ¿¡ Ãß°¡ Áß Ã¼Å© - 


// 2012-11-21 by bckim, ¿î¿µÅø±â´ÉÃß°¡, ÀÎÃ¦Æ®¾ÆÀÌÅÛÁ¦°Å±â´É
struct SUB_ENCHANT_INFO
{
	INT		nNumber;
	INT		nSubEnchantItemNum;
	INT		nEnchantCount;
	UINT64		SequenceNumber_DB;		// 2013-07-01 by bckim, 10ÀÎÃ¦Æ® ¹Ì¸¸ ÇÏÀÌÆÛÄ«µåÃ³¸®, ·Î±×Ãß°¡, ¿î¿µÅø º¸¿Ï
};
typedef vector<SUB_ENCHANT_INFO>	vectSUB_ENCHANT_INFO;
// 2012-11-21 by bckim, ¿î¿µÅø±â´ÉÃß°¡, ÀÎÃ¦Æ®¾ÆÀÌÅÛÁ¦°Å±â´É. End

///////////////////////////////////////////////////////////////////////////////
// START - Atum VECTOR3 Á¤ÀÇ
typedef SHORT		CoordUnit_t;			// ÁÂÇ¥ÀÇ ´ÜÀ§

typedef struct _AVECTOR3
{
	CoordUnit_t	x;
	CoordUnit_t	y;
	CoordUnit_t	z;

	_AVECTOR3() { Reset(); }
	_AVECTOR3(CoordUnit_t i_x, CoordUnit_t i_y, CoordUnit_t i_z)
	{
		x = i_x;
		y = i_y;
		z = i_z;
	}

	inline _AVECTOR3& operator=(const D3DXVECTOR3& rhs)
	{
		this->x = (CoordUnit_t)rhs.x;
		this->y = (CoordUnit_t)rhs.y;
		this->z = (CoordUnit_t)rhs.z;
		return *this;
	}

	inline D3DXVECTOR3& CopyA2DX(D3DXVECTOR3& outVec)
	{
		outVec.x = (float)this->x;
		outVec.y = (float)this->y;
		outVec.z = (float)this->z;
		return outVec;
	}

	inline void Reset()
	{
		x = y = z = 0;
	}
} AVECTOR3;		// Atum Protocol¿ë Vector

#define A2DX(vec3)		(D3DXVECTOR3(vec3.x, vec3.y, vec3.z))			// Convert AVECTOR3 to D3DXVECTOR3
#define AP2DX(pVec3)	(D3DXVECTOR3(pVec3->x, pVec3->y, pVec3->z))		// Convert AVECTOR3* to D3DXVECTOR3
// END - Atum VECTOR3 Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////

struct STRNBUF
{
	STRNBUF(int i_nSize) { szBuf = new char[i_nSize]; memset(szBuf, 0, i_nSize); nSize = i_nSize; }
	~STRNBUF() { if (szBuf != NULL) {delete(szBuf); szBuf = NULL;} nSize = 0; }

	char* GetBuffer() { return szBuf; }
	int GetSize() { return nSize; }

private:
	char	*szBuf;
	int		nSize;
};

inline float GGetLength(float x, float z)
{
	return sqrtf(x * x + z * z);
}

extern char CONFIG_ROOT[1024];

#ifndef __NPCID_T__
#define __NPCID_T__
typedef INT	NPCID_t;
typedef INT	NPCScriptID_t;
#endif// end__NPCID_T__

#define MAKEUINT32(low, high) ( (UINT)((USHORT)(low)) | ((UINT)((USHORT)(high)) << 16) )
#define MAKEUINT64(low, high) ( (UINT64)((UINT)(low)) | ((UINT64)((UINT)(high)) << 32) )

#define VALID_ATTACK_DISTANCE					100.0f		// Lock On, Attack °è»ê½Ã¿¡ ÇÊ¿äÇÑ ±âº» °Å¸®°ª
#define VALID_ATTACK_PINPOINT					0.523f		// 3.14 * 1/6 ==> 30µµ

#define SIZE_OTHER_SERVER_LIST					20			// ´Ù¸¥ ¼­¹öµéÀ» À§ÇØ IOCPSocketÀÇ Array Server¿¡ ÇÒ´ç

#define SIZE_MAX_CLIENT_REPORT					256
#define SIZE_MAX_VERSION						4		// Client Version
#define SIZE_MIN_CHARACTER_NAME					2		// 2006-03-16 by cmkwon, ÃÖ¼Ò Ä³¸¯ÅÍ¸í Character Name
#define SIZE_MAX_CHARACTER_NAME					20		// Character Name
#define SIZE_MAXUSE_CHARACTER_NAME				15		// 2006-05-19 by cmkwon, ÃÖ´ë´Â 20bytes ÀÌÁö¸¸ ½ÇÁ¦·Î´Â 15Bytes±îÁö¸¸ »ç¿ë °¡´É
#define SIZE_MAX_NICK_NAME						SIZE_MAX_CHARACTER_NAME		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
#define SIZE_MAX_INFLUENCE_OR_STAFFPREFIX		12

// 2008-10-08 by cmkwon, ´ë¸¸ 2´Ü°è °èÁ¤ ½Ã½ºÅÛ Áö¿ø ±¸Çö(email->uid) - 
#if !defined(SIZE_MAX_ACCOUNT_NAME)
#define SIZE_MAX_ACCOUNT_NAME					20		// 1Â÷ °èÁ¤ ÀÌ¿Ü¿¡ »ç¿ëµÉ 2Â÷ °èÁ¤ »çÀÌÁî, ´ëºÎºÐÀÌ ÀÌ°ÍÀ» »ç¿ëÇÑ´Ù.
#endif
#if !defined(SIZE_MAX_ORIGINAL_ACCOUNT_NAME)
#define SIZE_MAX_ORIGINAL_ACCOUNT_NAME			20		// 2008-10-08 by cmkwon, ´ë¸¸ 2´Ü°è °èÁ¤ ½Ã½ºÅÛ Áö¿ø ±¸Çö(email->uid) - ÀÎÁõ½Ã¿¡¸¸ »ç¿ëµÉ 1Â÷ °èÁ¤ »çÀÌÁî
#endif

#define SIZE_MAX_CITY_NAME						40
#define SIZE_MAX_PASSWORD						20		// 6 ~ 16, ÀÌ °ªÀ» ¹Ù²Ù¸é FtpPassword¶§¹®¿¡ UpdateFileList¸¦ ¸ðµÎ ¹Ù²ã¾ß ÇÏ¹Ç·Î ÁÖÀÇ ¿ä!
#define SIZE_MAX_PASSWORD_MD5					16		//
#define SIZE_MAX_PASSWORD_MD5_STRING			33		// 32 characters
#define SIZE_MAX_GUILD_NAME						30
#define SIZE_MAX_GUILD_MARK_IMAGE				1600	// bytes, 20x20 4Bytes image
#define SIZE_MAX_SERVER_NAME					20		// SIZE_MAX_SERVER_GROUP_NAME
#define SIZE_MAX_MAP_NAME						40
#define SIZE_MAX_MAP_DESCRIPTION				1024		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
#define SIZE_MAX_PET_NAME						40		// 2010-06-16 by hslee, 20 --> 40
#define SIZE_MAX_PET_OPERATOR_DESCRIPTION		128		// 2010-12-03 by shcho, Æê ½Ã½ºÅÛ °ü·Ã ½ºÆ®¸µ ·ÎÄÃ¸®Á¦ÀÌ¼Ç Ãß°¡
#define SIZE_MAX_MONSTER_NAME					40		// 2005-12-19 by cmkwon, 20 --> 40
#define SIZE_MAX_BUILDING_NAME					40		// 2008-01-03 by cmkwon, BuildingName ÇÊµå 40Bytes ·Î ¼öÁ¤ - , // = SIZE_MAX_SHOP_NAME
// 2008-01-03 by cmkwon, BuildingName ÇÊµå 40Bytes ·Î ¼öÁ¤ - »ç¿ëÇÏÁö ¾ÊÀ½
//#define SIZE_MAX_SHOP_NAME						20		// = SIZE_MAX_BUILDING_NAME
#define SIZE_MAX_NPC_NAME						40
#define SIZE_MAX_NPC_TALK_STRING				2000	// 2005-10-18 by cmkwon, 1000¿¡¼­ 2000À¸·Î º¯°æÇÔ, string1024¸¦ string2048·Î ¼öÁ¤ÇßÀ½
#define SIZE_MAX_BUILDING_NPC_TALK_STRING		1000	// 2005-10-18 by cmkwon, ÀÌÀü¿¡´Â SIZE_MAX_NPC_TALK_STRING¸¦ °°ÀÌ »ç¿ëÇÔ
#define SIZE_MAX_QUEST_NAME						40
#define SIZE_MAX_RARE_FIX_NAME					30
#define SIZE_MAX_ITEM_NAME						40
#define SIZE_MAX_ITEM_DESCRIPTION				200
#define SIZE_MAX_NUM_CHARACTER					3
#define SIZE_MAX_CHAT_MESSAGE					100			// 2006-05-04 by cmkwon, 60-->100
#define SIZE_MAX_ERROR_STRING					60
#define SIZE_MAX_A_LOG_LINE						1400		// check: SIZE_MAX_PACKET - 92 !!!
#define SIZE_MAX_LOG_ACTION_STRING				1000		// check: SIZE_MAX_A_LOG_LINE - N
#define SIZE_MAX_WORKSPACE_FILE_NAME			40			// Map¿¡¼­ Workspace File Name Size
#define SIZE_MAX_PROJECT_FILE_NAME				40			// Map¿¡¼­ Project File Name Size
#define SIZE_MAX_EVENT_PARAM_NAME				40			// Map¿¡¼­ Event Param Name Size
#define SIZE_MAX_MONSTER_REGION_NAME			40			// Map¿¡¼­ Event Param Name Size
//#define SIZE_MAX_NUM_MEMBER_IN_PARTY			8
#define HEIGHT_FLYING_ALTITUDE					30
#define SIZE_MAX_MAP_PER_A_FIELD_SERVER			100
#define SIZE_MAX_BLOCK_ELEMENT					100
#define MONSTER_CLIENT_INDEX_START_NUM			((ClientIndex_t)10000)		// monsterÀÇ client indexÀÇ ½ÃÀÛ ¹øÈ£
#define MONSTER_CLIENT_INDEX_END_NUM			((ClientIndex_t)15000)		// monsterÀÇ client indexÀÇ ½ÃÀÛ ¹øÈ£
#define SIZE_MAX_WARP_TARGET_NAME				40
#define SIZE_MAX_CITY_BRIEFING_LENGTH			100
#ifndef SIZE_MAX_IPADDRESS
	#define SIZE_MAX_IPADDRESS					16		// string IP Address
#endif
#define SIZE_MAX_OBJECT_NAME					40
#define SIZE_MAX_COUPON_NUMBER					30
#define SIZE_MAX_BLOCKED_ACCOUNT_REASON			200
#define SIZE_MAX_SERVICE_COUNTRY				40		// 2011-12-12 by hskim, GLog 2Â÷
#define SIZE_MAX_WEB_AUTHENTICATION_KEY			65		// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡
#define SIZE_MAX_TEMP_SYSTEM_STRING				1024	// 2013-05-20 by hskim, [º¸¾È ½Ã½ºÅÛ] ºñÁ¤»óÀûÀÎ ¹æ¹ýÀ¸·Î ¾ÆÀÌÅÛ Ãß°¡ ¹æÁö

#define SIZE_MAX_PACKET_PADDING					2		// 2012-12-22 by hskim, ÀÎÁõ ¼­¹ö ±¸Çö - ±âÁ¸ ¼­¹ö¿Í È£È¯ ¾ÈµÇµµ·Ï ±¸Á¶Ã¼ Å©±â ¹Ù²Þ

#define SIZE_MAX_MAC_ADDRESS					24		// 2015-09-23 Future, maximum string length representation of a MAC Addres (8 address parts * 3 digits)

// 2006-09-04 by cmkwon, LocalizationDefineCommon.h È­ÀÏ¿¡ Á¤ÀÇ
//#define SIZE_MAX_GUILD_MEMBER_CAPACITY			60		// ÃÖ´ë °¡´É ±æµå¿ø ¼ö
//#define SIZE_MAX_INITIAL_GUILD_CAPACITY			40		// ÃÊ±â ±æµå »ý¼º ½Ã °¡´É ±æµå¿ø ¼ö
#define ROLLING_USE_INTERVAL_TICK				3000	// ·Ñ¸µ Àç»ç¿ë ½Ã°£, // 2005-11-18 by cmkwon, 4ÃÊ->3ÃÊ
#define ROLLING_USE_CONSUMPTION_SP				3		// ·Ñ¸µ »ç¿ë½Ã ¼Ò¸ðµÇ´Â SP, // 2005-11-18 by cmkwon, 5->3


#define MAX_ITEM_COUNTS							2000000000					// ÃÖ´ë ¾ÆÀÌÅÛ Ä«¿îÆ®(20¾ï)
#define MAX_NOTIFY_ITEM_COUNTS					(MAX_ITEM_COUNTS-100000000)	// ÃÖ´ë °æ°í ¾ÆÀÌÅÛ Ä«¿îÆ®(19¾ï = 20¾ï-1¾ï)
#define ITEM_NOT_USING							0							// »ç¿ë½Ã°£ÀÌ ÀÖ´Â ¾ÆÀÌÅÛÀÏ °æ¿ì »ç¿ëÁßÀÌ ¾Æ´Ï´Ù
#define	MAX_ITEM_COUNTS_FOR_BUY					10							// 2008-07-16 by cmkwon, ITEM_GENERAL °ü·Ã ¹ö±× Ã¼Å© - »óÁ¡¿¡¼­ ³ÍÄ«¿îÆ®ºí ¾ÆÀÌÅÛ ±¸ÀÔ °¡´ÉÇÑ ÃÖ´ë °³¼ö, ÆÇ¸Å´Â »ó°ü ¾øÀ½.

#define SIZE_MAX_SZQUERY						2000			// 2007-01-30 by dhjin, Äõ¸® ¿Ï¼ºÀ» À§ÇÑ ½ºÆ®¸µ Å©±â
#define SIZE_MAX_TABLE_NAME						128				// 2008-08-27 by cmkwon, ³¯Äõ¸®¸¦ Procedure·Î ¼öÁ¤ - 

// Client Index Á¤ÀÇ
#if defined(_ATUM_FIELD_SERVER) || defined(_ATUM_NPC_SERVER)
#define CLIENT_INDEX_START_NUM					((ClientIndex_t)SIZE_MAX_UDP_CHANNEL_SESSION)	// 2008-08-29 by cmkwon, FieldServ<->NPCServ °¡ UDP Åë½Å ÇÒ Ã¤³Î¼ö - // client indexÀÇ ½ÃÀÛ ¹øÈ£
#elif defined(_ATUM_IM_SERVER)
#define CLIENT_INDEX_START_NUM					((ClientIndex_t)1)		// client indexÀÇ ½ÃÀÛ ¹øÈ£
#else
#define CLIENT_INDEX_START_NUM					((ClientIndex_t)0)		// 2008-08-29 by cmkwon, FieldServ<->NPCServ °¡ UDP Åë½Å ÇÒ Ã¤³Î¼ö - // client indexÀÇ ½ÃÀÛ ¹øÈ£
#endif
#define CLIENT_INDEX_FIELDSERVER_NUM			60001					// 2011-03-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡ - field server index ¹øÈ£ - ¹ß»çµÈ 2Çü ¹«±âÀÇ ÇÊµå ¼­¹ö Ã³¸®¸¦ À§ÇØ (¼±ÅÃµÈ À¯Àú°¡ ¾øÀ½)
#define INVALID_CLIENT_INDEX					((ClientIndex_t)~0)
#define IS_CHARACTER_CLIENT_INDEX(x)			((ClientIndex_t)(x) >= CLIENT_INDEX_START_NUM && (ClientIndex_t)(x) < MONSTER_CLIENT_INDEX_START_NUM)	// characterÀÎÁö È®ÀÎ
#define IS_MONSTER_CLIENT_INDEX(x)				((ClientIndex_t)(x) >= MONSTER_CLIENT_INDEX_START_NUM && (ClientIndex_t)(x) < 20000)					// monsterÀÎÁö È®ÀÎ, check: 20000ÀÌ¸é ÃæºÐÇÏ´Ù°í »ý°¢ÇÔ, 2003103, kelovon w/ chkwon
#define IS_VALID_CLIENT_INDEX(idx)				(idx >= CLIENT_INDEX_START_NUM && idx < MONSTER_CLIENT_INDEX_START_NUM && idx != INVALID_CLIENT_INDEX && idx != 0)
#define IS_VALID_CHARACTER_AND_MONSTER_INDEX(idx)				(idx >= CLIENT_INDEX_START_NUM && idx < 20000 && idx != INVALID_CLIENT_INDEX && idx != 0)			// 2009-09-09 ~ 2010-01-11 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ°£ Å¸°Ù ¼³Á¤À» À§ÇØ ¼öÁ¤			
#define INVALID_UNIQUE_NUMBER					(UINT)(~0)
#define IS_VALID_UNIQUE_NUMBER(num)				((num) != 0 && (num) != INVALID_UNIQUE_NUMBER)
#define IS_VALID_UID64(num)						((num) != 0 && (num) != INVALID_UID64)				// 2006-10-11 by cmkwon
#define SIZE_MAX_VERSION_STRING					64
// 2006-09-06 by cmkwon, LocalizationDefineCommon.h È­ÀÏ¿¡ Á¤ÀÇ
//#define CHARACTER_MAX_LEVEL						100
#define PI										3.1415926535f
#define SIZE_MAX_LONG_PACKET					200000	// ±ä packetÀ» º¸³¾ ¶§ »ç¿ëÇÒ bufferÀÇ »çÀÌÁî, »ç¿ë½Ã Å©±â ºñ±³ ÇÊ¿äÇÔ
#define DEFAULT_POSITION_X						100
#define DEFAULT_POSITION_Y						HEIGHT_FLYING_ALTITUDE
#define DEFAULT_POSITION_Z						100
#define SPEED_INCREASE_AMOUNT					19
#define REQUERED_TRADE_DISTANCE					500			// check: Á¤È®ÇÑ °Å¸® Á¤ÇÏ±â
#define REQUERED_P2P_PK_DISTANCE				1500			// check: Á¤È®ÇÑ °Å¸® Á¤ÇÏ±â
#define REQUERED_REQUEST_DISTANCE				1500			// check: Á¤È®ÇÑ °Å¸® Á¤ÇÏ±â
// 2005-12-08 by cmkwon, Á¦°ÅµÊ
//#define POSSESS_STORE_NUMBER					0			// Ã¢°í¿¡ ÀÖÀ» ¶§ possess ¼öÄ¡
#define POSSESS_AUCTION_NUMBER					((UID32_t)0xFFFFFFFF)	// °æ¸Å ÁßÀÏ ¶§ possess ¼öÄ¡, -1 = 0xFFFFFFFF
#define CHARACTER_SIZE							15.0f		// Ä³¸¯ÅÍ ¹ÝÁö¸§
#define INVALID_MAP_NAME						""
#define MAP_INDEX_ALL							(USHORT)(~0)
#define ATUM_LOAD_TEST_PREFIX_ACCOUNT_NAME			"*loadtest"
#define ATUM_LOAD_TEST_PREFIX_ACCOUNT_NAME_SIZE		9
#define SIZE_MAX_PARTY_MEMBER					10			// ÃÖ´ë ÆÄÆ¼¿ø ¼ö
#define SIZE_MAX_INITIAL_SUM_OF_STAT			24
#define SIZE_MAX_ENCHANT_PER_ITEM				40			// ÇÑ ¾ÆÀÌÅÛ¿¡ ºÙÀÏ ¼ö ÀÖ´Â ÃÖ´ë ÀÎÃ¦Æ® ¼ö
#define SIZE_MAX_ENCHANT_USE_ITEMKIND_PREVENTION_DELETE_ITEM	40	// ÆÄ¹æ(ÆÄ°ú¹æÁö) »ç¿ë °¡´ÉÇÑ ÃÖ´ë ÀÎÃ¦Æ®¼ö, // 2006-08-23 by cmkwon, 8 --> 40
#define SIZE_REMAIN_ENCHANT_COUNT_BY_PREVENTION_DELETE_ITEM		5	// ÀÎÃ¦Æ®½Ã ÆÄ¹æ(ÆÄ°ú¹æÁö) »ç¿ëÇÏ¸é ³²´Â ÀÎÃ¦Å© Ä«¿îÆ®
#define IS_VALID_PLANE_COORDINATES(x, z, maxX, maxZ)	((x) >= 0 && (z) >= 0 && (x) < (maxX) && (z) < (maxZ))
#define ACOS(fVal)									(((fVal) > 1.0f) ? acos(1.0f) : acos(fVal))
#define SIZE_MAX_FTP_URL							64
#define DOWNLOAD_BUFFER_SIZE						4096
#define SIZE_MAX_FTP_FILE_PATH						256
#define SIZE_MAX_FRIENDLIST_COUNT					60		// 2006-07-18 by cmkwon, 50 --> 60
#define SIZE_MAX_REJECTLIST_COUNT					60		// 2006-07-18 by cmkwon, 50 --> 60
#define SIZE_CITY_POSITION_RANDOM_XZ_RADIUS			30		// 2005-12-21 by cmkwon, 50 --> 30		

// 2006-09-04 by cmkwon, LocalizationDefineCommon.h È­ÀÏ¿¡ Á¤ÀÇ
//#define SIZE_MAX_ITEM_GENERAL						61		// Ä³¸¯ÅÍÀÇ ÀÎº¥Åä¸®¿¡ ¼ÒÀ¯ÇÒ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ ÃÖ´ë °³¼ö(1°³´Â SPI ¾ÆÀÌÅÛÀÇ Ä«¿îÆ®ÀÌ´Ù, Å¬¶óÀÌ¾ðÆ®´Â 60À» »ç¿ëÇÑ´Ù.)
//#define SIZE_MAX_ITEM_GENERAL_IN_STORE				101		// Ã¢°í¿¡ ¼ÒÀ¯ÇÒ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÀÇ ÃÖ´ë °³¼ö

#define TUTORIAL_MAP_INDEX							8001	// Æ©Åä¸®¾ó¸ÊÀº ÇÏ³ªÀÌ¹Ç·Î defineÇÑ´Ù
#define VCN_CITY_MAP_INDEX							2001	// ÀÏ¹Ý±º, Á¤±Ô±º µµ½Ã¸ÊÀÎµ¦½º
#define ANI_CITY_MAP_INDEX							2002	// ¹Ý¶õ±º µµ½Ã¸ÊÀÎµ¦½º
// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - Á¦°Å
//#define NORMAL_CITY_MAP_INDEX						2003	// 2007-05-21 by cmkwon, ÀÏ¹Ý±º µµ½Ã¸ÊÀÎµ¦½º - ¾ÆÄ«µ¥¹Ì¸Ê
#define INFLUENCE_CITY_MAP_INDEX					9999	// °¢ ¼¼·Âº° µµ½Ã¸ÊÀÎµ¦½º
#define INFLUENCE_GARDEN_MAP_INDEX					9998	// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - °¢ ¼¼·Âº° ¾Õ¸¶´ç(Garden)
#define VCN_CONFERENCEROOM_MAP_INDEX				1001	// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - Á¤±Ô±º È¸ÀÇ·ë ¸ÊÀÎµ¦½º
#define ANI_CONFERENCEROOM_MAP_INDEX				1002	// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - ¹Ý¶õ±º È¸ÀÇ·ë ¸ÊÀÎµ¦½º

#define STONES_RUIN_MAP_INDEX						3002	// 2012-07-02 by hskim, ¸ð¼±Àü °³¼± - ¸ð¼±Àü ÁøÇà½Ã ½ºÅæÁî ·çÀÎ ÁøÀÔ ºÒ°¡
#define TYLENT_JUNGLE_MAP_INDEX						3003	// 2012-07-02 by hskim, ¸ð¼±Àü °³¼± - ¸ð¼±Àü ÁøÇà½Ã ½ºÅæÁî ·çÀÎ ÁøÀÔ ºÒ°¡

#define HELL_MAP_1ST								4401	// 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ - Áö¿Á¸Ê2
#define HELL_MAP_2ND								4403	// 2012-05-08 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ 2Â÷ - Áö¿Á¸Ê 2

#define IS_HELL_2ND_MAP(MapIndex) (((MapIndex) == HELL_MAP_1ST) || ((MapIndex) == HELL_MAP_2ND))		// 2012-05-08 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ 2Â÷ - Áö¿Á¸Ê 2

// 2006-07-03 by cmkwon, 1000¹ø´ë´Â °³ÀÎ»óÁ¡¸ÊÀ¸·Î »ç¿ë ÇÒ °ÍÀÓ
//#define IS_CITY_MAP_INDEX(__MAP_INDEX)				(((MapIndex_t)__MAP_INDEX)/1000==1)		// °Ý³³°í¸Ê
#define IS_CONFERENCEROOM_MAP_INDEX(__MAP_INDEX)	( (VCN_CONFERENCEROOM_MAP_INDEX==(__MAP_INDEX)) || (ANI_CONFERENCEROOM_MAP_INDEX==(__MAP_INDEX)) )			// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - ¸ÊÀÎµ¦½º Á¤ÀÇ Ãß°¡ÇÔ
#define IS_BAZAAR_MAP_INDEX(__MAP_INDEX)			(IS_CONFERENCEROOM_MAP_INDEX(__MAP_INDEX))	// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - ±âÁ¸  ¹ÙÀÚ¸ÊÀ» ÀÌ¿ëÇÔ,	// 2006-07-19 by cmkwon, °³ÀÎ»óÁ¡¸Ê
#define IS_CITYWAR_MAP_INDEX(__MAP_INDEX)			(((MapIndex_t)__MAP_INDEX)/1000==5)		// µµ½ÃÁ¡·ÉÀüÀÌ ÀÏ¾î³ª´Â ¸Ê
#define IS_SPACE_MAP_INDEX(__MAP_INDEX)				(((MapIndex_t)__MAP_INDEX)/1000==6)		// ¿ìÁÖ¸Ê
#define IS_TUTORIAL_MAP_INDEX(__MAP_INDEX)			((__MAP_INDEX)==TUTORIAL_MAP_INDEX)		// Æ©Åä¸®¾ó¸Ê
#define IS_RACING_MAP_INDEX(__MAP_INDEX)			(((MapIndex_t)__MAP_INDEX)/1000==9)		// ·¹ÀÌ½Ì¸Ê
#define IS_VCN_CITY_MAP_INDEX(__MAP_INDEX)			((__MAP_INDEX) == VCN_CITY_MAP_INDEX)	// ÀÏ¹Ý±º, Á¤±Ô±º µµ½Ã¸ÊÀÎµ¦½ºÀÎ°¡?
#define IS_ANI_CITY_MAP_INDEX(__MAP_INDEX)			((__MAP_INDEX) == ANI_CITY_MAP_INDEX)	// ¹Ý¶õ±º µµ½Ã¸ÊÀÎµ¦½ºÀÎ°¡?
// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - Á¦°Å
//#define IS_NORMAL_CITY_MAP_INDEX(__MAP_INDEX)		((__MAP_INDEX) == NORMAL_CITY_MAP_INDEX)	// 2007-05-21 by cmkwon, ÀÏ¹Ý±º µµ½Ã¸ÊÀÎµ¦½ºÀÎ°¡?
#define IS_OUTPOST_CITY_MAP_INDEX(__MAP_INDEX)		(2004 == (__MAP_INDEX) || 2005 == (__MAP_INDEX) || 2006 == (__MAP_INDEX) || 2007 == (__MAP_INDEX))	// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ - 2007 µµ½Ã¸Ê µî·Ï		// 2008-09-10 by dhjin, 2006Ãß°¡ Â÷ÈÄ MAP_INFLUENCE_OUTPOST = 6000°ªÀ¸·Î º¯°æÇØ¾ßÇÒµíÇÏ´Ù.	// 2007-09-10 by dhjin, ÀüÁø±âÁö µµ½Ã¸Ê ÀÎ°¡? 
#define IS_STONES_RUIN_MAP_INDEX(__MAP_INDEX)			((__MAP_INDEX) == STONES_RUIN_MAP_INDEX)	// 2012-07-02 by hskim, ¸ð¼±Àü °³¼± - ¸ð¼±Àü ÁøÇà½Ã ½ºÅæÁî ·çÀÎ ÁøÀÔ ºÒ°¡
#define IS_TYLENT_JUNGLE_MAP_INDEX(__MAP_INDEX)			((__MAP_INDEX) == TYLENT_JUNGLE_MAP_INDEX)	// 2012-07-02 by hskim, ¸ð¼±Àü °³¼± - ¸ð¼±Àü ÁøÇà½Ã ½ºÅæÁî ·çÀÎ ÁøÀÔ ºÒ°¡

// 2008-02-12 by cmkwon, IS_CITY_MAP_INDEX() ¸ÅÅ©·Î¸¦ Æ÷°ýÀûÀÎ µµ½Ã¸ÊÀÎÁö ¿©ºÎ¸¦ ¸®ÅÏ
// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - ÀÏ¹Ý¼¼·Â µµ½Ã¸Ê Á¦¿Ü
//#define IS_CITY_MAP_INDEX(__MAP_INDEX)				(IS_VCN_CITY_MAP_INDEX(__MAP_INDEX) || IS_ANI_CITY_MAP_INDEX(__MAP_INDEX) || IS_NORMAL_CITY_MAP_INDEX(__MAP_INDEX) || IS_CONFERENCEROOM_MAP_INDEX(__MAP_INDEX) || IS_OUTPOST_CITY_MAP_INDEX(__MAP_INDEX))		// °Ý³³°í¸Ê - // 2007-05-21 by cmkwon, IS_NORMAL_CITY_MAP_INDEX() Ãß°¡ÇÔ
#define IS_CITY_MAP_INDEX(__MAP_INDEX)				(IS_VCN_CITY_MAP_INDEX(__MAP_INDEX) || IS_ANI_CITY_MAP_INDEX(__MAP_INDEX) || IS_CONFERENCEROOM_MAP_INDEX(__MAP_INDEX) || IS_OUTPOST_CITY_MAP_INDEX(__MAP_INDEX))		// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - ,// °Ý³³°í¸Ê - // 2007-05-21 by cmkwon, IS_NORMAL_CITY_MAP_INDEX() Ãß°¡ÇÔ

// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - ÀÏ¹Ý¼¼·Â µµ½Ã¸Ê Á¦¿Ü
//#define IS_INFLUENCE_CITY_MAP_INDEX(__MAP_INDEX)	(IS_VCN_CITY_MAP_INDEX(__MAP_INDEX) || IS_ANI_CITY_MAP_INDEX(__MAP_INDEX) || IS_NORMAL_CITY_MAP_INDEX(__MAP_INDEX))		// 2008-02-12 by cmkwon, ¼¼·Âº° µµ½Ã¸Ê
#define IS_INFLUENCE_CITY_MAP_INDEX(__MAP_INDEX)	(IS_VCN_CITY_MAP_INDEX(__MAP_INDEX) || IS_ANI_CITY_MAP_INDEX(__MAP_INDEX))		// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - , // 2008-02-12 by cmkwon, ¼¼·Âº° µµ½Ã¸Ê

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÆÃ±ä À¯Àú ÀçÁ¢¼Ó Ã³¸®
#define INFINITY_MAP_INDEX				9200
#define	INFINITY_MAP_INDEX_LAST			9299
#define IS_INFINITY_MAP_INDEX(__MAP_INDEX)		( INFINITY_MAP_INDEX <= (__MAP_INDEX) && INFINITY_MAP_INDEX_LAST >= (__MAP_INDEX) )


#define MONEY_ITEM_NUMBER							7000022		// ½ºÇÇ(SPI) ¾ÆÀÌÅÛ³Ñ¹ö
#define ITEM_NUM_CASH_NORMAL_PREMIUM_CARD			7001120		// ÀÏ¹Ý ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛ³Ñ¹ö
// 2006-07-05 by cmkwon, MonthlyPay ½Ã½ºÅÛ¿¡´Â ÇÊ¿ä ¾ø´Â define
//#define ITEM_NUM_CASH_SUPER_PREMIUM_CARD			7001130		// ½´ÆÛ ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛ³Ñ¹ö
//#define ITEM_NUM_CASH_UPGRADE_PREMIUM_CARD			7001140		// ½´ÆÛ ¾÷±×·¹ÀÌµå ¾ÆÀÌÅÛ³Ñ¹ö
#define ITEM_NUM_APLUS_GRADE_HP_KIT					7010430		// A+±Þ¼ö¸®Å°Æ®		- Å¬¶óÀÌ¾ðÆ®¿¡¼­¸¸ »ç¿ë
#define ITEM_NUM_APLUS_GRADE_DP_KIT					7010440		// A+±Þ½¯µåÅ°Æ®		- Å¬¶óÀÌ¾ðÆ®¿¡¼­¸¸ »ç¿ë
#define ITEM_NUM_S_GRADE_HP_KIT						7010290		// S±Þ¼ö¸®Å°Æ®		- Å¬¶óÀÌ¾ðÆ®¿¡¼­¸¸ »ç¿ë
#define ITEM_NUM_S_GRADE_DP_KIT						7010300		// S±Þ½¯µåÅ°Æ®		- Å¬¶óÀÌ¾ðÆ®¿¡¼­¸¸ »ç¿ë
#define ITEM_NUM_PARTNER							72300		// 2012-03-15 by jhseol, ÆÄÆ®³Ê·ù

#define ITEM_NUM_NEUTRON_RECHARGEABLE_BATTERY_MKIII	7026260		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // ´ºÆ®·Ð ÃæÀüÁö MKIII
#define ITEM_NUM_NEUTRON_RECHARGEABLE_BATTERY_MKII	7026280		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // ´ºÆ®·Ð ÃæÀüÁö MKII
#define ITEM_NUM_NEUTRON_RECHARGEABLE_BATTERY_MKI	7026290		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // ´ºÆ®·Ð ÃæÀüÁö MKI
#define ITEM_NUM_DEUS_EX_MACHINA_MKIII				7026300		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // µ¥¿ì½º ¿¢½º ¸¶Å°³ª MKIII
#define ITEM_NUM_DEUS_EX_MACHINA_MKII				7026310		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // µ¥¿ì½º ¿¢½º ¸¶Å°³ª MKII
#define ITEM_NUM_DEUS_EX_MACHINA_MKI				7026320		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸º¹ ¾ÆÀÌÅÛ Ãß°¡ // µ¥¿ì½º ¿¢½º ¸¶Å°³ª MKI


#define ITEM_NUM_BRIGHTNESS_OF_BCU_LEADER			7001320		// 2007-05-02 by dhjin, 'ÁöµµÀÚÀÇ ±¤ÈÖ' ¾ÆÀÌÅÛ ¹øÈ£ 
#define ITEM_NUM_BRIGHTNESS_OF_ANI_LEADER			7001330		// 2007-05-02 by dhjin, 'ÁöµµÀÚÀÇ ±¤ÈÖ' ¾ÆÀÌÅÛ ¹øÈ£ 

// start 2012-01-16 by hskim, Åë°è - È­ÆÐ
#define ITEM_NUM_SYSTEM_ORB							7025990
#define ITEM_NUM_QUASAR_ORB							7026000
#define ITEM_NUM_CLUSTER_ORB						7026010
#define ITEM_NUM_GALAXY_ORG							7026020
#define ITEM_NUM_UNIVERSE_ORB						7026030
// end 2012-01-16 by hskim, Åë°è - È­ÆÐ

#define SIZE_MAX_ARMOR_COLOR_COUNT						10			// 2005-12-08 by cmkwon, ¾Æ¸Ó°¡ °¡Áú¼ö ÀÖ´Â ÃÖ´ë »ö»ó °³¼ö
#define COLORItemNum_TO_ArmorSourceIndex(itemNum)		(((int)((itemNum)/100))*100)		// »ö»ó¾ÆÀÌÅÛÀÇ ItemNumÀ» ¾Æ¸ÓÀÇ SourceIndex·Î º¯È¯ÇÏ±â
#define ARMORSourceIndex_TO_BaseColorItemNum(itemNum)	(((int)((itemNum)/100))*100 + 1)	// ¾Æ¸ÓÀÇ SourceIndex·Î ±âº»»ö»ó¾ÆÀÌÅÛÀÇ ItemNumÀ¸·Î º¯È¯ÇÏ±â

#define TERM_ENABLE_CASH_PREMIUM_CARD_BUY_MONTH		1			// ÇÑ´Þ´õ Ãß°¡·Î ±¸ÀÔ °¡´ÉÇÏ´Ù
#define TERM_MONTHLYPAY_MAX_BUY_ENABLE_DAY			180			// MonthlyPay ±¸ÀÔ °¡´É ÃÖ´ë ³¯Â¥

#define CASH_SHOP_BUILDING_INDEX					9999		// À¯·áÈ­ »óÁ¡ BuindNPCIndex
#define FIELD_STORE_SHOP_BUILDING_INDEX				9998		// 2009-09-23 by cmkwon, ÇÊµåÃ¢°í Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - ÇÊµåÃ¢°í BuildingIndex

#define DEFAULT_OBJECT_MONSTER_OBJECT				90000000
#define IS_DEFAULT_OBJECT_MONSTER_OBJECT(num)		(  (int)((num)/10000000) == 9  )

#define DEFAULT_CHANGE_OBJECT_ADD_INDEX				90000000	// 2011-12-19 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ - º¯°æµÈ ±³Ã¼ ¿ÀºêÁ§Æ®ÀÇ °íÁ¤µÈ Ãß°¡ ÀÎµ¦½º °ª

#define TIMEGAP_CITYWAR_CHANGEABLE_TERM_HOUR		4		// ¿©´ÜÀåÀÌ º¯°æ °¡´ÉÇÑ(µðÆúÆ® ½Ã°£ÀÌÈÄºÎÅÍ) ´ÙÀ½ µµ½ÃÁ¡·ÉÀü ½Ã°£
#define CITYWAR_MINIMUM_TEX_PERCENT					2.0f	// µµ½ÃÁ¡·ÉÀü »óÁ¡ ÀÌ¿ë ÃÖ¼Ò tex
#define CITYWAR_MAXIMUM_TEX_PERCENT					100.0f	// µµ½ÃÁ¡·ÉÀü »óÁ¡ ÀÌ¿ë ÃÖ´ë tex

#define TICKGAP_SEND_INVASION_MSG					300000	// Ä§·« ¸Þ½ÃÁö Àü¼Û ÅÒ, ´ÜÀ§:ms, 300000 = 300ÃÊ = 5ºÐ
#define MAXIMUM_USABLE_FAME_POINT_FOR_ONE_DAY		20		// 2005-12-29 by cmkwon, ÇÏ·ç¿¡ ÇÑ¸í¿¡°Ô ¹ÞÀ»¼ö ÀÖ´Â ÃÖ´ë ¸í¼ºÄ¡

#define SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD		10		// 2006-01-23 by cmkwon, º¸½º ¸ó½ºÅÍ Á×À» ¶§ ·Î±× ³²±æ À¯Àú ¸®½ºÆ® ¼ýÀÚ

// 2006-09-28 by cmkwon, º¯°æ(1000-->300)
#define SIZE_MIN_SELECTED_INFLUENCE_TYPE_USER_COUNT_FOR_INFLUENCE_DISTRIBUTION		100		// 2009-03-31 by cmkwon, ¼¼·ÂÃÊ±âÈ­ ½Ã½ºÅÛ ±¸Çö - ±âÁ¸(300), // 2006-02-08 by cmkwon, ¼¼·Â ºÐÆ÷ Ã³¸®¸¦ À§ÇÑ Á¶°ÇÀÎ ¼¼·Â ¼±ÅÃ À¯ÀúÀÇ ÃÑÇÕ

#define SIZE_MAX_SIGN_BOARD_STRING					64		// 2006-04-17 by cmkwon, Àü±¤ÆÇ¿¡ µé¾î°¥ ½ºÆ®¸µ ÃÖ´ë ±æÀÌ
#define INFLWAR_APPLY_TAX_PERIOD_DAY				30		// 2006-05-19 by cmkwon, ¼¼·ÂÀü ºÐÆ÷ °è»ê¿¡ µé¾î°¡ Ä³¸¯ÅÍ °Ë»ö½Ã ÃÖ±Ù ·Î±×ÀÎÀÌ ÇöÀç¿¡¼­ ÀÌ³¯Â¥ ÀÌÀüº¸´Ù ÀÌÈÄ¿©¾ßÇÑ´Ù.

#define WEIGHT_RATE_HEAVY							0.7f	// 2006-10-13 by cmkwon, ÃÊ°ú½Ã ¿¬·á ¼Ò¸ð 2¹è 
#define WEIGHT_RATE_VERY_HEAVY						0.8f	// 2006-10-13 by cmkwon, ÃÊ°ú½Ã ¿¬·á ¼Ò¸ð 2¹è, ºÎ½ºÅÍ »ç¿ë ¾ÈµÊ

#define TERM_QUEST_REQUEST_WARP_EFFECTIVE_TIME		20000	// 2006-10-16 by cmkwon, ÆÄÆ¼ ¿öÇÁ À¯È¿½Ã°£(´ÜÀ§:ms)

#define TERM_STRATEGYPOINT_OCCUPY_TIME						7200000		// 2006-11-21 by cmkwon, Àü·«Æ÷ÀÎÆ® ÆøÆÄ½Ã ¼ÒÀ¯ ½Ã°£(´ÜÀ§:ms, 7200000 = 2½Ã°£ = 2*60*60*1000)
#define ADD_CONTRIBUTIONPOINT_STRATEGYPOINT_MONSTER			500			// 2006-11-21 by cmkwon, Àü·«Æ÷ÀÎÆ® ÆøÆÄ/¹æ¾î ¼º°ø½Ã Áõ°¡ ±â¿©µµ
#define ADD_CONTRIBUTIONPOINT_BOSS_MONSTER					1000		// 2007-05-15 by dhjin, ¸ð¼± ÆøÆÄ/¹æ¾î ¼º°ø½Ã Áõ°¡ ±â¿©µµ

// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±
#define CONTRIBUTIONPOINT_STRATEGYPOINT_MONSTER_WINNER		500
#define CONTRIBUTIONPOINT_STRATEGYPOINT_MONSTER_LOSER		100
#define CONTRIBUTIONPOINT_BOSS_MONSTER_WINNER				1000
#define CONTRIBUTIONPOINT_BOSS_MONSTER_LOSER				200
#define CONTRIBUTIONPOINT_OUTPOST_WAR_WINNER				500
#define CONTRIBUTIONPOINT_OUTPOST_WAR_LOSER					100

// end 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±

#define ADD_WARPOINT_STRATEGYPOINT_MONSTER					600			// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // 2007-09-06 by dhjin 100=>300, // 2007-05-15 by dhjin, Àü·«Æ÷ÀÎÆ® ÆøÆÄ/¹æ¾î ¼º°ø½Ã Áõ°¡ WARPOINT 
#define ADD_WARPOINT_BOSS_MONSTER							1000		// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // 2007-09-06 by dhjin 300=>500, // 2007-05-15 by dhjin, ¸ð¼± ÆøÆÄ/¹æ¾î ¼º°ø½Ã Áõ°¡ WARPOINT
#define ADD_WARPOINT_STRATEGYPOINT_MONSTER_BY_LOSE			200			// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // 2008-01-07 by dhjin, ¼¼·Â º¸»ó ¼öÁ¤ - Àü·«Æ÷ÀÎÆ® ÆøÆÄ/¹æ¾î ÆÐ¹è½Ã Áõ°¡ WARPOINT 
#define ADD_WARPOINT_BOSS_MONSTER_BY_LOSE					400			// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // 2008-01-07 by dhjin, ¼¼·Â º¸»ó ¼öÁ¤ - ¸ð¼± ÆøÆÄ/¹æ¾î ÆÐ¹è½Ã Áõ°¡ WARPOINT
#define ADD_WARPOINT_EVENT_MONSTER_BY_LOSE					250			// 2010-06-21 by jskim, ÀÌº¥Æ® ¸ó½ºÅÍ º¸»ó ¿öÆ÷ÀÎÆ®

#define TERM_GAME_PLAYTIME_LIMIT_FIRST_SEC					10800		// 2006-11-23 by cmkwon, 10800ÃÊ(3*60*60) = 3½Ã°£ - °æÇèÄ¡/SPI 50%
#define TERM_GAME_PLAYTIME_LIMIT_LAST_SEC					18000		// 2006-11-23 by cmkwon, 18000ÃÊ(5*60*60) = 5½Ã°£ - °æÇèÄ¡/SPI 0%

// 2009-04-06 by cmkwon, ½ºÅ³ °ü·Ã ¼Ó¼º Ãß°¡¾È ±¸Çö(»ç¿ë ½Ã°£ ÀúÀå) - ¼Ó¼ºÀ¸·Î Ã³¸®ÇÔ, Á¦°ÅµÊ.
//#define TERM_TICK_MUST_SERVER_CHECK_SKILL_REATTACK_TIME		600000		// 2006-12-15 by cmkwon, 10ºÐ(10*60*1000) RaattackTimeÀÌ 10ºÐ ÃÊ°úÀÎ ½ºÅ³Àº ¼­¹ö¿¡¼­µµ Ã¼Å©ÇÑ´Ù

#define WM_UPDATEFILE_DOWNLOAD_ERROR						WM_USER + 100	// 2007-01-05 by cmkwon
#define WM_UPDATEFILE_DOWNLOAD_INIT							WM_USER + 101	// 2007-01-05 by cmkwon
#define WM_UPDATEFILE_DOWNLOAD_PROGRESS						WM_USER + 102	// 2007-01-05 by cmkwon
#define WM_UPDATEFILE_DOWNLOAD_OK							WM_USER + 103	// 2007-01-05 by cmkwon
#define WM_UPDATE_VTCGUARD_OK								WM_USER + 104	// 2012-02-09 by hskim, º£Æ®³² X-Trap -> VTC °¡µå·Î º¯°æ

#define SIZE_MAX_SERVER_GROUP_COUNT							20				// 2007-01-18 by cmkwon, ¼­¹ö ±×·ì ÃÖ´ë °³¼ö

#define SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY				30				// 2007-03-29 by cmkwon, À¥ÀÎÁõÅ° ÃÖ´ë ½ºÆ®¸µ »çÀÌÁî(ÇöÀç´Â ¿¹´ç¸¸ »ç¿ë ¿¹Á¤)

#define SIZE_MAX_GAME_PUBLISHER_NAME						20				// 2007-04-09 by cmkwon
#define SIZE_MAX_ADD_CHARACTER_NAME							5				// 2007-04-10 by cmkwon, Ä³¸¯ÅÍ¸íµÚ¿¡ ºÙÀÌ´Â ½ºÆ®¸µ ÃÖ´ë »çÀÌÆ®


#define SIZE_MAX_GAME_SERVER_GROUP_NAME						30				// 2007-05-02 by cmkwon
#define COUNT_MAX_GAME_SERVER_GROUP_LIST					10					

// 2008-09-17 by cmkwon, Å¬¶óÀÌ¾ðÆ® ½ÇÇàÆÄÀÏµµ Ã¼Å©¼¶ Ã¼Å© Ãß°¡ - 
//#define	SIZE_MAX_RESOBJ_FILE_NAME							20				// 2007-05-28 by cmkwon, 
#define	SIZE_MAX_RESOBJ_FILE_NAME							30				// 2008-09-17 by cmkwon, Å¬¶óÀÌ¾ðÆ® ½ÇÇàÆÄÀÏµµ Ã¼Å©¼¶ Ã¼Å© Ãß°¡ - 30 Bytes ·Î ¼öÁ¤

#define	TICKGAP_WARHEAD_LIFETIME							20000			// 2007-06-12 by cmkwon, 2Çü ¹«±â ÅºµÎ°¡ ¹ß»ç ÈÄ À¯È¿ÇÑ ½Ã°£(ms)
#define	TICKGAP_WARHEAD_LIFETIME_LONG						50000			// 2007-06-12 by cmkwon, 2Çü ¹«±â ÅºµÎ°¡ ¹ß»ç ÈÄ À¯È¿ÇÑ ½Ã°£(ms) - // 2011-03-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡ - ÁÖÆ÷¿ë ±ä ½Ã°£ 2Çü ÅºµÎ ¹«±â

// 2007-06-27 by cmkwon, ÆÄÆ¼¿ø °æÇèÄ¡ ºÐ¹è ¹æ½Ä ½Ã½ºÅÛ Ãß°¡ - Å¸ÀÔ Á¤ÀÇ
#define MAX_LEVEL_GAP_APPLY_PARTY_BONUS						14				// 2008-06-10 by dhjin, EP3 Æí´ë ¼öÁ¤ - 7->14·Î º¯°æ // 2007-06-27 by cmkwon, ÆÄÆ¼ º¸³Ê½º Àû¿ë ÃÖ´ë ·¹º§Â÷			
#define EXP_DISTRIBUTE_TYPE_DAMAGE							0				// 2007-06-27 by cmkwon, ¸ó½ºÅÍ¿¡ ÁØ µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ ºÐ¹è
#define EXP_DISTRIBUTE_TYPE_EQUALITY						1				// 2007-06-27 by cmkwon, ±Õµî ºÐ¹è

#define TERM_10MINUTES_BY_SECOND							600				// 2007-06-28 by cmkwon, ¿¹´ç 7¿ù PC¹æ ÄáÀÌº¥Æ® Áö±Þ ½Ã°£ ¼öÁ¤ -
#define TERM_30MINUTES_BY_SECOND							1800			// 2007-06-28 by cmkwon, ¿¹´ç 7¿ù PC¹æ ÄáÀÌº¥Æ® Áö±Þ ½Ã°£ ¼öÁ¤ -
#define TERM_60MINUTES_BY_SECOND							3600			// 2007-06-28 by cmkwon, ¿¹´ç 7¿ù PC¹æ ÄáÀÌº¥Æ® Áö±Þ ½Ã°£ ¼öÁ¤
#define TERM_10MINUTES_BY_MILLISECOND						600000			// 2007-06-28 by cmkwon, Áß±¹ ¹æ½ÉÃë°ü·Ã(°ÔÀÓ ½Ã°£ ¾Ë¸² ±¸Çö) - 10ºÐ

#define SIZE_MAX_JUMIN_NUMBER								20				// 2007-06-29 by cmkwon, td_account Å×ÀÌºí¿¡ ÁÖ¹Î¹øÈ£ÀúÀåÇÏ±â 

#define RECHARGE_BULLET_ALL_CALIBER							255				// 2007-08-07 by cmkwon, 1Çü/2Çü ¹«±â ÃÑ¾Ë ÃæÀü ¾ÆÀÌÅÛ ±¸Çö - Caliber Á¤ÀÇ

#define SIZE_MAX_STRING_128									128				// 2007-08-09 by cmkwon, ¸ðµç ¼¼·Â¿¡ Ã¤ÆÃ Àü¼ÛÇÏ±â - Á¤ÀÇ Ãß°¡
#define SIZE_MAX_STRING_256									256				// 2007-08-09 by cmkwon, ¸ðµç ¼¼·Â¿¡ Ã¤ÆÃ Àü¼ÛÇÏ±â - Á¤ÀÇ Ãß°¡
#define SIZE_MAX_STRING_512									512				// 2007-08-09 by cmkwon, ¸ðµç ¼¼·Â¿¡ Ã¤ÆÃ Àü¼ÛÇÏ±â - Á¤ÀÇ Ãß°¡
#define SIZE_MAX_STRING_1024								1024			// 2007-08-09 by cmkwon, ¸ðµç ¼¼·Â¿¡ Ã¤ÆÃ Àü¼ÛÇÏ±â - Á¤ÀÇ Ãß°¡

#define SIZE_MAX_CONFERENCEROOM_USER						100				// 2007-08-30 by cmkwon, È¸ÀÇ·ë ½Ã½ºÅÛ ±¸Çö - È¸ÀÇ¸§ Çã°¡ ÃÖ´ë ÀÎ¿ø¼ö

#define	STR_INVALID_CHARACTER_NAME							" \r\n\t"		// 2007-10-22 by cmkwon, Ä³¸¯ÅÍ¸í¿¡ À¯È¿ÇÏÁö ¾ÊÀº ¹®ÀÚ
#define	STR_INVALID_PRE8SUFFIX_GUILD_NAME					" \r\n\t"		// 2007-10-22 by cmkwon, ¿©´Ü¸í ½ÃÀÛ°ú ³¡¿¡ À¯È¿ÇÏÁö ¾ÊÀº ¹®ÀÚ

#define SIZE_MAX_SELECTIVE_SHUTDOWN_INFO					30				// 2012-07-11 by hskim, ¼±ÅÃÀû ¼Ë´Ù¿î
#define SIZE_MAX_SHUTDOWN_SERVICE_BLOCK						24				// 2012-07-11 by hskim, ¼±ÅÃÀû ¼Ë´Ù¿î
#define STR_DEFAULT_VALUE_SELECTIVE_SHUTDOWN_INFO			"111111111111111111111111"		// 2012-07-11 by hskim, ¼±ÅÃÀû ¼Ë´Ù¿î

#ifdef	S_GAMEFORGE_KEY_CHANGE_JHSEOL
#define STR_XOR_KEY_STRING_SERVER_INFO						"0an%M)~Y*BAi~f5b)UM~)%#058v~#YVV~&#@"			// 2012-07-26 by jhseol, °ÔÀÓÆ÷Áö Key º¯°æ
#define STR_XOR_KEY_STRING_PRE_SERVER_ADDRESS				"+-faNsf(^fP{)3>fnao??_+|23kdasf*^@`d{]s*&DS"	// 2008-04-23 by cmkwon, PreServer ÁÖ¼Ò¸¦ IP¿Í µµ¸ÞÀÎ µÑ´Ù Áö¿ø - 
#define STR_XOR_KEY_STRING_DB_ID_PWD						"m045%h@M`g0nq~683tT__!*@^OTE~W09n3$q+b94`lwrei~53nm_%byr~(*PN$^*"	// 2012-07-26 by jhseol, °ÔÀÓÆ÷Áö Key º¯°æ
#define STR_XOR_KEY_STRING_SECURITY_COMMAND					"^@dk@11@<$HGJKAOkfjqpojg@1-09f-0102890jkfmlk!@#68f91!@jf902121Z"	// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö (Ä¿¸Çµå¿ë ¾ÏÈ£È­ Å°)
#else
#define STR_XOR_KEY_STRING_SERVER_INFO						"~8xANs(^fP{)34$(fcbTN$(C-=x"					// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - DB Server Password XOR Key
#define STR_XOR_KEY_STRING_PRE_SERVER_ADDRESS				"+-faNsf(^fP{)3>fnao??_+|23kdasf*^@`d{]s*&DS"	// 2008-04-23 by cmkwon, PreServer ÁÖ¼Ò¸¦ IP¿Í µµ¸ÞÀÎ µÑ´Ù Áö¿ø - 
#define STR_XOR_KEY_STRING_DB_ID_PWD						"@34ns%<<fdsa(Tflsd!sndsa^#)fndsla$nvsa$fndsla&nfdsJak(fnldsa!#F"	// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
#define STR_XOR_KEY_STRING_SECURITY_COMMAND					"^@dk@11@<$HGJKAOkfjqpojg@1-09f-0102890jkfmlk!@#68f91!@jf902121Z"	// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö (Ä¿¸Çµå¿ë ¾ÏÈ£È­ Å°)
#endif


#define COUNT_MAX_SECURITY_COMMAND							3				// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö (Ä¿¸Çµå¿ë ¾ÏÈ£È­ Å° ¸í·ÉÀÇ ÃÖ´ë °¹¼ö)
#define SIZE_MAX_SECURITY_COMMAND							1024			// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö (Ä¿¸Çµå¿ë ¾ÏÈ£È­ Å°ÀÇ ÃÖ´ë Å©±â)
#define SIZE_THRESHOLD_SECURITY_COMMAND						50				// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö (Ä¿¸Çµå¿ë ¾ÏÈ£È­ÀÎÁö Å°ÀÇ ±æÀÌ·Î ÆÇ´ÜÇÏ´Â ÃÖ¼Ò °ª)

#define SIZE_MAX_DB_USER_ID									128				// 2007-12-13 by cmkwon, 128 bytes ·Î º¯°æ, // 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­
#define SIZE_MAX_DB_USER_PWD								128				// 2007-12-13 by cmkwon, 128 bytes ·Î º¯°æ, // 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­

#define SIZE_MAX_WINDOW_DEGREE_NAME							128				// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ - Ãß°¡

#define SIZE_MAX_ODBC_CONN_STRING							128		// 2008-03-03 by cmkwon, SIZE_MAX_ODBC_CONN_STRING Á¤ÀÇ À§Ä¡ º¯°æ - GlobalGameServer.h ¿¡ ÀÖ´ø°ÍÀ» ¿©±â·Î ¿Å±è

#define	TICKGAP_DROP_ITEM_TIMEOUT							300000			// 2012-03-05 by hskim, µå¶ø ¾ÆÀÌÅÛ ÀÏÁ¤ ½Ã°£ÈÄ »èÁ¦ (5ºÐ : 1000 ms * 60 sec * 5 min)

///////////////////////////////////////////////////////////////////////////////
// 2010-11-29 by shcho, ¾Æ¸£ÇîÆ¼³ª ¿ÜºÎÀÎÁõ º¯°æ SOAP Ã³¸®
#define SIZE_MAX_URL_STRNIG									512
#define SIZE_MAX_GAME_ID									10
// end 2010-11-29 by shcho, ¾Æ¸£ÇîÆ¼³ª ¿ÜºÎÀÎÁõ º¯°æ SOAP Ã³¸®
///////////////////////////////////////////////////////////////////////////////


// 2008-04-29 by cmkwon, ¼­¹ö±º Á¤º¸ DB¿¡ Ãß°¡(½Å±Ô °èÁ¤ Ä³¸¯ÅÍ »ý¼º Á¦ÇÑ ½Ã½ºÅÛÃß°¡) - 
#define SIZE_MAX_INIT_LIMIT_USER_COUNT_PER_SERVER_GROUP		500				// 2008-04-29 by cmkwon, ¼­¹ö±º Á¤º¸ DB¿¡ Ãß°¡(½Å±Ô °èÁ¤ Ä³¸¯ÅÍ »ý¼º Á¦ÇÑ ½Ã½ºÅÛÃß°¡) - 

#define MIN_SUPPORT_YEAR			1970		// 2008-06-27 by cmkwon, ³¯Â¥½Ã°£ °ü·Ã ¹ö±× ¼öÁ¤(1970 ~ 2037) - 
#define MAX_SUPPORT_YEAR			2037		// 2008-06-27 by cmkwon, ³¯Â¥½Ã°£ °ü·Ã ¹ö±× ¼öÁ¤(1970 ~ 2037) - 

#define MAX_ITEM_COUNT_FOR_PACKAGE_ITEM						10				// 2008-08-26 by cmkwon, ItemAttribute Ãß°¡ - ÆÐÅ°Áö(Package) ¾ÆÀÌÅÛ, ÃÖ´ë °³¼ö Á¤ÀÇ


#define MAX_PACKET_SIZE_FOR_XIGNCODE						512				// 2008-11-28 by cmkwon, ´ë¸¸ Netpower_Tpe XignCode(°ÔÀÓ°¡µå) Àû¿ë - 

#define SIZE_MAX_ADDRESS									128				// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 


// 2010-06-01 by shcho, GLogDB °ü·Ã -
#define SIZE_MAX_GLOG_CHARACTER_RACE_NAME						32
#define SIZE_MAX_GLOG_CHARACTER_CLASS_NAME						32
#define SIZE_MAX_GLOG_EVENT_DESCRIPTION							512

// 2011-01-26 by hskim, ÀÎÁõ ¼­¹ö ±¸Çö
#define SIZE_MAX_AUTH_GAMENAME					30
#define SIZE_MAX_AUTH_CURRENTVER				30
#define SIZE_MAX_AUTH_PUBLISHER					35
#define SIZE_MAX_AUTH_ACCEPT_COMMENT			254
#define SIZE_MAX_AUTH_RESOURCE_FILE_NAME		254

#define MAX_ENCHANT_ONCE_APPLY_COUNT			10							// 2011-10-19 by hskim, EP4 [Free to play] - 10 È¸ ÀÎÃ¾Æ® ¾ÆÀÌÅÛ ±â´É ±¸Çö (±âÈ¹ÀÚÀÇ ½Ç¼ö ¹æÁö¸¦ À§ÇØ ½Ã½ºÅÛÀûÀ¸·Î ÇÑ°è Àû¿ë)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 2007-01-06 by cmkwon
#define AU_SERVER_TYPE_FTP				0
#define AU_SERVER_TYPE_HTTP				1






#define SIZE_STRING_32								32
#define SIZE_STRING_64								64
#define SIZE_STRING_128								128

enum EN_DAY_OF_WEEK
{
	DAY_OF_WEEK_SUMDAY					= 0,			// 2006-08-24 by cmkwon, ÀÏ¿äÀÏ
	DAY_OF_WEEK_MONDAY					= 1,			// 2006-08-24 by cmkwon, ¿ù¿äÀÏ
	DAY_OF_WEEK_TUESDAY					= 2,			// 2006-08-24 by cmkwon, È­¿äÀÏ
	DAY_OF_WEEK_WEDNESDAY				= 3,			// 2006-08-24 by cmkwon, ¼ö¿äÀÏ
	DAY_OF_WEEK_THURSDAY				= 4,			// 2006-08-24 by cmkwon, ¸ñ¿äÀÏ
	DAY_OF_WEEK_FRIDAY					= 5,			// 2006-08-24 by cmkwon, ±Ý¿äÀÏ
	DAY_OF_WEEK_SATURDAY				= 6,			// 2006-08-24 by cmkwon, Åä¿äÀÏ
	DAY_OF_WEEK_HAPPYHOUREVENT_PERIOD	= 7,			// 2006-08-24 by cmkwon, ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±â°£ ¼³Á¤ - ºñÀ² ÇÊµå´Â »ç¿ëÇÏÁö ¾ÊÀ½
	DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT	= 8				// 2006-08-24 by cmkwon, PCBang ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±â°£ ¹× ºñÀ² ¼³Á¤
};

#define ATUM_PRE_SERVER								0
#define ATUM_IM_SERVER								1
#define ATUM_LOG_SERVER								2
#define ATUM_MONITOR_SERVER							3
#define ATUM_FIELD_SERVER_1							6
#define ATUM_NPC_SERVER_1							7
#define ATUM_FIELD_SERVER_2							8
#define ATUM_NPC_SERVER_2							9
#define ATUM_FIELD_SERVER_3							10
#define ATUM_NPC_SERVER_3							11
#define ATUM_FIELD_SERVER_4							12
#define ATUM_NPC_SERVER_4							13
#define ATUM_FIELD_SERVER_5							14
#define ATUM_NPC_SERVER_5							15

// Æ¯¼ºÄ¡ °ªµé
#define OT_ATTACK_TRAIT_DEFAULT_VALUE	0.05f
#define DT_DEFENSE_TRAIT_DEFAULT_VALUE	0.03f
#define BT_AVOID_TRAIT_DEFAULT_VALUE	1.0f

///////////////////////////////////////////////////////////////////////////////
//	ATUM - CHaracter, Monster, Item - Parameter Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////

// 2005-06-23 by cmkwon
// ºñÆ® ÇÃ·¡±× ¼³Á¤ - ¼¼·Â Å¸ÀÔÀ¸·Î ´ÙÀ½ Áß¿¡ ÇÏ³ªÀÇ °ªÀ» °¡Áø´Ù.
#define INFLUENCE_TYPE_UNKNOWN				(BYTE)0x0000	// ¾Ë¼ö ¾øÀ½
#define INFLUENCE_TYPE_NORMAL				(BYTE)0x0001	// 2005-12-20 by cmkwon, ¹ÙÀÌÁ¦´ÏÀ¯ ÀÏ¹Ý±º
#define INFLUENCE_TYPE_VCN					(BYTE)0x0002	// 2005-12-20 by cmkwon, ¹ÙÀÌÁ¦´ÏÀ¯ Á¤±Ô±º, ÀÌÀü(V.C.U: Vijuenill City United.)
#define INFLUENCE_TYPE_ANI					(BYTE)0x0004	// 2005-12-20 by cmkwon, ¾Ë¸µÅÏ Á¤±Ô±º, ÀÌÀü(¹Ý ¹ÎÁ·ÁÖÀÇ ¿¬ÇÕ -¾Ë¸µÅÏ ½ÃÆ¼ ¹Ý¶õ±º- (A.N.I: Anti Nationalism Influence))
#define	INFLUENCE_TYPE_GM					(BYTE)0x0006
#define INFLUENCE_TYPE_RRP					(BYTE)0x0008	// ·Îº£´Ï¾Æ Çõ¸í ¿¬¹æ - ¹ÙÅ»·¯½º ¿¬¹æ±º- (R.R.P: Robenia Revolution Federation)
#define INFLUENCE_TYPE_ALL_MASK				(BYTE)0x00FF	// ¸ðµç ¼¼·Â, 255
#define INFLUENCE_TYPE_COUNT				4				// 2006-04-17 by cmkwon, ¼¼·ÂÀÇ °³¼ö(ÃÑ 4°³ - Normal, VCN, ANI, ALL)
#define INFLUENCE_TYPE_COUNT_EX_ALL			3				// 2009-09-16 by cmkwon, ¼¼·Â ÃÊ±âÈ­½Ã ¾îºäÂ¡ ¹æÁö ±¸Çö - ¼¼·ÂÀÇ °³¼ö(ÃÑ 3°³ - Normal, VCN, ANI)
#define COMPARE_INFLUENCE(VAR, MASK)		(((VAR) & (MASK)) != 0)

#define IS_NORMAL_INFLUENCE_TYPE(charInfl)	(COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_NORMAL))		// 2007-05-22 by cmkwon, Ãß°¡ÇÔ
#define IS_VCN_INFLUENCE_TYPE(charInfl)		(COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_VCN))			// 2007-05-22 by cmkwon
#define IS_ANI_INFLUENCE_TYPE(charInfl)		(COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_ANI))
#define IS_VALID_INFLUENCE_TYPE(charInfl)	(INFLUENCE_TYPE_NORMAL==(charInfl)|| INFLUENCE_TYPE_VCN==(charInfl)|| INFLUENCE_TYPE_GM==(charInfl)|| INFLUENCE_TYPE_RRP==(charInfl)||INFLUENCE_TYPE_ANI==(charInfl))		// 2006-02-08 by cmkwon

// 2010-06-01 by shcho, GLogDB °ü·Ã -
#define INFLUENCE_TYPE_ENG_NORMAL			"NORMAL_INFLUENCE"
#define INFLUENCE_TYPE_ENG_VCN				"VCN_INFLUENCE"
#define INFLUENCE_TYPE_ENG_ANI				"ANI_INFLUENCE"
#define INFLUENCE_TYPE_ENG_ALL_MASK			"ALL_INFLUENCE"

#define UNITKIND_ENG_BGEAR					"B-Gear"
#define UNITKIND_ENG_MGEAR					"M-Gear"
#define UNITKIND_ENG_AGEAR					"A-Gear"
#define UNITKIND_ENG_IGEAR					"I-Gear"
#define UNITKIND_ENG_UNKNOWN				"UNKNOWN_Gear"


// 2005-12-26 by cmkwon
// ºñÆ® ÇÃ·¡±× ¼³Á¤ ¾Æ´Ô - ti_MapInfoÅ×ÀÌºíÀÇ MapInfluenceType ÇÊµå ¸Ê ¼¼·Â Å¸ÀÔ ¼³Á¤
#define MAP_INFLUENCE_NEUTRALITY		(USHORT)0		// Áß¸³ ¿µ¿ª
#define MAP_INFLUENCE_NON_DISPUTE		(USHORT)400		// À¯Àú°£ ºñºÐÀï ¿µ¿ª // 2012-01-17 by hskim, EP4 [ºñºÐÀï Áö¿ª] - À¯Àú°£ °ø°Ý ±ÝÁö ±¸¿ª
#define MAP_INFLUENCE_HELL				(USHORT)500		// Áö¿Á ¿µ¿ª (Á×À¸¸é Backmap À¸·Î ÀÌµ¿ ¹× ÃÊ±â ÀÔÀå ºÒ°¡ ¼³Á¤) // 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
#define MAP_INFLUENCE_VCN_CITY			(USHORT)1000	// ¹ÙÀÌÁ¦´ÏÀ¯ µµ½Ã¸Ê
#define MAP_INFLUENCE_VCN_START			(USHORT)1001	// ¹ÙÀÌÁ¦´ÏÀ¯ ½ÃÀÛ¸Ê
#define MAP_INFLUENCE_VCN_MSWAR			(USHORT)1700	// ¹ÙÀÌÁ¦´ÏÀ¯ - ¸ð¼±Àü½Ã¿¡ Æ¨±â´Â ¸Ê - // 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define MAP_INFLUENCE_VCN_BASE			(USHORT)1701	// ¹ÙÀÌÁ¦´ÏÀ¯ - ¸ð¼±Àü½Ã¿¡ Æ¨±â´Â ¸Ê + ÀÏ¹Ý »óÈ²¿¡¼­ ¸¶À» ±ÍÈ¯ - // 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define MAP_INFLUENCE_VCN_LAST			(USHORT)1999	// ¹ÙÀÌÁ¦´ÏÀ¯ ¸¶Áö¸·¸Ê
#define MAP_INFLUENCE_ANI_CITY			(USHORT)2000	// ¾Ë¸µÅÏ µµ½Ã¸Ê
#define MAP_INFLUENCE_ANI_START			(USHORT)2001	// ¾Ë¸µÅÏ ½ÃÀÛ¸Ê
#define MAP_INFLUENCE_ANI_MSWAR			(USHORT)2700	// ¾Ë¸µÅÏ - ¸ð¼±Àü½Ã¿¡ Æ¨±â´Â ¸Ê - // 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define MAP_INFLUENCE_ANI_BASE			(USHORT)2701	// ¾Ë¸µÅÏ - ¸ð¼±Àü½Ã¿¡ Æ¨±â´Â ¸Ê + ÀÏ¹Ý »óÈ²¿¡¼­ ¸¶À» ±ÍÈ¯ - // 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define MAP_INFLUENCE_ANI_LAST			(USHORT)2999	// ¾Ë¸µÅÏ ¸¶Áö¸·¸Ê
#define MAP_INFLUENCE_NORMAL_CITY		(USHORT)3000	// 2007-05-22 by cmkwon, ÀÏ¹Ý±ºµµ½Ã¸Ê
#define MAP_INFLUENCE_INFINITY			(USHORT)5000	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
#define MAP_INFLUENCE_INFINITY_LAST		(USHORT)5999	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
#define MAP_INFLUENCE_OUTPOST			(USHORT)6000	// 2007-08-17 by dhjin, ÀüÁø±âÁö µµ½Ã ¸Ê 
#define MAP_INFLUENCE_OUTPOST_START		(USHORT)6001	// 2007-08-17 by dhjin, ÀüÁø±âÁö ¸Ê 
#define MAP_INFLUENCE_OUTPOST_LAST		(USHORT)6999	// 2007-08-17 by dhjin, ÀüÁø±âÁö ¸Ê
#define MAP_INFLUENCE_OUTPOST_TRIGGER	(USHORT)4303	// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ - Æ®¸®°Å ÀüÁø±âÁö ¸Ê Ãß°¡
#define MAP_INFLUENCE_ARENA				(USHORT)7000	// 2007-05-04 by dhjin, ARENA ¸Ê
#define MAP_INFLUENCE_ARENA_LAST		(USHORT)7999	// 2007-05-04 by dhjin, ARENA ¸Ê
#define MAP_INFLUENCE_EVENT_CITY		(USHORT)8000	// ÀÌº¥Æ® µµ½Ã¸Ê(ÀÇ¹Ì¾øÀ½)
#define MAP_INFLUENCE_EVENT_START		(USHORT)8001	// ÀÌº¥Æ® ½ÃÀÛ¸Ê, 2006-07-18 by cmkwon
#define MAP_INFLUENCE_EVENT_LAST		(USHORT)8999	// ÀÌº¥Æ® ¸¶Áö¸·¸Ê, 2006-07-18 by cmkwon
#define MAP_INFLUENCE_CONFLICT_AREA_CITY		(USHORT)9000	// ºÐÀï Áö¿ª ½ÃÀÛ¸Ê(ÀÇ¹Ì¾øÀ½), // 2006-02-09 by cmkwon
#define MAP_INFLUENCE_CONFLICT_AREA_START		(USHORT)9001	// ºÐÀï Áö¿ª ½ÃÀÛ¸Ê, // 2006-02-09 by cmkwon
#define MAP_INFLUENCE_CONFLICT_AREA_LAST		(USHORT)9999	// ºÐÀï Áö¿ª ¸¶Áö¸·¸Ê, // 2006-02-09 by cmkwon
#define IS_MAP_INFLUENCE_NON_DISPUTE(mapInfl)	( MAP_INFLUENCE_NON_DISPUTE == (mapInfl) )	// À¯Àú°£ ºñºÐÀï ¿µ¿ª // 2012-01-17 by hskim, EP4 [ºñºÐÀï Áö¿ª] - À¯Àú°£ °ø°Ý ±ÝÁö ±¸¿ª
#define IS_MAP_INFLUENCE_HELL(mapInfl)			( MAP_INFLUENCE_HELL == (mapInfl) )	// Áö¿Á ¿µ¿ª (Á×À¸¸é Backmap À¸·Î ÀÌµ¿) // 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
#define IS_MAP_INFLUENCE_VCN(mapInfl)			( MAP_INFLUENCE_VCN_CITY <= (mapInfl) && MAP_INFLUENCE_VCN_LAST >= (mapInfl) )	// ¹ÙÀÌÁ¦´ÏÀ¯ ¼¼·Â ¸Ê(1000 ~ 1999)ÀÎ°¡?
#define IS_MAP_INFLUENCE_ANI(mapInfl)			( MAP_INFLUENCE_ANI_CITY <= (mapInfl) && MAP_INFLUENCE_ANI_LAST >= (mapInfl) )	// ¾Ë¸µÅÏ ¼¼·Â ¸Ê(2000 ~ 2999)ÀÎ°¡?
#define IS_MAP_INFLUENCE_NORMAL(mapInfl)		( MAP_INFLUENCE_NORMAL_CITY == (mapInfl) )	// 2007-05-22 by cmkwon, ÀÏ¹Ý±ºµµ½Ã¸Ê(3000)ÀÎ°¡?
#define IS_MAP_INFLUENCE_CONFLICT_AREA(mapInfl)	( MAP_INFLUENCE_CONFLICT_AREA_CITY <= (mapInfl) && MAP_INFLUENCE_CONFLICT_AREA_LAST >= (mapInfl) )	// ºÐÀï Áö¿ª ¸Ê(9000 ~ 9999)ÀÎ°¡?
#define IS_MAP_INFLUENCE_EVENT_AREA(mapInfl)	( MAP_INFLUENCE_EVENT_CITY <= (mapInfl) && MAP_INFLUENCE_EVENT_LAST >= (mapInfl) )	// ÀÌº¥Æ®¸Ê(8000 ~ 8999)ÀÎ°¡?
#define IS_MAP_INFLUENCE_ARENA(mapInfl)			( MAP_INFLUENCE_ARENA <= (mapInfl) && MAP_INFLUENCE_ARENA_LAST >= (mapInfl) )	// ¾Æ·¹³ª ¸Ê(7000 ~ 7999)ÀÎ°¡?
#define IS_MAP_INFLUENCE_OUTPOST(mapInfl)		((MAP_INFLUENCE_OUTPOST_START <= (mapInfl) && MAP_INFLUENCE_OUTPOST_LAST >= (mapInfl)) || MAP_INFLUENCE_OUTPOST_TRIGGER == (mapInfl) )	// ÀüÁø±âÁö ¸Ê(6000 ~ 6999)ÀÎ°¡?	// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ - Æ®¸®°Å ÀüÁø±âÁö ¸Ê Ãß°¡
#define IS_MAP_INFLUENCE_INFINITY(mapInfl)		( MAP_INFLUENCE_INFINITY <= (mapInfl) && MAP_INFLUENCE_INFINITY_LAST >= (mapInfl) )	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 

#define IS_MAP_INFLUENCE_MSWAR(mapInfl)			( MAP_INFLUENCE_VCN_MSWAR == (mapInfl) || MAP_INFLUENCE_ANI_MSWAR == (mapInfl) || MAP_INFLUENCE_VCN_BASE == (mapInfl) || MAP_INFLUENCE_ANI_BASE == (mapInfl) )		// 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define IS_MAP_INFLUENCE_MSWAR_ANI(mapInfl)		( MAP_INFLUENCE_ANI_MSWAR == (mapInfl) || MAP_INFLUENCE_ANI_BASE == (mapInfl) )		// 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ
#define IS_MAP_INFLUENCE_MSWAR_VCN(mapInfl)		( MAP_INFLUENCE_VCN_MSWAR == (mapInfl) || MAP_INFLUENCE_VCN_BASE == (mapInfl) )		// 2012-12-05 by hskim, ¸ð¼±Àü ½ÃÀÛ½Ã¿¡ »ó´ë ¼¼·Â µµ½Ã¸ÊÀ¸·Î ¿öÇÁ

// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - CAtumSJ::IsSameCharacterInfluence8MapInfluence#·Î ´ëÃ¼
// // 2007-05-22 by cmkwon, ÀÏ¹Ý±º µµ½Ã¸Ê Ãß°¡
// #define IS_SAME_CHARACTER_8_MAP_INFLUENCE(charInfl, mapInfl)	(  ( COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_NORMAL) && IS_MAP_INFLUENCE_NORMAL(mapInfl) ) \
// 																|| ( COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_VCN) && IS_MAP_INFLUENCE_VCN(mapInfl) ) \
//																|| ( COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_ANI) && IS_MAP_INFLUENCE_ANI(mapInfl) )  )

// 2007-05-22 by cmkwon, ¾Æ·¡ #defineÀ¸·Î º¯°æ Ã³¸®ÇÔ
//#define IS_OTHER_INFLUENCE_CITY(charInfl, mapInfl)	( (COMPARE_INFLUENCE(charInfl, INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN) && MAP_INFLUENCE_ANI_CITY == mapInfl) \
//																|| (COMPARE_INFLUENCE(charInfl, INFLUENCE_TYPE_ANI) && MAP_INFLUENCE_VCN_CITY == mapInfl) )
#define IS_MAP_INFLUENCE_CITY(mapInfl)				( MAP_INFLUENCE_VCN_CITY == (mapInfl) || MAP_INFLUENCE_ANI_CITY == (mapInfl) || MAP_INFLUENCE_NORMAL_CITY == (mapInfl))

// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - CAtumSJ::IsOtherInfluenceMap#À¸·Î ´ëÃ¼ÇÔ
//#define IS_OTHER_INFLUENCE_MAP(charInfl, mapInfl)	( (COMPARE_INFLUENCE(charInfl, INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN) && IS_MAP_INFLUENCE_ANI(mapInfl)) \
//																|| (COMPARE_INFLUENCE(charInfl, INFLUENCE_TYPE_ANI) && IS_MAP_INFLUENCE_VCN(mapInfl)) )

// 2008-12-23 by dhjin, ÀüÀï º¸»ó Ãß°¡¾È
#define IS_MAP_INFLUENCE_WAR(mapInfl)				( IS_MAP_INFLUENCE_VCN(mapInfl) || IS_MAP_INFLUENCE_ANI(mapInfl) || IS_MAP_INFLUENCE_OUTPOST(mapInfl))

// check: ¸¸¾à Ã¹ bitÀ» »ç¿ëÇÏ°Ô µÈ´Ù¸é, °¢Á¾ DB schemaÀÇ RaceÀÇ typeÀ» int·Î º¯°æÇØ¾ß ÇÔ.
#define RACE_BATTALUS				(USHORT)0x0001	// ¹ÙÅ»·¯½º, 1
#define RACE_DECA					(USHORT)0x0002	// µ¥Ä«, 2
#define RACE_PHILON					(USHORT)0x0004	// ÇÊ·Ð, 4
#define RACE_SHARRINE				(USHORT)0x0008	// »þ¸°, 8
#define RACE_INFLUENCE_LEADER		(USHORT)0x0010	// 16, ¼¼·ÂÀü ¸®´õ, 2006-04-20 by cmkwon, RACE_MONSTER1(¿¹ºñ, 16)¸¦ º¯°æÇÑ °ÍÀÓ
// 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ - Á¤ÀÇ ¼öÁ¤, ¾Æ·¡¤·¿Í °°ÀÌ ¼öÁ¤
//#define RACE_INFLUENCE_SUBLEADER	(USHORT)0x0020	// 32, ¼¼·ÂÀü ºÎÁöµµÀÚ, 2006-12-08 by cmkwon, RACE_MONSTER2(¿¹ºñ, 32)¸¦ º¯°æÇÑ °ÍÀÓ ¿¹ºñ, 32
//#define RACE_NPC					(USHORT)0x0040	// NPC, 64
#define RACE_INFLUENCE_SUBLEADER_1	(USHORT)0x0020	// 32, ¼¼·ÂÀü ºÎÁöµµÀÚ 1 - VCN(ºÎ»ç·É°ü), ANI(ºÎÀÇÀå), // 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ -
#define RACE_INFLUENCE_SUBLEADER_2	(USHORT)0x0040	// 64, ¼¼·ÂÀü ºÎÁöµµÀÚ 2 - VCN(Âü¸ðÃÑÀå), ANI(ÀÛÀüº»ºÎÀå), // 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ -
#define RACE_OPERATION				(USHORT)0x0080	// °ü¸®ÀÚ, 128
#define RACE_GAMEMASTER				(USHORT)0x0100	// °ÔÀÓ¸¶½ºÅÍ, 256
#define RACE_MONITOR				(USHORT)0x0200	// ¸ð´ÏÅÍ, 512
#define RACE_GUEST					(USHORT)0x0400	// °Ô½ºÆ®, 1024
#define RACE_DEMO					(USHORT)0x0800	// ½Ã¿¬(µ¥¸ð)¿ë, 2048
#define RACE_DELETED_CHARACTER		(USHORT)0x4000	// »èÁ¦µÈ Ä³¸¯ÅÍ, 16384 // 2007-02-21 by cmkwon
#define RACE_ALL					(USHORT)0x000F	// ¸ðµç Á¾Á·, (RACE_BATTALUS|RACE_DECA|RACE_PHILON|RACE_SHARRINE), 15
#define RACE_ACCOUNT_TYPE_MASK		(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO)
#define RACE_INFLUENCE_SUBLEADER_MASK	(RACE_INFLUENCE_SUBLEADER_1|RACE_INFLUENCE_SUBLEADER_2)		// 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ - Á¤ÀÇ Ãß°¡
#define COMPARE_RACE(VAR, MASK)		(((VAR) & (MASK)) != 0)
/* - °èÁ¤ °ü·Ã ±ÇÇÑ -
			Á¢¼ÓÀÚ¼öÈ®ÀÎ	¸ó½ºÅÍ¼ÒÈ¯	¸ãÀÌµ¿	ÀÌµ¿/È£Ãâ	¾ÆÀÌÅÛ»ý¼º	¾ÆÀÌÅÛ»ç¿ë	ÀüÃ¼¸Þ½ÃÁö	½ºÅÚ½º	PK ¿©ºÎ
°ü¸®ÀÚ		°¡´É			°¡´É		°¡´É	°¡´É		°¡´É		¸ðµÎ°¡´É	°¡´É		°¡´É	°¡´É
°ÔÀÓ¸¶½ºÅÍ	°¡´É			°¡´É		°¡´É	°¡´É		ºÒ°¡		¸ðµÎ°¡´É	°¡´É		°¡´É	°¡´É
¸ð´ÏÅÍ		°¡´É			ºÒ°¡		°¡´É	ÀÌµ¿¸¸°¡´É	ºÒ°¡		ºÒ°¡		ºÒ°¡		±âº»	ºÒ°¡
°Ô½ºÆ®		ºÒ°¡			ºÒ°¡		ºÒ°¡	ºÒ°¡		ºÒ°¡		·¹º§30°íÁ¤	ºÒ°¡		ºÒ°¡	ºÒ°¡
µ¥¸ð		ºÒ°¡			ºÒ°¡		°¡´É	ºÒ°¡		ºÒ°¡		·¹º§50°íÁ¤	ºÒ°¡		°¡´É	°¡´É
ÀÏ¹Ý		ÀÏ¹Ý			ÀÏ¹Ý		ÀÏ¹Ý	ÀÏ¹Ý		ÀÏ¹Ý		ÀÏ¹Ý		ÀÏ¹Ý		ÀÏ¹Ý	ÀÏ¹Ý
*/


////////////////////////////////////
// Ä³¸¯ÅÍ À¯´ÖÀÇ Á¾·ù
#define UNITKIND_BT01				(USHORT)0x0001	// BT - 01Çü, B-GEAR, 1
#define UNITKIND_BT02				(USHORT)0x0002	// BT - 02Çü, B-GEAR, 2
#define UNITKIND_BT03				(USHORT)0x0004	// BT - 03Çü, B-GEAR, 4
#define UNITKIND_BT04				(USHORT)0x0008	// BT - 04Çü, B-GEAR, 8

#define UNITKIND_OT01				(USHORT)0x0010	// OT - 01Çü, M-GEAR, 16
#define UNITKIND_OT02				(USHORT)0x0020	// OT - 02Çü, M-GEAR, 32
#define UNITKIND_OT03				(USHORT)0x0040	// OT - 03Çü, M-GEAR, 64
#define UNITKIND_OT04				(USHORT)0x0080	// OT - 04Çü, M-GEAR, 128

#define UNITKIND_DT01				(USHORT)0x0100	// DT - 01Çü, A-GEAR, 256
#define UNITKIND_DT02				(USHORT)0x0200	// DT - 02Çü, A-GEAR, 512
#define UNITKIND_DT03				(USHORT)0x0400	// DT - 03Çü, A-GEAR, 1024
#define UNITKIND_DT04				(USHORT)0x0800	// DT - 04Çü, A-GEAR, 2048

#define UNITKIND_ST01				(USHORT)0x1000	// ST - 01Çü, I-GEAR, 4096
#define UNITKIND_ST02				(USHORT)0x2000	// ST - 02Çü, I-GEAR, 8192
#define UNITKIND_ST03				(USHORT)0x4000	// ST - 03Çü, I-GEAR, 16384
#define UNITKIND_ST04				(USHORT)0x8000	// ST - 04Çü, I-GEAR, 32768

#define UNITKIND_BGEAR_MASK			(USHORT)0x000F	// B-GEAR, 15
#define UNITKIND_MGEAR_MASK			(USHORT)0x00F0	// M-GEAR, 240
#define UNITKIND_AGEAR_MASK			(USHORT)0x0F00	// A-GEAR, 3840
#define UNITKIND_IGEAR_MASK			(USHORT)0xF000	// I-GEAR, 61440
#define UNITKIND_ALL_MASK			(USHORT)0xFFFF	// 65535

#define IS_BT(x) ((x&UNITKIND_BGEAR_MASK) != 0)	// B-GEAR
#define IS_OT(x) ((x&UNITKIND_MGEAR_MASK) != 0)	// M-GEAR
#define IS_DT(x) ((x&UNITKIND_AGEAR_MASK) != 0)	// A-GEAR
#define IS_ST(x) ((x&UNITKIND_IGEAR_MASK) != 0)	// I-GEAR

#define UNITKIND_BGEAR		((USHORT)0x0001)	// B-GEAR, 1
#define UNITKIND_MGEAR		((USHORT)0x0010)	// M-GEAR, 16
#define UNITKIND_AGEAR		((USHORT)0x0100)	// A-GEAR, 256
#define UNITKIND_IGEAR		((USHORT)0x1000)	// I-GEAR, 4096

#define IS_BGEAR(x) ((x&UNITKIND_BGEAR_MASK) != 0)	// B-GEAR
#define IS_MGEAR(x) ((x&UNITKIND_MGEAR_MASK) != 0)	// M-GEAR
#define IS_AGEAR(x) ((x&UNITKIND_AGEAR_MASK) != 0)	// A-GEAR
#define IS_IGEAR(x) ((x&UNITKIND_IGEAR_MASK) != 0)	// I-GEAR
#define IS_SAME_UNITKIND(x1, x2) ( (IS_BGEAR(x1)&&IS_BGEAR(x2))	 ||	\
									(IS_MGEAR(x1)&&IS_MGEAR(x2)) ||	\
									(IS_AGEAR(x1)&&IS_AGEAR(x2)) ||	\
									(IS_IGEAR(x1)&&IS_IGEAR(x2)) )

////////////////////////////////////
// Ä³¸¯ÅÍ ½ÅºÐ, STATUS_XXX
#define STATUS_BEGINNER_AIRMAN		((BYTE)0)	// ÈÆ·Ã ¿¡¾î¸Ç, 1 ~ 100	
#define STATUS_3RD_CLASS_AIRMAN		((BYTE)1)	// 3rd ¿¡¾î¸Ç, 101 ~ 300, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_2ND_CLASS_AIRMAN		((BYTE)2)	// 2nd ¿¡¾î¸Ç, 301 ~ 600, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_1ST_CLASS_AIRMAN		((BYTE)3)	// 1st ¿¡¾î¸Ç, 601 ~ 1000, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_3RD_CLASS_WINGMAN	((BYTE)4)	// 3rd À®¸Ç, 1001 ~ 1500, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_2ND_CLASS_WINGMAN	((BYTE)5)	// 2nd À®¸Ç, 1501 ~ 2000, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_1ST_CLASS_WINGMAN	((BYTE)6)	// 1st À®¸Ç, 2001 ~ 2500, ¹Ì¼ÇÀ¸·Î ½Â±Þ °¡´É
#define STATUS_3RD_CLASS_LEADER		((BYTE)7)	// 3rd ¸®´õ, 2501 ~ 5000, ¿©´Ü ¼³¸³
#define STATUS_2ND_CLASS_LEADER		((BYTE)8)	// 2nd ¸®´õ, 5000 ~ 10000, ¿©´Ü ¼³¸³ ÈÄ 1´Ü°è ¿©´Ü ¹Ì¼Ç Å¬¸®¾î ½Ã (5¸í Ãß°¡ ÀÎ¿ø È®Àå), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_1ST_CLASS_LEADER		((BYTE)9)	// 1st ¸®´õ, 10001 ~ 15000, ¿©´Ü ¼³¸³ ÈÄ 2´Ü°è ¿©´Ü ¹Ì¼Ç Å¬¸®¾î ½Ã (5¸í Ãß°¡ ÀÎ¿ø È®Àå ¹× ¿©´Ü ¸¶Å© ÀåÂø °¡´É), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_3RD_CLASS_ACE		((BYTE)10)	// 3rd ¿¡ÀÌ½º, 15001 ~ 20000, ¿©´Ü ¼³¸³ ÈÄ 3´Ü°è ¿©´Ü ¹Ì¼Ç Å¬¸®¾î ½Ã (5¸í Ãß°¡ ÀÎ¿ø È®Àå ¹× ¿©´Ü Ã¢°í °øÀ¯ °¡´É), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_2ND_CLASS_ACE		((BYTE)11)	// 2nd ¿¡ÀÌ½º, 20000 ~ 25000, ¿©´Ü ¼³¸³ ÈÄ 4´Ü°è ¿©´Ü ¹Ì¼Ç Å¬¸®¾î ½Ã (5¸í Ãß°¡ ÀÎ¿ø È®Àå ¹× µµ½ÃÀüÅõ ½ÅÃ» °¡´É), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_1ST_CLASS_ACE		((BYTE)12)	// 1st ¿¡ÀÌ½º, 25001 ~ 30000, ¿©´Ü ¼³¸³ ÈÄ 5´Ü°è ¿©´Ü ¹Ì¼Ç Å¬¸®¾î ½Ã (10¸í Ãß°¡ ÀÎ¿ø È®Àå), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_COPPER_CLASS_GENERAL	((BYTE)13)	// ÁØÀå¼º, -, ¿©´Ü ¼³¸³ ÈÄ °è±Þ ½ÂÁø ¹Ì¼Ç Å¬¸®¾î ½Ã (10¸í Ãß°¡ ÀÎ¿ø È®Àå), ÇØÃ¼ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_SILVER_CLASS_GENERAL	((BYTE)14)	// ¼ÒÀå¼º, -, 1°³ÀÇ µµ½Ã¿¡ ´ëÇÑ Á¡·É±Ç ¼ÒÀ¯ , ¼ÒÀ¯ ÈÄ ½ÇÆÐ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_GOLD_CLASS_GENERAL	((BYTE)15)	// ÁßÀå¼º, -, 2°³ÀÇ µµ½Ã¿¡ ´ëÇÑ Á¡·É±Ç ¼ÒÀ¯, ¼ÒÀ¯ ÈÄ ½ÇÆÐ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ
#define STATUS_MASTER_GENERAL		((BYTE)16)	// ´ëÀå¼º, -, ´ë·ú ³» Àü µµ½Ã¿¡ ´ëÇÑ Á¡·É±Ç ¼ÒÀ¯, ¼ÒÀ¯ ÈÄ ½ÇÆÐ ½Ã ·¹º§¿¡ µû¸¥ ½Àµæ °è±ÞÀ¸·Î º¯°æ

////////////////////////////////////
// Ä³¸¯ÅÍ & ¸ó½ºÅÍÀÇ »óÅÂ
typedef UINT64 BodyCond_t;

#define NUM_OF_BODYCONDITION_BIT		64

// Body Conditions
#define BODYCON_SET_OR_CLEAR_MASK		(BodyCond_t)0x0000000000000001		// set(1) or clear(0)
// start - exclusive condition
#define BODYCON_FLY_MASK				(BodyCond_t)0x0000000000000002		// ºñÇà
#define BODYCON_LANDING_MASK			(BodyCond_t)0x0000000000000040		// Âø·úÁß, exclusiveÇØ¾ß ÇÑ´Ù°í ¿äÃ»µÊ, by kelovon w/ jinkin, 20040203
#define BODYCON_LANDED_MASK				(BodyCond_t)0x0000000000000004		// Âø·ú ¿Ï·á
#define BODYCON_DEAD_MASK				(BodyCond_t)0x0000000000000008		// Æø¹ß
#define BODYCON_NOT_USED1_MASK			(BodyCond_t)0x0000000000000010
// end - exclusive condition
// start - new bodycon
#define BODYCON_WEAPON_POSITION_MASK	(BodyCond_t)0x0000000000000020		// 2005-12-16 by cmkwon, 1Çü/2Çü ¹«±â Æ÷Áö¼ÇÀ» À§ÇÑ ¹ÙµðÄÁµð¼Ç, BODYCON_NOT_USED2_MASK¸¦ º¯°æÇÔ
#define BODYCON_BOOSTER1_MASK			(BodyCond_t)0x0000000000000080		// ºÎ½ºÅÍ(1~10) - Æò»ó½Ã(°¡¸¸È÷ ÀÖÀ» ¶§)
#define BODYCON_BOOSTER2_MASK			(BodyCond_t)0x0000000000000100		// ºÎ½ºÅÍ(10~15) - ÀüÁø½Ã(WÅ°¸¦ ´©¸¥ »óÅÂ)
#define BODYCON_BOOSTER3_MASK			(BodyCond_t)0x0000000000000200		// ºÎ½ºÅÍ(70~80) - ºÎ½ºÅÍ½Ã(SpaceÅ°¸¦ ´©¸¥ »óÅÂ)
#define BODYCON_BOOSTER4_MASK			(BodyCond_t)0x0000000000000400		// ºÎ½ºÅÍ(81~100) - Á¤Áö½Ã(SÅ°¸¦ ´©¸¥ »óÅÂ)
#define BODYCON_NIGHTFLY_MASK			(BodyCond_t)0x0000000000000800		// ¾ß°£ºñÇà(¾ß°£µî)
#define BODYCON_EXPLOSION_MASK			(BodyCond_t)0x0000000000001000		// Æø¹ß
#define BODYCON_DAMAGE1_MASK			(BodyCond_t)0x0000000000002000		// µ¥¹ÌÁö1, Áö¼ÓÇü
#define BODYCON_DAMAGE2_MASK			(BodyCond_t)0x0000000000004000		// µ¥¹ÌÁö2, Áö¼ÓÇü
#define BODYCON_DAMAGE3_MASK			(BodyCond_t)0x0000000000008000		// µ¥¹ÌÁö3, Áö¼ÓÇü
#define BODYCON_CREATION_MASK			(BodyCond_t)0x0000000000010000		// ¸®½ºÆù
#define BODYCON_FIRE_MASK				(BodyCond_t)0x0000000000020000		// ¹ß»ç
#define BODYCON_BULLET_MASK				(BodyCond_t)0x0000000000040000		// ÅºÃ¼
#define BODYCON_HIT_MASK				(BodyCond_t)0x0000000000080000		// Å¸°Ý


///////////////////////////////////////////////////////////////////////////////
// Å¬¶óÀÌ¾ðÆ®¿¡¼­¸¸ »ç¿ëµÇ´Â BodyCondition
// ·¹ÀÌ´Ù ¹Ùµð ÄÁµð¼Ç ¼³Á¤
#define RADAR_BODYCON_BOOSTER_ON		(BodyCond_t)0x0000000000000008		// ÄÑÁø »óÅÂ
#define RADAR_BODYCON_BOOSTER_OFF		(BodyCond_t)0x0000000000000002		// ²¨Áø »óÅÂ

// cmkwon, ¿©±âºÎÅÍ´Â Ä³¸¯°ú ¸ó½ºÅÍ°¡ ´Ù¸£°Ô »ç¿ëÇÔ

///////////////////////////////////////////////////////////////////////////////
// Strat --> Character BodyCondition, BODYCON_XXX
#define BODYCON_SPRAY_MASK					(BodyCond_t)0x0000000000100000		// ¹°º¸¶ó,¸ÕÁö,´«
#define BODYCON_APRE_MASK					(BodyCond_t)0x0000000000200000		// 2005-12-07 by cmkwon, A±â¾î Æ÷ ÁöÁö´ë Bodycondition
// end - new bodycon
#define BODYCON_SHAKEING_MASK				(BodyCond_t)0x0000000004000000		// Èçµé¸²
#define BODYCON_FREEZING_MASK				(BodyCond_t)0x0000000008000000		// µ¿°á
#define BODYCON_ATTACKSLOW_MASK				(BodyCond_t)0x0000000010000000		// Àç°ø°Ý ´À·ÁÁü
#define BODYCON_SLOW_MASK					(BodyCond_t)0x0000000020000000		// ½ºÇÇµå ´À·ÁÁü
#define BODYCON_NOATTACK_MASK				(BodyCond_t)0x0000000040000000		// °ø°Ý±ÝÁö
#define BODYCON_ILLUSION_MASK				(BodyCond_t)0x0000000080000000		// Âø°¢
#define BODYCON_FIGHTING_MASK				(BodyCond_t)0x0000000100000000		// »ç±â°í¾ç
#define BODYCON_EVENT_HANDLE_MASK			(BodyCond_t)0x0000000200000000		// ÀÌº¥Æ®(¿öÇÁ, »óÁ¡) Ã³¸®Áß, ¼­¹ö´Â clientÀÇ ÀÌµ¿ Á¤º¸ ¹«½Ã
#define BODYCON_STOP_MASK					(BodyCond_t)0x0000000400000000		// 2010-03-30 by cmkwon, ÇÊµåÃ¢°í ºñÇàÁß ÀÌ¿ë ¹ö±× ¼öÁ¤(E0044710) - ±â¾î»óÅÂ ÀÏ¶§ B+Enter »óÅÂ, // Á¤Áö»óÅÂ	------> ¿©±â ÀÌÇÏ´Â 2002.11.25.¿¡ Ãß°¡
#define BODYCON_STEALTH_MASK				(BodyCond_t)0x0000000800000000		// ½ºÅÚ½º»óÅÂ
#define BODYCON_GHOST_MASK					(BodyCond_t)0x0000001000000000		// °í½ºÆ®»óÅÂ
#define BODYCON_CLOAKING_MASK				(BodyCond_t)0x0000002000000000		// Å¬·ÎÅ·»óÅÂ
#define BODYCON_FALL_MASK					(BodyCond_t)0x0000004000000000		// Ãß¶ô»óÅÂ
#define BODYCON_SKILLBAN_MASK				(BodyCond_t)0x0000008000000000		// ½ºÅ³»ç¿ë±ÝÁö»óÅÂ
#define BODYCON_RESIST_MASK					(BodyCond_t)0x0000010000000000		// ½ºÅ³ÀúÇ×»óÅÂ
#define BODYCON_SHIELD_MASK					(BodyCond_t)0x0000020000000000		// ½¯µå »óÅÂ, 20030917¿¡ Ãß°¡
#define BODYCON_BOOSTER5_MASK				(BodyCond_t)0x0000040000000000		// ¿ªÃàÁø ºÎ½ºÅÍ
#define BODYCON_TAKEOFF_MASK				(BodyCond_t)0x0000080000000000		// ÀÌ·ú
#define BODYCON_SIEGE_ON_MASK				(BodyCond_t)0x0000100000000000		// ½ÃÁî ¸ðµå ¿Â
#define BODYCON_SIEGE_OFF_MASK				(BodyCond_t)0x0000200000000000		// ½ÃÁî ¸ðµå ¿ÀÇÁ
#define BODYCON_DECOY_MASK					(BodyCond_t)0x0000400000000000		// µðÄÚÀÌ »óÅÂ
#define BODYCON_CHARACTER_MODE_STOP			(BodyCond_t)0x0001000000000000		// Ä³¸¯ÅÍ ¸ðµå - ¸ØÃã
#define BODYCON_CHARACTER_MODE_WALK			(BodyCond_t)0x0002000000000000		// Ä³¸¯ÅÍ ¸ðµå - °È±â
#define BODYCON_CHARACTER_MODE_RUN			(BodyCond_t)0x0004000000000000		// Ä³¸¯ÅÍ ¸ðµå - ¶Ù±â
#define BODYCON_EVENT_SELECTCHANNEL_MASK	(BodyCond_t)0x0008000000000000		// Ã¤³Î ¼±ÅÃ Ã¢ »óÅÂ, BODYCON_EVENT_HANDLE_MASK¿Í °°ÀÌ ¾²ÀÎ´Ù
#define BODYCON_ROLLING_MASK				(BodyCond_t)0x0010000000000000		// 2009-07-07 by cmkwon, ·Îº¿ ¾Æ¸Ó °ü·Ã Ã³¸® Ãß°¡ - ·Ñ¸µ ¹ÙµðÄÁµð¼Ç Ãß°¡
#define BODYCON_PET_POSITION_MASK			(BodyCond_t)0x0020000000000000		// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ ÆÑ ÀåÂø À§Ä¡ BodyCon Ãß°¡
#define BODYCON_MARK_POSITION_MASK			(BodyCond_t)0x0040000000000000		// 2011-03-21 by jhAhn	¸¶Å©½Ã½ºÅÛ Ã¢ÂøÀ§Ä¡ BodyCon Ãß°¡
// End --> Character BodyCondition
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Start --> Monster BodyCondition
#define COUNT_MONSTER_ATTACK_BODYCON	2		// ÇÏ³ªÀÇ Attack´ç °¡Áö´Â »óÅÂÀÇ ¼ýÀÚ(PreAttack, FireAttack)

#define BODYCON_MON_PREATTACK1_MASK		(BodyCond_t)0x0000000000100000		// °ø°Ý1ÀÇ ÁØºñ			(Effect = 20)
#define BODYCON_MON_FIREATTACK1_MASK	(BodyCond_t)0x0000000000200000		// °ø°Ý1ÀÇ ¹ß»ç			(Effect = 21)

#define BODYCON_MON_PREATTACK2_MASK		(BodyCond_t)0x0000000000400000		// °ø°Ý2ÀÇ ÁØºñ			(Effect = 22)
#define BODYCON_MON_FIREATTACK2_MASK	(BodyCond_t)0x0000000000800000		// °ø°Ý2ÀÇ ¹ß»ç			(Effect = 23)

#define BODYCON_MON_PREATTACK3_MASK		(BodyCond_t)0x0000000001000000		// °ø°Ý3ÀÇ ÁØºñ			(Effect = 24)
#define BODYCON_MON_FIREATTACK3_MASK	(BodyCond_t)0x0000000002000000		// °ø°Ý3ÀÇ ¹ß»ç			(Effect = 25)

#define BODYCON_MON_PREATTACK4_MASK		(BodyCond_t)0x0000000004000000		// °ø°Ý4ÀÇ ÁØºñ			(Effect = 26)
#define BODYCON_MON_FIREATTACK4_MASK	(BodyCond_t)0x0000000008000000		// °ø°Ý4ÀÇ ¹ß»ç			(Effect = 27)

#define BODYCON_MON_PREATTACK5_MASK		(BodyCond_t)0x0000000010000000		// °ø°Ý5ÀÇ ÁØºñ			(Effect = 28)
#define BODYCON_MON_FIREATTACK5_MASK	(BodyCond_t)0x0000000020000000		// °ø°Ý5ÀÇ ¹ß»ç			(Effect = 29)

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÄÁÆ®·Ñ½ºÅ³·Î ÀÌ¸§ º¯°æ
//#define BODYCON_MON_PREATTACK6_MASK		(BodyCond_t)0x0000000040000000		// °ø°Ý6ÀÇ ¹ß»ç			(Effect = 30)
//#define BODYCON_MON_FIREATTACK6_MASK	(BodyCond_t)0x0000000080000000		// °ø°Ý6ÀÇ ¿Ï·á			(Effect = 31)
#define BODYCON_MON_PRECONTROLSKILL_MASK	(BodyCond_t)0x0000000040000000		// ÄÁÆ®·Ñ½ºÅ³ÀÇ ¹ß»ç			(Effect = 30)
#define BODYCON_MON_FIRECONTROLSKILL_MASK	(BodyCond_t)0x0000000080000000		// ÄÁÆ®·Ñ½ºÅ³ÀÇ ¿Ï·á			(Effect = 31)

#define BODYCON_MON_AUTODESTROYED_MASK	(BodyCond_t)0x0000000100000000		// ¸ó½ºÅÍ ÀÚµ¿ ¼Ò¸ê		(Effect = 32)
#define BODYCON_MON_BUILDING			(BodyCond_t)0x0000001000000000		// 2007-09-06 by cmkwon, ¸ó½ºÅÍ º¯½ÅÁß			(Effect = 36)
#define BODYCON_MON_BUILDED				(BodyCond_t)0x0000002000000000		// 2007-09-06 by cmkwon, ¸ó½ºÅÍ º¯½Å¿Ï·á		(Effect = 37)
////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 6~15±îÁö Ãß°¡
#define BODYCON_MON_PREATTACK6_MASK		(BodyCond_t)0x0000010000000000		// °ø°Ý6ÀÇ ÁØºñ			(Effect = 40)
#define BODYCON_MON_FIREATTACK6_MASK	(BodyCond_t)0x0000020000000000		// °ø°Ý6ÀÇ ¹ß»ç			(Effect = 41)
#define BODYCON_MON_PREATTACK7_MASK		(BodyCond_t)0x0000040000000000		// °ø°Ý7ÀÇ ÁØºñ			(Effect = 42)
#define BODYCON_MON_FIREATTACK7_MASK	(BodyCond_t)0x0000080000000000		// °ø°Ý7ÀÇ ¹ß»ç			(Effect = 43)
#define BODYCON_MON_PREATTACK8_MASK		(BodyCond_t)0x0000100000000000		// °ø°Ý8ÀÇ ÁØºñ			(Effect = 44)
#define BODYCON_MON_FIREATTACK8_MASK	(BodyCond_t)0x0000200000000000		// °ø°Ý8ÀÇ ¹ß»ç			(Effect = 45)
#define BODYCON_MON_PREATTACK9_MASK		(BodyCond_t)0x0000400000000000		// °ø°Ý9ÀÇ ÁØºñ			(Effect = 46)
#define BODYCON_MON_FIREATTACK9_MASK	(BodyCond_t)0x0000800000000000		// °ø°Ý9ÀÇ ¹ß»ç			(Effect = 47)
#define BODYCON_MON_PREATTACK10_MASK	(BodyCond_t)0x0001000000000000		// °ø°Ý10ÀÇ ÁØºñ			(Effect = 48)
#define BODYCON_MON_FIREATTACK10_MASK	(BodyCond_t)0x0002000000000000		// °ø°Ý10ÀÇ ¹ß»ç			(Effect = 49)
#define BODYCON_MON_PREATTACK11_MASK	(BodyCond_t)0x0004000000000000		// °ø°Ý11ÀÇ ÁØºñ			(Effect = 50)
#define BODYCON_MON_FIREATTACK11_MASK	(BodyCond_t)0x0008000000000000		// °ø°Ý11ÀÇ ¹ß»ç			(Effect = 51)
#define BODYCON_MON_PREATTACK12_MASK	(BodyCond_t)0x0010000000000000		// °ø°Ý12ÀÇ ÁØºñ			(Effect = 52)
#define BODYCON_MON_FIREATTACK12_MASK	(BodyCond_t)0x0020000000000000		// °ø°Ý12ÀÇ ¹ß»ç			(Effect = 53)
#define BODYCON_MON_PREATTACK13_MASK	(BodyCond_t)0x0040000000000000		// °ø°Ý13ÀÇ ÁØºñ			(Effect = 54)
#define BODYCON_MON_FIREATTACK13_MASK	(BodyCond_t)0x0080000000000000		// °ø°Ý13ÀÇ ¹ß»ç			(Effect = 55)
#define BODYCON_MON_PREATTACK14_MASK	(BodyCond_t)0x0100000000000000		// °ø°Ý14ÀÇ ÁØºñ			(Effect = 56)
#define BODYCON_MON_FIREATTACK14_MASK	(BodyCond_t)0x0200000000000000		// °ø°Ý14ÀÇ ¹ß»ç			(Effect = 57)
#define BODYCON_MON_PREATTACK15_MASK	(BodyCond_t)0x0400000000000000		// °ø°Ý15ÀÇ ÁØºñ			(Effect = 58)
#define BODYCON_MON_FIREATTACK15_MASK	(BodyCond_t)0x0800000000000000		// °ø°Ý15ÀÇ ¹ß»ç			(Effect = 59)

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 6~15, ÄÁÆ®·Ñ½ºÅ³±îÁö Ãß°¡
#define BODYCON_MON_ATTACK1_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK1_MASK | BODYCON_MON_FIREATTACK1_MASK)	// °ø°Ý1
#define BODYCON_MON_ATTACK2_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK2_MASK | BODYCON_MON_FIREATTACK2_MASK)	// °ø°Ý2
#define BODYCON_MON_ATTACK3_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK3_MASK | BODYCON_MON_FIREATTACK3_MASK)	// °ø°Ý3
#define BODYCON_MON_ATTACK4_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK4_MASK | BODYCON_MON_FIREATTACK4_MASK)	// °ø°Ý4
#define BODYCON_MON_ATTACK5_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK5_MASK | BODYCON_MON_FIREATTACK5_MASK)	// °ø°Ý5
#define BODYCON_MON_ATTACK6_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK6_MASK | BODYCON_MON_FIREATTACK6_MASK)	// °ø°Ý6
#define BODYCON_MON_ATTACK7_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK7_MASK | BODYCON_MON_FIREATTACK7_MASK)	// °ø°Ý7
#define BODYCON_MON_ATTACK8_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK8_MASK | BODYCON_MON_FIREATTACK8_MASK)	// °ø°Ý8
#define BODYCON_MON_ATTACK9_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK9_MASK | BODYCON_MON_FIREATTACK9_MASK)	// °ø°Ý9
#define BODYCON_MON_ATTACK10_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK10_MASK | BODYCON_MON_FIREATTACK10_MASK)	// °ø°Ý10
#define BODYCON_MON_ATTACK11_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK11_MASK | BODYCON_MON_FIREATTACK11_MASK)	// °ø°Ý11
#define BODYCON_MON_ATTACK12_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK12_MASK | BODYCON_MON_FIREATTACK12_MASK)	// °ø°Ý12
#define BODYCON_MON_ATTACK13_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK13_MASK | BODYCON_MON_FIREATTACK13_MASK)	// °ø°Ý13
#define BODYCON_MON_ATTACK14_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK14_MASK | BODYCON_MON_FIREATTACK14_MASK)	// °ø°Ý14
#define BODYCON_MON_ATTACK15_MASK		(BodyCond_t)(BODYCON_MON_PREATTACK15_MASK | BODYCON_MON_FIREATTACK15_MASK)	// °ø°Ý15
#define BODYCON_MON_CONTROLSKILL_MASK	(BodyCond_t)(BODYCON_MON_PRECONTROLSKILL_MASK | BODYCON_MON_FIRECONTROLSKILL_MASK)	// ÄÁÆ®·Ñ½ºÅ³
#define BODYCON_MON_PREATTACK_ALL_MASK	(BodyCond_t)(BODYCON_MON_PREATTACK1_MASK | BODYCON_MON_PREATTACK2_MASK \
													| BODYCON_MON_PREATTACK3_MASK | BODYCON_MON_PREATTACK4_MASK \
													| BODYCON_MON_PREATTACK5_MASK | BODYCON_MON_PREATTACK6_MASK \
													| BODYCON_MON_PREATTACK7_MASK | BODYCON_MON_PREATTACK8_MASK \
													| BODYCON_MON_PREATTACK9_MASK | BODYCON_MON_PREATTACK10_MASK \
													| BODYCON_MON_PREATTACK11_MASK | BODYCON_MON_PREATTACK12_MASK \
													| BODYCON_MON_PREATTACK13_MASK | BODYCON_MON_PREATTACK14_MASK \
													| BODYCON_MON_PREATTACK15_MASK | BODYCON_MON_PRECONTROLSKILL_MASK)	// ¸ðµç °ø°Ý ÁØºñ
#define BODYCON_MON_FIREATTACK_ALL_MASK	(BodyCond_t)(BODYCON_MON_FIREATTACK1_MASK | BODYCON_MON_FIREATTACK2_MASK \
													| BODYCON_MON_FIREATTACK3_MASK | BODYCON_MON_FIREATTACK4_MASK \
													| BODYCON_MON_FIREATTACK5_MASK | BODYCON_MON_FIREATTACK6_MASK \
													| BODYCON_MON_FIREATTACK7_MASK | BODYCON_MON_FIREATTACK8_MASK \
													| BODYCON_MON_FIREATTACK9_MASK | BODYCON_MON_FIREATTACK10_MASK \
													| BODYCON_MON_FIREATTACK11_MASK | BODYCON_MON_FIREATTACK12_MASK \
													| BODYCON_MON_FIREATTACK13_MASK | BODYCON_MON_FIREATTACK14_MASK \
													| BODYCON_MON_FIREATTACK15_MASK | BODYCON_MON_FIRECONTROLSKILL_MASK)	// ¸ðµç °ø°Ý ÁØºñ
#define BODYCON_MON_ATTACKALL_MASK		(BodyCond_t)(BODYCON_MON_ATTACK1_MASK | BODYCON_MON_ATTACK2_MASK \
													| BODYCON_MON_ATTACK3_MASK | BODYCON_MON_ATTACK4_MASK \
													| BODYCON_MON_ATTACK5_MASK | BODYCON_MON_ATTACK6_MASK \
													| BODYCON_MON_ATTACK7_MASK | BODYCON_MON_ATTACK8_MASK \
													| BODYCON_MON_ATTACK9_MASK | BODYCON_MON_ATTACK10_MASK \
													| BODYCON_MON_ATTACK11_MASK | BODYCON_MON_ATTACK12_MASK \
													| BODYCON_MON_ATTACK13_MASK | BODYCON_MON_ATTACK14_MASK \
													| BODYCON_MON_ATTACK15_MASK | BODYCON_MON_CONTROLSKILL_MASK)		// ¸ó½ºÅÍ °ø°Ý °ü·Ã BodyConditionÀ» ClearÇÏ±â À§ÇÑ define

// End --> Monster BodyCondition
///////////////////////////////////////////////////////////////////////////////

// bit ¿¬»ê ¸ÅÅ©·Î
#define CLEAR_BODYCON_BIT(VAR, MASK) { VAR &= ~(MASK); }
#define SET_BODYCON_BIT(VAR, MASK)								\
{																\
	if ((MASK & BODYCON_EX_STATE_CLEAR_MASK) != 0)				\
	{															\
		CLEAR_BODYCON_BIT(VAR, BODYCON_EX_STATE_CLEAR_MASK);	\
	}															\
	if ((MASK & BODYCON_BOOSTER_EX_STATE_CLEAR_MASK) != 0)		\
	{															\
		CLEAR_BODYCON_BIT(VAR, BODYCON_BOOSTER_EX_STATE_CLEAR_MASK);\
	}															\
	VAR |= MASK;												\
}
#define COMPARE_BODYCON_BIT(VAR, MASK) (((VAR) & (MASK)) != 0)
#define BODYCON_EX_STATE_CLEAR_MASK		(BodyCond_t)(BODYCON_FLY_MASK | BODYCON_LANDING_MASK | BODYCON_LANDED_MASK | BODYCON_DEAD_MASK | BODYCON_NOT_USED1_MASK | BODYCON_CHARACTER_MODE_STOP | BODYCON_CHARACTER_MODE_WALK | BODYCON_CHARACTER_MODE_RUN)
										// BODYCON_FLY_MASK | BODYCON_LANDED_MASK | BODYCON_LANDING_MASK | BODYCON_DEAD_MASK | BODYCON_NOT_USED_MASK
										// À§ÀÇ ³×°¡Áö bitÀ» setÇÒ¶§´Â Ç×»ó ÀÌ mask·Î clearÇØÁÖ¾î¾ß ÇÔ
										// ³× state°¡ exclusiveÇÏ±â ¶§¹®ÀÌ´Ù
										//
										// ex)
										// CLEAR_BODYCON_BIT( c, BODYCON_EX_STATE_CLEAR_MASK)
										// SET_BODYCON_BIT( c, BODYCON_FLY_MASK)
#define BODYCON_BOOSTER_EX_STATE_CLEAR_MASK	(BodyCond_t)(BODYCON_BOOSTER1_MASK | BODYCON_BOOSTER2_MASK | BODYCON_BOOSTER3_MASK | BODYCON_BOOSTER4_MASK \
											| BODYCON_BOOSTER5_MASK | BODYCON_SIEGE_ON_MASK | BODYCON_SIEGE_OFF_MASK)
											// BODYCON_BOOSTER1_MASK | BODYCON_BOOSTER2_MASK | BODYCON_BOOSTER3_MASK | BODYCON_BOOSTER4_MASK
#define BODYCON_KEEPING_MASK			(BodyCond_t)(BODYCON_EX_STATE_CLEAR_MASK)	// Áö¼ÓÇü body coditions
#define BODYCON_CHARACTER_MODE_MASK		(BodyCond_t)(BODYCON_CHARACTER_MODE_STOP|BODYCON_CHARACTER_MODE_WALK|BODYCON_CHARACTER_MODE_RUN)		// 2006-01-23 by cmkwon
#define BODYCON_MON_BUILD_MASK			(BodyCond_t)(BODYCON_MON_BUILDING|BODYCON_MON_BUILDED)		// 2007-09-10 by dhjin



////////////////////////////////////
// ¸ó½ºÅÍÀÇ °è¿­
#define FACTION_GENERAL					0	// ÀÏ¹ÝÀûÀÎ ÇüÅÂ
#define FACTION_GUN						1	// Æ÷ÇüÅÂ (±â°üÆ÷, ºö)
#define FACTION_ROCKET					2	// ·ÎÄÏÇüÅÂ (·ÎÄÏ, À¯µµÅº)
#define FACTION_SPECIAL					3	// Æ¯¼öÇÑ ÇüÅÂ (ºÎ°úÀûÀÎ ±â´ÉÀÌ ÀÖ´Â ¹«±â)


///////////////////////////////////
// MonsterForm(¸ó½ºÅÍÀÇ ÇüÅÂ)
#define FORM_FLYING_RIGHT				0	// Á÷¼±ºñÇàÇü (ºñÇà±âÇüÅÂ·Î ºñÇàÇÏ¸ç °ø°Ý)
#define FORM_FLYING_COPTER				1	// Çï±âºñÇàÇü (Çï±âÇüÅÂ·Î ºñÇàÇÏ¸ç °ø°Ý)
#define FORM_GROUND_MOVE				2	// Áö»óÇü	  (¶¥ À§¸¦ ÀÌµ¿ÇÏ¸ç °ø°Ý)
#define FORM_FLYINGandGROUND_RIGHT		3	// Áö»óºñÇàÇü (ºñÇà)
#define FORM_FLYINGandGROUND_COPTER		4	// Áö»óºñÇàÇü (ºñÇà)
#define FORM_SWIMMINGFLYING_RIGHT		5	// À¯¿µºñÇàÇü (¹°¼Ó¿¡¼­ ¹°À§·Î ³ª¿Í °ø°Ý, Á÷¼±ºñÇà¼Í)
#define FORM_SWIMMINGFLYING_COPTER		6	// À¯¿µºñÇàÇü (¹°¼Ó¿¡¼­ ¹°À§·Î ³ª¿Í °ø°Ý, Çï±â ºñÇàÇü)
//#define FORM_SWIMMING					7	// ¹°¼ÓÀ¯¿µÇü (¹°¼Ó¿¡¼­ À¯¿µ)
#define FORM_OBJECT_STOP				8	// ¿ÀºêÁ§Æ® ¸ó½ºÅÍ TargetVectorÀÇ º¯È­°¡ ÀüÇô¾øÀ½
#define FORM_OBJECT_PLANE_ROTATE		9	// ¿ÀºêÁ§Æ® ¸ó½ºÅÍ TargetVector°¡ ¼öÆòÀ¸·Î¸¸ ÀÌµ¿µÊ(2Â÷¿ø º¯È­)
#define FORM_OBJECT_CANNON				10	// ¿ÀºêÁ§Æ® ¸ó½ºÅÍ TargetVector°¡ ±¸ÇüÅÂ·Î ÀÌµ¿ °¡´ÉÇÔ

#define IS_OBJECT_MON_FORM(MonForm)		((MonForm) == FORM_OBJECT_STOP || (MonForm) == FORM_OBJECT_PLANE_ROTATE || (MonForm) == FORM_OBJECT_CANNON)		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 

///////////////////////////////////
// Monster Attack Pattern(¸ó½ºÅÍÀÇ °ø°Ý ÇüÅÂ)
#define ATTPAT_NORMAL					0	// ÀÏ¹Ý ÀûÀÎ °ø°Ý ÇüÅÂ
//#define ATTPAT_BODYSLAM					1	// ¸öÃ¼¸¦ ÀÌ¿ëÇÑ ¸öÅë°ø°Ý
#define ATTPAT_SELFDESTRUCTION			2	// ¸öÃ¼¸¦ ÀÌ¿ëÇÑ ÀÚÆø°ø°Ý
#define ATTPAT_RETREAT					3	// HP°¡ 30% ÀÌÇÏÀÏ °æ¿ì ÈÄÅð¿Í °ø°ÝÀ» ¹Ýº¹
#define ATTPAT_HITANDRUN				4	// °ø°Ý ÈÄ ÈÄÅð¸¦ ¹Ýº¹ÇÏ´Â ÇüÅÂ
// 2005-05-02 by cmkwon, AttackObjec¿¡ ¼³Á¤ÇØ¾ßÇÔ
//#define ATTPAT_TIBE						5	// µ¿Á· °ø°ÝÀ» ¹ÞÀ¸¸é °°ÀÌ °ø°ÝÇÏ´Â ÇüÅÂ
#define ATTPAT_ONEATATTCK				6	// Ã³À½ Å¸°ÙÀ» ÀâÀº À¯´Ö¸¸ ¹«Á¶°Ç °ø°ÝÇÏ´Â ÇüÅÂ
#define ATTPAT_PK						7	// PK ¼öÄ¡°¡ ³ôÀº »ç¶÷À» °ø°Ý

///////////////////////////////////
// Monster Move Pattern(¸ó½ºÅÍÀÇ ÀÌµ¿ÇüÅÂ)
#define MOVPAT_STOP						0	// ÀÌµ¿ ¾øÀ½
#define MOVPAT_STRAIGHT					1	// Á÷¼± ÀÌµ¿
#define MOVPAT_RIGHTANDLEFT				2	// ÁÂ¿ì·Î Á÷¼± ÀÌµ¿
#define MOVPAT_8RIGHTANDLEFT			3	// ÁÂ¿ì·Î 8ÀÚ ÀÌµ¿
#define MOVPAT_PATROL					4	// ÀÏÁ¤ ±¸¿ª¿¡¼­¸¸ ¼øÂû
#define MOVPAT_PARTY					5	// ¹«¸®¸¦ Áö¾î ÀÌµ¿

#define MOVPAT_UPANDDOWN				10	// »óÇÏ·Î ÀÌµ¿
#define MOVPAT_OBLIQUE					11	// ´ë°¢¼± »ç¼±À¸·Î Á÷¼± ÀÌµ¿
#define MOVPAT_OBLIQUEANDCURVE			12	// ´ë°¢¼± »ç¼±À¸·Î °î¼± ÀÌµ¿
#define MOVPAT_TELEPORT					13	// ¼ø°£ °ø°£ÀÌµ¿

// start 2011-05-23 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¿þÀÌÆ÷ÀÎÆ® ±¸Çö
#define MOVPAT_FOLLOWPATH				20	//  Á¤ÇØÁø ±æ·Î ÀÌµ¿ (¿þÀÌÆ÷ÀÎÆ® µû¶ó°¡±â)
// end 2011-05-23 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¿þÀÌÆ÷ÀÎÆ® ±¸Çö

///////////////////////////////////
// Monster Belligerence(¸ó½ºÅÍÀÇ È£Àü¼º)
#define BELL_ATATTACK					0	// ¹«Á¶°Ç °ø°ÝÇüÅÂ
#define BELL_DEFENSE					1	// ¼± ¹æ¾î ÈÄ °ø°ÝÇüÅÂ
#define BELL_RETREAT					2	// HP°¡ 30% ÀÌÇÏÀÏ °æ¿ì ¹«Á¶°Ç °ø°ÝÇüÅÂ
#define BELL_TAGETATATTACK				3	// ¹«Á¶°Ç °ø°ÝÇüÅÂ, °¡Àå ¸ÕÀú ¼³Á¤µÈ Å¸°Ù¸¸ °ø°ÝÇÏ´Â ÇüÅÂ
#define BELL_NOTATTACK					4	// Àý´ë·Î °ø°ÝÇÏÁö ¾Ê´ÂÇüÅÂ
#define BELL_ATTACK_OUTPOST_PROTECTOR	5	// NPC¼ÒÀ¯ÀÇ º¸È£¸·, ¼¼·Â ±¸ºÐ¾øÀÌ °ø°ÝÇÑ´Ù.

///////////////////////////////////////////////////////////////////////////////
// 2007-09-05 by dhjin, ±ÔÄ¢ Á¤ÇÔ - ¼¼·ÂÀü ¸ó½ºÅÍ´Â 10 <= BELL_XXX <= 29ÀÌ¸ç Â¦¼ö=VCN, È¦¼ö=ANI, // 2006-11-20 by cmkwon, ±ÔÄ¢ Á¤ÇÔ - ¼¼·ÂÀü ¸ó½ºÅÍ´Â 10 <= BELL_XXX <= 19ÀÌ¸ç Â¦¼ö=VCN, È¦¼ö=ANI
#define BELL_INFLUENCE_VCN				10	// 2005-12-27 by cmkwon, ¹ÙÀÌÁ¦ÀÌ´ÏÀ¯ ¼¼·Â¸ó½ºÅÍ	- ¾Ë¸µÅÏ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.
#define BELL_INFLUENCE_ANI				11	// 2005-12-27 by cmkwon, ¾Ë¸µÅÏ ¼¼·Â¸ó½ºÅÍ			- ¹ÙÀÌÁ¦´ÏÀ¯ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.
#define BELL_STRATEGYPOINT_VCN			12	// 2005-12-27 by cmkwon, Àü·«Æ÷ÀÎÆ® ¼¼·Â¸ó½ºÅÍ		- ¾Ë¸µÅÏ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.
#define BELL_STRATEGYPOINT_ANI			13	// 2005-12-27 by cmkwon, Àü·«Æ÷ÀÎÆ® ¼¼·Â¸ó½ºÅÍ		- ¹ÙÀÌÁ¦´ÏÀ¯ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.
#define BELL_OUTPOST_PROTECTOR_VCN		14	// 2007-08-17 by dhjin,  ¹ÙÀÌÁ¦ÀÌ´ÏÀ¯ ÀüÁø±âÁö º¸È£¸·
#define BELL_OUTPOST_PROTECTOR_ANI		15	// 2007-08-17 by dhjin,  ¾Ë¸µÅÏ ÀüÁø±âÁö º¸È£¸·
#define BELL_OUTPOST_RESET_VCN			16	// 2007-08-17 by dhjin,  ¹ÙÀÌÁ¦ÀÌ´ÏÀ¯ ÀüÁø±âÁö Áß¾Ó Ã³¸® ÀåÄ¡
#define BELL_OUTPOST_RESET_ANI			17	// 2007-08-17 by dhjin,  ¾Ë¸µÅÏ ÀüÁø±âÁö Áß¾Ó Ã³¸® ÀåÄ¡
#define BELL_INFLUENCE_TELEPORT_VCN		18  // 2007-09-05 by dhjin,  ¹ÙÀÌÁ¦´ÏÀ¯ ¸ð¼±Àü½Ã ÅÚ·¹Æ÷Æ® 
#define BELL_INFLUENCE_TELEPORT_ANI		19  // 2007-09-05 by dhjin,  ¾Ë¸µÅÏ ¸ð¼±Àü½Ã ÅÚ·¹Æ÷Æ® 

#define BELL_ONEY_ATTACK_INFLUENCE_VCN	20	// 2010-07-06 by jskim, ±â¿©µµ ¾ø´Â ¼¼·Â ¸ó½ºÅÍ Ãß°¡ ¹ÙÀÌÁ¦ÀÌ´ÏÀ¯ ¼¼·Â, °ø°Ý¸¸ ÇÏ´Â ¸ó½ºÅÍ - ¾Ë¸µÅÏ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.
#define BELL_ONEY_ATTACK_INFLUENCE_ANI	21	// 2010-07-06 by jskim, ±â¿©µµ ¾ø´Â ¼¼·Â ¸ó½ºÅÍ Ãß°¡ ¾Ë¸µÅÏ ¼¼·Â, °ø°Ý¸¸ ÇÏ´Â ¸ó½ºÅÍ - ¹ÙÀÌÁ¦ÀÌ´ÏÀ¯ ¼¼·Â¸¸À» °ø°ÝÇÑ´Ù.

#define BELL_INFINITY_DEFENSE_MONSTER			30	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - À¯Àú °ø°ÝÇÏÁö ¾ÊÀ½, °ªÀÌ 31ÀÎ ¸ó½ºÅÍ °ø°Ý
#define BELL_INFINITY_ATTACK_MONSTER			31	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - °ªÀÌ 30ÀÎ ¸ó½ºÅÍ¿Í ÀÏ¹Ý À¯Àú °ø°Ý

#define IS_INFLWAR_MONSTER(monBell)				( ((monBell) >= 10) && ((monBell) <= 29) )		// 2007-09-05 by dhjin,  19 => 29
#define IS_MOTHERSHIPWAR_MONSTER(monBell)		( ((monBell) >= BELL_INFLUENCE_VCN) && ((monBell) <= BELL_INFLUENCE_ANI) )	// 2007-08-23 by cmkwon, Àü·«Æ÷ÀÎÆ®´Â ¸ð¼±ÀÌ ¾Æ´Ï´Ù, // 2007-08-21 by dhjin, ¸ð¼±Àü¿¡ ÇÊ¿äÇÑ ¸ó½ºÅÍÀÎÁö Ã¼Å©
#define IS_STRATEGYPOINT_MONSTER(monBell)		( ((monBell) >= BELL_STRATEGYPOINT_VCN) && ((monBell) <= BELL_STRATEGYPOINT_ANI) )

#define IS_ONEY_ATTACK_MONSTER(monBell)			( ((monBell) >= BELL_ONEY_ATTACK_INFLUENCE_VCN) && ((monBell) <= BELL_ONEY_ATTACK_INFLUENCE_ANI)) // 2010-07-06 by jskim, ±â¿©µµ ¾ø´Â ¼¼·Â ¸ó½ºÅÍ Ãß°¡

// 2015-07-21 by PanKJ use an inline function, because monBell is used twice in this macro
template<typename T> inline bool _is_teleport_monster(T monBell) { return monBell >= BELL_INFLUENCE_TELEPORT_VCN && monBell <= BELL_INFLUENCE_TELEPORT_ANI; }
// 2007-09-19 by cmkwon, ¼öÁ¤ÇÔ
// 2007-09-19 by dhjin, ÅÚ·¹Æ÷Æ® Ã¼Å©
#define IS_TELEPORT_MONSTER(monBell) _is_teleport_monster(monBell)

#define IS_BELL_VCN(monBell)					(0 == (monBell)%2)		// 2006-11-20 by cmkwon, 
#define IS_BELL_ANI(monBell)					(1 == (monBell)%2)		// 2006-11-20 by cmkwon, 
#define IS_SAME_CHARACTER_MONSTER_INFLUENCE(charInfl, monBell)	(  IS_INFLWAR_MONSTER(monBell) \
																	&& ( (COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN) && 0 == (monBell)%2) || (COMPARE_INFLUENCE((charInfl), INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_ANI) && 1 == (monBell)%2) )  )

// 2009-12-11 by cmkwon, µ¥¹ÌÁö ¾î±×·Î·Î Å¸°ÙÀ» º¯°æÇÏ´Â ¸ó½ºÅÍ ±¸Çö - ¼±°ø°Ý ¸ó½ºÅÍ ÀÎÁö ¿©ºÎ Ã¼Å©
#define IS_BELL_ATTACK(monBell)					((monBell) != BELL_NOTATTACK)

// 2007-10-01 by cmkwon, ÀüÁø±âÁöÀü °ü·Ã ¸ó½ºÅÍ Á¤ÀÇ
#define IS_OUTPOST_MONSTER(monBell)				( ((monBell) >= BELL_OUTPOST_PROTECTOR_VCN) && ((monBell) <= BELL_OUTPOST_RESET_ANI) )

// 2006-11-29 by cmkwon, INFLUENCE_TYPE_XXX¸¦ °¡Áö°í °°Àº ¼¼·Â¸ó½ºÅÍ Belligerence¸¦ ±¸ÇÑ´Ù
// 2006-11-29 by cmkwon, ¾Æ·¡ÀÇ Á¤ÀÇ´Â INFLUENCE_TYPE_NORMAL°ú IS_INFLWAR_MONSTER()´Â Ã¼Å©ÇÏÁö ¾Ê´Â´Ù, !!! »ç¿ëÇÏÁö Àü¿¡ ²À Ã¼Å©ÇØ¾ßÇÔ
#define GET_SAME_MONSTER_BELL_BY_CHARACTER_INFLTYPE(charInflTy)		( (INFLUENCE_TYPE_VCN==(charInflTy))?BELL_INFLUENCE_VCN:BELL_INFLUENCE_ANI )
#define GET_SAME_CHARACTER_INFL_BY_MONSTER_BELL(monBell)			( (IS_BELL_VCN(monBell))?INFLUENCE_TYPE_VCN:INFLUENCE_TYPE_ANI )
#define GET_OTHER_CHARACTER_INFL_BY_MONSTER_BELL(monBell)			( (IS_BELL_VCN(monBell))?INFLUENCE_TYPE_ANI:INFLUENCE_TYPE_VCN )				// 2009-01-12 by dhjin, ¼±Àü Æ÷°í
#define GET_OTHER_MONSTER_BELL_BY_CHARACTER_INFLTYPE(charInflTy)	( (INFLUENCE_TYPE_VCN==(charInflTy))?BELL_INFLUENCE_ANI:BELL_INFLUENCE_VCN )		// 2006-12-20 by cmkwon, Ä³¸¯ÅÍ¿Í Àû´ë¼¼·Â

// 2010-03-16 by cmkwon, ÀÎÇÇ2Â÷ MtoM, MtoC Å¸°Ù º¯°æ °ü·Ã ¼öÁ¤ - 
#define IS_WARABLE_M2M_BELL(AttMonBell, TarMonBell)		( (BELL_INFINITY_ATTACK_MONSTER == (AttMonBell) && BELL_INFINITY_DEFENSE_MONSTER == (TarMonBell)) || (BELL_INFINITY_DEFENSE_MONSTER == (AttMonBell) && BELL_INFINITY_ATTACK_MONSTER == (TarMonBell)) )

// 2006-12-13 by cmkwon
char *GetMonsterBellString(BYTE i_byMonBell);		

///////////////////////////////////
// Monster Attack Object(¸ó½ºÅÍÀÇ °ø°Ý Å¸°Ù ÇüÅÂ), ATTACKOBJ_XXX
#define ATTACKOBJ_CLOSERANGE			0	// °¡Àå ±Ù°Å¸® À¯´Ö°ø°Ý
#define ATTACKOBJ_FIRSTATTACK			1	// ¸ÕÀú °ø°ÝÇÑ À¯´Ö°ø°Ý
#define ATTACKOBJ_LOWHP					2	// HP°¡ °¡Àå ÀûÀº À¯´Ö°ø°Ý
#define ATTACKOBJ_HIGHHP				3	// HP°¡ °¡Àå ¸¹Àº À¯´Ö°ø°Ý
//#define ATTACKOBJ_PK					4	// PK ¼öÄ¡°¡ ³ôÀº »ç¶÷À» °ø°Ý - ÇöÀç´Â PK ½Ã½ºÅÛÀÌ ¾øÀ½
#define ATTACKOBJ_SAMERACE				5	// µ¿Àû¼ºÇâ, °ø°Ý´çÇÑ ¸ó½ºÅÍ ÁÖÀ§¿¡ Å¸ÀÔÀÌ °°Àº ¸ó½ºÅÍµµ °°ÀÌ °ø°ÝÇÑ´Ù
#define ATTACKOBJ_RANDOM				6	// ·£´ýÀ¸·Î ¼±ÅÃ
#define ATTACKOBJ_AGGRO					7	// 2009-12-11 by cmkwon, µ¥¹ÌÁö ¾î±×·Î·Î Å¸°ÙÀ» º¯°æÇÏ´Â ¸ó½ºÅÍ ±¸Çö - 


///////////////////////////////////
// Monster Move Pattern Option(¸ó½ºÅÍÀÇ ÀÌµ¿ ÆÐÅÏ ¿É¼Ç, ºñÆ® ÇÃ·¡±× »ç¿ë), MPOPTION_XXX,MPOPTION_BIT_XXX
// 2010-01-11 by cmkwon, ¸ó½ºÅÍ MPOption 64bit·Î º¯°æ - ±âÁ¸(8bit)
#define COMPARE_MPOPTION_BIT(VAR, MASK) (((VAR) & (MASK)) != 0)
#define MPOPTION_BIT_DEAD_FALL					((BitFlag64_t)0x0000000000000001)	// 1,	¸ó½ºÅÍ°¡ Æø¹ß½Ã Ãß¶ôÇÒ°ÍÀÎÁöÀÇ ÇÃ·¡±×(0ÀÌ¸é Æø¹ß)
#define MPOPTION_BIT_MOVE_PARTY					((BitFlag64_t)0x0000000000000002)	// 2,	MPOptionParam1 = Æí´ë ÇüÅÂ, MPOptionParam2 = ÃÖ´ëÆí´ë ¸¶¸®¼ö ==> µ¿Á·¼ºÇâµµ µ¿½Ã¿¡ °®´Â´Ù
#define MPOPTION_BIT_PATTERN_MONSTER			((BitFlag64_t)0x0000000000000004)	// 4,	¸ó½ºÅÍ°¡ »ý¼ºÈÄ MONSTER_EVENT_MON_NOT_MOVE_AFTER_CREATED_TERM_TICK ÀÌ½Ã°£ µ¿¾È ¿òÁ÷ÀÌÁö ¾Ê´Â´Ù, (ÆÐÅÏ°ú ¾Ö´Ï¸ÞÀÌ¼ÇÀÌ Àû¿ëµÇ´Â ½Ã°£)
#define MPOPTION_BIT_BOSS_MONSTER				((BitFlag64_t)0x0000000000000008)	// 8,	º¸½º ¸ó½ºÅÍ ÇÃ·¡±× - MoveRange¸¦ Å©°Ô, µå¶øSPI/µå¶ø¾ÆÀÌÅÛ/µå¶ø·¹¾î ·¹º§Â÷ ¼Õ½Ç ¾øÀ½
#define MPOPTION_BIT_AUTO_DESTROY				((BitFlag64_t)0x0000000000000010)	// 16,	»ý¼ºµÇ°í MPOptionParam1 ½Ã°£ÀÌ °æ°ú ÇÏ¸é ÀÚµ¿ ¼Ò¸êµÇ´Â ÇüÅÂÀÇ ¸ó½ºÅÍ(´ÜÀ§:ºÐ), ÃÖ¼Ò 1ºÐ ~ ÃÖ´ë 255ºÐ
#define MPOPTION_BIT_NAMED_MONSTER				((BitFlag64_t)0x0000000000000020)	// 32,	2005-12-16 by cmkwon, ³×ÀÓµå ¸ó½ºÅÍ - µå¶øSPI/µå¶ø¾ÆÀÌÅÛ/µå¶ø·¹¾î ·¹º§Â÷ ¼Õ½Ç ¾øÀ½
#define MPOPTION_BIT_RECOGNIZE_INVISIBLE		((BitFlag64_t)0x0000000000000040)	// 64,	2007-01-16 by cmkwon, ÀÎºñÁöºí ½ºÅ³ »óÅÂÀÇ Ä³¸¯ÅÍ¸¦ ÀÎ½ÄÇÑ´Ù
#define MPOPTION_BIT_KEY_MONSTER				((BitFlag64_t)0x0000000000000080)	// 128,	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ´ÙÀ½ ÀÌº¥Æ®°¡ ¹ß»ýÇÒ ¼ö ÀÖ´Â Key¸ó½ºÅÍ
#define MPOPTION_BIT_NO_ALPHABLENDING_IN_DEAD	((BitFlag64_t)0x0000000000000100)	// 256,	// 2010-01-11 by cmkwon, ¸ó½ºÅÍ ÆøÆÄ½Ã ¾ËÆÄºí·»µù(AlphaBlending) Ã³¸® ¾øÀ½ ÇÃ·¡±× - 
#define MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR	((BitFlag64_t)0x0000000000000200)	// 512,	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - ÆøÆÄ½Ã ÀÎÇÇ´ÏÆ¼ Á¾·á µÇ´Â Å°¸ó½ºÅÍ
#define MPOPTION_BIT_STOP_AUTO_CREATE			((BitFlag64_t)0x0000000000000400)	// 1024,	// 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ - ¸ó½ºÅÍ ÀÚµ¿ »ý¼º ±ÝÁö
#define MPOPTION_BIT_RECOGNIZE_STEALTH			((BitFlag64_t)0x0000000000000800)	// 2048,	// 2012-11-05 by jhseol, ½ºÅÚ½º »óÅÂÀÇ ÄÉ¸¯ÅÍ¸¦ ÀÎ½ÄÇÑ´Ù.
#define MPOPTION_BIT_ADD_DAMAGE_UNAPPLIED		((BitFlag64_t)0x0000000000001000)	// 4096,	// 2013-07-23 by jhseol, ¸ó½ºÅÍ Ãß°¡´ë¹ÌÁö ¹ÌÀû¿ë ¿É¼Ç.

///////////////////////////////////
// Monster Move Pattern Option - MPOPTION_MOVE_PARTY, Æí´ë ÇüÅÂ
#define FORMATION_COLUMN					0		// Á¾´ë
#define FORMATION_LINE						1		// È¾´ë
#define FORMATION_TRIANGLE					2		// »ï°¢
#define FORMATION_INVERTED_TRIANGLE			3		// ¿ª»ï°¢
#define FORMATION_BELL						4		// Á¾
#define FORMATION_INVERTED_BELL				5		// ¿ªÁ¾


///////////////////////////////////////////////////////////////////////////////
// ¸ó½ºÅÍ ½À¼º°ü·Ã
#define DES_MONHABIT_NOT					0	// (¹«¼Ó¼º)
#define DES_MONHABIT_DAY					1	// (³· ¸íÁß·ü 10% »ó½Â)
#define DES_MONHABIT_NIGHT					2	// (¹ã ¸íÁß·ü 10% »ó½Â)


///////////////////////////////////
// ¾ÆÀÌÅÛ Á¾·ù(±â°üÆ÷, ºö, ·ÎÄÏ.......), ITEMKIND_XXX
// Ä³¸¯ÅÍ¿ë ¾ÆÀÌÅÛ
//#define IS_CHARACTER_ITEM(x)			((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_SKILL_ATTRIBUTE  || ((x) >= ITEMKIND_GENERAL_ITEM_2ND_END && (x) <= ITEMKIND_GENERAL_ITEM_2ND_START))		// 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷, »ç¿ë ¾ÈÇÏ¸é Áö¿ìÁÒ. ¿ì¼± ÀÏ¹Ý ¾ÆÀÌÅÛ ±¸°£ Ãß°¡´Â ÇßÀ½
// ÀÏ¹Ý ¾ÆÀÌÅÛ
#define IS_GENERAL_ITEM(x)				(((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_GENERAL_ITEM_END) || ((x) >= ITEMKIND_GENERAL_ITEM_2ND_END && (x) <= ITEMKIND_GENERAL_ITEM_2ND_START))		// 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷, ÀÏ¹Ý ¾ÆÀÌÅÛ ±¸°£ Ãß°¡
// ¹«±â·ù
#define IS_WEAPON(x)					((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_DECOY)
// 1Çü ¹«±â
#define IS_PRIMARY_WEAPON(x)			((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_MASSDRIVE)
// 1-1Çü ¹«±â
#define IS_PRIMARY_WEAPON_1(x)			((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_MASSDRIVE)	// 1-1Çü ¹«±âÀÎ°¡?
#define ITEMKIND_AUTOMATIC				(BYTE)0		// ¿ÀÅä¸ÅÆ½·ù(1-1Çü)
#define ITEMKIND_VULCAN					(BYTE)1		// ¹ßÄ­·ù(1-1Çü)
#define ITEMKIND_DUALIST				(BYTE)2		// µà¾ó¸®½ºÆ®·ù(1-1Çü)	// 2005-08-01 by hblee : GRENADE -> DUALIST À¸·Î º¯°æ.
#define ITEMKIND_CANNON					(BYTE)3		// Ä³³í·ù(1-1Çü)
#define ITEMKIND_RIFLE					(BYTE)4		// ¶óÀÌÇÃ·ù(1-1Çü)
#define ITEMKIND_GATLING				(BYTE)5		// °³Æ²¸µ·ù(1-1Çü)
#define ITEMKIND_LAUNCHER				(BYTE)6		// ·±Ã³·ù(1-1Çü)
#define ITEMKIND_MASSDRIVE				(BYTE)7		// ¸Þ½ºµå¶óÀÌºê·ù(1-1Çü)
// 1-2Çü ¹«±â
//#define IS_PRIMARY_WEAPON_2(x)			((x) >= ITEMKIND_RIFLE && (x) <= ITEMKIND_MASSDRIVE)	// 1-2Çü(BEAM·ù) ¹«±âÀÎ°¡?
#define IS_PRIMARY_WEAPON_2(x)			(FALSE)		// 2005-09-27 by cmkwon, 1-2Çü ¹«±â´Â ¾ø´Ù
// 2005-09-27 by cmkwon, 1-1ÇüÀ¸·Î º¯°æÇÔ
//#define ITEMKIND_RIFLE					(BYTE)4		// ¶óÀÌÇÃ·ù(1-2Çü)
//#define ITEMKIND_GATLING				(BYTE)5		// °³Æ²¸µ·ù(1-2Çü)
//#define ITEMKIND_LAUNCHER				(BYTE)6		// ·±Ã³·ù(1-2Çü)
//#define ITEMKIND_MASSDRIVE				(BYTE)7		// ¸Þ½ºµå¶óÀÌºê·ù(1-2Çü)
// 2Çü ¹«±â
#define IS_SECONDARY_WEAPON(x)			((x) >= ITEMKIND_ROCKET && (x) <= ITEMKIND_DECOY)
// 2-1Çü ¹«±â
#define IS_SECONDARY_WEAPON_1(x)		((x) >= ITEMKIND_ROCKET && (x) <= ITEMKIND_MINE)
#define ITEMKIND_ROCKET					(BYTE)8		// ·ÎÄÏ·ù(2-1Çü)
#define ITEMKIND_MISSILE				(BYTE)9		// ¹Ì»çÀÏ·ù(2-1Çü)
#define ITEMKIND_BUNDLE					(BYTE)10	// ¹øµé·ù(2-1Çü)
#define ITEMKIND_MINE					(BYTE)11	// ¸¶ÀÎ·ù(2-1Çü)
// 2-2Çü ¹«±â
#define IS_SECONDARY_WEAPON_2(x)		((x) >= ITEMKIND_SHIELD && (x) <= ITEMKIND_DECOY)
#define ITEMKIND_SHIELD					(BYTE)12	// ½¯µå·ù(2-2Çü)
#define ITEMKIND_DUMMY					(BYTE)13	// ´õ¹Ì·ù(2-2Çü)
#define ITEMKIND_FIXER					(BYTE)14	// ÇÈ¼­·ù(2-2Çü)
#define ITEMKIND_DECOY					(BYTE)15	// µðÄÚÀÌ·ù(2-2Çü)
// ³ª¸ÓÁö ¾ÆÀÌÅÛ
#define ITEMKIND_DEFENSE				(BYTE)16	// ¾Æ¸Ó·ù
#define ITEMKIND_SUPPORT				(BYTE)17	// ±âÅ¸º¸Á¶Àåºñ·ù, ¿£Áø µî
#define ITEMKIND_ENERGY					(BYTE)18	// ¿¡³ÊÁö·ù, countable
#define ITEMKIND_INGOT					(BYTE)19	// 2008-11-24 by dhjin, ·°Å° ¾ÆÀÌÅÛ // ¿À¾î & º¸µå, countable
#define ITEMKIND_CARD					(BYTE)20	// ÀÏ¹Ý Ä«µå(Áö±Ý Á¸ÀçÇÏÁö ¾ÊÀ½, 20040716, kelovon, »ç¿ëÇÏ°Ô µÇ¸é »ì¸²)
#define ITEMKIND_ENCHANT				(BYTE)21	// ÀÎÃ¦Æ® Ä«µå
#define ITEMKIND_TANK					(BYTE)22	// ÅÊÅ©(EP ÅÊÅ© µî)
#define ITEMKIND_BULLET					(BYTE)23	// Åº¾Ë·ù(=ÅºÅë), countable
#define ITEMKIND_QUEST					(BYTE)24	// Äù½ºÆ®¿ë ¾ÆÀÌÅÛ, countable
#define ITEMKIND_RADAR					(BYTE)25	// ·¹ÀÌ´õ
#define ITEMKIND_COMPUTER				(BYTE)26	// ÄÄÇ»ÅÍ ¾ÆÀÌÅÛ
#define ITEMKIND_GAMBLE					(BYTE)27	// RareFix »ý¼º¿ë Ä«µå, ÀÎÃ¦Æ®¿Í °°Àº ¹æ¹ý »ç¿ë
#define ITEMKIND_PREVENTION_DELETE_ITEM	(BYTE)28	// ÀÎÃ¦Æ® ½Ã¿¡ ¾ÆÀÌÅÛ »èÁ¦ ¹æÁö ¾ÆÀÌÅÛ 
#define ITEMKIND_BLASTER				(BYTE)29	// 2005-08-01 by hblee : ºí·¡½ºÅÍ·ù ¾ÆÀÌÅÛ Ãß°¡.
#define ITEMKIND_RAILGUN				(BYTE)30	// 2005-08-01 by hblee : ·¹ÀÏ°Ç·ù ¾ÆÀÌÅÛ Ãß°¡.
#define ITEMKIND_ACCESSORY_UNLIMITED	(BYTE)31	// 2006-03-17 by cmkwon, »ç¿ë½Ã°£ÀÌ <¿µ¿ø>ÀÎ ¾×¼¼¼­¸® ¾ÆÀÌÅÛ
#define ITEMKIND_ACCESSORY_TIMELIMIT	(BYTE)32	// 2006-03-17 by cmkwon, »ç¿ë½Ã°£¿¡ ½Ã°£ Á¦ÇÑÀÌ ÀÖ´Â ¾×¼¼¼­¸® ¾ÆÀÌÅÛ
#define ITEMKIND_INFLUENCE_BUFF			(BYTE)33	// 2009-01-05 by dhjin, ¹Ì¼Ç¸¶½ºÅÍ - Æí´ë ¹öÇÁ ¾ÆÀÌÅÛ Ãß°¡ - ½ÇÁ¦·Î ITEMKIND_BUFF ¶ó´Â ÀÇ¹Ì·Î º¯°æ, // 2006-04-21 by cmkwon, ¼¼·Â ¹öÇÁ, ÇöÀç´Â ¼¼·Â¹öÇÁ ¾ÆÀÌÅÛ
#define ITEMKIND_INFLUENCE_GAMEEVENT	(BYTE)34	// 2006-04-21 by cmkwon, ¼¼·Â °ÔÀÓÀÌº¥Æ®
#define ITEMKIND_RANDOMBOX				(BYTE)35	// 2006-08-10 by cmkwon, ·£´ý ¹Ú½º - ÆÇµµ¶ó ¹Ú½º
#define ITEMKIND_MARK					(BYTE)36	// 2006-08-21 by cmkwon, ¸¶Å© - ÀÌÆåÆ® ¾ÆÀÌÅÛ
#define ITEMKIND_SKILL_SUPPORT_ITEM		(BYTE)37	// 2006-09-29 by cmkwon, º¸Á¶½ºÅ³¾ÆÀÌÅÛ
#define ITEMKIND_PET_ITEM				(BYTE)38	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê ¾ÆÀÌÅÛ.
#define ITEMKIND_GENERAL_ITEM_END		(BYTE)39	// General ItemÀÇ ³¡À» Ç¥½ÃÇÔ, ½ÇÀç·Î »ç¿ë ¾È µÊ

// DestParameter & ParamValue¿ë + ReqItemKind¿ë, ½ÇÁ¦ ¾ÆÀÌÅÛÀÇ Kind ÇÊµå¿¡´Â µé¾î°¡Áö ¾ÊÀ½, ÇöÀç enchant, skill¿¡¸¸ »ç¿ë, 20041019, kelovon with sjmin
#define ITEMKIND_ALL_ITEM				(BYTE)40	// ¸ðµç ¾ÆÀÌÅÛ, ReqItemKind È®ÀÎÇÒ ÇÊ¿ä ¾øÀ½
#define ITEMKIND_ALL_WEAPON				(BYTE)43	// ¸ðµç ¹«±â
#define ITEMKIND_PRIMARY_WEAPON_ALL		(BYTE)44	// 1Çü ¹«±â
#define ITEMKIND_PRIMARY_WEAPON_1		(BYTE)45	// 1-1Çü ¹«±â
#define ITEMKIND_PRIMARY_WEAPON_2		(BYTE)46	// 1-2Çü ¹«±â
#define ITEMKIND_SECONDARY_WEAPON_ALL	(BYTE)47	// 2Çü ¹«±â
#define ITEMKIND_SECONDARY_WEAPON_1		(BYTE)48	// 2-1Çü °ø°Ý ¹«±â
#define ITEMKIND_SECONDARY_WEAPON_2		(BYTE)49	// 2-2Çü ¹æ¾î ¹«±â
// ½ºÅ³·ù ¾ÆÀÌÅÛ
#define IS_SKILL_ITEM(x)				((x) >= ITEMKIND_SKILL_ATTACK && (x) <= ITEMKIND_SKILL_ATTRIBUTE)
#define ITEMKIND_SKILL_ATTACK			(BYTE)50	// °ø°Ý ½ºÅ³
#define ITEMKIND_SKILL_DEFENSE			(BYTE)51	// ¹æ¾î ½ºÅ³
#define ITEMKIND_SKILL_SUPPORT			(BYTE)52	// ÄÁÆ®·Ñ ½ºÅ³ + º¸Á¶ ½ºÅ³
#define ITEMKIND_SKILL_ATTRIBUTE		(BYTE)53	// ¼Ó¼º ½ºÅ³

#define ITEMKIND_COLOR_ITEM				(BYTE)60	// ¾Æ¸Ó ¾ÆÀÌÅÛÀÇ »ö»ó ¾ÆÀÌÅÛ, // 2005-12-06 by cmkwon

// ¸ó½ºÅÍ¿ë ¾ÆÀÌÅÛ
#define IS_MONSTER_ITEM(x)				((x) >= ITEMKIND_FOR_MON_PRIMARY && (x) <= ITEMKIND_FOR_MON_MAIN_ARMAMENT) // 2011-05-13 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷
#define IS_PRIMARY_WEAPON_MONSTER(x)	((x) >= ITEMKIND_FOR_MON_PRIMARY && (x) <= ITEMKIND_FOR_MON_RANGE_ATTACK)		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ ½ºÅ³ ¾ÆÀÌÅÛ Kind Ãß°¡
#define IS_SECONDARY_WEAPON_MONSTER(x)	((x) >= ITEMKIND_FOR_MON_SECONDARY && (x) <= ITEMKIND_FOR_MON_MAIN_ARMAMENT) // 2011-05-13 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷
#define ITEMKIND_FOR_MON_PRIMARY		(BYTE)100	// 1Çü ¸ó½ºÅÍ¿ë ¾ÆÀÌÅÛ
#define ITEMKIND_FOR_MON_GUN			(BYTE)101	// ¸ó½ºÅÍ ±â°üÆ÷·ù(1-1Çü)
#define ITEMKIND_FOR_MON_BEAM			(BYTE)102	// ¸ó½ºÅÍ ºö·ù(1-2Çü)
#define ITEMKIND_FOR_MON_ALLATTACK		(BYTE)103	// ¸ó½ºÅÍ ÀüÃ¼ °ø°Ý
#define ITEMKIND_FOR_MON_SKILL			(BYTE)104	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ ½ºÅ³ ¾ÆÀÌÅÛ Kind Ãß°¡
#define ITEMKIND_FOR_MON_RANGE_ATTACK	(BYTE)105	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ ¹üÀ§ °ø°Ý 
#define ITEMKIND_FOR_MON_SECONDARY		(BYTE)150	// 2Çü ¸ó½ºÅÍ¿ë ¾ÆÀÌÅÛ
#define ITEMKIND_FOR_MON_ROCKET			(BYTE)151	// ¸ó½ºÅÍ ·ÎÄÏ(2-1Çü)
#define ITEMKIND_FOR_MON_MISSILE		(BYTE)152	// ¸ó½ºÅÍ ¹Ì»çÀÏ·ù(2-1Çü)
#define ITEMKIND_FOR_MON_BUNDLE			(BYTE)153	// ¸ó½ºÅÍ ¹øµé·ù(2-1Çü)
#define ITEMKIND_FOR_MON_MINE			(BYTE)154	// ¸ó½ºÅÍ ¸¶ÀÎ·ù(2-1Çü)
#define ITEMKIND_FOR_MON_SHIELD			(BYTE)155	// ¸ó½ºÅÍ ½¯µå·ù(2-2Çü)
#define ITEMKIND_FOR_MON_DUMMY			(BYTE)156	// ¸ó½ºÅÍ ´õ¹Ì·ù(2-2Çü)
#define ITEMKIND_FOR_MON_FIXER			(BYTE)157	// ¸ó½ºÅÍ ÇÈ¼­·ù(2-2Çü)
#define ITEMKIND_FOR_MON_DECOY			(BYTE)158	// ¸ó½ºÅÍ µðÄÚÀÌ·ù(2-2Çü)
#define ITEMKIND_FOR_MON_FIRE			(BYTE)159	// ¸ó½ºÅÍ ÆÄÀÌ¾î·ù
#define ITEMKIND_FOR_MON_OBJBEAM		(BYTE)160	// ¸ó½ºÅÍ Ãæµ¹°¡´É ºö·ù e Á÷¼± ºö°ú Ãæµ¹ÇÒ °æ¿ì µ¥¹ÌÁö ¹ß»ý
#define ITEMKIND_FOR_MON_STRAIGHTBOOM	(BYTE)161	// ¸ó½ºÅÍ Á÷Áø ÆøÅº·ù e ÀÏÁ¤ÇÑ ¹æÇâ¼ºÀ» °¡Áö°í ¹«±â°¡ ¹ß»çµÇ¸ç ±× ¿ÀºêÁ§Æ®¿¡ ¸ÂÀ» °æ¿ì µ¥¹ÌÁö ¹ß»ý (ÆÐÅÏÀ¸·Î °¡´É ¿¹»ó, °ËÅä¹Ù¶÷)
#define ITEMKIND_FOR_MON_MAIN_ARMAMENT	(BYTE)162	// ¸ó½ºÅÍ ÁÖÆ÷ °ø°Ý·ù (ÀÎÇÇ´ÏÆ¼ 3Â÷ ¸ð¼±Àü) - // 2011-03-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡ - ¸ÞÀÎ ÁÖÆ÷·Î ¼³Á¤µÈ °æ¿ì ÀÏÁ¤ °Å¸®°¡ ¹þ¾î³ªµµ ¹ß»çµÇ´Â ÀÌÆåÆ®¸¦ º¼ ¼ö ÀÖÀ½

/////////////////////////////////////////////////
// start 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷
/////////////////////////////////////////////////
// ÀÏ¹Ý ¾ÆÀÌÅÛ·ù (IS_GENERAL_ITEM) 2Â÷
// 249 ºÎÅÍ ½ÃÀÛÇØ¼­ ÀÛÀº ¹øÈ£·Î ÇÒ´çÇÏ¼¼¿ä. ¿¹) 249 -> 248 -> 247 -> 246 -> 245 -> °è¼Ó
// ITEMKIND °¡ ¿©ºÐÀÌ ¾ó¸¶ ¾ø³×¿ä ÃÖ´ëÇÑ ¾Æ²¸¾²ÁÒ ¤Ð¤Ð
#define ITEMKIND_GENERAL_ITEM_2ND_END	ITEMKIND_PET_SOCKET_ITEM		// General Item 2Â÷ÀÇ ³¡À» Ç¥½ÃÇÔ

#define ITEMKIND_PET_SOCKET_ITEM		(BYTE)249	// ÆêÀÇ ÀåÂø ¼ÒÄÏ·ù ¾ÆÀÌÅÛ, // 2011-09-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - °ªÀÌ º¯°æµÉ½Ã¿¡´Â SQL ÀúÀå ÇÁ·Î½ÃÀú arena_CopyDBInfo ¹× arena_CopyDBInfo_Infinity °°ÀÌ ¼öÁ¤ÇÏ¼¼¿ä (Äõ¸®¿¡ 249 ¹øÈ£ ¹ÚÇô ÀÖÀ½)

#define ITEMKIND_GENERAL_ITEM_2ND_START	ITEMKIND_PET_SOCKET_ITEM		// General Item 2Â÷ÀÇ ½ÃÀÛÀ» Ç¥½ÃÇÔ
// end 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷
///////////////////////////////////////////////

#define ITEMKIND_UNKNOWN				(BYTE)255	// 2006-07-28 by cmkwon, 
#define ITEMKIND_ALL_ITEM_END			ITEMKIND_UNKNOWN	// Itemkind ÀÇ ³¡À» Ç¥½ÃÇÔ - // 2012-12-14 by hskim, ¸Þ¸ð¸®ÇÙ ÀÚµ¿ ºí·° ±â´É ±¸Çö
#define IS_VALID_ITEMKIND(x)			((x) >= ITEMKIND_AUTOMATIC && (x) <= ITEMKIND_ALL_ITEM_END)		// Itemkind °ª È®ÀÎ - // 2012-12-14 by hskim, ¸Þ¸ð¸®ÇÙ ÀÚµ¿ ºí·° ±â´É ±¸Çö
 
// °¢Á¾ ¸ÅÅ©·Î
	// IS_COUNTABLE_ITEM		==> °³¼ö¸¦ °è»êÇÏ´Â ¾ÆÀÌÅÛ·ùÀÎ°¡? ¿¡³ÊÁö·ù, INGOT, Åº¾Ë·ù µî
	// IS_BUNCH_COUNTABLE_ITEM	==> ¹­À½ °³¼ö¸¦ °è»êÇÏ´Â ¾ÆÀÌÅÛ·ùÀÎ°¡? Ä«µå·ù, ÀÎÃ¦Æ®·ù, °·ºí·ù
#define COUNT_BUNCH_COUNTABLE_ITEM_UNIT			10	// ¹­À½ °³¼ö ¾ÆÀÌÅÛ·ù ¹­À½ ´ÜÀ§

// 2008-11-26 by cmkwon, ´ë¸¸ Netpower_Tpe À¥¿¡¼­ ¾ÆÀÌÅÛ Ãß°¡ ÇÁ·Î½ÃÀú Ãß°¡ - 
//							- IS_COUNTABLE_ITEM ¼öÁ¤½Ã ¹Ýµå½Ã dbo.atum_IsCountableItem µµ °°ÀÌ ¼öÁ¤ ÇØ¾ß ÇÑ´Ù.
#define IS_COUNTABLE_ITEM(_ITEM_KIND)			((_ITEM_KIND) == ITEMKIND_ENERGY || (_ITEM_KIND) == ITEMKIND_INGOT || (_ITEM_KIND) == ITEMKIND_CARD || (_ITEM_KIND) == ITEMKIND_ENCHANT || (_ITEM_KIND) == ITEMKIND_BULLET || (_ITEM_KIND) == ITEMKIND_QUEST || (_ITEM_KIND) == ITEMKIND_GAMBLE || (_ITEM_KIND) == ITEMKIND_PREVENTION_DELETE_ITEM || (_ITEM_KIND) == ITEMKIND_INFLUENCE_BUFF || (_ITEM_KIND) == ITEMKIND_INFLUENCE_GAMEEVENT)
#define IS_SPECIAL_COUNTABLE_ITEM(_ITEM_KIND)	((_ITEM_KIND) == ITEMKIND_CARD || (_ITEM_KIND) == ITEMKIND_ENCHANT || (_ITEM_KIND) == ITEMKIND_GAMBLE || (_ITEM_KIND) == ITEMKIND_PREVENTION_DELETE_ITEM)

// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ¾Æ¸Ó(ITEMKIND_DEFENSE) Ãß°¡
#define COMPARE_ITEMKIND(_REQ_ITEM_KIND, _TARGET_KIND_VAR)											\
	((_REQ_ITEM_KIND == ITEMKIND_ALL_ITEM) || (_REQ_ITEM_KIND == _TARGET_KIND_VAR)				\
	|| ((_REQ_ITEM_KIND == ITEMKIND_ALL_WEAPON) && IS_WEAPON(_TARGET_KIND_VAR))						\
	|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_ALL) && IS_PRIMARY_WEAPON(_TARGET_KIND_VAR))		\
	|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_1) && IS_PRIMARY_WEAPON_1(_TARGET_KIND_VAR))		\
	|| ((_REQ_ITEM_KIND == ITEMKIND_PRIMARY_WEAPON_2) && IS_PRIMARY_WEAPON_2(_TARGET_KIND_VAR))		\
	|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_ALL) && IS_SECONDARY_WEAPON(_TARGET_KIND_VAR))	\
	|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_1) && IS_SECONDARY_WEAPON_1(_TARGET_KIND_VAR))	\
	|| ((_REQ_ITEM_KIND == ITEMKIND_DEFENSE) && (ITEMKIND_DEFENSE == _TARGET_KIND_VAR))	\
	|| ((_REQ_ITEM_KIND == ITEMKIND_SECONDARY_WEAPON_2) && IS_SECONDARY_WEAPON_2(_TARGET_KIND_VAR)))

// 2008-11-26 by cmkwon, ´ë¸¸ Netpower_Tpe À¥¿¡¼­ ¾ÆÀÌÅÛ Ãß°¡ ÇÁ·Î½ÃÀú Ãß°¡ - 
//							- IS_CHARGABLE_ITEM ¼öÁ¤½Ã ¹Ýµå½Ã dbo.atum_IsChargableItem µµ °°ÀÌ ¼öÁ¤ ÇØ¾ß ÇÑ´Ù.
//							- Ä«¿îÅÍºí ¾ÆÀÌÅÛÀ» Chargable Item ÀÌ µÉ¼ö ¾ø´Ù. Chargable ItemÀº ¹Ýµå½Ã ³ÍÄ«¿îÅÍºí ¾ÆÀÌÅÛ¸¸ °¡´ÉÇÏ´Ù.
#define IS_CHARGABLE_ITEM(_ITEM_KIND)	(IS_WEAPON(_ITEM_KIND)||(_ITEM_KIND)==ITEMKIND_TANK || (_ITEM_KIND)==ITEMKIND_ACCESSORY_UNLIMITED || (_ITEM_KIND)==ITEMKIND_ACCESSORY_TIMELIMIT)	// ITEM::ChargingÀ» ITEM_GENERAL::CurrentCount¿¡ ÇÒ´çÇØ¼­ ¾²´Â ¾ÆÀÌÅÛ

// 2007-08-29 by cmkwon, Á¶ÇÕ½Ã ¾Æ¸Óµµ ¹«±â¿Í °°ÀÌ ·¹¾î/ÀÎÃ¦Æ® Á¤º¸ À¯ÁöÇÏ±â - 
#define IS_ITEMKIND_REMAIN_RARE8ENCHANT_AS_MIX(_ITEM_KIND)	(IS_WEAPON(_ITEM_KIND) || ITEMKIND_DEFENSE==(_ITEM_KIND))

// 2009-02-17 by cmkwon, ÀÎÃ¦Æ® ÇÒ ¼ö ÀÖ´Â ¾ÆÀÌÅÛ Ä«ÀÎµå ¼öÁ¤ - ¹«±â, ¾Æ¸Ó, ·¹ÀÌ´õ, ¿£Áø
#define IS_ENCHANT_TARGET_ITEMKIND(_IT_)		(IS_WEAPON(_IT_) || ITEMKIND_DEFENSE == (_IT_) || ITEMKIND_RADAR == (_IT_) || ITEMKIND_SUPPORT == (_IT_) || ITEMKIND_ACCESSORY_UNLIMITED == (_IT_))	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ

// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ¾Æ¸Ó(ITEMKIND_DEFENSE)µµ Ãß°¡
#define IS_RARE_TARGET_ITEMKIND(_IT_)			(IS_WEAPON(_IT_) || ITEMKIND_DEFENSE == (_IT_))	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ

// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - 
//						- ¿ÜÇü º¯°æ °¡´É Kind ÃÑ 5Á¾, 1Çü/2Çü ¹«±â, ¸¶Å©, ¾Æ¸Ó, ·¹ÀÌ´õ, ÆÄÆ®³Ê
//						- ¾ÆÀÌÅÛÀÇ ÀÌÆåÆ® º¯°æ °¡´É Kind ÃÑ 2Á¾, 1Çü/2Çü ¹«±â
#define IS_ENABLE_CHANGE_ShapeItemNum(_ITEM_KIND)		(IS_WEAPON(_ITEM_KIND) || ITEMKIND_MARK == (_ITEM_KIND) || ITEMKIND_DEFENSE == (_ITEM_KIND) || ITEMKIND_RADAR == (_ITEM_KIND) || ITEMKIND_PET_ITEM == (_ITEM_KIND))		// 2012-12-03 by hskim, ÆÄÆ®³Ê ±â°£Á¦ ¿ÜÇü º¯°æ
#define IS_ENABLE_CHANGE_EffectItemNum(_ITEM_KIND)		(IS_WEAPON(_ITEM_KIND))


///////////////////////////////////////////////////////////////////////////////
// À¯·áÈ­ ¾ÆÀÌÅÛ kind - ITEM ±¸Á¶Ã¼¿¡¼­ SummonMonster ÇÊµå¸¦ »ç¿ëÇÑ´Ù
// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ¾Æ·¡ÀÇ ÄÃ·¯ ÅÇ¸¸ °íÁ¤ÇÏ°í ´Ù¸¥ ÅÇÀº ÀÇ¹Ì¸¦ µÎÁö ¾Ê´Â´Ù. ´ÜÁö ÅÇÀÇ ¼ø¼­¸¸À» ÀÇ¹ÌÇÏ°Ô ¼öÁ¤ÇÔ.
// #define CASH_ITEMKIND_PREMIUM_CARD			(BYTE)0
// #define CASH_ITEMKIND_OPTION_ITEM			(BYTE)1
// #define CASH_ITEMKIND_CARD_ITEM				(BYTE)2
// #define CASH_ITEMKIND_ACCESSORY_ITEM		(BYTE)3
// #define CASH_ITEMKIND_COLOR_ITEM			(BYTE)4			// 2007-08-09 by cmkwon, À¯·á»óÁ¡¿¡ ¾Æ¸Ó ÄÃ·¯ »óÁ¡ ÅÇ Ãß°¡
// #define CASH_ITEMKIND_PACKAGE_ITEM			(BYTE)5			// 2008-10-20 by cmkwon, À¯·á»óÁ¡¿¡ Ä³½¬ »óÁ¡ ÅÇ Ãß°¡

#define CASH_ITEMKIND_RECOMMEND_ITEM				((BYTE)0x00)	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - Ã¹¹øÂ° ÅÇÀº ÃßÃµÅÇ
#define CASH_ITEMKIND_COLOR_ITEM					((BYTE)0x09)	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ÄÃ·¯ÅÇÀº °íÁ¤ ÇØ¾ß ÇÏ¹Ç·Î ÀÏ´Ü Á¦ÀÏ µÚ·Î ÁöÁ¤ÇÔ.

#define CASHSHOP_BIT_TAPINDEX_MASK					((BYTE)0x0F)	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
#define CASHSHOP_BIT_RECOMMEND						((BYTE)0x10)	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
#define CASHSHOP_BIT_NEW							((BYTE)0x20)	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
#define GET_TABINDEX_BY_CASHSHOPINDEX(idx)				((idx)&CASHSHOP_BIT_TAPINDEX_MASK)				// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 0~15±îÁö°¡ ÅÇÀÎµ¦½ºÀÓ
#define GET_CASHSHOP_BIT_BY_CASHSHOPINDEX(idx)			((idx)&((BYTE)(~CASHSHOP_BIT_TAPINDEX_MASK)))	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 16~255±îÁö°¡ Ä³½¬¾ÆÀÌÅÛ ºñÆ®ÇÃ·¡±×
#define IS_NEW_CASHSHOPITEM_BY_CASHSHOPINDEX(idx)		(0 != ((idx)&CASHSHOP_BIT_NEW))				// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ½Å»óÇ° Ã¼Å©
#define IS_RECOMMEND_CASHSHOPITEM_BY_CASHSHOPINDEX(idx)	(0 != ((idx)&CASHSHOP_BIT_RECOMMEND))		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - ÃßÃµ»óÇ° Ã¼Å©


///////////////////////////////////////////////////////////////////////////////
// ITEM¿¡ OrbitType ÇÊµå¿¡ »ç¿ë define °ªÀ¸·Î ¹Ì»çÀÏ, ·ÎÄÏ µîÀÇ ±ËÀûÀ» ³ªÅ¸³½´Ù
#define ORBIT_SINGLE_CROSS_FIRE			1            // ÇÑ¹ß·Î ´ÙÀ½ ¸®¾îÅÃÅ¸ÀÓµ¿¾È ±ÕµîÇÏ°Ô ¾ù°¥·Á ¹ß»çÇÑ´Ù.
#define ORBIT_SINGLE_FIRE_100			32        // ÇÑ¹ß ¹ß»ç_100,150,200,250,300,320,350, ÇÑ¹ß¸¸ ¹ß»çµÇ¸ç ÀÏÁ¤ ½Ã°£ ÈÄ ´ÙÀ½ ¹ßÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_SINGLE_FIRE_150			33
#define ORBIT_SINGLE_FIRE_200			2	// ÇÑ¹ß ¹ß»ç_200,250,300,320,350, ÇÑ¹ß¸¸ ¹ß»çµÇ¸ç ÀÏÁ¤ ½Ã°£ ÈÄ ´ÙÀ½ ¹ßÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_SINGLE_FIRE_250			3
#define ORBIT_SINGLE_FIRE_300			4
#define ORBIT_SINGLE_FIRE_320			5
#define ORBIT_SINGLE_FIRE_350			6
#define ORBIT_TWIN_CROSS_FIRE			7	// ½Ö¹ß·Î ´ÙÀ½ ¸®¾îÅÃÅ¸ÀÓµ¿¾È ±ÕµîÇÏ°Ô ¾ù°¥·Á ¹ß»çÇÑ´Ù.
#define ORBIT_TWIN_FIRE_100				34        // ½Ö¹ß ¹ß»ç_100,150,200,250,300,320,350, ½Ö¹ß·Î ¹ß»çµÇ¸ç ÀÏÁ¤ ½Ã°£ ÈÄ ´ÙÀ½ ¹ßÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_TWIN_FIRE_150				35
#define ORBIT_TWIN_FIRE_200				8	// ½Ö¹ß ¹ß»ç_200,250,300,320,350, ½Ö¹ß·Î ¹ß»çµÇ¸ç ÀÏÁ¤ ½Ã°£ ÈÄ ´ÙÀ½ ¹ßÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_TWIN_FIRE_250				9
#define ORBIT_TWIN_FIRE_300				10
#define ORBIT_TWIN_FIRE_320				11
#define ORBIT_TWIN_FIRE_350				12
#define ORBIT_CROSS_FIRE				13	// ¾ù°¥·Á ¹ß»ç, ½Ö¹ß·Î ´ÙÀ½ ¸®¾îÅÃÅ¸ÀÓµ¿¾È ±ÕµîÇÏ°Ô ¾ù°¥·Á ¹ß»çÇÑ´Ù.
#define ORBIT_STRAIGHT_ROCKET_250		14	// Á÷¼± ·ÎÄÏ ¹ß»ç_250, Á÷¼±À¸·Î ¹ß»çµÈ´Ù.
#define ORBIT_FALL_ROCKET_250			15	// ÇÏ°­ ·ÎÄÏ ¹ß»ç_250, ÇÏ°­ ÈÄ Á÷¼±À¸·Î ·ÎÄÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_LEFT_ROCKET_200			16	// ¿ÞÂÊ Á÷¼± ·ÎÄÏ ¹ß»ç_200, ¿ÞÂÊºÎÅÍ Á÷¼± ·ÎÄÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_RIGHT_ROCKET_200			17	// ¿À¸¥ÂÊ Á÷¼± ·ÎÄÏ ¹ß»ç_200, ¿À¸¥ÂÊºÎÅÍ Á÷¼± ·ÎÄÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_THREE_ROCKET				18	// 3´Ü Á÷¼± ·ÎÄÏ ¹ß»ç, ·ÎÄÏÀÇ ¶óÀÌÇÁ Å¸ÀÓÀ» 3´Ü°è·Î ÇÏ¿© °¢°¢ Å¸ÀÓ¿¡¼­ ´ÜÀÌ ºÐ¸®µÇ¸é¼­ Å¸°ÙÂÊÀ¸·Î ¹æÇâÀ» ¿òÁ÷¿© ¹ß»çµÈ´Ù.
#define ORBIT_CHILD_ROCKET				19	// ÀÚÆøÅº ·ÎÄÏ ¹ß»ç, Å¸°ÙÀÇ ±ÙÃ³¿¡ ±îÁö ÀÌµ¿ÇÑ ÈÄ ÁÖº¯À¸·Î ÀÚÆøÅºÀÌ ºÐ¸®µÇ¾î ¹ß»çµÈ´Ù.
#define ORBIT_STRAIGHT_MISSILE_300		20	// Á÷¼± À¯µµ ¹Ì»çÀÏ ¹ß»ç_300, Á÷¼±À¸·Î Ãâ¹ßÇÏ¿© À¯µµµÈ´Ù.
#define ORBIT_FALL_MISSILE_300			21	// ÇÏ°­ À¯µµ ¹Ì»çÀÏ ¹ß»ç_300, ÇÏ°­ ÈÄ Á÷¼±À¸·Î À¯µµ ¹Ì»çÀÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_LEFT_MISSILE_300			22	// ¾ÈÂÊ À¯µµ ¹Ì»çÀÏ ¹ß»ç_300, ¾ÈÂÊºÎÅÍ Á÷¼±À¸·Î À¯µµ ¹Ì»çÀÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_RIGHT_MISSILE_270			23	// ¹Ù±ùÂÊ À¯µµ ¹Ì»çÀÏ ¹ß»ç_270, ¹Ù±ùÂÊºÎÅÍ Á÷¼±À¸·Î À¯µµ ¹Ì»çÀÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_UPDOWN_ROCKET_270			24	// »ó½Â °î¼±Çü ·ÎÄÏ ¹ß»ç_270, À§·Î »ó½Â ÈÄ Å¸°Ù±îÁö °î¼±ÇüÅÂ·Î ¶³¾îÁö´Â ·ÎÄÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_UPDOWN_MISSILE_300		25	// »ó½Â À¯µµÇü ¹Ì»çÀÏ ¹ß»ç_300, À§·Î »ó½Â ÈÄ Å¸°Ù±îÁö À¯µµÇü ¹Ì»çÀÏÀÌ ¹ß»çµÈ´Ù.
#define ORBIT_FLAME_THROWER				26	// ºÐ»çÇü ¹«±â ¹ß»ç, È­¿°ÀÌ³ª ³Ã±âµî ºÐ»ç°¡ °¡´ÉÇÑ ¹«±â°¡ ¹ß»çµÈ´Ù.
#define ORBIT_BODYSLAM					27	// ¸öÅë °ø°Ý ¹ß»ç, Á÷Á¢ Å¸°ÝÇÏ¿© °ø°ÝÇÑ´Ù.
#define ORBIT_MINE_300					28	// ¸¶ÀÎ ¹ß»ç_300,400, Æ¯Á¤ÇÑ Æ÷Æ®¸¦ °ø°£¿¡ ¹ß»çÇÑ´Ù.
#define ORBIT_MINE_400					29
#define ORBIT_RANGE_ROCKET_300			30	// ·ÎÄÏ ³­»ç_300,400, ÀûÀÇ ÁÖº¯À¸·Î ·ÎÄÏÀ» ¹ß»çÇÑ´Ù.
#define ORBIT_RANGE_ROCKET_400			31
#define ORBIT_FIRE						100	// ¸ó½ºÅÍ ÆÄÀÌ¾î·ù
#define ORBIT_OBJBEAM					101	// ¸ó½ºÅÍ Ãæµ¹°¡´É ºö·ù e Á÷¼± ºö°ú Ãæµ¹ÇÒ °æ¿ì µ¥¹ÌÁö ¹ß»ý
#define ORBIT_STRAIGHTBOOM				102	// ¸ó½ºÅÍ Á÷Áø ÆøÅº·ù e ÀÏÁ¤ÇÑ ¹æÇâ¼ºÀ» °¡Áö°í ¹«±â°¡ ¹ß»çµÇ¸ç ±× ¿ÀºêÁ§Æ®¿¡ ¸ÂÀ» °æ¿ì µ¥¹ÌÁö ¹ß»ý (ÆÐÅÏÀ¸·Î °¡´É ¿¹»ó, °ËÅä¹Ù¶÷)
#define ORBIT_CATCH						103	// ¸ó½ºÅÍ Àâ±â°ø°Ý·ù
#define ORBIT_STICK						104	// ¸ó½ºÅÍ ºÎÂø°ø°Ý·ù
#define ORBIT_ATTACK_ALL				105	// ¸ó½ºÅÍ ÀüÃ¼°ø°Ý·ù
#define ORBIT_SKILL_DEBUFF_TIMELIMIT	200	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ½Ã°£Çü DEBUFF
#define ORBIT_SKILL_DOT_TIMELIMIT		210	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ½Ã°£Çü DOT
#define IS_ORBIT_SKILL(x)				((x) >= ORBIT_SKILL_DEBUFF_TIMELIMIT && (x) <= ORBIT_SKILL_DOT_TIMELIMIT)		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 

///////////////////////////////////
// °Ç¹°(»óÁ¡) Á¾·ù, BUILDINGKIND_XXX
#define BUILDINGKIND_1ST_WEAPONSHOP		(BYTE)0		// ±âº»¹«±âÆÄÆ®
#define BUILDINGKIND_2ND_WEAPONSHOP		(BYTE)1		// °í±Þ¹«±âÆÄÆ®
#define BUILDINGKIND_COMPONENTSHOP		(BYTE)2		// ¼Ò¸ðÇ°ÆÄÆ®
#define BUILDINGKIND_PETSHOP			(BYTE)3		// ÆêÆÄÆ®
#define BUILDINGKIND_ANTIQUESHOP		(BYTE)4		// °ñµ¿Ç°ÆÄÆ®
#define BUILDINGKIND_CITYWARP			(BYTE)5		// ¿öÇÁ»óÁ¡(±¸ µµ½Ã¿öÇÁ, ±¸ BUILDINGKIND_HANGAR)
#define BUILDINGKIND_PUBLICOFFICE		(BYTE)6		// °üÁ¦ÆÄÆ®
#define BUILDINGKIND_ACTIONHOUSE		(BYTE)7		// °æ¸ÅÆÄÆ®
#define BUILDINGKIND_RACETRACK			(BYTE)8		// ·¹ÀÌ½ÌÆÄÆ®
#define BUILDINGKIND_REFINERY			(BYTE)9		// ¼ö¸®ÆÄÆ® - º¸±Þ »óÁ¡
#define BUILDINGKIND_LABORATORY			(BYTE)10	// ¿¬±¸ÆÄÆ®
#define BUILDINGKIND_FACTORY			(BYTE)11	// °øÀåÆÄÆ®, ±¸ BUILDINGKIND_ROBOTHANGAR
#define BUILDINGKIND_MILITARYACADEMY	(BYTE)12	// »ç°üÇÐ±³ÆÄÆ®
#define BUILDINGKIND_STORE				(BYTE)13	// Ã¢°í
#define BUILDINGKIND_SKILL_SHOP			(BYTE)14	// ½ºÅ³ »óÁ¡
#define BUILDINGKIND_AUCTION			(BYTE)15	// °æ¸Å, ¹°Ç° ´ëÇà »óÁ¡
#define BUILDINGKIND_CITY_OCCUPY_INFO	(BYTE)16	// µµ½Ã Á¡·É Á¤º¸ »óÁ¡
#define BUILDINGKIND_TUNING_COLOR		(BYTE)17	// 2005-11-11 by cmkwon, Æ©´× »ö»ó »óÁ¡
#define BUILDINGKIND_ARENA				(BYTE)18	// 2007-04-23 by dhjin, ARENA
#define BUILDINGKIND_WARPOINT_SHOP		(BYTE)19	// 2007-05-17 by dhjin, WARPOINT SHOP
#define BUILDINGKIND_BRIEFING_ROOM		(BYTE)20	// 2007-07-16 by dhjin, BRIEFING_ROOM
#define BUILDINGKIND_TUTORIAL			(BYTE)21	// 2007-07-16 by dhjin, BUILDINGKIND_TUTORIAL
#define BUILDINGKIND_CITYLEADER_LEADER  (BYTE)22	// 2007-08-27 by dhjin, BUILDINGKIND_CITYLEADER_LEADER
#define BUILDINGKIND_CITYLEADER_OUTPOST (BYTE)23	// 2007-08-27 by dhjin, BUILDINGKIND_CITYLEADER_OUTPOST
#define BUILDINGKIND_LUCKY				(BYTE)24	// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ¾Æ¸Ó/¹«±â/ÇØÇÇ¸¸ »ç¿ë, ¿É¼Ç¸Ó½ÅÀº ¾Æ·¡¿¡ µû·Î Ãß°¡// 2008-11-04 by dhjin, ·°Å°¸Ó½Å, BUILDINGKIND_LUCKY
#define BUILDINGKIND_WORLDRANKING		(BYTE)25	// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
#define BUILDINGKIND_INFINITY			(BYTE)30	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
#define BUILDINGKIND_INFINITY_SHOP		(BYTE)31	// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - »óÁ¡
#define BUILDINGKIND_LUCKY_OPTION_MACHINE	(BYTE)32	// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ¿É¼Ç¸Ó½ÅÀ» À§ÇÑ Ãß°¡
#define BUILDINGKIND_DISSOLUTION		(BYTE)33	// 2010-08-31 by shcho&jskim ¾ÆÀÌÅÛ¿ëÇØ ½Ã½ºÅÛ -
#define BUILDINGKIND_TRIGGER_CRYSTAL	(BYTE)34	// 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
#define BUILDINGKIND_PARTSSHOP			(BYTE)35	// 2012-02-29 by mspark, »óÁ¡ Å¸ÀÌÆ² ³Ö±â - ÆÄÃ÷»óÁ¡
#define BUILDINGKIND_GEARSHOP			(BYTE)36	// 2012-02-29 by mspark, »óÁ¡ Å¸ÀÌÆ² ³Ö±â - ±â¾î»óÁ¡
#define BUILDINGKIND_CARDSHOP			(BYTE)37	// 2012-02-29 by mspark, »óÁ¡ Å¸ÀÌÆ² ³Ö±â - Ä«µå»óÁ¡
#define BUILDINGKIND_MYSTERY_SHOP		(BYTE)38	// 2012-07-04 by JHAHN, ¹Ì½ºÅÍ¸® ¸Ó½Å ±¸Çö
#define BUILDINGKIND_INFLBUFF_SHOP		(BYTE)39	// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±
#define BUILDINGKIND_CASH_SHOP			(BYTE)100	// À¯·áÈ­ »óÇ° »óÁ¡




///////////////////////////////////
// °Ç¹° ¹× ´ã´ç NPC Á¤º¸
typedef struct _BUILDINGNPC {
	UINT		BuildingIndex;							// °Ç¹°(°¡°Ô µî) °íÀ¯ ¹øÈ£
	char		BuildingName[SIZE_MAX_BUILDING_NAME];
	BYTE		BuildingKind;							// °Ç¹° Á¾·ù, BUILDINGKIND_XXX
	MapIndex_t	MapIndex;								// ¼ÓÇØ ÀÖ´Â ¸Ê Index
	UINT		NPCIndex;								// ´ã´ç NPC
	char		NPCName[SIZE_MAX_NPC_NAME];
	char		GreetingTalk[SIZE_MAX_BUILDING_NPC_TALK_STRING];	// ÀÎ»ç¸»
	MapIndex_t	OwnerCityMapIndex;						// ÀÌ »óÁ¡À» ¼ÒÀ¯ÇÑ µµ½Ã¸ÊÀÇ ¸ÊÀÎµ¦½º
	INT			OwnerOrderNumber;						// µµ½Ã¸ÊÀÌ ¼ÒÀ¯°¡´ÉÇÑ »óÁ¡µéÀÇ ¼ÒÀ¯ ¼ø¼­
} BUILDINGNPC;	// °Ç¹° ¹× ´ã´ç NPC Á¤º¸

//////////////////////////////////////////////////////////////////////
// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê Å¸ÀÔ °ª
//2011-10-06 by jhahn ÆÄÆ®³Ê ¼ºÀåÇü ½Ã½ºÅÛ
enum PARTNER_TYPE 
{	// ÆÄÆ®³Ê(ÆÖ) Å¸ÀÔ ¿­°ÅÀÚ.

	PARTNER_TYPE_START = 0,			// ÀÏ¹ÝÇü.

	PARTNER_TYPE_NORMAL = 1,		// °ø°ÝÇü.
	PARTNER_TYPE_GROWTH,			// ¼ºÀåÇü.
	

	MAX_PARTNER_TYPE

};
//end 2011-10-06 by jhahn ÆÄÆ®³Ê ¼ºÀåÇü ½Ã½ºÅÛ

///////////////////////////////////
// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ÀåÂø ½½·Ô °ªµé Enum°ªÀ¸·Î º¯°æ.
// 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ¼û°ÜÁø ¾ÆÀÌÅÛ (ÀÎº¥Åä¸®¿¡ º¸¿©ÁöÁö ¾ÊÀ¸¸ç, ÀÎº¥Åä¸® °ø°£ °è»ê¿¡µµ Æ÷ÇÔµÇÁö ¾Ê´Â´Ù) - ÆÄÆ®³Ê ½Ã½ºÅÛ »ç¿ëµÈ ¼ÒÄÏ ¾ÆÀÌÅÛ ¿ë 
// ¾ÆÀÌÅÛ ÀåÂø À§Ä¡(POS_XXX)
enum EQUIP_POS
{
	POS_NONE = -1 ,

	POS_PROW = 0 , 
	POS_PROWIN ,
	POS_PROWOUT , 
	POS_WINGIN , 
	POS_WINGOUT , 

	POS_CENTER , 
	POS_REAR , 
	POS_ACCESSORY_UNLIMITED , 
	POS_ACCESSORY_TIME_LIMIT , 
	POS_PET , 

	MAX_EQUIP_POS , 

	POS_HIDDEN_ITEM = 98,			// 2011-09-20 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ¼û°ÜÁø ¾ÆÀÌÅÛ (ÀÎº¥Åä¸®¿¡ º¸¿©ÁöÁö ¾ÊÀ¸¸ç, ÀÎº¥Åä¸® °ø°£ °è»ê¿¡µµ Æ÷ÇÔµÇÁö ¾Ê´Â´Ù) - ÆÄÆ®³Ê ½Ã½ºÅÛ »ç¿ëµÈ ¼ÒÄÏ ¾ÆÀÌÅÛ ¿ë, ÃßÈÄ ´Ù¸¥ ¾ÆÀÌÅÛÀ¸·Îµµ »ç¿ëÇÒ °æ¿ì ¸µÅ©½ÃÅ² ¾ÆÀÌÅÛÀÌ »èÁ¦µÉ¶§ °°ÀÌ »èÁ¦µÇµµ·Ï Ã³¸® ÇÒ°Í
	POS_INVALID_POSITION = 99

};

/*
// ¾ÆÀÌÅÛ ÀåÂø À§Ä¡(POS_XXX)
#define POS_PROW						((BYTE)0)	// ·¹ÀÌ´õ(¼±µÎ °¡¿îµ¥)	--> ÁÂÃø 2¹ø
#define POS_PROWIN						((BYTE)1)	// CPU ÄÄÇ»ÅÍ(Áß¾Ó ÁÂÃø)	--> ¿ìÃø 2¹ø
#define POS_PROWOUT						((BYTE)2)	// 1Çü ¹«±â(¼±µÎ ÁÂÃø)	--> ÁÂÃø 1¹ø
#define POS_WINGIN						((BYTE)3)	// 2006-07-20 by cmkwon, ÀÌÆåÆ® ¾ÆÀÌÅÛ (Áß¾Ó À§ÂÊ), ¸¶Å© ¾ÆÀÌÅÛ
#define POS_WINGOUT						((BYTE)4)	// 2Çü ¹«±â(¼±µÎ ¿ìÃø)	--> ¿ìÃø 1¹ø
#define POS_CENTER						((BYTE)5)	// ¾Æ¸Ó(Áß¾Ó °¡¿îµ¥)	--> ÁÂÃø 4¹ø
#define POS_REAR						((BYTE)6)	// ¿£Áø(ÈÄ¹Ì °¡¿îµ¥)	--> ¿ìÃø 3¹ø

// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¼±¾ð º¯°æ.
//#define POS_ATTACHMENT					((BYTE)7)	// 2006-03-30 by cmkwon, ¹«Á¦ÇÑ ¾Ç¼¼»ç¸® - ºÎÂø¹°(ÈÄ¹Ì ¿ìÃø-¿¬·áÅÊÅ©|ÄÁÅ×ÀÌ³Ê°è¿­), ÁöµµÀÚ ±¤ÈÖ --> ÁÂÃø 3¹ø
#define POS_ACCESSORY_UNLIMITED			((BYTE)7)	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¹«Á¦ÇÑ ¾Ç¼¼»ç¸® 

//#define POS_PET							((BYTE)8)	// 2006-03-30 by cmkwon, ½Ã°£Á¦ÇÑ ¾Ç¼¼»ç¸®(ÈÄ¹Ì ÁÂÃø)	--> ¿ìÃø 4¹ø
#define POS_ACCESSORY_TIME_LIMIT		((BYTE)8)	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ½Ã°£Á¦ÇÑ ¾Ç¼¼»ç¸®
// End 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¼±¾ð º¯°æ.


// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¼±¾ð º¯°æ.
#define POS_PET							((BYTE)9)	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê ¾ÆÀÌÅÛ.

#define POS_INVALID_POSITION			((BYTE)99)	// 99,  invalid position
*/
// End 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ÀåÂø ½½·Ô °ªµé Enum°ªÀ¸·Î º¯°æ.

// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡
#define RETURN_USER_NOT_USING_ITEM		0		// ¾ÆÀÌÅÛ ¹Ì»ç¿ëÁß( º¹±ÍÀ¯Á®¾Æ´Ô )
#define RETURN_USER_USING_ITEM0			1		// ¾ÆÀÌÅÛ   »ç¿ëÁß( º¹±ÍÀ¯Á®)
#define RETURN_USER_PARTY_BUFFSKILL_NUM		7841131	// º¹±ÍÀ¯Á®°¡ ¾Æ´Ñ Æí´ë¿ø ÀÌ Àû¿ë¹ÞÀ» ¹öÇÁ½ºÅ³ ³Ñ¹ö
// End. 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡


#define POS_ITEMWINDOW_OFFSET			((BYTE)100)	// 100, ÀÌ ¹øÈ£ºÎÅÍ ¾ÆÀÌÅÛÃ¢¿¡ Á¸Àç

// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¾ÆÀÌÅÆ °³¼ö º¯°æ. (Æê) Ãß°¡. °è»ê. 9 -> MAX_EQUIP_POS
#define SIZE_MAX_POS					MAX_EQUIP_POS
//#define SIZE_MAX_POS					10			// ÀåÂø ¾ÆÀÌÅÛ slotÀÇ °³¼ö
//#define SIZE_MAX_POS					9			// ÀåÂø ¾ÆÀÌÅÛ slotÀÇ °³¼ö

#define IS_ATTACHABLE_POSITION(pos)		(SIZE_MAX_POS > (pos))							// 2009-10-22 by cmkwon, ½Ã°£ Á¦ÇÑÇü·ù ¾ÆÀÌÅÛ ½Ã½ºÅÛ Á¤¸® - ÀåÂø °¡´É ¾ÆÀÌÅÛ Ã¼Å©
#define IS_ATTACHABLE_ITEM(pItem)		(FALSE == IS_COUNTABLE_ITEM((pItem)->Kind) && IS_ATTACHABLE_POSITION((pItem)->Position))		// 2009-10-22 by cmkwon, ½Ã°£ Á¦ÇÑÇü·ù ¾ÆÀÌÅÛ ½Ã½ºÅÛ Á¤¸® - ÀåÂø °¡´É ¾ÆÀÌÅÛ Ã¼Å©

///////////////////////////////////
// WEAR - WEAR_XXX
#define WEAR_NOT_ATTACHED				0	// ¹ÌÀåÂø
#define WEAR_ATTACHED					1	// ÀåÂø

///////////////////////////////////////////////////////////////////////////////
// 2005-12-07 by cmkwon - ITEM_IN_XXX
#define ITEM_IN_CHARACTER				0	// Ä³¸¯ÅÍÀÎº¥, ¼±ÅÃÇÑ Ä³¸¯ÅÍ °ÔÀÓ ½ÃÀÛ½Ã ÇÑ¹ø¸¸ È£ÃâµÊ
#define ITEM_IN_STORE					1	// Ã¢°í
#define ITEM_IN_GUILD_STORE             2   // 2006-09-14 by dhjin, ¿©´Ü Ã¢°í 

///////////////////////////////////
// ¾ÆÀÌÅÛÀÇ Ä³¸¯ÅÍ ÆÄ¶óÀÌÅÍ - DES_XXX
#define DES_NULL							0		// ´ë»ó ÆÄ¶ó¹ÌÅÍ°¡ ¾ø´Â °æ¿ì »ç¿ë
#define DES_ATTACK_PART						1		// °ø°Ý ÆÄÆ®
#define DES_DEFENSE_PART					2		// ¹æ¾î ÆÄÆ®
#define DES_FUEL_PART						3		// ¿¬·á ÆÄÆ®
#define DES_SOUL_PART						4		// °¨ÀÀ ÆÄÆ®
#define DES_SHIELD_PART						5		// ½¯µå ÆÄÆ®
#define DES_DODGE_PART						6		// È¸ÇÇ ÆÄÆ®
#define DES_ALL_PART						106		// ¸ðµç ½ºÅÝ	// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ¸ðµç ½ºÅÝ DES_ALL_PART Ãß°¡
#define DES_BODYCONDITION					7		// ¸ö»óÅÂ
#define DES_ENDURANCE_01					8		// ³»±¸µµ 01
#define DES_ENDURANCE_02					9		// ³»±¸µµ 02
#define DES_CHARGING_01						10		// ÀåÅº¼ö 01
#define DES_CHARGING_02						11		// ÀåÅº¼ö 02
#define DES_PROPENSITY						12		// ¼ºÇâ
#define DES_HP								13		// È÷Æ®Æ÷ÀÎÆ®, MAX HP¸¦ +VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_DP								89		// ½¯µå(DEFENSE)Æ÷ÀÎÆ®
#define DES_SP								14		// ¼Ò¿ïÆ÷ÀÎÆ®
#define DES_EP								15		// ¿£ÁøÆ÷ÀÎÆ®
#define DES_SPRECOVERY						16		// ¼Ò¿ïÆ÷ÀÎÆ®È¸º¹·Â
#define DES_HPRECOVERY						17		// ¿¡³ÊÁöÆ÷ÀÎÆ®È¸º¹·Â
#define DES_MINATTACK_01					18		// (*) ÃÖ¼Ò °ø°Ý·Â 01
#define DES_MAXATTACK_01					71		// (*) ÃÖ´ë °ø°Ý·Â 01
#define DES_MINATTACK_02					19		// (*) ÃÖ¼Ò °ø°Ý·Â 02
#define DES_MAXATTACK_02					72		// (*) ÃÖ´ë °ø°Ý·Â 02
#define DES_ATTACKPROBABILITY_01			20		// °ø°ÝÈ®·ü 01
#define DES_ATTACKPROBABILITY_02			21		// °ø°ÝÈ®·ü 02
#define DES_DEFENSE_01						22		// (*) ¹æ¾î·Â 01 (µ¥¹ÌÁö °¨¼Ò È®·ü) 20040622 Ãß°¡
#define DES_DEFENSE_02						23		// (*) ¹æ¾î·Â 02 (µ¥¹ÌÁö °¨¼Ò È®·ü) 20040622 Ãß°¡
#define DES_DEFENSEPROBABILITY_01			24		// ¹æ¾îÈ®·ü 01 (È¸ÇÇ È®·ü)
#define DES_DEFENSEPROBABILITY_02			25		// ¹æ¾îÈ®·ü 02 (È¸ÇÇ È®·ü)
#define DES_SKILLPROBABILITY_01				26		// ½ºÅ³°ø°ÝÈ®·ü 01
#define DES_SKILLPROBABILITY_02				64		// ½ºÅ³°ø°ÝÈ®·ü 02
#define DES_FACTION_01						79		// ¼Ó¼º 01, check: Ãß°¡µÊ
#define DES_FACTION_02						80		// ¼Ó¼º 02, check: Ãß°¡µÊ
#define DES_FACTIONRESISTANCE_01			27		// ¼Ó¼ºÀúÇ×·Â 01
#define DES_FACTIONRESISTANCE_02			65		// ¼Ó¼ºÀúÇ×·Â 02
#define DES_SPEED							28		// (*) ÀÌµ¿¼Óµµ, FIXER µîÀÇ ¼Óµµ °¨¼Ò·®
#define DES_TRANSPORT						29		// ¿î¹Ý·Â
#define DES_MATERIAL						30		// ÀçÁú
#define DES_REATTACKTIME_01					31		// (*) ¸®¾îÅÃÅ¸ÀÓ 01 (- Áõ°¡)
#define DES_REATTACKTIME_02					32		// (*) ¸®¾îÅÃÅ¸ÀÓ 02 (- Áõ°¡)
#define DES_ABRASIONRATE_01					33		// ¸¶¸ðÀ² 01
#define DES_ABRASIONRATE_02					34		// ¸¶¸ðÀ² 02
#define DES_RANGE_01						35		// (*) À¯È¿°Å¸® 01
#define DES_RANGE_02						36		// (*) À¯È¿°Å¸® 02
#define DES_RANGEANGLE_01					37		// À¯È¿°¢µµ 01
#define DES_RANGEANGLE_02					38		// À¯È¿°¢µµ 02
#define DES_MULTITAGET_01					39		// ¸ÖÆ¼Å¸°Ù 01
#define DES_MULTITAGET_02					66		// ¸ÖÆ¼Å¸°Ù 02
#define DES_EXPLOSIONRANGE_01				40		// Æø¹ß¹Ý°æ 01
#define DES_EXPLOSIONRANGE_02				67		// Æø¹ß¹Ý°æ 02
#define DES_UNIT							41		// À¯´ÖÀÇ Á¾·ù (28 ~ 29ÀÌ °°ÀÌ ¾²¿© À¯´Ö¸¶´ÙÀÇ º¸Á¤°ªÀ¸·Î »ç¿ëµÊ)
#define DES_REVISION						42		// À¯´ÖÀÇ º¸Á¤°ª (28 ~ 29ÀÌ °°ÀÌ ¾²¿© À¯´Ö¸¶´ÙÀÇ º¸Á¤°ªÀ¸·Î »ç¿ëµÊ)
#define DES_FACTIONPROBABILITY_01			43		// ¼Ó¼º¿¡ ´ëÇÑ ¹æ¾îÈ®·ü 01
#define DES_FACTIONPROBABILITY_02			68		// ¼Ó¼º¿¡ ´ëÇÑ ¹æ¾îÈ®·ü 02
#define DES_SHOTNUM_01						44		// ÀÏÁ¡»ç ´ç ¹ß»ç ¼ö 01
#define DES_SHOTNUM_02						69		// ÀÏÁ¡»ç ´ç ¹ß»ç ¼ö 02
#define DES_MULTINUM_01						45		// µ¿½Ã ¹ß»ç ¼ö 01
#define DES_MULTINUM_02						70		// µ¿½Ã ¹ß»ç ¼ö 02
#define DES_ATTACKTIME_01					46		// Ã³À½ °ø°Ý ½ÃÀÇ Å¸ÀÓ 01 (- Áõ°¡)
#define DES_ATTACKTIME_02					47		// Ã³À½ °ø°Ý ½ÃÀÇ Å¸ÀÓ 02 (- Áõ°¡)
#define DES_TIME_01							48		// (*) Áö¼Ó ½Ã°£ 01, check: + -> *
#define DES_TIME_02							49		// (*) Áö¼Ó ½Ã°£ 02, check: + -> *
#define DES_OVERHITTIME_01					73		// (*) ¿À¹öÈýÈ¸º¹½Ã°£ 01, check: Ãß°¡µÊ
#define DES_OVERHITTIME_02					74		// (*) ¿À¹öÈýÈ¸º¹½Ã°£ 02, check: Ãß°¡µÊ
#define DES_UNITKIND						50		// ±âÃ¼ ¾÷±×·¹ÀÌµå½Ã ÇØ´ç ±âÃ¼
#define DES_ITEMKIND						51		// ¾ÆÀÌÅÛÀÇ Á¾·ù(ITEMKIND_XXX)
#define DES_SUMMON							52		// ¸ó½ºÅÍ ¼ÒÈ¯
#define DES_GRADUAL_HP_UP					53		// ¿¡³ÊÁö·ù, ÇöÀç HP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)µ¿¾È Á¡ÁøÀûÀ¸·Î VALUE¸¸Å­ ¿Ã·ÁÁÜ, Áß°£¿¡ ºÎ½ºÅÍ ÄÑ°Å³ª °ø°Ý ¹ÞÀ¸¸é cancel
#define DES_GRADUAL_DP_UP					81		// ¿¡³ÊÁö·ù, ÇöÀç DP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)µ¿¾È Á¡ÁøÀûÀ¸·Î VALUE¸¸Å­ ¿Ã·ÁÁÜ, Áß°£¿¡ ºÎ½ºÅÍ ÄÑ°Å³ª °ø°Ý ¹ÞÀ¸¸é cancel, check: Ãß°¡µÊ
#define DES_GRADUAL_SP_UP					54		// ¿¡³ÊÁö·ù, ÇöÀç SP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)µ¿¾È Á¡ÁøÀûÀ¸·Î VALUE¸¸Å­ ¿Ã·ÁÁÜ, Áß°£¿¡ ¹¹(?)µé¾î¿À¸é cancel
#define DES_GRADUAL_EP_UP					55		// ¿¡³ÊÁö·ù, ÇöÀç EP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)µ¿¾È Á¡ÁøÀûÀ¸·Î VALUE¸¸Å­ ¿Ã·ÁÁÜ, Áß°£¿¡ ¹¹(?)µé¾î¿À¸é cancel
#define DES_IN_TIME_HP_UP					56		// ¿¡³ÊÁö·ù, ÇöÀç HP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)ÀÌ Áö³­ ÈÄ VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_IN_TIME_DP_UP					82		// ¿¡³ÊÁö·ù, ÇöÀç DP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)ÀÌ Áö³­ ÈÄ VALUE¸¸Å­ ¿Ã·ÁÁÜ, check: Ãß°¡µÊ
#define DES_IN_TIME_SP_UP					57		// ¿¡³ÊÁö·ù, ÇöÀç SP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)ÀÌ Áö³­ ÈÄ VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_IN_TIME_EP_UP					58		// ¿¡³ÊÁö·ù, ÇöÀç EP¸¦ ÀÏÁ¤ ½Ã°£(ITEM.Time¿¡ ÀúÀå)ÀÌ Áö³­ ÈÄ VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_IMMEDIATE_HP_UP					59		// ¿¡³ÊÁö·ù, ÇöÀç HP¸¦ Áï½Ã VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_IMMEDIATE_DP_UP					83		// ¿¡³ÊÁö·ù, ÇöÀç DP¸¦ Áï½Ã VALUE¸¸Å­ ¿Ã·ÁÁÜ, check: Ãß°¡µÊ
#define DES_IMMEDIATE_SP_UP					60		// ¿¡³ÊÁö·ù, ÇöÀç SP¸¦ Áï½Ã VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_IMMEDIATE_EP_UP					61		// ¿¡³ÊÁö·ù, ÇöÀç EP¸¦ Áï½Ã VALUE¸¸Å­ ¿Ã·ÁÁÜ
#define DES_GROUNDMODE						62		// Æø°Ý¸ðµå
#define DES_SIEGEMODE						63		// ½ÃÁî¸ðµå
#define DES_WEIGHT_01						75		// (*) ¹«°Ô 01, check: Ãß°¡µÊ
#define DES_WEIGHT_02						76		// (*) ¹«°Ô 02, check: Ãß°¡µÊ
#define DES_BULLET_01						77		// (*) 1Çü ÃÑ¾Ë 01, 2005-11-02 by cmkwon ºö¼Ò¸ðÃÊ¾Ë·®À» º¯°æÇÔ
#define DES_BULLET_02						78		// (*) 2Çü ÃÑ¾Ë 02, 2005-11-02 by cmkwon ºö¼Ò¸ðÃÊ¾Ë·®À» º¯°æÇÔ
#define DES_PRIMARY_WEAPON					84		// 1Çü ¹«±â, ¹ü¿ëºö
#define DES_SECONDARY_WEAPON				85		// 2Çü ¹«±â, ¹ü¿ë
#define DES_ALL_WEAPON						86		// 1,2Çü ¹«±â ¸ðµÎ, ¹ü¿ë
#define DES_CRITICALHITRATE_01				87		// (*)Å©¸®Æ¼ÄÃ È®·ü 20040622 Ãß°¡
#define DES_CRITICALHITRATE_02				88		// (*)Å©¸®Æ¼ÄÃ È®·ü 20040622 Ãß°¡
#define DES_WARP							90		// µµ½Ã¿öÇÁ ¾ÆÀÌÅÛ¿ë
#define DES_REACTION_RANGE					91		// ITEMÀÇ ReactionRange º¯°æ
#define DES_RARE_FIX_NONE					92		// Á¢µÎ»ç, Á¢¹Ì»ç ¸ðµÎ ¾øÀ½, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_RARE_FIX_PREFIX					93		// Á¢µÎ»ç, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_RARE_FIX_SUFFIX					94		// Á¢¹Ì»ç, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_RARE_FIX_BOTH					95		// Á¢µÎ»ç, Á¢¹Ì»ç ¸ðµÎ Æ÷ÇÔ, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_REQ_MIN_LEVEL					96		// ¾ÆÀÌÅÛÀåÂø ¿ä±¸ MinLevelÀ» ³·Ãá´Ù
#define DES_REQ_MAX_LEVEL					97		// ¾ÆÀÌÅÛÀåÂø ¿ä±¸ MaxLevelÀ» ³·Ãá´Ù
#define DES_WARP_OUTPOST					98		// 2007-09-05 by dhjin, ÀüÁø±âÁö µµ½Ã¿öÇÁ ¾ÆÀÌÅÛ¿ë
#define DES_CHAT_BLOCK						99		// 2008-12-30 by cmkwon, ÁöµµÀÚ Ã¤ÆÃ Á¦ÇÑ Ä«µå ±¸Çö - 

#define DES_CASH_STAT_ALL_INITIALIZE		100		// À¯·á ¸ðµç ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
//#define DES_CASH_STAT_HALF_INITIALIZE		101		// (±¸Çö ¹ÌÈ®Á¤)À¯·á 50% ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
#define DES_CASH_STAT_PART_INITIALIZE		102		// À¯·á ºÎºÐ ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
#define DES_RARE_FIX_PREFIX_INITIALIZE		103		// Á¢µÎ»ç ÃÊ±âÈ­, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_RARE_FIX_SUFFIX_INITIALIZE		104		// Á¢¹Ì»ç ÃÊ±âÈ­, ITEMKIND_GAMEBLEÀÇ DestParam1¿¡ ¼³Á¤
#define DES_ENCHANT_INITIALIZE				105		// 2007-04-03 by cmkwon, ÀÎÃ¦Æ® ÃÊ±âÈ­, ITEMKIND_ENCHANTÀÇ DestParam1¿¡ ¼³Á¤
//											106		// ¸ðµç ½ºÅÝ(DES_ALL_PART)¿¡¼­ »ç¿ë	// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ¸ðµç ½ºÅÝ DES_ALL_PART Ãß°¡
#define DES_CASH_STEALTH					108		// ½ºÅÚ½º ¾ÆÀÌÅÛ
#define DES_CASH_HP_AND_DP_UP				109		// HP and DP UP ¾ÆÀÌÅÛ
#define DES_CASH_GUILD_ALL_MEMBERS_SUMMON	110		// ¸ðµç ¿©´Ü¿ø ¼ÒÈ¯ - ¿©´ÜÀå¸¸ »ç¿ë°¡´É
#define DES_CASH_GUILD_MEMBER_SUMMON		111		// ¿©´Ü¿ø 1¸í ¼ÒÈ¯ - ¿©´ÜÀå¸¸ »ç¿ë°¡´É
#define DES_CASH_NORMAL_RESTORE				112		// ÀÏ¹Ý ºÎÈ° Ä«µå - »ç
#define DES_CASH_SUPER_RESTORE				113		// ½´ÆÛ ºÎÈ° Ä«µå
#define DES_CASH_GUILD						114		// ÀÏ¹Ý/°í±Þ ¿©´Ü Ä«µå
#define DES_CASH_MONSTER_SUMMON				115		// ¸ó½ºÅÍ ¼ÒÈ¯ Ä«µå
#define DES_CASH_CHANGE_CHARACTERNAME		116		// Ä³¸¯ÅÍ¸í º¯°æ Ä«µå
#define DES_CASH_SKILL_INITIALIZE			117		// ½ºÅ³ ÃÊ±âÈ­ Ä«µå
#define DES_CASH_CHANGE_PILOTFACE			118		// ¾ó±¼ º¯°æ Ä«µå
#define DES_CASH_CRACKER_CARD				119		// Á¾ÇÕ ÆøÁ× Ä«µå


///////////////////////////////////////////////////////////////////////////////
// 2005-11-21 by cmkwon, »õ·Î Ãß°¡µÈ DesParam
#define DES_SKILL_REDUCE_SHIELD_DAMAGE		121		// ½¯µå¿¡ °¡´Â µ¥¹ÌÁö¸¦ ÁÙ¿©ÁØ´Ù.
#define DES_SKILL_INVINCIBLE				122		// ¹«Àû »óÅÂ
#define DES_SKILL_BIG_BOOM					123		// 		¹Ì±¸Çö - ÀÚ½ÅÀÇ HP¸¦ ¸ðµÎ ¼Ò¸ðÇÏ¿© ÁÖº¯¿¡°Ô µ¥¹ÌÁö¸¦ ÀÔÈ÷°í, ÀÚ½ÅÀº Æø¹ßÇÑ´Ù.
#define DES_SKILL_HALLUCINATION				124		// Ã¼ÇÁ »çÃâ.
#define DES_SKILL_RANGEDOWN_01				125		// 		¹Ì±¸Çö - ÀûÀÇ ±âº»¹«±â »ç°Å¸®¸¦ ÁÙÀÎ´Ù.
#define DES_SKILL_RANGEDOWN_02				126		// 		¹Ì±¸Çö - ÀûÀÇ °í±Þ ¹«±â »ç°Å¸®¸¦ ÁÙÀÎ´Ù.
#define DES_SKILL_PROHIBITION_01			127		// 		¹Ì±¸Çö - ÀûÀÇ ±âº» ¹«±â »ç¿ëÀ» ±ÝÁö½ÃÅ²´Ù.
#define DES_SKILL_PROHIBITION_02			128		// 		¹Ì±¸Çö - ÀûÀÇ °í±Þ ¹«±â »ç¿ëÀ» ±ÝÁö½ÃÅ²´Ù.
#define DES_ATTACK_RANGE_01					129		// ·¹ÀÌ´ÙÀÇ ±âº» ¹«±â Á¶ÁØ °Å¸®¸¦ Áõ°¡½ÃÅ²´Ù.(*)
#define DES_ATTACK_RANGE_02					130		// ·¹ÀÌ´ÙÀÇ °í±Þ ¹«±â Á¶ÁØ °Å¸®¸¦ Áõ°¡½ÃÅ²´Ù.(*)
#define DES_INVISIBLE						131		//	½ºÅÚ½º ¸ðµå·Î¼­ Àû¿¡°Ô º¸ÀÌÁö ¾ÊÀ¸¸ç, ·¹ÀÌ´Ù¿¡µµ ÀâÈ÷Áö ¾Ê´Â´Ù. ÀÚ½Åµµ °ø°ÝÀÌ µÇÁö ¾Ê´Â´Ù.
// 2005-12-02 by cmkwon, 154, 155, 156À¸·Î ³ª´®#define DES_SKILL_HYPERMOVING				132		// À¯´ÖÀÇ ÀüÃ¼ ÀÌµ¿¼Óµµ°¡ ÁÁ¾ÆÁö°í, ºÎ½ºÅÍ »ç¿ëÀÌ µÇÁö ¾Ê´Â´Ù.(*)
#define DES_SKILL_DEFENSE_01				133		// 		¹Ì±¸Çö - ÀûÀÇ ±âº»¹æ¾î·ÂÀ» ¶³¾î¶ß¸°´Ù.(*)
#define DES_SKILL_DEFENSE_02				134		// 		¹Ì±¸Çö - ÀûÀÇ °í±Þ¹æ¾î·ÂÀ» ¶³¾î¶ß¸°´Ù.(*)
#define DES_SKILL_FREEZINGBODY				135		// 		¹Ì±¸Çö - ÀûÀÇ HP,½¯µå È¸º¹À» ºÒ°¡´ÉÇÏ°Ô ¸¸µç´Ù. ¼ö¸®Å¶,½¯µåÅ¶,½ºÅ³È¸º¹(ÀüÃ¼)ÀÌ ¾ÈµÈ´Ù.
#define DES_SKILL_REVERSECONTROL			136		// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) -	// 		¹Ì±¸Çö - ÀûÀÇ ¿òÁ÷ÀÓÀ» ¹Ý´ë·Î ¿òÁ÷ÀÌ°Ô ÇÑ´Ù.
#define DES_SKILL_GRADUAL_HPSHIELD_DOWN		137		// 		¹Ì±¸Çö - ÀûÀÇ HP,½¯µå¸¦ Á¡Â÷ °¨¼Ò½ÃÅ²´Ù.
#define DES_SKILL_SLOWMOVING				138		// ÀûÀÇ ¿òÁ÷ÀÓÀ» µÐÇÏ°Ô ÇÑ´Ù.(ÃÖ´ëÀÌµ¿ ¼Óµµ, ºÎ½ºÅÍ ¼Óµµ, ÀÌµ¿ °¢µµ, ºÎ½ºÅÍ ÀÌµ¿ °¢µµ)(*)
#define DES_SKILL_BOOSTEROFF				139		// 		¹Ì±¸Çö - ÀûÀÇ ºÎ½ºÅÍ »ç¿ëÀ» ±ÝÁö½ÃÅ²´Ù.
#define DES_SKILL_COLLISIONDAMAGE_DOWN		140		// ¹è°æ ¿ÀºêÁ§Æ®,¹Ù´Ú Ãæµ¹½Ã µ¥¹ÌÁö¸¦ °¨¼Ò½ÃÅ²´Ù.
//#define DES_SKILL_CAMOUFLAGE				141		// 		¹Ì±¸Çö - À§ÀåÇÏ¿© ÀûÀÇ ½Ã¾ß¿¡ º¸ÀÌÁö ¾Ê´Â´Ù. Å¸°Ù¿¡´Â ÀâÈù´Ù.
#define DES_SKILL_RANDOMTELEPORT			142		// 		¹Ì±¸Çö - °°Àº ¸Ê¿¡¼­ ·£´ýÇÏ°Ô ÀÌµ¿ÇÑ´Ù.
#define DES_SKILL_ATTACK_ALL				143		// 		¹Ì±¸Çö - ÀüÃ¼ °ø°Ý ½ºÅ³
#define DES_SKILL_SCANNING					144		// Invisible »óÅÂÀÇ À¯Àú¸¦ º¼ ¼ö ÀÖµµ·Ï ÇÑ´Ù
#define DES_SKILL_REVERSEENGINE				145		// ¿£ÁøÀÌ ÈÄÁøÀÌ °¡´ÉÇÏ°Ô ÇÑ´Ù.
#define DES_SKILL_LOCKDOWN					146		// 		¹Ì±¸Çö - »ó´ë¹æÀ» ÀÏ½Ã Á¤Áö½ÃÅ²´Ù.(ÀüÁø,ÁÂ¿ì¼±È¸,ºÎ½ºÅÍ Á¤Áö, ´Ü, °ø°Ý±â´ÉÀº °¡´É)
#define DES_SKILL_STEALSP					147		// 		¹Ì±¸Çö - »ó´ë¹æÀÇ SP¸¦ ÀÏÁ¤·® –P¾Æ¿Â´Ù.
#define DES_SKILL_SMARTSP					148		// ÀÚ½ÅÀÌ »ç¿ëÇÏ´Â ¸ðµç ½ºÅ³ÀÇ »ç¿ë·®À» ÁÙ¿©ÁØ´Ù.(*)
#define DES_SKILL_SUMMON_FORMATION_MEMBER	149		// Æí´ë¿ø Áß 1¸íÀ» ¼ÒÈ¯ÇÑ´Ù.
#define DES_SKILL_CANCEL_MAGIC				150		// 		¹Ì±¸Çö - ÀûÀÇ ¹öÇÁ ½ºÅ³À» ¸ðµÎ ÇØÁ¦½ÃÅ²´Ù.
#define DES_SKILL_RANDOM_CANCEL				151		// 		¹Ì±¸Çö - Å¸°ÙÀÇ µð¹öÇÁ ½ºÅ³À» ÇÏ³ª ·£´ýÇÏ°Ô ÇØÁ¦ÇÑ´Ù.
#define DES_SKILL_STOPMAGIC					152		// ÀûÀÇ ½ºÅ³À» ÀÏÁ¤½Ã°£ »ç¿ë±ÝÁö½ÃÅ²´Ù. - 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ
#define DES_SKILL_CANCELALL					153		// 		¹Ì±¸Çö - ´ë»óÀÇ ¹öÇÁ ½ºÅ³À» ¸ðµÎ ÇØÁ¦ÇÑ´Ù.
#define DES_SKILL_REACTIONSPEED				154		// (*)ÇÏÀÌÆÛ ¹«ºù(¹ÝÀÀ¼Óµµ)
#define DES_SKILL_ENGINEANGLE				155		// (*)ÇÏÀÌÆÛ ¹«ºù(¼±È¸°¢)
#define DES_SKILL_ENGINEBOOSTERANGLE		156		// (*)ÇÏÀÌÆÛ ¹«ºù(ºÎ½ºÅÍ ¼±È¸°¢)

// 2006-03-30 by cmkwon
#define DES_DROP_EXP						157		// °æÇèÄ¡, ÇÁ¸®¹Ì¾ö°ú ÁßÃ¸ ºÒ°¡
#define DES_DROP_SPI						158		// SPI, ÇÁ¸®¹Ì¾ö°ú ÁßÃ¸ ºÒ°¡
#define DES_DROP_ITEM						159		// ¾ÆÀÌÅÛ µå¶øÀ², ÇÁ¸®¹Ì¾ö°ú ÁßÃ¸ ºÒ°¡
#define DES_HP_REPAIR_RATE_FLIGHTING		160		// ºñÇà½Ã HP È¸º¹À²
#define DES_DP_REPAIR_RATE					161		// DP È¸º¹À²
#define DES_SP_REPAIR_RATE					162		// SP È¸º¹À²

// 2006-07-26 by cmkwon
#define DES_BAZAAR_SELL						163		// 2006-07-26 by cmkwon, °³ÀÎ ÆÇ¸Å »óÁ¡
#define DES_BAZAAR_BUY						164		// 2006-07-26 by cmkwon, °³ÀÎ ±¸¸Å »óÁ¡

// 2006-08-14 by dhjin
#define DES_KILLMARK_EXP					165		// 2006-08-14 by dhjin, Å³¸¶Å© °æÇèÄ¡

// 2006-10-11 by cmkwon
#define DES_HYPER_BOOSTER					166		// 2006-10-11 by cmkwon, ºÎ½ºÅÍ °ÔÀÌÁö ¶³¾îÁöÁö ¾ÊÀ½

// 2006-11-17 by dhjin, 2Â÷ ½ºÅ³·Î ÀÎÇØ Ãß°¡µÈ »çÇ×
#define DES_SKILL_CHAFF_HP					167		// 2006-11-17 by dhjin, Ã¼ÇÁÀÇ HP
#define DES_SKILL_AIR_BOMBING				168		// 2006-11-17 by dhjin, °øÁß Æø°Ý
#define DES_SKILL_NO_WARNING				169		// 2006-11-17 by dhjin, Å¸±â¾î¿¡ ½ºÅ³ »ç¿ë½Ã, ÇØ´ç±â¾î´Â ¿ö´×À½°ú ·¹ÀÌ´õ Á¡¸êÀÌ ¿ï¸®Áö ¾Ê°Ô µÈ´Ù
#define DES_SKILL_ROLLING_TIME				170		// 2006-11-17 by dhjin, 30ÃÊµ¿¾È ·Ñ¸µ Àç»ç¿ë ½Ã°£ÀÌ ¾ø´Ù.
#define DES_SKILL_FULL_RECOVERY				171		// 2006-11-17 by dhjin, ±âÃ¼ÀÇ HP, ½¯µå, ¿¬·á, SP¸¦ ¿ÏÀüÈ¸º¹ÇÑ´Ù.
#define DES_SKILL_CAMOUFLAGE				172		// 2006-11-17 by dhjin, A±â¾îÀÇ À§Àå ½ºÅ³
#define DES_SKILL_BARRIER					173		// 2006-11-17 by dhjin, A±â¾îÀÇ °í±Þ¹«±â¿¡ ´ëÇÑ ¹«Àû ½ºÅ³
#define DES_SKILL_HYPERSHOT					174		// 2006-11-17 by dhjin, A±â¾îÀÇ ±âº»¹«±â Â÷Â¡¼¦, ½ºÇÃ·¡½¬ µ¥¹ÌÁö´Â Range
#define DES_SKILL_SHIELD_PARALYZE			175		// 2006-11-17 by dhjin, A±â¾îÀÇ ½¯µå¸¶ºñ ½ºÅ³, »ó´ë±â¾îÀÇ ½¯µå È¸º¹À²À» 0À¸·Î ¸¸µç´Ù.

#define DES_WARHEAD_SPEED					176		// 2007-06-11 by cmkwon, ÅºµÎÀÇ ¼Óµµ
#define DES_CHAT_ALL_INFLUENCE				177		// 2007-08-09 by cmkwon, ¸ðµç ¼¼·Â¿¡ Ã¤ÆÃ Àü¼ÛÇÏ±â - desparam Ãß°¡, À¯·á¾ÆÀÌÅÛ

// 2008-09-22 by dhjin, ½Å±Ô ÀÎÃ¾Æ®
#define DES_ENGINE_BOOSTER_TIME_UP			178		// 2008-09-22 by dhjin, ºÎ½ºÅÍ ½Ã°£ Áõ°¡
#define DES_ENGINE_MAX_SPEED_UP				179		// 2008-09-22 by dhjin, ¿£Áø ÀÏ¹Ý¼Óµµ(ÃÖ´ë) Áõ°¡
#define DES_ENGINE_MIN_SPEED_UP				180		// 2008-09-22 by dhjin, ¿£Áø ÀÏ¹Ý¼Óµµ(ÃÖ¼Ò) Áõ°¡
#define DES_ENGINE_BOOSTER_SPEED_UP			181		// 2008-09-22 by dhjin, ¿£Áø ºÎ½ºÅÍ¼Óµµ Áõ°¡
#define DES_ENGINE_GROUND_SPEED_UP			182		// 2008-09-22 by dhjin, ¿£Áø Áö»ó¼Óµµ Áõ°¡
#define DES_RADAR_OBJECT_DETECT_RANGE		183		// 2008-09-22 by dhjin, ·¹ÀÌ´õ ¹°Ã¼ °¨Áö ¹Ý°æ
#define DES_PIERCE_UP_01					184		// 2008-09-22 by dhjin, ±âº»¹«±â ÇÇ¾î½ºÀ² Áõ°¡ Ä«µå
#define DES_PIERCE_UP_02					185		// 2008-09-22 by dhjin, °í±Þ¹«±â ÇÇ¾î½ºÀ² Áõ°¡ Ä«µå
#define DES_ENGINE_ANGLE_UP                 186		// 2008-09-30 by dhjin, ¿£Áø È¸Àü°¢ Áõ°¡ Ä«µå
#define DES_ENGINE_BOOSTERANGLE_UP          187		// 2008-09-30 by dhjin, ¿£Áø ºÎ½ºÅÍ È¸Àü°¢ Áõ°¡ Ä«µå

// 2009-01-05 by dhjin, ¹Ì¼Ç¸¶½ºÅÍ - Æí´ë ¹öÇÁ ¾ÆÀÌÅÛ Ãß°¡ 
#define DES_ITEM_BUFF_INFLUENCE				188
#define DES_ITEM_BUFF_PARTY					189

// 2009-01-19 by dhjin, ÀÎÃ¾Æ® È®·ü Áõ°¡, 10ÀÎÃ¾ ÆÄ¹æ Ä«µå - ÀÎÃ¾Æ® È®·ü Áõ°¡ Ä«µå
#define DES_ENCHANT_PREVENTION_DELETE_USE_ENCHANT	190		// »ç¿ë ÇÏ±â À§ÇÑ ¾ÆÀÌÅÛÀÇ º¸À¯ÀÎÃ¦Æ® Ä«¿îÆ® ¼³Á¤
#define DES_ENCHANT_PREVENTION_DELETE_SAVE_ENCHANT	191		// ÀÎÃ¦Æ® ½ÇÆÐ½Ã ³²±â´Â ÀÎÃ¦Æ® Ä«¿îÆ® ¼³Á¤
#define DES_ENCHANT_INCREASE_PROBABILITY			192		// ÀÎÃ¦Æ® Ä«µå È®·ü Áõ°¡ºÐ ¼³Á¤

///////////////////////////////////////////////////////////////////////////////
// 2009-08-03 by cmkwon, EP3-4 Æí´ë ´ëÇü ½ºÅ³ ±¸Çö - DES_ Ãß°¡
#define DES_SKILL_DAMAGE_DISTRIBUTION				193		// µ¥¹ÌÁö¸¦ Æí´ë¿ø¿¡°Ô ºÐ»ê(ºÐ¹è) Ã³¸® ÇÑ´Ù.

///////////////////////////////////////////////////////////////////////////////
// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - 
#define DES_SHAPE_ITEM								194		// Á¶ÇÕ½Ã ¼Ò½º ¾ÆÀÌÅÛÀÇ ShapeItemNumÀ» º¯°æ ÇÒ Item, LinkItemÀ» »ç¿ë		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo
#define DES_EFFECT_ITEM								195		// Á¶ÇÕ½Ã ¼Ò½º ¾ÆÀÌÅÛÀÇ EffectItemNumÀ» º¯°æ ÇÒ Item, ÇØ´ç ItemNumÀ» »ç¿ë	(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo

///////////////////////////////////////////////////////////////////////////////
// 2009-09-23 by cmkwon, ÇÊµåÃ¢°í Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - 
#define DES_FIELD_STORE								196		// ÇÊµåÃ¢°í

///////////////////////////////////////////////////////////////////////////////
// 2009-10-01 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ °ü·Ã ÃÊ±âÈ­ ±â´É ±¸Çö - 
#define DES_INIT_SHAPE_ITEM							197		// Á¶ÇÕ½Ã ¼Ò½º ¾ÆÀÌÅÛÀÇ ShapeItemNumÀ» ÃÊ±âÈ­ ÇÒ Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo
#define DES_INIT_EFFECT_ITEM						198		// Á¶ÇÕ½Ã ¼Ò½º ¾ÆÀÌÅÛÀÇ EffectItemNumÀ» ÃÊ±âÈ­ ÇÒ Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo

///////////////////////////////////////////////////////////////////////////////
// 2009-11-02 by cmkwon, Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) ¾ÆÀÌÅÛ Ãß°¡ ±¸Çö - DES_XXX Ãß°¡
#define DES_INCREASE_INVENTORY_SPACE				199		// Ä³¸¯ÅÍ ÀÎº¥Åä¸® Áõ°¡
#define DES_INCREASE_STORE_SPACE					200		// Ä³¸¯ÅÍ Ã¢°í Áõ°¡

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
#define DES_ITEM_RESISTANCE							201		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀúÇ× ¾ÆÀÌÅÛ »ç¿ë Ã¼Å© 
#define DES_ITEM_ADDATTACK							202		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - Àý´ë°ª Å¸°ÝÄ¡ ¾ÆÀÌÅÛ »ç¿ë Ã¼Å© 
#define DES_ITEM_IGNOREDEFENCE						203		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹æ¾î·Â ¹«½Ã ¾ÆÀÌÅÛ »ç¿ë Ã¼Å©
#define DES_ITEM_IGNOREAVOID						204		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - È¸ÇÇ·Â ¹«½Ã ¾ÆÀÌÅÛ »ç¿ë Ã¼Å©
#define DES_ITEM_REDUCEDAMAGE						205		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - µ¥¹ÌÁö Àý´ë°ª °¨¼Ò ¾ÆÀÌÅÛ »ç¿ë Ã¼Å©
#define DES_ITEM_ADDATTACK_SEC						206		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - Àý´ë°ª Å¸°ÝÄ¡ ¾ÆÀÌÅÛ °í±Þ¹«±â¿ë(±â¹«¿Í ºÐ·ù)
#define DES_ITEM_ONCE_RESISTANCE					207		// 2009-09-09 ~ 2010-01-19 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÇÑ ¹ø¸¸ ÀúÇ×ÇÏ°í ¾ø¾îÁö´Â ÀúÇ× ¾ÆÀÌÅÛ Ãß°¡
#define DES_SKILL_MON_SILENCE_PRIMARY				210		// ¸ó½ºÅÍ »çÀÏ·±½º ½ºÅ³ (1Çü¹«±â Àû¿ë)
#define DES_SKILL_MON_SILENCE_SECOND				211		// ¸ó½ºÅÍ »çÀÏ·±½º ½ºÅ³ (2Çü¹«±â Àû¿ë)
#define DES_SKILL_MON_FREEZE_HP						212		// ¸ó½ºÅÍ ÇÁ¸®Áî ½ºÅ³ HP È¸º¹ ºÒ°¡  
#define DES_SKILL_MON_FREEZE_DP						213		// ¸ó½ºÅÍ ÇÁ¸®Áî ½ºÅ³ DP È¸º¹ ºÒ°¡
#define DES_SKILL_MON_FREEZE_SP						214		// ¸ó½ºÅÍ ÇÁ¸®Áî ½ºÅ³ SP È¸º¹ ºÒ°¡
#define DES_SKILL_MON_HOLD							215		// ¸ó½ºÅÍ È¦µå ½ºÅ³
#define DES_SKILL_MON_STEALING						216		// ¸ó½ºÅÍ ½ºÆ¿¸µ ½ºÅ³ 
#define DES_SKILL_MON_DRAIN							217		// ¸ó½ºÅÍ µå·¹ÀÎ ½ºÅ³
#define DES_SKILL_RELEASE							218		// M±â¾î ¸±¸®Áî ½ºÅ³
#define DES_SKILL_MON_SILENCE_SKILL					219		// ¸ó½ºÅÍ »çÀÏ·±½º ½ºÅ³ (½ºÅ³ Àû¿ë)

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
#define DES_PAIR_DRAIN_1_RATE						220		// µå·¹ÀÎ ¹ßµ¿ È®·ü
#define DES_PAIR_DRAIN_2_HP_DP_UP_RATE				221		// µå·¹ÀÎ È¸º¹·® HP->DP È¸º¹ (µ¥¹ÌÁöÀÇ %)
#define DES_ANTI_DRAIN_RATE							222		// µå·¹ÀÎ ¹ßµ¿ ÀúÇ× È®·ü
#define DES_PAIR_REFLECTION_1_RATE					223		// µ¥¹ÌÁö ¹Ý»ç È®·ü
#define DES_PAIR_REFLECTION_2_DAMAGE_RATE			224		// ¹Ý»ç µ¥¹ÌÁö·® %
#define DES_ANTI_REFLECTION_RATE					225		// µ¥¹ÌÁö ¹Ý»ç ÀúÇ× È®·ü

///////////////////////////////////////////////////////////////////////////////
// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - 
#define DES_BLIND									226
#define DES_SUPERINTEND								227
#define DES_IMMEDIATE_HP_OR_DP_UP					228		// HP¸¦ ¸ÕÀú Ã¤¿ì°í ±× ÀÌÈÄ¿¡ DP¸¦ Ã¤¿î´Ù.
#define DES_HIT_INVOKE_SKILL						229		// ¸íÁß½Ã ÇØ´ç µ¥½ºÆÄ¶÷°ª¿¡ ½ºÅ³À» ½ÃÀüÇÑ´Ù.
#define DES_TIME_BOMB								230

///////////////////////////////////////////////////////////////////////////////
// 2010-03-18 by cmkwon, ¸ó½ºÅÍº¯½Å ±¸Çö - 
#define DES_TRANSFORM_TO_MONSTER					231		// ParamValue¿¡ MonsterUnitKind ÀÔ·Â

///////////////////////////////////////////////////////////////////////////////
// 2010-03-23 by cmkwon, ÀÎÇÇ´ÏÆ¼ ÀÔÀå Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - 1È¸¸¸ Ãß°¡ ÀÔÀå °¡´É
#define DES_INFINITY_REENTRY_TICKET					232		// ÇöÀç´Â ParamValue´Â »ç¿ëÇÏÁö ¾ÊÀ½

///////////////////////////////////////////////////////////////////////////////
// 2010-04-05 by cmkwon, ¸ó½ºÅÍº¯½Å°ü·Ã ÇØÁ¦ Ä«µå ±¸Çö - 
#define DES_TRANSFORM_TO_GEAR						233		// ÇöÀç´Â ParamValue´Â »ç¿ëÇÏÁö ¾ÊÀ½.

///////////////////////////////////////////////////////////////////////////////
// 2010-05-18 by cmkwon, WarPoint Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö(ÀÏº»¿äÃ») - 
#define DES_PLUS_WARPOINT_RATE						234		// ParamValue: WarPoint Áõ°¡ Rate

///////////////////////////////////////////////////////////////////////////////
// 2010-06-01 by shcho, PC¹æ ±ÇÇÑ È¹µæ(Ä³½¬) ¾ÆÀÌÅÛ - 
#define DES_PCROOM_USE_CARD							235		//PC¹æ ÇÃ·¡±×¸¦ TRUE·Î º¯È¯ ½ÃÅ²´Ù.

///////////////////////////////////////////////////////////////////////////////
// 2010-08-26 by shcho&jsKim, ¹ã ¾ÆÀÌÅÛ ±¸Çö -
#define DES_MAX_SP_UP								236		// SPÀÇ ÃÖ´ëÄ¡¸¦ Áõ°¡½ÃÅ²´Ù.

///////////////////////////////////////////////////////////////////////////////
// 2010-08-27 by shcho&&jskim, WARPOINT Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
#define DES_WAR_POINT_UP							237		// WARPOINT Áõ°¡

///////////////////////////////////////////////////////////////////////////////
// 2010-11-30 by shcho, ·¹¾î¾ÆÀÌÅÛ µå¶øÈ®·ü Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
#define DES_RARE_ITEM_DROP_RATE						238		// ·¹¾î ¾ÆÀÌÅÛ µå¶øÈ®·ü Áõ°¡

///////////////////////////////////////////////////////////////////////////////
// 2010-12-21 by jskim, ¸¶À» ÀÌµ¿ ¼Óµµ Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
#define DES_RARE_ITEM_PARTNER_SPEED					239		// ¸¶À» ÀÌµ¿ ½ºÇÇµå Áõ°¡

///////////////////////////////////////////////////////////////////////////////
// 2010-12-21 by jskim, ÆÄÆ®³Ê µ¥¹ÌÁö Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
#define DES_RARE_ITEM_PARTNER_DAMAGE				240		// ÆÄÆ®³Ê µ¥¹ÌÁö Áõ°¡

///////////////////////////////////////////////////////////////////////////////
// 2010-12-21 by jskim, HP, DP Å°Æ® Àû¿ë·® Áõ°¡ ¾ÆÀÌÅÛ ±¸Çö
#define DES_RARE_ITEM_HPDP							241		// HP DP Å°Æ® Àû¿ë·®

// start 2011-05-02 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡ - Æ¯Á¤ ÁÂÇ¥¿¡ ¼ÒÈ¯
#define DES_SUMMON_POSITION_X						242		// ¼ÒÈ¯½Ã À§Ä¡ º¯°æ (»ó´ë°ª)
#define DES_SUMMON_POSITION_Y						243		// ¼ÒÈ¯½Ã À§Ä¡ º¯°æ (»ó´ë°ª)
#define DES_SUMMON_POSITION_Z						244		// ¼ÒÈ¯½Ã À§Ä¡ º¯°æ (»ó´ë°ª)
// end 2011-05-02 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡ - Æ¯Á¤ ÁÂÇ¥¿¡ ¼ÒÈ¯

// start 2011-10-18 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³
#define DES_MAPBUFF_RANDOM_ADD_REATTACKTIME			245		// ¸Ê¹öÇÁ Ãß°¡ ½Ã°£ Àû¿ë
#define DES_MAPBUFF_RANDOM_ADD_TIME					246		// ¸Ê¹öÇÁ Ãß°¡ ½Ã°£ Àû¿ë
#define DES_ENCHANT_ONCE_APPLY						247		// 2011-10-19 by hskim, EP4 [Free to play] - 10È¸ ÀÎÃ¾Æ® ¾ÆÀÌÅÛ ±â´É ±¸Çö
#define DES_GAMBLE_RARE_ITEM_FIX					248		// 2011-10-20 by hskim, EP4 [Free to play] - Á¢µÎ/Á¢¹Ì °íÁ¤ ¿É¼Ç
// end 2011-10-18 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³

// 2013-05-07 by jhseol,bckim ¹öÇÁ ÆÐ³ÎÆ¼
#define DES_BUFF_PENALTY_RATIO						251		// ÆÐ³ÎÆ¼ ºñÀ² (1.0f = 100%)
#define DES_BUFF_PENALTY_APPLIED_UNIT_KIND			252		// ÆÐ³ÎÆ¼ Àû¿ë ±â¾î (ReqUnitKind)
// end 2013-05-07 by jhseol,bckim ¹öÇÁ ÆÐ³ÎÆ¼

// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±
#define DES_SKILLTYPE_CONSECUTIVE_VICTORIES			253		// ¼¼·Â Æ÷ÀÎÆ® °³¼± - ¿¬½ÂÀÇ ¹öÇÁ·ù Á¾·ù
#define DES_SKILLTYPE_TRUN_AROUND					254		// ¼¼·Â Æ÷ÀÎÆ® °³¼± - ¿ªÀüÀÇ ¹öÇÁ·ù Á¾·ù

// 2015-08-02 by killburne
#define DES_SKILLTYPE_OUTPOST_BUFF					255

#define DES_SKILL_BUFF_MON_ATTACK_POWER				300		// ¸ó½ºÅÍ °ø°Ý½Ã - °ø°Ý·Â Áõ°¡ : Value Áõ°¡ %
#define DES_SKILL_BUFF_MON_ATTACK_PROBABILITY		301		// ¸ó½ºÅÍ °ø°Ý½Ã - °ø°Ý·Â È®À² : Value Áõ°¡ %
#define DES_SKILL_BUFF_MON_ATTACK_PIERCE			302		// ¸ó½ºÅÍ °ø°Ý½Ã - ÇÇ¾î½º Áõ°¡ : Value Áõ°¡ %
#define DES_SKILL_BUFF_MON_DEFENCE					303		// ¸ó½ºÅÍ ¹æ¾î½Ã - ¹æ¾î·Â Áõ°¡ : Value Áõ°¡ %
#define DES_SKILL_BUFF_MON_DEFENCE_AVOID			304		// ¸ó½ºÅÍ ¹æ¾î½Ã - È¸ÇÇ·Â Áõ°¡ : Value Áõ°¡ %
#define DES_SKILL_BUFF_PVP_ATTACK_POWER				305		// PVP - °ø°Ý·Â Áõ°¡ : Value Áõ°¡ %
#define DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY		306		// PVP - ¸íÁß·ü Áõ°¡ : Value Áõ°¡ %		// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó
#define DES_SKILL_BUFF_PVP_ATTACK_PIERCE			307		// PVP - ÇÇ¾î½º Áõ°¡ : Value Áõ°¡ %		// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó
#define DES_SKILL_BUFF_PVP_DEFENCE					308		// PVP - ¹æ¾î·Â Áõ°¡ : Value Áõ°¡ %		// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó
#define DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY		309		// PVP - È¸ÇÇ·Â Áõ°¡ : Value Áõ°¡ %		// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó
// end 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±

// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - COLLECTION INDEX DesParam µî·Ï (390~399 : 10°³ ¿¹¾à)
#define DES_COLLECTION_ARMOR_INDEX					390		// ¾Æ¸Ó ÄÃ·º¼Ç¿¡¼­ »ç¿ëµÉ ÀÎµ¦½º
// end 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - COLLECTION INDEX DesParam µî·Ï (390~399 : 10°³ ¿¹¾à)

// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡
#define DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER 		499		// ParamValue: WarPoint Áõ°¡ Rate ( ´ÙÈ¸¼º ) // 1È¸¼º DES_PLUS_WARPOINT_RATE
#define DES_BUFF_TO_RETURN_USER						500		// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡
// End 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡

// 2012-10-10 by hskim, ±â°£Á¦ ¼Ó¼º ±¸Çö (±â°£Á¦ ¿ÜÇü)
#define DES_FIXED_TERM_SHAPE_TIME					501		// ±â°£Á¦ ¿ÜÇü Àû¿ë ½Ã°£ (ÃÊ)
#define DES_FIXED_TERM_ITEM_TIME					502		// ±â°£Á¦ ¾ÆÀÌÅÛ Àû¿ë ½Ã°£ (ÃÊ)
// end 2012-10-10 by hskim, ±â°£Á¦ ¼Ó¼º ±¸Çö (±â°£Á¦ ¿ÜÇü)

#define DES_WEB_DELETE_ITEM							503		// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡ - À¥ ÀÎÅÍÆäÀÌ½º·Î ¿¡¼­ »èÁ¦ °¡´ÉÇÑ ¾ÆÀÌÅÛ

#define DES_INGAME_SHOW								504		// ÀÎ°ÔÀÓ Á¶ÇÕ½Ä¿¡ Ç¥½ÃÇÒÁö ¿©ºÎ // 2013-07-02 by bhsohn ÀÎ°ÔÀÓ Á¶ÇÕ½Ä ShowÃß°¡ Ã³¸®

// start 2011-09-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷

// ½Å ¹öÇÁ·ù ¾ÆÀÌÅÛ ¿¹¾à ( 600 ~ 699 : 100°³)  
// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó - Ãß°¡ DesParam ¼±¾ð
#define DES_OPTION_ITEM_DEFAULT_DESPARAM			600		// ÀÌ´ÞÀÇ ¾Æ¸Ó¿¡ Àû¿ëµÉ ¿É¼Ç ¾ÆÀÌÅÛÀÎÁö ±¸ºÐÇÒ DesParam

// ÆÄÆ®³Ê ¼ÒÄÏ ¾ÆÀÌÅÛ·ù ¿¹¾à (23000 ~ 23899 : 900 °³ )
#define DES_PET_SOCKET_ITEM_AUTOKIT					23000		// ÆÄÆ®³Ê ½Ã½ºÅÛ ¼ÒÄÏ·ù - ÀÚµ¿ Å°Æ®, ÇÊ¼öÀûÀ¸·Î DestParameter 0¹ø¿¡ ¼³Á¤ ÇØ¾ßÇÔ
#define DES_PET_SOCKET_ITEM_AUTOSKILL				23001		// ÆÄÆ®³Ê ½Ã½ºÅÛ ¼ÒÄÏ·ù - ÀÚµ¿ ½ºÅ³, ÇÊ¼öÀûÀ¸·Î DestParameter 0¹ø¿¡ ¼³Á¤ ÇØ¾ßÇÔ
#define DES_PET_SOCKET_ITEM_SPEED					23002		// ÆÄÆ®³Ê ½Ã½ºÅÛ ¼ÒÄÏ·ù - ÀÚµ¿ ½ºÅ³, ÇÊ¼öÀûÀ¸·Î DestParameter 0¹ø¿¡ ¼³Á¤ ÇØ¾ßÇÔ

// ÆÄÆ®³Ê ÀÚµ¿ Å°Æ® ¾ÆÀÌÅÛ·ù ¿¹¾à (23900 ~ 23909 : 10 °³)
#define DES_PET_SLOT_ITEM_AUTOKIT_HP				23900		// ÀÚµ¿ Å°Æ® HP		=> VALUE : µî±Þ 1 ~ 100 ±îÁö
#define DES_PET_SLOT_ITEM_AUTOKIT_SHIELD			23901		// ÀÚµ¿ Å°Æ® Shield	=> VALUE : µî±Þ 1 ~ 100 ±îÁö
#define DES_PET_SLOT_ITEM_AUTOKIT_SP				23902		// ÀÚµ¿ Å°Æ® SP		=> VALUE : µî±Þ 1 ~ 100 ±îÁö

// ÆÄÆ®³Ê ÀÚµ¿ ½ºÅ³ ¾ÆÀÌÅÛ·ù ¿¹¾à (23910 ~ 23919 : 10 °³)
#define DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR			23910		// ÀÚµ¿ ½ºÅ³ A ±â¾î
#define DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR			23911		// ÀÚµ¿ ½ºÅ³ B ±â¾î
#define DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR			23912		// ÀÚµ¿ ½ºÅ³ I ±â¾î
#define DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR			23913		// ÀÚµ¿ ½ºÅ³ M ±â¾î

#define IS_DES_PET_SOCKET_ITEM(pItemInfo)	( DES_PET_SOCKET_ITEM_AUTOKIT <= (pItemInfo)->ArrDestParameter[0] && (pItemInfo)->ArrDestParameter[0] <= DES_PET_SOCKET_ITEM_SPEED )
// end 2011-09-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷

// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - ITEM::IsExistDesParam() ÇÔ¼ö·Î ÅëÇÕÇÔ
//#define IS_EXIST_DES_PARAM(pItemInfo, des)	((des) == (pItemInfo)->DestParameter1 || (des) == (pItemInfo)->DestParameter2 \
//												|| (des) == (pItemInfo)->DestParameter3 || (des) == (pItemInfo)->DestParameter4)

// 2008-10-21 by cmkwon, ¸¶Áö¸·¿¡ ¼±ÅÃµÈ ·¹¾î´Â ´Ù½Ã ³ª¿ÀÁö ¾Ê°Ô ¼öÁ¤ - 
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 
//#define IS_DES_RARE_FIX_PREFIX(pItemInfo)	( DES_RARE_FIX_PREFIX == (pItemInfo)->DestParameter1 )
//#define IS_DES_RARE_FIX_SUFFIX(pItemInfo)	( DES_RARE_FIX_SUFFIX == (pItemInfo)->DestParameter1 )
#define IS_DES_RARE_FIX_PREFIX(pItemInfo)	( DES_RARE_FIX_PREFIX == (pItemInfo)->ArrDestParameter[0] )
#define IS_DES_RARE_FIX_SUFFIX(pItemInfo)	( DES_RARE_FIX_SUFFIX == (pItemInfo)->ArrDestParameter[0] )

// 2006-07-26 by cmkwon
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ ÇÔ.
//#define IS_BAZAAR_SKILL(pItemInfo)			(IS_EXIST_DES_PARAM((pItemInfo), DES_BAZAAR_SELL) || IS_EXIST_DES_PARAM((pItemInfo), DES_BAZAAR_BUY))
#define IS_BAZAAR_SKILL(pItemInfo)			((pItemInfo)->IsExistDesParam(DES_BAZAAR_SELL) || (pItemInfo)->IsExistDesParam(DES_BAZAAR_BUY))

// 2006-08-14 by dhjin
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 
//#define IS_KILLMARK_EXP(pItemInfo)			(DES_KILLMARK_EXP==(pItemInfo)->DestParameter1)
#define IS_KILLMARK_EXP(pItemInfo)			(DES_KILLMARK_EXP==(pItemInfo)->ArrDestParameter[0])

// 2013-05-28 by bhsohn ¼¼·Â¹öÇÁ°¡ [Á¤È­]¿¡ ´Ù ³¯¶ó°¡´Â Çö»ó Ã³¸®
#define IS_INFUENCE_BUFF(pItemInfo)   ((pItemInfo)->IsExistDesParam(DES_SKILLTYPE_CONSECUTIVE_VICTORIES) || (pItemInfo)->IsExistDesParam(DES_SKILLTYPE_TRUN_AROUND))
// END 2013-05-28 by bhsohn ¼¼·Â¹öÇÁ°¡ [Á¤È­]¿¡ ´Ù ³¯¶ó°¡´Â Çö»ó Ã³¸®

///////////////////////////////////////////////////////////////////////////////
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 
#define SIZE_MAX_DESPARAM_COUNT_IN_ITEM			10


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2005-12-21 by cmkwon, ¸Þ¸ð¸®ÇÙ Ã¼Å© Å¸ÀÔ Á¤ÀÇ
#define HACK_CHECK_TYPE_SHOTNUM					1		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_MULTINUM				2		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_REATTACKTIME			3		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_REPEATTIME				4		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_TIME					5		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_RANGEANGLE				6		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_BOOSTERANGLE			7		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_ORBITTYPE				8		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_ABILITYMIN				9		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_ABILITYMAX				10		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_SPEEDPENALTY			11		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_RANGE					12		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_TRANSPORT				13		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_TOTALWEIGHT				14		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_POSITION				15		// 2005-12-20 by cmkwon
#define HACK_CHECK_TYPE_MAP_CHECK_SUM			16		// 2007-04-10 by cmkwon
#define HACK_CHECK_TYPE_RESOBJ_CHECK_SUM		17		// 2007-05-28 by cmkwon

#define HACK_CHECK_TYPE_END				HACK_CHECK_TYPE_RESOBJ_CHECK_SUM		// 2012-12-14 by hskim, ¸Þ¸ð¸®ÇÙ ÀÚµ¿ ºí·° ±â´É ±¸Çö

///////////////////////////////////
// ¾ÆÀÌÅÛ ÀçÁú
#define MAT_MATERIAL01						0	// ÀçÁú1
#define MAT_MATERIAL02						1	// ÀçÁú2
#define MAT_MATERIAL03						2	// ÀçÁú3
#define MAT_MATERIAL04						3	// ÀçÁú4
#define MAT_MATERIAL05						4	// ÀçÁú5

///////////////////////////////////////////////////////////////////////////////
// º¸±Þ »óÁ¡¿¡¼­ È¸º¹½Ã ÇÊ¿äÇÑ SPI
#define COST_HP_REPAIR_PER_1HP				5		// 1HP ´ç È¸º¹ ºñ¿ë
#define COST_DP_REPAIR_PER_1DP				3		// 1DP ´ç È¸º¹ ºñ¿ë - ½¯µå
#define COST_EP_REPAIR_PER_1EP				1		// 1EP ´ç È¸º¹ ºñ¿ë - ¿¬·á
#define COST_SP_REPAIR_PER_1SP				200		// 1SP ´ç È¸º¹ ºñ¿ë - 

///////////////////////////////////
// Skills

/******************************************************************************
//  BEGIN - Deprecated - 2003. 04. 21.

// ±¸Á¶Ã¼ÀÇ ItemInfo(skill)À» ÀÌ¿ëÇÑ ¼¼ÆÃ¿¡ ´ÙÀ½ÀÇ Á¾·ù¿Í ·¹º§À» ¼¼ÆÃÇÑ´Ù
// skill & item ÀÇ RequiredLevel field¿¡ »ç¿ëÇÑ´Ù
#define LEVEL_TRANSMITTER			0x00	// Åë½Å±â ÀåÂø, ¸Þ½ÅÀú »ç¿ë°¡´É(·¹º§1)
#define LEVEL_BEAM					0x01	// ºö°è¿­ ÀåÂø(·¹º§3)
#define LEVEL_ROCKET				0x02	// ·ÎÄÏ ÀåÂø(·¹º§5)
#define LEVEL_PARTY					0x03	// ÆÄÆ¼ »ý¼º,°¡ÀÔ(·¹º§6)
#define LEVEL_PICKING				0x04	// ÀÚ¿øÃ¤Ãë(·¹º§10)
#define LEVEL_BANK					0x05	// ÀºÇà»ç¿ë°¡´É(·¹º§11)
#define LEVEL_SALE					0x06	// ¾ÆÀÌÅÛ ÀÚµ¿ÆÇ¸Å°¡´É(·¹º§12)
#define LEVEL_TALKROOM				0x07	// ´ëÈ­¹æ °³¼³(·¹º§13)
#define LEVEL_GUIDED				0x08	// À¯µµÅº°è¿­ ÀåÂø(·¹º§15)
#define LEVEL_PET					0x09	// Æê »ç¿ë°¡´É(·¹º§20)
#define LEVEL_GUILD					0x0A	// ±æµå »ý¼º(·¹º§30)
#define LEVEL_MAKING				0x0B	// ¾ÆÀÌÅÛ Á¦Á¶ ´É·Â(·¹º§50)
// END - Deprecated - 2003. 04. 21.
******************************************************************************/

///////////////////////////////////
// Skill - BaseNum & Macro

/* ½ºÅ³ÀÇ ItemNum ¹× BaseNumÀ» ¸¸µå´Â ¹ý
	- ItemNum: ÃÑ 7ÀÚ¸® Á¤¼ö, DB¿¡ ÀúÀå
		1,2	ÀÚ¸®: 78
		3	ÀÚ¸®: UnitKind(BT:0, OT: 1, DT: 2, ST: 3)
		4	ÀÚ¸®: Skill Á¾·ù(°ø°Ý: 0, ¹æ¾î: 1, ÄÁÆ®·Ñ: 2, ¼Ó¼º|º¸Á¶: 3)
		5,6	ÀÚ¸®: ½ºÅ³ ¹øÈ£
		7	ÀÚ¸®: Skill Level
	- BaseNum: ItemNum¿¡¼­ 7¹øÂ° ÀÚ¸®¸¦ 0À¸·Î ¹Ù²Û´Ù, DB¿¡ ÀúÀå ¾ÈµÊ
*/

// 2013-03-12 by jhseol, ½ºÅ³ ³Ñ¹ö¸µ ½Ã½ºÅÛ ¼öÁ¤
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
#define SKILL_KIND_ATTACK		ITEMKIND_SKILL_ATTACK				// °ø°Ý
#define SKILL_KIND_DEFENSE		ITEMKIND_SKILL_DEFENSE				// ¹æ¾î
#define SKILL_KIND_CONTROL		ITEMKIND_SKILL_SUPPORT				// ÄÁÆ®·Ñ
#define SKILL_KIND_ATTRIBUTE	ITEMKIND_SKILL_ATTRIBUTE			// ¼Ó¼º/º¸Á¶
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
#define SKILL_BASE_NUM(x)	((int)x/10)*10						// ½ºÅ³ÀÇ ItemNum¿¡¼­ BaseNumÀ» ±¸ÇÔ
#define SKILL_LEVEL(x)		(x - ((int)x/10)*10)				// ½ºÅ³ÀÇ ItemNum¿¡¼­ SkillLevelÀ» ±¸ÇÔ
#define SKILL_NUMBER(x)		((int)x/10) - ((int)x/1000)*100		// ½ºÅ³ ¹øÈ£
#define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill Á¾·ù(°ø°Ý: 0, ¹æ¾î: 1, ÄÁÆ®·Ñ: 2, ¼Ó¼º|º¸Á¶: 3)
#define SKILL_UNIT_KIND(x)	((int)x/10000) - ((int)x/100000)*10	// UnitKind(BT:0, OT: 1, DT: 2, ST: 3)

#define SKILL_KIND_ATTACK		0				// °ø°Ý
#define SKILL_KIND_DEFENSE		1				// ¹æ¾î
#define SKILL_KIND_CONTROL		2				// ÄÁÆ®·Ñ
#define SKILL_KIND_ATTRIBUTE	3				// ¼Ó¼º/º¸Á¶
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
// end 2013-03-12 by jhseol, ½ºÅ³ ³Ñ¹ö¸µ ½Ã½ºÅÛ ¼öÁ¤
//#define SKILL_KIND_SUPPORT		4

///////////////////////////////////////////////////////////////////////////////
// 2005-11-22 by cmkwon, ½ºÅ³ BaseNum Á¤ÀÇ
// B-Gear
#define BGEAR_SKILL_BASENUM_BACKMOVEMACH		7802010	// ÄÁÆ®·Ñ	- ¹é¹«ºê¸¶ÇÏ
#define BGEAR_SKILL_BASENUM_TURNAROUND			7802020	// ÄÁÆ®·Ñ	- ÅÏ¾î¶ó¿îµå	
#define BGEAR_SKILL_BASENUM_GROUNDBOMBINGMODE	7800040	// Åä±Û		- Áö»óÆø°Ý¸ðµå
#define BGEAR_SKILL_BASENUM_AIRBOMBINGMODE		7800070	// Åä±Û		- °øÁßÆø°Ý¸ðµå
#define BGEAR_SKILL_BASENUM_INVISIBLE			7803030	// Åä±Û		- ÀÎºñÁöºí
#define BGEAR_SKILL_BASENUM_BIG_BOOM			7803040	// ¾×Æ¼ºê	- ºòºÕ

// M-Gear
#define MGEAR_SKILL_BASENUM_SMARTSP				7813020	// ½Ã°£		- ½º¸¶Æ®SP
#define MGEAR_SKILL_BASENUM_CALLOFHERO			7813030	// Å¬¸¯		- ÄÝ¿ÀºêÈ÷¾î·Î
#define MGEAR_SKILL_BASENUM_REVERSEENGINE		7812020	// Åä±Û		- ¸®¹ö½º¿£Áø
#define MGEAR_SKILL_BASENUM_INVICIBLE			7811100	// ¾×Æ¼ºê 	- ¹«Àû
#define MGEAR_SKILL_BASENUM_SCANNING			7813060	// ¹öÇÁ 	- ½ºÄµ

// A-Gear
#define AGEAR_SKILL_BASENUM_SIEGEMODE			7820050	// Åä±Û		- ½ÃÁî¸ðµå
#define AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE	7821040	// Åä±Û		- ½ÃÁîµðÆæ½º¸ðµå
#define AGEAR_SKILL_BASENUM_GROUNDACCELERATOR	7822010	// Åä±Û 	- ±×¶ó¿îµå¿¢¼¿·¹ÀÌÅÍ
#define AGEAR_SKILL_BASENUM_AIRSIEGEMODE    	7820060	// Åä±Û 	- ¿¡¾î½ÃÁî¸ðµå
#define AGEAR_SKILL_BASENUM_CAMOUFLAGE	    	7823040	// Åä±Û 	- À§Àå
#define AGEAR_SKILL_BASENUM_BARIAL		    	7821060	// ½Ã°£  	- º£¸®¾î	// 2013-01-29 by jhseol, º£¸®¾î ½ºÅ³ ÀÌÆåÆ® ¹ö±×

// I-Gear
#define IGEAR_SKILL_BASENUM_BACKMOVEMACH		7832010	// ÄÁÆ®·Ñ	- ¹é¹«ºê¸¶ÇÏ
#define IGEAR_SKILL_BASENUM_TURNAROUND			7832020	// ÄÁÆ®·Ñ	- ÅÏ¾î¶ó¿îµå
#define IGEAR_SKILL_BASENUM_SILENCE				7833040	// ¾×Æ¼ºê	- Ä§¹¬
#define IGEAR_SKILL_BASENUM_FRENZY				7830030	// ¾×Æ¼ºê	- ÇÁ·»Áö
#define IGEAR_SKILL_BASENUM_BERSERKER			7833050	// ¾×Æ¼ºê	- ÆøÁÖ

// 2005-11-22 by cmkwon, ÀÓ½Ã¿ëÀ¸·Î ÇöÀç Å¬¶óÀÌ¾ðÆ®¿¡¼­ »ç¿ë Áß ÀÌ¾î¼­ Á¤ÀÇ ÇÔ, ÇâÈÄ Á¦°ÅÇØ¾ßÇÔ
#define SMART_SPEED								7812020
#define FLASH_ACCELERATOR						7822030

// 2005-11-22 by cmkwon
//// BT
//#define BT_SKILL_QUICKTURN			7802010	// Å¬¸¯Çü
//#define BT_SKILL_REVERSETURN		7802020	// Å¬¸¯Çü
//#define BT_SKILL_COPTERFLIGHT		7802030	// Å¬¸¯Çü
//
//// OT
//#define OT_SKILL_GRENADEMASTERY		7810010	// Áö¼ÓÇü
//#define OT_SKILL_LAUNCHERMASTERY	7810020	// Áö¼ÓÇü
//#define OT_SKILL_DUALSHOT			7810030	// ½Ã°£Çü
//#define OT_SKILL_SCORPING			7810040	// ½Ã°£Çü
//#define OT_SKILL_FRENZY				7810050	// ½Ã°£Çü
//#define OT_SKILL_CONVERGINGSHOT		7810060	// ½Ã°£Çü
//#define OT_SKILL_BUNDLEMASTERY		7810070	// Áö¼ÓÇü
//#define OT_SKILL_OVERRADAR			7810080	// ½Ã°£Çü
//#define OT_SKILL_CRAZYSHOT			7810090	// ½Ã°£Çü
//#define OT_SKILL_EYEFORANEYE		7810100	// ½Ã°£Çü
//#define OT_SKILL_BURSTSHOT			7810110	// ½Ã°£Çü
//#define OT_SKILL_FLYINGWORM			7811010	// ½Ã°£Çü
//#define OT_SKILL_DIFFUSION			7811020	// ½Ã°£Çü
//#define OT_SKILL_PARALLELMOVEMENT	7812030	// Å¬¸¯Çü
//#define OT_SKILL_SCREWPUSHING		7812040	// Å¬¸¯Çü
//#define OT_SKILL_OVERRISING			7812050	// Å¬¸¯Çü
////#define OT_SKILL_NONAME			7812060	// ½Ã°£Çü
//
//// DT
//#define DT_SKILL_CANNONMASTERY		7820010	// Áö¼ÓÇü
//#define DT_SKILL_MASSDRIVEMASTERY	7820020	// Áö¼ÓÇü
//#define DT_SKILL_CHARGINGSHOT		7820030	// ½Ã°£Çü
//#define DT_SKILL_WILDSHOT			7820040	// ½Ã°£Çü
//#define DT_SKILL_WIDERANGE			7820050	// ½Ã°£Çü
//#define DT_SKILL_HAWKEYE			7820060	// ½Ã°£Çü
//#define DT_SKILL_MINEMASTERY		7820070	// Áö¼ÓÇü
//#define DT_SKILL_SPHERETARGET		7820080	// ½Ã°£Çü
//#define DT_SKILL_CARPETBLAST		7820090	// ½Ã°£Çü
//#define DT_SKILL_ASSASSINSCOPE		7820100	// ½Ã°£Çü
//#define DT_SKILL_MINEDISTURBANCE	7820110	// ½Ã°£Çü
//#define DT_SKILL_CAMOUFLAGE			7821010	// ½Ã°£Çü
//#define DT_SKILL_SMOKESCREEN		7821020	// ½Ã°£Çü
//#define DT_SKILL_MIRRORIMAGE		7821030	// ½Ã°£Çü
//#define DT_SKILL_DASH				7822010	// Å¬¸¯Çü
//#define DT_SKILL_AUTODRIVING		7822020	// Å¬¸¯Çü
//#define DT_SKILL_CONCENTRATION		7822030	// ½Ã°£Çü
//#define DT_SKILL_OREGATHERING		7822040	// Å¬¸¯Çü
//#define DT_SKILL_OREFINDING			7822050	// Å¬¸¯Çü
//#define DT_SKILL_ORESEARCH			7822060	// Å¬¸¯Çü
//#define DT_SKILL_OREREFINING		7822070	// Å¬¸¯Çü
//
//// ST
//#define ST_SKILL_VULCANMASTERY		7830010	// Áö¼ÓÇü
//#define ST_SKILL_GATLINGMASTERY		7830020	// Áö¼ÓÇü
//#define ST_SKILL_STRAFESHOT			7830030	// ½Ã°£Çü
//#define ST_SKILL_SNIPESHOT			7830040	// ½Ã°£Çü
//#define ST_SKILL_BURSTSHOT			7830050	// ½Ã°£Çü
//#define ST_SKILL_WIDESHOT			7830060	// ½Ã°£Çü
//#define ST_SKILL_MISSILEMASTERY		7830070	// Áö¼ÓÇü
//#define ST_SKILL_STRAFEBLAST		7830080	// ½Ã°£Çü
//#define ST_SKILL_MULTIPLEBLAST		7830090	// ½Ã°£Çü
//#define ST_SKILL_FRENZYBLAST		7830100	// ½Ã°£Çü
//#define ST_SKILL_WIDEBLAST			7830110	// ½Ã°£Çü
//#define ST_SKILL_BLASTPROTECTION	7831010	// ½Ã°£Çü
//#define ST_SKILL_BLASTREFLECTION	7831020	// ½Ã°£Çü
//#define ST_SKILL_YOYOSPINNING		7832010	// Å¬¸¯Çü
//#define ST_SKILL_OVERBOOST			7832020	// Å¬¸¯Çü
//#define ST_SKILL_COPTERFLIGHT		7832030	// ½Ã°£Çü
//#define ST_SKILL_CONCENTRATION		7832040	// ½Ã°£Çü

// 2005-11-22 by cmkwon
// BT
//#define 	BT_SKILL_AUTOMATICMASTERY		7800010		// Áö¼ÓÇü
//#define 	BT_SKILL_RIFLEMASTERY			7800020		// Áö¼ÓÇü
//#define 	BT_SKILL_STRAFESHOT				7800030		// Áö¼ÓÇü
//#define 	BT_SKILL_SNIPESHOT				7800040		// ½Ã°£Çü
//#define 	BT_SKILL_BURSTSHOT				7800050		// ½Ã°£Çü
//#define 	BT_SKILL_WIDESHOT				7800060		// ½Ã°£Çü
//#define 	BT_SKILL_ROCKETMASTERY			7800070		// Áö¼ÓÇü
//#define 	BT_SKILL_STRAFEBLAST			7800080		// Áö¼ÓÇü
//#define 	BT_SKILL_SNIPEBLAST				7800090		// ½Ã°£Çü
//#define 	BT_SKILL_BURSTBLAST				7800100		// ½Ã°£Çü
//#define 	BT_SKILL_WIDEBLAST				7800200		// ½Ã°£Çü
//#define 	BT_SKILL_DODGE					7801010		// ½Ã°£Çü
//#define 	BT_SKILL_STRATEGICFIELD			7801020		// ½Ã°£Çü
//#define 	BT_SKILL_QUICKTURN				7802010		// Å¬¸¯Çü
//#define 	BT_SKILL_REVERSETURN			7802020		// Å¬¸¯Çü
//#define 	BT_SKILL_COPTERFLIGHT			7802030		// Å¬¸¯Çü
//#define 	BT_SKILL_CONCENTRATION			7802040		// ½Ã°£Çü
//
//// OT
//#define 	OT_SKILL_GRENADEMASTERY			7811010		// Áö¼ÓÇü
//#define 	OT_SKILL_LAUNCHERMASTERY		7811020		// Áö¼ÓÇü
//#define 	OT_SKILL_DUALSHOT				7811030		// ½Ã°£Çü
//#define 	OT_SKILL_SCORPING				7811040		// ½Ã°£Çü
//#define 	OT_SKILL_FRENZY					7811050		// ½Ã°£Çü
//#define 	OT_SKILL_CONVERGINGSHOT	 		7811060		// ½Ã°£Çü
//#define 	OT_SKILL_BUNDLEMASTERY			7811070		// Áö¼ÓÇü
//#define 	OT_SKILL_OVERRADAR				7811080		// ½Ã°£Çü
//#define 	OT_SKILL_CRAZYSHOT	 			7811090		// ½Ã°£Çü
//#define 	OT_SKILL_EYEFORANEYE 			7811100		// ½Ã°£Çü
//#define 	OT_SKILL_BURSTSHOT				7811110		// ½Ã°£Çü
//#define 	OT_SKILL_FLYINGWORM	 			7812010		// ½Ã°£Çü
//#define 	OT_SKILL_DIFFUSION	 			7812020		// Å¬¸¯Çü
//#define 	OT_SKILL_PARALLELMOVEMENT 		7812030		// Å¬¸¯Çü
//#define 	OT_SKILL_SCREWPUSHING 			7812040		// Å¬¸¯Çü
//#define 	OT_SKILL_OVERRISING	 			7812050		// ½Ã°£Çü
////#define 		Æí´ë È¸º¹				7812060
//
//// DT
//#define 	DT_SKILL_CANNONMASTERY			7820010		// Áö¼ÓÇü
//#define 	DT_SKILL_MASSDRIVEMASTERY		7820020		// Áö¼ÓÇü
//#define 	DT_SKILL_CHARGINGSHOT			7820030		// ½Ã°£Çü
//#define 	DT_SKILL_WILDSHOT				7820040		// ½Ã°£Çü
//#define 	DT_SKILL_WIDERANGE				7820050		// ½Ã°£Çü
//#define 	DT_SKILL_HAWKEYE				7820060		// ½Ã°£Çü
//#define 	DT_SKILL_MINEMASTERY			7820070		// Áö¼ÓÇü
//#define 	DT_SKILL_SPHERETARGET			7820080		// ½Ã°£Çü
//#define 	DT_SKILL_CARPETBLAST			7820090		// Å¬¸¯Çü
//#define 	DT_SKILL_ASSASSINSCOPE			7820100		// ½Ã°£Çü
//#define 	DT_SKILL_MINEDISTURBANCE		7820110		// ½Ã°£Çü
//#define 	DT_SKILL_CAMOUFLAGE				7821010		// ½Ã°£Çü
//#define 	DT_SKILL_SMOKESCREEN			7821020		// ½Ã°£Çü
//#define 	DT_SKILL_MIRRORIMAGE			7821030		// ½Ã°£Çü
//#define 	DT_SKILL_DASH					7822010		// Å¬¸¯Çü
//#define 	DT_SKILL_AUTODRIVING			7822020		// ½Ã°£Çü
//#define 	DT_SKILL_CONCENTRATION			7822030		// ½Ã°£Çü
//#define 	DT_SKILL_OREGATHERING			7822040		// Áö¼ÓÇü
//#define 	DT_SKILL_OREFINDING				7822050		// Áö¼ÓÇü
//#define 	DT_SKILL_ORESEARCH				7822060		// ½Ã°£Çü
//#define 	DT_SKILL_OREREFINING			7822070		// Å¬¸¯Çü
//
//// ST
//#define 	ST_SKILL_VULCANMASTERY			7830010		// Áö¼ÓÇü
//#define 	ST_SKILL_GATLINGMASTERY			7830020		// Áö¼ÓÇü
//#define 	ST_SKILL_STRAFESHOT				7830030		// Áö¼ÓÇü
//#define 	ST_SKILL_SNIPESHOT				7830040		// ½Ã°£Çü
//#define 	ST_SKILL_BURSTSHOT				7830050		// ½Ã°£Çü
//#define 	ST_SKILL_WIDESHOT				7830060		// ½Ã°£Çü
//#define 	ST_SKILL_MISSILEMASTERY			7830070		// Áö¼ÓÇü
//#define 	ST_SKILL_STRAFEBLAST			7830080		// Áö¼ÓÇü
//#define 	ST_SKILL_MULTIPLEBLAST			7830090		// ½Ã°£Çü
//#define 	ST_SKILL_FRENZYBLAST			7830100		// ½Ã°£Çü
//#define 	ST_SKILL_WIDEBLAST				7830110		// ½Ã°£Çü
//#define 	ST_SKILL_BLASTPROTECTION		7831010		// ½Ã°£Çü
//#define 	ST_SKILL_BLASTREFLECTION		7831020		// ½Ã°£Çü
//#define 	ST_SKILL_YOYOSPINNING			7832010		// Å¬¸¯Çü
//#define 	ST_SKILL_OVERBOOST				7832020		// ½Ã°£Çü
//#define 	ST_SKILL_COPTERFLIGHT			7832030		// Å¬¸¯Çü
//#define 	ST_SKILL_CONCENTRATION			7832040		// ½Ã°£Çü
////


///////////////////////////////////////////////////////////////////////////////
// Monster Target Type
#define MONSTER_TARGETTYPE_NORMAL			0			// ÀÏ¹Ý Å¸°Ù Å¸ÀÔ ¸ó½ºÅÍ
#define MONSTER_TARGETTYPE_TUTORIAL			1			// Æ©Åä¸®¾ó Å¸°Ù Å¸ÀÔ ¸ó½ºÅÍ	- Æ¯Á¤ À¯Àú¿¡°Ô¸¸ º¸ÀÌ°í ¶³¾îÁö´Â ¾ÆÀÌÅÛµµ Æ¯Á¤ À¯Àú¿¡°Ô¸¸ º¸ÀÎ´Ù
#define MONSTER_TARGETTYPE_MISSION			2			// ¹Ì¼Ç Å¸°Ù Å¸ÀÔ ¸ó½ºÅÍ		- ¹Ì¼ÇÀ» ¹ÞÀº À¯Àúµé¿¡°Ô¸¸ º¸ÀÌ°í ¶³¾îÁö´Â ¾ÆÀÌÅÛµµ Æ¯Á¤ À¯Àúµé¿¡°Ô¸¸ º¸ÀÎ´Ù


///////////////////////////////////////////////////////////////////////////////
// Event-Related Definitions
///////////////////////////////////////////////////////////////////////////////
// Event ±¸Á¶Ã¼
struct EVENTINFO
{
// 2010-04-14 by cmkwon, ¼­¹ö ¸Þ¸ð¸® ºÎÁ· ¹®Á¦ ¼öÁ¤ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤
// 	BYTE		m_EventReceiver;							// EVENT_RECEIVER_XXX
// 	INT			m_TileOrObjectIndex;						// TileIndex or ObjectIndex
// 	D3DXVECTOR3 m_vPosition;								// ¼­¹ö¿¡¼­´Â °­Á¦ ¿öÇÁ½Ã¿¡¸¸ »ç¿ëÇÑ´Ù. ¼­¹ö¼­´Â ·Îµù ½Ã ¹«Á¶°Ç 0À¸·Î ¸¸µç´Ù, ´Ü, ObjectEventÀÎ °æ¿ì´Â ObjectÀÇ Position, by kelovon, 20030713
// 	BYTE		m_bEventType;								// EVENT_TYPE_XXX
// 	short		m_EventwParam1;								// ¿ø·¡ event area index, Building Index, ~0ÀÌ¸é ³ªÁß¿¡ T_FP_EVENT_NOTIFY_WARP_OK ¿¡¼­ m_EventwParam1À» È®ÀÎÇØ¼­ ~0ÀÌ¸é SAFE_DELETE()ÇÑ´Ù
// 	short		m_EventwParam2;								// Ã£¾Æ°¥ event area index, Building Kind, ~0ÀÌ¸é defaultWarpTargetIndex¸¦ »ç¿ëÇÑ´Ù
// 	short		m_EventwParam3;								// ¸Ê ÀÌ¸§
// 	INT			m_NextEventIndex;							// 2005-07-15 by cmkwon, Å¸°Ù event area index
// 	BYTE		m_byObjectMonsterCreated;
// 	BYTE		m_byBossMonster;							// 2006-11-22 by cmkwon, º¯¼ö¸í º¯°æ(m_byIsCityWarMonster->m_byBossMonster) - µµ½ÃÁ¡·ÉÀüÀº ¾ø¾îÁü
// 	INT			m_nObejctMonsterUnitKind;					// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - ¿öÇÁÀÏ °æ¿ì´Â »ç¿ëºÒ°¡ ¼¼·Â ¼³Á¤(ºñÆ®ÇÃ·¡±×)À¸·Î »ç¿ëµÊ, 
// 	DWORD		m_dwLastTimeObjectMonsterCreated;
// 	//	char		m_EventstrParam[SIZE_MAX_EVENT_PARAM_NAME];	// ¸Ê ÀÌ¸§
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-14 by cmkwon, ¼­¹ö ¸Þ¸ð¸® ºÎÁ· ¹®Á¦ ¼öÁ¤ - 
	D3DXVECTOR3 m_vPosition;								// ¼­¹ö¿¡¼­´Â °­Á¦ ¿öÇÁ½Ã¿¡¸¸ »ç¿ëÇÑ´Ù. ¼­¹ö¼­´Â ·Îµù ½Ã ¹«Á¶°Ç 0À¸·Î ¸¸µç´Ù, ´Ü, ObjectEventÀÎ °æ¿ì´Â ObjectÀÇ Position, by kelovon, 20030713
	INT			m_nObejctMonsterUnitKind;					// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - ¿öÇÁÀÏ °æ¿ì´Â »ç¿ëºÒ°¡ ¼¼·Â ¼³Á¤(ºñÆ®ÇÃ·¡±×)À¸·Î »ç¿ëµÊ, 
	INT			m_NextEventIndex;							// 2005-07-15 by cmkwon, Å¸°Ù event area index
	DWORD		m_dwLastTimeObjectMonsterCreated;
	short		m_EventwParam1;								// ¿ø·¡ event area index, Building Index, ~0ÀÌ¸é ³ªÁß¿¡ T_FP_EVENT_NOTIFY_WARP_OK ¿¡¼­ m_EventwParam1À» È®ÀÎÇØ¼­ ~0ÀÌ¸é SAFE_DELETE()ÇÑ´Ù
	short		m_EventwParam2;								// Ã£¾Æ°¥ event area index, Building Kind, ~0ÀÌ¸é defaultWarpTargetIndex¸¦ »ç¿ëÇÑ´Ù
	short		m_EventwParam3;								// ¸Ê ÀÌ¸§
	BYTE		m_EventReceiver;							// EVENT_RECEIVER_XXX
	BYTE		m_bEventType;								// EVENT_TYPE_XXX
	BYTE		m_byObjectMonsterCreated;
	BYTE		m_byBossMonster;							// 2006-11-22 by cmkwon, º¯¼ö¸í º¯°æ(m_byIsCityWarMonster->m_byBossMonster) - µµ½ÃÁ¡·ÉÀüÀº ¾ø¾îÁü

	EVENTINFO()
	{
		ResetEVENTINFO();
	}
	void ResetEVENTINFO(void)
	{
		m_EventReceiver						= 0;
// 2010-04-14 by cmkwon, ¼­¹ö ¸Þ¸ð¸® ºÎÁ· ¹®Á¦ ¼öÁ¤ - ÇÊ¿ä ¾ø¾î¼­ Á¦°ÅµÊ
//		m_TileOrObjectIndex					= 0;
		m_vPosition							= D3DXVECTOR3(0,0,0);
		m_bEventType						= 0;
		m_EventwParam1						= 0;
		m_EventwParam2						= 0;
		m_EventwParam3						= 0;
		m_NextEventIndex					= 0;
		m_byObjectMonsterCreated			= FALSE;
		m_byBossMonster						= FALSE;		// 2006-11-22 by cmkwon, º¯¼ö¸í º¯°æ(m_byIsCityWarMonster->m_byBossMonster) - µµ½ÃÁ¡·ÉÀüÀº ¾ø¾îÁü
		m_nObejctMonsterUnitKind			= 0;
		m_dwLastTimeObjectMonsterCreated	= 0;
	}


	void * operator new(size_t size);
	void operator delete(void* p);
};

#define EVENT_RECEIVER_NONE		(BYTE)0		// ±×³É ÀÌº¥Æ®
#define	EVENT_RECEIVER_TILE		(BYTE)1		// TILE¿¡ ÀÇÇÑ ÀÌº¥Æ®
#define	EVENT_RECEIVER_OBJECT	(BYTE)2		// OBJECT¿¡ ÀÇÇÑ ÀÌº¥Æ®

// PK Type
#define PK_TYPE_PK						0x00
#define PK_TYPE_COMBAT					0x01
#define PK_TYPE_SAFE					0x02

// Event Type
typedef BYTE EventType_t;

#define EVENT_TYPE_NO_OBJECT_MONSTER		(EventType_t)0	// 
#define EVENT_TYPE_NOEVENT					(EventType_t)0
#define EVENT_TYPE_WARP						(EventType_t)1
#define EVENT_TYPE_WARP_TARGET				(EventType_t)2
#define EVENT_TYPE_ENTER_BUILDING			(EventType_t)3		// »óÁ¡¿µ¿ª ÀÌº¥Æ® - EventParam1(EventAreaIndex), EventParam2(ÀÌ·ú½Ã ÀÌµ¿ ÇÒ ÀÌ·ú½ÃÀÛ ¿ÀºêÁ§Æ® EventAreaIndex)
#define EVENT_TYPE_LANDING					(EventType_t)4		// Âø·ú Object
#define EVENT_TYPE_RACING_CHECK_POTINT		(EventType_t)5		// ·¹ÀÌ½Ì Ã¼Å© Æ÷ÀÎÆ® Object
#define EVENT_TYPE_OBJECT_MONSTER			(EventType_t)6		// ObjectMonster Position Information Object
//#define EVENT_TYPE_OBJECT_MONSTER_STOP		(EventType_t)6	// Á¤ÁöÇü, ObjectMonster Position Object
//#define EVENT_TYPE_OBJECT_MONSTER_ROTATE	(EventType_t)7		// Å¸°Ùº¤ÅÍ°¡ º¯ÇÏ´Â ÇüÅÂ, ObjectMonster Position Object
#define EVENT_TYPE_PATTERN_POINT			(EventType_t)8		// Ä³¸¯ÅÍ ¿öÇÁ½Ã ÆÐÅÏÀÇ ½ÃÀÛ/³¡ ¿ÀºêÁ§Æ®
#define EVENT_TYPE_OBJ_BUILDING_NPC			(EventType_t)9		// 2005-07-21 by cmkwon, »óÁ¡ NPC ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex), NextEventIndex(NPCIndex)
#define EVENT_TYPE_LANDING_PATTERN_START	(EventType_t)10		// 2005-07-14 by cmkwon, ±â¾î¸ðµå¿¡¼­ Âø·ú ÆÐÅÏÀÇ ½ÃÀÛ ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex), NextEventIndex(Âø·ú ¸ñÇ¥ ¿ÀºêÁ§Æ® EventAreaIndex)
#define EVENT_TYPE_LANDING_PATTERN_END		(EventType_t)11		// 2005-07-14 by cmkwon, ±â¾î¸ðµå¿¡¼­ Âø·ú ÆÐÅÏÀÇ Á¾·á ¿ÀºêÁ§Æ®, ¿ÀºêÁ§Æ®¸¦ ¸¸³ª¸é Âø·ú »óÅÂ°¡ µÈ´Ù. - EventParam1(EventAreaIndex), NextEventIndex(Âø·ú ÈÄ ÀÌµ¿ ÇÒ ¸ñÇ¥ ¿ÀºêÁ§Æ® EventAreaIndex)
#define EVENT_TYPE_TAKEOFF_PATTERN_START	(EventType_t)12		// 2005-07-14 by cmkwon, ±â¾î¸ðµå¿¡¼­ ÀÌ·ú ÆÐÅÏÀÇ ½ÃÀÛ ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex), NextEventIndex(ÀÌ·ú ¸ñÇ¥ ¿ÀºêÁ§Æ® EventAreaIndex)
#define EVENT_TYPE_TAKEOFF_PATTERN_END		(EventType_t)13		// 2005-07-14 by cmkwon, ±â¾î¸ðµå¿¡¼­ ÀÌ·ú ÆÐÅÏÀÇ Á¾·á ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex)
#define EVENT_TYPE_OBJ_ENTER_BUILDING		(EventType_t)14		// 2005-07-22 by cmkwon, ±â¾î°¡ Âø·ú ÈÄ  »óÁ¡À¸·Î ÀÌµ¿ÇÒ ÁÂÇ¥ ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex)
#define EVENT_TYPE_OBJ_QUEST_OBJECT			(EventType_t)15		// 2005-08-17 by cmkwon, Äù½ºÆ®¿¡¼­ ÀÌ¿ëµÇ´Â Ãæµ¹ ¿ÀºêÁ§Æ® - EventParam1(EventAreaIndex)
#define EVENT_TYPE_CHARACTERMODE_WARP			(EventType_t)21		// 2006-07-14 by cmkwon, Ä³¸¯ÅÍ¸ðµå¿¡¼­ ¿öÇÁ ¼Ò½º ¿ÀºêÁ§Æ®
#define EVENT_TYPE_CHARACTERMODE_WARP_TARGET	(EventType_t)22		// 2006-07-14 by cmkwon, Ä³¸¯ÅÍ¸ðµå¿¡¼­ ¿öÇÁ Å¸°Ù ¿ÀºêÁ§Æ®
#define EVENT_TYPE_ENTER_BUILDING_BAZAAR		(EventType_t)23		// 2006-07-19 by cmkwon, °³ÀÎ»óÁ¡ ¿µ¿ª
#define EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP	(EventType_t)24		// 2007-12-14 by cmkwon, Ä³¸¯ÅÍ¸ðµå¿¡¼­ ¹Ù·Î ¿öÇÁ ½Ã½ºÅÛ ±¸Çö - ÀÌ ¿ÀºêÁ§Æ®¿Í Ãæµ¹½Ã ¹Ù·Î ¿öÇÁµÊ

#define EVENT_TYPE_INFI_CINEMA				(EventType_t)30		// 2010. 05. 27 by jskim ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö
#define EVENT_TYPE_PARAM_INFI_CINEMA		701					// 2010. 05. 27 by jskim ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö

#define EVENT_TYPE_INFI_CINEMA_THIRD				(EventType_t)31		// 2011-06-23 by jhahn ÀÎÇÇ3Â÷ ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö
#define EVENT_TYPE_PARAM_INFITHIRD_CINEMA_FAIL			702						// 2011-06-23 by jhahn ÀÎÇÇ3Â÷ ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö
#define EVENT_TYPE_PARAM_INFITHIRD_CINEMA_SUCCESS		703						// 2011-06-23 by jhahn ÀÎÇÇ3Â÷ ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö

#define EVENT_TYPE_TUTORIAL_GATE_POINT		(EventType_t)100	// 

#define EVENT_TYPE2_TRIGGERWARP			    (EventType_t)11 

extern char *Get_EVENT_TYPE_String(EventType_t i_evType);

// Event Result
typedef BYTE EventResult_t;

//#define EVENT_RESULT_ERROR				0x00	// check: ºÒÇÊ¿äÇÏ´Ù°í ÆÇ´Ü, EVENT_RESULT_CLOSE_CONNECTION·Î ÀüÈ¯ÇÔ, ÇÊ¿äÇÏ¸é »ì¸², 20030812, kelovon
#define EVENT_RESULT_BREAK				0x01	// ÀÌº¥Æ®°¡ Ã³¸®µÇÁö ¾Ê¾ÒÀ½
#define EVENT_RESULT_CONTINUE			0x02	// ÀÌº¥Æ®°¡ ¼º°øÀûÀ¸·Î Ã³¸®µÊ, °è¼Ó ÁøÇàÇÏ¸é µÊ
#define EVENT_RESULT_CLOSE_CONNECTION	0x03	// ½É°¢ÇÑ ¿¡·¯ ¹ß»ý, ¿¬°á Á¾·á ¿ä±¸

///////////////////////////////////////////////////////////////////////////////
// Variable Type Definitions
///////////////////////////////////////////////////////////////////////////////
typedef USHORT ClientIndex_t;
typedef vector<ClientIndex_t>		vectorClientIndex;
typedef set<ClientIndex_t>			setClientIndex_t;		// 2009-12-11 by cmkwon, µ¥¹ÌÁö ¾î±×·Î·Î Å¸°ÙÀ» º¯°æÇÏ´Â ¸ó½ºÅÍ ±¸Çö - 

// MapIndex ¹× ChannelIndex °ü¸®¿ë
struct MAP_CHANNEL_INDEX
{
	// member variables
	MapIndex_t		MapIndex;
	ChannelIndex_t	ChannelIndex;

	// overloaded operators
	inline bool operator==(MAP_CHANNEL_INDEX &rhs)
	{
		return (this->MapIndex == rhs.MapIndex && this->ChannelIndex == rhs.ChannelIndex);
	}

	inline bool operator!=(MAP_CHANNEL_INDEX &rhs)
	{
		return (this->MapIndex != rhs.MapIndex || this->ChannelIndex != rhs.ChannelIndex);
	}
	inline bool IsSameMapChannelIndex(const MAP_CHANNEL_INDEX &rhs)
	{
		return (this->MapIndex == rhs.MapIndex && this->ChannelIndex == rhs.ChannelIndex);
	}

	// functions
	MAP_CHANNEL_INDEX() : MapIndex(0), ChannelIndex(0) {}
	MAP_CHANNEL_INDEX(MapIndex_t mapIdx, ChannelIndex_t channIdx) : MapIndex(mapIdx), ChannelIndex(channIdx) {}
	inline BOOL IsValid() { return (MapIndex != 0); }
	inline void Invalidate() { MapIndex = 0; }
	inline void Validate(MapIndex_t i_MapIndex, ChannelIndex_t i_ChannelIndex)
	{
		MapIndex = i_MapIndex; ChannelIndex = i_ChannelIndex;
	}
};

struct GEAR_STAT
{
	Stat_t	AttackPart;		// °ø°ÝÆÄÆ®
	Stat_t	DefensePart;	// ¹æ¾îÆÄÆ®
	Stat_t	FuelPart;		// ¿¬·áÆÄÆ®
	Stat_t	SoulPart;		// °¨ÀÀÆÄÆ®
	Stat_t	ShieldPart;		// ½¯µåÆÄÆ®
	Stat_t	DodgePart;		// È¸ÇÇÆÄÆ®, 0 ~ 255ÀÇ È®·ü
};


// 2012-04-12 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ¾Æ·¹³ª PlayCount Á¤º¸ ±¸Á¶Ã¼
struct SArenaPlayInfo
{
	int				nPlayCount;									// ³²Àº ÇÃ·¹ÀÌ Ä«¿îÆ®
	ATUM_DATE_TIME	atimeLastPlayTime;							// ¸¶Áö¸·À¸·Î ÇÃ·¹ÀÌ ÇÑ ½Ã°£
	ATUM_DATE_TIME	atimeResetTime;								// ¾Æ·¹³ª ÀÔÀåÈ½¼ö ÃÊ±âÈ­ ½Ã°£
};
// end 2012-04-12 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ¾Æ·¹³ª PlayCount Á¤º¸ ±¸Á¶Ã¼

///////////////////////////////////////////////////////////////////////////////
//	ATUM - Character - Parameter Struct Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////
struct CHARACTER_DB_EX;

struct CHARACTER
{
	ClientIndex_t		ClientIndex;
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];			// »ç¿ëÀÚ ¾ÆÀÌµð
	UID32_t				AccountUniqueNumber;						//  °èÁ¤ °íÀ¯ ¹øÈ£
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];		// À¯´Ö(Ä³¸¯ÅÍ) ÀÌ¸§
	UID32_t				CharacterUniqueNumber;						// Ä³¸¯ÅÍ °íÀ¯¹øÈ£
	BYTE				Gender;							// false(0) : ¿©, true(1) : ³²
	BYTE				PilotFace;						// È­¸é¿¡ ³ªÅ¸³ª´Â ÀÎ¹° ÄÉ¸¯ÅÍ
	BYTE				CharacterMode;					// 2005-07-13 by cmkwon, ÇöÀç Ä³¸¯ÅÍ »óÅÂ ÇÃ·¡±×
	USHORT				Race;							// Á¾Á·
	USHORT				UnitKind;						// À¯´ÖÀÇ Á¾·ù
	BYTE				InfluenceType;					// ¼¼·Â Å¸ÀÔÀ¸·Î
	BYTE				SelectableInfluenceMask;		// 2005-12-07 by cmkwon, ¼¼·Â¼±ÅÃ½Ã ¼±ÅÃ°¡´ÉÇÑ ¼¼·ÂMask, ÇÑ°èÁ¤¿¡´Â ÇÏ³ªÀÇ ¼¼·Â¸¸ ¼±ÅÃ °¡´ÉÇÔ
	BYTE				AutoStatType;					// ÀÚµ¿ ºÐ¹è ½ºÅÈ Å¸ÀÔ
	GEAR_STAT			GearStat;						// ±â¾î ½ºÅÈ
	GEAR_STAT			TotalGearStat;					// ÅëÇÕ ±â¾î ½ºÅÈ - ÄÄÇ»ÅÍ ¾ÆÀÌÅÛ ½ºÅÈ Æ÷ÇÔ
	char				GuildName[SIZE_MAX_GUILD_NAME];	// ±æµå ÀÌ¸§
	UID32_t				GuildUniqueNumber;				// ±æµå ¹øÈ£, 0ÀÌ¸é ±æµå ¾øÀ½
	BYTE				Level;							//
	Experience_t		Experience;						//
	Experience_t		DownExperience;					// Ä³¸¯ÀÌ Á×À»¶§ ¶³¾îÁø °æÇèÄ¡
	INT					DownSPIOnDeath;					// 2006-04-10 by cmkwon, Ä³¸¯ÀÌ Á×À»¶§ ¶³¾îÁø SPI
	BodyCond_t			BodyCondition;					// »óÅÂ, bit flag »ç¿ë
	INT					Propensity;						// ¸í¼ºÄ¡, ¼ºÇâ(¼±, ¾Ç)
	BYTE				Status;							// ½ÅºÐ
	USHORT				PKWinPoint;						// PK ½Â¸® ¼öÄ¡
	USHORT				PKLossPoint;					// PK ÆÐ¹è ¼öÄ¡
	USHORT				Material;						// ÀçÁú(HI : Main, LOW : Sub)
	SHORT				HP;								// Health Point
	float				CurrentHP;						// Current Health Point
	SHORT				DP;								// ½¯µå, Defense Point
	float				CurrentDP;						// ÇöÀç ½¯µå, Current Defense Point
	SHORT				SP;								// Skill Point
	SHORT				CurrentSP;						//
	SHORT				EP;								// Fuel
	float				CurrentEP;						// Current Fuel
	char				PetName[SIZE_MAX_PET_NAME];
	BYTE				PetLevel;
	Experience_t		PetExperience;
	MAP_CHANNEL_INDEX	MapChannelIndex;				// Ä³¸¯ÅÍ°¡ ¼ÓÇÑ ¸Ê ¹× Ã¤³Î
	D3DXVECTOR3			PositionVector;					// Ä³¸¯ÅÍ ÁÂÇ¥
	D3DXVECTOR3			TargetVector;					//
	D3DXVECTOR3			UpVector;						//
	BYTE				MaxLevel;
	BYTE				BonusStat;						// Ãß°¡ 2002.12.13
// 2005-11-15 by cmkwon, »èÁ¦ÇÔ
//	BYTE				BonusSkillPoint;				// Ãß°¡ 2002.12.13
	BYTE				BonusStatPoint;					// 2005-11-15 by cmkwon, ·¹º§¾÷ÀÌ ¾Æ´Ñ ´Ù¸¥¹æ¹ýÀ¸·Î ¹ÞÀº º¸³Ê½º ½ºÅÈ Æ÷ÀÎÆ®, BonusSkillPoint¸¦ º¯°æÇÔ // Ãß°¡ 2002.12.13
	PartyID_t			LastPartyID;					// ¸¶Áö¸· ÆÄÆ¼ ID, ºñÁ¤»óÀûÀ¸·Î Á¾·áµÇ¾úÀ» ¶§ ÆÄÆ¼ À¯Áö¸¦ À§ÇØ ¾²ÀÓ
	INT					RacingPoint;					// 2009-11-02 by cmkwon, Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) ¾ÆÀÌÅÛ Ãß°¡ ±¸Çö - ÇÏÀ§Ã¹¹øÂ°1Byte:ÀÎº¥Ãß°¡°³¼ö, ÇÏÀ§µÎ¹øÂ°1Byte:Ã¢°íÃß°¡°³¼ö, ÇÏÀ§¼¼¹øÂ°/³×¹øÂ°2Byte´Â »ç¿ëÇÏÁö ¾ÊÀ½, // Racing °á°ú Point
	LONGLONG			TotalPlayTime;					// ÃÊ´ÜÀ§
	ATUM_DATE_TIME		CreatedTime;					// Ä³¸¯ÅÍ »ý¼º ½Ã°£
	ATUM_DATE_TIME		LastStartedTime;				// ÃÖÁ¾ °ÔÀÓ ½ÃÀÛ ½Ã°£
	ATUM_DATE_TIME		LevelUpTime;					// 2006-12-18 by dhjin, ·¹º§¾÷ ½Ã°£
	INT					WarPoint;						// 2007-04-17 by dhjin, WP
	INT					CumulativeWarPoint;				// 2007-05-28 by dhjin, ´©ÀûWP
	INT					ArenaWin;						// 2007-06-07 by dhjin, ¾Æ·¹³ª ½ÂÆÐ ÀüÀû ½Â
	INT					ArenaLose;						// 2007-06-07 by dhjin, ¾Æ·¹³ª ½ÂÆÐ ÀüÀû ÆÐ
	INT					ArenaDisConnect;				// 2007-06-07 by dhjin, ¾Æ·¹³ª °­Á¦ Á¾·á
	LONGLONG			PCBangTotalPlayTime;			// 2007-06-07 by dhjin, PC¹æ ÃÑ ÇÃ·¹ÀÌ ½Ã°£, ÃÊ´ÜÀ§
	INT					SecretInfoOption;				// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç -
	char				NickName[SIZE_MAX_CHARACTER_NAME];	// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - ¿ùµå ·©Å·À» À§ÇÑ NickName
	SArenaPlayInfo		ArenaPlayInfo;					// 2012-04-12 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - º¸»ó : ¾Æ·¹³ª ÇÃ·¹ÀÌ Á¤º¸

	// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡
 	BYTE				bUsingReturnItem;
  	CHARACTER()
 	{
 		bUsingReturnItem = 0;
	}
	// end 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡

	// operator overloading
	CHARACTER& operator=(const CHARACTER_DB_EX& rhs);
// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - 
// 	BYTE GetInfluenceMask(void)
// 	{
// 		if(COMPARE_INFLUENCE(InfluenceType, INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN))
// 		{
// 			return INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN;
// 		}
// 		return InfluenceType;
// 	}
	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - 
	BYTE GetInfluenceMask(void)
	{
		if(IS_NORMAL_INFLUENCE_TYPE(InfluenceType))
		{
			if(IS_ANI_CITY_MAP_INDEX(Material))
			{
				return INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_ANI;
			}
			return INFLUENCE_TYPE_NORMAL|INFLUENCE_TYPE_VCN;
		}

		return InfluenceType|INFLUENCE_TYPE_NORMAL;
	};
	///////////////////////////////////////////////////////////////////////////////
	/// \fn			
	/// \brief		// 2009-10-12 by cmkwon, ÇÁ¸®½ºÄ« Á¦°Å ¹æ¾È Àû¿ë - 
	/// \author		cmkwon
	/// \date		2009-10-12 ~ 2009-10-12
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	USHORT GetStartCityMapIndex(void)
	{
		switch(InfluenceType)
		{
		case INFLUENCE_TYPE_NORMAL:
			{
				if(IS_ANI_CITY_MAP_INDEX(Material))
				{
					return Material;
				}
				return VCN_CITY_MAP_INDEX;	// 2009-10-12 by cmkwon, ¼³Á¤µÇÁö ¾ÊÀº À¯Àú ±âº»
			}
		case INFLUENCE_TYPE_VCN:				return VCN_CITY_MAP_INDEX;
		case INFLUENCE_TYPE_ANI:				return ANI_CITY_MAP_INDEX;
		}

		return VCN_CITY_MAP_INDEX;		// 2009-10-12 by cmkwon, ±âº»¸Ê
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) ¾ÆÀÌÅÛ Ãß°¡ ±¸Çö - 
	BYTE GetAddedPermanentInventoryCount(BYTE i_enStorage=ITEM_IN_CHARACTER);
	BOOL SetAddedPermanentInventoryCount(BYTE i_byAddCount, BYTE i_enStorage=ITEM_IN_CHARACTER);
};

// °ø°Ý Å¸ÀÔ: C(Ä³¸¯ÅÍ), M(¸ó½ºÅÍ), I(ÇÊµå¾ÆÀÌÅÛ), CI(Ä³¸¯ÅÍ¿¡ Á¾¼ÓµÈ ¾ÆÀÌÅÛ)
typedef enum
{
	ERR2ERR	= 0,
	C2C		= 1,
	C2M,
	C2I,
	C2CI,
	C2NULL,			// NULL Target
	M2C,
	M2M,			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - Ãß°¡
	M2I,
	M2CI,
	M2NULL,			// NULL Target
	NULL2NULL		// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±, [ÇØ´çÇÏ´Â º¯¼ö¸¦ ³Ö¾îÁÖ¼¼¿ä]
} enumAttackToTarget;

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
struct INVOKING_WEAR_ITEM_DESTPARAM {
	InvokingDestParamID_t	InvokingDestParamID;
	DestParam_t				InvokingDestParam;
	ParamValue_t			InvokingDestParamValue;
	EffectIdx_t				InvokingEffectIdx;
};
typedef vector<INVOKING_WEAR_ITEM_DESTPARAM>		InvokingWearItemDestParamList;
typedef ez_map<InvokingDestParamID_t, InvokingWearItemDestParamList>	ezmapInvokingDPIdDPList;

struct MEX_ITEM_INFO;

///////////////////////////////////////////////////////////////////////////////
//	ATUM - Item - Parameter Struct Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////
struct ITEM
{
	INT			ItemNum;						// ¾ÆÀÌÅÛ °íÀ¯¹øÈ£, ÀåÂø ¾ÆÀÌÅÛÀÏ ¶§ (ITEM_BASE*)
	BYTE		Kind;							// ¾ÆÀÌÅÛ Á¾·ù(±â°üÆ÷, ºö, ·ÎÄÏ, ½ºÅ³.....), ITEMKIND_XXX
	char		ItemName[SIZE_MAX_ITEM_NAME];	// ¾ÆÀÌÅÛ ÀÌ¸§
	float		AbilityMin;						// ¾ÆÀÌÅÛÃÖ¼Ò¼º´É
	float		AbilityMax;						// ¾ÆÀÌÅÛÃÖ´ë¼º´É
	USHORT		ReqRace;						// ÇÊ¿äÁ¾Á·
	GEAR_STAT	ReqGearStat;					// ÇÊ¿ä ±â¾î ½ºÅÈ
	USHORT		ReqUnitKind;					// ÇÊ¿äÀ¯´ÖÁ¾·ù
	BYTE		ReqMinLevel;					// ÇÊ¿ä ÃÖÀú ·¹º§
	BYTE		ReqMaxLevel;					// ÇÊ¿ä ÃÖÀú ·¹º§
	BYTE		ReqItemKind;					// ÇÊ¿ä¾ÆÀÌÅÛÁ¾·ù, ITEMKIND_XXX, check: ½ºÅ³¿¡¸¸ »ç¿ë, 20040818, kelovon
	USHORT		Weight;							// ¹«°Ô
	float		HitRate;						// ¸íÁßÈ®·ü(0~255)	// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
	BYTE		Defense;						// ¹æ¾î·Â
	float		FractionResistance;				// 2008-10-06 by dhjin, ÇÇ¾î½ºÀ²·Î ÀÏ´Ü »ç¿ë // ¼Ó¼ºÀúÇ×·Â(0~255) // 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
	BYTE		NaturalFaction;					// ÃµÀû°è¿­, Á¾Á·(¸ó½ºÅÍ, Ä³¸¯ÅÍ) Index (ÃµÀû)
	BYTE		SpeedPenalty;					// ½ºÇÇµåÆä³ÎÆ¼, ÀÌµ¿¼Óµµ¿¡¹ÌÄ¡´Â ¿µÇâ(-:°¨¼Ò)
	USHORT		Range;							// °ø°Ý¹üÀ§, ¿£Áø·ùÀÎ °æ¿ì¿¡´Â ºÎ½ºÅÍ °¡µ¿ ½Ã ¼Óµµ, ½ºÅ³ÀÇ °æ¿ì Àû¿ë ¹üÀ§
	BYTE		Position;						// ÀåÂøÀ§Ä¡
	BYTE		Scarcity;						// Èñ±Í¼º, °ÔÀÓ»ó¿¡ ³ª¿Ã È®·ü, defineÇØ¼­ »ç¿ë, see below
	float		Endurance;						// ³»±¸¼º, ³»±¸µµ
	Prob256_t	AbrasionRate;					// ¸¶¸ðÀ², ³»±¸µµ°¡ ÁÙ¾îµå´Â ´ÜÀ§·®(0~255)
	USHORT		Charging;						// ¹«±â·ù¿¡¼­´Â ÃÖ´ë ÀåÅº ¼ö, ¿¡³ÊÁö´Â ÇÑ¹ø¿¡ Àû¿ëµÇ´Â °³¼ö, TANK·ù´Â ¾ç
	BYTE		Luck;							// Çà¿î
	USHORT		MinTradeQuantity;				// ÃÖ¼Ò °Å·¡ ¼ö·®, Price´Â ÀÌ ¼ö·®¿¡ ´ëÇÑ °¡°ÝÀÌ´Ù

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	UINT		SellingPrice;
#else
	UINT		Price;							// ÃÖ¼Ò °Å·¡ ¼ö·®ÀÇ °¡°Ý
	UINT		CashPrice;						// ÃÖ¼Ò °Å·¡ ¼ö·®ÀÇ Çö±Ý °¡°Ý
#endif

// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - ¾Æ·¡¿Í °°ÀÌ ¹è¿­·Î ¼öÁ¤ ÇÔ.
// 	BYTE		DestParameter1;					// ´ë»óÆÄ¶ó¹ÌÅÍ1
// 	float		ParameterValue1;				// ¼öÁ¤ÆÄ¶ó¹ÌÅÍ1
// 	BYTE		DestParameter2;					// ´ë»óÆÄ¶ó¹ÌÅÍ2
// 	float		ParameterValue2;				// ¼öÁ¤ÆÄ¶ó¹ÌÅÍ2
// 	BYTE		DestParameter3;					// ´ë»óÆÄ¶ó¹ÌÅÍ3
// 	float		ParameterValue3;				// ¼öÁ¤ÆÄ¶ó¹ÌÅÍ3
// 	BYTE		DestParameter4;					// ´ë»óÆÄ¶ó¹ÌÅÍ4
// 	float		ParameterValue4;				// ¼öÁ¤ÆÄ¶ó¹ÌÅÍ4
	DestParam_t	ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø) - // 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â -
	float		ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 

	UINT		ReAttacktime;					// Àç °ø°Ý½Ã°£(ms)
	INT			Time;							// Áö¼Ó ½Ã°£(½ºÅ³·ù µî)
	USHORT		RepeatTime;						// ¹«±â·ù¿¡¼­´Â ³²Àº ÃÑ¾Ë ¼ö·Î »ç¿ë, ³ª¸ÓÁö´Â °³¼ö, ½Ã°£Çü ½ºÅ³·ù¿¡¼± ³²Àº ½Ã°£, ³ª¸ÓÁö ½ºÅ³Àº »ç¿ë ¿©ºÎ
	USHORT		Material;						// ÀçÁú
	USHORT		ReqMaterial;					// ÇÊ¿äÇÑ ÀçÁú ¼ö(Á¦ÀÛ,¼ö¸®½Ã ÇÊ¿ä)
	float		RangeAngle;						// ¹üÀ§°¢µµ(0 ~ PI), È­¸Á
	BYTE		UpgradeNum;						// ¾÷±×·¹ÀÌµå ¼ö, ¾÷±×·¹ÀÌµåÀÇ ÇÑ°è¸¦ ³ªÅ¸³¿.
	INT			LinkItem;						// ¸µÅ©¾ÆÀÌÅÛ, ¾ÆÀÌÅÛ°ú ¿¬°üµÈ ¾ÆÀÌÅÛ(ÃÑ¾Ë)
	BYTE		MultiTarget;					// µ¿½Ã¿¡ ÀâÀ» ¼ö ÀÖ´Â Å¸°ÙÀÇ ¼ö
	USHORT		ExplosionRange;					// Æø¹ß¹Ý°æ(Æø¹ß ½Ã µ¥¹ÌÁöÀÇ ¿µÇâÀÌ ¹ÌÄ¡´Â ¹Ý°æ)
	USHORT		ReactionRange;					// ¹ÝÀÀ¹Ý°æ(¸¶ÀÎ µîÀÌ ¹ÝÀÀÇÏ´Â ¹Ý°æ)
	BYTE		ShotNum;						// Á¡»ç ¼ö,	Á¡»ç ½Ã ¹ß»ç ¼ö¸¦ ³ªÅ¸³½´Ù.
	BYTE		MultiNum;						// µ¿½Ã ¹ß»ç Åº ¼ö,	1¹ø ¹ß»ç¿¡ ¸î¹ßÀÌ µ¿½Ã¿¡ ³ª°¡´À³Ä
	USHORT		AttackTime;						// °ø°Ý½Ã°£, °ø°ÝÀ» ÇÏ±â À§ÇØ ÇÊ¿äÇÑ ½Ã°£
	BYTE		ReqSP;							// SP ¼Ò¸ð·®(½ºÅ³)
	INT			SummonMonster;					// 2006-06-08 by cmkwon, À¯·áÈ­ »óÁ¡ÀÇ ÅÇ±¸ºÐÀÚ·Î »ç¿ëÇÑ´Ù.(CASH_ITEMKIND_XXXX)
	INT			NextSkill;						// ´ÙÀ½ ´Ü°èÀÇ ½ºÅ³ ¾ÆÀÌÅÛ ³Ñ¹ö(½ºÅ³)
	BYTE		SkillLevel;						// ·¹º§
	Prob256_t	SkillHitRate;					// ½ºÅ³¸íÁßÈ®·ü(0~255)
	BYTE		SkillType;						// ½ºÅ³ÇüÅÂ(½Ã°£ ¹× ¹ßµ¿ °ü·Ã), Áö¼Ó|Å¬¸¯|½Ã°£|À¯Áö
	BYTE		SkillTargetType;				// ½ºÅ³ Å¸ÄÏ Å¸ÀÔ, SKILLTARGETTYPE_XXX
	BYTE		Caliber;						// ±¸°æ(ÃÑ¾Ë, ÅºµÎ µî)
	BYTE		OrbitType;						// ¹Ì»çÀÏ, ·ÎÄÏ µîÀÇ ±ËÀû
	BitFlag64_t	ItemAttribute;					// ¾ÆÀÌÅÛÀÇ ¼Ó¼º, ITEM_ATTR_XXX
	FLOAT		BoosterAngle;					// ºÎ½ºÅÍ½Ã¿¡ À¯´ÖÀÇ È¸Àü°¢, ÇöÀç´Â ¿£Áø¿¡¸¸ »ç¿ë
	INT			CameraPattern;					// Ä«¸Þ¶ó ÆÐÅÏ
	INT			SourceIndex;					// 2005-08-22 by cmkwon, ÀÌÆåÆ®, ¾ÆÀÌÄÜ(ºò/½º¸ô) ¸®¼Ò½º µ¥ÀÌÅ¸
	vectINT *	pParamOverlapIdxList;			// 2010-01-18 by cmkwon, ¾ÆÀÌÅÛ »ç¿ë½Ã Parameter Áßº¹ Ã¼Å© ½Ã½ºÅÛ ±¸Çö - 
	char		Description[SIZE_MAX_ITEM_DESCRIPTION];	// ¾ÆÀÌÅÛ ¼³¸í
	BYTE		EnchantCheckDestParam;			// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	InvokingDestParamID_t	InvokingDestParamID;	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	InvokingDestParamID_t	InvokingDestParamIDByUse;// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ	
	InvokingWearItemDestParamList *	pInvokingDestParamList;			// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	InvokingWearItemDestParamList *	pInvokingDestParamByUseList;	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ	
	BYTE		IsTenderDropItem;				// 2010-04-09 by cmkwon, ÀÎÇÇ2Â÷ Ãß°¡ ¼öÁ¤(´Ü°èº° º¸»ó Ãß°¡) - CFieldIOCP::SetTenderItemList#¿¡¼­ ¼³Á¤µÊ

	// operator overloading
	ITEM& operator=(const MEX_ITEM_INFO& rhs);

	BOOL IsExistDesParam(DestParam_t desParam)		// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø)
	{
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤
// 		if(desParam == DestParameter1
// 			|| desParam == DestParameter2
// 			|| desParam == DestParameter3
// 			|| desParam == DestParameter4)
// 		{
// 			return TRUE;
// 		}
		// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 
		for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
		{
			if(desParam == ArrDestParameter[i])
			{
				return TRUE;
			}
		}

		return FALSE;
	};
	float GetParameterValue(DestParam_t i_destParam)		// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø)
	{
// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¡ÇÔ
// 		if(i_destParam == DestParameter1)
// 		{
// 			return ParameterValue1;
// 		}
// 		if(i_destParam == DestParameter2)
// 		{
// 			return ParameterValue2;
// 		}
// 		if(i_destParam == DestParameter3)
// 		{
// 			return ParameterValue3;
// 		}
// 		if(i_destParam == DestParameter4)
// 		{
// 			return ParameterValue4;
// 		}
		// 2009-04-21 by cmkwon, ITEM¿¡ DesParam ÇÊµå °³¼ö 8°³·Î ´Ã¸®±â - 
		for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
		{
			if(i_destParam == ArrDestParameter[i])
			{
				return ArrParameterValue[i];
			}
		}
		return 0.0f;
	};

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, µ¥¹ÌÁö ¾î±×·Î·Î Å¸°ÙÀ» º¯°æÇÏ´Â ¸ó½ºÅÍ ±¸Çö - 
	float GetSkillDamageForAggro(void)
	{
		if(FALSE == IS_SKILL_ITEM(Kind))
		{
			return 0.0f;
		}

		return AbilityMax;		
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, ¾ÆÀÌÅÛ »ç¿ë½Ã Parameter Áßº¹ Ã¼Å© ½Ã½ºÅÛ ±¸Çö - 
	BOOL CheckParamOverlap(ITEM *i_pUsingItemInfo);
};
typedef vector<ITEM*>			vectItemPtr;		// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - 


///////////////////////////////////////////////////////////////////////////////
// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
// SUB_EVENT_TYPE_XXX
#define SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT	3
/**************************************************
ÃßÈÄ ¾ÆÀÌÅÛ ÀÌº¥Æ® ¼­ºê¿É¼ÇÀÌ Ãß°¡µÉ¶§, ¼­¹ö±¸Á¶´Â À§ÀÇ Ä«¿îÆ®ÀÇ ¼ö¸¸ ´Ã¸®¸é ÇØ°áÀÌ µÇ³ª.
DB Å×ÀÌºí ¹× ÇÁ·Î½ÃÀú´Â Ãß°¡·Î ¼öÁ¤À» ÇÏ¿© Àû¿ë(¹Ý¿µ) ÇØ¾ßÇÑ´Ù.
1. atum2_db_account.dbo.td_ItemEventSubType			Å×ÀÌºí
2. atum2_db_[n].dbo.atum_InsertItemEventSubType		ÇÁ·Î½ÃÀú
3. atum2_db_[n].dbo.atum_GetItemEventSubType		ÇÁ·Î½ÃÀú
2013. 4. 23 ¼­ºê¿É¼Ç ¼³Á¤ Á¦ÇÑ Àû¿ë			- ITEM_EVENT_TYPE_ONLYONE °ú ITEM_EVENT_TYPE_ONEDAY ÀÌº¥Æ® ¿¡¸¸ Play Time ¼­ºê¿É¼Ç ¼³Á¤ °¡´É
2013. 4. 23 ¼­ºê¿É¼Ç ¿ì¼±¼øÀ§ °íÁ¤À¸·Î Àû¿ë - ÇÃ·¹ÀÌÅ¸ÀÓÀ» ¸ÕÀú Ã¼Å©ÇÏ°í ÈÄ¿¡ ¾ÆÀÌÅÛÀÇ À¯¹«¸¦ È®ÀÎ
											- ÀÌ°ÍÀ» ÇØÁ¦ÇÏ·Á¸é SetDefaultSubTypePriority() ÇÔ¼öÈ£ÃâÀ» ¾ÈÇÏ¸é µÊ
											- ±×¸®°í °ü·Ã ¾îµå¹ÎÅø Ã¼Å©¹Ú½º ÄÁÆ®·Ñ·¯µµ È°¼ºÈ­ ÇØ¾ßÇÔ.
**************************************************/

#define ITEM_EVENT_MASK_CHECK(VAR, MASK)		(((VAR) & (MASK)) != 0)
#define ITEM_EVENT_STEP_PASS_MASK_SUCCESS		0x01	// 1
#define ITEM_EVENT_STEP_PASS_MASK_FAIL			0x10	// 2
#define ITEM_EVENT_DES_EMPTY					0	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ®
#define ITEM_EVENT_DES_PLAY_TIME				1	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ÇÃ·¹ÀÌ ½Ã°£º° ¼±¹°Áö±Þ
#define ITEM_EVENT_DES_PLAY_REQUIRED_ITEM		2	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - Æ¯Á¤¾ÆÀÌÅÛ º¸À¯½Ã ¼±¹° Áö±Þ
#define ITEM_EVENT_DES_PLAY_REQUIRED_COUNT		3	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - Æ¯Á¤¾ÆÀÌÅÛ º¸À¯½Ã ¼±¹° Áö±Þ
// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ

// 2006-08-24 by dhjin, ÀÌº¥Æ® ¾ÆÀÌÅÛ Á¤º¸
struct SITEM_EVENT_INFO
{
	int				ItemEventUID;
	int				ItemEventType;		// ITEM_EVENT_TYPE_XXX
	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
	BOOL			SubEventType;
	DestParam_t		ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT];
	INT				ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT];
	// end 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
	int				OnlyPCBang;
	BYTE			byInfluenceType;
	int				ItemNum;
	int				Count;
	ATUM_DATE_TIME  StartTime;
	ATUM_DATE_TIME	EndTime;
	BOOL			MemberShip;			// 2006-09-29 by dhjin, ¸É¹ö½± À¯Àú¸¸ Áö±Þ
	BYTE			ReqLevel;			// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP Ãß°¡·Î ·¹º§ ÇÊµå Ãß°¡
	BOOL			NewMember;			// 2007-07-23 by dhjin, ÀÌº¥Æ® ±â°£¾È¿¡ °¡ÀÔÇÑ À¯Àú¿¡°Ô¸¸ Áö±Þ
	INT				UnitKind;			// 2007-07-24 by dhjin, Áö±Þ ´ë»ó À¯´Ö
	INT				PrefixCodeNum;		// 2007-07-25 by dhjin, ¾ÆÀÌÅÛ Á¢µÎ ¿É¼Ç
	INT				SuffixCodeNum;		// 2007-07-25 by dhjin, ¾ÆÀÌÅÛ Á¢¹Ì ¿É¼Ç
	BOOL			UseLastGameEndDate;	// 2008-02-01 by cmkwon, ItemEvent ¿¡ LastGameEndDate Ã¼Å© ·çÆ¾ Ãß°¡ - SITEM_EVENT_INFO ¿¡ ÇÊµå Ãß°¡
	ATUM_DATE_TIME  atLastGameEndDate;	// 2008-02-01 by cmkwon, ItemEvent ¿¡ LastGameEndDate Ã¼Å© ·çÆ¾ Ãß°¡ - SITEM_EVENT_INFO ¿¡ ÇÊµå Ãß°¡
	BOOL			CheckWithCharacterUID;	// 2009-11-19 by cmkwon, ¾ÆÀÌÅÛ ÀÌº¥Æ®¿¡ Account/Character Ã¼Å© ±â´É Ãß°¡ - FALSE(0)ÀÌ¸é AccountUID·Î Ã¼Å©
	INT				iLevelMin;
	INT				iLevelMax;
	INT				LoginCheckNumber;		// 2011-08-25 by shcho, È½¼öº° ¾ÆÀÌÅÛ Áö±Þ±â´É ±¸Çö - 0ÀÎ°æ¿ì »ç¿ë¾ÈÇÔÀ¸·Î Ã³¸®. 2°ªÀÏ °æ¿ì, ÇÏ·ç¿¡ ÇÑ¹ø ±âÇÑ±îÁö 2¹ø Á¢¼ÓÇØ¾ß Áö±Þ

	BOOL			UseFixedPeriod;			// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡
	INT				FixedPeriod_DAY;		// 2013-02-28 by bckim, º¹±ÍÀ¯Á® ¹öÇÁÃß°¡

	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
	char* GetDestParameterStr(DestParam_t i_DestParam)
	{
		switch(i_DestParam)
		{
		case ITEM_EVENT_DES_EMPTY:					return "ITEM_EVENT_DES_EMPTY";
		case ITEM_EVENT_DES_PLAY_TIME:				return "ITEM_EVENT_DES_PLAY_TIME";
		case ITEM_EVENT_DES_PLAY_REQUIRED_ITEM:		return "ITEM_EVENT_DES_PLAY_REQUIRED_ITEM";
		case ITEM_EVENT_DES_PLAY_REQUIRED_COUNT:	return "ITEM_EVENT_DES_PLAY_REQUIRED_COUNT";
		}
		return "ITEM_EVENT_DES_UNKNOWN";
	}

	char* GetDestParameterShotStr(DestParam_t i_DestParam)
	{
		switch(i_DestParam)
		{
		case ITEM_EVENT_DES_EMPTY:					return "Empty";
		case ITEM_EVENT_DES_PLAY_TIME:				return "PlayTime";
		case ITEM_EVENT_DES_PLAY_REQUIRED_ITEM:		return "ReqItem";
		case ITEM_EVENT_DES_PLAY_REQUIRED_COUNT:	return "ReqCount";
		}
		return "ITEM_EVENT_DES_UNKNOWN";
	}
	// end 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¼­ºêÅ¸ÀÔ Ãß°¡±¸Á¶·Î º¯°æ
};

typedef	vector<SITEM_EVENT_INFO>	vectItemEventInfo;
// ITEM_EVENT_TYPE_XXX
#define ITEM_EVENT_TYPE_ONLYONE					1
#define ITEM_EVENT_TYPE_ONEDAY					2
#define ITEM_EVENT_TYPE_INFLCHANGE				3
#define ITEM_EVENT_TYPE_LEVELUP					4
#define ITEM_EVENT_TYPE_COUPON_ONLYONE			5	// 2008-01-10 by cmkwon, ¾ÆÀÌÅÛ ÀÌº¥Æ® ½Ã½ºÅÛ¿¡ ½Å ÄíÆù ½Ã½ºÅÛ Ãß°¡ - ÇÑ°èÁ¤´ç ÇÑ¹ø¸¸ °¡´ÉÇÑ ÄíÆù ÀÌº¥Æ®
#define ITEM_EVENT_TYPE_COUPON_ONEDAY			6	// 2008-01-10 by cmkwon, ¾ÆÀÌÅÛ ÀÌº¥Æ® ½Ã½ºÅÛ¿¡ ½Å ÄíÆù ½Ã½ºÅÛ Ãß°¡ - ÇÑ°èÁ¤´ç ÇÏ·ç¿¡ ÇÑ¹ø °¡´ÉÇÑ ÄíÆù ÀÌº¥Æ®
#define ITEM_EVENT_TYPE_LOGIN					7	// 2011-08-25 by shcho, È½¼öº° ¾ÆÀÌÅÛ Áö±Þ±â´É ±¸Çö
#define ITEM_EVENT_TYPE_FIRST_PURCHASE			8	// 2013-03-29 by jhseol, ¾ÆÀÌÅÛ ÀÌº¥Æ® - ¸Å¿ù Ã¹ °áÀç½Ã ¼±¹°Áö±Þ

struct MAP_AREA {
	INT		MapIndex;
	INT		X;
	INT		Y;
	INT		Radius;

	BOOL IsPositionInArea(INT i_nMapIndex, INT i_nX, INT i_nY)
	{
		// map È®ÀÎ
		if (MapIndex == 0) {							return TRUE; }
		if (MapIndex != i_nMapIndex) {					return FALSE; }

		// °Å¸® È®ÀÎ
		if (X == -1 || Y == -1 || Radius == -1){		return TRUE; }
		if ((INT)LENGTH(X-i_nX, Y-i_nY) <= Radius)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}

	void Reset() { MapIndex = 0; X = 0; Y = 0; Radius = 0; }
};

typedef struct
{
	ITEM		*pItemInfo;
	DWORD		dwUsingPercent;
	BYTE		byArrayIndex;
	BYTE		byBodyConArrayIndex;		// 2006-12-15 by cmkwon, DBÀÇ ¼ø¼­ - ÀÌÆåÆ® ¹ÙµðÄÁµð¼Ç°ú ¿¬°ü ÀÖ´Ù
} MONSTER_ITEM;

// start 2011-03-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡
typedef struct 
{
	D3DXVECTOR3		Position;
} MONSTER_TARGET;
// end 2011-03-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡

struct ITEM_W_COUNT {
	INT		ItemNum;
	INT		Count;

	void Reset() { ItemNum = 0; Count = 0; }
};

struct ITEM_W_COUNT_CHECK
{
	BOOL	bChecked;		// 2006-08-28 by cmkwon
	INT		ItemNum;
	INT		Count;

	void Reset(void) { bChecked = FALSE; ItemNum = 0; Count = 0; }
};

struct MONSTER_W_COUNT	// 2005-10-19 by cmkwon
{
	INT		MonsterUniqueNumber;
	INT		Count;
	MAP_AREA		MapArea;			// 2007-03-14 by cmkwon, ¸ó½ºÅÍ À§Ä¡ ÁÂÇ¥¸¦ ¹Ì´Ï¸Ê¿¡ º¸¿©ÁÖ±â À§ÇØ

	void Reset() { MonsterUniqueNumber = 0; Count = 0; }
};
typedef vector<MONSTER_W_COUNT>			vectMONSTER_W_COUNT;

struct ITEM_W_COUNT_PROB {
	INT			ItemNum;
	INT			Count;
	Prob100_t	Prob100;		// È®·ü

	void Reset() { ItemNum = 0; Count = 0; Prob100 = 0; }
};

struct ITEM_UNIQUE_NUMBER_W_COUNT {
	UID64_t	ItemUniqueNumber;
	INT		Count;

	void Reset() { ItemUniqueNumber = 0; Count = 0; }
};

struct ItemID_t
{
	UID64_t	ItemUID;
	INT		ItemNum;

	ItemID_t() {}
	ItemID_t(UID64_t i_ItemUID, INT i_ItemNum)
	{
		ItemUID = i_ItemUID;
		ItemNum = i_ItemNum;
	}
};

typedef mt_vector<ItemID_t>		mtvectorItemID;		// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±

struct ITEM_UID_W_ITEMNUM_COUNT			// 2008-09-26 by cmkwon, Á¶ÇÕ½Ã GameLog ¼öÁ¤ - ¼±¾ð Ãß°¡ ÇÔ
{
	UID64_t		ItemUID;
	INT			ItemNum;
	INT			Count;
};


///////////////////////////////////////////////////////////////////////////////
//	ATUM - Monster - Parameter Struct Á¤ÀÇ
///////////////////////////////////////////////////////////////////////////////
//#define ARRAY_SIZE_MONSTER_SECONDARY_ITEM			5
#define ARRAY_SIZE_MONSTER_ITEM						16			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 10°³ Ãß°¡ 6 -> 16
#define ARRAY_SIZE_MONSTER_TARGET					10			// 2011-03-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡ - 10°³·Î Á¦ÇÑ
#define ARRAY_INDEX_MONSTER_SKILL_ITEM				15			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 10°³ Ãß°¡ 5 -> 15
#define ARRAY_SIZE_MONSTER_SKILL_ITEM				10			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ±âÁ¸ ¾ÆÀÌÅÛÀº °øÅë¾ÆÀÌÅÛÀ¸·Î »ç¿ëÇÏ°í Ãß°¡µÈ 10°³ ¾ÆÀÌÅÛÀº ½ºÅ³¾ÆÀÌÅÛÀ¸·Î »ç¿ëÇÑ´Ù.
#define SIZE_MAX_FILE_NAME			50	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - 
struct MONSTER_INFO
{
//	ClientIndex_t	MonsterIndex;					// ÀÎµ¦½º
//	SHORT			CurrentHP;						// ÇöÀç ¿¡³ÊÁö
//	BodyCond_t		BodyCondition;					// Ä³¸¯ÅÍ BodyCondition
//	ITEM			*ItemInfoPrimary;				// Primary °ø°Ý Item, DB¿¡¼­ ÀÐÀ» ¶§´Â ItemNumÀ» ÇÒ´çÇÏ°í, ±× ÀÌÈÄ¿¡ pointer¸¦ ÇÒ´çÇÔ, by kelovon
//	ITEM			*ItemInfoSecondary[ARRAY_SIZE_MONSTER_SECONDARY_ITEM];			// Secondary °ø°Ý Item
//	D3DXVECTOR3		PositionVector;					// Ä³¸¯ÅÍ ÁÂÇ¥
//	D3DXVECTOR3		TargetVector;					//
//	D3DXVECTOR3		UpVector;						//
	INT				MonsterUnitKind;				// ¸ó½ºÅÍ °íÀ¯¹øÈ£
	char			MonsterName[SIZE_MAX_MONSTER_NAME];	// ¸ó½ºÅÍ ÀÌ¸§
	BYTE			Level;							// ¸ó½ºÅÍÀÇ Level
	INT				MonsterHP;						// ¸¸ÇÇ
	USHORT			Race;							// Á¾Á·
	float			Defense;						// ¹æ¾î·Â, 0 ~ 255		// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ, // 2009-12-17 by cmkwon, µ¥¹ÌÁö °è»ê½Ä °ü·ÃÇÑ ÇÊµå(¹æ¾î·Â,È¸ÇÇ,ÇÇ¾î½º,È®·ü)¿¡ 255ÀÌ»ó °ªÀ» ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - ±âÁ¸ÀÚ·áÇü(BYTE)
	float			DefenseProbability;				// ¹æ¾î¼º°øÈ®·ü, 20030630, Ãß°¡µÊ // 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
	BYTE			Speed;							// ÀÌµ¿¼Óµµ
	MONSTER_ITEM	ItemInfo[ARRAY_SIZE_MONSTER_ITEM];	// ¸ó½ºÅÍ°¡ °¡Áö°í ÀÖ´Â ¸ðµç ¾ÆÀÌÅÛ
	USHORT			Size;							// ÇÊµå ¼­¹ö´Â SizeForClient¸¦ loading,  NPC ¼­¹ö´Â SizeForServer¸¦ loading
	BYTE			Faction;						// °è¿­
	SHORT			MonsterForm;					// Monster ÇüÅÂ
	BYTE			AttackPattern;					//
	SHORT			MovePattern;					//
	BYTE			Belligerence;					// È£Àü¼º
	BYTE			AttackObject;					// ATTACKOBJ_XXX
	SHORT			AttackRange;					// °ø°Ý¼ºÇâ ¸ó½ºÅÍÀÇ ¸ó½ºÅÍ°¡ Å¸°ÙÆÃ°¡´ÉÇÑ °Å¸®
	LONGLONG		Experience;						// ÆøÆÄ½Ã ÁÖ´Â °æÇèÄ¡
	BYTE			AlphaBlending;					// alpha blending ¿©ºÎ, TRUE(1), FALSE(0), clientÃø »ç¿ëÀ» À§ÇØ Ãß°¡, 20030616
	USHORT			HPRecoveryTime;					// HP Â÷´Â ½Ã°£
	SHORT			HPRecoveryValue;				// ÇÑ¹ø¿¡ Â÷´Â HPÀÇ ¾ç
	USHORT			RenderIndex;
	float			ScaleValue;
	BYTE			TextureIndex;
	UINT			SourceIndex;
	BYTE			QuickTurnAngle;					// ÃÖ´ë°¢ ¹üÀ§
	BYTE			QuickSpeed;						// ºü¸£°Ô ÀÌµ¿½Ã ¼Óµµ
	USHORT			QuickTurnTerm;					// ºü¸£°Ô ÀÌµ¿ÇÏ´Â Term
	BitFlag64_t		MPOption;						// 2010-01-11 by cmkwon, ¸ó½ºÅÍ MPOption 64bit·Î º¯°æ - ±âÁ¸(BYTE), MPOPTION_BIT_XXX
	BYTE			MPOptionParam1;
	BYTE			MPOptionParam2;
	USHORT			Range;
	FLOAT			TurnAngle;
	BYTE			MonsterHabit;					// ½À¼º, ex) ¾ß°£ °ø°Ý·Â Áõ°¡ µî
	BYTE			ClickEvent;						// 2007-09-05 by dhjin, ¸ó½ºÅÍ Å¬¸¯ ÀÌº¥Æ®
	HPActionIdx_t	HPActionIdx;					// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - HP Çàµ¿
	MonIdx_t		MonsterTarget;					// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ °­Á¦ °ø°Ý ´ë»ó ¹øÈ£
	ChangeTarget_t	ChangeTarget;					// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¸ó½ºÅÍ °ø°Ý ¹ÞÀ» °æ¿ì Å¸°Ù°ª º¯°æ ¿©ºÎ
	MonIdx_t		MonsterTarget2;					// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - ¸ó½ºÅÍ °­Á¦ °ø°Ý ´ë»ó ¹øÈ£ 2¹øÂ° ¿ì¼± ¼øÀ§
	char			PortraitFileName[SIZE_MAX_FILE_NAME];	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - ¸ó½ºÅÍ ÃÊ»óÈ­ ÆÄÀÏ
	INT				ChangeTargetTime;				// 2010-04-14 by cmkwon, ÀÎÇÇ2Â÷ ¸ó½ºÅÍ ·£ÅÒ Å¸°Ù º¯°æ Ã³¸® - 
	MONSTER_TARGET	MultiTargetInfo[ARRAY_SIZE_MONSTER_TARGET];				// 2011-03-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡
																			// ÁÖÀÇ »çÇ× => ÀÎµ¦½º´Â ³»ºÎÀûÀ¸·Î´Â 0ºÎÅÍ °ü¸®
																			// ´ë¿ÜÀûÀÎ ÀÎµ¦½º´Â 1ºÎÅÍ ½ÃÀÛ, 0Àº ¸ÖÆ¼ Å¸°ÙÀÌ ¾ø´Â °ªÀ» ÀÇ¹Ì
	INT				WayPointPattern;				// 2011-05-23 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¿þÀÌÆ÷ÀÎÆ® ±¸Çö - WayPointPattern ÇÊµå Ãß°¡
};


// 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )
//////////////////////////////////////////////////////////////////////////////////////////////////

// 2010-05-24 by shcho, ÀÎÇÇ´ÏÆ¼ ³­ÀÌµµ Á¶Àý -
struct INFINITY_DIFFICULTY_BONUS_INFO
{
	int iIncreaseStep; //³­ÀÌµµ ´Ü°è
	int iExp; //°æÇèÄ¡ È®·ü
	int iDrop_Item_Persent;//µå¶ø ¾ÆÀÌÅÛ È®·ü
	int iItem_EA_persent;//¾ÆÀÌÅÛ °³¼ö È®·ü
};
typedef vector<INFINITY_DIFFICULTY_BONUS_INFO>	vectorInfinity_DifficultyInfo_Bonus; //³­ÀÌµµ ¸®½ºÆ® º¤ÅÍ

// 2010-05-24 by shcho, ÀÎÇÇ´ÏÆ¼ ³­ÀÌµµ Á¶Àý -
struct INFINITY_DIFFICULTY_MONSTER_SETTING_INFO
{
	int iIncreaseStep; //³­ÀÌµµ ´Ü°è
	int iMonsterHP; //Àû¿ë µÉ HP
	int iMonsterDefense;//Àû¿ë µÉ ¹æ¾î·Â
	int iMonsterEvasion;//Àû¿ë µÉ È¸ÇÇ·Â
	int iMonsterAttackItem;//Àû¿ë µÉ ¸ó½ºÅÍ ¾ÆÀÌÅÛ µ¥¹ÌÁö & ½ºÅ³ Áö¼Ó½Ã°£
	int iMonsterPopulation;//Àû¿ë µÉ ¸ó½ºÅÍ °³Ã¼¼ö 
};
typedef vector<INFINITY_DIFFICULTY_MONSTER_SETTING_INFO>	vectorInfinity_DifficultyInfo_Monster; //³­ÀÌµµ ¸ó½ºÅÍ Àû¿ë ¸®½ºÆ® º¤ÅÍ

struct MONSTER_BALANCE_DATA						// ¸ó½ºÅÍ ¹ë·±½º È®Àå Á¤º¸.
{
	MONSTER_BALANCE_DATA( const INT a_Step = 0 )
	{

		this->DifficultyStep = a_Step;

		this->fMaxHPRatio = 1.0f;
		this->fDefenseRatio = 1.0f;
		this->fDefenseProbabilityRatio = 1.0f;
		this->fAtkMsgRatio = 1.0f;
		this->fIncreaseExpRatio = 1.0f;
		this->fIncreaseDropItemProbabilityRatio = 1.0f;
		this->fIncreaseDropItemCountRatio = 1.0f;
		this->fSummonCountRatio = 1.0f;
	}

	void Reset ( void )
	{
		memset ( this , 0x00 , sizeof( MONSTER_BALANCE_DATA ) );
	}

	void Init ( const INT a_Step )
	{
		this->DifficultyStep = a_Step;
		
		this->fMaxHPRatio = 1.0f;
		this->fDefenseRatio = 1.0f;
		this->fDefenseProbabilityRatio = 1.0f;
		this->fAtkMsgRatio = 1.0f;
		this->fIncreaseExpRatio = 1.0f;
		this->fIncreaseDropItemProbabilityRatio = 1.0f;
		this->fIncreaseDropItemCountRatio = 1.0f;
		this->fSummonCountRatio = 1.0f;
	}

	void operator= ( const MONSTER_BALANCE_DATA &a_MonsterBalanceData )
	{
		memcpy ( this , &a_MonsterBalanceData , sizeof ( MONSTER_BALANCE_DATA ) );
	}

	void operator= ( const INFINITY_DIFFICULTY_BONUS_INFO &a_Infinity_BonusInfo )
	{
		if ( this->DifficultyStep != a_Infinity_BonusInfo.iIncreaseStep )
			return;

		this->fIncreaseExpRatio = (float)a_Infinity_BonusInfo.iExp * 0.01f;
		this->fIncreaseDropItemProbabilityRatio = (float)a_Infinity_BonusInfo.iDrop_Item_Persent * 0.01f;
		this->fIncreaseDropItemCountRatio = (float)a_Infinity_BonusInfo.iItem_EA_persent * 0.01f;
	}

	void operator = ( const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO &a_Infinity_MonsterInfo ) 
	{
		if ( this->DifficultyStep != a_Infinity_MonsterInfo.iIncreaseStep )
			return;
		
		this->fMaxHPRatio = (float)a_Infinity_MonsterInfo.iMonsterHP * 0.01f;
		this->fDefenseRatio = (float)a_Infinity_MonsterInfo.iMonsterDefense * 0.01f;
		this->fDefenseProbabilityRatio = (float)a_Infinity_MonsterInfo.iMonsterEvasion * 0.01f;
		this->fAtkMsgRatio = (float)a_Infinity_MonsterInfo.iMonsterAttackItem * 0.01f;
		this->fSummonCountRatio = (float)a_Infinity_MonsterInfo.iMonsterPopulation * 0.01f;
	}

	void operator += ( const INFINITY_DIFFICULTY_BONUS_INFO &a_Infinity_BonusInfo )
	{
		if ( this->DifficultyStep != a_Infinity_BonusInfo.iIncreaseStep )
			return;

		this->fIncreaseExpRatio += (float)a_Infinity_BonusInfo.iExp * 0.01f;
		this->fIncreaseDropItemProbabilityRatio += (float)a_Infinity_BonusInfo.iDrop_Item_Persent * 0.01f;
		this->fIncreaseDropItemCountRatio += (float)a_Infinity_BonusInfo.iItem_EA_persent * 0.01f;
	}

	void operator += ( const INFINITY_DIFFICULTY_MONSTER_SETTING_INFO &a_Infinity_MonsterInfo ) 
	{
		if ( this->DifficultyStep != a_Infinity_MonsterInfo.iIncreaseStep )
			return;
		
		this->fMaxHPRatio += (float)a_Infinity_MonsterInfo.iMonsterHP * 0.01f;
		this->fDefenseRatio += (float)a_Infinity_MonsterInfo.iMonsterDefense * 0.01f;
		this->fDefenseProbabilityRatio += (float)a_Infinity_MonsterInfo.iMonsterEvasion * 0.01f;
		this->fAtkMsgRatio += (float)a_Infinity_MonsterInfo.iMonsterAttackItem * 0.01f;
		this->fSummonCountRatio += (float)a_Infinity_MonsterInfo.iMonsterPopulation * 0.01f;
	}

	// 2010. 07. 05 by hsLee. ¸ó½ºÅÍ ¼ÒÈ¯ °ü·Ã. (¹ë·±½º µ¥ÀÌÅÍ ´©¶ô ¼öÁ¤.) - MaxHp´Â 0ÀÌÇÏ°¡ µÉ¼ö ¾øÀ½.
	const bool IsValidData ( void ) const
	{
		return ( fMaxHPRatio > .0f );
	}

	INT		DifficultyStep;

	float	fMaxHPRatio;							// ¸ó½ºÅÍ HP Áõ°¡À².
	float	fDefenseRatio;							// ¸ó½ºÅÍ ¹æ¾î·Â Áõ°¡À².
	float	fDefenseProbabilityRatio;				// ¸ó½ºÅÍ ¹æ¾î¼º°ø(È¸ÇÇ) Áõ°¡À².
	float	fAtkMsgRatio;							// ¸ó½ºÅÍ °ø·Â·Â Áõ°¡À².
	float	fSummonCountRatio;						// ¸ó½ºÅÍ ¼ÒÈ¯ Áõ°¡À².
	
	float	fIncreaseExpRatio;						// ¸ó½ºÅÍ Ã³Ä¡ °æÇèÄ¡ È¹µæ Áõ°¡À².
	float	fIncreaseDropItemProbabilityRatio;		// ¸ó½ºÅÍ Ã³Ä¡ ¾ÆÀÌÅÛ µå¶øÀ²ÀÇ Áõ°¡À².
	float	fIncreaseDropItemCountRatio;			// ¸ó½ºÅÍ Ã³Ä¡ ¾ÆÀÌÅÛ µå¶ø °³¼ö Áõ°¡À².

};
// End 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )

// 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )
struct MONSTER_INFO_EXTEND
{

	float			fMaxHP;								// ¸ó½ºÅÍ ÃÖ´ë HP.
	float			Defense;							// ¸ó½ºÅÍ ¹æ¾î·Â.	// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
	float			DefenseProbability;					// ¸ó½ºÅÍ ¹æ¾îÀ².	// 2010-07-19 by dhjin, È®·ü ¼ö½Ä º¯°æ
	float			fAtkDmgRatio;						// ¸ó½ºÅÍ °ø°Ý·Â Áõ°¡À².

	float			fIncreaseExpRatio;					// ¸ó½ºÅÍ Ã³Ä¡½Ã È¹µæ °æÇèÄ¡ Áõ°¡À².
	float			fIncreaseDropProbabilityRatio;		// ¸ó½ºÅÍ Ã³Ä¡½Ã ¾ÆÀÌÅÛ µå¶øÈ®·üÀÇ Áõ°¡À².
	float			fIncreaseDropItemCountRatio;		// ¸ó½ºÅÍ Ã³Ä¡½Ã ¾ÆÀÌÅÛ µå¶ø °³¼ö Áõ°¡À².
};
// End. 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )


class MONSTER
{
public:
	MONSTER_INFO *		MonsterInfoPtr;					// ¸ó½ºÅÍ Á¤º¸ Æ÷ÀÎÅÍ
	ClientIndex_t		MonsterIndex;					// ÀÎµ¦½º
	float				CurrentHP;						// ÇöÀç ¿¡³ÊÁö
	D3DXVECTOR3			PositionVector;					// Ä³¸¯ÅÍ ÁÂÇ¥
	D3DXVECTOR3			TargetVector;					//
	D3DXVECTOR3			UpVector;						//
	BodyCond_t			BodyCondition;					// Ä³¸¯ÅÍ BodyCondition

// 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )
	MONSTER_INFO_EXTEND	MonsterInfoExtend;				// ¸ó½ºÅÍ È®Àå Á¤º¸. 
	MONSTER_BALANCE_DATA MonsterInfoBalance;			// ¸ó½ºÅÍ ¹ë·±½º Á¤º¸.
// End 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )

protected:
	MONSTER(){};
};


///////////////////////////////////////////////////////////////////////////////
// 2009-12-23 by cmkwon, Ãæµ¹ Ã¼Å©´Â ÇÏÁö¸¸ Ãæµ¹ µ¥¹ÌÁö ÀÔÁö ¾Ê´Â ¿ÀºêÁ§Æ® ±¸Çö - COLLISION_TYPE_XXX
#define COLLISION_TYPE_NONE					0
#define COLLISION_TYPE_NORMAL				1
#define COLLISION_TYPE_NO_DAMAGE			2

typedef struct _MAPOBJECTINFO
{
	_MAPOBJECTINFO()
	{
		Code				= 0;
		Alpha				= 0;
		Collision			= 0;
		CollisionForServer	= 0;
		ObjectRenderType	= 0;
		RenderIndex			= 0;
	}
	INT		Code;						// Object Type
	BYTE	Alpha;						//
	BYTE	Collision;					// Ãæµ¹ Ã³¸® ÇÃ·¡±×, // 2009-12-23 by cmkwon, Ãæµ¹ Ã¼Å©´Â ÇÏÁö¸¸ Ãæµ¹ µ¥¹ÌÁö ÀÔÁö ¾Ê´Â ¿ÀºêÁ§Æ® ±¸Çö - COLLISION_TYPE_XXX
	BYTE	CollisionForServer;
	BYTE	ObjectRenderType;
	USHORT	RenderIndex;

	// 2009-12-23 by cmkwon, Ãæµ¹ Ã¼Å©´Â ÇÏÁö¸¸ Ãæµ¹ µ¥¹ÌÁö ÀÔÁö ¾Ê´Â ¿ÀºêÁ§Æ® ±¸Çö - 
	BOOL IsCheckDamage(void)
	{
		if(FALSE == Collision
			|| COLLISION_TYPE_NO_DAMAGE == Collision)
		{
			return FALSE;
		}
		
		return TRUE;
	}
} MAPOBJECTINFO;

/*
typedef struct _MAPOBJECTINFO
{
	INT		Code;						// Object Type
	BOOL	Alpha;						//
	USHORT	RadiusForServer;			// Ojbect ¹ÝÁö¸§, ¼­¹ö¿ë
	USHORT	RadiusForClient;			// Ojbect ¹ÝÁö¸§, Å¬¶óÀÌ¾ðÆ®¿ë
	BOOL	Collision;					// Ãæµ¹ Ã³¸® ÇÃ·¡±×
	BOOL	CollisionForServer;
	USHORT	RenderIndex;
	char	ObjectName[SIZE_MAX_OBJECT_NAME];
	BYTE	ObjectRenderType;
} MAPOBJECTINFO;
*/

// ½ºÅ³ÇüÅÂ(½Ã°£ ¹× ¹ßµ¿ °ü·Ã)
#define SKILLTYPE_PERMANENT		0	// Áö¼ÓÇü
#define SKILLTYPE_CLICK			1	// Å¬¸¯Çü
#define SKILLTYPE_TIMELIMIT		2	// ½Ã°£Çü
#define SKILLTYPE_TOGGLE		3	// Åä±ÛÇü
#define SKILLTYPE_CHARGING		4	// Â÷Â¡Çü, ½ºÅ³ »ç¿ë ÀÌÈÄ ¹Ù·Î ´ÙÀ½ÀÇ µ¿ÀÛ(¹ß»ç µî)¿¡¸¸ 1È¸ Àû¿ëµÇ´Â ½ºÅ³

// ½ºÅ³ Å¸°Ù Å¸ÀÔ, SKILLTARGETTYPE_XXX
#define SKILLTARGETTYPE_ME								0	// ÀÚ±â ÀÚ½Å
#define SKILLTARGETTYPE_ONE								1	// ÇÏ³ªÀÇ Å¸°Ù
#define SKILLTARGETTYPE_PARTY_WITH_ME					2	// ÁÖÀ§ÀÇ ÆÄÆ¼¿ø, ÀÚ½Å Æ÷ÇÔ
#define SKILLTARGETTYPE_INRANGE_WITH_ME					3	// ÁÖÀ§, ÀÚ½Å Æ÷ÇÔ
#define SKILLTARGETTYPE_PARTY_WITHOUT_ME				4	// ÁÖÀ§ÀÇ ÆÄÆ¼¿ø, ÀÚ½Å Æ÷ÇÔ ¾È ÇÔ
#define SKILLTARGETTYPE_INRANGE_WITHOUT_ME				5	// ÁÖÀ§, ÀÚ½Å Æ÷ÇÔ ¾È ÇÔ
#define SKILLTARGETTYPE_ONE_OURS_INRANGE_WITHOUT_ME		6	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - »ç°Å¸® ¾È¿¡ ÀÚ½ÅÀ» Á¦¿ÜÇÑ ÇÏ³ªÀÇ ¾Æ±º
#define SKILLTARGETTYPE_ALL_OURS_INRANGE_WITHOUT_ME		7	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - »ç°Å¸® ¾È¿¡ ÀÚ½ÅÀ» Á¦¿ÜÇÑ ¸ðµç ¾Æ±º
#define SKILLTARGETTYPE_ONE_ENEMY_INRANGE				8	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - »ç°Å¸® ¾È¿¡ ÇÏ³ªÀÇ Àû
#define SKILLTARGETTYPE_MUTI_ENEMY_INRANGE				9	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - »ç°Å¸® ¾È¿¡ ¸ÖÆ¼ Å¸°Ù ¼ö ¸¸Å­¿¡ Àû
#define SKILLTARGETTYPE_ONE_EXCLUDE_ME					11	// ÇÏ³ªÀÇ Å¸°Ù, ÀÚ½ÅÀº Á¦¿Ü
#define SKILLTARGETTYPE_PARTY_ONE						21	// ¾Æ¹«³ª ÇÏ³ªÀÇ ÆÄÆ¼¿ø, // 2005-12-05 by cmkwon
#define SKILLTARGETTYPE_PARTY_ONE_EXCLUDE_ME			22	// ÀÚ½ÅÀ» Á¦¿ÜÇÑ ÇÏ³ªÀÇ ÆÄÆ¼¿ø, // 2005-12-05 by cmkwon
#define SKILLTARGETTYPE_ALL_ENEMY						23	// 2006-11-17 by dhjin, ÀÚ½ÅÀ» Áß½ÉÀ¸·Î ÇÑ Àû±â(Å¸¼¼·Â, ¸ó½ºÅÍ, 1:1, Æí´ëÀü, ¿©´ÜÀü)
#define SKILLTARGETTYPE_ONE_EXCEPT_OURS					24	// 2006-11-17 by dhjin, ÇÏ³ªÀÇ Å¸°Ù, ¾Æ±ºÀº Á¦¿Ü
#define SKILLTARGETTYPE_ALL_OURS						25  // 2007-02-06 by dhjin, ÀÚ½ÅÀ» Áß½ÉÀ¸·Î ÇÑ ¾Æ±º±â



// È¸±Íµµ(Scarcity) Á¾·ù
#define SCARCITY_100000			0	// È®·ü 0~99999
#define SCARCITY_1000000		1	// È®·ü 0~999999
#define SCARCITY_10000000		2	// È®·ü 0~9999999

// ¾ÆÀÌÅÛÀÇ ¼Ó¼º, ITEM_ATTR_XXX
#define ITEM_ATTR_AUTO_PICK_UP			0x00000001		// 1, ¸ó½ºÅÍ°¡ ÅÍÁö¸é ÀÚµ¿À¸·Î ¸Ô±â(¹Ù´Ú¿¡ ¾È ¶³¾îÁü)
#define ITEM_ATTR_NO_TRANSFER			0x00000002		// 2, Ã¢°í ÀÌµ¿, ¹ö¸®±â, »óÁ¡¿¡¼­ ±¸ÀÔ, »óÁ¡¿¡ ÆÇ¸Å, À¯Àú°£ °Å·¡ ºÒ°¡
#define ITEM_ATTR_QUEST_ITEM			0x00000004		// 4, Äù½ºÆ® ¾ÆÀÌÅÛ, º¸ÅëÀÇ °æ¿ì´Â ¶³¾îÁöÁö ¾Ê°í, Äù½ºÆ® ¼öÇàÁßÀÌ¸ç Á¶°Ç¿¡ ¸¸Á·ÇÒ ¶§¸¸ ¶³¾îÁü, check: deprecated, ITEMKIND_QUEST¸¦ »ç¿ë, 20040714, kelovon
#define ITEM_ATTR_TIME_LIMITE			0x00000008		// 8, ½Ã°£ Á¦ÇÑ ¾ÆÀÌÅÛ, ¾ÆÀÌÅÛ »ç¿ë½Ã »ç¿ë ½Ã°£ Á¦ÇÑÀÌ ÀÖ´Ù
#define ITEM_ATTR_KILL_MARK_ITEM		0x00000010		// 16, ¼¼·ÂÀü Å³¸¶Å© ¾ÆÀÌÅÛ(Ã¢°íÀÌµ¿/¹ö¸®±â/»óÁ¡¿¡ÆÇ¸Å °¡´É, À¯Àú°£°Å·¡/»óÁ¡¿¡¼­±¸ÀÔ ºÒ°¡), 2006-02-09 by cmkwon
#define ITEM_ATTR_SKILL_SUPPORT_ITEM	0x00000020		// 32, ½ºÅ³ º¸Á¶ ¾ÆÀÌÅÛ, 2006-09-29 by cmkwon Ãß°¡ µÊ
#define ITEM_ATTR_DELETED_TIME_LIMITE	0x00000040		// 64, ¾ÆÀÌÅÛÀÌ »ý¼º ÈÄ À¯È¿ ½Ã°£(ti_item.Endurance)ÀÌ °æ°úÇÏ¸é ÀÚµ¿À¸·Î »èÁ¦µÇ´Â ¾ÆÀÌÅÛ, 2006-09-29 by cmkwon
#define ITEM_ATTR_ACCOUNT_POSSESSION	0x00000080		// 128, °èÁ¤¼ÒÀ¯¾ÆÀÌÅÛ(°³ÀÎÃ¢°íÀÌµ¿/¹ö¸®±â/»óÁ¡¿¡ÆÇ¸Å/»óÁ¡¿¡¼­±¸ÀÔ °¡´É, À¯Àú°£°Å·¡/¿©´ÜÃ¢°íÀÌµ¿ ºÒ°¡), // 2007-09-17 by cmkwon, ¿¹´ç Ä³½¬»óÁ¡ Ä³½¬¾ÆÀÌÅÛ °Å·¡ ºÒ°¡ ±â´É Ãß°¡ - 

#define ITEM_ATTR_ARENA_ITEM					0x00000100		// 256, ¾Æ·¹³ª¿¡¼­¸¸ »ç¿ë °¡´ÉÇÑ ¾ÆÀÌÅÛ, 2007-06-01 by dhjin
#define ITEM_ATTR_SUPER_GAMBLE_ITEM				0x00000200		// 512, // 2007-11-07 by cmkwon, °·ºí/½´°· ºÙ´Â ½Ã½ºÅÛ ¼öÁ¤ - ½´ÆÛ°·ºí½´ÆÛ°·ºí ¿É¼ÇÀÌ ºÙÀ» °·ºíÄ«µå¾ÆÀÌÅÛ
#define	ITEM_ATTR_PACKAGE_ITEM					0x00000400		// 1024, // 2008-08-26 by cmkwon, ItemAttribute Ãß°¡ - ÆÐÅ°Áö(Package) ¾ÆÀÌÅÛ, ÇöÀç´Â ¹Ì½ºÅ×¸®Ä¸½¶¸¸ Àû¿ëµÊ
#define	ITEM_ATTR_FOR_ONLY_GAME_MASTER			0x00000800		// 2048, // 2008-08-26 by cmkwon, ItemAttribute Ãß°¡ - GM¿ë ¾ÆÀÌÅÛ, RACE_GAMEMASTER ¸¸ »ç¿ë °¡´ÉÇÑ ¾ÆÀÌÅÛ

#define ITEM_ATTR_UNIQUE_ITEM					0x00001000		// 4096, À¯´ÏÅ© ¾ÆÀÌÅÛ, ÀÎÃ¦Æ®/°·ºí ºÒ°¡, 2005-11-21 by cmkwon
#define ITEM_ATTR_BAZAAR_ITEM					0x00002000		// 8192, 2006-07-26 by cmkwon, °³ÀÎ»óÁ¡¿¡ ±¸ÀÔ/ÆÇ¸Å °¡´É ¾ÆÀÌÅÛ
#define ITEM_ATTR_LEGEND_ITEM					0x00004000		// 16384, // 2007-08-22 by cmkwon, ·¹Àüµå ¾ÆÀÌÅÛ, ±âº»Àº À¯´ÏÅ©¾ÆÀÌÅÛ ¼Ó¼ºÀÌ¸ç Á¶ÇÕ½Ã Å¸°Ù ¾ÆÀÌÅÛÀÌ ·¹Àüµå ÀÌ¸é ÀÎÃ¦Æ®°¡ À¯ÁöµÈ´Ù
#define ITEM_ATTR_FORBID_ARENA_COPY_ITEM		0x00008000		// 32768, // 2008-01-07 by dhjin, ¾Æ·¹³ª ÅëÇÕ - atum2_db_n => atum2_db_arena ·Î td_store¸¦ º¹»çÇÏ¸é ¾ÈµÇ´Â ¾ÆÀÌÅÛ

#define ITEM_ATTR_CASH_ITEM						0x00010000		// 65536, À¯·áÈ­ ¾ÆÀÌÅÛ
#define ITEM_ATTR_CASH_ITEM_PREMIUM_CARD		0x00020000		// 131072, // 2008-08-26 by cmkwon, ItemAttribute Ãß°¡ - ¸â¹ö½±(ÇÁ¸®¹Ì¾ö) ¾ÆÀÌÅÛ,  

#define ITEM_ATTR_LUCKY_ITEM					0x00040000		// 262144, // 2008-11-04 by dhjin, ·°Å°¸Ó½Å
#define ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED	0x00080000		// 524288, // 2008-11-26 by cmkwon, »ç¿ë ÈÄ ½Ã°£(Àý´ë½Ã°£) Á¦ÇÑ ¾ÆÀÌÅÛ ±¸Çö - Ãß°¡ ÇÔ

#define ITEM_ATTR_MISSION_MASTER				0x00100000		// 1048576, // 2008-12-15 by dhjin, ¹Ì¼Ç¸¶½ºÅÍ
#define ITEM_ATTR_ROBOT_ARMOR					0x00200000		// 2097152, // 2009-07-07 by cmkwon, ·Îº¿ ¾Æ¸Ó °ü·Ã Ã³¸® Ãß°¡ - ·Îº¿¾Æ¸Ó ¾ÆÀÌÅÛ ¼Ó¼º Ãß°¡

#define ITEM_ATTR_CANNOT_USE_INFINITY			0x00400000		// 4194304, // 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ´ÏÆ¼¿¡¼­ »ç¿ë ºÒ°¡ ¾ÆÀÌÅÛ
#define ITEM_ATTR_ONLY_USE_INFINITY				0x00800000		// 8388608, // 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ´ÏÆ¼¿¡¼­¸¸ »ç¿ë °¡´É ¾ÆÀÌÅÛ
#define ITEM_ATTR_CHECK_SKILL_RESISTANCE_STATE	0x01000000		// 16777216, // 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ·¹Áö½ºÅÁ½º Å°Æ® Àû¿ë ¿©ºÎ¸¦ Ã¼Å©ÇØ¾ßµÇ´Â ¾ÆÀÌÅÛ

#define ITEM_ATTR_NOT_STORE_SAVE				0x02000000		// 33554432, // 2011-06-07 by shcho, º£Æ®³² ¿äÃ» Ã¢°í¿¡¸¸ ÀúÀå ºÒ°¡ ¼Ó¼º ±¸Çö
#define ITEM_ATTR_WAREHOUSE_SHARE_BANNED		0x04000000		// 67108864, // 2012-01-14 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - °³ÀÎ Ã¢°í °øÀ¯ ±ÝÁö Ãß°¡ (ITEM_ATTR_ACCOUNT_POSSESSION °°Àº ¼Ó¼º°ú °°ÀÌ »ç¿ëÇØ¾ßÇÔ)
#define ITEM_ATTR_HYPER_GAMBLE_ITEM				0x08000000		// 134217728, // 2012-12-27 by hskim, ÇÏÀÌÆÛ Á¢µÎ/Á¡¹Ì ½Ã½ºÅÛ ±¸Çö

#define ITEM_ATTR_TRASH_ONLY					0x10000000		// 268435456, // 2015-06-13 Future, new Trading Option (only trashing allowed)

///////////////////////////////////////////////////////////////////////////////
// 2009-04-06 by cmkwon, ½ºÅ³ °ü·Ã ¼Ó¼º Ãß°¡¾È ±¸Çö(»ç¿ë ½Ã°£ ÀúÀå) - ITEM.ItemAttribute ÇÊµå¸¦ »ç¿ëÇÑ´Ù.
#define SKILL_ATTR_STORE_USING_TIME				0x00000001		// 1, // 2009-04-06 by cmkwon, ½ºÅ³ °ü·Ã ¼Ó¼º Ãß°¡¾È ±¸Çö(»ç¿ë ½Ã°£ ÀúÀå) - 1, ½ºÅ³ »ç¿ë ½Ã°£ ÀúÀåÇØ¾ß ÇÏ´Â ½ºÅ³
#define SKILL_ATTR_PARTY_FORMATION_SKILL		0x00000002		// 2, // 2009-08-03 by cmkwon, EP3-4 Æí´ë ´ëÇü ½ºÅ³ ±¸Çö - Æí´ë ´ëÇü ½ºÅ³

#define SIZE_MAX_STORE_ITEM		sizeof(ITEM_GENERAL)		// check: ¾ÆÀÌÅÛÀÇ Å¸ÀÔ Áß °¡Àå Å©±â°¡ Å« ±¸Á¶Ã¼ÀÇ Å©±â

// 2012-10-10 by hskim, ±â°£Á¦ ¼Ó¼º ±¸Çö (±â°£Á¦ ¿ÜÇü)

// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - #define ¼±¾ð
#define SHAPE_STAT_INIT_LEVEL					0		// ÃÊ±âÈ­ Lv
#define SHAPE_STAT_MAX_LEVEL					10		// ÀÎÃ¦Æ® °¡´É ÃÖ´ë Lv
#define SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL		10		// ±â°£ ¹«Á¦ÇÑ ½ÃÀÛ Lv

#define COLLECTION_TYPE_ARMOR					1		// ¾Æ¸Ó ÄÃ·º¼Ç Å¸ÀÔ
#define COLLECTION_XAM_ENCHANT_RETURN_ITEMNUM	7006380		// ÃÖ´ë °­È­ÀÌ»ó½Ã µÇµ¹·ÁÁÙ ALL±â¾î ¿ÜÇü Ä¸½¶ ¾ÆÀÌÅÛ ¹øÈ£
// end 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - #define ¼±¾ð

struct FIXED_TERM_INFO
{
	BOOL			bActive;			// Àû¿ë ¿©ºÎ
	ATUM_DATE_TIME	StartDate;			// ½ÃÀÛ ½Ã°£
	ATUM_DATE_TIME	EndDate;			// Á¾·á ½Ã°£

	UID32_t			TimerUID;			// ³»ºÎ ½Ã°£ °ü¸®¿ë UID (¼­¹ö¿¡¼­¸¸ »ç¿ë, Å¬¶ó¿¡¼­´Â »ç¿ë ±ÝÁö)
	
	// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ±¸Á¶Ã¼ º¯¼ö Ãß°¡
	ItemNum_t		nStatShapeItemNum;	// ¿Üº¯Å¶ ¾ÆÅÛ ¹øÈ£
	BYTE			nStatLevel;			// ·¹º§
	
	FIXED_TERM_INFO()
	{
		memset(this, 0x00, sizeof(FIXED_TERM_INFO));
	}
	// end 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ±¸Á¶Ã¼ º¯¼ö Ãß°¡
};

// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó - ÀÌº¥Æ® ¸®½ºÆ® ±¸Á¶Ã¼ ¼±¾ð
struct MONTHLY_ARMOR_EVNET_INFO
{
	INT				nEventUID;			// ÀÌº¥Æ® ¹øÈ£
	ATUM_DATE_TIME	atStartDate;		// ÀÌº¥Æ® ½ÃÀÛ ±â°£
	ATUM_DATE_TIME	atEndDate;			// ÀÌº¥Æ® Á¾·á ±â°£
	ItemNum_t		nArmorItemNum;		// ÀÌ´ÞÀÇ ¾Æ¸Ó ¿ÜÇü
	ItemNum_t		nArmorSourceIndex;	// ÀÌ´ÞÀÇ ¾Æ¸Ó ¿ÜÇü
	ItemNum_t		nOptionItemNum;		// ¿É¼Ç ¾ÆÀÌÅÛ
};
typedef vector<MONTHLY_ARMOR_EVNET_INFO>	vectMONTHLY_ARMOR_EVNET_INFO;
// end 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó - ÀÌº¥Æ® ¸®½ºÆ® ±¸Á¶Ã¼ ¼±¾ð

typedef enum FIXED_TERM_TYPE
{
	FIXED_TERM_NONE			= 0,			// ¾øÀ½ 
	FIXED_TERM_SHAPE		= 1,			// ±â°£Á¦ ¿ÜÇü
	FIXED_TERM_ITEM			= 2,			// ¾ÆÀÌÅÛ ±â°£Á¦
	FIXED_TERM_NULL			= 3				// NULL
};
// end 2012-10-10 by hskim, ±â°£Á¦ ¼Ó¼º ±¸Çö (±â°£Á¦ ¿ÜÇü)

struct ITEM_BASE
{
	BYTE		Kind;							// ¾ÆÀÌÅÛ Á¾·ù(±â°üÆ÷, ºö, ·ÎÄÏ, ½ºÅ³.....)
	UID64_t		UniqueNumber;					// ¾ÆÀÌÅÛ °íÀ¯¹øÈ£
	INT			ItemNum;						// ¾ÆÀÌÅÛ ¹øÈ£
	ITEM		*ItemInfo;						// ITEM¿¡ ´ëÇÑ pointer
};

// ÀÏ¹Ý ¾ÆÀÌÅÛ(¹«±â·ù, ¹æ¾î·ù, ...)
struct ITEM_GENERAL : public ITEM_BASE
{
	// store-item-specific fields
	UID32_t			AccountUniqueNumber;
	UID32_t			Possess;					// CharacterUniqueNumber or POSSESS_STORE_NUMBER
	BYTE			ItemStorage;				// 0:Ä³¸¯ÅÍÀÎº¥, 1:Ã¢°í // 2005-12-07 by cmkwon, ÇÑ°èÁ¤ÀÇ Ä³¸¯°£ Ã¢°í °øÀ¯¸¦ ¸·´Â´Ù. ITEM_IN_XXX
	BYTE			Wear;						// 0: ¹ÌÀåÂø, 1: ÀåÂø, 2:ÀåÀü, WEAR_XXX
	INT				CurrentCount;				// ¹«±â: ³²Àº ¹ß¼ö, ¿¡³ÊÁö: ³²Àº °³¼ö
// 2009-08-25 by cmkwon, »ç¿ëÇÏÁö ¾Ê´Â ÇÊµå Á¦°Å(td_Store.ScarcityNumber) - 
//	LONGLONG		ScarcityNumber;
	INT				ItemWindowIndex;			// °ÔÀÓ È­¸é¿¡¼­ ¾ÆÀÌÅÛ Ã¢¿¡ µé¾î°¡´Â ÀÚ¸®
	SHORT			NumOfEnchants;				// check: ¾ÆÁ÷ »ç¿ëÇÏÁö ¾ÊÀ½! 20031106, kelovon // Àû¿ëÇÑ enchantÀÇ ¼ö, 0ÀÌ¸é ¾Æ¹«°Íµµ Àû¿ëÇÏÁö ¾ÊÀ½
	INT				PrefixCodeNum;				// Á¢µÎ»ç, ¾øÀ¸¸é 0
	INT				SuffixCodeNum;				// Á¢¹Ì»ç, ¾øÀ¸¸é 0
	// derived from 'struct ITEM'
	float			CurrentEndurance;			// ÀÏ¹Ý ¾ÆÀÌÅÛ: ³²Àº ³»±¸µµ, ¿¡³ÊÁö·ù(TIMED_HP_UP): ³²Àº ½Ã°£
	
	INT				ColorCode;				// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - EffectItemNum, ½ÇÁ¦·Î´Â ¹«±âÀÇ ÅºµÎ ÀÌÆåÆ® ItemNumÀ» ÀÇ¹ÌÇÑ´Ù, // Æ©´×½Ã ¾Æ¸ÓÀÇ ColorCode
	INT				ShapeItemNum;			// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - 
	UID64_t			MainSvrItemUID;			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - Main¼­¹ö ¾ÆÀÌÅÛ UID Ãß°¡

	INT				UsingTimeStamp;				// ¾ÆÀÌÅÛÀÌ »ç¿ëµÈ ½Ã°£ ÃÊ´ÜÀ§(second)
	ATUM_DATE_TIME	UsingStartTime;				// ¾ÆÀÌÅÛ »ç¿ë ½ÃÀÛ ½Ã°£
	float			DesWeight;					// Áß·® ÀÎÃ¦Æ® ¼öÄ¡, 2006-01-24 by cmkwon
	ATUM_DATE_TIME	CreatedTime;				// ¾ÆÀÌÅÛ »ý¼º ½Ã°£, 2006-09-29 by cmkwon Ãß°¡ ÇÔ - ÀÏÁ¤ ½Ã°£ ÈÄ ÀÚµ¿ »èÁ¦µÇ´Â ¾ÆÀÌÅÛ

	INT				CoolingTimeStamp;				// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	ATUM_DATE_TIME	CoolingStartTime;				// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ

	FIXED_TERM_INFO	FixedTermShape;					// 2012-10-10 by hskim, ±â°£Á¦ ¼Ó¼º ±¸Çö (±â°£Á¦ ¿ÜÇü)

	// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó
	ItemNum_t		nMonthlyOptionItemNum;		// ÀÌ´ÞÀÇ ¾Æ¸Ó ¿É¼Ç ¾ÆÀÌÅÛ ¹øÈ£ (ÇØ´ç ¾ÆÀÌÅÛ¿¡ µî·ÏµÈ DexParamÀ» Àû¿ë)
	ATUM_DATE_TIME	atMonthlyEventEndDate;		// ÀÌ´ÞÀÇ ¾Æ¸Ó ÀÌº¥Æ® Á¾·á½Ã°£ 
	// end 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó

	inline ITEM_GENERAL()
	{// 2007-11-27 by cmkwon, ¼±¹°ÇÏ±â ·Î±× ¼öÁ¤ - Ãß°¡ÇÔ
		memset(this, 0x00, sizeof(ITEM_GENERAL));
	}
#ifdef _ATUM_SERVER

	inline ITEM_GENERAL(ITEM* pItemInfo)
	{
		memset(this, 0x00, sizeof(ITEM_GENERAL));
		if (pItemInfo != NULL)
		{
			///////////////////////////////////////////////////////////////////////////////
			// ±âº»°ª ¼³Á¤
			Wear				= WEAR_NOT_ATTACHED;
			CurrentCount		= (IS_CHARGABLE_ITEM(pItemInfo->Kind)?pItemInfo->Charging:1);
// 2009-08-25 by cmkwon, »ç¿ëÇÏÁö ¾Ê´Â ÇÊµå Á¦°Å(td_Store.ScarcityNumber) - 
//			ScarcityNumber		= 0;
			ItemWindowIndex		= POS_INVALID_POSITION;
			NumOfEnchants		= 0;
			ColorCode			= 0;
			UsingTimeStamp		= ITEM_NOT_USING;
			ShapeItemNum		= 0;	// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - ShapeItemNum ÇÊµå Ãß°¡

			UsingStartTime.Reset();
			CoolingStartTime.Reset();				// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
			
			*this				= *pItemInfo;		// ITEMÀÇ Á¤º¸ ÇÒ´ç			
			ItemInfo			= pItemInfo;		// ItemInfo Pointer ÇÒ´ç
		}
	}
	inline ITEM_GENERAL& operator=(const ITEM& rhs)
	{
		this->ItemNum			= rhs.ItemNum;
		this->Kind				= rhs.Kind;
		this->CurrentEndurance	= rhs.Endurance;
		return *this;
	}
#ifndef _ATUM_ADMINTOOL		// 2005-11-26 by cmkwon
	void * operator new(size_t size);
	void operator delete(void* p);
#endif // end_#ifndef _ATUM_ADMINTOOL
#endif // _ATUM_SERVER

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-15 by cmkwon, ¸â¹ö½¬ À¯Àú´Â ÅºÃ¢ÀÌ µÎ¹è·Î Ä¿Áø´Ù
	int GetMaxBulletCount(BOOL i_bIsMembershipUser)
	{
		if(NULL == ItemInfo)
		{
			return 0;
		}

		if(FALSE == i_bIsMembershipUser)
		{
			return ItemInfo->Charging;
		}

// 2007-10-15 by cmkwon, º£Æ®³²À» Á¦¿ÜÇÑ, ¸â¹ö½± À¯Àú´Â 2¹è
#if !defined(SERVICE_TYPE_VIETNAMESE_SERVER_1)
		return 2*ItemInfo->Charging;
#endif
		return ItemInfo->Charging;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - ShapeItemNum ÇÊµå Ãß°¡
	INT GetShapeItemNum(void)
	{
		if(FALSE == IS_ENABLE_CHANGE_ShapeItemNum(Kind))
		{
			return this->ItemInfo->ItemNum;
		}

		if(0 != ShapeItemNum)
		{
			return ShapeItemNum;
		}

		return this->ItemInfo->ItemNum;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, ±×·¡ÇÈ ¸®¼Ò½º º¯°æ ½Ã½ºÅÛ ±¸Çö - ShapeItemNum ÇÊµå Ãß°¡
	INT GetEffectItemNum(void)
	{
		if(FALSE == IS_ENABLE_CHANGE_EffectItemNum(Kind))
		{// 2009-08-26 by cmkwon, ÇöÀç´Â ¹«±â¸¸ Áö¿ø
			return 0;
		}

		if(0 != ColorCode)
		{
			return ColorCode;
		}

		return this->ItemInfo->ItemNum;
	}
};

struct LOG_GUILDSTORE_ITEM_INFO
{// 2006-09-27 by dhjin, ¿©´Ü ·Î±× Å×ÀÌºí¿¡¼­ ¾ò¾î¿À´Â °ª ÀúÀå ±¸Á¶Ã¼
	BYTE			LogType;
	ATUM_DATE_TIME	Time;
	UID32_t			GuildUID;
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID64_t			ItemUID;
	INT				ItemNum;
	CHAR			ItemName[SIZE_MAX_ITEM_NAME];
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				ItemResultCount;
	INT				ItemChangeCount;
};

// ½ºÅ³·ù ¾ÆÀÌÅÛ
struct ITEM_SKILL : public ITEM_BASE
{
	// store-item-specific fields
	UID32_t		AccountUniqueNumber;
	UINT		Possess;
	INT			ItemWindowIndex;				// °ÔÀÓ È­¸é¿¡¼­ ¾ÆÀÌÅÛ Ã¢¿¡ µé¾î°¡´Â ÀÚ¸®
	ATUM_DATE_TIME	UseTime;					// 2006-11-17 by dhjin, ½ºÅ³ »ç¿ëÇÑ ½Ã°£					

#ifdef _ATUM_SERVER
//	ITEM_SKILL() {};
	inline ITEM_SKILL(ITEM* pItem)
	{
		if (pItem != NULL)
		{
			*this = *pItem;
			ItemInfo = pItem;
		}
	}
	inline ITEM_SKILL& operator=(const ITEM& rhs)
	{
		this->ItemNum = rhs.ItemNum;
		this->Kind = rhs.Kind;

		return *this;
	}

	void * operator new(size_t size);
	void operator delete(void* p);
#endif // _ATUM_SERVER
};


///////////////////////////////////////////////////////////////////////////////
// 2009-08-03 by cmkwon, EP3-4 Æí´ë ´ëÇü ½ºÅ³ ±¸Çö - 
typedef ez_map<INT, ITEM_SKILL*>	mapINT2ItemSkillPtr;		// ItemNum and ITEN_SKILL pointer


inline int CopyItem2Buffer(char *pBuffer, ITEM_BASE *pItem)	// returns sizeof(ITEM_XXX)
{
	int retSize = 0;
	if (IS_GENERAL_ITEM(pItem->Kind))
	{
		*(ITEM_GENERAL*)pBuffer = *(ITEM_GENERAL*)pItem;
		retSize += sizeof(ITEM_GENERAL);
	}
	else if (IS_SKILL_ITEM(pItem->Kind))
	{
		*(ITEM_SKILL*)pBuffer = *(ITEM_SKILL*)pItem;
		retSize += sizeof(ITEM_SKILL);
	}
	else
	{
		// not reachable
		assert(0);
	}

	return retSize;
}

///////////////////////////////////////////////////////////////////////////////
// Enchant
///////////////////////////////////////////////////////////////////////////////
struct ENCHANT
{
	UID64_t		TargetItemUniqueNumber;
	INT			TargetItemNum;				// ´ë»ó ¾ÆÀÌÅÛÀÇ ItemNum
	INT			EnchantItemNum;				// enchant·Î ¾²ÀÎ ¾ÆÀÌÅÛÀÇ ItemNum
	// 2013-01-15 by bckim, ÀÎÃ¦Æ®¾ÆÀÌÅÛ ½ÃÄö½º³Ñ¹ö Ãß°¡
	UINT64		SequenceNumber_DB;

//#ifdef _ATUM_SERVER
//	void * operator new(size_t size);
//	void operator delete(void* p);
//#endif // _ATUM_SERVER

	// 2013-01-15 by bckim, ÀÎÃ¦Æ®¾ÆÀÌÅÛ ½ÃÄö½º³Ñ¹ö Ãß°¡
	ENCHANT()
	{
		TargetItemUniqueNumber = 0;
		TargetItemNum = 0;
		EnchantItemNum = 0;
		SequenceNumber_DB = 0;
	}
};

// 2013-01-15 by bckim, ÀÎÃ¦Æ®¾ÆÀÌÅÛ ½ÃÄö½º³Ñ¹ö Ãß°¡
struct ENCHANT_ITEM_SORT_BY_SEQUENCE_NUMBER : binary_function<ENCHANT, ENCHANT, bool>
{
	bool operator()(ENCHANT Param1, ENCHANT Param2)
	{
        return Param1.SequenceNumber_DB < Param2.SequenceNumber_DB; 	// ¿À¸§ Â÷¼ø Á¤·Ä
    }
};
// End. 2013-01-15 by bckim, ÀÎÃ¦Æ®¾ÆÀÌÅÛ ½ÃÄö½º³Ñ¹ö Ãß°¡

typedef vector<ENCHANT>						vectENCHANT;		// 2013-02-27 by bckim, ÀÎÃ¦Æ® ¸®½ºÆ®¼ø¼­ ÀçÁ¤·Ä(¾ÆÀÌÅÛ³Ñ¹ö¼ø¼­->ÀÎÃ¦Æ®µÈ¼ø¼­)		

struct ENCHANT_INFO
{
	INT			EnchantItemNum;			// ÇÊ¿äÇÑ Àç·á ¾ÆÀÌÅÛ ³Ñ¹ö
	INT			EnchantItemCount;		// ÇÊ¿äÇÑ Àç·á °³¼ö
	INT			EnchantCost;			// Àç·á Á¶ÇÕÀ» À§ÇÑ ¼ö¼ö·á
	Prob10K_t	ProbabilityPerLevel[SIZE_MAX_ENCHANT_PER_ITEM];	// °¢ ·¹º§¿¡ ´ëÇÑ ÀÎÃ¾Æ® ½Ã Á¶ÇÕµÉ È®·ü
};

// 2013-07-01 by bckim, 10ÀÎÃ¦Æ® ¹Ì¸¸ ÇÏÀÌÆÛÄ«µåÃ³¸®, ·Î±×Ãß°¡, ¿î¿µÅø º¸¿Ï
struct SUB_ENCHANT_INFO_SORT_BY_SEQUENCE_NUMBER : binary_function<SUB_ENCHANT_INFO, SUB_ENCHANT_INFO, bool>
{
	bool operator()(SUB_ENCHANT_INFO Param1, SUB_ENCHANT_INFO Param2)
	{
        return Param1.SequenceNumber_DB < Param2.SequenceNumber_DB; 	// ¿À¸§ Â÷¼ø Á¤·Ä
    }
};
// End. 2013-07-01 by bckim, 10ÀÎÃ¦Æ® ¹Ì¸¸ ÇÏÀÌÆÛÄ«µåÃ³¸®, ·Î±×Ãß°¡, ¿î¿µÅø º¸¿Ï


#define COUNT_ITEM_MIXING_SOURCE	5
#define COUNT_MAX_MIXING_COUNT		9999	// 2008-03-17 by cmkwon, Á¶ÇÕ ½Ã½ºÅÛ ±â´É Ãß°¡ - ÇÑ¹ø¿¡ Á¶ÇÕ °¡´ÉÇÑ ÃÖ´ëÄ«¿îÆ®

// start 2012-03-16 by hskim, DB Á¶ÇÕ½Ä Á¤¸® (°íÁ¤°ª ¼ö½ÄÀ¸·Î º¯°æ)
#define COUNT_ITEM_FIX_MIXING_SOURCE		2
#define INDEX_ITEM_FIX_MIXING_EQUIPMENT		0		// Àåºñ ÀÎµ¦½º (¹«±â/¾Æ¸Ó)
#define INDEX_ITEM_FIX_MIXING_EFFECT_CARD	1		// È¿°úÅÛ ÀÎµ¦½º (¹«±â ÀÌÆåÆ® º¯°æ Ä«µå/¹«±â ¿ÜÇü º¯°æ Ä«µå/¾Æ¸¶ ¿ÜÇü º¯°æ Ä«µå)

#define KIND_ITEM_FIX_MIXING_NONE			0		// ¾øÀ½
#define KIND_ITEM_FIX_MIXING_SHAPE_PRIMARY_WEAPON_A		1		// ¹«±â ¿ÜÇü - ±âº» ¹«±â A ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_PRIMARY_WEAPON_BMI	2		// ¹«±â ¿ÜÇü - ±âº» ¹«±â BMI ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_SECOND_WEAPON_A		3		// ¹«±â ¿ÜÇü - °í±Þ ¹«±â A ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_SECOND_WEAPON_BMI	4		// ¹«±â ¿ÜÇü - °í±Þ ¹«±â BMI ±â¾î
#define KIND_ITEM_FIX_MIXING_EFFECT_PRIMARY_WEAPON		5		// ¹«±â ¿ÜÇü - ±âº» ¹«±â ÀÌÆåÆ®
#define KIND_ITEM_FIX_MIXING_EFFECT_SECOND_WEAPON		6		// ¹«±â ¿ÜÇü - °í±Þ ¹«±â ÀÌÆåÆ®
#define KIND_ITEM_FIX_MIXING_SHAPE_DEFENSE_B			7		// ¾Æ¸Ó ¿ÜÇü - B ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_DEFENSE_M			8		// ¾Æ¸Ó ¿ÜÇü - M ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_DEFENSE_A			9		// ¾Æ¸Ó ¿ÜÇü - A ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_DEFENSE_I			10		// ¾Æ¸Ó ¿ÜÇü - I ±â¾î
#define KIND_ITEM_FIX_MIXING_SHAPE_DEFENSE_INIT			11		// ¾Æ¸Ó ¿ÜÇü - ÃÊ±âÈ­
#define KIND_ITEM_FIX_MIXING_SHAPE_SECOND_EFFECT_INIT	12		// ÀÌÆåÆ® ¿ÜÇü - °í±Þ ¹«±â ÃÊ±âÈ­ 
#define KIND_ITEM_FIX_MIXING_SHAPE_PRIMARY_EFFECT_INIT	13		// ÀÌÆåÆ® ¿ÜÇü - ±âº» ¹«±â ÃÊ±âÈ­ 

#define KIND_ITEM_FIX_MIXING_SHAPE_PET_ITEM				21		// ÆÄÆ®³Ê ¿ÜÇü - // 2012-12-03 by hskim, ÆÄÆ®³Ê ±â°£Á¦ ¿ÜÇü º¯°æ
// end2012-03-16 by hskim, DB Á¶ÇÕ½Ä Á¤¸® (°íÁ¤°ª ¼ö½ÄÀ¸·Î º¯°æ)
 
struct ITEM_MIXING_INFO
{
	INT				TargetItemNum;			// ¿Ï¼ºµÈ Àç·á ItemNum
	Prob10K_t		MixingProbability;		// Á¦ÀÛµÉ È®·ü(1~10000)
	INT				MixingCost;				// Àç·á Á¶ÇÕÀ» À§ÇÑ ¼ö¼ö·á
	ITEM_W_COUNT	SourceItem[COUNT_ITEM_MIXING_SOURCE];	// ÇÊ¿äÇÑ Àç·á ItemNum ¹× °³¼ö
	INT				NumOfSourceItems;		// Á¶ÇÕÇÒ ¾ÆÀÌÅÛÀÇ °³¼ö(·Îµù ½Ã °è»êÇØ¼­ ÇÒ´çÇÏ±â!)
	BYTE			Visible;				// È­¸é¿¡ Ç¥½Ã ¿©ºÎ - // 2013-05-06 by hskim, ¾ÆÀÌÅÛ ¹Ì¸® º¸±â (¼Ó¼º °ª Ãß°¡)

	// 2013-05-06 by hskim, ¾ÆÀÌÅÛ ¹Ì¸® º¸±â (¼Ó¼º °ª Ãß°¡) 
	ITEM_MIXING_INFO()
	{
		TargetItemNum		= 0;				// ¿Ï¼ºµÈ Àç·á ItemNum
		MixingProbability	= 0;				// Á¦ÀÛµÉ È®·ü(1~10000)
		MixingCost			= 0;				// Àç·á Á¶ÇÕÀ» À§ÇÑ ¼ö¼ö·á
		memset(SourceItem, 0x00, sizeof(ITEM_W_COUNT) * COUNT_ITEM_MIXING_SOURCE); // ÇÊ¿äÇÑ Àç·á ItemNum ¹× °³¼ö  
		NumOfSourceItems	= 0;				// Á¶ÇÕÇÒ ¾ÆÀÌÅÛÀÇ °³¼ö(·Îµù ½Ã °è»êÇØ¼­ ÇÒ´çÇÏ±â!)
		Visible				= 0;							// È­¸é¿¡ Ç¥½Ã ¿©ºÎ - 
	}
	// end 2013-05-06 by hskim, ¾ÆÀÌÅÛ ¹Ì¸® º¸±â (¼Ó¼º °ª Ãß°¡) 
};

// start 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo
struct ITEM_MIXING_INFO_TOOL
{
	INT				UniqueID;				// À¯´ÏÅ© ID
	INT				TargetItemNum;			// ¿Ï¼ºµÈ Àç·á ItemNum
	Prob10K_t		MixingProbability;		// Á¦ÀÛµÉ È®·ü(1~10000)
	INT				MixingCost;				// Àç·á Á¶ÇÕÀ» À§ÇÑ ¼ö¼ö·á
	BYTE			Visible;				// È­¸é¿¡ Ç¥½Ã ¿©ºÎ - // 2013-05-06 by hskim, ¾ÆÀÌÅÛ ¹Ì¸® º¸±â (¼Ó¼º °ª Ãß°¡)
};

struct ITEM_MIXING_ELEMENT_TOOL
{
	INT				UniqueID;				// À¯´ÏÅ© ID
	ITEM_W_COUNT	SourceItem;				// ÇÊ¿äÇÑ Àç·á ItemNum
};
// end 2012-02-20 by hskim, DB Á¤±ÔÈ­ - ItemMixingInfo

// DBGOUT
#ifdef _ATUM_SERVER
#ifdef _DEBUG
extern void DbgOut (LPCTSTR pFormat, ...);
#define DBGOUT ::DbgOut
#else
#define DBGOUT __noop
#endif
#endif // _ATUM_SERVER

#define SIZE_DES_PARAM_PER_RARE_ITEM_INFO	9

#define RARE_ITEM_USE_TYPE_NORMAL			1		// ÀÏ¹Ý(µå·Ó½Ã) »ç¿ë
#define RARE_ITEM_USE_TYPE_GAMBLE			2		// °·ºí »ç¿ë
#define RARE_ITEM_USE_TYPE_SUPERGAMBLE		4		// ½´ÆÛ°·ºí »ç¿ë
#define RARE_ITEM_USE_TYPE_HYPERGAMBLE		8		// ÇÏÀÌÆÛ°·ºí »ç¿ë - // 2012-12-27 by hskim, ÇÏÀÌÆÛ Á¢µÎ/Á¡¹Ì ½Ã½ºÅÛ ±¸Çö

struct RARE_ITEM_INFO
{
	INT			CodeNum;			// Á¢µÎ»ç, Á¢¹Ì»ç ±¸ºÐµÊ
	char		Name[SIZE_MAX_RARE_FIX_NAME];
	INT			ReqUseType;			// BitFlag »ç¿ë
	INT			ReqMinLevel;
	INT			ReqMaxLevel;
	BYTE		ReqItemKind;		// ÇÊ¿ä¾ÆÀÌÅÛÁ¾·ù, ·¹¾î°¡ Àû¿ëµÇ´Â ¾ÆÀÌÅÛ Á¾·ù, ITEMKIND_XXX
	GEAR_STAT	ReqGearStat;		// ÇÊ¿ä ±â¾î ½ºÅÈ
	BYTE		DesParameter[SIZE_DES_PARAM_PER_RARE_ITEM_INFO];
	FLOAT		ParameterValue[SIZE_DES_PARAM_PER_RARE_ITEM_INFO];
	Prob100K_t	Probability;		// ·¹¾î ¾ÆÀÌÅÛ ¼º°ø È®·ü, ¹üÀ§: 1 ~ 100000
};
typedef vector<RARE_ITEM_INFO*>			vectRARE_ITEM_INFOPtrList;		// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - 
typedef map<int, RARE_ITEM_INFO*>		mapRARE_ITEM_INFOPtrList;		// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - <CodeNum, RARE_ITEM_INFO*>

// 1000¹ø´ë Á¢µÎ»ç 5000¹ø´ë Á¢¹Ì»ç 1Çü ¾ÆÀÌÅÛ
// 2000¹ø´ë Á¢µÎ»ç 6000¹ø´ë Á¢¹Ì»ç 2Çü ¾ÆÀÌÅÛ
#define IS_RARE_PREFIX(_CODE_NUM)	(_CODE_NUM > 0 && _CODE_NUM < 5000)
#define IS_RARE_SUFFIX(_CODE_NUM)	(_CODE_NUM >= 5000)


///////////////////////////////////////////////////////////////////////////////
// 2010-04-09 by cmkwon, ÀÎÇÇ2Â÷ Ãß°¡ ¼öÁ¤(´Ü°èº° º¸»ó Ãß°¡) - DROP_TYPE_XXX
typedef INT		DropType_t;
#define DROP_TYPE_DEAD_MONSTER						0		// ÀÏ¹Ý ¸ó½ºÅÍ ÆøÆÄ½Ã µå¶øµÇ´Â ¾ÆÀÌÅÛ
#define DROP_TYPE_DEAD_ALL_INFINITY_KEY_MONSTER		1		// ÀÎÇÇ´ÏÆ¼ º¸»óÀ¸·Î ÇØ´ç ´Ü°è Å°¸ó½ºÅÍ°¡ ¸ðµÎ ÆøÆÄ½Ã µå¶øµÇ´Â ¾ÆÀÌÅÛ ¸®½ºÆ®
#define DROP_TYPE_SP_LOST_NATION_REWARD				2		// 2015-09-26 Future, reward for Looser Nation by SP Win

struct MONSTER2ITEM
{
	INT				MonsterUniqueNumber;	// ¸ó½ºÅÍ Á¾·ù
	INT				ItemNumber;				// ¾ÆÀÌÅÛ°íÀ¯³Ñ¹ö
	INT				MinCount;				// »ý¼ºÇÒ ¾ÆÀÌÅÛÀÇ ÃÖ¼Ò °³¼ö
	INT				MaxCount;				// »ý¼ºÇÒ ¾ÆÀÌÅÛÀÇ ÃÖ´ë °³¼ö, ¸¸¾à °³¼ö°¡ Á¤ÇØÁ® ÀÖ´Ù¸é MinCount = MixCount
	Prob1000K_t		Probability;			// »ý¼º È®·ü: 0 ~ 1000000
	Prob100K_t		PrefixProbability;		// Á¢µÎ»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
	Prob100K_t		SuffixProbability;		// Á¢¹Ì»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
	INT				QuestIndex;				// Äù½ºÆ® ¾ÆÀÌÅÛÀÏ °æ¿ì QuestIndex ÀúÀå, ¾Æ´Ï¸é 0
	DropType_t		DropType;				// DROP_TYPE_XXX // 2010-04-09 by cmkwon, ÀÎÇÇ2Â÷ Ãß°¡ ¼öÁ¤(´Ü°èº° º¸»ó Ãß°¡) - 
};

struct CHARACTER2ITEM		// 2006-03-02 by cmkwon
{
	INT				ItemNumber;				// ¾ÆÀÌÅÛ°íÀ¯³Ñ¹ö
	INT				MinCount;				// »ý¼ºÇÒ ¾ÆÀÌÅÛÀÇ ÃÖ¼Ò °³¼ö
	INT				MaxCount;				// »ý¼ºÇÒ ¾ÆÀÌÅÛÀÇ ÃÖ´ë °³¼ö, ¸¸¾à °³¼ö°¡ Á¤ÇØÁ® ÀÖ´Ù¸é MinCount = MixCount
	Prob1000K_t		Probability;			// »ý¼º È®·ü: 0 ~ 1000000
	INT				QuestIndex;				// Äù½ºÆ® ¾ÆÀÌÅÛÀÏ °æ¿ì QuestIndex ÀúÀå, ¾Æ´Ï¸é 0
};
typedef vector<CHARACTER2ITEM>			vectCHARACTER2ITEM;

struct CHARACTER2ITEMLIST		// 2006-03-02 by cmkwon
{
	BYTE				InfluenceType0;			// ¼¼·Â Å¸ÀÔÀ¸·Î
	USHORT				UnitKindMask0;			// À¯´ÖÀÇ Á¾·ù
	vectCHARACTER2ITEM	vectCharacter2Item;
};
typedef vector<CHARACTER2ITEMLIST>		vectCHARACTER2ITEMLIST;


#ifndef _ATUM_CLIENT
struct PROJECTINFO
{
	int				m_useTileSetIndex;
	MapIndex_t		m_nMapIndex;
	USHORT			m_nCreateChannelCounts;
	char			m_strFieldIP[SIZE_MAX_IPADDRESS];
	short			m_sFieldListenPort;
	short			m_sFieldUDPPort;
	char			m_strNPCIP[SIZE_MAX_IPADDRESS];
	short			m_sNPCListenPort;
	short			m_sNPCUDPPort;
	short			m_sXSize;						// mapÀÇ °¡·Î Å¸ÀÏ ¼ö
	short			m_sYSize;						// mapÀÇ ¼¼·Î Å¸ÀÏ ¼ö
	short			m_sMaximumAltitude;				// mapÀÇ ÃÖ´ë ÀÌµ¿ °¡´É °íµµ
	short			m_sMinimumAltitude;				// mapÀÇ ÃÖÀú ÀÌµ¿ °¡´É °íµµ
	short			m_sWaterAltitude;				// mapÀÇ ¹° ³ôÀÌ
	float			m_fFrontPositionDistance;		// 2004-04-09 by cmkwon, ¸ó½ºÅÍÀÇ ´ÙÀ½ ÁÂÇ¥¸¦ ±¸ÇÏ±â À§ÇÑ °Å¸®, ¸Êº°·Î ´Ù¸¦¼ö ÀÖ´Ù.
	BOOL			m_bCreateNPCThread;
	BOOL			m_bAutoCreateMonster;
	MapIndex_t		m_DeadDefaultMapIndex;			// 2004-03-26 by cmkwon, Ä³¸¯ÀÌ Á×¾úÀ» ¶§ °¡¾ßÇÏ´Â default map°ú °ü·Ã
//	BOOL			m_bGuildWarMap;					// ±æµåÀü Àü¿ë ¸ÊÀÎÁö ¿©ºÎ
	BOOL			m_bAutoDPRecovery;				// 2004-03-26 by cmkwon, ÀÚµ¿ DP È¸º¹ ¿©ºÎ
//	BOOL			m_bTutorialMap;					// 2004-03-26 by cmkwon, Æ©Åä¸®¾ó ¸ÊÀÎÁö ¿©ºÎ
	DWORD			m_dwMaxUserCounts;				// 2004-11-22 by cmkwon, Ã¤³Î´ç Max À¯Àú Ä«¿îÆ®(È¥Àâµµ°è»êÀ» À§ÇØ ÇÊ¿ä) //2004-03-26 by cmkwon, ÇØ´ç ¸ÊÀÌ°¢ Ã¤³Î´ç »ç¿ë°¡´ÉÇÑ Bandwidth (config È­ÀÏ¿¡´Â Mbps ·Î ÀúÀå µÇ¾îÀÖ°í loading½Ã¿¡ bps ´ÜÀ§·Î º¯°æÇÑ´Ù)
	D3DXVECTOR3		m_CityWarpTargetPosition;		// µµ½Ã ¸ÊÀÇ °­Á¦ ¿öÇÁ Å¸°Ù ÁÂÇ¥ ¼³Á¤ À§ÇØ
	int				m_nUserVisibleDiameter;			// À¯Àú Move Àü¼Û °Å¸®, Áö¸§
	int				m_nMonsterVisibleDiameter;		// ¸ó½ºÅÍ Move Àü¼Û °Å¸®, Áö¸§
	int				m_nQuestIndexForWarp;			// ÀÌ¸ÊÀ¸·Î °¡±â À§ÇØ¼­´Â ÀÌÄù½ºÆ®¸¦ ²À ¿Ï·áÇØ¾ßÇÑ´Ù.
	int				m_nMapInfluenceType;			// 2005-12-28 by cmkwon, ¸ÊÀÇ ¼¼·Â ¼³Á¤(MAP_INFLUENCE_XXX)
	MapIndex_t		m_nBeforeMapIndex;				// 2006-08-02 by cmkwon, Å¸¼¼·Â¸Ê¿¡¼­ Á×¾úÀ» °æ¿ì ºÎÈ°½Ã ¿©±â¿¡ ¼³Á¤µÈ ¸Ê¿¡¼­ ºÎÈ°µÊ
	INT				m_VCNWarpObjectIndex;			// 2006-12-08 by cmkwon
	INT				m_ANIWarpObjectIndex;			// 2006-12-08 by cmkwon
	INT				m_TeleportWarpObjectIndex;		// 2007-09-15 by dhjin
	MapIndex_t		m_nBeforeMapIndex2;				// 2008-06-20 by cmkwon, EP3 ¹é¸Ê ½Ã½ºÅÛ ¼öÁ¤(¸Ê´ç ¹é¸ÊÀ» 2°³ ¼³Á¤ ÇÒ ¼ö ÀÖ´Ù) - 


	PROJECTINFO()
	{
	}
	PROJECTINFO(PROJECTINFO &i_refPrj)
	{
		*this = i_refPrj;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-20 by cmkwon, EP3 ¹é¸Ê ½Ã½ºÅÛ ¼öÁ¤(¸Ê´ç ¹é¸ÊÀ» 2°³ ¼³Á¤ ÇÒ ¼ö ÀÖ´Ù) - 
	BOOL IsValidBeforeMapIndex(void)
	{
		if(0 != m_nBeforeMapIndex
			|| 0 != m_nBeforeMapIndex2)
		{
			return TRUE;
		}

		return FALSE;
	}
};
#endif // _ATUM_CLIENT

struct GBUFFER
{
	char	ptr[SIZE_MAX_LONG_PACKET];
	int		size;

	void Reset() { size = 0; }
	char* GetPtr() { return ptr; }
	int GetSize() { ASSERT_ASSERT(size <= SIZE_MAX_LONG_PACKET); return size; }
};

#ifndef SIZE_MAX_PACKET
#define SIZE_MAX_PACKET						1492	// ÆÐÅ¶ ÃÖ´ë »çÀÌÁî(¿©·¯°³ÀÇ ¸Þ¼¼Áö°¡ ÇÏ³ªÀÇ ÆÐÅ¶À¸·Î Àü¼ÛµÉ¼ö ÀÖÀ½)
#endif

struct GBUFFER_SIZE_MAX_PACKET
{
	char	ptr[SIZE_MAX_PACKET];
	int		size;

	void Reset() { size = 0; }
	char* GetPtr() { return ptr; }
	int GetSize() { ASSERT_ASSERT(size <= SIZE_MAX_PACKET); return size; }
};

// Field ServerÀÇ IDÀÇ °ü¸®¸¦ À§ÇØ
struct SERVER_ID
{
	// member functions
	SERVER_ID() { Reset(); }
	SERVER_ID(char *IP, int port) { Reset(); SetValue(IP, port); }
	SERVER_ID(const char *szIPPort) { Reset(); SetValue(szIPPort); }
	inline void SetValue(char *IP, int port);
	inline void SetValue(const char *szIPPort);
	inline bool CompareValue(char *IP, int port);
	inline const char* GetString(char* buffer);
	inline const char* GetString(string& str);
	inline bool operator==(SERVER_ID &rhs);
	inline bool operator!=(SERVER_ID &rhs);
	inline void Reset() { memset(this, 0, sizeof(SERVER_ID)); }

	// member variables
	char	IP[SIZE_MAX_IPADDRESS];
	int		port;
};


void SERVER_ID::SetValue(char *IP, int port)
{
	strncpy(this->IP, IP, SIZE_MAX_IPADDRESS);
	this->port = port;
}

void SERVER_ID::SetValue(const char *szIPPort)
{
	char *token;
	char seps[] = " \t,";
	char buffer[64];
	strncpy(buffer, szIPPort, 64);

	token = strtok(buffer, seps);
	if (token == NULL) return;
	strncpy(IP, token, SIZE_MAX_IPADDRESS);
	token = strtok(NULL, seps);
	if (token == NULL) return;
	port = atoi(token);
}

bool SERVER_ID::CompareValue(char *IP, int port)
{
	return strncmp(this->IP, IP, SIZE_MAX_IPADDRESS) == 0 && this->port == port;
}

const char* SERVER_ID::GetString(char* buffer)
{
	if (strncmp(IP, "", SIZE_MAX_IPADDRESS) == 0)
	{
		sprintf(buffer, "0.0.0.0, %d", port);
	}
	else
	{
		sprintf(buffer, "%s, %d", IP, port);
	}
	return buffer;
}

const char* SERVER_ID::GetString(string& str)
{
	char port_str[10];
	sprintf(port_str, "%d", port);
	if (strncmp(IP, "", SIZE_MAX_IPADDRESS) == 0)
	{
		str = "0.0.0.0";
	}
	else
	{
		str = IP;
	}
	str += ", ";
	str += port_str;
	return str.c_str();
}

bool SERVER_ID::operator==(SERVER_ID &rhs)
{
	return (strncmp(IP, rhs.IP, SIZE_MAX_IPADDRESS) == 0 && port == rhs.port);
}

bool SERVER_ID::operator!=(SERVER_ID &rhs)
{
	return !(operator==(rhs));
}

inline bool operator<(const SERVER_ID& lhs, const SERVER_ID& rhs)
{
	bool ret;
	int res_strcmp = strncmp(lhs.IP, rhs.IP, SIZE_MAX_IPADDRESS);

	if (res_strcmp < 0)
	{
		ret = TRUE;
	}
	else if (res_strcmp > 0)
	{
		ret = FALSE;
	}
	else // lhs.IP == rhs.IP
	{
		ret = lhs.port < rhs.port;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// Version Management
///////////////////////////////////////////////////////////////////////////////
class VersionInfo
{
public:
	VersionInfo()
	{
		memset(this, 0x00, sizeof(VersionInfo));
		m_bIsValid			= FALSE;				// 2006-12-28 by cmkwon
	}

	VersionInfo(const char* string)
	{
		m_bIsValid			= FALSE;				// 2006-12-28 by cmkwon

		char *token;
		char seps[] = ".";
		char buffer[SIZE_MAX_VERSION_STRING];
		memset(buffer, 0x00, SIZE_MAX_VERSION_STRING);
		strncpy(buffer, string, SIZE_MAX_VERSION_STRING);

		token = strtok(buffer, seps);
		if(NULL == token)
		{// 2006-12-28 by cmkwon
			return;
		}
		version[0] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{// 2006-12-28 by cmkwon
			return;
		}
		version[1] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{// 2006-12-28 by cmkwon
			return;
		}
		version[2] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{// 2006-12-28 by cmkwon
			return;
		}
		version[3] = atoi(token);

		if( 0 == (version[0] | version[1] | version[2] | version[3]) )
		{// 2006-12-28 by cmkwon
			return;
		}

		m_bIsValid			= TRUE;				// 2006-12-28 by cmkwon
		sprintf(strBuf, "%d.%d.%d.%d", version[0], version[1], version[2], version[3]);
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsValidVersionInfo(void)
	/// \brief		
	/// \author		cmkwon
	/// \date		2006-12-28 ~ 2006-12-28
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsValidVersionInfo(void)
	{
		if( 0 == (version[0] | version[1] | version[2] | version[3]) )
		{
			return FALSE;
		}

		return m_bIsValid;
	}

	VersionInfo(const USHORT* ver)
	{
		version[0] = ver[0];
		version[1] = ver[1];
		version[2] = ver[2];
		version[3] = ver[3];
		sprintf(strBuf, "%d.%d.%d.%d", ver[0], ver[1], ver[2], ver[3]);

		if( 0 != (version[0] | version[1] | version[2] | version[3]) )
		{
			m_bIsValid = TRUE;
		}
	}

	inline void SetVersion(USHORT v0, USHORT v1, USHORT v2, USHORT v3)
	{
		version[0] = v0; version[1] = v1; version[2] = v2; version[3] = v3;
		sprintf(strBuf, "%d.%d.%d.%d", v0, v1, v2, v3);
	
		if( 0 != (version[0] | version[1] | version[2] | version[3]) )
		{
			m_bIsValid = TRUE;
		}
	}

	inline void SetVersion(const USHORT* ver)
	{
		version[0] = ver[0];
		version[1] = ver[1];
		version[2] = ver[2];
		version[3] = ver[3];
		sprintf(strBuf, "%d.%d.%d.%d", ver[0], ver[1], ver[2], ver[3]);
		if( 0 != (version[0] | version[1] | version[2] | version[3]) )
		{
			m_bIsValid = TRUE;
		}
	}

	inline void SetVersion(const char* string)
	{
		m_bIsValid		= FALSE;

		char *token;
		char seps[] = ".";
		char buffer[SIZE_MAX_VERSION_STRING];
		strncpy(buffer, string, SIZE_MAX_VERSION_STRING);

		token = strtok(buffer, seps);
		if(NULL == token)
		{
			return;
		}
		version[0] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return;
		}
		version[1] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return;
		}
		version[2] = atoi(token);

		token = strtok(NULL, seps);
		if(NULL == token)
		{
			return;
		}
		version[3] = atoi(token);

		sprintf(strBuf, "%d.%d.%d.%d", version[0], version[1], version[2], version[3]);

		if( 0 != (version[0] | version[1] | version[2] | version[3]) )
		{
			m_bIsValid = TRUE;
		}
	}

	inline const USHORT* GetVersion(USHORT* o_pVersion) const
	{
		o_pVersion[0] = version[0];
		o_pVersion[1] = version[1];
		o_pVersion[2] = version[2];
		o_pVersion[3] = version[3];

		return version;
	}

	inline const USHORT* GetVersion() const
	{
		return version;
	}

	inline char* GetVersionString()
	{
		if (strlen(strBuf) == 0)
		{
			sprintf(strBuf, "%d.%d.%d.%d", version[0], version[1], version[2], version[3]);
		}

		return strBuf;
	}

	inline BOOL Compare(VersionInfo *v)
	{
		if ( version[0] == v->GetVersion()[0] && version[1] == v->GetVersion()[1] && version[2] == v->GetVersion()[2] && version[3] == v->GetVersion()[3] )
		{
			return TRUE;
		}

		return FALSE;
	}

	static bool lt_data(VersionInfo* d1, VersionInfo* d2)
	{
		if (d1->GetVersion()[0] != d2->GetVersion()[0])
		{
			return d1->GetVersion()[0] < d2->GetVersion()[0];
		}
		if (d1->GetVersion()[1] != d2->GetVersion()[1])
		{
			return d1->GetVersion()[1] < d2->GetVersion()[1];
		}
		if (d1->GetVersion()[2] != d2->GetVersion()[2])
		{
			return d1->GetVersion()[2] < d2->GetVersion()[2];
		}
		if (d1->GetVersion()[3] != d2->GetVersion()[3])
		{
			return d1->GetVersion()[3] < d2->GetVersion()[3];
		}

		return FALSE;
	}

	static bool gt_data(VersionInfo* d1, VersionInfo* d2)
	{
		return !lt_data(d1, d2);
	}

	inline bool operator==(const VersionInfo & v)
	{
		if ( version[0] == v.GetVersion()[0] && version[1] == v.GetVersion()[1] && version[2] == v.GetVersion()[2] && version[3] == v.GetVersion()[3] )
		{
			return TRUE;
		}
		return FALSE;
	}

	inline bool operator!=(const VersionInfo & v)
	{
		return !(*this == v);
	}

	inline bool operator<(const VersionInfo & v)
	{
		if (this->GetVersion()[0] != v.GetVersion()[0])
		{
			return this->GetVersion()[0] < v.GetVersion()[0];
		}
		if (this->GetVersion()[1] != v.GetVersion()[1])
		{
			return this->GetVersion()[1] < v.GetVersion()[1];
		}
		if (this->GetVersion()[2] != v.GetVersion()[2])
		{
			return this->GetVersion()[2] < v.GetVersion()[2];
		}
		if (this->GetVersion()[3] != v.GetVersion()[3])
		{
			return this->GetVersion()[3] < v.GetVersion()[3];
		}

		return FALSE;
	}

	inline bool operator>(const VersionInfo & v)
	{
		if (this->GetVersion()[0] != v.GetVersion()[0])
		{
			return this->GetVersion()[0] > v.GetVersion()[0];
		}
		if (this->GetVersion()[1] != v.GetVersion()[1])
		{
			return this->GetVersion()[1] > v.GetVersion()[1];
		}
		if (this->GetVersion()[2] != v.GetVersion()[2])
		{
			return this->GetVersion()[2] > v.GetVersion()[2];
		}
		if (this->GetVersion()[3] != v.GetVersion()[3])
		{
			return this->GetVersion()[3] > v.GetVersion()[3];
		}

		return FALSE;
	}

	inline bool operator<=(const VersionInfo &v) { return !(*this > v); }
	inline bool operator>=(const VersionInfo &v) { return !(*this < v); }

protected:
	BOOL	m_bIsValid;				// 2006-12-28 by cmkwon, 
	USHORT	version[SIZE_MAX_VERSION];
	char	strBuf[SIZE_MAX_VERSION_STRING];
};
typedef mt_map<VersionInfo, VersionInfo>					mtmapVersionInfo;

bool operator<(const VersionInfo &v1, const VersionInfo &v2);

struct MAP_INFO
{
	MapIndex_t		MapIndex;						// 
	MapIndex_t		RenderMapIndex;					// Render °ü·Ã Á¤º¸´Â ÀÌ¸ÊÀÇ ÀÎµ¦½º Á¤º¸¿Í °°À½, ¾Æ·¡ ÆÄ¶ó¹ÌÅÍ´Â ¼³Á¤ÇÏÁö ¾Ê¾ÆµµµÊ
	MapIndex_t		BeforeMapIndex;					// 2006-08-02 by cmkwon, Å¸¼¼·Â¸Ê¿¡¼­ Á×¾úÀ» °æ¿ì ºÎÈ°½Ã ¿©±â¿¡ ¼³Á¤µÈ ¸Ê¿¡¼­ ºÎÈ°µÊ
	char			MapName[SIZE_MAX_MAP_NAME];
	SHORT			MapInfluenceType;				// ¸ÊÀÇ ¼¼·Â Å¸ÀÔ ¼³Á¤(MAP_INFLUENCE_XXX)
	MapIndex_t		CityMapIndex;					// µµ½ÃÁ¡·ÉÀü Á¡·É °Ý³³°í ¸ÊÀÎµ¦½º
	INT				CityWarQuestIndex;				// 
	INT				CityWarQuestNPCIndex;
	INT				CityWarQuestMonsterUID;
	SHORT			MaxUserCount;
	SHORT			MaxGroundHeight;				// Áö»óÀ¸·ÎºÎÅÍ ÃÖ´ë ³ôÀÌ
	SHORT			MinGroundHeight;				// Áö»óÀ¸·ÎºÎÅÍ ÃÖ¼Ò ³ôÀÌ
	SHORT			WaterHeight;
	SHORT			UserVisibleDistance;
	SHORT			MonsterVisibleDistance;
	INT				QuestIndexForWarp;
	SHORT			FrontPositionDistance;
	BYTE			AutoRecoveryFlag;
	MapIndex_t		DefaultMapIndex;				
	AVECTOR3		CityWarpTargetPosition;
	SHORT			ClientMaxAltitudeHeight;		// Å¬¶óÀÌ¾ðÆ® ÃÖ´ë ³ôÀÌ, 2005-07-11 by cmkwon
	DWORD			DayFogColor;
	DWORD			NightFogColor;
	SHORT			DayFogStartDistance;
	SHORT			DayFogEndDistance;
	SHORT			NightFogStartDistance;
	SHORT			NightFogEndDistance;
	AVECTOR3		DaySunDirection;
	AVECTOR3		NightSunDirection;
	BYTE			WaterType;
	BYTE			PKZoneFlag;
	BYTE			TileRenderingFlag;
	BYTE			SkyRenderingFlag;
	BYTE			SunRenderingFlag;
	BYTE			FogRenderingFlag;
	INT				VCNWarpObjectIndex;			// 2006-12-08 by cmkwon
	INT				ANIWarpObjectIndex;			// 2006-12-08 by cmkwon
	MapIndex_t		Dat;						// 2007-03-15 by dhjin
	MapIndex_t		Map;						// 2007-03-15 by dhjin
	MapIndex_t		Tex;						// 2007-03-15 by dhjin
	MapIndex_t		Cloud;						// 2007-03-15 by dhjin
	MapIndex_t		Sky;						// 2007-03-15 by dhjin
	MapIndex_t		Nsky;						// 2007-03-15 by dhjin
	MapIndex_t		Bgm;						// 2007-03-15 by dhjin
	INT				TeleportWarpObjectIndex;	// 2007-09-05 by dhjin
	MapIndex_t		BeforeMapIndex2;			// 2008-06-20 by cmkwon, EP3 ¹é¸Ê ½Ã½ºÅÛ ¼öÁ¤(¸Ê´ç ¹é¸ÊÀ» 2°³ ¼³Á¤ ÇÒ ¼ö ÀÖ´Ù) - 
	char			MapDescription[SIZE_MAX_MAP_DESCRIPTION];		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 
	void ResetMAP_INFO(void)
	{
		memset(this, 0x00, sizeof(*this));
	}
};
typedef vector<MAP_INFO>		vectorMAP_INFO;

// String Conversion Functions
const char *GetCharacterString(const CHARACTER *pCharac, string &str);
const char *GetCharacterString2(const CHARACTER *pCharac, string &str); //ocdao991 - 27/01/2015
const char *GetItemString(const ITEM_BASE *pItemBase, string &str);
const char *GetItemGeneralString(const ITEM_GENERAL *pItemGeneral, string &str);
const char *GetItemSkillString(const ITEM_SKILL *pItemSkill, string &str);
const char *DBG_GET_POSITION_STRING(D3DXVECTOR3& vec, string& str);
const char *GetRaceString(USHORT race);
const char *GetSmallRaceOrInfluenceString(USHORT race, BYTE InfluenceType);
char* Underbar2Space(char *string);
const char *GetTimeString(string &str);

const char *GetMapString(MAP_CHANNEL_INDEX &refMapChannelIndex, string &str);
#define GET_MAP_STRING(refMapChannelIndex)	GetMapString(refMapChannelIndex, string())

BYTE* GetIPAddressInBytes(char *i_szIPAddress, BYTE *o_pIPArray);
const char *GetIPAddressString(BYTE *i_pIPArray, STRNBUF &i_strnbuf);
const char *GetItemDesParameter(DestParam_t i_byDesParameter);		// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø)
BOOL GIsValidInfuenceType(BYTE i_byInfluenceTy);
char *GGetLowerCase(char *o_szLowercaseSting, char *i_szString, int nMaxStringSize);

enum EN_CHECK_TYPE
{
	CHECK_TYPE_BATTLE_P2P_PK		= 0,		// 1:1 ´ë°á 
	CHECK_TYPE_BATTLE_PARTY_WAR		= 1,		// Æí´ëÀü
	CHECK_TYPE_CHARACTER_MENT		= 2,		// "/¸àÆ®' ¸í·É¾î
	CHECK_TYPE_PENALTY_ON_DEAD		= 3,		// Ä³¸¯ÅÍ Á×¾úÀ» ¶§ ÆÐ³ÎÆ¼(°æÇèÄ¡ ´Ù¿î or SPI ´Ù¿î) Àû¿ë ·¹º§
	CHECK_TYPE_TRADE				= 4,		// À¯Àú°£ °Å·¡// 2005-12-06 by cmkwon
	CHECK_TYPE_BATTLE_GUILD_WAR		= 5,		// ¿©´ÜÀü
	CHECK_TYPE_LOWLEVEL_ADVANTAGE	= 6,		// ³·Àº ·¹º§ ÇýÅÃ
	CHECK_TYPE_BAZAAR				= 7,		// 2006-08-07 by cmkwon, °³ÀÎ »óÁ¡ °Å·¡(±¸ÀÔ/ÆÇ¸Å »óÁ¡)

	CHECK_TYPE_GUILD_CREATE			= 10,		// ±æµå »ý¼º
	CHECK_TYPE_GUILD_JOIN			= 11,		// ¿©´Ü °¡ÀÔ// 2005-12-07 by cmkwon

	CHECK_TYPE_PARTY_JOIN			= 20,		// ÆÄÆ¼ °¡ÀÔ// 2005-12-07 by cmkwon

	CHECK_TYPE_CHAT_ALL				= 100,		// Ã¤ÆÃ - ÀüÃ¼Ã¤ÆÃ// 2005-12-07 by cmkwon
	CHECK_TYPE_CHAT_MAP				= 101,		// Ã¤ÆÃ - ¸ÊÃ¤ÆÃ// 2005-12-07 by cmkwon
	CHECK_TYPE_CHAT_REGION			= 102,		// Ã¤ÆÃ - Áö¿ªÃ¤ÆÃ// 2005-12-07 by cmkwon
	CHECK_TYPE_CHAT_PTOP			= 103,		// Ã¤ÆÃ - ±Ó¸»Ã¤ÆÃ// 2005-12-07 by cmkwon
	CHECK_TYPE_CHAT_SELL_ALL		= 104,		// ¸Å¸Å Ã¤ÆÃ
	CHECK_TYPE_CHAT_CASH_ALL		= 105,		// Ã¤ÆÃ - À¯·áÀüÃ¼Ã¤ÆÃ// 2005-12-07 by cmkwon
	//////////////////////////////////////////////////////////////////////////
	// 2008-06-17 by dhjin, EP3 VOIP - ½Ã½ºÅÛ º¯°æÀ¸·Î ¹Ø°ú °°ÀÌ ¼öÁ¤
//	CHECK_TYPE_VOIP_1to1			= 106,		// À½¼ºÃ¤ÆÃ - 1:1 Ã¤ÆÃ// 2005-12-07 by cmkwon
//	CHECK_TYPE_CHAT_WAR				= 107		// 2008-05-19 by dhjin, EP3 - Ã¤ÆÃ ½Ã½ºÅÛ º¯°æ, ÀüÀï Ã¤ÆÃ
	CHECK_TYPE_CHAT_WAR				= 106,		// 2008-05-19 by dhjin, EP3 - Ã¤ÆÃ ½Ã½ºÅÛ º¯°æ, ÀüÀï Ã¤ÆÃ

	CHECK_TYPE_VOIP_NONE			= 201,		// 2008-06-17 by dhjin, EP3 VOIP - À½¼ºÃ¤ÆÃÁßÀÌ ¾Æ´Ï´Ù.
	CHECK_TYPE_VOIP_1to1			= 202,		// À½¼ºÃ¤ÆÃ - 1:1 Ã¤ÆÃ// 2005-12-07 by cmkwon
	CHECK_TYPE_VOIP_PARTY			= 203,		// 2008-06-17 by dhjin, EP3 VOIP - ÆÄÆ¼ À½¼º Ã¤ÆÃ
	CHECK_TYPE_VOIP_GUILD			= 204		// 2008-06-17 by dhjin, EP3 VOIP - ¿©´Ü À½¼º Ã¤ÆÃ

};
BOOL GCheckLimitLevel(EN_CHECK_TYPE i_checkType, int i_nLevel);
BOOL GCheckRaceAndInfluenceType(EN_CHECK_TYPE i_checkType, USHORT i_usRace, BYTE i_byInfluenceTy, USHORT i_usPeerRace, BYTE i_byPeerInfluenceTy);

// UID32_t(Unique Number) Generator
class CUID32Generator
{
public:
	CUID32Generator(UID32_t min = 1, UID32_t max = ((UID32_t)~0)-1)
	{
		m_num = min-1;
		m_min = min;
		m_max = max;
	}
	inline UID32_t GetNext()
	{
		if (m_num < m_min-1) return INVALID_UID32;
		if (m_num > m_max) Reset();
		return ++m_num;
	}
	inline UID32_t GetCurrent() { return m_num; }
	inline void Reset() { m_num = m_min-1; }
	inline UID32_t GetMin() { return m_min; }
	inline UID32_t GetMax() { return m_max; }
private:
	UID32_t	m_num;	// ÇöÀç »ý¼ºµÈ UID32
	UID32_t	m_min;	// ÃÖ¼Ò UID32
	UID32_t	m_max;	// ÃÖ´ë UID32
};

// UID16_t(Unique Number) Generator
class CUID16Generator
{
public:
	CUID16Generator(UID16_t min = 1, UID16_t max = ((UID16_t)~0)-1)
	{
		m_num = min-1;
		m_min = min;
		m_max = max;
	}
	inline UID16_t GetNext()
	{
		if (m_num < m_min-1) return INVALID_UID16;
		if (m_num > m_max) Reset();
		return ++m_num;
	}
	inline UID16_t GetCurrent() { return m_num; }
	inline void Reset() { m_num = m_min-1; }
	inline UID16_t GetMin() { return m_min; }
	inline UID16_t GetMax() { return m_max; }
private:
	UID16_t	m_num;	// ÇöÀç »ý¼ºµÈ UID16
	UID16_t	m_min;	// ÃÖ¼Ò UID16
	UID16_t	m_max;	// ÃÖ´ë UID16
};

///////////////////////////////////////////////////////////////////////////////
// Admin Tool¿ë Server List
///////////////////////////////////////////////////////////////////////////////
// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ÇÔ
//struct GAME_SERVER_INFO_FOR_ADMIN
//{
//	const char	*ServerName;
//	const char	*ServerIP;
//	const char	*DBIP;
//	int			DBPort;
//	const char	*DBUID;
//	const char	*DBPWD;
//	const char	*DBName;
//};
struct GAME_SERVER_INFO_FOR_ADMIN
{
	const char	*ServerName;
// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ ÇÔ.
//	char		XOREncodedServerIP[2*SIZE_MAX_IPADDRESS];		// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡µÈ ÇÊµå
//	char		XOREncodedDBIP[2*SIZE_MAX_IPADDRESS];			// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡µÈ ÇÊµå
	char		XOREncodedServerIP[2*SIZE_MAX_ADDRESS];		// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 
	char		XOREncodedDBIP[2*SIZE_MAX_ADDRESS];			// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 
	int			DBPort;
	char		XOREncodedDBUID[2*SIZE_MAX_DB_USER_ID];		// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡µÈ ÇÊµå
	char		XOREncodedDBPWD[2*SIZE_MAX_DB_USER_PWD];		// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡µÈ ÇÊµå
	const char	*DBName;
// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ ÇÔ.
//	char		LogDBIP[2*SIZE_MAX_IPADDRESS];			// 2007-11-07 by cmkwon, ·Î±× DB ¼­¹ö µû·Î ±¸ÃàÇÏ±â - XOR ¾ÏÈ£È­µÇ¾î ÀÖÀ½, º¹È£È­ ÇÏ¿© »ç¿ë ÇØ¾ß ÇÔ
	char		LogDBIP[2*SIZE_MAX_ADDRESS];			// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 
	int			LogDBPort;								// 2007-11-07 by cmkwon, ·Î±× DB ¼­¹ö µû·Î ±¸ÃàÇÏ±â - XOR ¾ÏÈ£È­µÇ¾î ÀÖÀ½, º¹È£È­ ÇÏ¿© »ç¿ë ÇØ¾ß ÇÔ
	char		LogDBUID[2*SIZE_MAX_DB_USER_ID];		// 2007-11-07 by cmkwon, ·Î±× DB ¼­¹ö µû·Î ±¸ÃàÇÏ±â - XOR ¾ÏÈ£È­µÇ¾î ÀÖÀ½, º¹È£È­ ÇÏ¿© »ç¿ë ÇØ¾ß ÇÔ
	char		LogDBPWD[2*SIZE_MAX_DB_USER_PWD];		// 2007-11-07 by cmkwon, ·Î±× DB ¼­¹ö µû·Î ±¸ÃàÇÏ±â - XOR ¾ÏÈ£È­µÇ¾î ÀÖÀ½, º¹È£È­ ÇÏ¿© »ç¿ë ÇØ¾ß ÇÔ
// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ ÇÔ.
//	char		ServerIP[SIZE_MAX_IPADDRESS];
//	char		DBIP[SIZE_MAX_IPADDRESS];
	char		ServerIP[SIZE_MAX_ADDRESS];				// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 
	char		DBIP[SIZE_MAX_ADDRESS];					// 2009-01-15 by cmkwon, PreServer, DBServer Á¤º¸ DNS·Î ¼³Á¤ °¡´ÉÇÏ°Ô ¼öÁ¤ - 
	char		DBUID[SIZE_MAX_DB_USER_ID];
	char		DBPWD[SIZE_MAX_DB_USER_PWD];
};

///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
typedef enum EN_CASH_PREMIUM_CARD_STATE
{
	CASH_PREMIUM_CARD_STATE_NOT_EXIST	= 0,
	CASH_PREMIUM_CARD_STATE_NORMAL		= 1,
	CASH_PREMIUM_CARD_STATE_INSERTING	= 2,
	CASH_PREMIUM_CARD_STATE_UPDATING	= 3,
	CASH_PREMIUM_CARD_STATE_DELETING	= 4
};

struct SCASH_PREMEIUM_CARD_INFO
{
	void ResetCASH_PREMEIUM_CARD_INFO(void)
	{
		enCardState			= CASH_PREMIUM_CARD_STATE_NOT_EXIST;
		n64UniqueNumber10	= 0;
		nAccountUID10		= 0;
		nCardItemNum		= 0;
		atumTimeCreatedTime.Reset();
		atumTimeUpdatedTime.Reset();
		atumTimeExpireTime.Reset();

		fExpRate			= 0.0f;
		fSPIRate			= 0.0f;
		fDropRate			= 0.0f;
		fDropRareRate		= 0.0f;
		fExpRepairRate		= 0.0f;
		nInventoryPlusCounts		= 0;		// 2006-09-06 by cmkwon
		nStorePlusCounts			= 0;		// 2006-09-06 by cmkwon
		nGuildCapacityPlusCounts	= 0;		// 2006-09-06 by cmkwon
	}

	void ResetAllPlusRateByPremiumCard(int i_nLangTy)
	{
		fExpRate			= 0.0f;
		fSPIRate			= 0.0f;
		fDropRate			= 0.0f;
		fDropRareRate		= 0.0f;
		fExpRepairRate		= 0.0f;
		nInventoryPlusCounts		= 0;		// 2006-09-06 by cmkwon
		nStorePlusCounts			= 0;		// 2006-09-06 by cmkwon
		nGuildCapacityPlusCounts	= 0;		// 2006-09-06 by cmkwon

		if(CASH_PREMIUM_CARD_STATE_NOT_EXIST == enCardState)
		{// 2006-10-11 by cmkwon
			return;
		}

		///////////////////////////////////////////////////////////////////////////////
		// ÇÁ¸®¹Ì¾ö ¿É¼Ç [¼­¹ö,±âÈ¹]
		nInventoryPlusCounts		= COUNT_IN_MEMBERSHIP_ADDED_INVENTORY;		// 2006-09-06 by cmkwon
		nStorePlusCounts			= COUNT_IN_MEMBERSHIP_ADDED_STORE;			// 2006-09-06 by cmkwon
		nGuildCapacityPlusCounts	= COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY;	// 2006-09-06 by cmkwon
		fExpRepairRate				= 0.5f;										// 2006-10-19 by cmkwon, ¸ðµÎ Á×À» ¶§ ÆÐ³ÎÆ¼ 50% °¨¼Ò

		// 2008-04-25 by cmkwon, Áö¿ø ¾ð¾î/¼­ºñ½º Ãß°¡½Ã ²À Ãß°¡ µÇ¾î¾ß ÇÏ´Â »çÇ× - [¾ð¾î-¿É¼Ç] ¸â¹ö½± Ãß°¡ ÇýÅÃ ¼³Á¤
		switch(i_nLangTy)
		{
		case LANGUAGE_TYPE_VIETNAMESE:
			{// 2006-10-11 by cmkwon, º£Æ®³²¸¸ Àû¿ë(°æÇèÄ¡ 20%, ¾ÆÀÌÅÛµå¶øÀ² 20%, SPI µå¶øÀ² 40%)
				fExpRate			= 0.2f;
				fSPIRate			= 0.4f;
				fDropRate			= 0.2f;
				fDropRareRate		= 0.0f;
			}
			break;
		}
	}
	EN_CASH_PREMIUM_CARD_STATE	enCardState;
	UID64_t			n64UniqueNumber10;
	UID32_t			nAccountUID10;
	INT				nCardItemNum;
	ATUM_DATE_TIME	atumTimeCreatedTime;	// »ý¼ºµÈ ½Ã°£
	ATUM_DATE_TIME	atumTimeUpdatedTime;	// ¼öÁ¤µÈ ½Ã°£
	ATUM_DATE_TIME	atumTimeExpireTime;		// ¸¸·á ½Ã°£

	float			fExpRate;
	float			fSPIRate;
	float			fDropRate;
	float			fDropRareRate;
	float			fExpRepairRate;

	int				nInventoryPlusCounts;		// 2006-09-06 by cmkwon, ÀÎº¥Åä¸® Ãß°¡ Ä«¿îÆ®
	int				nStorePlusCounts;			// 2006-09-06 by cmkwon, Ã¢°í Ãß°¡ Ä«¿îÆ®
	int				nGuildCapacityPlusCounts;	// 2006-09-06 by cmkwon, ¿©´Ü¿ø Ãß°¡ Ä«¿îÆ®
};

struct SCITY_OCCUPY_INFO
{
	UID32_t			OccupyGuildUID;
	char			OccupyGuildName[SIZE_MAX_GUILD_NAME];
	UID32_t			OccupyGuildMasterCharacterUID;
	float			fTexPercent;								// ¼¼±Ý ÆÛ¼¾Æ®(2 ~ 100)
	ATUM_DATE_TIME	CityWarDefaultTime;
	ATUM_DATE_TIME	CityWarSettingTime;
	INT				nDefenseCounts;
	INT				nSumOfTex;
	char			szBriefing[SIZE_MAX_CITY_BRIEFING_LENGTH];

	void ResetCITY_OCCUPY_INFO(void);
};

struct SCOUPON
{
	INT					nCouponUniqueNumber;
	char				szCouponNumber[SIZE_MAX_COUPON_NUMBER];
	ATUM_DATE_TIME		atimeExpireTime;
	BOOL				bUsedFlag;
	char				szUsedAccountName[SIZE_MAX_ACCOUNT_NAME];
	ATUM_DATE_TIME		atimeUsedTime;
};


enum EN_BLOCKED_TYPE
{
	T_BLOCKEDACCOUNT_UNKNOWN			= 0,		// ¾Ë¼ö ¾øÀ½
	T_BLOCKEDACCOUNT_NORMAL				= 1,		// Æ¯º°ÇÑ »çÀ¯ ¾øÀ½, ÀÓ½Ã·Î
	T_BLOCKEDACCOUNT_MONEY_RELATED		= 2,		// µ· °ü·Ã - SPI, 			
	T_BLOCKEDACCOUNT_ITEM_RELATED		= 3,		// ¾ÆÀÌÅÛ °ü·Ã - »ç±â(¾ÆÀÌÅÛ ±¸¸Å, ÆÇ¸Å)  
	T_BLOCKEDACCOUNT_SPEEDHACK_RELATED	= 4,		// ½ºÇÇµåÇÙ °ü·Ã
	T_BLOCKEDACCOUNT_CHAT_RELATED		= 5,		// Ã¤ÆÃ °ü·Ã - ¿å¼³ 
	T_BLOCKEDACCOUNT_CHAT_GAMEBUG		= 6,		// °ÔÀÓ ¹ö±× °ü·Ã - °ÔÀÓ ¹ö±× ÀÌ¿ëÇÑ À¯Àú
	T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK	= 7,	// ¸Þ¸ð¸®ÇÙ ¿ÀÅä ºí·° - // 2012-12-14 by hskim, ¸Þ¸ð¸®ÇÙ ÀÚµ¿ ºí·° ±â´É ±¸Çö
	T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK	= 8		// ½ºÇÇµåÇÙ ¿ÀÅä ºí·° - // 2013-01-29 by hskim, ½ºÇÇµåÇÙ ÀÚµ¿ ºí·° ±â´É ±¸Çö
};

char * GetStringBLOCKED_TYPE(int i_blocktype);
int GetBlockTypeBYBlockTypeString(char *i_szTyString);	// 2008-01-30 by cmkwon, °èÁ¤ ºí·° ·Î±× ³²±â±â ±¸Çö - GetBlockTypeBYBlockTypeString() Ãß°¡
struct SBLOCKED_ACCOUNT_INFO
{
	char			szBlockedAccountName[SIZE_MAX_ACCOUNT_NAME];
	EN_BLOCKED_TYPE	enBlockedType;
	ATUM_DATE_TIME	atimeStartTime;
	ATUM_DATE_TIME	atimeEndTime;
	INT				AdminFieldServerClientIndex;		// 2008-01-31 by cmkwon, °èÁ¤ ºí·°/ÇØÁ¦ ¸í·É¾î·Î °¡´ÉÇÑ ½Ã½ºÅÛ ±¸Çö - PreServer<->FieldServer ¿¡¼­¸¸ »ç¿ëÇÔ
	char			szBlockAdminAccountName[SIZE_MAX_ACCOUNT_NAME];
	char			szBlockedReasonForUser[SIZE_MAX_BLOCKED_ACCOUNT_REASON];
	char			szBlockedReasonForOnlyAdmin[SIZE_MAX_BLOCKED_ACCOUNT_REASON];		// 2007-01-10 by cmkwon
};

///////////////////////////////////////////////////////////////////////////////
// 2006-04-12 by cmkwon, ¼¼·ÂÀü º¸°­ ½Ã½ºÅÛÀ¸·Î Ãß°¡µÊ
#define MAX_INFLUENCEWAR_WARTIMESTAGE			5		// 2006-04-13 by cmkwon, 0 ~ 5
#define MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT		5		// 2006-04-13 by cmkwon
#define	STRATEGYPOINT_MONSTER_NUM_VCN			2052000	// 2007-11-07 by dhjin, Àü·«Æ÷ÀÎÆ® VCN¹øÈ£
#define	STRATEGYPOINT_MONSTER_NUM_ANI			2052100	// 2007-11-07 by dhjin, Àü·«Æ÷ÀÎÆ® ANI¹øÈ£

struct SDB_INFLUENCE_WAR_INFO
{
	BYTE			InfluenceType;					// INFLUENCE_TYPE_VCN(2) or INFLUENCE_TYPE_ANI(4)
	BYTE			WartimeStage;					// Àü½Ã´Ü°è
	INT				ReqContributionPoint;			// Àü½Ã´Ü°è°¡ µÇ±â À§ÇÑ ÇÊ¿ä ±â¿©µµ(ContributionPoint)
	float			HPRepairRate;					// ÇØ´ç Àü½Ã´Ü°è ÀÏ ¶§ HPÈ¸º¹·ü                            
	float			DPRepairRate;					// ÇØ´ç Àü½Ã´Ü°è ÀÏ ¶§ DPÈ¸º¹·ü                            
	float			SPRepairRate;					// ÇØ´ç Àü½Ã´Ü°è ÀÏ ¶§ SPÈ¸º¹·ü                            
	INT				EventTime;						// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® Áö¼Ó ½Ã°£(´ÜÀ§:ºÐ)
	float			EventExperience;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® ÀÌº¥Æ® - °æÇèÄ¡          
	float			EventDropSPI;					// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® ÀÌº¥Æ® - µå¶ø SPI        
	float			EventDownExperience;			// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® ÀÌº¥Æ® - °æÇèÄ¡ µå¶ø     
	float			EventDropItem;					// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® ÀÌº¥Æ® - µå¶ø Item       
	float			EventDropRareItem;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ½ÃÀÛµÇ´Â ÀÌº¥Æ® ÀÌº¥Æ® - µå¶ø ·¹¾î Item  
	INT				SupplyItemNum;					// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ Áö±ÞµÇ´Â ItemNum
	INT				SupplyItemCount;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ Áö±ÞµÇ´Â Item Count
	BYTE			IsSummonJacoMonster;
	INT				BossMonsterUnitKind;			// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ MonsterUnitKind          
	INT				BossMonsterCount;				// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â Monster count
	INT				BossMonsterSummonTerm;			// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â ½Ã°£ ÅÒ(¿¹»ó 15ºÐ, ´ÜÀ§:ºÐ)
	SHORT			BossMonsterSummonMapIndex;		// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â MapIndex        
	SHORT			BossMonsterSummonPositionX;		// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â ÁÂÇ¥ x                   
	SHORT			BossMonsterSummonPositionY;		// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â ÁÂÇ¥ y                   
	SHORT			BossMonsterSummonPositionZ;		// °¢ ´Ü°è ÇÊ¿ä ±â¿©µµ¸¦ ³Ñ¾î¼­´Â ½ÃÁ¡¿¡ ¼ÒÈ¯µÇ´Â ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â ÁÂÇ¥ z                   
	INT				SummonMonsterUnitKind[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];		// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ ¼ÒÈ¯µÇ´Â MonsterUnitKind
	INT				SummonCount[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];				//  ¼ÒÈ¯µÇ´Â Monster Count
	SHORT			SummonMapIndex[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];			//  ¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â MapIndex
	SHORT			SummonPositionX[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];			//  ¼ÒÈ¯µÇ´Â ÁÂÇ¥ x
	SHORT			SummonPositionY[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];			//  ¼ÒÈ¯µÇ´Â ÁÂÇ¥ y
	SHORT			SummonPositionZ[MAX_INFLUENCEWAR_EVENT_SUMMON_COUNT];			//  ¼ÒÈ¯µÇ´Â ÁÂÇ¥ z
	INT				SupplyItemNum2;					// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ Áö±ÞµÇ´Â ItemNum	// 2007-05-16 by dhjin, Ãß°¡µÊ
	INT				SupplyItemCount2;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ °ÝÃß ÈÄ ÇØ´ç Àü½Ã´Ü°è¿¡ Áö±ÞµÇ´Â Item Count	// 2007-05-16 by dhjin, Ãß°¡µÊ
	INT				WinBossMonsterUnitKind;			// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - °ø°Ý ¸ð¼± ½Â¸® ¼¼·ÂÀÏ °æ¿ì ¼ÒÈ¯µÇ¾î¾ß µÇ´Â ¸ð¼±UID
	INT				LossBossMonsterUnitKind;		// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - °ø°Ý ¸ð¼± ÆÐ¹è ¼¼·ÂÀÏ °æ¿ì ¼ÒÈ¯µÇ¾î¾ß µÇ´Â ¸ð¼±UID

	// 2008-05-20 by cmkwon, ¸ðµç ÀÌº¥Æ®(HappyHoure,MotherShip,Item) ±×·ì µ¿½Ã¿¡ °¡´ÉÇÏ°Ô - 
	BOOL IsValidGameEvent(void)
	{
		if(0 >= EventTime)
		{
			return FALSE;
		}

		if( 0.0f < EventExperience
			|| 0.0f < EventDropSPI
			|| 0.0f < EventDownExperience
			|| 0.0f < EventDropItem
			|| 0.0f < EventDropRareItem )
		{
			return TRUE;
		}

		return FALSE;
	}
};
typedef vector<SDB_INFLUENCE_WAR_INFO>			vectDB_INFLUENCE_WAR_INFO;

struct SINFLUENCE_WAR_DATA
{
	BYTE			InfluenceType;			// INFLUENCE_TYPE_VCN(2) or INFLUENCE_TYPE_ANI(4)
	INT				MGameServerID;			// ¼­¹ö±º Ã¤³Î °íÀ¯ ¹øÈ£ - global.cfg ÆÄÀÏ¿¡ ¼³Á¤ÇÑ´Ù.
	BYTE			WartimeStage;			// ÇöÀç Àü½Ã´Ü°è
	INT				ContributionPoint;		// ´©Àû ±â¿©µµ - ¾Æ¸¶ ÇÑ´Þ ´©Àû ±â¿©µµ°¡ µÉ°ÍÀÌ´Ù
	UID32_t			InflLeaderCharacterUID;	// ¼¼·Â ÁöµµÀÚ CharacterUID
	UID32_t			InflSub1LeaderCharacterUID;	// 2006-12-08 by dhjin, ºÎÁöµµÀÚ1
	UID32_t			InflSub2LeaderCharacterUID; // 2006-12-08 by dhjin, ºÎÁöµµÀÚ2
	CHAR			InflLeaderCharacterName[SIZE_MAX_CHARACTER_NAME];		// 2007-12-03 by dhjin, ÁöµµÀÚ 
	CHAR			InflSubLeader1CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-12-03 by dhjin, ºÎÁöµµÀÚ1
	CHAR			InflSubLeader2CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-12-03 by dhjin, ºÎÁöµµÀÚ2
	SHORT			MSWarOptionType;		// 2008-04-10 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - 
	SHORT			ConsecutiveVictories;	// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±
};

struct SINFLBOSS_MONSTER_SUMMON_DATA
{
	MONSTER_INFO	*pMonsterInfo;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ Á¤º¸
	INT				SummonCount;				// ¼ÒÈ¯µÉ Monster Count
	INT				SummonTerm;					// ¼ÒÈ¯µÇ±â À§Èù ½Ã°£ ÅÒ(¿¹»ó 15ºÐ, ´ÜÀ§:ºÐ)
	ATUM_DATE_TIME	atimeInsertedTime;			// ¼ÒÈ¯ ¸ó½ºÅÍ Ãß°¡µÈ ½Ã°£ - ¼ÒÈ¯µÇ±â ¿ìÇÑ ½Ã°£ ÅÒ Ã³¸®¸¦ À§ÇØ ÇÊ¿ä
	SHORT			SummonMapIndex;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â MapIndex
	AVECTOR3		SummonPosition;				// ¼¼·ÂÀüº¸½º¸ó½ºÅÍ°¡ ¼ÒÈ¯µÇ´Â ÁÂÇ¥
	BYTE			BossStep;					// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - ¸ð¼± ´Ü°è
	BYTE			BeforeWinCheck;				// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - Àü ´Ü°è ½ÂÆÐ ¿©ºÎ
};

int GDelimiterIndex(char *i_szStr, int i_nMinIndex);

struct SSUMMONBOSSMONSTER_INFO
{// 2007-02-06 by dhjin, ¼ÒÈ¯ ¸ó½ºÅÍ Á¤º¸ 
	BOOL			bSummonBossMonster;
	ATUM_DATE_TIME	SummonBossTime;
	INT				SummonMonsterUnitkind;
	INT				ContributionPoint;			// 2008-04-01 by dhjin, ¸ð¼±Àü, °ÅÁ¡Àü Á¤º¸Ã¢ ±âÈ¹¾È - 
	BYTE			BossStep;					// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - ¸ð¼± ´Ü°è
	BYTE			BeforeWinCheck;				// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ - Àü ´Ü°è ½ÂÆÐ ¿©ºÎ
};

struct SDELETED_CHARACTER_INFO
{// 2007-02-22 by dhjin, ÄÉ¸¯ÅÍ º¹±¸ Á¤Ã¥À¸·Î ÀÎÇÑ »èÁ¦µÈ ÄÉ¸¯ÅÍ Á¤º¸ ±¸Á¶Ã¼.
	UID32_t			AccountUID;
	CHAR 			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR 			CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE			InflueceType0;			// 2008-01-17 by cmkwon, T_A: »èÁ¦ »óÅÂÀÇ Ä³¸¯ÅÍ ÀÇ ¼¼·Â, ¼±ÅÃ °¡´ÉÇÑ ¼¼·Â º¸¿©ÁÖ±â Ãß°¡ - SDELETED_CHARACTER_INFO ¿¡ ÇÊµå Ãß°¡
	BYTE			SelectableInflueceMask;	// 2008-01-17 by cmkwon, T_A: »èÁ¦ »óÅÂÀÇ Ä³¸¯ÅÍ ÀÇ ¼¼·Â, ¼±ÅÃ °¡´ÉÇÑ ¼¼·Â º¸¿©ÁÖ±â Ãß°¡ - SDELETED_CHARACTER_INFO ¿¡ ÇÊµå Ãß°¡
	ATUM_DATE_TIME	atDeletedDTime;			// 2008-03-03 by cmkwon, »èÁ¦ »óÅÂÀÇ Ä³¸¯ÅÍ¸®½ºÆ® Á¤º¸¿¡ »èÁ¦ µÈ ³¯Â¥ º¸¿© ÁÖ±â - 
};

struct SSUMMON_STRATEGYPOINT_INFO
{// 2007-02-23 by dhjin, /°ÅÁ¡Á¤º¸ ¸í·É¾î
	MapIndex_t		MapIndex;
	BYTE			InfluenceType;
	CHAR			MapName[SIZE_MAX_MAP_NAME];
	ATUM_DATE_TIME	SummonStrategyPointTime;
};

struct SSTRATEGYPOINT_DISPLAY_INFO
{
	MapIndex_t		MapIndex;
	BYTE			InfluenceType;
	ATUM_DATE_TIME	SummonStrategyPointTime;
	FLOAT			HPRate;
};

struct SSTRATEGYPOINT_SUMMONTIME_INFO
{// 2007-02-28 by dhjin, Àü·«Æ÷ÀÎÆ® »ý¼º Á¤º¸ ±¸Á¶Ã¼
	MapIndex_t		MapIndex;
	BYTE			SummonCount;
	ATUM_DATE_TIME	SummonTime;
	BOOL			SummonAttribute;		// 2008-04-08 by dhjin, ¼ÒÈ¯ °¡´É ½Ã°£ ¼³Á¤ - 
};
typedef vector<SSTRATEGYPOINT_SUMMONTIME_INFO>	vectSSTRATEGYPOINT_SUMMONTIME_INFO;

struct SSTRATEGYPOINT_SUMMON_RANGE
{// 2007-02-28 by dhjin, Àü·«Æ÷ÀÎÆ® »ý¼º ÁÖ±â ±¸Á¶Ã¼
	SHORT			SummonRange;
	BOOL			StrategyPointSummonCheck;
	BYTE			MaxSummonCountDaily;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	SHORT			NewSummonRange;
	BYTE			NewMaxSummonCountDaily;
};

struct SSTRATEGYPOINT_NOT_SUMMON_TIME
{// 2007-02-28 by dhjin, Àü·«Æ÷ÀÎÆ® »ý¼ºµÇ¸é ¾ÈµÇ´Â ¿äÀÏº° ½Ã°£ Á¤º¸ ±¸Á¶Ã¼
	SHORT			DayOfWeek;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	BOOL			CheckSummon;			// 2008-04-04 by dhjin, ¼ÒÈ¯ °¡´É ½Ã°£ ¼³Á¤ - 0:±ÝÁö½Ã°£ , 1:¼ÒÈ¯½Ã°£
};
typedef vector<SSTRATEGYPOINT_NOT_SUMMON_TIME>	vectSSTRATEGYPOINT_NOT_SUMMON_TIME;

struct SSTRATEGYPOINT_DAY
{// 2007-03-07 by dhjin, Àü·«Æ÷ÀÎÆ® »ý¼º Ã¼Å© ¿äÀÏ.
	USHORT			Year;
	BYTE			Month;
	BYTE			Day;
	BYTE			MaxCount;
};

struct SSPPossibleTimeInfo
{// 2009-03-23 by dhjin, Àü·«Æ÷ÀÎÆ® ½Ã°£ ¼³Á¤ ¼öÁ¤ - Àü·«Æ÷ÀÎÆ® ¼ÒÈ¯ °¡´ÉÇÑ ½Ã°£°ª ÀüºÎ ÀúÀå
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	INT				PossibleMinRange;
	BOOL			CheckSummon;			// 2008-04-04 by dhjin, ¼ÒÈ¯ °¡´É ½Ã°£ ¼³Á¤ - 0:±ÝÁö½Ã°£ , 1:¼ÒÈ¯½Ã°£
};
typedef vector<SSPPossibleTimeInfo>		vectSSPPossibleTimeInfo;

///////////////////////////////////////////////////////////////////////////////
// 2006-07-25 by cmkwon, °³ÀÎ»óÁ¡ °ü·Ã
#define SIZE_MAX_BAZAAR_ITEM_COUNT		16			// 2006-07-25 by cmkwon, ÆÇ¸Å È¤Àº ±¸ÀÔ ¸®½ºÆ® ÃÖ´ë °³¼ö
#define SIZE_MAX_BAZAAR_NAME			32			// 2006-07-25 by cmkwon, 
#define SIZE_MAX_BAZAAR_NAME			32			// 2006-07-25 by cmkwon, 
#define REQUERED_BAZAAR_DISTANCE		100.0f		// 2006-08-01 by cmkwon, °³ÀÎ »óÁ¡°ú °Å·¡ÇÏ±â À§ÇÑ °Å¸®

#define BAZAAR_TYPE_NONE				0		// 2006-07-25 by cmkwon
#define BAZAAR_TYPE_SELL				1		// °³ÀÎ ÆÇ¸Å »óÁ¡
#define BAZAAR_TYPE_BUY					2		// °³ÀÎ ±¸¸Å »óÁ¡

#define BAZAAR_STATE_NONE				0		// 2006-07-25 by cmkwon
#define BAZAAR_STATE_READY				1		// 2006-07-26 by cmkwon, °³ÀÎ »óÁ¡ ÁØºñ »óÅÂ - ½ºÅ³ »ç¿ëÀº ½ÃÀÛµÇ¾úÁö¸¸ °³ÀÎ »óÁ¡Àº ½ÃÀÛÇÏÁö ¾ÊÀº »óÅÂ
#define BAZAAR_STATE_ACTIVE				2		// 2006-07-26 by cmkwon, Å¸À¯Àú°¡ °³ÀÎ »óÁ¡ ÆÇ¸Å/±¸ÀÔ °¡´É »óÅÂ
#define BAZAAR_STATE_END				3		// 2006-07-26 by cmkwon, Ç°Àý

///////////////////////////////////////////////////////////////////////////////
// 2006-08-25 by cmkwon
struct SHAPPY_HOUR_EVENT
{
	INT				EventUniqueNumber;		// 
	INT				ServerGroupID;			// 2006-08-25 by cmkwon, 0(=AllServerGroup), 1(=10061), 2(10062), ...
	INT				DayOfWeek;				// ¿äÀÏ	- ÀÏ(0) ¿ù(1) È­(2) ¼ö(3) ¸ñ(4) ±Ý(5) Åä(6) ÇØ´ç¼¼·ÂHappyHourEvent±â°£(7) PCBangHappyHourEvent±â°£³»¿ë(8)
	ATUM_DATE_TIME	atimeStartTime2;		//
	ATUM_DATE_TIME	atimeEndTime2;			//
	float			fEXPRate2;
	float			fSPIRate2;
	float			fEXPRepairRate2;
	float			fDropItemRate2;
	float			fDropRareItemRate2;
	float			fWarPointRate2;			// 2007-06-26 by dhjin, WarPoint ÀÌº¥Æ®
	BYTE			InfluenceType3;			// 2007-10-30 by cmkwon, ¼¼·Âº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö - SHAPPY_HOUR_EVENT ±¸Á¶Ã¼¿¡ ÇÊµå Ãß°¡
	INT				MinLevel;				// 2008-03-13 by dhjin, Levelº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö -
	INT				MaxLevel;				// 2008-03-13 by dhjin, Levelº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö -
};

struct SHAPPY_HOUR_EVENT_PERIOD
{// 2007-10-30 by cmkwon, ¼¼·Âº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö ±¸Á¶Ã¼ Ãß°¡
	INT				EventUniqueNumber;		// 
	ATUM_DATE_TIME	atimeStartTime2;		//
	ATUM_DATE_TIME	atimeEndTime2;			//
	BYTE			InfluenceType3;			//

	void InitSHAPPY_HOUR_EVENT_PERIOD(BYTE i_byInflTy)
	{
		EventUniqueNumber		= 0;
		atimeStartTime2.Reset();
		atimeEndTime2.Reset();
		InfluenceType3			= i_byInflTy;
	}
};

#define SERVERGROUP_START_ID			10061					// 2006-08-25 by cmkwon, global.cfg ÆÄÀÏÀÇ atum2_db_1 DB¿¡ »ç¿ëÇÏ´Â ¹øÈ£¿Í °°µµ·Ï ÇØ¾ßÇÑ´Ù.
#define STR_ALL_SERVERGROUP_NAME		"All ServerGroup"		// 2006-08-25 by cmkwon

///////////////////////////////////////////////////////////////////////////////
// end 2011-12-12 by hskim, GLog 2Â÷

#define GLOG_EXCUTE_FILE_NAME				"./GLogClient.exe"
#define GLOG_EXCUTE_FILE_NAME_WATCHPROC		"../Bin/Release/GLogClient.exe"
#define GLOG_EXCUTE_FILE_NAME_VISUALC		"../../Bin/Release/GLogClient.exe"

///////////////////////////////////////////////////////////////////////////////
// 2006-10-02 by cmkwon, WebLauncher °ü·Ã
#define SIZE_MAX_ARGV_COUNT					10					// 2006-10-02 by cmkwon, ½ÇÇà ÀÎÀÚ ÃÖ´ë °³¼ö
// 2008-12-18 by cmkwon, ÀÏº» Arario ·±Ã³ ¼öÁ¤ - ¾Æ·¡¿Í °°ÀÌ 400 À¸·Î ¼öÁ¤
//#define SIZE_MAX_ARGV_PARAM_STRING_SIZE		50					// 2006-10-02 by cmkwon, ÇÏ³ªÀÇ ½ÇÇà ÀÎÀÚ ½ºÆ®¸µ ÃÖ´ë ±æÀÌ
#define SIZE_MAX_ARGV_PARAM_STRING_SIZE		400		// 2008-12-18 by cmkwon, ÀÏº» Arario ·±Ã³ ¼öÁ¤ - SessionKey(301 Bytes) º¸´Ù Ä¿¾ß ÇÑ´Ù. // 2006-10-02 by cmkwon, ÇÏ³ªÀÇ ½ÇÇà ÀÎÀÚ ½ºÆ®¸µ ÃÖ´ë ±æÀÌ

#define EXCUTE_FILE_TYPE_SC_EXE				1
#define EXCUTE_FILE_TYPE_SC_LAUNCHER_ATM	2
#define EXCUTE_FILE_TYPE_SC_ATM				3
#define EXCUTE_FILE_TYPE_SC_WEBLAUNCHER		10


struct SEXCUTE_PARAMETER
{
	int			i_nExcuteFileType;
	char		o_szAccountName0[SIZE_MAX_ACCOUNT_NAME];
	char		o_szPreServerIP0[SIZE_MAX_IPADDRESS];
	int			o_nBirthYear;								// 2007-06-05 by cmkwon, Ãâ»ý³âµµ
	char		o_szPassword[SIZE_MAX_PASSWORD_MD5_STRING];				// 2008-12-18 by cmkwon, ÀÏº» Arario ·±Ã³ ¼öÁ¤ - SEXCUTE_PARAMETER ¿¡ ¸â¹ö º¯¼ö Ãß°¡
	char		o_szSessionKey[SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY];	// 2008-12-18 by cmkwon, ÀÏº» Arario ·±Ã³ ¼öÁ¤ - SEXCUTE_PARAMETER ¿¡ ¸â¹ö º¯¼ö Ãß°¡
	char		o_szSelectiveShutdownInfo[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO];		// 2012-07-11 by hskim, ¼±ÅÃÀû ¼Ë´Ù¿î
};

extern int		g_nargvCount;
extern char		g_szArrargv[SIZE_MAX_ARGV_COUNT][SIZE_MAX_ARGV_PARAM_STRING_SIZE];

// 2008-04-25 by cmkwon, Áö¿ø ¾ð¾î/¼­ºñ½º Ãß°¡½Ã ²À Ãß°¡ µÇ¾î¾ß ÇÏ´Â »çÇ× - [¼­ºñ½º-ÇÊ¼ö] C_Exe1, C_Exe2 ½ÇÇà½Ã ½ÇÇà ÆÄ¶ó¹ÌÅÍ Ã³¸® ÇÔ¼ö ¼±¾ð Ãß°¡
Err_t GSetExcuteParameterList(int i_nParamCount, char ** i_ppszParamString);
Err_t GCheckExcuteParameterList(SEXCUTE_PARAMETER *io_pExeParam);

///////////////////////////////////////////////////////////////////////////////
// 2007-05-09 by cmkwon
struct SREG_DATA_EXE_2
{
	VersionInfo		ClientVersion;
	VersionInfo		DVersion;		// DeleteFileListVersion
	VersionInfo		NVersion;		// NoticeFileVersion
// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ - ÇØ»óµµ ÀúÀå ½Ã½ºÅÛ ¼öÁ¤
//	int				WindowDegree;	// ÇØ»óµµ ¼±ÅÃ ÀÎµ¦½º
	char			WindowDegree[SIZE_MAX_WINDOW_DEGREE_NAME];		// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ - Ãß°¡
	char			BeforeAccountName[SIZE_MAX_ACCOUNT_NAME];
	char			SelectedServerGroupName[SIZE_MAX_SERVER_NAME];
	int				IsWindowMode;	// 2008-01-03 by cmkwon, À©µµ¿ì¸ðµå »óÅÂ ÀúÀåÇÏ±â - SREG_DATA_EXE_2 ¿¡ ÇÊµå Ãß°¡

	void resetREG_DATA_EXE_2(void)
	{
		ClientVersion.SetVersion(0,0,0,0);
		DVersion.SetVersion(0,0,0,0);
		NVersion.SetVersion(0,0,0,0);
		// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ -
		//WindowDegree		= -1;
		MEMSET_ZERO(WindowDegree, SIZE_MAX_WINDOW_DEGREE_NAME);		// 2007-12-27 by cmkwon, À©µµ¿ìÁî ¸ðµå ±â´É Ãß°¡ - Ãß°¡
		MEMSET_ZERO(BeforeAccountName, SIZE_MAX_ACCOUNT_NAME);
		MEMSET_ZERO(SelectedServerGroupName, SIZE_MAX_SERVER_NAME);
		IsWindowMode		= FALSE;	// 2008-01-03 by cmkwon, À©µµ¿ì¸ðµå »óÅÂ ÀúÀåÇÏ±â - 
	}
};
Err_t GLoadExe1VersionInfo(VersionInfo *o_pLauncherVerInfo, char *i_szVersionInfoFileName);
Err_t GLoadExe2VersionInfo(SREG_DATA_EXE_2 *o_pRegDataEXE2, char *i_szVersionInfoFileName);
Err_t GSaveExe1VersionInfo(VersionInfo *i_pLauncherVerInfo, char *i_szVersionInfoFileName);
Err_t GSaveExe2VersionInfo(SREG_DATA_EXE_2 *i_pRegDataEXE2, char *i_szVersionInfoFileName);

struct PCBANG_INFO
{// 2007-01-22 by dhjin, PC¹æ DBÁ¤º¸ ±¸Á¶Ã¼
	UID32_t		PCBangUID;
	char		ST_IP[SIZE_MAX_IPADDRESS];
	char		ED_IP[4];
	char		User_Id[SIZE_MAX_ACCOUNT_NAME];
	char		Branch_Name[50];
	char		Branch_RegNO[12];
	char		Branch_Tel[14];
	char		ZipCode[15];
	char		Addr_Sido[10];
	char		Addr_SiGuGun[20];
	char		Addr_Dong[100];
	char		Addr_Detail[50];
	BYTE		User_Level;				// 2007-06-25 by dhjin, PC¹æ µî±Þ Ãß°¡

	void ResetPCBANG_INFO(void)
	{
		MEMSET_ZERO(this, sizeof(PCBANG_INFO));
	}
	
	void SetPCBANG_INFO(UID32_t i_uidPCBangUID, char *i_szST_IP, char *i_szED_IP, char *i_szUser_Id, char *i_szBranch_Name, char *i_szBranch_RegNO, char *i_szBranch_Tel, char *i_szZipCode, char *i_szAddr_Sido, char *i_szAddr_SiGuGun, char *i_szAddr_Dong, char *i_szAddr_Detail, BYTE i_byUser_Level)
	{
		this->ResetPCBANG_INFO();

		PCBangUID		= i_uidPCBangUID;
		STRNCPY_MEMSET(ST_IP, i_szST_IP, SIZE_MAX_IPADDRESS);
		STRNCPY_MEMSET(ED_IP, i_szED_IP, 4);
		STRNCPY_MEMSET(User_Id, i_szUser_Id, SIZE_MAX_ACCOUNT_NAME);
		STRNCPY_MEMSET(Branch_Name, i_szBranch_Name, 50);
		STRNCPY_MEMSET(Branch_RegNO, i_szBranch_RegNO, 12);
		STRNCPY_MEMSET(Branch_Tel, i_szBranch_Tel, 14);
		STRNCPY_MEMSET(ZipCode, i_szZipCode, 15);
		STRNCPY_MEMSET(Addr_Sido, i_szAddr_Sido, 10);
		STRNCPY_MEMSET(Addr_SiGuGun, i_szAddr_SiGuGun, 20);
		STRNCPY_MEMSET(Addr_Dong, i_szAddr_Dong, 100);
		STRNCPY_MEMSET(Addr_Detail, i_szAddr_Detail, 50);
		User_Level		= i_byUser_Level;
	}
};
typedef vector<PCBANG_INFO>		vectorPCBANG_INFO;


//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// 2007-04-17 by dhjin, ¾Æ·¹³ª¿¡ °ü·ÃµÈ »óÅÂ.
#define	ARENA_STATE_NONE				0	// ¾Æ·¹³ª¿¡ °ü·ÃµÈ ÀÏÀ» ÇÏ°í ÀÖÁö ¾Ê´Ù.
#define	ARENA_STATE_JOINING				1	// ¾Æ·¹³ªÆÀ¿¡ Âü°¡ÇÑ »óÅÂ, ¾ÆÁ÷ ÆÀ¿øÀÌ ²Ë Â÷Áö ¾Ê¾Ò´Ù.
#define	ARENA_STATE_READY				2	// ¾Æ·¹³ªÆÀ¿øÀÌ ²Ë Â÷¼­ ÆÀ¿øµéÀÇ ÁØºñ ¿Ï·á¸¦ ±â´Ù¸®´Â »óÅÂ.
#define	ARENA_STATE_READY_FINISH		3	// ¾Æ·¹³ªÆÀ¿øÀÌ ²Ë Â÷¼­ ÆÀ¿øÀÌ ÁØºñ ¿Ï·á¸¦ ´©¸¥»óÅÂ (FieldIOCPSocket¿¡¼­¸¸ »ç¿ë)
#define	ARENA_STATE_FIGHTING_WARREADY	101	// ¾Æ·¹³ª¹æ¿¡ ÀÔÀåÇÏ¿© 1ºÐ°£ ´ë±âÇÏ´Â »óÅÂ.
#define	ARENA_STATE_FIGHTING_WARING		102	// ¾Æ·¹³ª¹æ¿¡¼­ ÀüÀïÁßÀÎ »óÅÂ.
#define	ARENA_STATE_FIGHTING_WARFINISH	103	// ¾Æ·¹³ª¹æ¿¡¼­ ÀüÀïÀÌ ³¡³ª°í º¸»óÀ» ¹Þ´Â »óÅÂ, ¸¶À»·Î ¿öÇÁÇÏ±â Àü±îÁöÀÇ »óÅÂ.
// 2012-04-12 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ¼±ÅÃÇÑ ¸Ê ¹øÈ£ Ãß°¡
#define	ARENA_MAP_SELECT_ALL			0	
#define	ARENA_MAP_SELECT_RANDOM			9100
// end 2012-04-12 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ¼±ÅÃÇÑ ¸Ê ¹øÈ£ Ãß°¡

const char * GET_ARENA_STATE_STRING(BYTE i_byAState);

#define	SIZE_MAX_TEAM_PW			5

// 2007-04-17 by dhjin, ¾Æ·¹³ª ¸ðµå.
#define	ARENAMODE_DEATHMATCH		1
#define	ARENAMODE_ROUND				2

// 2007-04-17 by dhjin, ÄÉ¸¯ÅÍ ·¹º§ ·©Å© ÀÌ¸§.
#define	SIZE_MAX_LEVELRANK			40

// 2007-04-17 by dhjin, ¼­¹ö¿¡¼­ Áö¿øÇÒ ¼ö ÀÖ´Â ÆÀ ÃÖ´ë ¼ö 
#define SIZE_MAX_TEAM				500

// 2007-04-19 by dhjin, ¼­¹ö¿¡¼­ Áö¿øÇÏ´Â ¾Æ·¹³ª ¼ö
#define SIZE_MAX_ARENA_ROOM			80

// 2007-04-17 by dhjin, ÀÔÀå°¡´ÉÇÑ ·¹º§º° ¹æ
#define	ARENA_STAGE_D_START_LEVEL			11	// ÀÔÀå°¡´ÉÇÑ D¹æ ½ÃÀÛ·¹º§ 11~
#define ARENA_STAGE_D_END_LEVEL				31	// ÀÔÀå°¡´ÉÇÑ D¹æ ¸¶Áö¸··¹º§ 31
#define ARENA_STAGE_C_START_LEVEL			32	// ÀÔÀå°¡´ÉÇÑ C¹æ ½ÃÀÛ·¹º§ 32~
#define ARENA_STAGE_C_END_LEVEL				51	// ÀÔÀå°¡´ÉÇÑ C¹æ ¸¶Áö¸··¹º§ 51
#define ARENA_STAGE_B_START_LEVEL			52	// ÀÔÀå°¡´ÉÇÑ B¹æ ½ÃÀÛ·¹º§ 52~
#define ARENA_STAGE_B_END_LEVEL				71	// ÀÔÀå°¡´ÉÇÑ B¹æ ¸¶Áö¸··¹º§ 71
#define ARENA_STAGE_A_START_LEVEL			72	// ÀÔÀå°¡´ÉÇÑ A¹æ ½ÃÀÛ·¹º§ 72~
// 2010-03-03 by cmkwon, ÃÖ´ë·¹º§»óÇâ °ü·Ã ¾Æ·¹³ª ÀÔÀå½Ã ·¹º§ ¹ö±× - 110À¸·Î ¼öÁ¤, Ä³¸¯ÅÍ ÃÖ´ë·¹º§ »óÇâ½Ã¸¶´Ù ¼öÁ¤ ÇÊ¿ä
//#define ARENA_STAGE_A_END_LEVEL				100	// ÀÔÀå°¡´ÉÇÑ A¹æ ¸¶Áö¸··¹º§ 100
#define ARENA_STAGE_A_END_LEVEL				125	// ÀÔÀå°¡´ÉÇÑ A¹æ ¸¶Áö¸··¹º§ 100

#define ARENA_STAGE_START_LEVEL				25	// 2012-06-08 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ÄÉ¸¯ÅÍ : ¹æ ÀÔÀå·¹º§

// 2012-06-14 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ÄÉ¸¯ÅÍ : ÇÃ·¹ÀÌ Å¸ÀÔ
#define ARENA_PLAY_TYPE_NO						0
#define ARENA_PLAY_TYPE_ATT						1
#define ARENA_PLAY_TYPE_DEF						2
#define ARENA_PLAY_TYPE_DOD						3
// end 2012-06-14 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ÄÉ¸¯ÅÍ : ÇÃ·¹ÀÌ Å¸ÀÔ

// 2007-04-18 by dhjin, ¿¡·¯°ª
#define STATE_ERROR							-1

// 2007-04-20 by dhjin, ¾Æ·¹³ª ½ÂÆÐ ¿©ºÎ 
#define ARENA_ISSUE_WIN						1
#define ARENA_ISSUE_LOSE					2
#define ARENA_ISSUE_DRAW					3

// 2007-05-16 by dhjin, ¾Æ·¹³ª ½ÂÆÐ ¿©ºÎ Á¶°Ç
#define ARENA_END_TYPE_POINT				1	// 2007-05-16 by dhjin, Æ÷ÀÎÆ®°¡ 0ÀÌ µÇ¾î °ÔÀÓÀÌ Á¾·á
#define ARENA_END_TYPE_TIME					2	// 2007-05-16 by dhjin, °æ±â Á¦ÇÑ ½Ã°£ ÃÊ°ú·Î °ÔÀÓÀÌ Á¾·á
#define ARENA_END_TYPE_GIVEUP				3	// 2007-05-16 by dhjin, ¾î´À ÇÑÆÀÀÌ ÀüºÎ ¾Æ·¹³ª ¸Ê¿¡¼­ ³ª°¡ °ÔÀÓÀÌ Á¾·á

// 2007-04-20 by dhjin, ¾Æ·¹³ª Æ½ °£°Ý. 1ÃÊ
#define ARENA_TICK_INTERVAL			1000

// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö
#define SECURITY_TICK_INTERVAL		1000

// 2012-07-11 by hskim, ¼±ÅÃÀû ¼Ë´Ù¿î
#define SELECTIVE_SHUTDOWN_TICK_INTERVAL		1000

// 2011-07-21 by hskim, ÀÎÁõ ¼­¹ö ±¸Çö - Á¢¼ÓÈÄ Timeout Ã³¸®
#define SPTOMP_TICK_INTERVAL		1000

// 2011-06-22 by hskim, »ç¼³ ¼­¹ö ¹æÁö
#define STATISTICS_TICK_INTERVAL		600000		// 10 ºÐ

// 2007-04-27 by dhjin, ¿äÃ»ÇÑ ÆÀ ¸ñ·ÏÀ» ÇÑ ¹ø¿¡ ÀüÇØÁÖ´Â ¾ç
#define ARENA_MAX_REQUEST_TEAM				20

// 2007-05-09 by dhjin, ¾Æ·¹³ª ÁØºñ ¹öÆ° µô·¹ÀÌ ´ÜÀ§ : ÃÊ
#define ARENA_READYBUTTON_INTERVAL			3

// 2007-06-05 by dhjin, ¾Æ·¹³ª ´ë±â ÆÀ Á¤º¸ ¿äÃ» Á¤ÀÇ
#define ARENA_REQUEST_TEAM_STATE_OUR		1	// 2007-06-05 by dhjin, ¸ðµÎ º¸±â
#define ARENA_REQUEST_TEAM_STATE_JOINING	2	// 2007-06-05 by dhjin, ´ë±â ÆÀ
#define ARENA_REQUEST_TEAM_STATE_OTHER		3	// 2007-06-05 by dhjin, »ó´ë ÆÀ

///////////////////////////////////////////////////////////////////////////////
// 2007-04-17 by dhjin, Arena ½Ã½ºÅÛ¿¡ ÇÊ¿äÇÑ ±¸Á¶Ã¼ Á¤ÀÇ
struct SDBARENA_INFO
{// 2007-04-17 by dhjin, ti_ArenaInfo °ª
	BYTE	ArenaMode;				// ¾Æ·¹³ª ¸ðµå 1 : DeathMatch, 2 : Round
	BYTE	ReqMemberNum;			// ÆÀ ÀÎ¿ø Á¦ÇÑ
	INT		PayInfluencePointWIN;	// ½Â¸® ¼¼·Â¿¡°Ô º¸»óµÇ´Â ¼¼·ÂÆ÷ÀÎÆ®°ª
	INT		PayInfluencePointLOSE;	// ÆÐ¹è ¼¼·Â¿¡°Ô º¸»óµÇ´Â ¼¼·ÂÆ÷ÀÎÆ®°ª
	INT		PayWarPointWIN;			// ½Â¸® ÆÀ¿¡°Ô º¸»óµÇ´Â WP°ª
	INT		PayWarPointLOSE;		// ÆÐ¹è ÆÀ¿¡°Ô º¸»óµÇ´Â WP°ª
//////////////////////////////////////////////////////////////////////////////
// 2012-05-23 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ÃÖ¼Ò º¸»ó ¿öÆ÷ ÇÊµå Ãß°¡
	INT		DefaultPayWarPointWIN;	/// º¸»óÈ½¼ö ÃÊ°ú½Ã ±âº»ÀûÀ¸·Î Áö±ÞµÇ´Â WP
	INT		DefaultPayWarPointLOSE;	/// º¸»óÈ½¼ö ÃÊ°ú½Ã ±âº»ÀûÀ¸·Î Áö±ÞµÇ´Â WP
// end 2012-05-23 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ÃÖ¼Ò º¸»ó ¿öÆ÷ ÇÊµå Ãß°¡
};
typedef vector<SDBARENA_INFO>	vectorSDBARENA_INFO;

struct SDBARENA_MAPINFO
{// 2007-04-17 by dhjin, ti_ArenaMapInfo °ª 
	MapIndex_t 	ArenaMapIndex;			// ¸Ê ÀÎµ¦½º
	BYTE 	ArenaMode;					// ¾Æ·¹³ª ¸ðµå 1 : DeathMatch, 2 : Round
	BYTE 	ReqMemberNum; 				// ÆÀ ÀÎ¿ø Á¦ÇÑ
	BYTE 	PlayLimitedTime;			// °æ±â Á¦ÇÑ ½Ã°£
	BYTE 	WINCondition;				// °æ±â ½Â¸® Á¶°Ç
	INT		LvDSupplyItemNum1;				// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ1
	INT		LvDSupplyItemCount1;			// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ1 ¼ö 
	INT		LvDSupplyItemNum2;				// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ2
	INT		LvDSupplyItemCount2;			// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ2 ¼ö
	INT		LvDSupplyItemNum3;				// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ3
	INT		LvDSupplyItemCount3;			// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ3 ¼ö
	// 2012-09-14 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß part3 - ¾Æ·¹³ª Áö±Þ ¾ÆÀÌÅÛ Ãß°¡ (¾Æ·¹³ª Àü¿ë Åº¾à ÀçÃæÀü Ä«µå)
	INT		LvDSupplyItemNum4;				// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ4
	INT		LvDSupplyItemCount4;			// ¾Æ·¹³ª Àü¿ë Áö±Þ ¾ÆÀÌÅÛ4 ¼ö
	// end 2012-09-14 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß part3 - ¾Æ·¹³ª Áö±Þ ¾ÆÀÌÅÛ Ãß°¡ (¾Æ·¹³ª Àü¿ë Åº¾à ÀçÃæÀü Ä«µå)
#ifdef _ARENA_ADDITIONAL_SUPPLY_ITEMS
	INT		LvDSupplyItemNum5;
	INT		LvDSupplyItemCount5;
#endif
};
typedef vector<SDBARENA_MAPINFO>	vectorSDBARENA_MAPINFO;

struct SARENA_INFO
{// 2007-04-17 by dhjin, À¯Àú°¡ °¡Áö°í ÀÖ´Â Á¤º¸.
	BYTE	ArenaMode;				// Arena ¸ðµå Á¤º¸
	BYTE	State;					// Arena »óÅÂ Á¤º¸
	BYTE	ShootingDown;			// Arena ¹æ¿¡¼­ °ÝÃß½ÃÅ² Á¤º¸
	BYTE	SufferingAttack;		// Arena ¹æ¿¡¼­ ÇÇ°Ý´çÇÑ Á¤º¸
	BYTE	LostPoint;				// Arena ¹æ¿¡¼­ ÇÇ°Ý ´çÇÏÁö ¾Ê°í Á×Àº Ä«¿îÆ®
	SHORT	SaveSP;					// Arena ¹æ ÀÔÀå Àü SP
	SHORT	SaveEP;					// Arena ¹æ ÀÔÀå Àü EP
	INT		TeamNum;				// Arena ÆÀ ¹øÈ£
	INT		DeathMatchRoomNum;		// Arena µ¥½º¸ÅÄ¡ ¹æ ¹øÈ£
	BodyCond_t	SaveBodyCondition;	// Arena ¹æ ÀÔÀå Àü BodyCondition
	float	SaveHP;					// Arena ¹æ ÀÔÀå Àü HP
	float	SaveDP;					// Arena ¹æ ÀÔÀå Àü DP
	ATUM_DATE_TIME	ReadyButtonClickTime;	// Arena ÁØºñ ¿Ï·á ¹öÆ° ´©¸¥ ½Ã°£ ÀúÀå, 3ÃÊ µô·¹ÀÌ ÇÊ¿ä
	BOOL	WatchState;				// Arena °üÀü »óÅÂ 0 : °üÀü »óÅÂ°¡ ¾Æ´Ï´Ù. 1 : °üÀü »óÅÂ				
	BOOL	Command;				// 2008-02-21 by dhjin, ¾Æ·¹³ª ÅëÇÕ - '/¾Æ·¹³ªÀÌµ¿' ¸í·É¾î »ç¿ëÀ¸·Î ¾Æ·¹³ª ¼­¹ö·Î ¿Â°ÍÀÎÁö Ã¼Å© ºñÆ®

	///////////////////////////////////////////////////////////////////////////////
	/// \fn			void ResetArenaInfo(void)
	/// \brief		
	/// \author		cmkwon
	/// \date		2007-05-14 ~ 2007-05-14
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	void ResetArenaInfo(void)
	{
		MEMSET_ZERO(this, sizeof(SARENA_INFO));
		TeamNum				= -1;
		DeathMatchRoomNum	= -1;
	}
	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsValidArenaState(void)
	/// \brief		
	/// \author		cmkwon
	/// \date		2007-05-14 ~ 2007-05-14
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsValidArenaState(void)
	{
		if(ARENA_STATE_NONE == State)
		{
			return FALSE;
		}

		if(0 > TeamNum
			&& 0 > DeathMatchRoomNum)
		{
			return FALSE;
		}

		return TRUE;
	}
};

struct SARENA_PAY_INFO
{// 2007-04-17 by dhjin, ¾Æ·¹³ª º¸»ó Á¤º¸
	INT		PayInfluencePointWIN;	// ½Â¸® ¼¼·Â¿¡°Ô º¸»óµÇ´Â ¼¼·ÂÆ÷ÀÎÆ®°ª
	INT		PayInfluencePointLOSE;	// ÆÐ¹è ¼¼·Â¿¡°Ô º¸»óµÇ´Â ¼¼·ÂÆ÷ÀÎÆ®°ª
	INT		PayWarPointWIN;			// ½Â¸® ÆÀ¿¡°Ô º¸»óµÇ´Â WP°ª
	INT		PayWarPointLOSE;		// ÆÐ¹è ÆÀ¿¡°Ô º¸»óµÇ´Â WP°ª
	//////////////////////////////////////////////////////////////////////////////
	// 2012-05-23 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ÃÖ¼Ò º¸»ó ¿öÆ÷ ÇÊµå Ãß°¡
	INT		DefaultPayWarPointWIN;
	INT		DefaultPayWarPointLOSE;
	// end 2012-05-23 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ß - ÃÖ¼Ò º¸»ó ¿öÆ÷ ÇÊµå Ãß°¡
};

struct SARENA_MAP_MANAGER
{// 2007-05-02 by dhjin, ¾Æ·¹³ª ¸Ê Á¤º¸
	MapIndex_t		ArenaMap;			// 2007-04-30 by dhjin, ¾Æ·¹³ª ¸Ê
	ChannelIndex_t	ArenaMapChannel;	// 2007-04-30 by dhjin, ¾Æ·¹³ª Ã¤³Î
	BOOL			bArenaChannelSet;	// 2007-04-30 by dhjin, ¾Æ·¹³ª Ã¤³Î ÇÒ´ç ¿©ºÎ 0 : ÇÒ´çµÇÁö ¾ÊÀ½, 1 : ÇÒ´çµÊ
};
typedef mt_vector<SARENA_MAP_MANAGER>				mtvectSARENA_MAP_MANAGER;

typedef mt_vector<ClientIndex_t>					mtvectorClientIndex_t;

struct SARENA_OTHER_TEAM_INFO
{// 2007-05-28 by dhjin, »ó´ëÆÀ Á¤º¸ ÀúÀå ÈÄ Å¬¶óÀÌ¾ðÆ®¿¡°Ô Àü¼Û
	BYTE ArenaMode;								// ¾Æ·¹³ª ¸ðµå 1 : DeathMatch, 2 : Round
	BYTE TeamSize;								// ÆÀ ÃÖ´ë ÀÎ¿ø¼ö
};
typedef vector<SARENA_OTHER_TEAM_INFO>				vectSARENA_OTHER_TEAM_INFO;

struct SARENA_GM_COMMAND_INFO_TEAM
{// 2007-05-28 by dhjin, /¾Æ·¹³ª GM¸í·É¾î Á¤º¸ Àü¼Û
	BYTE 	ArenaMode;							// ¾Æ·¹³ª ¸ðµå 1 : DeathMatch, 2 : Round
	BYTE 	TeamState;							// »óÅÂ Á¤º¸
	BYTE	Level;								// ÆÀ ·¹º§
	BYTE	MemberCount;						// ÆÀ ÃÖ´ë ÀÎ¿ø¼ö
	INT		CurrenMemberCount;					// ÇöÀç ÆÀ ÀÎ¿ø¼ö
};
typedef vector<SARENA_GM_COMMAND_INFO_TEAM>				vectSARENA_GM_COMMAND_INFO_TEAM;


///////////////////////////////////////////////////////////////////////////////
// 2007-05-11 by cmkwon, ¹Ì½ºÅ×¸®Ä¸½¶ ½Ã½ºÅÛ
struct MYSTERY_ITEM_DROP
{
	INT				MysteryItemDropNum;				
	USHORT			ReqUnitKind;					// ÇÊ¿äÀ¯´ÖÁ¾·ù
	BYTE			ReqMinLevel;					// ÇÊ¿ä ÃÖÀú ·¹º§
	BYTE			ReqMaxLevel;					// ÇÊ¿ä ÃÖÀú ·¹º§
	INT				DropItemNum;
	INT				MinCount;
	INT				MaxCount;
	Prob1000K_t		Probability;			// Áö±Þ È®·ü(»ó´ë°ªÀ¸·Î Ã³¸®µÊ): 0 ~ 1000000
	Prob100K_t		PrefixProbability;		// Á¢µÎ»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
	Prob100K_t		SuffixProbability;		// Á¢¹Ì»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
	USHORT			Period;					// ·°Å°¸Ó½Å ±â°£, 2008-11-04 by dhjin, ·°Å°¸Ó½Å
	INT				CountPerPeriod;			// ±â°£µ¿¾È µå¶øµÉ ÃÖ´ë ¾ÆÀÌÅÛ ¼ö, 2008-11-04 by dhjin, ·°Å°¸Ó½Å
	INT				DropCount;				// ±â°£µ¿¾È ÇöÀç µå¶øµÈ ¾ÆÀÌÅÛ ¼ö, 2008-11-04 by dhjin, ·°Å°¸Ó½Å
	ATUM_DATE_TIME	Starttime;				// ·°Å°¸Ó½Å ½ÃÀÛ ½Ã°£, 2008-11-04 by dhjin, ·°Å°¸Ó½Å
};
typedef multimap<INT, MYSTERY_ITEM_DROP>		mmapINT2MYSTERY_ITEM_DROP;		// 2007-05-14 by cmkwon
typedef mt_multimap<INT, MYSTERY_ITEM_DROP>		mtmapINT2MYSTERY_ITEM_DROP;		// 2008-11-10 by dhjin, ·°Å°¸Ó½Å multimap -> mt_multimapÀ¸·Î º¯°æ
typedef vector<MYSTERY_ITEM_DROP*>				vectMYSTERY_ITEM_DROPPtr;			// 2007-05-14 by cmkwon

// 2008-11-04 by dhjin, ·°Å°¸Ó½Å
struct LUCKYITEM_DROP_INFO
{
	BOOL			LuckItemUsing;
	USHORT			Period;
	UID64_t			MysteryItemDropNumUID;
	INT				MysteryItemDropNum;				
	INT				DropItemNum;
	INT				MinCount;
	INT				MaxCount;
	Prob100K_t		PrefixProbability;		// Á¢µÎ»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
	Prob100K_t		SuffixProbability;		// Á¢¹Ì»ç°¡ ºÙÀ» È®·ü, 0 ~ 100000
};

///////////////////////////////////////////////////////////////////////////////
// 2007-05-28 by cmkwon
struct SRESOBJ_CHECKSUM
{
	char		szResObjFileName[SIZE_MAX_RESOBJ_FILE_NAME];
	int			nFileSize;
	// 2009-05-29 by cmkwon, Hash¾Ë°í¸®Áò Ãß°¡(SHA256) - 
	//UINT		uiObjCheckSum;	
	BYTE		byDigest[32];		// 2009-05-29 by cmkwon, Hash¾Ë°í¸®Áò Ãß°¡(SHA256) - 

	// 2009-05-29 by cmkwon, Hash¾Ë°í¸®Áò Ãß°¡(SHA256) - 
	BOOL IsSame(int i_nLen, BYTE i_byCheckSum[32])
	{
		if(i_nLen != nFileSize)
		{
			return FALSE;
		}
		
		for(int i=0; i < 4 ;i++)
		{
			if(((INT64*)i_byCheckSum)[i] != ((INT64*)byDigest)[i])
			{
				return FALSE;
			}
		}

		return TRUE;
	}
};
typedef map<string, SRESOBJ_CHECKSUM>			mapstring2SRESOBJ_CHECKSUM;		// 2007-05-28 by cmkwon

//////////////////////////////////////////////////////////////////////////
// 2007-07-06 by dhjin, Tutorial
struct STUTORIAL_INFO
{// 2007-07-06 by dhjin, Tutorial Á¤º¸
	INT			TutorialUID;					// Tutorial ¹øÈ£ 
	INT			TutorialPayItem1;				// Tutorial ¾ÆÀÌÅÛ ¹øÈ£ 
	INT			TutorialPayItemCount1;			// Tutorial ¾ÆÀÌÅÛ ¼ö
	INT			TutorialPayItem2;				
	INT			TutorialPayItemCount2;
	INT			TutorialPayItem3;
	INT			TutorialPayItemCount3;
};
typedef vector<STUTORIAL_INFO>	vectSTutorialInfo;			// 2007-07-06 by dhjin
typedef vector<INT>				vectTutorial;				// 2007-07-06 by dhjin
// 2010-01-18 by cmkwon, ¾ÆÀÌÅÛ »ç¿ë½Ã Parameter Áßº¹ Ã¼Å© ½Ã½ºÅÛ ±¸Çö - À§ÂÊÀ¸·Î ÀÌµ¿
//typedef vector<INT>				vectINT;					// 2008-01-31 by cmkwon, ½Ã°£Á¦ÇÑ ¾ÆÀÌÅÛ DB ¿¡ Ãß°¡ Áß Ã¼Å© - 

// 2008-01-31 by cmkwon, ½Ã°£Á¦ÇÑ ¾ÆÀÌÅÛ DB ¿¡ Ãß°¡ Áß Ã¼Å© - find_if() ÇÔ¼öÀÚ
struct Sfind_if_INT
{
	Sfind_if_INT(int i_nValue): m_nFindValue(i_nValue){};
	bool operator()(INT i_nValue)
	{
		return i_nValue == m_nFindValue;
	}
	int m_nFindValue;
};


// 2007-07-09 by dhjin, ±âº» ¹«±â
#define FIRST_WEAPON_AGEAR					7000050
#define SECOND_WEAPON_AGEAR					7900440
#define FIRST_WEAPON_BGEAR					7001950
#define SECOND_WEAPON_BGEAR					7900000
#define FIRST_WEAPON_MGEAR					7001950
#define SECOND_WEAPON_MGEAR					7900000
#define FIRST_WEAPON_IGEAR					7001950
#define SECOND_WEAPON_IGEAR					7900000

// 2007-07-11 by dhjin, ¸ó½ºÅÍ ¼ÒÈ¯ ½Ã°£ ´ÜÀ§ MS
#define TICK_CREATE_MONSTER_TERM			1000

//////////////////////////////////////////////////////////////////////////
// 2007-08-03 by dhjin, PC¹æ IP¸®½ºÆ® ¼öÁ¤°ú °ü·ÃµÈ ±¸Á¶Ã¼
struct SIPClassD
{
	UID32_t		PCBangUID;			// 2007-08-03 by dhjin, PC¹æ UID 
	BYTE		IPClassDStart;		// 2007-08-03 by dhjin, ½ÃÀÛ IP Class D
	BYTE		IPClassDEnd;		// 2007-08-03 by dhjin, ³¡ IP Class D
};
typedef vector<SIPClassD>					vectorSIPClassD;
typedef mt_map<DWORD, vectorSIPClassD>		mtmapDWORDvectorSIPClassD;

//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, OutPost °ü·Ã
#define	OUTPOST_NORMAL		0x00
#define	OUTPOST_WARSTART	0x10
#define	OUTPOST_WARING		0x20
#define	OUTPOST_WAREND		0x30
#define	OUTPOST_RESETSTART	0x01
#define	OUTPOST_RESETING	0x02
#define	OUTPOST_RESETEND	0x03
#define IS_OUTPOST(x)		( (x & 0xFF) > 0 )
#define IS_OUTPOST_WAR(x)	( (x & 0xF0) > 0 )
#define IS_OUTPOST_RESET(x)	( (x & 0x0F) > 0 )
#define IS_OUTPOST_RESETING(x) ( (x &  OUTPOST_RESETING) == OUTPOST_RESETING)

//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, °øÁö»çÇ× ÁöµµÀÚ, ÀüÁø±âÁö ¿©´ÜÀåÀÎÁö ±¸ºÐ »ó¼ö
#define	NOTICE_LEADER			1
#define	NOTICE_GUILDCOMMANDER	2

// 2007-08-13 by dhjin, °øÁö»çÇ× ¹®ÀÚ¿­ Å©±â
#define	SIZE_MAX_NOTICE			512

// 2007-11-06 by dhjin, ÀüÁø±âÁö ½Â¸®½Ã ¹ÞÀ» ¾ÆÀÌÅÛ 
#define	OUTPOST_PAY_ITEM		7010720	// º¸±Þ»óÀÚ ¾ÆÀÌÅÛ
#define	OUTPOST_PAY_ITEM_COUNT	1		// º¸±Þ»óÀÚ ¾ÆÀÌÅÛ ¼ö

//////////////////////////////////////////////////////////////////////////
// 2007-08-16 by dhjin, ÀüÁø±âÁö °ü·Ã Á¤º¸
struct	SOUTPOST_INFO
{
	BYTE			OutPostInfluence;						// ÀüÁø±âÁö ¼ÒÀ¯ ¼¼·Â
	BYTE			OutPostNextWarTimeSet;					// ÀüÁø±âÁö ´ÙÀ½ ÀüÀï ½Ã°£ ¼³Á¤
	INT				OutPostMapIndex;						// ÀüÁø±âÁö ¸Ê ¹øÈ£
	INT				OutPostCityMapIndex;					// ÀüÁø±âÁö µµ½Ã ¸Ê ¹øÈ£
	UID32_t			OutPostGuildUID;						// ÀüÁø±âÁö ¼ÒÀ¯ ¿©´Ü
	CHAR			OutPostGuildName[SIZE_MAX_GUILD_NAME];	// ÀüÁø±âÁö ¼ÒÀ¯ ¿©´Ü¸í
	ATUM_DATE_TIME	OutPostGetTime;							// ÀüÁø±âÁö ¼ÒÀ¯ ½ÃÀÛ ½Ã°£
	ATUM_DATE_TIME	OutPostNextWarTime;						// ÀüÁø±âÁö ´ÙÀ½ ÀüÀï ½Ã°£
};
typedef vector<SOUTPOST_INFO>			vectSOutPostInfo;

// 2009-07-08 by cmkwon, ÀüÀï °ü·Ã Á¤ÀÇ À§Ä¡ ÀÌµ¿(LocalizationDefineCommon.h) - 
//// 2007-08-16 by dhjin, ÀüÁø±âÁö °ü·Ã define
//#define	OUTPOST_WARTIME					120	// ÀüÁø±âÁöÀü ½Ã°£, 120ºÐ
//#define OUTPOST_WARTIME_FOR_TESTSERVER	60	// 2008-10-29 by cmkwon, Å×¼·Àº ÀüÁø±âÁöÀü 1½Ã°£(60ºÐ)À¸·Î ¼³Á¤ - 

// 2009-05-12 by cmkwon, (ÀÏº»¿äÃ») ÀÏº»¸¸ ÀüÁø ±âÁöÀü ÁÖ±â 7ÀÏ·Î ¼öÁ¤ - LocalizationDefineCommon.h ·Î À§Ä¡ ÀÌµ¿
//#define	OUTPOST_NEXTWARGAP				5	// 2008-10-22 by dhjin, ÀüÁø±âÁö 3Â÷ 10 -> 5ÀÏ·Î ¼öÁ¤ // ´ÙÀ½ ÀüÁø±âÁöÀü±îÁöÀÇ ÀÏ ¼ö, 10ÀÏ 

#define	OUTPOST_RESET					5	// ÀüÁø±âÁö °¢ÀÎ ½ÃÅ°´Â ½Ã°£, 1ºÐ
#define	OUTPOST_PAY_WARPOINT			600	// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // ÀüÁø±âÁöÀü ½Â¸® ¼¼·Â Áö±Þ WarPoint
#define	OUTPOST_PAY_WARPOINT_BY_LOSE	200	// 2011-12-27 by hskim, EP4 [¹ë·±½º Á¶Á¤] // 2008-01-07 by dhjin, ¼¼·Â º¸»ó ¼öÁ¤ - ÀüÁø±âÁöÀü ÆÐ¹è ¼¼·Â Áö±Þ WarPoint

//////////////////////////////////////////////////////////////////////////
// 2007-08-16 by dhjin, µµ½Ã ÁöµµÀÚ, ¿©´ÜÀå °ü·Ã Á¤º¸
struct	SCITYLEADER_INFO
{
	INT			MapIndex;					// °ü¸®µÇ´Â µµ½Ã ¸Ê ¹øÈ£ ex)1001 : ÁöµµÀÚ °ü·Ã ¸Ê
	BYTE		Influence;					// ¼¼·Â
	UID32_t		CharacterUID;				// À¯Àú UID
	INT			ExpediencyFundCumulative;	// ´©Àû ÆÇ°ø ºñ
	INT			ExpediencyFund;				// °¡¿ë ÆÇ°ø ºñ
	INT			ExpediencyFundRate;			// ÆÇ°øºñÀ², %´ÜÀ§, 1000´ÜÀ§·Î °è»ê 10 => 1%
	CHAR		Notice[SIZE_MAX_NOTICE];	// °øÁö»çÇ×
};
typedef vector<SCITYLEADER_INFO>		vectSCityLeaderInfo;

// 2007-08-16 by dhjin, ÆÇ°øºñ °ü·Ã define, ÆÇ°øºñÀ², %´ÜÀ§, 1000´ÜÀ§·Î °è»ê 10 => 1%
#define	EXPEDIENCYFUND_LEADER				10		// ¼¼·Â ÁöµµÀÚ ÆÇ°øºñÀ²
#define	EXPEDIENCYFUND_GUILDCOMMANDER		10		// ÀüÁø±âÁö ¼ÒÀ¯ ¿©´Ü ÆÇ°øºñÀ²
#define	EXPEDIENCYFUND_LEADER_OUTPOST		5		// ÀüÁø±âÁö ¼ÒÀ¯ ¼¼·Â ÁöµµÀÚ Ãß°¡ ÆÇ°øºñÀ²
#define	EXPEDIENCYFUND_LEADER_INFLUENCEWAR	2		// ¸ð¼±Àü °ø°Ý ¼º°ø ½Ã Ãß°¡ »ó½Â ºñÀ²
#define EXPEDIENCYFUND_RATE_VALUE			1000	// ÆÇ°øºñÀ², %´ÜÀ§, 1000´ÜÀ§·Î °è»ê 10 => 1%

// 2007-08-16 by dhjin, °øÁö»çÇ× °ü·Ã define
#define	NOTICE_LEADER						1	// °øÁö»çÇ× ÁöµµÀÚ
#define	NOTICE_GUILDCOMMANDER				2	// °øÁö»çÇ× ÀüÁø±âÁö ¿©´ÜÀå
#define	SIZE_MAX_NOTICE						512	// °øÁö»çÇ× ¹®ÀÚ¿­ Å©±â

// 2007-08-30 by dhjin, ¸ð¼±Àü½Ã Àü·«Æ÷ÀÎÆ® »ý¼º ½Ã°£ 
#define STRATEGYPOINT_SUMMONTIME_BY_INFLUENCEBOSS	120 // ¸ð¼±Àü½Ã Àü·«Æ÷ÀÎÆ® »ý¼º ½Ã°£ 120ºÐ

// 2007-09-03 by dhjin, Àü·«Æ÷ÀÎÆ® »ý¼º ½Ã°£ ´Ù½Ã ¼³Á¤ ½Ã °¡Á®¾ßµÇ´Â Gap
#define STRATEGYPOINT_SUMMON_GAP_BY_INFLUENCEWAR	4	// ¼¼·ÂÀüÀï ½Ã °¡Á®¾ßµÇ´Â GAP
#define STRATEGYPOINT_SUMMON_GAP_BY_OUTPOST			8	// ÀüÁø±âÁö ÀüÀï ½Ã °¡Á®¾ßµÇ´Â GAP

// 2007-09-04 by dhjin, ¸ð¼±Àü½Ã ÅÚ·¹Æ÷Æ® °ü·Ã
#define TELEPORT_BUILDING_TIME_SECOND				180   // ¸ð¼±Àü½Ã ÅÚ·¹Æ÷Æ®°¡ ¸¸µé¾îÁö´Â ½Ã°£(180ÃÊ)

// 2007-09-05 by dhjin, ÅÚ·¹Æ÷Æ® »óÅÂ°ª
#define TELEPORT_STATE_NOT_SUMMON					0	// ÅÚ·¹Æ÷Æ® ¼ÒÈ¯ »óÅÂ°¡ ¾Æ´Ñ °ª
#define TELEPORT_STATE_NONE							1	// ÅÚ·¹Æ÷Æ® Æò»ó½Ã »óÅÂ°ª
#define TELEPORT_STATE_READY						2   // Àü·«Æ÷ÀÎÆ®°¡ ÆÄ±«µÇ¾î ÅÚ·¹Æ÷Æ® ¼³Ä¡°¡ °¡´ÉÇÑ »óÅÂ
#define TELEPORT_STATE_BUILDING						3   // ÅÚ·¹Æ÷Æ® ºôµù Áß
#define TELEPORT_STATE_COMPLETE						4	// ÅÚ·¹Æ÷Æ® ¼³Ä¡µÇ¾î ÀÖ´Â »óÅÂ

// 2007-09-05 by dhjin, Monster ClickEvent ÇÊµå °ª
#define MONSTER_CLICKEVENT_OUTPOST_RESET			1	// ÀüÁø±âÁö Áß¾ÓÃ³¸®ÀåÄ¡ Å¬¸¯ ÀÌº¥Æ® (¸®¼Â)
#define MONSTER_CLICKEVENT_TELEPORT_BUILDING		2	// ÅÚ·¹Æ÷Æ® Å¬¸¯ ÀÌº¥Æ® (ºôµù)

struct STELEPORT_INFO_BUILDING
{// 2007-09-07 by dhjin, ÇöÀç »ý¼ºµÇ¾î ÀÖ´Â ÅÚ·¹Æ÷Æ® Á¤º¸
	BOOL			bBuilded;		// ÅÚ·¹Æ÷Æ®°¡ ¿Ï¼ºµÇ¾úÀ¸¸é TRUE, ºôµùÁßÀÌ¶ó¸é FALSE
	INT				MapInfluence;	// ¼¼·Â 
	MapIndex_t		MapIndex;
	ATUM_DATE_TIME	BuildStartTime;
};

// 2007-10-06 by cmkwon, ºÎÁöµµÀÚ 2¸íÀÇ È£ÄªÀ» ´Ù¸£°Ô ¼³Á¤ -
char *GetLeader8SubLeaderString(BYTE i_byInflTy, USHORT i_usRace);

// 2007-10-10 by dhjin, ¸É¹ö½± À¯Àú ¿öÇÁºñ¿ë Àý°¨ %
#define	MEMBERSHIP_DISCOUNT_WARPFEE		50		// 2007-10-10 by dhjin, 50%

// 2007-10-16 by cmkwon, ·Î±× Ãß°¡ - OutPost.h ÆÄÀÏ¿¡ ÀÖ´ø°ÍÀ» ¿©±â·Î ÀÌµ¿ÇÔ
struct SOutPostProtectorDamage
{
	BYTE	Influence;
	UID32_t	GuildUID;
	CHAR 	GuildName[SIZE_MAX_GUILD_NAME];
	float	Damage;
};
typedef	mt_vector<SOutPostProtectorDamage>		mtvectSOutPostProtectorDamage;

// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - Ãß°¡ÇÔ
extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];
extern void GDecryptGameServerInfoByXOR(void);

// 2007-10-30 by cmkwon, ¼¼·Âº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö -
extern char *GetDayOfWeekString(int i_nDayOfWeek);

// 2007-10-30 by cmkwon, ¼¼·Âº° ÇØÇÇ¾Æ¿ö ÀÌº¥Æ® ±¸Çö -
extern int GetArrayIndexByInfluenceType(BYTE i_byInflTy);
extern BYTE GetInfluenceTypeByArrayIndex(int i_nArrIdx);


//////////////////////////////////////////////////////////////////////////
// 2007-10-26 by dhjin, Poll °ü·Ã

struct SACTION_BY_LEVEL_DB
{// 2007-10-26 by dhjin, ·¹º§¿¡ µû¸¥ ¿©·¯ Çàµ¿µé
	BYTE		Level;
	INT			PollPoint;		// ·¹º§¿¡ µû¸¥ ÅõÇ¥ Á¡¼ö
};
typedef vector<SACTION_BY_LEVEL_DB>		vectSACTION_BY_LEVEL_DB;

struct SPOLLDATE_DB
{// 2007-10-26 by dhjin, ¼±°Å ÀÏÁ¤
	ATUM_DATE_TIME	ApplicationStartDate;		// ÈÄº¸ ½ÅÃ» ½ÃÀÛ ³¯
	ATUM_DATE_TIME	ApplicationEndDate;			// ÈÄº¸ ½ÃÃ» ³¡³ª´Â ³¯
	ATUM_DATE_TIME	VoteStartDate;				// ÅõÇ¥ ½ÃÀÛ ³¯
	ATUM_DATE_TIME	VoteEndDate;				// ÅõÇ¥ ¸¶Áö¸· ³¯
	ATUM_DATE_TIME	Election;					// ¼±ÃâÀÏ
};

struct SVOTER_LIST
{// 2007-10-26 by dhjin, ÅõÇ¥ÀÚ ¸®½ºÆ® 
	UID32_t		AccountUID;						//	ÅõÇ¥ÀÚ °èÁ¤ UID
	UID32_t		CharacterUID;					//	ÅõÇ¥ÀÚ UID
	INT			LeaderCandidateNum;				//	ÅõÇ¥ÀÚ°¡ ÅõÇ¥ÇÑ ÈÄº¸ÀÚ ¹øÈ£
};
typedef mt_vector<SVOTER_LIST>			mtvectSVOTER_LIST;

#define SIZE_MAX_CAMPAIGNPROMISES				512

struct SLEADER_CANDIDATE
{// 2007-10-26 by dhjin, ÁöµµÀÚ ÈÄº¸µé
	BYTE			Influence;									//  ÁöµµÀÚ ÈÄº¸ÀÇ ¼¼·Â
	BOOL			DeleteCandidate;							//  ÀÌ¹ø ¼±°Å ±â°£¿¡ ÈÄº¸ Å»Åð Çß´ÂÁö ¿©ºÎ, TRUE = Å»Åð
	INT				LeaderCandidateNum;							//	ÁöµµÀÚ ÈÄº¸ ¹øÈ£
	INT				PollCount;									//  ÁöµµÀÚ ÈÄº¸ µæÇ¥¼ö
	UID32_t			AccountUID;									//	ÁöµµÀÚ ÈÄº¸ °èÁ¤ UID
	UID32_t			CharacterUID;								//	ÁöµµÀÚ ÈÄº¸ UID
	UID32_t			GuildUID;									//  ÁöµµÀÚ ÈÄº¸ ±æµå UID
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  ÁöµµÀÚ ÈÄº¸ ÀÌ¸§
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  ÁöµµÀÚ ÈÄº¸ ±æµå¸í
	CHAR			CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];//  ÁöµµÀÚ ÈÄº¸ °ø¾à	
};
typedef mt_vector<SLEADER_CANDIDATE>		mtvectSLEADER_CANDIDATE;

#define LEADERCANDIDATE_REQUIRE_LEVEL			125
#define LEADERCANDIDATE_REQUIRE_PROPENSITY		1000
#define LEADERCANDIDATE_REQUIRE_GUILDMEMBERNUM  1		// 2007-11-28 by dhjin, ¿©´Ü¿ø ÀÎ¿ø Ã¼Å© ¾øÀ½ - ±âÈ¹ ¿äÃ»
#define LEADERCANDIDATE_REQUIRE_GUILDFAME		1000
#define LEADERVOTE_REQUIRE_LEVEL				125
#define LEADERVOTE_REQUIRED_FAME				1000

///////////////////////////////////////////////////////////////////////////////
// 2007-11-28 by cmkwon, ÅëÁö½Ã½ºÅÛ ±¸Çö -
#define SIZE_MAX_NOTIFY_MSG_STRING				512		// 2007-11-28 by cmkwon, ½ºÆ®¸µ ÃÖ´ë ±æÀÌ
#define NOTIFY_MSG_TYPE_TEXT					1		// ÅëÁö¸Þ½ÃÁöÅ¸ÀÔ - ÀÏ´Ü ½ºÆ®¸µ
#define NOTIFY_MSG_TYPE_RECV_GIFT				2		// ÅëÁö¸Þ½ÃÁöÅ¸ÀÔ - ¾ÆÀÌÅÛ ¼±¹° ¹ÞÀº ¸Þ½ÃÁö
struct SNOTIFY_MSG		// 2007-11-28 by cmkwon, ÅëÁö½Ã½ºÅÛ ±¸Çö -
{
	UID64_t			NotifyMsgUID;				// 
	UID32_t			CharacterUID;				// ÅëÁö¸Þ½ÃÁö ¹ÞÀ» Ä³¸¯ÅÍ UID, 0 ÀÌ¸é ¸ðµç Ä³¸¯ÅÍ¿¡°Ô Àü¼Û(0 ÀÎ°ÍÀº À¯Àú°¡ Áö¿ï ¼ö ¾ø´Ù)
	BYTE			NotifyMsgType;				// NOTIFY_MSG_TYPE_XXX
	char			NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];	// ÅëÁö¸Þ½ÃÁö ½ºÆ®¸µ
	UID32_t			SenderCharacterUID;								// ÅëÁö¸Þ½ÃÁö Àü¼ÛÇÑ CharacterUID
	char			SenderCharacterName[SIZE_MAX_CHARACTER_NAME];	// ÅëÁö¸Þ½ÃÁö Àü¼ÛÇÑ CharacterName
	ATUM_DATE_TIME	CreateTime;										// ÅëÁö¸Þ½ÃÁö DB¿¡ »ý¼ºµÈ ½Ã°£
};
typedef mt_vector<SNOTIFY_MSG>		mtvectSNOTIFY_MSG;				


//////////////////////////////////////////////////////////////////////////
// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö Á¤º¸
struct SARENA_SERVER_INFO
{
	BOOL			ArenaFieldServerCheck;		// ¾Æ·¹³ª ÇÊµå ¼­¹öÀÎÁö Ã¼Å©, TRUE=>¾Æ·¹³ªÇÊµå¼­¹ö	
	char			ArenaServerName[SIZE_MAX_SERVER_NAME];
	INT				ArenaServerID;
	char			ArenaFieldServerIP[SIZE_MAX_IPADDRESS];
	int				ArenaFieldServerPort;
	char			ArenaIMServerIP[SIZE_MAX_IPADDRESS];
	int				ArenaIMServerPort;	
};

struct SAFS_NEED_MFSINFO
{// 2008-01-03 by dhjin, ¾Æ·¹³ª ÅëÇÕ - MFS¿Í AFS ÄÉ¸¯ÅÍ Á¤º¸ ¸ÅÄª ÀúÀå
	ClientIndex_t	MFSClientIdx;
	UID32_t			MFSCharacterUID;
	INT				MFSServerID;
};

#define	JAMBOREE_SERVER_ID			10080			// 2008-03-05 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ´ëÈ¸ ¼­¹ö ID

//////////////////////////////////////////////////////////////////////////
// 2008-03-24 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - 
#define MS_WAR_HP_RATE					10				// 2008-03-26 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - ¸ð¼±Àü HP º¸¿©ÁÖ´Â ºñÀ²
#define MS_WAR_MIN_HP_RATE				0				// 2008-03-26 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - ¸ð¼±Àü HP º¸¿©ÁÖ´Â ÃÖÀúHP ºñÀ²

INT CalcGradeByRate(float i_nMaxValue, float i_nValue, INT i_nRate); // 2008-03-24 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - ºñÀ²¿¡ µû¸¥ µî±ÞÀ» °è»êÇÑ´Ù.

// 2008-03-24 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - ºñÆ® ÇÃ·¡±× ¼³Á¤
#define MS_WAR_OPTION_TYPE_UNKNOWN					(USHORT)0x0000	// ¾Ë¼ö ¾øÀ½
#define MS_WAR_OPTION_TYPE_NONE						(USHORT)0x0001	// 1 - ¸ð¼±Àü Á¤º¸ Ç¥½Ã ¼³Á¤ ¾ÈµÊ
#define MS_WAR_OPTION_TYPE_ATT_ACTIVATED			(USHORT)0x0002	// 2 - (°ø) È°¼ºÈ­ ¿Ï·á Ç¥½Ã
#define MS_WAR_OPTION_TYPE_ATT_ACTIVE				(USHORT)0x0004	// 4 - (°ø) È°¼ºÈ­ ÁøÇà Áß Ç¥½Ã
#define MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME			(USHORT)0x0008	// 8 - (°ø) È°¼ºÈ­ ÁøÇà ½Ã°£ Ç¥½Ã
#define MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP			(USHORT)0x0010	// 16 - (°ø) ÅÚ·¹Æ÷Æ® HP Ç¥½Ã
#define MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE	(USHORT)0x0020	// 32 - (°ø) Àü·«Æ÷ÀÎÆ® »ýÁ¸¿©ºÎ Ç¥½Ã
#define MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP		(USHORT)0x0040	// 64 - (¼ö) Àü·«Æ÷ÀÎÆ® HP Ç¥½Ã
#define MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE	(USHORT)0x0080	// 128 - (¼ö) Àü·«Æ÷ÀÎÆ® »ýÁ¸¿©ºÎ Ç¥½Ã
#define MS_WAR_OPTION_TYPE_LEADER					(USHORT)0x0100	// 256 - ÁöµµºÎ Àû¿ë
		
#define IS_MS_WAR_OPTION_TYPE(_CurrentOption, _CompareOption)   ((_CurrentOption & _CompareOption) != 0)

struct MSWARINFO_DISPLAY
{// 2008-03-26 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - À¯Àú¿¡°Ô ÇÊ¿äÇÑ Á¤º¸ Ç¥½Ã µ¥ÀÌÅ¸
	MapIndex_t		MapIndex;
	INT				MapInfluenceType;
	BYTE			Belligerence;
	INT				HPRate;
	INT				TelePortState;
	ATUM_DATE_TIME  TelePortBuildingStartTime;
	ATUM_DATE_TIME  TelePortBuildingEndTime;
};
typedef vector<MSWARINFO_DISPLAY>		vectMSWARINFO_DISPLAY;

enum EN_MSWARINFO_MODIFY_TYPE
{// 2008-03-27 by dhjin, ¸ð¼±Àü Á¤º¸ Ç¥½Ã ±âÈ¹¾È - ¸ð¼± Á¤º¸ Ç¥½Ã ¼öÁ¤µÇ´Â Å¸ÀÔ
	T_MSWARINFO_MODIFY_UNKNOWN					= 0,		// ¾Ë¼ö ¾øÀ½
	T_MSWARINFO_MODIFY_HPRATE					= 1,		// HPµî±Þ º¯°æ
	T_MSWARINFO_MODIFY_CREATE_TELEPORT			= 2,		// Teleport »ý¼º
	T_MSWARINFO_MODIFY_CHANGE_TELEPORT_STATE	= 3			// Teleport »óÅÂ°ª º¯°æ
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, ¸ð¼±Àü, °ÅÁ¡Àü Á¤º¸Ã¢ ±âÈ¹¾È - 
struct SMSWAR_INFO_RESULT
{// 2008-04-01 by dhjin, ¸ðÀüÀü ÀüÀåÁ¤º¸
	BYTE			AttInfluence;
	INT				MonsterUID;
	INT				ContributionPoint;
	ATUM_DATE_TIME	MSWarStartTime;
	ATUM_DATE_TIME	MSWarEndTime;
	BYTE			WinInfluence;
};
typedef vector<SMSWAR_INFO_RESULT>		vectSMSWAR_INFO_RESULT;

struct SSPWAR_INFO_RESULT
{// 2008-04-01 by dhjin, °ÅÁ¡Àü ÀüÀåÁ¤º¸
	BYTE			AttInfluence;
	INT				SPSummonMapIndex;
	ATUM_DATE_TIME	SPWarStartTime;
	ATUM_DATE_TIME	SPWarEndTime;
	BYTE			WinInfluence;
};
typedef vector<SSPWAR_INFO_RESULT>		vectSSPWAR_INFO_RESULT;
typedef SSPWAR_INFO_RESULT				MSG_FC_INFO_SPWARINFO_RESULT_OK;

///////////////////////////////////////////////////////////////////////////////
// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 
typedef UINT32 ExceptMon_t;
#define EXCEPTMON_OBJECT_MONSTER_MASK		(ExceptMon_t)0x00000001			// ¿ÀºêÁ§Æ® ¸ó½ºÅÍ MonsterForm °ªÀÌ 8,9,10
#define EXCEPTMON_INFLUENCE_TYPE_MASK		(ExceptMon_t)0x00000002			// ¼¼·ÂÀü ¸ó½ºÅÍ Belligerence
#define EXCEPTMON_BELL_NOTATTACK_MASK		(ExceptMon_t)0x00000004			// °ø°Ý ¼ºÇâÀÌ ¾ø´Â ¸ó½ºÅÍ Belligerence °ªÀÌ 4ÀÎ ¸ó½ºÅÍ

struct SEVENT_MONSTER
{
	INT					EventMonsterUID;			
	INT					ServerGroupID;				// ServerGroupID ( 0ÀÌ¸é ¸ðµç ¼­¹ö±º¿¡ Àû¿ë, 0ÀÌ ¾Æ´Ï¸é ÇØ´ç ¼­¹ö±º¸¸ Àû¿ë)
	ATUM_DATE_TIME		StartDateTime;				// EventMonster ½ÃÀÛ ³¯Â¥½Ã°£
	ATUM_DATE_TIME		EndDateTime;				// EventMonster Á¾·á ³¯Â¥½Ã°£
	MapIndex_t			SummonerMapIndex;			// ¼ÒÈ¯µÇ´Â ¸ÊÀÎµ¦½º(0ÀÌ¸é ¸ðµç¸Ê¿¡¼­ ¼ÒÈ¯µÊ, 0ÀÌ ¾Æ´Ï¸é ÁöÁ¤ÇÑ ¸Ê¿¡¼­¸¸ ¼ÒÈ¯µÊ)
	BYTE				SummonerReqMinLevel;		// ¼ÒÈ¯ÇÏ´Â ¸ó½ºÅÍÀÇ ÃÖ¼Ò ·¹º§ Ã¼Å©(0ÀÌ¸é Ã¼Å©ÇÏÁö ¾ÊÀ½)
	BYTE				SummonerReqMaxLevel;		// ¼ÒÈ¯ÇÏ´Â ¸ó½ºÅÍÀÇ ÃÖ°í ·¹º§ Ã¼Å©(0ÀÌ¸é Á¦Å©ÇÏÁö ¾ÊÀ½)
	ExceptMon_t			SummonerExceptMonster;		// ¼ÒÈ¯ÇÏ´Â ¸ó½ºÅÍ Á¦¿Ü Ã¼Å©  Bit Flag
	INT					SummonMonsterNum;			// ¼ÒÈ¯µÇ´Â MonsterUniqueNumber
	INT					SummonMonsterCount;			// ¼ÒÈ¯½Ã ¼ÒÈ¯µÇ´Â ¸ó½ºÅÍ ¼ýÀÚ
	INT					SummonDelayTime;			// ¸ó½ºÅÍ »ç¸Á ÈÄ ¼ÒÈ¯±îÁö °É¸®´Â Áö¿¬½Ã°£(´ÜÀ§:ÃÊ)
	Prob10K_t			SummonProbability;			// ¼ÒÈ¯ ¼º°ø È®·ü(0~10000)
}; 
typedef mt_vector<SEVENT_MONSTER>				mtvectSEVENT_MONSTER;

struct SSUMMON_EVENT_MONSTER
{
	BOOL				IsSummonChecked;			// ¼ÒÈ¯ ¿©ºÎ Ã¼Å© ÇÃ·¡±×
	INT					SummonMonsterNum;			// ¼ÒÈ¯µÇ´Â MonsterUniqueNumber
	INT					SummonMonsterCount;			// ¼ÒÈ¯½Ã ¼ÒÈ¯µÇ´Â ¸ó½ºÅÍ ¼ýÀÚ
	INT					SummonDelayTime;			// ¸ó½ºÅÍ »ç¸Á ÈÄ ¼ÒÈ¯±îÁö °É¸®´Â Áö¿¬½Ã°£(´ÜÀ§:ÃÊ)
	Prob10K_t			SummonProbability;			// ¼ÒÈ¯ ¼º°ø È®·ü(0~10000)	
	
	inline SSUMMON_EVENT_MONSTER& operator=(const SEVENT_MONSTER& rhs)
	{
		this->SummonMonsterNum		= rhs.SummonMonsterNum;
		this->SummonMonsterCount	= rhs.SummonMonsterCount;
		this->SummonDelayTime		= rhs.SummonDelayTime;
		this->SummonProbability		= rhs.SummonProbability;
		return *this;
	}
};
typedef mt_vector<SSUMMON_EVENT_MONSTER>		mtvectSSUMMON_EVENT_MONSTER;


///////////////////////////////////////////////////////////////////////////////
// 2008-04-29 by cmkwon, ¼­¹ö±º Á¤º¸ DB¿¡ Ãß°¡(½Å±Ô °èÁ¤ Ä³¸¯ÅÍ »ý¼º Á¦ÇÑ ½Ã½ºÅÛÃß°¡) - 
struct SDBSERVER_GROUP
{
	int				ServerGroupID;
	char			ServerGroupName[SIZE_MAX_SERVER_NAME];
	int				LimitUserCount;
	BOOL			LockCreateCharacterForNewAccount;
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-15 by dhjin, EP3 ÆíÁö ½Ã½ºÅÛ - 
#define	LETTER_PASSED_RANGE_DAY		14			// 2008-04-29 by dhjin, EP3 ÆíÁö ½Ã½ºÅÛ - ÆíÁö »èÁ¦ ±â°£, ´ÜÀ§ ³¯
#define	SIZE_MAX_LETTER_TITLE		40		// 2008-04-15 by dhjin, EP3 ÆíÁö ½Ã½ºÅÛ - ÆíÁö Á¦¸ñ
#define	SIZE_MAX_LETTER_CONTENT		1000		// 2008-04-15 by dhjin, EP3 ÆíÁö ½Ã½ºÅÛ - ÆíÁö ³»¿ë
struct SLETTER_INFO
{
	UID64_t			LetterUID;
	UID32_t			RecvCharacterUID;
	CHAR			SendCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME  SendDate;
	BOOL			CheckRead;
	CHAR			Title[SIZE_MAX_LETTER_TITLE];
	CHAR			Content[SIZE_MAX_LETTER_CONTENT];
};
typedef vector<SLETTER_INFO>			vectSLETTER_INFO;

// 2008-06-03 by cmkwon, AdminTool, DBTool »ç¿ë½Ã ¾ÆÀÌÅÛ °Ë»ö½Ã ÄÞº¸¹Ú½º¿¡¼­ °Ë»ö ±â´É Ãß°¡(K0000143) - 
char *StringCullingForValidChar(char *i_szSource, int i_nCullingSize);

//////////////////////////////////////////////////////////////////////////
// 2008-05-27 by dhjin, EP3 - ¿©´Ü ¼öÁ¤ »çÇ× - ¿©´Ü Áö¿øÀÚ °ü¸®
#define	GUILDINTRODUCTION_PASSED_RANGE_DAY		5			// 2008-05-27 by dhjin, EP3 - ¿©´Ü ¼öÁ¤ »çÇ× - ¿©´Ü Áö¿øÀÚ °ü¸®
struct SGUILD_INTRODUCTION
{
	char			IntroductionContent[SIZE_MAX_NOTICE];
	ATUM_DATE_TIME	RegDate;
};

struct SGUILD_APPLICANT_INFO
{
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT		UnitKind;
	BYTE		Level;				
};
typedef vector<SGUILD_APPLICANT_INFO>	vectSGUILD_APPLICANT_INFO;

struct SGUILD_SEARCH_INTRODUCTION
{// 2008-05-27 by dhjin, EP3 - ¿©´Ü ¼öÁ¤ »çÇ× - ¿©´Ü ¼Ò°³ °Ë»ö
	UID32_t			GuildUID;
	char			GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	WriteDate;
	char			GuildIntroduction[SIZE_MAX_NOTICE];
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};
typedef vector<SGUILD_SEARCH_INTRODUCTION>	vectSGUILD_SEARCH_INTRODUCTION;

//////////////////////////////////////////////////////////////////////////
// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - 
#define ITEM_DISTRIBUTE_TYPE_DAMAGE			0	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - °³ÀÎ ½Àµæ	
#define ITEM_DISTRIBUTE_TYPE_ORDER			1	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - ¼øÂ÷ ½Àµæ 
#define ITEM_DISTRIBUTE_TYPE_RANDOM			2	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - ·£´ý ½Àµæ

#define PARTY_JOIN_TYPE_INVITE_MASTER		0	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - Æí´ëÀå ÃÊ´ë Âü¿© ¹æ½Ä 
#define PARTY_JOIN_TYPE_FREE				1	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - ÀÚÀ¯ Âü¿© ¹æ½Ä

#define SIZE_MAX_PARTY_NAME					32	// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - ÆÄÆ¼ ÀÌ¸§ ÃÖ´ë ±æÀÌ

#define SIZE_MAX_RECOMMENDATION				5	// 2008-06-04 by dhjin, EP3 Æí´ë ¼öÁ¤ -  ÃßÃµ ÄÉ¸¯ÅÍ Àü¼Û ÃÖ´ë ¼ö

struct SPARTY_INFO
{// 2008-06-02 by dhjin, EP3 Æí´ë ¼öÁ¤ - Æí´ë Á¤º¸
	BYTE		PartyInfluence;
	char		PartyName[SIZE_MAX_PARTY_NAME];
	BYTE		PartyJoinType;
	BYTE		ExpDistributeType;
	BYTE		ItemDistributeType;
	BOOL		PartyLock;
	CHAR		PartyPW[SIZE_MAX_TEAM_PW];
	INT			MinLevel;
	INT			MaxLevel;
	BYTE		FormationType;
	BYTE		IsAppliedFormationSkill;	// 2009-08-03 by cmkwon, EP3-4 Æí´ë ´ëÇü ½ºÅ³ ±¸Çö - 
};

//////////////////////////////////////////////////////////////////////////
// 2008-06-09 by dhjin, EP3 Ã¤ÆÃ¹æ -
#define SIZE_MAX_CHATROOM_NAME				34		// 2008-06-09 by dhjin, EP3 Ã¤ÆÃ¹æ - Ã¤ÆÃ¹æ ÀÌ¸§ ÃÖ´ë ¼ö	
#define SIZE_MAX_CHATROOM_COUNT				1000	// 2008-06-16 by dhjin, EP3 Ã¤ÆÃ¹æ - Ã¤ÆÃ¹æ ÃÖ´ë ¼ö

//////////////////////////////////////////////////////////////////////////
// 2008-06-20 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç -
#define USER_INFO_OPTION_SECRET_LEVEL				(USHORT)0x0001			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - °è±Þ (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_POSITION			(USHORT)0x0002			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - À§Ä¡ (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_PROPENSITY			(USHORT)0x0004			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ¸í¼º (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_GUILD				(USHORT)0x0008			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ¼Ò¼Ó (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_LASTLOGIN			(USHORT)0x0010			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ÃÖÁ¾ Á¢¼ÓÀÏ (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_ITEMINFO			(USHORT)0x0080			// 2012-07-12 by isshin ¾ÆÅÛ¹Ì¸®º¸±â On / Off ±â´É	// EP4 À¯ÀúÁ¤º¸¿É¼Ç - ¾ÆÀÌÅÛ Á¤º¸ (ÀÏ¹Ý)
#define USER_INFO_OPTION_SECRET_GUILD_LEVEL			(USHORT)0x0100			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - °è±Þ (¿©´Ü)
#define USER_INFO_OPTION_SECRET_GUILD_POSITION		(USHORT)0x0200			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - À§Ä¡ (¿©´Ü)
#define USER_INFO_OPTION_SECRET_GUILD_PROPENSITY	(USHORT)0x0400			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ¸í¼º (¿©´Ü)
#define USER_INFO_OPTION_SECRET_GUILD_GUILD			(USHORT)0x0800			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ¼Ò¼Ó (¿©´Ü)
#define USER_INFO_OPTION_SECRET_GUILD_LASTLOGIN		(USHORT)0x1000			// 2008-06-23 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç - ÃÖÁ¾ Á¢¼ÓÀÏ (¿©´Ü)
#define USER_INFO_OPTION_SECRET_GUILD_ITEMINFO		(USHORT)0x8000			// 2012-07-12 by isshin ¾ÆÅÛ¹Ì¸®º¸±â On / Off ±â´É	// EP4 À¯ÀúÁ¤º¸¿É¼Ç - ¾ÆÀÌÅÛ Á¤º¸ (¿©´Ü)
#define USER_INFO_OPTION_MISSIONMASTER				(USHORT)0x2000			// 2008-12-08 by dhjin, ¹Ì¼Ç¸¶½ºÅÍ - À¯Àú°¡ ¹Ì¼Ç ¸¶½ºÅÍ ¼³Á¤À» Çß´ÂÁö¿¡ ´ëÇÑ ³»¿ë
#define USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK	(USHORT)0x7F7F		// 2012-10-17 by jhseol, ¾ÆÅÛ ¹Ì¸®º¸±â ¹«Á¶°Ç ON ½ÃÅ°±â - ¾ÆÅÛ ¹Ì¸®º¸±â¸¸ Áö¿î ¿Ã ¸¶½ºÅ©

struct SUSER_INFO_OPTION
{// 2008-06-20 by dhjin, EP3 À¯ÀúÁ¤º¸¿É¼Ç -
	BYTE				PilotFace;
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				UnitKind;
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];
	BYTE				Level;
	MAP_CHANNEL_INDEX	MapChannelIndex;
	INT					Propensity;
	ATUM_DATE_TIME		LastStartedTime;
	INT					SecretInfoOption;
	char				NickName[SIZE_MAX_CHARACTER_NAME];			// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
};

//////////////////////////////////////////////////////////////////////////
// 2008-08-18 by dhjin, 1ÃÊ ÅÒ ½Ã½ºÅÛ ±âÈ¹¾È
#define INTERVAL_SYSTEM_MS			200   

//////////////////////////////////////////////////////////////////////////
// 2008-08-25 by dhjin, ÅÂ±¹ PC¹æ IPÁ¤º¸ ·Îµù
#define MinPCBangMinTick			10
#define MaxPCBangMinTick			1440


typedef vector<MapIndex_t>			vectMapIndexList;		// 2008-09-10 by cmkwon, ÅëÇÕ¾Æ·¹³ª ¸Ê¼³Á¤(map.cfg) °ü·Ã ¼öÁ¤ - 

//////////////////////////////////////////////////////////////////////////
// 2008-10-13 by dhjin, ¿©´ÜÀå À§ÀÓ °¡´É ·¹º§ Ã¼Å© Ãß°¡.
#define GuildCommanderMinLevel		20

//////////////////////////////////////////////////////////////////////////
// 2008-11-10 by dhjin, ·°Å°¸Ó½Å
struct SLUCKY_MACHINE
{
	INT			BuildingIndex;
	BYTE		MachineKind;		// 2009-03-03 by dhjin, ·°Å°¸Ó½Å ¼öÁ¤¾È
	INT			MachineNum;			// 2009-03-03 by dhjin, ·°Å°¸Ó½Å ¼öÁ¤¾È
	INT			CoinItemNum;
	BYTE		SlotNum;			// 2009-03-03 by dhjin, ·°Å°¸Ó½Å ¼öÁ¤¾È
	INT			SlotProbability;
	INT			MysteryItemDropNum;
};
typedef vector<SLUCKY_MACHINE>		vectSLUCKY_MACHINE;
typedef vector<SLUCKY_MACHINE*>		vectSLUCKY_MACHINEPtr;	// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ±âÁ¸ ¼Ò½º ¼öÁ¤ - 

#define MaxPayLuckyMachineItem		3			// ÃÖ´ë ½½·Ô Ä«¿îÆ®(MAX_LUCKY_MACHINE_SLOT)¿Í °°Àº ÀÇ¹Ì·Î »ç¿ë

//////////////////////////////////////////////////////////////////////////
// 2008-12-08 by dhjin, ¹Ì¼Ç¸¶½ºÅÍ
#define SIZE_MAX_MISSIONMASTER_HELP_COUNT	5

//////////////////////////////////////////////////////////////////////////
// 2008-12-23 by dhjin, ÀüÀï º¸»ó Ãß°¡¾È
#define	BGEAR_WAR_CONTRIBUTION		0.7f
#define	IGEAR_WAR_CONTRIBUTION		1
#define	MGEAR_WAR_CONTRIBUTION		3
#define	AGEAR_WAR_CONTRIBUTION		1	
#define PAY_WAR_CONTRIBUTION_BY_BOSS				2000000
#define PAY_WAR_CONTRIBUTION_BY_OUTPOST				1000000
#define PAY_WAR_CONTRIBUTION_BY_STRATEGYPOINT		500000
#define PAY_WAR_CONTRIBUTION_LEADER_BY_BOSS			200
#define PAY_WAR_CONTRIBUTION_1_GUILD_BY_BOSS		100
#define PAY_WAR_CONTRIBUTION_2_GUILD_BY_BOSS		80
#define PAY_WAR_CONTRIBUTION_3_GUILD_BY_BOSS		50
#define PAY_WAR_CONTRIBUTION_1_GUILD_BY_BOSS_PLUS	30000000
#define PAY_WAR_CONTRIBUTION_2_GUILD_BY_BOSS_PLUS	20000000
#define PAY_WAR_CONTRIBUTION_3_GUILD_BY_BOSS_PLUS	10000000
#define PAY_TYPE_BOSS				(BYTE)0
#define PAY_TYPE_OUTPOST			(BYTE)1
#define PAY_TYPE_STRATEGYPOINT		(BYTE)2
#define PAY_TYPE_BOSS_LEADER		(BYTE)10
#define PAY_TYPE_BOSS_1_GUILD		(BYTE)11
#define PAY_TYPE_BOSS_2_GUILD		(BYTE)12
#define PAY_TYPE_BOSS_3_GUILD		(BYTE)13

// 2009-07-08 by cmkwon, ÀüÀï °ü·Ã Á¤ÀÇ À§Ä¡ ÀÌµ¿(LocalizationDefineCommon.h) - 
//#define PAY_MINIMUN_COUNT			10

#define PAY_MINIMUN_BY_BOSS				100000
#define PAY_MINIMUN_BY_OUTPOST			100000	
#define PAY_MINIMUN_BY_STRATEGYPOINT	50000
#define PAY_MAXMUN_BY_BOSS				10000000	// 2009-03-18 by cmkwon, ÀüÀï º¸»ó Ãß°¡¾È ¼öÁ¤(°³ÀÎ º¸»ó ÃÖ´ë°ª ¼³Á¤) - ¸ð¼±Àü Ãµ¸¸
#define PAY_MAXMUN_BY_OUTPOST			10000000	// 2009-03-18 by cmkwon, ÀüÀï º¸»ó Ãß°¡¾È ¼öÁ¤(°³ÀÎ º¸»ó ÃÖ´ë°ª ¼³Á¤) - ÀüÁø±âÁöÀü Ãµ¸¸
#define PAY_MAXMUN_BY_STRATEGYPOINT		5000000		// 2009-03-18 by cmkwon, ÀüÀï º¸»ó Ãß°¡¾È ¼öÁ¤(°³ÀÎ º¸»ó ÃÖ´ë°ª ¼³Á¤) - °ÅÁ¡Àü 5¹é¸¸

#define NOT_OTHER_INFL_STAY			0
#define OTHER_INFL_STAY				1
#define NONE_DAMAGE_CONTRIBUTION	0
#define DAMAGE_CONTRIBUTION			1

struct SWAR_CONTRIBUTION_INFO
{// À¯Àú°¡ »ó´ë¼¼·Â¸Ê¿¡ ¸Ó¹® ½Ã°£°ú Á×Àº ¼ö¸¦ °ü¸®ÇÑ´Ù.
	ATUM_DATE_TIME	OtherInflStayStartTime;
	INT				OtherInflStayTime;
	BOOL			CheckOtherInflStay;		// NOT_OTHER_INFL_STAY : »ó´ë¼¼·Â¸ÊÀÌ ¾Æ´Ô, OTHER_INFL_STAY : »ó´ë¼¼·Â¸Ê, 0->1 : »ó´ë¼¼·Â¸ÊÀ¸·Î ÀÌµ¿, 1->0 : »ó´ë¼¼·ÂÀÌ ¾Æ´Ñ ¸ÊÀ¸·Î ÀÌµ¿
	INT				DeathCount;
	UID64_t			Contribution;
	BOOL			CheckDamageContribution; // NONE_DAMAGE_CONTRIBUTION : ¾ÆÁ÷ µ¥¹ÌÁöÁØ ±â¿©µµ ¾øÀ½, DAMAGE_CONTRIBUTION : µ¥¹ÌÁö ±â¿©µµ°¡ ÀÖÀ½
};

///////////////////////////////////////////////////////////////////////////////
// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - 
#define SIZE_MAX_ADMIN_NOTICE_STRING			256		// ÃÖ´ë °øÁö »çÇ× ±æÀÌ 
#define SIZE_MAX_COUNT_ADMIN_NOTICE_STRING		20		// °øÁö »çÇ× ÃÖ´ë °³¼ö
#define MIN_LOOP_SECONDS_OF_ADMIN_NOTICE		60		// ÀÚµ¿ °øÁö ½Ã½ºÅÛÀÌ ´Ù½Ã ½ÃÀÛÇÏ´Â ÃÖ¼Ò Interval time(second), 60ÃÊ = 1ºÐ
#define MAX_LOOP_SECONDS_OF_ADMIN_NOTICE		604800	// ÀÚµ¿ °øÁö ½Ã½ºÅÛÀÌ ´Ù½Ã ½ÃÀÛÇÏ´Â ÃÖ´ë Interval time(second), 604800ÃÊ = 10080ºÐ = 168½Ã°£ = 7ÀÏ
#define MIN_INTERVAL_SECONDS_OF_ADMIN_NOTICE	5		// °øÁö ½ºÆ®¸µÀ» Àü¼ÛÇÏ°í ´ÙÀ½ °øÁö ½ºÆ®¸µ Àü¼ÛÇÏ±â ±îÁöÀÇ ÃÖ¼Ò Interval time(second), 5ÃÊ
#define MAX_INTERVAL_SECONDS_OF_ADMIN_NOTICE	86400	// °øÁö ½ºÆ®¸µÀ» Àü¼ÛÇÏ°í ´ÙÀ½ °øÁö ½ºÆ®¸µ Àü¼ÛÇÏ±â ±îÁöÀÇ ÃÖ¼Ò Interval time(second), 86400ÃÊ = 1440ºÐ = 24½Ã°£ = 1ÀÏ
#define DELAY_SEC_AFTER_SERVER_STARTED			300		// ¼­¹ö ½ÃÀÛ ÈÄ ÀÚµ¿ °øÁö ½Ã½ºÅÛÀÌ ´Ù½Ã ½ÃÀÛÇÏ´Â Áö¿¬ ½Ã°£ 300ÃÊ = 5ºÐ

struct SANoticeInfo
{
	BOOL			UsingFlag;
	INT				LoopSec;					// ´ÜÀ§:ÃÊ
	INT				IntervalSec;				// ´ÜÀ§:ÃÊ
	char			EditorAccountName[SIZE_MAX_ACCOUNT_NAME];
	ATUM_DATE_TIME	LastStartedTime;
	INT				LastSentNoticeStringIndex;
};
struct SANoticeString		// AdminNoticeString
{
	INT				NoticeStringIndex;
	char			NoticeString[SIZE_MAX_ADMIN_NOTICE_STRING];
};
typedef vector<SANoticeString>			vectSANoticeString;
typedef mt_vector<SANoticeString>		mtvectSANoticeString;

struct sort_SANoticeString_by_StrIdx
{
	bool operator()(SANoticeString op1, SANoticeString op2)
	{
		return op1.NoticeStringIndex < op2.NoticeStringIndex;		// ¿À¸²Â÷¼ø Á¤·Ä
	}
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, ¼±Àü Æ÷°í
#define MSWAR_NOT_START		0
#define MSWARING_BEFORE		1
#define MSWARING			2
#define MSWAR_END_WIN		11
#define MSWAR_END_LOSS		21
#define MSWAR_FORBID_GAP_HOUR 12
#define MSWAR_NEXT_LEADER_SETP	99
#define MSWAR_FINAL_SETP	5

struct SDECLARATION_OF_WAR
{
	BYTE			Influence;
	BYTE			MSWarStep;
	INT				NCP;
	INT				MSNum;
	MapIndex_t		MSAppearanceMap;
	ATUM_DATE_TIME	MSWarStepStartTime;
	ATUM_DATE_TIME	MSWarStepEndTime;
	ATUM_DATE_TIME	MSWarStartTime;
	ATUM_DATE_TIME	MSWarEndTime;
	BYTE			SelectCount;
	BOOL			GiveUp;
	BYTE			MSWarEndState;		// MSWAR_NOT_START : ÀüÀï ½ÃÀÛÇÏÁö ¾ÊÀ½, MSWARING : ÀüÀïÁß, MSWAR_END_WIN : ½Â¸®, MSWAR_END_LOSS : ÆÐ¹è
};

struct SDECLARATION_OF_WAR_FORBID_TIME
{
	INT				DayOfWeek;
	ATUM_DATE_TIME	ForbidStartTime;
	ATUM_DATE_TIME	ForbidEndTime;
};


///////////////////////////////////////////////////////////////////////////////
// 2009-01-22 by cmkwon, ÀüÀï °ü·Ã °ÔÀÓ ·Î±× ¼öÁ¤ - WPUT_XXX, WarPointUpdateType
#define WPUT_UNKNOWN				0	
#define WPUT_GENERAL				1	
#define WPUT_ARENA					2	
#define WPUT_MOTHERSHIPWAR			3	
#define WPUT_STRATEGYPOINTWAR		4	
#define WPUT_OUTPOSTWAR				5	
#define WPUT_SHOP					6	
#define WPUT_ADMIN_COMMAND			7	
#define WPUT_TRIGGER				8		// 2011-11-07 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - NCG °ÅÁ¡Àü ½Ã½ºÅÛ
#define WPUT_KILL					9		// 2015-07-22 by Future, Hideable WP Reward message
extern char *GetWPUTString(BYTE i_byWPUT);


///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
#define SIZE_MAX_SERVICE_NAME			20						// ¿ùµå ·©Å°¿¡¼­ ¼­ºñ½º ±ºÀ» À§ÇÑ ¼­ºñ½º¸í ÃÖ´ë ±ÛÀÚ¼ö, ÃÖ´ë ¿µ¹® 19±ÛÀÚ
#define SIZE_MAX_NAME_FOR_RANKING		SIZE_MAX_CHARACTER_NAME	// 
#define SIZE_MAX_SERVICE_SYMBOL_IMAGE	2000					// 48x28 jpg ÆÄÀÏ,
#define SIZE_MAX_WRK_COUNT				100						// ¿ùµå·©Å· ÇÊ¼ö ·©Å·
#define SIZE_MAX_REQUEST_COUNT			20						// ÇÑ¹ø¿¡ ¿äÃ» °¡´ÉÇÑ ÃÖ´ë ·©Å·¸®½ºÆ®

#define RANKING_SCOPE_LOCAL				0		// ÇöÀç ¼­ºñ½º ·ÎÄÃ ·©Å·, Name ÀÌ NickName
#define RANKING_SCOPE_WORLD				1		// ¿ùµå ·©Å· Á¤º¸, Name ÀÌ CharacterName
#define IS_VALID_RANKING_SCOPE(s)		((s) == 0 || (s) == 1)

#define RANKING_TYPE_LEVEL				0		// Level ·©Å·
#define RANKING_TYPE_FAME				1		// °³ÀÎ ¸í¼º ·©Å·
#define RANKING_TYPE_PVP				2		// 1:1 PvP ·©Å·

struct SWRK_SERVICE_INFO
{
	INT			ServiceUID;
	char		ServiceName[SIZE_MAX_SERVICE_NAME];
	BYTE		ServiceSymbolIamge[SIZE_MAX_SERVICE_SYMBOL_IMAGE];
	INT			SymbolImageSize;
};
struct SWRK_SERVER_GROUP
{
	INT			ServerGroupID;
	char		ServerGroupNameforRK[SIZE_MAX_SERVER_NAME];
};
struct SWRK_BASE
{
	INT				ServiceUID;
	INT				UnitKind;
	char			ServerGroupNameforRK[SIZE_MAX_SERVER_NAME];
	char			NameforRK[SIZE_MAX_NAME_FOR_RANKING];
	BYTE			InfluenceType;
	BYTE			Level;
	BYTE			RankingScope;		// RANKING_SCOPE_XXX
};
typedef SWRK_BASE	SWRK_LEVEL;
struct SWRK_LEVEL_FOR_DB: public SWRK_LEVEL
{
	INT				ServerGroupID;
	UID32_t			CharacterUID;
	Experience_t	Experience;	
	ATUM_DATE_TIME	LevelUpTime;	
};

struct SWRK_FAME: public SWRK_BASE
{
	INT				Fame;
};
struct SWRK_FAME_FOR_DB: public SWRK_FAME
{
	INT				ServerGroupID;
	UID32_t			CharacterUID;
};

struct SWRK_PVP: public SWRK_BASE
{
	INT			WinPoint;
	INT			LossPoint;
	INT			Score;
};
struct SWRK_PVP_FOR_DB: public SWRK_PVP
{
	INT				ServerGroupID;
	UID32_t			CharacterUID;
};
typedef vector<SWRK_SERVER_GROUP>		vectSWRK_SERVER_GROUP;

typedef mt_vector<SWRK_SERVICE_INFO>	mtvectSWRK_SERVICE_INFO;

typedef mt_vector<SWRK_LEVEL>			mtvectSWRK_LEVEL;
typedef mt_vector<SWRK_LEVEL*>			mtvectSWRK_LEVELPtr;
typedef mt_vector<SWRK_LEVEL_FOR_DB>	mtvectSWRK_LEVEL_FOR_DB;

typedef mt_vector<SWRK_FAME>			mtvectSWRK_FAME;
typedef mt_vector<SWRK_FAME*>			mtvectSWRK_FAMEPtr;
typedef mt_vector<SWRK_FAME_FOR_DB>		mtvectSWRK_FAME_FOR_DB;

typedef mt_vector<SWRK_PVP>				mtvectSWRK_PVP;
typedef mt_vector<SWRK_PVP*>			mtvectSWRK_PVPPtr;
typedef mt_vector<SWRK_PVP_FOR_DB>		mtvectSWRK_PVP_FOR_DB;

extern int GetArrIdxByUnitMaskforWRK(INT i_unitMask);
extern int GetUnitMaskByArrIdxforWRK(INT i_arrIdx);


//////////////////////////////////////////////////////////////////////////
// 2009-03-03 by dhjin, ·°Å°¸Ó½Å ¼öÁ¤¾È
#define SIZE_MAX_LUCKYMACHINE_DESCRIPTION		200
struct LUCKY_MACHINE_OMI
{
	INT			order;
	INT			BuildingInx;
	BYTE		MachineKind;
	INT			MachineNum;
	INT			CoinItemNum;
	BYTE		SlotNum;
	INT			SourceIndex;
	char		szDescription[SIZE_MAX_LUCKYMACHINE_DESCRIPTION];
};


///////////////////////////////////////////////////////////////////////////////
// 2009-03-09 by cmkwon, ÀÏº» Arario nProtect¿¡ CSÀÎÁõ Àû¿ëÇÏ±â - 
#define MAX_PACKET_SIZE_FOR_NPROTECT	128			// 2009-03-09 by cmkwon, ÀÏº» Arario nProtect¿¡ CSÀÎÁõ Àû¿ëÇÏ±â - ÃÖ´ë ÇÁ·ÎÅäÄÝ Å©±â

//////////////////////////////////////////////////////////////////////////
// 2009-03-10 by dhjin, ´Ü°èº° ¸ð¼± ½Ã½ºÅÛ
#define MSBOSSSTEP_UNKNOWN_BEFOREWAR	0			// Àü ÁÖ±â¿¡ ¸ð¼±Àü Á¤º¸°¡ ¾ø°Å³ª Ã¹ ÁÖ±â ¸ð¼±
#define MSBOSSSTEP_WIN_BEFOREWAR		1			// Àü ÁÖ±â¿¡ ¸ð¼±Àü ½Â¸®
#define MSBOSSSTEP_LOSS_BEFOREWAR		2			// Àü ÁÖ±â¿¡ ¸ð¼±Àü ÆÐ¹è



///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, ¼¼·ÂÃÊ±âÈ­ ½Ã½ºÅÛ ±¸Çö - 
#define QUEST_INDEX_OF_SELECT_INFLUENCE				112		// ¼¼·Â ¼±ÅÃ ¹Ì¼Ç QuestIndex, ¸ðµç ¼­ºñ½º µ¿ÀÏÇÔ

struct SQUEST_MATCHING
{
	INT		QuestIndexForBCU;
	INT		QuestIndexForANI;
};
typedef vector<SQUEST_MATCHING>			vectSQUEST_MATCHING;

struct SQUEST_MATCH_CHANGE
{
	INT		FromQuestIndex;
	INT		ToQuestIndex;
};
typedef vector<SQUEST_MATCH_CHANGE>		vectSQUEST_MATCH_CHANGE;

struct SITEM_MATCHING
{
	INT		ItemNumForBCU;
	INT		ItemNumForANI;
};
typedef vector<SITEM_MATCHING>			vectSITEM_MATCHING;

struct SITEM_MATCH_CHANGE
{
	INT		FromItemNum;
	INT		ToItemNum;
};
typedef vector<SITEM_MATCH_CHANGE>		vectSITEM_MATCH_CHANGE;



///////////////////////////////////////////////////////////////////////////////
// 2009-04-06 by cmkwon, ÄÝ¿Àºê È÷¾î·Î ½ºÅ³ ½Ã½ºÅÛ º¯°æ - 
struct SSKILL_CONFIRM_USE
{
	int					SkillConfirmUseUID;
	DWORD				ExpireTick;
	UID32_t				TargetCharacterUID;
	int					UsingSkillItemNum;		// »ç¿ë ½ºÅ³ ItemNum
	MAP_CHANNEL_INDEX	MapChannelIndex;		// ½ºÅ³ »ç¿ëÀÚÀÇ MapChannelIndex
	D3DXVECTOR3			PositionVec3;
};
typedef mt_vector<SSKILL_CONFIRM_USE>			mtvectSSKILL_CONFIRM_USE;


///////////////////////////////////////////////////////////////////////////////
// 2009-07-09 by jwlee, °ÔÀÓ Á¤º¸ Å×ÀÌºí µ¥ÀÌÅÍ DB¿¡ ¹Ð¾î³Ö±â Åø ±¸Çö -
#define SIZE_MAX_ITEM_INFO_NAME					250
#define SIZE_MAX_TABINDEX						250
#define SIZE_MAX_ITEMDESCRIPTION				250
#define SIZE_MAX_OBJECT_DESCRIPTION				250

typedef struct 
{
	int		ShopOrder;
	int		UniqueNumber;
	int		ItemNum;
}SHOP_INFO;
typedef mt_vector<SHOP_INFO>			vectSHOP_INFO;

typedef struct
{
	int		MapIndex;
	int		BuildingIndex;
	int		WarpTargetMapIndex;
	int		WarpTargetIndex;
	char	WarpTargetName[SIZE_MAX_WARP_TARGET_NAME];
	int		Fee;
}CITY_TARGET_WARPMAP;
typedef mt_vector<CITY_TARGET_WARPMAP>			vectCITY_TARGET_WARPMAP;

typedef struct
{
	char			MapName[SIZE_MAX_MAP_NAME];
	MapIndex_t		MapIndex;
	int				StratrgyPiontNum;
}STRATEGYPOINT_SUMMON_MAPINDEX;
typedef mt_vector<STRATEGYPOINT_SUMMON_MAPINDEX>			vectSTRATEGYPOINT_SUMMON_MAPINDEX;

struct MONSTER_DATA : public MONSTER_INFO
{
	int		ControlSkil;
	int		SizeForServer;
	int		SizeForClient;
	int		MonsterItem[ARRAY_SIZE_MONSTER_ITEM];
};
typedef mt_vector<MONSTER_DATA>			vectMONSTER_DATA;

struct MAPOBJECT : public _MAPOBJECTINFO
{
	char	ObjectName[SIZE_MAX_OBJECT_NAME];
	USHORT	RadiusForServer;
	USHORT	RadiusForClient;
	char	ObjectDescription[SIZE_MAX_OBJECT_DESCRIPTION];
};
typedef mt_vector<MAPOBJECT>			vectMAPOBJECT;

struct LUCKY_MACHINE : public LUCKY_MACHINE_OMI
{
	int		SlotProbability;
	int		MysteryItemDropNum;
};
typedef mt_vector<LUCKY_MACHINE>			vectLUCKY_MACHINE;

typedef vector<MYSTERY_ITEM_DROP>				vectMYSTERY_ITEM_DROP;

typedef struct
{
	float	IsDisable;
	float	GameUID;
	float	ItemNum;
	char	ItemName[SIZE_MAX_ITEM_NAME];
	float	TabIndex;
	char	IconFileName[SIZE_MAX_TABINDEX];
	float	ItemUnitPrice;
	char	ItemDescription[SIZE_MAX_ITEMDESCRIPTION];
}ITEM_INFO;
typedef vector<ITEM_INFO>			vectItem_Info;			  

// 2009-09-16 by cmkwon, ¼¼·Â ÃÊ±âÈ­½Ã ¾îºäÂ¡ ¹æÁö ±¸Çö - ti_InfluenceRate Å×ÀÌºí Ãß°¡, 
struct STI_INFLUENCERATE
{
	BYTE	StartLevel;
	BYTE	EndLevel;
};
typedef vector<STI_INFLUENCERATE>		vectSTI_INFLUENCERATE;


///////////////////////////////////////////////////////////////////////////////
// 2009-08-03 by cmkwon, EP3-4 Æí´ë ´ëÇü ½ºÅ³ ±¸Çö - AtumProtocol.h¿¡ Á¤ÀÇµÇ¾î ÀÖ´Â°ÍÀ» ¿©±â·Î ¿Å±è
#define FLIGHT_FORM_NONE				(BYTE)0 // Æí´ë ºñÇà ¾È ÇÔ
#define FLIGHT_FORM_2_COLUMN			(BYTE)1 // ÀÌ·Ä Á¾´ë, ÀÌ·Ä Á¾´ë ¸ð¾çÀ¸·Î µÎ ÁÙ·Î ³ª¶õÈ÷ ¼± ¸ð¾çÀÌ´Ù
#define FLIGHT_FORM_2_LINE				(BYTE)2 // ÀÌ·Ä È¾´ë, ÀÌ·Ä È¾´ë ¸ð¾çÀ¸·Î µÎ ÁÙ·Î ³ª¶õÈ÷ ¼± ¸ð¾çÀÌ´Ù
#define FLIGHT_FORM_TRIANGLE			(BYTE)3 // »ï°¢ Æí´ë, »ï°¢Çü ¸ð¾çÀ¸·Î »ó´ÜºÎÅÍ 1, 2, 3°³ÀÇ À¯´ÖÀÌ À§Ä¡ÇÑ´Ù
#define FLIGHT_FORM_INVERTED_TRIANGLE	(BYTE)4 // ¿ª»ï°¢ Æí´ë, ¿ª »ï°¢Çü ¸ð¾çÀ¸·Î »ó´ÜºÎÅÍ 3, 2, 1°³ÀÇ À¯´ÖÀÌ À§Ä¡ÇÑ´Ù
#define FLIGHT_FORM_BELL				(BYTE)5 // Á¾ ÇüÅÂ, Á¾ ¸ð¾çÀ¸·Î »ó´ÜºÎÅÍ 1, 3, 2°³ÀÇ À¯´ÖÀÌ À§Ä¡ÇÑ´Ù
#define FLIGHT_FORM_INVERTED_BELL		(BYTE)6 // ¿ªÁ¾ ÇüÅÂ, ¿ªÁ¾ ¸ð¾çÀ¸·Î »ó´ÜºÎÅÍ 2, 3, 1°³ÀÇ À¯´ÖÀÌ À§Ä¡ÇÑ´Ù
#define FLIGHT_FORM_X					(BYTE)7 // XÀÚ ÇüÅÂ
#define FLIGHT_FORM_STAR				(BYTE)8 // º° ÇüÅÂ

#define ITEM_UID_FOR_PARTY_FLIGHT_FORMATION_SKILL_START			(INVALID_UID64-1000)	// Á¦ÀÏ µÚ¿¡ 1000°³¸¦ Æí´ë ½ºÅ³À» UID·Î »ç¿ëÇÏµµ·Ï ÇÑ´Ù.

///////////////////////////////////////////////////////////////////////////////
// 2009-09-16 by cmkwon, ¼¼·Â ÃÊ±âÈ­½Ã ¾îºäÂ¡ ¹æÁö ±¸Çö - 
#define PERIOD_OF_DAY_FOR_VALID_INFLUENCE		30		// À¯È¿ÇÑ ¼¼·Â ÃÖÁ¢ Á¢¼Ó ÀÏ¼ö

struct SINFL_USER_COUNT
{
	BYTE			StartLevel;
	BYTE			EndLevel;
	ATUM_DATE_TIME	UpdatedTime;
	INT				AllowablePercent;
	INT				arrUserCount[INFLUENCE_TYPE_COUNT_EX_ALL];		// index ==> 0:Normal, 1:BCU, 2:ANI

	BOOL AddUserCount(BYTE i_byInflTy, int i_nValues=1)
	{
		switch(i_byInflTy)
		{
		case INFLUENCE_TYPE_NORMAL:		arrUserCount[0] += i_nValues;		break;
		case INFLUENCE_TYPE_VCN:		arrUserCount[1] += i_nValues;		break;
		case INFLUENCE_TYPE_ANI:		arrUserCount[2] += i_nValues;		break;
		default:
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	BOOL SubtractUserCount(BYTE i_byInflTy, int i_nValues=1)
	{
		switch(i_byInflTy)
		{
		case INFLUENCE_TYPE_NORMAL:		arrUserCount[0] = max(0, arrUserCount[0] - i_nValues);		break;
		case INFLUENCE_TYPE_VCN:		arrUserCount[1] = max(0, arrUserCount[1] - i_nValues);		break;
		case INFLUENCE_TYPE_ANI:		arrUserCount[2] = max(0, arrUserCount[2] - i_nValues);		break;
		default:
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	BOOL IsSetForAll(void)
	{
		if(0 == StartLevel && 0 == EndLevel)
		{
			return TRUE;
		}
		return FALSE;
	}
};
typedef mt_vector<SINFL_USER_COUNT>		mtvectSINFL_USER_COUNT;

///////////////////////////////////////////////////////////////////////////////
// 2009-10-06 by cmkwon, º£Æ®³² °ÔÀÓ °¡µå X-TRAPÀ¸·Î º¯°æ - 
#define XTRAP_SIZE_MAX_SESSIONBUF		320			// 2009-10-06 by cmkwon, º£Æ®³² °ÔÀÓ °¡µå X-TRAPÀ¸·Î º¯°æ - ÃÖ´ë ÇÁ·ÎÅäÄÝ Å©±â
#define XTRAP_CSFILE_NUM				5			// 2009-10-06 by cmkwon, º£Æ®³² °ÔÀÓ °¡µå X-TRAPÀ¸·Î º¯°æ - ½Ç½Ã°£ Áö¿ø ½ÇÇàÆÄÀÏ ¾÷µ¥ÀÌÆ® °³¼ö
// 2009-10-29 by cmkwon, º£Æ®³² X-TRAP ÀÚµ¿¾÷µ¥ÀÌÆ® ±â´É Àû¿ë - 

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
	#define XTRAP_KEY_STRING				"660970B4963BD8A390256D9844CFE862F4BDBA635141DB3F0785BDC3E5836D033AFA4B1C226F9494F98192EBFF893A88756E0C810FF0175398D854AF913E10F40F7D04245246E406A670095295516F08D696B66EAE38D3BD5E287A3A23"
#endif


///////////////////////////////////////////////////////////////////////////////
// 2009-10-22 by cmkwon, ½Ã°£ Á¦ÇÑÇü·ù ¾ÆÀÌÅÛ ½Ã½ºÅÛ Á¤¸® - 
extern int GetNumberOfBits(INT64 i_n64Mask);
typedef enum{
	TIME_TERM_USING_ITEM	= 1,		//
	TIME_TERM_DELETE_ITEM,				//
	TIME_TERM_PREMIUM,					//
	TIME_TERM_EVENT						//
										//
} TIME_TERM_t;

// 2009. 10. 27 by jskim ÁøÇà·ü È®ÀÎ
#define WM_PROGRESSBAR_RANGE							WM_USER + 400	
#define WM_PROGRESSBAR_POS								WM_USER + 401	
//end 2009. 10. 27 by jskim ÁøÇà·ü È®ÀÎ


///////////////////////////////////////////////////////////////////////////////
// 2009-11-04 by cmkwon, ÅÂ±¹ °ÔÀÓ°¡µå Apex·Î º¯°æ - 
#define SECURITY_APEX_MaxPacketLen			1024	
#define SECURITY_APEX_ClientIpFlag			0x01
#define SECURITY_APEX_Cmd_Login				0x11
#define SECURITY_APEX_Cmd_Logout			0x12
#define SECURITY_APEX_Cmd_ApexData			0x13
#define SECURITY_APEX_Cmd_SimpleInputMsg	0x14
#define SECURITY_APEX_Cmd_ClientRet			0x15 

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2010. 05. 19 by hsLee ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷ ³­ÀÌµµ Á¶Àý. (½ÅÈ£Ã³¸® + ¸ó½ºÅÍ Ã³¸®(¼­¹ö) )
#define DEFAULT_LEVEL_INFINITY_DIFFICULTY	25					// ÀÎÇÇ´ÏÆ¼ ¹æ »ý¼º½Ã ±âº» ¼±ÅÃ ³­ÀÌµµ. ( ¸ðµå Á¤º¸°¡ [È®Àå-¼öÁ¤]µÈ´Ù¸é ¿ÜºÎ ÂüÁ¶·Î ¹Ù²Ü ¼ö ÀÖÀ½. )


////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - 

#define TICK_CREATE_MONSTER_TERM_ONLY_INFINITY		2592000000		// 1000Msec * 60 Sec * 60 Min * 24 Hour * 30 Day : ¾à ÇÑ ´Þ µÚ¿¡ ¼ÒÈ¯ µÇ´Â ÅÒ
#define TICK_FIRST_FORCED_CREATE_MONSTER_TERM		120000		// 1000Msec * 60 Sec * 2¤¤ Min : Ã¹ °­Á¦ ¼ÒÈ¯ ÅÒ
#define COUNT_DELAY_FORCED_CREATE_MONSTER_TERM		60			// 60 Sec * 1 Min : °­Á¦ ¼ÒÈ¯ ÈÄ ´Ù½Ã °­Á¦ ¼ÒÈ¯ Ã¼Å©±îÁö ÅÒ
#define TICK_TENDER_PROCESSING_TERM					20000		// 1000Msec * 20 Sec : Tender ÁøÇà ÅÒ

typedef enum
{
	HPACTION_TALK_CONDITION_NONE				= 0,
	HPACTION_TALK_CONDITION_CREATE				= 1,				// »ý¼º½Ã ´ëÈ­ Ãâ·Â
	HPACTION_TALK_CONDITION_DEAD				= 2,				// Á×À½½Ã ´ëÈ­ Ãâ·Â
	HPACTION_TALK_CONDITION_HP_RATE				= 3,				// HP»óÅÂ¿¡ µû¶ó ´ëÈ­ Ãâ·Â
	HPACTION_TALK_CONDITION_DAMAGED_RANDOM		= 4,				// ÇÇÇØ ¹ÞÀ» °æ¿ì ·£´ý ´ëÈ­ Ãâ·Â
	HPACTION_TALK_CONDITION_ATTACK				= 5,				// ÇØ´ç HPAction  °ø°Ý½Ã ´ëÈ­ Ãâ·Â
	HPACTION_TALK_CONDITION_TARGET_CHANGE		= 6,				// Å¸°Ù º¯°æ ½Ã ´ëÈ­ Ãâ·Â
} eHPACTION_TALK_CONDITION;
typedef enum
{
	HPACTION_TALK_IMPORTANCE_NONE				= 0,
	HPACTION_TALK_IMPORTANCE_ALL				= 1,				// ¸ðµç À¯Àú¿¡°Ô ´ëÈ­ Àü¼Û
	HPACTION_TALK_IMPORTANCE_INFL				= 2,				// °°Àº ¼¼·Â À¯Àú¿¡°Ô ´ëÈ­ Àü¼Û
	HPACTION_TALK_IMPORTANCE_CHANNEL			= 3,				// °°Àº Ã¤³Î À¯Àú¿¡°Ô ´ëÈ­ Àü¼Û
//	HPACTION_TALK_IMPORTANCE_PARTY				= 4,				// ÆÄÆ¼¿ø¿¡°Ô ´ëÈ­ Àü¼Û
} eHPACTION_TALK_IMPORTANCE;

#define SIZE_MAX_HPTALK_DESCRIPTION					SIZE_MAX_CHAT_MESSAGE
#define HPTALK_DAMAGED_RANDOM_RATE				3					// ÇÇ°Ý½Ã ´ë»ç ÇÒ È®·ü 3%
#define SIZE_MAX_CINEMATALK_DESCRIPTION				SIZE_MAX_HPTALK_DESCRIPTION

struct HPACTION
{
	HPActionUID_t					HPActionUID;
	HPActionIdx_t					HPActionNum;
	HPValueRate_t					HPMaxValueRate;
	HPValueRate_t					HPMinValueRate;
	ItemIdx_t						UseItemArrayIdx;
	ItemIdx_t						NextUseItemArrayIdx;
	HPHitRate_t						HitRate;
	HPUseCount_t					UseCount;
	eHPACTION_TALK_CONDITION		HPTalkCondition;
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							PreHPCameraTremble;
	HPTalk_t						PreHPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	MSec_t							HPCameraTremble;
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};
typedef vector<HPACTION>			vectHPAction;	
struct HPACTION_TALK_DAMAGED_RANDOM
{
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};
typedef vector<HPACTION_TALK_DAMAGED_RANDOM>	vectHPActionTalkDamagedRandom;
struct HPACTION_TALK_HPRATE
{
	HPValueRate_t					HPValueRate;
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							HPCameraTremble;
	ClientIndex_t					TargetClientIdx;			// ´ë»ó¿¡ °ü·ÃµÈ ´ë»ç°¡ ÀÖ´Ù¸é ¼³Á¤ÇÑ´Ù.
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};
typedef vector<HPACTION_TALK_HPRATE>	vectHPActionTalkHPRate;
struct HPACTION_ATTACK_HPRATE
{
	HPActionUID_t					HPActionUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÇÑ ¾ÆÀÌÅÛÀ¸·Î ¿©·¯ ´ë»ç °¡´ÉÇÏ°Ô ¼öÁ¤
	HPValueRate_t					HPMaxValueRate;
	HPValueRate_t					HPMinValueRate;
	ItemIdx_t						UseItemArrayIdx;
	ItemIdx_t						NextUseItemArrayIdx;
	HPHitRate_t						HitRate;
	HPUseCount_t					UseCount;
};
typedef vector<HPACTION_ATTACK_HPRATE>	vectHPActionAttackHPRate;

struct HPACTION_TALK_ATTACK
{
	ItemIdx_t						ItemNum;
	HPActionUID_t					HPActionUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÇÑ ¾ÆÀÌÅÛÀ¸·Î ¿©·¯ ´ë»ç °¡´ÉÇÏ°Ô ¼öÁ¤
	ItemIdx_t						UseItemArrayIdx;
	MSec_t							PreHPCameraTremble;
	HPTalk_t						PreHPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	MSec_t							HPCameraTremble;
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};
typedef vector<HPACTION_TALK_ATTACK>	vectHPActionTalkAttack; 

struct DEBUFFINFO {
	INT			ItemNum;						// ¾ÆÀÌÅÛ °íÀ¯¹øÈ£, ÀåÂø ¾ÆÀÌÅÛÀÏ ¶§ (ITEM_BASE*)
	BYTE		Kind;							// 104 : ¸ó½ºÅÍ ½ºÅ³ // ¾ÆÀÌÅÛ Á¾·ù(±â°üÆ÷, ºö, ·ÎÄÏ, ½ºÅ³.....), ITEMKIND_XXX
	DestParam_t	DesParam;						// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø)
	float		DesParamValue;
	INT			Time;							// Áö¼Ó ½Ã°£(½ºÅ³·ù µî) ´ÜÀ§ : ms
	BYTE		SkillLevel;						// ·¹º§
	BYTE		SkillType;						// ½ºÅ³ÇüÅÂ(½Ã°£ ¹× ¹ßµ¿ °ü·Ã), Áö¼Ó|Å¬¸¯|½Ã°£|À¯Áö
	DWORD		dwUseTime;						// µð¹öÇÁ Àû¿ë ½Ã°£
};
typedef mt_vector<DEBUFFINFO>					mtvectorDebuffInfo;

struct DOTINFO {
	UID32_t				FirstTimerUID;
	ClientIndex_t		ClientIndex;
	INT					ItemNum;						// ¾ÆÀÌÅÛ °íÀ¯¹øÈ£, ÀåÂø ¾ÆÀÌÅÛÀÏ ¶§ (ITEM_BASE*)
	BYTE				Kind;							// 104 : ¸ó½ºÅÍ ½ºÅ³ // ¾ÆÀÌÅÛ Á¾·ù(±â°üÆ÷, ºö, ·ÎÄÏ, ½ºÅ³.....), ITEMKIND_XXX
	DestParam_t			DesParam;						// 2011-08-01 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·áÇü º¯°æ (DestParameter - 255 -> 32767 Áö¿ø)
	float				DesParamValue;
	INT					Time;							// Áö¼Ó ½Ã°£(½ºÅ³·ù µî) ´ÜÀ§ : ms
};
typedef mt_vector<DOTINFO>					mtvectorDotInfo;

typedef enum
{
	INFINITY_MODE_NONE				= 0,
	INFINITY_MODE_BOSSRUSH			= 1,
	INFINITY_MODE_DEFENCE			= 2,
	INFINITY_MODE_MSHIPBATTLE		= 3					// 2011-02-18 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ð¼±¹èÆ²

} eINFINITY_MODE;

// start 2011-04-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
// typedef enum
// {
// 	INFINITY_KEYMONSTER_NONE					= 0,
// 	INFINITY_KEYMONSTER_DEAD_FOR_NEXT_STEP		= 1,
// 	INFINITY_KEYMONSTER_ALIVE_FOR_GAME_CLEAR	= 2,
// 
// } eINFINITY_KEYMONSTER;
// end 2011-04-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö

typedef enum		// ÀÎÇÇ´ÏÆ¼ ½ºÅµ »óÅÂ°ª.
{
	INFINITY_SKIP_CINEMA_NONE		= 0 ,			
	INFINITY_SKIP_CINEMA_NORMAL			,			// ½ºÅµ - Á¤»ó Á¾·á.
	INFINITY_SKIP_CINEMA_REQ_SKIP		,			// ½ºÅµ - À¯Àú ¿äÃ».

} eINFINITY_SKIP_CINEMA;


struct CINEMAINFO {	// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - ±¸Á¶ º¯°æ
	CinemaNum_t			CinemaNum;	
	CinemaOrder_t		CinemaOrder;		// 51000 : ¼º°ø Á¾·á ½Ã³×¸¶, 52000 : ½ÇÆÐ Á¾·á ½Ã³×¸¶, CINEMA_FIN_XXX
	MSec_t				StartTime;			// msec
	EffectIdx_t			EffectIdx;
	Position_t			EffectPosition_X;
	Position_t			EffectPosition_Y;
	Position_t			EffectPosition_Z;	
	MSec_t				EffectPlayTime;		// msec	
	EffectPlayCount_t	EffectPlayCount;
	ObjectIdx_t			ObjectIdx;					// ¸Ê¿¡ ÂïÈù ObjectÀÇ EvnetParamIndex
	ObjectNum_t			ChangeObjectIdx;			// º¯°æµÉ ObjectNum
	MSec_t				ChangeTime;			// msec			
	MSec_t				CameraTremble;		// msec	
	KeyMonster_t		KeyMonster;		// 2011-04-21 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
	MonIdx_t			MonsterIdx;					// ¼ÒÈ¯µÉ MonsterUnitkind
	SummonCnt_t			SummonCount;	
	Distance_t			SummonRandomPos;	// M
	ObjectIdx_t			ObjectBodyConditionIdx;		// ¸Ê¿¡ ÂïÈù ObjectÀÇ EvnetParamIndex
	BodyCond_t			BodyCondition;				// ObjectBodyConditionIdx Object¿¡ ¼³Á¤ ÇÒ BodyCondition(¼º°ø:1024, ½ÇÆÐ:32)
	FileName_t			TipFileName[SIZE_MAX_FILE_NAME];	
	FileName_t			SkyFileName[SIZE_MAX_FILE_NAME];
	CinemaTalk_t		CinemaTalk[SIZE_MAX_CINEMATALK_DESCRIPTION];
	FileName_t			SoundFileName[SIZE_MAX_FILE_NAME];		// 2010-03-29 by cmkwon, ÀÎÇÇ2Â÷ ½Ã³×¸¶¿¡ È¿°úÀ½ ÆÄÀÏ ¼³Á¤ Ãß°¡ - 
	// start 2011-04-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡
	QuestIndex_t		QuestIndex;					// 2011-03-09 by hskim, ½Ã³×¸¶ Äù½ºÆ® ÀÎµ¦½º ¼³Á¤ Ãß°¡
	eCINEMA_MONSTER_INDEX_TYPE	MonsterIdxType;		// CINEMA_MONSTER_INDEX_CREATE = 0 ,	CINEMA_MONSTER_INDEX_CREATE = 1
	CustomIdx_t			CustomIdx;					// 1xxx = ½Ã³×¸¶ 1¹ø , 2xxx = ½Ã³×¸¶ 2¹ø , 3xxx = ½Ã³×¸¶ 3¹ø
	MonIdx_t			UpdateMonsterIdx;			// Àû¿ëÇÒ ¸ó½ºÅÍ ÀÎµ¦½º
	// end 2011-04-28 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ °ü·Ã ±â´É Ãß°¡
};	
typedef vector<CINEMAINFO>					vectorCinemaInfo;

struct REVISIONINFO {
	RevisionNum_t		RevisionNum;
	INT					UnitKind;
	Lv_t				RevisionLevel;
	RevisionHP_t		RevisionHP;
	RevisionDP_t		RevisionDP;
	RevisionWeapon1_t	RevisionStandardWeapon;
	RevisionWeapon2_t	RevisionAdvancedWeapon;
};
typedef vector<REVISIONINFO>				vectorRevisionInfo;

struct INFINITY_MONSTERINFO {
	UID32_t				InfinityMonsterUID;
	MonIdx_t			InfinityMonsterIdx;
	MonIdx_t			MonsterIdx;	
	InfiMonTargetType_t	TargetType;
	InfiMonTargetCount_t	TargetCount;
};
typedef vector<INFINITY_MONSTERINFO>		vectorInfinityMonsterInfo;

struct INFINITY_MODEINFO {
	InfiModeUID_t		InfinityModeUID;
	MapIndex_t			MapIdx;		
	eINFINITY_MODE		ModeTypeNum;
	MonIdx_t			InfinityMonsterIdx;
	CinemaNum_t			CinemaNum;
	RevisionNum_t		RevisionNum;
	ResetCycle_t		ResetCycle;
	EntranceCount_t		EntranceCount;
	MSec_t				LimitTime;
	Lv_t				MinLv;
	Lv_t				MaxLv;
	AdmissionCount_t	MinAdmissionCount;
	AdmissionCount_t	MaxAdmissionCount;
	Minute_t			TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ »ç¸Á½Ã ÆÐ³ÎÆ¼ Ãß°¡
	Penalty_t			HPPenaltyValue;			// 2011-06-14 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÆÐ³ÎÆ¼ ±â´É Ãß°¡ (HP ¹× ½Ã°£ µ¿½Ã Áö¿øÀ» À§ÇØ)
};
typedef vector<INFINITY_MODEINFO>		vectorInfinityModeInfo;

struct INFINITY_IMPUTE {
	InfiModeUID_t		InfinityModeUID;
	EntranceCount_t		EntranceCount;
};
typedef mt_vector<INFINITY_IMPUTE>			mt_vectorInfinityImpute;
typedef vector<INFINITY_IMPUTE>				vectINFINITY_IMPUTE;			// 2010-04-05 by cmkwon, ÀÎÇÇ ÀçÀÔÀå Ä«µå °ü·Ã ½Ã½ºÅÛ ¼öÁ¤ - 

typedef enum
{
	INFINITY_STATE_NONE					= 0,
	INFINITY_STATE_UNPREPARED			= 1,				// ÀÎÇÇ´ÏÆ¼ ÀÔÀåÀº ÇßÁö¸¸ ·¡µÚ´Â ´©¸£Áö ¾ÊÀº »óÅÂ
	INFINITY_STATE_READY				= 2,				// ÀÎÇÇ´ÏÆ¼ ¸Ê¿¡ ÀÔÀå Àü »óÅÂ
	INFINITY_STATE_READY_ENTER_CHECK	= 3,				// 2010-03-23 by cmkwon, ÀÎÇÇ´ÏÆ¼ ÀÔÀå Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - MF·ÎºÎÅÍ Start ÀÎÁõÀ» º¸³½ »óÅÂ
	INFINITY_STATE_READY_ENTER_CHECKED	= 4,				// 2010-03-23 by cmkwon, ÀÎÇÇ´ÏÆ¼ ÀÔÀå Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - MF·ÎºÎÅÍ Start ÀÎÁõÀ» ¹ÞÀº »óÅÂ
	INFINITY_STATE_ENTERING				= 5,				// 2009-09-09 ~ 2010-01-29 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ´ÏÆ¼ ½ÃÀÛ ¹öÆ° ´©¸¥ »óÅÂ Ãß°¡
	INFINITY_STATE_MAPLOADED			= 6,				// ÀÎÇÇ´ÏÆ¼ ¸Ê ·Îµù ¿Ï·á
	INFINITY_STATE_PLAYING				= 7,				// ÀÎÇÇ´ÏÆ¼ ¸Ê¿¡ ÀÔÀåÇÏ¿© ÁøÇàÁß »óÅÂ 
	INFINITY_STATE_DONE_SUCCESS			= 8,				// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - // ÀÎÇÇ´ÏÆ¼ Á¾·á ¼º°ø »óÅÂ
	INFINITY_STATE_DONE_FAIL			= 9,				// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - // ÀÎÇÇ´ÏÆ¼ Á¾·á ½ÇÆÐ »óÅÂ

	// PLAYING_STEP ¾Æ´Ñ °æ¿ì ¿©±â¿¡ Ãß°¡ÇÏ¼¼¿ä

// start 2011-04-08 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
// [ÁÖÀÇ] ½ºÅÜÀÌ Ãß°¡µÉ°æ¿ì ¾Æ·¡ »çÇ× ¼öÁ¤ÇØÁÖ±â
// IS_INFINITY_STATE_PLAYING_LAST/IS_INFINITY_STATE_PLAYING/IS_INFINITY_STATE_PLAYING_STEP/CINEMA_PLAYING_STEP_MAX
// CFieldIOCPSocket::InfinityOnCharacterGameEndRoutine ÇÔ¼ö ³» switch ¹®

	INFINITY_STATE_PLAYING_STEP1		= 101,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 1 - ÁÖÆ÷ ÆÄ±«
	INFINITY_STATE_PLAYING_STEP2		= 102,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 2 - È°ÁÖ·Î ÆÄ±«
	INFINITY_STATE_PLAYING_STEP3		= 103,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 3 - °Ý³³°í Ä§Åõ
	INFINITY_STATE_PLAYING_STEP4		= 104,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 4 - ·¹ÀÌ´õ ±âÁö ÆÄ±«
	INFINITY_STATE_PLAYING_STEP5		= 105,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 5-1	- ¿£Áø½Ç ÆÄ±« 1´Ü°è (º¸È£¸· ÆÄ±«)
	INFINITY_STATE_PLAYING_STEP6		= 106,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 5-2	- ¿£Áø½Ç ÆÄ±« 2´Ü°è (¿£Áø ÆÄ±«)
	INFINITY_STATE_PLAYING_STEP7		= 107,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 5-3	- ¿¬Ãâ (Àû±º ¸ð¼± Ä§¸ô ¿¬Ãâ)
	INFINITY_STATE_PLAYING_STEP8		= 108,				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 6		- º¸³Ê½º ½ºÅÜ (Å»Ãâ¼± Àâ±â)
	INFINITY_STATE_PLAYING_STEP9		= 109				// ÀÎÇÇ´ÏÆ¼ 3Â÷ ½ºÅÜ 7		- Á¾·á Ã³¸®
// end 2011-04-08 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
} eINFINITY_STATE;

// start 2011-04-11 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
#define IS_INFINITY_STATE_PLAYING_LAST(_IT_)		(INFINITY_STATE_PLAYING_STEP9 == (_IT_))		// ¸¶Áö¸· ½ºÅÜÀº ´©±¸? - ²À ¼³Á¤ÇØÁÖ±â
#define IS_INFINITY_STATE_PLAYING(_IT_)		(INFINITY_STATE_PLAYING == (_IT_) || INFINITY_STATE_PLAYING_STEP1 == (_IT_) ||	\
											INFINITY_STATE_PLAYING_STEP2 == (_IT_) || INFINITY_STATE_PLAYING_STEP3 == (_IT_) ||		\
											INFINITY_STATE_PLAYING_STEP4 == (_IT_) || INFINITY_STATE_PLAYING_STEP5 == (_IT_) ||		\
											INFINITY_STATE_PLAYING_STEP6 == (_IT_) || INFINITY_STATE_PLAYING_STEP7 == (_IT_) ||	\
											INFINITY_STATE_PLAYING_STEP8 == (_IT_) || INFINITY_STATE_PLAYING_STEP9 == (_IT_))

#define IS_INFINITY_STATE_PLAYING_STEP(_IT_)		( INFINITY_STATE_PLAYING_STEP1 == (_IT_) ||	INFINITY_STATE_PLAYING_STEP2 == (_IT_) ||	\
													INFINITY_STATE_PLAYING_STEP3 == (_IT_) || INFINITY_STATE_PLAYING_STEP4 == (_IT_) ||	\
													INFINITY_STATE_PLAYING_STEP5 == (_IT_) || INFINITY_STATE_PLAYING_STEP6 == (_IT_) || \
													INFINITY_STATE_PLAYING_STEP7 == (_IT_) || INFINITY_STATE_PLAYING_STEP8 == (_IT_) || \
													INFINITY_STATE_PLAYING_STEP9 == (_IT_))
// end 2011-04-11 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö


struct INFINITY_PLAYING_INFO {
	InfinityCreateUID_t InfinityCreateUID;
	InfiModeUID_t		InfinityModeUID;
	MapIndex_t			MapIdx;		
	eINFINITY_MODE		ModeTypeNum;
	eINFINITY_STATE		InfinityState;
	Lv_t				RevisionLevel;
	RevisionHP_t		RevisionHP;
	RevisionDP_t		RevisionDP;
	RevisionWeapon1_t	RevisionStandardWeapon;
	RevisionWeapon2_t	RevisionAdvancedWeapon;
	KillCnt_t			KillCount;
	DeathCnt_t			DeathCount;
	Minute_t			TimePenaltyValue;			// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ »ç¸Á½Ã ÆÐ³ÎÆ¼ Ãß°¡
	BOOL				bHaveReentryTicket;			// 2010-03-23 by cmkwon, ÀÎÇÇ´ÏÆ¼ ÀÔÀå Ä³½¬ ¾ÆÀÌÅÛ ±¸Çö - ÇØ´ç ¾ÆÀÌÅÛÀ» ¼ÒÀ¯ÇÏ±â ÀÖ´ÂÁö ¿©ºÎ
	Penalty_t			HPPenaltyValue;				// 2011-06-14 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÆÐ³ÎÆ¼ ±â´É Ãß°¡ (HP ¹× ½Ã°£ µ¿½Ã Áö¿øÀ» À§ÇØ)
};

#define INFINITY_CREATE_MAP_CHANNEL_IDX_ERROR	-1

struct INFINITY_INSERT_ITEM_INFO {
	UID64_t			ItemUID;
	INT				ItemNum;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	BYTE			Wear;
	INT				ShapeItemNum;
	INT				UsingTimeStamp;
	ATUM_DATE_TIME	CreatedTime;
	INT				CoolingTimeStamp;		// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
};
typedef mt_vector<INFINITY_INSERT_ITEM_INFO>			mtvectorInfinityInsertItemInfo;

struct INFINITY_UPDATE_ITEM_INFO {
	UID64_t			ItemUID;
	UID64_t			MainSvrItemUID;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				UsingTimeStamp;
	INT				CoolingTimeStamp;		// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
};
typedef mt_vector<INFINITY_UPDATE_ITEM_INFO>			mtvectorInfinityUpdateItemInfo;

struct INFINITY_DELETE_ITEM_INFO {
	UID64_t			MainSvrItemUID;
};
typedef mt_vector<INFINITY_DELETE_ITEM_INFO>			mtvectorInfinityDeleteItemInfo;


///////////////////////////////////////////////////////////////////////////////
// 2009-12-11 by cmkwon, µ¥¹ÌÁö ¾î±×·Î·Î Å¸°ÙÀ» º¯°æÇÏ´Â ¸ó½ºÅÍ ±¸Çö - 
#define TICK_MONSTER_TARGET_UNCHANGE_TERM			15000			// 15ÃÊ°£ Å¸°Ù À¯Áö
#define TICK_MONSTER_DELETE_TARGET_TERM				15000			// 2010-03-02 by cmkwon, ¸ó½ºÅÍ Å¸°Ù »èÁ¦ Ã¼Å© 15ÃÊ·Î º¯°æ - // ¸ó½ºÅÍ°¡ 30ÃÊ°£ °ø°ÝÇÏÁö ¸øÇÑ Å¸°ÙÀº »èÁ¦ÇÑ´Ù.

struct TENDER_INFO {
	TenderUID_t		TenderUID;
	ItemNum_t		TenderItemNum;
};
typedef vector<TENDER_INFO>								vectorTenderInfo;


#define MAX_DICE_COUNT						100
#define MIN_DICE_COUNT						1
#define EXIT_DICE_COUNT						100		// 2009-09-09 ~ 2010-01 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¼Ò½º Ã¼Å©, ¹«ÇÑ ·çÇÁ ´ëºñ¿ë

///////////////////////////////////////////////////////////////////////////////
// 2009-12-29 by cmkwon, Ä³¸¯ÅÍ ÃÖ´ë ·¹º§ »óÇâ(110À¸·Î) - 1~100~110
#define CHARACTER_LEVEL_100					100
#define CHARACTER_LEVEL_100_MAX_STAT_POINT	300
#define CHARACTER_LEVEL_110					110
#define CHARACTER_LEVEL_110_MAX_STAT_POINT	340

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-17 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·á ±¸Á¶ °áÁ¤
#define PET_LEVEL_25						25
#define PET_LEVEL_MAX						PET_LEVEL_25

// 2010-02-19 by cmkwon, 100ÀÌÇÏ ·¹º§ ÃÖ´ë½ºÅÈ ¼öÄ¡ ¹ö±× ¼öÁ¤ - 
#define GET_MAX_STAT_POINT(level)			( ((level) <= CHARACTER_LEVEL_100) ? CHARACTER_LEVEL_100_MAX_STAT_POINT : CHARACTER_LEVEL_110_MAX_STAT_POINT)

// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÀÎÇÇ »óÁ¡
struct INFINITY_SHOP_INFO {
	InfinityShopUID_t		InfinityShopUID;
	ItemNum_t				BuyItemNum;
	InfinityShopItemCnt_t	BuyItemCount;
	ItemNum_t				TradeItemNum1;
	InfinityShopItemCnt_t	TradeItemCount1;
	ItemNum_t				TradeItemNum2;
	InfinityShopItemCnt_t	TradeItemCount2;
	ItemNum_t				TradeItemNum3;
	InfinityShopItemCnt_t	TradeItemCount3;
	ItemNum_t				TradeItemNum4;
	InfinityShopItemCnt_t	TradeItemCount4;
	ItemNum_t				TradeItemNum5;	
	InfinityShopItemCnt_t	TradeItemCount5;
};
typedef vector<INFINITY_SHOP_INFO>						vectorInfinityShopInfo;


///////////////////////////////////////////////////////////////////////////////
// 2010-01-18 by cmkwon, ¾ÆÀÌÅÛ »ç¿ë½Ã Parameter Áßº¹ Ã¼Å© ½Ã½ºÅÛ ±¸Çö - 
struct SITEM_PARAM_OVERLAP
{
	INT			ItemNum;
	INT			OverlapIndex;		// °°Àº OverlapIndex°¡ ÀÖÀ¸¸é »ç¿ë ºÒ°¡
};
typedef vector<SITEM_PARAM_OVERLAP>		vectSITEM_PARAM_OVERLAP;	// 
typedef ez_map<INT, vectINT>			ezmapINT2vectINT;			// ITEMNUM and OverlapIndexVector

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ÆÃ±ä À¯Àú ÀçÁ¢¼Ó Ã³¸®
struct INFINITY_DISCONNECTUSER_INFO {
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];
	InfinityCreateUID_t		InfinityCreateUID;		// ÇØ´ç ¹æÀÇ °íÀ¯ ¹øÈ£
	USHORT					MainServerID;			// ÀÔÀåÇÑ Ä³¸¯ÅÍÀÇ ¼­¹ö
	eINFINITY_MODE			InfinityMode;
};
typedef mt_vector<INFINITY_DISCONNECTUSER_INFO>		mtvectorInfinityDisConnectUser;

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
struct INVOKING_ITEM_DESTPARAM_TYPE {
	ItemUID_t				InvokingItemUID;
	DestParam_t				InvokingDestParam;
	ParamValue_t			InvokingDestParamValue;
	EffectIdx_t				InvokingEffectIdx;
};
typedef vector<INVOKING_ITEM_DESTPARAM_TYPE>		InvokingItemDestParamTypeList;
typedef mt_vector<INVOKING_ITEM_DESTPARAM_TYPE>		mtInvokingItemDestParamTypeList;

///////////////////////////////////////////////////////////////////////////////
// 2010-03-31 by dhjin, ÀÎÇÇ´ÏÆ¼(±âÁö¹æ¾î) - CINEMA_FIN_XXX
#define CINEMA_FIN_SUCCESS				51000
#define CINEMA_FIN_FAIL					52000
#define PENALTY_REMAIN_HP				100

// start 2011-04-08 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö
#define CINEMA_PLAYING_STEP_MAX			9			// ÃÖ´ë Áö¿øÇÏ´Â Playing Step ¼ö - ÃÖ´ë ¼ö º¯°æ½Ã eINFINITY_STATE / IS_INFINITY_STATE_PLAYING ¼öÁ¤ÇÒ °Í

#define CINEMA_PLAYING_STEP_START_NUMBER			10001		// PLAYING ½ºÅÜ ½ÃÀÛ ¹øÈ£	- STEP1 = 10001,	STEP2 = 10002,	STEP3 = 10003,	STEP4 = 10004,	STEP5 = 10005,	STEP6 = 10006, STEP7 = 10007, STEP8 = 10008
// end 2011-04-08 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ´Ü°èº° ±â´É ±¸Çö

// 2010-04-07 by cmkwon, ÀÎÇÇ2Â÷ Ãß°¡ ¼öÁ¤ - 
typedef enum
{
	INFINITY_FIN_UNKNOWN					= 0,		// ¾Ë ¼ö ¾øÀ½
	INFINITY_FIN_SUCCESS_BY_PASS_ALL_STEP	= 100,		// ¼º°ø: ¸ðµç ´Ü°è(Step)À» Åë°ú
	INFINITY_FIN_SUCCESS_BY_TIMEOVER,					// ¼º°ø: Á¦ÇÑ ½Ã°£ °æ°ú
	INFINITY_FIN_SUCCESS_BY_DEAD_ALL_KEY_MONSTER_FOR_NEXTSTEP,	// 2010-04-09 by cmkwon, ÀÎÇÇ2Â÷(¼º°ø Á¶°Ç Ãß°¡) - MPOPTION_BIT_KEY_MONSTER¸¦ ¼³Á¤ÇÑ ¸ó½ºÅÍ°¡ ¸ðµÎ Á¦°ÅµÊ
	INFINITY_FIN_FAIL_BY_TIMEOVER			= 200,		// ½ÇÆÐ: ¼¼ÇÑ ½Ã°£ °æ°ú
	INFINITY_FIN_FAIL_BY_DEAD_KEYMONSTER				// ½ÇÆÐ: Å°¸ó½ºÅÍ ÆÄ±«µÊ
} INFINITY_FIN_t;


///////////////////////////////////////////////////////////////////////////////
// 2010-04-20 by cmkwon, ½Å±Ô ·¯Å° ¸Ó½Å ±¸Çö - ±âÁ¸ ¼Ò½º ¼öÁ¤ - LUCKY_MACHINE_KIND_XXX, LUCKY_MACHINE_XXX
#define LUCKY_MACHINE_KIND_UNKNOWN				0		// ¾Ë¼ö ¾øÀ½
#define LUCKY_MACHINE_KIND_HAPPY_MACHINE		1		// ÇØÇÇ¸Ó½Å
#define LUCKY_MACHINE_KIND_WEAPON_MACHINE		2		// ¹«±â¸Ó½Å
#define LUCKY_MACHINE_KIND_ARMOR_MACHINE		3		// ¾Æ¸Ó¸Ó½Å
#define LUCKY_MACHINE_KIND_MYSTARY_MACHINE		4		// ¹Ì½ºÅ×¸®¸Ó½Å

// 2010-06-01 by shcho, GLogDB °ü·Ã -
struct MOSTSTAYED_MAP_INFO
{
	MapIndex_t  MapIndex;	 // ¸Ó¹«¸¥ ¸Ê ÀÎµ¦½º
	DWORD		StayedTime;	 // ¸Ó¹°·¶´ø ÃÑ ´©Àû½Ã°£
};

// 2010-08-05 by dhjin, ¹ö´×¸Ê - 
typedef INT		BurningMapUID_t;
typedef	USHORT	UnitKind_t;
struct BURNING_MAP_INFO {
	BurningMapUID_t		BurningMapUID;
	MapIndex_t			MapIndex;
	UnitKind_t			ReqUnitKind;
	Lv_t				ReqMinLv;
	Lv_t				ReqMaxLv;
};
typedef vector<BURNING_MAP_INFO>	BurningMapInfoList;


///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-17 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·á ±¸Á¶ °áÁ¤

//////////
// ±âÁ¸

//// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ
//#define SIZE_MAX_PETSKILLITEM 6			// ÆêÀÇ ½ºÅ³ ¾ÆÀÌÅÛ °³¼ö.
//#define SIZE_MAX_PETSOCKETITEM 6		// ÆêÀÇ ¼ÒÄÏ ½½·Ô °³¼ö.
//
//typedef struct 
//{	// ÆêÀÇ ±âº» Á¤º¸.
//
//	char	szPetName[SIZE_MAX_PET_NAME];
//
//	INT		UniqueNumber;
//	INT		PetIndex;
//	INT		PetKind;
//	BOOL	EnableName;
//	BOOL	EnableLevel;
//
//} tPET_BASEDATA;
//
//typedef std::vector< tPET_BASEDATA > vectorPetBaseData;
//
//typedef struct 
//{	// Æê ·¹º§¾÷ °ü·Ã Á¤º¸.
//
//	INT				UniqueNumber;
//	INT				Level;
//	Experience_t	NeedExp;	
//	DOUBLE			Stamina;
//	INT				CitySourceIndex;
//	INT				FieldSourceIndex;
//	INT				PetAttackIndex; // 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê °ø°Ý Ã³¸® ÃÑ¾Ë ÀÎµ¦½º ÇÊµå Ãß°¡
//
//	INT				PetSkillItemIndex[SIZE_MAX_PETSKILLITEM];
//
//} tPET_LEVEL_DATA;
//
//typedef std::vector< tPET_LEVEL_DATA > vectorPetLevelData;
//
//struct tPET_BASE_ALL_DATA
//{
//	tPET_BASEDATA		BaseData;
//	vectorPetLevelData  vectPetLevelDataInfo;
//
//	tPET_LEVEL_DATA *rtn_LevelData ( const int a_iLevel )
//	{
//		for ( unsigned ui = 0; ui < this->vectPetLevelDataInfo.size(); ++ui )
//		{
//			if ( this->vectPetLevelDataInfo[ui].Level == a_iLevel )
//				return &this->vectPetLevelDataInfo[ui];
//		}
//		return NULL;
//	}
//
//};
//
//typedef std::vector< tPET_BASE_ALL_DATA > vectorPetDataInfo;
//
//struct tPET_CURRENTINFO
//{	// ¼ÒÀ¯ÇÑ Æê Á¤º¸.
//
//	UID64_t			CreatedPetUID;
//
//	BOOL			EnableEditPetName;
//	BOOL			EnableLevelUp;
//
//	char			szPetName[SIZE_MAX_PET_NAME];
//	
//	INT				PetIndex;
//	INT				PetLevel;
//	
//	Experience_t	PetExp;
//	
//	DOUBLE			Stamina;
//
//	INT				SourceIndex_Field;
//	INT				SourceIndex_City;
//
//	const ITEM			*pItem_PetSkill[SIZE_MAX_PETSKILLITEM];
//	const ITEM			*pItem_PetSocketItem[SIZE_MAX_PETSOCKETITEM];
//
//#ifdef _ATUM_SERVER
//
//	tPET_CURRENTINFO ( tPET_CURRENTINFO *pPetCurInfo )
//	{
//		if ( pPetCurInfo )
//			memcpy ( this , pPetCurInfo , sizeof( tPET_CURRENTINFO ) );
//		else
//			memset ( this , 0x00 , sizeof( tPET_CURRENTINFO ) );
//	}
//
//	void * operator new(size_t size);
//	void operator delete(void* p);
//#endif
//	
//};
//
//typedef std::vector		< tPET_CURRENTINFO* > vecPetCurrentInfo;
//
//typedef mt_vector		< tPET_CURRENTINFO* > mtvectPetCurrentInfo;
//
// End 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ

///////////////////////////////////////////////////////////////////////////////////////
// ¼öÁ¤

#define SIZE_MAX_PETSOCKET					6		// ÆêÀÇ ¼ÒÄÏ ½½·Ô °¹¼ö
#define SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT	6		// ÆêÀÇ ¿ÀÅä ½ºÅ³ ¼ÒÄÏ °¹¼ö

typedef struct 
{
	INT		PetIndex;
	char	PetName[SIZE_MAX_PET_NAME];	
	INT		PetKind;
	BOOL	EnableName;
	BOOL	EnableLevel;
	INT		MaxLevel;
	INT		BaseSocketCount;

} tPET_BASEDATA;

typedef std::vector< tPET_BASEDATA > vectorPetBaseData;

typedef struct 
{
	INT				PetIndex;
	INT				Level;
	Experience_t	NeedExp;	
	INT				UseWeaponIndex;
	BYTE			SlotCountSkill;		// ÇöÀç ·¹º§¿¡¼­ Áö¿øÇÏ´Â ½ºÅ³ ¼ö
	BYTE			SlotCountSocket;	// ÇöÀç ·¹º§¿¡¼­ Áö¿øÇÏ´Â ¼ÒÄÏ ¼ö
	BYTE			KitLevelHP;			// ÇöÀç ·¹º§¿¡¼­ Áö¿øÇÏ´Â ÃÖ°í HP Kit
	BYTE			KitLevelShield;		// ÇöÀç ·¹º§¿¡¼­ Áö¿øÇÏ´Â ÃÖ°í ½¯µå Kit
	BYTE			KitLevelSP;			// ÇöÀç ·¹º§¿¡¼­ Áö¿øÇÏ´Â ÃÖ°í SP Kit

} tPET_LEVEL_DATA;

typedef std::vector< tPET_LEVEL_DATA > vectorPetLevelData;

struct tPET_BASE_ALL_DATA
{
	tPET_BASEDATA		BaseData;
	vectorPetLevelData  vectPetLevelDataInfo;

	tPET_LEVEL_DATA *rtn_LevelData ( const int a_iLevel )
	{
		for ( unsigned ui = 0; ui < this->vectPetLevelDataInfo.size(); ++ui )
		{
			if ( this->vectPetLevelDataInfo[ui].Level == a_iLevel )
				return &this->vectPetLevelDataInfo[ui];
		}
		return NULL;
	}

};

typedef std::vector< tPET_BASE_ALL_DATA > vectorPetDataInfo;

struct SPET_KIT_SLOT_ITEM_LEVEL
{
	int Level;				// ·¹º§				[0 ~ 100]
	int	ItemNum;			// ¾ÆÀÌÅÛ ¹øÈ£
};

struct SSort_KIT_SLOT_ITEM_LEVEL_BY_LEVEL: binary_function<int, int, bool>
{
	bool operator()(SPET_KIT_SLOT_ITEM_LEVEL param1, SPET_KIT_SLOT_ITEM_LEVEL param2)
	{
        return param1.Level < param2.Level;
    };
};

typedef std::vector<SPET_KIT_SLOT_ITEM_LEVEL> vectorPetKitLevel;		// ¼Óµµ ÃÖÀûÈ­¸¦ À§ÇØ¼­ ²À Á¤¿­ÇØ¼­ »ç¿ëÇÏ¼¼¿ä


struct SPET_AUTOSKILL_SLOT_ITEM
{
	int	ItemNum;
};

typedef std::vector<SPET_AUTOSKILL_SLOT_ITEM> vectorPetAutoSkill;


struct SPET_KIT_SLOT_DATA
{
	INT		ItemNum;
	float	TriggerValue;
};

struct SPET_AUTOSKILL_SLOT_DATA
{
	INT		ItemNum[SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT];
};

struct tPET_CURRENTINFO
{
	UID64_t			CreatedPetUID;
	char			PetName[SIZE_MAX_PET_NAME];
	INT				PetIndex;
	INT				PetLevel;
	Experience_t	PetExp;
	BYTE			PetExpRatio;
	BYTE			PetEnableSocketCount;
	UID64_t			PetSocketItemUID[SIZE_MAX_PETSOCKET];

	// ½½·Ô µ¥ÀÌÅÍ Á¤º¸

	SPET_KIT_SLOT_DATA	PetKitHP;
	SPET_KIT_SLOT_DATA	PetKitShield;
	SPET_KIT_SLOT_DATA	PetKitSP;

	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;

	// ¸µÅ©

	ITEM			*pItemPetSocket[SIZE_MAX_PETSOCKET];

#ifdef _ATUM_SERVER
	tPET_CURRENTINFO ( tPET_CURRENTINFO *pPetCurInfo )
	{
		if ( pPetCurInfo )
			memcpy ( this , pPetCurInfo , sizeof( tPET_CURRENTINFO ) );
		else
			memset ( this , 0x00 , sizeof( tPET_CURRENTINFO ) );
	}

	void * operator new(size_t size);
	void operator delete(void* p);
#endif
	
};

typedef std::vector		< tPET_CURRENTINFO* > vecPetCurrentInfo;
typedef mt_vector		< tPET_CURRENTINFO* > mtvectPetCurrentInfo;

// end 2011-08-17 by hskim, ÆÄÆ®³Ê ½Ã½ºÅÛ 2Â÷ - ÀÚ·á ±¸Á¶ °áÁ¤
///////////////////////////////////////////////////////////////////////////////////////

// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¿ÀÆÛ·¹ÀÌÅÍ Á¤º¸

struct OperatorInfo
{	// ¿ÀÆÛ·¹ÀÌÅÍ Á¤º¸.
	int		ActionType;
	int		Itemnum;
	int		FunctionIndex;
	double	FunctionValue;
	int		SourceIndex;
	char	ActionDesc[128]; 
};

typedef std::vector< OperatorInfo > vectorOperatorInfo;

// END 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¿ÀÆÛ·¹ÀÌÅÍ Á¤º¸

// 2010-08-31 by shcho&jskim ¾ÆÀÌÅÛ¿ëÇØ ½Ã½ºÅÛ - ¿ëÇØ ¾ÆÀÌÅÛ Á¤º¸ ±¸Á¶Ã¼ 

#define SIZE_MAX_DISSOLUTION_ITEMS	3

struct tDissolutionItemInfo
{
	INT		SourceItemnum;				// ¿ëÇØ ÇÒ ¾ÆÀÌÅÛ ¹øÈ£
	INT		ResultItemNum[SIZE_MAX_DISSOLUTION_ITEMS];			// °á°ú ¾ÆÀÌÅÛ ¹øÈ£
	INT		ResultItemMinCount[SIZE_MAX_DISSOLUTION_ITEMS];		// ¾ÆÀÌÅÛ ÃÖ¼Ò °¹¼ö
	INT		ResultItemMaxCount[SIZE_MAX_DISSOLUTION_ITEMS];		// ¾ÆÀÌÅÛ ÃÖ´ë °¹¼ö
	INT		ResultItemProbabillity[SIZE_MAX_DISSOLUTION_ITEMS];	// ¾ÆÀÌÅÛ ³ª¿Ã È®·ü
};

struct DissolutionItems
{
	INT Itemnum[SIZE_MAX_DISSOLUTION_ITEMS];
	INT Itemcount[SIZE_MAX_DISSOLUTION_ITEMS];
};

typedef std::vector<tDissolutionItemInfo> vectorDissolutionItemInfo; // Åø¿¡¼­ »ç¿ë
typedef mt_map<INT, tDissolutionItemInfo> mtmapDissolutionItemInfo; // ¸Ê¿¡ ÀúÀå

// END 2010-08-31 by shcho&jskim ¾ÆÀÌÅÛ¿ëÇØ ½Ã½ºÅÛ - ¿ëÇØ ¾ÆÀÌÅÛ Á¤º¸ ±¸Á¶Ã¼ 

// start 2011-03-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡

// 2011-03-18 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÀÛ¾÷Áß Å¬¶ó¿Í ¼­¹ö °øÅëµÈ ÀÚ·áÇü ÅëÇÕ
typedef enum 
{
	DB_ITEM,
	DB_MONSTER_INFO,
	DB_MAPOBJECT,
	DB_RARE_ITEM,
	DB_BUILDINGNPC,
	DB_GUILD_MARK,
	DB_MAP_INFO,
	DB_MIXING_INFO,
	DB_MYSTERY_ITEM_DROP,
	// 2009-03-04 by bhsohn ·°Å° ¸Ó½Å ¼öÁ¤¾È
	DB_LUCKYMACHINE,
	// end 2009-03-04 by bhsohn ·°Å° ¸Ó½Å ¼öÁ¤¾È
	// 2010. 02. 11 by ckPark ¹ßµ¿·ù ÀåÂø¾ÆÀÌÅÛ
	DB_INVOKINGWEARITEM_DPNUM,			// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	DB_INVOKINGWEARITEM_DPNUM_BY_USE,	// 2009-09-09 ~ 2010-02-10 by dhjin, ÀÎÇÇ´ÏÆ¼ - ¹ßµ¿·ùÀåÂø¾ÆÀÌÅÛ
	// end 2010. 02. 11 by ckPark ¹ßµ¿·ù ÀåÂø¾ÆÀÌÅÛ

	// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ
	DB_BURNING_MAP,
	// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ

	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ
	DB_PET_BASEDATA,						// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê ±âº» Á¤º¸.
	DB_PET_LEVELDATA,						// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê ·¹º§¾÷ °ü·ÃÁ¤º¸.
	DB_PET_OPERATOR,						// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - ¿ÀÆÛ·¹ÀÌÅÍ Á¤º¸
	// End 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ
	DB_DISSOLUTIONITEM,					// 2010-08-31 by shcho&&jskim, ¿ëÇØ ½Ã½ºÅÛ ±¸Çö
	DB_MONSTER_MULTI_TARGET					// 2011-03-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡

} DB_TYPE;

// 2011. 03. 08 by jskim ÀÎÇÇ3Â÷ ±¸Çö
struct DATA_HEADER
{
	DB_TYPE nType;
	int nDataCount;
};
// end 2011. 03. 08 by jskim ÀÎÇÇ3Â÷ ±¸Çö

///////////////////////////////////////////////////////////////////////////////
// MonsterMultiTarget - Parameter Struct Á¤ÀÇ
// 2011-03-17 by hskim, ÀÎÇÇÆ¼Æ¼ 3Â÷ ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡
///////////////////////////////////////////////////////////////////////////////
struct MONSTER_MULTI_TARGET
{
	INT			MonsterIndex;						// ¸ó½ºÅÍ °íÀ¯ ¹øÈ£
	INT			PointIndex;							// Ãß°¡µÈ ¸ó½ºÅÍ Å¸°ÙÀÇ ¹øÈ£
	D3DXVECTOR3	TargetPosition;						// Ãß°¡µÈ ½ÇÁ¦ ÁÂÇ¥ (x,y,z)
};
typedef std::vector<MONSTER_MULTI_TARGET>	vectorMonsterMultiTarget;
typedef std::vector<MONSTER_MULTI_TARGET>::iterator vectoritMonsterMultiTarget;
// end 2011-03-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¸ó½ºÅÍ ¸ÖÆ¼ Å¸°ÙÆÃ ±â´É Ãß°¡

// 2010-11 by dhjin, ¾Æ¶ó¸®¿À Ã¤³Î¸µ ·Î±×ÀÎ.
typedef enum
{
	CONNECT_PUBLISHER_DEFAULT = 0,
	CONNECT_PUBLISHER_NHN_JPN = 1
} eCONNECT_PUBLISHER;

char* GetConnectPublisherName(eCONNECT_PUBLISHER i_eCONNECT_PUBLISHER);

// start 2011-05-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ¿¬Ãâ
///////////////////////////////////////////////////////////////////////////////
// ½Ã³×¸¶(DB:account, ti_Cinema) Ä¿½ºÅÒ ÀÎµ¦½º(CustomIdx) Á¤ÀÇ
// 2011-05-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ¿¬Ãâ
//
// ½Ã³×¸¶¿¡¼­ ¹ü¿ëÀûÀ¸·Î »ç¿ëÇÏÁö ¾ÊÀ» ±â´ÉÀº ¿©±â¿¡ Ãß°¡ ±¸ÇöÇÑ´Ù.
///////////////////////////////////////////////////////////////////////////////
enum
{
	INFINITY3_STEP5_MOVIE_START			= 1,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 5 - Àû±º ¸ð¼± Ä§¸ô ¿¬Ãâ ½ÃÀÛ
	INFINITY3_STEP5_MOVIE_END			= 2,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 5 - Àû±º ¸ð¼± Ä§¸ô ¿¬Ãâ Á¾·á
	INFINITY3_STEP5_TIMELIMIT_SET		= 3,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 6 - ¸ð¼± Å»Ãâ ½Ã°£ Àç¼³Á¤
	INFINITY3_STEP6_MOVE_SUCCESS_POSITION	= 4,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 7 - Ä³¸¯ÅÍ À§Ä¡ ÀÌµ¿ (¼º°ø) - ½Ã³×¸¶ ÆÄÀÏ Á¦ÀÛ½Ã »èÁ¦ ¿¹Á¤
	INFINITY3_STEP6_MOVE_FAIL_POSITION		= 5,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 7 - Ä³¸¯ÅÍ À§Ä¡ ÀÌµ¿ (½ÇÆÐ) - ½Ã³×¸¶ ÆÄÀÏ Á¦ÀÛ½Ã »èÁ¦ ¿¹Á¤
	INFINITY3_FAIL_MOVIE_START				= 6			// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ÇÆÐ½Ã - ¾Æ±º ¸ð¼± Ä§¸ô ¿¬Ãâ ½ÃÀÛ
};


///////////////////////////////////////////////////////////////////////////////
// MSG_FC_INFINITY_MOVIE ¸Þ½ÃÁö ¿¬Ãâ ¹øÈ£(UniqueNumber) Á¤ÀÇ
// 2011-05-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ¿¬Ãâ
///////////////////////////////////////////////////////////////////////////////
enum
{
	INFINITY_MOVIE_001_INF3_STEP5		= 1,		// ÀÎÇÇ´ÏÆ¼ 3Â÷ - Àû±º ¸ð¼± Ä§¸ô ¿¬Ãâ
	INFINITY_MOVIE_002_INF3_FAIL		= 2			// ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¾Æ±º ¸ð¼± Ä§¸ô ¿¬Ãâ
};

///////////////////////////////////////////////
// MSG_FC_INFINITY_MOVIE ¸Þ½ÃÁö Command Á¤ÀÇ
enum
{
	MOVIE_COMMAND_START					= 1,		// ¿¬Ãâ ½ÃÀÛ
	MOVIE_COMMAND_END					= 2			// ¿¬Ãâ Á¾·á
};
// end 2011-05-17 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½Ã³×¸¶ ¿¬Ãâ

///////////////////////////////////////////////////////////////////////////////
// WayPoint °ü·Ã - Parameter Struct ¹× Define Á¤ÀÇ
// 2011-05-23 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¿þÀÌÆ÷ÀÎÆ® ±¸Çö
///////////////////////////////////////////////////////////////////////////////

// SWAY_POINT.byPostionAttribute ¼Ó¼º
#define WAY_POINT_POSITION_ATTRIBUTE_RELATIVE			0			// »ó´ë ÁÂÇ¥						- ÃßÈÄ °³¹ß
#define WAY_POINT_POSITION_ATTRIBUTE_ABSOLUTE			1			// Àý´ë ÁÂÇ¥		- ±â´É °³¹ß

// CWayPointPattern.m_byCompletionAction ¼Ó¼º
#define WAY_POINT_COMPLETION_ACTION_REPEAT				0			// ¹Ýº¹
#define WAY_POINT_COMPLETION_ACTION_RECIPROCATION		1			// ¿Õº¹
#define WAY_POINT_COMPLETION_ACTION_STOP				2			// Á¤Áö

#define WAY_POINT_COLLISION_THRESHOLD					2000.0f		// WAY_POINT µµÂø ÆÇÁ¤¿ë Threshold °ª

struct SWAY_POINT
{
	UINT dwPatternIndex;				// ÆÐÅÏ ¹øÈ£
	UINT dwSequenceNum;					// ¿þÀÌÆ÷ÀÎÆ® ¼ø¼­
	BYTE byPostionAttribute;			// ¼Ó¼º				(Àý´ë ÁÂÇ¥, »ó´ë ÁÂÇ¥)								/ ±¸Çö »çÇ× : Àý´ë ÁÂÇ¥
	D3DXVECTOR3 vPoint;					// ÁÂÇ¥
	
	SWAY_POINT()
	{
		dwPatternIndex = 0;
		dwSequenceNum = 0;
		byPostionAttribute = WAY_POINT_POSITION_ATTRIBUTE_RELATIVE;
		vPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	SWAY_POINT(UINT i_dwPatternNum, UINT i_dwSequenceNum, BYTE i_byPostionAttribute, D3DXVECTOR3 i_vPoint)
	{
		dwPatternIndex = i_dwPatternNum;
		dwSequenceNum = i_dwSequenceNum;
		byPostionAttribute = i_byPostionAttribute;
		vPoint = i_vPoint;
	}
};

struct SWAY_POINT_MOVE
{
	UINT dwIndex;						// ÆÐÅÏ ¹øÈ£
	BYTE byCompletionAction;			// ¿Ï·á ÈÄ µ¿ÀÛ		(¹Ýº¹, ¿Õº¹, Á¤Áö)
};

typedef vector<SWAY_POINT>	vectorWayPoint;
typedef vector<SWAY_POINT_MOVE>	vectorWayPointMove;

struct SSORT_WAY_POINT_BY_SEQUENCE_NUMBER : binary_function<SWAY_POINT, SWAY_POINT, bool>
{
	bool operator()(SWAY_POINT Param1, SWAY_POINT Param2)
	{
        return Param1.dwSequenceNum < Param2.dwSequenceNum;		// ¿À¸§ Â÷¼ø Á¤·Ä
    };
};
// end 2011-05-23 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ¿þÀÌÆ÷ÀÎÆ® ±¸Çö

// start 2011-05-30 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÇÃ·¹ÀÌ ½Ã°£ Àç¼³Á¤ ±â´É
#define INFINITY3_STEP6_TIMELIMIT_VALUE			60000		// (1 min * 60 sec * 1000 ms)
// end 2011-05-30 by hskim, ÀÎÇÇ´ÏÆ¼ 3Â÷ - ÇÃ·¹ÀÌ ½Ã°£ Àç¼³Á¤ ±â´É

// start 2011-06-01 ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 7 - Ä³¸¯ÅÍ À§Ä¡ ÀÌµ¿
#define INFINITY3_STEP6_MOVE_SUCCESS_POSTION_X			500.0f
#define INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Y			2500.0f
#define INFINITY3_STEP6_MOVE_SUCCESS_POSTION_Z			3500.0f

#define INFINITY3_STEP6_MOVE_FAIL_POSTION_X				9800.0f
#define INFINITY3_STEP6_MOVE_FAIL_POSTION_Y				3500.0f
#define INFINITY3_STEP6_MOVE_FAIL_POSTION_Z				5000.0f
// end 2011-06-01 ÀÎÇÇ´ÏÆ¼ 3Â÷ - ½ºÅÜ 7 - Ä³¸¯ÅÍ À§Ä¡ ÀÌµ¿

// start 2011-07-15 by shcho - Ä³½¬¾ÆÀÌÅÛ Ã¼Å© ½Ã½ºÅÛ ±¸Çö
struct cashItemCheckData
{
	int ItemNumber;
	int	Price;
	int	CashPrice;
	int	Time;
	int ItemAttribute;
	int UniqueNumber;
};

typedef vector<cashItemCheckData> vectCashCheckData;

#define OLD_DATA	0
#define NEW_DATA	1
// end 2011-07-15 by shcho - Ä³½¬¾ÆÀÌÅÛ Ã¼Å© ½Ã½ºÅÛ ±¸Çö

// START 2011-11-03 by shcho, yedang ¼Ë´Ù¿îÁ¦ ±¸Çö - ¼Ë´Ù¿îÁ¦¿¡ Ã¼Å©µÉ ³ªÀÌ
struct ShutdownUserData
{ // Ãß°¡µÇ Á¤º¸°¡ ÀÖÀ¸¹Ç·Î ÀÏ´Ü ¸¸µé¾î µÎÀÚ
	UID32_t			Account_UniqueNumber;
	UID32_t			CharacterUniqueNumber[3];
};
typedef vector<ShutdownUserData> vectShutDownUserData;

#define SHUTDOWNMINORS_YEARS_OLD	16
// END 2011-11-03 by shcho, yedang ¼Ë´Ù¿îÁ¦ ±¸Çö - ¼Ë´Ù¿îÁ¦¿¡ Ã¼Å©µÉ ³ªÀÌ

// 2011-11-18 by shcho, ¼­¹ö´Ù¿î ÇÁ¸®Æä¾î¼­¹ö´Ù¿î Á¦°Å Ã³¸® - Ã¼Å© ÇÔ¼ö Ãß°¡
BOOL CheckAdminCommand(char *token);
		   
char *StrCaseStr(const char *s, const char *find);		// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡ - RawData Àü¼Û ±â´É Ãß°¡
char *StrRCaseStr(const char *s, const char *find);		// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡ - RawData Àü¼Û ±â´É Ãß°¡
// start 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³
struct STRIGGER_MAP_BUFF
{
	MapIndex_t MapIndex;		// Àû¿ë ¸Ê
	ItemNum_t ItemNum;		// Àû¿ë ¹öÇÁ (ItemNum : ½ºÅ³ ¹øÈ£)

	STRIGGER_MAP_BUFF()
	{
		MapIndex = 0;
		ItemNum = 0;
	}

	STRIGGER_MAP_BUFF(MapIndex_t i_MapIndex, ItemNum_t i_ItemNum)
	{
		MapIndex = i_MapIndex;
		ItemNum = i_ItemNum;
	}
};
typedef vector<STRIGGER_MAP_BUFF>	vectorTriggerMapBuff; //³­ÀÌµµ ¸®½ºÆ® º¤ÅÍ
// end 2011-10-10 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - È­»êÀç / ¸ð·¡ ÆøÇ³

// start 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

// Æ®¸®°Å - NGC°ÅÁ¡Àü

#define STRIGEER_NGCINFLWAR_MAPINDEX_1ST		4101
#define STRIGEER_NGCINFLWAR_MAPINDEX_2ND		4301
#define STRIGEER_NGCINFLWAR_WW2_EVENT_MAPINDEX_BCU		9021		// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - 2Â÷ ¼¼°è´ëÀü BCU ÀÌº¥Æ®¸Ê
#define STRIGEER_NGCINFLWAR_WW2_EVENT_MAPINDEX_ANI		9022		// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - 2Â÷ ¼¼°è´ëÀü ANI ÀÌº¥Æ®¸Ê
#define STRIGEER_NGCINFLWAR_WW2_EVENT_ITEM_NUM			COLLECTION_XAM_ENCHANT_RETURN_ITEMNUM			// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - º¸»ó ¾ÆÀÌÅÛ ¹øÈ£
#define STRIGEER_NGCINFLWAR_WW2_EVENT_ITEM_COUNT		1			// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - º¸»ó ¾ÆÀÌÅÛ °¹¼ö
#define STRIGEER_NGCINFLWAR_WW2_EVENT_TRIGGER_FUNID_BCU		6000	// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - 2Â÷ ¼¼°è´ëÀü BCU Æ®¸®°Å ID
#define STRIGEER_NGCINFLWAR_WW2_EVENT_TRIGGER_FUNID_ANI		7000	// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - 2Â÷ ¼¼°è´ëÀü ANI Æ®¸®°Å ID

#define STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_UID			7035110
#define STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_COUNT			1
#define STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_UID			7035120
#define STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_COUNT		1
#define STRIGEER_NGCINFLWAR_EVENT_2ND_ALL_ITEM_UID					7035130
#define STRIGEER_NGCINFLWAR_EVENT_2ND_ALL_ITEM_COUNT				1

#ifdef _DEBUG
#define STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_PAY_RANK		2		// Å×½ºÆ® ¿ë ¼³Á¤
#define STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_PAY_RANK	4
#else
#define STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_PAY_RANK		5		// ½ÇÁ¦ °ÔÀÓ ¿ë ¼³Á¤
#define STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_PAY_RANK	15
#endif

// Æ®¸®°Å - Å©¸®½ºÅ»

#define STRIGEER_CRYSTAL_MAPINDEX_1ST			4100
#define STRIGEER_CRYSTAL_MAPINDEX_2ND			4300

#ifdef _DEBUG
#define STRIGGER_CRYSTAL_NEXT_CREATE_TIME		10		// ºÐ
#else
#define STRIGGER_CRYSTAL_NEXT_CREATE_TIME		60		// ºÐ
#define STRIGGER_CRYSTAL_NEXT_EVENT_1ST			20		// ºÐ
#define STRIGGER_CRYSTAL_NEXT_EVENT_2ND			45		// ºÐ
#endif

#define STRIGEER_CRYSTAL_EVENT_NONE						0
#define STRIGEER_CRYSTAL_EVENT_HELL_WARP				1
#define STRIGGER_CRYSTAL_EVENT_SUMMON_BOSS_MONSTER		2
#define STRIGGER_CRYSTAL_EVENT_SHOP_KIT					3
#define STRIGGER_CRYSTAL_EVENT_EXP_100					4
#define STRIGGER_CRYSTAL_EVENT_SHOP_HYPER_CARD			5
#define STRIGGER_CRYSTAL_EVENT_SHOP_SEAL				6
#define STRIGGER_CRYSTAL_EVENT_DROP_100					7
#define STRIGGER_CRYSTAL_EVENT_EXP_SPI_DROP_100			8

#define STRIGGER_CRYSTAL_EVENT_2ND_WARPOINT				21
#define STRIGGER_CRYSTAL_EVENT_2ND_SUMMON_BOSS_MONSTER	22
#define STRIGGER_CRYSTAL_EVENT_2ND_EXP_150				23
#define STRIGGER_CRYSTAL_EVENT_2ND_DROP_150				24
#define STRIGGER_CRYSTAL_EVENT_2ND_EXP_SPI_DROP_150		25
#define STRIGGER_CRYSTAL_EVENT_2ND_CAPSULE_ITEM			26
#define STRIGGER_CRYSTAL_EVENT_2ND_SHOP_PREFIX_SUFFIX	27
#define STRIGGER_CRYSTAL_EVENT_2ND_SHOP_KIT				28
#define STRIGGER_CRYSTAL_EVENT_2ND_SHOP_HYPER_CARD		29
#define STRIGGER_CRYSTAL_EVENT_2ND_SHOP_WEAPON			30
#define STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP			31

#define STRIGGER_CRYSTAL_EVENT_VALUE_SUMMON_BOSS_MONSTER_UID					2111500

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_TOTAL_COUNT		6
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_01				2114600
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_02				2114700
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_03				2114800
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_04				2114900
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_05				2115000
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_06				2115100

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_COUNT				1
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_RANDOM_POS			0

#define STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX				9369
#define STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX		9370
#define STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX				9371

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_ITEM_BUILDING_INDEX	9372
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX			9373
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX	9374
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX			9375

///////////////////////////////////////////////////////////////////////////////////////
// ÀÌ¸§							±âÁ¸ ¿ÀºêÁ§Æ®	±³Ã¼ ¿ÀºêÁ§Æ®	EventwParam1
///////////////////////////////////////////////////////////////////////////////////////
// È­»ê¸Ê »óÁ¡ 1				3202000			93202000		201
// È­»ê¸Ê »óÁ¡ 2				3202100			93202100		202
// È­»ê¸Ê »óÁ¡ 3				3202200			93202200		203
//
// Áö¿Á¸Ê ¿öÇÁ Ãæµ¹¿ÀºêÁ§Æ® #1	3202600			93202600		9857		<-- ¿ä±â ¿ø·¡ 9856 ÀÌ³ª 9857 ±³Ã¼
// Áö¿Á¸Ê ¿öÇÁ Ãæµ¹¿ÀºêÁ§Æ® #2	3202700			93202700		9856		<-- ¿ä±â ¿ø·¡ 9857 ÀÌ³ª 9856 ±³Ã¼ 
// Áö¿Á¸Ê ¿öÇÁ ¿ÀºêÁ§Æ®			3202800			93202800		401
///////////////////////////////////////////////////////////////////////////////////////

#define STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME									0			// ¼¥ ¸®¼Â½Ã ÃÊ±â ±¸¸Å ¼ýÀÚ (È¤½Ã ¸ð¸¦ »óÈ²À» À§ÇØ 0À¸·Î ÃÊ±âÈ­)

#define STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_IDX						201
#define STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_NUM_DEFAULT				3202000
#define STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_NUM_CHANGE					93202000

#define STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_IDX					202
#define STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_DEFAULT			3202100
#define STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_CHANGE			93202100

#define STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_IDX						203
#define STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_NUM_DEFAULT				3202200
#define STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_NUM_CHANGE				93202200

#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_IDX							9857
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_NUM_DEFAULT					3202600
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_NUM_CHANGE						93202600

#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_IDX							9856
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_NUM_DEFAULT					3202700
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_NUM_CHANGE					93202700

#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_IDX							401
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_NUM_DEFAULT					3202800
#define STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_NUM_CHANGE					93202800

#define STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SALES_VOLUME							100
#define STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SALES_VOLUME					5
#define STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SALES_VOLUME							1
#define STRIGGER_CRYSTAL_EVENT_VALUE_PREFIX_SUFFIX_VOLUME						3

#define STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME						60			// ´ÜÀ§ : ºÐ 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_EXP							1.0f		// ´ÜÀ§ : 100.0 ÆÛ¼¾Æ®
#define STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_SPI							1.0f		// ´ÜÀ§ : 100.0 ÆÛ¼¾Æ®
#define STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_DROP_ITEM					1.0f		// ´ÜÀ§ : 100.0 ÆÛ¼¾Æ®

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME					60			// ´ÜÀ§ : ºÐ 
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_EXP						1.5f		// ´ÜÀ§ : 150.0 ÆÛ¼¾Æ®
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_SPI						1.5f		// ´ÜÀ§ : 150.0 ÆÛ¼¾Æ®
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_DROP_ITEM				1.5f		// ´ÜÀ§ : 150.0 ÆÛ¼¾Æ®

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_1ST							4400		// Áö¿Á¸Ê Ã¹¹øÂ°
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_2ND							4401		// Áö¿Á¸Ê µÎ¹øÂ°

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_1ST						4402		// Áö¿Á¸Ê2 Ã¹¹øÂ°
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_2ND						4403		// Áö¿Á¸Ê2 µÎ¹øÂ°

// 2013-03-21 by bckim, Áö¿Á¸Ê ºÎÈ°Ä«µå »ç¿ë½Ã ¹ö±× ¼öÁ¤ 
#define IS_CRYSTAL_EVENT_MAP( currentMapNum )	((currentMapNum == STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_1ST) || \
												(currentMapNum == STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_2ND) || \
												(currentMapNum == STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_1ST) || \
												(currentMapNum == STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_2ND))
// End. 2013-03-21 by bckim, Áö¿Á¸Ê ºÎÈ°Ä«µå »ç¿ë½Ã ¹ö±× ¼öÁ¤ 

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_01			2110200 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_02			2110300 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_03			2110400 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_04			2110500 

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT			10

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_05			2109700  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_06			2109800  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_07			2109900  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_08			2110000  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_09			2110100  

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT		1

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_01			2111000 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_02			2111100 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_03			2111200 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_04			2111300 
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_05			2111400 

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT			10

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_06			2110600  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_07			2110700  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_08			2110800  
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_09			2110900  

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT		1

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS			800

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_REWARD_ITEM_UID				7033570

#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_ENTER_CONDITION_ITEM_UID_01	7033570
#define STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_ENTER_CONDITION_ITEM_UID_02	7033580

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_WAYPOINT						400

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_CAPSULE_ITEM_UID				7004540
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_CAPSULE_ITEM_COUNT				2

///////////////////////////////////////////////////////////////////////////////////////
// ÀÌ¸§								±âÁ¸ ¿ÀºêÁ§Æ®	±³Ã¼ ¿ÀºêÁ§Æ®	EventwParam1	EventType
///////////////////////////////////////////////////////////////////////////////////////
// ÈÞ¾çÁö¸Ê »óÁ¡ 1					3202000			93202000		401				9			9372	ÈÞÇâÁö ¸Ê »óÁ¡(Á¢µÎ/Á¢¹Ì »óÁ¡)
// ÈÞ¾çÁö¸Ê »óÁ¡ 2					3202100			93202100		402				9			9373	ÈÞÇâÁö ¸Ê »óÁ¡(S±Þ Å°Æ®)
// ÈÞ¾çÁö¸Ê »óÁ¡ 3					3202200			93202200		403				9			9374	ÈÞÇâÁö ¸Ê »óÁ¡(ÇÏÀÌÆÛ Ä«µå)
// ÈÞ¾çÁö¸Ê »óÁ¡ 4					3207500			93207500		404				9			9375	ÈÞÇâÁö ¸Ê »óÁ¡(100·¹º§ ÀÌ»ó ¹«±â)

// ÈÞ¾çÁö¸Ê ¿öÇÁ Ãæµ¹¿ÀºêÁ§Æ® #1	3207900			93207900		7142			8			<-- ¿ä±â ¿ø·¡ 7141 ÀÌ³ª 7142 ±³Ã¼
// ÈÞ¾çÁö¸Ê ¿öÇÁ Ãæµ¹¿ÀºêÁ§Æ® #2	3208000			93208000		7141			1			<-- ¿ä±â ¿ø·¡ 7142 ÀÌ³ª 7141 ±³Ã¼
// ÈÞ¾çÁö¸Ê ¿öÇÁ ¿ÀºêÁ§Æ®			3202800			93202800		411				6
///////////////////////////////////////////////////////////////////////////////////////

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_IDX			401
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_NUM_DEFAULT	3202000
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_NUM_CHANGE	93202000

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_IDX					402
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_NUM_DEFAULT			3202100
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_NUM_CHANGE				93202100

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_IDX				403
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_DEFAULT		3202200
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_CHANGE		93202200

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_IDX					404
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_NUM_DEFAULT			3207500
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_NUM_CHANGE			93207500

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_IDX						7142
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_NUM_DEFAULT				3207900
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_NUM_CHANGE					93207900

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_IDX						7141
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_NUM_DEFAULT				3208000
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_NUM_CHANGE				93208000

#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_IDX						411
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_NUM_DEFAULT				3208100
#define STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_NUM_CHANGE				93208100

// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ
#define STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX			401
#define STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT	3218000			// (»¡°­ ¿ÀÇÁÁ§Æ®) º¸È£¸· ¾ø´Â °Å
#define STRIGGER_CRYSTAL_EVENT_OUTPOST_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE		93218000		// (ÆÄ¶õ ¿ÀºêÁ§Æ®) º¸È£¸· ÀÖ´Â °Å
// end 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ

// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå
#define STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX			401
#define STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT	3225600			// (»¡°­ ¿ÀÇÁÁ§Æ®) º¸È£¸· ¾ø´Â °Å
#define STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE	93225600		// (ÆÄ¶õ ¿ÀºêÁ§Æ®) º¸È£¸· ÀÖ´Â °Å
// end 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå

struct STRIGGER_MAP
{
	MapTriggerID_t	MapTriggerID;		// ¸Ê Æ®¸®°Å ID
	MapIndex_t		MapIndex;			// Àû¿ë ¸Ê ¹øÈ£
	ChannelIndex_t	MapChannel;			// Àû¿ë ¸Ê Ã¤³Î
	FunctionID_t	FunctionID;			// Àû¿ë ±â´É Á¾·ù
};
typedef vector<STRIGGER_MAP>	vectorTriggerMap;

struct STRIGGER_FUNCTION_CRYSTAL
{
	FunctionID_t		FunctionID;
	CrystalGroupID_t	CrystalGroupID;
	INT					PeriodTime;				// ºÐ
	BYTE				RandomSequence;			// 0 = Å©¸®½ºÅ» ¼ø¼­ Á¤ÇØ´ë·Î , 1 = Å©¸®½ºÅ» ¼ø¼­ ·¥´ý
};
typedef vector<STRIGGER_FUNCTION_CRYSTAL> vectorTriggerFunctionCrystal;

struct STRIGGER_CRYSTAL_GROUP
{
	CrystalGroupID_t	CrystalGroupID;
	DestroyGroupID_t	DestroyGroupID;
	EventID_t			EventID;
};
typedef vector<STRIGGER_CRYSTAL_GROUP> vectorTriggerCrystalGroup;

struct STRIGGER_CRYSTAL_DESTROY_GROUP
{
	DestroyGroupID_t	DestroyGroupID;
	SequenceNumber_t	SequenceNumber;
	MonIdx_t			TargetMonster;

	void operator= (const STRIGGER_CRYSTAL_DESTROY_GROUP &rCrystalDestoryGroup)
	{
		memcpy(this ,&rCrystalDestoryGroup , sizeof(STRIGGER_CRYSTAL_DESTROY_GROUP));
	}
};
typedef vector<STRIGGER_CRYSTAL_DESTROY_GROUP> vectorTriggerCrystalDestroyGroup;

struct STRIGGER_ATTACK_USER_DAMAGE
{
	MonIdx_t	MonsterIdx;
	UID32_t		CharacterUniqueNumber;
	UID32_t		GuildUniqueNumber;
	BYTE		InfluenceType;
	float		fDamage;
};

struct SSORT_TRIGGER_ATTACK_USER_DAMAGE_BY__DAMAGE : binary_function<STRIGGER_ATTACK_USER_DAMAGE, STRIGGER_ATTACK_USER_DAMAGE, bool>
{
	bool operator()(STRIGGER_ATTACK_USER_DAMAGE Param1, STRIGGER_ATTACK_USER_DAMAGE Param2)
	{
        return Param1.fDamage > Param2.fDamage;		// ³»¸² Â÷¼ø Á¤·Ä
    };
};

typedef	mt_vector<STRIGGER_ATTACK_USER_DAMAGE> mtvectTriggerAttackUserDamage;
typedef	vector<MonIdx_t> vectorDestroyedSequence;		// mtvectTriggerCrystalDamage »ç¿ëÇÏ¿© µ¿±âÈ­

// ¸ðµç ¸ó½ºÅÍ »ç¸Á½Ã ¾ÆÀÌÅÛ Áö±Þ
struct STRIGGER_ALL_MONSTER_DEAD_ITEM_PAY		
{
	BOOL		Enable;			// »ç¿ë ¿©ºÎ
	TimeUnit_t	TimeLimit;		// Áö±Þ ÇÑµµ Á¦ÇÑ ½Ã°£
	ItemNum_t	ItemNum;		// ¾ÆÀÌÅÛ º¸»ó
};
// end 2011-10-28 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - Å©¸®½ºÅ» ½Ã½ºÅÛ

// start 2011-11-07 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - NCG °ÅÁ¡Àü ½Ã½ºÅÛ
struct STRIGGER_FUNCTION_NGC_INFLWAR
{
	FunctionID_t		FunctionID;
	NGCInflWarGroupID_t	NGCInflWarGroupID;
	INT					PeriodTime;				// ºÐ
	MonIdx_t			BossMonster;
	INT					WinInfluenceWP;
	INT					LossInfluenceWP;
	INT					FirstDamageGuildWP;
	INT					FixedTimeType;			// Æ¯Á¤ ½Ã°£ »çÀÌ¿¡ »ý¼ºµÇ´Â °æ¿ì Å¸ÀÔ
	INT					DestroyCrystalcount;	// 2013-07-08 by jhseol, Æ®¸®°Å ½Ã½ºÅÛ È®Àå - ÆÄ±«ÇØ¾ßÇÒ Å©¸®½ºÅ»ÀÇ ¼ö
};
typedef vector<STRIGGER_FUNCTION_NGC_INFLWAR> vectorTriggerFunctionNGCInflWar;

// 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ
struct STRIGGER_FUNCTION_NGC_OUTPOST
{
	FunctionID_t				FunctionID;					// Æ®¸®°Å ID
	SummonMosterGroupID_t		SummonMonsterGroupID;		// ¼ÒÈ¯ ¸ó½ºÅÍ ±×·ì ID
	MapIndex_t					StandardOutPostMap;			// ±âÁØÀÌ µÉ ÀüÁø±âÁö ¸Ê
	MonIdx_t					BossMonster;
	INT							WinInfluenceWP;
	INT							LossInfluenceWP;
	MapIndex_t					NextOutPostMap;				// ´ÙÀ½ ÀüÁø±âÁö ¸Ê
	INT							DestroyCrystalcount;		// ÆÄ±«ÇØ¾ßÇÒ Å©¸®½ºÅ»ÀÇ ¼ö
};
typedef vector<STRIGGER_FUNCTION_NGC_OUTPOST> vectorTriggerFunctionNGCOutPost;

struct STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO
{
	MonIdx_t					BossMonster;
	MonIdx_t					CrystalNum;				// Å©¸®½ºÅ» ¹øÈ£
	INT							RegenMinTimeMinutes;	// ¸®Á¨ ÃÖ¼Ò ½Ã°£(ºÐ)
	INT							RegenMaxTimeMinutes;	// ¸®Á¨ ÃÖ´ë ½Ã°£(ºÐ)
	BOOL						IsDestroy;				// ÆÄ±«¿©ºÎ
	ATUM_DATE_TIME				DestroyTime;			// ÆÄ±«½Ã°£
};
typedef vector<STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO> vectorTriggerOutPostBossKillInfo;
typedef mt_vector<STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO> mtvectorTriggerOutPostMonsterKillInfo;

struct STRIGGER_CRYSTAL_BUFF
{
	FunctionID_t				FunctionID;				// Æ®¸®°Å ID
	INT							SkillItemNum;			// ½ºÅ³ ¹øÈ£
};
typedef vector<STRIGGER_CRYSTAL_BUFF> vectorTriggerCrystalBuff;
// end 2013-01-21 by jhseol, NGC ÀüÀü±âÁö Æ®¸®°Å ½Ã½ºÅÛ

struct STRIGGER_NGC_INFLWAR_MONSTER_GROUP
{
	NGCInflWarGroupID_t	NGCInflWarGroupID;
	MonIdx_t			Monster;
	INT					SummonCount;
	INT					SummonRandomPos;
};
typedef vector<STRIGGER_NGC_INFLWAR_MONSTER_GROUP> vectorTriggerNGCInflWarMonsterGroup;

typedef enum
{
	MAP_ENTRANCE_CONDITION_OPTION_NONE				= 0,		// ¾øÀ½
	MAP_ENTRANCE_CONDITION_OPTION_DELETE			= 1,		// »èÁ¦
} eMAP_ENTRANCE_CONDITION_OPTION;

struct SMAP_ENTRANCE_CONDITION
{
	ItemNum_t						ItemNum;			// ¾ÆÀÌÅÛ ¹øÈ£
	INT								RequireCount;		// ¿ä±¸ ¼ýÀÚ
	eMAP_ENTRANCE_CONDITION_OPTION	Option;				// »èÁ¦ ¿©ºÎ
};
typedef vector<SMAP_ENTRANCE_CONDITION> vectorMapEntranceCondition;
// end 2011-11-07 by hskim, EP4 [Æ®¸®°Å ½Ã½ºÅÛ] - NCG °ÅÁ¡Àü ½Ã½ºÅÛ

// start 2011-12-21 by hskim, EP4 [µ¿¿µ»ó 1È¸ Àç»ý]
typedef enum
{
	SYSTEM_EVENT_OPENING_MOVIE		= 0

} eSYSTEM_EVENT;
// end 2011-12-21 by hskim, EP4 [µ¿¿µ»ó 1È¸ Àç»ý]

// start 2012-03-13 by hskim, ÇöÁöÈ­ °ü·Ã DB ±¸Á¶ º¯°æ
struct SOVERLAP_ITEM
{
	ItemNum_t	ItemNum;
	INT			CashPrice;
	INT			Tab;
	INT			ItemAttribute;
};
typedef vector<SOVERLAP_ITEM> vectorOverlapItem;
// end 2012-03-13 by hskim, ÇöÁöÈ­ °ü·Ã DB ±¸Á¶ º¯°æ

// start 2012-03-05 by hskim, µå¶ø ¾ÆÀÌÅÛ ÀÏÁ¤ ½Ã°£ÈÄ »èÁ¦
struct SDELETE_DROP_ITEM
{
	UINT				ItemFieldIndex;
	D3DXVECTOR3			Position;
};
typedef vector<SDELETE_DROP_ITEM>	vectorDeleteDropItem;		
// end 2012-03-05 by hskim, µå¶ø ¾ÆÀÌÅÛ ÀÏÁ¤ ½Ã°£ÈÄ »èÁ¦

// 2012-06-08 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ÄÉ¸¯ÅÍ : Å¸ÀÔ º¹»ç
#define CHARACTER_B_ACC_NAME "MSAce_34122_B_Copy1"
#define CHARACTER_M_ACC_NAME "MSAce_34122_M_Copy1"
#define CHARACTER_I_ACC_NAME "MSAce_34122_I_Copy1"
#define CHARACTER_A_ACC_NAME "MSAce_34122_A_Copy1"

#define CHARACTER_TYPE_B_ATT "MSAce34122BAtt"
#define CHARACTER_TYPE_B_DEF "MSAce34122BDef"
#define CHARACTER_TYPE_B_DOD "MSAce34122BDod"

#define CHARACTER_TYPE_M_ATT "MSAce34122MAtt"
#define CHARACTER_TYPE_M_DEF "MSAce34122MDef"
#define CHARACTER_TYPE_M_DOD "MSAce34122MDod"

#define CHARACTER_TYPE_A_ATT "MSAce34122AAtt"
#define CHARACTER_TYPE_A_DEF "MSAce34122ADef"
#define CHARACTER_TYPE_A_DOD "MSAce34122ADod"

#define CHARACTER_TYPE_I_ATT "MSAce34122IAtt"
#define CHARACTER_TYPE_I_DEF "MSAce34122IDef"
#define CHARACTER_TYPE_I_DOD "MSAce34122IDod"
// end 2012-06-08 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ÄÉ¸¯ÅÍ : Å¸ÀÔ º¹»ç

// 2012-06-21 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ¾Æ·¹³ª Àü¿ë Äü½½·Ô ( 0 = ¸ÞÀÎ, 1 = ¾Æ·¹³ª )
#define SLOT_PURPOSE_MAIN	0
#define SLOT_PURPOSE_ARENA	1
// end 2012-06-21 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - ¾Æ·¹³ª Àü¿ë Äü½½·Ô ( 0 = ¸ÞÀÎ, 1 = ¾Æ·¹³ª )

#ifdef S_BONUSEXPSYSTEM_RENEWAL
// 2012-10-08 by jhjang ÇØÇÇ¾Æ¿ö °æÇèÄ¡ ¸®´º¾ó
#define KILLCOUNT_MAX		300
#define KILLCOUNT_ITEMNUM	7038810
// end 2012-10-08 by jhjang ÇØÇÇ¾Æ¿ö °æÇèÄ¡ ¸®´º¾ó
#endif // S_BONUSEXPSYSTEM_RENEWAL

///////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü
#define RenewalStrategyPointWeekDay					7
#define RenewalStrategyPointSummonTimeTermMin		1800 // 30ºÐ*60ÃÊ

struct SRENEWAL_STRATEGYPOINT_SUMMON_TIME
{//¿äÀÏº° °Å°ËÀü ¼³Á¤ ½Ã°£ ¹× ¼¼·Âº° ¼ÒÈ¯ °ÅÁ¡ ¼ö
	SHORT			DayOfWeek;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	BYTE			CountBCU;
	BYTE			CountANI;
};
typedef vector<SRENEWAL_STRATEGYPOINT_SUMMON_TIME>	vectSRENEWAL_STRATEGYPOINT_SUMMON_TIME;

struct SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO
{//°ÅÁ¡ ¸Ê º¸À¯¼¼·Â Á¤º¸
	MapIndex_t		MapIndex;
	char			MapName[SIZE_MAX_MAP_NAME];
	int				Influence;
};
typedef vector<SSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO>	vectSSTRATEGYPOINT_MAP_INFLUENCESTRATEGYPOINT_INFO;
// end 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü

#define OUTPOST_BUFF_SKILL_AKRON               7993910
#define OUTPOST_BUFF_SKILL_RAKION              7993920
#define OUTPOST_BUFF_SKILL_CORONADO            7993930
#define OUTPOST_BUFF_SKILL_NGC_MILITARY_BASE   7993940

// 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_2ND	7841051
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_3RD	7841061
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_4TH	7841071
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_5TH	7841081
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_6TH	7841091
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_7TH	7841101
#define CONSECUTIVE_VICTORIES_BUFF_SKILL_8TH	7841111

#define TURN_AROUND_BUFF_SKILL_1ST				7841121		
#define TURN_AROUND_BUFF_SKILL_2ND				7841122

#define TURN_AROUND_BUFF_SKILL_1ST_THRESHOLD	7000
#define TURN_AROUND_BUFF_SKILL_2ND_THRESHOLD	8000
#define TURN_AROUND_BUFF_SKILL_NEW_THRESHOLD	4000		// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó - ¹ö½º¸¦ ºÎ¿©ÇÏ´Â ¼¼·ÂÆ÷ÀÎÆ® Â÷ÀÌ
#define TURN_AROUND_BUFF_SKILL_MAXIMUM_POINT_GAP	20000	// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó - ¹öÇÁ Àû¿ë ÃÖ´ë ¼¼·ÂÆ÷ÀÎÆ®Â÷ 2¸¸Á¡, ¹öÇÁ ºñÀ²·Î È¯»êÇÏ¸é [0.75 *(20000/100) = 150%]
#define TURN_AROUND_BUFF_SKILL_100P_VALUE			0.0075f	// 2013-08-01 by jhseol, ¿ªÀüÀÇ ¹öÇÁ ¸®´º¾ó - ¼¼·ÂÆ÷ÀÎÆ® 100Æ÷ÀÎÆ®´ç ¹öÇÁ Áõ°¡ Æø
// end 2013-05-09 by hskim, ¼¼·Â Æ÷ÀÎÆ® °³¼±

// 2013-05-20 by hskim, [º¸¾È ½Ã½ºÅÛ] ºñÁ¤»óÀûÀÎ ¹æ¹ýÀ¸·Î ¾ÆÀÌÅÛ Ãß°¡ ¹æÁö
#define TEMPORARY_SYSTEM_INFOMATION_INDEX_ITEMSECURITY	2
// end 2013-05-20 by hskim, [º¸¾È ½Ã½ºÅÛ] ºñÁ¤»óÀûÀÎ ¹æ¹ýÀ¸·Î ¾ÆÀÌÅÛ Ãß°¡ ¹æÁö


// 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ±¸Á¶Ã¤ ¼±¾ð
struct COLLECTION_INFO
{
	INT				CollectionType;		// ÄÃ·º¼Ç Å¸ÀÔ (1 : ¾Æ¸Ó)
	UID32_t			AccountUID;			// °èÁ¤ UID
	UID32_t			CharacterUID;		// Ä³¸¯ÅÍ UID
	ItemNum_t		ShapeNum;			// Àû¿ëÇÒ ¿ÜÇüÀÇ ¾ÆÀÌÅÛ ¹øÈ£
	ItemNum_t		ShapeItemNum;		// ¿Üº¯Å¶ ¾ÆÀÌÅÛ ¹øÈ£
	BYTE			EnchantLevel;		// °­È­ ´Ü°è
	UINT			RemainSeconds;		// ³²Àº ½Ã°£(ÃÊ) : ÃÖ´ë 136³â
	ATUM_DATE_TIME	EndTime;			// ÇöÁ¦ ½Ã°£¿¡¼­ ³²Àº ½Ã°£À» ´õÇÏ¿© »êÃâµÈ ¿ÜÇüÀÇ Àý´ë½Ã°£
	INT				ActivedCount;		// ¿ÜÇüÀÌ Àû¿ëµÈ ¾ÆÀÌÅÛÀÇ ¼ö (0:Àû¿ëµÈ ¾Æ¸Ó°¡ ¾øÀ½=»ç¿ëÁßÀÌ ¾Æ´Ô, 1ÀÌ»ó »ç¿ëÁß)

	COLLECTION_INFO()
	{
		MEMSET_ZERO(this, sizeof(COLLECTION_INFO));
	}
};
typedef vector<COLLECTION_INFO>	COLLECTION_LIST;

struct COLLECTION_SHAPE_CHANGE
{
	INT				CollectionType;		// ÄÃ·º¼Ç Å¸ÀÔ (1 : ¾Æ¸Ó)
	ItemUID_t		TargetItemUID;		// ¿ÜÇüº¯°æÀ» Àû¿ë¹ÞÀ» ¾ÆÀÌÅÛUID
	ItemNum_t		ShapeNum;			// ¿ÜÇü ¹øÈ£(¼Ò½ºÀÎµ¦½º)

	COLLECTION_SHAPE_CHANGE()
	{
		MEMSET_ZERO(this, sizeof(COLLECTION_SHAPE_CHANGE));
	}
};
// end 2013-05-31 by jhseol,bckim ¾Æ¸Ó ÄÃ·º¼Ç - ±¸Á¶Ã¤ ¼±¾ð
#endif
