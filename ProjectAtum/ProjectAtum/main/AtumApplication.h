// AtumApplication.h: interface for the CAtumApplication class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMAPPLICATION_H__32E91AE4_C7F3_424D_B983_17488D8A215B__INCLUDED_)
#define AFX_ATUMAPPLICATION_H__32E91AE4_C7F3_424D_B983_17488D8A215B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "D3DApp.h"
#include "IMEKey.h"		// imm32.lib
#include "INFGameMainQSlot.h"
// 2008-03-19 by bhsohn Ep3옵션창
//#include "INFSystem.h"
#include "INFOptionSystem.h"

#include "CInput.h"

// 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경
#include "sha256.h"
// end 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경
#include "zwave_sdk_client.h"		 // 2012-10-13 by jhahn 핵쉴드->Xigncode변경

class CFrustum;
class CD3DHanFont;
class CKeyBoardInput;
class CCamera;
class CShuttleRender;
class CShuttleChild;
class CCharacterChild;
class CTraceRender;
class IMEKey;
class CChat;
class CFieldWinSocket;
class CIMSocket;
// 2007-11-22 by bhsohn 아레나 통합서버
class CIMSocketManager;
class CFieldWinSocketManager;
class CArenaManager;

class CMapInitThread;
class CGameData;
class CEffectRender;
class CAtumNode;
class CInterface;
class CAbuseFilter;
class CAtumSound;
class CSceneData;
class CAtumDatabase;
class CINFStageEffect;
class CINFGameMain;
class CStoreData;
class CQuestData;
class CBackground;
class CINFSelect;
class CINFChannelList;
class CUnitRender;
class CCharacterRender;						// 2005-07-21 by ispark
class CFxSystem;
class CTutorialSystem;
class CInputManager;
class CGlobalImage;
class CUnitData;
class CSinglePlayGame;	// 2008-08-11 by bhsohn 단독으로 실행 되게끔
// 2008-11-13 by bhsohn 조이스틱 작업
class CJoyStickInput;	
class CJoyStickFreedback;
// end 2008-11-13 by bhsohn 조이스틱 작업

class CWorldRankManager;		// 2009-02-13 by bhsohn 월드 랭킹 시스템

// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
class CInfinityManager;
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
class CSkinnedMesh;
class CMeshInitThread;
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

// 2010-06-15 by shcho&hslee 펫시스템
class CPetManager;
// end 2010-06-15 by shcho&hslee 펫시스템

class CINFEnemyItemInfo;	// 2012-06-14 by isshin 아템미리보기

class CMapLoad;    // 2010. 10. 05 by jskim 맵로딩 구조 변경

// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupManager;
class CINFImageList;
// end 2011. 10. 10 by jskim UI시스템 변경
class CVTCThread;			// 2011. 1. 18 by jskim 배트남 VTC가드

class CScreenShotManager;	// 2015-07-18 Future, Screen Shot Manager

typedef struct 
{
	INT			ItemNum;						// 아이템 고유번호, 장착 아이템일 때 (ITEM_BASE*)
	BYTE		byItemKind;				// 아이템 종류(기관포, 빔, 로켓, 스킬.....)
	UID64_t		nUniqueNumber;			// 아이템 고유번호
	INT			nAmount;				// 아이템 갯수
	UINT		nBuildingIndex;			// 건물 인덱스
	char		szIconName[20];			// 아이템 ID
	char		szName[50];				// 아이템 이름
	UINT		uSellingPrice;			// 판매 가격 
	INT			SourceIndex;			// 이펙트, 아이콘(빅/스몰) 리소스 데이타
	BYTE		bySelectType;
	POINT		ptIcon;
} stMultiSelectItem;

// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
// 게임 시작 시 구조체
typedef struct 
{
	AVECTOR3		vPos;
	BitFlag16_t		MapWeather;
} stGameStartInfo;

// 2007-12-21 by bhsohn 프로그램 다중 락 구조 생성
// 락관련 모드
#define		MULTI_LOCK_ALL_MATCHING		0		//	모두 매칭 되야지만 풀린다.
#define		MULTI_LOCK_ONE_MATCHING		1		//	하나만 매칭되도 풀린다.

// 게임 시작하기 위한 기본 정보
typedef struct 
{
	MessageType_t			nLockMsg;
	vector<MessageType_t>	vecUnLockMsg;
	int						nLockType;
	BOOL					bFiledServer;
} stMultiLockInfo;
// end 2007-12-21 by bhsohn 프로그램 다중 락 구조 생성

// 2007-11-22 by bhsohn 아레나 통합서버
// 게임 스타트 완료시 처리 되는 구조체
typedef struct 
{
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];			// 2007-04-09 by cmkwon
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];		// 2007-04-09 by cmkwon
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];	// 2007-04-09 by cmkwon
	
	MAP_CHANNEL_INDEX	MapChannelIndex;		// 캐릭터가 속한 맵 및 채널
	BYTE				CharacterMode0;				// 
	
	AVECTOR3			PositionVector;
	BitFlag16_t			MapWeather;	
	BOOL				bMemberPCBang;			// 가맹 PC방 플래그
}structGameStartOkInfo;
// end 2007-11-22 by bhsohn 아레나 통합서버

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
struct structLoadingGameData
{
	structLoadingGameData()
	{
		LoadingPriority = _NOTHING_PRIORITY;
	}
	CGameData*		pGameData;
	int				MeshIndex;
	int				MeshType;
	int				Step;					// 로딩 스텝(1, 6)
	int				Text_Cnt;				// 텍스쳐 로딩 진행 수
	int				LoadingPriority;
	CSkinnedMesh*	pSkinnedMesh;
};
struct structLoadingGameInfo
{
	structLoadingGameInfo()
	{
		LoadingPriority = _NOTHING_PRIORITY;
	}	
	char			MeshName[16];
	int				MeshType;
	CSkinnedMesh*	pSkinnedMesh;
	int				LoadingPriority;
};
// 2011. 1. 18 by jskim 배트남 VTC가드
typedef bool (_stdcall *func1) ();
typedef bool (_stdcall *func2) ();
typedef bool (_stdcall *func3) (char*);
typedef void (_stdcall *func4) ();
// end 2011. 1. 18 by jskim 배트남 VTC가드

// 2007-07-10 by bhsohn 통신 타입 에러에 대한 처리
#define SIZE_MAX_ERROR_CHAT_MESSAGE			512

// 2008-11-13 by bhsohn 조이스틱 작업
#define MAX_JOYCTL_LIST			10			// MAX_JOY_LIST와 동일한 값이어야한다.

// 2009-01-22 by bhsohn Xign Code 사용하는 나라만 모듈 추가
//////////////////////// 게임 가드 설정////////////////////////
//
#define		USE_GAMEGUARD_NONE					0		//게임가드 사용안함
#define		USE_GAMEGUARD_AHNLAB_HACKSHIELD		1		//핵쉴드 게임가드
#define		USE_GAMEGUARD_XIGNCODE				2		//Xign 게임가드
#define		USE_GAMEGUARD_NPROTECTOR			3		// 2009-01-28 by bhsohn nProtector 게임 가드 추가				
// 2009. 06. 17 by ckPark Frost System
#define		USE_GAMEGUARD_FROST					4
// end 2009. 06. 17 by ckPark Frost System
	// 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
#define		USE_GAMEGUARD_XTRAP					5
	//end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)

// 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
#define		USE_GAMEGUARD_APEX					6
// end 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
// 2011. 1. 18 by jskim 배트남 VTC가드
#define		USE_GAMEGUARD_VTC					7
// end 2011. 1. 18 by jskim 배트남 VTC가드


// 2013-07-12 by bhsohn China Work
#if defined(_DEBUG) || defined(TEST140)|| defined(LANGUAGE_CHINA)// 2012-10-22 by bhsohn 게임가드 디파인 수정
// 게임가드 사용안함
#define		GAMEGUARD_TYPE 					USE_GAMEGUARD_NONE
#define		NO_GAMEGUARD

	// 2011. 01. 18 by jsKim 140번 핵실드 제거 
#elif defined(LANGUAGE_VIETNAM)
// 2011. 1. 18 by jskim 배트남 VTC가드
// 	#define		GAMEGUARD_TYPE 					USE_GAMEGUARD_XTRAP
// 	#define		DEFINE_USE_GAMEGUARD_XTRAP
// 
// 	#pragma comment(lib, "XTrap4Client_st")
// 	#pragma comment(lib, "XTrap4Client_ClientPart_st")
// 	#pragma comment(lib, "XTrap4Client_ServerPart_st")
// 	#pragma comment(lib, "XTrap_Unipher_st")
// 	#include "Xtrap_C_Interface.h"
	//end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
 	#define		GAMEGUARD_TYPE 					USE_GAMEGUARD_VTC
	#define		DEFINE_USE_GAMEGUARD_VTC
// end 2011. 1. 18 by jskim 배트남 VTC가드

	// 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
#elif defined(ETERNAL_SKIES)
	#define				GAMEGUARD_TYPE					USE_GAMEGUARD_NONE		
#else 
	#define				GAMEGUARD_TYPE					USE_GAMEGUARD_AHNLAB_HACKSHIELD		
// END 2012-10-22 by bhsohn 게임가드 디파인 수정
#endif

// end 2009. 06. 17 by ckPark Frost System


class CAtumApplication : public CD3DApplication  
{
	friend class CShuttleChild;
public:
	CAtumApplication();
	virtual ~CAtumApplication();

	HRESULT InitDShowTextureRenderer();
	HRESULT MovieRender();
	
	HRESULT CheckMovieStatus(void);
	void CleanupDShow(void);
	
	
	HRESULT AddToROT(IUnknown *pUnkGraph); 
    void RemoveFromROT(void);

	D3DSURFACE_DESC & GetBackBufferDesc() { return m_d3dsdBackBuffer; }
	FLOAT 	GetFPS();
	FLOAT	GetElapsedTime() { return m_fElapsedTime; }
	HWND	GetHwnd() { return m_hWnd; }
	D3DCAPS9 & GetDeviceCaps() { return m_d3dCaps; }
	VOID	SendMovePacket(BOOL bActive);	// 2004-11-08 by jschoi
	HRESULT	InitStageEffect( int nIndex, float fPlayTime=5.0f, BOOL bEnableSkip=TRUE );
	DWORD	ResourceLoadThread();	// load thread function(CInitThread와 함께 사용한다.)
	DWORD	ResourceRestoreThread();
	VOID	RenderGame();
	VOID	CleanText();
	VOID	CheckError(DWORD err);
	VOID	LoadPath( char* strPath, UINT nType, char * strFileName );
	VOID	CalcObjectSourceScreenCoords(D3DXVECTOR3 vObjPos,int iScreenWidth, int iScreenHeight,int &iCoordX, int &iCoordY, int &iCoordW);
	VOID	CheckRenderState();
	VOID	SetCamPosInit();
	FLOAT	Distance3DTo2D(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2);
	VOID	ChangeEffectTexture(CAtumNode * pNode,char * str);
	// 2008-01-18 by bhsohn 중국 클라이언트 죽는 문제 해결
	//VOID	ChangeGameState(DWORD dwType);
	VOID	ChangeGameState(DWORD dwType, BOOL bUseThread=FALSE);
	void	OnScreenShotCreated(char* screenShotName);
	void	DeleteEffectList();
	void	InitSpeedHackTimer();
	BOOL	IsWindowMode() { return m_bWindowed; }
	// 2007-07-20 by bhsohn 브리핑룸 수정
	DWORD	GetGameState();
	
    HRESULT OneTimeSceneInit();
    HRESULT InitDeviceObjects();
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();
    HRESULT Render();
    HRESULT FrameMove();
	HRESULT InActiveFrameMove();										// 2006-07-06 by ispark
    HRESULT FinalCleanup();
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HRESULT ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format );
	VOID	NetworkErrorMsgBox( char* strMsg);

	void	RenderCity();
	// remodeling, 2004-03-17 by dhkwon
	// SendFieldSocket~
	// 2007-11-22 by bhsohn 아레나 통합서버
	//VOID	SendFieldSocketConnectLogin();
	VOID	SendFieldSocketConnectLogin(CFieldWinSocket* pFieldSocket);
	VOID	SendFieldSocketChangeCharacterBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet);//FieldSocketCharacterChangeCurrentHPDPSPEP,FieldSocketCharacterChangeCurrentHP
	VOID	SendFieldSocketCityGetBuildingList( MAP_CHANNEL_INDEX mapChannelIndex );
	VOID	SendFieldSocketGetShopWarpTargetMapList( MAP_CHANNEL_INDEX mapChannelIndex );	
	void	SendArenaLoginOk();
	void	SendAreneEnterRoomWarp();
	void	SendFCArenaCharacterGameStart();					// 메인서버에서 아레나로 로드시
	void	SendFCCharacterGameStartFromArenaToMainServer();	// 아레나 리소스 로드완료 
	int		NextStepArenaState();		// 아레나 상태값 업데이트 
	void	ConevertArenaRenderUserName(int nArenaState, char* pSrcDst);
	CHARACTER* GetMFSMyShuttleInfo();	//MainServer 정보를 얻어온다.
	void	SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo);//MainServer 정보를 갱신한다. 

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	void	DelMultiItemList(BOOL bInitCtlBtn=FALSE);	
	BOOL	CheckMultItemSel(UID64_t	 nUniqueNumber);	
	BOOL	CheckCityStoreMultItemSel(UID64_t	 nUniqueNumber); // 창고에서 아이템 선택 여부 판다.ㄴ
	int		GetMultiSelectItem();
	int		GetCityStoreMultiSelectItem();
	void	GetMultiItemSPI(int nShopId, char* pDstSpi);

	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	deque<stMultiSelectItem>* GetMultiSelVector(BYTE byType);

	// by bhsohn 2007-03-12 China Working
	void DxSetDialogBoxMode(BOOL bEnable);

	// 2007-04-05 by bhsohn 맵로드시, 체크섬 추가
	MAP_CHANNEL_INDEX GetMyShuttleMapChannelIndex();

	// 2007.04.24 by bhsohn China IME Working
	void	UpdateReadingString(char* pText, int nTextLen);
	void	UpdateIMEType(char* pText, int nTextLen);

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	BOOL	IsMyShuttleCharacter();
	// 2007-05-21 by bhsohn China IME Working
	void	RenderIMEType(int x, int y);

	// 2007-07-11 by bhsohn omi 체크섬 추가
	void	SendOMICheckSum();

	// 2010. 03. 12 by ckPark Interface.tex 체크섬 추가
	void	SendInterfaceCheckSum( void );
	// end 2010. 03. 12 by ckPark Interface.tex 체크섬 추가

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	// 2009. 01. 22 by ckPark 각 세력별 오퍼레이터 MP3 분리
	//void	StartNarrationSound(char* pNarrationSound);
	void	StartNarrationSound(char* pNarrationSound, char* pErNarrationSound=NULL);
	void	EndNarrationSound();	
	void	SetFirstUserIntoFreeSka(BOOL bTake);
	BOOL	GetFirstUserIntoFreeSka();	
	
	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	BOOL	SendMeshObjectCheckSum(int nMeshIndex);

	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	void	ReLoadEnemyRenderList();		

	// 2007-09-06 by bhsohn 프리미엄 유저는 총알수 2배
	BOOL	IsPrimiumUser();

	// 2007-09-05 by bhsohn 전진 기지전
	void	EnableChatMode(BOOL bChat, BOOL bUpdatIMEDevice);	
	BOOL	IsMyShuttleLeader();
	BOOL	IsMyShuttleGuildMaster();
	BOOL	IsLeaderOutPost();

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	BYTE	IsUseSecondaryPasswordSystem();
	BYTE	IsSetSecondaryPassword();	
	void	EnterLock(BOOL bUnLock);
	BOOL	GetEnterLock();
	void	SendDeleteCharcter(char* pEncordPassWord);

	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	BYTE	IsSecondPassLock();
	void	SetSecondPassLock(BYTE bySecondPassLock);

	// 2007-12-21 by bhsohn 프로그램 다중 락 구조 생성
	void	EnterMultiLock(BOOL bFiledServer, MessageType_t nLockMsg, vector<MessageType_t> vecUnLockMsg, int nLockType);
	void	CheckMultiLock(BOOL bFiledServer, MessageType_t msgUnLockMsg);
	// 2007-11-22 by bhsohn 아레나 통합서버

	// 해당 메시지 Lock중 인지 확인.
	bool	IsEnableMultiLock ( MessageType_t nLockMsg );

	void	OnClickArenaShop();
	void	RequestArenaTeamList();
	void	SetArenaState(int nState);
	int		GetArenaState();
	void	ArenaFinishWarp(BOOL bForceCloseArenaSocket);	// 아레나 종료 창에 대한 확인 
	void	ArenaGameStarOk(structGameStartOkInfo *pMsg, BOOL bGotoArena);
	void	CloseArenaSocket();
	CHARACTER*	GetArenaCharacterInfo();
	void	StartGmConnectAenenaServer();		// GM이 아레나 서버로 연결 
	void	GmConnectAenenaIMServer();			// GM이 아레나 IM서버에 연결 
	void	SetGmModeConnect(BOOL bGmModeConnect);
	BOOL	IsGmModeConnect();

	// 2008-03-03 by bhsohn 통합 아레나 IM서버 연결과정 수정
	void	UserConnectAenenaIMServer();


	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void	OnClickInfinityField( void );
	void	UserConnectInfinityIMServer( void );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2008-02-15 by bhsohn 네트워크 속도 체크
	VOID	StartNetCheck(char* pCmd);
	VOID	SendCheckNetState();

	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	BOOL	IsDoingInfluenceWar();

	// 2008-03-17 by bhsohn 윈도우 이동시 Tick안들어오는 문제 처리
	BOOL	IsPossibleWindowMove();

	// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
	BOOL	IsPossibleShopOpen();

	// 2008-04-07 by bhsohn 시스템키 걸러내는 시스템 추가
	BOOL	IsSysKeyDownPermisiion(WPARAM wParam, LPARAM lParam);

	// 2008-06-16 by bhsohn 태국 게임 종료후 웹페이지 뜨우게 함
	BOOL	ExcuteExplore();

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	void	InitOptionEtcInfo(sOPTION_ETC* pEtc);
	BOOL	IsOptionEtc(int nOptionIdx);
	stcuctRateRect GetOptionInterfacePOS();
	void	SetOptionInterfacePOS(stcuctRateRect* pRect);


	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장

	stcuctRateRect GetChatInterfacePOS(void);
	void	SetChatInterfacePOS(stcuctRateRect* pRect);

	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장


	int		GetFriendListOnCount();

	// 2008-08-11 by bhsohn 단독으로 실행 되게끔
	BOOL	IsSingletonMode();
	void	SingPlayGameStartOk(MSG_FC_CHARACTER_GAMESTART_OK * pMsg);

	// 2008-09-17 by bhsohn 게임 실행파일명 체크섬 추가
	void	SendAtmCheckSum();

	// 2008-11-11 by bhsohn 퀘스트 CheckSum추가
	void	SendQuestCheckSum();

	// 2008-11-13 by bhsohn 조이스틱 작업
	// 컨트롤 조이스틱
	CJoyStickInput *GetJoystickControl();
	BOOL GetAsyncKeyState_DIK_DIJ(int code);	
	// 조이스틱 컨트롤 갯수
	int GetJoystickCtrlDeviceListLen();	
	char* GetJoystickCtrlDeviceList(int i_nIdx);

	// 진동패드
	CJoyStickFreedback *GetJoyStickFeedback();
	void SetJoyStickFeedback(BOOL bSetFeedBack, int nPowerIndex);

	// 조이스틱 사용유무
	BOOL IsUseJoyStick();
	BOOL IsUseJoyFeedBack();	// 진동 사용 유무
	void SetUseJoyStick(BOOL bUse);
	void SetUseJoyFeedBack(BOOL bUse);

	// 조이스틱 옵션정보
	void InitJoyStickOptionInfo(structJoyStickOptionInfo* pJoysticOp);

	// 조이스틱 장치 초기화
	int InitJoyStickDevice(BOOL bJoyControl, BOOL bJoyFeed, char* o_pDevice);
	// end 2008-11-13 by bhsohn 조이스틱 작업

	// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
	void CreateMyAppEffectData(int i_nType, D3DXVECTOR3	i_vPos, D3DXVECTOR3	i_vVel, D3DXVECTOR3	i_vUp); // 내 위치에 이펙트 생성
	void SendCharcterShowMapEffect(MSG_FC_CHARACTER_SHOW_MAP_EFFECT* i_pMsg);
	D3DXVECTOR3 GetMyShuttleCharacterPos();
	D3DXVECTOR3 GetViewDirVel();
	// end 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가

	// 2009-01-28 by bhsohn nProtector 게임 가드 추가
	BOOL InitGameGuardNProtector();
	BOOL SetGameGuardHWND();
	BOOL TickGetCheckGameGuardNProtector();		
	void	HS_SendHackShieldErrorMsg(long lHackClientCode, char * pMsg = NULL);
	// end 2009-01-28 by bhsohn nProtector 게임 가드 추가
	// 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
	void TickGetCheckGameGuardXTrap();
	//end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	CWorldRankManager*	GetWorldRankManager();
	void RqAllWorldRankInfo();
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가
		// 2012-11-28 by jhjang 게임포지 웹 상점 통합 추가 작업
	// 2012-11-23 by jhjang 게임포지 웹 상점 통합 작업
	// void PopupWebWindow(BOOL i_bPopup);
	// void PopupWebWindow(BOOL i_bPopup,char* strWebSite = NULL);
	void PopupWebWindow(BOOL i_bPopup,char* strWebSite = NULL,char* strWebParameter = NULL,int nWebPosX = -1,int nWebPosY = -1,int nWebWidth = -1,int nWebHeight = -1);
	// end 2012-11-23 by jhjang 게임포지 웹 상점 통합 작업
	// end 2012-11-28 by jhjang 게임포지 웹 상점 통합 추가 작업
	// end 2009-02-24 by bhsohn 캐쉬샵 충전 웹페이지 추가

	// 2009-02-26 by bhsohn Japan IME
	VOID CleanIMEControl();
	// end 2009-02-26 by bhsohn Japan IME

	// 2009-03-10 by bhsohn nProtector CS모듈 추가
	void SendNpGameMonCheckCSAuth2(PVOID dwArg);
	// end 2009-03-10 by bhsohn nProtector CS모듈 추가

	// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	void AddMultiSelWarningMsg(UID64_t	UniqueNumber);
	// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	CInfinityManager*	GetInfinityManager( void );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

   	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	void LoadMeshPorcess();
	BOOL ObjectLoadingTimeChack();	
	BOOL IsEmptyLoadingGameDataList(void);
	void LoadingGameDataSort();
	structLoadingGameData* vecFrontGameData();
	void DeleteIsLoadingGameData(structLoadingGameData* nGameData);
	void vecPushBackGameData(structLoadingGameData* nGameData);
	void vecPushGameData(structLoadingGameData* i_pGameData);
	BOOL DeleteLoadingGameData(CSkinnedMesh *i_pSkinnedMesh);
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	VOID ShotDownGameTime(); // 2012-07-17 by jhahn, 셧다운 시간 표시
	
	void FieldSocketCharacterPCBangPremiumApply(MSG_FC_CHARACTER_PCBANG_PREMIUN_APPLY* pMsg);	// 2012-10-16 by bhsohn NPlay PC방 모듈
	void FieldSocketCharacterXignCodeAckPacket(MSG_FC_CHARACTER_XIGNCODE_ACK_PACKET* pMsg);	// 2012-10-17 by bhsohn XignCode작업
	
	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	void FieldSocketWarInflConseVictites(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES* pMsg);
	void FieldSocketShopInflBuffOk(MSG_FC_SHOP_INFLUENCE_BUFF_OK* pMsg);
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	void SetCharacterChangeCurrentHPAndDamageBodyCondition(float fHP){CharacterChangeCurrentHPAndDamageBodyCondition(fHP);}			// 2013-06-25 by ssjung 데미지 이펙트가 안나오는 현상 수정
	
