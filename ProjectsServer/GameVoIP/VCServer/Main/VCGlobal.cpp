// VCGlobal.cpp: implementation of the CVCGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCGlobal.h"
#include "Config.h"
#include "VCIOCP.h"
#include "VoIPParam.h"
#include "C2DWinSocket.h"
#include "VoIPProtocol.h"
#include "VCIOCPSocket.h"


///////////////////////////////////////////////////////////////////////////
// Global Variable
CVCGlobal *		g_pVCGlobal = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVCGlobal::CVCGlobal()
{
	m_pVCIOCP2					= NULL;
	
	m_pC2DWinSocket				= NULL;
	m_nTimerIDSendSelfStatus	= 0;

	MEMSET_ZERO(m_szVDServerIP, SIZE_MAX_IPADDRESS);
	m_usVDServerPort			= 0;
}

CVCGlobal::~CVCGlobal()
{
	this->EndServerSocket();
	
	this->DestroyC2DWinSocket();
}


BOOL CVCGlobal::StartTimerSendSelfStatus(void)
{
	if(NULL == m_hMainWndHandle || m_nTimerIDSendSelfStatus)
	{
		return FALSE;
	}

	m_nTimerIDSendSelfStatus = ::SetTimer(m_hMainWndHandle, TIMERID_SELFSTATUS, TIMERGAP_SELFSTATUS, NULL);
	return TRUE;
}
void CVCGlobal::EndTimerSendSelfStatus(void)
{
	if(m_nTimerIDSendSelfStatus && m_hMainWndHandle)
	{
		::KillTimer(m_hMainWndHandle, TIMERID_SELFSTATUS);
	}
	m_nTimerIDSendSelfStatus = 0;
}

BOOL CVCGlobal::InitServerSocket(void)
{
	// TODO: Place code here.
	DBGOUT("Starting VCServer...\n");
	if(NULL != m_pGIOCP){		return FALSE;}

	m_pVCIOCP2 = new CVCIOCP(m_nPortListening, m_szIPLocal, this->GetMaxRoomCounts());
	m_pGIOCP = m_pVCIOCP2;
	if(m_pGIOCP->IOCPInit() == FALSE)
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] VCServer IOCPInit Error\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR", MB_OK);
		return FALSE;
	}
	
	CVoIPGlobal::InitServerSocket();
	return TRUE;
}

BOOL CVCGlobal::EndServerSocket(void)
{
	CVoIPGlobal::EndServerSocket();

	// TODO: Place code here.
	this->EndTimerSendSelfStatus();

	m_pC2DWinSocket->CloseSocket();

	return TRUE;
}

