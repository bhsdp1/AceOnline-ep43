// RoomManager.cpp: implementation of the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "RoomManager.h"
#include "Room.h"
#include "VCIOCPSocket.h"


//////////////////////////////////////////////////////////////////////
// CRoomManager
//////////////////////////////////////////////////////////////////////

CRoomManager::CRoomManager(int i_nMaxRoomCount)
{
	m_nMaxRoomCounts	= i_nMaxRoomCount;

	m_mtvectorRoomList.clear();	
	m_pVCIOCP1			= NULL;
}

CRoomManager::~CRoomManager()
{
	for_each(m_mtvectorRoomList.begin(), m_mtvectorRoomList.end(), Delete_Object());
}

void CRoomManager::ResetRoomManager(void)
{
	for_each(m_mtvectorRoomList.begin(), m_mtvectorRoomList.end(), Delete_Object());
}

void CRoomManager::SetRouteIOCP(CVCIOCP *pIOCP)
{
	m_pVCIOCP1			= pIOCP;
}

CRoom *CRoomManager::CreateRoom(RoomID_t i_roomID, int i_nConcurrentTalkingCount, CVCIOCPSocket *i_pCreateVCSocket)
{
	m_mtvectorRoomList.lock();

	CRoom *pTmRoom = this->FindRoomWithRoomID(i_roomID);
	DWORD dwChannIdx = UINT_MAX;
	if(NULL == pTmRoom)
	{
		pTmRoom = new CRoom;
		pTmRoom->SetRoomID(i_roomID);
		pTmRoom->SetConcurrentTalkingCount(i_nConcurrentTalkingCount);
		pTmRoom->SetManagerUserName(i_pCreateVCSocket->GetUserName());
		dwChannIdx = pTmRoom->PutMemberIntoRoom(i_pCreateVCSocket);
		m_mtvectorRoomList.push_back(pTmRoom);
	}
	m_mtvectorRoomList.unlock();
	
	i_pCreateVCSocket->SetJoinedRoom(pTmRoom, dwChannIdx);
	return pTmRoom;
}

CRoom *CRoomManager::JoinRoom(RoomID_t i_roomID, CVCIOCPSocket *i_pJoinVCSocket)
{
	m_mtvectorRoomList.lock();
	CRoom *pTmRoom = this->FindRoomWithRoomID(i_roomID);
	if(NULL == pTmRoom)
	{
		m_mtvectorRoomList.unlock();
		return NULL;
	}
	DWORD dwChannIdx = pTmRoom->PutMemberIntoRoom(i_pJoinVCSocket);
	m_mtvectorRoomList.unlock();

	i_pJoinVCSocket->SetJoinedRoom(pTmRoom, dwChannIdx);
	return pTmRoom;
}

void CRoomManager::DeleteRoom(CRoom *i_pDelRoom)
{
	m_mtvectorRoomList.lock();
	CRoom *pTmRoom = NULL;
	mtvectorRoomPtr::iterator itr(m_mtvectorRoomList.begin());
	for(; itr != m_mtvectorRoomList.end(); itr++)
	{
		pTmRoom = *itr;
		if(i_pDelRoom == pTmRoom)
		{
			SAFE_DELETE(pTmRoom);
			m_mtvectorRoomList.erase(itr);
			break;
		}
	}
	m_mtvectorRoomList.unlock();
}

CRoom *CRoomManager::FindRoomWithRoomID(RoomID_t i_roomID)
{
	m_mtvectorRoomList.lock();
	if(false == m_mtvectorRoomList.empty())
	{		
		mtvectorRoomPtr::iterator itr(m_mtvectorRoomList.begin());
		for(; itr != m_mtvectorRoomList.end(); itr++)
		{
			CRoom *pRoom = *itr;
			if(i_roomID == pRoom->m_idRoomID)
			{
				m_mtvectorRoomList.unlock();
				return pRoom;
			}
		}
	}
	m_mtvectorRoomList.unlock();
	return NULL;
}