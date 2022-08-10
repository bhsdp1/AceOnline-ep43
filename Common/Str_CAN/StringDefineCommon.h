// 2005-04-28 by cmkwon
// #include "StringDefineCommon.h"
#ifndef _STRING_DEFINE_COMMON_H_
#define _STRING_DEFINE_COMMON_H_

///////////////////////////////////////////////////////////////////////////////
// 1 - Команды, обрабатываемые на FieldServer
	#define STRCMD_CS_COMMAND_GET_WARPOINT_ALL_USER		"/wpalluser"
	#define STRCMD_CS_COMMAND_GET_RND_DGA_TIMES			"/getrndtimes"
	#define STRCMD_CS_COMMAND_MENT_0					"/ment"
	#define STRCMD_CS_COMMAND_MENT_1					"/статус"
	#define STRCMD_CS_COMMAND_MENT_2					"/м"
	#define STRCMD_CS_COMMAND_MENT_HELP					"Формат: /ment или /статус или /м [|Текст] - Установить ment персонажа;"
	#define STRCMD_CS_COMMAND_MOVE					"/move"
	#define STRCMD_CS_COMMAND_MOVE_1					"/move"
	#define STRCMD_CS_COMMAND_MOVE_HELP					"Формат: /move [ID_карты] [|№_канала] - Переход на определенный канал карты;"
	#define STRCMD_CS_COMMAND_COORDINATE				"/coor"
	#define STRCMD_CS_COMMAND_COORDINATE_1				"/coordinate"
	#define STRCMD_CS_COMMAND_COORDINATE_HELP			"Формат: /coordinate или /coor [X] [Y] - Переход к заданным координатам;"
	#define STRCMD_CS_COMMAND_LIST						"/list"
	#define STRCMD_CS_COMMAND_LIST_1					"/list"
	#define STRCMD_CS_COMMAND_LIST_HELP					"Формат: /list - Список пользователей на карте (макс. 20);"
	#define STRCMD_CS_COMMAND_USERSEND					"/senduser"
	#define STRCMD_CS_COMMAND_USERSEND_1				"/senduser"
	#define STRCMD_CS_COMMAND_USERSEND_HELP				"Формат: /senduser [Имя_игрока] [ID_карты] - Перенос персонажа на заданную карту;"
	#define STRCMD_CS_COMMAND_INFObyNAME				"/info"
	#define STRCMD_CS_COMMAND_INFObyNAME_1				"/info"
	#define STRCMD_CS_COMMAND_INFObyNAME_HELP			"Формат: /info [ID_моба или ID_предмета] - Инфо о монстре или предмете;"
	#define STRCMD_CS_COMMAND_QUESTINFO					"/quest"
	#define STRCMD_CS_COMMAND_QUESTINFO_1				"/quest"
	#define STRCMD_CS_COMMAND_QUESTINFO_HELP			"Формат: /quest - Данные по квестам;"
	#define STRCMD_CS_COMMAND_QUESTDEL					"/delQuest"
	#define STRCMD_CS_COMMAND_QUESTDEL_1				"/delQuest"
	#define STRCMD_CS_COMMAND_QUESTDEL_HELP				"Формат: /delQuest [№_квеста] – Отменить квест;"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND			"/itemKind"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND_1		"/itemKind"
	#define STRCMD_CS_COMMAND_ITEMINFObyKIND_HELP		"Формат: /itemKind [|Тип_предмета(0~53)] - Показать предметы указаного типа;"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND			"/insertItemKind"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND_1		"/insertItemKind"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyKIND_HELP		"Формат: /insertItemKind [Тип_предмета(0~53)] -  Additional specific types of items;"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE		"/insertItemNumRange"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_1	"/insertItemNumRange"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE_HELP	"Формат: /insertItemNumRange [от_предмета]~[к_предмету] - Additional specific items;"
	#define STRCMD_CS_COMMAND_STATINIT					"/initStat"
	#define STRCMD_CS_COMMAND_STATINIT_1				"/initStatus"
	#define STRCMD_CS_COMMAND_STATINIT_2				"/initStatus"
	#define STRCMD_CS_COMMAND_STATINIT_HELP				"Формат: /initStatus или /initStat - Инициировать все параметры;"
	#define STRCMD_CS_COMMAND_PARTYINFO					"/partyInfo"
	#define STRCMD_CS_COMMAND_PARTYINFO_1				"/partyInfo"
	#define STRCMD_CS_COMMAND_PARTYINFO_HELP			"Формат: /partyInfo - Показать информацию о звене;"
	#define STRCMD_CS_COMMAND_GAMETIME					"/Time"
	#define STRCMD_CS_COMMAND_GAMETIME_1				"/Time"
	#define STRCMD_CS_COMMAND_GAMETIME_HELP				"Формат: /Time [|Устанавливаемое_время(0~23)] - Изменить текущее время (меняется лишь личное время);"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_0				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_1				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_2				"/string"
	#define STRCMD_CS_COMMAND_STRINGLEVEL_HELP			"Формат: /string [0~5] - Decides levels shown by debug message;"
	#define STRCMD_CS_COMMAND_MONSUMMON					"/summon"
	#define STRCMD_CS_COMMAND_MONSUMMON_1				"/summon"
	#define STRCMD_CS_COMMAND_MONSUMMON_HELP			"Формат: /summon [ID_моба|Имя_моба] [Кол-во] - Призвать монстра ('_'используется вместо пробела);"
	#define STRCMD_CS_COMMAND_SKILLALL					"/allSkill"
	#define STRCMD_CS_COMMAND_SKILLALL_1				"/allSkill"
	#define STRCMD_CS_COMMAND_SKILLALL_HELP				"Формат: /allSkill [Уровень] - Предоставить все подходящие умения;"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL				"/allItem"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL_1			"/allItem"
	#define STRCMD_CS_COMMAND_ITEMINSERTALL_HELP		"Формат: /allItem - Предоставить все указанные предметы, кроме умений и исчисляемых предметов;"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON			"/allWeapon"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON_1		"/allWeapon"
	#define STRCMD_CS_COMMAND_ITEMINSERTWEAPON_HELP		"Формат: /allWeapon - Все вооружение, подходящие данному корпусу;"
	#define STRCMD_CS_COMMAND_ITEMDELALL_0				"/delAllItem"
	#define STRCMD_CS_COMMAND_ITEMDELALL_1				"/delAllItem"
	#define STRCMD_CS_COMMAND_ITEMDELALL_2				"/delAll"
	#define STRCMD_CS_COMMAND_ITEMDELALL_HELP			"Формат: /delAllItem или /delAll - Уничтожить все неустановленные предметы (исключая навыки);"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM		"/item"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_1		"/item"
	#define STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_HELP	"Формат: /item [ID_предмета] [Кол-во] - Получить предметы;"
	#define STRCMD_CS_COMMAND_ITEMDROP					"/dropItem"
	#define STRCMD_CS_COMMAND_ITEMDROP_1				"/dropItem"
	#define STRCMD_CS_COMMAND_ITEMDROP_HELP				"Формат: /dropItem [ID_предмета] [|Кол-во] - Сбросить предмет на поле;"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL			"/server"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL_1			"/server"
	#define STRCMD_CS_COMMAND_USERSINFOTOTAL_HELP		"Формат: /server - Показать данные о сервере;"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP			"/serverMap"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP_1			"/serverMap"
	#define STRCMD_CS_COMMAND_USERSINFOperMAP_HELP		"Формат: /serverMap - Показать данные обо всех картах;"
	#define STRCMD_CS_COMMAND_CHANNELINFO				"/channelInfo"
	#define STRCMD_CS_COMMAND_CHANNELINFO_1				"/channelInfo"
	#define STRCMD_CS_COMMAND_CHANNELINFO_HELP			"Формат: /channelInfo - Показать информацию о канале текущей карты на текущей карте;"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG				"/dbg"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG_1			"/dbg"
	#define STRCMD_CS_COMMAND_DEBUGPRINTDBG_HELP		"Формат: /dbg - Только для тестирования;"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMF			"/testf"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMF_HELP		"Формат: /testf [Param1] [Param2] [Param3];"
	#define STRCMD_CS_COMMAND_BULLETCHARGE				"/bullet"
	#define STRCMD_CS_COMMAND_BULLETCHARGE_1			"/chargeBullet"
	#define STRCMD_CS_COMMAND_BULLETCHARGE_HELP			"Формат: /chargeBullet или /bullet [|Кол-во_стд._зарядов] [|Кол-во_ул._зарядов] – пополнить боезапас;"
	#define STRCMD_CS_COMMAND_REPAIRALL					"/heal"
	#define STRCMD_CS_COMMAND_REPAIRALL_1				"/repairAll"
	#define STRCMD_CS_COMMAND_REPAIRALL_HELP			"Формат: /repairAll или /heal [|Имя_игрока] - Восстановить HP, UTC, топливо до 100%;"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM				"/healparam"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM_1			"/repairParam"
	#define STRCMD_CS_COMMAND_REPAIRbyPARAM_HELP		"Формат: /repairParam или /healparam [|%_уменьшения] [|Имя_игрока] - HP, UTC, и топливо задаются параметром [%_уменьшения];"
	#define STRCMD_CS_COMMAND_USERNORMALIZE				"/normal"
	#define STRCMD_CS_COMMAND_USERNORMALIZE_1			"/user"
	#define STRCMD_CS_COMMAND_USERNORMALIZE_HELP		"Формат: /normal или /user - Сменить тип аккаунта с администраторского или ГМ на обычный;"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE			"/specialize"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE_1			"/admin"
	#define STRCMD_CS_COMMAND_USERSPECIALIZE_HELP		"Формат: /specialize или /admin - Сменить тип аккаунта с обычного на администраторский или ГМ;"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY			"/god"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY_1		"/invincible"
	#define STRCMD_CS_COMMAND_USERINVINCIBILITY_HELP	"Формат: /invincible или /god - Бессмертие в режиме ГМ или администратора;"
	#define STRCMD_CS_COMMAND_POWERUP					"/powerUp"
	#define STRCMD_CS_COMMAND_POWERUP_1					"/powerUp"
	#define STRCMD_CS_COMMAND_POWERUP_HELP				"Формат: /powerUp [усиление(%)];"
	#define STRCMD_CS_COMMAND_VARIABLESET				"/setVariable"
	#define STRCMD_CS_COMMAND_VARIABLESET_1				"/setVariable"
	#define STRCMD_CS_COMMAND_VARIABLESET_HELP			"Формат: /setVariable [variable] - adjusts (normal) variables;"
	#define STRCMD_CS_COMMAND_LEVELSET					"/level"
	#define STRCMD_CS_COMMAND_LEVELSET_1				"/level"
	#define STRCMD_CS_COMMAND_LEVELSET_HELP				"Формат: /level [|уровень] [|%_опыта] [|Имя_игрока] - Устанавливает уровень и процент опыта персонажа;"
	#define STRCMD_CS_COMMAND_USERINVISIABLE			"/ghost"
	#define STRCMD_CS_COMMAND_USERINVISIABLE_1			"/invisible"
	#define STRCMD_CS_COMMAND_USERINVISIABLE_HELP		"Формат: /invisible или /ghost - Невидимость для прочих персонажей;"
	#define STRCMD_CS_COMMAND_USERCOUNT					"/count"


    #define STRCMD_CS_COMMAND_PARTNERLEVELSET			"/partnerlevel"
	#define STRCMD_CS_COMMAND_PARTNERLEVELSET_1			"/partnerlevel"
	#define STRCMD_CS_COMMAND_PARTNERLEVELSET_HELP		"format: /partnerlevel [|level] [|percentage of exp] "
	
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_0			"/messagef"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_1			"/msgf"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGF_HELP		"Формат: /msgf - Только для тестирования;"
	#define STRCMD_CS_COMMAND_GAMEEVENT					"/event"
	#define STRCMD_CS_COMMAND_GAMEEVENT_1				"/event"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1EXP			"exppoint"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1SPI			"SPI"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1EXPR			"restoreexppoint"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1ITEM			"item"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P1RARE			"rareitem"
	#define STRCMD_CS_COMMAND_GAMEEVENT_P2END			"finish"
	#define STRCMD_CS_COMMAND_GAMEEVENT_HELP			"Формат: /event [exppoint|SPI|restoreexppoint|item|rareitem] [|рейты(%)|finish] [время(минуты)] - начать ивент, отменить;"
	#define STRCMD_CS_COMMAND_PREMEUM					"/premium"
	#define STRCMD_CS_COMMAND_PREMEUM_1					"/premium"
	#define STRCMD_CS_COMMAND_PREMEUM_PNORMAL			"standard"
	#define STRCMD_CS_COMMAND_PREMEUM_PSUPER			"super"
	#define STRCMD_CS_COMMAND_PREMEUM_PUPGRADE			"upgrade"
	#define STRCMD_CS_COMMAND_PREMEUM_PEND				"finish"
	#define STRCMD_CS_COMMAND_PREMEUM_HELP				"Формат: /premium [standard|super|upgrade|finish] - Установить тип Премиума;"
