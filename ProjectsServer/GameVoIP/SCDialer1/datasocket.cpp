// DataSocket.cpp: implementation of the DataSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCDialer1.h"
#include "DataSocket.h"
#include "DialerDefine.h"
#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//##ModelId=3A4D4D5B031B
DataSocket::DataSocket()
{
	m_nError = 0;
	m_bConnecting = FALSE;
	m_wmsocket = UM_MSGSOCKET;
}

//##ModelId=3A4D4D5B031C
DataSocket::DataSocket(	UINT wmsocket )
{
	m_nError = 0;
	m_bConnecting = FALSE;
	m_wmsocket = wmsocket;
}

//##ModelId=3A4D4D5B031A
DataSocket::~DataSocket()
{
	if( m_bConnecting )
		Close();
}

//##ModelId=3A4D4D5B0312
BOOL DataSocket::Connect( HWND hWnd, LPCTSTR ip, unsigned int port)
{
	m_hWnd = hWnd;
	m_socket = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_socket == INVALID_SOCKET )
		return FALSE;
	memset((char *)&m_my_addr, 0x00, sizeof( m_my_addr ));
	m_my_addr.sin_family = AF_INET;
	m_my_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	m_my_addr.sin_port = htons( 0 );
	if( bind( m_socket, (struct sockaddr *)&m_my_addr, sizeof( m_my_addr ))
		== SOCKET_ERROR) {
		m_nError = WSAGetLastError();
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	memset((char *)&m_peer_addr, 0x00, sizeof( m_peer_addr ) );
	m_peer_addr.sin_family = AF_INET;
	m_peer_addr.sin_addr.s_addr = inet_addr( (LPCTSTR)ip );
	m_peer_addr.sin_port = htons( (u_short)port );

	WSAAsyncSelect( m_socket, hWnd, m_wmsocket, FD_CONNECT );
	if( connect( m_socket, (SOCKADDR*)&m_peer_addr, sizeof( m_peer_addr ) ) 
		== SOCKET_ERROR ){
		m_nError = WSAGetLastError();
		if( m_nError != WSAEWOULDBLOCK )
			return FALSE; 
	}
	return TRUE;
}
//##ModelId=3A4D4D5B02FC
int DataSocket::GetError()
{
	int val = m_nError;
	m_nError = 0;
	return val;
}

//##ModelId=3A4D4D5B030A
BOOL DataSocket::Send(char *data, int length)
{
	/*
typedef struct __WSABUF {
    u_longlen;     // buffer length
    char FAR *buf; // pointer to buffer
} WSABUF, FAR * LPWSABUF;
 */
	/*
int WSASend (
  SOCKET s,                                               
  LPWSABUF lpBuffers,                                     
  DWORD dwBufferCount,                                    
  LPDWORD lpNumberOfBytesSent,                            
  DWORD dwFlags,                                          
  LPWSAOVERLAPPED lpOverlapped,                           
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionROUTINE  
);
 */		
#ifdef _DEBUG
	char msgtype[100][30] = 
	{
	"_ERROR", 
	"_LOGIN", 
	"_OK", 
	"_DIAL", 
	"_ALERT", // 20
	"_STARTRTP", 
	"_CONNECTED", 
	"_HANGUP", 
	"_BUTTON", 
	"_COMPLETE", 
	"_ALIVE", 
	"_REGISTRATION", 
	"_RETRIEVE", 
	"_REGOK", 
	"_ROUTERTP", // 30
	"_ROUTEOK", 
	"_REQPROXY", 
	"_RESPROXY", 
	"_NOPROXY", 
	"_PHONEBOOK", 
	"_PREREGISTRATION",///PCN;PWD 구조가 너무 길때 PWD 먼저보냄 = 0x24, 
	"_SIGNUP", 
	"_SEARCHUSER",// = 0x26, 
	"_MYSTATUS", 
	"_FORWARDING",  // 40
	"_VIDEO",
	"_NOTICE",
	"_TEXT",
	"_REREGISTRATION",  // 강제 로그아웃 후에 로그인
	"_GETRATE",
	"_MONEY",
	"_UPDATE",
	"_NONFASTSTART",
	"_ASK_ACTIVE_CONFIRM", //by cje 021112
	"_RETRY_ACTIVE_CONFIRM",  // 50
	"_ACTIVE_CONFIRM_OK",
	"_PASSIVE_START",
	"_VIDEOPAUSE",
	"_VOICECODEC",// = 54,
	"_VIDEOCODEC",// 55,
	"_REQMAKECONFERENCE",//
	"_RESMAKECONFERENCE",//
	"_REQJOINCONFERENCE",//
	"_RESJOINCONFERENCE",//
	"_REQINVITECONFERENCE",// 60,
	"_RESINVITECONFERENCE",//
	"_STATUSCONFERENCE",//
	"_LEAVECONFERENCE",// 
	"_CLOSECONFERENCE"//
	};

	MESSAGE msg;
	memset(&msg, 0x00, sizeof(msg));
	memcpy(&msg, data, sizeof(msg));
	if(msg.Type >= _ERROR && msg.Type <= _CLOSECONFERENCE)
	{
		TRACE("\n[Protocol] [S] %s,%d,%s\n", msgtype[msg.Type - _ERROR], msg.Data.dialmsg.RtpPort, msg.Data.dialmsg.PhoneNumber);
	}
	else
	{
		TRACE("\n[Protocol] [S] Invalid\n");
	}
#endif
	if( send( m_socket, data, length, 0 ) == SOCKET_ERROR ){
		m_nError = WSAGetLastError();
		if( m_nError != WSAEWOULDBLOCK )
			return FALSE; 
	}	

	return TRUE;
}

