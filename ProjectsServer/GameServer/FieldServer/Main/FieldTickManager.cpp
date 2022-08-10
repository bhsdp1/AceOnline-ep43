#include "stdafx.h"
#include "FieldTickManager.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"	// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급 - 로그를 남기기 위해 해더 포함

CFieldTickManager::CFieldTickManager(CFieldIOCP *i_pFieldIOCP, DWORD i_nTickInterval /* = FIELD_DEFAULT_TICK_INTERVAL */)
: CTickManager(i_nTickInterval)
{
	m_pFieldIOCP6 = i_pFieldIOCP;
}

CFieldTickManager::~CFieldTickManager()
{
}

void CFieldTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif
	
	// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	switch( pTickEvent->TickEventType )
	{
	case SM_TICK_EVENT_SUB_ITEM_EVENT:
		{
			if ( TRUE == g_pFieldGlobal->IsArenaServer() )	// 아레나 서버에선 서브타임 이벤트 아이템을 지급하지 않는다.
			{
				return;
			}
			int nItemEventUID = pTickEvent->nTickEventParam1;
			UID32_t nCharacterUID = pTickEvent->nTickEventParam2;
			int nPlayTimeBySecond = (int)pTickEvent->vpTickEventParam1;
			CFieldIOCPSocket *pSock = m_pFieldIOCP6->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID, TRUE);
			if ( NULL == pSock )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error1] CFieldTickManager::DoTickEvent, CUID(%8d) pSock Is NULL \r\n", nCharacterUID);
				return;
			}

			ATUM_DATE_TIME atCurrentDateTime;
			atCurrentDateTime.SetCurrentDateTime();
			if ( nPlayTimeBySecond > atCurrentDateTime.GetTimeDiffTimeInSeconds(pSock->m_character.LastStartedTime) )
			{
				char timestr[SIZE_MAX_SQL_DATETIME_STRING] = {0,};
				pSock->m_atLoginDate.GetSQLDateTimeString(timestr, SIZE_MAX_SQL_DATETIME_STRING);
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error2] CFieldTickManager::DoTickEvent, CUID(%8d) Re Start User (%s) EventTime(%d)\r\n", nCharacterUID, timestr, nPlayTimeBySecond);
				return;
			}

			SITEM_EVENT_INFO* ItemEventInfo = m_pFieldIOCP6->m_pGiveItemManager->GetItemEventInfoByEventUID(nItemEventUID);
			if ( NULL == ItemEventInfo )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Error3] CFieldTickManager::DoTickEvent, CUID(%8d) ItemEevntUID(%4d) ItemEventInfo Is NULL \r\n",nCharacterUID, nItemEventUID);
				return;
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[S_ITEM_EVENT_SUB_TYPE_JHSEOL][Notify] CFieldTickManager::DoTickEvent, Run TickEvent AUID(%8d) CUID(%8d) ItemEevntUID(%4d) \r\n"
				, pSock->m_character.AccountUniqueNumber, pSock->m_character.CharacterUniqueNumber, nItemEventUID);
			if ( TRUE == m_pFieldIOCP6->ItemEventSubTypeCheck(pSock->m_character.AccountUniqueNumber, pSock->m_character.CharacterUniqueNumber, *ItemEventInfo, ITEM_EVENT_DES_PLAY_TIME) )
			{
				// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
				if( TRUE== pSock->IsValidCharacter(FALSE, TRUE) )
				{
					QPARAM_CHECK_EVENTITEM *pQParam = new QPARAM_CHECK_EVENTITEM;
					pQParam->AccountUID		= pSock->m_character.AccountUniqueNumber;
					pQParam->ItemEventUID	= ItemEventInfo->ItemEventUID;
					pQParam->ItemEventType  = ItemEventInfo->ItemEventType;
					pQParam->ItemNum		= ItemEventInfo->ItemNum;
					pQParam->Count			= ItemEventInfo->Count;
					pQParam->PrefixCodeNum	= ItemEventInfo->PrefixCodeNum;
					pQParam->SuffixCodeNum	= ItemEventInfo->SuffixCodeNum;
					pQParam->CharacterUID	= (FALSE == ItemEventInfo->CheckWithCharacterUID) ? 0 : pSock->m_character.CharacterUniqueNumber;	// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
					if ( CS_ARENASERVER_PLAYING == pSock->GetClientState() )
					{
						pQParam->InsertStorage = ITEM_IN_STORE;		// 아레나서버 이용중이면 창고로 아이템을 넣어준다.
					}
					m_pFieldIOCP6->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CheckEventItem, pSock, pSock->m_character.AccountUniqueNumber, pQParam);
				}
				// end 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
			}
		}
		break;
	default:
		{
		}
		break;
	}
#endif
	// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
	return;
}

void CFieldTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CFieldTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->ResetAllP2PPKFamePoint();
}

void CFieldTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoHourlyWorkIOCP(pDateTime);
}

void CFieldTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoMinutelyWorkIOCP(pDateTime);
#ifdef _AUTO_COH
	if (pDateTime->Minute == 0 || pDateTime->Minute == 30 || pDateTime->Minute == 1 || pDateTime->Minute == 31)
	{
		if (!m_pFieldIOCP6->m_InflWarManager.IsDoingInfluenceWar())
		{
			int maps[] = {3008};
			char* messages[] = {"\\rPvP\\r","\\ePvP\\e","\\\cPvP\\c","\\yPvP\\y"};
			SCALL_WARP_EVENT tmCallWarpEvent;
			MEMSET_ZERO(&tmCallWarpEvent, sizeof(SCALL_WARP_EVENT));
			tmCallWarpEvent.byPermissionInfluenceMask = INFLUENCE_TYPE_VCN | INFLUENCE_TYPE_ANI;
			tmCallWarpEvent.dwCallWarpEventID = timeGetTime();
			tmCallWarpEvent.dwSupportTermTick = 10000;
			tmCallWarpEvent.vWarpPosition = D3DXVECTOR3(5120.0f, 900.0f, 5120.0f);
			tmCallWarpEvent.CallMapChannelIndex = MAP_CHANNEL_INDEX(maps[rand() % (sizeof(maps) / sizeof(maps[0]))], 0);
			tmCallWarpEvent.nPermissionUserCnts = 1000;
			tmCallWarpEvent.byPermissionMinLevel = 0;
			tmCallWarpEvent.byPermissionMaxLevel = 0;
			tmCallWarpEvent.usReqUnitKind = UNITKIND_ALL_MASK;

			m_pFieldIOCP6->AddCallWarpEventRequest(&tmCallWarpEvent);

			INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST, T_FC_EVENT_CALL_WARP_EVENT_REQUEST, pSCallWarp, SendBuf);
			pSCallWarp->dwCallWarpEventID0 = tmCallWarpEvent.dwCallWarpEventID;
			pSCallWarp->dwSupportTermTick0 = tmCallWarpEvent.dwSupportTermTick;
			STRNCPY_MEMSET(pSCallWarp->szUserMessage, messages[rand() % (sizeof(messages) / sizeof(messages[0]))], SIZE_STRING_128);
			m_pFieldIOCP6->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST), tmCallWarpEvent.byPermissionInfluenceMask
				, tmCallWarpEvent.byPermissionMinLevel, tmCallWarpEvent.byPermissionMaxLevel, &tmCallWarpEvent.CallMapChannelIndex, FALSE, TRUE, tmCallWarpEvent.usReqUnitKind);
		}
	}
#endif
}

void CFieldTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pFieldIOCP6->OnDoSecondlyWorkFIOCP(pDateTime);
}