// 2008-02-14 by cmkwon,   
//	#define STRCMD_CS_COMMAND_CITYWAR					"/citywar"
//	#define STRCMD_CS_COMMAND_CITYWAR_1					"/citywar"
//	#define STRCMD_CS_COMMAND_CITYWAR_PSTART			"start"
//	#define STRCMD_CS_COMMAND_CITYWAR_PEND				"finish"
//	#define STRCMD_CS_COMMAND_CITYWAR_HELP				"Формат: /citywar [start|finish] - Война;"
	#define STRCMD_CS_COMMAND_STEALTH					"/stealth"
	#define STRCMD_CS_COMMAND_STEALTH_1					"/stealth"
	#define STRCMD_CS_COMMAND_STEALTH_HELP				"Формат: /stealth - Невидимость для мобов;"
	#define STRCMD_CS_COMMAND_RETURNALL					"/returnAll"
	#define STRCMD_CS_COMMAND_RETURNALL_1				"/returnAll"
	#define STRCMD_CS_COMMAND_RETURNALL_HELP			"Формат: /returnAll [ID_карты] - Все члены гильдии переносятся на указанную карту;"
// start 2011-10-28 by hskim, EP4 [Ж®ё®°Е ЅГЅєЕЫ] - Е©ё®ЅєЕ» ЅГЅєЕЫ
#define STRCMD_CS_COMMAND_RESETTRIGGER				"/resetTrigger"
#define STRCMD_CS_COMMAND_RESETTRIGGER_1			"/resetTrigger"
#define STRCMD_CS_COMMAND_RESETTRIGGER_HELP			"format: /resetTrigger [TriggerNumber] - Off the trigger and default the event."
// end 2011-10-28 by hskim, EP4 [Ж®ё®°Е ЅГЅєЕЫ] - Е©ё®ЅєЕ» ЅГЅєЕЫ

// start 2012-02-13 by hskim, ёуЅєЕН ѕЖАМЕЫ µе¶ш ЕЧЅєЖ® ён·Йѕо
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM			"/testmonsterdrop"		// ј­№ц Б¤єё єё±в
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM_1		"/testmonsterdrop"
#define STRCMD_CS_COMMAND_TEST_MONSTER_DROPITEM_HELP	"format: /testmonsterdrop [monster number] [test numbers : (1~100000)] - Monsters that drop items at the show simulation. (A very large load on the server gives an absolute Sacred Shield Do not use)"
// end 2012-02-13 by hskim, ёуЅєЕН ѕЖАМЕЫ µе¶ш ЕЧЅєЖ® ён·Йѕо

// start 2011-06-22 by hskim, »зјі ј­№ц №жБц
	#define STRCMD_CS_COMMAND_SERVERINFO				"/getserverinfo"		// ј­№ц Б¤єё єё±в
// end 2011-06-22 by hskim, »зјі ј­№ц №жБц
			   
// 2007-10-30 by cmkwon, јј·Вє° ЗШЗЗѕЖїц АМєҐЖ® ±ёЗц - ён·Йѕо ЗьЅД јцБ¤µК ѕЖ·ЎїЎј­ ґЩЅГ Б¤АЗ ЗФ
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT			"/happyEvent"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_1			"/happyEvent"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PSTART		"start"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PEND		"end"
//	#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP		"Формат: /happyEvent [start|end] [progress time(unit:min.)]"

// 1_end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 2 - Команды, используемые на IMServer. Некоторые используются с теми же командами, что и выше
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMI			"/testi"
	#define STRCMD_CS_COMMAND_DEBUGSETPARAMI_HELP		"Формат: /testi - IMServer for debug;"
	#define STRCMD_CS_COMMAND_WHO						"/who"
	#define STRCMD_CS_COMMAND_WHO_1						"/who"
	#define STRCMD_CS_COMMAND_WHO_HELP					"Формат: /who [|Кол-во_игроков] - Список пользователей на сервере в данный момент (вне зависимости от карты);"
	#define STRCMD_CS_COMMAND_REGISTERADMIN				"/registerAdmin"
	#define STRCMD_CS_COMMAND_REGISTERADMIN_1			"/registerAdmin"
	#define STRCMD_CS_COMMAND_REGISTERADMIN_HELP		"Формат: /registerAdmin - Дать серверу команду отсылать сообщение администратору в ходе ивента;"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_0			"/messagei"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_1			"/msgi"
	#define STRCMD_CS_COMMAND_DEBUGPRINTMSGI_HELP		"Формат: /msgi - Показать полный протокол, отсылаемый и получаемый клиентом и IM сервером;"
	#define STRCMD_CS_COMMAND_SERVERDOWN				"/serverDown"
	#define STRCMD_CS_COMMAND_SERVERDOWN_1				"/serverDown"
	#define STRCMD_CS_COMMAND_SERVERDOWN_HELP			"Формат: /serverDown [certified_no.] - Server shutdown;"
	#define STRCMD_CS_COMMAND_WHOAREYOU					"/whoareYou"
	#define STRCMD_CS_COMMAND_WHOAREYOU_1				"/whoareYou"
	#define STRCMD_CS_COMMAND_WHOAREYOU_HELP			"Формат: /whoareYou [Имя_игрока] - Инфо об игроке;"
	#define STRCMD_CS_COMMAND_GOUSER					"/go"
	#define STRCMD_CS_COMMAND_GOUSER_1					"/go"
	#define STRCMD_CS_COMMAND_GOUSER_HELP				"Формат: /go [Имя_игрока] - Переместиться к игроку;"
	#define STRCMD_CS_COMMAND_COMEON					"/comeon"
	#define STRCMD_CS_COMMAND_COMEON_1					"/comeon"
	#define STRCMD_CS_COMMAND_COMEON_HELP				"Формат: /comeon [Имя_игрока] - Вызвать игрока;"
	#define STRCMD_CS_COMMAND_GUILDCOMEON				"/comeonGuild"
	#define STRCMD_CS_COMMAND_GUILDCOMEON_1				"/comeongi"
	#define STRCMD_CS_COMMAND_GUILDCOMEON_HELP			"Формат: /comeonGuild или /comeongi [Название_бригады] - Вызвать всю бригаду;"
	#define STRCMD_CS_COMMAND_GUILDSEND					"/sendGuild"
	#define STRCMD_CS_COMMAND_GUILDSEND_1				"/sendgi"
	#define STRCMD_CS_COMMAND_GUILDSEND_HELP			"Формат: /sendGuild или /sendgi [Название_бригады] [ID_карты] - Переместить бригаду на указанную карту;"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG				"/whisperChat"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG_1			"/whChat"
	#define STRCMD_CS_COMMAND_CHATPTOPFLAG_HELP			"Формат: /whisperChat или /whChat - Вкл. и Выкл. чата;"
	#define STRCMD_CS_COMMAND_GUILDINFO					"/guildInfo"
	#define STRCMD_CS_COMMAND_GUILDINFO_1				"/giInfo"
	#define STRCMD_CS_COMMAND_GUILDINFO_HELP			"Формат: /guildInfo или /giInfo - Показать данные о бригаде;"
	#define STRCMD_CS_COMMAND_WEATHERSET				"/weather"
	#define STRCMD_CS_COMMAND_WEATHERSET_1				"/weather"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1NORMAL		"standard"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1FINE			"clear"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1RAIN			"rain"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1SNOW			"snow"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1CLOUDY		"cloudy"
	#define STRCMD_CS_COMMAND_WEATHERSET_P1FOG			"foggy"
	#define STRCMD_CS_COMMAND_WEATHERSET_P2ALL			"whole"
	#define STRCMD_CS_COMMAND_WEATHERSET_P3ON			"on"
	#define STRCMD_CS_COMMAND_WEATHERSET_P3OFF			"off"
	#define STRCMD_CS_COMMAND_WEATHERSET_HELP			"Формат: /weather [standard|clear|rain|snow|cloudy|foggy] [whole|ID_карты] [on|off] - Управление погодой;"
	#define STRCMD_CS_COMMAND_CHATFORBID				"/forbidChat"
	#define STRCMD_CS_COMMAND_CHATFORBID_1				"/banChat"
	#define STRCMD_CS_COMMAND_CHATFORBID_HELP			"Формат: /forbidChat или /banChat [Имя_игрока] [Срок(минуты)] - Запретить чат;"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE			"/releaseChat"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_1		"/unbanChat"
	#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_HELP	"Формат: /releaseChat или /unbanChat [Имя_игрока] - Отменить запрет чата;"
	#define STRCMD_CS_COMMAND_COMMANDLIST_0				"/?"
	#define STRCMD_CS_COMMAND_COMMANDLIST_1				"/help"
	#define STRCMD_CS_COMMAND_COMMANDLIST_2				"/command"
	#define STRCMD_CS_COMMAND_COMMANDLIST_HELP			"Формат: /? или /help - Список команд;"
	#define	STRCMD_CS_COMMAND_XCHAT						"123"
	#define	STRMSG_S_GLOBALCHAT_0001					"123"	

	// 2005-07-20 by cmkwon
	#define STRCMD_CS_COMMAND_BONUSSTAT_0				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_1				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_2				"/BonusStat"
	#define STRCMD_CS_COMMAND_BONUSSTAT_HELP			"Формат: /BonusStat [Кол-во] [|Имя_игрока] - Добавление очков Характеристик;"
