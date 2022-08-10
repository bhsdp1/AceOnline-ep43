// WinPacket.cpp: implementation of the Packet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WinPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CRecvPacket class member function ////////////////////////////////////////
CRecvPacket::CRecvPacket()
{
	m_pPacket = NULL;
	m_nPacketLength = 0;
	m_nCurrentLength = 0;
}

CRecvPacket::~CRecvPacket()
{
	if( m_pPacket )
		delete m_pPacket;
}

USHORT CRecvPacket::AddTail(char *pBlock, int nLength)
{
	if( m_nPacketLength == 0 )
	{	// header에서 packet size만 빼낸다.
		memcpy( (void*)&m_nPacketLength, pBlock, sizeof(USHORT));
		nLength -= sizeof(USHORT);
		pBlock += sizeof(USHORT);
	}
	char *p = new char[nLength+m_nCurrentLength];
	if(m_nCurrentLength > 0 )
	{
		memcpy( p, m_pPacket, m_nCurrentLength );
		delete m_pPacket;
	}
	memcpy( p + m_nCurrentLength, pBlock, nLength );
	m_pPacket = p;
	m_nCurrentLength += nLength;

	return	m_nCurrentLength;
}

char* CRecvPacket::PopPacket()
{
	if( m_nPacketLength == m_nCurrentLength )
	{
		m_nCurrentLength = 0;
		m_nPacketLength = 0;
		char* p = m_pPacket;
		m_pPacket = NULL;
		return p;
	}
	return NULL;
}

void CRecvPacket::Init()
{
	m_nCurrentLength = 0;
	m_nPacketLength = 0;
	if(m_pPacket)
	 delete m_pPacket;
	m_pPacket = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CSendPacket class member function ////////////////////////////////////////

CSendPacket::CSendPacket(char* pBlock, int nLength)
{
	m_pPacket = new char[nLength+sizeof(USHORT)];
	memcpy((void*)m_pPacket,(const void*)&nLength,sizeof(USHORT));
	char* p = m_pPacket + sizeof(USHORT);
	memcpy((void*)p,(const void*)pBlock, nLength);

	m_nPacketLength = nLength+sizeof(USHORT);//전체 길이는 패킷의 길이부분 포함길이.
}

CSendPacket::~CSendPacket()
{
	if( m_pPacket )
		delete m_pPacket;
}

USHORT CSendPacket::SetPacket(char *pBlock, int nLength)
{
	if(m_pPacket)// 기존 packet이 남아있으면 지워버린다.
		delete m_pPacket;
	m_pPacket = new char[nLength+sizeof(USHORT)];
	memcpy((void*)m_pPacket,(const void*)&nLength,sizeof(USHORT));
	char* p = m_pPacket + sizeof(USHORT);
	memcpy((void*)p,(const void*)pBlock, nLength);

	m_nPacketLength = nLength+sizeof(USHORT);//전체 길이는 패킷의 길이부분 포함길이.

	return	m_nPacketLength;
}

char* CSendPacket::PopPacket()
{
	m_nPacketLength = 0;
	char* p = m_pPacket;
	m_pPacket = NULL;
	return p;
}

void CSendPacket::Init()
{
	m_nPacketLength = 0;
	if(m_pPacket)
	 delete m_pPacket;
	m_pPacket = NULL;
}
