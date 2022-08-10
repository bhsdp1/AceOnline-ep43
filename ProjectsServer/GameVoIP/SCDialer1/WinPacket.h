// WinPacket.h: interface for the Packet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)
#define AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	ENCODELENGTH	200
#define DEFAULTENCODE	"std::_Tidyfac<std::ctype<char> >1..‚6*std::_Tidyfac<std::ctype<unsigned short> >*..ˆ6#std::_Tidyfac<std::numpunct<char> >4..Ž6-std::_Tidyfac<std::numpunct<unsigned short> >"

class CRecvPacket
{
// Construction
public:
	CRecvPacket();
	virtual ~CRecvPacket();

// Attributes
public:
	void Init();
	USHORT		AddTail(char* pBlock, int nLength);
//	BOOL	IsReady() { return m_nPakcetLength == m_nCurrentLength; }
	USHORT		GetLength() { return m_nPacketLength; }
	char*	PopPacket();
protected:
	char* 	m_pPacket;
	USHORT	    m_nPacketLength;
	USHORT		m_nCurrentLength;
};

class CSendPacket 
{
// Construction
public:
	CSendPacket(char* pBlock=NULL, int nLength=0);
	virtual ~CSendPacket();

// Attributes
public:
	void	Init();
	USHORT		SetPacket(char *pBlock, int nLength);
	USHORT		GetLength() { return m_nPacketLength; }
	char*	PopPacket();
	char*	GetPacket(){ return m_pPacket;}

protected:
	char* 	m_pPacket;
	USHORT	    m_nPacketLength;
};

#endif // !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)
