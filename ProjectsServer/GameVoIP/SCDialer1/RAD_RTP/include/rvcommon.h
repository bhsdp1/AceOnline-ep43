/*

NOTICE:
This document contains information that is proprietary to RADVision LTD..
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.

*/

#ifndef __RVCOMMON_H
#define __RVCOMMON_H

/*#define _EXAMINE_*/
#ifdef __RV_SYS
    #define CDECL  __cdecl
#else
    #ifdef __RV_VXD
        #define CDECL  __cdecl
    #else
        #define CDECL

    #endif
#endif

#ifdef __RV_SYS		// Win NT sys drive

   #include <ntddk.h>
   #include <time.h>
   #define printf  DbgPrint

#else
#ifdef __RV_VXD

   #pragma pack(8)

   #ifndef VToolsD    // Not using VToolsD
      #include <memory.h>
      #include <malloc.h>
      #include <string.h>
      #include <stdio.h>
      #include <stdlib.h>
      #include <time.h>
   #else
      #include <vtoolsc.h>
      #define KdPrint(_x_)   dprintf _x_
   #endif

   //#define IS_32
   #define BLD_COFF

   #include <basedef.h>
   #include <vmm.h>

   //#pragma VxD_LOCKED_CODE_SEG
   //#pragma VxD_LOCKED_DATA_SEG

   #pragma intrinsic(memcmp, memcpy, memset, strcat, strcmp, strcpy, strlen)
   #define printf_		printf

#else
   #include <memory.h>
   #include <malloc.h>
   #include <string.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <time.h>
#endif
#endif /* __RV_VXD */

#include <rvstd.h>

#define WeAreRunningWindows
/*#define NOFILESYSTEM*/

typedef unsigned long   UINTPTR;
typedef   signed long   INTPTR;

typedef unsigned long	ULONG;
typedef unsigned long	DWORD;
typedef unsigned char	UCHAR;
typedef unsigned short	u_short;

typedef unsigned int	UINT32;
typedef unsigned short	UINT16;
typedef unsigned char	UINT8;
typedef signed   int	INT32;
typedef signed   short	INT16;
typedef signed   char	INT8;

typedef unsigned int	UINT;
typedef signed   int	INT;

typedef unsigned char   BYTE;

#ifndef TRUE
#define TRUE  1
typedef unsigned  BOOL;
#endif

#ifndef FALSE
#define FALSE 0
#endif



#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif


#define bcopy(source, dest, size) memcpy((dest),   (source), ((UINT16)size))
#define bzero(buffer, size)       memset((buffer), 0,        ((UINT16)size))

#ifdef __RV_VXD
  #define RVAPI
  #define RVCALLCONV   __cdecl
  #define CALLCONVC   __cdecl
#else
#ifdef __RV_SYS
  #define RVAPI
  #define RVCALLCONV   __cdecl
  #define CALLCONVC   __cdecl
  #define free(_pMem_)    ExFreePool(_pMem_)

  #define calloc(_len_, _cnt_)    memset(ExAllocatePool(NonPagedPool,_len_*_cnt_), 0, _len_*_cnt_)

  #define malloc(_size_)  ExAllocatePool(NonPagedPool,_size_)
  #define free(_pMem_)    ExFreePool(_pMem_)
  typedef unsigned  BOOL;

#else
#if defined(WIN32) || defined(UNDER_CE)
/*
#define malloc(size)  memfAlloc(size, __FILE__, __LINE__)
#define calloc(size, elemSize)  memfCalloc(size, (elemSize), __FILE__, __LINE__)

#define free(p)  memfFree((p))
*/


void *memfAlloc(int size, const char *name, int lineno);

void *memfCalloc(int size, int elemSize, const char *name,  int lineno);

void  memfFree(void *ptr);

void  memfView(void);

void  memfEnd(void);


  #define RVAPI      __declspec(dllexport)
  #define RVCALLCONV   __stdcall
  #define CALLCONVC   __cdecl


#if defined(NOTHREADS) && defined(M_INSTANCE)
  #define RV_TLS_ALWAYS __declspec(thread)
  #define RV_TLS __declspec(thread)
#else
#if !defined(NOTHREADS) && !defined(M_INSTANCE)
  #define RV_TLS_ALWAYS __declspec(thread)
#else
  #define RV_TLS_ALWAYS
#endif
  #define RV_TLS
#endif

#else
  #define RVAPI
  #define RVCALLCONV far pascal __export
  #define CALLCONVC __cdecl
  #define printf
#endif /* WIN32 */
#endif /* __RV_SYS */
#endif /* __RV_VXD */

#ifndef RVVXDAPI
  #define RVVXDAPI     RVAPI
  #define VXDCALLCONV  RVCALLCONV
#endif
/*#define CALL_TIME*/
#ifdef CALL_TIME
typedef struct
{
        __int64 time;
        int number;
        int param;
        int len;
        char funct[20];

}callTimeStructT;
typedef struct
{
        callTimeStructT callTimeStruct[2000];
        int callTimeNum;
}callTimeT;
__int64 callTimeGetTime() ;
__int64  callTimeGetFr();
extern callTimeT callTime;
#endif

#ifdef WIN32
#pragma warning (disable : 4706 4514 4310 4127 4206)
#endif

#ifndef NONOTRVDEFS
#define CALLCONV RVCALLCONV
#ifdef ERROR
#undef ERROR
#endif
#define ERROR RVERROR
#endif

typedef struct {
    UINT vtNodeCount;
    UINT channels;
    UINT chanDescs;
    UINT messages;
    UINT tpktChans;
    UINT tpktNoBuffers;
    UINT protocols;
    UINT maxProcs;
    UINT maxBuffSize;
	UINT useAnnexE;
	UINT t_R1;
	UINT t_R2;
	UINT n_R1;
	UINT t_IMA1;
	UINT n_IMA1;
	UINT t_DT;
} globalInitNumbers;

#endif  /* __RVCOMMON_H */
