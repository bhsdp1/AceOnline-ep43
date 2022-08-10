// S2CWinSocket.h: interface for the CS2CWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_S2CWINSOCKET_H__9C3D8BCD_6E64_4BD0_BEE3_DFA5AF82668B__INCLUDED_)
#define AFX_S2CWINSOCKET_H__9C3D8BCD_6E64_4BD0_BEE3_DFA5AF82668B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"
#include "VoIPParam.h"


class CS2CWinSocket : public CWinSocket  
{
public:
	CS2CWinSocket(HWND hWnd, UINT wmSock=WM_S2C_ASYNC_EVENT, UINT wmNotify=WM_S2C_PACKET_NOTIFY);
	virtual ~CS2CWinSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);
	
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

protected:

};

#endif // !defined(AFX_S2CWINSOCKET_H__9C3D8BCD_6E64_4BD0_BEE3_DFA5AF82668B__INCLUDED_)
