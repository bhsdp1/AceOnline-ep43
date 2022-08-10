#pragma once
// 2015-06-12 Future, Kill Count System Properties
#ifdef S_BONUS_KILL_SYSTEM_RENEWAL
// The Amount of Kills needed to reward the bonus Items
#define KILLCOUNT_MAX_BG		100
#define KILLCOUNT_MAX_MG		50
#define KILLCOUNT_MAX_IG		75
#define KILLCOUNT_MAX_AG		100

#define KILLCOUNT_MAX(GearType)		(IS_BGEAR(GearType) ? KILLCOUNT_MAX_BG : IS_MGEAR(GearType) ? KILLCOUNT_MAX_MG : IS_IGEAR(GearType) ? KILLCOUNT_MAX_IG : IS_AGEAR(GearType) ? KILLCOUNT_MAX_AG : 9999)

#define HIGHLIGHT_COUNT_AREA	10	// The Area in which the kills will be highlighted until the User reaches the KILLCOUNT_MAX
struct KILL_REWARD_ITEM				// Struct for Bonus Item Rewards
{
	INT ItemNum;
	INT Count;
};
#define KILLCOUNT_CACHE_TIME_IN_MINUTES	10	// The time in minutes for which the Killcount of a Character should be cached
struct KILLOUNT_CACHE_DATA
{
	UID32_t			CharacterUniqueNumber;
	INT				KillCount;
	ATUM_DATE_TIME	CacheTime;
};
// Note: Kill Reward Items are in GlobalGameServer: "BonusKillSystemRewardItemNUMs[]"
#endif // S_BONUS_KILL_SYSTEM_RENEWAL