protected:
	// MsgProc~
	int MsgProcGame( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int MsgProcCity( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int MsgProcCreate( UINT uMsg, WPARAM wParam, LPARAM lParam );
	// OnRecvFieldSocket~
	// 2007-11-22 by bhsohn 아레나 통합서버
	//int		OnRecvFieldSocketMessage( DWORD dwMsgType );
	int		OnRecvFieldSocketMessage( DWORD wParam, UINT nSocketNotifyType);
	// 2007-11-22 by bhsohn 아레나 통합서버
	//VOID	OnRecvFieldSocketWS_CONNECTED( USHORT nConnect );
	VOID	OnRecvFieldSocketWS_CONNECTED( USHORT nConnect, CFieldWinSocket* pFieldSocket);
	VOID	OnRecvFieldSocketWS_CLOSED(void);
	VOID	OnRecvCopyData(WPARAM wParam, COPYDATASTRUCT* pCopyDataStruct);
	// FieldSocket~
	VOID	FieldSocketConnectLoginOk(MSG_FC_CONNECT_LOGIN_OK * pMsg);
	VOID	FieldSocketCharacterGetCharacterOk(MSG_FC_CHARACTER_GET_CHARACTER_OK * pMsg);
	VOID	FieldSocketCharacterCreateOk(MSG_FC_CHARACTER_CREATE_OK* pMsg);
	VOID	FieldSocketCharacterDeleteOk(void);
	VOID	FieldSocketCharacterGetOtherInfoOk(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	VOID	FieldSocketCharacterGetMonsterInfoOk(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg);
	VOID	FieldSocketCharacterGameStartOk(MSG_FC_CHARACTER_GAMESTART_OK * pMsg);
	VOID	FieldSocketCharacterConnectGameStartOk(MSG_FC_CHARACTER_CONNECT_GAMESTART_OK * pMsg);
	// 2008-03-10 by bhsohn 아레나에서 귀환 포탈 처리
	//VOID	FieldSocketCharacterGameEndOk(MSG_FC_CHARACTER_GAMEEND_OK * pMsg);
	VOID	FieldSocketCharacterGameEndOk(UINT nSocketNotifyType, MSG_FC_CHARACTER_GAMEEND_OK * pMsg);
	VOID	FieldSocketMoveOk( MSG_FC_MOVE_OK* pMsg);
	VOID	FieldSocketMonsterMoveOk(MSG_FC_MONSTER_MOVE_OK* pMsg);
	VOID	FieldSocketCharacterChangeHPDPSPEP(MSG_FC_CHARACTER_CHANGE_HPDPSPEP * pMsg);
	VOID	FieldSocketCharacterChangeCurrentHPDPSPEP(MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP * pMsg);
	VOID	FieldSocketCharacterChangeCurrentHP(MSG_FC_CHARACTER_CHANGE_CURRENTHP * pMsg);
	VOID	FieldSocketCharacterChangeCurrentDP(MSG_FC_CHARACTER_CHANGE_CURRENTDP * pMsg);
	VOID	FieldSocketCharacterChangeCurrentSP(MSG_FC_CHARACTER_CHANGE_CURRENTSP * pMsg);
	VOID	FieldSocketCharacterChangeCurrentEP(MSG_FC_CHARACTER_CHANGE_CURRENTEP * pMsg);
	VOID	FieldSocketCharacterChangeBodycondition(MSG_FC_CHARACTER_CHANGE_BODYCONDITION * pMsg);
	VOID	FieldSocketCharacterChangeUnitkind(MSG_FC_CHARACTER_CHANGE_UNITKIND * pMsg);
	VOID	FieldSocketCharacterPayWarPoint(MSG_FC_CHARACTER_PAY_WARPOINT* pMsg);
	VOID	FieldSocketCharacterUpdateSubleaderOk(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK * pMsg);
	VOID	FieldSocketPartyCreateOk(MSG_FC_PARTY_CREATE_OK* pMsg);
	VOID	FieldSocketPartyRequestInviteQuestion(MSG_FC_PARTY_REQUEST_INVITE_QUESTION* pMsg);
	VOID	FieldSocketPartyRequestInviteQuestion_THE_ARENA(MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA* pMsg);	/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 파티 가입 준비
	VOID	FieldSocketPartyAutoCreated_THE_ARENA(MSG_FC_PARTY_AUTO_CREATED_THE_ARENA* pMsg);	/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 파티를 만들라는 패킷.
	VOID	FieldSocketPartyRejectInviteOk(MSG_FC_PARTY_REJECT_INVITE_OK* pMsg);
	VOID	FieldSocketPartyPutMember(MSG_FC_PARTY_PUT_MEMBER* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoAll(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoLevel(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoCurrentHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoCurrentDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoCurrentSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoCurrentEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP* pMsg);
	VOID	FieldSocketPartyUpdateMemberInfoBodyCondition(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION* pMsg);
	VOID	FieldSocketCityCheckWarpStateOk(MSG_FC_CITY_CHECK_WARP_STATE_OK* pMsg);
	VOID	FieldSocketEventWarpOtherFieldServer(MSG_FC_EVENT_WARP * pMsg);
	VOID	FieldSocketEventWarpSameMap(MSG_FC_EVENT_WARP_SAME_MAP * pMsg);
	VOID	FieldSocketEventWarpSameFieldServer(MSG_FC_EVENT_WARP_SAME_FIELD_SERVER * pMsg);
	VOID	FieldSocketEventOtherWarped(MSG_FC_EVENT_OTHER_WARPED * pMsg);
	VOID	FieldSocketEventWarpConnectOk(MSG_FC_EVENT_WARP_CONNECT_OK* pMsg);
	VOID	FieldSocketStorePutItemHeader(MSG_FC_STORE_PUT_ITEM_HEADER* pMsg);
	VOID	FieldSocketStorePutItem(MSG_FC_STORE_PUT_ITEM* pMsg);
	VOID	FieldSocketStorePutItemDone(MSG_FC_STORE_PUT_ITEM_DONE* pMsg);
	VOID	FieldSocketCharacterChangeLevel(MSG_FC_CHARACTER_CHANGE_LEVEL* pMsg);
	VOID	FieldSocketCharacterChangeExp(MSG_FC_CHARACTER_CHANGE_EXP* pMsg);
	VOID	FieldSocketCharacterChangeStat(MSG_FC_CHARACTER_CHANGE_STAT * pMsg);
	VOID	FieldSocketCharacterChangeTotalGearStat(MSG_FC_CHARACTER_CHANGE_TOTALGEAR_STAT * pMsg);	
	VOID	FieldSocketCharacterChangeStatus(MSG_FC_CHARACTER_CHANGE_STATUS* pMsg);
	VOID	FieldSocketItemChangeWindowPositionOk(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK * pMsg);
	VOID	FieldSocketMoveLandingOk(MSG_FC_MOVE_LANDING_OK* pMsg);
	VOID	FieldSocketMoveTakeOffOk(MSG_FC_MOVE_TAKEOFF_OK* pMsg);
	VOID	FieldSocketItemShowItem(MSG_FC_ITEM_SHOW_ITEM* pMsg);
	VOID	FieldSocketBattleDropMineOk(MSG_FC_BATTLE_DROP_MINE_OK* pMsg);
	VOID	FieldSocketItemGetItemOk(MSG_FC_ITEM_GET_ITEM_OK* pMsg);
	VOID	FieldSocketItemHideItem(MSG_FC_ITEM_HIDE_ITEM* pMsg);
	VOID	FieldSocketPartyPutItemOther(MSG_FC_PARTY_PUT_ITEM_OTHER* pMsg);
	VOID	FieldSocketItemGetItemFail(MSG_FC_ITEM_GET_ITEM_FAIL* pMsg);
	VOID	FieldSocketMissileMoveOk(MSG_FC_MISSILE_MOVE_OK* pMsg);
	VOID	FieldSocketCityRequestEnterBuildingOk(MSG_FC_CITY_REQUEST_ENTER_BUILDING_OK* pMsg);
	VOID	FieldSocketEventEnterBuilding(MSG_FC_EVENT_ENTER_BUILDING* pMsg);
	VOID	FieldSocketEventLeaveBuildingOk(MSG_FC_EVENT_LEAVE_BUILDING_OK* pMsg);
	VOID	FieldSocketShopCheckGiveTargetOK(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK* pMsg);
	VOID	FieldSocketShopBuyCashItemOK(MSG_FC_SHOP_BUY_CASH_ITEM_OK* pMsg);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    VOID	FieldSocketShopBuyItemUpdate(MSG_FC_SHOP_BUY_ITEM_UPDATE* pMsg);
#endif
	
	VOID	FieldSocketShopPutItemHeader(MSG_FC_SHOP_PUT_ITEM_HEADER* pMsg);
	VOID	FieldSocketShopPutItem(MSG_FC_SHOP_PUT_ITEM* pMsg);
	VOID	FieldSocketShopPutItemDone(MSG_FC_SHOP_PUT_ITEM_DONE* pMsg);
	VOID	FieldSocketShopGetItemInfoOk(MSG_FC_SHOP_GET_ITEMINFO_OK* pMsg);
	VOID	FieldSocketShopBuyItemOk(MSG_FC_SHOP_BUY_ITEM_OK* pMsg);
	VOID	FieldSocketShopSellItemOk(MSG_FC_SHOP_SELL_ITEM_OK* pMsg);
	VOID	FieldSocketShopGetUsedItemPriceOk(MSG_FC_SHOP_GET_USED_ITEM_PRICE_OK* pMsg);
	VOID	FieldSocketItemThrowAwayItemOk(MSG_FC_ITEM_THROW_AWAY_ITEM_OK* pMsg);
	
	VOID	FieldSocketItemUseEnergyOk(MSG_FC_ITEM_USE_ENERGY_OK* pMsg);
	VOID	FieldSocketItemUseItemOk(MSG_FC_ITEM_USE_ITEM_OK* pMsg);
	VOID	FieldSocketItemUseCardItemMonstersummonNotify(MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY* pMsg);
	VOID	FieldSocketItemUseCardItemChangecharacternameNotify(MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY* pMsg);
	VOID	FieldSocketItemUseCardItemSkillinitializeNotify(MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY* pMsg);
	VOID	FieldSocketItemUseCardItemChangepilotfaceNotify(MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY* pMsg);
	VOID	FieldSocketItemUseCarditemGuildNotify(MSG_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY* pMsg);	
	VOID	FieldSocketItemUseCarditemDeleteAllEnchant(MSG_FC_ITEM_DELETE_ALL_ENCHANT* pMsg);

	VOID	FieldSocketChatRegion(MSG_FC_CHAT_REGION* pMsg);
	VOID    FieldSocketChatCashAll(MSG_FC_CHAT_CASH_ALL* pMsg);
	VOID	FieldSocketChatMap(MSG_FC_CHAT_MAP* pMsg);
	VOID	FieldSocketChatArena(MSG_FC_CHAT_ARENA* pMsg);
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	VOID	FieldSocketChatInfinity( MSG_FC_CHAT_INFINITY* pMsg );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	VOID	FieldSocketTradeRequestTradeOk(MSG_FC_TRADE_REQUEST_TRADE_OK* pMsg);
	VOID	FieldSocketTradeAcceptTradeOk(MSG_FC_TRADE_ACCEPT_TRADE_OK* pMsg);
	VOID	FieldSocketTradeRejectTradeOk(MSG_FC_TRADE_REJECT_TRADE_OK* pMsg);
	VOID	FieldSocketTradeRejectTrading(MSG_FC_TRADE_REJECT_TRADING* pMsg);
	VOID	FieldSocketTradeShowTradeWindow(MSG_FC_TRADE_SHOW_TRADE_WINDOW* pMsg);
	VOID	FieldSocketTradeOkTradeOk(MSG_FC_TRADE_OK_TRADE_OK* pMsg);
	VOID	FieldSocketTradeCancelTradeOk(MSG_FC_TRADE_CANCEL_TRADE_OK* pMsg);
	VOID	FieldSocketTradeDeleteItem(MSG_FC_TRADE_DELETE_ITEM* pMsg);
public:
	VOID	FieldSocketTradeUpdateItemCount(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	VOID	FieldSocketTradeInsertItem(MSG_FC_TRADE_INSERT_ITEM* pMsg);
protected:
	VOID	FieldSocketTradeTransItemOk(MSG_FC_TRADE_TRANS_ITEM_OK* pMsg);
	VOID	FieldSocketStoreUpdateEndurance(MSG_FC_STORE_UPDATE_ENDURANCE* pMsg);
	VOID	FieldSocketTradeSeeItemOk(MSG_FC_TRADE_SEE_ITEM_OK* pMsg);
	// 2010-06-15 by shcho&hslee 펫시스템 - 거래 시 펫 정보 전송
	VOID	FieldSocketTradeSeePetData(MSG_TRADE_PET_DATA* pMsg);
	VOID	FieldSocketTradeDeletePetData(MSG_TRADE_PET_DATA* pMsg); // 거래시 펫 정보 삭제 처리
	VOID	FieldSocketTradeInsertPetData(MSG_FC_ITEM_PET_BASEDATA* pMsg); // 거래시 펫 정보 추가 처리
	// end 2010-06-15 by shcho&hslee 펫시스템 - 거래 시 펫 정보 전송
	VOID	FieldSocketCitywarGetOccupyinfoOk(MSG_FC_CITYWAR_GET_OCCUPYINFO_OK* pMsg);
	VOID	FieldSocketCitywarBringSumoftexOk(MSG_FC_CITYWAR_BRING_SUMOFTEX_OK* pMsg);
	VOID	FieldSocketCitywarStartWar(MSG_FC_CITYWAR_START_WAR* pMsg);
	VOID	FieldSocketCitywarEndWar(MSG_FC_CITYWAR_END_WAR* pMsg);
	VOID	FieldSocketCharacterGetCashMoneyCountOk(MSG_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK* pMsg);
	VOID	FieldSocketCharacterChangeCharacterModeOk(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK* pMsg);
	VOID	FieldSocketSkillUseSkill(MSG_FC_SKILL_USE_SKILL_OK *pMsg);
	VOID	FieldSocketInvenItempHidePetData(MSG_FC_ITEM_NOTIFY_WINDOW_POSITION* pMsg);	   //2011-10-06 by jhahn 파트너 성장형 시스템
	VOID	FieldSocketSkillCancelSkillOK(MSG_FC_SKILL_CANCEL_SKILL_OK * pMsg);
	VOID	FieldSocketInvalidateSkill(MSG_FC_SKILL_INVALIDATE_SKILL *pMsg);
	VOID	FieldSocketCharacterGetAccountUniqueNumberOk(MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK* pMsg);
	VOID	FieldSocketBattleRequestPkOk(MSG_FC_BATTLE_REQUEST_PK_OK* pMsg);
	VOID	FieldSocketBattleCancelPK(void);
	VOID	FieldSocketBattleDropBundleOk(MSG_FC_BATTLE_DROP_BUNDLE_OK* pMsg);
	VOID	FieldSocketCharacterDeleteOtherInfo(MSG_FC_CHARACTER_DELETE_OTHER_INFO* pMsg);
	VOID	FieldSocketCharacterMonsterCreated(MSG_FC_MONSTER_CREATED* pMsg);
	VOID	FieldSocketMonsterChangeBodycondition(MSG_FC_MONSTER_CHANGE_BODYCONDITION* pMsg);
	VOID	FieldSocketCharacterChangeMonsterHP(MSG_FC_MONSTER_CHANGE_HP* pMsg);
	VOID	FieldSocketCharacterShowEffectOk(MSG_FC_CHARACTER_SHOW_EFFECT_OK* pMsg);
	VOID	FieldSocketBattleRequestP2PPkOk(MSG_FC_BATTLE_REQUEST_P2P_PK_OK* pMsg);
	VOID	FieldSocketBattleAcceptRequestP2PPkOk(MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK* pMsg);
	VOID	FieldSocketBattleRejectRequestP2PPkOk(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK* pMsg);
	VOID	FieldSocketBattleSurrenderP2PPkOk(MSG_FC_BATTLE_SURRENDER_P2P_PK_OK* pMsg);
	VOID	FieldSocketCharacterChangeCharacterMent(MSG_FC_CHARACTER_CHANGE_CHARACTER_MENT* pMsg);
	VOID	FieldSocketBattleRejectSurrenderP2PPkOk(MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK* pMsg);
	VOID	FieldSocketBattleEndP2PPk(MSG_FC_BATTLE_END_P2P_PK* pMsg);
	VOID	FieldSocketCharacterChangePKPoint(MSG_FC_CHARACTER_CHANGE_PKPOINT* pMsg);
	VOID	FieldSocketBattleDropDummyOk(MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg);
	VOID	FieldSocketBattleDeleteDummyOK(MSG_FC_BATTLE_DELETE_DUMMY_OK* pMsg);
	VOID	FieldSocketBattleExplodeDummyOK(MSG_FC_BATTLE_EXPLODE_DUMMY_OK* pMsg);
	VOID	FieldSocketBattleAttackHideItem(MSG_FC_BATTLE_ATTACK_HIDE_ITEM* pMsg);
	VOID	FieldSocketBattleAttackExplodeItem(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM* pMsg);
	VOID	FieldSocketMonsterHide(MSG_FC_MONSTER_HIDE* pMsg);
	VOID	FieldSocketMonsterShow(MSG_FC_MONSTER_SHOW* pMsg);
	VOID	FieldSocketBattleDropFixerOk(MSG_FC_BATTLE_DROP_FIXER_OK* pMsg);
	VOID	FieldSocketInfoGetItemInfoOk(ITEM & item);
	VOID	FieldSocketInfoGetRareItemInfoOk(RARE_ITEM_INFO & rareItem);
	VOID	FieldSocketInfoGetMonsterInfoOk(MSG_FC_INFO_GET_MONSTER_INFO_OK *mon);
	VOID	FieldSocketInfoGetMapObjectInfoOk(MAPOBJECTINFO & obj);
	VOID	FieldSocketBattleAttackHideItemWKind(MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND* pMsg);
	VOID	FieldSocketBattleAttackExplodeItemWKind(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND* pMsg);
	VOID	FieldSocketConnectSyncTime(MSG_FC_CONNECT_SYNC_TIME* pMsg);
	VOID	FieldSocketItemReloadItemInfo(void);
	VOID	FieldSocketItemUseEnchantOk(MSG_FC_ITEM_USE_ENCHANT_OK* pMsg);
	VOID	FieldSocketItemPutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg);
	VOID	FieldSocketQuestRequestStartResult(MSG_FC_QUEST_REQUEST_START_RESULT* pMsg);
	VOID	FieldSocketQuestRequestSuccessResult(MSG_FC_QUEST_REQUEST_SUCCESS_RESULT* pMsg);
	VOID	FieldSocketQuestRequestSuccessCheckResult(MSG_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT* pMsg);
	VOID	FieldSocketQuestPutAllQuest(MSG_FC_QUEST_PUT_ALL_QUEST* pMsg);
	VOID	FieldSocketSkillSetupSkillOk(MSG_FC_SKILL_SETUP_SKILL_OK* pMsg);
	VOID	FieldSocketRequestRequestOk(MSG_FC_REQUEST_REQUEST_OK* pMsg);
	VOID	FieldSocketRequestRejectRequestOk(MSG_FC_REQUEST_REJECT_REQUEST_OK* pMsg);
	VOID	FieldSocketRequestCancelRequest(MSG_FC_REQUEST_CANCEL_REQUEST* pMsg);
	VOID	FieldSocketRequestAcceptRequestOk(MSG_FC_REQUEST_ACCEPT_REQUEST_OK* pMsg);
	VOID	FieldSocketPartyBattleStart(MSG_FC_PARTY_BATTLE_START* pMsg);
	VOID	FieldSocketPartyBattleEnd(MSG_FC_PARTY_BATTLE_END* pMsg);
	VOID	FieldSocketQuestUpdateMonsterCount(MSG_FC_QUEST_UPDATE_MONSTER_COUNT* pMsg);
	VOID	FieldSocketQuestPutAllQuestMonsterCount(MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT* pMsg);
	VOID	FieldSocketItemResetSkillEnchantToLoadedItem(void);
	VOID	FieldSocketBattleToggleShieldResult(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT* pMsg);
	VOID	FieldSocketBattleToggleDecoyOk(MSG_FC_BATTLE_TOGGLE_DECOY_OK* pMsg);
	VOID	FieldSocketBattlePriBulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	VOID	FieldSocketBattleSecBulletReloaded(MSG_FC_BATTLE_SEC_BULLET_RELOADED* pMsg);
	VOID	FieldSocketEventChangeWeather(MSG_FC_EVENT_CHANGE_WEATHER* pMsg);
	VOID	FieldSocketItemUpdateItemPos(MSG_FC_ITEM_UPDATE_ITEM_POS* pMsg);
	VOID	FieldSocketCharacterGetOtherRenderInfoOk(MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK* pMsg);
	VOID	FieldSocketInfoGetBuildingNPCInfoOk(MSG_FC_INFO_GET_BUILDINGNPC_INFO_OK* pMsg);
	VOID	FieldSocketEventSuggestChannels(MSG_FC_EVENT_SUGGEST_CHANNELS* pMsg);
	VOID	FieldSocketCityGetBuildingListOk(MSG_FC_CITY_GET_BUILDING_LIST_OK* pMsg);
	VOID	FieldSocketGetShopWarpTargetMapListOk(MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK* pMsg);
	VOID	FieldSocketAdminGetServerStatOk(MSG_FC_ADMIN_GET_SERVER_STAT_OK* pMsg);
	VOID	FieldSocketConnectClose(MSG_FC_CONNECT_CLOSE* pMsg);
	VOID	FieldSocketString128(MSG_FC_STRING_128* pMsg);
	VOID	FieldSocketString256(MSG_FC_STRING_256* pMsg);
	VOID	FieldSocketString512(MSG_FC_STRING_512* pMsg);
	VOID	FieldSocketWorldNotification(MSF_FC_WORLD_NOTIFICATION* pMsg);
	VOID	FieldSocketError(MSG_ERROR* pMsg);
	VOID	FieldSocketInvalidPacket(char* pPacket);
	VOID	FieldSocketMonsterSkillUseSkill(MSG_FC_MONSTER_SKILL_USE_SKILL* pMsg);
	VOID	FieldSocketMonsterSkillEndSkill(MSG_FC_MONSTER_SKILL_END_SKILL* pMsg);
	VOID	FieldSocketInfoGetSimpleItemInfoOk(MEX_ITEM_INFO & ItemInfo);
	VOID	FieldSocketInfoGetEnchantCostOk(MSG_FC_INFO_GET_ENCHANT_COST_OK* pMsg);
	VOID	FieldSocketItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg);
	VOID	FieldSocketGuildGetWarInfoOk(MSG_FC_GUILD_GET_WAR_INFO_OK* pMsg);
	VOID	FieldSocketStoreMoveItemOk(MSG_FC_STORE_MOVE_ITEM_OK* pMsg);
	VOID	FieldSocketCharacterGetOtherParamfactorOk(MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK* pMsg);
	VOID	FieldSocketStoreUpdatePossess(MSG_FC_STORE_UPDATE_POSSESS* pMsg);
	VOID	FieldSocketStoreUpdateRareFix(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	VOID	FieldSocketQuestDiscardQuestOk(MSG_FC_QUEST_DISCARD_QUEST_OK* pMsg);
	VOID	FieldSocketTimerStartTimer(MSG_FC_TIMER_START_TIMER* pMsg);
	VOID	FieldSocketTimerStopTimer(MSG_FC_TIMER_STOP_TIMER* pMsg);
	VOID	FieldSocketTimerUpdateTimer(MSG_FC_TIMER_UPDATE_TIMER* pMsg);
	VOID	FieldSocketTimerPauseTimer(MSG_FC_TIMER_PAUSE_TIMER* pMsg);
	VOID	FieldSocketTimerContinueTimer(MSG_FC_TIMER_CONTINUE_TIMER* pMsg);
	VOID	FieldSocketSKillPrepareUseOk(MSG_FC_SKILL_PREPARE_USE_OK* pMsg);
	VOID	FieldSocketCancelPrepareOk(MSG_FC_SKILL_CANCEL_PREPARE_OK* pMsg);
	VOID	FieldSocketMoveWeaponVelOk(MSG_FC_MOVE_WEAPON_VEL_OK* pMsg);
	VOID	FieldSocketCountdownStart(MSG_FC_COUNTDOWN_START* pMsg);
	VOID	FieldSocketBattleShowDamage(MSG_FC_BATTLE_SHOW_DAMAGE* pMsg);
	VOID	FieldSocketItemUseBuffOk(MSG_FC_ITEM_USE_INFLUENCE_BUFF_OK* pMsg);
	VOID	FieldSocketItemUseInfluenceGameEventOk(MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK* pMsg);
	VOID	FieldSocketShopBuyWarPointItemOK(MSG_FC_SHOP_BUY_WARPOINT_ITEM_OK* pMsg);


	VOID	FieldSocketBattleAttackOk(MSG_FC_BATTLE_ATTACK_OK* pMsg);
	VOID	FieldSocketBattleAttackFindOk(MSG_FC_BATTLE_ATTACK_FIND_OK* pMsg);
	VOID	FieldSocketBattleMineAttackOk(MSG_FC_BATTLE_MINE_ATTACK_OK* pMsg);
	VOID	FieldSocketBattleMineAttackFindOk(MSG_FC_BATTLE_MINE_ATTACK_FIND_OK* pMsg);
	VOID	FieldSocketObjectChangeBodyConditionOk(MSG_FC_OBJECT_CHANGE_BODYCONDITION_OK* pMsg);
	VOID	FieldSocketBattleAttackEvasionOk(MSG_FC_BATTLE_ATTACK_EVASION_OK* pMsg);
	VOID	FieldSocketWarNotifyInvasion(MSG_FC_WAR_NOTIFY_INVASION *pMsg);
	VOID	FieldSocketWarNotifyInfluenceMonsterDead(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD *pMsg);
	VOID	FieldSocketWarNotifyInfluenceAutoMonsterDead(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD *pMsg); // 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
	VOID	FieldSocketWarNotifyInfluenceMonsterIvasion(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION* pMsg);
	VOID	FieldSocketWarBossMonsterSummonDataInvasion(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA* pMsg);
	VOID	FieldSocketWarSignBoardInsertString(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING* pMsg);
	VOID	FieldSocketWarReqSignBoardStringListOk(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK* pMsg);
	VOID	FieldSocketWarSignBoardDeleteString(MSG_FC_WAR_SIGN_BOARD_DELETE_STRING* pMsg);
	VOID	FieldSocketWarUpdateContributionPointOk(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK* pMsg);
	VOID	FieldSocketWarJacoMonsterSummon(MSG_FC_WAR_JACO_MONSTER_SUMMON* pMsg);
	VOID 	FieldSocketWarInfluenceData(MSG_FC_WAR_INFLUENCE_DATA* pMsg);
	VOID	FieldSocketEventNotifyMsgGetOK(MSG_FC_EVENT_NOTIFY_MSG_GET_OK* pMsg);
	VOID	FieldSocketCityWarInfoInfluenceOK(MSG_FC_CITY_WARINFO_INFLUENCE_OK* pMsg);
	VOID	FieldSocketCityWarInfoOutPostOK(MSG_FC_CITY_WARINFO_OUTPOST_OK * pMsg);
	VOID	FieldSocketInfoGetGuildMarkOK(MSG_FC_INFO_GET_GUILDMARK_OK* pMsg);
	
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	VOID	FieldSocketWarStrategypointMonsterSummon(MSG_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON* pMsg);

	VOID	FieldSocketAuctionInsertItem(MSG_FC_AUCTION_INSERT_ITEM* pMsg);
	VOID	FieldSocketAuctionRegisterItemOk(MSG_FC_AUCTION_REGISTER_ITEM_OK* pMsg);
	VOID	FieldSocketAuctionCancerRegisterOk(MSG_FC_AUCTION_CANCEL_REGISTER_OK* pMsg);
	VOID	FieldSocketAuctionBuyItemOk(MSG_FC_AUCTION_BUY_ITEM_OK* pMsg);
	VOID	FieldSocketAuctionPutEnchant(MSG_FC_AUCTION_PUT_ENCHANT* pMsg);

	VOID	FieldSocketMoveLockonOk(MSG_FC_MOVE_LOCKON_OK* pMsg);
	VOID	FieldSocketMoveUnlockonOk(MSG_FC_MOVE_UNLOCKON_OK* pMsg);
	VOID	FieldSocketMoveRolling(MSG_FC_MOVE_ROLLING_OK *pMsg);
	VOID	FieldSocketInfoGetCurrentMapInfoOk(MSG_FC_INFO_GET_CURRENT_MAP_INFO_OK* pMsg);
	VOID	FieldSocketCharacterCashPremiumCardInfo(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* pMsg);
	VOID	FieldSocketStoreInsertUsingItem(MSG_FC_STORE_INSERT_USINGITEM* pMsg);
	VOID	FieldSocketStoreDeleteUsingItem(MSG_FC_STORE_DELETE_USINGITEM* pMsg);
	VOID	FieldSocketGuildSummonMember(MSG_FC_GUILD_SUMMON_MEMBER* pMsg);
	VOID	FieldSocketItemUseCarditemRestoreNotify(MSG_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY* pMsg);
	VOID	FieldSocketItemUseCarditemGuildsummonNotify(MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY* pMsg);
	// 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
	VOID	FieldSocketInfoGetHappyHourEventInfoOk(BOOL bLevelUp, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	VOID	FieldSocketInfoGetGameEventInfoOk(MSG_FC_INFO_GET_GAME_EVENT_INFO_OK* pMsg);
	VOID	FieldSocketMonsterTutorialMonsterDeadNotify(MSG_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY* pMsg);

	// 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화
	VOID	FieldSocketMonsterHPTalk( MSG_FC_MONSTER_HPTALK* pMsg );
	// end 2009. 09. 18 by ckPark 인피니티 필드 HP에 따른 몬스터 대화
// 2011-02-08 by jhahn EP4 트리거  시스템 오브젝트 변경			
	VOID	FieldObjectChangeOK(MSG_FC_OBJECT_CHANGE* pMsg);
	VOID	FieldSocketShopClose(MSG_FC_SHOP_CLOSE* pMsg);
	VOID	FieldSocketSetLimitTime( MSG_FC_SET_LIMITTIME* pMsg);
//end 2011-02-08 by jhahn EP4 트리거  시스템 오브젝트 변경			

	// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	VOID	FieldSocketMonsterSkill( MSG_FC_MONSTER_SKILL* pMsg );
	VOID	FieldSocketMonsterMonsterBarrierUsing( MSG_FC_BATTLE_MONSTER_BARRIER_USING* pMsg );
	VOID	FieldSocketMonsterMonsterBarrierUse( MSG_FC_BATTLE_MONSTER_BARRIER_USE* pMsg );
	VOID	FieldSocketCharacterDebuffDotInfo( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK* pMsg );
	VOID	FieldSocketCharacterDebuffDotApply( MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING* pMsg );
	VOID	FieldSocketCharacterDebuffDotRelease( MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE* pMsg );
	// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
	//2011-02-08 by jhahn EP4 트리거  시스템
	VOID	FieldSocketTriggerMapBuffSkill(MSG_FC_TRIGGER_MAP_BUFF_SKILL* pMsg );				//2011-10-24 by jhahn 트리거 시스템 
	//end 2011-02-08 by jhahn EP4 트리거  시스템

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	VOID	FieldSocketInfinityModeListOK( MSG_FC_INFINITY_MODE_LIST_OK* pMsg );
	VOID	FieldSocketInfinityReadyListOK( MSG_FC_INFINITY_READY_LIST_OK* pMsg );
	VOID	FieldSocketInfinityCreateOK( MSG_FC_INFINITY_CREATE_OK* pMsg );
	VOID	FieldSocketInfinityMemberInfoList( MSG_FC_INFINITY_MEMBER_INFO_LIST_OK* pMsg );
	VOID	FieldSocketInfinityLeaveOK( MSG_FC_INFINITY_LEAVE_OK* pMsg );
	VOID	FieldSocketInfinityBanOK( MSG_FC_INFINITY_BAN_OK* pMsg );
	VOID	FieldSocketInfinityJoinRequest( MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pMsg );
	VOID	FieldSocketInfinityJoinOK( MSG_FC_INFINITY_JOIN_OK* pMsg );
	VOID	FieldSocketInfinityEnter( MSG_FC_INFINITY_ENTER* pMsg );
	VOID	FieldSocketInfinityReadyOK( MSG_FC_INFINITY_READY_OK* pMsg );
	VOID	FieldSocketInfinityReadyCancelOK( MSG_FC_INFINITY_READY_CANCEL_OK* pMsg );
	VOID	FieldSocketInfinityChangeMaster( MSG_FC_INFINITY_CHANGE_MASTER_OK* pMsg );
	VOID	FieldSocketInfinityCinema( MSG_FC_INFINITY_CINEMA* pMsg );
	VOID	FieldSocketInfinitySuccessFIN( void );
	VOID	FieldSocketInfinityFailFIN( void );
	VOID	FieldSocketInfinityReadyFinishMainSvrStart( void );
	VOID	FieldSocketInfinityTenderDropItemInfo( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pMsg );
	VOID	FieldSocketInfinityTenderStart( MSG_FC_INFINITY_TENDER_START* pMsg );
	VOID	FieldSocketInfinityPutInTenderOK( MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK* pMsg );
	VOID	FieldSocketInfinityTenderResult( MSG_FC_INFINITY_TENDER_RESULT* pMsg );
	VOID	FieldSocketInfinityTenderResultTimeOver( MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER* pMsg );
	VOID	FieldSocketInfinityTenderResultAllGiveUp( MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP* pMsg );
	VOID	FieldSocketInfinityApplyResistanceItem( MSG_FC_INFO_APPLY_RESISTANCE_ITEM* pMsg );
	VOID	FieldSocketInfinityShopItemHeader( void );
	VOID	FieldSocketInfinityShopItem( MSG_FC_SHOP_INFINITY_ITEM* pMsg );
	VOID	FieldSocketInfinityShopItemDone( void );
	VOID	FieldSocketInfinityChangeLimitTime( MSG_FC_INFINITY_CHANGE_LIMITTIME* pMsg );
	VOID	FieldSocketInfinityJoinCancelRequestMaster( MSG_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER* pMsg );
	VOID	FieldSocketInfinityRequestRestartByDisconnect( void );
	VOID	FieldSocketInfinityDeletedCinema( MSG_FC_INFINITY_DELETED_CINEMA* pMsg );
	VOID	FieldSocketInfinityEnterByDisConnect( MSG_FC_INFINITY_ENTER_BY_DISCONNECT* pMsg );
	VOID	FieldSocketInfinityPartyAutoCreated( MSG_FC_PARTY_AUTO_CREATED* pMsg );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	VOID	FieldSocketInfinityRegsiterDifficultyBonusInfoListData ( MSG_INFINITY_DIFFICULTY_LIST_OK* pMsg );
	VOID	FieldSocketInfinityChangeRoomDifficultyLevelOK( MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK* pMsg );
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
	VOID	FieldSocketInfinityReadyCancelAllOK ( MSG_FC_INFINITY_READY_CANCEL_ALL_OK* pMsg );
	// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

	// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
	VOID	FieldSocketInfinityCinemaSkip ( MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK * pMsg );
	// End 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	VOID	FieldSocketInfoApplyDestParam( MSG_FC_INFO_APPLY_DESTPARAM* pMsg );
	VOID	FieldSocketInfoApplyDestParamList( MSG_FC_INFO_APPLY_DESTPARAM_LIST* pMsg );
	VOID	FieldSocketItemEndCoolItemOK( MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK* pMsg );
	VOID	FieldSocketItemGetCoolTimeInfoOK( MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK* pMsg );
	VOID	FieldSocketItemUseInvokingWearItemBuff( MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF* pMsg );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
	VOID	FieldSocketInfinityChangeAliveForGameClearMonsterHP( MSG_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP* pMsg );
	VOID	FieldSocketMonsterCreatedAliveForGameClear( MSG_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR* pMsg );
	// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)

	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	VOID	FieldSocketMonsterCancelSkill( MSG_FC_MONSTER_SKILL_CANCEL* pMsg );
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)

	VOID	FieldSocketCharacterTutorialSkipOk(MSG_FC_CHARACTER_TUTORIAL_SKIP_OK* pMsg);
	VOID	FieldSocketTutorialStartOK(MSG_FC_TUTORIAL_START_OK* pMsg);
	VOID	FieldSocketTutorialCompleteOK(MSG_FC_TUTORIAL_COMPLETE_OK* pMsg);
	VOID	FieldSocketTutorialEndOK();
	VOID	FieldSocketCharacterDeadNotify(MSG_FC_CHARACTER_DEAD_NOTIFY* pMsg);
	VOID	FieldSocketCitywarMonsterDead(MSG_FC_CITYWAR_MONSTER_DEAD* pMsg);
	VOID	FieldSocketCharacterChangeInfluenceType(MSG_FC_CHARACTER_CHANGE_INFLUENCE_TYPE* pMsg);
	VOID	FieldSocketCharacterChangePropensity(MSG_FC_CHARACTER_CHANGE_PROPENSITY* pMsg);
	VOID	FieldSocketRequestRepairOk(MSG_FC_SHOP_REQUEST_REPAIR_OK* pMsg);
	VOID	FieldSocketSkillConfirmUseAck(MSG_FC_SKILL_CONFIRM_USE_ACK* pMsg);
	VOID	FieldSocketSkillConfirmUse(MSG_FC_SKILL_CONFIRM_USE* pMsg);
	VOID	FieldSocketStoreUpdateItemStorage(MSG_FC_STORE_UPDATE_ITEMSTORAGE* pMsg);
	VOID	FieldSocketShopBuyColorItemOk(MSG_FC_SHOP_BUY_COLOR_ITEM_OK* pMsg);
	VOID	FieldSocketStoreUpdateUsingItem(MSG_FC_STORE_UPDATE_USINGITEM* pMsg);
	VOID	FieldSocketStoreUpdateItemNum(MSG_FC_STORE_UPDATE_ITEMNUM* pMsg);
	VOID	FieldSocketStoreLogGuildItemOK(MSG_FC_STORE_LOG_GUILD_ITEM_OK* pMsg);
	VOID	FieldSocketStoreLogGuildItemOKHeader();
	VOID	FieldSocketStoreLogGuildItemOKDone();
	VOID	FieldSocketEventCallWarpEventReqeust(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST* pMsg);

	// 2007-03-29 by dgwoo 방송용 옵저버 모드.
	VOID	FieldSocketCharacterObserverStartOK(MSG_FC_CHARACTER_OBSERVER_START* pMsg);
	VOID	FieldSocketCharacterObserverEndOK(MSG_FC_CHARACTER_OBSERVER_END* pMsg);
	VOID	FieldSocketCharacterObserverRegOK(MSG_FC_CHARACTER_OBSERVER_REG* pMsg);
	VOID	FieldSocketCharacterObserverInfoOK(MSG_FC_CHARACTER_OBSERVER_INFO* pMsg);

	VOID	FieldSocketArenaWatchReady(MSG_FC_ARENA_WATCH_READY* pMsg);
	VOID 	FieldSocketArenaWatchStart(MSG_FC_ARENA_WATCH_START* pMsg);
	VOID	FieldSocketArenaWatchEnd();
	VOID	FieldSocketArenaWatchRemoveUser(MSG_FC_ARENA_WATCH_REMOVE_USER* pMsg);
	VOID	FieldSocketCharacterWatchInfo(MSG_FC_CHARACTER_WATCH_INFO* pMsg);

	// 2007-08-16 by dgwoo 전진기지
	VOID	FieldSocketOutPostWarBefore(MSG_FC_OUTPOST_WAR_BEFORE* pMsg);
	VOID	FieldSocketOutPostWarStart(MSG_FC_OUTPOST_WAR_START* pMsg);
	VOID	FieldSocketOutPostProtectorDestroy(MSG_FC_OUTPOST_PROTECTOR_DESTROY* pMsg);
	VOID	FieldSocketOutPostResetStartOk(MSG_FC_OUTPOST_RESET_START_OK* pMsg);
	VOID	FieldSocketOutPostResetDestroy(MSG_FC_OUTPOST_RESET_DESTROY* pMsg);
	VOID	FieldSocketOutPostResetSuccess(MSG_FC_OUTPOST_RESET_SUCCESS* pMsg);
	VOID	FieldSocketOutPostWarEnd(MSG_FC_OUTPOST_WAR_END* pMsg);
	VOID	FieldSocketInfoNoticeRequestOk(MSG_FC_INFO_NOTICE_REQUEST_OK* pMsg);
	VOID	FieldSocketInfoNoticeRegOk();
	VOID	FieldSocketInfoNoticeModifyOk();
	VOID	FieldSocketInfoExpediencyfundPayBackOk(MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK* pMsg);
	VOID	FieldSocketInfoExpediencyfundRequestOk(MSG_FC_INFO_EXPEDIENCYFUND_REQUEST_OK* pMsg);
	VOID	FieldSocketOutPostNextWarInfoRequestOk(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK* pMsg);
	VOID 	FieldSocketOutPostNextWarSetTimeOk(MSG_FC_OUTPOST_NEXTWAR_SET_TIME_OK* pMsg);
	VOID	FieldSocketOutPostWarInfo(MSG_FC_OUTPOST_WAR_INFO* pMsg);

	// 2007-09-19 by dgwoo 텔레포트.
	VOID	FieldSocketEventTeleportBuildComplete(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE* pMsg);
	VOID	FieldSocketEventTeleportDestroy(MSG_FC_EVENT_TELEPORT_DESTROY* pMsg);

	// 2007-04-19 by dgwoo 아레나 시스템.
	VOID	FieldSocketArenaRequestTeamOK(MSG_FC_ARENA_REQUEST_TEAM_OK* pMsg);
	VOID	FieldSocketArenaCreateTeamOK(MSG_FC_ARENA_CREATE_TEAM_OK* pMsg);
	VOID	FieldSocketArenaEnterTeamOK(MSG_FC_ARENA_ENTER_TEAM_OK* pMsg);
	VOID	FieldSocketArenaTeamMemberList(MSG_FC_ARENA_TEAM_MEMBER_LIST* pMsg);
	VOID	FieldSocketArenaReaveTeamOK();
	VOID	FieldSocketArenaTeamReady(MSG_FC_ARENA_TEAM_READY* pMsg);
	VOID	FieldSocketArenaEnterRoom(MSG_FC_ARENA_ENTER_ROOM* pMsg);
	VOID	FieldSocketArenaRoomWarStart();
	VOID	FieldSocketArenaRoomWarInfo(MSG_FC_ARENA_ROOM_WAR_INFO* pMsg);
	VOID	FieldSocketArenaRoomWarFinishHeader(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER* pMsg);
	VOID	FieldSocketArenaRoomWarFinish(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);
	VOID	FieldSocketArenaRoomWarFinishDone(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE* pMsg);
	VOID	FieldSocketArenaRoomWarFinishDraw(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW* pMsg);
	VOID	FieldSocketArenaFinishWarpOK(MSG_FC_ARENA_FINISH_WARP_OK* pMsg);
	VOID	FieldSocketArenaBeforRoomWarFinish();
	VOID	FieldSocketArenaRequestCreateTeamInfoOK(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK* pMsg);
	VOID	FieldSocketArenaTeamMatching();
	VOID	FieldSocketArenaRequestOtherTeamInfoOK(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK* pMsg);
	VOID	FieldSocketArenaGMCommandInfoHeader();
	VOID	FieldSocketArenaGMCommandInfo(MSG_FC_ARENA_GM_COMMAND_INFO* pMsg);
	VOID	FieldSocketArenaGMCommandInfoDone(MSG_FC_ARENA_GM_COMMAND_INFO_DONE* pMsg);

	VOID	FieldSocketBazaarSellPutItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg);
	VOID	FieldSocketBazaarBuyPutItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg);
	VOID	FieldSocketBazaarSellCancelItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg);
	VOID	FieldSocketBazaarBuyCancelItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg);
	VOID	FieldSocketBazaarSellStartOK();
	VOID	FieldSocketBazaarBuyStartOK();
	VOID	FieldSocketBazaarInfoOK(MSG_FC_BAZAAR_INFO_OK* pMsg);
	VOID	FieldSocketBazaarSellReQuestItemListOK(MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK* pMsg);
	VOID	FieldSocketBazaarBuyReQuestItemListOK(MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK* pMsg);
	VOID	FieldSocketBazaarSellItemEnchantlistOK(MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK* pMsg);
	VOID	FieldSocketBazaarCustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg);
	VOID	FieldSocketBazaarSellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	VOID	FieldSocketBazaarBuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);

	VOID	FieldSocketItemUseRandomBoxOk(MSG_FC_ITEM_USE_RANDOMBOX_OK* pMsg);
	VOID	FieldSocketItemUseRandomBoxOkDone();	// 2008-08-26 by bhsohn 랜덤박스 시스템 변경
	VOID	FieldSocketStoreRequestQuickslotOK(MSG_FC_STORE_REQUEST_QUICKSLOT_OK* pMsg);
	VOID	FieldSocketCharacterUseBonusstatOK(MSG_FC_CHARACTER_USE_BONUSSTAT_OK* pMsg);
	VOID	FieldSocketUseSkillSupportItemOK(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK* pMsg);
	VOID	FieldSocketGetServerDataTimeOK(MSG_FC_INFO_GET_SERVER_DATE_TIME_OK* pMsg);
	VOID	FieldSocketRequestPartyWarp(MSG_FC_QUEST_REQUEST_PARTY_WARP* pMsg);

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	VOID	FieldSocketChatAllInfluenceOK(MSG_FC_CHAT_ALL_INFLUENCE * pMsg);
	VOID	FieldSocketChatOutPostGuild(MSG_FC_CHAT_OUTPOST_GUILD* pMsg);

	// 2007-10-29 by dgwoo 폴시스템
	VOID	FieldSocketCityPollRequestLeaderCandidateListOKHeader(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER* pMsg);
	VOID	FieldSocketCityPollRequestLeaderCandidateListOK(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK* pMsg);
	VOID	FieldSocketCityPollRequestLeaderCandidateListOKDone();
	VOID	FieldSocketCityPollRequestLeaderCandidateInfoOK(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK* pMsg);
	VOID	FieldSocketCityPollRequestLeaderCandidateInfoOkGuildMark(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK* pMsg);
	VOID	FieldSocketCityPollRegLeaderCandidateOK(MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK* pMsg);
	VOID	FieldSocketCityPollDeleteLeaderCandidateOK();
	VOID	FieldSocketCityPollRequestPollDateOK(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK* pMsg);
	VOID	FieldSocketCityPollVoteOK();
	VOID	FieldSocketCityPollLeaderElectionInfo(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO* pMsg);
