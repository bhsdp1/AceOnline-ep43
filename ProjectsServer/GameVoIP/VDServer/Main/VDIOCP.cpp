// VDIOCP.cpp: implementation of the CVDIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VDIOCP.h"
#include "VDGlobal.h"
#include "VDIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVDIOCP::CVDIOCP(int nPort, char *szLocalIP)
	: CIOCP(0, SIZE_MAX_VDSERVER_SESSION, nPort, szLocalIP, ST_NORMAL_SERVER)
{
	CVDIOCPSocket::ms_pVDIOCP	= this;

	CVDIOCPSocket * pVDSocket = new CVDIOCPSocket[m_dwArrayClientSize];
	this->m_pArrayIOCPSocket = pVDSocket;
	for(DWORD i = 0; i < m_dwArrayClientSize; i++)
	{
		m_ArrayClient[i] = &pVDSocket[i];
		m_ArrayClient[i]->InitIOCPSocket(i);
	}
}

CVDIOCP::~CVDIOCP()
{
	ListenerClose();
	IOCPClean();
	memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (NULL != m_pArrayIOCPSocket)
	{
		delete[] (CVDIOCPSocket*)m_pArrayIOCPSocket;
		m_pArrayIOCPSocket = NULL;
	}
}

void test(void)
{
	char te[1024];
	memset(te, 0x00, 1024);
}

BOOL CVDIOCP::IOCPInit(void)
{
	// TODO: Place code here.

	return CIOCP::IOCPInit();
}

void CVDIOCP::IOCPClean(void)
{
	// TODO: Place code here.
	char szSystemLog[512];
	sprintf(szSystemLog, "CNPCIOCP::IOCPClean_1\r\n");
	DBGOUT(szSystemLog);
	g_pVDGlobal->WriteSystemLog(szSystemLog);

	CIOCP::IOCPClean();
}

SThreadInfo *CVDIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);
	
	return pTInfo;
}



void CVDIOCP::AddVCServerList(CVDIOCPSocket *i_pSocket)
{	
	m_mtvectorVCServerList.pushBackLock(i_pSocket);
}

void CVDIOCP::DeleteVCServerList(CVDIOCPSocket *i_pSocket)
{
	m_mtvectorVCServerList.lock();
	mtvectorVDIOCPSocketPtr::iterator itr(m_mtvectorVCServerList.begin());
	while (itr != m_mtvectorVCServerList.end())
	{
		if(*itr == i_pSocket)
		{
			m_mtvectorVCServerList.erase(itr);
			break;
		}
		itr++;
	}
	m_mtvectorVCServerList.unlock();
}

struct sort_mtvectorVCServerList: binary_function<CVDIOCPSocket*, CVDIOCPSocket*, bool>
{
	bool operator()(CVDIOCPSocket *pSocket1, CVDIOCPSocket *pSocket2)
	{
        return pSocket1->GetBandwidthPercent() < pSocket1->GetBandwidthPercent();	// 오름 차순 정렬
    };
};

CVDIOCPSocket *CVDIOCP::FindWellSuitedVCServer(RoomID_t i_rID)
{	
	m_mtvectorVCServerList.lock();

	CVDIOCPSocket *pRetSocket = NULL;	
	///////////////////////////////////////////////////////////////////////////////
	// RoomID를 찾지 못했다면 VCServerList를 정렬하고 첫번째 CVDIOCPSocket*를 리턴한다.
	if(false == m_mtvectorVCServerList.empty())
	{

		if(1 < m_mtvectorVCServerList.size())
		{
			///////////////////////////////////////////////////////////////////////////////
			// 정렬
			sort(m_mtvectorVCServerList.begin(), m_mtvectorVCServerList.end(), sort_mtvectorVCServerList());
		}
		pRetSocket = m_mtvectorVCServerList[0];
	}
	
	m_mtvectorVCServerList.unlock();
	return pRetSocket;
}

CVDIOCPSocket *CVDIOCP::FindVCServer(RoomID_t i_rID)
{
	m_mtvectorVCServerList.lock();
	CVDIOCPSocket *pRetSocket = NULL;
	mtvectorVDIOCPSocketPtr::iterator itr(m_mtvectorVCServerList.begin());
	while (itr != m_mtvectorVCServerList.end())
	{
		pRetSocket = *itr;
		if(TRUE == pRetSocket->m_VCServerInfo.FindRoomID(i_rID))
		{
			m_mtvectorVCServerList.unlock();
			return pRetSocket;
		}
		itr++;
	}
	m_mtvectorVCServerList.unlock();
	return NULL;
}

BOOL CVDIOCP::IsSearchVCServer(CVDIOCPSocket *i_pVDSock)
{
	m_mtvectorVCServerList.lock();
	mtvectorVDIOCPSocketPtr::iterator itr(m_mtvectorVCServerList.begin());
	while (itr != m_mtvectorVCServerList.end())
	{
		if(*itr == i_pVDSock)
		{
			return TRUE;
		}
		itr++;
	}
	m_mtvectorVCServerList.unlock();
	return FALSE;
}