#include "stdafx.h"
#include "ArmorCollection.h"
#include "FieldDataManager.h"
#include "FieldIOCPSocket.h"
#include "AtumLogSender.h"
#include "FieldGlobal.h"

CArmorCollection::CArmorCollection()
{
	m_CollectionList.clear();
	m_CollectionList.reserve(200);
}

void CArmorCollection::CollectionLoad()
{
	//	디비 로드 및 초기값 컬렉션 리스트 클라이언트 전송.(데이터 가공없이 일단 보내줘)	
	if ( NULL != m_pFieldIOCPSocket && NULL != m_pFieldDBManager )
	{
		m_pFieldDBManager->MakeAndEnqueueQuery(QT_CollectionArmorListLoad, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, NULL, NULL, 1);
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "  [NOTIFY] SC_COLLECTION_ARMOR_JHSEOL_BCKIM, CArmorCollection::CollectionLoad() - AUID(%d) CUID(%d) CNAME(%d)\r\n",
			m_pFieldIOCPSocket->m_character.AccountUniqueNumber, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, m_pFieldIOCPSocket->m_character.CharacterName);
	}
}

void CArmorCollection::CollectionUpdate(COLLECTION_INFO *i_pCollectioninfo)
{
	if ( NULL != m_pFieldIOCPSocket && NULL != m_pFieldDBManager && NULL != i_pCollectioninfo )
	{
		COLLECTION_INFO *pQParamCollectionInfo = new COLLECTION_INFO;
		MEMSET_ZERO(pQParamCollectionInfo, sizeof(COLLECTION_INFO));
		memcpy(pQParamCollectionInfo, i_pCollectioninfo, sizeof(COLLECTION_INFO));
		m_pFieldDBManager->MakeAndEnqueueQuery(QT_CollectionArmorUpdate, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamCollectionInfo);
	}
}

void CArmorCollection::CollectionAdd(COLLECTION_INFO* i_pCollectioninfo, BOOL i_bDB_Update /* = FALSE*/)
{
	if ( NULL != i_pCollectioninfo )
	{
		COLLECTION_INFO tmCollectionInfo;
		MEMSET_ZERO(&tmCollectionInfo, sizeof(tmCollectionInfo));
		memcpy(&tmCollectionInfo, i_pCollectioninfo, sizeof(tmCollectionInfo));
		m_CollectionList.push_back(tmCollectionInfo);
		
		if ( TRUE == i_bDB_Update )
		{
			CollectionUpdate(GetCollectionInfoPtr(i_pCollectioninfo->ShapeNum));
			CAtumLogSender::SendLogMessageCollectionState(m_pFieldIOCPSocket, &tmCollectionInfo, GAMELOG_COLLECTION_STATE_ADD);
		}
	}
}

void CArmorCollection::CollectionEnchant(COLLECTION_INFO* i_pCollectioninfo, BYTE i_nAddEnchantLevel, UINT i_nRemainSeconds)
{
	if( NULL == i_pCollectioninfo )
	{
		return ;
	}

	i_pCollectioninfo->EnchantLevel = min(SHAPE_STAT_MAX_LEVEL, (i_pCollectioninfo->EnchantLevel + i_nAddEnchantLevel));
	i_pCollectioninfo->RemainSeconds = i_nRemainSeconds;
	if ( 0 < i_pCollectioninfo->ActivedCount )
	{
		SetEndTime(i_pCollectioninfo);
	}
	CollectionUpdate(i_pCollectioninfo);
	CAtumLogSender::SendLogMessageCollectionState(m_pFieldIOCPSocket, i_pCollectioninfo, GAMELOG_COLLECTION_STATE_ENCHANT);
}

