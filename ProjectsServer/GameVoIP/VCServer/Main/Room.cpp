// RoomManager.cpp: implementation of the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Room.h"
#include "VCIOCPSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CVCIOCP * CRoom::ms_pVCIOCP2 = NULL;


//////////////////////////////////////////////////////////////////////
// CRoom
//////////////////////////////////////////////////////////////////////
CRoom::CRoom()
{
	InitializeCriticalSection(&m_csRoom);
	m_idRoomID				= 0;
	m_dwMemberCounts		= 0;
	m_dwMaxChannelCounts	= SIZE_MAX_CHANNEL_COUNT;
	memset(m_ArrayIOCPSocketPtrJoinedUser, 0x00, sizeof(m_ArrayIOCPSocketPtrJoinedUser[0])*m_dwMaxChannelCounts);
	m_nConcurrentTalkingCount	= SIZE_MAX_CHANNEL_COUNT;
	MEMSET_ZERO(m_szManagerUserName, SIZE_MAX_USER_NAME);
}

CRoom::~CRoom()
{
	DeleteCriticalSection(&m_csRoom);
}


void CRoom::ResetRoom(void)
{
	m_dwMemberCounts		= 0;
	memset(m_ArrayIOCPSocketPtrJoinedUser, 0x00, sizeof(m_ArrayIOCPSocketPtrJoinedUser[0])*m_dwMaxChannelCounts);
	m_nConcurrentTalkingCount	= SIZE_MAX_CHANNEL_COUNT;
	MEMSET_ZERO(m_szManagerUserName, SIZE_MAX_USER_NAME);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CRoom::SetConcurrentTalkingCount(int i_nCount)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CRoom::SetConcurrentTalkingCount(int i_nCount)
{
	m_nConcurrentTalkingCount = min(SIZE_MAX_TALKING_USER_COUNT, max(i_nCount, 1));		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 동시 말하기 최대 인원 제한
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CRoom::GetConcurrentTalkingCount(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CRoom::GetConcurrentTalkingCount(void)
{
	return m_nConcurrentTalkingCount;
}

void CRoom::SetManagerUserName(char *i_szUserName)
{
	strncpy(m_szManagerUserName, i_szUserName, SIZE_MAX_USER_NAME);
}


DWORD CRoom::PutMemberIntoRoom(CVCIOCPSocket *pRoute)
{
	this->LockRoom();
	if(m_dwMemberCounts < m_dwMaxChannelCounts)
	{
		for(int i=0; i < m_dwMaxChannelCounts; i++)
		{
			if(m_ArrayIOCPSocketPtrJoinedUser[i] == NULL)
			{
				m_dwMemberCounts++;
				m_ArrayIOCPSocketPtrJoinedUser[i] = pRoute;
				this->UnlockRoom();
				return i;
			}
		}
	}
	this->UnlockRoom();
	return UINT_MAX;
}

BOOL CRoom::DeleteMemberFromRoom(CVCIOCPSocket *pRoute)
{
	BOOL nRet = FALSE;
	if(0 == m_dwMemberCounts){			return nRet;}
	
	this->LockRoom();
	for(int i=0; i < m_dwMaxChannelCounts; i++)
	{
		if(m_ArrayIOCPSocketPtrJoinedUser[i] == pRoute)
		{
			m_dwMemberCounts--;
			m_ArrayIOCPSocketPtrJoinedUser[i] = NULL;
			nRet = TRUE;
			break;
		}
	}
	this->UnlockRoom();

	return nRet;
}

void CRoom::SendDataAllMember(BYTE *pData, int nSize, CVCIOCPSocket *i_pExcludeRoute/*=NULL*/)
{
	CVCIOCPSocket *pArrSocket[SIZE_MAX_CHANNEL_COUNT];
	LockRoom();
	memcpy(pArrSocket, m_ArrayIOCPSocketPtrJoinedUser, sizeof(pArrSocket[0])*SIZE_MAX_CHANNEL_COUNT);
	UnlockRoom();
	for(int i=0; i < m_dwMaxChannelCounts; i++)
	{
		if(pArrSocket[i]
			&& pArrSocket[i]->IsUsing()
			&& VOIP_STATE_VC_Joined_Room == pArrSocket[i]->GetVoIPState()	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
			&& pArrSocket[i] != i_pExcludeRoute)
		{
			pArrSocket[i]->SendAddData(pData, nSize);
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CRoom::GetTalkingToken(CVCIOCPSocket *i_pVCISock)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CRoom::GetTalkingToken(CVCIOCPSocket *i_pVCISock)
{
	if(SIZE_MAX_CHANNEL_COUNT == GetConcurrentTalkingCount())
	{// 모두 동시 말하기 가능이면 항상 TRUE를 리턴한다
		return TRUE;
	}

	CVCIOCPSocket *pArrSocket[SIZE_MAX_CHANNEL_COUNT];
	LockRoom();
	memcpy(pArrSocket, m_ArrayIOCPSocketPtrJoinedUser, sizeof(pArrSocket[0])*SIZE_MAX_CHANNEL_COUNT);
	UnlockRoom();

	int nCurrTalkingCounts = 0;
	for(int i=0; i < m_dwMaxChannelCounts; i++)
	{
		if(pArrSocket[i]
			&& pArrSocket[i] != i_pVCISock
			&& pArrSocket[i]->IsUsing()
			&& pArrSocket[i]->GetEnableTalking())	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 기존( && pArrSocket[i]->IsTalking()) )
		{
			nCurrTalkingCounts++;			
		}

		if(nCurrTalkingCounts >= this->GetConcurrentTalkingCount())
		{// 동시 말하기 카운트와 같을 때

			if(strncmp(i_pVCISock->GetUserName(), m_szManagerUserName, SIZE_MAX_USER_NAME))
			{// 관리자가 아닐 경우

				return FALSE;
			}

			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
			//pArrSocket[i]->SetTalking(FALSE);
			pArrSocket[i]->SetEnableTalking(FALSE);
			
			BYTE SendBuf[128];
			///////////////////////////////////////////////////////////////////////////////
			// 전송 받은 RTP 데이타를 다른 유저들에게 전송한다	
			INIT_MSG(MSG_C2S_LOST_TOKEN, T_C2S_LOST_TOKEN, pSMsg, SendBuf);
			pSMsg->nChannelIndex	= pArrSocket[i]->GetRoomChannelIndex();			
			strncpy(pSMsg->szUserName, pArrSocket[i]->GetUserName(), SIZE_MAX_USER_NAME);
			this->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_LOST_TOKEN));

			break;
		}
	}

	return TRUE;
}