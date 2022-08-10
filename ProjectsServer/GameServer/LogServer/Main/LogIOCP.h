// LogIOCP.h: interface for the CLogIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGIOCP_H__C2CBAF93_CC10_4AB6_BB88_EDEE284BA929__INCLUDED_)
#define AFX_LOGIOCP_H__C2CBAF93_CC10_4AB6_BB88_EDEE284BA929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"

#define COUNT_MAX_LOG_CLIENT	10

class CLogIOCPSocket;
class CAtumLogDBManager;		
class CGLogDBManager;			// 2010-06-01 by shcho, GLogDB 관련 -
class CLogIOCP : public CIOCP
{
public:
	CLogIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CLogIOCP();

public:
	BOOL IOCPInit();
	void IOCPClean();
	//BOOL OpenUDPPortForOtherServer(void);

	void WriteGameLog(char *i_szLogString);
protected:
//	int CreateUDPPorts();		// return nLogIOCPSocket;
//	int LoadServerInfo();		// parsing server info file, and loading



protected:
	CLogIOCPSocket			*m_pFieldServer1IOCPSocket;
	CSystemLogManager		m_GameLogManager;				// 2006-05-23 by cmkwon



public:
	CAtumLogDBManager		*m_pAtumLogDBManager;
	CGLogDBManager			*m_pGLogDBManager;				// 2010-06-01 by shcho, GLogDB 관련 -
};
/*
class ServerInfo
{
	char			IP[SIZE_MAX_IPADDRESS];
	int				Port;
	ENServerType	ServerType;
}
*/
#endif // !defined(AFX_LOGIOCP_H__C2CBAF93_CC10_4AB6_BB88_EDEE284BA929__INCLUDED_)
