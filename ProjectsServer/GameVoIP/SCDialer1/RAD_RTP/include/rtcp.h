/*

NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever without 
written prior approval by RADVision LTD.

RADVision LTD. reserves the right to revise this publication and make changes 
without obligation to notify any person of such revisions or changes.

*/ 
#ifdef __cplusplus
extern "C" {
#endif


#ifndef __RTCP_H
#define __RTCP_H



#include <rvcommon.h>

#ifndef __RTP_H
DECLARE_OPAQUE(HRTCPSESSION);
#endif


#define ERR_RTCP_GENERALERROR     -350   /* general error */
#define ERR_RTCP_SSRCCOLLISION    -351   /* ssrc collision */
#define ERR_RTCP_ILLEGALSSRC      -352   /* an illegal ssrc was specified */
#define ERR_RTCP_ILLEGALSDES      -353   /* an illegal sdes was specified */
#define ERR_RTCP_ILLEGALPACKET    -354   /* illegal RTCP pkt encountered */



typedef BOOL (RVCALLCONV *LPSSRCENUM)(
        IN  HRTCPSESSION  hRTCP,
        IN  UINT32        ssrc);


typedef struct {
    BOOL    valid;
    UINT32  mNTPtimestamp;
    UINT32  lNTPtimestamp;
    UINT32  timestamp;
    UINT32  packets;
    UINT32  octets;
} RTCPSRINFO;

typedef struct {
    BOOL    valid;
    UINT32  fractionLost;
    UINT32  cumulativeLost;
    UINT32  sequenceNumber;
    UINT32  jitter;
    UINT32  lSR;
    UINT32  dlSR;

} RTCPRRINFO;

typedef struct {
    BOOL        selfNode;
    RTCPSRINFO  sr;
    RTCPRRINFO  rrFrom;
    RTCPRRINFO  rrTo;
    char        cname[255];
} RTCPINFO;



                      /* == Basic RTCP Functions == */


RVVXDAPI
INT32 VXDCALLCONV rtcpInit(void);

RVVXDAPI
INT32 VXDCALLCONV rtcpEnd(void);


RVVXDAPI
int VXDCALLCONV rtcpGetAllocationSize(
	IN  int sessionMembers);

RVVXDAPI
HRTCPSESSION VXDCALLCONV rtcpOpenFrom(
        IN  UINT32  ssrc,
        IN  UINT16  port,
        IN  char *  cname,
	IN  int	    sessionMembers,
	IN  void *  buffer,
	IN  int	    bufferSize);

RVVXDAPI
HRTCPSESSION VXDCALLCONV rtcpOpen(
        IN  UINT32  ssrc,
        IN  UINT16  port,
        IN  char *  cname);

RVVXDAPI
INT32 VXDCALLCONV rtcpClose(
        IN  HRTCPSESSION  hRCTP);

RVVXDAPI
void VXDCALLCONV rtcpSetRemoteAddress(
        IN  HRTCPSESSION  hRTCP,
        IN  UINT32        ip,
        IN  UINT16        port);

RVVXDAPI
INT32 VXDCALLCONV rtcpStop(
        IN  HRTCPSESSION  hRTCP);


RVVXDAPI
INT32 VXDCALLCONV rtcpRTPPacketRecv(
        IN  HRTCPSESSION  hRTCP,
        IN  UINT32        ssrc,
        IN  UINT32        myTimestamp,
        IN  UINT32        timestamp,
        IN  UINT16        sequence);

RVVXDAPI
INT32 VXDCALLCONV rtcpRTPPacketSent(
        IN  HRTCPSESSION  hRTCP,
        IN  INT32         bytes,
        IN  UINT32        timestamp);

RVVXDAPI
UINT16 VXDCALLCONV rtcpGetPort(
        IN  HRTCPSESSION  hRCTP);


                   /* == ENDS: Basic RTCP Functions == */



                    /* == Accessory RTCP Functions == */


RVVXDAPI
BOOL VXDCALLCONV rtcpCheckSSRCCollision(
        IN  HRTCPSESSION  hRTCP);

/* ouch! */
#define rtcpEnumParticipients  rtcpEnumParticipants

RVVXDAPI
BOOL VXDCALLCONV rtcpEnumParticipants(
        IN  HRTCPSESSION  hRTCP,
        IN  LPSSRCENUM    enumerator);

RVVXDAPI
INT32 VXDCALLCONV rtcpGetSourceInfo(
        IN   HRTCPSESSION hRTCP,
        IN   UINT32       ssrc,
        OUT  RTCPINFO*    info);	

RVVXDAPI
INT32 VXDCALLCONV rtcpSetGroupAddress(
        IN  HRTCPSESSION  hRTCP,
        IN  UINT32        ip);

RVVXDAPI
INT32 VXDCALLCONV rtcpGetSSRC(
        IN  HRTCPSESSION  hRTCP);

RVVXDAPI
INT32 VXDCALLCONV rtcpSetSSRC(
        IN  HRTCPSESSION  hRTCP,
        IN  UINT32        ssrc);

                 /* == ENDS: Accessory RTCP Functions == */

RVVXDAPI
INT32 VXDCALLCONV rtcpGetEnumNext(
                IN  HRTCPSESSION  hRTCP,
                IN  INT32         prev,
                IN  INT32 *       ssrc);


RVVXDAPI
INT32 VXDCALLCONV rtcpGetEnumFirst(
                IN  HRTCPSESSION  hRTCP,
                IN  INT32 *       ssrc);


#endif  /* __RTCP_H */
#ifdef __cplusplus
}
#endif
