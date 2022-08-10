#if !defined(AFX_STDAFX_H__089438CC_C68E_4C33_AF85_35A57D2D8F38__INCLUDED_)
#define AFX_STDAFX_H__089438CC_C68E_4C33_AF85_35A57D2D8F38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Comon Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Delete the two includes below if you do not wish to use the MFC
//  database classes
#include <afxdb.h>			// MFC database classes
#include <afxdao.h>			// MFC DAO database classes
#include <afxsock.h>
#include <MMSystem.h>

#include "DbgOut_C.h"

#define MEMSET_ZERO(p, Size) (memset((p), 0x00, (Size)))

#define STRNCPY_MEMSET(szDest, szSource, MaxSize)		{ MEMSET_ZERO((szDest), (MaxSize));	\
															strncpy( (szDest), (szSource), (MaxSize)-1 );}

// CG: The following line was added by the Windows Multimedia component.
#pragma comment(lib, "winmm.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__089438CC_C68E_4C33_AF85_35A57D2D8F38__INCLUDED_)
