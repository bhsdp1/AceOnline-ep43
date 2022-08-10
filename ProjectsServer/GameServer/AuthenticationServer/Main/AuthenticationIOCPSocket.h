// AuthenticationIOCPSocket.h: interface for the CAuthenticationIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHENTICATIONIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
#define AFX_AUTHENTICATIONIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AuthenticationIOCP.h"
#include "AtumProtocol.h"

class CAuthenticationIOCPSocket : public CIOCPSocket
{
	friend CAuthenticationIOCP;
public:
	CAuthenticationIOCPSocket();
	~CAuthenticationIOCPSocket();

public:
	BOOL OnRecvdPacketAuthenticationServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	void OnConnect(void);
	void OnClose(int reason = 0);

	// Process Protocol
	ProcessResult Process_PAUTH_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);

	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

private:
	static CAuthenticationIOCP		*ms_pAuthenticationIOCP;
};

#endif // !defined(AFX_AUTHENTICATIONIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
