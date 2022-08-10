// AuthenticationIOCP.h: interface for the CAuthenticationIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "IOCP.h"
#include "ServerGroup.h"
#include "AtumAuthenticationDBManager.h"
#include "AuthenticationTickManager.h"

class CAuthenticationIOCPSocket;
class CAuthenticationIOCP : public CIOCP
{
	friend class CAuthenticationIOCPSocket;
	friend class CAtumAuthenticationDBManager;

public:
	CAuthenticationIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CAuthenticationIOCP();

public:
	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);

public:
	inline CAuthenticationIOCPSocket* GetAuthenticationIOCPSocket(int idx);

public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

private:
	CAtumAuthenticationDBManager		*m_pAtumDBManager;
	CAuthenticationTickManager			*m_pAuthenticationTickManager;
};

CAuthenticationIOCPSocket* CAuthenticationIOCP::GetAuthenticationIOCPSocket(int idx)
{
	return (CAuthenticationIOCPSocket*)GetIOCPSocket(idx);
}

#endif // !defined(AFX_AUTHIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
