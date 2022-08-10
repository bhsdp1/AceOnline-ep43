// DataQueue.cpp: implementation of the CChannelData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChannelData::CChannelData()
:m_AudioDecoder(2, t_G7231Low)
{
	m_bUsingFlag				= FALSE;
	memset(m_szUserName, 0x00, SIZE_MAX_USER_NAME);

	//////////////////////////////////////////////////////////////////////
	// Jitter Buffering
	m_bFlagPlayStart		= FALSE;

	//////////////////////////////////////////////////////////////////////
	// Audio Decoder
	AudioCodecInit();

	m_bAudioPauseFlag		= FALSE;
}

CChannelData::~CChannelData()
{
	EmptyDataQueue();
}

void CChannelData::InitializeVariable()
{
	m_bUsingFlag			= FALSE;
	memset(m_szUserName, 0x00, SIZE_MAX_USER_NAME);
	
	m_bFlagPlayStart		= FALSE;
	EmptyDataQueue();	
		
	m_bAudioPauseFlag		= FALSE;
}

int CChannelData::SetDataToBuffer(SJittData *lp)
{
	int nRet;
	m_mtlistJittDataPtr.lock();
	if(m_mtlistJittDataPtr.size() < SIZE_MAX_JITTERBUFFER)
	{
		m_mtlistJittDataPtr.push_back(lp);
	}
	else
	{
		SAFE_DELETE(lp);
	}
	nRet = m_mtlistJittDataPtr.size();
	m_mtlistJittDataPtr.unlock();
	return nRet;
}

SJittData* CChannelData::GetDataFromBuffer()
{	
	SJittData *pJitt = NULL;
	m_mtlistJittDataPtr.lock();
	if(m_mtlistJittDataPtr.empty() == false)
	{
		pJitt = *m_mtlistJittDataPtr.begin();
		m_mtlistJittDataPtr.pop_front();
	}
	m_mtlistJittDataPtr.unlock();
	return pJitt;
}

DWORD CChannelData::GetDataCount()
{
	return m_mtlistJittDataPtr.size();
}

void CChannelData::EmptyDataQueue()
{
	SJittData* pJD;
	m_mtlistJittDataPtr.lock();
	while(m_mtlistJittDataPtr.empty() == false)
	{
		pJD = *m_mtlistJittDataPtr.begin();
		m_mtlistJittDataPtr.pop_front();
		SAFE_DELETE(pJD);
	}
	m_mtlistJittDataPtr.unlock();
}

void CChannelData::DeleteDataFromBuffer(int count)
{
	SJittData* pJD;
	for(int i=0; i < count; i++)
	{		
		pJD = GetDataFromBuffer();
		SAFE_DELETE(pJD);
	}
}


BOOL CChannelData::AudioCodecInit(void)
{
	return m_AudioDecoder.AudioCodecLoadLibrary();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//		Audio Decoder
//////////////////////////////////////////////////////////////////////////
BOOL CChannelData::AudioDecode(BYTE *pEncodedData, DWORD dwDataSize)
{
	const int nPacketBytes	= 20;
	const int nFrameBytes	= 480;
	if(dwDataSize%nPacketBytes != 0){	return FALSE;}
	
	SJittData *pJitt	= NULL;
	int nCount;
	for(int i=0; i < dwDataSize/nPacketBytes ; i++)
	{
		pJitt = (SJittData*)(new BYTE[sizeof(SJittData) + nFrameBytes]);
		memset(pJitt, 0x00, sizeof(SJittData) + nFrameBytes);
		pJitt->dataSize = nFrameBytes;
		pJitt->lpData = (BYTE*)pJitt + sizeof(SJittData);					
		
		m_AudioDecoder.AudioCodecDecoding(&(pEncodedData[i*nPacketBytes]), (short*)pJitt->lpData);
		nCount = SetDataToBuffer(pJitt);
		
		if(GetPlayStartFlag() == FALSE)
		{
			if(nCount >= m_nStartCount)
			{
				SetPlayStartFlag(TRUE);
			}
		}
		else if(nCount >= m_nResetCount)
		{
			DeleteDataFromBuffer(m_nResetCount/2);				
		}
	}
	return TRUE;
}



