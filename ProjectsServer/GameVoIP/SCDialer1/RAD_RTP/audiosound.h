#if !defined(AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_)
#define AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// audiosound.h : header file
//

#include <mmsystem.h>
#include <rtp.h>
#include <payload.h>
#include "DataQueue.h"
#include "AudioSound.h"
#include "AudioCodec.h"
#include "RTPCtrlManager.h"
#include "Queue.h"
#include "INCLUDE\rvcommon.h"	// Added by ClassView
#include "SystemLogManager.h"	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 


/////////////////////////////////////////////////////////////////////////////
// CAudioSound thread
#define WM_AUDIO_ENDTHREAD				WM_USER+100

// Recording Message
#define WM_RECORD_START					WM_AUDIO_ENDTHREAD+11
#define WM_RECORD_STOP					WM_AUDIO_ENDTHREAD+12
#define WM_RECORD_SENDDTMF				WM_AUDIO_ENDTHREAD+14
#define WM_RECORD_SENDMUTEAUDIORTP		WM_AUDIO_ENDTHREAD+15
#define WM_RECORD_RECORDINGFILEPOINTER	WM_AUDIO_ENDTHREAD+16

// Playing Message
#define WM_PLAY_START					WM_AUDIO_ENDTHREAD+20
#define WM_PLAY_STOP					WM_AUDIO_ENDTHREAD+21
#define WM_PLAY_PLAYBLOCK				WM_AUDIO_ENDTHREAD+22
#define WM_PLAY_RESET					WM_AUDIO_ENDTHREAD+23

/////////////////////////////////////////////////////////////////////////////
// CMixSound thread
#define WM_MIX_START					WM_AUDIO_ENDTHREAD+30
#define WM_MIX_STOP						WM_AUDIO_ENDTHREAD+31
#define WM_MIX_FORMRTPTOBUFFER			WM_AUDIO_ENDTHREAD+32
#define WM_MIX_PLAY						WM_AUDIO_ENDTHREAD+33
#define WM_MIX_ENDRTPSESSION			WM_AUDIO_ENDTHREAD+34
#define WM_MIX_LOCALPLAYG7231			WM_AUDIO_ENDTHREAD+35

/////////////////////////////////////////////////////////////////////////////
// Serial Phone
#define WM_SERIAL_READ					WM_AUDIO_ENDTHREAD+40
#define WM_SERIAL_WRITE					WM_AUDIO_ENDTHREAD+41
#define WM_SERIAL_EVENT					WM_AUDIO_ENDTHREAD+42
#define WM_SERIAL_SIGNAL				WM_AUDIO_ENDTHREAD+43
#define WM_SERIAL_MESSAGE				WM_AUDIO_ENDTHREAD+44			// from Application

/////////////////////////////////////////////////////////////////////////////
// log
#define WM_RTPDATA_READ					WM_AUDIO_ENDTHREAD+45
#define WM_RTPDATA_WRITE				WM_AUDIO_ENDTHREAD+46

#define MAX_SERIAL_BUFFER_SIZE			240

#define LOCALPLAYG7231_BUFFERSIZE		3696    // 24 * 126 = 3024	안내음성
												// 24 *  83 = 1992	ringback tone
												// 24 * 133 = 3192	dialtone
												// 24 *   5 = 120	DTMF

#ifdef _G7231CODEC								// MixSound.h
#define MULTIMEDIA_TIMER_TIME			5		// 240 sample
#endif

#ifdef _G729CODEC								// MixSound.h
#define MULTIMEDIA_TIMER_TIME			10		//  80 sample
#define WAVEOUT_BUFFER_SIZE				160		// byte
#endif

