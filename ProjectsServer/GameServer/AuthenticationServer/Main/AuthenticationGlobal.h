// AuthenticationGlobal.h: interface for the CAuthenticationGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTHGLOBAL_H__C876B35F_9CED_4C8B_BB4C_3BB6B23FD13D__INCLUDED_)
#define AFX_AUTHGLOBAL_H__C876B35F_9CED_4C8B_BB4C_3BB6B23FD13D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalGameServer.h"
#include "AtumProtocol.h"			// 2007-05-02 by cmkwon


typedef vector<SGAME_SERVER_GROUP>			vectSGAME_SERVER_GROUP;		// 2007-05-02 by cmkwon


class CAuthenticationIOCP;
class CAuthenticationIOCPSocket;
class CAuthenticationGlobal;
extern CAuthenticationGlobal *		g_pAuthenticationGlobal;

class CServerGroup;
class CAuthenticationGlobal : public CGlobalGameServer
{
	friend class CAuthenticationIOCP;

public:
	CAuthenticationGlobal();
	virtual ~CAuthenticationGlobal();

	void Reset();

public:
	// virtual Function
	virtual BOOL InitServerSocket(void);				// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);					// 소켓관련 종료 함수

	virtual BOOL LoadConfiguration(void);				// Server 설정을 위한 Configuration Load
};

#endif // !defined(AFX_AUTHGLOBAL_H__C876B35F_9CED_4C8B_BB4C_3BB6B23FD13D__INCLUDED_)
