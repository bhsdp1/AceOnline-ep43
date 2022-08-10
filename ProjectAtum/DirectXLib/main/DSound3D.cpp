// DSound3D.cpp: implementation of the DSound3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSound3D.h"
#include "wavefile.h"
#include "AtumApplication.h"
#include "dxutil.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark


//#define SAFE_DELETE(p){ if(NULL != p){ delete (p); (p) = NULL;}}
//#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

Init_DS3D::Init_DS3D()
{
}

Init_DS3D::~Init_DS3D()
{

}

/*----------------------------------------------------------------------------*
		Init3DSound
*-----------------------------------------------------------------------------*/
HRESULT Init_DS3D::Init3DSound(HWND hWnd)
{
	FLOG( "Init_DS3D::Init3DSound(HWND hWnd)" );
	HRESULT		hr;	
	
    // Create IDirectSound using the primary sound device
   if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS8, NULL ) ) )
       return hr;

    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS8->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) ) )
        return hr;
 

    if( m_pDS8 == NULL )
        return CO_E_NOTINITIALIZED;

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;  //DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
   	if( FAILED( hr = m_pDS8->CreateSoundBuffer( &dsbd, &m_pDSBPrimary, NULL ) ) )
        return hr;

    
	WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) 2; //dwPrimaryChannels; 
    wfx.nSamplesPerSec  = 22050; //dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) 16; //dwPrimaryBitRate; 
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = m_pDSBPrimary->SetFormat(&wfx) ) )
        return hr;

	

	

	return S_OK;
}





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DSound3D::DSound3D()
{
	FLOG( "DSound3D()" );
	m_pDS8 = NULL;
	m_pDSBPrimary = NULL;	
	m_pWaveFile = NULL ;
	
	m_pWaveFile = new WaveFile();
	m_pBuffer = new LPDIRECTSOUNDBUFFER[PLAY_BUFFER];
	m_p3DBuffer = new LPDIRECTSOUND3DBUFFER[PLAY_BUFFER];
	m_p3D_Listener = new LPDIRECTSOUND3DLISTENER[PLAY_BUFFER];

	for(int i=0;i<PLAY_BUFFER;i++)
	{
	   m_p3DBuffer[i] = NULL;
	   m_pBuffer[i] = NULL;
	   m_p3D_Listener[i] = NULL;
	}

	m_StartTimeIndex = 0;
	m_bCurrentBuffer = 0;

}

DSound3D::~DSound3D()
{
	FLOG( "~DSound3D()" );
	SAFE_DELETE(m_pWaveFile);
	
	for(int i=0;i<PLAY_BUFFER;i++)
	{
		if(m_p3DBuffer[i])
			SAFE_RELEASE(m_p3DBuffer[i]);
		if(m_pBuffer[i])
			SAFE_RELEASE(m_pBuffer[i]);		
		if(m_p3D_Listener[i])
			SAFE_RELEASE(m_p3D_Listener[i]);		
	}
	if(m_pDSBPrimary)
		SAFE_RELEASE(m_pDSBPrimary);
	if(m_pDS8)
		SAFE_RELEASE(m_pDS8);
}


