// audiosound.cpp : implementation file
//

#include "stdafx.h"
#include "audiosound.h"
#include "G7231Codec.h"
#include "H323Connection.h"		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef _ECHO_CANCELLER
/////////////////////////////////////////////////////////////////////////////////////
// Echo Cancellation
/* Constants */
#define	ON		1
#define	OFF		0
#define	FRAME		240		// 1 frame of speech data 

/* Flag */
extern "C" short UseLec;						// 에코 캔슬러 시작 플래그
extern "C" short Offset;							// 에코 캔슬러의 Offset 설정 변수
extern "C" void initlec();						// 에코 캔슬러 모듈 초기화
extern "C" void lec(short*, short*, short*);	// 에코 캔슬러 처리 함수
#endif // _ECHO_CANCELLER


#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */
#define	BIAS		(0x84)		/* Bias for linear code. */
short CAudioSound::seg_end[] = {0xFF, 0x1FF, 0x3FF, 0x7FF,
			    0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
CSystemLogManager CAudioSound::ms_SystemLogManager;		// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 


VOID SerialReadThreadFunction(LPVOID lpParam);
/////////////////////////////////////////////////////////////////////////////
// CAudioSound
IMPLEMENT_DYNCREATE(CAudioSound, CWinThread)

CAudioSound::CAudioSound():m_RxQueue(2048), m_TxQueue(2048)
{
	//AfxMessageBox("CAudioSound Error");
}

// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 인자추가(, CH323Connection *i_p323Conn)
CAudioSound::CAudioSound(int nFrameCount, int JitterBufferSize, HWND hCtrl, BOOL bSerial, int nSerialPort, int nTermperSecond, CH323Connection *i_p323Conn)
:m_RxQueue(4096), m_TxQueue(2048)
{
	// Get the current OS information.
	memset(&m_osVersion, 0x00, sizeof(OSVERSIONINFO));
	m_osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&m_osVersion);
	
	m_hSerialReadThread		= NULL;
	m_IDSerialReadThread	= 0;
	m_bSerialReadThreadEnd	= FALSE;

	
	b_nPlayG7231Value		= -1;
	m_hCtrl					= hCtrl;
	m_dwSerialDeviceState	= bSerial;
	m_nSerialPort			= nSerialPort;
	m_hSerialPort			= INVALID_HANDLE_VALUE;	
	m_bSerialWriteWaitThreadEnd = FALSE;
	r_pRTPCtrlManager		= NULL;
	m_bSignalContinue		= FALSE;
	
	m_bSerialTimeOutTone	= FALSE;

	m_bSerialSendDummyPacket = FALSE;
	m_dwLastTimeSendDummyPakcet = 0;


	m_dwSerialDialtoneState	= SDIAL_NO_PLAY;
	m_bSerialDialtoneReady	= FALSE;	
	m_nSerialDialtoneCount	= 0;
	m_nSerialDialtoneSize	= 0;

	m_bSerialBusytoneReady	= FALSE;		
	m_nSerialBusytoneCount	= 0;
	m_nSerialBusytoneSize	= 0;
	
	ZeroMemory(&m_ovRead, sizeof(OVERLAPPED));
	ZeroMemory(&m_ovWrite, sizeof(OVERLAPPED));

	m_dwSerialDeviceGarbageCount = 0;		
	
	/*
	// cmkwon 20020517 에코 샘플 만들기
	m_pfRef = NULL;
	m_pfEcho = NULL;
	m_pfOut = NULL;
	m_nEchoCount = 0;

	m_pfRef = fopen("C:\\Ref.raw", "wb");
	m_pfEcho = fopen("C:\\Echo.raw", "wb");
	m_pfOut = fopen("C:\\Out.raw", "wb");
	//*/
#ifdef _ECHO_CANCELLER
	ZeroMemory(m_pEchoCancelBuffer, sizeof(m_pEchoCancelBuffer));
	m_bStartEchoCancellerFlag		= FALSE;
	m_bApplyEchoCancellerFlag		= FALSE;
	InitializeCriticalSection(&m_csEchoCanceller);
#endif

	m_bMMTimer				= TRUE;
	
	///////////////////////////////////////////////////////////////////////////////////
	// Recording 관련 변수 초기화
	r_hRecord				= NULL;
	r_bRecording			= FALSE;					// Recording 시작 플래그
	r_pEncoder				= NULL;						// Audio Encoder 	
	r_nEncodedCount			= 0;
	r_nSendEncodedCount		= 0;
	r_nDeviceIndex			= WAVE_MAPPER;				// waveOut device index
	r_bSendMuteAudioRTP		= FALSE;					// 무음을 보내는 flag
	r_bDTMFSendFlag			= FALSE;					// Inbound DTMF 플래그
	r_nDTMFSendCount		= 0;						// Inbound DTMF 전송시 전송 패캣 count	
	
	r_AudioPayloadType = GetAudioPayloadType();	
	//Audio Codec에 따른 설정의 차이( G.7231, G.729 )
	if(r_AudioPayloadType == t_G7231 )		// 8000 kHz,  16 bitpersample,  Mono
	{
		m_nPacketByte = 24; m_nFrameByte = 480;
		r_nInBufferSize = m_nFrameByte;
		r_nSendEncodedCount = nFrameCount;
		r_nMaxInputBuffers = 50/nFrameCount;		
	}
	else if( r_AudioPayloadType == t_G7231Low)
	{
		m_nPacketByte = 20; m_nFrameByte = 480;
		r_nInBufferSize = m_nFrameByte;
		r_nSendEncodedCount = nFrameCount;		
		r_nMaxInputBuffers = 50/nFrameCount;
	}
	else
	{
		//AfxMessageBox("Audio Type Error");
	}
	
	memset(&r_WaveFormatEx,0x00,sizeof(r_WaveFormatEx));
	r_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	r_WaveFormatEx.nChannels = 1;
	r_WaveFormatEx.wBitsPerSample = 16;	
	r_WaveFormatEx.nSamplesPerSec = 8000;
	r_WaveFormatEx.nAvgBytesPerSec = r_WaveFormatEx.nSamplesPerSec*(r_WaveFormatEx.wBitsPerSample/8);
	r_WaveFormatEx.nBlockAlign = (r_WaveFormatEx.wBitsPerSample/8)*	r_WaveFormatEx.nChannels;
	r_WaveFormatEx.cbSize = 0;
		
#ifdef _G7231CODEC								
	r_pEncoder = new CG7231Codec(1);			// Encoder Create
#endif

	if( !r_pEncoder || !r_pEncoder->AudioCodecLoadLibrary())
	{
		// 2007-07-24 by cmkwon, 필요없음
		//AfxMessageBox("Error : Audio Codec Initialize Error!");		
	}

	r_nAvailableCount = waveInGetNumDevs();			// waveIn driver cap
	if( r_nAvailableCount <= 0 && !m_dwSerialDeviceState)
	{
		//AfxMessageBox("Your computer hasn't sound In device(etc, Mic)"); 
	}

	///////////////////////////////////////////////////////////////////////////////////
	// Playing 관련 변수 초기화
	p_hPlay					= NULL;
	p_bPlay					= FALSE;	
	p_nDeviceIndex			= WAVE_MAPPER;				// waveIn device index
	p_nWriteCount			= 0;						// 플레이한 프레임 카운트
	p_dwWriteNotSkipCount	= 0;						//
	
	memset(&p_WaveFormatEx,0x00,sizeof(p_WaveFormatEx));
	p_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	p_WaveFormatEx.nChannels = 1;
	p_WaveFormatEx.wBitsPerSample = 16;	
	p_WaveFormatEx.nSamplesPerSec = 8000;
	p_WaveFormatEx.nAvgBytesPerSec = p_WaveFormatEx.nSamplesPerSec*(p_WaveFormatEx.wBitsPerSample/8);
	p_WaveFormatEx.nBlockAlign = (p_WaveFormatEx.wBitsPerSample/8)*p_WaveFormatEx.nChannels;
	p_WaveFormatEx.cbSize = 0;	
		
	p_nAvailableCount = waveOutGetNumDevs();
	if( p_nAvailableCount <= 0 && !m_dwSerialDeviceState)
	{
		//AfxMessageBox("Your computer hasn't sound out device(etc, Speaker)"); 
	}

	///////////////////////////////////////////////////////////////////////////////////
	// Buffering 관련 변수 초기화
	b_bSetTimerFlag				= FALSE;
//	b_nPlayCount				= 0;
	b_nTimer_id					= 0;
	b_nBufferPlay_Start_Count	= JitterBufferSize;
	b_nBufferPlay_Reset_Count	= b_nBufferPlay_Start_Count * 2 -1;

	for(int i=0; i< CRTPCTRLARRAYSIZE; i++)
	{
		b_pPlayBufferArray[i]	= NULL;
		b_arrDecoder[i]			= NULL;
		b_bPlayStartFlag[i]		= FALSE;
	}
	
	for(i = 0; i< CRTPCTRLARRAYSIZE; i++)
	{		
		b_pPlayBufferArray[i]	= new CDataQueue;				
#ifdef _G7231CODEC								// MixSound.cpp
		b_arrDecoder[i]			= new CG7231Codec(2);
#endif
#ifdef _G729CODEC								// MixSound.cpp
		b_arrDecoder[i]			= new CG729Codec(2);
#endif
		if(!b_arrDecoder[i] || !b_arrDecoder[i]->AudioCodecLoadLibrary())
		{
			//AfxMessageBox("Audio ElemediaCodec Decoder Initialize Error!");
			break;
		}
	}

	///////////////////////////////////////////////////////////////////
	// Ring Back tone
	b_bPlayG7231Ready	= FALSE;
	b_bPlayG7231Looped	= FALSE;
	b_nPlayG7231Count	= 0;
	b_nPlayG7231Size	= 0;	

	b_pPlayG7231Decoder = NULL;
#ifdef _G7231CODEC								// MixSound.cpp
	b_pPlayG7231Decoder = new CG7231Codec(2);
	if(!b_pPlayG7231Decoder || !b_pPlayG7231Decoder->AudioCodecLoadLibrary())
	{
		//AfxMessageBox("Audio ElemediaCodec Decoder Initialize Error!");		
	}
#endif
	memset(b_PlayG7231Buffer, 0x00, LOCALPLAYG7231_BUFFERSIZE);
	////////////////////////////////////////////////////////////////////////

	if(m_dwSerialDeviceState)
	{
		InitializeSerialDevice();
	}
	m_nKeepAliveHookOn = 50;
	m_nKeepAliveHookOff = 51;
	m_bKeepAliveKey = FALSE;
	m_pRawFile = NULL;
	m_bLogWriteFile = FALSE;

	m_bSerialTestMode = FALSE;
	m_lRxSerialData = m_lTxSerialData = m_lRxRTPData = m_lTxRTPData = 0;

	m_nPlayVolumeOfPercent	= INVALID_SPK_VOLUME_VALUE;		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 

	m_nSilenceFrameCnt		= 0;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	m_nThresholdLevel		= 200;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	m_bSilenceState			= FALSE;	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	m_nSkipFrameCnt			= 0;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	///////////////////////////////////////////////////////////////////////////////
//	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	ms_SystemLogManager.InitLogManger(TRUE, "SCDialer", ".\\Res-Voip\\");

	m_pH323Conn				= i_p323Conn;		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
}

CAudioSound::~CAudioSound()
{
	TRACE("\nCAudioSound::~CAudioSound()\n");
	if(m_hSerialReadThread)
	{
		m_bSerialReadThreadEnd = TRUE;
		WaitForSingleObject(m_hSerialReadThread, INFINITE);
		CloseHandle(m_hSerialReadThread);
		m_hSerialReadThread = NULL;
	}

	if(m_dwSerialDeviceState && m_dwSerialDeviceState != SDEVICE_ERROR)
	{
		DeleteCriticalSection(&m_SerialCritical);
	}

	if(m_hSerialPort != INVALID_HANDLE_VALUE)
	{		
		SetCommMask(m_hSerialPort, 0);
		if (!PurgeComm(m_hSerialPort, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR)){ TRACE("PurgeComm");}
		
		CloseHandle(m_hSerialPort);
		m_hSerialPort = INVALID_HANDLE_VALUE;
	}

	if(m_ovRead.hEvent != NULL)
	{
		CloseHandle(m_ovRead.hEvent);
	}
	if(m_ovWrite.hEvent != NULL)
	{
		CloseHandle(m_ovWrite.hEvent);
	}
		
	/*
	// cmkwon 20020517 에코 샘플 만들기
	if(m_pfRef){ fclose(m_pfRef);}
	if(m_pfEcho){ fclose(m_pfEcho);}
	if(m_pfOut){ fclose(m_pfOut);}
	//*/

	if(r_pEncoder)
	{
		delete r_pEncoder;
		r_pEncoder = NULL;
	}

	for(int i=0; i< CRTPCTRLARRAYSIZE; i++)
	{		
		if(b_arrDecoder[i])
		{
			delete b_arrDecoder[i];
			b_arrDecoder[i] = NULL;
		}
		if(b_pPlayBufferArray[i])
		{
			b_pPlayBufferArray[i]->InitializeVariable ();
			delete b_pPlayBufferArray[i];
			b_pPlayBufferArray[i] = NULL;		
		}
	}

	if(b_pPlayG7231Decoder)
	{
		delete b_pPlayG7231Decoder;
		b_pPlayG7231Decoder = NULL;
	}
	m_pRawFile = NULL;

#ifdef _ECHO_CANCELLER
	DeleteCriticalSection(&m_csEchoCanceller);
#endif
}

