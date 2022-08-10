// CG7231Codec.h
//

#if !defined(CG7231Codec_h)
#define CG7231Codec_h

/*
 * Audio G.723.1 Codoc
 * Elemedia Codec
 */
#include <windows.h>
#include "sx5363api.h"
#include "AudioCodec.h"

class CG7231Codec : public CAudioCodec
{
public:
    CG7231Codec(int format=0 );
	~CG7231Codec();


public:
	BOOL	AudioCodecLoadLibrary(void);
	DWORD	AudioCodecEncoding(short* sourceData, BYTE* encodingData);
	DWORD	AudioCodecDecoding(BYTE *sourceData, short* decodingData);
	BOOL	Audio_InitEncod(void) ;
	BOOL	Audio_InitDecod(void);
	BOOL	AudioCodecOpen(void);
	void	AudioCodecClose(void);
		

private:
	PSX5363OPEN		pCodec_Open;
	PSX5363CLOSE	pCodec_Close;
	PSX5363INITDEC	pEnc_Init;
	PSX5363INITENC	pDec_Init;	
	PSX5363ENCODER	pSpeechEncode53 ;
	PSX5363ENCODER	pSpeechEncode63 ;
	PSX5363DECODER	pSpeechDecode ;

	DWORD			m_dwFeatures;
};

#endif /* CG7231Codec_h */
