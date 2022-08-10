// RoomManager.h: interface for the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOM_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_)
#define AFX_ROOM_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VoIPProtocol.h"

class CVCIOCPSocket;
class CVCIOCP;
class CRoom
{
	friend class CRoomManager;
	friend class CVCIOCPSocket;
public:
	CRoom();
	~CRoom();
	void ResetRoom(void);

	void SetRoomID(RoomID_t	i_roomID){			m_idRoomID = i_roomID;}
	RoomID_t GetRoomID(void){					return m_idRoomID;}
	DWORD GetMembersCount(void){				return m_dwMemberCounts;}
	void SetConcurrentTalkingCount(int i_nCount);
	int GetConcurrentTalkingCount(void);
	void SetManagerUserName(char *i_szUserName);
	char *GetMangerUserName(void){				return m_szManagerUserName;}

	void LockRoom(void){						EnterCriticalSection(&m_csRoom);}
	void UnlockRoom(void){						LeaveCriticalSection(&m_csRoom);}
	
	DWORD PutMemberIntoRoom(CVCIOCPSocket *pRoute);
	BOOL DeleteMemberFromRoom(CVCIOCPSocket *pRoute);
	void SendDataAllMember(BYTE *pData, int nSize, CVCIOCPSocket *i_pExcludeRoute=NULL);

	BOOL GetTalkingToken(CVCIOCPSocket *i_pVCISock);

private:
	CRITICAL_SECTION		m_csRoom;
	RoomID_t				m_idRoomID;	
	DWORD					m_dwMemberCounts;
	DWORD					m_dwMaxChannelCounts;
	CVCIOCPSocket			*m_ArrayIOCPSocketPtrJoinedUser[SIZE_MAX_CHANNEL_COUNT];
	int						m_nConcurrentTalkingCount;		// 동시 말하기 가능 카운트, 2005-10-04 by cmkwon
	char					m_szManagerUserName[SIZE_MAX_USER_NAME];		// 방을 만든 사람의 UserName


public:
	static CVCIOCP			*ms_pVCIOCP2;
};

#endif // !defined(AFX_ROOM_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_)