enum 
{
	SDEVICE_NOT_USE		= 0,		// Serial Phone을 사용하지 안음(사운드 카드이용)
	SDEVICE_USE			= 1,		// Serial Phone을 사용함
	SDEVICE_READY		= 2,		// Serial Phone을 사용할 수 있음(Ready)
	SDEVICE_ERROR		= 3,		// Serial Phone Error State
	SDIAL_NO_PLAY		= 0,		// Serial Phone Dialtone 초기값
	SDIAL_PLAY_HOOK		= 1,		// Serial Phone Dialtone Play(Hook Off)
	SDIAL_PLAY_SWITCH	= 2,		// Serial Phone Dialtone Play(Hook Switch On)
	SDIAL_PLAY_DAILING	= 3,		// Serial Phone Dialtone Play(Dialing)
	SDIAL_PLAY_RECV_DIAL= 4,		// Serial Phone Dialtone Play(Received Dial)
	SDIAL_ERROR			= 5			// Serial Phone Dialtone Error State
};


#define INVALID_SPK_VOLUME_VALUE	-1		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 

class CRTPCtrlManager;
class CH323Connection;		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 

class CAudioSound : public CWinThread
{
	DECLARE_DYNCREATE(CAudioSound)
public:	
	DWORD				m_dwSerialDeviceGarbageCount;
	DWORD				m_dwSerialDeviceErrorCount;
	DWORD				m_dwSerialDeviceState;	// CAudioSound의 Device가 Serial인지의 플래그
												// 0	==> Serial Device not use
												// 1	==> Serial Device use
												// 2	==> Serial Device Ready
												// 3	==> Serial Device Error
													
	DWORD				m_dwSerialDialtoneState;// Serial Phone Dialtone Stat
												// 0	==> 초기값
												// 1	==> Dialtone Play(Hook Off stat)
												// 2	==> Dialtone Play(Hook switch stat);
												// 3	==> Dialtone no Play(serial device disconnect

	OVERLAPPED			m_ovWrite;				// Serial device에 데이타를 쓸때 사용되는 오버랩드구조체

	BOOL				m_bSerialWriteWaitThreadEnd;
	DWORD				m_tmSerialAlive;		// Serial Phone Alive Message Check variable
	CQueue				m_TxQueue;				// Serial device에 데이타를 쓰기전에 사용하는 버퍼
	CQueue				m_RxQueue;				// Serial device에서 데이타를 읽어서 저장하는 버퍼
	
	int					m_nSerialPort;			// Serial device를 사용할경우 사용할 com port
	HANDLE				m_hSerialPort;			// Serial device를 open한 핸들
	OVERLAPPED			m_ovRead;				// Serial device에서 데이타를 읽을때 사용되는 오버랩드구조체	
	BYTE				m_bfSerialRead[MAX_SERIAL_BUFFER_SIZE];
	BYTE				m_bfSerialWrite[MAX_SERIAL_BUFFER_SIZE * 2];
	CRITICAL_SECTION	m_SerialCritical; 
		
	BOOL				m_bSignalContinue;		// DTMF, Hook-On, Hook-Off를 인식하기 위한 플래그, 마지막에 시그널이 온경우 다음의 처음 데이타가 시그털 값이다.
	HWND				m_hCtrl;				// Serial Port로 부터 받은 시그널을 컨트롤에 보내기 위한 
	CList<DWORD, DWORD>	m_DTMFList;
	BOOL				m_bSerialConnectSend;
	
	BOOL				m_bSerialTimeOutTone;	// 10초 남았다는 소리를 들려주기 위한것.
	

	BOOL				m_bSerialSendDummyPacket;
	DWORD				m_dwLastTimeSendDummyPakcet;
	
	// Serial Phone Dialtone Play
	BOOL				m_bSerialDialtoneReady;		
	int					m_nSerialDialtoneCount;
	int					m_nSerialDialtoneSize;
	BYTE				m_SerialDialtoneBuffer[LOCALPLAYG7231_BUFFERSIZE * 5];
	
	// Serial Busy tone Play
	BOOL				m_bSerialBusytoneReady;		
	int					m_nSerialBusytoneCount;
	int					m_nSerialBusytoneSize;
	BYTE				m_SerialBusytoneBuffer[LOCALPLAYG7231_BUFFERSIZE];