/*----------------------------------------------------------------------------*
		Init3DSound
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::Init3DSound(HWND hWnd)
{
	FLOG( "DSound3D::Init3DSound(HWND hWnd)" );
	HRESULT		hr;	
	
    // Create IDirectSound using the primary sound device
   if( FAILED( hr = DirectSoundCreate8( NULL, &m_pDS8, NULL ) ) )
       return hr;

    // Set DirectSound coop level 
    if( FAILED( hr = m_pDS8->SetCooperativeLevel( hWnd, DSSCL_PRIORITY ) ) )
        return hr;
 

    if( m_pDS8 == NULL )
        return CO_E_NOTINITIALIZED;

    // Get the primary buffer 
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize        = sizeof(DSBUFFERDESC);
    dsbd.dwFlags       = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;  //DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat   = NULL;
       
   	if( FAILED( hr = m_pDS8->CreateSoundBuffer( &dsbd, &m_pDSBPrimary, NULL ) ) )
        return hr;

    
	WAVEFORMATEX wfx;
    ZeroMemory( &wfx, sizeof(WAVEFORMATEX) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = (WORD) 2; //dwPrimaryChannels; 
    wfx.nSamplesPerSec  = 22050; //dwPrimaryFreq; 
    wfx.wBitsPerSample  = (WORD) 16; //dwPrimaryBitRate; 
    wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hr = m_pDSBPrimary->SetFormat(&wfx) ) )
        return hr;

	return S_OK;
}





/*----------------------------------------------------------------------------*
		SetWaveFileName
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::SetWaveFileName(TCHAR *strFileName)
{
	FLOG( "DSound3D::SetWaveFileName(TCHAR *strFileName)" );
	
	GUID    guid3DAlgorithm = GUID_NULL;
    WaveFile waveFile;
    
	// WAVEFILE_READ, WAVEFILE_WRITE 2가지 플래그
	waveFile.Open( strFileName, NULL, WAVEFILE_READ );

    WAVEFORMATEX* pwfx = waveFile.GetFormat();
    if( pwfx == NULL )
    {
		MessageBox(NULL,"Invalid wave file format.",NULL,MB_OK);
		return DSERR_BADFORMAT;
    }

    if( pwfx->nChannels > 1 )		// DSBCAPS_CTRL3D사용을 위해서는 모노WAVE여야함.
    {
        // Too many channels in wave.  Sound must be mono when using DSBCAPS_CTRL3D
	    MessageBox(NULL,"Wave file must be mono for 3D control.",NULL,MB_OK);
		return DSERR_BADFORMAT;
    }

    if( pwfx->wFormatTag != WAVE_FORMAT_PCM )	// 다이렉트사운드는 PCM WAV이어ㅏ야함.
    {
        // Sound must be PCM when using DSBCAPS_CTRL3D
		MessageBox(NULL,"Wave file must be PCM for 3D control.",NULL,MB_OK);
        return DSERR_BADFORMAT;
    }

    // Get the software DirectSound3D emulation algorithm to use
    // Ask the user for this sample, so display the algorithm dialog box.
    
    guid3DAlgorithm = DS3DALG_HRTF_LIGHT;

	return DS_OK;
}


/*----------------------------------------------------------------------------*
		DSCreate
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm)
{
	FLOG( "DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm)" );
	HRESULT hr;

    DWORD                dwDSBufferSize = NULL;
    WaveFile*           pWaveFile      = NULL;

    if(m_pDS8  == NULL )
        return CO_E_NOTINITIALIZED;
    if( strWaveFileName == NULL) // || ppSound == NULL || dwNumBuffers < 1 )
        return E_INVALIDARG;

    //pWaveFile = new WaveFile();
    if( m_pWaveFile == NULL )
    {
			DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) WaveFile Open Error\n");
		DBGOUT("WaveFile Open Error\n");
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    if(hr = m_pWaveFile->Open( strWaveFileName, NULL, WAVEFILE_READ ))
	{

		if(FAILED(hr))
		{
			DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) WaveFile Open Error\n");
			goto LFail;
		}
	}

    if( m_pWaveFile->GetSize() == 0 )
    {
        // Wave is blank, so don't create it.
        hr = E_FAIL;
        goto LFail;
    }

    // Make the DirectSound buffer the same size as the wav file
    m_dwDSBufferSize = m_pWaveFile->GetSize();

    // Create the direct sound buffer, and only request the flags needed
    // since each requires some overhead and limits if the buffer can 
    // be hardware accelerated
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
    dsbd.dwSize          = sizeof(DSBUFFERDESC);
    dsbd.dwFlags         = dwCreationFlags;
    dsbd.dwBufferBytes   = m_dwDSBufferSize;
    dsbd.guid3DAlgorithm = guid3DAlgorithm;
    dsbd.lpwfxFormat     = m_pWaveFile->m_pwfx;

	int		i;
	for(i=0 ; i<PLAY_BUFFER ; i++)
	{
		hr = m_pDS8->CreateSoundBuffer( &dsbd, &m_pBuffer[i], NULL );	
		if( hr == DS_NO_VIRTUALIZATION )
        return E_FAIL;
            
		if( FAILED(hr) )
		{
			if( hr != DSERR_BUFFERTOOSMALL )
			   goto LFail;
		}

		FillBufferWithSound( m_pBuffer[i], FALSE );
	
		m_pBuffer[i]->SetCurrentPosition(0);
	


	}


  	
	return S_OK;


LFail:
#ifdef _DEBUG
	if(hr==CO_E_NOTINITIALIZED)
	{
		DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) CO_E_NOTINITIALIZED\n");
	}
 	else if(hr==E_INVALIDARG)
	{
		DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) E_INVALIDARG\n");
	}
	else if(hr==E_OUTOFMEMORY)
	{
		DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) E_OUTOFMEMORY\n");
	}
	else
	{
		DBGOUT("ERROR : DSound3D::DSCreate(LPTSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm) OTHER FAIL\n");
	}
#endif
    return hr;
}


/*----------------------------------------------------------------------------*
			FillBufferWithSound
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger)
{
	FLOG( "DSound3D::FillBufferWithSound(LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger)" );
	HRESULT hr; 
    VOID*   pDSLockedBuffer      = NULL; // Pointer to locked buffer memory
    DWORD   dwDSLockedBufferSize = 0;    // Size of the locked DirectSound buffer
    DWORD   dwWavDataRead        = 0;    // Amount of data read from the wav file 

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;

    // Make sure we have focus, and we didn't just switch in from
    // an app which had a DirectSound device
    if( FAILED( hr = RestoreBuffer( pDSB, NULL ) ) ) 
        return hr;//DXTRACE_ERR( TEXT("RestoreBuffer"), hr );

    // Lock the buffer down
    if( FAILED( hr = pDSB->Lock( 0, m_dwDSBufferSize, 
                                 &pDSLockedBuffer, &dwDSLockedBufferSize, 
                                 NULL, NULL, 0L ) ) )
        return hr;//DXTRACE_ERR( TEXT("Lock"), hr );

    // Reset the wave file to the beginning 
    m_pWaveFile->ResetFile();

    if( FAILED( hr = m_pWaveFile->Read( (BYTE*) pDSLockedBuffer,
                                        dwDSLockedBufferSize, 
                                        &dwWavDataRead ) ) )           
        return hr;//DXTRACE_ERR( TEXT("Read"), hr );

    if( dwWavDataRead == 0 )
    {
        // Wav is blank, so just fill with silence
        FillMemory( (BYTE*) pDSLockedBuffer, 
                    dwDSLockedBufferSize, 
                    (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
    }
    else if( dwWavDataRead < dwDSLockedBufferSize )
    {
        // If the wav file was smaller than the DirectSound buffer, 
        // we need to fill the remainder of the buffer with data 
        if( bRepeatWavIfBufferLarger )
        {       
            // Reset the file and fill the buffer with wav data
            DWORD dwReadSoFar = dwWavDataRead;    // From previous call above.
            while( dwReadSoFar < dwDSLockedBufferSize )
            {  
                // This will keep reading in until the buffer is full 
                // for very short files
                if( FAILED( hr = m_pWaveFile->ResetFile() ) )
                    return hr;//DXTRACE_ERR( TEXT("ResetFile"), hr );

                hr = m_pWaveFile->Read( (BYTE*)pDSLockedBuffer + dwReadSoFar,
                                        dwDSLockedBufferSize - dwReadSoFar,
                                        &dwWavDataRead );
                if( FAILED(hr) )
                    return hr;//DXTRACE_ERR( TEXT("Read"), hr );

                dwReadSoFar += dwWavDataRead;
            } 
        }
        else
        {
            // Don't repeat the wav file, just fill in silence 
            FillMemory( (BYTE*) pDSLockedBuffer + dwWavDataRead, 
                        dwDSLockedBufferSize - dwWavDataRead, 
                        (BYTE)(m_pWaveFile->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
        }
    }

    // Unlock the buffer, we don't need it anymore.
    pDSB->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );

    return S_OK;
}


/*----------------------------------------------------------------------------*
			RestoreBuffer
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, BOOL *pbWasRestored)
{
	FLOG( "DSound3D::RestoreBuffer(LPDIRECTSOUNDBUFFER pDSB, BOOL *pbWasRestored)" );
	HRESULT hr;

    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;
    if( pbWasRestored )
        *pbWasRestored = FALSE;

    DWORD dwStatus;
    if( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
        return hr;//DXTRACE_ERR( TEXT("GetStatus"), hr );

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
        // Since the app could have just been activated, then DirectSound 
        // may not be giving us control yet, so restoring the buffer may fail.  
        // If it does, sleep until DirectSound gives us control.
        do 
        {
            hr = pDSB->Restore();
            if( hr == DSERR_BUFFERLOST )
                Sleep( 10 );
        }
        while( hr != DS_OK );

        if( pbWasRestored != NULL )
            *pbWasRestored = TRUE;

        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

/*----------------------------------------------------------------------------*
		PlayBufferCheck
*-----------------------------------------------------------------------------*/
LPDIRECTSOUNDBUFFER DSound3D::PlayBufferCheck()
{
	FLOG( "DSound3D::PlayBufferCheck()" );
/*	int			 i;
	HRESULT		hr;


	DWORD	dwStatus = 0 ;	//DSBSTATUS_PLAYING;
	
	
	for(i=0 ; i<PLAY_BUFFER ; i++)
	{
		if(m_pBuffer[i])
		{
			hr = m_pBuffer[i]->GetStatus(&dwStatus);	
			if ( ( dwStatus & DSBSTATUS_PLAYING ) == 0 )
			{
				break;
			}
		
		}
	}	
		

		
	if(i != PLAY_BUFFER)
	{
	//	m_StartTimeIndex++;
		return m_pBuffer[i];
	
	}
	else
	{
		return m_pBuffer[rand() % PLAY_BUFFER];
	}
*/
/*	else
	{
		DWORD		CurrentTime;
		DWORD		Index=0;
		CurrentTime = timeGetTime();
			
		for(int j=0 ; j<PLAY_BUFFER ; j++)
		{
			if( CurrentTime >= m_pdwStartTime[j])
			{
				CurrentTime = m_pdwStartTime[j];								
				Index = j;	
			}
		}
	
	//	m_StartTimeIndex = 0;
		m_pBuffer[Index]->Stop();

		return m_pBuffer[Index];
	}

*/
		m_pBuffer[m_bCurrentBuffer]->Stop();

		return m_pBuffer[m_bCurrentBuffer];

}


