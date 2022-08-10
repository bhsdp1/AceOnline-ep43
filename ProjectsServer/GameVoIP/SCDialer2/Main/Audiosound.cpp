// audiosound.cpp : implementation file
//

#include "stdafx.h"
#include "audiosound.h"
#include "G7231Codec.h"
#include "ChannelData.h"
#include "AVManager.h"
#include "VoIPParam.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
CSystemLogManager CAudioSound::ms_SystemLogManager;		// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 

/////////////////////////////////////////////////////////////////////////////
// CAudioSound

IMPLEMENT_DYNCREATE(CAudioSound, CWinThread)

CAudioSound::CAudioSound()
{
	AfxMessageBox("CAudioSound Error");
}

CAudioSound::CAudioSound(CAVManager *pAVMas, MediaType audioPayload, int nFrameCount, int JitterBufferSize)
{
	m_pAVMaster				= pAVMas;
	m_dwLastTick			= timeGetTime();
	///////////////////////////////////////////////////////////////////////////////////
	// Recording 관련 변수 초기화
	m_hRecord				= NULL;
	m_bFlagRecording		= FALSE;					// Recording 시작 플래그
	m_pEncoder				= NULL;						// Audio Encoder 	
	m_nEncodedCount			= 0;
	r_nSendEncodedCount		= 0;
	r_nDeviceIndex			= WAVE_MAPPER;				// waveOut device index
	
	//////////////////////////////////////////////////////////
	// Audio Codec에 따른 설정의 차이(G.723.1High, G.723.1Low)
	m_AudioPayloadType = audioPayload;		
	if(m_AudioPayloadType == t_G7231 )
	{	// 8000 kHz,  16 bitpersample,  Mono
		
		m_nPacketBytes = 24; 
		m_nFrameBytes = 480;
		r_nSendEncodedCount = nFrameCount;
		m_nRecordMaxInputBuffers = 50/nFrameCount;		
	}
	else if( m_AudioPayloadType == t_G7231Low)
	{	// 8000 kHz,  16 bitpersample,  Mono

		m_nPacketBytes = 20; 
		m_nFrameBytes = 480;
		r_nSendEncodedCount = nFrameCount;		
		m_nRecordMaxInputBuffers = 50/nFrameCount;
	}
	else
	{
		char	szError[1024];
		sprintf(szError, "[ERROR] CAudioSound::CAudioSound invalid AudioPayload, audioPayload[%d]\r\n", m_AudioPayloadType);
		DbgOut(szError);
//		CSaveLastError::SaveLastError(szError);
	}
		
	memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
	m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels = 1;
	m_WaveFormatEx.wBitsPerSample = 16;	
	m_WaveFormatEx.nSamplesPerSec = 8000;
	m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec*(m_WaveFormatEx.wBitsPerSample/8);
	m_WaveFormatEx.nBlockAlign = (m_WaveFormatEx.wBitsPerSample/8)*	m_WaveFormatEx.nChannels;
	m_WaveFormatEx.cbSize = 0;
		
	m_pEncoder = new CG7231Codec(1, audioPayload);					// Encoder Create
	if(!m_pEncoder || !m_pEncoder->AudioCodecLoadLibrary())
	{
		char	szError[1024];
		char	szTemp[1024];
		GetCurrentDirectory(1024, szTemp);
		sprintf(szError, "[ERROR] CAudioSound::CAudioSound codec load fail1, CurrentDirectory[%s]\r\n", szTemp);
		DBGOUT(szError);
//		CSaveLastError::SaveLastError(szError);
	}
	r_nAvailableCount = waveInGetNumDevs();							// waveIn driver cap


	///////////////////////////////////////////////////////////////////////////////////
	// Playing 관련 변수 초기화
	m_hPlay					= NULL;
	m_bFlagPlaying			= FALSE;
	m_dwPlayingBytes		= 0;
	p_nDeviceIndex			= WAVE_MAPPER;				// waveIn device index
	
	p_nAvailableCount = waveOutGetNumDevs();
	

	///////////////////////////////////////////////////////////////////////////////////
	// Buffering 관련 변수 초기화
	b_nTimer_id					= 0;
	b_nBufferPlay_Start_Count	= JitterBufferSize;
	b_nBufferPlay_Reset_Count	= b_nBufferPlay_Start_Count + 2;

	m_nPlayVolumeOfPercent	= INVALID_SPK_VOLUME_VALUE;		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 

	m_nSilenceFrameCnt		= 0;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	m_nThresholdLevel		= 500;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

// 2008-10-21 by cmkwon, SCDialer2 수정 - 
//	m_bSilenceState			= FALSE;	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	this->SetSilenceState(FALSE);

	m_nSkipFrameCnt			= 0;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	///////////////////////////////////////////////////////////////////////////////
//	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	ms_SystemLogManager.InitLogManger(TRUE, "SCDialer2", ".\\Res-Voip\\");
}

