// VDIOCPSocket.h: interface for the CVDIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VDIOCPSOCKET_H__F252B879_10AE_41F9_B3EF_0F0EF02CA06E__INCLUDED_)
#define AFX_VDIOCPSOCKET_H__F252B879_10AE_41F9_B3EF_0F0EF02CA06E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCPSocket.h"
#include "VoIPProtocol.h"
#include "VCServerInfo.h"

class CVDIOCPSocket;
typedef mt_vector<CVDIOCPSocket*>			mtvectorVDIOCPSocketPtr;
typedef mt_map<RoomID_t, CVDIOCPSocket*>	mtmapVDIOCPSocketPtr;


class CVDIOCP;
class CVDIOCPSocket : public CIOCPSocket
{
	friend CVDIOCP;

public:
	CVDIOCPSocket();
	virtual ~CVDIOCPSocket();

	void ResetVDIOCPSocket(void);
	
	DWORD GetBandwidthPercent(void){			return m_VCServerInfo.m_dwBandwidthPercent;};
	VOIP_SOCK_TYPE_t GetVoIPSockType(void){		return m_VoIPSockTy;};		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	VOIP_STATE_t GetVoIPState(void){			return m_VoIPState;};		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	BOOL IsValidVCServer(BOOL i_bCheckRegistered=TRUE);

	// 가상 함수
	virtual BOOL OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP="", int nPeerPort=0, SThreadInfo *i_pThreadInfo=NULL);
	virtual void OnConnect(void);
	virtual void OnClose(int reason = 0);
	virtual BOOL OnError(int errCode);
	virtual void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection=FALSE);


private:
	// from VCServer
	ProcessResult _Process_C2D_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_C2D_REGISTER_SELF(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_C2D_VCSERVER_STATUS_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_C2D_CREATE_ROOM_NOTIFY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_C2D_DELETE_ROOM_NOTIFY(const char* pPacket, int nLength, int &nBytesUsed);

	// from SCVoIP
	ProcessResult _Process_S2D_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult _Process_S2D_GET_VCSERVER_CONNECT_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	

protected:
	VOIP_SOCK_TYPE_t	m_VoIPSockTy;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	VOIP_STATE_t		m_VoIPState;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

	///////////////////////////////////////////////////////////////////////////////
	// VCServerInfo 관련
	CVCServerInfo		m_VCServerInfo;


	static CVDIOCP		*ms_pVDIOCP;
};



#endif // !defined(AFX_VDIOCPSOCKET_H__F252B879_10AE_41F9_B3EF_0F0EF02CA06E__INCLUDED_)
