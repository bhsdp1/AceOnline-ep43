/* name and version number:	@(#)sx5363api.h	1.13 	*/
/* date of get:			5/1/98 17:04:06	*/
/* date of delta:		12/8/97 11:15:56 */

/***************************************************************************/
/* File Name: s5363api.h                                                   */
/* Function: defines the application programming interface (API) of        */
/*           the SX5363S G.723.1 Compliant Speech coder                    */
/*      Copyright (C) 1995-1996 Lucent Technologies. All Rights Reserved.  */
/***************************************************************************/

#ifndef __sx5363api__
#define __sx5363api__

#ifdef __MWERKS__
#include "windows.h"
#else
#include <windows.h>
#endif

#define SX5363_FRAMESIZE            240
#define SX5363_FRAMESIZE_4KBPS      320			
#define SX53_PACKSIZE               20
#define SX63_PACKSIZE               24

/*
 * Feature flags for the SX5363S coder
 */

#define SX_MODE_LOW_COMPLEXITY      (1<<0)	/* Use low complexity (non-compliant) algorithm */
#define SX_MODE_USE_MMX             (1<<1)      /* Use MMX if available on platform (Don't care if not available) */
#define SX_MODE_4KBPS               (1<<2)	/* Use 4.0 and 4.8 kbps mode instead of 5.3 and 6.4 kbps mode */
					

/* 
 * type definition used specifically for SX5363S coder
 */

#ifdef SX_WIN16
typedef void FAR *                  HCODEC;
typedef short                       SHORT;
typedef short FAR *                 LPSHORT;	/* speech buffer */
typedef char                        CHAR;
#else
typedef HANDLE                      HCODEC;
typedef SHORT FAR *                 LPSHORT;	/* speech buffer */
#endif

typedef HCODEC FAR*                 LPHCODEC;

typedef unsigned long               SXRESULT;	/* status codes */


/*							 
 * codec status codes - SXRESULT
 */
#define SX_OK               0x00          /* no error */
#define SX_ERR_OPEN         0x01          /* fail to open the codec  */
#define SX_ERR_BADPNT       0x02          /* bad data pointer */
#define SX_ERR_BADPARA      0x03          /* bad input parameter */
#define SX_ERR_EXPIRE       0x04          /* codec expired */
#define SX_ERR_NOINI        0x05          /* .ini file not found or invalid */

/*
 *	Microsoft Visual C++ specific interface: for 16-bit and 32-bit DLLs
 */ 

#ifdef _WIN32
#define SXAPI		WINAPI			  /* for 32-bit DLL function API */
#else
#define SXAPI		FAR PASCAL			  /* for 16-bit DLL function API */
#endif

/*
 * API function prototypes 
 */

/***************************************************************************
   Name: 	   SX5363_Open(), SX5363_InitDecod(), SX5363_InitEncod(), 
                   SX5363_Close()

   Function:   Open one instance of the speech codec and performs all necessary 
               initialization for the speech codec.

               SX5363_Open() Must be called once and only once for 
               encoding/decoding each speech stream before using other 
               codec functions.

               SX5363_InitDecod() and SX5363_InitEncod() merely clear
               decoder and encoder structures and may be called as often
               as necessary.

               SX5363_Close() must be called at coder completion. Do not call
               any other codec functions (except SX5363_Open) after this
               call as memory is deallocated.

   Parameters: HCODEC (output) -- a speech codec handle allocated.   
   Return:     Execution status code of 32-bit integer              
 ***************************************************************************/
SXRESULT SXAPI  SX5363_Open(LPHCODEC lphCodec);
typedef SXRESULT (WINAPI * PSX5363OPEN)(LPHCODEC lphCodec);

SXRESULT SXAPI  SX5363_InitDecod(HCODEC hCodec);
typedef SXRESULT (WINAPI * PSX5363INITDEC)(HCODEC hCodec);

SXRESULT SXAPI  SX5363_InitEncod(HCODEC hCodec);
typedef SXRESULT (WINAPI * PSX5363INITENC)(HCODEC hCodec);

SXRESULT SXAPI  SX5363_Close(LPHCODEC lphCodec);
typedef SXRESULT (WINAPI * PSX5363CLOSE)(LPHCODEC lphCodec);

/***************************************************************************
   Name:       SX53_SpeechEncode(), SX63_SpeechEncode()
   Function:   encodes a 16-bit PCM speech frame of SX5363_FRAMESIZE samples 
   	       into SX5363_PACKSIZE bytes of compressed data frame.      
   Parameters: hCodec (input) -- the instance handle of the speech codec for
                                 the current speech stream.
               Features (input) -- Additional features supported by the SX5363S.
               lpSpeechBuffer (input) -- a long pointer to the speech frame.
                                         that contains the SX5363_FRAMESIZE 16-bit PCM samples to 
                                         be compressed. The input data are returned unchanged.
               lpBitStream (output) -- a long pointer to a string containing 
                                       the compressed result of SX5363_PACKSIZE bytes with the least
                                       significant two bits of the last byte being unused.
               lpBytesProduced (output) -- a long pointer to the number of
                                           bytes of butstream data produced by the encoder.
   Return:     Execution status code of 32-bit integer              
 ***************************************************************************/

SXRESULT SXAPI  SX53_SpeechEncode(HCODEC hCodec, DWORD Features, LPSHORT lpSpeechBuffer, 
							LPBYTE lpBitStream, LPDWORD lpBytesProduced);
typedef SXRESULT (WINAPI * PSX5363ENCODER)(HCODEC hCodec, DWORD Features, LPSHORT lpSpeechBuffer, 
							LPBYTE lpBitStream, LPDWORD lpBytesProduced);

SXRESULT SXAPI  SX63_SpeechEncode(HCODEC hCodec, DWORD Features, LPSHORT lpSpeechBuffer, 
							LPBYTE lpBitStream, LPDWORD lpBytesProduced);

/***************************************************************************
   Name:       SX5363_SpeechDecode()                                                   
   Function:   decodes a compressed data frame into SX5363_FRAMESIZE speech
               samples of 16-bit PCM.
   Parameters: hCodec (input) -- the instance handle of the speech codec for
                                 the current speech stream.
               Features (input) -- Additional features supported by the SX5363S.
               lpBitStream (input) -- a long pointer to a string containing 
                                      the data frame to be decoded. 
               lpSpeechBuffer (output) -- a long pointer to the speech frame.
                                          that contains the decoded SX5363_FRAMESIZE 16-bit PCM samples.
               lpBytesUsed (output) -- a long pointer to the number of bytes
                                       of bitstream data consumed by the decoder.
   Return:     Execution status code of 32-bit integer              
 ***************************************************************************/
SXRESULT SXAPI  SX5363_SpeechDecode(HCODEC hCodec, DWORD Features, LPBYTE lpBitStream,									
								   LPSHORT lpSpeechBuffer, LPDWORD lpBytesUsed);
typedef SXRESULT (WINAPI * PSX5363DECODER)(HCODEC hCodec, DWORD Features, LPBYTE lpBitStream,									
								   LPSHORT lpSpeechBuffer, LPDWORD lpBytesUsed);

#endif









