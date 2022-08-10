// MTAutoCriticalSection.h: interface for the CMTAutoCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCAUTOCRITICALSECTION_H__17CA6620_BF43_4FA3_A3A6_09F508E3CBCF__INCLUDED_)
#define AFX_SYNCAUTOCRITICALSECTION_H__17CA6620_BF43_4FA3_A3A6_09F508E3CBCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MTCriticalSection.h"

class CMTAutoCriticalSection : public CMTCriticalSection  
{
public:
	CMTAutoCriticalSection();
	CMTAutoCriticalSection(CRITICAL_SECTION *i_pCritSec);
	virtual ~CMTAutoCriticalSection();

	void UnlockMTAutoCriticalSection(void);
};

#endif // !defined(AFX_SYNCAUTOCRITICALSECTION_H__17CA6620_BF43_4FA3_A3A6_09F508E3CBCF__INCLUDED_)