// 2_end
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// 3 - КОманды, используемые в AtumMonitor. Некоторые используются с теми же командами, что и выше
	#define STRCMD_CS_COMMAND_PASSWORDSET				"/setPassword"
	#define STRCMD_CS_COMMAND_PASSWORDSET_1				"/setPassword"
	#define STRCMD_CS_COMMAND_PASSWORDSET_HELP			"Формат: /setPassword [Логин] [Пароль];"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK			"/rollbackPassword"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK_1		"/rollbackPassword"
	#define STRCMD_CS_COMMAND_PASSWORDROLLBACK_HELP		"Формат: /rollbackPassword [Логин];"
	#define STRCMD_CS_COMMAND_PASSWORDLIST				"/passwordList"
	#define STRCMD_CS_COMMAND_PASSWORDLIST_1			"/passwordList"
	#define STRCMD_CS_COMMAND_PASSWORDLIST_HELP			"Формат: /passwordList;"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT			"/encrypt"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT_1			"/encrypt"
	#define STRCMD_CS_COMMAND_PASSWORDENCRYPT_HELP		"Формат: /encrypt [string_that_will_be_encrypted];"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCK				"/accountblock"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCK_1			"/accountblock"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKT_HELP		"Формат: /accountblock [Логин] - Блок аккаунта;"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE		"/releaseAccount"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_1		"/unbanAcc"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_HELP	"Формат: /releaseAccount или /banAcc [Логин] - Снять блок;"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST			"/blockedList"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_1		"/banList"
	#define STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_HELP		"Формат: /blockedList или /banList - Список блокированых;"
// 3_end
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// 4 - Общий игровой сервер CommonGameServer
	#define STRCMD_CS_COMMON_DB_0000 "Только для сервера Mgame!\r\n"
	#define STRCMD_CS_COMMON_DB_0001 "Пожалуйста введите имя"
	#define STRCMD_CS_COMMON_DB_0002 "Пожалуйста введите пароль"
	#define STRCMD_CS_COMMON_DB_0003 "Пожалуйста введите DB имя"
	#define STRCMD_CS_COMMON_DB_0004 "Пожалуйста введите DB пароль"

	#define STRCMD_CS_COMMON_MAP_0000 "Часть со сложным кодом: Игнорировать цель гиперпрыжка 1 на карте 0101, необходимо найти легкий способ удаления из редактора карт!\r\n"
	#define STRCMD_CS_COMMON_MAP_0001 "MAP: %04d, m_DefaltWarpTargetIndex: %d\r\n"
	#define STRCMD_CS_COMMON_MAP_0002 "Часть со сложным кодом: Игнорировать цель гиперпрыжка 1 на карте 0101, необходимо найти легкий способ удаления из редактора карт!\r\n"
	#define STRCMD_CS_COMMON_MAP_0003 "MAP: %04d, m_DefaltWarpTargetIndex: %d\r\n"
	#define STRCMD_CS_COMMON_MAP_0004 "    ObjMon ==> ObjNum[%8d] EvType[%d] EvIndex[%3d] summon monster[%8d] summon time[%6dsecond], Pos(%4d, %4d, %4d)\r\n"
	#define STRCMD_CS_COMMON_MAP_0005 "[ERROR] ObjectMonster EventParam1 Index overlap Error ==> ObjectNum[%8d] EventType[%d] EventIndex[%3d] summon monster[%8d] summon time[%6dsecond], Pos(%4d, %4d, %4d)\r\n"
	#define STRCMD_CS_COMMON_MAP_0006 "  Total Monster Count : [%4d] <== Including object monster\r\n"

	#define STRCMD_CS_COMMON_DOWNLOAD_0000 "Ошибка при закачке файла"
	#define STRCMD_CS_COMMON_DOWNLOAD_0001 "Ошибка при создании файла"
	#define STRCMD_CS_COMMON_DOWNLOAD_0002 "Ошибка чтения закачанного файла"

	#define STRCMD_CS_COMMON_DATETIME_0000 "%dday%dhour%dminute%dsecond"

	#define STRCMD_CS_COMMON_RACE_NORMAL		"обычный"
	#define STRCMD_CS_COMMON_RACE_BATTALUS		"Батталус"
	#define STRCMD_CS_COMMON_RACE_DECA			"Дакайан"
	#define STRCMD_CS_COMMON_RACE_PHILON		"Филлон"
	#define STRCMD_CS_COMMON_RACE_SHARRINE		"Шрайн"
	#define STRCMD_CS_COMMON_RACE_MONSTER1		"Резерв"
	#define STRCMD_CS_COMMON_RACE_MONSTER2		"Резерв"
	#define STRCMD_CS_COMMON_RACE_NPC			"NPC"
	#define STRCMD_CS_COMMON_RACE_OPERATION		"Админ"
	#define STRCMD_CS_COMMON_RACE_GAMEMASTER	"ГМ"
	#define STRCMD_CS_COMMON_RACE_MONITOR		"Наблюдатель"
	#define STRCMD_CS_COMMON_RACE_GUEST			"Гость"
	#define STRCMD_CS_COMMON_RACE_DEMO			"Демо"
	#define STRCMD_CS_COMMON_RACE_ALL			"Все расы"
	#define STRCMD_CS_COMMON_RACE_UNKNOWN		"Неизвестно"

	#define STRCMD_CS_COMMON_MAPNAME_UNKNOWN	"Нет имени"

	#define STRCMD_CS_STATUS_BEGINNER_AIRMAN		"Новобранец"
	#define STRCMD_CS_STATUS_3RD_CLASS_AIRMAN		"Рядовой"
	#define STRCMD_CS_STATUS_2ND_CLASS_AIRMAN		"Ефрейтер"
	#define STRCMD_CS_STATUS_1ST_CLASS_AIRMAN		"Сержант"
	#define STRCMD_CS_STATUS_3RD_CLASS_WINGMAN		"Старшина"
	#define STRCMD_CS_STATUS_2ND_CLASS_WINGMAN		"Мичман"
	#define STRCMD_CS_STATUS_1ST_CLASS_WINGMAN		"Лейтенант"
	#define STRCMD_CS_STATUS_3RD_CLASS_LEADER		"Корнет"
	#define STRCMD_CS_STATUS_2ND_CLASS_LEADER		"Капрал"
	#define STRCMD_CS_STATUS_1ST_CLASS_LEADER		"Командор"
	#define STRCMD_CS_STATUS_3RD_CLASS_ACE			"Капитан"
	#define STRCMD_CS_STATUS_2ND_CLASS_ACE			"Флагман"
	#define STRCMD_CS_STATUS_1ST_CLASS_ACE			"Полковник"
	#define STRCMD_CS_STATUS_COPPER_CLASS_GENERAL	"Майор"
	#define STRCMD_CS_STATUS_SILVER_CLASS_GENERAL	"Адмирал"
	#define STRCMD_CS_STATUS_GOLD_CLASS_GENERAL		"Генерал"
	#define STRCMD_CS_STATUS_MASTER_GENERAL			"Маршал"

	#define STRCMD_CS_ITEMKIND_AUTOMATIC			"Автоматическое"
	#define STRCMD_CS_ITEMKIND_VULCAN				"Вулкан"
	#define STRCMD_CS_ITEMKIND_DUALIST				"Сдвоенное"		// 2005-08-01 by hblee : changed from GRENADE to DUALIST.
	#define STRCMD_CS_ITEMKIND_CANNON				"Пушечное"
	#define STRCMD_CS_ITEMKIND_RIFLE				"Ружейное"
	#define STRCMD_CS_ITEMKIND_GATLING				"Гатлинг"
	#define STRCMD_CS_ITEMKIND_LAUNCHER				"Пусковая установка"
	#define STRCMD_CS_ITEMKIND_MASSDRIVE			"Ускоритель материи"
	#define STRCMD_CS_ITEMKIND_ROCKET				"Ракетное"
	#define STRCMD_CS_ITEMKIND_MISSILE				"Реактивное"
	#define STRCMD_CS_ITEMKIND_BUNDLE				"Система залпового огня"

	#define STRCMD_CS_ITEMKIND_MINE				"Минное"
	#define STRCMD_CS_ITEMKIND_SHIELD				"Щиты"
	#define STRCMD_CS_ITEMKIND_DUMMY				"Учебное"			
	#define STRCMD_CS_ITEMKIND_FIXER				"Ремонтное"
	#define STRCMD_CS_ITEMKIND_DECOY				"Приманка"
	#define STRCMD_CS_ITEMKIND_DEFENSE				"Корпус"
	#define STRCMD_CS_ITEMKIND_SUPPORT				"Двигатели"
	#define STRCMD_CS_ITEMKIND_ENERGY				"Энергетическое"
	#define STRCMD_CS_ITEMKIND_INGOT				"Ресурсы"
	#define STRCMD_CS_ITEMKIND_CARD				"Стандартная карта"
	#define STRCMD_CS_ITEMKIND_ENCHANT				"Модуль улучшения"
	#define STRCMD_CS_ITEMKIND_TANK				"Танковое"
	#define STRCMD_CS_ITEMKIND_BULLET				"Боеприпасы"
	#define STRCMD_CS_ITEMKIND_QUEST				"Квестовое"
	#define STRCMD_CS_ITEMKIND_RADAR				"Радар"
	#define STRCMD_CS_ITEMKIND_COMPUTER				"Компьютер"
	#define STRCMD_CS_ITEMKIND_GAMBLE				"Модуль добавления"
	#define STRCMD_CS_ITEMKIND_PREVENTION_DELETE_ITEM	"Модуль защиты от разрушения"
	#define STRCMD_CS_ITEMKIND_BLASTER				"Бластерное"	// 2005-08-01 by hblee : Blaster type added.
	#define STRCMD_CS_ITEMKIND_RAILGUN				"Рельсовое"		// 2005-08-01 by hblee : Rail gun type added.
	#define STRCMD_CS_ITEMKIND_ACCESSORY_UNLIMITED	"Постоянные предметы"		// 2006-03-17 by cmkwon, »зїлЅГ°ЈАМ <їµїш>АО ѕЧјјј­ё® ѕЖАМЕЫ
	#define STRCMD_CS_ITEMKIND_ACCESSORY_TIMELIMIT	"Временные предметы"		// 2006-03-17 by cmkwon, »зїлЅГ°ЈїЎ ЅГ°Ј Б¦ЗСАМ АЦґВ ѕЧјјј­ё® ѕЖАМЕЫ
	#define STRCMD_CS_ITEMKIND_ALL_WEAPON			"Все виды оружия"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_ALL	"Стрелковое оружие"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_1		"Стрелковое улевое оружие"
	#define STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_2		"Стрелковое оружие"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_ALL	"Ракетное оружие"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_1	"Ракетное осколочное оружие"
	#define STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_2	"Ракетное оружие"
	#define STRCMD_CS_ITEMKIND_SKILL_ATTACK			"Умение атаки"
	#define STRCMD_CS_ITEMKIND_SKILL_DEFENSE		"Умение защиты"
	#define STRCMD_CS_ITEMKIND_SKILL_SUPPORT		"Умение поддержки"
	#define STRCMD_CS_ITEMKIND_SKILL_ATTRIBUTE		"Умение применения"
	#define STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY		"Предмет для монстра 1 типа"
	#define STRCMD_CS_ITEMKIND_FOR_MON_GUN			"Пулемет монстра (тип1-1)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_BEAM			"Лучевое оружие монстра (тип1-2)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK	"Атаковать всех монстров"
	#define STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY	"Предмет для монстра 2 типа"
	#define STRCMD_CS_ITEMKIND_FOR_MON_ROCKET		"Ракета монстра (тип2-1)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_MISSILE		"Реактивное оружие монстра (тип2-1)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE		"Сборное оружие монстра (тип2-1)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_MINE			"Минное оружие монстра (тип2-1)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_SHIELD		"Щитовое оружие монстра (тип2-2)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_DUMMY		"Отвлекающее оружие монстра (тип2-2)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_FIXER		"Ремонтное оружие монстра (тип2-2)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_DECOY		"Приманка монстра (тип2-2)"
	#define STRCMD_CS_ITEMKIND_FOR_MON_FIRE			"Огненное оружие монстра"
	#define STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM		"Возможное лучевое поражение монстра"
	#define STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM	"Бомбовое поражение монстра"
	#define STRCMD_CS_ITEMKIND_UNKNOWN				"Неизвестный предмет"

	#define STRCMD_CS_UNITKIND_UNKNOWN				"Неизвестный корабль"

	#define STRCMD_CS_STAT_ATTACK_PART				"Атака"
	#define STRCMD_CS_STAT_DEFENSE_PART				"Защита"
	#define STRCMD_CS_STAT_FUEL_PART				"Топливо"
	#define STRCMD_CS_STAT_SOUL_PART				"Дух"
	#define STRCMD_CS_STAT_SHIELD_PART				"Щит"
	#define STRCMD_CS_STAT_DODGE_PART				"Ловкость"
	#define STRCMD_CS_STAT_BONUS					"Бонусный параметр"
	#define STRCMD_CS_STAT_ALL_PART					"Все параметры"
	#define STRCMD_CS_STAT_UNKNOWN					"Неизвестный параметр"

	#define STRCMD_CS_AUTOSTAT_TYPE_FREESTYLE		"Любой тип"
	#define STRCMD_CS_AUTOSTAT_TYPE_BGEAR_ATTACK	"Тип атаки"
	#define STRCMD_CS_AUTOSTAT_TYPE_BGEAR_MULTI		"Мультитип"	
	#define STRCMD_CS_AUTOSTAT_TYPE_IGEAR_ATTACK	"Тип атаки"
	#define STRCMD_CS_AUTOSTAT_TYPE_IGEAR_DODGE		"Тип ловкости"
	#define STRCMD_CS_AUTOSTAT_TYPE_AGEAR_ATTACK	"Тип атаки"
	#define STRCMD_CS_AUTOSTAT_TYPE_AGEAR_SHIELD	"Тип щита"
	#define STRCMD_CS_AUTOSTAT_TYPE_MGEAR_DEFENSE	"Тип защиты"
	#define STRCMD_CS_AUTOSTAT_TYPE_MGEAR_SUPPORT	"Тип поддержки"
	#define STRCMD_CS_AUTOSTAT_TYPE_UNKNOWN			"UNKNOWN_AUTOSTAT_TYPE"

