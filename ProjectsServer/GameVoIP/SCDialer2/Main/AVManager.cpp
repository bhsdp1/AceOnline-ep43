// AVManager.cpp: implementation of the CAVManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AVManager.h"
#include "ChannelData.h"
#include "AudioSound.h"
#include "S2CWinSocket.h"
#include "SCDialer2Ctl.h"
#include "VoIPProtocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CAVManager::CAVManager(CSCDialer2Ctrl *i_pCtrl, CS2CWinSocket *i_pS2CWinSocket, DWORD dwChannSize, int audioPayload, int nFrameCount, int JitterBufferSize)
{
	m_pSCDialer2Ctrl		= i_pCtrl;
	m_pS2CWinSocket2		= i_pS2CWinSocket;	
	m_dwCountsChannel		= dwChannSize - 1;						// 자신의 채널을 빼고 생성
	m_nJitterBufferSize		= JitterBufferSize;
	m_dwCountsUsingChannel	= 0;
	m_bAudioSendPauseFlag	= TRUE;

	m_idRoomID					= 0;
	m_nConcurrentTalkingCount	= SIZE_MAX_CHANNEL_COUNT;
	MEMSET_ZERO(m_szUserName, SIZE_MAX_USER_NAME);

	m_pChannelData			= new CChannelData[m_dwCountsChannel];
	for(int i=0; i < m_dwCountsChannel; i++)
	{
		m_pChannelData[i].SetChannelIndex(i);
		m_pChannelData[i].m_nStartCount = JitterBufferSize;
		m_pChannelData[i].m_nResetCount = JitterBufferSize + 2;
	}
	m_pAudioSound			= new CAudioSound(this, (MediaType)audioPayload, nFrameCount, JitterBufferSize);
	if(m_pAudioSound->CreateThread() == FALSE)
	{	// 스레드 생성
		/*
		 *	1 . THREAD_PRIORITY_TIME_CRITICAL
			2 . THREAD_PRIORITY_HIGHEST
			3 . THREAD_PRIORITY_ABOVE_NORMAL
			4 . THREAD_PRIORITY_NORMAL
			5 . THREAD_PRIORITY_BELOW_NORMAL
			6 . THREAD_PRIORITY_LOWEST
			7 . THREAD_PRIORITY_IDLE 

		 */
	}
	m_pAudioSound->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
}

CAVManager::~CAVManager()
{
	HANDLE hThread = m_pAudioSound->m_hThread;
	this->AudioEnd();
	if(hThread)
	{
		::WaitForSingleObject(hThread, 5000);
	}
	m_pAudioSound = NULL;
	SAFE_DELETE_ARRAY(m_pChannelData);
}

void CAVManager::ResetAVMaster(void)
{
	this->AudioStop();
	for(int i=0; i < m_dwCountsChannel; i++)
	{
		m_pChannelData[i].InitializeVariable();
	}
	m_bAudioSendPauseFlag	= TRUE;
}

BOOL CAVManager::GetPlayStartFlags(DWORD idx)
{
	if(NULL == m_pChannelData || m_dwCountsChannel <= idx)
	{
		return FALSE;
	}
	return m_pChannelData[idx].GetPlayStartFlag();
}

void CAVManager::SetPlayStartFlags(DWORD idx, BOOL bFlag)
{
	if(NULL == m_pChannelData || m_dwCountsChannel <= idx)
	{
		return;
	}
	m_pChannelData[idx].SetPlayStartFlag(bFlag);
}

DWORD CAVManager::GetDataCounts(DWORD idx)
{
	if(NULL == m_pChannelData || m_dwCountsChannel <= idx)
	{
		return 0;
	}
	return m_pChannelData[idx].GetDataCount();
}

SJittData* CAVManager::GetDataFromBuffers(DWORD idx)
{
	if(NULL == m_pChannelData || m_dwCountsChannel <= idx)
	{
		return 0;
	}
	return m_pChannelData[idx].GetDataFromBuffer();
}

void CAVManager::SetAudioSendPauseFlag(BOOL i_bFlag)
{
	m_bAudioSendPauseFlag = i_bFlag;
}