BOOL CAudioSound::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	//SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	DWORD dwPro = GetThreadPriority();
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
	ON_THREAD_MESSAGE(WM_RECORD_START, OnRecordStart)
	ON_THREAD_MESSAGE(WM_RECORD_STOP, OnRecordStop)
	ON_THREAD_MESSAGE(MM_WIM_DATA, OnRecordSoundData)	
	ON_THREAD_MESSAGE(WM_RECORD_SENDDTMF, OnRecordSendDTMF)
	ON_THREAD_MESSAGE(WM_RECORD_SENDMUTEAUDIORTP, OnRecordSetMuteAudioRTP)	

	// Playing
	ON_THREAD_MESSAGE(WM_PLAY_START, OnPlayStart)
	ON_THREAD_MESSAGE(WM_PLAY_STOP, OnPlayStop)
	ON_THREAD_MESSAGE(WM_PLAY_PLAYBLOCK, OnPlayWriteSoundData)
	ON_THREAD_MESSAGE(MM_WOM_DONE, OnPlayEndSoundData)	
	ON_THREAD_MESSAGE(WM_PLAY_RESET, OnPlayReset)
	
	// buffering
	ON_THREAD_MESSAGE(WM_MIX_START, OnMixStart)
	ON_THREAD_MESSAGE(WM_MIX_STOP, OnMixStop)
	ON_THREAD_MESSAGE(WM_MIX_FORMRTPTOBUFFER, OnMixFromRTPtoBuffer)
	ON_THREAD_MESSAGE(WM_MIX_PLAY, OnMixPlay)	
	ON_THREAD_MESSAGE(WM_MIX_ENDRTPSESSION, OnMixEndRTPSession)
	ON_THREAD_MESSAGE(WM_MIX_LOCALPLAYG7231, OnMixLocalPlayG7231)

	// Serial
	ON_THREAD_MESSAGE(WM_SERIAL_READ, OnSerialRead)
	ON_THREAD_MESSAGE(WM_SERIAL_WRITE, OnSerialWrite)
	ON_THREAD_MESSAGE(WM_SERIAL_EVENT, OnSerialEvent)
	ON_THREAD_MESSAGE(WM_SERIAL_SIGNAL, OnSerialSignal)
	ON_THREAD_MESSAGE(WM_SERIAL_MESSAGE, OnSerialMessage)

	// log
	ON_THREAD_MESSAGE(WM_RTPDATA_READ, OnLogRTPDataRead)
	ON_THREAD_MESSAGE(WM_RTPDATA_WRITE, OnLogRTPDataWrite)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAudioSound message handlers
LRESULT CAudioSound::OnAudioEndThread(WPARAM wParam, LPARAM lParam)
{	
	if(r_bRecording)
	{
		OnRecordStop(0, 0);
	}

	if(r_pEncoder)
	{
		delete r_pEncoder;
		r_pEncoder = NULL;
	}

	if(p_bPlay)
	{
		OnPlayStop(0,0);
	}

	if(b_bSetTimerFlag == TRUE && b_nTimer_id)
	{
		timeKillEvent(b_nTimer_id);
		b_bSetTimerFlag = FALSE;
		b_nTimer_id = 0;
	}
	TRACE("\nCAudioSound::OnAudioEndThread()\n");
	
	::PostQuitMessage(0);
	return TRUE;
}

LPWAVEHDR CAudioSound::CreateWaveHeader(int size)
{
	LPWAVEHDR lpHdr = new WAVEHDR;
	ZeroMemory(lpHdr, sizeof(WAVEHDR));
	
	BYTE* lpByte = new BYTE[size];
	lpHdr->lpData = (char *) lpByte;
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
				TRACE("\n		SetRecordSoundDevice successed(%s),  Total Device[%d],  Set Device Index[%d]", 
					strDevice, r_nAvailableCount, r_nDeviceIndex);
/*				CString str;
				str.Format("SetRecordSoundDevice successed(%s),  Total Device[%d],  Set Device Index[%d]", 
					strDevice, r_nAvailableCount, r_nDeviceIndex);
				AfxMessageBox(str);
*/				return TRUE;	
			}
		}
		else
		{
			break;
		}
	}

	TRACE("\n		SetRecordSoundDevice failed(%s),  Total Device[%d],  Set Device Index[%d]", 
		strDevice, r_nAvailableCount, r_nDeviceIndex);
/*	CString str;
	str.Format("SetRecordSoundDevice failed(%s),  Total Device[%d],  Set Device Index[%d]", 
		strDevice, r_nAvailableCount, r_nDeviceIndex);
	AfxMessageBox(str);
*/
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
/*				CString str;
				str.Format("SetPlaySoundDevice successed(%s),  Total Device[%d],  Set Device Index[%d]", 
					strDevice, p_nAvailableCount, p_nDeviceIndex);
				AfxMessageBox(str);
*/
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
/*	CString str;
	str.Format("SetPlaySoundDevice failed(%s),  Total Device[%d],  Set Device Index[%d]", 
		strDevice, p_nAvailableCount, p_nDeviceIndex);
	AfxMessageBox(str);
*/
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CRecordSound::OnRecordStart(WPARAM wParam, LPARAM lParam)
// - Recording 준비
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordStart(WPARAM wParam, LPARAM lParam)
{
	r_pRTPCtrlManager = (CRTPCtrlManager*)lParam;
	if(m_dwSerialDeviceState)
	{
		if(m_hSerialPort != INVALID_HANDLE_VALUE)
		{
			m_bSerialSendDummyPacket = TRUE;
			r_bRecording = TRUE;
			return TRUE;
		}
		
		return FALSE;
	}
	
	MMRESULT mmReturn = 0;	
	
	if(r_bRecording)
		return FALSE;

	TRACE("\n		CRecordSound StartRecording Start Device Index[%d], tick[%ld]", r_nDeviceIndex, GetTickCount());
	if(!r_hRecord && r_nAvailableCount > 0)
	{	
		// open wavein device
		Sleep(500);
		TRACE("\n[OnRecordStart] waveInOpen Begin\n");
		mmReturn = ::waveInOpen( &r_hRecord, r_nDeviceIndex, &r_WaveFormatEx, 
			::GetCurrentThreadId(), 0, CALLBACK_THREAD);
		TRACE("\n[OnRecordStart] waveInOpen End\n");

		if(mmReturn)
		{	
			TRACE("\n[OnRecordStart] MMRESULT = %d, GetLastError() = %d\n", mmReturn, GetLastError());
			for(int i=0; i< r_nAvailableCount; i++)
			{
				Sleep(500);
				TRACE("\n[OnRecordStart] [retry] waveInOpen Begin (%d)\n", i);
				mmReturn = ::waveInOpen(&r_hRecord, i, &r_WaveFormatEx, 
					::GetCurrentThreadId(), 0, CALLBACK_THREAD);
				TRACE("\n[OnRecordStart] [retry] waveInOpen End (%d)\n", i);
				if(!mmReturn)
				{
//					DbgOut("\n[SCDialer1] OnStartRecording [retry]: device index = %d", i);
/*					CString str;
					str.Format("OnStartRecording [retry]: device index = %d", i);
					AfxMessageBox(str);
*/					break;
				}
				else
					TRACE("\n[OnRecordStart] [retry] MMRESULT = %d, GetLastError() = %d\n", mmReturn, GetLastError());

			}
			
			if(mmReturn)
			{
				char errorbuffer[MAX_PATH];
				char errorbuffer1[MAX_PATH];
				waveInGetErrorText( mmReturn, errorbuffer, MAX_PATH);
				sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);
//				DbgOut("\n[OnRecordStart] [Error] %s\n", errorbuffer1); 
			}			
		}
		else
		{
//				DbgOut("\n[SCDialer1] OnStartRecording : device index = %d", r_nDeviceIndex);
/*				CString str;
				str.Format("OnStartRecording : device index = %d", r_nDeviceIndex);
				AfxMessageBox(str);
*/		}
	}
	if(r_hRecord && !mmReturn)
	{			
		for(int i=0; i < r_nMaxInputBuffers; i++)
		{
			LPWAVEHDR lpHdr = CreateWaveHeader(m_nFrameByte);
			mmReturn = ::waveInPrepareHeader(r_hRecord,lpHdr, sizeof(WAVEHDR));
			mmReturn = ::waveInAddBuffer(r_hRecord, lpHdr, sizeof(WAVEHDR));
		}
		
		// Recording Start
		mmReturn = ::waveInStart(r_hRecord);
		if(mmReturn)
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveInGetErrorText( mmReturn, errorbuffer, MAX_PATH);
			sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);				
			//AfxMessageBox(errorbuffer1); 				
		}
		else
		{
			r_bRecording = TRUE;
			TRACE("\n		CRecordSound StartRecording End, tick[%ld]", GetTickCount());
			
			return ERROR_SUCCESS;
		}		
	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnRecordStop(WPARAM wParam, LPARAM lParam)
