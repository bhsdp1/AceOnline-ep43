#pragma once

//----------------------------------------------------------------------------------------------------------//
// 2015-05-17 Future, New Configuration Style                                                               //
//----------------------------------------------------------------------------------------------------------//
// _DEBUG
// WIKIGAMES_ENG
// ETERNAL_SKIES
// DREAM_ACE
//

#ifdef _DEBUG
	#define CUSTOM_OPTIMIZER_HSSON					// ini 파일 읽어서 게임에 적용
	#define MULTI_LOADER_HSSON						// 클라이언트를 여러개 띄울 수 있도록 수정
	#define GAMEGUARD_NOT_EXECUTE_HSSON				// 게임 가드 실행 안함
	#define _ATUM_DEVELOP							// 서버용) 서버소스상 디버깅일 때 활성화 해줘야 함
	//#define _WARRIOR_ANTICHEAT
	//#define _WARRIORSW_PROCESS_LIST_SENDER //2015-04-01 implement to send process list for dream
#endif // _DEBUG

#define S_ARARIO_HSSON							// 아라리오 채널링

#if defined(_JPN)
#define LANGUAGE_JAPAN								// ??? ??? ???? ???
#include "Str_JPN/StringDefineCommon.h"
#include "Str_JPN/StringDefineServer.h"
#include "Str_JPN/StringDefineClient.h"
#endif

#if defined(_KOR)
#define YEDANG_RELEASE								// ???, ??? ?? ??? ??
#define KOR_INPUT_LANGUAGE_HSSON					// ?? ?? ??
#define KOR_HANGEUL_START_HSSON						// ?? ??? ??
#define KOR_CASHSHOP_INTERFACE_HSSON				// ??? ?????
#define KOR_GAME_RATINGS_HSSON						// ?? ??? ??? ??? ??
#define KOR_CHARACTER_INTERFACE_POS_HSSON			// ??? ?? ?? ??
#include "Str_KOR/StringDefineCommon.h"
#include "Str_KOR/StringDefineServer.h"
#include "Str_KOR/StringDefineClient.h"
#endif

#if defined(_VIE)
#include "Str_VIE/StringDefineCommon.h"
#include "Str_VIE/StringDefineServer.h"
#include "Str_VIE/StringDefineClient.h"
#endif

#if defined(_ENG)
#define S_DA_SERVER_SETTING_HSSON				// 2015-05-18 Future, Dream Ace Configuration
#include "Str_DA/StringDefineCommon.h"
#include "Str_DA/StringDefineServer.h"
#include "Str_DA/StringDefineClient.h"
#endif

#if defined(_RUS)
#include "Str_RUS/StringDefineCommon.h"
#include "Str_RUS/StringDefineServer.h"
#include "Str_RUS/StringDefineClient.h"
#endif

#if defined(_ETRS)
#define S_ETRS_SERVER_SETTING_HSSON				// 2015-05-16 Future, Eternal Skies Configuration
#include "Str_ETRS/StringDefineCommon.h"
#include "Str_ETRS/StringDefineServer.h"
#include "Str_ETRS/StringDefineClient.h"
#endif

//----------------------------------------------------------------------------------------------------------//
// 국가별로 차별화를 두지만 컨텐츠 레벨(진도) 또는 요구(특성)에 따라서 변화 될 작업                         //
//----------------------------------------------------------------------------------------------------------//

