// H323Connection.cpp: implementation of the CH323Connection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "H323Connection.h"
#include "RTPCtrl.h"
#include "SCDialer1Ctl.h"				// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
//
//

/*
// Constructin Parameter 
typedef enum
{
	t_PCMU		= 0,
	t_G7231Low	= 50,		// G.7231 Low
	t_G7231		= 4,		// G.7231 High
	t_PCMA		= 8,
	t_G722		= 9, 
	t_G728		= 15,
	t_G729		= 18,
	t_H261		= 31,
	t_H263		= 34
} MediaType;
//*/

//////////////////////////////////////////////////////////////////////////////////////////////////
// Construction(MediaType audioPayload, MediaType videoPayload, int nFrameCount, int JitterBufferSize)
// 
// - CH323Connection 의 생성자 
// - 클래스의 멤버 변수를 초기화 하고 _Initialize함수를 호출한다.
// - audioPayload 는 연결에 사용할 Audio Codec Type
// - videoPayload 는 연결에 사용할 Video Codec Type
// - InbandDTMF 는 Inband DTMF를 위한 Flag
// - nFrameCount 는 하나의 RTP Packet으로 전송할 Audio Frame 의 개수( 1 ~ 5 사의의 값이어야함)
// - JitterBufferSize 는 지터버퍼의 Play 사이즈를 나타낸다( 4 ~ 10 사이의 값이 적당함)
//////////////////////////////////////////////////////////////////////////////////////////////////

// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 인자추가(CSCDialer1Ctrl *i_pDialer1)
CH323Connection::CH323Connection(CSCDialer1Ctrl *i_pDialer1, MediaType audioPayload, MediaType videoPayload, BOOL InbandDTMF, int nFrameCount, int JitterBufferSize
								 , HWND hCtrl, BOOL bSerial, int nSerialPort, int nTermperSecond)
{
#ifdef _KWON_DEBUG
	TRACE("\n\n===============================================================");
	TRACE("\n==> CH323Connection Constructor");
#endif

	SetAudioPayloadType(audioPayload);		// 연결에 사용할 Audio Codec Type setting
	SetVideoPayloadType(videoPayload);		// 연결에 사용할 Video Codec Type setting

	m_ConnType = t_NULL;

	if(audioPayload != t_NULL && videoPayload != t_NULL)
	{
		m_ConnType = t_AudioAndVideo;						// Both Audio and Video
	}
	else if(videoPayload == t_NULL)
	{	
		m_ConnType = t_Audio;								// Only Audio
	}
	else 
	{
		m_ConnType = t_Video;								// Only Video
	}
	
	m_pAudioSound		= NULL;
	m_pRTPCtrlManager	= NULL;	

	memset(&m_bitFlag, 0x00, sizeof(BitFlag));				// 
	m_bitFlag.InbandDTMF = InbandDTMF;

	m_pSCDialer1Ctrl		= i_pDialer1;			// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 

	_Initialize(nFrameCount,JitterBufferSize, hCtrl, bSerial, nSerialPort, nTermperSecond);											// Initialization Function
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ~CH323Connection()
//
// - 소멸자 
// - 멤버 변수들을 소멸시키는 작업을 한다.
//////////////////////////////////////////////////////////////////////////////////////////////////
CH323Connection::~CH323Connection()
{
#ifdef _KWON_DEBUG
	TRACE("\n==> CH323Connection Destructor");
#endif
	TRACE("\nCH323Connection::~CH323Connection()\n");
	if(m_pAudioSound && m_bitFlag.MixStart == 1)
	{
		// audio rtp buffering starting
		m_pAudioSound->PostThreadMessage(WM_MIX_STOP, 0, 0);				
		m_bitFlag.MixStart = 0;
		Sleep(20);
	}
	if(m_pAudioSound)
	{
		HANDLE	hThread = m_pAudioSound->m_hThread;
		if(hThread)
		{
			Sleep(100);
			m_pAudioSound->PostThreadMessage(WM_AUDIO_ENDTHREAD,0,0);
			::WaitForSingleObject(hThread, 1000);
			m_pAudioSound = NULL;
		}
	}
	
	if(m_pRTPCtrlManager)
	{
		delete m_pRTPCtrlManager;	
		m_pRTPCtrlManager = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// _Initialize()
//
// - 클래스의 생성자에서 호출되는 함수로 멤버 변수를 생성한다. 
//////////////////////////////////////////////////////////////////////////////////////////////////
void CH323Connection::_Initialize(int nFrameCount, int JitterBufferSize, HWND hCtrl, BOOL bSerial, int nSerialPort,int nTermperSecond)
{	
	// audio channel이 사용될때만 audio 관련 클래스 객체를 생성한다.
	TRACE("\nCH323Connection FrameCount=%d, JitterBufferSize=%d bSerial=%d, nSerialPort=%d\n", nFrameCount, JitterBufferSize, bSerial, nSerialPort);
	if( m_ConnType == t_Audio || m_ConnType == t_AudioAndVideo)			
	{
		// 음성을 녹음하는 클래스 생성 (thread)
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 인자추가(, CH323Connection *i_p323Conn)
		m_pAudioSound = new CAudioSound(nFrameCount, JitterBufferSize, hCtrl, bSerial, nSerialPort, nTermperSecond, this);
		if(m_pAudioSound && !m_pAudioSound->CreateThread())		// 스레드 생성
		{
			AfxMessageBox("m_pAudioSound CreateThread Error!");
			return;
		}		
	}
	
	// RTP Channel 관리하는 클래스인 CRTPCtrlManager 객체를 생성.
	// 인자로 RTP Channel이 audio인지 video인지 혹은 audio와 video를 둘다 
	// 사용하는지를 생성자의 인자로 사용한다.
	m_pRTPCtrlManager = new CRTPCtrlManager(m_ConnType, m_pAudioSound);		
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// int CH323Connection::CreateRTPSession(UINT Port, MediaType ty, int sNum);
//
// - RTP Channel을 하나 만든다.
// - Port 는 channel이 open할 포트번호이다.
// - ty 는 Open할 채널의 미디어 타입
// - sNum 은 RTP channel의 구분자이다.  channel마다 다른 구분자를 인자로 주면된다.
// - channel이 만들어 지면 만들어진 channel로 data를 받을수 있는 상태가 된다.
// - return 값은 open 한 채널의 미디어 타입
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CH323Connection::CreateRTPSession(UINT Port, MediaType ty, int sNum)
{
	MediaType Ret = t_NULL ;

	if(!(m_ConnType & ty) || ty == t_AudioAndVideo)
	{
		// 생성된 H323Connection의 미디어 타입과 CreateRTPSession 할 미디어 타입이 맞지 않는다.
		return Ret;
	}
	
	// audio channel이 사용될때 CPlaySound클래스를  play 가능 상태로 준비시키고
	// CMixSound클래스 또한 버퍼링이 가능하게 준비시킨다.
	if(ty == t_Audio || ty == t_AudioAndVideo)
	{
		if(m_bitFlag.PlayStart == 0)
		{
			// Play Sound Start before Audio Port open
			m_pAudioSound->PostThreadMessage(WM_PLAY_START, 0, 0);	
			m_bitFlag.PlayStart = 1;
			// Times for Audio Device open
			Sleep(100);
		}

		if(m_bitFlag.MixStart == 0)
		{
			// audio rtp buffering starting
			m_pAudioSound->PostThreadMessage(WM_MIX_START, 0, 0);				
			m_bitFlag.MixStart = 1;
		}
	}

	// rtp channel open
	Ret = m_pRTPCtrlManager->RtpOpen(sNum, ty, Port);

#ifdef _KWON_DEBUG
	TRACE("\n	CreateRTPSession index[%d],  In[%d] ==> return [%d]", sNum, m_ConnType, Ret);	
#endif
	
	if(ty == t_NULL)
	{
		return FALSE;
	}
	
	return TRUE;		
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// int CH323Connection::StartRTPSession(char* ip, UINT port, MediaType ty, int sNum)
//
// - channel의 상대방 IP Address와  Port를 설정한다.
// - ip 는 RTP 데이타를 전송할 상대방의 IP Address
// - port 는 RTP 데이타를 전송할 상대방의 Port
// - ty 는 Start할 채널이 미디어 타입
// - sNum 은 channel의 구분자로 이미 만들어진 channel의 구분자를 받는다.
// - return 값은 Start 한 채널의 미디어 타입
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CH323Connection::StartRTPSession(char* ip, UINT port, MediaType ty, int sNum)
{
	MediaType Ret = t_NULL ;

	if(!(m_ConnType & ty) || ty == t_AudioAndVideo)
	{
		// 생성된 H323Connection의 미디어 타입과 StartRTPSession 할 미디어 타입이 맞지 않는다.
		return Ret;
	}

#ifdef _KWON_DEBUG
		TRACE("\n	StartRTPSession MediaType[%d] index[%d] Destination IP:%s,  Audio Port:%d", ty, sNum, ip, port);
#endif		

	// 상대방 IP Address와 Video Port를 설정한다.
	Ret = m_pRTPCtrlManager->RTPStart(sNum, ip, ty, port);
	
	// audio channel사용시 sound recording을 시작한다.
	if(ty == t_Audio && m_bitFlag.RecordStart == 0 )
	{		
		m_pAudioSound->PostThreadMessage(WM_RECORD_START, (WPARAM)NULL, (LPARAM)m_pRTPCtrlManager );
		m_bitFlag.RecordStart = 1;
	}
	
	if(ty == t_NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// int CH323Connection::EndRTPSession(MediaType ty, int sNum);
//
// - 이미 만들어진 channel 을 닫는 함수이다.
// - 인자인 sNum은 만들어진 channel의 구분자이다.
//////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CH323Connection::EndRTPSession(MediaType ty, int sNum)
{	
	int count;
	MediaType	Ret = t_NULL;

	if(!(m_ConnType & ty) || ty == t_AudioAndVideo)
	{
		// 생성된 H323Connection의 미디어 타입과 StartRTPSession 할 미디어 타입이 맞지 않는다.
		return Ret;
	}

	if(ty == t_Audio)
	{
		// 현재 연결되어 있는 Audio session의 count를 구한다.
		count = m_pRTPCtrlManager->GetSessionCount(ty);
		
#ifdef _KWON_DEBUG
		if(count > 0)
		{
			TRACE("\n	EndRTPSession Audio index[%d] SessionCount[%d]", sNum, count);
		}
#endif		
		// audio channel을 사용하고 현재 연결된 session의 count가 1이면 		
		if(count <= 1)
		{
			if(m_bitFlag.RecordStart)
			{
				m_pAudioSound->PostThreadMessage(WM_RECORD_STOP, 0L, 0L);
				m_bitFlag.RecordStart = 0;
				Sleep(100);
			}

			// 열려진 rtp channel을 닫는다.
			Ret = m_pRTPCtrlManager->RTPEnd(sNum, ty);
			
			//if(m_bitFlag.MixStart)
			//{
			//	m_pAudioSound->PostThreadMessage(WM_MIX_STOP, 0, 0);
			//	m_bitFlag.MixStart = 0;				
			//}

			if(m_bitFlag.PlayStart)
			{
				m_pAudioSound->PostThreadMessage(WM_PLAY_STOP,0L,0L);
				m_bitFlag.PlayStart = 0;
			}						
		}
		else if( count > 1)
		{
			int idx = m_pRTPCtrlManager->SessionToIndex(ty, sNum);
			// 열려진 rtp channel을 닫는다.
			Ret = m_pRTPCtrlManager->RTPEnd(sNum, ty);

			m_pAudioSound->PostThreadMessage(WM_MIX_ENDRTPSESSION, 0, idx);
		}

	}
	else
	{
		// 현재 연결되어 있는 Video session의 count를 구한다.
		count = m_pRTPCtrlManager->GetSessionCount(ty);
#ifdef _KWON_DEBUG
		TRACE("\n	EndRTPSession Video index[%d] SessionCount[%d]", sNum, count);
#endif
		// 열려진 rtp channel을 닫는다.
		Ret = m_pRTPCtrlManager->RTPEnd(sNum, ty);

	}
	
	if(ty == t_NULL)
	{
		return FALSE;
	}
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// int	CH323Connection::EndAllRTPSession();
//
// - 현재 연결된 session을 모두 닫는다.
//////////////////////////////////////////////////////////////////////////////////////////////////
int	CH323Connection::EndAllRTPSession()
{
	int r;
	
	// audio channel을 사용한다면 audio recording과 audio playing을 멈춘다.
	if(m_ConnType == t_Audio || m_ConnType == t_AudioAndVideo)
	{
		m_pAudioSound->PostThreadMessage(WM_RECORD_STOP, 0L, 0L);
		m_bitFlag.RecordStart = 0;
		Sleep(100);
				
		// 모든 rtp channel을 닫는다.
		if(m_pRTPCtrlManager){ r = m_pRTPCtrlManager->ALLRTPEnd();}
		
		//if(m_bitFlag.MixStart)
		//{
		//	m_pAudioSound->PostThreadMessage(WM_MIX_STOP, 0, 0);
		//	m_bitFlag.MixStart = 0;
		//	Sleep(100);
		//}
		
		m_pAudioSound->PostThreadMessage(WM_PLAY_STOP,0L,0L);
		m_bitFlag.PlayStart = 0;				
	}
	else
	{
		// 모든 rtp channel을 닫는다.
		r = m_pRTPCtrlManager->ALLRTPEnd();
	}	

	return r;
}


//////////////////////////////////////////////////////////////////////
// UINT CH323Connection::GetLocalAudioPort(int sNum);
//
// - 인자로 받은 sNum의 구분자를 가진 channel의 열려진 Audio port를 리턴한다.
//////////////////////////////////////////////////////////////////////
UINT CH323Connection::GetLocalAudioPort(int sNum)
{
	return m_pRTPCtrlManager->GetLocalPort(sNum, t_Audio);
}


//////////////////////////////////////////////////////////////////////
// UINT CH323Connection::GetLocalVideoPort(int sNum)
//
// - 인자로 받은 sNum의 구분자를 가진 channel의 열려진 Video port를 리턴한다.
//////////////////////////////////////////////////////////////////////
UINT CH323Connection::GetLocalVideoPort(int sNum)
{
	return m_pRTPCtrlManager->GetLocalPort(sNum, t_Video);
}

//////////////////////////////////////////////////////////////////////
// UINT CH323Connection::GetLocalVideoPort(int sNum)
//
// - 인자로 받은 sNum의 구분자를 가진 channel의 열려진 Video port를 리턴한다.
//////////////////////////////////////////////////////////////////////
UINT CH323Connection::GetLocalPort(MediaType ty, int sNum)
{
	return m_pRTPCtrlManager->GetLocalPort(sNum, ty);
}


//////////////////////////////////////////////////////////////////////
// int CH323Connection::SendVideoData(BYTE *Data, DWORD dwSize, int ty);
//
// - Video Data를 rtp channel로 보낸다.
//////////////////////////////////////////////////////////////////////
int CH323Connection::SendVideoData(BYTE *Data, DWORD dwSize)
{
	return m_pRTPCtrlManager->SendRTPData(Data, dwSize, t_Video);
}



//////////////////////////////////////////////////////////////////////
// SendDTMF(int number);
//
// - 상대방에 DTMF를 RTP Packet으로 보낸다.( In-band )
// - m_bitFlag.InbandDTMF 플래그가 TRUE 일때 In-band 로 전송한다
// - number 는 전송 할 DTMF 로 문자열을 받아서 메세지로 보낼때 int값으로 변경
//////////////////////////////////////////////////////////////////////
void CH323Connection::SendDTMF(const char* number)
{
	if(m_pAudioSound && m_bitFlag.InbandDTMF)
	{
		m_pAudioSound->PostThreadMessage(WM_RECORD_SENDDTMF, 0L, atoi(number));
	}
}



#ifdef	_LOCAL_PLAY_G7231
//////////////////////////////////////////////////////////////////////
// void CH323Connection::PlayRingBackTone(BOOL bIN);
//
// - Ringback Tone을 자체적으로 Play 한다.
// - 인자인 bIN이 참이면 Ringback Tone play를 시작하고 거짓이면 play를 멈춘다.
// - windows98에서 PlaySound()함수가 동작하지 않을때 사용함. 
// - nFlag : 0 이면 Ring back tone play, 1 이면 안내멘트 play
//////////////////////////////////////////////////////////////////////
void CH323Connection::LocalPlayG7231(int nFlag, BOOL bIN)
{	
	TRACE("\nLocalPlayG7231(%d,%d)\n",nFlag,bIN);
	if(bIN)
	{		
		if(m_bitFlag.PlayStart == 0)
		{
			// Play Sound Start before Audio Port open
			m_pAudioSound->PostThreadMessage(WM_PLAY_START, 0, 0);	
			m_bitFlag.PlayStart = 1;
			// Times for Audio Device open
			Sleep(50);
		}
		
		if(m_bitFlag.MixStart == 0)
		{
			// audio rtp buffering starting
			m_pAudioSound->PostThreadMessage(WM_MIX_START, 0, 0);				
			m_bitFlag.MixStart = 1;				
		}
	}
	else
	{
		/*
		// 현재 연결되어 있는 Audio session의 count를 구한다.			
		if(!m_pRTPCtrlManager->GetSessionCount(t_Audio))
		{
			if(m_bitFlag.PlayStart)
			{
				m_pAudioSound->PostThreadMessage(WM_PLAY_STOP,0L,0L);
				m_bitFlag.PlayStart = 0;
			}
		}
		//*/
	}
				
	m_pAudioSound->PostThreadMessage(WM_MIX_LOCALPLAYG7231, nFlag, bIN); // 	
}
#endif

//////////////////////////////////////////////////////////////////////
// void CH323Connection::SendMuteAudioRTP(BOOL bIN);
//
// - 묵을을 보낸다.
//////////////////////////////////////////////////////////////////////
void CH323Connection::SendMuteAudioRTP(BOOL bIN)
{
	if(m_pAudioSound)
	{
		m_pAudioSound->PostThreadMessage(WM_RECORD_SENDMUTEAUDIORTP, (WPARAM)0, (LPARAM)bIN );
	}
}

//////////////////////////////////////////////////////////////////////
// BOOL CH323Connection::SetVideoDecoder(void *de);
//
// - 
//////////////////////////////////////////////////////////////////////
BOOL CH323Connection::SetVideoDecoder(void *de)
{
	m_pRTPCtrlManager->SettingPARAM(de, t_Video);		// 2 is Video
	return TRUE;
}



#ifdef	_RECORDING_FILE
//////////////////////////////////////////////////////////////////////
// void CH323Connection::SetRecordingFilePointer(FILE* pfA, FILE* pfC)
//
// - 녹취에 필요한 file pointer를 설정하는 변수임
// - pfA는 Agent쪽 file pointer (자신의 음성)
// - pfC는 Client쪽 file pointer (상대방 음성)
//////////////////////////////////////////////////////////////////////
void CH323Connection::SetRecordingFilePointer(FILE* pfA, FILE* pfC)
{
	if(m_pAudioSound && m_pMixSound)
	{
		m_pAudioSound->PostThreadMessage(WM_RECORDSOUND_RECORDINGFILEPOINTER, (WPARAM)NULL, (LPARAM)pfA);
		m_pMixSound->PostThreadMessage(WM_MIXSOUND_RECORDINGFILEPOINTER, (WPARAM)NULL, (LPARAM)pfC);
	}	
}

//////////////////////////////////////////////////////////////////////
// void CH323Connection::PlayRecordingFile(FILE* pfA, FILE* pfC)
//
// - 녹취한 두 file를 play 하는 함수임 
// - 두 화일을 설정한 후에 바로 플레이를 시작한다.
// - pfA는 Agent쪽 file pointer
// - pfC는 Client쪽 file pointer
//////////////////////////////////////////////////////////////////////
void CH323Connection::PlayRecordingFile(FILE* pfA, FILE* pfC)
{
	if(m_pPlaySound && m_pMixSound)
	{
		
		if(m_bitFlag.PlayStart == 0)
		{
			// Play Sound Start before Audio Port open
			m_pPlaySound->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);	
			m_bitFlag.PlayStart = 1;
			// Times for Audio Device open
			Sleep(100);
		}						
		
		m_pMixSound->PostThreadMessage(WM_MIXSOUND_SETPLAYRECORDINGFILE, (WPARAM)pfA, (LPARAM)pfC);

		if(m_bitFlag.MixStart == 0)
		{
			// audio rtp buffering starting
			m_pMixSound->PostThreadMessage(WM_MIXSOUND_STARTMIXTHREAD, 0, 0);				
			m_bitFlag.MixStart = 1;
		}
	}
}	
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CH323Connection::GetPlayVolumeW(BYTE *o_pbyVolPercent)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CH323Connection::GetPlayVolumeW(BYTE *o_pbyVolPercent)
{
	if(NULL == m_pAudioSound)
	{
		return FALSE;
	}
	return m_pAudioSound->GetPlayVolume(o_pbyVolPercent);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CH323Connection::SetPlayVolumeW(BYTE i_byVolPercent)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CH323Connection::SetPlayVolumeW(BYTE i_byVolPercent)
{
	if(NULL == m_pAudioSound)
	{
		return FALSE;
	}
	return m_pAudioSound->SetPlayVolume(i_byVolPercent);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CH323Connection::OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
/// \brief		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
/// \author		cmkwon
/// \date		2008-10-22 ~ 2008-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CH323Connection::OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
{
	m_pSCDialer1Ctrl->OnVolume(i_bIsSpkDevice, i_byVolPercent);	
}

CString CH323Connection::CheckAudioDevice()
{
	CString strDeviceList;
	CString strDevice;
	UINT num = ::waveOutGetNumDevs();
	for(UINT id = 0; id < num; id++)
	{
		WAVEOUTCAPS wodc;
		::waveOutGetDevCaps(id, &wodc, sizeof(WAVEOUTCAPS));
		
		strDevice.Format("%s", wodc.szPname);	
		strDeviceList += strDevice;
		strDeviceList += ";";
	} 

	strDeviceList.Delete(strDeviceList.GetLength() - 1);

	strDeviceList += ",";

	num = ::waveInGetNumDevs();
	for(id = 0; id < num; id++)
	{
		WAVEINCAPS widc;
		::waveInGetDevCaps(id, &widc, sizeof(WAVEINCAPS));

		strDevice.Format("%s", widc.szPname);
		strDeviceList += strDevice;
		strDeviceList += ";";
	}

	strDeviceList.Delete(strDeviceList.GetLength() - 1);

	return strDeviceList;
}

BOOL CH323Connection::SetRecordSoundDevice(CString strDevice)
{
	if(m_pAudioSound)
	{
		return m_pAudioSound->SetRecordSoundDevice(strDevice);		
	}
	return FALSE;
}

BOOL CH323Connection::SetPlaySoundDevice(CString strDevice)
{
	if(m_pAudioSound)
	{
		return m_pAudioSound->SetPlaySoundDevice(strDevice);		
	}
	return FALSE;
}

void CH323Connection::SerialPhoneMessage(DWORD dwMsg)
{
	if(m_pAudioSound)
	{
		m_pAudioSound->PostThreadMessage(WM_SERIAL_MESSAGE, 0, dwMsg);
	}
}

void CH323Connection::SetInBandDTMF(BOOL bInBand)
{
	m_bitFlag.InbandDTMF = bInBand;
}

void CH323Connection::ShowVolumeConfig(short nMode)
{
	if(m_pAudioSound)
		m_pAudioSound->ShowVolumeConfig(nMode);
}

void CH323Connection::SetKeepAliveSerial(CString strAliveKeyList)
{

	TRACE("\n[SetKeepAliveKey] Key = %s, m_pAudioSound - 0x%0x\n", strAliveKeyList, m_pAudioSound);


	if(!m_pAudioSound)
		return;

	CStringArray arrayTemp;
	int count = ParseData(strAliveKeyList, ',', &arrayTemp);
	if(count > 1)
	{
		CStringArray arrayValid;
		count = ParseData(arrayTemp.GetAt(0), ';', &arrayValid);
		if(count == 2)
		{
			m_pAudioSound->m_nKeepAliveHookOn = atoi(arrayValid.GetAt(0));
			m_pAudioSound->m_nKeepAliveHookOff = atoi(arrayValid.GetAt(1));
			TRACE("\n[SetKeepAliveKey] ValidKey : HookOn(%d), HookOff(%d)\n", m_pAudioSound->m_nKeepAliveHookOn, m_pAudioSound->m_nKeepAliveHookOff);
		}
		count = ParseData(arrayTemp.GetAt(1), ';', &m_pAudioSound->m_arrayInvalidKeepAliveList);
		TRACE("\n[SetKeepAliveKey] InValidKey : %s, Count = %d\n", arrayTemp.GetAt(1), m_pAudioSound->m_arrayInvalidKeepAliveList.GetSize());
	}
}

int CH323Connection::ParseData(CString str, char sep, CStringArray *array)
{
	int start = 0;
	int step = 0;
	int count = 0;
	array->RemoveAll();

	if(str.Right(1) != sep)
		str += sep;

	do
	{
		step = str.Find(sep, start);
		array->Add(str.Mid(start, step - start));
		start = step + 1;
		count++;
	}while(start < str.GetLength());

	int size = array->GetSize();
	if(size > 9 )
	{
		for(int i=0; i<size; i++)
		{
			CString str = array->GetAt(i);
			str.Remove(' ');
			array->SetAt(i, str); 
		}	
	}
	
	return count;

}

void CH323Connection::SetBusyTone(BOOL bUse)
{
	if(m_pAudioSound)
		m_pAudioSound->m_bSerialBusytoneReady = bUse;
}

BOOL CH323Connection::LogRecordStart()
{
/*	LocalPlayG7231(0, FALSE);

	if(m_pAudioSound && m_pAudioSound->m_pRawFile)
	{
		m_pAudioSound->m_pRawFile->Close(); 
		delete m_pAudioSound->m_pRawFile; 
		m_pAudioSound->m_pRawFile = NULL;
	}
	m_pAudioSound->m_pRawFile = new CFile();
	if(m_pAudioSound->m_pRawFile->Open("record.raw", CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite) != 0)
	{
		CreateRTPSession(4010, t_Audio, 0);
		StartRTPSession("127.0.0.1", 4010, t_Audio, 0);
		m_pAudioSound->m_bLogWriteFile = TRUE;
	}
	else
	{
		delete m_pAudioSound->m_pRawFile;
		m_pAudioSound->m_pRawFile = NULL;
		m_pAudioSound->m_bLogWriteFile = FALSE;
		AfxMessageBox("화일을 Open할수 없습니다.");
		return FALSE;
	}
*/
	return TRUE;
}

BOOL CH323Connection::LogRecordStop()
{
/*	EndAllRTPSession();		
	
	if(m_pAudioSound && m_pAudioSound->m_pRawFile)
	{
		m_pAudioSound->m_bLogWriteFile = FALSE;
		m_pAudioSound->m_pRawFile->Close();
		delete m_pAudioSound->m_pRawFile;
		m_pAudioSound->m_pRawFile = NULL;
	}
*/
	return TRUE;
}

BOOL CH323Connection::LogPlayStart()
{
	LocalPlayG7231(0, FALSE);

/*	if(m_pAudioSound && m_pAudioSound->m_pRawFile)
	{
		m_pAudioSound->m_pRawFile->Close();
		delete m_pAudioSound->m_pRawFile;
		m_pAudioSound->m_pRawFile = NULL;
	}
	
	m_pAudioSound->m_pRawFile = new CFile();
	if(m_pAudioSound->m_pRawFile->Open("record.raw", CFile::modeNoTruncate | CFile::modeRead) != 0)
	{
		m_pAudioSound->m_pRawFile->SeekToBegin(); 
		CreateRTPSession(4010, t_Audio, 0);
		StartRTPSession("127.0.0.1", 4010, t_Audio, 0);
	}
	else
	{
		delete m_pAudioSound->m_pRawFile;
		m_pAudioSound->m_pRawFile = NULL;
		AfxMessageBox("화일을 Open할수 없습니다.");
		return FALSE;
	}
*/
	if(m_pAudioSound)
	{
		CreateRTPSession(5010, t_Audio, 0);
		StartRTPSession("127.0.0.1", 5010, t_Audio, 0);
	}
	return TRUE;
}

BOOL CH323Connection::LogPlayStop()
{
	if(m_pAudioSound)
	{
		EndAllRTPSession();
	}	
	
	return TRUE;
}

#ifdef _ECHO_CANCELLER
//////////////////////////////////////////////////////////////////////////
// cmkwon 2003.09.05일 추가
// EchoCanceller를 적용을 설정하는 함수
// bFlag를 적용시 TRUE로 하면됨
// nOffset은 장치에 따라 다를수 있음
//		- Windows Audio 장치를 이용할 경우 1로 하면됨
void CH323Connection::ApplyEchoCanceller(BOOL bFlag, int nOffset)
{
	if(NULL == m_pAudioSound){	return;}

	m_pAudioSound->InitEchoCanceller(bFlag, nOffset);
}
#endif

void CH323Connection::LogEnable()
{
	if(m_pAudioSound)
	{
		m_pAudioSound->m_bSerialTestMode = TRUE;
	}	
}

void CH323Connection::LogDisable()
{
	if(m_pAudioSound)
	{
		m_pAudioSound->m_bSerialTestMode = FALSE;
	}	
}