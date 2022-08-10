// VCIOCP.cpp: implementation of the CVCIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCIOCP.h"
#include "VCGlobal.h"
#include "VCIOCPSocket.h"
#include "Room.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCIOCP::CVCIOCP(int nPort, char *szLocalIP, int i_nMaxRoomCount)
	: CIOCP(0, SIZE_MAX_VCSERVER_SESSION, nPort, szLocalIP, ST_NORMAL_SERVER), m_RoomManager(i_nMaxRoomCount)
{
	CVCIOCPSocket::ms_pVCIOCP	= this;
	CRoom::ms_pVCIOCP2			= this;


	CVCIOCPSocket * pVCSocket	= new CVCIOCPSocket[m_dwArrayClientSize];
	this->m_pArrayIOCPSocket	= pVCSocket;
	for(DWORD i = 0; i < m_dwArrayClientSize; i++)
	{		
		m_ArrayClient[i] = &pVCSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}
}

CVCIOCP::~CVCIOCP()
{
	ListenerClose();
	IOCPClean();
	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CVCIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}
}


BOOL CVCIOCP::IOCPInit(void)
{
	// TODO: Place code here.


	return CIOCP::IOCPInit();
}

void CVCIOCP::IOCPClean(void)
{
	// TODO: Place code here.
	char szSystemLog[512];
	sprintf(szSystemLog, "CVCIOCP::IOCPClean_1\r\n");
	DBGOUT(szSystemLog);
	g_pVCGlobal->WriteSystemLog(szSystemLog);


	CIOCP::IOCPClean();
}

SThreadInfo *CVCIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);
	
	return pTInfo;
}



BOOL CVCIOCP::On_D2C_CREATE_ROOM_NOTIFY_OK(MSG_D2C_CREATE_ROOM_NOTIFY_OK *i_pRMsg)
{	
	CVCIOCPSocket *pVCSocket = this->GetVCIOCPSocket(i_pRMsg->ClientIndex);
	if(NULL == pVCSocket
		|| strncmp(i_pRMsg->szUserName, pVCSocket->GetUserName(), SIZE_MAX_USER_NAME))
	{// Error를 전송하지는 안는다
		return FALSE;
	}

	CRoom *pRoom = m_RoomManager.CreateRoom(i_pRMsg->CreateRoomID, i_pRMsg->nConcurrentTalkingCount, pVCSocket);
	if(NULL == pRoom)
	{// Error를 전송하지는 안는다
		return FALSE;
	}

	pVCSocket->m_VoIPState		= VOIP_STATE_VC_Joined_Room;	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	BYTE SendBuf[128];
	INIT_MSG(MSG_C2S_CREATE_ROOM_OK, T_C2S_CREATE_ROOM_OK, pSendMsg, SendBuf);
	pSendMsg->nChannelIndex = pVCSocket->m_dwChannelIndex;
	pVCSocket->SendAddData(SendBuf, MSG_SIZE(MSG_C2S_CREATE_ROOM_OK));
	return TRUE;
}

BOOL CVCIOCP::On_S2C_JOIN_ROOM(MSG_S2C_JOIN_ROOM *i_pRMsg, CVCIOCPSocket *i_pJoinSocket)
{
	CRoom *pRoom = m_RoomManager.JoinRoom(i_pRMsg->joinRoomID, i_pJoinSocket);
	if(NULL == pRoom)
	{// Error를 전송하지는 안는다

		return FALSE;
	}

	return TRUE;
}


BOOL CVCIOCP::OnDeleteMemberFromRoom(CVCIOCPSocket *i_pDelSocket, CRoom *i_pRoom)
{
	m_RoomManager.LockRoomManager();
	if(FALSE == i_pRoom->DeleteMemberFromRoom(i_pDelSocket))
	{
		m_RoomManager.UnlockRoomManager();
		return FALSE;
	}
		
	BYTE SendBuf[128];
	if(0 == i_pRoom->GetMembersCount())
	{	
		///////////////////////////////////////////////////////////////////////////////
		// Room을 지우기 전에 메세지를 만든다		
		INIT_MSG(MSG_C2D_DELETE_ROOM_NOTIFY, T_C2D_DELETE_ROOM_NOTIFY, pSMsg, SendBuf);
		pSMsg->DeleteRoomID = i_pRoom->GetRoomID();
		
		m_RoomManager.DeleteRoom(i_pRoom);
		m_RoomManager.UnlockRoomManager();

		///////////////////////////////////////////////////////////////////////////////
		// VDServer로 전송한다
		g_pVCGlobal->Send2VDServer(SendBuf, MSG_SIZE(MSG_C2D_DELETE_ROOM_NOTIFY));
	}
	else
	{
		m_RoomManager.UnlockRoomManager();
		INIT_MSG(MSG_C2S_GOOUT_USER, T_C2S_GOOUT_USER, pSMsg, SendBuf);
		pSMsg->nChannelIndex	= i_pDelSocket->m_dwChannelIndex;
		strncpy(pSMsg->szUserName, i_pDelSocket->GetUserName(), SIZE_MAX_USER_NAME);

		i_pRoom->SendDataAllMember(SendBuf, MSG_SIZE(MSG_C2S_GOOUT_USER));
	}
	return TRUE;
}