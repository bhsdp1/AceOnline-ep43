// H323Connection.h: interface for the CH323Connection class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_H323CONNECTION_H__F20BD5BF_5AF8_40E2_A028_3C301488B941__INCLUDED_)
#define AFX_H323CONNECTION_H__F20BD5BF_5AF8_40E2_A028_3C301488B941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*///////////////////////////////////////////////////////////////////////
 		CH323Connection class
 
  1. Project->Settings->C/C++->Project Options에 다음 include를 추가한다. 
 
 		/I "." /I "./RAD_RTP" /I "./RAD_RTP/include"
 
 
  2. 필히 Project->Settings->C/C++->Preprocessor Definitions 위치에 
     선언해야 하는 것

	 1) Audio Codec Type     
 			Audio Codec G.723.1 ==>  _G7231CODEC
 						G.729   ==>  _G729CODEC

  
  3. 필요시 Project->Settings->C/C++->Preprocessor Definitions 위치에 
     선언하여 사용할수 있는 기능

     1) _LOCAL_PLAY_G7231
		- Ringback tone, 인사말을 Play하는데 사용한다.
	 
	 2) _RECORDING_FILE
		- 1:1 음성 통화를 화일로 저장하기 위해 사용
		
      
 ///////////////////////////////////////////////////////////////////////////////*/

#include "AudioSound.h"
#include "RTPCtrlManager.h"
#include "INCLUDE\rvcommon.h"	// Added by ClassView

class CSCDialer1Ctrl;			// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
class CH323Connection  
{
public:
	typedef struct
	{
		unsigned RecordStart:2;
		unsigned PlayStart:2;
		unsigned MixStart:2;
		unsigned InbandDTMF:2;
	} BitFlag;
public:

	// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 인자추가(CSCDialer1Ctrl *i_pDialer1)
	CH323Connection(CSCDialer1Ctrl *i_pDialer1, MediaType audioPayload, MediaType videoPayload, BOOL InbandDTMF, int nFrameCount, int JitterBufferSize
		, HWND hCtrl, BOOL bSerial = FALSE, int nSerialPort = 0, int nTermperSecond = -5);
	virtual ~CH323Connection();

public:	
	// log
	BOOL LogPlayStop();
	BOOL LogPlayStart();
	BOOL LogRecordStop();
	BOOL LogRecordStart();
	void LogEnable();
	void LogDisable();

	void SetBusyTone(BOOL bUse);
	int ParseData(CString str, char sep, CStringArray *array);
	void SetKeepAliveSerial(CString strAliveKeyList);
	void ShowVolumeConfig(short nMode);
	void SetInBandDTMF(BOOL bInBand);
	BOOL CreateRTPSession(UINT Port, MediaType ty, int sNum );
	BOOL StartRTPSession(char* ip, UINT Port, MediaType ty, int sNum);
	BOOL EndRTPSession(MediaType ty, int sNum );
	int	EndAllRTPSession();
	UINT GetLocalAudioPort(int sNum);
	UINT GetLocalPort(MediaType ty, int sNum);
	UINT GetLocalVideoPort(int sNum);
	BOOL SetRecordSoundDevice(CString strDevice);
	BOOL SetPlaySoundDevice(CString strDevice);
	CString CheckAudioDevice();

	int		SendVideoData(BYTE* Data, DWORD dwSize);
	BOOL	SetVideoDecoder(void* de);
	DWORD	AudioVolume(BOOL flag, int value);
	void	SendMuteAudioRTP(BOOL bIN);
	void	SendDTMF(const char* number);

	void	SerialPhoneMessage(DWORD dwMsg);

#ifdef _ECHO_CANCELLER
	void ApplyEchoCanceller(BOOL bFlag, int nOffset);
#endif	
#ifdef	_LOCAL_PLAY_G7231
	void	LocalPlayG7231(int nFlag, BOOL bIN);
#endif

#ifdef	_RECORDING_FILE
	void	SetRecordingFilePointer(FILE* pfA, FILE* pfC);
	void	PlayRecordingFile(FILE* pfA, FILE* pfC);
#endif

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	BOOL GetPlayVolumeW(BYTE *o_pbyVolPercent);
	BOOL SetPlayVolumeW(BYTE i_byVolPercent);

	void OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent);		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 


private:
	void _Initialize(int nFrameCount, int JitterBufferSize, HWND hCtrl, BOOL bSerial, int nSerialPort, int nTermperSecond);

	CRTPCtrlManager*	m_pRTPCtrlManager;
	CAudioSound*		m_pAudioSound;
	MediaType			m_ConnType;
	BitFlag				m_bitFlag;

	CSCDialer1Ctrl		*m_pSCDialer1Ctrl;		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
};

#endif // !defined(AFX_H323CONNECTION_H__F20BD5BF_5AF8_40E2_A028_3C301488B941__INCLUDED_)
