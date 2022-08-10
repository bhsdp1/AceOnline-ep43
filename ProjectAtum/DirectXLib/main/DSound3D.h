// DSound3D.h: interface for the DSound3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSOUND3D_H__F57A4F17_88F8_45A3_BEFD_65C3F26FB89E__INCLUDED_)
#define AFX_DSOUND3D_H__F57A4F17_88F8_45A3_BEFD_65C3F26FB89E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>


#define PLAY_BUFFER	4


class Init_DS3D
{
public:
	LPDIRECTSOUND8					m_pDS8 ;
	LPDIRECTSOUNDBUFFER				m_pDSBPrimary ;
	
	Init_DS3D();
	~Init_DS3D();

	HRESULT	Init3DSound(HWND hwnd);

};


class WaveFile;
class DSound3D  
{
public:
//	DWORD		*	m_pdwStartTime;
	DWORD		m_StartTimeIndex;

	LPDIRECTSOUND8					m_pDS8 ;
	LPDIRECTSOUNDBUFFER				m_pDSBPrimary ;
	
	LPDIRECTSOUNDBUFFER			* m_pBuffer;
	
	LPDIRECTSOUND3DBUFFER		*	m_p3DBuffer;
	LPDIRECTSOUND3DLISTENER		*	m_p3D_Listener;
	
	
	DS3DBUFFER				m_3DBuffer;
	DS3DLISTENER            m_3DListener;  // Listener properties


	WaveFile	*	m_pWaveFile;
	DWORD		m_dwDSBufferSize;
	LPDIRECTSOUNDBUFFER		PlayBufferCheck();

	BYTE					m_bCurrentBuffer;


public:
	DSound3D();
	virtual ~DSound3D();

	HRESULT	Init3DSound(HWND hwnd);

	HRESULT	  SetWaveFileName(TCHAR *strFileName);
	HRESULT	  DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm);
	HRESULT	  FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger);
	HRESULT	  RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, BOOL *pbWasRestored);
	HRESULT		Play(DWORD dwPriority, DWORD dwFlags,LONG lVolume);
	HRESULT		Reset();
	HRESULT	 Set_Listener_Params(D3DXVECTOR3 vPos, float x, float y, float z);

	VOID	StopSound(bool bLoof);
};


#endif // !defined(AFX_DSOUND3D_H__F57A4F17_88F8_45A3_BEFD_65C3F26FB89E__INCLUDED_)
