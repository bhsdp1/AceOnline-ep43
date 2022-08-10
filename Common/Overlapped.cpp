///////////////////////////////////////////////////////////////////////////////
//  Overlapped.cpp : 
//  
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Overlapped.h"
#include "VMemPool.h"


//////////////////////////////////////////////////////////////////////////
// static 관련
//////////////////////////////////////////////////////////////////////////
char* COverlapped::pXOR_ENCODE_BYTES	= XOR_ENCODE_BYTES;


//////////////////////////////////////////////////////////////////////////
// Construction/Destruction 
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			COverlapped::COverlapped(int Mode, int Type, BOOL Encoding)
/// \brief		COverlapped 생성자
				// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		Mode		[in] Read(0) or Write(1) Operation Mode
/// \param		Type		[in] Static(2) or Dynamic(3) Overlapped Type
/// \param		Encoding	[in] 패킷의 인코딩 플래그
/// \return		없음
///////////////////////////////////////////////////////////////////////////////
COverlapped::COverlapped(ENOperationMode i_enOperationMode,
						 ENOverlappedType i_enOverlappedType,
						 BOOL Encoding,
						 ENDataType i_enDataType)		// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
:m_enOperationMode(i_enOperationMode), m_enOverlappedType(i_enOverlappedType), m_bEncodingFlag(Encoding), m_enDataType(i_enDataType)		// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
{
	ResetOverlapped();
}

//////////////////////////////////////////////////////////////////
// COverlapped 소멸자
// 멤버 변수 모두 정적이므로 처리할것 없음
COverlapped::~COverlapped()
{
}