// - Recording stop
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordStop(WPARAM wParam, LPARAM lParam)
{
	
	MMRESULT mmReturn = 0;
	
	if(!r_bRecording){ return FALSE;}

	if(m_dwSerialDeviceState)
	{
		r_bRecording	= FALSE;
		r_nEncodedCount	= 0;
	}
	
	TRACE("\n		CRecordSound StopRecording");
	r_bRecording	= FALSE;
	mmReturn = ::waveInStop(r_hRecord);
	if(!mmReturn)
	{
		mmReturn = ::waveInReset(r_hRecord);
	}	
	if(!mmReturn)
	{
		mmReturn = ::waveInClose(r_hRecord);
	}	
	r_hRecord			= NULL;	
	r_bDTMFSendFlag		= FALSE;
	r_bSendMuteAudioRTP	= FALSE;
	r_nEncodedCount		= 0;

	m_bMMTimer = TRUE;

	return mmReturn;	
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnRecordSoundData(WPARAM wParam, LPARAM lParam)
// - 음성 데이타가 버퍼에 다 차면 호출되는 콜백 핸들러
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordSoundData(WPARAM wParam, LPARAM lParam)
{	
	if(m_bMMTimer){ m_bMMTimer = FALSE;}
	if(b_bPlayG7231Ready){ OnMixLocalPlayG7231(FALSE, FALSE);}

	//this->PostThreadMessage(WM_MIX_PLAY, 0, 0);

	LPWAVEHDR	lpHdr = (LPWAVEHDR)lParam;
	short		OutBuffer[240];
	if(lpHdr)
	{		
		short int * lpInt = (short int*) lpHdr->lpData;		
		::waveInUnprepareHeader(r_hRecord, lpHdr, sizeof(WAVEHDR));			
		
		if(r_bRecording && r_pRTPCtrlManager)
		{			
			// r_bDTMFSendFlag 가 TRUE 이면 음성 data 대신 (음성 빼버림) DTMF RTP 전송
			if(r_bDTMFSendFlag && (r_AudioPayloadType == t_G7231 || r_AudioPayloadType == t_G7231Low))
			{
				if( r_nDTMFSendCount + r_nSendEncodedCount >= 5)
				{
					memcpy(r_pEncodedBuffer, &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte*(5 - r_nDTMFSendCount));					
					r_nDTMFSendCount = 0;
					r_bDTMFSendFlag = FALSE;					
				}
				else
				{
					memcpy(r_pEncodedBuffer, &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte*r_nSendEncodedCount);
					r_nDTMFSendCount += r_nSendEncodedCount;					
				}
				r_nEncodedCount = r_nSendEncodedCount;
			}
			else if(r_bSendMuteAudioRTP == TRUE){}
			else 
			{
				CopyMemory(OutBuffer, lpInt, lpHdr->dwBufferLength);
#ifdef _ECHO_CANCELLER
				if(m_bStartEchoCancellerFlag)
				{
					LockEchoCanceller();
					lec(m_pEchoCancelBuffer, lpInt, OutBuffer);					
					
					/*
					// cmkwon 20020517 에코 샘플 만들기
					if(m_pfRef)
					{
						fwrite(m_pEchoCancelBuffer, 1, 480, m_pfRef);					
					}					
					if(m_pfEcho)
					{
						fwrite(lpInt, 1, 480, m_pfEcho);						
					}
					if(m_pfOut)
					{
						fwrite(OutBuffer, 1, 480, m_pfOut);						
					}
					//*/
					UnlockEchoCanceller();
				}
#endif // _ECHO_CANCELLER
				if(r_pEncoder)
				{						

					///////////////////////////////////////////////////////////////////////////////
					// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
					int nAverageSignalLevel	= GetAverageSignalLevel(OutBuffer, lpHdr->dwBytesRecorded/sizeof(short int));
					m_nSilenceFrameCnt		= (m_nThresholdLevel > nAverageSignalLevel) ? m_nSilenceFrameCnt+1 : 0;

					if(0 != r_nEncodedCount
						|| m_nSilenceFrameCnt < 33)
					{
						///////////////////////////////////////////////////////////////////////////////
						// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
						if(m_bSilenceState)
						{
//							this->WriteSystemLogEX(FALSE, "말하기 상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//							DbgOut("말하기 상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);
						}
						else
						{
//							this->WriteSystemLogEX(FALSE, "	말하기 상태 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
						}

						m_bSilenceState		= FALSE;
						r_pEncoder->AudioCodecEncoding(OutBuffer, &r_pEncodedBuffer[r_nEncodedCount*m_nPacketByte]);
						r_nEncodedCount++;
					}
					else
					{
						///////////////////////////////////////////////////////////////////////////////						
						// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
						if(FALSE == m_bSilenceState)
						{
//							this->WriteSystemLogEX(FALSE, "무음   상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//							DbgOut("무음   상태로 변경 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);
						}
						else
						{
//							this->WriteSystemLogEX(FALSE, "	무음   상태 : AverageSignalLevel[%4d] SilenceFrameCnt[%6d], EncodedCount[%3d] SkipFrameCnt[%4d]\r\n", nAverageSignalLevel, m_nSilenceFrameCnt, r_nEncodedCount, m_nSkipFrameCnt);	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
						}

						m_bSilenceState		= TRUE;
						m_nSkipFrameCnt++;		// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 

					}
				}
			}
			
			if(r_nEncodedCount >= r_nSendEncodedCount)
			{
				//TRACE("\n Send RTP %d, time[%d]", r_nEncodedCount, GetTickCount());
				/////////////////////////////////////////////////////////////////////////////
				// RTP channel 로 인코딩된 데이타를 보낸다.
				r_pRTPCtrlManager->SendRTPData(r_pEncodedBuffer, m_nPacketByte*r_nEncodedCount, t_Audio);		// Audio 			
				ZeroMemory(r_pEncodedBuffer, sizeof(r_pEncodedBuffer));
				r_nEncodedCount = 0;				
			}
		}
			
		if(r_bRecording)
		{			
			::waveInPrepareHeader(r_hRecord,lpHdr, sizeof(WAVEHDR));
			::waveInAddBuffer(r_hRecord, lpHdr, sizeof(WAVEHDR));
		}
		else
		{		
			if(lpHdr->lpData)
			{
				delete[] lpHdr->lpData;
			}
			if(lpHdr)
			{
				delete lpHdr;		
			}			
		}
	}
	
	return ERROR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnRecordSendDTMF(WPARAM wParam, LPARAM lParam)
// - Inbound DTMF를 보내기 위해 DTMF 데이타를 세팅한다.
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnRecordSendDTMF(WPARAM wParam, LPARAM lParam)
{
	HRSRC	hRsrc = NULL;
	HGLOBAL hGResource = NULL;
	LPVOID	lpFile = NULL;
	
	char	dtmf[40] = "IDR_G7231_";
	char	tmp[10];
	
	if(r_AudioPayloadType == t_G7231Low)
	{
		strcpy(dtmf, "IDR_G7231L_");
	}

	if(!r_bRecording)
	{
		TRACE("\n Error : OnSendDTMF error Recording[%d]", r_bRecording);
		return FALSE;
	}
	itoa(lParam, tmp, 10);	
	strcat(dtmf, tmp);

	hRsrc = FindResource(AfxGetApp()->m_hInstance, dtmf, "G7231");
	if(!hRsrc)
	{
		TRACE("\n Error : SendDTMF error Resource[%s]", dtmf);
		return FALSE;
	}
	hGResource = LoadResource(AfxGetApp()->m_hInstance, hRsrc);
	if(!hGResource)
	{
		TRACE("\n Error : SendDTMF error Resource[%s]", dtmf);
		return FALSE;
	}
	lpFile = LockResource(hGResource);
	if(!lpFile)
	{
		TRACE("\n Error : SendDTMF error Resource[%s]", dtmf);
		return FALSE;
	}

	memset(r_byDTMFBuffer, 0x00, sizeof(r_byDTMFBuffer));
	memcpy(r_byDTMFBuffer, lpFile, m_nPacketByte * 5);
		
	// by gomeisa 2003.01.29
//	if(SDEVICE_NOT_USE != m_dwSerialDeviceState && SDIAL_PLAY_SWITCH == m_dwSerialDialtoneState)
	if( (SDEVICE_NOT_USE != m_dwSerialDeviceState && SDIAL_PLAY_SWITCH == m_dwSerialDialtoneState))
	// Serial Phone 사용하면서 Hook switch On일때 DTMF RTP를 바로 전송한다.
	// Hook switch On이 아닐 경우는 원래대로 처리
	{	
		m_bSerialSendDummyPacket = FALSE;
		for(int i = 0; i < 5/r_nSendEncodedCount; i++)
		{			
			r_pRTPCtrlManager->SendRTPData(&r_byDTMFBuffer[i* r_nSendEncodedCount * m_nPacketByte], r_nSendEncodedCount * m_nPacketByte, t_Audio);
			if(r_AudioPayloadType == t_G7231 || r_AudioPayloadType == t_G7231Low)
			{
				Sleep(20 * r_nSendEncodedCount);
			}

			if(i == (5/r_nSendEncodedCount -1) && 5%r_nSendEncodedCount != 0)
			{
				r_pRTPCtrlManager->SendRTPData(&r_byDTMFBuffer[(i + 1)* r_nSendEncodedCount * m_nPacketByte], r_nSendEncodedCount * m_nPacketByte, t_Audio);
			}
		}
		m_bSerialSendDummyPacket = TRUE;
	}
	else
	{		
		r_nDTMFSendCount = 0;
		r_bDTMFSendFlag = TRUE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT	CAudioSound::OnRecordSetMuteAudioRTP(WPARAM wParam, LPARAM lParam);
// 
// - 무음을 보내는 변수를 설정하는 함수임
// - lParam으로 TRUE or FALSE의 값으로 받는다.
////////////////////////////////////////////////////////////////////////////////////
LRESULT	CAudioSound::OnRecordSetMuteAudioRTP(WPARAM wParam, LPARAM lParam)
{
	r_bSendMuteAudioRTP = lParam;
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayStart(WPARAM wParam, LPARAM lParam)
// - Play Starting
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayStart(WPARAM wParam, LPARAM lParam)
{
	if(m_dwSerialDeviceState)
	{
		if(m_hSerialPort != INVALID_HANDLE_VALUE)
		{
			TRACE("\nCAudioSound::OnPlayStart(p_bPlay=TRUE)\n");
			p_bPlay = TRUE;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	MMRESULT mmReturn = 0;
	
	if(!r_hRecord && r_nAvailableCount > 0)
	{	
		// open wavein device
		Sleep(500);
		TRACE("\n[OnRecordStart] waveInOpen Begin\n");
		mmReturn = ::waveInOpen( &r_hRecord, r_nDeviceIndex, &r_WaveFormatEx, 
			::GetCurrentThreadId(), 0, CALLBACK_THREAD);
		TRACE("\n[OnRecordStart] waveInOpen End\n");

		if(mmReturn)
		{	
			TRACE("\n[OnRecordStart] MMRESULT = %d, GetLastError() = %d\n", mmReturn, GetLastError());
			for(int i=0; i< r_nAvailableCount; i++)
			{
				Sleep(500);
				TRACE("\n[OnRecordStart] [retry] waveInOpen Begin (%d)\n", i);
				mmReturn = ::waveInOpen(&r_hRecord, i, &r_WaveFormatEx, 
					::GetCurrentThreadId(), 0, CALLBACK_THREAD);
				TRACE("\n[OnRecordStart] [retry] waveInOpen End (%d)\n", i);
				if(!mmReturn)
				{
//					DbgOut("\n[SCDialer1] OnStartRecording [retry]: device index = %d", i);
/*					CString str;
					str.Format("OnStartRecording [retry]: device index = %d", i);
					AfxMessageBox(str);
*/					break;
				}
				else
					TRACE("\n[OnRecordStart] [retry] MMRESULT = %d, GetLastError() = %d\n", mmReturn, GetLastError());

			}
			
			if(mmReturn)
			{
				char errorbuffer[MAX_PATH];
				char errorbuffer1[MAX_PATH];
				waveInGetErrorText( mmReturn, errorbuffer, MAX_PATH);
				sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);
//				DbgOut("\n[OnRecordStart] [Error] %s\n", errorbuffer1); 
			}			
		}

	}

	unsigned long time;
	time = timeGetTime();
	TRACE("\n		CPlaySound StartPlaying Start Device Index[%d], tick[%ld] ", p_nDeviceIndex, time);
	if(!p_bPlay && p_nAvailableCount > 0)
	{
		// open wavein device
		mmReturn = ::waveOutOpen( &p_hPlay, p_nDeviceIndex, &p_WaveFormatEx, 
			::GetCurrentThreadId(), 0, CALLBACK_THREAD);
		
		if(mmReturn )
		{
			char errorbuffer[MAX_PATH];
			char errorbuffer1[MAX_PATH];
			waveOutGetErrorText( mmReturn, errorbuffer, MAX_PATH);
			sprintf(errorbuffer1,"WAVEOUT:%x:%s",mmReturn,errorbuffer);

			TRACE("\n[SCDialer1] [Error] [OnPlayStart] device index[%d] : %s", p_nDeviceIndex, errorbuffer1);
			for(int i=0; i< p_nAvailableCount; i++)
			{
				mmReturn = ::waveOutOpen( &p_hPlay, i, &p_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
				if(!mmReturn)
				{
					TRACE("\n[SCDialer1] OnStartPlaying [retry]: device index = %d", i);
/*					CString str;
					str.Format("OnStartPlaying [retry]: device index = %d", i);
					AfxMessageBox(str);
*/					break;
				}
				else
				{
					waveOutGetErrorText( mmReturn, errorbuffer, MAX_PATH);
					sprintf(errorbuffer1,"WAVEOUT:%x:%s",mmReturn,errorbuffer);
					TRACE("\n[SCDialer1] [Error] [OnPlayStart] device index[%d] : %s", i, errorbuffer1);
				}
			}
			
			if(mmReturn)
			{
				char errorbuffer[MAX_PATH];
				char errorbuffer1[MAX_PATH];
				waveOutGetErrorText( mmReturn, errorbuffer, MAX_PATH);
				sprintf(errorbuffer1,"WAVEOUT:%x:%s",mmReturn,errorbuffer);
				TRACE("\n[SCDialer1] OnStartPlaying [retry]: device index = %d", i);
				//AfxMessageBox(errorbuffer1);
			}
			  
		}		
		else
		{
			TRACE("\n[SCDialer1] OnStartPlaying : device index = %d", p_nDeviceIndex);
/*			CString str;
			str.Format("OnStartPlaying : device index = %d", p_nDeviceIndex);
			AfxMessageBox(str);
*/		}
		
		
		if(!mmReturn)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 스피커 볼륨을 전달한다.
			BYTE bySpkVol = 0;
			if(m_pH323Conn
				&& this->GetPlayVolume(&bySpkVol))
			{
				m_pH323Conn->OnVolumeW(TRUE, bySpkVol);
			}

			p_bPlay = TRUE;
			TRACE("====> End, Term[%d]", GetTickCount() - time);
		}		
	}
	//*/
	
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayStop(WPARAM wParam, LPARAM lParam)
// - Play Stop
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayStop(WPARAM wParam, LPARAM lParam)
{	
	m_bSerialDialtoneReady = FALSE;
	m_bSerialBusytoneReady = FALSE;

	
	
	MMRESULT mmReturn = 0;

	if(!p_bPlay){ return FALSE;}

	
	//TRACE("\n		CPlaySound StopPlaying");
	if(m_dwSerialDeviceState)
	{
		DWORD	dwRead = MAX_SERIAL_BUFFER_SIZE, dwError, dwErrorFlags;
		COMSTAT	comstat;
		DWORD	dwWritten;
		p_bPlay = FALSE;
		BYTE sBuff[MAX_SERIAL_BUFFER_SIZE];
		ZeroMemory(sBuff, MAX_SERIAL_BUFFER_SIZE);
		if(!WriteFile( m_hSerialPort, sBuff, dwRead , &dwWritten, &m_ovWrite))
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				while (!GetOverlappedResult(m_hSerialPort ,&m_ovWrite,&dwWritten, FALSE))
				{
					dwError = GetLastError();
					if(dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hSerialPort,&dwErrorFlags, &comstat);							
						break;
					}
				}
			}
			else
			{
				dwWritten = 0;
				ClearCommError (m_hSerialPort, &dwErrorFlags ,&comstat);					
			}
		}
		
		return TRUE;
	}	
	
	p_bPlay = FALSE;
	mmReturn = ::waveOutReset(p_hPlay);	
	if(!mmReturn)
	{
		mmReturn = ::waveOutClose(p_hPlay);
	}	
	p_hPlay = NULL;
	p_nWriteCount = 0;
	p_dwWriteNotSkipCount = 0;	


	m_bMMTimer = TRUE;

	int i=0;
	while(i < CRTPCTRLARRAYSIZE)
	{
		b_pPlayBufferArray[i]->InitializeVariable();
		b_bPlayStartFlag[i] = FALSE;
		i++;
	}

#ifdef _ECHO_CANCELLER
	if(m_bApplyEchoCancellerFlag)
	{
		LockEchoCanceller();
		EndEchoCanceller();
		UnlockEchoCanceller();
	}
#endif // _ECHO_CANCELLER
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam)
// - Play Data Receive and write
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam)
{	
	LPWAVEHDR lpHdr = (LPWAVEHDR)lParam;
	MMRESULT mmResult = 0;
	
	if(lpHdr)
	{
		if(p_bPlay)	
		{						
			///////////////////////////////////////////////////////////////////////////////
			// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
			if(INVALID_SPK_VOLUME_VALUE != m_nPlayVolumeOfPercent)
			{
				this->SetPlayVolume(m_nPlayVolumeOfPercent, TRUE);
				m_nPlayVolumeOfPercent		= INVALID_SPK_VOLUME_VALUE;
			}

			short int* lpInt = (short int*) lpHdr->lpData;
			DWORD dwSamples = lpHdr->dwBufferLength/sizeof(short int);
			
			mmResult = ::waveOutPrepareHeader(p_hPlay, lpHdr, sizeof(WAVEHDR));
			if(mmResult)
			{
				TRACE("error from waveoutprepareheader\n");
			}			
			mmResult = ::waveOutWrite(p_hPlay, lpHdr, sizeof(WAVEHDR));
			if(mmResult)
			{
				TRACE("error from waveoutwrite\n");
			}
			
			p_nWriteCount++;			
		}
		else
		{
			if(lpHdr && lpHdr->lpData)
			{
				delete[] lpHdr->lpData;
			}					
			if(lpHdr)
			{
				delete lpHdr;
			}
		}
	}
	
	return ERROR_SUCCESS;
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
		::waveOutUnprepareHeader(p_hPlay, lpHdr, sizeof(WAVEHDR));
#ifdef _ECHO_CANCELLER
		if(m_bApplyEchoCancellerFlag)
		{
			LockEchoCanceller();
			StartEchoCanceller();
			memcpy(m_pEchoCancelBuffer, lpHdr->lpData, lpHdr->dwBufferLength);
			UnlockEchoCanceller();			
		}
#endif
		if(lpHdr && lpHdr->lpData)
		{
			delete[] lpHdr->lpData;
			delete lpHdr;
			lpHdr = NULL;
		}
		if(lpHdr)
		{
			delete lpHdr;
		}	
	}
	return ERROR_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////
// LRESULT CAudioSound::OnPlayReset(WPARAM wParam, LPARAM lParam)
// - waveOut device를 reset한다.
////////////////////////////////////////////////////////////////////////////////////
LRESULT CAudioSound::OnPlayReset(WPARAM wParam, LPARAM lParam)
{	
	TRACE("\n			Reset");
	if(p_bPlay)
	{
		::waveOutReset(p_hPlay);		
		p_nWriteCount = 0;
	}
	
#ifdef _ECHO_CANCELLER
	if(m_bApplyEchoCancellerFlag)
	{
		LockEchoCanceller();
		EndEchoCanceller();
		UnlockEchoCanceller();
	}
#endif // _ECHO_CANCELLER

	return TRUE;
}

#define _SERIAL_ALIVE_TIME_GAP		10000
/////////////////////////////////////////////////////////////////////////////
// CMixSound message handlers

#include <winioctl.h>

BOOL InstallAndStartDriver(HANDLE hSCManager, LPCTSTR DriverName, LPCTSTR ServiceExe);
BOOL LoadDynamicNTDriver(VOID);
BOOL UnloadDynamicNTDriver(VOID);
HANDLE LoadDriver(OSVERSIONINFO *os, BOOL *fNTDynaLoaded);

#define FILE_DEVICE_UNKNOWN             0x00000022
#define IOCTL_UNKNOWN_BASE              FILE_DEVICE_UNKNOWN

#define IOCTL_NOTIFY_USER_EVENT     CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0801, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_THREAD_START_EVENT    CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0802, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_THREAD_STOP_EVENT     CTL_CODE(IOCTL_UNKNOWN_BASE, 0x0803, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

void CALLBACK TimerProcMix(UINT uiID,UINT uiMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
	DWORD	tmGap, tmCur;
	CAudioSound* pSound = (CAudioSound*)dwUser;
	
	if(pSound == NULL){ return;}

	if(pSound->m_dwSerialDeviceState)
	{
		tmCur = timeGetTime();
		tmGap = tmCur - pSound->m_tmSerialAlive;
		if(tmGap > _SERIAL_ALIVE_TIME_GAP)
		{
			if(pSound->m_dwSerialDeviceState == SDEVICE_READY)
			{				
				pSound->m_dwSerialDialtoneState = SDIAL_NO_PLAY;
				pSound->m_dwSerialDeviceErrorCount = 10;
				pSound->m_dwSerialDeviceState = SDEVICE_ERROR;			// serial device error
				pSound->OnSerialSignal(0, 31);							// Serial Device Disconnect
				TRACE("\n			Serial Cable removed(3), DeviceState[%d], errCount[%d], Gap[%d]", pSound->m_dwSerialDeviceState, pSound->m_dwSerialDeviceErrorCount, tmGap);				
			}
			else if(pSound->m_dwSerialDeviceState == SDEVICE_ERROR)
			{		
				if(pSound->m_dwSerialDeviceErrorCount > 0)
				{
					pSound->m_dwSerialDeviceErrorCount--;					
				}
			}			
		}
		if(pSound->m_TxQueue.m_nTotalBytes < 500)
		{
			pSound->OnMixPlay(0, 0);
		}	
		pSound->OnSerialWrite(0, 40);
		
		if(pSound->m_bSerialSendDummyPacket == TRUE)
		{
			tmGap = tmCur - pSound->m_dwLastTimeSendDummyPakcet;						
			if((pSound->r_AudioPayloadType == t_G7231 || pSound->r_AudioPayloadType == t_G7231Low) 
				&& tmGap >= 30 * pSound->r_nSendEncodedCount)
			{
				pSound->m_dwLastTimeSendDummyPakcet = tmCur;
				BYTE	byDummy[240];
				memset(byDummy, 0x00, 240);
				pSound->r_pRTPCtrlManager->SendRTPData(byDummy, pSound->m_nPacketByte * pSound->r_nSendEncodedCount, t_Audio);				
			}
		}
	}
	else
	{
		if(pSound->p_bPlay)
		{
			MMTIME pmmt;
			pmmt.wType = TIME_BYTES;
			waveOutGetPosition(pSound->p_hPlay, &pmmt, sizeof(MMTIME));
			DWORD posGap = pSound->p_nWriteCount*480 - pmmt.u.cb;
			
			if(posGap < 4800 || (posGap < 5760 && posGap % 3 == 0))
			{
// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//				pSound->WriteSystemLogEX(FALSE, " Begin--> \r\n");		// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
				pSound->OnMixPlay(0, 0);				
// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//				pSound->WriteSystemLogEX(FALSE, "      -->End \r\n");	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
			}
		}
	}
	//---------------------- 녹음 raw file ----------------------------->
/*	BYTE buff[24];
	if(pSound && pSound->GetMainWnd())
	{			
		if(pSound->m_pRawFile && pSound->m_pRawFile->Read(buff, 24) == 24)
		{
			JittData*	lpbyte;
			lpbyte = new JittData;
			lpbyte->dataSize = 24;
			lpbyte->sequenceNumber = 1;
			lpbyte->time = 240;
			lpbyte->lpData = new BYTE[lpbyte->dataSize];
			
			memcpy(lpbyte->lpData, buff, lpbyte->dataSize);	
			
			if(pSound)
			{
				pSound->PostThreadMessage(WM_MIX_FORMRTPTOBUFFER, (LPARAM)lpbyte, 0);
			}
		}
		else
		{			
			pSound->m_pRawFile->Close();
			delete pSound->m_pRawFile;
			pSound->m_pRawFile = NULL;
		}
	}
*/
}

/////////////////////////////////////////////////////////////////////////////
// LRESULT	CAudioSound::OnMixStart(WPARAM wParam, LPARAM lParam)
//
// - CMixSound 멀티미디어 타이머를 시작한다.
// - wParam 은 필요 없음
// - lParam 는 CPlaySound 스레드의 포인터
// - return 값은 필요 없음
/////////////////////////////////////////////////////////////////////////////
LRESULT	CAudioSound::OnMixStart(WPARAM wParam, LPARAM lParam)
{	
	if(b_bSetTimerFlag == FALSE)
	{
		//TRACE("\n					CMixSound Start 1 ");
		b_nTimer_id=timeSetEvent(MULTIMEDIA_TIMER_TIME, 1, &TimerProcMix, (DWORD)this, TIME_PERIODIC);
		b_bSetTimerFlag = TRUE;
	}
	//TRACE("====> 2");
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
LRESULT	CAudioSound::OnMixStop(WPARAM wParam, LPARAM lParam)
{	
	if(b_bSetTimerFlag == TRUE && b_nTimer_id)
	{
		//TRACE("\n					CMixSound Stop 1 ");
		timeKillEvent(b_nTimer_id);
		b_bSetTimerFlag = FALSE;
		b_nTimer_id = 0;
	}
	//TRACE("====> 2");

	int i=0;
	while(i < CRTPCTRLARRAYSIZE)
	{
		b_pPlayBufferArray[i]->InitializeVariable();
		b_bPlayStartFlag[i] = FALSE;
		i++;
	}
	
	return TRUE;
}


LRESULT	CAudioSound::OnMixFromRTPtoBuffer(WPARAM wParam, LPARAM lParam)
{
	JittData	*lp = NULL, *lpIn = NULL;
	int			nCount = 0;
	int			nByte = 0;
	
	lp = (JittData*)wParam;
	if((lParam < 0 && lParam > CRTPCTRLARRAYSIZE))
	{
		if( lp && lp->lpData){ delete[] lp->lpData; delete lp; lp = NULL;}
		else{ if(lp){ delete lp;}}
		return 0;
	}
	
	if(m_dwSerialDeviceState && m_bSerialTimeOutTone)
	{	
		delete lp->lpData;
		delete lp;
		return 0;
	}
	if(b_bPlayG7231Ready)
	{
		// RTP Data 가 들어오면 ringing back tone 을 없앤다. (PC2PC, Phone2PC ==> Serial Phone)
		TRACE("\nRingStop in OnMixFromRTPtoBuffer\n");
		OnMixLocalPlayG7231(0, 0);
		b_bPlayStartFlag[0] = FALSE;
		b_pPlayBufferArray[0]->EmptyDataQueue();
	}

#ifdef _G7231CODEC
	if(lp->dataSize%24 == 0){ nByte = 24;}
	else{ nByte = 20;}
#endif
#ifdef _G729CODEC
	nByte = 10;
#endif

	if(b_arrDecoder[lParam] && b_pPlayBufferArray[lParam] && lp && lp->lpData && lp->dataSize > 0 )
	{
		for(int i=0; i < lp->dataSize/nByte ; i++)
		{	
			lpIn = NULL;
			lpIn = new JittData;
			memset(lpIn, 0x00, sizeof(JittData));
			lpIn->dataSize = m_nFrameByte;
			lpIn->sequenceNumber = lp->sequenceNumber ;
			lpIn->time = lp->time + 240 * i;
			lpIn->lpData = new BYTE[lpIn->dataSize];
			memset(lpIn->lpData, 0x00, lpIn->dataSize);	
			
			b_arrDecoder[lParam]->AudioCodecDecoding(&(lp->lpData[i*nByte]), (short*)lpIn->lpData);	
			
			nCount = b_pPlayBufferArray[lParam]->SetDataToBuffer(lpIn);
//			//DbgOut("\n SetDataToBuffer count[%d], Flag[%d]", nCount, b_bPlayStartFlag[lParam]);

			if(nCount >= b_nBufferPlay_Start_Count && b_bPlayStartFlag[lParam] == FALSE)
			{
				b_bPlayStartFlag[lParam] = TRUE;
				// 2008-07-15 by cmkwon, SCDialer 죽는 버그 수정 -  OnMixPlay() 두개의 스레드에서 호출하면서 발생하는 문제 해결
				//OnMixPlay(0, 0);				
			}
			
			if(nCount >= b_nBufferPlay_Reset_Count)
			{	
				b_pPlayBufferArray[lParam]->DeleteDataFromBuffer(nCount - b_nBufferPlay_Start_Count);
			}			
		}
	}
		
	if( lp && lp->lpData){ delete[] lp->lpData; delete lp; lp = NULL;}
	else{ if(lp){ delete lp;}}

	return TRUE;
}

void CAudioSound::MixingOperation(short *src1, short *src2, short *des, int shortSize)
{
	int in;
		
	for(int i=0; i < shortSize ; i++)
	{		
		in = (src1[i]+src2[i])*3/5;

		if(in < SHRT_MIN )
		{
			des[i] = SHRT_MIN;
		}
		else if(in > SHRT_MAX)
		{
			des[i] = SHRT_MAX;
		}
		else
		{
			des[i] = in;
		}			
	}	
}

LRESULT	CAudioSound::OnMixPlay(WPARAM wParam, LPARAM lParam)
{	
	LPWAVEHDR	lpHdr = NULL;
	JittData*	lp[CRTPCTRLARRAYSIZE];		
	int			count = 0;
	int			nSize;
	int			arrIndex[CRTPCTRLARRAYSIZE];
	
	int i=0;
	while(i < CRTPCTRLARRAYSIZE)
	{
		lp[i] = NULL;
		if( (b_bPlayStartFlag[i] == TRUE) && (b_pPlayBufferArray[i]->GetDataCount() >= 1))
		{			
			lp[i] = b_pPlayBufferArray[i]->GetDataFromBuffer();
			arrIndex[count] = i;			
			count++;
		}
		else if(b_bPlayStartFlag[i] == TRUE)
		{
			if(m_dwSerialDeviceState == 0)
			{
				MMTIME pmmt;
				pmmt.wType = TIME_BYTES;
				waveOutGetPosition(p_hPlay, &pmmt, sizeof(MMTIME));
				DWORD posGap = p_nWriteCount*480 - pmmt.u.cb;
				if(posGap < 100)
				{
					b_bPlayStartFlag[i] = FALSE;
				}
			}
			else
			{
				if(m_TxQueue.m_nTotalBytes < 100)
				{
					b_bPlayStartFlag[i] = FALSE;
				}
			}
		}
		i++;
	}	

	if( count > 0)
	{
		nSize = lp[arrIndex[0]]->dataSize;
		lpHdr = CreateWaveHeader( nSize );	
		
		if(count == 1)
		{
			
#ifdef _KWON_DEBUG			
//			if(b_nPlayCount%10 == 0)
			{
				//TRACE("\n");
			}
			//TRACE("[%ld]->%3d ", lp[arrIndex[0]]->sequenceNumber, m_pPlayBufferArray[arrIndex[0]]->GetDataCount() );	// Sequence Number
			//TRACE("[%ld]->%3d ", lp[arrIndex[0]]->time , m_pPlayBufferArray[0]->GetDataCount() );		// Time Stamp
#endif
			memcpy( lpHdr->lpData, lp[arrIndex[0]]->lpData , nSize);				
		}
		else if(count == 2)
		{		
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);		
		}
		else if(count == 3)
		{
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		}
		else if(count == 4)
		{
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		}
		else if(count == 5)
		{
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		}
		else if(count == 6)
		{
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		}
		else if(count == 7)
		{
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[1]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[3]]->lpData, (short*)lp[arrIndex[2]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[5]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[2]]->lpData, (short*)lp[arrIndex[0]]->lpData, (int)nSize /2);		
			MixingOperation((short*)lp[arrIndex[4]]->lpData, (short*)lp[arrIndex[6]]->lpData, (short*)lp[arrIndex[4]]->lpData, (int)nSize /2);
			MixingOperation((short*)lp[arrIndex[0]]->lpData, (short*)lp[arrIndex[4]]->lpData, (short*)lpHdr->lpData, (int)nSize /2);
		}
		else
		{
			if(lpHdr && lpHdr->lpData)
			{
				delete[] lpHdr->lpData;
			}
			if(lpHdr)
			{
				delete lpHdr;
			}
			
			lpHdr = NULL;
			
			//AfxMessageBox("Error : OnPlayMixSound error mix count[%d]", count);
		}

		if(lpHdr)
		{
			//this->PostThreadMessage(WM_PLAY_PLAYBLOCK, GetCurrentThreadId(),(LPARAM)lpHdr);

			if(m_dwSerialDeviceState)				// Serial Device OnMixPlay
			{				
				BYTE rBuff[240];
				Change16to8(480, (short*)lpHdr->lpData, rBuff);
				if(p_bPlay){ m_TxQueue.InsertFront(rBuff, 240);}
				delete[] lpHdr->lpData;
				delete lpHdr;
				lpHdr = NULL;
			}
			else
			{
				OnPlayWriteSoundData(GetCurrentThreadId(), (LPARAM)lpHdr);
			}			
		}
		
		for( i =0 ; i < count; i++)
		{
			if(lp[arrIndex[i]] && lp[arrIndex[i]]->lpData)
			{
				delete[] lp[arrIndex[i]]->lpData;
			}
			if(lp[arrIndex[i]])
			{
				delete lp[arrIndex[i]];
			}
		}			
	}

	if(!p_bPlay)
	{			
		// 2003.06.11 by gomeisa : Dial tone 은수화기를 들고 있거나 온후크를 누른 상태에서만 재생되게 하고
		// Busy tone 은 수화기 On/off 상관없이 재생
		
		// Dialtone Ready and Play stat
		//			if(m_dwSerialDialtoneState == SDIAL_PLAY_HOOK || m_dwSerialDialtoneState == SDIAL_PLAY_SWITCH )			
		if(m_bSerialBusytoneReady && b_pPlayG7231Decoder)
		{
			if(m_dwSerialDeviceState)
			{
				BYTE	Data[480];
				BYTE	rBuff[240];
				
				b_pPlayG7231Decoder->AudioCodecDecoding(&(m_SerialBusytoneBuffer[m_nSerialBusytoneCount*24]), (short*)Data);					
				Change16to8(480, (short*)Data, rBuff);
				m_TxQueue.InsertFront(rBuff, 240);
			}			
			m_nSerialBusytoneCount = (m_nSerialBusytoneCount+1)%(m_nSerialBusytoneSize);
		}
		else if(m_bSerialDialtoneReady && b_pPlayG7231Decoder)
		{
			// 2003.06.11 by gomeisa : 수화기를 들고 있거나 온후크를 누른 상태에서만 재생
			
			if(m_dwSerialDeviceState)
			{
				if(m_dwSerialDialtoneState == SDIAL_PLAY_HOOK || m_dwSerialDialtoneState == SDIAL_PLAY_SWITCH )			
				{
					BYTE	Data[480];
					BYTE	rBuff[240];
					
					b_pPlayG7231Decoder->AudioCodecDecoding(&(m_SerialDialtoneBuffer[m_nSerialDialtoneCount*24]), (short*)Data);							
					Change16to8(480, (short*)Data, rBuff);
					m_TxQueue.InsertFront(rBuff, 240);						
					m_nSerialDialtoneCount = (m_nSerialDialtoneCount+1)%(m_nSerialDialtoneSize);
				}
			}
		}
	}
	else
	{
//		//DbgOut("\n	bPlay[%d], DialState[%d], PlayReady[%d], count[%d], Time[%d], count[%d]",
		//	p_bPlay, m_dwSerialDialtoneState, b_bPlayG7231Ready, b_pPlayBufferArray[0]->GetDataCount(), m_bSerialTimeOutTone, count);
		
		// 2008-07-15 by cmkwon, SCDialer 죽는 버그 수정 - 아래와 같이 수정, b_nPlayG7231Size 이 다른 스레드에서 0 로 변경되는 현상이 있다.
		//if(b_bPlayG7231Ready && b_pPlayG7231Decoder)
		int nTmb_nPlayG7231Size			= b_nPlayG7231Size;
		int nTmm_nSerialBusytoneSize	= m_nSerialBusytoneSize;
		if(b_bPlayG7231Ready && b_pPlayG7231Decoder && nTmb_nPlayG7231Size > 0)
		{
			JittData	*lpIn = NULL;
			lpIn = new JittData;
			lpIn->dataSize = m_nFrameByte;
			lpIn->sequenceNumber = b_pPlayBufferArray[0]->GetHighSeqNuber();
			lpIn->time = 0;
			lpIn->lpData = new BYTE[lpIn->dataSize];
			memset(lpIn->lpData, 0x00, lpIn->dataSize);
			
			b_arrDecoder[0]->AudioCodecDecoding(&(b_PlayG7231Buffer[b_nPlayG7231Count*24]), (short*)lpIn->lpData);	
			
			int nCount = b_pPlayBufferArray[0]->SetDataToBuffer(lpIn);
			if(m_dwSerialDialtoneState)
			{
				if(b_bPlayStartFlag[0] == FALSE)
				{
					if(count <= 0)
					{
						BYTE	Data[480];
						BYTE	rBuff[240];					
						memset(Data, 0x00, 480);
						Change16to8(480, (short*)Data, rBuff);
						m_TxQueue.InsertFront(rBuff, 240);
					}
					if(b_pPlayBufferArray[0]->GetDataCount() >= 4)
					{
						b_bPlayStartFlag[0] = TRUE;
					}
				}
			}
			else
			{
				if(nCount > 3 && b_bPlayStartFlag[0] == FALSE)
				{
					b_bPlayStartFlag[0] = TRUE;
				}
			}
			
			if(b_bPlayG7231Looped)
			{
				b_nPlayG7231Count = (b_nPlayG7231Count+1)%(nTmb_nPlayG7231Size);		
			}
			else
			{
				if(b_nPlayG7231Count >= nTmb_nPlayG7231Size-1)
				{
					OnMixLocalPlayG7231(0, 0);
					if(!m_DTMFList.IsEmpty())
					{
						OnMixLocalPlayG7231(m_DTMFList.GetHead(), TRUE);
						m_DTMFList.RemoveHead();
					}
				}
				else
				{
					b_nPlayG7231Count = b_nPlayG7231Count+1;
				}					
			}				
		}
		else if(m_bSerialBusytoneReady && b_pPlayG7231Decoder && nTmm_nSerialBusytoneSize > 0)
		{
			if(!m_dwSerialDeviceState)
			{
				JittData	*lpIn = NULL;
				lpIn = new JittData;
				lpIn->dataSize = m_nFrameByte;
				lpIn->sequenceNumber = 0;
				lpIn->time = 0;
				lpIn->lpData = new BYTE[lpIn->dataSize];
				memset(lpIn->lpData, 0x00, lpIn->dataSize);
				
				b_pPlayG7231Decoder->AudioCodecDecoding(&(m_SerialBusytoneBuffer[m_nSerialBusytoneCount*24]), (short*)lpIn->lpData);	
				
				int nCount = b_pPlayBufferArray[0]->SetDataToBuffer(lpIn);
				
				if(b_pPlayBufferArray[0]->GetDataCount() > 3 && b_bPlayStartFlag[0] == FALSE)
				{
					b_bPlayStartFlag[0] = TRUE;
				}
			}
			
			m_nSerialBusytoneCount = (m_nSerialBusytoneCount+1)%(nTmm_nSerialBusytoneSize);
		}
		else if(count <= 0 )
			// Play할 것이 아무것도 없으면 무음을 Play한다.
		{
			if(m_dwSerialDeviceState)
			{
				BYTE	Data[480];
				BYTE	rBuff[240];
				
				memset(Data, 0x00, 480);
				
				Change16to8(480, (short*)Data, rBuff);
				m_TxQueue.InsertFront(rBuff, 240);
			}
			
		}
	}
	return TRUE;
}

LRESULT	CAudioSound::OnMixEndRTPSession(WPARAM wParam, LPARAM lParam)
{

	if( lParam >= 0 && lParam < CRTPCTRLARRAYSIZE)
	{
		b_pPlayBufferArray[lParam]->InitializeVariable();
		b_bPlayStartFlag[lParam] = FALSE;
	}
	return TRUE;
}


LRESULT CAudioSound::OnMixLocalPlayG7231(WPARAM wParam, LPARAM lParam)
{
	if(lParam)
	{	
		HRSRC	hRsrc = NULL;
		HGLOBAL hGResource = NULL;
		LPVOID	lpFile = NULL;
		
		if(LOWORD(wParam) == 0 )		// Ring back tone Play
		{
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_RING", "G7231");
			b_nPlayG7231Value		= LOWORD(wParam);
			b_nPlayG7231Size		= 83;
			b_bPlayG7231Looped		= TRUE;
		}
		else if( LOWORD(wParam) == 1)	// 안내음성 Play
		{
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_MENT", "G7231");			
			b_nPlayG7231Value		= LOWORD(wParam);
			b_nPlayG7231Size		= 126;
			b_bPlayG7231Looped		= TRUE;
		}
		else if( LOWORD(wParam) == 2)	// Serial Phone Dialtone
		{
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_DIALTONE", "G7231");				
			m_nSerialDialtoneCount	= 0;				
			//m_nSerialDialtoneSize	= 138;			
			m_nSerialDialtoneSize	= 614;
		}
		else if(LOWORD(wParam) == 3)	// DTMF tone
		{			
			char	dtmf[20] = "IDR_G7231_";
			char	tmp[10];
			
			if(b_nPlayG7231Size){	m_DTMFList.AddTail(wParam);	return TRUE;}
			itoa(HIWORD(wParam), tmp, 10);	
			strcat(dtmf, tmp);
			
			hRsrc = FindResource(AfxGetApp()->m_hInstance, dtmf, "G7231");			
			b_nPlayG7231Value		= LOWORD(wParam);
			b_nPlayG7231Size		= 5;
			b_bPlayG7231Looped		= FALSE;
			m_bSerialTimeOutTone	= TRUE;			// Time Out tone 플래그를 TRUE로 설정
		}
		else if(LOWORD(wParam) == 4)	// Send/Redial key
		{				
			//if(b_nPlayG7231Size){	m_DTMFList.AddTail(wParam);	return TRUE;}
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_SENTBTN", "G7231");
			b_nPlayG7231Value		= LOWORD(wParam);
			b_nPlayG7231Size		= 20;
			b_bPlayG7231Looped		= FALSE;
		}
		else if(LOWORD(wParam) == 5)	// busy tone
		{				
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_BUSY", "G7231");				
			m_nSerialBusytoneCount	= 0;				
			m_nSerialBusytoneSize	= 30;
			b_bPlayG7231Looped		= TRUE;
		}
		else if(LOWORD(wParam) == 6)	// time out tone
		{
			hRsrc = FindResource(AfxGetApp()->m_hInstance, "IDR_G7231_TIMEOUT", "G7231");
			b_nPlayG7231Value		= LOWORD(wParam);
			b_nPlayG7231Size		= 50;
			b_bPlayG7231Looped		= FALSE;
			m_bSerialTimeOutTone	= TRUE;			// Time Out tone 플래그를 TRUE로 설정
		}
		else
		{
			return FALSE;
		}
		
		if(hRsrc)
		{
			hGResource = LoadResource(AfxGetApp()->m_hInstance, hRsrc);
			if(hGResource)
			{
				lpFile = LockResource(hGResource);			
				if(lpFile)
				{				
					if(wParam == 2)
					{	// Serial Phone Dialtone
						memset(m_SerialDialtoneBuffer, 0x00, sizeof(m_SerialDialtoneBuffer));
						memcpy(m_SerialDialtoneBuffer, lpFile, 24 * m_nSerialDialtoneSize);	
						m_bSerialDialtoneReady = TRUE;	
					}
					else if(wParam == 5)
					{
						memset(m_SerialBusytoneBuffer, 0x00, sizeof(m_SerialBusytoneBuffer));
						memcpy(m_SerialBusytoneBuffer, lpFile, 24 * m_nSerialBusytoneSize);	
						m_bSerialBusytoneReady = TRUE;
					}
					else				
					{
						memset(b_PlayG7231Buffer, 0x00, sizeof(b_PlayG7231Buffer));						
						if(LOWORD(wParam) == 3)		// DTMF tone
						{
							memcpy(b_PlayG7231Buffer, lpFile, 24*b_nPlayG7231Size);
						}						
						else						
						{
							memcpy(b_PlayG7231Buffer, lpFile, 24 * b_nPlayG7231Size);	
						}
						b_bPlayG7231Ready = TRUE;						
					}
				}
			}			
		}

	}
	else
	{
		m_bSerialBusytoneReady	= FALSE;
		m_nSerialBusytoneCount	= 0;
		m_nSerialBusytoneSize	= 0;

		m_bSerialTimeOutTone	= FALSE;
		b_nPlayG7231Value		= -1;
		b_bPlayG7231Ready		= FALSE;
		b_nPlayG7231Count		= 0;
		b_nPlayG7231Size		= 0;			
	}
	return TRUE;	
}


LRESULT CAudioSound::OnSerialRead(WPARAM wParam, LPARAM lParam)
{	
	if(m_dwSerialDeviceState == 0){	return FALSE;}
	if(lParam != 0 && m_dwSerialDeviceState == SDEVICE_READY)
	{
		m_tmSerialAlive = timeGetTime();			// Serial Device Alive Message time 
	}
	if(lParam)		// ReadFile이 완료 된후 이기에 WriteFile 이벤트를 Set하며 읽은 데이타를 버퍼에 넣는다.
	{
		if(m_dwSerialDeviceState == SDEVICE_READY || (m_dwSerialDeviceState == SDEVICE_ERROR && m_dwSerialDeviceErrorCount == 0))		
		{			
			int i = 0;
			while(i < lParam)
			{
				if(i==0 && m_bSignalContinue)
				{
					m_bSignalContinue = FALSE;					
					if(m_bfSerialRead[0] != 255 && m_bfSerialRead[0] != 31)						
					{
						PostThreadMessage(WM_SERIAL_SIGNAL, 0, m_bfSerialRead[0]);
						m_bfSerialRead[0] = m_bfSerialRead[i+1];
					}					
				}
				else if(m_bfSerialRead[i] == 255)
				{
					if(i == lParam -1)
					{ 
						m_bSignalContinue = TRUE;
						if(i > 2)
						{
							m_bfSerialRead[i] = m_bfSerialRead[i-1];
						}
					}
					else
					{ 
						if(m_bfSerialRead[i+1] != 255 && m_bfSerialRead[i+1] != 31)						
						{
							PostThreadMessage(WM_SERIAL_SIGNAL, 0, m_bfSerialRead[i+1]);

							if( i > 2 && i < lParam -2)
							{
								m_bfSerialRead[i] = m_bfSerialRead[i-1];
								m_bfSerialRead[i+1] = m_bfSerialRead[i+2];
							}							
						}
					}
				}				
				i++;
			}			
			
			if(r_bRecording)
			{				
				m_RxQueue.InsertFront(m_bfSerialRead, lParam);
			}
		}		
	}

	DWORD	dwRead = 0, dwErrorFlags;
	COMSTAT	comstat;
	DWORD	length;
	BYTE	rBuff[240];								// RTP 패킷을 만들기 위해 240 bytes를 입력큐에서 빼내어 저장하는 버퍼
	short	pBuff[240];								// RTP 패킷을 만들기 위해 8bit값을 16bit값으로 변경하여 저장할 버퍼

	ZeroMemory(m_bfSerialRead, MAX_SERIAL_BUFFER_SIZE);
	if(!ReadFile(m_hSerialPort, m_bfSerialRead, MAX_SERIAL_BUFFER_SIZE, &dwRead, &m_ovRead))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			ClearCommError(m_hSerialPort, &dwErrorFlags, &comstat);
//			DbgOut("\n ReadFile Error[%d]", dwErrorFlags);
		}
	}	
	
	length = m_RxQueue.ExtractBack(rBuff, 240);	
	// 2003.06.25 by gomeisa
	if(m_bSerialTestMode)
	{
		m_lRxSerialData += length;
		PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, m_lRxSerialData, 10003);
	}

	if(length == 240)
	{
		if(m_bSerialSendDummyPacket == TRUE){ m_bSerialSendDummyPacket = FALSE;}
		if(r_bDTMFSendFlag && r_AudioPayloadType == t_G7231)
		{
			if( r_nDTMFSendCount + r_nSendEncodedCount >= 5)
			{
				memcpy(r_pEncodedBuffer, &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte*(5 - r_nDTMFSendCount));					
				r_nDTMFSendCount = 0;
				r_bDTMFSendFlag = FALSE;					
			}
			else
			{
				memcpy(r_pEncodedBuffer, &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte*r_nSendEncodedCount);
				r_nDTMFSendCount += r_nSendEncodedCount;					
			}
			r_nEncodedCount = r_nSendEncodedCount;
		}
		else if(r_bDTMFSendFlag && r_AudioPayloadType == t_G7231Low)
		{
			if(r_nDTMFSendCount >= 4)
			{
				memcpy(&r_pEncodedBuffer[r_nEncodedCount*m_nPacketByte], &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte);
				r_nDTMFSendCount = 0;
				r_bDTMFSendFlag = FALSE;					
			}
			else
			{
				memcpy(&r_pEncodedBuffer[r_nEncodedCount*m_nPacketByte], &r_byDTMFBuffer[r_nDTMFSendCount*m_nPacketByte], m_nPacketByte);
				r_nDTMFSendCount++;
			}
			r_nEncodedCount++;
		}
		else if(r_bSendMuteAudioRTP == TRUE){}
		else 
		{
			Change8to16(length, rBuff, pBuff);
			if(r_pEncoder)
			{						
				r_pEncoder->AudioCodecEncoding(pBuff, &r_pEncodedBuffer[r_nEncodedCount*m_nPacketByte]);
				r_nEncodedCount++;
			}
		}		
	}	
	
	if(r_nEncodedCount >= r_nSendEncodedCount && r_pRTPCtrlManager)
	{
		//TRACE("\n Send RTP %d, time[%d]", r_nEncodedCount, GetTickCount());
		/////////////////////////////////////////////////////////////////////////////
		// RTP channel 로 인코딩된 데이타를 보낸다.
		
		// application 에 log 를 보여주기 위한...  record 기능
/*		if(m_bLogWriteFile && m_pRawFile)
		{
			m_pRawFile->Write(r_pEncodedBuffer, m_nPacketByte*r_nEncodedCount);			
		}
		else
*/		{
			r_pRTPCtrlManager->SendRTPData(r_pEncodedBuffer, m_nPacketByte*r_nEncodedCount, t_Audio);		// Audio 			
		}
		ZeroMemory(r_pEncodedBuffer, sizeof(r_pEncodedBuffer));
		r_nEncodedCount = 0;				
	}
	
	return TRUE;
}

