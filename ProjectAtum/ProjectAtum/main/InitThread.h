// InitThread.h: interface for the CInitThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITTHREAD_H__67641E05_0176_4053_B770_10F42028A8CD__INCLUDED_)
#define AFX_INITTHREAD_H__67641E05_0176_4053_B770_10F42028A8CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumThread.h"

class CInitThread : public CAtumThread  
{
public:
	CInitThread();
	virtual ~CInitThread();
	virtual DWORD Run();

};

#endif // !defined(AFX_INITTHREAD_H__67641E05_0176_4053_B770_10F42028A8CD__INCLUDED_)
