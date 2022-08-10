// PreIOCP.h: interface for the CPreIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
#define AFX_PREIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "IOCP.h"
#include "ServerGroup.h"
#include "AtumPreDBManager.h"
#include "PreTickManager.h"


// client의 pre server내의 상태
typedef enum
{
	CP_NOTCONNECTED					= 0,	// Client가 접속하지 않은 상태
	CP_CONNECTED					= 1,	// Client가 Socket 접속한 상태
	CP_SINGLE_FILE_VERSIONCHECKED	= 2,	// Client가 SINGLE FILE VERSION이 update된 상태
	CP_VERSIONCHECKED				= 3,	// Client가 VERSION이 update된 상태
	CP_LOGINED						= 4,	// Client가 AccountName을 가지고 Login 된 상태, Launcher에 의한 로그인
	CP_FIELD_LOGINED				= 5,	// Client가 필드 서버에 의해 로그인 된 상태, 캐릭터 선택 화면에 있는 상황
	CP_GAMESTARTED					= 6		// Client가 게임을 시작하여 맵에 있는 상황
} CP_State;


class CAccountInfo
{
public:
	CAccountInfo()
	{
		memset(this, 0x00, sizeof(CAccountInfo));
	}

	void SetBirthday(int i_nBirthday);	// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 함수 추가

public:
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t				AccountUniqueNumber;
	USHORT				AccountType;								// 2006-06-02 by cmkwon
	char				PublicClientIP[SIZE_MAX_IPADDRESS];
	char				PrivateClientIP[SIZE_MAX_IPADDRESS];
	char				CurrentCharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CurrentCharacterUniqueNumber;
	char				CurrentServerGroup[SIZE_MAX_SERVER_NAME];
	SERVER_ID			CurrentFieldServerID;
	ClientIndex_t		CurrentClientIndex;			// 필드 서버에서의 client index
	ClientIndex_t		CurrentPreServerClientIndex;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - PreServer ClientIndex
	MAP_CHANNEL_INDEX	CurrentMapChannel;
	CP_State			ClientState;
	ATUM_DATE_TIME		LauncherLoginTime;
	ATUM_DATE_TIME		AccountRegisteredDate;		// 2006-06-01 by cmkwon, 계정이 DB에 등록된 일시
	int					nExtAuthAccountIDNum;		// 2006-05-22 by cmkwon, 외부 인증의 AccountIDNum
	char				PasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];		// 2006-06-02 by cmkwon
	int					GameContinueTimeInSecondOfToday;				// 2006-11-15 by cmkwon, 오늘 하루 게임 접속 시간
	ATUM_DATE_TIME		LastGameEndDate;								// 2006-11-15 by cmkwon, 마지막 게임 종료 시간
	ATUM_DATE_TIME		atBirthday;						// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 생년 월일
	char				SecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];		// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CAccountInfo 에 필드 추가
#ifdef S_ARARIO_HSSON
	eCONNECT_PUBLISHER	eOtherPublisherConncect;				// 2010-11 by dhjin, 아라리오 채널링 로그인.
#endif
	BOOL				bSelectiveShutdownTarget;										// 2012-07-11 by hskim, 선택적 셧다운
	char				SelectiveShutdownInfo[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO];		// 2012-07-11 by hskim, 선택적 셧다운
};

inline MEX_ACCOUNT_INFO_PRE_SERVER& MEX_ACCOUNT_INFO_PRE_SERVER::operator=(const CAccountInfo& rhs)
{
	strncpy(this->AccountName, rhs.AccountName, SIZE_MAX_ACCOUNT_NAME);
	this->AccountUniqueNumber = rhs.AccountUniqueNumber;
	strncpy(this->PublicClientIP, rhs.PublicClientIP, SIZE_MAX_IPADDRESS);
	strncpy(this->PrivateClientIP, rhs.PrivateClientIP, SIZE_MAX_IPADDRESS);
	strncpy(this->CurrentCharacterName, rhs.CurrentCharacterName, SIZE_MAX_CHARACTER_NAME);
	this->CurrentCharacterUniqueNumber = rhs.CurrentCharacterUniqueNumber;
	strncpy(this->CurrentServerGroup, rhs.CurrentServerGroup, SIZE_MAX_SERVER_NAME);
	this->CurrentFieldServerID = rhs.CurrentFieldServerID;
	this->CurrentMapChannel = rhs.CurrentMapChannel;
	this->ClientState = rhs.ClientState;

	return *this;
}