/*----------------------------------------------------------------------------*
		Play
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::Play(DWORD dwPriority, DWORD dwFlags,LONG lVolume)
{
	FLOG( "DSound3D::Play(DWORD dwPriority, DWORD dwFlags,LONG lVolume)" );
	if(lVolume > 0)
		lVolume = 0;
	LPDIRECTSOUNDBUFFER		lpDS = PlayBufferCheck();

    if( lpDS == NULL )
        return CO_E_NOTINITIALIZED;
	
	if(RestoreBuffer( lpDS, NULL ))
	{
		if( FAILED(FillBufferWithSound(lpDS, FALSE)) )
			return E_FAIL;
		
		if(FAILED(Reset()))
			return E_FAIL;
	
	}
	
	if(FAILED(lpDS->SetVolume(lVolume)))
		return E_FAIL;
	if(FAILED(lpDS->Play( 0, dwPriority, dwFlags )))
		return E_FAIL;
	m_bCurrentBuffer++;
	if(m_bCurrentBuffer==4)
		m_bCurrentBuffer=0;
	return S_OK;
}

/*----------------------------------------------------------------------------*
			Reset
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::Reset()
{
	FLOG( "DSound3D::Reset()" );
    if( m_pBuffer == NULL )
        return CO_E_NOTINITIALIZED;

	for(int i=0 ; i<PLAY_BUFFER ; i++)
	{
		if(m_pBuffer[i])
		{
			if(FAILED(m_pBuffer[i]->SetCurrentPosition( 0 )))
				return E_FAIL;
		}
	}
    return S_OK;
}

/*----------------------------------------------------------------------------*
		Set_Listener_Params
*-----------------------------------------------------------------------------*/
HRESULT DSound3D::Set_Listener_Params(D3DXVECTOR3 vPos, float x, float y, float z)
{
	FLOG( "DSound3D::Set_Listener_Params(D3DXVECTOR3 vPos, float x, float y, float z)" );
	D3DXVECTOR3 vVel,vUp,vVelTemp,vVel2;
	float fLength = D3DXVec3Length(&(vPos - g_pD3dApp->m_pShuttleChild->m_vPos));
	if(fLength > 200.0f)	
	{
		D3DXMATRIX mat;
		vVelTemp = D3DXVECTOR3(0,0,1);	// 기준벡터
		vUp = D3DXVECTOR3(0,1,0);
		D3DXVec3Normalize(&vVel,&(vPos-g_pD3dApp->m_pShuttleChild->m_vPos));
		float fAngle = ACOS(D3DXVec3Dot(&vVel,&g_pD3dApp->m_pShuttleChild->m_vVel));
		D3DXMatrixRotationAxis(&mat,&vUp,fAngle);
		D3DXVec3TransformCoord( &vVel2, &vVel, &mat );
		if(ACOS(D3DXVec3Dot(&vVel,&g_pD3dApp->m_pShuttleChild->m_vVel)) < fAngle)
			D3DXVec3TransformCoord( &vVel, &vVelTemp, &mat );
		else
		{
			D3DXMatrixRotationAxis(&mat,&vUp,-fAngle);
			D3DXVec3TransformCoord( &vVel, &vVelTemp, &mat );
		}
	}
	else
		vVel = D3DXVECTOR3(0,0,0);

	
	if(!m_pBuffer[m_bCurrentBuffer])
		return E_FAIL;
//	for(int i=0 ; i<PLAY_BUFFER ; i++)
//	{
		//3DBuffer	
		if( FAILED(m_pBuffer[m_bCurrentBuffer]->QueryInterface( IID_IDirectSound3DBuffer, 
														   (VOID**)&m_p3DBuffer[m_bCurrentBuffer]) ) )
		{
			return E_FAIL;		
		}
			
		//Listener 
		if( FAILED(m_pDSBPrimary->QueryInterface( IID_IDirectSound3DListener, 
													   (VOID**)&m_p3D_Listener[m_bCurrentBuffer]) ) )
		{
			return E_FAIL;		
		}

		
		m_3DBuffer.dwSize = sizeof(DS3DBUFFER);
		m_p3DBuffer[m_bCurrentBuffer]->GetAllParameters( &m_3DBuffer );
		
		// Set new 3D buffer parameters
		m_3DBuffer.dwMode = DS3DMODE_HEADRELATIVE;
//		m_p3DBuffer[i]->SetAllParameters( &m_3DBuffer, DS3D_IMMEDIATE );


		//여기서 좌표셋팅..
//		m_3DBuffer.vPosition =  (vPos - g_pD3dApp->m_pShuttleChild->m_vPos)*0.001f;//Pos.x;
//		m_3DBuffer.vPosition = fLength*vVel*0.001f;//Pos.x;
		m_3DBuffer.vPosition = fLength*vVel*0.001f;

		
		
//		m_3DBuffer.vConeOrientation = g_pD3dApp->m_pShuttleChild->m_vVel;
		if(FAILED(m_p3DBuffer[m_bCurrentBuffer]->SetAllParameters( &m_3DBuffer, DS3D_IMMEDIATE)))
			return E_FAIL;
	


		
		//3DListener(청취자 위치)
	//	m_3D_Listener.dwSize = sizeof(DS3DLISTENER);

	//	m_p3D_Listener[i].SetAllParameters( &m_3D_Listener, DS3D_IMMEDIATE  );  
					
			


		SAFE_RELEASE(m_p3DBuffer[m_bCurrentBuffer]);		
		SAFE_RELEASE(m_p3D_Listener[m_bCurrentBuffer]);

//	}		

	return S_OK;
}

VOID DSound3D::StopSound(bool bLoof)
{
	FLOG( "DSound3D::StopSound(bool bLoof)" );
	if(bLoof)
	{
		for(int i=0;i<4;i++)
		{
			if(m_pBuffer[i])
			{
				m_pBuffer[i]->Stop();
			}
		}
	}
	else
	{
		int n = m_bCurrentBuffer - 1;
		if(n < 0)
			n = 3;
		if(m_pBuffer[n])
			m_pBuffer[n]->Stop();
	}
}









































