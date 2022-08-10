// MapInitThread.h: interface for the CMapInitThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPINITTHREAD_H__AE4C6BDC_7D6A_4DE5_91A9_9BF507AD0478__INCLUDED_)
#define AFX_MAPINITTHREAD_H__AE4C6BDC_7D6A_4DE5_91A9_9BF507AD0478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumThread.h"

class CMapInitThread : public CAtumThread  
{
public:
	CMapInitThread();
	virtual ~CMapInitThread();
	virtual DWORD Run();
};

#endif // !defined(AFX_MAPINITTHREAD_H__AE4C6BDC_7D6A_4DE5_91A9_9BF507AD0478__INCLUDED_)
