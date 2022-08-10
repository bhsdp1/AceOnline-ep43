// LogWinSocket.cpp: implementation of the CLogWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogWinSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogWinSocket::CLogWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify)
:CWinSocket(hWnd, wmSock, wmNotify, FALSE)
{

}

CLogWinSocket::~CLogWinSocket()
{

}

BOOL CLogWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{	
	return FALSE;
}
