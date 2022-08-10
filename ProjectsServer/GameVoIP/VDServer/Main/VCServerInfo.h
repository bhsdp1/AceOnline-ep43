// VCServerInfo.h: interface for the CVCServerInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCSERVERINFO_H__57DD1042_3E26_4135_8391_5575F93168F6__INCLUDED_)
#define AFX_VCSERVERINFO_H__57DD1042_3E26_4135_8391_5575F93168F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VoIPParam.h"

class CVCServerInfo  
{
public:
	CVCServerInfo();

	void ResetVCServerInfo(void);
		
	BOOL FindRoomID(RoomID_t i_roomID);
	void PutRoomID(RoomID_t i_roomID);
	void PopRoomID(RoomID_t i_roomID);
public:
	ServerID_t			m_idServerID;
	DWORD				m_dwMaxRoomCounts;
	DWORD				m_dwCurRoomCounts;
	DWORD				m_dwMaxUserCounts;
	DWORD				m_dwCurUserCounts;
	DWORD				m_dwMaxBandwidth;
	DWORD				m_dwCurBandwidth;
	DWORD				m_dwBandwidthPercent;

	mtvectorRoomID		m_mtvectorRoomIDList;
};

#endif // !defined(AFX_VCSERVERINFO_H__57DD1042_3E26_4135_8391_5575F93168F6__INCLUDED_)
