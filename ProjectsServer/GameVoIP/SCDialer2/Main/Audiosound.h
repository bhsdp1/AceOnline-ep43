#if !defined(AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_)
#define AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// audiosound.h : header file
//

#include <mmsystem.h>
#include <afxmt.h>
#include <afxtempl.h>
#include "AudioCodec.h"
#include "SystemLogManager.h"	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 

/////////////////////////////////////////////////////////////////////////////
// CAudioSound thread
#define WM_AUDIO_ENDTHREAD				WM_USER+200

// Recording Message
#define WM_RECORD_STOP					WM_AUDIO_ENDTHREAD+12
#define WM_RECORD_SENDDTMF				WM_AUDIO_ENDTHREAD+14
#define WM_RECORD_SENDMUTEAUDIORTP		WM_AUDIO_ENDTHREAD+15
#define WM_RECORD_RECORDINGFILEPOINTER	WM_AUDIO_ENDTHREAD+16

// Playing Message
#define WM_PLAY_STOP					WM_AUDIO_ENDTHREAD+21
#define WM_PLAY_PLAYBLOCK				WM_AUDIO_ENDTHREAD+22
#define WM_PLAY_RESET					WM_AUDIO_ENDTHREAD+23

/////////////////////////////////////////////////////////////////////////////
// CMixSound thread
#define WM_MIX_FORMRTPTOBUFFER			WM_AUDIO_ENDTHREAD+32
#define WM_MIX_PLAY						WM_AUDIO_ENDTHREAD+33
#define WM_MIX_ENDRTPSESSION			WM_AUDIO_ENDTHREAD+34
#define WM_MIX_LOCALPLAYG7231			WM_AUDIO_ENDTHREAD+35

#define LOCALPLAYG7231_BUFFERSIZE		3024    // 24 *  83 = 1992	ringback tone
												// 24 * 126 = 3024	안내음성


const UINT		MULTIMEDIA_TIMER_TIME			= 15;		// 240 sample
const int		SIZE_ENCODED_BUFFER				= 256;
const int		SIZE_DTMF_BUFFER				= 120;

#define INVALID_SPK_VOLUME_VALUE	-1		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 

class CChannelData;
class CAVManager;
class CAudioSound : public CWinThread
{
	DECLARE_DYNCREATE(CAudioSound)
private:
	CAVManager			*m_pAVMaster;
	DWORD				m_dwLastTick;

	int					m_nFrameBytes;				// 
	int					m_nPacketBytes;	
	HWAVEIN				m_hRecord;
	BOOL				m_bFlagRecording;
	WAVEFORMATEX		m_WaveFormatEx;
	int					m_nRecordMaxInputBuffers;
	MediaType			m_AudioPayloadType;								// Audio Codec Type
	CAudioCodec*		m_pEncoder;										// waveOut data Encoder
	BYTE				m_pEncodedBuffer[SIZE_ENCODED_BUFFER];			// audio encoded buffer
	int					m_nEncodedCount;								// audio encoded count
	int					r_nSendEncodedCount;							// rtp send encoded count
	int					r_nDeviceIndex;									// waveOut device index
	UINT				r_nAvailableCount;								// Get Driver Count
	
	// Playing 관련 변수
	HWAVEOUT			m_hPlay;
	BOOL				m_bFlagPlaying;
	DWORD				m_dwPlayingBytes;
	int					p_nDeviceIndex;									// waveIn device index
	UINT				p_nAvailableCount;								// Get Driver Count	

	UINT				b_nTimer_id;									// Multimedia timer id	
	int					b_nBufferPlay_Start_Count;
	int					b_nBufferPlay_Reset_Count;
	
	inline void MixingOperation(short* src1, short* src2, short* des, int shortSize);
	
public:
	CAudioSound(CAVManager *pAVMas, MediaType audioPayload, int nFrameCount, int JitterBufferSize);           

	LPWAVEHDR CreateWaveHeader(int size);
	BOOL SetRecordSoundDevice(CString strDevice);
	BOOL SetPlaySoundDevice(CString strDevice);

	BOOL GetFlagPlaying(void){					return m_bFlagPlaying;}

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
	void SetSilenceState(BOOL i_bSilenceState);		// 2008-10-21 by cmkwon, SCDialer2 수정 - 
	
// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
//	///////////////////////////////////////////////////////////////////////////////	
//	// 2008-07-15 by cmkwon, 로그을 남기기 위해서 - 
	static CSystemLogManager	ms_SystemLogManager;
	static BOOL WriteSystemLogEX(BOOL bPrintDBGOUT, const char* pFormat, ...);
	

protected:
	CAudioSound();           // protected constructor used by dynamic creation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAudioSound)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnAudioEndThread(WPARAM wParam, LPARAM lParam);
	
	// Recording
	BOOL RecordStart(void);
	afx_msg LRESULT OnRecordStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecordSoundData(WPARAM wParam, LPARAM lParam);	
	
	// Playing
	BOOL PlayStart(void);
	afx_msg LRESULT OnPlayStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayEndSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayWriteSoundData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayReset(WPARAM wParam, LPARAM lParam);	

	// buffering
	BOOL MixTimerStart(void);
	void MixTimerStop(void);
	afx_msg LRESULT	OnMixPlay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT	OnMixEndRTPSession(WPARAM wParam, LPARAM lParam);	
	
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


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDIOSOUND_H__89ED033B_D214_45E2_82A6_596827D7B10C__INCLUDED_)
