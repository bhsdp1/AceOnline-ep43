// RacingDemon.cpp: implementation of the CRacingDemon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RacingDemon.h"
#include "FieldMapChannel.h"

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD WINAPI WorkerRacingDemon(LPVOID lpParam)
/// \brief		Worker Thread function
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		lpParam	[in] CIOCP의 포인터
/// \return		스레드종료시 리턴값
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkerRacingDemon(LPVOID lpParam)
{
	return ((CRacingDemon*)lpParam)->Worker();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRacingDemon::CRacingDemon()
{
	m_bWorkerEndFlag		= FALSE;
	
	m_hThreadWorker			= NULL;
}

CRacingDemon::~CRacingDemon()
{
	this->CleanRacingDemon();
}


BOOL CRacingDemon::InitRacingDemon(void)
{
	if(m_hThreadWorker)
	{
		return FALSE;
	}
	m_bWorkerEndFlag		= FALSE;
	m_hThreadWorker = chBEGINTHREADEX(NULL, 0, WorkerRacingDemon, (LPVOID)this, 0, 0);
	if(NULL == m_hThreadWorker)
	{
		return FALSE;
	}

	return TRUE;
}

void CRacingDemon::CleanRacingDemon(void)
{
	if(NULL == m_hThreadWorker)
	{
		return;
	}

	m_bWorkerEndFlag	= TRUE;
	DWORD dwReted		= ::WaitForSingleObject(m_hThreadWorker, INFINITE);
	if(WAIT_OBJECT_0 != dwReted)
	{
		// 리턴이 WAIT_FAILED임
		int nError = GetLastError();
		SetLastError(0);
	}
	::CloseHandle(m_hThreadWorker);
	
	m_bWorkerEndFlag	= FALSE;
	m_hThreadWorker		= NULL;
}


DWORD CRacingDemon::Worker(void)
{
	time_t ltime;
	struct tm *today = NULL;	
	mtvectorFieldMapChannelPtr::iterator itr;

	while (FALSE == m_bWorkerEndFlag)
	{
		m_mtvectorRacingMapChannelList.lock();

		time(&ltime);
		today = localtime(&ltime);
		itr = m_mtvectorRacingMapChannelList.begin();
		for(; itr != m_mtvectorRacingMapChannelList.end(); itr++)
		{
			(*itr)->ProcessRacing(today);
		}

		m_mtvectorRacingMapChannelList.unlock();
		Sleep(100);
	}
	return 0;
}


BOOL CRacingDemon::PutRacingMapChannel(CFieldMapChannel *i_racingMapChannel)
{
	m_mtvectorRacingMapChannelList.lock();

	mtvectorFieldMapChannelPtr::iterator itr(m_mtvectorRacingMapChannelList.begin());
	for(; itr != m_mtvectorRacingMapChannelList.end(); itr++)
	{
		if(i_racingMapChannel == (*itr))
		{
			m_mtvectorRacingMapChannelList.unlock();
			return TRUE;
		}
	}

	m_mtvectorRacingMapChannelList.push_back(i_racingMapChannel);
	m_mtvectorRacingMapChannelList.unlock();

	return TRUE;
}

void CRacingDemon::PopRacingMapChannel(CFieldMapChannel *i_racingMapChannel)
{
	m_mtvectorRacingMapChannelList.lock();

	mtvectorFieldMapChannelPtr::iterator itr(m_mtvectorRacingMapChannelList.begin());
	for(; itr != m_mtvectorRacingMapChannelList.end(); itr++)
	{
		if(i_racingMapChannel == (*itr))
		{
			m_mtvectorRacingMapChannelList.erase(itr);
			break;
		}
	}
	m_mtvectorRacingMapChannelList.unlock();
}


CFieldMapChannel *CRacingDemon::FindRacingMapChannel(MAP_CHANNEL_INDEX i_mapChanIdx)
{
	CFieldMapChannel *pRet = NULL;
	m_mtvectorRacingMapChannelList.lock();
	mtvectorFieldMapChannelPtr::iterator itr(m_mtvectorRacingMapChannelList.begin());
	for(; itr != m_mtvectorRacingMapChannelList.end(); itr++)
	{
		if(i_mapChanIdx == (*itr)->GetMapChannelIndex())
		{
			pRet = *itr;
			break;
		}
	}
	m_mtvectorRacingMapChannelList.unlock();

	return pRet;
}



BOOL CRacingDemon::MakeMessageRacingDemon(MessageType_t i_msgType, void *o_pMsg)
{
	switch(i_msgType)
	{
	case T_FC_RACING_RACINGLIST_REQUEST_ACK:
		{
			MSG_FC_RACING_RACINGLIST_REQUEST_ACK *pMsg = (MSG_FC_RACING_RACINGLIST_REQUEST_ACK*)o_pMsg;
			MAP_CHANNEL_INDEX *pMapChanIdx = (MAP_CHANNEL_INDEX*)((BYTE*)pMsg + sizeof(MSG_FC_RACING_RACINGLIST_REQUEST_ACK));
			pMsg->byRacingListCounts = m_mtvectorRacingMapChannelList.size();
			int nIdx = 0;
			while (nIdx < m_mtvectorRacingMapChannelList.size())
			{
				pMapChanIdx[nIdx] = m_mtvectorRacingMapChannelList[nIdx]->GetMapChannelIndex();
				nIdx++;
			}
		}
		break;
	default:
		{
			return FALSE;
		}
	}

	return TRUE;
}