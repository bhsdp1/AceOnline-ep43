// NPCGlobal.h: interface for the CNPCGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCGLOBAL_H__12EE2E44_49ED_4053_AB20_B0601FFF49DD__INCLUDED_)
#define AFX_NPCGLOBAL_H__12EE2E44_49ED_4053_AB20_B0601FFF49DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3dapp.h"

class CNPCGlobal;
extern CNPCGlobal *			g_pNPCGlobal;

///////////////////////////////////////////////////////////////////////////////
/// \class		CNPCGlobal
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-19 ~ 2004-03-19
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CNPCIOCP;
class CN2FSocket;
class CNPCGlobal : public CGlobalGameServer
{
public:
	CNPCGlobal();
	virtual ~CNPCGlobal();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetIPFieldServer(char *i_szIP);
	char *GetIPFieldServer(void);
	void SetPortFieldServer(int i_nPort);
	int GetPortFieldServer(void);

	CN2FSocket * GetN2FSocket(void);
	BOOL WriteN2FSocket(BYTE *pData, int nSize);	// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) -

	///////////////////////////////////////////////////////////////////////////
	// Method
	BOOL CreateN2FSocket(HWND i_hWnd);

	void DestroyN2FSocket(void);


	// virtual Function
	virtual BOOL InitServerSocket(void);				// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);					// 소켓관련 종료 함수

	virtual BOOL LoadConfiguration(void);				// Server 설정을 위한 Configuration Load

	CD3DApplication		m_D3DApp;

	BOOL				m_bIsArenaServer;				// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	BOOL GetIsArenaServer(void);						// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
protected:
	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	char				m_szIPFieldServer[SIZE_MAX_IPADDRESS];			// one (NPC)
	int					m_nPortFieldServer;								// one (NPC)

	CN2FSocket *		m_pN2FSocket;
};

#endif // !defined(AFX_NPCGLOBAL_H__12EE2E44_49ED_4053_AB20_B0601FFF49DD__INCLUDED_)