// 2007-10-30 by cmkwon, јј·Вє° ЗШЗЗѕЖїц АМєҐЖ® ±ёЗц - ѕЖ·ЎїЎј­ ґЩЅГ Б¤АЗ ЗФ
//	#define STRCMD_CS_INFLUENCE_TYPE_NORMAL			"Генеральская армия города Бигенью"
//	#define STRCMD_CS_INFLUENCE_TYPE_VCN			"Регулярная армия города Бигенью"
//	#define STRCMD_CS_INFLUENCE_TYPE_ANI			"Регулярная армия города Арлингтон"
	#define STRCMD_CS_INFLUENCE_TYPE_RRP			"Армия Батталусской федерации"

	#define STRCMD_CS_POS_PROW						"Расположение радара(вверху в центре)"
	#define STRCMD_CS_POS_PROWIN					"Компьютер(сверху слева)"
	#define STRCMD_CS_POS_PROWOUT					"Стрелковое оружие(сверху слева)"
	#define STRCMD_CS_POS_WINGIN					"Не используется(в центре справа)"
	#define STRCMD_CS_POS_WINGOUT					"Ракетное оружие(сверху справа)"
	#define STRCMD_CS_POS_CENTER					"Броня (в центре посередине)"
	#define STRCMD_CS_POS_REAR						"Двигатель(внизу посередине)"


	// 2010-06-15 by shcho&hslee ЖкЅГЅєЕЫ
	//#define STRCMD_CS_POS_ATTACHMENT				"Дополнительно(внизу справа, топливный бак или контейнер)"
	#define STRCMD_CS_POS_ACCESSORY_UNLIMITED		"Дополнительно(сзади справа, топливный бак или контейнер)"

	// 2010-06-15 by shcho&hslee ЖкЅГЅєЕЫ
	//#define STRCMD_CS_POS_PET						"Not to use(Left rear side)"
	#define STRCMD_CS_POS_ACCESSORY_TIME_LIMIT		"Не используется (слева сзади)"

	#define STRCMD_CS_POS_PET						"Партнер"

	#define STRCMD_CS_POS_INVALID_POSITION			"Ожидаемое положение"
    #define STRCMD_CS_HIDDEN_ITEM					"Hidden Position"		// 2011-09-20 by hskim, ЖДЖ®іК ЅГЅєЕЫ 2Вч - јы°ЬБш ѕЖАМЕЫ	
	#define STRCMD_CS_POS_ITEMWINDOW_OFFSET			"Положение инвентаря"

	// 2005-12-07 by cmkwon
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_0		"/QuestComplete"
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_1		"/QuestCom"
	#define STRCMD_CS_COMMAND_QUESTCOMPLETION_HELP	"Формат: /QuestComplete или /QuestCom [|ID_квеста];"

	// 2006-02-08 by cmkwon
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_0		"/NationRatio"
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_1		"/InflDist"
	#define STRCMD_CS_COMMAND_INFLDITRIBUTION_HELP	"Формат: /NationRatio или /InflDist;"
	#define STRCMD_CS_COMMAND_CHANGEINFL_0			"/ChangeNation"
	#define STRCMD_CS_COMMAND_CHANGEINFL_1			"/InflChange"
	#define STRCMD_CS_COMMAND_CHANGEINFL_HELP		"Формат: /ChangeNation или /InflChange [|1(Новичок)|2(Бигенью)|4(Арлингтон)];"

	// 2006-03-02 by cmkwon
	#define STRCMD_CS_COMMAND_GOMONSTER_0			"/GoMon"
	#define STRCMD_CS_COMMAND_GOMONSTER_1			"/GoMonster"
	#define STRCMD_CS_COMMAND_GOMONSTER_HELP		"Формат: /GoMonster или /GoMon [ID_моба|Имя_моба];"

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-20 by dhjin, EP3 - ї©ґЬ јцБ¤ »зЗЧ - БЦј® Гіё® №ШАё·О АМµї
	// 2006-03-07 by cmkwon
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_NULL		"Пилот"
//	#define STRCMD_CS_GUILD_RANK_COMMANDER			"Лидер бригады"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_1		"Лидер гр. №1"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_1			"Пилот гр. №1"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_2		"Лидер гр. №2"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_2			"Пилот гр. №2"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_3		"Лидер гр. №3"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_3			"Пилот гр. №3"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_4		"Лидер гр. №4"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_4			"Пилот гр. №4"
//	#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_5		"Лидер гр. №5"
//	#define STRCMD_CS_GUILD_RANK_PRIVATE_5			"Пилот гр. №5"

	// 2006-04-17 by cmkwon
	#define STRCMD_CS_COMMAND_SIGNBOARD_0			"/Noticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_1			"/Noticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_HELP		"Формат: /Noticeboard [|Срок(минуты)] [Сообщение] - Помещает записку на электронную доску объявлений на заданный промежуток времени;"
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_0		"/DeleteNoticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_1		"/DeleteNoticeboard"
	#define STRCMD_CS_COMMAND_SIGNBOARD_DEL_HELP	"Формат: /DeleteNoticeboard [ID_сообщения] - Удаляет указаное сообщение с электронной доски объявлений;"
	
	// 2006-04-20 by cmkwon
	#define STRCMD_CS_COMMON_RACE_INFLUENCE_LEADER	"Лидер фракции"
	#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER	"Сублидер фракции"
	// 2006-04-21 by cmkwon
	#define STRCMD_CS_ITEMKIND_INFLUENCE_BUFF		"Бафф влияния"
	#define STRCMD_CS_ITEMKIND_INFLUENCE_GAMEEVENT	"Ивент влияния"

	// 2006-04-24 by cmkwon
	#define STRCMD_CS_COMMAND_CONPOINT_0			"/ContributionPoint"
	#define STRCMD_CS_COMMAND_CONPOINT_1			"/ContributionPoint"
	#define STRCMD_CS_COMMAND_CONPOINT_HELP			"Формат: /ContributionPoint [Фракция(2:Бигенью, 4:Арлингтон)] [Кол-во] - Увеличить очки указанной фракции;"

	// 2006-05-08 by cmkwon
	#define STRCMD_CS_COMMAND_CALLGM_0				"/CallGM"
	#define STRCMD_CS_COMMAND_CALLGM_1				"/CallGM"  // Helper
	#define STRCMD_CS_COMMAND_CALLGM_2				"/CallGM"  // Help
	#define STRCMD_CS_COMMAND_CALLGM_HELP			"Формат: /CallGM  [Суть_обращения] - Обратиться к Администрации;"
	#define STRCMD_CS_COMMAND_VIEWCALLGM_0			"/ViewCallGM"
	#define STRCMD_CS_COMMAND_VIEWCALLGM_1			"/ViewCallGM"   // See helper
	#define STRCMD_CS_COMMAND_VIEWCALLGM_2			"/VCGM"   // See help
	#define STRCMD_CS_COMMAND_VIEWCALLGM_HELP		"Формат: /ViewCallGM или /VCGM [|номер(1~10)] - Отображает номера запросов к ГМу;"
	#define STRCMD_CS_COMMAND_BRINGCALLGM_0			"/BringCallGM"
	#define STRCMD_CS_COMMAND_BRINGCALLGM_1			"/BringCallGM"   // Bring helper
	#define STRCMD_CS_COMMAND_BRINGCALLGM_2			"/BCGM"   // Bring help
	#define STRCMD_CS_COMMAND_BRINGCALLGM_HELP		"Формат: /BringCallGM или /BCGM [|номер(1~10)] - Принять вопрос на обработку (Удаляется с сервера);"

	// 2006-07-18 by cmkwon
	#define STRCMD_CS_COMMAND_COMEONINFL_0			"/ComeOnInfl"
	#define STRCMD_CS_COMMAND_COMEONINFL_1			"/ComeOnNation"
	#define STRCMD_CS_COMMAND_COMEONINFL_2			"/ComeOnNation"	 
