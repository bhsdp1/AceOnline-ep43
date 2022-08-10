// ExploreHost.h: interface for the CExploreHost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLOREHOST_H__A66D831C_1531_4A68_8F4C_C1401D5404E4__INCLUDED_)
#define AFX_EXPLOREHOST_H__A66D831C_1531_4A68_8F4C_C1401D5404E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exdisp.h>
#include <EXDISPID.H> 
class CExploreHost  
{
public:
	CExploreHost();
	virtual ~CExploreHost();

	
	void	RefreshExplore(HWND parentWnd, LPSTR strWebAddress, 
							INT nWebPosX, INT nWebPosY,
							INT nWebWidth,INT  nWebHeight, BOOL isWindowMode);
	void	ReleaseExplore(HWND orghWnd = NULL);

private:
	long m_hWnd;
	IWebBrowser2*		m_pWebBrowser;

};

#endif // !defined(AFX_EXPLOREHOST_H__A66D831C_1531_4A68_8F4C_C1401D5404E4__INCLUDED_)
