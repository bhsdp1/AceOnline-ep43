// RoomManager.h: interface for the CRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROOMMANAGER_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_)
#define AFX_ROOMMANAGER_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VoIPProtocol.h"


class CRoom;
typedef mt_vector<CRoom*>			mtvectorRoomPtr;

class CVCIOCP;
class CVCIOCPSocket;
class CRoomManager
{
public:
	CRoomManager(int i_nMaxRoomCount);
	~CRoomManager();

	void ResetRoomManager(void);
	
	CRoom *CreateRoom(RoomID_t i_roomID, int i_nConcurrentTalkingCount, CVCIOCPSocket *i_pCreateVCSocket);
	CRoom *JoinRoom(RoomID_t i_roomID, CVCIOCPSocket *i_pJoinVCSocket);
	void DeleteRoom(CRoom *i_pDelRoom);
	CRoom *FindRoomWithRoomID(RoomID_t i_roomID);

	void SetRouteIOCP(CVCIOCP *pIOCP);
	void LockRoomManager(void){					m_mtvectorRoomList.lock();}
	void UnlockRoomManager(void){				m_mtvectorRoomList.unlock();}
	
private:	
	mtvectorRoomPtr		m_mtvectorRoomList;			
	int					m_nMaxRoomCounts;
	
	CVCIOCP				*m_pVCIOCP1;
};

#endif // !defined(AFX_ROOMMANAGER_H__9A55270F_177F_455A_9088_863745FFE2C8__INCLUDED_)