COLLECTION_INFO* CArmorCollection::ChangeActivedCount(ItemNum_t i_nShapeNum, BOOL i_bIncrease)
{
	COLLECTION_INFO* temp =  GetCollectionInfoPtr(i_nShapeNum);

	if( NULL == temp )
	{
		return NULL;
	}

	if ( TRUE == i_bIncrease )
	{
		temp->ActivedCount++;
		if ( 1 == temp->ActivedCount )
		{
			SetEndTime(temp);
			if ( NULL != m_pFieldIOCPSocket )
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_INFO, T_FC_COLLECTION_INFO, pCollectionInfo, SendBuf);
				memcpy(pCollectionInfo, temp, sizeof(COLLECTION_INFO));
				m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_INFO));
				CAtumLogSender::SendLogMessageCollectionState(m_pFieldIOCPSocket, temp, GAMELOG_COLLECTION_STATE_TIME_START);
			}
		}
	}
	else
	{
		temp->ActivedCount = max(0, (temp->ActivedCount - 1));
		if ( 0 == temp->ActivedCount )
		{
			ReFreshRemainSeconds(temp);
			CAtumLogSender::SendLogMessageCollectionState(m_pFieldIOCPSocket, temp, GAMELOG_COLLECTION_STATE_TIME_END);
		}
	}

	return temp;
}

void CArmorCollection::SetEndTime(COLLECTION_INFO* i_pCollectionInfo)
{
	if ( NULL != i_pCollectionInfo )
	{
		i_pCollectionInfo->EndTime.SetCurrentDateTime(FALSE);
		i_pCollectionInfo->EndTime.AddDateTime(0,0,0,0,0,i_pCollectionInfo->RemainSeconds);
	}
}

void CArmorCollection::ReFreshRemainSeconds(COLLECTION_INFO* i_pCollectionInfo)
{
	ATUM_DATE_TIME atCurrentTime(TRUE);
	i_pCollectionInfo->RemainSeconds = (UINT)max(0, i_pCollectionInfo->EndTime.GetTimeDiffTimeInSeconds(atCurrentTime));
	i_pCollectionInfo->EndTime.Reset();
	CollectionUpdate(i_pCollectionInfo);
}

COLLECTION_INFO* CArmorCollection::GetCollectionInfoPtr(ItemNum_t i_nShapeNum)
{
	COLLECTION_LIST::iterator itr = m_CollectionList.begin();
	for ( ; itr != m_CollectionList.end() ; itr++ )
	{
		if ( i_nShapeNum == itr->ShapeNum )
		{
			return &*itr;
		}
	}
	return NULL;
}

ATUM_DATE_TIME CArmorCollection::GetEndTime(ItemNum_t i_nShapeNum)
{
	ATUM_DATE_TIME tmCurrentTime(TRUE);
	COLLECTION_LIST::iterator itr = m_CollectionList.begin();
	for ( ; itr != m_CollectionList.end() ; itr++ )
	{
		if ( i_nShapeNum == itr->ShapeNum )
		{
			if ( 0 < itr->ActivedCount )
			{
				tmCurrentTime = itr->EndTime;
			}
		}
	}
	return tmCurrentTime;
}

void CArmorCollection::CollectionFinish()
{
	COLLECTION_LIST::iterator itr = m_CollectionList.begin();
	for ( ; itr != m_CollectionList.end() ; itr++ )
	{
		if ( 0 < itr->ActivedCount )
		{
			ReFreshRemainSeconds(&*itr);
		}
	}
	m_CollectionList.clear();
}

void CArmorCollection::SendCollectionInfoAllData()
{
	if ( NULL == m_pFieldIOCPSocket )
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "  [ERROR] SC_COLLECTION_ARMOR_JHSEOL_BCKIM, CArmorCollection::SendCollectionInfoAllData() - sock is null\r\n");
		return;
	}
	COLLECTION_LIST::iterator itr = m_CollectionList.begin();
	for ( ; itr != m_CollectionList.end() ; itr++ )
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_INFO, T_FC_COLLECTION_INFO, pCollectionInfo, SendBuf);
		memcpy(pCollectionInfo, &*itr, sizeof(COLLECTION_INFO));
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_INFO));
	}
}