//#define S_EP4_TEST_SERVER_HSKIM				// EP4 전달용 테스트 서버
//#define SC_DARK_CRASH_HSSON
//#define SC_DARK_CRASH_FILE_OUTPUT_HSSON
//#define S_GLOG_HSSON
//#define S_AUTO_UPDATE_VERSION_BY_PRESVR_HSSON
//#define S_ARENA_NOT_INFO						// 아레나에서는 할 수 없게 함
//#define S_MANAGER_ADMIN_HSSON					// 게임 마스터도 계정 블럭/언블럭 시킬 수 있도록 수정
// 2013-07-08 게임포지 추가
//#define S_HACK_SHIELD_AUTO_UPDATE_HSKIM		// 핵쉴드 자동 업데이트 사용
//#define S_ADMIN_CHARACTER_LOGIN_IP_CHECK		// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 특수계정은 등록된 아이피가 아니면 접속 차단
//#define C_OBJECT_RESOURCE_ORGANIZE_MSPARK		// 2012-04-18 by mspark, 오브젝트 리소스 정리
//#define C_INTERFACE_RESOURCE_ORGANIZE_ISSHIN	// 2012-04-23 by isshin, 인터페이스 리소스 정리				   
//#define C_USER_CTRL_KEY_LOCK
//#define C_USER_COPYPAST_JHSHN					// not exist in client and in server
//#define C_USER_EFFECT_CONTROL
//#define S_GAMEFORGE_KEY_CHANGE_JHSEOL
//#define S_SELECTIVE_SHUTDOWN_HSKIM
//#define S_BPSOFTBILLING_JHSEOL
//#define S_FULL_LOG_JHSEOL
//#define S_DISABLE_CHECKSUM_RESOURCE
//#define S_OPTION_PARTNER__JHAHN
// ep4-2차
//#define S_ARENA_EX_GAMEFORGE_VERSION_JHSEOL	// 2012-09-21 by jhseol, 아레나 추가개발 GF버전 - 아레나 전용 캐릭터를 사용하되 기본.고급.아머만 삭제하고 대신 본인 케릭터의 장비를 가져오도록 셋팅.
//#define SC_SHUT_DOWNMIN_JHAHN
//#define S_NPLAY_PCBANG_MODULE_JHSEOL
//#define S_ITEM_VIEW_UNCONDITIONAL_ON			// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
//#define C_ITEM_VIEW_UNCONDITIONAL_ON			// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
//#define S_TEMP_PRESERVER_MAKE_HSKIM			// PreServer 이전(아이피 변경) 시에 임시 PreServer 제작용 메크로
//#define TEMP_PRESERVER_MAKE_IP				"182.162.137.2"		// 변경 대상 아이피
//#define S_GLOG_3ND_KHK
//#define S_KOR_BUSAN_MONITORTOOL_MAKE
//#define S_DELETE_BUY_CASH_GIVE				// 2012-11-12 by hskim, 캐나다 선물하기 기능 삭제
//#define C_CASHSHOP_IMAGE_REMOVE				// 2012-11-16 by mspark, 캐나다 선물, 충전 기능 제거
//#define S_KOR_TEST_GLOG_CHARDATA
//#define C_WEB_CASHSHOP
//#define S_WEB_CASHSHOP_JHSEOL
//#define S_MONTHL_CASH_BUY_GIFT_JHSEOL			// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
//#define S_ITEM_EVENT_SUB_TYPE_JHSEOL			// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
//#define S_ITEM_EVENT_RETURN_USER_BCKIM		// 2013-02-28 by bckim, 복귀유져 버프추가
//#define C_ITEM_EVENT_RETURN_USER_BHSOHN		// 2013-03-06 by bhsohn 복귀 유저 시스템
//#define C_FALL_OF_BOOSTER_USE_STOP_MSPARK
//#define SC_SECURITY_COMMAND_HSKIM				// 비밀키 적용 (클라 서버) [선행조건 (인증 서버 적용)]
//#define C_SECURITY_COMMAND_JHAHN				// 비밀키 클라이언트 확인 - 절대 주석 풀지 마세요
//#define _ADVANCED_CONFIG
///////////////////////////////////////////////////////////////////
// 전국가 반영 컨텐츠
#define MULTI_LOADER_HSSON						// allow multiple client instances
#define GUILD_WAREHOUSE_ACCESS_HSSON			// 멤버쉽이 아니더라도 여단창고 이용가능
#define NEW_CASHSHOP_INTERFACE_HSSON			// 신규 캐쉬샵 작업
#define KOR_CASHSHOP_REFILL_WEB_HSSON			// 캐쉬샵 충전 웹페이지 추가
#define WAREHOUSE_SHARES_HSSON					// 캐릭터 간에 창고를 공유 할 수 있음
#define SC_GROWING_PARTNER_HSKIM_JHAHN			// 캐릭터 정보 창에서 파트너UI 활성화
#define S_INFINITY3_HSKIM						// 인피니티 3차 적용 (서버)
#define INFI_QUEST_JSKIM					    // 인피니티 퀘스트
#define MULTI_TARGET_JSKIM						// 멀티타켓
#define S_AUTHENTICATION_SERVER_HSKIM			// 인증 서버 적용 (서버)
#define S_LOGIN_ITEM_EVENT_SHCHO			    // 로그인시 아이템 증정
#define S_EP4_HSKIM								// EP4 1차 (서버)
#define C_EPSODE4_UI_CHANGE_JSKIM				// 2011. 11. 17 by jskim EP4 UI 변경
#define C_EPSODE4_SETTING_JHAHN					// 2011. 11. 17 by jhahn EP4 트리거 시스템 
#define S_AUTHENTICATION_SERVER_2ND_HSKIM		// 인증 서버 2차 적용 (서버) - 서버 접속 불가시 서버 실행 불가
#define S_STATISTICS_HSKIM						// 화폐 통계
#define S_DELETE_DROP_ITEM_HSKIM				// 드랍된 아이템 5분뒤에 삭제
#define S_ARENA_EX_1ST_JHSEOL					// 2012-05-29 by jhseol, 아레나 추가개발 - 맵추가, 자동파티, 편대기능 강화, 판정기능 강화, 보상횟수 제한, ti_arenainfo 필드추가
#define C_ARENA_EX_1ST_MSPARK					// 2012-05-29 by mspark, 아레나 추가개발 - 맵 이미지 추가, 이미지 좌표 변경, 툴팁 추가
#define C_UI_REPAIR_ISSHIN						// 2012-06-21 by isshin UI버그 수정 - Ep4만 적용
#define S_ENEMY_INFO_JHSEOL						// 2012-07-19 by jhseol, 아템미리보기
#define C_ENEMY_INFO_ISSHIN						// 2012-06-14 by isshin 아템미리보기
#define S_TIMEITEM_LOG_HIDE_JHSEOL				// 2012-08-28 by jhseol, 시간제 아이템 관련 로그 삭제 요청
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL		// 2013-02-26 by jhseol, 전쟁 시스템 리뉴얼 부가옵션 - 모선전이 있는 날에도 거점 생성이 가능
//#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL		// 2013-03-18 by jhseol, 거점 생성시간 수정 - 거점 생성시간이 거점전 시작시간에 몰리는 현상을 수정하여 균등하게 분산되도록 함.
#define S_SEARCHEYE_SHUTTLE_POSITION_JHSEOL		// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define C_SEARCHEYE_SHUTTLE_POSITION_MSPARK		// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define S_KOR_TEST_MSWAR_BACK_TO_CITY_HSKIM		// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프 - 한국 먼저 테스트 후 전국가 반영을 위해
//#define _EXTENDED_CHATCOLOR
//#define C_GAME_SINGLE_THREAD_YMJOO
#define _STAFF_WARP_PERMISSIONS
#define _SHOW_INFLUENCE_ON_GMNATION
#define _SHOW_SPEAKER_IN_CHAT
#define _CROSS_NATION_CHAT						// Cross Nation Chat
#define _CROSS_NATION_CHAT_SPI_COST	100000
#define _REWORKED_COLORSHOP
#define _NCP_BASED_MSWAR
#define _ARENA_WP_HH
//#define _DBG_INFO
//#define _SHOW_LATENCY
#define _OUTPOST_BUFFS							//gives special buffs to outpost winners
#define _ARENA_ADDITIONAL_SUPPLY_ITEMS
#define S_MINI_DUMP_HSKIM
#define S_ACCESS_INTERNAL_SERVER_HSSON
#define	S_TRIGGER_OUTPOST_JHSEOL				// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
//#define	S_SKILL_NUMBERING_RENEWAL_JHSEOL	// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정 - 서버용
//#define	C_SKILL_NUMBERING_RENEWAL_BHSOHN	// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정 - 클라용
//#define SC_SHUT_DOWNMIN_SHCHO
//#define S_GM_COMMAND_USE_SHCHO
#define S_TRIGGER_SYSTEM_EX						// 2013-07-08 by jhseol, 트리거 시스템 확장 - 기존 트리거에 전진기지 트리거의 제어기 기능을 추가
#define S_LAUNCHER_USE_ID_PASSWORD_HSKIM
#define C_USER_SCREENSHOTMODE_ISSHIN			//유저 스크린샷 모드 지원
#define S_MS_WAR_FIN_SUPPLY_ITEM_ADD
#define CHECK_SUM_ON
#define C_CANADA_HACKSHEILD_JHAHN				// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전
#define S_SERVER_CRASH_MSGBOX_REMOVE_MSPARK		// 2012-10-18 by mspark, 게임포지 서버 충돌 시 메시지 박스 보이지 않는 작업
#define S_DISABLE_PREPARE_SHUTDOWN				// 2012-11-21 by hskim, PrepareShutdown 기능 삭제
#define C_ADMINISTRATOR_COPYPASTE_PERMISSION	// 2012-11-21 by mspark, 관리자 복사, 붙여넣기 허용
#define S_AUTO_BLOCK_SYSTEM_HSKIM				// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
#define S_ADDITIONAL_EXPERIENCE					// 2012-11-12 by hskim, 휴식 경험치
#define C_WARNING_APPEARANCE_CHANGE				// 2012-10-30 by mspark, 외형 변경 경고 메시지 추가
#define C_ABUSE_FILTER_RELAX					// 2013-04-03 by ssjung 캐나다 욕설 필터 완화
#define S_DB_FULL_LOG_JHSEOL					// 2013-01-04 by jhseol, DB 풀로그 남기기
#define S_REMANING_PROCESS_REMOVE_MSPARK
#define S_DISABLE_ALONE_RUN_MODE_HSKIM			// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
#define S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM	// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
#define SC_PARTNER_SHAPE_CHANGE_HSKIM			// 2012-12-03 by hskim, 파트너 기간제 외형 변경
#define GAMEFORGE4D_CHAT_MACRO_OUTPUT_TIME		// 2013-07-30 by ssjung 게임 포지 일 경우 매크로 딜레이 15초 에서 5분으로 수정

