// AtumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Atum.h"
#include "AtumDlg.h"
#include "PreUpdateWinSocket.h"
#include <fstream>
#include "dbgout_c.h"
#include "Wininet.h"
#include "MGameDecryption.h"
#include <FTP\FTPManager.h>
#include "AtumError.h"
#include "HttpManager.h"			// 2007-01-03 by cmkwon
#include "selectgameserverdlg.h"	// 2007-05-15 by cmkwon

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumDlg dialog

CAtumDlg::CAtumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAtumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAtumDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pPreUpdateWinsocket = NULL;
// 2008-04-23 by cmkwon, PreServer 주소를 IP와 도메인 둘다 지원 - 
// 	m_szMainPreServerIP.Format("%d.%d.%d.%d"
// 			, MAIN_PRE_SERVER_IP_0, MAIN_PRE_SERVER_IP_1
// 			, MAIN_PRE_SERVER_IP_2, MAIN_PRE_SERVER_IP_3);
	// 2008-04-23 by cmkwon, PreServer 주소를 IP와 도메인 둘다 지원 - 
	char szPreServer[1024];
	MEMSET_ZERO(szPreServer, 1024);
	XOR::XORDecrypt(szPreServer, CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR, STR_XOR_KEY_STRING_PRE_SERVER_ADDRESS);
	m_szMainPreServerIP	= szPreServer;

	m_pHttpManager		= NULL;		// 2007-01-05 by cmkwon
}

void CAtumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAtumDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAtumDlg, CDialog)
	//{{AFX_MSG_MAP(CAtumDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PACKET_NOTIFY, OnSocketNotify)
	ON_MESSAGE(WM_ASYNC_EVENT, OnAsyncSocketMessage)

	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_ERROR, OnUpdateFileDownloadError)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_INIT, OnUpdateFileDownloadInit)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_PROGRESS, OnUpdateFileDownloadProgress)
	ON_MESSAGE(WM_UPDATEFILE_DOWNLOAD_OK, OnUpdateFileDownloadOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumDlg message handlers

BOOL CAtumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	this->SetWindowText(STRMSG_WINDOW_TEXT);		// 2006-12-22 by cmkwon

	// Initialize winsock 2.0
	CWinSocket::SocketInit();

	// Make socket instance & connect
	m_pPreUpdateWinsocket = new CPreUpdateWinSocket(GetSafeHwnd());
	if (!m_pPreUpdateWinsocket->Connect((LPCSTR)m_szMainPreServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		AtumMessageBox(STRERR_S_ATUMEXE_0001);
// 2007-05-15 by cmkwon, 다른 곳으로 이동
//		SAFE_DELETE(m_pPreUpdateWinsocket);
		EndDialog(-1);
		return FALSE;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAtumDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAtumDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAtumDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

struct sort_SGAME_SERVER_GROUP_By_Tap8OrderIndex			// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - - LoginServer 리스트 정렬 
{
	bool operator()(SGAME_SERVER_GROUP op1, SGAME_SERVER_GROUP op2)
	{
		return op1.usPreServerTab8OrderIndex < op2.usPreServerTab8OrderIndex;				// 오름차순 정렬
	}
};

LONG CAtumDlg::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	CPreUpdateWinSocket *pPreUpdateWinSocket = (CPreUpdateWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CPreUpdateWinSocket::WS_ERROR:
		{
		}
		break;
	case CPreUpdateWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				/////////////////////////////////////////////
				// Send Version
// 2007-01-18 by cmkwon, HTTP 자동 업데이트 구현으로 아래와 같이 수정함
//				char buff[SIZE_MAX_PACKET];
//				*(USHORT*)buff = (USHORT)T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION;
//				MSG_PC_DEFAULT_UPDATE_LAUNCHER_VERSION msgVersion;
//				const USHORT* pCurrentVersion = m_CurrentLauncherVersion.GetVersion();
//				msgVersion.Version[0] = pCurrentVersion[0];
//				msgVersion.Version[1] = pCurrentVersion[1];
//				msgVersion.Version[2] = pCurrentVersion[2];
//				msgVersion.Version[3] = pCurrentVersion[3];
//				memcpy(buff + SIZE_FIELD_TYPE_HEADER, &msgVersion, sizeof(MSG_PC_DEFAULT_UPDATE_LAUNCHER_VERSION));
//				pPreUpdateWinSocket->Write(buff, MSG_SIZE(MSG_PC_DEFAULT_UPDATE_LAUNCHER_VERSION));

// 2007-05-15 by cmkwon, 두개의 PreServer 지원 시스템 구현으로 시스템 수정
//				INIT_MSG_WITH_BUFFER(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, pSMsg, SendBuf);
//				m_CurrentLauncherVersion.GetVersion(pSMsg->Version);
//				pPreUpdateWinSocket->Write(SendBuf, MSG_SIZE(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION));

				if(m_vectGameServerGroupList.empty())
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2007-05-15 by cmkwon, PreServer IP 리스트 요청한다
					// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 추가된 프로토콜로 요청함
					//pPreUpdateWinSocket->WriteMessageType(T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST);
					pPreUpdateWinSocket->WriteMessageType(T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST);
				}
				else
				{
					INIT_MSG_WITH_BUFFER(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, pSMsg, SendBuf);
					m_CurrentLauncherVersion.GetVersion(pSMsg->Version);
					pPreUpdateWinSocket->Write(SendBuf, MSG_SIZE(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION));
				}
			}
			else
			{
				AtumMessageBox(STRERR_S_ATUMEXE_0001);
				EndDialog(-1);
			}
		}
		break;
	case CPreUpdateWinSocket::WS_CLOSED:
		{
// 2007-05-15 by cmkwon, 다른 곳으로 이동
//			SAFE_DELETE(pPreUpdateWinSocket);

			AtumMessageBox(STRERR_S_ATUMEXE_0002);

			OnCancel();
		}
		break;
	case CPreUpdateWinSocket::WS_RECEIVED:
		{
			char * pPacket = NULL;
			int len,nType;
			pPreUpdateWinSocket->Read(&pPacket, len);

			if(pPacket)
			{
				nType = 0;
				memcpy(&nType, pPacket, SIZE_FIELD_TYPE_HEADER);

				switch(nType)
				{
				case T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK:
					{
						m_progressCtrl.SetRange(0,100);
						m_progressCtrl.SetPos(100);

						if (m_szCrocessSuffix == "")
						{
							ExecLauncher();
						}
						else
						{
							// 크로세스를 통해 실행
							ExecLauncherCrocess();
						}

						OnCancel();
					}
					break;
				case T_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO:
					{
						MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo
							= (MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 타이머 동작 시작
						SetTimer(TIMERID_SEND_ALIVE_PACKET, 30000, NULL);

						m_UpdatingLauncherVersion.SetVersion(pMsgUpdateInfo->UpdateVersion);		// 2007-01-06 by cmkwon
						BOOL ret = GetLauncherFileFTP(pMsgUpdateInfo);
						if (!ret)
						{
							// error
							AtumMessageBox(STRERR_S_ATUMEXE_0003);
							ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
							EndDialog(-1);
							OnCancel();
						}
						else
						{
							if (m_szCrocessSuffix == "")
							{
								ExecLauncher();
							}
							else
							{
								// 크로세스를 통해 실행
								ExecLauncherCrocess();
							}

							OnCancel();
						}
					}
					break;
				case T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO:		// 2007-01-06 by cmkwon
					{
						MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo
							= (MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 타이머 동작 시작
						SetTimer(TIMERID_SEND_ALIVE_PACKET, 30000, NULL);

						m_UpdatingLauncherVersion.SetVersion(pMsgUpdateInfo->UpdateVersion);

						switch(pMsgUpdateInfo->nAutoUpdateServerType)
						{
						case AU_SERVER_TYPE_HTTP:
							{// 2007-01-08 by cmkwon, HTTP Update
								GetLauncherFileHTTP(pMsgUpdateInfo);
							}
							break;
						default:
							{// 2007-01-08 by cmkwon, FTP Update
								MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO tmFTPUpdateInfo;

								MEMSET_ZERO(&tmFTPUpdateInfo, sizeof(tmFTPUpdateInfo));
								for(int i=0; i < SIZE_MAX_VERSION; i++)
								{
									tmFTPUpdateInfo.UpdateVersion[i]	= pMsgUpdateInfo->UpdateVersion[i];
								}
								STRNCPY_MEMSET(tmFTPUpdateInfo.FtpIP, pMsgUpdateInfo->UpdateServerIP, SIZE_MAX_FTP_URL);
								tmFTPUpdateInfo.FtpPort				= pMsgUpdateInfo->UpdateServerPort;
								STRNCPY_MEMSET(tmFTPUpdateInfo.FtpAccountName, pMsgUpdateInfo->AccountName, SIZE_MAX_ACCOUNT_NAME);
								STRNCPY_MEMSET(tmFTPUpdateInfo.FtpPassword, pMsgUpdateInfo->Password, SIZE_MAX_PASSWORD);
								STRNCPY_MEMSET(tmFTPUpdateInfo.LauncherFileDownloadPath, pMsgUpdateInfo->LauncherFileDownloadPath, SIZE_MAX_FTP_FILE_PATH);
								
								BOOL ret = GetLauncherFileFTP(&tmFTPUpdateInfo);
								if (!ret)
								{
									// error
									AtumMessageBox(STRERR_S_ATUMEXE_0003);
									ShellExecute(NULL, "open", STRMSG_S_GAMEHOMEPAGE_DOMAIN, NULL, NULL, SW_SHOWNORMAL);
									EndDialog(-1);
									OnCancel();
								}
								else
								{
									if (m_szCrocessSuffix == "")
									{
										ExecLauncher();
									}
									else
									{
										// 크로세스를 통해 실행
										ExecLauncherCrocess();
									}

									OnCancel();
								}
							}
						}
					}
					break;
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 추가된 프로토콜로 처리함
//				case T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK:
//					{
//						MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK *pRGServerList = (MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);
//						
//						for(int i=0; i < COUNT_MAX_GAME_SERVER_GROUP_LIST; i++)
//						{
//							if(0 == strncmp(pRGServerList->arrGameServerGroupList[i].szGameServerGroupName, "", SIZE_MAX_GAME_SERVER_GROUP_NAME)
//								|| 0 == strncmp(pRGServerList->arrGameServerGroupList[i].szPreServerIP0, "", SIZE_MAX_IPADDRESS))
//								// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 필드 제거, 포트는 기본 포트 사용
//								//|| 0 == pRGServerList->arrGameServerGroupList[i].usPreServerPort0)
//							{
//								break;
//							}
//// 2007-05-15 by cmkwon
////							DbgOut("%s(%s:%d)\r\n", pRGServerList->arrGameServerGroupList[i].szGameServerGroupName, pRGServerList->arrGameServerGroupList[i].szPreServerIP0, pRGServerList->arrGameServerGroupList[i].usPreServerPort0);
//							m_vectGameServerGroupList.push_back(pRGServerList->arrGameServerGroupList[i]);
//						}
//						
//						if(m_vectGameServerGroupList.empty())
//						{// 2007-05-15 by cmkwon, 연결 할 PreServer 정보가 없다
//							AtumMessageBox(STRERR_S_ATUMEXE_0001);
//							EndDialog(-1);
//							return FALSE;
//						}
//
//						BOOL bChangePreServer = FALSE;
//						CSelectGameServerDlg dlg(&m_vectGameServerGroupList, this);
//						
//						if(1 < m_vectGameServerGroupList.size())
//						{// 2007-05-15 by cmkwon, 연결 할 PreSerer 정보가 2개 이상이면
//
//							// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - LoginServer List 정렬
//							sort(m_vectGameServerGroupList.begin(), m_vectGameServerGroupList.end(), sort_SGAME_SERVER_GROUP_By_Tap8OrderIndex());
//
//							if(IDCANCEL == dlg.DoModal())
//							{
//								EndDialog(-1);
//								SAFE_DELETE(pPacket);
//								return FALSE;
//							}
//							
//							if(0 != strncmp(dlg.m_selectedGameServerGroup.szPreServerIP0, pPreUpdateWinSocket->m_szPeerIP, SIZE_MAX_IPADDRESS))
//							{// 2007-05-15 by cmkwon, 선태한 PreServer IP와 현재 연결중인 Preserver IP가 다르면 PreServer를 변경해야 한다.
//								bChangePreServer		= TRUE;
//							}
//						}
//
//						if(FALSE == bChangePreServer)
//						{
//							INIT_MSG_WITH_BUFFER(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, pSMsg, SendBuf);
//							m_CurrentLauncherVersion.GetVersion(pSMsg->Version);
//							pPreUpdateWinSocket->Write(SendBuf, MSG_SIZE(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION));
//						}
//						else
//						{
//							pPreUpdateWinSocket->CloseSocket();		
//							Sleep(500);
//
//							// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - Port는 기본 포트를 사용함
//							if (FALSE == pPreUpdateWinSocket->Connect(dlg.m_selectedGameServerGroup.szPreServerIP0, PRE_SERVER_PORT))
//							{
//								int err = GetLastError();
//								AtumMessageBox(STRERR_S_ATUMEXE_0001);
//								EndDialog(-1);
//								SAFE_DELETE(pPacket);
//								return FALSE;
//							}
//
//							// 2007-05-15 by cmkwon, 연결하는 PreServer IP를 저장
//							m_szMainPreServerIP	= dlg.m_selectedGameServerGroup.szPreServerIP0;
//						}
//
//					}
//					break;

				case T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK:
					{
						MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK *pRGServerList = (MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						
						for(int i=0; i < COUNT_MAX_GAME_SERVER_GROUP_LIST; i++)
						{
							if(0 == strncmp(pRGServerList->arrGameServerGroupList[i].szGameServerGroupName, "", SIZE_MAX_GAME_SERVER_GROUP_NAME)
								|| 0 == strncmp(pRGServerList->arrGameServerGroupList[i].szPreServerIP0, "", SIZE_MAX_IPADDRESS))
							{
								break;
							}
// 2007-05-15 by cmkwon
//							DbgOut("%s(%s:%d)\r\n", pRGServerList->arrGameServerGroupList[i].szGameServerGroupName, pRGServerList->arrGameServerGroupList[i].szPreServerIP0, pRGServerList->arrGameServerGroupList[i].usPreServerPort0);
							m_vectGameServerGroupList.push_back(pRGServerList->arrGameServerGroupList[i]);
						}
						
						if(m_vectGameServerGroupList.empty())
						{// 2007-05-15 by cmkwon, 연결 할 PreServer 정보가 없다
							AtumMessageBox(STRERR_S_ATUMEXE_0001);
							EndDialog(-1);
							return FALSE;
						}

						///////////////////////////////////////////////////////////////////////////////
						// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - 첫번째 서버 정보를 임시 변수에 저장
						SGAME_SERVER_GROUP tmFirstGServ;
						MEMSET_ZERO(&tmFirstGServ, sizeof(tmFirstGServ));
						tmFirstGServ		= *(m_vectGameServerGroupList.begin());

						BOOL bChangePreServer = FALSE;
						CSelectGameServerDlg dlg(&m_vectGameServerGroupList, this);
						
						if(1 < m_vectGameServerGroupList.size())
						{// 2007-05-15 by cmkwon, 연결 할 PreSerer 정보가 2개 이상이면

							///////////////////////////////////////////////////////////////////////////////
							// 2007-09-14 by cmkwon, 로그인 서버가 2개 이상이면 무조건 연결을 종료하고 다시 연결하도록 수정한다.
							pPreUpdateWinSocket->CloseSocket();		
							Sleep(500);
							bChangePreServer		= TRUE;

							// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - LoginServer List 정렬
							sort(m_vectGameServerGroupList.begin(), m_vectGameServerGroupList.end(), sort_SGAME_SERVER_GROUP_By_Tap8OrderIndex());

							if(IDCANCEL == dlg.DoModal())
							{
								EndDialog(-1);
								SAFE_DELETE(pPacket);
								return FALSE;
							}

// 2007-09-14 by cmkwon, 체크 필요 없음							
//							if(0 != strncmp(dlg.m_selectedGameServerGroup.szPreServerIP0, pPreUpdateWinSocket->m_szPeerIP, SIZE_MAX_IPADDRESS))
//							{// 2007-05-15 by cmkwon, 선태한 PreServer IP와 현재 연결중인 Preserver IP가 다르면 PreServer를 변경해야 한다.
//							}
						}

						if(FALSE == bChangePreServer)
						{
							// 2009-01-15 by cmkwon, PreServer, DBServer 정보 DNS로 설정 가능하게 수정 - 첫번째 리스트의 연결하는 PreServer 정보를 IP 변경해서 저장
							m_szMainPreServerIP	= tmFirstGServ.szPreServerIP0;

							INIT_MSG_WITH_BUFFER(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, pSMsg, SendBuf);
							m_CurrentLauncherVersion.GetVersion(pSMsg->Version);
							pPreUpdateWinSocket->Write(SendBuf, MSG_SIZE(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION));
						}
						else
						{
							// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - Port는 기본 포트를 사용함
							if (FALSE == pPreUpdateWinSocket->Connect(dlg.m_selectedGameServerGroup.szPreServerIP0, PRE_SERVER_PORT))
							{
								int err = GetLastError();
								AtumMessageBox(STRERR_S_ATUMEXE_0001);
								EndDialog(-1);
								SAFE_DELETE(pPacket);
								return FALSE;
							}

							// 2007-05-15 by cmkwon, 연결하는 PreServer IP를 저장
							m_szMainPreServerIP	= dlg.m_selectedGameServerGroup.szPreServerIP0;
						}

					}
					break;

				case T_ERROR:
					{
						MSG_ERROR *pRecvMsg;
						pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						char buf[128];
						Err_t error = pRecvMsg->ErrorCode;

						DBGOUT(STRERR_S_ATUMEXE_0004, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_PRE_SERVER", m_pPreUpdateWinsocket->m_szPeerIP);

						switch (error)
						{
						case ERR_COMMON_UNKNOWN_ERROR:
							break;
						default:
							{
								sprintf(buf, STRERR_S_ATUMEXE_0005, GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								AtumMessageBox(buf);
							}
							break;
						}	// end of 'switch (error)'
					}
				default:
					{
					}
					break;
				}

				SAFE_DELETE(pPacket);
			}
		}
		break;
	}// end_switch(LOWORD(wParam))

	return 0;
}

// WPARAM: Socket descriptor
LONG CAtumDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	m_pPreUpdateWinsocket->OnAsyncEvent(lParam);

	return 0;
}

LONG CAtumDlg::OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam)
{
	char szErrString[1024];
	char szErrAddStr[1024];
	MEMSET_ZERO(szErrString, 1024);
	MEMSET_ZERO(szErrAddStr, 1024);
	switch(wParam)
	{
	case ERR_CANNOT_CONNECT_AUTO_UPDATE_SERVER:
		{
			strcpy(szErrAddStr, STRERR_S_ATUMEXE_0006);
		}
		break;
	case ERR_LOCAL_FILE_CREATE_FAIL:
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0001);
		}
		break;
	case ERR_UPDATE_FILE_NOT_FOUND:
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0000);
		}
		break;
	case ERR_UPDATE_FILE_DOWNLOADING_FAIL:
		{
			strcpy(szErrAddStr, STRCMD_CS_COMMON_DOWNLOAD_0002);
		}
		break;
	default:
		{
			wsprintf(szErrAddStr, "Nomal download error(%d) !!", wParam);
		}
	}

	wsprintf(szErrString, STRMSG_060526_0001
		, STRMSG_S_GAMEHOMEPAGE_DOMAIN, szErrAddStr);
	AtumMessageBox(szErrString);
	OnCancel();
	return FALSE;
}