typedef mt_map<string, CAccountInfo*, greater<string> >		mtmapAccountInfoPtr;
// 2007-01-09 by cmkwon, AtumParam.h로 위치 변경 typedef mt_map<VersionInfo, VersionInfo>					mtmapVersionInfo;

class CPreIOCPSocket;
class CAccountBlockManager;
class CInnovaLibrary;			// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
class CHttpManager;				// PreServer 시작 시 자동 버젼 업데이트 갱신
class CPreIOCP : public CIOCP
{
	friend class CPreIOCPSocket;
	friend class CAtumPreDBManager;

public:
	CPreIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CPreIOCP();

public:
	// account 관련
	BOOL IsLogined(char* accountName);
	BOOL GetAccountInfo(CAccountInfo *o_pAccountInfo, const char* accountName);
	CAccountInfo *GetAccountInfoPtr(const char *i_szAccountName);
	BOOL InsertAccountInfo(char* accountName, CAccountInfo* accountInfo);
	BOOL DeleteAccountInfo(char* accountName);
	void DeleteAccountInfoByServerGroupName(const char *i_szServerGroup);
//	BOOL SendForceLogout(char* accountName);
	// 2010-11 by dhjin, 아라리오 채널링 로그인.
	INT	GetOtherPublisherConncectCnt(eCONNECT_PUBLISHER	i_eOtherPublisherConncect, char * i_pServerName);

	// FTP server로부터 version의 list를 load함
//	BOOL CreateVersionInfoFromFTP();
	BOOL CreateVersionInfoFromLocalFile();
	BOOL WriteVersionInfoToLocalFile();
	BOOL GetLatestUpdateVersion(VersionInfo *o_pLatestVersion);
	BOOL GetNextUpdateVersion(VersionInfo *o_pNextVersion, VersionInfo *i_pVersion);	// 해당 버전이 있는지 확인하고, 업데이트할 버전을 리턴함

	// 계정 관리
	BOOL BlockAccount(SBLOCKED_ACCOUNT_INFO *i_pBlockedAccountInfo, CPreIOCPSocket *i_pSocketToNotify, BOOL i_bDisconnectUser=TRUE);
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 
	//BOOL UnblockAccount(const char *i_szAccountName, CPreIOCPSocket *i_pSocketToNotify);
	BOOL UnblockAccount(MSG_PA_ADMIN_UNBLOCK_ACCOUNT *i_pUnblockAccount, CPreIOCPSocket *i_pSocketToNotify);

	// 계정 연결 끊기
	BOOL DisconnectAccount(CAccountInfo *i_pAccountInfo, Err_t i_nError, CPreIOCPSocket *i_pSocketToNotify);