void CAudioSound::Change8to16(int sourceBytes, BYTE* source, short* dest)
{
	int i = 0;
	while(i < sourceBytes)
	{
		dest[i] = ulaw2linear(source[i]);		
		//dest[i] = (source[i] - 128) * 256;
		i++;
	}
}
	
void CAudioSound::Change16to8(int sourceBytes, short* source, BYTE* dest)
{
	int i = 0;
	while(i < sourceBytes/2)
	{
		dest[i] = linear2ulaw(source[i]);
		//dest[i] = (128 + source[i]/256);
		i++;
	}
}

LRESULT CAudioSound::OnSerialWrite(WPARAM wParam, LPARAM lParam)
{	
	DWORD	dwError;
	DWORD	dwErrorFlags;
	COMSTAT	comstat;
	DWORD	length = 0;
	DWORD	dwWritten;
	
	if(m_dwSerialDeviceState == 0){	return FALSE;}

	length = m_TxQueue.ExtractBack(m_bfSerialWrite, lParam);	
	if(length && (m_dwSerialDeviceState == SDEVICE_READY))
	{		
		if(!WriteFile(m_hSerialPort, m_bfSerialWrite, length , &dwWritten, &m_ovWrite))
		{
			int loopCount = 0;	
			while ( !GetOverlappedResult(m_hSerialPort , &m_ovWrite, &dwWritten, FALSE))
			{				
				dwError = GetLastError();
				if(dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hSerialPort, &dwErrorFlags, &comstat);
					break;
				}
				loopCount++;
				if(loopCount > 10)
				{
					//flush the port
//					DbgOut("\n\n =====================================> 555555 write error : %d ", dwError);
					PurgeComm(m_hSerialPort, PURGE_TXCLEAR | PURGE_TXABORT);
					break;
				}
				Sleep(1);
			}		
		}
		// 2003.06.25 by gomeisa
		if(m_bSerialTestMode)
		{
			m_lTxSerialData += length;
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, m_lTxSerialData, 10004);
		}
	
		FlushFileBuffers(m_hSerialPort);
	}
			
	return TRUE;
}