// 2008-12-09 by dgwoo 미션마스터.
	VOID	FieldSocketQuestRequestMissionMasterHelpInvite(MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE* pMsg);
	VOID	FieldSocketQuestMissionMasterHelpInviteOK(MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK* pMsg);
	VOID	FieldSocketQuestMissionMasterHelpRejectOK(MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK* pMsg);
// end 2008-12-09 by dgwoo 미션마스터.

	// 2008-12-30 by bhsohn 전쟁보상추가 기획안
	VOID	FieldSocketBossContributionGuild(MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD* pMsg);
	// end 2008-12-30 by bhsohn 전쟁보상추가 기획안


	// 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	VOID	SetWarDeclareInfo(SDECLARATION_OF_WAR* pWarInfo, SDECLARATION_OF_WAR_FORBID_TIME* pForbidTime, BOOL bSetDefault);
	// end 2009. 02. 10 by ckPark 선전 포고 시스템 추가 수정
	
	
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	// 선전포고 정보가 도착했을 경우
	VOID	FieldSocketWarDeclareInfoOK(MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK* pMsg);
	// end 2009. 01. 12 by ckPark 선전 포고 시스템




	// HackShield
	// 2008-04-04 by bhsohn 핵쉴드 방식 변경
	//VOID	FieldSocketMoveHackShield_GuidReqMsg(MSG_FC_MOVE_HACKSHIELD_GuidReqMsg* pMsg);
	VOID	FieldSocketMoveHackShield_CRCReqMsg(MSG_FC_MOVE_HACKSHIELD_CRCReqMsg* pMsg);

	// 2008-11-28 by bhsohn XignCode추가
	VOID	FieldSocketMoveXignCodeReqScanInit(UINT nSocketNotifyType, MSG_FC_MOVE_XIGNCODE_REQ_SCAN_INIT* pMsg);
	VOID	FieldSocketMoveXignCodeReqScanCheck(UINT nSocketNotifyType, MSG_FC_MOVE_XIGNCODE_REQ_SCAN_CHECK* pMsg);


	// ETC
	VOID	InitCharacterGameStartOk(AVECTOR3 vPos, BitFlag16_t MapWeather);// CharacterGameStartOk, CharacterConnectGameStartOk
	VOID	CharacterChangeCurrentHPAndDamageBodyCondition(float fCurrentHP);//FieldSocketCharacterChangeCurrentHP,FieldSocketCharacterChangeCurrentHPDPSPEP
	VOID	CharacterChangeCurrentDP(float sCurrentDP);
	VOID	CharacterChangeCurrentSP(SHORT sCurrentSP);
	VOID	CharacterChangeCurrentEP(float fCurrentEP);
	VOID	EventWarpOtherMap(MAP_CHANNEL_INDEX &channelIndex);

	VOID	FieldSocketNotifyServerShutdown();
	VOID	FieldSocketWarMonsterCreated(MSG_FC_WAR_MONSTER_CREATED* pMsg);
	VOID	FieldSocketWarMonsterAutoDestroyed(MSG_FC_WAR_MONSTER_AUTO_DESTROYED* pMsg);
	VOID	FieldSocketWarMonsterDead(MSG_FC_WAR_MONSTER_DEAD* pMsg);

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	VOID	FieldSocketInfoSecondarypasswordCheckPassword(BOOL bSuccess);
	VOID	FieldSecondLockOk(MSG_FC_INFO_SECONDARYPASSWORD_LOCK_OK* pMsg);
	VOID	FieldSocketSecondPasswordUpdateOk(BOOL bSuccess);	
	// 2007-11-22 by bhsohn 아레나 통합서버
	void	FieldSocketConnectArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg);
	void	FieldSocketConnectArenaServerLoginOk(MSG_FC_CONNECT_ARENASERVER_LOGIN_OK* pMsg);
	void	FieldSocketArenaPossibleGameStart(MSG_FC_ARENA_POSSIBLE_GAMESTART* pMsg);	
	void	FieldSocketCharacterReadyGamestartFromArenaToMainserver(MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK* pMsg);


	// 2009. 04. 06 by ckPark 게임시작시 중량 인챈트 적용 안된상태로 나오던 버그(0001388)
	void	FieldSocketPutEnchantDone( void );
	// end 2009. 04. 06 by ckPark 게임시작시 중량 인챈트 적용 안된상태로 나오던 버그(0001388)

	VOID	FieldSocketEventCouponEventUseCouponOK(MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK* pMsg);
	VOID	FieldSocketEventCouponEventInfo(MSG_FC_EVENT_COUPON_EVENT_INFO* pMsg);

	// 2008-02-15 by bhsohn 네트워크 속도 체크
	VOID	FieldSocketConnectNetWorkCheckOk(MSG_FC_CONNECT_NETWORK_CHECK_OK* pMsg);

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	void	FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg);
	
	// 2015-05-1 by killburne
	void	FieldSocketInfoStrategicPointInfoDisplay(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY* pMsg);
	void	FieldSocketInfoStrategicPointInfoListDisplay(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST* pMsg);
	// 2008-03-10 by bhsohn 아레나에서 귀환 포탈 처리
	void	FieldSocketArenaUseCityWarpItem();
	void	FieldSocketInfoMSWarInfoDisplay(MSG_FC_INFO_MSWARINFO_DISPLAY* pMsg);
	void	FieldSocketInfoMSWarInfoDisplayOptionOK(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK*);
	// 2008-03-19 by bhsohn 모선전, 거점전 정보창
	void FieldSocketInfoMsWarInfoResultOk(MSG_FC_INFO_MSWARINFO_RESULT_OK* pMsg);
	void FieldSocketInfoSpWarInfoResultOkHeader();
	void FieldSocketInfoSpWarInfoResultOk(MSG_FC_INFO_SPWARINFO_RESULT_OK* pMsg);
	// end 2008-03-19 by bhsohn 모선전, 거점전 정보창
	void FieldSocketEventClickTeleportOK(MSG_FC_EVENT_CLICK_TELEPORT_OK* pMsg);
	// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	void FieldSocketInfoSpWarInfoResultOkDone();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 편지 리스트 메시지
	void FieldSocketChatLetterRqTitleHeader();
	void FieldSocketChatLetterRqTitle(MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK* pMsg);	
	void FieldSocketChatLetterRqTitleOkDone();

	void FieldSocketChatALLLetterRqTitle(MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK* pMsg); // 전체 편지
	void FieldSocketChatALLLetterRqTitleOkDone();	 // 전체 편지 다 받았음

	void FieldSocketLetterReadOk(MSG_FC_CHAT_LETTER_READ_OK* pMsg);		// 편지 내용 요청
	void FieldSocketAllLetterReadOk(MSG_FC_CHAT_ALLLETTER_READ_OK* pMsg);		// 전체편지 내용 요청
	void FieldSocketChatLetterDelOk();	// 편지 삭제
	void FieldSocketChatSendOk();		// 편지 보내기
	void FiedldSocketChatLetterReceive(MSG_FC_CHAT_LETTER_RECEIVE* pMsg);
	void FiedldSocketChatAllLetterReceive(MSG_FC_CHAT_ALLLETTER_RECEIVE* pMsg);
	void FieldSocketCharacterGetUserInfoOk(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg);
	
	void FieldSocketCharacterGetUserItemInfoSecretOk(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK* pMsg);	// 2012-06-14 by isshin 아템미리보기					// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
	void FieldSocketCharacterGetUserItemInfoOk(MSG_FC_ENEMY_ITEM_INFO* pMsg);		// 2012-06-14 by isshin 아템미리보기
	void FieldSocketCharacterGetUserItemInfoDone(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg);	// 2012-06-14 by isshin 아템미리보기		
	void FieldSocketCharacterChangeInfoOptionSecetOk();
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 2008-11-04 by dgwoo LuckyMachine기획 추가.
	void FieldSocketItemUseRuckyItemOK(MSG_FC_ITEM_USE_LUCKY_ITEM_OK* pMsg);
	// 2008-11-04 by dgwoo LuckyMachine기획 추가.

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	void FieldSocketItemChangedShapeItemNum( MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg );
	void FieldSocketItemChangedEffectItemNum( MSG_FC_ITEM_CHANGED_EFFECTITEMNUM* pMsg );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	// 2010. 01. 12 by ckPark 무게 게이지 계산시 아이템 스텟이 아닌 원본 스텟으로 계산하던 문제 해결
	void FieldSocketPutAllQuestDone( void );
	// end 2010. 01. 12 by ckPark 무게 게이지 계산시 아이템 스텟이 아닌 원본 스텟으로 계산하던 문제 해결
	
	// 2009-02-03 by bhsohn 미션 시작시 완료 체크 체크
	void FieldSocketQuestPutAllQuestMonsterCountDone();
	// end 2009-02-03 by bhsohn 미션 시작시 완료 체크 체크

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	void FieldSocketInfoWRKGetServiceInfoOk(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg);
	void FieldSocketInfoWRKGetServiceInfoOkImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg);
	void FieldSocketInfoWRKGetServiceInfoOkDone();
	void FieldSocketInfoWRKGetLevelRankerListOk(MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK* pMsg);
	void FieldSocketInfoWRKGetFameRankerListOk(MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK* pMsg);
	void FieldSocketInfoWRKGetPVPRankerListOk(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK* pMsg);
	void FieldSocketCharacterChangeNickNameOk(MSG_FC_CHARACTER_CHANGE_NICKNAME_OK* pMsg);
	void FieldSocketInfoWrkGetSelfRankingOk(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK* pMsg);
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2009-03-10 by bhsohn nProtector CS모듈 추가
	void FieldSocketMoveNProtectReqAuthData(MSG_FC_MOVE_NPROTECT_REQ_AUTH_DATA* pMsg);
	// end 2009-03-10 by bhsohn nProtector CS모듈 추가
	// 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
	void FieldSocketMoveXTrapReqStep(MSG_FC_MOVE_XTRAP_REQ_STEP* pMsg);	
	//end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)

	// 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
	void FieldSocketApexReqApexData( MSG_FC_MOVE_APEX_REQ_APEXDATA* pMsg );
	// end 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업

	// 2009. 10. 14 by jskim 프리스카 제거 
	void FieldSocketCharacterChangeStart(MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK* pMsg);
	//end 2009. 10. 14 by jskim 프리스카 제거 
	// 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
	void FieldSocketChangeAddedInventoryCount(MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT* pMsg);
	//end 2009. 11. 3 by jskim 캐쉬(인벤/창고 확장) 아이템 추가 구현
	// 2010. 03. 18 by jskim 몬스터변신 카드
	void FieldSocketItemUpdateTransformerOk(MSG_FC_ITEM_UPDATE_TRANSFORMER_OK* pMsg);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	// 2009-04-01 by bhsohn 세력 불균형, 세력 초기화 시스템
	void FieldSocketQuestInserQuest(MSG_FC_QUEST_INSERT_QUEST* pMsg);
	void FieldSocketQuestDeleteQuest(MSG_FC_QUEST_DELETE_QUEST* pMsg);
	// end 2009-04-01 by bhsohn 세력 불균형, 세력 초기화 시스템
	
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void FieldSocketCharacterKillCount(MSG_FC_CHARACTER_KILLCOUNT* pMsg);
	void FieldSocketCharacterRestCount(MSG_FC_CHARACTER_RESTCOUNT* pMsg);
	void FieldSocketCharacterBonusExpRate(MSG_FC_CHARACTER_BONUS_EXP_RATE* pMsg);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	
	// 2010. 06. 08 by jskim 홈프리미엄 프로토콜 처리
	void FieldSocketHomePremium(MSG_FC_ITEM_HOMEPREMIUM_INFO* pMsg);
	//end 2010. 06. 08 by jskim 홈프리미엄 프로토콜 처리

	// 2010-06-15 by shcho&hslee 펫시스템 - 프로토콜 처리
	void FieldSocketCurrentPetBaseData(MSG_FC_ITEM_PET_BASEDATA* pMsg);
