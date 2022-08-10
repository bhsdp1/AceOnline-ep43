// VTCThread.h: interface for the CVTCThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VTCTHREAD_H__E67E4904_1105_407B_87B3_B7679488FE0C__INCLUDED_)
#define AFX_VTCTHREAD_H__E67E4904_1105_407B_87B3_B7679488FE0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumThread.h"

#define GUARD_TIME 10000
class CVTCThread : public CAtumThread  
{
public:
	CVTCThread();
	virtual ~CVTCThread();
	virtual DWORD Run();
};
#endif