// MeshInitThread.h: interface for the CMeshInitThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHINITTHREAD_H__3AA93703_BFE3_41AA_A5AC_17064AB096BB__INCLUDED_)
#define AFX_MESHINITTHREAD_H__3AA93703_BFE3_41AA_A5AC_17064AB096BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumThread.h"
#include "AtumApplication.h"

#define GAMEDATA_LOADING_TIME 0.1
class CMeshInitThread : public CAtumThread 
{
private:
	queue<structLoadingGameInfo*> m_queLoadingGameInfo;
public:	
	CMeshInitThread();
	virtual ~CMeshInitThread();
	virtual DWORD Run();

	void QuePushGameData( structLoadingGameInfo* GameInfo ) { m_queLoadingGameInfo.push(GameInfo); }
	void QuePopGameData() { m_queLoadingGameInfo.pop(); }
	
	void CreateGameData( structLoadingGameInfo* GameInfo );
};

#endif // !defined(AFX_MESHINITTHREAD_H__3AA93703_BFE3_41AA_A5AC_17064AB096BB__INCLUDED_)