// 2008-09-09 by cmkwon, /јј·ВјТИЇ ён·Йѕо АОАЪ ё®ЅєЖ®їЎ ±вѕоЕёАФ ГЯ°Ў - commented
//	#define STRCMD_CS_COMMAND_COMEONINFL_HELP		"Формат: /ComeOnNation или /ComeOnInfl [1(Новичок)|2(Бигенью)|4(Арлингтон)|3|5|6|7] [Кол-во_игроков] [0|Мин_ур.] [0|Макс_ур.] [Сообщение] - Вызывает всех персонажей указанного уровня какой-либо фракции одновременно с отправкой сообщения;"
  // 2006-07-24 by cmkwon
	#define STRCMD_CS_COMMAND_ITEMINMAP_0			"/InsertItemInMap"
	#define STRCMD_CS_COMMAND_ITEMINMAP_1			"/SendItem"
	#define STRCMD_CS_COMMAND_ITEMINMAP_2			"/AdditemOnMap"
	#define STRCMD_CS_COMMAND_ITEMINMAP_HELP		"Формат: /SendItem или /InsertItemInMap или /AdditemOnMap [1(Новичок)|2(Бигенью)|4(Арлингтон)|3|5|6|7] [ID_предмета] [Кол-во] - Дает определенный предмет всем принадлежащим к фракции пользователям на данной карте;"

	// 2006-07-28 by cmkwon
	#define STRCMD_CS_ITEMKIND_COLOR_ITEM			"Цвет предмета"

	// 2006-08-03 by cmkwon, іЄ¶ує° іЇВҐ ЗҐЗц №жЅДАМ ґЩёЈґЩ
	// ЗС±№(Korea):		YYYY-MM-DD HH:MM:SS
	// №М±№(English):	MM-DD-YYYY HH:MM:SS
	// єЈЖ®іІ(Vietnam):	DD-MM-YYYY HH:MM:SS
	#define NATIONAL_ATUM_DATE_TIME_STRING_FORMAT(Y, M, D, h, m, s)				"%02d-%02d-%04d %02d:%02d:%02d", M, D, Y, h, m, s
	#define NATIONAL_ATUM_DATE_TIME_STRING_FORMAT_EXCLUDE_SECOND(Y, M, D, h, m)	"%02d-%02d-%04d %02d:%02d", M, D, Y, h, m

	// 2006-08-08 by dhjin, ·№є§єРЖч
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_0		"/LevelDistribution"		// 2006-08-08 by dhjin
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_1		"/LevelDist"				// 2006-08-08 by dhjin
	#define STRCMD_CS_COMMAND_DISTRIBUTIONLEVEL_HELP	"Формат: /LevelDistribution или /LevelDist - Отображает текущее распределение уровней пользователей;"	// 2006-08-08 by dhjin

	// 2006-08-10 by cmkwon
	#define STRCMD_CS_ITEMKIND_RANDOMBOX				"Случайный предмет"

	// 2006-08-21 by cmkwon
	#define STRCMD_CS_ITEMKIND_MARK						"Отметить"

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-24 by cmkwon
	// Е¬¶уАМѕрЖ®їЎј­ёё »зїлЗПґВ ён·Йѕо(Just command for client)
	#define STRCMD_C_COMMAND_CALL						"/call"
	#define STRCMD_C_COMMAND_CALL_HELP					"Формат: /call [Имя_игрока] - Запрос голосового общения с определенным персонажем;"
	#define STRCMD_C_COMMAND_PARTYCALL					"/formcall"
	#define STRCMD_C_COMMAND_PARTYCALL_HELP				"Формат: /formcall - Голосовой чат между членами звена. Может быть использован только лидером звена;"
	#define STRCMD_C_COMMAND_PARTYCALLEND				"/formcallend"
	#define STRCMD_C_COMMAND_PARTYCALLEND_HELP			"Формат: /formcallend - Прекратить голосовой чат между членами звена. Может быть использован только лидером звена;"
	#define STRCMD_C_COMMAND_GUILDCALL					"/brigcall"
	#define STRCMD_C_COMMAND_GUILDCALL_HELP				"Формат: /brigcall - Начать голосовой чат между членами бригады. Может быть использован только лидером бригады;"
	#define STRCMD_C_COMMAND_GUILDCALLEND				"/brigcallend"
	#define STRCMD_C_COMMAND_GUILDCALLEND_HELP			"Формат: /brigcallend - Прекратить голосовой чат между членами бригады. Может быть использован только лидером бригады;"
	#define STRCMD_C_COMMAND_CALLEND					"/endcall"
	#define STRCMD_C_COMMAND_CALLEND_HELP				"Формат: /endcall - Прекратить голосовой чат бригады, звена или обычного типа;"
	#define STRCMD_C_COMMAND_COMBAT						"/confront"
	#define STRCMD_C_COMMAND_BATTLE						"/fight"
	#define STRCMD_C_COMMAND_BATTLE_HELP				"Формат: /fight [Имя_игрока] - Предложить PvP определенному персонажу;"
	#define STRCMD_C_COMMAND_SURRENDER					"/surrender"
	#define STRCMD_C_COMMAND_SURRENDER_HELP				"Формат: /surrender [Имя_игрока] -  Сдаться в PvP с определенным персонажем;"
	#define STRCMD_C_COMMAND_PARTYBATTLE				"/formfight"
	#define STRCMD_C_COMMAND_PARTYBATTLE_HELP			"Формат: /formfight [Имя_игрока] - Предложить PvP звеном определенному персонажу (или лидеру другого звена). Может быть использовано только лидером звена;"
	#define STRCMD_C_COMMAND_PARTYCOMBAT				"/formconfront"
	#define STRCMD_C_COMMAND_PARTYWAR					"/formbattle"
	#define STRCMD_C_COMMAND_GUILDBATTLE				"/brigfight"
	#define STRCMD_C_COMMAND_GUILDCOMBAT				"/brigconfront"
	#define STRCMD_C_COMMAND_GUILDCOMBAT_HELP			"Формат: /brigconfront [Имя_игрока] -  Предложить PvP бригадой определенному персонажу (или лидеру другой бригады). Может быть использовано только лидером бригады;"
	#define STRCMD_C_COMMAND_GUILDWAR					"/brigbattle"
	#define STRCMD_C_COMMAND_GUILDSURRENDER				"/brigsurrender"
	#define STRCMD_C_COMMAND_GUILDSURRENDER_HELP		"Формат: /brigsurrender -  Сдаться в бригадном PvP. Может быть использовано только лидером бригады;"
	#define STRCMD_C_COMMAND_NAME						"/name"
	#define STRCMD_C_COMMAND_NAME_HELP					"Формат: /name [Имя_игрока] [Класс(2~11)] - Изменить статус персонажа. Может быть использовано только лидером бригады;"
	#define STRCMD_C_COMMAND_WARP						"/warp"
	#define STRCMD_C_COMMAND_CANCELSKILL				"/cancelskill"
	#define STRCMD_C_COMMAND_INITCHAT					"/initchat"
	#define STRCMD_C_COMMAND_INITCHAT_HELP				"Формат: /initchat -  Перезапустить окно чата;"
	#define STRCMD_C_COMMAND_REFUSEBATTLE				"/refusefight"
	#define STRCMD_C_COMMAND_REFUSEBATTLE_HELP			"Формат: /refusefight - Вкл/Выкл переключатель автоматического ответа на предложение PvP;"
	#define STRCMD_C_COMMAND_REFUSETRADE				"/refusetrade"
	#define STRCMD_C_COMMAND_REFUSETRADE_HELP			"Формат: /refusetrade -  Вкл/Выкл переключатель автоматического ответа на предложение обмена;"
	#define STRMSG_C_050810_0001						"/CloseWindow"
	#define STRMSG_C_050810_0001_HELP					"Формат: /Closewindow -  Отключает всплывающие сообщения;"
	#define STRMSG_C_050810_0002						"/OpenWindow"
	#define STRMSG_C_050810_0002_HELP					"Формат: /Openwindow -  Включает всплывающие сообщения;"

// 2006-09-29 by cmkwon
#define STRCMD_CS_ITEMKIND_SKILL_SUPPORT_ITEM			"Предмет умения поддержки"

// 2010-06-15 by shcho&hslee ЖкЅГЅєЕЫ - Жк ѕЖАМЕЫ.
#define STRCMD_CS_ITEMKIND_PET_ITEM						"Предмет партнера"
#define STRCMD_CS_ITEMKIND_PET_SOCKET_ITEM				"Partner Socket Item"		// 2011-09-01 by hskim, ЖДЖ®іК ЅГЅєЕЫ 2Вч

