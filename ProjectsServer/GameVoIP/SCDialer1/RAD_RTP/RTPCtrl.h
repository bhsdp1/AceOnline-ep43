// RTPCtrl.h: interface for the CRTPCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPCTRL_H__7E93BBEE_8033_4D8D_9740_9DDDDC3EF14B__INCLUDED_)
#define AFX_RTPCTRL_H__7E93BBEE_8033_4D8D_9740_9DDDDC3EF14B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <mmsystem.h>
#include <rtp.h>
#include <rtcp.h>
#include <payload.h>
#include <afxtempl.h>
#include "AudioCodec.h"
#include "AudioSound.h"
#include "DataQueue.h"


////////////////////////////////////////////////////////////////////
// 
extern MediaType	g_AudioPayloadType;
extern MediaType	g_VideoPayloadType;

MediaType GetAudioPayloadType(void);
MediaType GetVideoPayloadType(void);
void SetAudioPayloadType(MediaType ty);
void SetVideoPayloadType(MediaType ty);

//////////////////////////////////////////////////////////////////////


typedef enum
{
	Idle				= 0,
	CreateRTPSession	= 1,
	StartRTPSession		= 2,	
} EnumRTPStatus;

class CAudioSound;
class CRTPCtrl
{	
public:
	CRTPCtrl(MediaType ty, CAudioSound* pSound);
	virtual ~CRTPCtrl();
	/*
	int RTPSetRemoteRTP(UINT32 remoteIP, UINT remotePort);
	int RTPSetRemoteRTCP(UINT32 remoteIP, UINT remotePort);
	//*/

public:
	BOOL GetRTCPInfo(unsigned int ssrc);
	BOOL RTPReceiveStart();	
	int  SendRTPData(BYTE *data, DWORD dwSize, MediaType ty);
	UINT GetLocalPort(void);
	BOOL RTPEnd();
	BOOL RTPStart(char* remoteIP, UINT remotePort);
	int RTPOpen(UINT Port);
	void ReceivePacket(rtpParam* p);
		
	HRTPSESSION			hRTP;
    HRTCPSESSION		hRTCP;
	UINT32				timeStamp;				// time stamp
	UINT16				mySequenceNumber;		// Local Sequence Number
	UINT16				localPort;				// Opened local port
	RTCPINFO			rtcpInfo;
	UINT32				recvPacketCount;		// 받은 packet count

	int					m_index;
	BOOL				m_bFirstRtpPacket;		// 처음 받은 packet flag	
	int					m_nPacketSize;			// Audio Endcoded Packet size(byte)
	
// 0721  2차 project
	EnumRTPStatus		m_enumRTPStatus;			// RTP Channel Status
	MediaType	m_Payload;	

	void*				m_pVideoDecoder;
	CAudioSound*			m_AudioSound;

private:
	void InitializeVariable(void);
};

#endif // !defined(AFX_RTPCTRL_H__7E93BBEE_8033_4D8D_9740_9DDDDC3EF14B__INCLUDED_)
