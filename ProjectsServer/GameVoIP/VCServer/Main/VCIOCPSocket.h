// VCIOCPSocket.h: interface for the CVCIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VCIOCPSOCKET_H__AB4B0925_7E58_444A_976F_73FFD74BD71D__INCLUDED_)
#define AFX_VCIOCPSOCKET_H__AB4B0925_7E58_444A_976F_73FFD74BD71D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCPSocket.h"
#include "VoIPProtocol.h"

class CVCIOCP;
class CRoom;
class CVCIOCPSocket : public CIOCPSocket  
{
	friend CVCIOCP;
public:
	CVCIOCPSocket();
	virtual ~CVCIOCPSocket();
		
	void ResetVCIOCPSocket(void);
	VOIP_SOCK_TYPE_t GetVoIPSockType(void){		return m_VoIPSockTy;};		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	VOIP_STATE_t GetVoIPState(void){			return m_VoIPState;};		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	// 가상 함수
	virtual BOOL OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP="", int nPeerPort=0, SThreadInfo *i_pThreadInfo=NULL);
	virtual void OnConnect(void);
	virtual void OnClose(int reason = 0);
	virtual BOOL OnError(int errCode);
	virtual void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection=FALSE);

	char *GetUserName(void){				return m_szUserName;}
	void SetJoinedRoom(CRoom *pRoom, DWORD i_ChannIdx);
	int GetRoomChannelIndex(void);

// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
// 	void SetTalking(BOOL i_bFlag);
// 	BOOL GetTalking(void);
// 	BOOL IsTalking(void);
	void SetEnableTalking(BOOL i_bFlag);	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	BOOL GetEnableTalking(void);			// 2008-10-15 by cmkwon, 말하기 기능 수정 - 

private:
	ProcessResult _Process_S2C_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_S2C_CREATE_ROOM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_S2C_JOIN_ROOM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_S2C_GOOUT_ROOM(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult _Process_S2C_REQUEST_TOKEN(const char* pPacket, int nLength, int &nBytesUsed);

	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	ProcessResult _Process_S2C_CHANGE_TALKING_STATE(const char* pPacket, int nLength, int &nBytesUsed);	
	
	ProcessResult _Process_S2C_RTP_PACKET(const char* pPacket, int nLength, int &nBytesUsed);

protected:
	CRoom				*m_pJoinedRoom;
	DWORD				m_dwChannelIndex;
	char				m_szUserName[SIZE_MAX_USER_NAME];

	BOOL				m_bTalking;				// 2005-10-05 by cmkwon, 말하는 중
	DWORD				m_dwLastTickRecvdAudio;	// 2005-10-05 by cmkwon, 마지막으로 Audio Data를 받은 Tick

	VOIP_SOCK_TYPE_t	m_VoIPSockTy;			// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	VOIP_STATE_t		m_VoIPState;			// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	static CVCIOCP		*ms_pVCIOCP;
};

#endif // !defined(AFX_VCIOCPSOCKET_H__AB4B0925_7E58_444A_976F_73FFD74BD71D__INCLUDED_)