/*
	// Serial Error Flags
	#define CE_RXOVER           0x0001  // Receive Queue overflow
	#define CE_OVERRUN          0x0002  // Receive Overrun Error
	#define CE_RXPARITY         0x0004  // Receive Parity Error
	#define CE_FRAME            0x0008  // Receive Framing error
	#define CE_BREAK            0x0010  // Break Detected
	#define CE_TXFULL           0x0100  // TX Queue is full
	#define CE_PTO              0x0200  // LPTx Timeout
	#define CE_IOE              0x0400  // LPTx I/O Error
	#define CE_DNS              0x0800  // LPTx Device not selected
	#define CE_OOP              0x1000  // LPTx Out-Of-Paper
	#define CE_MODE             0x8000  // Requested mode unsupported
*/

LRESULT CAudioSound::OnSerialEvent(WPARAM wParam, LPARAM lParam)
{
	DWORD	dwEvent;

	dwEvent = lParam;

	TRACE("\n CE_RXPARITY[%d]	", dwEvent);
	if((dwEvent & EV_ERR) == EV_ERR )
	{	
		DWORD dwError = 0;
		COMSTAT	comstat;
		
		ClearCommError(m_hSerialPort, &dwError, &comstat);
		int a = 0;
		switch(dwError)
		{
		case CE_IOE:
			TRACE("\n CE_IOE");
			break;
		case CE_RXPARITY:
			TRACE("\n CE_RXPARITY");			
			break;
		case CE_RXOVER:
			TRACE("CE_RXOVER");
			break;
		case CE_TXFULL:
			TRACE("CE_TXFULL");
			break;
		case CE_OVERRUN:
			TRACE("CE_OVERRUN");
			break;
		case CE_BREAK:
			TRACE("CE_BREAK");
			break;
		case CE_FRAME:
			TRACE("CE_FRAME");
			break;
		case CE_MODE:
			TRACE("CE_MODE");
			break;
		}
	}

	return TRUE;
}

