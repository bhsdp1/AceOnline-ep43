// VMemPool.h: interface for the CVMemMan class.
//
//	//-- CMKWON 2003-05-23 --//
//	- vmPoolAddObject(), vmPoolInit(), vmPoolClean() 함수는 동기화 처리되지 않음, 자체 적으로 동기화 처리를 해야함
//	- 객체를 생성하지 못함, 객체의 생성없이 static함수만으로 동작됨
//	-
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMEMPOOL_H__BD52675B_6C82_4CDE_8618_0141D7A4653F__INCLUDED_)
#define AFX_VMEMPOOL_H__BD52675B_6C82_4CDE_8618_0141D7A4653F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VMem.h"

const DWORD	SIZE_MAX_VIRTUALPOOL_ARRAY			= 12;
const DWORD SIZE_MAX_OBJECTTYPE					= 65536;	// 최대 Object Buffer Size 65536 Bytes

typedef struct
{
	DWORD	dwObjTypeSize;
	DWORD	dwObjPoolSize;
} SObjectInfo;

class CVMemPool
{
private:
	CVMemPool();
	~CVMemPool();

public:
	static BOOL vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize = 1024);
	static BOOL vmPoolInit(void);
	static void vmPoolClean(void);
	static void vmPoolSaveCurrentStatus(void);		// 2009-05-04 by cmkwon, 메모리풀 상태정보 로그 남기기 - 

	inline static void* vmObjectNew(DWORD dwObjSize);
	inline static void vmObjectDelete(void *p, DWORD dwObjSize);

private:
	inline static CVMem* vmSearchValidVMem(DWORD dwObjSize);

private:
	static SObjectInfo		ms_ArrayObjInfo[SIZE_MAX_VIRTUALPOOL_ARRAY];	// 메모리 풀을 사용할 Object 정보 배열
	static DWORD			ms_dwCountArrayObjInfo;							// 메모리 풀을 사용할 Object 정보 배열 카운트
	static CVMem			ms_ArrayVMem[SIZE_MAX_VIRTUALPOOL_ARRAY];		// 메모리 풀 배열
	static DWORD			ms_dwSizeArrayVMem;								// 사용중인 메모리 풀 배열 사이즈
	static BOOL				ms_bFlagVMemInit;								// 메모리풀 초기화 플래그
};

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmObjectNew
// 반환되는 형  : void *
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리풀에서 사이즈에 맞는 메모리를 할당하고 그 주소를 리턴한다.
void * CVMemPool::vmObjectNew(DWORD dwObjSize)
{
	CVMem *pVMem = vmSearchValidVMem(dwObjSize);
	if(pVMem)
	{
		return pVMem->ObjectNew();
	}

	char	szError[1024];
	sprintf(szError, "CVMemPool::vmObjectNew error, FlagVMemInit[%2d] SizeArrayVMem[%2d] ObjSize[%4d]\r\n"
		, ms_bFlagVMemInit, ms_dwSizeArrayVMem, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmObjectDelete
// 반환되는 형  : void
// 함 수 인 자  : void *p
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리풀에서 인자로 받은 주소를 해제한다.
void CVMemPool::vmObjectDelete(void *p, DWORD dwObjSize)
{
	CVMem *pVMem = vmSearchValidVMem(dwObjSize);
	if(pVMem)
	{
		pVMem->ObjectDelete(p);
		return;
	}

	char	szError[1024];
	sprintf(szError, "CVMemPool::vmObjectDelete error, FlagVMemInit[%2d] SizeArrayVMem[%2d] p[0x%X] ObjSize[%4d]\r\n"
		, ms_bFlagVMemInit, ms_dwSizeArrayVMem, p, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMemPool::vmSearchValidVMem
// 반환되는 형  : CVMem*
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리 풀에서 ObjectTypeSize에 맞는 CVMem의 객체를 찾아서 리턴하는 함수
CVMem* CVMemPool::vmSearchValidVMem(DWORD dwObjSize)
{
	DWORD i;
	for(i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		if(ms_ArrayObjInfo[i].dwObjTypeSize == dwObjSize)
		{
			return &ms_ArrayVMem[i];
		}
	}

	char	szError[1024];
	char	szTemp[512];
	sprintf(szError, "CVMemPool::vmSearchValidVMem error, FlagVMemInit[%2d] SizeArrayVMem[%2d],"
		, ms_bFlagVMemInit, ms_dwSizeArrayVMem);
	for(i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		sprintf(szTemp, " %2d[%4d]", ms_ArrayObjInfo[i].dwObjTypeSize);
		strcat(szError, szTemp);
	}
	strcat(szError, "\r\n");
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}


#endif // !defined(AFX_VMEMPool_H__BD52675B_6C82_4CDE_8618_0141D7A4653F__INCLUDED_)