// 2006-11-17 by cmkwon, єЈЖ®іІ ЗП·з °ФАУ ЅГ°Ј °ь·Г
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_0			"/TimeLimitSystem"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_1			"/TimeLimitSystem"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_P2ON		"on"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_P2OFF		"off"
#define STRCMD_CS_COMMAND_TIMELIMITSYSTEM_HELP		"Формат: /TimeLimitSystem [on|off] - Включение и выключение системы временного ограничения;"
#define STRCMD_CS_COMMAND_PLAYTIME_0				"/PlayTime"
#define STRCMD_CS_COMMAND_PLAYTIME_1				"/PlayTime"
#define STRCMD_CS_COMMAND_PLAYTIME_HELP				"Формат: /PlayTime - Отображает время в игре за день;"

// 2007-10-06 by cmkwon, єОБцµµАЪ 2ёнАЗ ИЈДЄА» ґЩёЈ°Ф јіБ¤ - ѕЖ·ЎїЎ јј·Вє°·О ґЩёЈ°Ф Б¤АЗЗФ
//// 2006-12-13 by cmkwon
//#define STRCMD_CS_COMMON_INFLUENCE_LEADER			"Лидер"
//#define STRCMD_CS_COMMON_INFLUENCE_SUBLEADER			"Сублидер"

// 2007-01-08 by dhjin
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_0			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_1			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_2			"/BonusStatPoint"
#define STRCMD_CS_COMMAND_BONUSSTAT_POINT_HELP		"Формат: /BonusStatPoint [Кол-во_очков] [|Имя_игрока] - добавление очков Хар-ки;"

// 2007-01-25 by dhjin
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_0			"/PCBang"
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_1			"/PCBang"
#define STRCMD_CS_COMMAND_PCBANGUSERCOUNT_HELP		"Формат: /PCBang -  счет PC-клуба пользователя;"

// 2007-10-06 by dhjin, єОБцµµАЪ ј±Гв №ж№э єЇ°жАё·О јцБ¤
// 2007-02-13 by dhjin, єОБцµµАЪ
//#define STRCMD_CS_COMMAND_SUBLEADER_0				"/Subleader"
//#define STRCMD_CS_COMMAND_SUBLEADER_1				"/Subleader"
//#define STRCMD_CS_COMMAND_SUBLEADER_HELP			"Формат: /Subleader [Имя_игрока] - Установить сублидера"
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_ERROR		"Установка сублидера отменена."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_0			"Невозможно установить больше сублидеров."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_1			"%s стал первым сублидером."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_2			"%s стал вторым сублидером."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_10			"%s не существует."
//#define STRCMD_CS_COMMAND_SUBLEADER_RESULT_20			"%s уже является сублидером."

// 2007-02-23 by dhjin, °ЕБЎБ¤єё
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_0			"/StrategyPointInfo"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_1			"/StrategyPointInfo"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_HELP		"Формат: /StrategyPointInfo - Отображает текущий прогресс по стратегической точке;"
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_EMPTY		"Сейчас не идет войн за стратегические точки."
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_EXIST		"Идет война за стратегические точки."
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_ZONE		"Прогресс по локации."
#define STRCMD_CS_COMMAND_STRATEGYPOINTINFO_STARTTIME		"Время начала."

// 2007-03-29 by cmkwon
#define STRCMD_CS_UNITKIND_BGEAR					"B-корпус"
#define STRCMD_CS_UNITKIND_MGEAR					"M-корпус"
#define STRCMD_CS_UNITKIND_AGEAR					"A-корпус"
#define STRCMD_CS_UNITKIND_IGEAR					"I-корпус"
#define STRCMD_CS_UNITKIND_BGEAR_ALL				"Все B-корпуса"
#define STRCMD_CS_UNITKIND_MGEAR_ALL				"Все M-корпуса"
#define STRCMD_CS_UNITKIND_AGEAR_ALL				"Все A-корпуса"
#define STRCMD_CS_UNITKIND_IGEAR_ALL				"Все I-корпуса"
#define STRCMD_CS_UNITKIND_GEAR_ALL					"Все корпуса"

// 2007-03-30 by dhjin, їЙАъ№ц ёрµе АЇАъ µо·П
#define STRCMD_CS_COMMAND_OBSERVER_REG_START_0  		"/Observerstart"  // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_START_1  		"/Observerstart"   // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_END_0  			"/Observerend"   // 2007-03-30 by dhjin, Client only 
#define STRCMD_CS_COMMAND_OBSERVER_REG_END_1  			"/Observerend"   // 2007-03-30 by dhjin, Client only
#define STRCMD_CS_COMMAND_OBSERVER_REG_0   			"/Observer"
#define STRCMD_CS_COMMAND_OBSERVER_REG_1   			"/Observer"
#define STRCMD_CS_COMMAND_OBSERVER_REG_HELP   			"Формат: /Observer [n] [CharacterName] - CharacterName  save user at n number;"

// 2007-04-10 by cmkwon, Jamboree server ±є °ь·Г
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_0   			"/InitJamboree"   
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_1   			"/InitJamboree"  
#define STRCMD_CS_COMMAND_JAMBOREE_INIT_HELP  			"Формат: /InitJamboree [validation number] - Initialize jamboree server DB(atum2_db_20);"
#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_0  			"/EntrantJamboree"
#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_1  			"/EntrantJamboree"  
#define STRCMD_CS_COMMAND_JAMBOREE_ENTRANTS_HELP 		"Формат: /EntrantJamboree [CharacterName] - Designated character will be duplicated to jamboree server DB(atum2_db_20);"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_1  "1_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_2  "2_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_3  "3_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_4  "4_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_5  "5_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_6  "6_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_7  "7_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_8  "8_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_9  "9_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_10  "10_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_11  "11_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_12  "12_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_13  "13_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_14  "14_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_15  "15_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_16  "16_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_17  "17_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_18  "18_"
#define STRCMD_CS_JAMBOREE_PREADD_CHARACTER_NAME_19  "19_"

// 2007-04-17 by dhjin, ·№є§ ·©Е©їЎ °ьЗС µо±Ю
#define STRCMD_CS_CHARACTER_12_LEVEL_RANK  "Ученик"
#define STRCMD_CS_CHARACTER_22_LEVEL_RANK  "Юниор"
#define STRCMD_CS_CHARACTER_32_LEVEL_RANK  "Пилот"
#define STRCMD_CS_CHARACTER_42_LEVEL_RANK  "Навигатор"
#define STRCMD_CS_CHARACTER_52_LEVEL_RANK  "Ас"
#define STRCMD_CS_CHARACTER_62_LEVEL_RANK  "Ветеран"
#define STRCMD_CS_CHARACTER_72_LEVEL_RANK  "Лучший стрелок"
#define STRCMD_CS_CHARACTER_82_LEVEL_RANK  "Глава флота"
#define STRCMD_CS_CHARACTER_92_LEVEL_RANK  "Герой"

// 2007-05-09 by cmkwon, 
#define STRMSG_VERSION_INFO_FILE_NAME				"VersionInfo.ver"
#define STRMSG_REG_KEY_NAME_LAUNCHER_VERSION		"LauncherVersion"
#define STRMSG_REG_KEY_NAME_CLIENT_VERSION			"ClientVersion"
#define STRMSG_REG_KEY_NAME_DELETE_VERSION			"DeleteVersion"
// 2007-12-27 by cmkwon, А©µµїмБо ёрµе ±вґЙ ГЯ°Ў -
//#define STRMSG_REG_KEY_NAME_WINDOWDEGREE			"WindowDegree"
#define STRMSG_REG_KEY_NAME_ACCOUNT_NAME			"AccountName"
#define STRMSG_REG_KEY_NAME_PWD						"Password"
#define STRMSG_REG_KEY_NAME_SERVER_GROUP_NAME		"ServerGroupName"

// 2007-05-23 by dhjin, ARENA ЖА Гв·В °ь·Г ЅєЖ®ёµ
#define STRMSG_CS_STRING_ARENA_NOT_SEARCH   "Невозможно найти команду арены."
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_0   "/ARENA"
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_1   "/ARENA"
#define STRMSG_CS_COMMAND_ARENA_TEAM_INFO_HELP  "Формат: /arena [2(Бигенью)|4(Арлингтон)] - Отображает текущий прогресс на арене;"

// 2010. 06. 04 by hsLee ARENA АОЗЗґПЖј °ь·Г. - 
// 2010. 06. 04 by hsLee АОЖјЗЗґП ЗКµе 2Вч і­АМµµ Б¶Аэ. (GM ён·Йѕо ГЯ°Ў. /nextscene(ґЩАЅ ЅГіЧё¶ ѕА ИЈГв.) )
#define STRCMD_CS_COMMAND_INFINITY_NEXT_SCENE		"/nextscene"
// End 2010. 06. 04 by hsLee АОЖјЗЗґП ЗКµе 2Вч і­АМµµ Б¶Аэ. (GM ён·Йѕо ГЯ°Ў. /nextscene(ґЩАЅ ЅГіЧё¶ ѕА ИЈГв.) )

// 2007-06-15 by dhjin, °ьАь
#define STRMSG_CS_COMMAND_WATCH_START_INFO_0		"/WatchStart"
#define STRMSG_CS_COMMAND_WATCH_START_INFO_1		"/WatchStart"
#define STRMSG_CS_COMMAND_WATCH_START_INFO_HELP	"формат:/WatchStart - Запуск таймера;"
#define STRMSG_CS_COMMAND_WATCH_END_INFO_0			"/WatchEnd"
#define STRMSG_CS_COMMAND_WATCH_END_INFO_1			"/WatchEnd"
#define STRMSG_CS_COMMAND_WATCH_END_INFO_HELP		"Формат: /WatchEnd - Остановка таймера;"

// 2007-06-22 by dhjin, WarPoint ГЯ°Ў
#define STRMSG_CS_COMMAND_WARPOINT_0    "/WarPoint"
#define STRMSG_CS_COMMAND_WARPOINT_1    "/WarPoint"
#define STRMSG_CS_COMMAND_WARPOINT_HELP    "Формат: /WarPoint [Кол-во(1~1000000)] [|Имя_игрока] - Добавить Очки Войны;"

// 2007-06-26 by dhjin, їцЖчАОЖ® АМєҐЖ® °ь·Г ГЯ°Ў
#define STRCMD_CS_COMMAND_GAMEEVENT_P1WARPOINT		"WarPoint"

// 2007-07-11 by cmkwon, Материализация блок-системы арены - Добавить команду(/forbidAreana, /releaseArena)
#define STRCMD_CS_COMMAND_ARENAFORBID_0    "/forbidArena "
#define STRCMD_CS_COMMAND_ARENAFORBID_1    "/forbidArena"
#define STRCMD_CS_COMMAND_ARENAFORBID_2    "/forbidArena"
#define STRCMD_CS_COMMAND_ARENAFORBID_HELP   "Формат: /forbidArena [Имя_игрока] [|Время(минуты)] - Запретить вход на арену;"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_0  "/releaseArena "
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_1  "/releaseArena"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_2  "/releaseArena"
#define STRCMD_CS_COMMAND_ARENAFORBIDRelease_HELP "Формат: /releaseArena [Имя_игрока] - Разрешить вход на арену;"

