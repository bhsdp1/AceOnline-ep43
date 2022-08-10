// RTPCtrlManager.cpp: implementation of the CRTPCtrlManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTPCtrlManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPCtrlManager::CRTPCtrlManager(MediaType mType, CAudioSound* pSound)
{
#ifdef _KWON_DEBUG
	//TRACE("\n========> CRTPCtrlManager Constructor");
#endif

	rtpInit();				// RADVision RTP Modules initialize	
	//rtcpInit();				// RADVision RTCP Modules initialize

	m_nArrSize			= CRTPCTRLARRAYSIZE;
	AudioSessionCount	= 0;
	VideoSessionCount	= 0;
	m_MMediaType		= mType;		// Audio or Video

	for(int i=0; i < m_nArrSize; i++)
	{
		AudioSessionArray[i] = -1;
		VideoSessionArray[i] = -1;
		
		m_pRTPCtrlArray[i].Audio = NULL;
		if(mType == t_Audio || mType == t_AudioAndVideo)
		{			
			m_pRTPCtrlArray[i].Audio = new CRTPCtrl(t_Audio, pSound);		// Audio
			m_pRTPCtrlArray[i].Audio->m_index = i;
		}

		m_pRTPCtrlArray[i].Video = NULL;
		if( mType == t_Video || mType == t_AudioAndVideo)
		{
			m_pRTPCtrlArray[i].Video = new CRTPCtrl(t_Video, NULL);		// Video
			m_pRTPCtrlArray[i].Video->m_index = i;
		}
	}	
}

CRTPCtrlManager::~CRTPCtrlManager()
{
#ifdef _KWON_DEBUG
	//TRACE("\n========> CRTPCtrlManager Destructor");
#endif
	
	for(int i=0; i < m_nArrSize; i++)
	{
		if(m_pRTPCtrlArray[i].Audio)
		{
			delete m_pRTPCtrlArray[i].Audio;
			m_pRTPCtrlArray[i].Audio = NULL;
		}

		if(m_pRTPCtrlArray[i].Video)
		{
			delete m_pRTPCtrlArray[i].Video;
			m_pRTPCtrlArray[i].Video = NULL;
		}
	}
		
	rtpEnd();
	//rtcpEnd();	
}

//////////////////////////////////////////////////////////////////
/*
UINT32 CRTPCtrlManager::rtpRate(int session)
{
	return 0;
}

void CRTPCtrlManager::rtpSetRtcp(int session, UINT32 ip,UINT16 port)
{
	int i;
	i = SessionToIndex(session);

	if(i == -1)
		return ;

	m_pAudioRTPCtrlArray[i]->RTPSetRemoteRTCP( ip, port);

}

void CRTPCtrlManager::rtpSetRtp(int session,UINT32 ip,UINT16 port)
{
	int i;
	i = SessionToIndex(session);

	if(i == -1)
		return ;

	m_pAudioRTPCtrlArray[i]->RTPSetRemoteRTP( ip, port);
}

int CRTPCtrlManager::RtpClose(int session)
{
	int i;

	if(AudioSessionCount==0)
		return -2;

	i = SessionToIndex(session);

	if(i == -1)
		return i;

	ArrayDeleteSession(session);

	return  m_pAudioRTPCtrlArray[i]->RTPEnd();
}

CRTPCtrl* CRTPCtrlManager::GetRTPCtrl(int session)
{
	int i;
	i = SessionToIndex(session);

	if(i == -1)
		return NULL;
	return &m_pAudioRTPCtrlArray[i];
}
//*/
////////////////////////////////////////////////////////



int CRTPCtrlManager::SessionToIndex(MediaType ty, int session)
{
	if( ty == t_Audio)
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (AudioSessionArray[i] == session)
			{
				return i;
			}
		}
	}
	else
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (VideoSessionArray[i] == session)
			{
				return i;
			}
		}
	}

	return -1;
}

int CRTPCtrlManager::ArrayAddSession(MediaType ty, int session)
{
	if( ty == t_Audio)
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (AudioSessionArray[i] == session)
			{
				return i;
			}		
		}
		
		for ( i = 0; i < m_nArrSize; i++)
		{
			if( AudioSessionArray[i] == -1)
			{
				AudioSessionArray[i] = session;
				AudioSessionCount++;
				return i;
			}
		}
	}
	else
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (VideoSessionArray[i] == session)
			{
				return i;
			}		
		}
		
		for ( i = 0; i < m_nArrSize; i++)
		{
			if( VideoSessionArray[i] == -1)
			{
				VideoSessionArray[i] = session;
				VideoSessionCount++;
				return i;
			}
		}
	}
	return -1;
}

