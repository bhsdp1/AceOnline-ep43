// LogGlobal.h: interface for the CLogGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGGLOBAL_H__08391A32_1888_4503_AE1D_79961DF9DDCE__INCLUDED_)
#define AFX_LOGGLOBAL_H__08391A32_1888_4503_AE1D_79961DF9DDCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLogExecManager.h"		// 2011-12-12 by hskim, GLog 2차

class CLogGlobal;
extern CLogGlobal *					g_pLogGlobal;

///////////////////////////////////////////////////////////////////////////////
/// \class		CLogGlobal
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-19 ~ 2004-03-19
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CLogGlobal : public CGlobalGameServer
{
public:
	CLogGlobal();
	virtual ~CLogGlobal();

	virtual BOOL InitServerSocket(void);				// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);					// 소켓관련 종료 함수

	virtual BOOL LoadConfiguration(void);				// Server 설정을 위한 Configuration Load
	
	// 2010-06-01 by shcho, GLogDB 관련 -
	char *	GetGamePublisherName(void);
	char	m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];			// 2010-06-01 by shcho, GLogDB 관련 -

	BOOL	m_bArenaFieldServerCheck;										// 2011-12-12 by hskim, GLog 2차
	BOOL	IsArenaServer(void)	{ return m_bArenaFieldServerCheck; }	// 2011-12-12 by hskim, GLog 2차

public:
	CGLogExecManager GLogExec;		// 2011-12-12 by hskim, GLog 2차
};

#endif // !defined(AFX_LOGGLOBAL_H__08391A32_1888_4503_AE1D_79961DF9DDCE__INCLUDED_)