	OSVERSIONINFO		m_osVersion;											// OS 버젼 체크에 필요

	HANDLE				m_hSerialReadThread;									// 시리얼 디바이스에서 데이타를 읽을 스레드 핸들
	DWORD				m_IDSerialReadThread;
	BOOL				m_bSerialReadThreadEnd;									// 종료 플래그

	BOOL InitializeSerialDevice();


	
	/*
	// cmkwon 20020517 에코 샘플 만들기
	FILE*				m_pfRef;
	FILE*				m_pfEcho;
	FILE*				m_pfOut;
	int					m_nEchoCount;
	BYTE				m_byEchoTemp[480];
	//*/
#ifdef _ECHO_CANCELLER	
	BOOL				m_bStartEchoCancellerFlag;
	BOOL				m_bApplyEchoCancellerFlag;
	CRITICAL_SECTION	m_csEchoCanceller;
	short				m_pEchoCancelBuffer[480];

	void InitEchoCanceller(BOOL bFlag, int nOffset);
	void StartEchoCanceller(void);
	void EndEchoCanceller(void);
	void LockEchoCanceller(void);
	void UnlockEchoCanceller(void);
#endif

	int					m_nFrameByte;
	int					m_nPacketByte;
	
	// Recording 관련 변수
	HWAVEIN				r_hRecord;
	BOOL				r_bRecording;
	WAVEFORMATEX		r_WaveFormatEx; 	
	int					r_nMaxInputBuffers;		
	int					r_nInBufferSize;
	CRTPCtrlManager*	r_pRTPCtrlManager;			// Audio RTP Send
	MediaType			r_AudioPayloadType;			// Audio Codec Type
	CAudioCodec*		r_pEncoder;					// waveOut data Encoder
	BYTE				r_pEncodedBuffer[24*5];		// audio encoded buffer
	int					r_nEncodedCount;			// audio encoded count
	int					r_nSendEncodedCount;		// rtp send encoded count
	BOOL				r_bSendMuteAudioRTP;		// 묵음을 보내는 flag
	BYTE				r_byDTMFBuffer[240];		// DTMF member variable
	BOOL				r_bDTMFSendFlag;			// Inbound DTMF 플래그
	int					r_nDTMFSendCount;			// Inbound DTMF 전송시 전송 패캣 count	
	int					r_nDeviceIndex;				// waveOut device index
	int					r_nAvailableCount;			// Get Driver Count
	
//private:
	// Playing 관련 변수
	HWAVEOUT			p_hPlay;
	BOOL				p_bPlay;
	WAVEFORMATEX		p_WaveFormatEx;	
	DWORD				p_nWriteCount;
	DWORD				p_dwWriteNotSkipCount;
	int					p_nDeviceIndex;				// waveIn device index
	int					p_nAvailableCount;			// Get Driver Count
	
//private:
	// buffering	
	CDataQueue*			b_pPlayBufferArray[CRTPCTRLARRAYSIZE];	// Play Buffer Array	
	BOOL				b_bPlayStartFlag[CRTPCTRLARRAYSIZE];	// Buffer Play Start Flag ( Start when buffer count if 5)
	CAudioCodec*		b_arrDecoder[CRTPCTRLARRAYSIZE];		// Audio Decoder Array

	BOOL				b_bSetTimerFlag;						// Timer Flag
	UINT				b_nTimer_id;							// Multimedia timer id
	
	int					b_nBufferPlay_Start_Count;
	int					b_nBufferPlay_Reset_Count;

	//////////////////////////////////////////////////////////////////
	// Local Play G.7231 ( Ring-Back tone, 안내음성 )
	int					b_nPlayG7231Value;
	BOOL				b_bPlayG7231Ready;
	BOOL				b_bPlayG7231Looped;
	int					b_nPlayG7231Count;
	int					b_nPlayG7231Size;
	BYTE				b_PlayG7231Buffer[LOCALPLAYG7231_BUFFERSIZE];
	CAudioCodec*		b_pPlayG7231Decoder;

