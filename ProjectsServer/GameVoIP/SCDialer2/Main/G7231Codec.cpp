// CG7231Codec.cpp
//

#include "stdafx.h"
#include "G7231Codec.h"


CG7231Codec::CG7231Codec(int format, MediaType audioty)
{
	m_nCodecFormat = format;				// 0 <= Encoder and Decoder
											// 1 <= Encoder Only
											// 2 <= Decoddr Only

	m_nFrameSize = 480;						// Source size(byte)	

	switch (audioty)
	{
	case t_G7231Low:						// G.7231 Low bit rate
		m_nPacketSize = SX53_PACKSIZE;		// Destination size(20 byte)		
		break;
	case t_G7231:							// G.7231 High bit rate
		m_nPacketSize = SX63_PACKSIZE;		// Destination size(24 byte)	
		break;
	default:
		AfxMessageBox("G.723.1 :  Construction Error !!");
		return;
	}

	m_hCODEC = NULL;
	m_hCODECDLibrary = NULL;
	m_dwFeatures = 0;

	PSX5363OPEN pCodec_Open = NULL;
	PSX5363CLOSE pCodec_Close = NULL;
	PSX5363INITDEC pEnc_Init = NULL;
	PSX5363INITENC pDec_Init = NULL;
	
	PSX5363ENCODER pSpeechEncode53 = NULL;
	PSX5363ENCODER pSpeechEncode63 = NULL;
	PSX5363DECODER pSpeechDecode = NULL;
}

CG7231Codec::~CG7231Codec()
{
	AudioCodecClose();
}

BOOL	CG7231Codec::AudioCodecLoadLibrary(void)
{
	m_hCODECDLibrary = LoadLibrary("sx5363s.dll");
		
	if ((UINT)m_hCODECDLibrary < HINSTANCE_ERROR)
	{		
		return FALSE;
	}
	
	pCodec_Open = (PSX5363OPEN) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_Open");
	pCodec_Close = (PSX5363CLOSE) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_Close");
	
	if(m_nCodecFormat == 0)
	{
		pEnc_Init = (PSX5363INITENC) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_InitEncod");
		pSpeechEncode53 = (PSX5363ENCODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX53_SpeechEncode");
		pSpeechEncode63 = (PSX5363ENCODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX63_SpeechEncode");

		pDec_Init = (PSX5363INITDEC) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_InitDecod");
		pSpeechDecode = (PSX5363DECODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_SpeechDecode");

		if ((!pCodec_Open) || (!pCodec_Close) || (!pEnc_Init) || (!pDec_Init) ||
			(!pSpeechEncode53) || (!pSpeechEncode63) || (!pSpeechDecode))
		{
			return FALSE;
		}
	}
	else if ( m_nCodecFormat == 1)
	{		
		pEnc_Init = (PSX5363INITENC) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_InitEncod");
		pSpeechEncode53 = (PSX5363ENCODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX53_SpeechEncode");
		pSpeechEncode63 = (PSX5363ENCODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX63_SpeechEncode");				

		if ((!pCodec_Open) || (!pCodec_Close) || (!pEnc_Init) ||
			(!pSpeechEncode53) || (!pSpeechEncode63) )
		{
			return FALSE;
		}
	}	
	else if( m_nCodecFormat == 2)
	{
		pDec_Init = (PSX5363INITDEC) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_InitDecod");
		pSpeechDecode = (PSX5363DECODER) GetProcAddress((HMODULE)m_hCODECDLibrary, "SX5363_SpeechDecode");

		if ((!pCodec_Open) || (!pCodec_Close) || (!pDec_Init) || (!pSpeechDecode))
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

DWORD	CG7231Codec::AudioCodecEncoding(short* sourceData, BYTE* encodingData)
{
	if(NULL == m_hCODEC){	return 0;}
	DWORD BytesProduced = -1;
	if( m_nCodecFormat == 0 || m_nCodecFormat == 1)
	{		
		if (m_nPacketSize == SX63_PACKSIZE)
		{
			if(!(SX_OK==pSpeechEncode63( m_hCODEC, m_dwFeatures, sourceData, encodingData, &BytesProduced))) 
			{
				TRACE("ERROR: SX80LC_SpeechEncode Failed\n");
				return -1;
			}		       
		}
		else if(m_nPacketSize== SX53_PACKSIZE)
		{
			if(!(SX_OK==pSpeechEncode53( m_hCODEC, m_dwFeatures, sourceData, encodingData, &BytesProduced))) 
			{
				TRACE("ERROR: SX80LC_SpeechEncode Failed\n");
				return -1;
			}		
		}
	}

	return BytesProduced;
}
DWORD	CG7231Codec::AudioCodecDecoding(BYTE *sourceData, short* decodingData)
{
	if(NULL == m_hCODEC){	return 0;}
	DWORD BytesUsed = -1;	
	if( m_nCodecFormat == 0 || m_nCodecFormat == 2)
	{
		if(!(SX_OK == pSpeechDecode( m_hCODEC, m_dwFeatures, sourceData, decodingData,  &BytesUsed))) 
		{
			TRACE("G.723.1 SpeechDecode Failed\n");
			return -1;
		}
	}

	return BytesUsed;
}

BOOL	CG7231Codec::Audio_InitEncod(void) 
{
	if(!(SX_OK == pEnc_Init(m_hCODEC))) 
	{
		TRACE("ERROR: G.723.1 _EncodeInit Failed\n");
		return FALSE;
	}	
	return TRUE;
}

BOOL	CG7231Codec::Audio_InitDecod(void)
{
	if(!(SX_OK==pDec_Init( m_hCODEC))) 
	{
		TRACE("ERROR: G.723.1 _DecodeInit Failed\n");
		return FALSE;
	}
	
	return TRUE;
}

BOOL	CG7231Codec::AudioCodecOpen(void)
{
	if (SX_OK != pCodec_Open(&m_hCODEC))
	{
		TRACE("Codec failed to open\n");
		return FALSE;
	}

	if (NULL == m_hCODEC)
	{
		TRACE("Codec failed to return valid handle\n");
		return FALSE;
	}

	return TRUE;
}

void	CG7231Codec::AudioCodecClose(void)
{
	if(m_hCODEC != NULL)
		pCodec_Close(&m_hCODEC);

	if (m_hCODECDLibrary)
		FreeLibrary((HMODULE)m_hCODECDLibrary);
}

