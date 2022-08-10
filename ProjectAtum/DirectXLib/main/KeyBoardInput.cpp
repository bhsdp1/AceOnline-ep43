// KeyBoardInput.cpp: implementation of the CKeyBoardInput class.
//
//////////////////////////////////////////////////////////////////////

#include "KeyBoardInput.h"
#include "DXUtil.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeyBoardInput::CKeyBoardInput()
{
	FLOG( "CKeyBoardInput()" );
	m_pDI = NULL;
	m_pKeyboard = NULL;
	memset(m_cDiks,0x00,sizeof(m_cDiks));

	// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
	m_bLostDevice = FALSE;

	// 2008-01-11 by bhsohn 포커스 없을시, 키보드 처리 안되게 함
	m_bGetFocus = TRUE;

}

CKeyBoardInput::~CKeyBoardInput()
{
	FLOG( "~CKeyBoardInput()" );

}

HRESULT CKeyBoardInput::OnCreateDevice( HWND hDlg )
{
	FLOG( "CKeyBoardInput::OnCreateDevice( HWND hDlg )" );
    HRESULT hr;
    DWORD   dwCoopFlags=0;

	FreeDirectInput();
	dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;// | DISCL_NOWINKEY;

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
        return hr;
    
    // Obtain an interface to the system keyboard device.
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
        return hr;
	if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
    hr = m_pKeyboard->SetCooperativeLevel( hDlg, dwCoopFlags );
    m_pKeyboard->Acquire();

    return S_OK;
}

HRESULT CKeyBoardInput::ReadImmediateData( HWND hDlg )
{
	FLOG( "CKeyBoardInput::ReadImmediateData( HWND hDlg )" );
    HRESULT hr;
//    TCHAR   strNewText[256*5 + 1] = TEXT("");

    if( NULL == m_pKeyboard ) 
        return S_OK;
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( &m_cDiks, sizeof(m_cDiks) );
    hr = m_pKeyboard->GetDeviceState( sizeof(m_cDiks), &m_cDiks );
    if( FAILED(hr) ) 
    {
        hr = m_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pKeyboard->Acquire();

       return S_OK; 
    }
   
    return S_OK;
}

VOID CKeyBoardInput::FreeDirectInput()
{
	FLOG( "CKeyBoardInput::FreeDirectInput()" );
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( m_pKeyboard ) 
        m_pKeyboard->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pKeyboard );
    SAFE_RELEASE( m_pDI );
}

BOOL CKeyBoardInput::GetAsyncKeyState(int code)
{
	FLOG( "CKeyBoardInput::GetAsyncKeyState(int code)" );
	// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
	if((TRUE == m_bLostDevice)
		||(FALSE == m_bGetFocus))// 2008-01-11 by bhsohn 포커스 없을시, 키보드 처리 안되게 함
	{		
		return FALSE;		
	}
	// end 2007-06-18 by bhsohn 알트텝 관련 버그 처리
	return m_cDiks[code];
}


// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
void CKeyBoardInput::SetKeyBoardLostDevice(BOOL bLostDevice)
{
	m_bLostDevice = bLostDevice;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-01-11 by bhsohn 포커스 없을시, 키보드 처리 안되게 함
/// \date		2008-01-11 ~ 2008-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CKeyBoardInput::SetKeyBoardFocus(BOOL bGetFocus)
{	
	m_bGetFocus = bGetFocus;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-06-20 by bhsohn EP3 옵션관련 처리
/// \date		2008-06-20 ~ 2008-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CKeyBoardInput::GetKeyBoardFocus()
{
	return m_bGetFocus;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CKeyBoardInput::GetKeyBoardLostDevice()
{
	return m_bLostDevice;
}