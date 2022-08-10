// RTPCtrl.cpp: implementation of the CRTPCtrl class.
//
//////////////////////////////////////////////////////////////////////



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "stdafx.h"
#include "RTPCtrl.h"

/*
typedef enum
{
    t_PCMU		=  0,
    t_G7231Low	= 50,		// G.7231 Low
	t_G7231		=  4,		// G.7231 High
    t_PCMA		=  8,
    t_G722		=  9, 
    t_G728		= 15,
    t_G729		= 18,
    t_H261		= 31,
    t_H263		= 34,
	t_NULL		=100
} MediaType;
//*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTPCtrl::CRTPCtrl(MediaType ty, CAudioSound* pSound)
{	
	if(ty == t_Audio)
	{
		m_Payload = GetAudioPayloadType();
		switch (m_Payload)
		{
		case t_G7231Low:			
			m_nPacketSize = 20;		
			break;
		case t_G7231:				
			m_nPacketSize = 24;		
			break;
		case t_G729:
			m_nPacketSize = 10;
			break;
		case t_PCMU:
		case t_PCMA:
			break;
		default:
			AfxMessageBox("CRTPCrtl Constructor m_Payload[%d] Audio!!", m_Payload);			
		}
	}
	else if(ty == t_Video)
	{
		m_Payload = GetVideoPayloadType();
		if(m_Payload != t_H263)
		{
			AfxMessageBox("CRTPCrtl Constructor m_Payload[%d] Video!!", m_Payload);			
		}		
	}
	else
	{
		AfxMessageBox("CRTPCrtl Constructor tFormat[%d] !!", ty);
	}
	
	hRTP =NULL;
	hRTCP = NULL;    
	timeStamp = 0;
	localPort = 0;
	recvPacketCount = 0;

	memset(&rtcpInfo, 0x00, sizeof(RTCPINFO));
	m_bFirstRtpPacket	= TRUE;
	m_Payload = t_NULL;
	m_enumRTPStatus = Idle;

	m_pVideoDecoder = NULL;	
	m_AudioSound = pSound;
}

CRTPCtrl::~CRTPCtrl()
{	
	if(hRTP)
	{
		rtpClose(hRTP);
	}
}
//////////////////////////////////////////////////////////////
//
/*
int CRTPCtrl::RTPSetRemoteRTP(UINT32 remoteIP, UINT remotePort)
{
	rtpSetRemoteAddress(hRTP, remoteIP, remotePort);
	rtpSetEventHandler(hRTP, rtpEventHandler, this);
    
	return remotePort;
}

int CRTPCtrl::RTPSetRemoteRTCP(UINT32 remoteIP, UINT remotePort)
{
	rtcpSetRemoteAddress(hRTCP, remoteIP, remotePort);
    
	return remotePort;
}
//*/
///////////////////////////////////////////////////////////////




