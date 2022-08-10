// S2DWinSocket.h: interface for the CS2DWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_S2DWINSOCKET_H__2208E3E6_4F59_43BF_8B85_5ABD29437CBF__INCLUDED_)
#define AFX_S2DWINSOCKET_H__2208E3E6_4F59_43BF_8B85_5ABD29437CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"
#include "VoIPParam.h"


class CS2DWinSocket : public CWinSocket  
{
public:
	CS2DWinSocket(HWND hWnd, UINT wmSock=WM_S2D_ASYNC_EVENT, UINT wmNotify=WM_S2D_PACKET_NOTIFY);
	virtual ~CS2DWinSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);
	
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

protected:

};

#endif // !defined(AFX_S2DWINSOCKET_H__2208E3E6_4F59_43BF_8B85_5ABD29437CBF__INCLUDED_)
