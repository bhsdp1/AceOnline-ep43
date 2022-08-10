// LoadLibrary.cpp: implementation of the CLoadLibrary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoadLibrary.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoadLibrary::CLoadLibrary()
{
	m_hLibrary		= NULL;
}

CLoadLibrary::~CLoadLibrary()
{
	if(m_hLibrary)
	{
		this->FreeLibraryw();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadLibrary::LoadLibraryw(char *i_szLibFileName)
{
	mt_auto_lock mtA(this->GetmtlockPtr());

	m_hLibrary	= ::LoadLibrary(i_szLibFileName);
	if (NULL == m_hLibrary)
	{
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLoadLibrary::FreeLibraryw(void)
{
	mt_auto_lock mtA(this->GetmtlockPtr());

	HANDLE hLib = m_hLibrary;
	if(NULL == hLib)
	{
		return;
	}

	::FreeLibrary((HMODULE)hLib);
	m_hLibrary		= NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadLibrary::GetProcAddressw(FARPROC *o_ppProc, char *i_szFunctionName)
{
	mt_auto_lock mtA(this->GetmtlockPtr());

	*o_ppProc	= NULL;
	if(NULL == m_hLibrary)
	{
		g_pGlobalGameServer->WriteSystemLogEX(TRUE, "[ERROR] innBill CLoadLibrary::GetProcAddressw# invalid Handle !!, hLibaray(0x%X) FuncName(%s) \r\n", m_hLibrary, i_szFunctionName);
		return FALSE;
	}

	*o_ppProc = ::GetProcAddress((HMODULE)m_hLibrary, i_szFunctionName);
	if(NULL == *o_ppProc)
	{
		g_pGlobalGameServer->WriteSystemLogEX(TRUE, "[ERROR] innBill CLoadLibrary::GetProcAddressw# no such functionName!!, hLibaray(0x%X) FuncName(%s) \r\n", m_hLibrary, i_szFunctionName);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mt_lock *CLoadLibrary::GetmtlockPtr(void)
{
	return &m_mtlock;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
/// \author		cmkwon
/// \date		2010-04-26 ~ 2010-04-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLoadLibrary::LoadProcList(void)
{
	return FALSE;		// 반드시 자식 클래스에서 구현
}