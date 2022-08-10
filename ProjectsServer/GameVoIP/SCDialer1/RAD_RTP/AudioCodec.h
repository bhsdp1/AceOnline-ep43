// AudioCodec.h: interface for the CAudioCodec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOCODEC_H__75C02F06_990E_4567_A2B2_27383CCBB9EB__INCLUDED_)
#define AFX_AUDIOCODEC_H__75C02F06_990E_4567_A2B2_27383CCBB9EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
typedef enum
{
    t_PCMU,
    t_G7231HI,
	t_G7231LO,
    t_G729,
} enumAudioCodecType;
//*/



typedef enum
{
    t_PCMU			=  0,
    t_G7231Low		= 50,		// G.7231 Low
	t_G7231			=  4,		// G.7231 High
    t_PCMA			=  8,
    t_G722			=  9, 
    t_G728			= 15,
    t_G729			= 18,
    t_H261			= 31,
    t_H263			= 34,
	t_NULL			= 64,		// 2진수 ==>   1000000
	t_Audio			=128,		// 2진수 ==>  10000000
	t_Video			=256,		// 2진수 ==> 100000000
	t_AudioAndVideo	=384		// 2진수 ==> 110000000
} MediaType;


typedef HANDLE     HCODEC;     /* speech codec handle */

class CAudioCodec  
{
public:	
	CAudioCodec();
	virtual ~CAudioCodec();

public:
	virtual BOOL	AudioCodecLoadLibrary(void)=0;
	virtual DWORD	AudioCodecEncoding(short* sourceData, BYTE* encodingData)=0;
	virtual DWORD	AudioCodecDecoding(BYTE *sourceData, short* decodingData)=0;
	virtual BOOL	Audio_InitEncod(void) =0;
	virtual BOOL	Audio_InitDecod(void) =0;
	virtual BOOL	AudioCodecOpen(void)=0;
	virtual void	AudioCodecClose(void)=0;

	int		GetAudioFrameSize();
	int		GetAudioPacketSize();

protected:
	HANDLE				m_hCODECDLibrary;
	HCODEC				m_hCODEC;
	int					m_nFrameSize;		//bytes size
	int					m_nPacketSize;		//bytes size

	int					m_nCodecFormat;		// 0 <= Encoder and Decoder
											// 1 <= Encoder Only
											// 2 <= Decoddr Only
};

#endif // !defined(AFX_AUDIOCODEC_H__75C02F06_990E_4567_A2B2_27383CCBB9EB__INCLUDED_)
