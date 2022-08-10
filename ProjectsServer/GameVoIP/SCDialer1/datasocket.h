// DataSocket.h: interface for the DataSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASOCKET_H__666BA375_0CF3_11D4_83FD_00001CD80732__INCLUDED_)
#define AFX_DATASOCKET_H__666BA375_0CF3_11D4_83FD_00001CD80732__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//##ModelId=3A4D4D5B02DE
class DataSocket  
{
public:
	//##ModelId=3A4D4D5B031B
	DataSocket();
	//##ModelId=3A4D4D5B031C
	DataSocket(	UINT wmsocket );
	//##ModelId=3A4D4D5B031A
	virtual ~DataSocket();
	
	BOOL Listen( HWND hWnd, unsigned int port , unsigned int max );
	long OnAccepted();

	//##ModelId=3A4D4D5B0312
	BOOL Connect( HWND hWnd, LPCTSTR ip, unsigned int port);
	//##ModelId=3A4D4D5B030A
	BOOL Send( char* data, int length);
	//##ModelId=3A4D4D5B0307
	BOOL Receive( char *data, int length);

	//##ModelId=3A4D4D5B0306
	BOOL Close();
	BOOL CloseListener();
	//##ModelId=3A4D4D5B02FE
	long OnConnected( LPARAM lParam );

	//##ModelId=3A4D4D5B02FD
	BOOL& IsConnecting(){ return m_bConnecting; }
	//##ModelId=3A4D4D5B02FC
	int GetError();
	int GetSockName(sockaddr *name, int *namelen);

	struct sockaddr_in		m_my_addr, m_peer_addr;
private:
	//##ModelId=3A4D4D5B02F4 
	SOCKET					m_socket, m_listener;
	//##ModelId=3A4D4D5B02EB
	HWND					m_hWnd;

	//##ModelId=3A4D4D5B02EA
	int m_nError;
	//##ModelId=3A4D4D5B02E9
	BOOL m_bConnecting;
protected:
	//##ModelId=3A4D4D5B02E8
	UINT m_wmsocket;
};

#endif // !defined(AFX_DATASOCKET_H__666BA375_0CF3_11D4_83FD_00001CD80732__INCLUDED_)