/*
	Serial Device Signal ( lParam )
	Serial Device	:	Control
	0 - 12			|	0 - 12			==> DTMF
	13				|	13				==> Hook switch on
	14				|	14				==> Send/Redial
	15				|	15				==> Home Page
	16				|	16				==> Phone Book List
	17				|	17				==> Shopping Mall Homepage
	85				|	18				==> Hook On
	170				|	19				==> Hook Off	
	50				|	20				==> Alive Message from SerialPhone to PC(Hoon On State)
	51				|	21				==> Alive Message from SerialPhone to PC(Hoon Off State)
	22				|	22				==> Hook switch off
	23				|	23				==> Function 1
	24				|	24				==> Function 2
	없음			|	30				==> Serial device connect OK
	없음			|	31				==> Serial device disconnect(통화 도중 Serial device 제거)
	없음			|	32				==> Serial device connect error(Port가 사용중입니다.)
	없음			|	33				==> Serial device connect error(Port가 유효하지 않습니다.)
	없음			|	34				==>	Received Dial Message(PCtoPC로 전화가 온경우)
	없음			|	35				==>	Received Hanup Message(PCtoPC로 전화가 와서 Ringing중에 전화를 건사람이 전화를 끊을때 처리를 위해)
	없음			|	36				==>	Received Busy Message(PCtoPC로 전화를 걸때 상대방이 통화중일때 Busy Tone을 발생하기 위해서)
	없음			|	37				==> Send/Redial DTMF tone 재생을 취소
	없음			|	52				==> Serial device is invalid (Device 가 invalid 하다.)

//*/
LRESULT CAudioSound::OnSerialSignal(WPARAM wParam, LPARAM lParam)
{
	// Application 에 serial log 를 보여주기 위한 메세지...
	// 2003.06.25 by gomeisa
	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, lParam, 10000);

	if(!IsWindow(m_hCtrl) || !m_dwSerialDeviceState){	return FALSE;}

	if(m_dwSerialDeviceGarbageCount > 0)
	{
		m_dwSerialDeviceGarbageCount--;
		return FALSE;
	}