void CAVManager::SetAndSendAudioSendPauseFlag(BOOL i_bFlag)
{
// 2008-10-15 by cmkwon, 말하기 기능 수정 - 체크하면 안된다. 기본이 FALSE이기 때문에 FALSE를 설정하면 처음에 적용되지 않는 버그 수정
// 	if(m_bAudioSendPauseFlag == i_bFlag)
// 	{
// 		return;
// 	}
	SetAudioSendPauseFlag(i_bFlag);

	if(FALSE == i_bFlag)		// 2008-10-21 by cmkwon, SCDialer2 수정 - 말하기 시작이므로 기존을 무음으로 설정한다.
	{
		m_pAudioSound->SetSilenceState(TRUE);			
	}

	if(_SC2_CREATED_ROOM_STATUS > m_pSCDialer2Ctrl->m_nCtlStatus
		|| m_pS2CWinSocket2->IsConnected() == FALSE)
	{
		return;
	}
	
	char SendBuf[512];
	INIT_MSG(MSG_S2C_REQUEST_TOKEN, T_S2C_REQUEST_TOKEN, pSeToken, SendBuf);
	pSeToken->byIsTalk		= (m_bAudioSendPauseFlag==FALSE)?TRUE:FALSE;
	m_pS2CWinSocket2->Write(SendBuf, MSG_SIZE(MSG_S2C_REQUEST_TOKEN));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAVManager::SetConcurrentTalkingCount(int i_nCount)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAVManager::SetConcurrentTalkingCount(int i_nCount)
{
	m_nConcurrentTalkingCount = min(SIZE_MAX_CHANNEL_COUNT, max(i_nCount, 1));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAVManager::GetConcurrentTalkingCount(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-10-05 ~ 2005-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAVManager::GetConcurrentTalkingCount(void)
{
	return m_nConcurrentTalkingCount;
}


void CAVManager::AudioStart(void)
{
	if (NULL == m_pAudioSound) {	return;}
	
	m_pAudioSound->PlayStart();
	m_pAudioSound->RecordStart();
}

void CAVManager::AudioStop(void)
{
	if (NULL == m_pAudioSound) {	return;}
	AudioPlayStop();
	AudioRecordStop();
	Sleep(100);
}

void CAVManager::AudioEnd(void)
{
	if (NULL == m_pAudioSound) {	return;}
	AudioStop();	
	m_pAudioSound->PostThreadMessage(WM_AUDIO_ENDTHREAD,0,0);	
}


void CAVManager::AudioPlayStop(void)
{
	if (NULL == m_pAudioSound) {	return;}
	m_pAudioSound->PostThreadMessage(WM_PLAY_STOP,0L,0L);
}

void CAVManager::AudioRecordStop(void)
{
	if (NULL == m_pAudioSound) {	return;}
	m_pAudioSound->PostThreadMessage(WM_RECORD_STOP, 0L, 0L);
}


int CAVManager::SendRTPData(BYTE *pData, int nLen, int media, BOOL bVideoKeyFrame)
{
	if(_SC2_CREATED_ROOM_STATUS > m_pSCDialer2Ctrl->m_nCtlStatus
		|| m_pS2CWinSocket2->IsConnected() == FALSE
		|| nLen > SIZE_MAX_PACKET
		|| this->GetAudioSendPauseFlag())
	{
		return 0;
	}
	
	char SendBuf[512];
	INIT_MSG(MSG_S2C_RTP_PACKET, T_S2C_RTP_PACKET, pSeRTP, SendBuf);
	pSeRTP->byMediaType	= media;
	pSeRTP->usRTPLength	= nLen;
	memcpy(SendBuf + MSG_SIZE(MSG_S2C_RTP_PACKET), pData, nLen);

	return m_pS2CWinSocket2->Write(SendBuf, MSG_SIZE(MSG_S2C_RTP_PACKET)+nLen);
}

BOOL CAVManager::RecvRTPData(DWORD dwChannIdx, char *pData, int nLen, int media, BOOL bVideoKeyFrame/* = FALSE*/)
{
	return this->RecvRTPData(dwChannIdx, (BYTE*)pData, nLen, media, bVideoKeyFrame);
}
BOOL CAVManager::RecvRTPData(DWORD dwChannIdx, BYTE *pData, int nLen, int media, BOOL bVideoKeyFrame)
{	
	if(_SC2_CREATED_ROOM_STATUS > m_pSCDialer2Ctrl->m_nCtlStatus
		|| FALSE == m_pAudioSound->GetFlagPlaying()
		|| NULL == m_pChannelData 
		|| media)
	{
		return FALSE;
	}

	if(dwChannIdx > m_dwSelfChannelIndex)
	{
		dwChannIdx--;
	}
	if(dwChannIdx >= this->GetChannelCounts()){		return FALSE;}

	m_pChannelData[dwChannIdx].AudioDecode(pData, nLen);	
	return TRUE;
}

DWORD CAVManager::OtherJoinedRoom(char *i_szUserName, DWORD i_dwChannIdx)
{
	if(i_dwChannIdx > m_dwSelfChannelIndex)
	{
		i_dwChannIdx--;
	}
	if(i_dwChannIdx >= this->GetChannelCounts()){		return UINT_MAX;}
	
	if(m_pChannelData[i_dwChannIdx].GetUsingFlag() == FALSE)
	{
		m_dwCountsUsingChannel++;
		m_pChannelData[i_dwChannIdx].SetUsingFlag(TRUE);		
	}
	m_pChannelData[i_dwChannIdx].SetUserName(i_szUserName);
	return m_dwCountsUsingChannel;
}

DWORD CAVManager::OtherGoOutedRoom(DWORD dwChannIdx)
{
	if(dwChannIdx > m_dwSelfChannelIndex)
	{
		dwChannIdx--;
	}
	if(dwChannIdx >= this->GetChannelCounts()){		return UINT_MAX;}
	
	if(m_pChannelData[dwChannIdx].GetUsingFlag() == TRUE)
	{
		m_dwCountsUsingChannel--;		
	}
	m_pChannelData[dwChannIdx].InitializeVariable();
	return m_dwCountsUsingChannel;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAVManager::GetPlayVolumeW(BYTE *o_pbyVolPercent)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAVManager::GetPlayVolumeW(BYTE *o_pbyVolPercent)
{
	if(NULL == m_pAudioSound)
	{
		return FALSE;
	}
	return m_pAudioSound->GetPlayVolume(o_pbyVolPercent);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAVManager::SetPlayVolumeW(BYTE i_byVolPercent)
/// \brief		// 2008-06-12 by cmkwon, SCVoIP.exe 에 Volume 설정 기능 추가 - 
/// \author		cmkwon
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAVManager::SetPlayVolumeW(BYTE i_byVolPercent)
{
	if(NULL == m_pAudioSound)
	{
		return FALSE;
	}
	return m_pAudioSound->SetPlayVolume(i_byVolPercent);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAVManager::OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
/// \brief		// 2008-10-22 by cmkwon, SCDialer1, SCDialer2 수정 - 
/// \author		cmkwon
/// \date		2008-10-22 ~ 2008-10-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAVManager::OnVolumeW(BOOL i_bIsSpkDevice, BYTE i_byVolPercent)
{
	if(m_pSCDialer2Ctrl)
	{
		m_pSCDialer2Ctrl->OnVolume(i_bIsSpkDevice, i_byVolPercent);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAVManager::SendChangedTalkingState(TALKING_STATE_t i_talkingState)
/// \brief		// 2008-10-15 by cmkwon, 말하기 기능 수정 - 
/// \author		cmkwon
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAVManager::SendChangedTalkingState(TALKING_STATE_t i_talkingState)
{
	if(NULL == m_pAudioSound
		|| _SC2_CREATED_ROOM_STATUS > m_pSCDialer2Ctrl->m_nCtlStatus
		|| FALSE != this->GetAudioSendPauseFlag())	
	{
		return FALSE;
	}

	char SendBuf[512];
	INIT_MSG(MSG_S2C_CHANGE_TALKING_STATE, T_S2C_CHANGE_TALKING_STATE, pSMsg, SendBuf);
	pSMsg->talkingState		= i_talkingState;
	pSMsg->nChannelIndex	= this->GetSelfChannelIndex();
	STRNCPY_MEMSET(pSMsg->szUserName, this->GetUserName(), SIZE_MAX_USER_NAME);
	
	return m_pS2CWinSocket2->Write(SendBuf, MSG_SIZE(MSG_S2C_CHANGE_TALKING_STATE));
}