void rtpEventHandler(HRTPSESSION hRTP, void* context)
{			
	rtpParam	p;
	H263param	m_h263p;						//인코딩 데이타의 정보 구조체
	JittData*	lpbyte;
	CRTPCtrl*	rtpCtrl;
	BYTE		buff[256];
	int			nSize = 0;
			
	int re;

	rtpCtrl = (CRTPCtrl*)context;

	ZeroMemory(buff, sizeof(buff));
	//re = rtpReadEx(hRTP, buff, sizeof(buff), rtpCtrl->timeStamp , &p);
	// rtcp를 사용하지 않음
	re = rtpRead(hRTP, buff, sizeof(buff), &p);

	if( re == RVERROR )
	{
		return;
	}		
	else
	{
		if(!(rtpCtrl->mySequenceNumber%100))
		{
		}
	}

	rtpCtrl->ReceivePacket( &p);

	if( p.payload == G7231 || p.payload == G729 )
	{
#ifdef _KWON_DEBUG
		//if(rtpCtrl->recvPacketCount % 330 == 0)		
		{
			//TRACE("\n				RTP Channel Received index[%d] Count[%d] ", rtpCtrl->m_index , rtpCtrl->recvPacketCount);
			//rtpCtrl->GetRTCPInfo(p.sSrc);
			/*
			if(rtpCtrl->recvPacketCount % 10 == 0)
			{
				DbgOut("\n");
			}
			TRACE("	[%d]", p.sequenceNumber);
			//*/
		}
#endif			

#ifdef _G7231CODEC								// RTPCtrl.cpp
		/////////////////////////////////////////////////////////////////////////////
		// G.723.1 Audio Codec
		rtpG7231Unpack(buff, p.len , &p  , NULL);
		nSize = p.len - p.sByte;
		if(nSize <= 0 ) return;
		if(nSize%20 != 0 && nSize%24 != 0) return;
#endif
#ifdef _G729CODEC								// RTPCtrl.cpp
		////////////////////////////////////////////////////////////////////////////////
		// G.729 Audio Codec		
		rtpG729Unpack(buff, p.len , &p  , NULL);
		nSize = p.len - p.sByte;
		if(nSize <= 0 ) return;
		if(nSize%10 != 0 ) return;
#endif		
		lpbyte = new JittData;
		lpbyte->dataSize = nSize;
		lpbyte->sequenceNumber = p.sequenceNumber;
		lpbyte->time = p.timestamp;
		lpbyte->lpData = new BYTE[lpbyte->dataSize];
		
		memcpy(lpbyte->lpData, &(buff[p.sByte]), lpbyte->dataSize);	
		
		if(rtpCtrl->m_AudioSound)
		{
			rtpCtrl->m_AudioSound->PostThreadMessage(WM_MIX_FORMRTPTOBUFFER, (LPARAM)lpbyte, rtpCtrl->m_index);

			// log 를 남기기 위해..
			// 2003.06.25 by gomeisa
			rtpCtrl->m_AudioSound->m_lRxRTPData += nSize;
			rtpCtrl->m_AudioSound->PostThreadMessage(WM_RTPDATA_READ, rtpCtrl->m_AudioSound->m_lRxRTPData, 10005);
		}
	}
	else if(p.payload == H263)	// 사용시 buff 사이즈를 크게해야함.
	{
		rtpH263Unpack(buff, p.len , &p  , &m_h263p);		
	}
}


// RTP Port를 open할때 RTCP 포트가 열려져있으면 RTP Port만 Open되고
// RTCP 포트는 열리지 않는다.
int CRTPCtrl::RTPOpen(UINT Port)
{
	
	UINT openPort;
	
	if (hRTP) 
		return rtpGetPort(hRTP);
	
	/*
	// rtcp를 사용하지 않음
	// rtpOpenEx 호출시 cname가 있으면 자동으로 rtcp 포트도 open한다
	struct hostent *hHost;
	struct sockaddr_in sin;
	
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	hHost = gethostbyaddr((char*)&sin.sin_addr, 4, PF_INET);
	//*/
	openPort = Port;

	int nCount = 0;
	do
	{
		if(nCount >= 20)
		{
			break;
		}

		hRTP=rtpOpen((UINT16)(openPort),1,0xff);		

		/*
		// rtcp를 사용하지 않음
		// rtpOpenEx 호출시 cname가 있으면 자동으로 rtcp 포트도 open한다
		if(hHost != NULL)
		{
			hRTP=rtpOpenEx((UINT16)(openPort),1,0xff,hHost->h_name);			
		}
		else
		{
			hRTP=rtpOpenEx((UINT16)(openPort),1,0xff,"WebCall World");			
		}
		//*/
		localPort = openPort;
		openPort +=2;
		nCount++;
	}while(!hRTP);

	if( nCount >= 10)
	{
		return 0;
	}
		
	rtpUseSequenceNumber(hRTP);
	srand( (unsigned)time( NULL ) );
	mySequenceNumber = rand() / (RAND_MAX / 65535 + 1);

	/*
	// rtcp를 사용하지 않음
	if (!hRTCP)
	{
	    hRTCP = rtpGetRTCPSession(hRTP);
		if(!hRTCP)
		{
			TRACE("\nError : RTCP Open Error !");
		}
	}
	//*/

	openPort -=2;

	RTPReceiveStart();
	m_enumRTPStatus = CreateRTPSession;    
	
	return openPort;
}

