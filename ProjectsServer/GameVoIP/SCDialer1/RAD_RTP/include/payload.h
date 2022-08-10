#ifdef __cplusplus
extern "C" {
#endif



/*

NOTICE:
This document contains information that is proprietary to RADVision LTD.
No part of this publication may be reproduced in any form whatsoever without 
written prior approval by RADVision LTD.

RADVision LTD. reserves the right to revise this publication and make changes 
without obligation to notify any person of such revisions or changes.

*/ 

#ifndef __PAYLOAD_H
#define __PAYLOAD_H



typedef enum
{
    PCMU  =  0,	
    G7231 =  4,			
    PCMA  =  8,
    G722  =  9, 
    G728  = 15,
    G729  = 18,
    H261  = 31,
    H263  = 34
} payload_e;



/* == G.711 U-Law == */

RVVXDAPI
INT32 VXDCALLCONV rtpPCMUPack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpPCMUUnpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpPCMUGetHeaderLength(void);


/* == G.711 A-Law == */

RVVXDAPI
INT32 VXDCALLCONV rtpPCMAPack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpPCMAUnpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpPCMAGetHeaderLength(void);


/* == G.722 == */

RVVXDAPI
INT32 VXDCALLCONV rtpG722Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);


RVVXDAPI
INT32 VXDCALLCONV rtpG722Unpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpG722GetHeaderLength(void);

/* == G.728 == */

RVVXDAPI
INT32 VXDCALLCONV rtpG728Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);


RVVXDAPI
INT32 VXDCALLCONV rtpG728Unpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpG728GetHeaderLength(void);

/* == G.729 == */

RVVXDAPI
INT32 VXDCALLCONV rtpG729Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);


RVVXDAPI
INT32 VXDCALLCONV rtpG729Unpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpG729GetHeaderLength(void);


/* == G.723.1 == */
RVVXDAPI
INT32 VXDCALLCONV rtpG7231Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpG7231Unpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpG7231GetHeaderLength(void);


/* == H.261 == */

typedef struct
{
    int sBit;
    int eBit;
    int i;
    int v;
    int gobN;
    int mbaP;
    int quant;
    int hMvd;
    int vMvd;
} H261param;

RVVXDAPI
INT32 VXDCALLCONV rtpH261Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpH261Unpack(
        OUT  void*buf,
        IN   INT32       len,
        OUT  rtpParam*p,
        OUT  void *      param);

RVVXDAPI
INT32 VXDCALLCONV rtpH261GetHeaderLength(void);


/* == H.263 == */

typedef struct
{
    int f;
    int p;
    int sBit;
    int eBit;
    int src;
    int i;
    int a;
    int s;
    int dbq;
    int trb;
    int tr;
    int gobN;
    int mbaP;
    int quant;
    int hMv1;
    int vMv1;
    int hMv2;
    int vMv2;
} H263param;

RVVXDAPI
INT32 VXDCALLCONV rtpH263Pack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpH263Unpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);


RVVXDAPI
INT32 VXDCALLCONV rtpH263GetHeaderLength(void);



/* == H.263a == */

typedef struct
{
    int f;
    int p;
    int sBit;
    int eBit;
    int src;
    int i;
    int u;
    int a;
    int s;
    int dbq;
    int trb;
    int tr;
    int gobN;
    int mbaP;
    int quant;
    int hMv1;
    int vMv1;
    int hMv2;
    int vMv2;
} H263aparam;

RVVXDAPI
INT32 VXDCALLCONV rtpH263aPack(
        IN  void *      buf,
        IN  INT32       len,
        IN  rtpParam *  p,
        IN  void     *  param);

RVVXDAPI
INT32 VXDCALLCONV rtpH263aUnpack(
        OUT  void *      buf,
        IN   INT32       len,
        OUT  rtpParam *  p,
        OUT  void *      param);


RVVXDAPI
INT32 VXDCALLCONV rtpH263aGetHeaderLength(void);




#endif  /* __PAYLOAD_H */
#ifdef __cplusplus
}              
#endif



