// ChannelData.h: interface for the CChannelData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_)
#define AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include "mt_stl.h"
#include "G7231Codec.h"
#include "VoIPParam.h"

const int SIZE_MAX_JITTERBUFFER			= 30;

class SJittData
{
public:
	int				dataSize;
	BYTE*			lpData;
};

typedef mt_list<SJittData*>			mtlistSJittDataPtr;


class CChannelData  
{
public:
	CChannelData();
	virtual ~CChannelData();

	BOOL GetUsingFlag(void){			return m_bUsingFlag;}
	void SetUsingFlag(BOOL flag){		m_bUsingFlag = flag;}
	char *GetUserName(void){			return m_szUserName;}
	void SetUserName(char *id){			strncpy(m_szUserName, id, SIZE_MAX_USER_NAME);}
	DWORD GetChannelIndex(void){		return m_dwChnnelIndex;}
	void SetChannelIndex(DWORD dwIdx){	m_dwChnnelIndex = dwIdx;}


	//////////////////////////////////////////////////////////////////////
	// Jitter Buffering
	void DeleteDataFromBuffer(int count);
	DWORD GetDataCount(void);
	SJittData* GetDataFromBuffer(void);
	int SetDataToBuffer(SJittData* lp);
	void InitializeVariable(void);
	void EmptyDataQueue(void);
	BOOL GetPlayStartFlag(void){		return m_bFlagPlayStart;}
	void SetPlayStartFlag(BOOL bFlag){	m_bFlagPlayStart = bFlag;}

	//////////////////////////////////////////////////////////////////////
	// Audio Decoder
	BOOL AudioDecode(BYTE *pEncodedData, DWORD dwDataSize);

protected:
	//////////////////////////////////////////////////////////////////////
	// Audio Decoder
	BOOL AudioCodecInit(void);

protected:
	BOOL					m_bUsingFlag;
	DWORD					m_dwChnnelIndex;
	char					m_szUserName[SIZE_MAX_USER_NAME];	
	
	//////////////////////////////////////////////////////////////////////
	// Jitter Buffering
	mtlistSJittDataPtr		m_mtlistJittDataPtr;
	BOOL					m_bFlagPlayStart;

	//////////////////////////////////////////////////////////////////////
	// Audio Decoder
	CG7231Codec				m_AudioDecoder;
	
public:	
	int						m_nStartCount;
	int						m_nResetCount;

	BYTE					m_bAudioPauseFlag;
};
#endif // !defined(AFX_DATAQUEUE_H__53325E8D_5B20_47DB_A70F_A173E5488D49__INCLUDED_)