LONG CAtumDlg::OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam)
{
	m_progressCtrl.SetRange32(0, wParam);
	m_progressCtrl.SetPos(0);
	return TRUE;
}

LONG CAtumDlg::OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam)
{
	m_progressCtrl.SetPos(wParam);
	return TRUE;
}

LONG CAtumDlg::OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam)
{
	///////////////////////////////////////////////////////////////////////////////
	// update version write registry
	m_CurrentLauncherVersion = m_UpdatingLauncherVersion;
	((CAtumApp*)AfxGetApp())->WriteProfile();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-05 by cmkwon, Update Httpmanager thread 
	if(m_pHttpManager)
	{
		m_pHttpManager->ThreadEnd(1000);
		SAFE_DELETE(m_pHttpManager);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-05 by cmkwon, excute Launcher
	if (m_szCrocessSuffix == "")
	{
		ExecLauncher();
	}
	else
	{
		// 크로세스를 통해 실행
		ExecLauncherCrocess();
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-05 by cmkwon, program end
	OnCancel();
	return TRUE;
}



void CAtumDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	SAFE_DELETE(m_pPreUpdateWinsocket);

	CWinSocket::SocketClean();

}

BOOL CAtumDlg::GetLauncherFileFTP(MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo)
{
	CFTPManager ftpManager;
	BOOL bRet;

	DbgOut("Updating launcher file(%s) From %s:%d By FTP\r\n", pMsgUpdateInfo->LauncherFileDownloadPath, pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort);	

	bRet = ftpManager.ConnectToServer(pMsgUpdateInfo->FtpIP, pMsgUpdateInfo->FtpPort,
									pMsgUpdateInfo->FtpAccountName, pMsgUpdateInfo->FtpPassword);
	if (!bRet)
	{
		AtumMessageBox(STRERR_S_ATUMEXE_0006);
		return FALSE;
	}

	HINTERNET hFile;
 	int nFileSize = ftpManager.GetFileSize(pMsgUpdateInfo->LauncherFileDownloadPath, hFile);
 	if (nFileSize == -1)
 	{
// 		AtumMessageBox(STRERR_S_ATUMEXE_0007);
// 		return FALSE;
		nFileSize = 2700000;	// 2006-06-30 by cmkwon, 1.5MBytes = 1572864
 	}
	m_progressCtrl.SetRange32(0, nFileSize);

	bRet = ftpManager.DownloadFile(pMsgUpdateInfo->LauncherFileDownloadPath, NULL, &m_progressCtrl);
	if (!bRet)
	{
		AtumMessageBox(STRERR_S_ATUMEXE_0008);
		return FALSE;
	}

	// set version
	m_CurrentLauncherVersion.SetVersion(
		pMsgUpdateInfo->UpdateVersion[0],
		pMsgUpdateInfo->UpdateVersion[1],
		pMsgUpdateInfo->UpdateVersion[2],
		pMsgUpdateInfo->UpdateVersion[3]
	);
	((CAtumApp*)AfxGetApp())->WriteProfile();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumDlg::GetLauncherFileHTTP(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-03 ~ 2007-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDlg::GetLauncherFileHTTP(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo)
{
	m_pHttpManager = new CHttpManager;

	DbgOut("Updating launcher file(%s) From %s:%d By HTTP\r\n", pMsgUpdateInfo->LauncherFileDownloadPath, pMsgUpdateInfo->UpdateServerIP, pMsgUpdateInfo->UpdateServerPort);

	m_pHttpManager->DownloadFileByHttp(pMsgUpdateInfo->UpdateServerIP, pMsgUpdateInfo->UpdateServerPort, pMsgUpdateInfo->LauncherFileDownloadPath, LAUNCHER_FILE_NAME, TRUE, this->GetSafeHwnd());
	return TRUE;
}

void CAtumDlg::ExecLauncher()
{
	// Write Profile
	((CAtumApp*)AfxGetApp())->WriteProfile();

	char	szCmdLine[1024];			MEMSET_ZERO(szCmdLine, 1024);
	char	szTemp[1024];				MEMSET_ZERO(szTemp, 1024);
	sprintf(szCmdLine, "%s %s", LAUNCHER_FILE_NAME, m_szMainPreServerIP);		// 2007-05-15 by cmkwon, m_szMainPreServerIP 추가

	UINT ret = WinExec(szCmdLine, SW_SHOWNORMAL);
	DbgOut("EXE 1 CommandLine : %s, RetCode(%d)\r\n", szCmdLine, ret);		// 2007-05-16 by cmkwon
	if ( ret <= 31 )	// exec failed...
	{
		switch (ret)
		{
		case 0:						// The system is out of memory or resources.
			AtumMessageBox(STRERR_S_ATUMEXE_0009);
			break;
		case ERROR_BAD_FORMAT:		// The .exe file is invalid.
			AtumMessageBox(STRERR_S_ATUMEXE_0010);
			break;
		case ERROR_FILE_NOT_FOUND:	// The specified file was not found.
			AtumMessageBox(STRERR_S_ATUMEXE_0011);
			break;
		case ERROR_PATH_NOT_FOUND:	// The specified path was not found.
			AtumMessageBox(STRERR_S_ATUMEXE_0012);
			break;
		}
	}
}

void CAtumDlg::ExecLauncherCrocess()
{
	DWORD dwExitCode;
	PROCESS_INFORMATION pi;

	STARTUPINFO si = {sizeof(si)};
	ZeroMemory(&si,sizeof(si));

	CreateProcess(
		NULL,				// name of executable module
		LAUNCHER_FILE_NAME,	// command line string
		NULL,				//LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
		0,					//BOOL bInheritHandles,                      // handle inheritance option
		0,					//DWORD dwCreationFlags,                     // creation flags
		NULL,				//LPVOID lpEnvironment,                      // new environment block
		NULL,				//LPCTSTR lpCurrentDirectory,                // current directory name
		&si,				//LPSTARTUPINFO lpStartupInfo,               // startup information
		&pi					//LPPROCESS_INFORMATION lpProcessInformation // process information
		);


	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_OBJECT_0) {
		// Process 종료
		//AtumMessageBox("Process Terminated!");
	}

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CAtumDlg::AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/*=NULL*/, UINT nType/*=MB_OK*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-20 ~ 2006-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CAtumDlg::AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption/*=NULL*/, UINT nType/*=MB_OK*/)
{
	return MessageBox(lpszText, lpszCaption, nType);
}

void CAtumDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	if(m_pHttpManager)
	{
		m_pHttpManager->SetDownloadThreadCancelFlag(TRUE);
		Sleep(100);
		m_pHttpManager->ThreadEnd(1000);
		SAFE_DELETE(m_pHttpManager);
	}
	
	CDialog::OnCancel();
}

void CAtumDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDEvent)
	{
	case TIMERID_SEND_ALIVE_PACKET:
		{
			// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 전송
			if(m_pPreUpdateWinsocket
				&& m_pPreUpdateWinsocket->IsConnected())
			{
				m_pPreUpdateWinsocket->WriteMessageType(T_PC_CONNECT_ALIVE);
			}
		}
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CAtumDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 타이머 종료
	KillTimer(TIMERID_SEND_ALIVE_PACKET);
	return CDialog::DestroyWindow();
}