///////////////////////////////////////////////////////////////////////////////
// 2007-08-02 by cmkwon, марка бригады, маскирующая материализацию системы - добавлена последовательность.
#define STRMSG_070802_0001    "Знак бригады успешно зарегистрирован."
#define STRMSG_070802_0002    "Регистрация будет завершена по окончании процесса сканирования."
#define STRMSG_070802_0003    "Вы принимаете выбранный пилотом %d знак бригады?"
#define STRMSG_070802_0004    "Нет статуса знака бригады."
#define STRMSG_070802_0005    "Знак бригады в статусе ожидания."
#define STRMSG_070802_0006    "Знак бригады в нормальном статусе."
#define STRMSG_070802_0007    "Знак бригады в статусе ошибки."

// 2007-08-24 by cmkwon, ЅєЗЗДїѕЖАМЕЫ »зїл °ЎґЙ/±ЭБц јіБ¤ ±вґЙ ГЯ°Ў - ён·Йѕо ГЯ°Ў
#define STRCMD_CS_COMMAND_UsableSpeakerItem_0			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_1			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_2			"/UseSpeaker"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_P1Able		"Enable"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_P1Forbid		"Forbid"
#define STRCMD_CS_COMMAND_UsableSpeakerItem_HELP		"Формат: /UseSpeaker [Enable|Forbid] - Запрет/разрешение на использование спикера;"

// 2007-08-27 by cmkwon, PrepareShutdown command(GM can shutdown game server in SCAdminTool)
#define STRCMD_CS_COMMAND_PrepareShutdown_0				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_1				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_2				"/PrepareShutdown"
#define STRCMD_CS_COMMAND_PrepareShutdown_P1Start		"Start"
#define STRCMD_CS_COMMAND_PrepareShutdown_P1Release		"Release"
#define STRCMD_CS_COMMAND_PrepareShutdown_HELP			"Формат: /PrepareShutdown [Start|Release] - Подготовка сервера к остановке, отключает всех пользователей;"

// 2007-08-30 by cmkwon, ИёАЗ·л ЅГЅєЕЫ ±ёЗц - ён·Йѕо ГЯ°Ў
#define STRCMD_CS_COMMAND_EntrancePermission_0                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_1                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_2                                     "/EntrancePermission"
#define STRCMD_CS_COMMAND_EntrancePermission_HELP                      "Формат: /EntrancePermission [|Имя_игрока] - Может быть использована только лидером, позволяет персонажу быть занесенным в список на вход в конференц-зал;"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_0                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_1                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_2                               "/EntrancePermissionDeny"
#define STRCMD_CS_COMMAND_EntrancePermissionDeny_HELP                "Формат: /EntrancePermissionDeny [Имя_игрока] - Может быть использована только лидером, удаляет персонажа из списка на вход в конференц-зал;"

// 2007-10-05 by cmkwon, разные для каждой фракции.
#define STRCMD_071005_0000					"%ddyas %dhour(s) %dminute(s) %dsecond(s)", Day, Hour, Minute, Second  // Day, Hour, Minute, Second is parameter name. don't need to translate.
#define STRCMD_071005_0001					"%dYear %dmonth %dday", Year, Month, Day //Year, Month, Day is parameter name. don't need to translate.
#define STRCMD_071005_0002					"%dYear %dmonth", Year, Month // Year, Month is parameter name. don't need to translate.
#define STRCMD_071005_0003					"%dMonth %dday", Month, Day // Month, Day is parameter name. don't need to translate.


// 2007-10-06 by cmkwon, Установить имена двух сублидеров каждой фракции
#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER_1		"Сублидер 1" // общее для обеих фракций.
#define STRCMD_CS_COMMON_RACE_INFLUENCE_SUBLEADER_2		"Сублидер 2" // общее для обеих фракций.
#define STRCMD_VCN_INFLUENCE_LEADER						"Главнокомандующий"
#define STRCMD_VCN_INFLUENCE_SUBLEADER_1				"Зам. командующего"
#define STRCMD_VCN_INFLUENCE_SUBLEADER_2				"Начальник штаба"
#define STRCMD_ANI_INFLUENCE_LEADER					"Председатель"
#define STRCMD_ANI_INFLUENCE_SUBLEADER_1				"Вице-председатель"
#define STRCMD_ANI_INFLUENCE_SUBLEADER_2				"Стратег"
#define STRCMD_OUTPOST_GUILD_MASTER					"Владелец %s"

// 2007-10-06 by dhjin, command to set 2 sub-leader
#define STRCMD_CS_COMMAND_SUBLEADER1_0				"/appointment1"
#define STRCMD_CS_COMMAND_SUBLEADER1_1				"/appointment1"
#define STRCMD_CS_COMMAND_SUBLEADER1_HELP			"Формат: /appointment1 [Имя_игрока] - BCU : установить имя 1-го сублидера фракции Бигенью, ANI : установить имя 1-го сублидера фракции Арлингтон;"
#define STRCMD_CS_COMMAND_SUBLEADER2_0				"/appointment2"
#define STRCMD_CS_COMMAND_SUBLEADER2_1				"/appointment2"
#define STRCMD_CS_COMMAND_SUBLEADER2_HELP			"Формат: /appointment2 [Имя_игрока] - BCU : установить имя 2-го сублидера фракции Бигенью, ANI : установить имя 2-го сублидера фракции Арлингтон;"

// 2007-10-30 by cmkwon, ивент "Счастливые часы" для каждой фракции - команда изменена.
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT				"/HappyEvent"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_1			"/happyevent"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PSTART	"Start"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_PEND		"End"
#define STRCMD_CS_COMMAND_HAPPYHOUREVENT_HELP		"Формат: /HappyEvent [255(Все_нации)|0(Новичок)|2(Бигенью)|4(Арглинтон)] [Start|End] [срок(минуты)] - Установить счастливый час;"
 
// 2007-10-30 by cmkwon, ивент "Счастливые часы" для каждой фракции - по старой системе.
#define STRCMD_CS_INFLUENCE_TYPE_NORMAL					"Фракция"
#define STRCMD_CS_INFLUENCE_TYPE_VCN					"Бигенью"
#define STRCMD_CS_INFLUENCE_TYPE_ANI					"Арлингтон"
#define STRCMD_CS_INFLUENCE_TYPE_ALL_MASK					"Все фракции"  // 2007-10-30 by cmkwon, Ивент "Счастливые часы" для каждой фракции - дополнение
 
// 2007-11-05 by cmkwon, Очки Войны награда - команда добавлена
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_0			"/AddWarPointInMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_1			"/WPAddedMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_2			"/WPAddedMap"
#define STRCMD_CS_COMMAND_WAPPOINTINMAP_HELP		"Формат: /WPAddedMap или /WPAddedMap [1(Новичок)|2(Бигенью)|4(Арглинтон)|3|5|6|7] [Кол-во_Очков_Войны(1~)] - Игроки выбранной фракции на данной карте получают Очки Войны;"

// 2007-11-19 by cmkwon, система вызова ГМ - новая команда
#define STRCMD_CS_COMMAND_STARTCALLGM_0			"/StartCallGM"
#define STRCMD_CS_COMMAND_STARTCALLGM_1			"/StartHelper"
#define STRCMD_CS_COMMAND_STARTCALLGM_2			"/StartHelp"
#define STRCMD_CS_COMMAND_STARTCALLGM_HELP		"Формат: /StartCallGM [|Время(минуты)] - Начать работу системы петиций;"
#define STRCMD_CS_COMMAND_ENDCALLGM_0				"/EndCallGM"
#define STRCMD_CS_COMMAND_ENDCALLGM_1				"/EndHelper"
#define STRCMD_CS_COMMAND_ENDCALLGM_2				"/EndHelp"
#define STRCMD_CS_COMMAND_ENDCALLGM_HELP			"Формат: /EndCallGM - Прекратить работу системы петиций;"

// 2007-12-27 by cmkwon, А©µµїмБо ёрµе ±вґЙ ГЯ°Ў - STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW ГЯ°Ў
#define STRMSG_REG_KEY_NAME_WINDOWDEGREE_NEW		"WindowDegreeNew"

// 2008-01-03 by cmkwon, А©µµїмёрµе »уЕВ АъАеЗП±в - 
#define STRMSG_REG_KEY_NAME_WINDOWMODE				"WindowMode"
#define STRMSG_REG_KEY_NAME_VSYNC					"VSync"

// 2008-01-31 by cmkwon, °иБ¤ єн·°/ЗШБ¦ ён·Йѕо·О °ЎґЙЗС ЅГЅєЕЫ ±ёЗц - ён·Йѕо ГЯ°Ў
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_0					"/Block"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_1					"/BlockAccount"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_2					"/ban"
#define STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_HELP				"Формат: /Block или /BlockAccount [Имя_аккаунта] [Тип_бана(1:Обычный|2:Деньги|3:Предметы|4:SpeedHack|5:Чат|6:Ошибки_игры)] [Период(дни)] [Причина_блокировки_для_пользователя] [Причина_блокировки_для_администрации];"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_0				"/Unblock"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_1				"/UnblockAccount"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_2				"/unban"
#define STRCMD_CS_COMMAND_NEWACCOUNTUNBLOCK_HELP			"Формат: /Unblock или /UnblockAccount [Имя_аккаунта];"

// 2008-02-20 by cmkwon, ён·ЙѕоГЯ°Ў(БўјУБЯАОАЇАъёрµОїЎ°ФѕЖАМЕЫБц±Ю- 
#define STRCMD_CS_COMMAND_ITEMALLUSER_0                                    "/ItemAllUser"
#define STRCMD_CS_COMMAND_ITEMALLUSER_1                                    "/ItemAll"
#define STRCMD_CS_COMMAND_ITEMALLUSER_2                                    "/AddAllItem"
#define STRCMD_CS_COMMAND_ITEMALLUSER_HELP                               "Формат: /ItemAllUser или /ItemAll или /AddAllItem [1(Новичок)|2(Бигенью)|4(Арлингтон)|255(Все)] [ID_предмета] [Кол-во] - Находящийся в игре пользователь выбранной фракции получит означенный предмет;"