#ifdef _KWON_DEBUG	
	//if(lParam != 50 && lParam != 51 && 12 < lParam)
	if(m_dwSerialDeviceState) // Audio device 가 아니면
	{
		if(lParam == m_nKeepAliveHookOn || m_nKeepAliveHookOff == 51)
		{
			CTime t = CTime::GetCurrentTime();
//			DbgOut("\n Serial ==> Signal[%3d], Device[%3d], Dialtone[%3d] %d:%d:%d", lParam, m_dwSerialDeviceState, m_dwSerialDialtoneState, 
//				t.GetHour(), t.GetMinute(), t.GetSecond());
		}
		else
		{
			CTime t = CTime::GetCurrentTime();
//			DbgOut("\n	Serial ==> Signal[%3d], Device[%3d], Dialtone[%3d] %d:%d:%d", lParam, m_dwSerialDeviceState, m_dwSerialDialtoneState, 
//				t.GetHour(), t.GetMinute(), t.GetSecond());
		}		
	}
#endif

		
	if(m_dwSerialDeviceState == SDEVICE_ERROR && m_dwSerialDeviceErrorCount == 0)
	{		
		m_tmSerialAlive = timeGetTime();
		m_dwSerialDialtoneState = SDIAL_NO_PLAY;
		m_dwSerialDeviceState = SDEVICE_READY;
		if(m_bKeepAliveKey)
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
		
		TRACE("\n			Serial Cable repair,	DeviceState[%d], errCount[%d]", m_dwSerialDeviceState, m_dwSerialDeviceErrorCount);
	}
	else if(m_dwSerialDeviceState == SDEVICE_READY)
	{
		DWORD	tmGap, tmCur;
		
		tmCur = timeGetTime();
		tmGap = tmCur - m_tmSerialAlive;
		m_tmSerialAlive = tmCur;
//		//DbgOut("	==>Gap[%d]", tmGap);
	}
	
	if(lParam > 0 && lParam <= 17 || lParam == 100)
	{
		if(!m_bSerialConnectSend)
		{
			if(m_bKeepAliveKey)
			{
				m_bSerialConnectSend = TRUE;
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
			}
		}
		if(lParam == 100) // iRoad 2차 sp 는 Redial 버튼이 100 이다.
			lParam = 14;

		if(m_bKeepAliveKey)
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, lParam);
		switch(lParam)
		{
		case 13:	// Hook switch on			
			if(m_dwSerialDialtoneState == SDIAL_NO_PLAY)
			{
				if(!m_bSerialDialtoneReady)
				{	
					if(m_bKeepAliveKey)
						OnMixLocalPlayG7231(2, 1);
				}
				m_nSerialDialtoneCount = 0;
				m_dwSerialDialtoneState = SDIAL_PLAY_SWITCH;
			}	
			break;
		case 14:	// Send/Redial button click	
//			OnMixLocalPlayG7231(4, TRUE);							// Send Button Play
			break;
		case 15:	// Home Page		
			break;
		case 16:	// Phone Book List		
			break;
		case 17:	// Shopping Mall Homepage
			break;
		}
	}
	else if(lParam == 85)
	{
		if(m_bSerialSendDummyPacket == TRUE){ m_bSerialSendDummyPacket = FALSE;}
		m_TxQueue.Clear();
		if(!m_bSerialConnectSend)
		{
			if(m_bKeepAliveKey)
			{
				m_bSerialConnectSend = TRUE;
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
			}
		}
		if(!m_DTMFList.IsEmpty()){	m_DTMFList.RemoveAll();}

		if(m_bKeepAliveKey)
			SendMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 18);
		// Hook On
		if(m_bSerialBusytoneReady)
		{
			m_bSerialBusytoneReady	= FALSE;
			m_nSerialBusytoneCount	= 0;
			m_nSerialBusytoneSize	= 0;
		}
		if(m_dwSerialDialtoneState == SDIAL_PLAY_HOOK || m_dwSerialDialtoneState == SDIAL_PLAY_SWITCH 
			|| m_dwSerialDialtoneState == SDIAL_PLAY_RECV_DIAL)
		{			
			m_dwSerialDialtoneState = SDIAL_NO_PLAY;			
		}
	}
	else if(lParam == 170)
	{
		if(!m_bSerialConnectSend)
		{
			if(m_bKeepAliveKey)
			{
				m_bSerialConnectSend = TRUE;
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);			
			}

		}
		if(m_bKeepAliveKey)
		{
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 19);
		}
		// Hook Off	
		if(m_dwSerialDialtoneState == SDIAL_NO_PLAY)
		{
			if(!m_bSerialDialtoneReady)
			{	
				if(m_bKeepAliveKey)
				{
					OnMixLocalPlayG7231(2, 1);
				}
			}
			m_nSerialDialtoneCount = 0;
			m_dwSerialDialtoneState = SDIAL_PLAY_HOOK;
		}
		else if(m_dwSerialDialtoneState == SDIAL_PLAY_SWITCH)
		{
			m_dwSerialDialtoneState = SDIAL_PLAY_HOOK;					 
		}		
	}
	else if(lParam == m_nKeepAliveHookOn)
	{
		if(!m_bSerialConnectSend)
		{
				m_bKeepAliveKey = TRUE;
				m_bSerialConnectSend = TRUE;
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
		}
		if(m_bKeepAliveKey)
		{
			SendMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 20);			
		}		
				
		if(SDIAL_PLAY_SWITCH != m_dwSerialDialtoneState && SDIAL_PLAY_RECV_DIAL != m_dwSerialDialtoneState)
		{
			m_dwSerialDialtoneState = SDIAL_NO_PLAY;
		}
	}
	else if(lParam == m_nKeepAliveHookOff)
	{
		if(!m_bSerialConnectSend)
		{
			m_bKeepAliveKey = TRUE;
			m_bSerialConnectSend = TRUE;
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
		}
		if(m_bKeepAliveKey)
		{
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 21);
		}			
		if(m_dwSerialDialtoneState == SDIAL_NO_PLAY)
		{
			if(!m_bSerialDialtoneReady){	OnMixLocalPlayG7231(2, 1);}
			m_nSerialDialtoneCount = 0;
			m_dwSerialDialtoneState = SDIAL_PLAY_HOOK;
		}
	}
	else if(lParam == 22)
	{	
		if(m_bSerialSendDummyPacket == TRUE){ m_bSerialSendDummyPacket = FALSE;}
		if(!m_bSerialConnectSend)
		{
			if(m_bKeepAliveKey)
			{
				m_bSerialConnectSend = TRUE;
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);
			}
		}
		if(!m_DTMFList.IsEmpty()){	m_DTMFList.RemoveAll();}
		if(m_bKeepAliveKey)
		{
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 22);
		}

		if(m_bSerialBusytoneReady)
		{
			m_bSerialBusytoneReady	= FALSE;
			m_nSerialBusytoneCount	= 0;
			m_nSerialBusytoneSize	= 0;
		}
		if(m_dwSerialDialtoneState == SDIAL_PLAY_HOOK || m_dwSerialDialtoneState == SDIAL_PLAY_SWITCH 
			|| m_dwSerialDialtoneState == SDIAL_PLAY_RECV_DIAL)
		{			
			m_dwSerialDialtoneState = SDIAL_NO_PLAY;			
		}		
	}
	else if(lParam == 23 || lParam == 24){ PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, lParam);}
	else if(lParam == 30){	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 30);}
	else if(lParam == 31)
	{
		TRACE("\n			Serial Cable removed(5), DeviceState[%d], errCount[%d]", m_dwSerialDeviceState, m_dwSerialDeviceErrorCount);
		m_dwSerialDeviceGarbageCount = 3;
		m_bSerialConnectSend = FALSE; 
//		if(m_bKeepAliveKey)
//		{
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 31);
//		}

	}
	else if(lParam == 32){	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 32);}
	else if(lParam == 33){	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 33);}
	else	// serial device disconnect
	{
		/*
		TRACE("\n			Serial Cable removed(1), DeviceState[%d], errCount[%d]", m_dwSerialDeviceState, m_dwSerialDeviceErrorCount);
		if(m_dwSerialDeviceState == SDEVICE_READY)
		{
			m_dwSerialDeviceGarbageCount = 3;
			m_dwSerialDeviceErrorCount = 10;
			m_dwSerialDeviceState = SDEVICE_ERROR;
			PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 31);
			TRACE("\n			Serial Cable removed(2), DeviceState[%d], errCount[%d]", m_dwSerialDeviceState, m_dwSerialDeviceErrorCount);
		}
		//*/
		TRACE("\n			Unknown lParam[%d] in OnSerialSignal", lParam);
		for(int i = 0; i < m_arrayInvalidKeepAliveList.GetSize(); i++)
		{
			if(lParam == atoi(m_arrayInvalidKeepAliveList.GetAt(i)))
			{
				if(m_dwSerialDeviceState == SDEVICE_READY)
				{
					TRACE("\n			Invalid KeepAliveKey lParam[%d] in OnSerialSignal", lParam);
					m_dwSerialDeviceGarbageCount = 3;
					m_bSerialConnectSend = FALSE;
					m_bKeepAliveKey = FALSE;
					PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, 0, 52);
				}
			}
		}

	}		
	
	return TRUE;
}

