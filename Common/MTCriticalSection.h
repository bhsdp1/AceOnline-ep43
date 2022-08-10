// MTCriticalSection.h: interface for the CMTCriticalSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCCRITICALSECTION_H__0C995D67_AF59_441F_B5B8_EB73383A2B7C__INCLUDED_)
#define AFX_SYNCCRITICALSECTION_H__0C995D67_AF59_441F_B5B8_EB73383A2B7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMTCriticalSection
{
public:
	CMTCriticalSection();
	CMTCriticalSection(CRITICAL_SECTION *i_pCritSec);	
	virtual ~CMTCriticalSection();

	void ResetMTCriticalSection(void);

	void Enter(void);
	void Leave(void);
#if (_WIN32_WINNT >= 0x0400)
	BOOL Try(void);
#endif// (_WIN32_WINNT >= 0x0400)_endif


	CRITICAL_SECTION *GetCriticalSectionPtr(void){		return &m_criticalSection;};

protected:
	CRITICAL_SECTION		m_criticalSection;
	CRITICAL_SECTION		*m_pCriticalSection;
};

#endif // !defined(AFX_SYNCCRITICALSECTION_H__0C995D67_AF59_441F_B5B8_EB73383A2B7C__INCLUDED_)
