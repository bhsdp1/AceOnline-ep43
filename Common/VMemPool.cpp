// VMemPool.cpp: implementation of the CSaveLastError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VMemPool.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Global Variable/Function
//////////////////////////////////////////////////////////////////////
bool operator<(SObjectInfo lhObj, SObjectInfo rhObj)
{
	return lhObj.dwObjTypeSize < rhObj.dwObjTypeSize;
}

//////////////////////////////////////////////////////////////////////
//	Static Variable
//////////////////////////////////////////////////////////////////////
SObjectInfo	CVMemPool::ms_ArrayObjInfo[SIZE_MAX_VIRTUALPOOL_ARRAY];
DWORD		CVMemPool::ms_dwCountArrayObjInfo = 0;
CVMem		CVMemPool::ms_ArrayVMem[SIZE_MAX_VIRTUALPOOL_ARRAY];
DWORD		CVMemPool::ms_dwSizeArrayVMem = 0;
BOOL		CVMemPool::ms_bFlagVMemInit = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CVMemPool::CVMemPool()
{
}

CVMemPool::~CVMemPool()
{
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmPoolAddObject
// 반환되는 형  : BOOL
// 함 수 인 자  : DWORD dwObjTypeSize
// 함 수 인 자  : DWORD dwObjPoolSize
// 함 수 설 명  : static 함수
//					메로리 풀을 할당하기 전에 메모리를 할당할 Object의 타입과 사이즈를 추가하는 함수
BOOL CVMemPool::vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize)
{
	if(0 == dwObjTypeSize
		|| dwObjTypeSize%4 != 0
		|| SIZE_MAX_OBJECTTYPE < dwObjTypeSize 
		|| dwObjTypeSize * dwObjPoolSize > 1024 * 1024 * 100)
	{	// 유효성 체크
		// 요청한 Memory Pool 사이즈가 100 메가 보다 크면 FALSE를 리턴한다.
		
		char	szError[1024];
		sprintf(szError, "CVMemPool::vmPoolAddObject invalid parameter, ObjTypeSize[%4d] ObjPoolSize[%d]\r\n"
			, dwObjTypeSize, dwObjPoolSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	
	BOOL		bAddedFlag = FALSE;	
	for(DWORD i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		if(ms_ArrayObjInfo[i].dwObjTypeSize == dwObjTypeSize)
		{
			if(ms_ArrayObjInfo[i].dwObjTypeSize * (ms_ArrayObjInfo[i].dwObjPoolSize + dwObjPoolSize) > 1024*1024*100)
			{
				return FALSE;
			}
			ms_ArrayObjInfo[i].dwObjPoolSize += dwObjPoolSize;
			bAddedFlag = TRUE;
			break;
		}
	}

	if(FALSE == bAddedFlag && ms_dwCountArrayObjInfo < SIZE_MAX_VIRTUALPOOL_ARRAY - 1)
	{
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjTypeSize = dwObjTypeSize;
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjPoolSize = dwObjPoolSize;
		ms_dwCountArrayObjInfo++;
		bAddedFlag = TRUE;
		
		if(ms_dwCountArrayObjInfo > 1)
		{	// Object Info가 2개 이상일때 ObjectTypeSize를 기준으로 정렬한다.
			
			sort(&ms_ArrayObjInfo[0], &ms_ArrayObjInfo[ms_dwCountArrayObjInfo]);
		}
	}

	return bAddedFlag;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmPoolInit
// 반환되는 형  : BOOL
// 함 수 인 자  : void
// 함 수 설 명  : static 함수
//					메모리 풀을 위한 메모리를 할당한다.
BOOL CVMemPool::vmPoolInit(void)
{
	if(ms_bFlagVMemInit 
		|| !ms_dwCountArrayObjInfo 
		|| ms_dwCountArrayObjInfo >= SIZE_MAX_VIRTUALPOOL_ARRAY)
	{
		char	szError[1024];
		sprintf(szError, "CVMemPool::vmPoolInit invalid variable, FlagVMemInit[%2d] CountArrayObjInfo[%2d]\r\n"
			, ms_bFlagVMemInit, ms_dwCountArrayObjInfo);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	BOOL bRet;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		bRet = ms_ArrayVMem[i].AllocVMem(ms_ArrayObjInfo[i].dwObjTypeSize, ms_ArrayObjInfo[i].dwObjPoolSize);
		if(FALSE == bRet)
		{
			return FALSE;
		}
	}
	ms_dwSizeArrayVMem	= ms_dwCountArrayObjInfo;
	ms_bFlagVMemInit	= TRUE;

	return ms_bFlagVMemInit;
}



////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmPoolClean
// 반환되는 형  : void
// 함 수 설 명  : 메모리 풀의 메로리를 해제한다.
//
void CVMemPool::vmPoolClean()
{
	if(FALSE == ms_bFlagVMemInit)
	{
		char	szError[1024];
		sprintf(szError, "CVMemPool::vmPoolClean invalid variable, FlagVMemInit[%2d]\r\n"
			, ms_bFlagVMemInit);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);		
		return;
	}
	ms_bFlagVMemInit = FALSE;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		ms_ArrayVMem[i].FreeAllVMem();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-05-04 by cmkwon, 메모리풀 상태정보 로그 남기기 - 
/// \author		cmkwon
/// \date		2009-05-04 ~ 2009-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CVMemPool::vmPoolSaveCurrentStatus(void)
{
	for(DWORD i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		CVMem *pVMem = &ms_ArrayVMem[i];
		pVMem->SaveCurrentStatus();
	}	
}