BOOL CRTPCtrl::RTPStart(char *remoteIP, UINT remotePort)
{
	if(m_enumRTPStatus == Idle)
	{
		TRACE("\nError : CRTPCtrl::RTPStart error !!");
		return FALSE;
	}
	
	rtpSetRemoteAddress(hRTP, inet_addr(remoteIP), remotePort);
	//rtcpSetRemoteAddress(hRTCP, inet_addr(remoteIP), remotePort+1);
	// rtcp를 사용하지 않음
	
	m_enumRTPStatus = StartRTPSession;
	return TRUE;
	
}


///////////////////////////////////////////////////////////////////////
// RTPOpen함수 호출시에 자동으로 호출함.
BOOL CRTPCtrl::RTPReceiveStart()
{
	if(!hRTP)
	{
		return FALSE;
	}

	rtpSetEventHandler(hRTP, rtpEventHandler, this);
	return TRUE;
}


BOOL CRTPCtrl::RTPEnd()
{	
	if(hRTP)
	{
		rtpClose(hRTP);
	}
	
	this->InitializeVariable();

	return TRUE;
}

UINT CRTPCtrl::GetLocalPort()
{
	return localPort;
}

int CRTPCtrl::SendRTPData(BYTE *data, DWORD dwSize, MediaType ty)
{
	rtpParam rParam;		// rtp Header의 구조체	
	H263param m_h263p;		// H263 정보 구조체
	BYTE*	Buff=NULL;
	int nHeader;
	int re;
	
	if(hRTP == NULL || m_enumRTPStatus != StartRTPSession )
	{
		return 0;
	}

	if(ty == t_Audio )		// Audio Data 
	{
#ifdef _G7231CODEC								// RTPCtrl.cpp
		nHeader = rtpG7231GetHeaderLength();	
#endif
#ifdef _G729CODEC								// RTPCtrl.cpp
		nHeader = rtpG729GetHeaderLength();
#endif
		
		Buff = new BYTE[nHeader+dwSize];
		
		memset(&rParam, 0x00, sizeof(rtpParam));
		memset(Buff, 0x00, nHeader+dwSize);
		
		memcpy(&Buff[nHeader], data, dwSize);
		
		rParam.timestamp = timeStamp;
		rParam.sequenceNumber = mySequenceNumber++;		
		rParam.sByte = nHeader;
		
#ifdef _G7231CODEC								// RTPCtrl.cpp
		timeStamp = timeStamp + 240 *  dwSize/m_nPacketSize;
		rtpG7231Pack( Buff, rParam.sByte+dwSize ,&rParam,   NULL);
#endif		
#ifdef _G729CODEC								// RTPCtrl.cpp
		timeStamp = timeStamp + 160 *  dwSize/m_nPacketSize;
		rtpG729Pack( Buff, rParam.sByte+dwSize ,&rParam,   NULL);
#endif		
	}
	else if(ty == t_Video)		// Video Data
	{
		nHeader = rtpH263GetHeaderLength();

		Buff = new BYTE[nHeader+dwSize];

		memset(&rParam, 0x00, sizeof(rtpParam));
		memset(&m_h263p, 0x00, sizeof(H263param));
		memset(Buff, 0x00, nHeader+dwSize);

		rParam.timestamp = timeStamp;
		rParam.sequenceNumber = mySequenceNumber++;
		rParam.sByte = nHeader;

		timeStamp = timeStamp + 5000;

		memcpy( &Buff[rParam.sByte], data, dwSize);
		
		rtpH263Pack(Buff, dwSize + nHeader, &rParam, &m_h263p);		
	}
		
	if(hRTP != NULL)
	{
		re = rtpWrite(hRTP , Buff, nHeader+dwSize,&rParam); 

		// log 를 남기기 위해..
		// 2003.06.25 by gomeisa
		m_AudioSound->m_lTxRTPData += nHeader+dwSize;
		m_AudioSound->PostThreadMessage(WM_RTPDATA_WRITE, m_AudioSound->m_lTxRTPData, 10006);
	}

	if(Buff)
	{
		delete[] Buff;
	}

	if(re <= 0)
	{
		return 0;
	}
	
	return re;
}