//##ModelId=3A4D4D5B0307
BOOL DataSocket::Receive(char *data, int length)
{
	int nReted = 0;
	if((nReted = recv( m_socket, data, length, 0 )) == SOCKET_ERROR )
	{
		m_nError = WSAGetLastError();
//		if( m_nError != WSAEWOULDBLOCK )
			return FALSE; 
	}
#ifdef _DEBUG
	char msgtype[100][30] = 
	{
	"_ERROR", 
	"_LOGIN", 
	"_OK", 
	"_DIAL", 
	"_ALERT", // 20
	"_STARTRTP", 
	"_CONNECTED", 
	"_HANGUP", 
	"_BUTTON", 
	"_COMPLETE", 
	"_ALIVE", 
	"_REGISTRATION", 
	"_RETRIEVE", 
	"_REGOK", 
	"_ROUTERTP", // 30
	"_ROUTEOK", 
	"_REQPROXY", 
	"_RESPROXY", 
	"_NOPROXY", 
	"_PHONEBOOK", 
	"_PREREGISTRATION",///PCN;PWD 구조가 너무 길때 PWD 먼저보냄 = 0x24, 
	"_SIGNUP", 
	"_SEARCHUSER",// = 0x26, 
	"_MYSTATUS", 
	"_FORWARDING",  // 40
	"_VIDEO",
	"_NOTICE",
	"_TEXT",
	"_REREGISTRATION",  // 강제 로그아웃 후에 로그인
	"_GETRATE",
	"_MONEY",
	"_UPDATE",
	"_NONFASTSTART",
	"_ASK_ACTIVE_CONFIRM", //by cje 021112
	"_RETRY_ACTIVE_CONFIRM",  // 50
	"_ACTIVE_CONFIRM_OK",
	"_PASSIVE_START",
	"_VIDEOPAUSE",
	"_VOICECODEC",// = 54,
	"_VIDEOCODEC",// 55,
	"_REQMAKECONFERENCE",//
	"_RESMAKECONFERENCE",//
	"_REQJOINCONFERENCE",//
	"_RESJOINCONFERENCE",//
	"_REQINVITECONFERENCE",// 60,
	"_RESINVITECONFERENCE",//
	"_STATUSCONFERENCE",//
	"_LEAVECONFERENCE",// 
	"_CLOSECONFERENCE"//
	};

	MESSAGE msg;
	memset(&msg, 0x00, sizeof(msg));
	memcpy(&msg, data, sizeof(msg));
	if(msg.Type >= _ERROR && msg.Type <= _CLOSECONFERENCE)
	{
		TRACE("\n[Protocol] [R] %s,%d,%s\n", msgtype[msg.Type - _ERROR], msg.Data.dialmsg.RtpPort, msg.Data.dialmsg.PhoneNumber);
	}
	else
	{
		TRACE("\n[Protocol] [R] Invalid\n");
	}
#endif
	return nReted;
}

