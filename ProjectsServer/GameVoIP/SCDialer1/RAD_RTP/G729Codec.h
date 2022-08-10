// G729Codec.h: interface for the CG729Codec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_G729CODEC_H__A9732879_B713_45F4_B97B_9CD12150677F__INCLUDED_)
#define AFX_G729CODEC_H__A9732879_B713_45F4_B97B_9CD12150677F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef HANDLE     HCODEC;     /* speech codec handle */

#include "sx80api.h"
#include "AudioCodec.h"


class CG729Codec : public CAudioCodec 
{
public:
	CG729Codec(int format=0);
	virtual ~CG729Codec();

public:	
	BOOL	AudioCodecLoadLibrary(void);
	DWORD	AudioCodecEncoding(short* sourceData, BYTE* encodingData);
	DWORD	AudioCodecDecoding(BYTE *sourceData, short* decodingData);
	BOOL	Audio_InitEncod(void) ;
	BOOL	Audio_InitDecod(void);
	BOOL	AudioCodecOpen(void);
	void	AudioCodecClose(void);
	

private:	
	PSX80LCCODECOPEN	pSX80LC_CodecOpen;
	PSX80LCCODECCLOSE	pSX80LC_CodecClose;
	PSX80LCENCODECINIT	pSX80LC_EncodeInit;
	PSX80LCDECODECINIT	pSX80LC_DecodeInit;
	PSX80LCSPEECHENCODE	pSX80LC_SpeechEncode;
	PSX80LCSPEECHDECODE	pSX80LC_SpeechDecode;

};

#endif // !defined(AFX_G729CODEC_H__A9732879_B713_45F4_B97B_9CD12150677F__INCLUDED_)
