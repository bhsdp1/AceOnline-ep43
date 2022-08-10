// RTPCtrlManager.h: interface for the CRTPCtrlManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPMODULE_H__81422C62_E73C_4C39_A8E4_CCE3AA5C5BBE__INCLUDED_)
#define AFX_RTPMODULE_H__81422C62_E73C_4C39_A8E4_CCE3AA5C5BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <rtp.h>
#include <rtcp.h>
#include <payload.h>
#include "RTPBase.h"
#include "RTPCtrl.h"

class CRTPCtrl;
class CAudioSound;
class CRTPCtrlManager
{
	typedef struct
	{
		CRTPCtrl*	Audio;
		CRTPCtrl*	Video;
		MediaType	Type;
	}_ArrayRTPCtrl;
public:
	CRTPCtrlManager(MediaType mType, CAudioSound* pSound);
	virtual ~CRTPCtrlManager();
	
public:	
	void SettingPARAM(void* pParam,MediaType ty);
	MediaType RtpOpen(int session, MediaType ty, int startPort);
	MediaType RTPStart(int session, char *remoteIP, MediaType ty, UINT port);
	MediaType RTPEnd(int session, MediaType ty);
	UINT GetLocalPort(int session, MediaType ty);
	
	/*
	void rtpSetRtcp(int session, UINT32 ip,UINT16 port);
	void rtpSetRtp(int session,UINT32 ip,UINT16 port);
	int RtpClose(int session);
	UINT32 rtpRate(int session);
	CRTPCtrl* GetRTPCtrl(int session);
	//*/


// 0720
public:
	int ALLRTPEnd();
	int GetSessionCount(MediaType ty);	
	int SendRTPData(BYTE* Data, DWORD dwSize, MediaType ty);
	int SessionToIndex(MediaType ty, int session);

private:
	int ArrayAddSession(MediaType ty, int session);
	int ArrayDeleteSession(MediaType ty, int session);

private:
	_ArrayRTPCtrl		m_pRTPCtrlArray[CRTPCTRLARRAYSIZE];
	int					AudioSessionArray[CRTPCTRLARRAYSIZE];
	int					AudioSessionCount;
	int					VideoSessionArray[CRTPCTRLARRAYSIZE];
	int					VideoSessionCount;

	int					m_nArrSize;
	MediaType			m_MMediaType;
};

#endif // !defined(AFX_RTPMODULE_H__81422C62_E73C_4C39_A8E4_CCE3AA5C5BBE__INCLUDED_)