//##ModelId=3A4D4D5B0306
BOOL DataSocket::Close()
{
//	KillTimer( m_hWnd, 1 ); // kill alive timer 
	m_bConnecting = FALSE;
	WSAAsyncSelect( m_socket, m_hWnd, 0, 0 );
	if( m_socket != INVALID_SOCKET )
		closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	return TRUE;
}
BOOL DataSocket::CloseListener()
{
	if( m_listener != INVALID_SOCKET )
		closesocket( m_listener );
	m_listener = INVALID_SOCKET;
	return TRUE;
}
BOOL DataSocket::Listen( HWND hWnd, unsigned int port , unsigned int max = 1)
{
	m_hWnd = hWnd;
	m_listener = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_listener == INVALID_SOCKET )
		return FALSE;
	memset((char *)&m_my_addr, 0x00, sizeof( m_my_addr ));
	m_my_addr.sin_family = AF_INET;
	m_my_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	m_my_addr.sin_port = htons( (u_short)port );
	if( bind( m_listener, (SOCKADDR*)&m_my_addr, sizeof( m_my_addr ))
		== SOCKET_ERROR) 
		return FALSE;

	if( listen( m_listener, max ) == SOCKET_ERROR ){
		m_nError = WSAGetLastError();
		if( m_nError != WSAEWOULDBLOCK )
			return FALSE;
	}

	WSAAsyncSelect( m_listener, hWnd, m_wmsocket, FD_ACCEPT );
	return TRUE;

}

long DataSocket::OnAccepted()
{
	int length = sizeof(m_peer_addr);
	m_socket = accept( m_listener, (SOCKADDR*)&m_peer_addr, &length );
	if( m_socket == SOCKET_ERROR )
		return -1;
	WSAAsyncSelect( m_socket, m_hWnd, m_wmsocket, FD_READ | FD_CLOSE );
	m_bConnecting = TRUE;
	return 0;
}


//##ModelId=3A4D4D5B02FE
long DataSocket::OnConnected(LPARAM lParam )
{
	UINT state = 0;
	int ret =WSAGETSELECTERROR( lParam ) ;
	switch(ret )
	{
	case WSAEAFNOSUPPORT:
		state = IDS_SOC_AFNOSUPPORT;
		break;
	case WSAECONNREFUSED:
		state = IDS_SOC_CONNREFUSED;
		break;
	case WSAENETUNREACH:
		state = IDS_SOC_NETUNREACH;
		break;
	case WSAEFAULT:
		state = IDS_SOC_FAULT;
		break;
	case WSAEINVAL:
		state = IDS_SOC_INVAL;
		break;
	case WSAEISCONN:
		state = IDS_SOC_INCONN;
		break;
	case WSAEMFILE:
		state = IDS_SOC_MFILE;
		break;
	case WSAENOBUFS:
		state = IDS_SOC_NOBUFS;
		break;
	case WSAENOTCONN:
		state = IDS_SOC_NOTCONN;
		break;
	case WSAETIMEDOUT:
		state = IDS_SOC_TIMEOUT;
		break;
	default:
		if(ret) state = IDS_SOC_NETUNREACH;
		break;
	}
	if( state != 0 )
	{
//		CString str; str.Format("%d", state);
//		AfxMessageBox(str);
		SendMessage( m_hWnd, UM_SOKET_ERROR, NULL, (LPARAM)state);
		return -1L;
	} else {
		WSAAsyncSelect( m_socket, m_hWnd, m_wmsocket, FD_READ | FD_CLOSE );
		m_bConnecting = TRUE;
	}
	return 0L;
}

int DataSocket::GetSockName(sockaddr *name, int *namelen)
{
	return getsockname(m_socket, name, namelen);
}
