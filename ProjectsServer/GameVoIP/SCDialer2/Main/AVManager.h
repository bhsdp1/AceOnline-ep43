// AVManager.h: interface for the CAVManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVMASTER_H__4FBBB41D_D949_413D_B667_F174B44FA276__INCLUDED_)
#define AFX_AVMASTER_H__4FBBB41D_D949_413D_B667_F174B44FA276__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VoIPParam.h"

class CChannelData;
class CAudioSound;
class CS2CWinSocket;
class CSCDialer2Ctrl;
class SJittData;

class CAVManager
{
public:
	CAVManager(CSCDialer2Ctrl *i_pCtrl, CS2CWinSocket *i_pS2CWinSocket, DWORD dwChannSize, int audioPayload, int nFrameCount, int JitterBufferSize);
	virtual ~CAVManager();

	void ResetAVMaster(void);

	void SetRoomID(RoomID_t i_roomID){			m_idRoomID = i_roomID;}
	RoomID_t GetRoomID(void){					return m_idRoomID;}
	void SetUserName(char *i_szUserName){		strncpy(m_szUserName, i_szUserName, SIZE_MAX_USER_NAME);}
	char *GetUserName(void){					return m_szUserName;}
	void SetAudioSendPauseFlag(BOOL i_bFlag);
	BOOL GetAudioSendPauseFlag(void){			return m_bAudioSendPauseFlag;}
	void SetConcurrentTalkingCount(int i_nCount);
	int GetConcurrentTalkingCount(void);

	
	void SetSelfChannelIndex(DWORD idx){		m_dwSelfChannelIndex = idx;}
	DWORD GetSelfChannelIndex(void){			return m_dwSelfChannelIndex;};
	DWORD GetChannelCounts(void){				return m_dwCountsChannel;}
	BOOL GetPlayStartFlags(DWORD idx);
	void SetPlayStartFlags(DWORD idx, BOOL bFlag);
	DWORD GetDataCounts(DWORD idx);	
	SJittData* GetDataFromBuffers(DWORD idx);
	void SetAndSendAudioSendPauseFlag(BOOL i_bFlag);

	void AudioStart(void);
	void AudioStop(void);
	void AudioEnd(void);

	int SendRTPData(BYTE *pData, int nLen, int media, BOOL bVideoKeyFrame = FALSE);
	BOOL RecvRTPData(DWORD dwChannIdx, char *pData, int nLen, int media, BOOL bVideoKeyFrame = FALSE);
	BOOL RecvRTPData(DWORD dwChannIdx, BYTE *pData, int nLen, int media, BOOL bVideoKeyFrame = FALSE);	

	DWORD OtherJoinedRoom(char *i_szUserName, DWORD i_dwChannIdx);
	DWORD OtherGoOutedRoom(DWORD dwChannIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
	BOOL GetPlayVolumeW(BYTE *o_pbyVolPercent);
	BOOL SetPlayVolumeW(BYTE i_byVolPercent);

	void OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent);		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 

	// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
	BOOL SendChangedTalkingState(TALKING_STATE_t i_talkingState);

protected:
	void AudioPlayStop(void);
	void AudioRecordStop(void);

protected:
	DWORD				m_dwCountsChannel;	
	DWORD				m_dwCountsUsingChannel;
	DWORD				m_dwSelfChannelIndex;
	CChannelData		*m_pChannelData;

	CAudioSound			*m_pAudioSound;
	int					m_nJitterBufferSize;

	CSCDialer2Ctrl		*m_pSCDialer2Ctrl;
	CS2CWinSocket		*m_pS2CWinSocket2;
	BYTE				m_bAudioSendPauseFlag;

	RoomID_t			m_idRoomID;
	int					m_nConcurrentTalkingCount;		// 동시 말하기 가능 카운트, 2005-10-04 by cmkwon
	char				m_szUserName[SIZE_MAX_USER_NAME];
};

#endif // !defined(AFX_AVMASTER_H__4FBBB41D_D949_413D_B667_F174B44FA276__INCLUDED_)
