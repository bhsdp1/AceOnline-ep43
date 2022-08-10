// VDIOCP.h: interface for the CVDIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VDIOCP_H__F7F31ECE_87FA_40BA_96C5_4ED333D6FFA9__INCLUDED_)
#define AFX_VDIOCP_H__F7F31ECE_87FA_40BA_96C5_4ED333D6FFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "VDIOCPSocket.h"

#define SIZE_MAX_VDSERVER_SESSION			10		// VoIP Distribute Server에서 지원할 최대동접수 COUNT_MAX_SOCKET_SESSION 이하

class CVDIOCPSocket;
class CVDIOCP : public CIOCP
{
	friend CVDIOCPSocket;
public:
	CVDIOCP(int nPort, char *szLocalIP);
	virtual ~CVDIOCP();

	CVDIOCPSocket* GetVDIOCPSocket(int idx){		return (CVDIOCPSocket*)GetIOCPSocket(idx);}


	void AddVCServerList(CVDIOCPSocket *i_pSocket);
	void DeleteVCServerList(CVDIOCPSocket *i_pSocket);
	CVDIOCPSocket *FindWellSuitedVCServer(RoomID_t i_rID);
	CVDIOCPSocket *FindVCServer(RoomID_t i_rID);
	BOOL IsSearchVCServer(CVDIOCPSocket *i_pVDSock);

	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

protected:
	mtvectorVDIOCPSocketPtr			m_mtvectorVCServerList;
};

#endif // !defined(AFX_VDIOCP_H__F7F31ECE_87FA_40BA_96C5_4ED333D6FFA9__INCLUDED_)