//	void FieldSocketCurrentPetSkillData(MSG_FC_ITEM_PET_SKILLDATA* pMsg);		   //2011-10-06 by jhahn 파트너 성장형 시스템
//	void FieldSocketCurrentPetSocketData(MSG_FC_ITEM_PET_SOCKETDATA* pMsg);		   //2011-10-06 by jhahn 파트너 성장형 시스템
	// End 2010-06-15 by shcho&hslee 펫시스템 - 프로토콜 처리

	void FieldSocketStoreUpdateArmorItemEventInfo(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO* pMsg); // 2013-05-15 by ssjung 이달의 아머 - 이달의 아머 변경 사항 패킷 관련 

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	void FieldSocketCollectionInfo(MSG_FC_COLLECTION_INFO* pMsg);
	void FieldSocketCollectionShapeChange(MSG_FC_COLLECTION_SHAPE_CHANGE* pMsg);
	void UpdateStoreShapeItemNum(MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg );
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	// 2013-06-26 by bhsohn 아머 컬렉션 추가 개발
	void PopupUseRandomBoxOk(INT nResultItemNum0);	// 생성된 ItemNum
	void PopupGetCollectionInfo(INT nResultItemNum0);	// 얻은 아머 컬렉션 정보
	// END 2013-06-26 by bhsohn 아머 컬렉션 추가 개발

	// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
	void FieldSocketCollectionMontylyAromorInit();
	void FieldSocketCollectionMontylyAromorInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg);
	// end 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 

	void FieldSocketWarInfluenceConsecutivePoint(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT* pMsg);	// 2013-08-02 by ssjung 역전의 버프 툴팁표시

	// OnRecvIMSocket~
	// 2007-11-22 by bhsohn 아레나 통합서버
	//int	OnRecvIMSocketMessage(DWORD wParam);
	int	OnRecvIMSocketMessage(DWORD wParam, UINT uMsg);
	// IMSocket~
	VOID	IMSocketConnectLoginOk(void);
	VOID	IMSocketChatPtoP(MSG_IC_CHAT_PTOP* pMsg);
	VOID	IMSocketChatParty(MSG_IC_CHAT_PARTY* pMsg);
	VOID	IMSocketChatGuild(MSG_IC_CHAT_GUILD* pMsg);
	VOID	IMSocketChatAll(char* pPacket);
	VOID	IMSocketChatSellAll(MSG_IC_CHAT_SELL_ALL* pMsg);
	VOID	IMSocketChatCashAll(MSG_IC_CHAT_CASH_ALL* pMsg);
	VOID	IMSocketChatInfluenceAll(char* pPacket);
	VOID	IMSocketChatWar(MSG_IC_CHAT_WAR* pMsg);
	VOID	IMSocketChatChatRoom(MSG_IC_CHAT_CHATROOM* pMsg);
	VOID	IMSocketChatCrossNation(MSG_IC_CHAT_CNC* pMsg);	// 2014-08-03 Future, adding Cross Nation Chat
	
	// 2007-11-22 by bhsohn 아레나 통합서버
