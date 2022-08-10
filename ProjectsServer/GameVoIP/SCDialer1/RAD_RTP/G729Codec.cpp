// G729Codec.cpp: implementation of the CG729Codec class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "G729Codec.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CG729Codec::CG729Codec(int format)
{
	m_nCodecFormat = format;	// 0 <= Encoder and Decoder
								// 1 <= Encoder Only
								// 2 <= Decoddr Only

	m_nPacketSize = 10;			// byte	
	m_nFrameSize = 160;			// byte			
	
	m_hCODEC = NULL;
	m_hCODECDLibrary = NULL;

	pSX80LC_CodecOpen = NULL;
	pSX80LC_CodecClose = NULL;
	pSX80LC_EncodeInit = NULL;
	pSX80LC_DecodeInit = NULL;
	pSX80LC_SpeechEncode = NULL;
	pSX80LC_SpeechDecode = NULL;
}

CG729Codec::~CG729Codec()
{
	AudioCodecClose();
}

BOOL CG729Codec::AudioCodecLoadLibrary(void)
{
	if(!( m_hCODECDLibrary = LoadLibrary("sx80lc32.dll"))) 
	{		
		return FALSE;
	}

	pSX80LC_CodecOpen = (PSX80LCCODECOPEN) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_CodecOpen");
	pSX80LC_CodecClose = (PSX80LCCODECCLOSE) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_CodecClose");
	
	if(m_nCodecFormat == 0)
	{
		pSX80LC_EncodeInit = (PSX80LCENCODECINIT) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_EncodeInit");
		pSX80LC_SpeechEncode = (PSX80LCSPEECHENCODE) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_SpeechEncode");
		
		pSX80LC_DecodeInit = (PSX80LCDECODECINIT) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_DecodeInit");	
		pSX80LC_SpeechDecode = (PSX80LCSPEECHDECODE) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_SpeechDecode");

		if ((!pSX80LC_CodecOpen) || (!pSX80LC_CodecClose) || (!pSX80LC_EncodeInit) || 
			(!pSX80LC_DecodeInit) || (!pSX80LC_SpeechEncode) || (!pSX80LC_SpeechDecode))
		{
			return FALSE;
		}
	}
	else if ( m_nCodecFormat == 1)
	{		
		pSX80LC_EncodeInit = (PSX80LCENCODECINIT) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_EncodeInit");
		pSX80LC_SpeechEncode = (PSX80LCSPEECHENCODE) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_SpeechEncode");
				
		if ((!pSX80LC_CodecOpen) || (!pSX80LC_CodecClose) || (!pSX80LC_EncodeInit)|| (!pSX80LC_SpeechEncode) )
		{
			return FALSE;
		}
	}	
	else if( m_nCodecFormat == 2)
	{
		pSX80LC_DecodeInit = (PSX80LCDECODECINIT) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_DecodeInit");	
		pSX80LC_SpeechDecode = (PSX80LCSPEECHDECODE) GetProcAddress((HINSTANCE)m_hCODECDLibrary,"SX80LC_SpeechDecode");

		if ((!pSX80LC_CodecOpen) || (!pSX80LC_CodecClose) || (!pSX80LC_DecodeInit) || (!pSX80LC_SpeechDecode))
		{
			return FALSE;
		}
	}
	
	if( AudioCodecOpen() != TRUE)
	{
		return FALSE;
	}

	if(m_nCodecFormat == 0)
	{		
		Audio_InitEncod();
		Audio_InitDecod();
	}
	else if ( m_nCodecFormat == 1)
	{		
		Audio_InitEncod();				
	}	
	else if( m_nCodecFormat == 2)
	{
		Audio_InitDecod();
	}	

	return TRUE;
}

DWORD	CG729Codec::AudioCodecEncoding(short* sourceData, BYTE* encodingData)
{
	DWORD BytesProduced = -1;

	if( m_nCodecFormat == 0 || m_nCodecFormat == 1)
	{
		if(!(SX_OK==pSX80LC_SpeechEncode( m_hCODEC, (WORD*)sourceData, encodingData, &BytesProduced))) 
		{
			TRACE("ERROR: SX80LC_SpeechEncode Failed\n");
			return -1;
		}
	}
	return BytesProduced;
}

DWORD	CG729Codec::AudioCodecDecoding(BYTE *sourceData, short* decodingData)
{
	DWORD BytesUsed = -1;
	
	if( m_nCodecFormat == 0 || m_nCodecFormat == 2)
	{
		if(!(SX_OK==pSX80LC_SpeechDecode( m_hCODEC, sourceData, (WORD*)decodingData, &BytesUsed))) 
		{
			TRACE("ERROR: SX80LC_SpeechDecode Failed\n");
			return -1;
		}
	}

	return BytesUsed;
}

BOOL	CG729Codec::Audio_InitEncod(void) 
{
	if(!(SX_OK == pSX80LC_EncodeInit( m_hCODEC))) 
	{
		TRACE("ERROR: SX80LC_EncodeInit Failed\n");
		return FALSE;
	}	
	return TRUE;
}

BOOL	CG729Codec::Audio_InitDecod(void)
{
	if(!(SX_OK==pSX80LC_DecodeInit( m_hCODEC))) 
	{
		TRACE("ERROR: SX80LC_DecodeInit Failed\n");
		return FALSE;
	}
	return TRUE;
}

BOOL	CG729Codec::AudioCodecOpen(void)
{
	if(!(SX_OK==pSX80LC_CodecOpen( &m_hCODEC))) 
	{
		TRACE("ERROR: SX80LC_CodecOpen Failed\n");
		return FALSE;
	}

	if (NULL == m_hCODEC)
	{
		TRACE("G.729 : Codec failed to return valid handle\n");
		return FALSE;
	}

	return TRUE;
}

void	CG729Codec::AudioCodecClose(void)
{
	if(m_hCODEC != NULL)
		pSX80LC_CodecClose(&m_hCODEC);

	if (m_hCODECDLibrary)
		FreeLibrary((HMODULE)m_hCODECDLibrary);
}