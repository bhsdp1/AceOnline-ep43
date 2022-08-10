// C2DWinSocket.h: interface for the CC2DWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_C2DWINSOCKET_H__36D3B9EB_3DF6_490F_A70D_18448B087E41__INCLUDED_)
#define AFX_C2DWINSOCKET_H__36D3B9EB_3DF6_490F_A70D_18448B087E41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"


class CVCIOCP;
class CC2DWinSocket : public CWinSocket  
{
public:
	CC2DWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CC2DWinSocket();

	void SetVCIOCP(CVCIOCP *i_pIOCP);

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);
	
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CVCIOCP			*m_pVCIOCP1;
};

#endif // !defined(AFX_C2DWINSOCKET_H__36D3B9EB_3DF6_490F_A70D_18448B087E41__INCLUDED_)
