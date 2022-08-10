/*

NOTICE:
This document contains information that is proprietary to RADVision LTD..
No part of this publication may be reproduced in any form whatsoever without 
written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make changes 
without obligation to notify any person of such revisions or changes.

*/ 
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RTP_H
#define __RTP_H



#include <rvcommon.h>

#ifndef CALLCONVC
#define CALLCONVC __cdecl
#endif

DECLARE_OPAQUE(HRTPSESSION);

#ifndef __RTCP_H    
DECLARE_OPAQUE(HRTCPSESSION);
#endif


typedef struct {
    IN OUT  UINT32  timestamp;
    IN OUT  BOOL    marker;
    IN OUT  BYTE    payload;
    
    OUT     UINT32  sSrc;
    OUT     UINT16  sequenceNumber;
    OUT     int     sByte;
    OUT     int     len;
} rtpParam;


typedef void (CALLCONVC *LPRTPEVENTHANDLER)
    (
        IN  HRTPSESSION  hRTP,
        IN  void *       context
    );



typedef struct 
{
    int				   isAllocated;
    int                socket;
    UINT32             sSrc;
    UINT32             sSrcMask;
    UINT32             sSrcPattern;
    LPRTPEVENTHANDLER  eventHandler;
    void *             context;
    UINT16             sequenceNumber;
    UINT32             ip;
    UINT16             port;
    BOOL               useSequenceNumber;
    HRTCPSESSION       hRTCP;
#ifdef _ATM_
    UINT8 *            atmAddr;         /* remote ATM Address (local insocket)
    UINT32             addrLength;       ATM Address length
    UINT16             selector;         STM selector*/
    BOOL               bUseATM;
#endif
} rtpSession;		/* HRTPSESSION */ 

                       /* == Basic RTP Functions == */


RVVXDAPI
INT32 VXDCALLCONV rtpInit(void);

RVVXDAPI
void VXDCALLCONV rtpEnd(void);

RVVXDAPI
int VXDCALLCONV rtpGetAllocationSize(void);

RVVXDAPI
HRTPSESSION VXDCALLCONV rtpOpenFrom(
        IN  UINT16  port,
        IN  UINT32  ssrcPattern,
        IN  UINT32  ssrcMask,
	IN  void*   buffer,
	IN  int	    bufferSize);
	
RVVXDAPI
HRTPSESSION VXDCALLCONV rtpOpen(
        IN  UINT16  port,
        IN  UINT32  ssrcPattern,
        IN  UINT32  ssrcMask);

RVVXDAPI
HRTPSESSION VXDCALLCONV rtpOpenEx(
        IN  UINT16  port,
        IN  UINT32  ssrcPattern,
        IN  UINT32  ssrcMask,
        IN  char *  cname);

RVVXDAPI
UINT32 VXDCALLCONV rtpClose(
        IN  HRTPSESSION  hRTP);

RVVXDAPI
UINT32 VXDCALLCONV rtpGetSSRC(
        IN  HRTPSESSION  hRTP);

RVVXDAPI
void VXDCALLCONV rtpSetEventHandler(
        IN  HRTPSESSION        hRTP,
        IN  LPRTPEVENTHANDLER  eventHandler,
        IN  void *             context);

RVVXDAPI
void VXDCALLCONV rtpSetRemoteAddress(
        IN  HRTPSESSION  hRTP,
        IN  UINT32       ip,
        IN  UINT16       port);
   
RVVXDAPI
INT32 VXDCALLCONV rtpPack(
        IN  HRTPSESSION  hRTP,
        IN  void *       buf,
        IN  INT32        len,
        IN  rtpParam *   p);

RVVXDAPI
INT32 VXDCALLCONV rtpWrite(
        IN  HRTPSESSION  hRTP,
        IN  void  *      buf,
        IN  INT32        len,
        IN  rtpParam *   p);

RVVXDAPI
INT32 VXDCALLCONV rtpUnpack(
        IN  HRTPSESSION  hRTP,
        IN  void *buf,
        IN  INT32 len,
        OUT rtpParam* p);

RVVXDAPI
INT32 VXDCALLCONV rtpRead(
        IN   HRTPSESSION  hRTP,
        IN   void *       buf,
        IN   INT32        len,
        OUT  rtpParam *   p);

RVVXDAPI
INT32 VXDCALLCONV rtpReadEx(
        IN   HRTPSESSION  hRTP,
        IN   void *       buf,
        IN   INT32        len,
        IN   UINT32       timestamp,
        OUT  rtpParam *   p);

RVVXDAPI
UINT16 VXDCALLCONV rtpGetPort(
        IN  HRTPSESSION  hRTP);


RVVXDAPI
char * VXDCALLCONV rtpGetVersion(void);

RVVXDAPI
UINT32 VXDCALLCONV rtpGetVersionNum(void);


                    /* == ENDS: Basic RTP Functions == */



                     /* == Accessory RTP Functions == */


RVVXDAPI
HRTCPSESSION VXDCALLCONV rtpGetRTCPSession(
        IN  HRTPSESSION  hRTP);


RVVXDAPI
INT32 VXDCALLCONV rtpSetRTCPSession(
        IN  HRTPSESSION   hRTP,
        IN  HRTCPSESSION  hRTCP);

RVVXDAPI
INT32 VXDCALLCONV rtpGetHeaderLength(void);

RVVXDAPI
UINT32 VXDCALLCONV rtpRegenSSRC(
        IN  HRTPSESSION  hRTP);

RVVXDAPI
INT32 VXDCALLCONV rtpSetGroupAddress(
        IN  HRTPSESSION  hRTP,
        IN  UINT32       ip);

RVVXDAPI
INT32 VXDCALLCONV rtpResume(
        IN  HRTPSESSION  hRTP);

RVVXDAPI 
INT32 VXDCALLCONV rtpUseSequenceNumber(
        IN  HRTPSESSION  hRTP);

RVVXDAPI 
INT32 VXDCALLCONV rtpSetReceiveBufferSize(
                IN HRTPSESSION  hRTP,
		IN int size);

RVVXDAPI 
INT32 VXDCALLCONV rtpSetTrasmitBufferSize(
                IN HRTPSESSION  hRTP,
		IN int size);

RVVXDAPI 
INT32 VXDCALLCONV rtpSetTransmitBufferSize(
                IN HRTPSESSION  hRTP,
		IN int size);

RVVXDAPI 
INT32 VXDCALLCONV rtpGetAvailableBytes(
                IN HRTPSESSION  hRTP);

               /* == ENDS: Accessory RTP Functions == */




#endif  /* __RTP_H */
#ifdef __cplusplus
}
#endif