#ifdef _ATUM_CLIENT
	#define C_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL	// 2012-11-22 by jhseol, 전쟁 시스템 리뉴얼 - 전진기지전(OutPost)때 인피 입장 제한
	#define _WIREFRAME
	#define _NOCLIP
	#define _DRAW_EVENTS
	#define _CRASH_HANDLER
	#define _ENHANCED_EXCEPTIONS
	#define _HIDE_PERCENTAGE_ON_MAXLEVEL
	#define _DISABLE_BSTOP_AUTOHORIZONT
	#define _ENHANCED_MIXING_DISPLAY
	#define _SHOW_GUILD_NAME
	#define _SHOW_LEADER_INFO					//Need _SHOW_GUILD_NAME defined to work
	//#define _WARRIOR_CLASS_KILL_BAR			//additional bar with showing current class and percent for next class
	#define _WARRIOR_SHOW_SPHP_4ALL
	#define _WARRIOR_ADDITIONAL_INFO			//show additional info near aim (hp and ammo status)
	#define S_SERVER_CRASH_FILENAME_ADD_INFO	// 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
	//#define CUSTOM_OPTIMIZER_HSSON
	//#define C_CLIENT_FILE_LOG
	//#define _ADVANCED_CONFIG
	#define _INSTANT_LAUNCH
	#define _NO_FADE