int CRTPCtrlManager::ArrayDeleteSession(MediaType ty, int session)
{
	if( ty == t_Audio)
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (AudioSessionArray[i] == session)
			{
				AudioSessionArray[i] = -1;
				AudioSessionCount--;
				return 0;
			}		
		}
	}
	else
	{
		for (int i=0; i < m_nArrSize; i++)
		{
			if (VideoSessionArray[i] == session)
			{
				VideoSessionArray[i] = -1;
				VideoSessionCount--;
				return 0;
			}		
		}
	}

	return -1;
}

void CRTPCtrlManager::SettingPARAM(void* pParam, MediaType ty)
{	
#ifdef _KWON_DEBUG
	TRACE("\n				SettingPARAM  ty[%d], pParam[%ld]", ty, pParam);
#endif

	if(ty == t_Video)
	{
		for(int i=0; i<m_nArrSize; i++)
		{
			m_pRTPCtrlArray[i].Video->m_pVideoDecoder = pParam;
		}
	}
}

MediaType CRTPCtrlManager::RtpOpen(int session, MediaType ty, int startPort)
{
	int i;
	int Aport, Vport;

	Aport = Vport = 0;
	
	switch(ty)
	{
	case t_Audio:		// only Audio Channel Open
		{
			if( m_MMediaType == t_Audio || m_MMediaType == t_AudioAndVideo)
			{
				i = ArrayAddSession(t_Audio, session);
				
				if(i == -1)
				{
					return t_NULL;		// RTP Array is full.
				}
				Aport = m_pRTPCtrlArray[i].Audio->RTPOpen((UINT16)(startPort));									
#ifdef _KWON_DEBUG
				TRACE("\n				Open Audio RTP Channel index[%d],  Port[%d]", i, Aport);
#endif
				if(Aport)
				{
					return ty;
				}
			}
		}
		break;
	case t_Video:		// only Video Channel Open
		{
			if( m_MMediaType == t_Video || m_MMediaType == t_AudioAndVideo)
			{
				i = ArrayAddSession(t_Video, session);
				
				if(i == -1)
				{
					return t_NULL;		// RTP Array is full.
				}

				Vport = m_pRTPCtrlArray[i].Video->RTPOpen((UINT16)(startPort));
									
#ifdef _KWON_DEBUG
				TRACE("\n			Open Video RTP Channel index[%d],  Port[%d]", i, Vport);
#endif
				if(Vport)
				{
					return ty;
				}				
			}
		}
		break;
/*
	case t_AudioAndVideo:		// Audio and Video Channel Open
		{
			if( m_MMediaType == t_AudioAndVideo)
			{
				i = ArrayAddSession(t_Audio, session);
				
				if(i == -1)
				{
					return t_NULL;		// RTP Array is full.
				}
				Aport = m_pRTPCtrlArray[i].Audio->RTPOpen((UINT16)(startPort));									
#ifdef _KWON_DEBUG
				TRACE("\n				Open Audio RTP Channel index[%d],  Port[%d]", i, Aport);
#endif

				i = ArrayAddSession(t_Video, session);
				
				if(i == -1)
				{
					return t_NULL;		// RTP Array is full.
				}
				Vport = m_pRTPCtrlArray[i].Video->RTPOpen((UINT16)(startPort+4));
#ifdef _KWON_DEBUG
				TRACE("\n				Open Video RTP Channel index[%d],  Port[%d]", i, Vport);
#endif
				if(Aport && Vport)
				{
					return ty;
				}
			}
		}
		break;
//*/
	default:
		{			
			//AfxMessageBox("\nError : RTPOpen error ty[%d]", ty);
		}
	}

	return t_NULL;	
}

MediaType CRTPCtrlManager::RTPStart(int session, char *remoteIP, MediaType ty, UINT port)
{
	int i = -1;
	
	i = SessionToIndex(ty, session);

	if(i == -1)
	{
		return t_NULL;
	}

	switch(ty)
	{
	case t_Audio:		// only Audio Channel Open
		{
			if( m_MMediaType == t_Audio || m_MMediaType == t_AudioAndVideo)
			{
				if(m_pRTPCtrlArray[i].Audio->RTPStart(remoteIP, port))
				{
					return ty;
				}
			}
		}
		break;	
	case t_Video:		// only Video Channel Open
		{
			if( m_MMediaType == t_Video || m_MMediaType == t_AudioAndVideo)
			{
				if(m_pRTPCtrlArray[i].Video->RTPStart(remoteIP, port))
				{
					return ty;								
				}
			}
		}
		break;	
	default:
		{
			//AfxMessageBox("RTPStart Error !!");
		}		
	}	

	return t_NULL;
}