void CRTPCtrl::InitializeVariable()
{
	hRTP =NULL;
	hRTCP = NULL;    
	timeStamp = 0;
	localPort = 0;
	recvPacketCount = 0;

	memset(&rtcpInfo, 0x00, sizeof(RTCPINFO));

	m_bFirstRtpPacket	= TRUE;
	m_Payload = t_NULL;
	m_enumRTPStatus = Idle;

	m_pVideoDecoder = NULL;	
}



// 연결하여 사용할 Audio and Video Type을 저장하는 global variable
MediaType	g_AudioPayloadType;
MediaType	g_VideoPayloadType;

MediaType GetAudioPayloadType(void)
{
	return g_AudioPayloadType;
}
MediaType GetVideoPayloadType(void)
{
	return g_VideoPayloadType;
}
void SetAudioPayloadType(MediaType ty)
{
	g_AudioPayloadType = ty;	
}
void SetVideoPayloadType(MediaType ty)
{
	g_VideoPayloadType = ty;
}

MediaType DecisionAudioOrVideo(MediaType ty)
{	

	switch (ty)
	{
	case t_PCMU:
	case t_G7231Low:								
	case t_G7231:							
	case t_PCMA:
	case t_G722:
	case t_G729:	
		{
			return t_Audio;			// Audio Data
		}		
	case t_H261:
	case t_H263:
		{
			return t_Video;			// Video Data
		}		
	default:
		return t_NULL;				// Nothing and other
	}
	
}


BOOL CRTPCtrl::GetRTCPInfo(unsigned int ssrc)
{	
	if( hRTP == NULL || hRTCP == NULL)
		return FALSE;

	memset(&rtcpInfo, 0x00, sizeof(RTCPINFO));
	rtcpGetSourceInfo(hRTCP, ssrc, &rtcpInfo);

#ifdef _KWON_DEBUG
	//TRACE("\n		\nselfNode[%d]", rtcpInfo.selfNode);
	/*
	selfNode
	If this field is set to TRUE this structure contains information about a source created by this
	session (like a data source created by a previous call to the rtcpOpen() function). In this case
	only the sr and cname fields are relevant in the structure.
	//*/

	//TRACE("\n	Sender report	==> valid[%2d]  mNTP[%ld]  lNTP[%ld]  timestamp[%ld]  packets[%ld]  octet[%ld]", 
	//	rtcpInfo.sr.valid, rtcpInfo.sr.mNTPtimestamp, rtcpInfo.sr.lNTPtimestamp, rtcpInfo.sr.timestamp, rtcpInfo.sr.packets,rtcpInfo.sr.octets);
	//TRACE("\n	Receiver report	==> valid[%2d]  fractionLost[%ld]  cumulativeLost[%ld]  sequenceNumber[%ld]  jitter[%ld]  lSR[%ld]  dlSR[%ld]", 
	//	rtcpInfo.rrFrom.valid, rtcpInfo.rrFrom.fractionLost, rtcpInfo.rrFrom.cumulativeLost, rtcpInfo.rrFrom.sequenceNumber, rtcpInfo.rrFrom.jitter,rtcpInfo.rrFrom.lSR, rtcpInfo.rrFrom.dlSR);
	TRACE("\n	Local receiver report ==> fractionLost[%ld]  cumulativeLost[%ld]  jitter[%ld]", 
		rtcpInfo.rrTo.fractionLost, rtcpInfo.rrTo.cumulativeLost, rtcpInfo.rrTo.jitter);
	
	//TRACE("\n		cname[%s]", rtcpInfo.cname);
	/*
	cname
	Character line that contains the cname of the source that is identified by the ssrc parameter.
	//*/
#endif
	
	return TRUE;
}

void CRTPCtrl::ReceivePacket(rtpParam* p)
{
	recvPacketCount++;

	if(m_bFirstRtpPacket)		// First RTP Packet ==> member varibale initialize
	{		
		m_bFirstRtpPacket = FALSE;	
	}
	else
	{		
	}
}

