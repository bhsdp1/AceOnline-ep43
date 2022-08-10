// LoadLibrary.h: interface for the CLoadLibrary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADLIBRARY_H__6846754F_AF70_4B1A_9928_2FCB26324C20__INCLUDED_)
#define AFX_LOADLIBRARY_H__6846754F_AF70_4B1A_9928_2FCB26324C20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \version	
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CLoadLibrary  
{
public:
	CLoadLibrary();
	virtual ~CLoadLibrary();

	virtual BOOL LoadProcList(void);

	BOOL LoadLibraryw(char *i_szLibFileName);
	void FreeLibraryw(void);
	BOOL GetProcAddressw(FARPROC *o_ppProc, char *i_szFunctionName);
	mt_lock *GetmtlockPtr(void);

protected:
	HANDLE			m_hLibrary;
	mt_lock			m_mtlock;
};

#endif // !defined(AFX_LOADLIBRARY_H__6846754F_AF70_4B1A_9928_2FCB26324C20__INCLUDED_)