//	VOID	IMSocketChatGetGuildOk(MSG_IC_CHAT_GET_GUILD_OK* pMsg);
//	VOID	IMSocketChatChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg);
//	VOID	IMSocketChatChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg);
	VOID	IMSocketChatGetGuildOk(MSG_IC_CHAT_GET_GUILD_OK* pMsg, UINT nSocketNotifyType);
	VOID	IMSocketChatChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg, UINT nSocketNotifyType);
	VOID	IMSocketChatChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg, UINT nSocketNotifyType);
	// end 2007-11-22 by bhsohn 아레나 통합서버

	void	FieldSocketEventInfluenceMark(MSG_FC_EVENT_INFLUENCEMARK* pMsg);
	void	FieldSocketEventInfluenceMarkEnd();

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	void	FieldSocketTradeOkTradeNotify(MSG_FC_TRADE_OK_TRADE_NOTIFY* pMsg);
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
	
	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	void	FieldSocketDissolDissolutionPutItem(MSG_FC_DISSOLVED_SUCCESS_AND_FAILED* pMsg);
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	
	// 2011-05-17 by jhahn	인피3차 몬스터 교체
	void	FieldMonsterChangeOK(MSG_FC_MONSTER_CHANGE_INDEX* pMsg);
	// end 2011-05-17 by jhahn	인피3차 몬스터 교체
	//2011-10-06 by jhahn 파트너 성장형 시스템
	void	FieldPetNameChangeOK(MSG_FC_ITEM_PET_SET_NAME_OK* pMsg );
	void	FieldPetExpRationOK(MSG_FC_ITEM_PET_SET_EXP_RATIO_OK* pMsg);
	
	void	FieldPetLevelChangeOK(MSG_FC_ITEM_PET_CHANGE_LEVEL* pMsg);
	void	FieldPetExpChangeOK(MSG_FC_ITEM_PET_CHANGE_EXP* pMsg);

	void	FieldPetSetSocket(MSG_FC_ITEM_PET_SET_SOCKET_OK* pMsg);
	void	FieldPetSetKitSlot(MSG_FC_ITEM_PET_SET_KIT_SLOT_OK* pMsg);	
	void	FieldPetSetAutoSkillSlot(MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK* pMsg);
	//end 2011-10-06 by jhahn 파트너 성장형 시스템
	// 2011-05-30 by jhahn, 인피3차 몬스터 삭제
	void	FieldMonsterDeleteOK(MSG_FC_MONSTER_CINEMA_DELETE_NOTIFY* pMsg);
	// end 2011-05-30 by jhahn, 인피3차 몬스터 삭제

	// 2011-05-30 by jhahn, 인피3차 시간 셋팅
	void	FieldSocketInfinityResetTime( MSG_FC_INFINITY_SET_LIMITTIME* pMsg );
	// end 2011-05-30 by jhahn, 인피3차 시간 셋팅


	// 2011-05-17 by jhahn	인피3차시네마 변경
	void	FieldInfinityCinemaOK(MSG_FC_INFINITY_MOVIE* pMsg);
	// end 2011-05-17 by jhahn	인피3차시네마 변경
	
	VOID	IMSocketPartyAcceptInviteOk(MSG_IC_PARTY_ACCEPT_INVITE_OK* pMsg);
	VOID	IMSocketPartyPutMember(MSG_IC_PARTY_PUT_MEMBER* pMsg);
	VOID	IMSocketPartyLeaveOk(MSG_IC_PARTY_LEAVE_OK* pMsg);
	VOID	IMSocketPartyBanMemberOk(MSG_IC_PARTY_BAN_MEMBER_OK* pMsg);
	VOID	IMSocketPartyDismemberOk(MSG_IC_PARTY_DISMEMBER_OK* pMsg);
	VOID	IMSocketPartyTransferMasterOk(MSG_IC_PARTY_TRANSFER_MASTER_OK* pMsg);
	VOID	IMSocketPartyPutAllMember(MSG_IC_PARTY_PUT_ALL_MEMBER* pMsg);
	VOID	IMSocketPartyChangeFlightFormationOk(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK* pMsg);
	VOID	IMSocketPartyGetFlightPosition(MSG_IC_PARTY_GET_FLIGHT_POSITION* pMsg);
	VOID	IMSocketPartyChangeFlightPosition(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION* pMsg);
	VOID	IMSocketPartyCancelFlightPosition(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION* pMsg);
	VOID	IMSocketPartyAllFlightPosition(MSG_IC_PARTY_ALL_FLIGHT_POSITION* pMsg);
	VOID	IMSocketPartyPutLastPartyInfo(MSG_IC_PARTY_PUT_LAST_PARTY_INFO* pMsg);
	VOID	IMSocketPartyUpdateMemberInfoMapName(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME* pMsg);
	VOID	IMSocketPartyMemberInvalidated(MSG_IC_PARTY_MEMBER_INVALIDATED* pMsg);
	VOID	IMSocketPartyMemberRejoined(MSG_IC_PARTY_MEMBER_REJOINED* pMsg);
	VOID	IMSocketPartyUpdateItemPos(MSG_IC_PARTY_UPDATE_ITEM_POS* pMsg);
	VOID	IMSocketGuildCreateOk(MSG_IC_GUILD_CREATE_OK* pMsg);
	VOID	IMSocketGuildGetGuildInfoOk(MSG_IC_GUILD_GET_GUILD_INFO_OK* pMsg);
	VOID	IMSocketGuildLeaveOk(MSG_IC_GUILD_LEAVE_OK* pMsg);
	VOID	IMSocketGuildBanMemberOk(MSG_IC_GUILD_BAN_MEMBER_OK* pMsg);
	VOID	IMSocketGuildRequestInviteQuestion(MSG_IC_GUILD_REQUEST_INVITE_QUESTION* pMsg);
	VOID	IMSocketGuildAcceptInviteOk(MSG_IC_GUILD_ACCEPT_INVITE_OK* pMsg);
	VOID	IMSocketGuildRejectInviteOk(MSG_IC_GUILD_REJECT_INVITE_OK* pMsg);
	VOID	IMSocketGuildSetMemberState(MSG_IC_GUILD_SET_MEMBER_STATE* pMsg);
	VOID	IMSocketGuildDismemberOk(MSG_IC_GUILD_DISMEMBER_OK* pMsg);
	VOID	IMSocketGuildCancelDismemberOk(MSG_IC_GUILD_CANCEL_DISMEMBER_OK* pMsg);
	VOID	IMSocketGuildGetDismemberDateOk(MSG_IC_GUILD_GET_DISMEMBER_DATE_OK* pMsg);
	VOID	IMSocketGuildChangeGuildNameOk(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK* pMsg);
	VOID	IMSocketGuildSetGuildMarkOk(MSG_IC_GUILD_SET_GUILD_MARK_OK* pMsg);
	VOID	IMSocketGuildGetGuildMarkOk(MSG_IC_GUILD_GET_GUILD_MARK_OK* pMsg);
	VOID	IMSocketGuildSetRankOk(MSG_IC_GUILD_SET_RANK_OK* pMsg);
	VOID	IMSocketGuildChangeGuildState(MSG_IC_GUILD_CHANGE_GUILD_STATE* pMsg);
	VOID	IMSocketGuildLoadingGuildDone(MSG_IC_GUILD_LOADING_GUILD_DONE* pMsg);
	VOID	IMSocketGuildGetOtherGuildInfoOk(MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK* pMsg);
	VOID	IMSocketAdminGetServerStatOk(MSG_IC_ADMIN_GET_SERVER_STAT_OK* pMsg);
	VOID	IMSocketConnectClose(MSG_IC_CONNECT_CLOSE* pMsg);
	VOID	IMSocketString128(MSG_IC_STRING_128* pMsg);
	VOID	IMSocketString256(MSG_IC_STRING_256* pMsg);						// 2006-04-13 by ispark
	VOID	IMSocketString512(MSG_IC_STRING_512* pMsg);						// 2006-04-13 by ispark
	VOID	IMSocketGuildWarReady(MSG_IC_GUILD_WAR_READY* pMsg);	
	VOID	IMSocketGuildStartWar(MSG_IC_GUILD_START_WAR* pMsg);
	VOID	IMSocketGuildEndWar(MSG_IC_GUILD_END_WAR* pMsg);
	VOID	IMSocketError(MSG_ERROR* pMsg);
	VOID	IMSocketInvalidPacket(char* pPacket);
	VOID	IMSocketVoipGetAccountUniquenumberOk(MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK* pMsg);
	VOID	IMSocketVoip1to1Dial(MSG_IC_VOIP_1to1_DIAL* pMsg);
	VOID	IMSocketVoip1to1DialOk(MSG_IC_VOIP_1to1_DIAL_OK* pMsg);
	VOID	IMSocketVoip1to1End(MSG_IC_VOIP_1to1_END* pMsg);
	VOID	IMSocketVoipNtoNCreateRoomRequestOk(MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK* pMsg);
	VOID	IMSocketVoipNtoNInviteYou(MSG_IC_VOIP_NtoN_INVITE_YOU* pMsg);
	VOID	IMSocketVoipError(MSG_IC_VOIP_ERROR* pMsg);
	VOID	IMSocketVoipNtoNGooutAll(MSG_IC_VOIP_NtoN_GOOUT_ALL* pMsg);
	VOID	IMSocketVoIPSetOK(MSG_IC_VOIP_SET_OK* pMsg);
	VOID	IMSocketChatFriendlistAll(MSG_IC_CHAT_FRIENDLIST_LOADING_OK* pMsg);
	VOID	IMSocketChatFriendlistInsertOk(MSG_IC_CHAT_FRIENDLIST_INSERT_OK* pMsg);
	VOID	IMSocketChatRejectlistAll(MSG_IC_CHAT_REJECTLIST_LOADING_OK* pMsg);
	VOID	IMSocketChatRejectlistInsertOk(MSG_IC_CHAT_REJECTLIST_INSERT_OK* pMsg);
	VOID	IMSocketChatFriendlistRefreshOk(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK* pMsg);
	VOID	IMSocketChatFriendlistDeleteOk(MSG_IC_CHAT_FRIENDLIST_DELETE_OK* pMsg);
	VOID	IMSocketChatRejectlistDeleteOk(MSG_IC_CHAT_REJECTLIST_DELETE_OK* pMsg);
	VOID	IMSocketCountdownStart(MSG_IC_COUNTDOWN_START* pMsg);
	VOID	IMSocketItemUseCarditemGuildChangeMemberCapacity(MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY* pMsg);
	VOID	IMSocketGuildGetGuildMemberListOk(MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK* pMsg);
	VOID	IMSocketAdminCallGMInfoOk(MSG_IC_ADMIN_CALLGM_INFO_OK* pMsg);
	VOID	IMSocketAdminCallGMViewOk(char* pPacket);
	VOID	IMSocketAdminCallGMBringOk(char* pPacket);
	VOID	IMSocketGuildEndWarAdminNotify(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY* pMsg);

	VOID	IMSocketChatRoomCreateOK(MSG_IC_CHATROOM_CREATE_OK* pMsg);
	VOID	IMSocketChatRoomListInfoOK(MSG_IC_CHATROOM_LIST_INFO_OK* pMsg);
	VOID	IMSocketChatRoomRequestInviteQuestion(MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION *pMsg);
	VOID	IMSocketChatRoomJoinOK();
	VOID	IMSocketChatRoomAcceptInviteOK(MSG_IC_CHATROOM_ACCEPT_INVITE_OK* pMsg);
	VOID	IMSocketChatRoomRejectInviteOK();
	VOID	IMSocketChatRoomLeaveOK(MSG_IC_CHATROOM_LEAVE_OK* pMsg);
	VOID	IMSocketChatRoomBanOK(MSG_IC_CHATROOM_BAN_OK* pMsg);
	VOID	IMSocketChatRoomChangeNameOK(MSG_IC_CHATROOM_CHANGE_NAME_OK* pMsg);
	VOID	IMSocketChatroomChangeMasterOK(MSG_IC_CHATROOM_CHANGE_MASTER_OK* pMsg);
	VOID	IMSocketChatRoomChangeLockPWOK(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK* pMsg);
	VOID	IMSocketChatRoomChangeMaxMemberOK(MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK* pMsg);
	VOID	IMSocketChatRoomMemberInfoOK(MSG_IC_CHATROOM_MEMBER_INFO_OK* pMsg);
	VOID	IMSocketChatRoomOtherMemberInfoOK(MSG_IC_CHATROOM_MEMBER_INFO_OK* pMsg);
	// 2008-07-11 by bhsohn 접속한 친구 리스트 시스템 추가
	void	IMSocketChatOnlineEachOtherFriendCount(MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT* pMsg);

	// 2008-06-16 by bhsohn EP3관련 버그수정
	void	IMSocketGuildUpdateIntroductionOk();
	void	IMSocketGuildDelIntroductionOk();
	void	IMSocketUpdateSelfIntroOk();
	void	IMSocketDelSelfIntroOk();

	// 2008-06-18 by bhsohn 여단원증가 카드 관련 처리
	void	IMSocketGuildChangeMemberShip(MSG_IC_GUILD_CHANGE_MEMBERSHIP* pMsg);

	
	// 2009. 01. 12 by ckPark 선전 포고 시스템
	// 선전포고 설정이 도착했을 경우
	void	IMSocketWarDeclareSetOK(MSG_IC_INFO_DECLARATION_MSWAR_SET_OK* pMsg);
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
	// 2010. 03. 18 by jskim 몬스터변신 카드
	void	IMSocketPartyUpdateItemTransformerOk(MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK* pMsg);
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	// 2008-07-17 by bhsohn 길드원 랭크 갱신시 팅기는 문제 해결
	void	IMGuildChangeFameRank(MSG_IC_GUILD_CHANGE_FAME_RANK* pMsg);

	// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
	VOID	IMChatFriendListInsertNotify(MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY* pMsg);
	// end 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가

	// 2007-04-19 by bhsohn 서치아이 아이템 추가
	// 서치 아이 이펙트 사용
	VOID	FieldSocketCharacterMapEffectOk(MSG_FC_CHARACTER_SHOW_MAP_EFFECT_OK* pMsg);

	// 2007.04.24 by bhsohn China IME Working
	BOOL	RenderReadingText(int x, int y);

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	VOID ShowOpMissionComplete(MSG_FC_QUEST_REQUEST_SUCCESS_RESULT* pMsg);	

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	void	CompleteNarrationSound(char* pNarrationMusic);	

	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	VOID	RequestHappyHourInfo();
	// 2007-11-28 by dgwoo 최초 접속시 받을 메시지가 있는지 서버로 요청.
	VOID	RequestNotifyMsg();

	// 2007-11-05 by bhsohn 몬스터 오브젝트 체크섬 처리
	BOOL	IsMeshMonsterObject(int nMeshIndex);

	// 2007-12-21 by bhsohn 프로그램 다중 락 구조 생성
	void	InitMultiLockInfo();

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	BOOL	ForceMousePos(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void	ShowPartyChangeInfo(SPARTY_INFO* pNewSetupInfo);

	// 2008-12-30 by bhsohn 전쟁보상추가 기획안
	VOID	CreateChatWarContribution(BYTE i_ItemUpdateType, int i_nOrgItemCount, int i_nNewItemCount);

private:
	void	RenderDbg();

	// 2004-12-13 by jschoi - Fade-In & Out 시스템
	void	FadeEffectTick();
	void	FadeEffectRender();

	// 2005-03-03 by jschoi - RequestEnable 
	void	RenderRequestEnable();
	void	RequestEnableTick(float fElapsedTime);

	// 2005-03-17 by jschoi - Error 처리
	BOOL	FieldSocketErrorByMsgType(MSG_ERROR* pMsg);
	BOOL	FieldSocketErrorByErrorCode(MSG_ERROR* pMsg);
	BOOL	IMSocketErrorByMsgType(MSG_ERROR* pMsg);
	BOOL	IMSocketErrorByErrorCode(MSG_ERROR* pMsg);

	/*--------------------------------------------------------------------------*/
	// 2006-06-05 by ispark, 핵 쉴드 본 함수
	BOOL	HS_Init();
	BOOL	HS_UnInit();
	BOOL	HS_StartService();
	BOOL	HS_StopService();
	/*--------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------*/
	// 핵 쉴드 관련 함수
	BOOL	HS_SaveFuncAdress();

	// 2008-04-04 by bhsohn 핵쉴드 방식 변경
	//void	HS_MakeGuidAckMsg(unsigned char *pbyReqMsg, unsigned char *pbyAckMsg);
	//void	HS_MakeCRCAckMsg(unsigned char *pbyReqMsg, unsigned char *pbyAckMsg);
	// end 2008-04-04 by bhsohn 핵쉴드 방식 변경

	// 2009-01-28 by bhsohn nProtector 게임 가드 추가
	//void	HS_SendHackShieldErrorMsg(long lHackClientCode, char * pMsg = NULL);
	/*--------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------*/
	// 2006-06-15 by ispark, 일정 시간에 패킷 보내는 함수
	void	SendOtherPacket();
	/*--------------------------------------------------------------------------*/
	
	// 2006-11-16 by ispark, 알파 렌더링
	void	RenderAlpha();

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	void	AddMultiItemList(deque<stMultiSelectItem>* pVector, const stMultiSelectItem* pSelItem, BOOL bfrontAdd);	
	void	DelMultiItem(deque<stMultiSelectItem>* pVector, INVEN_DISPLAY_INFO *pDisplayInfo);
	void	DelCityStoreMultiItem(UID64_t	 nUniqueNumber);	// 창고에서 선택 취소
	void	SellMultiResponseOK();
	void	SellMutltiOneItem();
	void	Convert_DWORD_To_String(DWORD dwNum, char* pDstSpi);
	void	ToStoreMultiResponseOK();
	void	UpdateMutltiItemError();
	void	UpdateMultiResponseOK();
	void	ToStoreMultiItemError();
	void	SellMutltiItemError();	
	void	FromStoreMultiResponseOK();
	void	FromStoreMultiItemError();
	BOOL	IsCheckCityStoreFull();
	BOOL	IsCheckInvenFull();
	BOOL	IsCheckSPIEmpty();
	BOOL	IsCheckGuildStore();

	// 2007-02-28 by bhsohn 다중 판매시 0 spi에 대한 처리
	void	UpdateMultiSellZeroSPIItem(UID64_t nUniqueNumber);

	// 2007-03-09 by bhsohn 다중 선택 창고 부분 보안
	BOOL	IsStoreHave(int nItemNum);
	BOOL	IsInvenHave(int nItemNum);

	// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
	void	UpdateEnemySkillState(bool bUseSkill, CUnitData* pUnit, ClientIndex_t	ClientIndex, INT ItemNum);

	// 2007-07-09 by bhsohn 바자 상점과 상점 동시 오픈시에 대한 처리
	VOID	CancelBazzarSkill(int nSkillItemNum);

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	void	PlayNarrationSound();

	// 2007-08-03 by bhsohn 캐릭터 오브젝트 체크썸 보냄
	void	ResetCharecterOmiInfo();
	
	// 2007-08-07 by bhsohn 스피커 아이템 추가
	VOID	AddSpeakerGMString(char* pGmChat, BOOL bGmColor);

	// 2007-08-23 by bhsohn 세력전시 기여도가 큰 유저에 대한 메시지 처리
	VOID	AlertMsgWarMonsterInfo(MEX_MONSTER_INFO* pMonsterInfo, char* pBestGuildName);

	// 2007-09-05 by bhsohn China IME Working
	void	TickImeChange();

	// 2008-01-11 by bhsohn 1분동안 강제로 클라이언트 멈추게 하면 클라이언트 강제 종료
	BOOL	IsCompultionGame(DWORD dwCapTime);		// 강제 접속 종료 여부 판단
	// 2007-11-22 by bhsohn 아레나 통합서버
	CFieldWinSocket* GetFieldWinSocket(UINT nSocketNotifyType);		// 현재 상태에 맞는 필드 소켓
	CIMSocket* GetIMWinSocket(UINT nSocketNotifyType);				// 현재 상태에 맞는 IM소켓
	void DoModalAreanQuestion();
	void ReCreateAllGameObject();										// 게임 오브젝트를 다 지운후 다시 생성
	void SetMyShuttleInfo(CHARACTER* pCharacter);		// 내기체의 클라이언트 인덱스 값 변경
	void SendArenaIMLogin();

	// 2008-03-03 by bhsohn 통합 아레나 IM서버 연결과정 수정
	void ArenaIMSocketConnectLoginOk();
	void IMSocketConnectFmToImOk();

	// 2008-05-28 by bhsohn Multi Lock끝나고 요청할 사항 있을시에 대한 처리
	void MultiLockFinished(MessageType_t	nPreLockMsg);

	// 2008-06-03 by bhsohn EP3 여단 관련 처리
	void IMSocketNoticeWriteOk(MSG_IC_GUILD_NOTICE_WRITE_OK* pMsg);		// 여단 공지 사항 등록
	// 여단 지원자 관리
	void IMSocketGetApplicantOkHeader();
	void IMSocketGetApplicantOKDone();
	void IMSocketGetApplicantOk(MSG_IC_GUILD_GET_APPLICANT_OK *pMsg);	
	void IMSocketGetIntroducitonOk(MSG_IC_GUILD_GET_INTRODUCTION_OK* pMsg);
	void IMSocketGuildSearchIntroOk(MSG_IC_GUILD_SEARCH_INTRODUCTION_OK* pMsg);
	void IMSocketGuildSearchIntroDone();
	void IMSocketGuildMemerLevelUp(MSG_IC_GUILD_MEMBER_LEVEL_UP* pMsg);

	// 2008-06-03 by bhsohn EP3 편대 관련 처리
	void IMSocketPartyListInfoOk(MSG_IC_PARTY_LIST_INFO_OK* pMsg); // 편대 리스트 
	void IMSocketPartyReCommandMemberOk(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK* pMsg) ;
	void IMSocketPartyChangeInfoOk();
	void IMSocketPartyInfo(MSG_IC_PARTY_INFO* pMsg);
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void IMSocketPartyGetAutoInfoOK( MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK* pMsg );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2008-08-11 by bhsohn 단독으로 실행 되게끔
	BOOL SetTempCharcaterInfo();

	// 2008-08-18 by bhsohn 1초 간격 아이템 이동 시스템 
	void TickItemSell();
	void IntervalSellMutltiOneItem();	// 간격을 주고 판매


	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	void TickInfinityStart( void );
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)


	// 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)
	void	TickInfintyMapLoadedSend( void );
	// end 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)


	// 2008-11-13 by bhsohn 조이스틱 작업
	void TickJoyStickInfo();

	// 2009-04-21 by bhsohn 인벤에 아이템 이벤트 지급시 메시지창 안찍히는 문제해결
	void TradeUpdateItemAddChat(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	// end 2009-04-21 by bhsohn 인벤에 아이템 이벤트 지급시 메시지창 안찍히는 문제해결

public:

	void	StartFadeEffect(BOOL bFadeIn,int nTime,D3DCOLOR dwColor);
	void	EndFadeEffect();
	float	GetFadeAlpha();
	void	SetWeb(BOOL bWeb) { m_bWeb = bWeb; }
	BOOL	GetWeb() { return m_bWeb; }
//	BOOL	GetCashShopFlag(){return m_bCashShopFlag;}
	BOOL	GetTestServerFlag() { return m_bTestServer; }	// TRUE : 테스트 서버임.
	// 2012-11-28 by jhjang 테섭 접속 여부 처리 추가
	BOOL	GetTestServerConnect() { return m_bIsTestServerConnect; }
	void 	SetTestServerConnect(BOOL bIsTestServerConnect) { m_bIsTestServerConnect = bIsTestServerConnect; }
	// end 2012-11-28 by jhjang 테섭 접속 여부 처리 추가

	// 2012-11-28 by jhjang 테섭 접속 여부 처리 추가
	BYTE	GetDBSid()				{	return m_nDBSid;	}
	void	SetDBSid(BYTE nDBSid)	{	m_nDBSid = nDBSid;	}
	// end 2012-11-28 by jhjang 테섭 접속 여부 처리 추가
	void	RenderCandidate(int x, int y);
	int		SerchMissionCondition(int nNewItemCount = 0);
	void	SetHanFontLang(LPARAM lParam);
	char*	GetFontStyle(){ return m_strFont;}
	void	SendMissionComplete(int nPartyMissionIndex = 0);// 2006-03-28 by ispark, 파티미션일 경우 인덱스를 넘겨 받는다.

	int		GetUnitArmorColorNum(){return m_nArmorNum;}
	int		GetUnitArmorColorMax(){return m_nArmorNumMax;}
	void	SetUnitArmorColorNum(int nNum){ m_nArmorNum = nNum;}
	void	SetUnitArmorColorMax(int nNum){ m_nArmorNumMax = nNum;}

	// 2005-03-18 by jschoi
	void	SetPremiumCardInfo(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* sMsg);
	MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* GetPrimiumCardInfo() { return &m_PremiumCardInfo; }

	// 2005-12-21 by ispark
	void	SendHackTime_WeaponInfo();
	void	SendHackTime_TotalWeight();
	void	SendHackTime_EngineInfo();
	void	SendHackTime_MemoryCheck(float ValidDistance, float CurrentDistance, float fElapsedTime);

	// 2006-04-13 by ispark
	void	CheckReadyStringFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes);

	
	// 2006-06-05 by ispark, 핵 쉴드
	BOOL	HS_Start();
	void	HS_Close();
	int	__stdcall HS_CallbackProc ( long lCode, long lParamSize, void* pParam );

	// 2008-11-28 by bhsohn XignCode추가
	BOOL StartGameGuard();
	BOOL CloseGameGuard();
	// end 2008-11-28 by bhsohn XignCode추가

	// 2006-06-15 by ispark
