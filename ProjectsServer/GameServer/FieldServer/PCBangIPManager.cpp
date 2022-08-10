// PCBangIPManager.cpp: implementation of the CPCBangIPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCBangIPManager.h"
#include "FieldGlobal.h"			// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 
#include "ODBCStatement.h"			// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCBangIPManager::CPCBangIPManager()
{

}

CPCBangIPManager::~CPCBangIPManager()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::IsPCBangIPClassCCheck(DWORD i_dwIPClassC)
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::IsPCBangIPClassCCheck(DWORD i_dwIPClassC)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(i_dwIPClassC);
	if (itr != m_mtmapPCBangIPList.end())
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::InitPCBangIPList()
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::InitPCBangIPList()
{
	mt_auto_lock mtAuto(&m_mtmapPCBangIPList);
	m_mtmapPCBangIPList.clear();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::InsertPCBangIPList(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)
/// \brief		
/// \author		dhjin
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::InsertPCBangIPList(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	if(this->IsPCBangIPClassCCheck(i_dwIPClassC))
	{
		this->InsertPCBangIPClassD(i_dwIPClassC, i_pIPClassD);
	}
	else
	{
		vectorSIPClassD	vectIPClassD;
		vectIPClassD.clear();
		vectIPClassD.push_back(*i_pIPClassD);
		m_mtmapPCBangIPList.insert(pair<DWORD, vectorSIPClassD>(i_dwIPClassC, vectIPClassD));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPCBangIPManager::InsertPCBangIPClassD(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)	
/// \brief		PC방 IP D클래스 추가
/// \author		dhjin
/// \date		2007-08-06 ~ 2007-08-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPCBangIPManager::InsertPCBangIPClassD(DWORD i_dwIPClassC, SIPClassD* i_pIPClassD)		
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(i_dwIPClassC);
	if (itr != m_mtmapPCBangIPList.end())
	{
		itr->second.push_back(*i_pIPClassD);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPCBangIPManager::SetPCBangIPList(mtmapDWORDvectorSIPClassD * i_pPCBangIPList)
/// \brief		PC방 IP를 설정 요청 처리
/// \author		dhjin
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPCBangIPManager::SetPCBangIPList(mtmapDWORDvectorSIPClassD * i_pPCBangIPList)
{
	mt_auto_lock mtA(&m_mtmapPCBangIPList);
	m_mtmapPCBangIPList.clear();

	mtmapDWORDvectorSIPClassD::iterator itr = i_pPCBangIPList->begin();
	for ( ; itr != i_pPCBangIPList->end() ; itr++)	
	{
		m_mtmapPCBangIPList.insert(pair<DWORD, vectorSIPClassD>(itr->first, itr->second));
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPCBangIPManager::IsPCBangIPCheck(char *i_szClientIP, UID32_t *o_PCBangUID)
/// \brief		
/// \author		dhjin
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPCBangIPManager::IsPCBangIPCheck(char *i_szClientIP, UID32_t *o_PCBangUID)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2009-07-20 by cmkwon, 예당 PCBang 체크 시스템 수정 - 
	if(LANGUAGE_TYPE_DEFAULT == g_pFieldGlobal->GetLanguageType()
		&& FALSE == g_pFieldGlobal->InMasangServer())		// 2009-08-12 by cmkwon, 마상소프트 내의 서버 제외
	{
		return FALSE;		// 2012-10-06 by hskim, 한국 자체 서비스 - 자체 서비스시 예당 PC방 모듈 사용 안함

		*o_PCBangUID			= 0;

		CODBCStatement odbcStmt;
		const char	*szIP		= "192.168.2.39";
		int			nPort		= 1456;
		const char	*szDatabase	= "unipidoriMember";
		const char *szUserID	= "masanguspip";
		const char *szPass		= "masang##ip!!";

		BOOL bRet = odbcStmt.Init(szIP, nPort, szDatabase, szUserID, szPass, g_pFieldGlobal->GetMainWndHandle());
		if(FALSE == bRet)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB Error] CPCBangIPManager::IsPCBangIPCheck connect error !! DBServer(%s:%d) DatabaseName(%s) UserID(%s) PWD(%s) \r\n"
				, szIP, nPort, szDatabase, szUserID, szPass);
			return FALSE;
		}

		const int nIPSize = 16;
		wchar_t wcClientIP[nIPSize];
		MEMSET_ZERO(wcClientIP, sizeof(wcClientIP[0])*nIPSize);
		MultiByteToWideChar(CODE_PAGE, 0, i_szClientIP, -1, wcClientIP, nIPSize);

		SQLHSTMT hstmt = odbcStmt.GetSTMTHandle();
		SQLINTEGER cb = SQL_NTS;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, nIPSize, 0, wcClientIP, 0,		&cb);
		bRet = odbcStmt.ExecuteQuery(PROCEDURE_090720_0397);
		if(FALSE == bRet)
		{
			odbcStmt.ProcessLogMessagesForStmt("[DB ERROR] ExecuteQuery(PROCEDURE_090720_0397@) Error !!\r\n");
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DB Error] BillingReqExecuteDirectConnect ExecuteQuery(PROCEDURE_090720_0397@) error !! %s \r\n"
				, i_szClientIP);
			return FALSE;
		}
		cb = SQL_NTS;
		SQLBindCol(hstmt, 1, SQL_C_LONG, o_PCBangUID, 0,			&cb);
		while(TRUE)
		{
			SQLRETURN ret = SQLFetch(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
		}
		odbcStmt.FreeStatement();

		if(0 == *o_PCBangUID)
		{
			return FALSE;
		}
		return TRUE;
	}


	mt_auto_lock mtA(&m_mtmapPCBangIPList);		// 2007-01-24 by cmkwon

	DWORD dwClientIP = inet_addr(i_szClientIP);
	BYTE  byDClassIP = (dwClientIP >> 24 ) & 0xFF;
	dwClientIP &= 0x00FFFFFF;

	mtmapDWORDvectorSIPClassD::iterator itr = m_mtmapPCBangIPList.find(dwClientIP);
	if(itr != m_mtmapPCBangIPList.end())
	{
		vectorSIPClassD::iterator itrClassD = itr->second.begin();
		for( ; itrClassD != itr->second.end() ; itrClassD++)
		{
			if(IS_IN_RANGE(itrClassD->IPClassDStart, byDClassIP, itrClassD->IPClassDEnd))
			{
				*o_PCBangUID = itrClassD->PCBangUID;
				return TRUE;
			}
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtmapDWORDvectorSIPClassD* CPCBangIPManager::GetPCBangIPList()
/// \brief		
/// \author		dhjin
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtmapDWORDvectorSIPClassD* CPCBangIPManager::GetPCBangIPList()
{
	return &m_mtmapPCBangIPList;
}