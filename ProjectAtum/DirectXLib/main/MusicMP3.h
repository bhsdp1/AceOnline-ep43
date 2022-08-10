
#ifndef _MUSICMP3_H__7A46D706_C6C0_4F7F_985C_BC9415B61698__INCLUDED_
#define _MUSICMP3_H__7A46D706_C6C0_4F7F_985C_BC9415B61698__INCLUDED_

#include <dshow.h> 
#include <commdlg.h>


class CMusicMP3
{
public:
	WCHAR m_wFileName[1024];

	IGraphBuilder		*	m_pGB;
	IMediaEventEx		*	m_pME;
	IMediaSeeking		*	m_pMS;
//	IMediaControl		*	m_pMC;

	CMusicMP3();
	~CMusicMP3();

	HRESULT Atum_MusicInit(TCHAR * strFileName);
	HRESULT	Atum_PlayMP3(long Volume);
	HRESULT Atum_MusicStop();
	HRESULT	Atum_LoopMusic(LONGLONG LMute = 0);

	HRESULT	SetAtumMusicVolume(long volume);

	HRESULT ConnectFilters( IBaseFilter* pUpFilter, IBaseFilter* pDownFilter, int nMAX=3);

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	BOOL IsNowPlay();

	LONGLONG			m_pLPos,m_pLStopPos;
	IMediaSeeking*		m_ppMS;
	BOOL				m_bNowPlay;
	BOOL				m_bDefaultMusic;
};

#endif //_MUSICMP3_H__7A46D706_C6C0_4F7F_985C_BC9415B61698__INCLUDED_endif