BOOL CVCGlobal::LoadConfiguration(void)
{
	// set config root path
	this->SetConfigRootPath();

	DBGOUT("========================================\n");
	DBGOUT("Loading Server Informations...\n");

	const int SIZE_BUFF = 512;
	string stringGlobalConfigFileName = string(this->GetConfigRootPath()) + string(GLOBAL_CONFIG_FILE_NAME);
	string stringConfigFileName = string(this->GetConfigRootPath()) + string(VC_SERVER_CONFIG_FILE_NAME);
	
	char buff[SIZE_BUFF];
	const char *token;
	char seps[] = " \t";
	char seps_servergroup[] = ".";

	
	ifstream fin;
	CConfig config;

	if (!config.LoadUserDefinedVariables(stringGlobalConfigFileName.c_str()))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables() error, FileName[%s]\r\n"
			, stringGlobalConfigFileName);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables(stringConfigFileName.c_str()))
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error1\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	fin.open(stringConfigFileName.c_str());

	if (! fin.is_open())
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Can't open config file\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		token = config.strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}
		
		if(stricmp(token, "LocalIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed1.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalPublicIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed2.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetPublicIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalListenPort") == 0)
		{			
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed3.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortListening(atoi(token));
			}		
		}
		else if(0 == stricmp(token, "VDServer"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed4.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				strncpy(m_szVDServerIP, (char*)token, SIZE_MAX_IPADDRESS);
			}
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed5.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				m_usVDServerPort = atoi(token);
			}
		}
		else if(0 == stricmp(token, "MaxRoomCounts"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed6.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			
			m_dwMaxRoomCounts = atoi(token);
		}
		else if(0 == stricmp(token, "MaxUserCounts"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed7.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			
			m_dwMaxUserCounts = atoi(token);
		}
		else if(0 == stricmp(token, "MaxBandwidth"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed8.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			
			m_dwMaxBandwidth = atoi(token)*1000*1000;			// Mega bit로 변경
		}
		else
		{
			// configuration file error!
			assert(0);
		}
	}


	DBGOUT("========================================\n");

	fin.close();

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////
// Method
///////////////////////////////////////////////////////////////////////////
BOOL CVCGlobal::CreateC2DWinSocket(HWND i_hWnd)
{
	if(m_pC2DWinSocket){				return FALSE;}

	m_pC2DWinSocket = new CC2DWinSocket(i_hWnd, WM_C2D_ASYNC_EVENT, WM_C2D_PACKET_NOTIFY);
	return TRUE;
}

BOOL CVCGlobal::ConnectC2DWinSocket(char *i_szIP, USHORT i_usPort)
{
	if(NULL == m_pC2DWinSocket){		return FALSE;}

	return m_pC2DWinSocket->Connect(i_szIP, i_usPort);
}

void CVCGlobal::DestroyC2DWinSocket(void)
{
	if(m_pC2DWinSocket && m_pC2DWinSocket->IsConnected())
	{
		m_pC2DWinSocket->CloseSocket();
	}
	SAFE_DELETE(m_pC2DWinSocket);
}

BOOL CVCGlobal::Send2VDServer(BYTE *i_pData, int i_nDataSize)
{
	if(NULL == m_pC2DWinSocket
		|| FALSE == m_pC2DWinSocket->IsConnected())
	{
		return FALSE;
	}

	return m_pC2DWinSocket->Write(i_pData, i_nDataSize);
}

BOOL CVCGlobal::Send2VDServer(MessageType_t i_msgType)
{
	if(NULL == m_pC2DWinSocket
		|| FALSE == m_pC2DWinSocket->IsConnected())
	{
		return FALSE;
	}
	return m_pC2DWinSocket->WriteMessageType(i_msgType);
}

void CVCGlobal::OnAsyncEventC2DWinSocket(SOCKET i_hSocket, LPARAM i_lParam)
{
	if(NULL == m_pC2DWinSocket)
	{
		return;
	}
	m_pC2DWinSocket->OnAsyncEvent(i_lParam);
}

void CVCGlobal::OnPacketNotifyC2DWinSocket(WPARAM i_wParam)
{
	if(NULL == m_pC2DWinSocket)
	{
		return;
	}

	BYTE		SendBuf[1024];
	char		szSystemLog[512];
	USHORT		usNotifyType = LOWORD(i_wParam);
	USHORT		usNotifyParam = HIWORD(i_wParam);

	switch(usNotifyType)
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			this->EndTimerReconnect();
			if(usNotifyParam == FALSE)
			{				
				sprintf(szSystemLog, "[Error] WndProc(), Can't connect to VDServer[%15s:%4d]\r\n",
					this->GetVDServerIP(), this->GetVDServerPort());
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				
				this->StartTimerReconnect();
			}
			else
			{
				DBGOUT("VD Server에 접속 하였습니다.\n");
				if(FALSE == this->InitServerSocket())
				{
					MessageBox(this->GetMainWndHandle(), "InitServerSocket Error", "ERROR", MB_OK);
				}
				else
				{
					this->StartTimerTraffic();
					this->StartTimerAliveCheck();
					this->StartTimerSendSelfStatus();
					
					this->Send2VDServer(T_C2D_CONNECT);
				}
			}
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			sprintf(szSystemLog, "VD Server[%15s:%4d]와의 연결이 종료 되었습니다. 재접속을 시도합니다.\r\n",
				this->GetVDServerIP(), this->GetVDServerPort());
			this->WriteSystemLog(szSystemLog);
			DBGOUT(szSystemLog);
			
			this->EndServerSocket();
			this->StartTimerReconnect();
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			char			*pPacket = NULL;
			int				nReadLen = 0;
			MessageType_t	nReadType = 0;

			while (TRUE)
			{
				m_pC2DWinSocket->Read(&pPacket, nReadLen);
				if(NULL == pPacket){						break;}

				nReadType = *(MessageType_t*)pPacket;				
				switch(nReadType)
				{
				case T_D2C_CONNECT_OK:			// No body 
					{
						INIT_MSG(MSG_C2D_REGISTER_SELF, T_C2D_REGISTER_SELF, pSendMsg, SendBuf);
						pSendMsg->vcServerID.unionIP.dwIP	= inet_addr(this->GetPublicIPLocal());
						pSendMsg->vcServerID.usPort			= this->GetPortListening();
						pSendMsg->dwMaxRoomCounts			= this->GetMaxRoomCounts();
						pSendMsg->dwMaxUserCounts			= this->GetMaxUserCounts();
						pSendMsg->dwMaxBandwidth			= this->GetMaxBandwidth();

						m_pC2DWinSocket->Write(SendBuf, MSG_SIZE(MSG_C2D_REGISTER_SELF));
					}
					break;
				case T_D2C_REGISTER_SELF_OK:	// No body
					{
					}
					break;
				
				case T_D2C_CREATE_ROOM_NOTIFY_OK:		// T0_D2C_ROOM
					{
						MSG_D2C_CREATE_ROOM_NOTIFY_OK	*pRMsg = (MSG_D2C_CREATE_ROOM_NOTIFY_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						m_pVCIOCP2->On_D2C_CREATE_ROOM_NOTIFY_OK(pRMsg);
					}
					break;
				
				// T0_ERROR
				case T_ERROR:
					{
						MSG_ERROR *pRError = (MSG_ERROR*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						sprintf(szSystemLog, "	CVCGlobal::OnPacketNotifyC2DWinSocket T_ERROR ==> ErrType(0x%6X) MsgType(0x%6X) CloseFlag(%d) Param1(%4d) Param2(%4d)\r\n"
							, pRError->ErrorCode, pRError->MsgType, pRError->CloseConnection
							, pRError->ErrParam1, pRError->ErrParam2);
						this->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);

						if(pRError->CloseConnection)
						{
							this->EndServerSocket();
							this->StartTimerReconnect();
						}
					}
					break;
				
				////////////////////////////////////////////////////////////////////////
				// Unknown Message	
				default:
					{
						sprintf(szSystemLog, "[Error] CVCGlobal::OnPacketNotifyC2DWinSocket Unknown Message Type ==> %d (0x%08X)\r\n"
							, nReadType, nReadType);
						this->WriteSystemLog(szSystemLog);
						DBGOUT(szSystemLog);
					}
				}

				SAFE_DELETE(pPacket);
			}//while_end
		}
		break;
	}//switch_end
}


BOOL CVCGlobal::OnTimer(WPARAM i_TimerId)
{
	switch(i_TimerId)
	{
	case TIMERID_TRAFFIC:
		{
			this->CalculateIOCPTraffic();
			this->CheckServerThread();
		}
		break;
	case TIMERID_ALIVE_CHECK:
		{
			//////////////////////////////////////////////////////
			// VDServer로 Alive를 전송한다.
			m_pC2DWinSocket->WriteMessageType(T_C2D_SOCKET_ALIVE);
						
			this->CheckClientAlive();
		}
		break;
	case TIMERID_RECONNECT:
		{
			this->EndTimerReconnect();
			if(FALSE == m_pC2DWinSocket->IsConnected()
				&& INVALID_SOCKET == m_pC2DWinSocket->GetSocketHandle())
			{
				this->ConnectC2DWinSocket(this->GetVDServerIP(), this->GetVDServerPort());
			}
		}
		break;
	case TIMERID_SELFSTATUS:
		{
			BYTE SendBuf[512];
			INIT_MSG(MSG_C2D_VCSERVER_STATUS_INFO, T_C2D_VCSERVER_STATUS_INFO, pSendMsg, SendBuf);
			pSendMsg->dwCurRoomCounts		= 100;
			pSendMsg->dwCurUserCounts		= 100;
			pSendMsg->dwCurBandwidth		= 1000;

			m_pC2DWinSocket->Write(SendBuf, MSG_SIZE(MSG_C2D_VCSERVER_STATUS_INFO));
		}
		break;
	}
	return TRUE;
}