//	template<class VECTYPE> void	SetIntDataByVector(vector<VECTYPE> &vecInfo, int nIndex);
//	template<class VECTYPE> void	SetClientIndexDataByVector(vector<VECTYPE> &vecInfo, ClientIndex_t nClientIndex);

	// 2006-09-27 by ispark
	void LostDeviceTick();

	void SetEffectUseItem(ITEM* pItemInfo);
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void SetPlayEffect(INT nEffectNum,INT nSoundNum);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2007-02-12 by bhsohn 포커스 잃어 먹었을 떄의 문제 처리
	void UpdateCheckTime();
	FLOAT GetCheckElapsedTime();

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	void	StartSellMultiItem();	
	BOOL	SetMultiSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//BOOL	SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType);
	BOOL	SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType, INVEN_DISPLAY_INFO  *pDisSelectItem);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	void	OnCtrlBtnClick(BOOL bCtrlBtnClick);
	BOOL	GetCtrlBtnClick();	
	void	StartToStoreMultiItem();
	void	ToStoreMultiOneItem();
	void	StartFromStoreMultiItem();
	void	FromStoreMultiOneItem();

	// 2007-05-21 by bhsohn 아레나 맵에선 미션창 클릭 안되게 변경
	MAP_INFO* GetMyShuttleMapInfo();
	BOOL	IsWarMotherShip();

	// 2007-06-18 by bhsohn 알트텝 관련 버그 처리
	void	SetKeyBoardLostDevice(BOOL bLostDevice);

	// 2007-11-27 by bhsohn 테스트 서버 디파인
	BOOL IsTestServerDefine() { return m_bTestServerDefine; }
// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
	void	ConnectLoginIMServer(char* pUserId,char* pCharacterName, char* pUserPassWord);
	void	SendGameStart(ClientIndex_t	ClientIndex, UID32_t CharacterUniqueNumber);
	// 2007-11-22 by bhsohn 아레나 통합서버
	void	InitOnlyGameStartOk(AVECTOR3 vPos, BitFlag16_t MapWeather, BOOL bArenaRestart);
	void	UpdateGameStartMapInfo();

	// 2007-12-17 by bhsohn 아이템 정보가 다 안오면 게임 대기상태로
	BOOL	IsLockMode();

	// 2008-08-11 by bhsohn 단독으로 실행 되게끔
	void	InitItemList();
	void	AddItemList(MSG_FC_TRADE_INSERT_ITEM* pMsg);
	void	SaveItemList();
	void	SetUnitDetailInfo(CHARACTER* pCharac);
	void	SaveUnitDetailInfo();
	void	SetLoginOkInfo(MSG_FC_CONNECT_LOGIN_OK* pLoginInfo);
	void	SaveLoginOkInfo();
	void	SetGameStartOkInfo(MSG_FC_CHARACTER_GAMESTART_OK* pGamestartInfo);
	void	SaveGameStartOkInfo();
	void	SaveSinglePlayInfo();

	// 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화
	DWORD	GetCurrentGameTime();
	// end 2009-03-16 by bhsohn A기어 포대 방향 프로토콜 최소화

	// 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
	void	StartApexClient( void );
	// end 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	void	ChangeMapObject( ObjectIdx_t nOrigObjIndex, ObjectNum_t nChangeObjIndex );
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
	void SetPlaySearchEyeEffect(CUnitData * pUnit, INT nEffectNum, INT nSoundNum, bool bMoveCheck = true);
	void ClearPlaySearchEyeEffect(CUnitData * pUnit, INT nEffectNum);
	// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정

	// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	int AddINFCnt();	//인터페이스 창 중복 누적수를 더하고, 최종값을 계산하여 리턴
	int RemoveINFCnt();	//인터페이스 창 중복 누적수를 빼고, 최종값을 계산하여 리턴
	int GetINFCnt();	//인터페이스 창 중복 누적수를 리턴
	void SetINFCnt(int nInfCnt);
	// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정

	// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
	void SetItemNum(INT nItemNum);
	// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정

	// 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
	void SetClientLogOff(BOOL bClientLogOff) {m_bClientLogOff = bClientLogOff;}
	BOOL GetClientLogOff() {return m_bClientLogOff;}
	// END 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결

	// 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정
	DWORD GetGameCurrentTime() {return m_dwCurrentTime;}
	// END 2013-03-25 by bhsohn 파트너 키트 자동 사용 소켓 관련 버그 수정

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES*	GetInflConVictorites() {return &m_structInflConVictorites;}
	void	SetInflConVictorites(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES* pMsg) {memcpy(&m_structInflConVictorites, pMsg, sizeof(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES));}			
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	float GetPVPBuffPercent(){return m_fPVPBuffPercent;}		// 2013-08-02 by ssjung 역전의 버프 툴팁표시

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	void LookChangeLimitTimeMSG();	// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
#endif
	BOOL GetInitFlag()	{return m_bInitFlag;}									// 2013-07-02 유저가 처음 로딩 시 거부자 채팅 보이는 문제 수정
	void SetInitFlag(BOOL bFlag)	{m_bInitFlag = bFlag;}						// 2013-07-02 유저가 처음 로딩 시 거부자 채팅 보이는 문제 수정

	// 2015-09-26 Future, global method to end the game properly (logOff = whether its a log off (true) or game quit (false))
	void EndGame(BOOL logOff);
	void SendFieldSocketCharacterGameEnd(BOOL logOff);

public:
	// 2004-11-11 by jschoi 스피드핵 관련 변수
	int							m_nSendMovePacketCount;	// MovePacket을 보낸 수
	int							m_nCheckSpeedHackCount;	// 스피드핵 체크 카운트 1초당 5개
	DWORD						m_dwCheckSpeedHackTime;		// 스피드핵 체크 타임 timeGetTime()
	DWORD						m_dwSpeedHackTimer;
	DWORD						m_dwHackCheckTime;		// 2005-12-21 by ispark, 핵 체크 타임(1분당)
	DWORD						m_dwMoveCheckTime;		// 2006-01-10 by ispark, 무브 체크 타임(1분당)
	CD3DHanFont		*			m_pFontDebug;				// 텍스트 표시용 폰트
	CD3DHanFont		*			m_pFontCandidate[CHAR_INF_CANDIDATE];				// 한폰트
	// 2007.04.24 by bhsohn China IME Working
	CD3DHanFont		*			m_pFontCompositon;
	CD3DHanFont		*			m_pFontIMEType;

	CKeyBoardInput	*			m_pKeyBoard;				// KeyBoard 제어
	CCamera			*			m_pCamera;					// Camera 제어

	// 2008-11-13 by bhsohn 조이스틱 작업
	CJoyStickInput*				m_pJoyStick;				// 조이스틱 제어
	CJoyStickFreedback*			m_pJoyStickFreedback;				// 진동 조이스틱 제어	
	// end 2008-11-13 by bhsohn 조이스틱 작업

	CUnitRender		*			m_pUnitRender;
	CCharacterRender*			m_pCharacterRender;			// 2005-07-13 by ispark 캐릭터 렌더링
	CShuttleChild	*			m_pShuttleChild;			// 셔틀 Data
	CCharacterChild *			m_pCharacterChild;			// 캐릭터 Data// 2005-07-21 by ispark

	DWORD						m_dwStateBlock;				// 
	DWORD						m_dwGameState;				
	LPDIRECT3DVERTEXBUFFER9		m_pVBSleep;
	BOOL						m_bBazaar;					// 바자맵
	// 2012-07-17 by jhahn, 셧다운 시간 표시
	BOOL						FristSeting;
	int							nCount;
	int							nOldTime;	
	int							nNewTime;
	//end 2012-07-17 by jhahn, 셧다운 시간 표시

	CD3DHanFont		*			m_pFontInput;
	IMEKey						m_inputkey;
	CChat			*			m_pChat;
	BOOL						m_bChatMode;
	BOOL						m_bFixUnitDirMode;				// 유닛 진행방향 고정 모드
	BOOL						m_bMessageBox;					// 메시지 박스가 화면상에 존재하는가?
	char						m_strChat[SIZE_MAX_CHAT_MESSAGE];

	// 2007-11-22 by bhsohn 아레나 통합서버
	//CFieldWinSocket *			m_pFieldWinSocket;				// 필드 서버
	CFieldWinSocketManager *			m_pFieldWinSocket;				// 필드 서버

	BOOL						m_bReconnectFieldServer;		// 다른 IP 서버로의 재접속
	
	// 2007-11-22 by bhsohn 아레나 통합서버
	//CIMSocket		*			m_pIMSocket;					// 채팅 서버
	CIMSocketManager		*			m_pIMSocket;			// 채팅 서버
	

	char						m_strUserID[SIZE_MAX_ACCOUNT_NAME];
	char						m_strUserPassword[SIZE_MAX_PASSWORD_MD5_STRING];
	UINT						m_accountUniqueNumber;
	char						m_strFieldIP[SIZE_MAX_IPADDRESS];
	char						m_strFieldIPCurrent[SIZE_MAX_IPADDRESS];
	INT							m_nFieldPort;
	char						m_strChatIP[SIZE_MAX_IPADDRESS];
	INT							m_nChatPort;

	BOOL						m_bOtherFieldWarping;
	USHORT						m_sWarpAreaIndex;

	INT							m_nInvalidClientIndexCont;		// 타캐릭터 리스트중 인덱스가 0 인 리스트 수
	///////////// Thread ////////////////
	CMapInitThread	*			m_pMapInitThread;				// 맵 로딩
	CRITICAL_SECTION			m_cs;							// 동기화 객체
	///////////// Tile Texture Buffer /////////////
	CGameData		*			m_pData;						// 데이타파일 (맵타일파일)

	D3DXVECTOR3					m_vClientOldPos;				// Move 정보 보낼시에 움직인 거리를 체크하기위해 이전 셔틀좌표 저장
	///////////// Render 참조 /////////////
	INT							m_nCheckRenderPos;				// 렌더링 검사를 해야할 부분을 체크하는 변수
	FLOAT						m_fRenderDeleteCheckTime;		// 사용하지 않은 리소스를 지울 시간 체크 
	BYTE						m_nItemStorage;					//아이템창, 창고 구분
	// VoIP	//////////////////////////////////////////////////////////////////
	VOIP_STATE					m_VOIPState;
	/////////////////////////////////////////////////////////////////////////
	// Effect
	CEffectRender	*			m_pEffectRender;//by dhkwon	2003.5.13
	/////////////////////////////////////////////////////////////////////////
	FLOAT						m_fCheckSocketTime;				// Socket Check Time
	FLOAT						m_fOldTickTime;					// 지난 ElapsedTime
	FLOAT						m_fReturnKeyTime;				// Return Key Check Time

	CAtumNode		*			m_pEffectList;

	/////////////////////////////////////////////////////////////////////////
	BOOL						m_bDebugText;
	/////////////////////////////////////////////////////////////////////////
	BOOL						m_bClientQuit;
	// 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결
	BOOL						m_bClientLogOff;
	// END 2012-12-21 by bhsohn 베트남 게임 종료 안되는 문제 해결

	BOOL						m_bResCheck;
	BOOL						m_bReadyBeginScene;

	BOOL						m_bDeleteItemSend;			// Item 을 지우라고 서버에 전송

	CInterface				*	m_pInterface;
	BOOL						m_IsFullMode;					// 창모드인지
	CAbuseFilter	*			m_pAbuseFilter;
	CGameData		*			m_pAbuseFilterData;

	FLOAT						m_fBattleCountCheckTime;	// 파티 대결시 카운트 다운 들어가기 위한 변수
	INT							m_nBattlePartyCount;
	INT							m_bDegree;					// 0 저사양, 1 중사양 , 2 고사양, 3 충돌처리(유닛)
	INT							m_bMovePlayer;				// 동영상 플레이 제한

	char						m_strDebug[512];			// 윤원씨 디버그용 에러 메세지
	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//LPD3DXSPRITE				m_pd3dxSprite;
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	FLOAT						m_fUserCheckTime;
	char						m_strSeverUserNum[128];
	char						m_strMapUserNum[128];

	// 2012-11-07 by bhsohn 베트남 패킷 처리
	FLOAT						m_fServerLiveTime;
	// END 2012-11-07 by bhsohn 베트남 패킷 처리

	// remodeling, 2004-03-19 by dhkwon
	CAtumSound					*m_pSound;
	CSceneData					*m_pScene;
	CAtumDatabase				*m_pDatabase;

	BOOL						m_bReadyToGetGuildInfo;

	// 2007-01-07 by bhsohn 길드 정보 동기화 문제 관련 처리
	BOOL						m_bReadyFieldGameStartOk;

	CINFStageEffect	*			m_pStageEffect;
	CQuestData		*			m_pQuestData;
	CFxSystem *					m_pFxSystem;	
	BOOL						m_bWeb;
	BOOL						m_bRequestEnable;		// 서버로 요청 할 수 있는 상태인가?
	BOOL						m_bInputItemDone;		// 2007-07-16 by dgwoo 처음 진입시 모든 아이템을 받았는가?
	float						m_fRequestEnableTime;	// 메시지 애니메이션을 위한 시간 변수
	DWORD						m_dwTargetting;			// 2005-03-25 by jschoi - 타겟팅 관련 
	CTutorialSystem*			m_pTutorial;			// 튜토리얼 시스템
	CInputManager*				m_pInputManager;
	
	// 캐릭터
	BOOL						m_bCharacter;			// TRUE : 캐릭터모드, FLASE: 기어모드

	// 2005-08-10 by ispark
	// 그림자 렌더링 장치에 관한 설정 체크 (그래픽 카드에서 지원에 의한 문제)
	int							m_nDephbias;

	// 아머 칼러를 렌더링하기 위한 변수지정
	int							m_nArmorNum;		// 나의 셔틀 칼라
	int							m_nArmorNumMax;		// 아머가 가질수 있는 최대 칼라수
	
	D3DXVECTOR3					m_vShuttleOldPos;		// 전 좌표

	// 2010. 10. 05 by jskim 맵로딩 구조 변경
	CMapLoad*					m_pMaploading;
	// end 2010. 10. 05 by jskim 맵로딩 구조 변경
