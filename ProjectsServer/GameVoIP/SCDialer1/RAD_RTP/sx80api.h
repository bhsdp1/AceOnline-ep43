/* name and version number:	@(#)sx80api.h	1.2 	*/
/* date of get:			3/2/98 17:49:49	*/
/* date of delta:		11/13/97 13:21:59 */
/* sx80api.h */

#define SX80LC_FRAMESIZE      80
#define SX80LC_BITSLENGTH     80	
#define SX80LC_BYTELENGTH     SX80LC_BITSLENGTH/8
#define SX80LC_BITSLEFTOVER   (SX80LC_BYTLENGTH*8-SX80LC_BITSLENGTH)

#define SXAPI               WINAPI
#define LPBYTE              unsigned char FAR*
#define LPWORD              unsigned short FAR*
typedef HCODEC FAR*         LPHCODEC;

#define SX_OK               0x00        /* no error */
#define SX_ERR_OPEN         0x01        /* fail to open the codec  */
#define SX_ERR_BADPNT       0x02        /* bad data pointer */
#define SX_ERR_BADPARA      0x03        /* bad input parameter */
#define SX_ERR_EXPIRE       0x04        /* codec expired */
#define SX_ERR_NOINI	    0x05        /* ini not found or invalid */

/* SX80LC_CodecOpen                                                       */
/*                                                                        */
/* Description: Open Speech Coder Instance and Allocate Required Global   */
/* Heap Memory Required for a Single Coder Instance.  Must be Executed    */
/* Only Once at the Beginning of Each New Coder Instance.                 */
/*                                                                        */
/* Input/Output: LPHCODEC Pointer to HCODEC which upon output points      */
/* Global Memory Used by Each Speech Coder Instance.                      */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_CodecOpen( LPHCODEC lphCodec);
typedef unsigned long (SXAPI * PSX80LCCODECOPEN)(LPHCODEC lphCodec);


/* SX80LC_EncodeInit                                                      */
/*                                                                        */
/* Description: Initialize the Speech Encoder for Instance hCodec.  Must  */
/* be Executed for Each Instance Prior to Running the Encoder Function.   */
/*                                                                        */
/* Input: HCODEC Pointer to Global Memory Used by Speech Coder Instance   */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_EncodeInit( HCODEC hCodec);
typedef unsigned long (SXAPI * PSX80LCENCODECINIT)(HCODEC hCodec);


/* SX80LC_DecodeInit                                                      */
/*                                                                        */
/* Description: Initialize the Speech Decoder for Instance hCodec.  Must  */
/* be Executed for Each Instance Prior to Running the Decoder Function.   */
/*                                                                        */
/* Input: HCODEC Pointer to Global Memory Used by Speech Coder Instance   */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_DecodeInit( HCODEC hCodec);
typedef unsigned long (SXAPI * PSX80LCDECODECINIT)(HCODEC hCodec);


/* SX80LC_SpeechEncode                                                    */
/*                                                                        */
/* Description: Encode one 10ms, 80 Sample Speech Frame into 8 Bytes      */
/* of Bitstream.                                                          */
/*                                                                        */
/* Input:                                                                 */
/* HCODEC Pointer to Global Memory Used by Speech Coder Instance          */
/* LPWORD Pointer to 80 Speech Samples in 16 Bit Linear Signed PCM Format */
/*                                                                        */
/* Output:                                                                */
/* LPBYTE Pointer to Output Encoded 8 Bytes to be Transmitted             */
/* LPDWORD Pointer to a DWORD Representing the Number of Encoded Bytes    */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_SpeechEncode( HCODEC hCodec, LPWORD speech_in, LPBYTE bitstream_out, LPDWORD lpBytesProduced );
typedef unsigned long (SXAPI * PSX80LCSPEECHENCODE)(HCODEC hCodec, LPWORD speech_in, LPBYTE bitstream_out, LPDWORD lpBytesProduced );


/* SX80LC_SpeechDecode                                                    */
/*                                                                        */
/* Description: Decode one 8 Byte Bitstream into one 10ms, 80 Sample      */
/* Speech Frame.                                                          */
/*                                                                        */
/* Input:                                                                 */
/* HCODEC Pointer to Global Memory Used by Speech Coder Instance          */
/* LPBYTE Pointer to Input 8 Byte Bitstream                               */
/*                                                                        */
/* Output:                                                                */
/* LPWORD Pointer to 80 Speech Samples in 16 Bit Linear Signed PCM Format */
/* LPDWORD Pointer to a DWORD Representing the Number of Decoded Bytes    */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_SpeechDecode( HCODEC hCodec, LPBYTE bitstream_in, LPWORD bufo, LPDWORD lpBytesConsumed );
typedef unsigned long (SXAPI * PSX80LCSPEECHDECODE)( HCODEC hCodec, LPBYTE bitstream_in, LPWORD bufo, LPDWORD lpBytesConsumed );


/* SX80LC_CodecClose                                                      */
/*                                                                        */
/* Description: Close one Speech Coder Instance and Free Global Heap      */
/* Memory.                                                                */
/*                                                                        */
/* Input:                                                                 */
/* HCODEC Pointer to Global Memory Used by Speech Coder Instance          */
/*                                                                        */
/* Return:                                                                */
/* SX_OK               0x00	no error                                  */
/* SX_ERR_OPEN         0x01     fail to open the codec                    */
/* SX_ERR_BADPNT       0x02     bad data pointer                          */
/* SX_ERR_BADPARA      0x03     bad input parameter                       */
/* SX_ERR_EXPIRE       0x04     codec expired                             */
/* SX_ERR_NOINI        0x05     ini not found or invalid                  */
/*                                                                        */
unsigned long SXAPI SX80LC_CodecClose( HCODEC hCodec);
typedef unsigned long (SXAPI * PSX80LCCODECCLOSE)( HCODEC hCodec);

