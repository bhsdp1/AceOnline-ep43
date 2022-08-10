///////////////////////////////////////////////////////////////////////////////
//  DefineGlobal.h : 프로젝트와 상관없이 기본적으로 적용되는 #define
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEFINE_GLOBAL_H_
#define _DEFINE_GLOBAL_H_

#include "DebugAssert.h"
#include <math.h>
#include <process.h>

///////////////////////////////////////////////////////////////////////////////
/// 프로토콜 기술을 위해 사용, c++ code로서의 의미는 없다
#define _ARRAY(x)
#define ARRAY_(x)			/// x가 n개 있는 것
#define STRUCT_(x)			/// x라는 struct 하나

#define LODWORD(ll)			((DWORD)(ll))
#define HIDWORD(ll)			((DWORD)(((UINT64)(ll) >> 32) & 0xFFFFFFFF))

// RAND_MAX는 0x7FFF이므로 그 이상이 필요할 때는 이놈을 사용하기, 20040708, kelovon
inline int rand32()
{
	int i1, i2, i3;
	i1 = rand()&0x0FFF;
	i2 = (rand()&0x0FFF) << 12;
	i3 = (rand()&0x007F) << 24;
	return (i1|i2|i3);
}

#define RANDF1(min, max)	((float)((min)*10 + (rand32() % (int)(((max)-(min)+0.1)*10)))/10)		// float, 소수점 이하 1자리, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RANDF2(min, max)	((float)((min)*100 + (rand32() % (int)(((max)-(min)+0.01)*100)))/100)	// float, 소수점 이하 2자리, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RANDI(min, max)		((min) + (rand32() % ((max)-(min)+1)))			// int, (min) ~ (max), [min, max], 주의: max값 포함됨
#define RAND100()			(rand32() % 100)								// int, 0 ~ 99
#define RAND1000()			(rand32() % 1000)								// int, 0 ~ 999
#define RAND10K()			(rand32() % 10000)								// int, 0 ~ 9999
#define RAND100K()			(rand32() % 100000)								// int, 0 ~ 99999
#define RAND1000K()			(rand32() % 1000000)							// int, 0 ~ 999999
#define RAND256()			(rand32() % 256)								// int, 0 ~ 255

#define IS_IN_RANGE(_MIN, _VALUE, _MAX)		((_MIN) <= (_VALUE) && (_MAX) >= (_VALUE))

#define LENGTH(_X, _Y)						sqrt((double)((_X)*(_X)+(_Y)*(_Y)))

#define MAX_INT64_VALUE			((INT64)0x7FFFFFFFFFFFFFFF)
#define MAX_UINT64_VALUE		((UINT64)0xFFFFFFFFFFFFFFFF)
#define MAX_INT_VALUE			((INT)0x7FFFFFFF)
#define MAX_UINT_VALUE			((UINT)0x7FFFFFFF)
#define MAX_SHORT_VALUE			((SHORT)0x7FFF)
#define MAX_USHORT_VALUE		((USHORT)0xFFFF)
#define MAX_BYTE_VALUE			((BYTE)0x7F)
#define MAX_RAND100_VALUE		99				// int, 0 ~ 99
#define MAX_RAND1000_VALUE		999				// int, 0 ~ 999
#define MAX_RAND10K_VALUE		9999			// int, 0 ~ 9999
#define MAX_RAND100K_VALUE		99999			// int, 0 ~ 99999
#define MAX_RAND1000K_VALUE		999999			// int, 0 ~ 999999
#define MAX_RAND256_VALUE		255				// int, 0 ~ 255

// 2012-10-10 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#define BONUS_EXP_RATE_PARTY_COUNT				0.1f
#define BONUS_EXP_RATE_PARTY_DEFAULT			1.2f
// end 2012-10-10 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

#define SAFE_DELETE(p) { if(NULL != p){ delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) { if(NULL != p){ delete[] (p); (p) = NULL;}}
#define SAFE_DELETE_DESTROY(p) { if(NULL != p) { p->Destroy(); delete (p); (p) = NULL; }}		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

#define IS_VALID_ARRAY_INDEX(idx, ArraySize) (((idx) >= 0) && ((idx) < (ArraySize)))
#define MEMSET_ZERO(p, Size) (memset((p), 0x00, (Size)))

///////////////////////////////////////////////////////////////////////////////
// 2006-05-24 by cmkwon, szDest를 초기화 하고 복사한다.
#define STRNCPY_MEMSET(szDest, szSource, MaxSize)		{ MEMSET_ZERO((szDest), (MaxSize));	\
														  strncpy( (szDest), (szSource), (MaxSize)-1 );}

#define GET_CASERETURN_STRING_BY_TYPE(str)	case str:		return #str ""


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
#ifdef _ATUM_SERVER
inline char* replace(char *io_szStr, char i_OldChar, char i_NewChar)
{
	if (i_OldChar != i_NewChar)
	{
		char * psz		= io_szStr;
		char * pszEnd	= io_szStr + strlen(io_szStr);
		while (psz < pszEnd)
		{
			if (*psz == i_OldChar)
			{
				*psz = i_NewChar;
			}
			psz = CharNext(psz);
		}
	}
	return io_szStr;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-04 by cmkwon, atof관련 문제 해결(언어별 소숫점 구분자 변환) - 
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
inline float atof_AfterChangeCommaToDot(char *i_szStr)
{
	char szTemp[1024];
	STRNCPY_MEMSET(szTemp, i_szStr, 1024);

	replace(szTemp, ',', '.');

	return (float)atof(szTemp);
}
#endif // _ATUM_SERVER

struct Delete_Object
{
	template<typename T>
		void operator()(T *ptr)
	{
		delete ptr;
	}
};

struct Delete_Object_MAP
{
	template<typename T>
		void operator()(T ptr)
	{
		delete ptr.second;
	}
};


#ifndef chBEGINTHREADEX
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadID)                 \
      ((HANDLE) _beginthreadex(                     \
         (void *) (psa),                            \
         (unsigned) (cbStack),                      \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *) (pvParam),                        \
         (unsigned) (fdwCreate),                    \
         (unsigned *) (pdwThreadID)))
#endif

#endif	// _DEFINE_GLOBAL_H_endif