CAudioSound::~CAudioSound()
{
	SAFE_DELETE(m_pEncoder);	
}

BOOL CAudioSound::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	
	MixTimerStart();
	return TRUE;
}

int CAudioSound::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAudioSound, CWinThread)
	//{{AFX_MSG_MAP(CAudioSound)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_AUDIO_ENDTHREAD, OnAudioEndThread)

	// Recording
	ON_THREAD_MESSAGE(WM_RECORD_STOP, OnRecordStop)
	ON_THREAD_MESSAGE(MM_WIM_DATA, OnRecordSoundData)	

	// Playing
	ON_THREAD_MESSAGE(WM_PLAY_STOP, OnPlayStop)
	ON_THREAD_MESSAGE(WM_PLAY_PLAYBLOCK, OnPlayWriteSoundData)
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnPlayEndSoundData)	
	ON_THREAD_MESSAGE(WM_PLAY_RESET, OnPlayReset)
	
	// buffering
	ON_THREAD_MESSAGE(WM_MIX_PLAY, OnMixPlay)	
	ON_THREAD_MESSAGE(WM_MIX_ENDRTPSESSION, OnMixEndRTPSession)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioSound message handlers
LRESULT CAudioSound::OnAudioEndThread(WPARAM wParam, LPARAM lParam)
{
	if(m_bFlagRecording)
	{
		OnRecordStop(0, 0);
	}

	SAFE_DELETE(m_pEncoder);
	
	if(m_bFlagPlaying)
	{
		OnPlayStop(0,0);
	}

	MixTimerStop();

	::PostQuitMessage(0);
	return TRUE;
}

LPWAVEHDR CAudioSound::CreateWaveHeader(int size)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR)(new BYTE[sizeof(WAVEHDR) + size]);
	ZeroMemory(lpHdr, sizeof(WAVEHDR) + size);
		
	lpHdr->lpData = (char*)lpHdr + sizeof(WAVEHDR);
	lpHdr->dwBufferLength = size;	
	return lpHdr;
}