#endif

#ifdef _ATUM_SERVER
	#define _AUTO_DGA
	#define _NATION_BUFFS
#endif

#if defined(_REWORKED_COLORSHOP)
#define DEFAULT_COLOR_PRICE	50000				// 2015-06-25 by St0rmy, in case that the coloritem is missing in SSQL Table, the user won't get DC'd when SHOP_PRICES_PER_BUILDING_NPC is defined
#define COLOR_RESET_ITEMNUM 8000000				//This item resets the colorcode to 0 when bought in colorshop (needs to have kind 60)
#endif

#if defined(DREAM_ACE) || defined(ETERNAL_SKIES)
#define S_BONUS_KILL_SYSTEM_RENEWAL				// 2015-06-11 by Future, Bonus System by Kills
#else
#define S_BONUSEXPSYSTEM_RENEWAL				// Counters by Mob Kills (3600 & 300)
#endif

#if defined(DEACTICATION_ALL)					// 모든국가 사용안함
#define S_ARENA_EX_1ST_RESTORE_JHSEOL			// 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#define C_ARENA_EX_1ST_RESTORE_MSPARK			// 2012-09-14 by mspark, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#endif

#ifdef INTECOM_VIET
#define C_CLIENT_LIVE_TIME						// 2012-11-07 by bhsohn 베트남 패킷 처리
#define C_ARENA_PASSWORD_ALPHABET				// 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
#define S_B_GEAR_DAMAGE_PROBABILITY_INFO		// 2013-05-06 by bckim, B기어 타겟 데미지 확인

#endif

