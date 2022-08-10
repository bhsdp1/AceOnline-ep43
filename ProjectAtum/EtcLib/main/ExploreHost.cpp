// ExploreHost.cpp: implementation of the CExploreHost class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <olectl.h>
#include <exdispid.h>
#include "ExploreHost.h"

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-23 by bhsohn 익스폴로워 모드로 업그레이드
/// \date		2013-05-23~ 2013-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExploreHost::CExploreHost()
{
	m_pWebBrowser = NULL;
	m_hWnd = NULL;
	CoInitialize(NULL);
}

CExploreHost::~CExploreHost()
{	
//	CoUninitialize();
}

void CExploreHost::RefreshExplore(HWND parentWnd, LPSTR strWebAddress, 
								  INT nWebPosX, INT nWebPosY,
								  INT nWebWidth,INT  nWebHeight, BOOL isWindowMode)
{ 
	int pos_x = 0;
	int pos_y = 0;
	int lWidth = nWebWidth;
	int lHeight = nWebHeight;
	
	::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&m_pWebBrowser);
	
	m_pWebBrowser->put_Left(pos_x);
	m_pWebBrowser->put_Top(pos_y);
	m_pWebBrowser->put_Height(lHeight);
	m_pWebBrowser->put_Width(lWidth);

	m_pWebBrowser->put_ToolBar(VARIANT_FALSE);
	m_pWebBrowser->put_MenuBar(VARIANT_FALSE);
	m_pWebBrowser->put_AddressBar(VARIANT_FALSE);
	m_pWebBrowser->put_StatusBar(VARIANT_FALSE);
	
	m_pWebBrowser->get_HWND(&m_hWnd);
	
	SetParent((HWND)m_hWnd, parentWnd); // 부모 바꾸기
	SetWindowLong((HWND)m_hWnd, GWL_STYLE, WS_CHILD); // 윈도우 스타일 바꾸기
		
	int len = lstrlen( strWebAddress) + 1;
	
	WCHAR *pszW;
	pszW = new WCHAR[len];
	MultiByteToWideChar( CP_ACP, 0, strWebAddress, -1, pszW, len );
	
	VARIANT v;
	VariantInit( &v );
	v.vt = VT_BSTR;
	v.bstrVal = SysAllocString(pszW);
	VARIANT vtEmpty;
	::VariantInit(&vtEmpty);
	
	m_pWebBrowser->Navigate2( &v, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty );
	m_pWebBrowser->put_Visible(VARIANT_TRUE);	

	SysFreeString(v.bstrVal); 	
	delete[] pszW;
}

void CExploreHost::ReleaseExplore(HWND orghWnd)
{
	if(m_pWebBrowser)
	{
 		if(orghWnd)
		{
 			SetParent((HWND)m_hWnd, orghWnd); // 부모 바꾸기		
		}
		m_pWebBrowser->Quit();
	}
}