MediaType CRTPCtrlManager::RTPEnd(int session, MediaType ty)
{
	int i;
	MediaType ret = t_NULL;

	switch(ty)
	{
	case t_Audio:
		{
			if(AudioSessionCount==0)
			{
				return ret;
			}
			
			i = SessionToIndex(t_Audio, session);
			
			if(i == -1)
			{
				return ret;
			}
			
			ArrayDeleteSession(t_Audio, session);
			m_pRTPCtrlArray[i].Audio->RTPEnd();
#ifdef _KWON_DEBUG
			TRACE("\n				Close Audio RTP Channel index[%d]", i);
#endif
		}
		break;	
	case t_Video:
		{
			if(VideoSessionCount==0)
			{
				return ret;
			}		
			
			i = SessionToIndex(t_Video, session);	
			
			if(i == -1)
			{
				return ret;
			}
			m_pRTPCtrlArray[i].Video->RTPEnd();
			
		}
		break;	
	default:
		{
			//AfxMessageBox("\n RTPEnd error !!");
			return ret;
		}
	}
	return ty; 
}


UINT CRTPCtrlManager::GetLocalPort(int session, MediaType ty)
{
	int i;

	switch(ty)
	{
	case t_Audio:
		{
			
			i = SessionToIndex(ty, session);
			
			if(i == -1 || !m_pRTPCtrlArray[i].Audio)
			{
				return 0;
			}
			
			return m_pRTPCtrlArray[i].Audio->GetLocalPort();
		}
		break;
	case t_Video:
		{			
			i = SessionToIndex(ty, session);
			
			if(i == -1 || !m_pRTPCtrlArray[i].Video)
			{
				return 0;
			}
			
			return m_pRTPCtrlArray[i].Video->GetLocalPort();
		}
		break;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////
// SendRTPData(BYTE* Data, DWORD dwSize, MediaType ty);
//
// - RTP 채널로 RTP Packet을 보낸다.
// - Data 는 전송 할 데이터의 포인터
// - dwSize 는 전송 할 데이터의 사이즈(byte)
// - ty 는 전송할 데이터의 미디어 타입, Audio 와 Video 만 전송 가능
// - return 값은 전송한 채널의 개수
///////////////////////////////////////////////////////////////////////////
int CRTPCtrlManager::SendRTPData(BYTE* Data, DWORD dwSize, MediaType ty)
{	
	int nCount=0;
	int r = 0;
	
	if( ty == t_Audio )
	{
		for(int i=0 ; i < m_nArrSize;i++)
		{
			if(m_pRTPCtrlArray[i].Audio->SendRTPData(Data, dwSize, ty))
			{				
				nCount++;
				if(nCount >= AudioSessionCount)
				{
					break;
				}
			}			
		}		
	}
	else if( ty == t_Video ) 
	{
		for(int i=0 ; i < m_nArrSize;i++)
		{
			if(m_pRTPCtrlArray[i].Video->m_enumRTPStatus == StartRTPSession)
			{			
				if( m_pRTPCtrlArray[i].Video->SendRTPData(Data, dwSize, ty))
				{
					nCount++;
					if(nCount >= VideoSessionCount)
					{
						break;
					}
				}
			}			
		}		
	}
	else
	{
		//AfxMessageBox("Error : SendRTPData error ty[%d]", ty);
	}

	return nCount;
}

int CRTPCtrlManager::GetSessionCount(MediaType ty)
{
	if(ty == t_Audio)
	{
		return AudioSessionCount;
	}
	else
	{
		return VideoSessionCount;
	}
}

int CRTPCtrlManager::ALLRTPEnd()
{
	int r=0;

	for(int i=0; i< m_nArrSize; i++)
	{
		if(m_pRTPCtrlArray[i].Audio && m_pRTPCtrlArray[i].Audio->m_enumRTPStatus != Idle )
		{
#ifdef _KWON_DEBUG
			TRACE("\n				Close Audio RTP Channel index[%d]", i);
#endif			
			m_pRTPCtrlArray[i].Audio->RTPEnd();
		}

		if(m_pRTPCtrlArray[i].Video && m_pRTPCtrlArray[i].Video->m_enumRTPStatus != Idle)
		{
#ifdef _KWON_DEBUG
			TRACE("\n				Close Video RTP Channel index[%d]", i);
#endif
			m_pRTPCtrlArray[i].Video->RTPEnd();
		}
	}
	return r;
}
