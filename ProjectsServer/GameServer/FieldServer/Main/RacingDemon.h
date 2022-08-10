// RacingDemon.h: interface for the CRacingDemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RACINGDEMON_H__0471B819_2BDA_49EC_8A46_39F234B6E80D__INCLUDED_)
#define AFX_RACINGDEMON_H__0471B819_2BDA_49EC_8A46_39F234B6E80D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldMapChannel;
typedef mt_vector<CFieldMapChannel*>				mtvectorFieldMapChannelPtr;

class CRacingDemon  
{
public:
	CRacingDemon();
	virtual ~CRacingDemon();

	BOOL InitRacingDemon(void);	
	void CleanRacingDemon(void);

	DWORD Worker(void);

	BOOL PutRacingMapChannel(CFieldMapChannel *i_racingMapChannel);
	void PopRacingMapChannel(CFieldMapChannel *i_racingMapChannel);
	CFieldMapChannel *FindRacingMapChannel(MAP_CHANNEL_INDEX i_mapChanIdx);

	BOOL MakeMessageRacingDemon(MessageType_t i_msgType, void *o_pMsg);
protected:
	DWORD							m_bWorkerEndFlag:1;
	HANDLE							m_hThreadWorker;

	mtvectorFieldMapChannelPtr		m_mtvectorRacingMapChannelList;
};

#endif // !defined(AFX_RACINGDEMON_H__0471B819_2BDA_49EC_8A46_39F234B6E80D__INCLUDED_)