//////////////////////////////////////////////////////////////////////////
// Method
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// COverlapped 패킷에 데이타를 추가하는 함수
// pData		- Data Pointer
// nSize		- 추가할 데이타 Size
// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 인자추가(SSendedOverlappedInfo *o_pSendedOverInfo)
// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
BOOL COverlapped::AddData(SSendedOverlappedInfo *o_pSendedOverInfo, BYTE* pData, int nSize)
{	
	if(m_bEncodedFlag
		|| NULL == pData
		|| nSize <= 0
		|| m_WSABUF.len + nSize > SIZE_MAX_PACKET)
	{	// 이미 인코딩된 패킷임
		// 패킷의 사이즈 유효성 체크

		return FALSE;
	}

	if( EN_DATA_TYPE_ACEONLINE == m_enDataType )
	{
		// EN_DATA_TYPE_ACEONLINE

		o_pSendedOverInfo->pOverlapped	= this;		// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
		int nLen = 0;
		if(0 == m_WSABUF.len)
		{
			o_pSendedOverInfo->pbyData		= (BYTE*)(lpBuff + SIZE_PACKET_HEADER);	// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
			memcpy(lpBuff+SIZE_PACKET_HEADER, pData, nSize);
			m_WSABUF.len = nSize + SIZE_PACKET_HEADER;
		}
		else
		{
			o_pSendedOverInfo->pbyData		= (BYTE*)(&lpBuff[m_WSABUF.len]);	// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 
			memcpy(&lpBuff[m_WSABUF.len], pData, nSize);
			m_WSABUF.len += nSize;
		}
		*(USHORT*)m_WSABUF.buf = m_WSABUF.len - SIZE_PACKET_HEADER;
	}
	else
	{
		// EN_DATA_TYPE_RAW

		// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
		o_pSendedOverInfo->pOverlapped	= this;
		int nLen = 0;
		BYTE *pBuff = NULL;

		pBuff = (BYTE*)&lpBuff[m_WSABUF.len];
		o_pSendedOverInfo->pbyData = pBuff;
		memcpy(pBuff, pData, nSize);
		m_WSABUF.len += nSize;
		// end 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////
// COverlapped 패킷에 있는 데이타 사이즈
// 인자 없음
int COverlapped::GetDataSize(void)
{
	return m_WSABUF.len;
}

//////////////////////////////////////////////////////////////////
// COverlapped 패킷을 초기화 하는 함수
// 인자 없음
void COverlapped::ResetOverlapped(void)
{
	Internal				= 0;
	InternalHigh			= 0;
	Offset					= 0;
	OffsetHigh				= 0;
	hEvent					= NULL;

	m_bEncodedFlag			= FALSE;
	m_WSABUF.buf			= lpBuff;
	m_WSABUF.len			= 0;
	m_nSizesocketaddr_in	= sizeof(sockaddr_in);
	memset(&m_socketaddr_in, 0x00, sizeof(sockaddr_in));
	memset(lpBuff, 0x00, SIZE_MAX_SOCKET_BUFFER);

	m_bSessionEnd			= FALSE;		// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가

	if(EN_OPERATION_MODE_READ == this->GetOperationMode()){		m_WSABUF.len = SIZE_MAX_SOCKET_BUFFER;}
}


// Encoding 이 FALSE일 경우만 사용되는 함수
// m_bEncodingFlag 가 FALSE일때 Sequence Number를 설정한다.
void COverlapped::SetSequenceNumber(BYTE seq)
{
	if(m_bEncodingFlag == TRUE){ return;}
	*(lpBuff + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG) = seq;
}


// m_bEncodingFlag 가 TRUE이고 m_bEncodedFlag가 TRUE일때 패킷에 설정된 Sequence Number
// m_bEncodingFlag 가 FALSE일때 패킷에 설정된 Sequence Number
BYTE COverlapped::GetSequenceNumber()
{
	return *(lpBuff + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG);
}

// 패킷이 Encoding 되었는지의 여부를 리턴
BOOL COverlapped::IsPacketEncoded()
{
	return m_bEncodedFlag;
}

// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
void COverlapped::SetSessionEnd(BYTE bSessionEnd)
{
	m_bSessionEnd = bSessionEnd;
}
// end 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가

void COverlapped::EncodePacket(BYTE seq)
{
	if( EN_DATA_TYPE_RAW == m_enDataType ) { return; }		// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
	if(m_bEncodedFlag == TRUE){ return;}

	if(m_bEncodingFlag == FALSE)
	{
		*(lpBuff+SIZE_BODY_LENGTH+SIZE_ENCODE_FLAG) = seq;
		return;
	}

	// XOR_N 생성, 4, 8, 12, ... , 124
	BYTE XOR_N = (BYTE)(GetTickCount() % 30 + 1) * 4;

	// 변수들 선언, 정의
	BYTE checkSum = 0;
	int nDummyLen = seq%4;
	int woffset = SIZE_BODY_LENGTH;
	int roffset = SIZE_PACKET_HEADER;
	int xoffset = XOR_N;
	char *pBlock	= (char*)lpBuff;
	int nLength = m_WSABUF.len - SIZE_PACKET_HEADER;

	// size를 복사
	*(unsigned short*)lpBuff
		= (unsigned short)(nLength + SIZE_CHECKSUM + nDummyLen);

	// encode flag 1B를 생성하고 복사
	BYTE encodeFlag = ENCODE_MASK | (XOR_N_MASK & XOR_N);
	*(BYTE*)(lpBuff+woffset) = encodeFlag;
	woffset += SIZE_ENCODE_FLAG;

	// sequence number(1B)를 XOR한후, 복사, checksum 계산 시작
	checkSum ^= seq;
	*(BYTE*)(lpBuff+woffset) = (seq ^ pXOR_ENCODE_BYTES[XOR_N-1]);
	woffset += SIZE_SEQ_NUM;

	// MSGs를 4B씩 XOR, checksum 계산
	int nEndValue = (int)(nLength / 4 + 1) * 4;
	DWORD checkSum_4B = 0;
	while (roffset < nEndValue)
	{
		checkSum_4B ^= *(DWORD*)(pBlock+roffset);
		*(DWORD*)(lpBuff+woffset)
			= *(DWORD*)(pBlock+roffset) ^ *(DWORD*)(pXOR_ENCODE_BYTES+xoffset);
		woffset += 4;
		roffset += 4;
		xoffset = (xoffset + 4) % SIZE_XOR_ENCODE_BYTES;
	}

	// 4B의 checkSum_4B를 1B의 checkSum으로 변환
	checkSum ^= ( *((char*)&checkSum_4B)
				^ *((char*)&checkSum_4B+1)
				^ *((char*)&checkSum_4B+2)
				^ *((char*)&checkSum_4B+3) );

	// 남아있는 nLength - nEndValue 만큼의 데이터를 처리 -> XOR + checkSum
	for (int i = 0; i < nLength + 4 - nEndValue; i++)
	{
		checkSum ^= pBlock[roffset];
		lpBuff[woffset]
			= pBlock[roffset] ^ pXOR_ENCODE_BYTES[xoffset];
		woffset++;
		roffset++;
		xoffset = (xoffset + 1) % SIZE_XOR_ENCODE_BYTES;
	}

	// checksum을 추가
	lpBuff[woffset++] =  checkSum ^ pXOR_ENCODE_BYTES[xoffset];

	// dummy data는 XOR_ENCODE_BYTES를 사용함
	char *dummyData = pXOR_ENCODE_BYTES + XOR_N;

	memcpy(lpBuff+woffset, dummyData, nDummyLen);
	woffset += nDummyLen;

	m_WSABUF.len = woffset;
	*(unsigned short*)lpBuff = woffset-SIZE_PACKET_HEADER;

	m_bEncodedFlag = TRUE;
}


void* COverlapped::operator new(size_t size)
{
	return CVMemPool::vmObjectNew(size);
}

void COverlapped::operator delete(void* p)
{
	CVMemPool::vmObjectDelete(p, sizeof(COverlapped));
}