// 2008-02-21 by dhjin, ѕЖ·№іЄЕлЗХ- ѕЖ·№іЄГЯ°Ўён·Йѕо
#define STRCMD_CS_COMMAND_ARENAMOVE_0                                                         "/ArenaMove"
#define STRCMD_CS_COMMAND_ARENAMOVE_1                                                         "/ArenaMove"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_0                                                  "/TeamArenaLeave"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_1                                                  "/TeamServerReturn"
#define STRCMD_CS_COMMAND_TEAMARENALEAVE_HELP                                   "Формат: /TeamArenaLeave [2(СИНИЙ)|4(КРАСНЫЙ)|6(СИНИЙ_И_КРАСНЫЙ)];"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_0                                     "/TargetArenaLeave"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_1                                     "/TargetArenaLeave"
#define STRCMD_CS_COMMAND_TARGETARENALEAVE_HELP                                 "Формат: /TargetArenaLeave [Имя_игрока];"
#define STRCMD_CS_COMMAND_ARENAEVENT_0                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_1                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_2                                                         "/ArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENT_HELP                                                    "Формат: /ArenaEvent [Номер_комнаты];"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_0                                    "/ArenaEventRelease"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_1                                    "/CancelArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_2                                    "/CancelArenaEvent"
#define STRCMD_CS_COMMAND_ARENAEVENTRELEASE_HELP                                "Формат: /ArenaEventRelease [Номер_комнаты];"

// 2008-06-03 by cmkwon, AdminTool, DBTool »зїлЅГ ѕЖАМЕЫ °Л»цЅГ ДЮєё№ЪЅєїЎј­ °Л»ц ±вґЙ ГЯ°Ў(K0000143) - 
#define STRCMD_CS_ITEMKIND_ALL_ITEM							"Все предметы"

//////////////////////////////////////////////////////////////////////////
// 2008-05-20 by dhjin, EP3 - ї©ґЬ јцБ¤ »зЗЧ	// 2006-03-07 by cmkwon
#define STRCMD_CS_GUILD_RANK_PRIVATE_NULL		"Пилот"
#define STRCMD_CS_GUILD_RANK_COMMANDER			"Лидер Бригады"
#define STRCMD_CS_GUILD_RANK_SUBCOMMANDER		"Заместитель лидера"				// 2008-05-20 by dhjin, EP3 - ї©ґЬ јцБ¤ »зЗЧ
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_1		"Лидер группы №1"
#define STRCMD_CS_GUILD_RANK_PRIVATE_1			"Пилот группы №1"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_2		"Лидер группы №2"
#define STRCMD_CS_GUILD_RANK_PRIVATE_2			"Пилот группы №2"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_3		"Лидер группы №3"
#define STRCMD_CS_GUILD_RANK_PRIVATE_3			"Пилот группы №3"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_4		"Лидер группы №4"
#define STRCMD_CS_GUILD_RANK_PRIVATE_4			"Пилот группы №4"
#define STRCMD_CS_GUILD_RANK_SQUAD_LEADER_5		"Лидер группы №5"
#define STRCMD_CS_GUILD_RANK_PRIVATE_5			"Пилот группы №5"

//////////////////////////////////////////////////////////////////////////
// 2008-06-19 by dhjin, EP3 - АьАеБ¤єё
#define STRCMD_COMMAND_WAR_OPTION_0					"/MotherShipInfoOption"
#define STRCMD_COMMAND_WAR_OPTION_1					"/MotherShipInfoOption"

// 2008-08-18 by dhjin, јј·Вё¶Е©АМєҐЖ® 
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_0				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_1				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_2				"/influencemarkevent"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENT_HELP			"Формат: /influencemarkevent [фракция 2(BCU)|4(ANI)]"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_0			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_1			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_2			"/influencemarkeventend"
#define STRCMD_CS_COMMAND_INFLUENCEMARKEVENTEND_HELP		"Формат: /influencemarkeventend"

//////////////////////////////////////////////////////////////////////////
// 2008-08-25 by dhjin, ЕВ±№ PC№ж IPБ¤єё ·Оµщ
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_0				"/PCBangReloadTime"
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_1				"/PCBangReloadTime"
#define STRCMD_CS_COMMAND_PCBANGRELOADTIME_HELP				"Формат: /PCBangreloadtime [Минуты] - 10 мин. - 1440 мин."


// 2008-08-21 by dhjin, АП№Э, ЖЇјц °иБ¤АЗ єОБцµµАЪ АУён Б¦ЗС
#define STRMSG_080821_0001				"Невозможно присвоить данный ранг выбранному персонажу."


// 2008-09-09 by cmkwon, /јј·ВјТИЇ ён·Йѕо АОАЪ ё®ЅєЖ®їЎ ±вѕоЕёАФ ГЯ°Ў - 
#define STRCMD_CS_COMMAND_COMEONINFL_HELP2		"Формат: /ComeOnInfl [1(Новичок)|2(ПВБ)|4(АНК)|255(Все)] [макс. игроков] [0|мин. уровень] [0|макс. уровень] [1(B)|16(M)|256(A)|4096(I)] [Сообщение] - Призывает игроков на ивентовую карту"

// 2008-09-09 by cmkwon, "/kick" ён·Йѕо ГЯ°Ў - 
#define STRCMD_CS_COMMAND_KICK_0							"/Kick"
#define STRCMD_CS_COMMAND_KICK_1							"/Kick"
#define STRCMD_CS_COMMAND_KICK_HELP							"Формат: /Kick [имя] - отключает персонажа."


// 2008-09-12 by cmkwon, "/ёнјє" ён·Йѕо ГЯ°Ў - 
#define STRCMD_CS_COMMAND_ADD_FAME_0							"/Fame"
#define STRCMD_CS_COMMAND_ADD_FAME_1							"/Fame"
#define STRCMD_CS_COMMAND_ADD_FAME_HELP							"Формат: /fame [личные ОС] [бригадные ОС] - увеличивает очки славы."

// 2008-12-30 by cmkwon, БцµµАЪ Г¤ЖГ Б¦ЗС Д«µе ±ёЗц - 
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_0			"/ReleaseLeaderChatBlock"
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_1			"/RLChatBlock"
#define STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_HELP			"format: /ReleaseLeaderChatBlock [Имя_игрока] - Снятие блокировки."

// 2009-10-12 by cmkwon, ѓtѓЉЃ[ѓXѓJ‚МЌнЏњ€Д“K—p - 
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_0                       "/StartCityMap"
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_1                       "/StartCityMap"
#define STRCMD_CS_COMMAND_CHANGE_StartCityMapIndex_HELP                              "Формат: /StartCityMap [2001|2002] [|CharacterName] - установка начального города для игрока."

#define STRCMD_CS_COMMAND_TOGGLE_CNC						"/cnc"
#define STRCMD_CS_COMMAND_TOGGLE_CNC_HELP						"format: /cnc - Toggle the CNC status (\\gEnabled\\g/\\rDisabled\\r)"

///////////////////////////////////////////////////////////////////////////////
// 2010-01-08 by cmkwon, Дополнительная информация, основанная на максимальном уровне.)
#define STRCMD_CS_CHARACTER_96_LEVEL_RANK "Центурион"
#define STRCMD_CS_CHARACTER_100_LEVEL_RANK "Полковник"
#define STRCMD_CS_CHARACTER_104_LEVEL_RANK "Генерал"
#define STRCMD_CS_CHARACTER_108_LEVEL_RANK "Комендант"
#define STRCMD_CS_CHARACTER_112_LEVEL_RANK "Завоеватель"
#define STRCMD_CS_CHARACTER_116_LEVEL_RANK "Защитник"
#define STRCMD_CS_CHARACTER_120_LEVEL_RANK "Легенда"
#define STRCMD_CS_CHARACTER_XX_LEVEL_RANK "Игрок"
#define STRCMD_CS_COMMAND_ENDARENA				"/endarena"				// 2012-09-24 by jhseol, ѕЖ·№іЄ Бѕ·б ён·Йѕо јцБ¤

// 2014-05-16 by bckim, ?? ??? ??
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_TIME		"/resetreversalbufftime"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_STATE		"/resetreversalbuffstate"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_EXECUTE	"/resetreversalbuffexecute"
#define STRCMD_CS_COMMAND_RESET_REVERSAL_BUFF_END		"/resetreversalbuffend"
#define STRCMD_CS_COMMAND_COMPAT_POWER					"/compatpower"
// End. 2014-05-16 by bckim, ?? ??? ??

///////////////////////////////////////////////////////////////////////////////
// 2012-11-29 by jhseol, OX??? ? ???? - ??? ??
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_0			"/MapWarpRestrict"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_1			"/?????"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_HELP		"format: /????? [MapIndex] - ?? ?? ?? ??? ???"
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_RESULT_0	"[%d]?? ?? ??? ?? ?????."
#define STRCMD_CS_COMMAND_MAP_WARP_RESTRICT_RESULT_1	"[%d]?? ?? ??? ?? ?????."
#define STRCMD_CS_COMMAND_MAP_WARP_ERROR				"[%d]? ???? ?? ??????."
// end 2012-11-29 by jhseol, OX??? ? ???? - ??? ??

///////////////////////////////////////////////////////////////////////////////
// 2013-07-26 by jhseol, ? ?? ????
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_0			"/AccountChangeNation"
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_1			"/AccountChangeNation"
#define STRCMD_CS_COMMAND_ACCOUNT_CHANGEINFL_HELP		"format: /AccountChangeNation [accountUID] [2(BCU)|4(ANI)]"

// 2013-11-19 by bckim, ??? ?? ?? ??? ??
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_0			"/formationinfo"
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_1			"/formationinfo"
#define STRCMD_CS_COMMAND_PARTYMEMBER_INFO_HELP			"format: /formationinfo [CharacterName] - Information on players formation member's (member's must log inside the game)"
// End. 2013-11-19 by bckim, ??? ?? ?? ??? ??

// 2015-11-25 Future, Multiple IP Restriction System
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_0		"/IPUnique"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_1		"/IPBlock"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_0	"format: /IPUnique - Gets the current Multiple IP Restriction Status from the Server"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_1	"format: /IPUnique - [(TRUE|ENABLE|1)|(FALSE|DISABLE|0)]"
#define STRCMD_CS_COMMAND_MULTI_IP_RESTRICTION_HELP_2	"- Sets the current Multiple IP Restriction Status and kicks all double logged users"

// 2016-01-03 Future, leader warp restriction
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_0			"/ForbidWarp"
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_1			"/RestrictWarp"
#define STRCMD_CS_COMMAND_LEADER_FORBID_WARP_HELP		"format: /ForbidWarp [PlayerName] - Towns the given player and restricts his ability to warp for 30 minutes"
#define STRCMD_CS_COMMAND_LEADER_RELEASE_WARP			"/ReleaseWarp"
#define STRCMD_CS_COMMAND_LEADER_RELEASE_WARP_HELP		"format: /ReleaseWarp [PlayerName] - Releases the warping restriction from a restricted player"

#endif // end_#ifndef _STRING_DEFINE_COMMON_H_