	void MixingOperation(short* src1, short* src2, short* des, int shortSize);
	void Change8to16(int sourceBytes, BYTE* source, short* dest);
	void Change16to8(int sourceBytes, short* source, BYTE* dest);

	// keep alive
	BOOL m_bKeepAliveKey;
	int m_nKeepAliveHookOff;
	int m_nKeepAliveHookOn;
	CStringArray m_arrayInvalidKeepAliveList;

// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
	static CSystemLogManager	ms_SystemLogManager;
	static BOOL WriteSystemLogEX(BOOL bPrintDBGOUT, const char* pFormat, ...);

public:

	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 인자추가(, CH323Connection *i_p323Conn)
	CAudioSound(int nFrameCount, int JitterBufferSize, HWND hCtrl, BOOL bSerial, int nSerialPort, int nTermperSecond, CH323Connection *i_p323Conn);           

	LPWAVEHDR CreateWaveHeader(int size);
	BOOL SetRecordSoundDevice(CString strDevice);
	BOOL SetPlaySoundDevice(CString strDevice);

	unsigned char linear2ulaw(int pcm_val);
	static int search(int val, short *table, int size);
	static short seg_end[];
	int ulaw2linear( unsigned char	u_val);


	// buffering
//	int					b_nPlayCount;
	BOOL				m_bMMTimer;					// Audio Play에 Multi Media Timer를 사용 ==> Recording을 안하는 동안 사용

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	BOOL GetPlayVolume(BYTE *o_pbyVolPercent);
	BOOL SetPlayVolume(BYTE i_byVolPercent, BOOL i_bApplyInstantly=FALSE);
	int					m_nPlayVolumeOfPercent;		// INVALID_SPK_VOLUME_VALUE 이면 적용이 필요 없고 INVALID_SPK_VOLUME_VALUE 이 아닌 유효한 값이면 적용하고 INVALID_SPK_VOLUME_VALUE 로 수정한다.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-30 by cmkwon, VoIP 시스템 수정 - 
	int							m_nSkipFrameCnt;
	int							m_nThresholdLevel;
	BOOL						m_bSilenceState;
	int							m_nSilenceFrameCnt;
	int GetAverageSignalLevel(SHORT *i_sSampleBuffer, int i_nSampleCount);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
	CH323Connection				*m_pH323Conn;			



protected:
	CAudioSound();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	BOOL m_bLogWriteFile;
	CFile* m_pRawFile;

	// log
	long m_lRxSerialData, m_lTxSerialData, m_lRxRTPData, m_lTxRTPData;
	BOOL m_bSerialTestMode;

	void ShowVolumeConfig(short nMode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioSound)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnAudioEndThread(WPARAM wParam, LPARAM lParam);
	
	// Recording
	afx_msg LRESULT OnRecordStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecordStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecordSoundData(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT	OnRecordSendDTMF(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnRecordSetMuteAudioRTP(WPARAM wParam, LPARAM lParam);
	
	// Playing
	afx_msg LRESULT OnPlayStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayEndSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayReset(WPARAM wParam, LPARAM lParam);	

	// buffering
	afx_msg LRESULT	OnMixStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMixStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMixFromRTPtoBuffer(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMixPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMixEndRTPSession(WPARAM wParam, LPARAM lParam);	
	afx_msg LRESULT OnMixLocalPlayG7231(WPARAM wParam, LPARAM lParam);

	// Serial
	afx_msg LRESULT OnSerialRead(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSerialWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSerialEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSerialSignal(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSerialMessage(WPARAM wParam, LPARAM lParam);

	// log
	afx_msg void OnLogRTPDataRead(WPARAM wParam, LPARAM lParam); // 10003
	afx_msg void OnLogRTPDataWrite(WPARAM wParam, LPARAM lParam); // 10004
// Implementation
protected:
	virtual ~CAudioSound();

	// Generated message map functions
	//{{AFX_MSG(CAudioSound)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_)
