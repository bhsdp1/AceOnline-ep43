// RTPBase.h: interface for the CRTPBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTPBASE_H__CAF95040_9365_43D2_8769_35E7D5EF10E6__INCLUDED_)
#define AFX_RTPBASE_H__CAF95040_9365_43D2_8769_35E7D5EF10E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>

#define CRTPCTRLARRAYSIZE	7

class CRTPBase  
{
public:
	virtual int GetCRTPCtrlArraySize()=0;
	CRTPBase();
	virtual ~CRTPBase();

};

#endif // !defined(AFX_RTPBASE_H__CAF95040_9365_43D2_8769_35E7D5EF10E6__INCLUDED_)