#ifdef _DEBUG
	// 이벤트 오브젝트 렌더
	BOOL						m_bEventObjectRender;	// TURE : 가능, FALSE : 불가능
#endif
	
	// IME관련 후보리스트 저장
	vector<string>	m_vecStingCandidate;
	int				m_nGlabalImageCase;
	char			m_strFont[64];

	/*--------------------------------------------------------------------------*/
	// 데이터를 일정 시간에만 보내기 위한 변수
	vectorClientIndex			m_vecCharacterOtherInfo;
	vectorClientIndex			m_vecCharacterOtherInfoRequest;
	vectorClientIndex			m_vecCharacterOtherMove;
	vectorClientIndex			m_vecCharacterOtherRenderInfo;
	vectorClientIndex			m_vecCharacterOtherRenderInfoRequest;
	vectorClientIndex			m_vecMoveTarget;

	vector<INT>					m_vecMapObjectInfo;
	vector<INT>					m_vecMonsterInfo;
	vector<INT>					m_vecSimpleItemInfo;
	vector<INT>					m_vecItemInfo;
	/*--------------------------------------------------------------------------*/

	/*--------------------------------------------------------------------------*/
	// 2006-07-05 by ispark, 강제 종료시 처리하는 부분
	BOOL						m_bShutDown;				// 강제 종료?
	// 2007-07-10 by bhsohn 통신 타입 에러에 대한 처리
	//char						m_strMsgLastError[SIZE_MAX_CHAT_MESSAGE];
	char						m_strMsgLastError[SIZE_MAX_ERROR_CHAT_MESSAGE];

	BOOL						m_bGameShutDown;		// 2006-08-04 by ispark, 게임 강제 종료
	float						m_fGameShutDownTime;	// 2006-08-04 by ispark, 게임 강제 종료 시간
	/*--------------------------------------------------------------------------*/
	
	BOOL						m_bCheckQuickSlotSave;

	// 2007-07-27 by dgwoo 세력 선택을 한 유저인가?
	BOOL						m_bInfregular;	

	vector<MSG_FC_TRADE_INSERT_ITEM>			m_vecTutorialInsertItem;
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>     m_vecTutorialUpdateItem;

	vector<CItemInfo*>							m_vecWearDisplayInfo;		// 2012-06-14 by isshin 아템미리보기

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	CInfinityManager*			m_pInfinityManager;		// 인피니티 UI관련 정보

	typedef enum ARENASOCKET_TYPE { ARENASOCKET_ARENA, ARENASOCKET_INFINITY, ARENASOCKET_NONE };

	ARENASOCKET_TYPE			m_ArenaSocketType;		// 아레나 소켓 타입

	BOOL						m_bInfinityMapload;		// 인피니티 맵 로딩 중인가?(인피니티 맵 로딩은 서버의 승인이 올때까지 대기한다)

	BOOL						m_bEnterInfinityAllowed;// 인피니티 맵에 입장 허가(모든 파티원들이 맵로딩이 다 종료됬을때 TRUE)

	BOOL						m_bInfinityRestart;		// 인피니티 튕겼을때 재입장인가?
	BOOL						m_bAddInfinityRestartMessageBox;	// 인피니티 재입장 메세지 박스를 띄울것인가?
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


	// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
	BOOL						m_bWaitInfinityMapLoad;	// 인피니티 맵 로딩을 기다리는 중인가?

	DWORD						m_nInfinityMapLoadTime;	// 인피니티 맵 로딩을 시작할 시간

	MSG_FC_ARENA_POSSIBLE_GAMESTART	m_ArenaStartPacket;	// 인피니티 맵 로딩에 사용할 패킷 저장
	// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)


	// 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)
	BOOL						m_bInfinityMapLoadedSendEnable;
	BOOL						m_bInfinitySkillSetupOKDone;
	// end 2010. 03. 23 by ckPark 인피니티 필드 2차(인피니티 필드 입장 캐쉬아이템)


	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	DWORD							m_dwTimeCheck;	
	vector<structLoadingGameData*>	m_vecLoadingGameData;
	CMeshInitThread*				m_pMeshInitThread;
	structLoadingGameData*			m_pLoadingGameData;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

	// 2011. 10. 10 by jskim UI시스템 변경
	CINFImageList*				m_pImageList;
	// end 2011. 10. 10 by jskim UI시스템 변경

	// 2011. 1. 18 by jskim 배트남 VTC가드
	HINSTANCE m_hGuardLibrary;
	func1	m_fnVTCDLLInit;
	func2	m_fnVTCDLLGetStatus;
	func3	m_fnVTCDLLSetUser;
	func4	m_fnVTCDLLExit;

	CVTCThread* m_pVTCGuard;
	// end 2011. 1. 18 by jskim 배트남 VTC가드

	bool				b_CanadaMutex;// 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전

#ifdef _WIREFRAME
	BOOL		m_bWireframe; //2014-09-03 St0rmy wireframe
#endif
#ifdef _NOCLIP
	BOOL		m_bNoclip; //2014-09-03 St0rmy NoClip
#endif
#ifdef _DRAW_EVENTS
	BOOL		m_bDrawEvents; //2015-01-29 by St0rmy, Draw Events
#endif
#ifdef _SHOW_LATENCY
	FLOAT		m_fLatency;
#endif

private:
//	BOOL						m_bCashShopFlag;		// 캐쉬샵 플레그
	BOOL						m_bTestServer;			// 테스트 서버 플래그
	BOOL						m_bIsTestServerConnect;	// 2012-11-28 by jhjang 테섭 접속 여부 처리 추가
	BYTE						m_nDBSid;
	BOOL						m_bDeadByP2PPK;			// PK시 적 유닛에게 죽었나? TRUE : 적 유닛에게 죽었다.
	
	MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO		m_PremiumCardInfo;	

	// 2007-02-12 by bhsohn 포커스 잃어 먹었을 떄의 문제 처리	
	FLOAT						m_fCheckElapsedTime;	
	DWORD						m_dwCurrentTime;

	// 2007-02-12 by bhsohn Item 다중 선택 처리
	// 판매 벡터
	deque<stMultiSelectItem>	m_vecSellMultiSelectItem;			
	deque<stMultiSelectItem>	m_vecInvenStoreMultiSelectItem;	
	deque<stMultiSelectItem>	m_vecCityStoreMultiSelectItem;	
	BOOL						m_bCtrlBtnClick;
	BOOL						m_bStartMultiSell;
	BOOL						m_bStartMultiStore;

	// 2007-02-28 by bhsohn 다중 판매시 0 spi에 대한 처리
	stMultiSelectItem			m_struLastSelMultiItem;
	
	// 2007-04-09 by bhsohn 관리자 계정 표출 정보 처리
	char						m_strConnectSeverName[1024];	

	// 2007.04.24 by bhsohn China IME Working
	char						m_szReadingString[1024];	
	char						m_szImeString[1024];	

	// 2007-05-21 by bhsohn China IME Working
	FLOAT						m_fIMETypeShowTime;			
	vector<MSG_FC_ARENA_GM_COMMAND_INFO> m_vecTeamInfo;			// GM명령어 /아레나.

	// 2007-07-10 by bhsohn 통신 타입 에러에 대한 처리
	int							m_nPreComType;
	int							m_nPreIMComType;

	// 2007-07-24 by bhsohn 나레이션 mp3추가
	char				m_chNarrationSound[256];
	// 2009. 01. 22 by ckPark 각 세력별 오퍼레이터 MP3 분리
	char				m_chErNarrationSound[256];
	// end 2009. 01. 22 by ckPark 각 세력별 오퍼레이터 MP3 분리
	
	// 2007-08-03 by bhsohn 캐릭터 오브젝트 체크썸 보냄
	vector<int>					m_vecSendCharacterOmiInfo;

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	BYTE			m_bIsUseSecondaryPasswordSystem;	// 시스템을 사용할지 여부
	BYTE			m_bIsSetSecondaryPassword;		// 암호를 설정했는지 여부

	// 2007-10-30 by dgwoo 아이템 목록이 올경우는 마지막에 한번만 소팅해준다.
	BOOL			m_bResortingItem;

	// 2007-11-27 by bhsohn 테스트 서버 디파인
	BOOL			m_bTestServerDefine;

	// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
	stGameStartInfo		m_struGameStartInfo;
	BOOL				m_bGameStartInfo;	
	// 2007-11-22 by bhsohn 아레나 통합서버
	CArenaManager*		m_pArenaManager;
	
	// 2007-12-17 by bhsohn 아이템 정보가 다 안오면 게임 대기상태로
	

	// 2007-12-21 by bhsohn 프로그램 다중 락 구조 생성	
	stMultiLockInfo		m_vecLockProtocol;

	// 2008-01-11 by bhsohn 1분동안 강제로 클라이언트 멈추게 하면 클라이언트 강제 종료
	DWORD				m_dwLastestTickTime;		// Tick에 마지막으로 	



	// 2008-02-15 by bhsohn 네트워크 속도 체크
	structNetCheckInfo	m_stNetCheckInfo;

	// 2008-06-20 by bhsohn EP3 옵션관련 처리
	float				m_fMouseLockTime;
	BOOL				m_bWndMoving;
	int					m_nOnFriendCount;

	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	BYTE				m_bSecondPassLock;

	// 2008-08-11 by bhsohn 단독으로 실행 되게끔
	CSinglePlayGame*	m_pSinglePlayGame;	

	// 2008-08-18 by bhsohn 1초 간격 아이템 이동 시스템 	
	DWORD				m_dwSendTermTime;
	BOOL				m_bSendTerm;

	// 2008-11-13 by bhsohn 조이스틱 작업
	float				m_fJoyStickCheckTime;
	float				m_fJoyStickFeedbackPlayTime;	// 진동시간
	int					m_nJoyStickFeedbackType;		// 진동타입	
	int					m_nJoyContorlLen;
	char				m_chJoystickCtrlList[MAX_JOYCTL_LIST][MAX_PATH];
	// end 2008-11-13 by bhsohn 조이스틱 작업

	// 2009-01-28 by bhsohn nProtector 게임 가드 추가	
	#if defined(DEFINE_USE_GAMEGUARD_NPROTECTOR)	
		CNPGameLib *		m_pNpgl;
		float				m_fnProtectorHackCheckTime;
	#endif
	// end 2009-01-28 by bhsohn nProtector 게임 가드 추가
	// 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
	#if defined(DEFINE_USE_GAMEGUARD_XTRAP)	
		float				m_fXTrapHackCheckTime;
	#endif
	//end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)

	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	CWorldRankManager*		m_pWorldRankManager;
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)
	stMultiSelectItem		m_stMultiWarningSelItem;
	// end 2009-04-08 by bhsohn 다중 선택시, 유니크 및 인챈트 된 아이템 판매시 경고 메시지 출력 기획 문서(K0000174)

	// 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경
	BYTE					m_nOMICheckSum[32];
	long					m_nOMIFileSize;
	// end 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경


	// 2010. 03. 12 by ckPark Interface.tex 체크섬 추가
	BYTE					m_nInterfaceCheckSum[32];
	long					m_nInterfaceFileSize;
	// end 2010. 03. 12 by ckPark Interface.tex 체크섬 추가


	// 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업
	#if defined( DEFINE_USE_GAMEGUARD_APEX )
		_FUNC_C_REC				m_pApexfp_rec;
	#endif
	// end 2009. 11. 03 by ckPark 태국 APEX 게임가드 작업

	// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	int m_nINFCnt;				// 인터페이스창 중복 누적수
	// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정

	// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
	INT						m_nItemNum;
	// end 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정

	//2013-04-11 by ssjung 모선전 준비상태일 때 맵 이동시 다시 뮤직플레이어 되는 상황수정
	BOOL m_bBossMonsterSummeryReady;
	BYTE m_byInfluence;
	//end 2013-04-11 by ssjung 모선전 준비상태일 때 맵 이동시 다시 뮤직플레이어 되는 상황수정

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES		m_structInflConVictorites;
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	BOOL				m_bInitFlag;		// 2013-07-02 유저가 처음 로딩 시 거부자 채팅 보이는 문제 수정

	float				m_fServerLockTime;// 2013-07-05 by bhsohn 서버종료시, 경고음 시스템

	float				m_fPVPBuffPercent;	// 2013-08-02 by ssjung 역전의 버프 툴팁표시	

	ULONG_PTR m_GdiplusToken;						// 2015-07-18 Future, GDI+ for ScreenShot Conversions

	CScreenShotManager*	m_pScreenShotManager;	// 2015-07-18 Future, Adding Screenshot Manager

#ifdef C_DISCONNECT_INACTIVE_PLAYERS
	float	m_fLastActivity;	// 2015-09-25 Future, kicking of inactive players
#endif // C_DISCONNECT_INACTIVE_PLAYERS

#ifdef _ADVANCED_CONFIG
public:
	//St0rmy: Advanced Configuration Variables
	BYTE				m_nAdvConfRenderLevel; // 1, 2, 3, 4
	BYTE				m_nAdvConfAntiAliasing;// 0, 2, 4, 8
	BYTE				m_nAdvConfAntiAliasQuality;//1, 2, 3, 4 (Only on 4 samples): 5, 6, 7, 8
	BOOL				m_bAdvConfBufferOptimize; //TRUE, FALSE
	BYTE				m_nAdvConfTextureFilter;//1, 2, 3, 4 -> None, Point, Linear, Anisotropic
	BYTE				m_nAdvConfAnisotropicLevel;//1-16
	BOOL				m_bAdvConfVSync;//TRUE, FALSE
	BYTE				m_bAdvConfAffinity;//0, 1, 2
	void				ReadAdvancedConfig();
#endif
};

// 전역변수선언
extern CAtumApplication			*g_pD3dApp;
extern CAtumSJ					*g_pAtumSJ;
extern LPDIRECT3DDEVICE9		g_pD3dDev;	// 전역 디바이스 PTR
extern sOPTION_SYSTEM			*g_pSOption;
extern sOPTION_CHARACTER		*g_pSOptionCharacter;
extern int						g_nMoveCountPerRate;
extern float					g_fMoveCountFrame;
// 2008-06-20 by bhsohn EP3 옵션관련 처리
extern sOPTION_ETC				*g_pSOptionEtc;

// 2008-11-13 by bhsohn 조이스틱 작업	
extern structJoyStickOptionInfo*		g_pJoysticOp;	
// end 2008-11-13 by bhsohn 조이스틱 작업

extern CSceneData				*g_pScene;
extern CShuttleChild			*g_pShuttleChild;
extern CCharacterChild			*g_pCharacterChild;		// 2005-07-21 by ispark
extern CAtumDatabase			*g_pDatabase;
extern CInterface				*g_pInterface;
extern CINFGameMain				*g_pGameMain;
extern CStoreData				*g_pStoreData;
extern CQuestData				*g_pQuestData;
extern CCamera					*g_pCamera;
extern CBackground				*g_pGround;

// 2007-11-22 by bhsohn 아레나 통합서버
//extern CFieldWinSocket			*g_pFieldWinSocket;
//extern CIMSocket				*g_pIMSocket;
extern CFieldWinSocketManager	*g_pFieldWinSocket;
extern CIMSocketManager			*g_pIMSocket;

extern CINFSelect				*g_pSelect;
extern CINFChannelList			*g_pChannelList;
extern CFrustum					*g_pFrustum;
extern float					g_fSendMovePacket;
extern BOOL						g_bDetailDrawFrame;		// 세부 컬링 옵션
extern CTutorialSystem			*g_pTutorial;			// 튜토리얼
extern CInputManager			*g_pInputManager;
extern CInput					g_input;
extern int						g_nRenderCandidate;		// IME관련 후보리스트 출력 위치
extern CGlobalImage				*g_pGImage;
extern BOOL						g_bEventRecovery;		// 이벤트 회복율 관련 Flag
extern float					g_fEventRecoveryRate;	// 이벤트 회복율
extern ATUM_DATE_TIME			g_ServerDateTime;		// 2006-10-11 by ispark, 서버 시간
extern int						g_GameStartTime;		// 2006-10-11 by ispark, 게임 시작 시간
// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
//extern int						g_INFCnt;				// 인터페이스창 중복 누적수
// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
// 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
extern ATUM_DATE_TIME			g_ArenaServerDateTime;
extern int						g_ArenaStartTime;	
// end 2009. 09. 10 by jsKim 아레나 진입시 시작 카운터 버그 수정
#endif