	// 계정 관련 
	int On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, MSG_FP_CONNECT_AUTH_USER *i_pMsg);
	int On_MSG_FP_EVENT_NOTIFY_WARP(MSG_FP_EVENT_NOTIFY_WARP *i_pMsg);
	int On_MSG_FP_EVENT_ENTER_SELECT_SCREEN(CP_State *o_pState, MSG_FP_EVENT_ENTER_SELECT_SCREEN *i_pMsg);
	int On_MSG_FP_EVENT_GAME_STARTED(CP_State *o_pState, MSG_FP_EVENT_GAME_STARTED *i_pMsg);
	int On_MSG_FP_EVENT_MAP_CHANGED(CP_State *o_pState, MSG_FP_EVENT_MAP_CHANGED *i_pMsg);
	int On_MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE(MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE *i_pMsg);

	// debug
	void DBG_PrintAccountMap();

	// LoadBlockedIDList
	void LoadBlockedAccounts();
	BOOL InitDBTable(CODBCStatement *i_pODBCStmt);

	BOOL LoadSystemEvent(CODBCStatement *i_pODBCStmt);		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]

	// 서버 그룹 관리
	vector<CServerGroup*> * GetAllServerGroupVector(void) { return &m_AllServerGroupVector; }
	CServerGroup * GetServerGroup(const char * szServerGruopName);
	void ResetServerGroup(const char *i_szServerGroupName, BOOL i_bExcludeIMServer=FALSE);
	void SendServerInfoForAdmin(CPreIOCPSocket *i_pSocket);
	BOOL SendErrorToFieldServer(char *i_szServerGroupName, SERVER_ID i_FieldServerid, MessageType_t msgType, Err_t err, int errParam1=0, int errParam2=0, char* errMsg=NULL, BOOL bCloseConnection=FALSE);

	void SendMessageToAllFieldServer(BYTE *i_pBuff, int i_nSize);
	void SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize);			// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 

	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);

	void CheckOnlyLauncherLoginedList(ATUM_DATE_TIME *pDateTime);

	void StoredLogTotalUser(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	BOOL ExtAuthODBCInit(void);
	void ExtAuthODBCReset(void);
	BOOL ExtAuthODBCAuthentication(char *i_szAccName, char *i_szPwd);

	// 2012-07-11 by hskim, 선택적 셧다운
	BOOL SelectiveShutdownProgress(char *pAccountName);
	BOOL SelectiveShutdownNotice(char *pAccountName, int nMin);
	// end 2012-07-11 by hskim, 선택적 셧다운


public:
	inline CPreIOCPSocket* GetPreIOCPSocket(int idx);

	// PreServer 시작 시 자동 버젼 업데이트 갱신
	BOOL PreGetVersionInfoFromFTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);
	BOOL PreGetVersionInfoByHTTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);
	CString GetOnlyPathWithoutFileName(CString & i_szFilePath);
	BOOL AutoUpdateVersionByPreSvr();

public:
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);

private:
	mtmapAccountInfoPtr		m_AccountInfoMap;
	CAtumPreDBManager		*m_pAtumDBManager;
	CPreTickManager			*m_pPreTickManager;				// Tick manager
	VersionInfo				m_LatestClientVersion;
	// 2004-10-19 by cmkwon
	//ez_map<VersionInfo, VersionInfo>	m_mapVersionOld2New;	// OldVersion -> NewVersion
	BOOL					m_bPauseService;				// 잠시 서비스를 중단할 때 사용(예: version reloading...)
	mt_set<string>			m_setBlockedAccounts;			// 사용 금지 계정
	mtmapVersionInfo		m_mtmapVersionOld2New;	// OldVersion -> NewVersion
	VersionInfo				m_LatestLauncherVersion;		// launcher 버젼
	VersionInfo				m_LatestDeleteFileListVersion;	// delete file list 버젼
	VersionInfo				m_LatestNoticeVersion;			// notice.txt 버젼

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-09 by cmkwon, 버전 Reload를 위한 임시 변수들
	int						m_tmTotalNumOfClientVersions;	
	mtmapVersionInfo		m_tmmtmapVersionOld2New;
	VersionInfo				m_tmLatestLauncherVersion;
	VersionInfo				m_tmLatestDeleteFileListVersion;
	VersionInfo				m_tmLatestNoticeVersion;

	CAccountBlockManager	*m_pAccountBlockManager;		// 계정 블럭 관리자

	// 서버 그룹 관리
	vector<CServerGroup*>	m_AllServerGroupVector;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	CODBCStatement			m_odbcExtAuth;					// 2006-04-27 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// PreServer 시작 시 자동 버젼 업데이트 갱신	
	char					m_szDownloadHttpServerIP[SIZE_MAX_FTP_URL];

	// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
	ATUM_DATE_TIME			m_OpeningMovieStandard;			// 동영상 재생 여부 기준 시간
};

CPreIOCPSocket* CPreIOCP::GetPreIOCPSocket(int idx)
{
	return (CPreIOCPSocket*)GetIOCPSocket(idx);
}

#endif // !defined(AFX_PREIOCP_H__89992FF9_ADF7_4FFF_A639_F9BC79F85619__INCLUDED_)
