  // IOCP.cpp: implementation of the IOCP.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <TIME.H>
#include "SocketHeader.h"
#include "IOCP.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////////
// static 멤버 변수/ static 멤버 함수
//////////////////////////////////////////////////////////////////////////
BOOL CIOCP::m_bSocketInitFlag = FALSE;
BOOL CIOCP::SocketInit(void)
{
	if(m_bSocketInitFlag)
		return m_bSocketInitFlag;

	WSADATA wsaData;

	WORD wVersionRequested = MAKEWORD(2, 2);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return FALSE;
	}

	m_bSocketInitFlag = TRUE;
	return m_bSocketInitFlag;
}

void CIOCP::SocketClean(void)
{
	if(!m_bSocketInitFlag)
		return;

	WSACleanup();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			EN_NETWORK_STATE CIOCP::SeekNetworkState(DWORD i_dwMaxBandwidth, DWORD i_dwCurrentBandwidth)
/// \brief		Max Bandwidth와 Current Bandwidth를 가지고 현재 네트워크 상태를 구하는 함수
/// \author		cmkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param	i_dwMaxBandwidth		[in] Max Bandwidth로 계산의 기준이 된다.
/// \param	i_dwCurrentBandwidth	[in] Current Bandwidth
/// \return		네트워크 상태를 리턴함
///////////////////////////////////////////////////////////////////////////////
EN_NETWORK_STATE CIOCP::SeekNetworkState(DWORD i_dwMaxBandwidth, DWORD i_dwCurrentBandwidth)
{
	if(i_dwMaxBandwidth == 0
		|| i_dwCurrentBandwidth == 0)
	{
		return EN_NETWORK_STATE_BEST;
	}
	else if(i_dwMaxBandwidth <= i_dwCurrentBandwidth)
	{
		return EN_NETWORK_STATE_WORST;
	}

	int		nHalfBandwidth = i_dwMaxBandwidth/2;	// Max Bandwidth의 절반
	int		nOne_Ten = i_dwMaxBandwidth/10;			// Max Bandwidth의 1/10
	int		nRemainBandwidth = i_dwMaxBandwidth - i_dwCurrentBandwidth;	// Max Bandwidth에서 남은 Bandwidth로 음수의 값이 될수도 있다.
	if	   (nRemainBandwidth > nHalfBandwidth)			 {		return EN_NETWORK_STATE_BEST;}
	else if(nRemainBandwidth > nHalfBandwidth-1*nOne_Ten){		return EN_NETWORK_STATE_GOOD;}
	else if(nRemainBandwidth > nHalfBandwidth-2*nOne_Ten){		return EN_NETWORK_STATE_NORMAL;}
	else if(nRemainBandwidth > nHalfBandwidth-3*nOne_Ten){		return EN_NETWORK_STATE_BAD;}
	
	return EN_NETWORK_STATE_WORST;
}

//////////////////////////////////////////////////////////////////////////
// Local Function
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD WINAPI ListenerThread(LPVOID lpParam)
/// \brief		Listener Thread function
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		lpParam	[in] CIOCP의 포인터
/// \return		스레드종료시 리턴값
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI ListenerThread(LPVOID lpParam)
{
	return ((CIOCP*)lpParam)->Listen();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD WINAPI WorkerThread(LPVOID lpParam)
/// \brief		Worker Thread function
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		lpParam	[in] CIOCP의 포인터
/// \return		스레드종료시 리턴값
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI WorkerThread(LPVOID lpParam)
{
	return ((CIOCP*)lpParam)->Worker();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD WINAPI IOCPSocketWriteThread(LPVOID lpParam)
/// \brief		소켓에 데이타를 Write하는 스레드
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		lpParam	[in] CIOCP의 포인터
/// \return		스레드종료시 리턴값
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI IOCPSocketWriteThread(LPVOID lpParam)
{
	return ((CIOCP*)lpParam)->Writer();
}


//////////////////////////////////////////////////////////////////////////
// Construction/Destruction 
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			CIOCP::CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort, char *szLocalIP, ENServerType ServerType)
/// \brief		생성자
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \param		dwStartIndex	[in] 임의의 IOCPSocket을 요청하면 시작 인덱스보다 큰값을 넘겨준다
/// \param		dwSessionSize	[in] 최대 접속가능 숫자(TCP, UDP 모두)
/// \param		nPort			[in] ListeningPort
/// \param		szLocalIP		[in] Local Computer IP Address
/// \param		ServerType		[in] 서버 타입
/// \return		없음
///////////////////////////////////////////////////////////////////////////////
CIOCP::CIOCP(DWORD dwStartIndex,
			 DWORD dwSessionSize,
			 int nPort,
			 char *szLocalIP/*="127.0.0.1"*/,
			 ENServerType ServerType/*=ST_NORMAL_SERVER*/)
{
	CIOCPSocket::ms_pIOCP = this;

	if(dwStartIndex >= dwSessionSize)
	{
		dwStartIndex = dwSessionSize -1;
	}
	m_dwStartClientIndex	= dwStartIndex;
	m_dwArrayClientSize		= dwSessionSize;
	m_dwLastClientIndex		= 0;
	m_nServerType			= ServerType;
	m_bThreadSocketWriteEndFlag	= FALSE;
	m_hCompletionPort		= NULL;
	m_hListenerThread		= NULL;
	m_hListener				= INVALID_SOCKET;
	m_bListeningFlag		= FALSE;
	m_nListenerPort			= nPort;
	m_nCurrentClientCounts	= 0;

	m_dwWorkerCount			= 0;
	m_bServiceStartFlag		= FALSE;
	InitializeCriticalSection(&m_crtlClientArray);

	InitializeCriticalSection(&m_crtlTotalTrafficInfo);
	m_bFlagCalcTrafficInfo	= FALSE;
	memset(m_TCPTrafficInfo, 0x00, sizeof(STrafficInfo) * 2);
	m_TCPTrafficInfo[0].dwTimeGapSecond = timeGetTime();
	memset(m_UDPTrafficInfo, 0x00, sizeof(STrafficInfo) * 2);
	m_UDPTrafficInfo[0].dwTimeGapSecond = m_TCPTrafficInfo[0].dwTimeGapSecond;

	m_enIOCPNetworkState	= EN_NETWORK_STATE_BEST;
	m_uTrafficMaxBandwidth		= 50 * 1000 * 1000;					// 50 Mbps 

	int i;
	for(i = 0; i < COUNT_MAX_IOCP_THREAD; i++)
	{
		m_hWorkerThread[i] = NULL;
	}

	for(i = 0; i < COUNT_SOCKET_WRITE_THREAD; i++)
	{
		m_hThreadSocketWriteArray[i] = NULL;
	}

	m_pArrayIOCPSocket = NULL;
	memset(m_ArrayClient, 0x00, sizeof(m_ArrayClient[0])*COUNT_MAX_SOCKET_SESSION);

	m_queueClientIndex.InitQueueINT(m_dwStartClientIndex, m_dwArrayClientSize-1);

	time((time_t*)&m_dwTimeStarted);
	m_dwTickStarted = GetTickCount();

	memset(m_szLocalIPAddress, 0x00, SIZE_MAX_IPADDRESS);
	if(strncmp(szLocalIP, "", SIZE_MAX_IPADDRESS) == 0
		|| strncmp(szLocalIP, "127.0.0.1", SIZE_MAX_IPADDRESS) == 0)
	{
		/////////////////////////////////////////////////////////////////////
		// 자신의 IP Address를 구한다
		char	host[100];
		HOSTENT	*p;
		char	ip[SIZE_MAX_IPADDRESS];

		gethostname(host, 100);
		if(p = gethostbyname(host))
		{
			sprintf(ip, "%d.%d.%d.%d", (BYTE)p->h_addr_list[0][0], (BYTE)p->h_addr_list[0][1],(BYTE)p->h_addr_list[0][2], (BYTE)p->h_addr_list[0][3]);
			if(p->h_addr_list[1])
			{
				sprintf(ip, "%d.%d.%d.%d", (BYTE)p->h_addr_list[1][0], (BYTE)p->h_addr_list[1][1],(BYTE)p->h_addr_list[1][2], (BYTE)p->h_addr_list[1][3]);
			}

			STRNCPY_MEMSET(m_szLocalIPAddress, ip, SIZE_MAX_IPADDRESS);
		}
	}
	else
	{
		STRNCPY_MEMSET(m_szLocalIPAddress, szLocalIP, SIZE_MAX_IPADDRESS);
	}

	m_dwTlsIndex = TLS_OUT_OF_INDEXES;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CIOCP::~CIOCP()
/// \brief		소멸자
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///
/// \return		없음
///////////////////////////////////////////////////////////////////////////////
CIOCP::~CIOCP()
{
	DeleteCriticalSection(&m_crtlClientArray);
	DeleteCriticalSection(&m_crtlTotalTrafficInfo);
}


//////////////////////////////////////////////////////////////////////////
// Method
//////////////////////////////////////////////////////////////////////////

BOOL CIOCP::Bind(UINT nPort)
{
	m_hListener = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_hListener )
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::Bind WSASocket() LastError[%d]\r\n", nErr);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	SOCKADDR_IN 	serv_addr;
	memset(&serv_addr, 0x00, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(nPort);

	if( SOCKET_ERROR == ::bind( m_hListener, (LPSOCKADDR)&serv_addr, sizeof(serv_addr)))
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::Bind bind() LastError[%d] Port[%4d]\r\n", nErr, nPort);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);

		closesocket(m_hListener);
		m_hListener = INVALID_SOCKET;
		return FALSE;
	}
	DbgOut("Listener open success port[%d]\n", nPort);

	return TRUE;
}

BOOL CIOCP::AddIoCompletionPort(CIOCPSocket* pIOCPSocket)
{
	if(m_hCompletionPort == NULL || pIOCPSocket->IsUsing() == FALSE || pIOCPSocket->GetSocket() == INVALID_SOCKET){ return FALSE;}

	HANDLE hret = CreateIoCompletionPort((HANDLE)pIOCPSocket->GetSocket(), m_hCompletionPort, (DWORD)pIOCPSocket, 0);
	if(NULL == hret )
	{
		pIOCPSocket->Close(0x10005);
		return FALSE;
	}

	return TRUE;
}

CIOCPSocket* CIOCP::FindEmptyIOCPSocket(DWORD dwIdx)
{
	if(NULL == m_pArrayIOCPSocket){				return NULL;}
	
	CIOCPSocket *pRetSocket = NULL;
	if(dwIdx < m_dwArrayClientSize)
	{// UDP 통신에 사용

		pRetSocket = GetIOCPSocket(dwIdx);				
	}
	else
	{
		int nRetedIdx = -1;
		if(FALSE == m_queueClientIndex.PopQueueINT(&nRetedIdx))
		{
			return NULL;
		}
		
		pRetSocket = GetIOCPSocket(nRetedIdx);
	}

	if(NULL == pRetSocket
		|| pRetSocket->IsUsing()
		|| FALSE == pRetSocket->m_mtCritSecForClose.Try())
	{
		return NULL;
	}
	
	pRetSocket->m_mtCritSecForClose.Leave();
	return pRetSocket;
}
//{
//	if(NULL == m_pArrayIOCPSocket){ return NULL;}
//	CIOCPSocket* pIOCPSocket = NULL;
//	this->LockClientArrayIndex();
//	if(dwIdx < m_dwArrayClientSize)
//	{
//		if(m_ArrayClient[dwIdx] 
//			&& m_ArrayClient[dwIdx]->IsUsing() == FALSE
//			&& m_ArrayClient[dwIdx]->m_mtCritSecForClose.Try())
//		{
//			m_ArrayClient[dwIdx]->m_mtCritSecForClose.Leave();
//			pIOCPSocket = m_ArrayClient[dwIdx];
//		}
//	}
//	else
//	{
//		if(m_dwStartClientIndex == 0)
//		{
//			for(int i = 0; i < m_dwArrayClientSize; i++)
//			{
//				if(m_ArrayClient[m_dwLastClientIndex] == NULL)
//				{
//					break;
//				}
//				else if(m_ArrayClient[m_dwLastClientIndex]->IsUsing() == FALSE
//					&& m_ArrayClient[m_dwLastClientIndex]->m_mtCritSecForClose.Try())
//				{
//					m_ArrayClient[m_dwLastClientIndex]->m_mtCritSecForClose.Leave();
//					pIOCPSocket = m_ArrayClient[m_dwLastClientIndex];
//					m_dwLastClientIndex = (++m_dwLastClientIndex)%m_dwArrayClientSize;
//					break;										
//				}
//				m_dwLastClientIndex = (++m_dwLastClientIndex)%m_dwArrayClientSize;
//			}
//		}
//		else
//		{
//			if(m_dwLastClientIndex < m_dwStartClientIndex)
//			{
//				m_dwLastClientIndex = m_dwStartClientIndex;
//			}
//			for(int i = 0; i < m_dwArrayClientSize-m_dwStartClientIndex; i++)
//			{
//				if(m_ArrayClient[m_dwLastClientIndex] == NULL)
//				{
//					break;
//				}
//				else if(m_ArrayClient[m_dwLastClientIndex]->IsUsing() == FALSE					
//					&& m_ArrayClient[m_dwLastClientIndex]->m_mtCritSecForClose.Try())
//				{
//					m_ArrayClient[m_dwLastClientIndex]->m_mtCritSecForClose.Leave();
//					pIOCPSocket = m_ArrayClient[m_dwLastClientIndex];
//					m_dwLastClientIndex = (++m_dwLastClientIndex)%m_dwArrayClientSize;
//					break;
//				}
//				m_dwLastClientIndex = (++m_dwLastClientIndex)%m_dwArrayClientSize;
//				if(m_dwLastClientIndex < m_dwStartClientIndex)
//				{
//					m_dwLastClientIndex = m_dwStartClientIndex;
//				}
//			}
//		}
//	}
//	this->UnlockClientArrayIndex();
//	return pIOCPSocket;
//}

CIOCPSocket* CIOCP::GetIOCPSocket(DWORD dwIdx)
{
	if(NULL == m_pArrayIOCPSocket || dwIdx >= m_dwArrayClientSize)
	{
//		char	szError[1024];
//		sprintf(szError, "CIOCP::GetIOCPSocket invalid parameter, ArrayClientSize[%4d] idx[%4d]\r\n"
//			, m_dwArrayClientSize, dwIdx);
//		GWriteSystemLog(szError);
//		DBGOUT(szError);
		return NULL;
	}

	return *(m_ArrayClient+dwIdx);
}

void CIOCP::ListenerClose()
{
	m_bListeningFlag	= FALSE;
	if(m_hListener != INVALID_SOCKET)
	{
		closesocket(m_hListener);
	}
	m_hListener			= INVALID_SOCKET;

	if(m_hListenerThread)
	{
		DWORD dwRet;
		dwRet = WaitForSingleObject(m_hListenerThread, INFINITE);
		if(WAIT_OBJECT_0 != dwRet)
		{
			// 리턴이 WAIT_FAILED임
			int nError = GetLastError();
			SetLastError(0);
		}
		CloseHandle(m_hListenerThread);
	}
	m_hListenerThread	= NULL;
	m_bServiceStartFlag	= FALSE;
}

// 서버가 실행된 이후 지난 시간을 초단위로 반환
DWORD CIOCP::GetCurrentServerTime(void)
{
	return m_dwTimeStarted + (timeGetTime() - m_dwTickStarted)/1000;
}

// 서버가 실행된 이후 지난 시간을 Milli-Seconds 단위로 반환
DWORD CIOCP::GetCurrentServerTimeInMilliSeconds(void)
{
	return timeGetTime() - m_dwTickStarted;
}

void CIOCP::GetLocalAddress(char* o_szIP)
{
	memset(o_szIP, 0x00, SIZE_MAX_IPADDRESS);		// 인자를 초기화 한다.

	// 자신의 IP Address를 구한다
	char	host[100];
	HOSTENT	*p;
	gethostname(host, 100);
	if(p = gethostbyname(host))
	{
		sprintf(o_szIP, "%d.%d.%d.%d",
			(BYTE)p->h_addr_list[0][0], (BYTE)p->h_addr_list[0][1],
			(BYTE)p->h_addr_list[0][2], (BYTE)p->h_addr_list[0][3]);
		
		return;
	}
}

void CIOCP::ClientCheck(void)
{
// 2007-03-12 by cmkwon, 종료될 소켓 종료 처리 추가, 아래와 같이 수정함
//	int		i = 0;
//	while(GetListeningFlag() == TRUE
//		&& i < m_dwArrayClientSize
//		&& m_ArrayClient[i])
//	{
//		if(m_ArrayClient[i]->IsUsing()
//			&& m_ArrayClient[i]->m_bUDPFlag == FALSE)
//		{
//			m_ArrayClient[i]->ClientCheck();
//		}
//		i++;
//	}

	for(int i = 0;i < m_dwArrayClientSize; i++)
	{
		if(FALSE == GetListeningFlag())
		{
			break;
		}

		if(NULL == m_ArrayClient[i]
			|| FALSE == m_ArrayClient[i]->IsUsing()
			|| m_ArrayClient[i]->m_bUDPFlag)
		{
			continue;
		}

		if(m_ArrayClient[i]->m_bMustClose)
		{
			m_ArrayClient[i]->Close(0x10009);			// 2007-03-12 by cmkwon
			continue;
		}

		m_ArrayClient[i]->ClientCheck();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
/// \author		cmkwon
/// \date		2010-04-08 ~ 2010-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCP::SendAlivePacket2OtherServers(void)
{
	// do nothing
}

void CIOCP::LockClientArrayIndex(void)
{
	EnterCriticalSection(&m_crtlClientArray);
}

void CIOCP::UnlockClientArrayIndex(void)
{
	LeaveCriticalSection(&m_crtlClientArray);
}

BOOL CIOCP::PushClientIndex(int i_nIdx)
{
	if(FALSE == m_queueClientIndex.IsValidDataNum(i_nIdx))
	{// Queue의 유효한 데이타가 아니면 무조건 TRUE를 리턴한다
		return TRUE;
	}

	return m_queueClientIndex.PushQueueINT(i_nIdx);
}

//////////////////////////////////////////////////////////////////////////
// Method / bandwidth 계산 관련
//////////////////////////////////////////////////////////////////////////
void CIOCP::LockTotalTrafficInfo(void)
{
	EnterCriticalSection(&m_crtlTotalTrafficInfo);
}

void CIOCP::UnlockTotalTrafficInfo(void)
{
	LeaveCriticalSection(&m_crtlTotalTrafficInfo);
}

void CIOCP::CalcTotalTrafficInfo(void)
{
	if(GetFlagCalcTrafficInfo() == FALSE)
	{
		return;
	}
	char	szSysLog[1024];
	DWORD	dwCurTick	= timeGetTime();

	///////////////////////////////////////////////////////////////////////////
	// Bandwidth 계산을 위해 동기화 해야하는 객체에서 계산을 위한 더미 객체로 데이타를 가져온다.
	this->LockTotalTrafficInfo();
	memcpy(&m_TCPTrafficInfo[1], &m_TCPTrafficInfo[0], sizeof(STrafficInfo));
	memset(&m_TCPTrafficInfo[0], 0x00, sizeof(STrafficInfo));
	m_TCPTrafficInfo[0].dwTimeGapSecond = dwCurTick;
	memcpy(&m_UDPTrafficInfo[1], &m_UDPTrafficInfo[0], sizeof(STrafficInfo));
	memset(&m_UDPTrafficInfo[0], 0x00, sizeof(STrafficInfo));	
	m_UDPTrafficInfo[0].dwTimeGapSecond = dwCurTick;
	this->UnlockTotalTrafficInfo();

	///////////////////////////////////////////////////////////////////////////
	// Time이 1보다 작을경우 1로 처리한다.
	m_TCPTrafficInfo[1].dwTimeGapSecond		= max(1, (dwCurTick - m_TCPTrafficInfo[1].dwTimeGapSecond)/1000);
	m_UDPTrafficInfo[1].dwTimeGapSecond		= max(1, (dwCurTick - m_UDPTrafficInfo[1].dwTimeGapSecond)/1000);

	///////////////////////////////////////////////////////////////////////////
	// 현재 사용되고 있는 IOCPSocket에서 계산을 위해 Bandwidth 정보를 가져온다
	int i = 0;
	while(GetListeningFlag() == TRUE
		&& i < m_dwArrayClientSize
		&& m_ArrayClient[i])
	{
		if(m_ArrayClient[i]->IsUsing() == TRUE)
		{
			if(m_ArrayClient[i]->m_bUDPFlag == FALSE)
			{
				m_ArrayClient[i]->LockTrafficInfo();
				m_TCPTrafficInfo[1].dwBytesSend			+= m_ArrayClient[i]->m_TrafficInfo.dwBytesSend;
				m_TCPTrafficInfo[1].dwCountSendPacket	+= m_ArrayClient[i]->m_TrafficInfo.dwCountSendPacket;
				m_TCPTrafficInfo[1].dwBytesRecv			+= m_ArrayClient[i]->m_TrafficInfo.dwBytesRecv;
				m_TCPTrafficInfo[1].dwCountRecvPacket	+= m_ArrayClient[i]->m_TrafficInfo.dwCountRecvPacket;
				memset(&m_ArrayClient[i]->m_TrafficInfo, 0x00, sizeof(STrafficInfo));
				m_ArrayClient[i]->UnlockTrafficInfo();
			}
			else
			{
				m_ArrayClient[i]->LockTrafficInfo();
				m_UDPTrafficInfo[1].dwBytesSend			+= m_ArrayClient[i]->m_TrafficInfo.dwBytesSend;
				m_UDPTrafficInfo[1].dwCountSendPacket	+= m_ArrayClient[i]->m_TrafficInfo.dwCountSendPacket;
				m_UDPTrafficInfo[1].dwBytesRecv			+= m_ArrayClient[i]->m_TrafficInfo.dwBytesRecv;
				m_UDPTrafficInfo[1].dwCountRecvPacket	+= m_ArrayClient[i]->m_TrafficInfo.dwCountRecvPacket;
				memset(&m_ArrayClient[i]->m_TrafficInfo, 0x00, sizeof(STrafficInfo));
				m_ArrayClient[i]->UnlockTrafficInfo();
			}
		}
		i++;
	}

	///////////////////////////////////////////////////////////////////////////
	// TCP LanHeader 와 UDP LanHeader를 더하여 준다
	m_TCPTrafficInfo[1].dwBytesSend			+= m_TCPTrafficInfo[1].dwCountSendPacket * SIZE_TCP_LAN_HEADER;
	m_TCPTrafficInfo[1].dwBytesRecv			+= m_TCPTrafficInfo[1].dwCountRecvPacket * SIZE_TCP_LAN_HEADER;
	m_UDPTrafficInfo[1].dwBytesSend			+= m_UDPTrafficInfo[1].dwCountSendPacket * SIZE_UDP_LAN_HEADER;
	m_UDPTrafficInfo[1].dwBytesRecv			+= m_UDPTrafficInfo[1].dwCountRecvPacket * SIZE_UDP_LAN_HEADER;

	///////////////////////////////////////////////////////////////////////////
	// 패킷 레벨 시스템을 위해 네크워크 상태를 계산한다.
	EN_NETWORK_STATE enNetState = CIOCP::SeekNetworkState(m_uTrafficMaxBandwidth, (m_TCPTrafficInfo[1].dwBytesRecv*8)/m_TCPTrafficInfo[1].dwTimeGapSecond);	
	if(enNetState != m_enIOCPNetworkState)
	{
		sprintf(szSysLog, "[Notify] CIOCP::CalcTotalTrafficInfo_1 Network State Change (%d) ==> (%d)\r\n",
			m_enIOCPNetworkState, enNetState);
		g_pGlobal->WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);
		m_enIOCPNetworkState = enNetState;
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Bandwidth를 System Log로 남긴다.
	if(GetFlagCalcTrafficInfo())
	{
		STrafficInfo *pTCP = GetPTCPTrafficInfo();
		STrafficInfo *pUDP = GetPUDPTrafficInfo();
						
		UINT	TCPRecv_bps, TCPRecv_cps, TCPSend_bps, TCPSend_cps;
		UINT	UDPRecv_bps, UDPRecv_cps, UDPSend_bps, UDPSend_cps;
		
		TCPRecv_bps = (pTCP->dwBytesRecv * 8)/pTCP->dwTimeGapSecond;
		TCPRecv_cps = pTCP->dwCountRecvPacket/pTCP->dwTimeGapSecond;
		TCPSend_bps = (pTCP->dwBytesSend * 8)/pTCP->dwTimeGapSecond;
		TCPSend_cps = pTCP->dwCountSendPacket/pTCP->dwTimeGapSecond;
		UDPRecv_bps = (pUDP->dwBytesRecv * 8)/pUDP->dwTimeGapSecond;
		UDPRecv_cps = pUDP->dwCountRecvPacket/pUDP->dwTimeGapSecond;
		UDPSend_bps = (pUDP->dwBytesSend * 8)/pUDP->dwTimeGapSecond;
		UDPSend_cps = pUDP->dwCountSendPacket/pUDP->dwTimeGapSecond;
		sprintf(szSysLog, "Traffic\r\n\t\t\tTotal : Recv[%10d bps, %6d cps]	\t\t\t\t\t\t	Send[%10d bps, %6d cps]\r\n"
			, TCPRecv_bps + UDPRecv_bps, TCPRecv_cps + UDPRecv_cps, TCPSend_bps + UDPSend_bps, TCPSend_cps + UDPSend_cps);
		sprintf(&szSysLog[strlen(szSysLog)], "\t\t\tTCP   : Recv[%10d bps, %6d cps, %8d bytes, %6d Counts] Send[%10d bps, %6d cps, %8d bytes, %6d Counts] %d sec\r\n"
			, TCPRecv_bps, TCPRecv_cps, pTCP->dwBytesRecv, pTCP->dwCountRecvPacket, TCPSend_bps, TCPSend_cps, pTCP->dwBytesSend, pTCP->dwCountSendPacket, pTCP->dwTimeGapSecond);
		sprintf(&szSysLog[strlen(szSysLog)], "\t\t\tUDP   : Recv[%10d bps, %6d cps, %8d bytes, %6d Counts] Send[%10d bps, %6d cps, %8d bytes, %6d Counts] %d sec\r\n"
			, UDPRecv_bps, UDPRecv_cps, pUDP->dwBytesRecv, pUDP->dwCountRecvPacket, UDPSend_bps, UDPSend_cps, pUDP->dwBytesSend, pUDP->dwCountSendPacket, pUDP->dwTimeGapSecond);
		g_pGlobal->WriteSystemLog(szSysLog);
	}
}

STrafficInfo *CIOCP::GetPTCPTrafficInfo(void)
{
	return &m_TCPTrafficInfo[1];
}

STrafficInfo *CIOCP::GetPUDPTrafficInfo(void)
{
	return &m_UDPTrafficInfo[1];
}

void CIOCP::UpdateTCPTrafficInfo(STrafficInfo *pTCPTraffic)
{
	this->LockTotalTrafficInfo();
	m_TCPTrafficInfo[0].dwBytesSend			+= pTCPTraffic->dwBytesSend;
	m_TCPTrafficInfo[0].dwBytesRecv			+= pTCPTraffic->dwBytesRecv;
	m_TCPTrafficInfo[0].dwCountSendPacket	+= pTCPTraffic->dwCountSendPacket;
	m_TCPTrafficInfo[0].dwCountRecvPacket	+= pTCPTraffic->dwCountRecvPacket;
	this->UnlockTotalTrafficInfo();
}

void CIOCP::UpdateUDPTrafficInfo(STrafficInfo *pUDPTraffic)
{
	this->LockTotalTrafficInfo();
	m_UDPTrafficInfo[0].dwBytesSend			+= pUDPTraffic->dwBytesSend;
	m_UDPTrafficInfo[0].dwBytesRecv			+= pUDPTraffic->dwBytesRecv;
	m_UDPTrafficInfo[0].dwCountSendPacket	+= pUDPTraffic->dwCountSendPacket;
	m_UDPTrafficInfo[0].dwCountRecvPacket	+= pUDPTraffic->dwCountRecvPacket;
	this->UnlockTotalTrafficInfo();
}


//////////////////////////////////////////////////////////////////////////
// Method / Thread Proc
//////////////////////////////////////////////////////////////////////////
// 소켓에 데이타를 Write하는 스레드
BOOL CIOCP::Writer(void)
{
	char	szTemp1[1024];
	sprintf(szTemp1, "CIOCP::Writer_ WriterThread, \t\t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());
	g_pGlobal->WriteSystemLog(szTemp1);
	DBGOUT(szTemp1);

	SThreadInfo *pstInfo = NULL;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId			= GetCurrentThreadId();
	stInfo.enThreadCheckType	= THREAD_CHECK_TYPE_IOCP_WRITER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);
	while(TRUE)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if(pstInfo){		break;}
		Sleep(100);
	}

	int idx;

	do
	{
		// Thread 종료 플래그
		if(m_bThreadSocketWriteEndFlag == TRUE){ break;}

		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = TRUE;
		for(idx = 0; idx < m_dwArrayClientSize; idx++)
		{
			pstInfo->dwSocketIndex = idx;
			m_ArrayClient[idx]->Write(FALSE);
		}
		pstInfo->bThreadUseFlag = FALSE;
		Sleep(30);	// check: 원활한 디버깅 위해서, 20031014, kelovon, 원래값 30ms
	} while(TRUE);

	return 4;
}


//////////////////////////////////////////////////////////////////////////
// Method / TLS 관련
//////////////////////////////////////////////////////////////////////////
BOOL CIOCP::TLSDataInit(LPVOID &lpvData)
{
	// Initialize the TLS index for this thread.
	lpvData = (LPVOID) LocalAlloc(LPTR, sizeof(IOCPWorkerTLSDATA*));
	if ( !TlsSetValue(m_dwTlsIndex, lpvData) )
	{
		return FALSE;
	}

	*(IOCPWorkerTLSDATA**)lpvData = new IOCPWorkerTLSDATA;

	return TRUE;
}

BOOL CIOCP::TLSDataClean(LPVOID &lpvData)
{
	SAFE_DELETE( *(IOCPWorkerTLSDATA**)lpvData );

	// Release the dynamic memory before the thread returns.
	lpvData = TlsGetValue(m_dwTlsIndex);
	if (lpvData != 0)
	{
		LocalFree((HLOCAL) lpvData);
	}

	return TRUE;
}

IOCPWorkerTLSDATA* CIOCP::GetIOCPWorkerTLSDATA()
{
	LPVOID lpvData;

	// Retrieve a data pointer for the current thread.
	lpvData = TlsGetValue(m_dwTlsIndex);
	if (lpvData == NULL)
	{
		// error
		return NULL;
	}

	return *(IOCPWorkerTLSDATA**)lpvData;
}


//////////////////////////////////////////////////////////////////////////
// Virtual / IOCP 객체 초기화
//////////////////////////////////////////////////////////////////////////
BOOL CIOCP::IOCPInit(void)
{
	char	szSystemLog[1024];
	sprintf(szSystemLog, "########################	Server IOCPInit		########################\r\n");
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);
	if(TRUE == m_bServiceStartFlag){ return TRUE;}
// 2008-09-08 by cmkwon, 아래에서 TRUE로 설정 한다.
//	m_bServiceStartFlag = TRUE;

	SYSTEM_INFO		SystemInfo;
	GetSystemInfo(&SystemInfo);
	m_dwWorkerCount	= SystemInfo.dwNumberOfProcessors + 2;				// 시스템의 CPU 개수 + 2

	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, SystemInfo.dwNumberOfProcessors);
	if(NULL == m_hCompletionPort)
	{
		m_bServiceStartFlag = FALSE;

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::IOCPInit CreateIoCompletionPort(), LastError[%d] Processors[%d]\r\n", nErr, SystemInfo.dwNumberOfProcessors);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	// Allocate a TLS index
	if ((m_dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
	{
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::IOCPInit TlsAlloc(), LastError[%d] TlsIndex[%d]\r\n", nErr, m_dwTlsIndex);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	m_IOCPThreadCheck.SetCheckThreadCounts(m_dwWorkerCount + COUNT_SOCKET_WRITE_THREAD + 1);
	int i;
	for(i = 0; i < m_dwWorkerCount; i++)
	{
		m_hWorkerThread[i] = chBEGINTHREADEX(NULL, 0, WorkerThread, (LPVOID)this, 0, 0);
		if(NULL == m_hWorkerThread[i])
		{
			m_bServiceStartFlag = FALSE;

			int nErr = GetLastError();
			SetLastError(0);
			char	szError[1024];
			sprintf(szError, "CIOCP::IOCPInit Worker thread create(), LastError[%d] WorkerCount[%d]\r\n", nErr, m_dwWorkerCount);
			g_pGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
	}

	for(i = 0; i < COUNT_SOCKET_WRITE_THREAD; i++)
	{
		m_hThreadSocketWriteArray[i] = chBEGINTHREADEX(NULL, 0, IOCPSocketWriteThread, (LPVOID)this, 0, 0);
		if(NULL == m_hThreadSocketWriteArray[i])
		{
			int nErr = GetLastError();
			SetLastError(0);
			char	szError[1024];
			sprintf(szError, "CIOCP::IOCPInit IOCPWrite thread create(), LastError[%d] WorkerCount[%d]\r\n", nErr, m_dwWorkerCount);
			g_pGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
	}

	m_hListenerThread = chBEGINTHREADEX(NULL, 0, ListenerThread, (LPVOID)this, 0, 0);
	if(NULL == m_hListenerThread)
	{
		m_bServiceStartFlag = FALSE;
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::IOCPInit Listener thread create() LastError[%d]\r\n", nErr);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return m_bServiceStartFlag;
	}

	m_bServiceStartFlag = TRUE;		// 2008-09-08 by cmkwon, 위에서 설정하던것으로 모든것을 처리후에 여기에서 설정하다록 한다.
	return m_bServiceStartFlag;
}

void CIOCP::IOCPClean(void)
{
	ListenerClose();
	if(m_hCompletionPort)
	{
		int i;
		m_bThreadSocketWriteEndFlag = TRUE;
		DWORD dwRet = WaitForMultipleObjects(COUNT_SOCKET_WRITE_THREAD, m_hThreadSocketWriteArray, TRUE, INFINITE);
		if(WAIT_OBJECT_0 <= dwRet && dwRet <= WAIT_OBJECT_0 + COUNT_SOCKET_WRITE_THREAD -1)
		{
			// 모든 Thread가 signaled 상태가 됨, 모두 정상 종료
		}
		else if(WAIT_ABANDONED_0 <= dwRet && dwRet <= WAIT_ABANDONED_0 + COUNT_SOCKET_WRITE_THREAD -1 )
		{
			// 모든 Thread가 signaled 상태가 됨, 적어도 하나 이상의 object가 버려짐
		}
		else if(WAIT_TIMEOUT == dwRet)
		{
			// 모든 Thread가 signaled 상태가 되지 못함, 설정된 Time을 벗어남
		}
		else
		{
			// 리턴이 WAIT_FAILED임
			int nError = GetLastError();
			SetLastError(0);
		}
		for(i = 0; i < COUNT_SOCKET_WRITE_THREAD; i++)
		{
			CloseHandle(m_hThreadSocketWriteArray[i]);
			m_hThreadSocketWriteArray[i] = NULL;
		}

		if (NULL != m_pArrayIOCPSocket)
		{
			for(i = 0; i < m_dwArrayClientSize; i++)
			{
				if(m_ArrayClient[i] != NULL && m_ArrayClient[i]->IsUsing() == TRUE)
				{
					m_ArrayClient[i]->Close(0x00002);
				}
			}
		}

		for(i = 0; i < m_dwWorkerCount; i++)
		{
			PostQueuedCompletionStatus(m_hCompletionPort, 0, NULL, NULL);
		}

		dwRet = WaitForMultipleObjects(m_dwWorkerCount, m_hWorkerThread, TRUE, INFINITE);
		if(WAIT_OBJECT_0 <= dwRet && dwRet <= WAIT_OBJECT_0 + m_dwWorkerCount -1)
		{
			// 모든 Thread가 signaled 상태가 됨, 모두 정상 종료
		}
		else if(WAIT_ABANDONED_0 <= dwRet && dwRet <= WAIT_ABANDONED_0 + m_dwWorkerCount -1 )
		{
			// 모든 Thread가 signaled 상태가 됨, 적어도 하나 이상의 object가 버려짐
		}
		else if(WAIT_TIMEOUT == dwRet)
		{
			// 모든 Thread가 signaled 상태가 되지 못함, 설정된 Time을 벗어남
		}
		else
		{
			// 리턴이 WAIT_FAILED임
			int nError = GetLastError();
			SetLastError(0);
		}
		for(i = 0; i < m_dwWorkerCount; i++)
		{
			CloseHandle(m_hWorkerThread[i]);
			m_hWorkerThread[i] = NULL;
		}

		CloseHandle(m_hCompletionPort);
		m_hCompletionPort = NULL;
	}

	if(m_dwTlsIndex != TLS_OUT_OF_INDEXES)
	{
		TlsFree(m_dwTlsIndex);
		m_dwTlsIndex = TLS_OUT_OF_INDEXES;
	}

	char	szSystemLog[1024];
	sprintf(szSystemLog, "########################	Server IOCPClean	########################\r\n\r\n");
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);
}


//////////////////////////////////////////////////////////////////////////
// Virtual / Thread Proc
//////////////////////////////////////////////////////////////////////////
BOOL CIOCP::Listen(void)
{
	char	szTemp1[1024];
	sprintf(szTemp1, "CIOCP::Listen_ ListenerThread, \t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());
	g_pGlobal->WriteSystemLog(szTemp1);
	DBGOUT(szTemp1);

	if(INVALID_SOCKET != m_hListener)
	{
		return FALSE;
	}

	if(Bind(m_nListenerPort) == FALSE)
	{
		MessageBox(NULL, "Cannot Bind Local Address!", NULL, MB_OK);
		return FALSE;
	}

	if(SOCKET_ERROR == listen(m_hListener, SOMAXCONN))
	{
		int nErr = WSAGetLastError();
		WSASetLastError(0);
		char	szError[1024];
		sprintf(szError, "CIOCP::Listen listen() LastError[%d] hListener[0x%X]\r\n", nErr, m_hListener);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	m_bListeningFlag = TRUE;

	SThreadInfo *pstInfo = NULL;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId			= GetCurrentThreadId();
	stInfo.enThreadCheckType	= THREAD_CHECK_TYPE_IOCP_LISTENER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);
	while(TRUE)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if(pstInfo){		break;}
		Sleep(100);
	}


	SOCKET			soc;							// WSAAccept 로 부터 받은 소켓 핸들
	sockaddr_in		addr;							// WSAAccept 의 인자 구조체
	int				nSize;							// WSAAccept 의 인자로 addr 구조체의 사이즈
	char			szAddress[SIZE_MAX_IPADDRESS];	// 접속한 Client의 IP Address 저장 배열
	int				nPort;
	CIOCPSocket*	pIOCPSock;						// vector에서 할당 받은 소켓

	nSize = sizeof(sockaddr_in);
	do
	{
		soc = WSAAccept(m_hListener, (sockaddr*)&addr, &nSize, NULL, 0);
		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = TRUE;
		if(INVALID_SOCKET != soc )
		{
			STRNCPY_MEMSET(szAddress, inet_ntoa(addr.sin_addr), sizeof(szAddress));
			nPort = ntohs(addr.sin_port);
			if(strncmp(szAddress, "", SIZE_MAX_IPADDRESS) == 0)
			{
				DbgOut("\n Client IP Error");
			}
			
			pIOCPSock = FindEmptyIOCPSocket();
			if(NULL == pIOCPSock )
			{
				closesocket(soc);

				char	szError[1024];
				sprintf(szError, "CIOCP::Listen FindEmptyIOCPSocket() CurrentClientNumber[%d] ClientIP[%s]\r\n"
					, m_nCurrentClientCounts, szAddress);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
				pstInfo->bThreadUseFlag = FALSE;
				continue;
			}
			pIOCPSock->SetSocket(soc);					// 소켓을 할당한다

			BOOL bOption = TRUE;
			pIOCPSock->SetOption(IPPROTO_TCP, TCP_NODELAY, (char*)&bOption, sizeof(BOOL));

			/*
			// cmkwon 20030318 수정중
			linger li;
			li.l_onoff	= TRUE;
			li.l_linger	= 1;
			pIOCPSock->SetOption(SOL_SOCKET, SO_LINGER, (char*)&li, sizeof(li));
			//*/

			HANDLE hret = CreateIoCompletionPort((HANDLE)soc, m_hCompletionPort, (DWORD)pIOCPSock, 0);
			if(NULL == hret )
			{
				pIOCPSock->Close(0x10006);

				int nErr = GetLastError();
				SetLastError(0);
				char	szError[1024];
				sprintf(szError, "CIOCP::Listen CreateIoCompletionPort() LastError[%d] hCompletionPort[0x%X] pIOCPSock[0x%X]\r\n"
					, nErr, m_hCompletionPort, pIOCPSock);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
				pstInfo->bThreadUseFlag = FALSE;
				continue;
			}
			pIOCPSock->SetPeerAddress(szAddress, nPort);
			pIOCPSock->OnConnect();										// User Overloading function
			pIOCPSock->Read();
		}
		else
		{	// Listener Socket 이 close 된 경우

			if(FALSE != m_bListeningFlag)
			{
				int nErr = GetLastError();
				SetLastError(0);
				char	szError[1024];
				sprintf(szError, "CIOCP::Listen WSAAccept Failure, LastError[%d]\r\n", nErr);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
			}
			break;
		}
		pstInfo->bThreadUseFlag = FALSE;
	}while(TRUE);

	pstInfo->bThreadUseFlag = FALSE;
	return 3;
}

DWORD CIOCP::Worker(void)
{
	char	szTemp1[1024];
	sprintf(szTemp1, "CIOCP::Worker_ WorkerThread, \t\t\t\t\t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());
	g_pGlobal->WriteSystemLog(szTemp1);
	DBGOUT(szTemp1);

	COverlapped		*pOverlapped;
	DWORD			dwRead;
	CIOCPSocket*	pIOCPSocket;
	BOOL			bRet;

	// TLS
	LPVOID lpvData;

	if (!TLSDataInit(lpvData))
	{
		// init error
		char	szError[1024];
		sprintf(szError, "[Error] CIOCP::Worker TLSDataInit Error\r\n");
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		MessageBox(NULL, szError, "ERROR", MB_OK);
		return -1;
	}

	SThreadInfo *pstInfo = NULL;
	SThreadInfo	stInfo;
	memset(&stInfo, 0x00, sizeof(SThreadInfo));
	stInfo.dwThreadId			= GetCurrentThreadId();
	stInfo.enThreadCheckType	= THREAD_CHECK_TYPE_IOCP_WORKER;
	m_IOCPThreadCheck.AddThreadInfo(stInfo);
	while(TRUE)
	{
		pstInfo = m_IOCPThreadCheck.GetThreadInfo(stInfo.dwThreadId);
		if(pstInfo){		break;}
		Sleep(100);
	}

	srand( timeGetTime() );			// Random Number를 위해서
	do
	{
		pOverlapped	= NULL;
		dwRead		= 0;
		pIOCPSocket	= NULL;
		bRet = GetQueuedCompletionStatus(m_hCompletionPort, &dwRead, (DWORD*)&pIOCPSocket, (LPOVERLAPPED*)&pOverlapped, INFINITE);
		pstInfo->dwLastUseStartTick = timeGetTime();
		pstInfo->bThreadUseFlag = TRUE;
		if(FALSE == bRet)
		{
			int errCode = GetLastError();
			SetLastError(0);

			if(ERROR_PORT_UNREACHABLE == errCode
				&& NULL != pIOCPSocket)
			{	// UDP 상대방 UDP Port가 Open되어있지 않음

				char szSystemLog[1024];
				sprintf(szSystemLog, "[ERROR] CIOCP::Worker() UDP ERROR_PORT_UNREACHABLE Error, SocketIndex[%3d] ==> Error[ERROR_PORT_UNREACHABLE]\r\n", pIOCPSocket->m_nClientArrayIndex);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
			}

			if(ERROR_SUCCESS == errCode && 0 == dwRead)
			{	// socket이 close 됨

				//DBGOUT("	IOCPSocket End 1 error[%d] Overlapped[%ld], ThreadID[%d]\n", errCode, pOverlapped, GetCurrentThreadId());
				SAFE_DELETE_COverlapped(pOverlapped);
				if(NULL != pIOCPSocket)
				{
					pIOCPSocket->Close(0x00003);
				}
			}
			else if(NULL == pOverlapped)
			{	// completion port로 부터 작업완료된 패킷이 나오지 안았음
				// 파라미터값을 사용할수 없음

				//DBGOUT("	IOCPSocket End 2 error[%d] Overlapped[%ld], ThreadID[%d]\n", errCode, pOverlapped, GetCurrentThreadId());
				if(NULL != pIOCPSocket)
				{
					pIOCPSocket->Close(0x00004);
				}
			}
			else
			{	// I/O operation이 실패 했을때 발생(Error Code : 64, 995)
				// 파라미터갑을 사용가능

				//DBGOUT("	IOCPSocket End 3 error[%d] pOver[%d] Overlapped Type[%d], ThreadID[%d]\n", errCode, pOverlapped, pOverlapped->ovType, GetCurrentThreadId());
				SAFE_DELETE_COverlapped(pOverlapped);
				if(NULL != pIOCPSocket)
				{
					pIOCPSocket->Close(0x00005);
				}
			}
			pstInfo->bThreadUseFlag = FALSE;
			continue;
		}


		if(NULL == pIOCPSocket || NULL == pOverlapped)
		{	// IOCP Worker Thread 종료

			break;
		}

		if(0 == dwRead)
		{	// CLOSE

			//DbgOut("	IOCPSocket End 4 Overlapped[%ld], ThreadID[%d]\n", pOverlapped, GetCurrentThreadId());
			SAFE_DELETE_COverlapped(pOverlapped);
			pIOCPSocket->Close(0x00006);
			pstInfo->bThreadUseFlag = FALSE;
			continue;
		}

		switch(pOverlapped->GetOperationMode())
		{
		case COverlapped::EN_OPERATION_MODE_READ:
			{
				pIOCPSocket->OnReceive((char*)pOverlapped->GetWSABUFPtr()->buf, dwRead, (ENServerType)m_nServerType,
					inet_ntoa(pOverlapped->Getsocketaddr_inPtr()->sin_addr), ntohs(pOverlapped->Getsocketaddr_inPtr()->sin_port), pstInfo);
				pIOCPSocket->SetRecvOperationFlag(FALSE);
				pIOCPSocket->Read();
			}
			break;
		case COverlapped::EN_OPERATION_MODE_WRITE:
			{
				if((int)dwRead < pOverlapped->GetWSABUFPtr()->len)
				{
					char	szError[1024];
					sprintf(szError, "CIOCP::Worker 패킷이 전송이 모두 안됨, wsaBuf.len[%d] dwRead[%d]\r\n"
						, pOverlapped->GetWSABUFPtr()->len, dwRead);
					g_pGlobal->WriteSystemLog(szError);
					DBGOUT(szError);
					
					SAFE_DELETE(pOverlapped);
					pIOCPSocket->Close(0x10008);
					
					//				// 전송 된 패킷이 모두 전송이 안된 경우 처리
					//				DbgOut("\n 패킷이 전송이 모두 안됨[%8x]",pOverlapped);
					//				pIOCPSocket->ReWrite((char*)pOverlapped->lpBuff+dwRead, pOverlapped->wsaBuf.len - dwRead);
					//				if(pOverlapped->ovType == COverlapped::OV_TYPE_DYNAMIC)
					//				{
					//					DbgOut("\n 전송 안된 패킷 지움[%x]",pOverlapped);
					//					SAFE_DELETE(pOverlapped);
					//				}
				}
				else
				{
					// 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
					if( TRUE == pOverlapped->GetSessionEnd() )
					{
						SAFE_DELETE(pOverlapped);
						pIOCPSocket->Close(0x10010);
					}
					// end 2013-03-13 by hskim, 웹 캐시 상점 - RawData 전송 기능 추가
					else
					{
						pIOCPSocket->OnWrite(pOverlapped, dwRead);
						pIOCPSocket->Write(TRUE);
					}
				}
			}
			break;
		default:
			{
			}
		}
		pstInfo->bThreadUseFlag = FALSE;
	} while(TRUE);
	pstInfo->bThreadUseFlag = FALSE;


	if (!TLSDataClean(lpvData))
	{
		// init error
		return -1;
	}

	return 2;
}

SThreadInfo *CIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	return m_IOCPThreadCheck.CheckThreadInfo(i_dwThreadIdToExclude);
}

void CIOCP::SendMessageToMonitor(BYTE *pData, int nDataSize)
{
	m_MonitorIOCPSocketPtrVector.lock();
	mt_vector<CIOCPSocket*>::iterator itrV(m_MonitorIOCPSocketPtrVector.begin());
	while (itrV != m_MonitorIOCPSocketPtrVector.end())
	{
		if ((*itrV)->m_PeerSocketType == ST_MONITOR_SERVER)
		{
			(*itrV)->SendAddData(pData, nDataSize);
		}
		itrV++;
	}
	m_MonitorIOCPSocketPtrVector.unlock();
}

void CIOCP::SendMessageToAdminTool(BYTE *pData, int nDataSize)
{
	m_MonitorIOCPSocketPtrVector.lock();
	mt_vector<CIOCPSocket*>::iterator itrV(m_MonitorIOCPSocketPtrVector.begin());
	while (itrV != m_MonitorIOCPSocketPtrVector.end())
	{
		if ((*itrV)->m_PeerSocketType == ST_ADMIN_TOOL)
		{
			(*itrV)->SendAddData(pData, nDataSize);
		}
		itrV++;
	}
	m_MonitorIOCPSocketPtrVector.unlock();
}

BOOL CIOCP::InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket)
{
	m_MonitorIOCPSocketPtrVector.lock();
	m_MonitorIOCPSocketPtrVector.push_back(i_pSocket);
	m_MonitorIOCPSocketPtrVector.unlock();

	return TRUE;
}

BOOL CIOCP::DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket)
{
	m_MonitorIOCPSocketPtrVector.lock();
	mt_vector<CIOCPSocket*>::iterator itr(m_MonitorIOCPSocketPtrVector.begin());
	while(itr != m_MonitorIOCPSocketPtrVector.end())
	{
		if(i_pSocket == *itr)
		{
			m_MonitorIOCPSocketPtrVector.erase(itr);
			break;
		}
		itr++;
	}
	m_MonitorIOCPSocketPtrVector.unlock();

	return TRUE;
}
