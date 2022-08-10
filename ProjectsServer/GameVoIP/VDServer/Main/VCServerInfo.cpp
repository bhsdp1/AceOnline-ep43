// VCServerInfo.cpp: implementation of the CVCServerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCServerInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCServerInfo::CVCServerInfo()
{
	m_idServerID.ResetServerID_t();
	m_dwMaxRoomCounts		= 0;
	m_dwCurRoomCounts		= 0;
	m_dwMaxUserCounts		= 0;
	m_dwCurUserCounts		= 0;
	m_dwMaxBandwidth		= 0;
	m_dwCurBandwidth		= 0;
	m_dwBandwidthPercent	= 0;

	m_mtvectorRoomIDList.clear();
}

void CVCServerInfo::ResetVCServerInfo(void)
{
	m_idServerID.ResetServerID_t();
	m_dwMaxRoomCounts		= 0;
	m_dwCurRoomCounts		= 0;
	m_dwMaxUserCounts		= 0;
	m_dwCurUserCounts		= 0;
	m_dwMaxBandwidth		= 0;
	m_dwCurBandwidth		= 0;
	m_dwBandwidthPercent	= 0;

	m_mtvectorRoomIDList.clear();
}


BOOL CVCServerInfo::FindRoomID(RoomID_t i_roomID)
{
	m_mtvectorRoomIDList.lock();
	mtvectorRoomID::iterator itr(m_mtvectorRoomIDList.begin());
	for(;itr != m_mtvectorRoomIDList.end(); itr++)
	{
		if(i_roomID == *itr)
		{
			m_mtvectorRoomIDList.unlock();
			return TRUE;
		}
	}
	m_mtvectorRoomIDList.unlock();
	return FALSE;
}

void CVCServerInfo::PutRoomID(RoomID_t i_roomID)
{
	m_mtvectorRoomIDList.pushBackLock(i_roomID);
}

void CVCServerInfo::PopRoomID(RoomID_t i_roomID)
{
	m_mtvectorRoomIDList.lock();
	mtvectorRoomID::iterator itr(m_mtvectorRoomIDList.begin());
	for(;itr != m_mtvectorRoomIDList.end(); itr++)
	{
		if(i_roomID == *itr)
		{
			m_mtvectorRoomIDList.erase(itr);
			break;
		}
	}
	m_mtvectorRoomIDList.unlock();
}