#ifndef INTECOM_VIET
#define C_MOUSEFOCUS_BACKGROUND_NOTRESET		// 2012-12-17 by jhjang 게임이 백그라운드일때, 마우스 포커스를 초기화 하지 않도록 수정
#define C_JOIN_CHATROOM_MESSAGE					// 2013-01-08 by mspark, 채팅방에 새로운 유저가 입장했을 경우 시스템 메시지 추가
#define C_LONGTIME_WINDOW_ON					// 2012-12-06 by bhsohn 장기간 window 킨후, 게임접속시 게임 멈추어 있는 현상 처리
#define C_BAZZER_NOT_EXIT						// 2013-01-08 by jhjang 개인상점을 연 상태에서 클라이언트 멈춰있을때 강제 종료 막기 추가
#define C_AGEAR_REFINERYSHOP_CLOSE_MSPARK		// 2013-02-04 by mspark, A기어 보급상점에 랜딩 시 이동 버튼을 누를경우, 보급상점 창이 닫히도록 작업
#define S_BUGFIX_DUPLICATE_ITEM_HSKIM			// 2013-02-21 by hskim, 베트남 아이템 복사 버그 보안 (DB 처리가 끝날때까지 관련 패킷은 무시) - 삭제 주기를 1시간 (3600000) 에서 26.5시간으로 변경 (90000000)
#define C_INGAME_MIX_ITEM						// 2013-02-20 by bhsohn 인게임 조합창 처리
#define S_ADMINTOOL_IP_SEARCH_JHSEOL			// 2013-06-28 by jhseol, 캐나다 어드민툴 IP조회 문제
#define C_SHOP_ITEMCNT_INIT_BHSOHN				// 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가
#define C_DECA_TOOLTIP	
#define S_CANNOT_DO_GIFT_UNDER_LEVEL_BCKIM		// 2013-07-08 by bckim, 캐나다요청 레벨70 미만은 선물하기 불가
#define C_FOCUS_MISS_CLIENT_NO_CLOSE			// 2013-06-28 by ssjung 포커스를 잃었을 시 1분후 클라이언트가 종료되는 현상 캐나다 예외처리
#define C_LEAST_LV_GIFT							// 2013-07-09 by bhsohn 70 레벨 이상만 선물하기 가능하게 수정
#define SC_COLLECTION_ARMOR_JHSEOL_BCKIM		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 컨텐츠 선언
#define C_LABORATORY_UI_CHANGE_JWLEE

#if defined(TEST140) || defined(YEDANG_KOR) || defined(WIKIGAMES_ENG)
// 2013-06-18 한국적용
#define S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 2013-04-18 by jhseol,bckim 이달의 아머 - 컨텐츠 선언
#define C_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM
#endif

#endif

//	#endif
//#define S_WEB_CASHSHOP_SERVER_MODULE_HSKIM		// 2013-03-13 by hskim, 웹 캐시 상점
//#define S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM	// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
// 2013-06-10 한국 추가
//#define S_UPGRADE_INFLUENCE_POINT_HSKIM		// 2013-05-09 by hskim, 세력 포인트 개선
//#define C_UPGRADE_INFLUENCE_POINT_BHSOHN		// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
// 2013-07-09 캐나다, 아르헨, 게임포지 추가		// 2013-07-11 일본 추가 
//#define S_ADMINTOOL_CASHSHOP_REALIGNMENT_BCKIM	// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여
//#define C_ITEM_EVENT_NOTIFY_MSG_CHANGE_JHSEOL		// 2013-04-08 by jhseol, 이벤트 아이템 지급 알림방법 변경
// 2013-04-19 한국적용
// 2013-05-23 한국적용
//#define SC_BUFF_PENALTY_JHSEOL_BCKIM			// 2013-05-07 by jhseol,bckim 버프 패널티
// 2013-06-27 한국적용
//#define S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL		// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
//#define C_SERVER_DOWN_ALARM
//#define S_MULTI_RESOLUTION_JHSEOL 			// 2013-07-30 by jhseol, 멀티해상도 지원


/////////////////////////////////////////////////////////////
// 2015-05-25 Future, new Shop System (Prices per shop, not per Item)
#if defined(ETERNAL_SKIES)|| defined(DREAM_ACE) // add your servers here
	#define SHOP_PRICES_PER_BUILDING_NPC
	#define SCREENSHOT_FILE_FORMAT_JPG			// 2015-06-21 Future, ScreenShots as JPG
#endif

// Kill Rewards
#define _WP_REWARD_PER_KILL
#define WP_REWARD_KILL_MIN_COUNT	50
#define WP_REWARD_KILL_MAX_COUNT	100

#define _SPI_REWARD_PER_KILL
#define SPI_REWARD_KILL_MIN_COUNT	500000
#define SPI_REWARD_KILL_MAX_COUNT	1000000

#define _EXTENDED_KILL_MESSAGE	// 2015-07-22 Future, extended message like: Future was shot down. Reward ... SPI ... WP

////////////////////////////////////////////////////////////
// 2015-09-25 Future, Inactivity Checks
//#define C_DISCONNECT_INACTIVE_PLAYERS
//#define DISCONNECT_INACTIVE_PLAYERS_AFTER_SECONDS	1800	// 30 mins * 60 seconds
//#define DISCONNECT_INACTIVE_PLAYERS_WARNING_TIME	300		// 5 mins * 60 seconds
//#define EP1_CHARACTER