/*
	Serial Device Signal ( lParam )
	Serial Device	:	Control
	0 - 12			|	0 - 12			==> DTMF
	13				|	13				==> Hook switch on
	14				|	14				==> Send/Redial
	15				|	15				==> Home Page
	16				|	16				==> Phone Book List
	17				|	17				==> Shopping Mall Homepage
	85				|	18				==> Hook On
	170				|	19				==> Hook Off	
	50				|	20				==> Alive Message from SerialPhone to PC(Hoon On State)
	51				|	21				==> Alive Message from SerialPhone to PC(Hoon Off State)
	22				|	22				==> Hook switch off
	없음			|	23				==> Serial device connect Undefine Message
	없음			|	30				==> Serial device connect OK
	없음			|	31				==> Serial device disconnect(통화 도중 Serial device 제거)
	없음			|	32				==> Serial device connect error(Port가 사용중입니다.)
	없음			|	33				==> Serial device connect error(Port가 유효하지 않습니다.)
	없음			|	34				==>	Received Dial Message(PCtoPC로 전화가 온경우)
	없음			|	35				==>	Received Hanup Message(PCtoPC로 전화가 와서 Ringing중에 전화를 건사람이 전화를 끊을때 처리를 위해)
	없음			|	36				==>	Received Busy Message(PCtoPC로 전화를 걸때 상대방이 통화중일때 Busy Tone을 발생하기 위해서)
	없음			|	37				==> Send/Redial 톤 cancel
	없음			|	52				==> Serial device is invalid (Device 가 invalid 하다.)
//*/
LRESULT CAudioSound::OnSerialMessage(WPARAM wParam, LPARAM lParam)
{
	if(!m_dwSerialDeviceState)
	{		
		switch(lParam)
		{
			case 36: // busy tone
			{
				TRACE("\nBusy tone of sound card\n");
				if(!p_bPlay)
				{
					OnPlayStart(0, 0);
				}
				OnMixLocalPlayG7231(5, TRUE);
				break;
			}
		}
		return TRUE;
	}
	switch(lParam)
	{
	case 13:	// Hook switch on	
		break;
	case 14:	// Send/Redial button click		
		break;
	case 15:	// Home Page		
		break;
	case 16:	// Phone Book List		
		break;
	case 17:	// Shopping Mall Homepage		
		break;
	case 18:	// Hook On	
		break;
	case 19:	// Hook Off	
		break;
	case 20:	// Alive Message from SerialPhone to PC(Hoon On State)		
		break;
	case 21:	// Alive Message from SerialPhone to PC(Hoon Off State)		
		break;
	case 22:	// Hook switch off		
		break;
	case 30:	// Serial device connect ok
		break;
	case 31:
		break;
	case 32:
		break;	
	case 33:
		break;
	case 34:
		// Received Dial
		if(m_dwSerialDialtoneState == SDIAL_NO_PLAY)
		{
			m_dwSerialDialtoneState = SDIAL_PLAY_RECV_DIAL;
		}
		TRACE("\n			OnSerialMessage[%d],   dwSerialDialtone[%d]", lParam, m_dwSerialDialtoneState);
		break;
	case 35:
		OnMixLocalPlayG7231(4, FALSE);
		if(m_dwSerialDialtoneState == SDIAL_PLAY_RECV_DIAL)
		{
			m_dwSerialDialtoneState = SDIAL_NO_PLAY;
		}
		break;
	case 36:
		{
			OnMixLocalPlayG7231(5, TRUE);
		}
		break;
	case 37:
		{
			OnMixLocalPlayG7231(4, FALSE);
		}
		break;
	}	
	
	return TRUE;
}
/*
 * linear2ulaw() - Convert a linear PCM value to u-law
 *
 * In order to simplify the encoding process, the original linear magnitude
 * is biased by adding 33 which shifts the encoding range from (0 - 8158) to
 * (33 - 8191). The result can be seen in the following encoding table:
 *
 *	Biased Linear Input Code	Compressed Code
 *	------------------------	---------------
 *	00000001wxyza			000wxyz
 *	0000001wxyzab			001wxyz
 *	000001wxyzabc			010wxyz
 *	00001wxyzabcd			011wxyz
 *	0001wxyzabcde			100wxyz
 *	001wxyzabcdef			101wxyz
 *	01wxyzabcdefg			110wxyz
 *	1wxyzabcdefgh			111wxyz
 *
 * Each biased linear code has a leading 1 which identifies the segment
 * number. The value of the segment number is equal to 7 minus the number
 * of leading 0's. The quantization interval is directly available as the
 * four bits wxyz.  * The trailing bits (a - h) are ignored.
 *
 * Ordinarily the complement of the resulting code word is used for
 * transmission, and so the code word is complemented before it is returned.
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char CAudioSound::linear2ulaw(int		pcm_val)	/* 2's complement (16-bit range) */
{
	int		mask;
	int		seg;
	unsigned char	uval;

	/* Get the sign and the magnitude of the value. */
	if (pcm_val < 0) {
		pcm_val = BIAS - pcm_val;
		mask = 0x7F;
	} else {
		pcm_val += BIAS;
		mask = 0xFF;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, 8);

	/*
	 * Combine the sign, segment, quantization bits;
	 * and complement the code word.
	 */
	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
		return (uval ^ mask);
	}
}

int CAudioSound::search(	int		val,	short		*table,	int		size)
{
	int		i;

	for (i = 0; i < size; i++) {
		if (val <= *table++)
			return (i);
	}
	return (size);
}


int CAudioSound::ulaw2linear( unsigned char	u_val)
{
	int		t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
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
	if(NULL == p_hPlay)
	{
		return FALSE;
	}

	DWORD dwVol = 0;
	if(MMSYSERR_NOERROR != waveOutGetVolume(p_hPlay, &dwVol))
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

	if(NULL == p_hPlay)
	{
		if(i_bApplyInstantly)
		{
			return FALSE;
		}

		m_nPlayVolumeOfPercent = i_byVolPercent;
		return TRUE;
	}
	

	DWORD dwVol			= (0xFFFF * i_byVolPercent) / 100;	// Percent 를 변환
	if(MMSYSERR_NOERROR != waveOutSetVolume(p_hPlay, MAKELONG(dwVol, dwVol)))
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


BOOL CAudioSound::InitializeSerialDevice()
{
	if(m_dwSerialDeviceState)
	{
		m_bSerialConnectSend = FALSE;
		if(m_DTMFList.GetCount()){	m_DTMFList.RemoveAll();}

		m_ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		InitializeCriticalSection(&m_SerialCritical);

		char	szPort[15];
		DWORD	dwCommEvents;
		wsprintf(szPort,"COM%d", m_nSerialPort);
		m_hSerialPort = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL
			, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL);
		TRACE("\nSerial Port Open : port[%d], return[%x]\n", m_nSerialPort, m_hSerialPort);
		LPVOID lpMsgBuf;
		if (!FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL ))
		{
			// Handle the error.
			return FALSE;
		}
		
		// Process any inserts in lpMsgBuf.
		// ...
		
		// Display the string.
		TRACE("\n\tError Code = %s\n",(LPCTSTR)lpMsgBuf);
		
		// Free the buffer.
		LocalFree( lpMsgBuf );

		// application 에 serial log 를 보내주기 위한 코드
		// 2003.06.25 by gomeisa
		PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, m_nSerialPort, 10001);
		if(m_hSerialPort != INVALID_HANDLE_VALUE)
		{			
			// set the timeout values -----
			COMMTIMEOUTS CommTimeouts;

			GetCommTimeouts(m_hSerialPort, &CommTimeouts);
			CommTimeouts.ReadIntervalTimeout = 20;
			CommTimeouts.ReadTotalTimeoutMultiplier = 20;
			CommTimeouts.ReadTotalTimeoutConstant = 20;
			CommTimeouts.WriteTotalTimeoutMultiplier = 100;
			CommTimeouts.WriteTotalTimeoutConstant = 100;
			
			if(SetCommTimeouts(m_hSerialPort, &CommTimeouts))
			{
				//set the comm events -----
				dwCommEvents =  EV_ERR;
				if(SetCommMask (m_hSerialPort, dwCommEvents) )
				{					
					SetupComm(m_hSerialPort, 1024, 1024);
					//set the DCB -----
					DCB dcb;
					char szDcb[50];
					if(GetCommState(m_hSerialPort, &dcb)) 
					{					
						sprintf(szDcb, "baud=%d parity=%c data=%d stop=%d"
							, 115200, 's', 8, 1);
						
						BuildCommDCB(szDcb,& dcb);
						SetCommState( m_hSerialPort, &dcb);						
					}
					
					//flush the port
					PurgeComm(m_hSerialPort, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
					
					OnSerialRead(0, 0);							// Read Buffer Ready					
					OnMixStart(0, 0);							// Multimedia Timer Start(for Dialtone)					
					m_tmSerialAlive = timeGetTime();			// Serial Device Alive Message time 
					m_dwSerialDeviceState	= SDEVICE_READY;	// Serial Device Ready State
					//OnSerialSignal(0, 30);						// Serial device connect OK
				}
			}
		}
		else 
			// 시리얼 폰으로 동작을 요청하였지만 Open시에 오류가 발생함.
			// Run을 Return 함으로 다시 시작하려면 프로그램을 다시 시작해야한다.
		{
			DWORD dwErr = GetLastError();
			if(dwErr == ERROR_ACCESS_DENIED)
			{
				OnSerialSignal(0, 32);
			}
			else if(dwErr == ERROR_FILE_NOT_FOUND)
			{
				OnSerialSignal(0, 33);
			}
			else
			{
//				DbgOut("\n Serial Device Open Error[%d]", GetLastError());
				// application 에 serial log 를 보내주기 위한 코드
				PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, m_nSerialPort, 10002);
			}			
			m_dwSerialDeviceState	= SDEVICE_ERROR;	// Serial Device Ready State
			return FALSE;
		}
	}	// End of Serial

	m_hSerialReadThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SerialReadThreadFunction, 
						this, 0, &m_IDSerialReadThread);

	return TRUE;
}

VOID SerialReadThreadFunction(LPVOID lpParam)
{	
	CAudioSound* pSound = (CAudioSound*)lpParam;
	
	DWORD		dwRead	= 0;													// Serial Port로 부터 읽은 바이트수	
	DWORD		dwRet	= 0;													// WaitForMultipleObjects의 Return value
	OVERLAPPED	*povRead = &pSound->m_ovRead;
	HANDLE		hReadEvent = povRead->hEvent;
	HANDLE		hSerialPort = pSound->m_hSerialPort;
	BOOL		*bEndFlag = &pSound->m_bSerialReadThreadEnd;

	if(!pSound->m_dwSerialDeviceState || hSerialPort == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while(*bEndFlag == FALSE)
	{
		dwRead = 0;		
		dwRet = WaitForSingleObject(hReadEvent, 100);
		switch(dwRet)
		{
		case WAIT_OBJECT_0:
			{
				if(GetOverlappedResult(hSerialPort, povRead, &dwRead, FALSE))
				{						
					if(dwRead)
					{
						pSound->OnSerialRead(0, dwRead);
					}
					else
					{
						pSound->OnSerialRead(0, dwRead);
						Sleep(1);
					}
				}
				else
				{
					TRACE("\n GetOverlappedResult Error[%d]", GetLastError());
				}					
			}
			break;			
		case WAIT_TIMEOUT:
			{
			}
			break;
		default:
			{
			}
		}	
	}	
}

void CAudioSound::ShowVolumeConfig(short nMode)
{
	// nMode : 0 - spk,  1 - mic

	CString strCommand;
	if(nMode)
		strCommand.Format("sndvol32.exe /d %d /r", r_nDeviceIndex);
	else
		strCommand.Format("sndvol32.exe /d %d", p_nDeviceIndex);

	WinExec(strCommand, SW_SHOWNORMAL);	
}

void CAudioSound::OnLogRTPDataRead(WPARAM wParam, LPARAM lParam)
{
	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, wParam, lParam);
}

void CAudioSound::OnLogRTPDataWrite(WPARAM wParam, LPARAM lParam)
{
	PostMessage(m_hCtrl, WM_SERIAL_SIGNAL, wParam, lParam);
}

#ifdef _ECHO_CANCELLER
void CAudioSound::InitEchoCanceller(BOOL bFlag, int nOffset)
{
	LockEchoCanceller();
	if(m_bApplyEchoCancellerFlag)
	{
		m_bApplyEchoCancellerFlag	= FALSE;
		EndEchoCanceller();
	}
	
	m_bApplyEchoCancellerFlag = bFlag;
	if(m_bApplyEchoCancellerFlag)
	{
		Offset = nOffset;		
	}
	UnlockEchoCanceller();
}
void CAudioSound::StartEchoCanceller(void)
{
	if(m_bStartEchoCancellerFlag){	return;}

	UseLec = ON;
	initlec();
	m_bStartEchoCancellerFlag = TRUE;
}

void CAudioSound::EndEchoCanceller(void)
{
	m_bStartEchoCancellerFlag = FALSE;
	UseLec = OFF;
	initlec();
}

void CAudioSound::LockEchoCanceller(void)
{
	EnterCriticalSection(&m_csEchoCanceller);
}

void CAudioSound::UnlockEchoCanceller(void)
{
	LeaveCriticalSection(&m_csEchoCanceller);
}
#endif


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