////////////////////////////////////////////////////////////////////////////////////
// BOOL CAudioSound::SetRecordSoundDevice(CString strDevice)
// - 
////////////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::SetRecordSoundDevice(CString strDevice)
{	
	WAVEINCAPS wodc;

	r_nAvailableCount = waveInGetNumDevs();			// waveIn driver cap
	if(r_nAvailableCount <= 0)
	{
		return FALSE;
	}

	for(int i=0; i < r_nAvailableCount; i++)
	{		
		if(::waveInGetDevCaps(i, &wodc, sizeof(WAVEINCAPS)) == MMSYSERR_NOERROR )
		{
			if(strDevice.Compare(wodc.szPname) == 0)
			{
				r_nDeviceIndex = i;				
				return TRUE;	
			}
		}
		else
		{
			break;
		}
	}	
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////
// BOOL CAudioSound::SetPlaySoundDevice(CString strDevice)
// - 
////////////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::SetPlaySoundDevice(CString strDevice)

{
	WAVEOUTCAPS wodc;

	p_nAvailableCount = waveOutGetNumDevs();
	if(p_nAvailableCount <= 0)
	{
		return FALSE;
	}

	for(int i=0; i < p_nAvailableCount; i++)
	{		
		if(::waveOutGetDevCaps(i, &wodc, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR )
		{
			if(strDevice.Compare(wodc.szPname) == 0)
			{
				p_nDeviceIndex = i;
				TRACE("\n			SetPlaySoundDevice successed(%s),  Total Device[%d],  Set Device Index[%d]", 
					strDevice, p_nAvailableCount, p_nDeviceIndex);

				return TRUE;	
			}
		}
		else
		{
			break;
		}
	}

	TRACE("\n			SetPlaySoundDevice failed(%s),  Total Device[%d],  Set Device Index[%d]", 
		strDevice, p_nAvailableCount, p_nDeviceIndex);

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAudioSound::GetPlayVolume(BYTE *o_pbyVolPercent)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::GetPlayVolume(BYTE *o_pbyVolPercent)
{
	if(NULL == m_hPlay)
	{
		return FALSE;
	}
	
	DWORD dwVol = 0;
	if(MMSYSERR_NOERROR != waveOutGetVolume(m_hPlay, &dwVol))
	{
		return FALSE;
	}
	
	dwVol				= (DWORD)(dwVol & 0xFFFF);	// 좌/우 한쪽 Volume 을 가져온다.
	*o_pbyVolPercent	= (dwVol * 100) / 0xFFFF;	// percent 단위로 변환
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAudioSound::SetPlayVolume(BYTE i_byVolPercent, BOOL i_bApplyInstantly/*=FALSE*/)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::SetPlayVolume(BYTE i_byVolPercent, BOOL i_bApplyInstantly/*=FALSE*/)
{
	i_byVolPercent		= min(100, i_byVolPercent);
	i_byVolPercent		= max(0, i_byVolPercent);
	
	if(NULL == m_hPlay)
	{
		if(i_bApplyInstantly)
		{
			return FALSE;
		}
		
		m_nPlayVolumeOfPercent = i_byVolPercent;
		return TRUE;
	}
	
	DWORD dwVol			= (0xFFFF * i_byVolPercent) / 100;	// Percent 를 변환
	if(MMSYSERR_NOERROR != waveOutSetVolume(m_hPlay, MAKELONG(dwVol, dwVol)))
	{
		return FALSE;
	}
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
/// \author		cmkwon
/// \date		2008-09-22 ~ 2008-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAudioSound::GetAverageSignalLevel(SHORT *i_sSampleBuffer, int i_nSampleCount)
{
	if(0 >= i_nSampleCount)
	{
		return 0;
	}
	
	int nSum = 0;
	
	for(int i=0; i < i_nSampleCount; i++)
	{
		if(0 > i_sSampleBuffer[i])
		{
			nSum -= i_sSampleBuffer[i];
		}
		else
		{
			nSum += i_sSampleBuffer[i];
		}
	}
	
	return nSum/i_nSampleCount;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAudioSound::SetSilenceState(BOOL i_bSilenceState)
/// \brief		// 2008-10-21 by cmkwon, SCDialer2 수정 - 
/// \author		cmkwon
/// \date		2008-10-21 ~ 2008-10-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAudioSound::SetSilenceState(BOOL i_bSilenceState)
{
	m_bSilenceState		= i_bSilenceState;
}

BOOL CAudioSound::RecordStart(void)
{	
	if(m_bFlagRecording || r_nAvailableCount <= 0)
		return FALSE;

	// open wavein device
	MMRESULT mmReturn = ::waveInOpen(&m_hRecord, r_nDeviceIndex, &m_WaveFormatEx, this->m_nThreadID, 0, CALLBACK_THREAD);
	if(mmReturn)
	{	
		for(int i=0; i< r_nAvailableCount; i++)
		{
			mmReturn = ::waveInOpen(&m_hRecord, i, &m_WaveFormatEx, this->m_nThreadID, 0, CALLBACK_THREAD);
			if(!mmReturn)
			{					
				break;
			}
		}
		
		if(mmReturn)
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveInGetErrorText( mmReturn, errorbuffer, MAX_PATH);
			sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);
			
			char	szError[1024];
			sprintf(szError, "[ERROR] CAudioSound::PlayStart waveInOpen error, AvailableCounts[%d] %s\r\n", r_nAvailableCount, errorbuffer1);
			DBGOUT(szError);
//			CSaveLastError::SaveLastError(szError);
		}			
	}
	
	if(!mmReturn)
	{			
		for(int i=0; i < m_nRecordMaxInputBuffers; i++)
		{
			LPWAVEHDR lpHdr = CreateWaveHeader(m_nFrameBytes);
			mmReturn = ::waveInPrepareHeader(m_hRecord,lpHdr, sizeof(WAVEHDR));
			mmReturn = ::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
		}
		
		// Recording Start
		mmReturn = ::waveInStart(m_hRecord);
		if(mmReturn)
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveInGetErrorText( mmReturn, errorbuffer, MAX_PATH);
			sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);				
			
			char	szError[1024];
			sprintf(szError, "[ERROR] CAudioSound::PlayStart waveInStart error, %s\r\n", errorbuffer1);
			DBGOUT(szError);
//			CSaveLastError::SaveLastError(szError);
		}
		else
		{
			m_bFlagRecording = TRUE;				
		}
	}
	
	return m_bFlagRecording;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnRecordStop(WPARAM wParam, LPARAM lParam)
// - Recording stop
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordStop(WPARAM wParam, LPARAM lParam)
{	
	if(!m_bFlagRecording)
		return FALSE;
	
	m_bFlagRecording	= FALSE;
	MMRESULT mmReturn = ::waveInStop(m_hRecord);
	if(!mmReturn)
	{
		mmReturn = ::waveInReset(m_hRecord);
	}	
	if(!mmReturn)
	{
		mmReturn = ::waveInClose(m_hRecord);
	}	
	m_hRecord			= NULL;	

	return mmReturn;	
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnRecordSoundData(WPARAM wParam, LPARAM lParam)
// - 음성 데이타가 버퍼에 다 차면 호출되는 콜백 핸들러
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordSoundData(WPARAM wParam, LPARAM lParam)
{	
	LPWAVEHDR	lpHdr = (LPWAVEHDR)lParam;
	if(NULL == lpHdr){		return	0L;}
	
	::waveInUnprepareHeader(m_hRecord, lpHdr, sizeof(WAVEHDR));	
	if(FALSE == m_bFlagRecording)
	{
		SAFE_DELETE(lpHdr);
		return 0L;
	}
	::waveInPrepareHeader(m_hRecord, lpHdr, sizeof(WAVEHDR));
	::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
	
	if(m_pEncoder)
	{						
		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 아래와 같이 수정 함.
		//m_pEncoder->AudioCodecEncoding((short int*)lpHdr->lpData, &m_pEncodedBuffer[m_nEncodedCount*m_nPacketBytes]);
		//m_nEncodedCount++;
		
		///////////////////////////////////////////////////////////////////////////////
		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		
		// 2009-01-21 by cmkwon, vista에서는 lpHdr->dwBytesRecorded 이 0이 넘어 온다, 아래와 같이 lpHdr->dwBufferLength 로 변경 함.
		// int nAverageSignalLevel	= GetAverageSignalLevel((short int*)lpHdr->lpData, lpHdr->dwBytesRecorded/sizeof(short int));
		int nAverageSignalLevel	= GetAverageSignalLevel((short int*)lpHdr->lpData, lpHdr->dwBufferLength/sizeof(short int));
		m_nSilenceFrameCnt		= (m_nThresholdLevel > nAverageSignalLevel) ? m_nSilenceFrameCnt+1 : 0;
		
		if(0 != m_nEncodedCount
			|| m_nSilenceFrameCnt < 33)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
			if(m_bSilenceState)
			{
				// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
				m_pAVMaster->SendChangedTalkingState(TALKING_STATE_START_TALKING);

//				this->WriteSystemLogEX(FALSE, "말하기 상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//				DbgOut("말하기 상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);
			}
			else
			{
//				this->WriteSystemLogEX(FALSE, "	말하기 상태 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
			}
			
			// 2008-10-21 by cmkwon, SCDialer2 수정 - 
			//m_bSilenceState		= FALSE;
			this->SetSilenceState(FALSE);
			m_pEncoder->AudioCodecEncoding((short int*)lpHdr->lpData, &m_pEncodedBuffer[m_nEncodedCount*m_nPacketBytes]);
			m_nEncodedCount++;
		}
		else
		{
			///////////////////////////////////////////////////////////////////////////////						
			// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
			if(FALSE == m_bSilenceState)
			{
				// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
				m_pAVMaster->SendChangedTalkingState(TALKING_STATE_END_TALKING);

//				this->WriteSystemLogEX(FALSE, "무음   상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//				DbgOut("무음   상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);
			}
			else
			{
//				this->WriteSystemLogEX(FALSE, "	무음   상태 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, m_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
			}
			
			// 2008-10-21 by cmkwon, SCDialer2 수정 - 
			//m_bSilenceState		= TRUE;
			this->SetSilenceState(TRUE);
			m_nSkipFrameCnt++;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
		}
	}
		
	if(m_nEncodedCount >= r_nSendEncodedCount)
	{		
		//////////////////////////////////////////////////////////////////
		// RTP channel 로 인코딩된 데이타를 보낸다.
		if (m_pAVMaster)
		{
			m_pAVMaster->SendRTPData(m_pEncodedBuffer, m_nPacketBytes*m_nEncodedCount, 0);		// Audio 			
		}
		ZeroMemory(m_pEncodedBuffer, SIZE_ENCODED_BUFFER);
		m_nEncodedCount = 0;				
	}
	return 1L;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayStart(WPARAM wParam, LPARAM lParam)
// - Play Starting
////////////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::PlayStart(void)
{
	if(m_bFlagPlaying || p_nAvailableCount <= 0){	return FALSE;}

	// open wavein device
	MMRESULT mmReturn = ::waveOutOpen( &m_hPlay, p_nDeviceIndex, &m_WaveFormatEx, this->m_nThreadID, 0, CALLBACK_THREAD);		
	if(mmReturn )
	{
		for(int i=0; i< p_nAvailableCount; i++)
		{
			mmReturn = ::waveOutOpen( &m_hPlay, i, &m_WaveFormatEx, this->m_nThreadID, 0, CALLBACK_THREAD);
			if(!mmReturn)
			{					
				break;
			}
		}
		
		if(mmReturn)
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveOutGetErrorText( mmReturn, errorbuffer, MAX_PATH);
			sprintf(errorbuffer1,"WAVEOUT:%x:%s",mmReturn,errorbuffer);
			
			char	szError[1024];
			sprintf(szError, "[ERROR] CAudioSound::PlayStart waveOutOpen error, AvailableCounts[%d] %s\r\n", p_nAvailableCount, errorbuffer1);
			DBGOUT(szError);
//			CSaveLastError::SaveLastError(szError);
		}
		
	}		
				
	if(!mmReturn)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 스피커 볼륨 전달
		BYTE byVol = 0;
		if(m_pAVMaster
			&& this->GetPlayVolume(&byVol))
		{
			m_pAVMaster->OnVolumeW(TRUE, byVol);
		}

		m_bFlagPlaying = TRUE;
	}		
	
	
	return m_bFlagPlaying;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayStop(WPARAM wParam, LPARAM lParam)
// - Play Stop
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayStop(WPARAM wParam, LPARAM lParam)
{	
	if(!m_bFlagPlaying)
		return FALSE;	

	m_bFlagPlaying = FALSE;
	MMRESULT mmReturn = ::waveOutReset(m_hPlay);	
	if(!mmReturn)
	{
		mmReturn = ::waveOutClose(m_hPlay);
	}	
	m_hPlay = NULL;
	m_dwPlayingBytes = 0;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam)
// - Play Data Receive and write
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR)lParam;
	if(NULL == lpHdr || FALSE == m_bFlagPlaying || NULL == m_hPlay)		// 2008-06-12 by cmkwon, 체크 추가 - NULL == m_hPlay
	{
		SAFE_DELETE(lpHdr);
		return 0L;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	if(INVALID_SPK_VOLUME_VALUE != m_nPlayVolumeOfPercent)
	{
		this->SetPlayVolume(m_nPlayVolumeOfPercent, TRUE);
		m_nPlayVolumeOfPercent		= INVALID_SPK_VOLUME_VALUE;
	}


	DWORD dwBytes = lpHdr->dwBufferLength;	
	MMRESULT mmResult = ::waveOutPrepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
	if(mmResult)
	{
		int nErr = GetLastError();
		SetLastError(0);

		char	szError[1024];
		sprintf(szError, "[ERROR] CAudioSound::OnPlayWriteSoundData waveOutPrepareHeader error, ErrorCode[%d]\r\n", nErr);
		DbgOut(szError);
//		CSaveLastError::SaveLastError(szError);
		return 0L;
	}			
	
	mmResult = ::waveOutWrite(m_hPlay, lpHdr, sizeof(WAVEHDR));
	if(mmResult)
	{
		int nErr = GetLastError();
		SetLastError(0);

		char	szError[1024];
		sprintf(szError, "[ERROR] CAudioSound::OnPlayWriteSoundData waveOutWrite error, ErrorCode[%d]\r\n", nErr);
		DbgOut(szError);
//		CSaveLastError::SaveLastError(szError);
		return 0L;
	}
	
	m_dwPlayingBytes += dwBytes;
	return 1L;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayEndSoundData(WPARAM wParam, LPARAM lParam)
// - Play가 끝난 헤더
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayEndSoundData(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR)lParam;
	if(lpHdr)
	{
		::waveOutUnprepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));		
		SAFE_DELETE(lpHdr);	
	}
	return ERROR_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayReset(WPARAM wParam, LPARAM lParam)
// - waveOut device를 reset한다.
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayReset(WPARAM wParam, LPARAM lParam)
{
	if(m_bFlagPlaying)
	{
		::waveOutReset(m_hPlay);		
	}
	m_dwPlayingBytes = 0;
	return m_bFlagPlaying;
}


/////////////////////////////////////////////////////////////////////////////
// CMixSound message handlers

void CALLBACK TimerProcMix(UINT uiID,UINT uiMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	CAudioSound* pSound = (CAudioSound*) dwUser;
	pSound->PostThreadMessage(WM_MIX_PLAY, NULL, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// LRESULT	CAudioSound::OnMixStart(WPARAM wParam, LPARAM lParam)
//
// - CMixSound 멀티미디어 타이머를 시작한다.
// - wParam 은 필요 없음
// - lParam 는 CPlaySound 스레드의 포인터
// - return 값은 필요 없음
/////////////////////////////////////////////////////////////////////////////
BOOL CAudioSound::MixTimerStart(void)
{
	if(0 != b_nTimer_id){		return FALSE;}

	b_nTimer_id = timeSetEvent(MULTIMEDIA_TIMER_TIME,1, &TimerProcMix, (DWORD)this, TIME_PERIODIC);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// LRESULT	CAudioSound::OnMixStop(WPARAM wParam, LPARAM lParam)
//
// - CMixSound 멀티미디어 타이머를 멈춘다.
// - wParam 은 필요 없음
// - lParam 는 필요 없음
// - return 값은 필요 없음
/////////////////////////////////////////////////////////////////////////////
void CAudioSound::MixTimerStop(void)
{
	if(0 == b_nTimer_id)
	{
		return;
	}
	
	timeKillEvent(b_nTimer_id);		
	b_nTimer_id = 0;
}

LRESULT	CAudioSound::OnMixPlay(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == m_bFlagPlaying || NULL == m_pAVMaster){		return 0L;}

	DWORD dwCurTick = timeGetTime();
	DWORD dwTickGap = dwCurTick - this->m_dwLastTick;

	MMTIME pmmt;
	pmmt.wType = TIME_BYTES;
	MMRESULT mmReturn = waveOutGetPosition(m_hPlay, &pmmt, sizeof(MMTIME));
	DWORD posGap = m_dwPlayingBytes - pmmt.u.cb;

	//DBGOUT("Offset[%6d] Bytes[%6d] Gap[%4d] TickGap[%4d] Count[%4d]\n", 
	//	pmmt.u.cb, m_dwPlayingBytes, posGap, dwTickGap, m_pAVMaster->GetDataCounts(0));
	if(mmReturn 
		|| posGap > m_nFrameBytes*b_nBufferPlay_Reset_Count)
	{
		return 0L;
	}
	else if(posGap > m_nFrameBytes*(b_nBufferPlay_Start_Count-1) 
		&& dwTickGap < 30)
	{
		return 0L;
	}
	this->m_dwLastTick = dwCurTick;
	
	SJittData*				lp[SIZE_MAX_CHANNEL_COUNT-1];
	int						arrIndex[SIZE_MAX_CHANNEL_COUNT-1];
	int						nMixingChannelCounts = 0;

	memset(lp, 0x00, sizeof(lp[0])*(SIZE_MAX_CHANNEL_COUNT-1));
	int i=0;
	while(i < m_pAVMaster->GetChannelCounts())
	{
		if(m_pAVMaster->GetPlayStartFlags(i) == TRUE)
		{			
			lp[i] = m_pAVMaster->GetDataFromBuffers(i);
			if(lp[i])
			{
				arrIndex[nMixingChannelCounts] = i;
				nMixingChannelCounts++;
			}
			else if(posGap < 100)
			{
				m_pAVMaster->SetPlayStartFlags(i, FALSE);
			}
		}
		i++;
	}	
		
	if(nMixingChannelCounts <= 0)
	{
		return 0L;
	}
	else if(nMixingChannelCounts > 10)
	{
		char	szError[1024];
		sprintf(szError, "[ERROR] CAudioSound::OnMixPlay index error, MixingChannelCounts[%d]\r\n", nMixingChannelCounts);
		DbgOut(szError);
//		CSaveLastError::SaveLastError(szError);
		return 0L;
	}
	
	int			nSize = lp[arrIndex[0]]->dataSize;
	LPWAVEHDR	lpHdr = CreateWaveHeader(nSize);	
	switch(nMixingChannelCounts)
	{
	case 1:
		memcpy(lpHdr->lpData, lp[arrIndex[0]]->lpData , lp[arrIndex[0]]->dataSize);
		break;
	case 2:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 3:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize/2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lpHdr->lpData, (int)nSize/2);
		break;
	case 4:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 5:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize/2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize/2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize/2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize/2);
		break;
	case 6:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 7:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 8:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 9:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 10:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 11:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 12:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[11]]->lpData, (short*)lp[arrIndex[10]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 13:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[11]]->lpData, (short*)lp[arrIndex[10]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 14:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[11]]->lpData, (short*)lp[arrIndex[10]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[13]]->lpData, (short*)lp[arrIndex[12]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 15:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[11]]->lpData, (short*)lp[arrIndex[10]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[13]]->lpData, (short*)lp[arrIndex[12]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[14]]->lpData, (short*)lp[arrIndex[12]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;
	case 16:
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[7]]->lpData, (short*)lp[arrIndex[6]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[9]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[11]]->lpData, (short*)lp[arrIndex[10]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[13]]->lpData, (short*)lp[arrIndex[12]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[14]]->lpData, (short*)lp[arrIndex[15]]->lpData, (short*)lp[arrIndex[14]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
		MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[10]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[14]]->lpData, (short*)lp[arrIndex[12]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
		MixingOperation((short*)lp[arrIndex[8]]->lpData, (short*)lp[arrIndex[12]]->lpData, (short*)lp[arrIndex[8]]->lpData, (int)nSize /2);
		
		MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[8]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		break;	
	}	
	for(i = 0 ; i < nMixingChannelCounts; i++)
	{
		SAFE_DELETE(lp[arrIndex[i]]);
	}
	
	if(lpHdr)
	{		
		OnPlayWriteSoundData(GetCurrentThreadId(), (LPARAM)lpHdr);
	}
	return TRUE;
}

LRESULT	CAudioSound::OnMixEndRTPSession(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
BOOL CAudioSound::WriteSystemLogEX(BOOL bPrintDBGOUT, const char* pFormat, ...)
{
	char szLogMsg[1024];
	va_list args;
	va_start(args, pFormat);
	vsprintf(szLogMsg, pFormat, args);
	
	if (bPrintDBGOUT)
	{
		DBGOUT(szLogMsg);
	}
	
	return ms_SystemLogManager.WriteSystemLog(szLogMsg, TRUE);
}

