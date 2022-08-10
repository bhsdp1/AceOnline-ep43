// VCIOCP.h: interface for the CVCIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCIOCP_H__CB2BCF2C_18D2_42D2_AF78_22BBE53C2167__INCLUDED_)
#define AFX_VCIOCP_H__CB2BCF2C_18D2_42D2_AF78_22BBE53C2167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "RoomManager.h"

#define SIZE_MAX_VCSERVER_SESSION			100		// VoIP Call Server에서 지원할 최대동접수 COUNT_MAX_SOCKET_SESSION 이하


class CVCIOCPSocket;
class CVCIOCP : public CIOCP  
{
public:
	CVCIOCP(int nPort, char *szLocalIP, int i_nMaxRoomCount);
	virtual ~CVCIOCP();

	CVCIOCPSocket* GetVCIOCPSocket(int idx){		return (CVCIOCPSocket*)GetIOCPSocket(idx);}

	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

	BOOL On_D2C_CREATE_ROOM_NOTIFY_OK(MSG_D2C_CREATE_ROOM_NOTIFY_OK *i_pRMsg);
	BOOL On_S2C_JOIN_ROOM(MSG_S2C_JOIN_ROOM *i_pRMsg, CVCIOCPSocket *i_pJoinSocket);
	BOOL OnDeleteMemberFromRoom(CVCIOCPSocket *i_pDelSocket, CRoom *i_pRoom);

protected:	
	CRoomManager		m_RoomManager;
	
};

#endif // !defined(AFX_VCIOCP_H__CB2BCF2C_18D2_42D2_AF78_22BBE53C2167__INCLUDED_)
