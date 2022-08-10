// VoIPGlobal.h: interface for the CVoIPGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOIPGLOBAL_H__9634F468_5556_4123_8871_213E5FC927D3__INCLUDED_)
#define AFX_VOIPGLOBAL_H__9634F468_5556_4123_8871_213E5FC927D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "SocketHeader.h"


//////////////////////////////////////////////////////////////////////////
// Configuration
#define GLOBAL_CONFIG_FILE_NAME					"Global.cfg"
#define VD_SERVER_CONFIG_FILE_NAME				"VDServer.cfg"
#define VC_SERVER_CONFIG_FILE_NAME				"VCServer.cfg"

//////////////////////////////////////////////////////////////////////////
// Timer Variables
const UINT TIMERID_TRAFFIC						= 100;					// Traffic 모니터 Timer
const UINT TIMERID_ALIVE_CHECK					= 101;					// 서버에서 Alive Check Timer
const UINT TIMERID_RECONNECT					= 102;
const UINT TIMERID_SELFSTATUS					= 103;
const UINT TIMERGAP_TRAFFIC						= 10000;				// 10초(10 * 1000)
const UINT TIMERGAP_ALIVE_CHECK					= 30000;				// 30초(30 * 1000)
const UINT TIMERGAP_RECONNECT					= 5000;
const UINT TIMERGAP_SELFSTATUS					= 1000;

class CVoIPGlobal;
//////////////////////////////////////////////////////////////////////////
// Global Variable
//////////////////////////////////////////////////////////////////////////
extern CVoIPGlobal			*g_pVoIPGlobal;		// 전역변수관리를 위한 전역클래스포인터

//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// 윈도우 메세지 프로시저

class CIOCP;
class CVoIPGlobal : public CGlobal  
{
public:
	CVoIPGlobal();
	virtual ~CVoIPGlobal();

	void SetConfigRootPath(void);
	char *GetConfigRootPath(void);
	HINSTANCE GetInstanceHandle(void);
	HWND GetMainWndHandle(void);
	UINT GetTimerIDTraffic(void);
	UINT GetTimerIDAliveCheck(void);
	UINT GetTimerIDReconnect(void);

	void SetIPLocal(char *i_szIP);
	char *GetIPLocal(void);
	void SetPublicIPLocal(char *i_szIP);
	char *GetPublicIPLocal(void);
	void SetPortListening(int i_nPort);
	int GetPortListening(void);
	CIOCP *GetGIOCP(void);

	///////////////////////////////////////////////////////////////////////////
	// virtual Function	
	virtual BOOL InitServerSocket(void);						// 소켓관련 초기화 함수
	virtual BOOL EndServerSocket(void);							// 소켓관련 종료 함수
	virtual BOOL LoadConfiguration(void){	return FALSE;};		// Server 설정을 위한 Configuration Load


	///////////////////////////////////////////////////////////////////////////
	// Method
	void WndRegisterClass(HINSTANCE hInstance, UINT IDI_Icon, char *szClass);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, char *szClass, char *szWindowText);

	BOOL StartTimerTraffic(void);
	void EndTimerTraffic(void);
	BOOL StartTimerAliveCheck(void);
	void EndTimerAliveCheck(void);
	BOOL StartTimerReconnect(void);	
	void EndTimerReconnect(void);

	void CheckServerThread(void);
	void CalculateIOCPTraffic(void);
	void CheckClientAlive(void);

	
protected:
	//////////////////////////////////////////////////////////////////////////
	// 윈도우 관련 멤버 변수
	HINSTANCE			m_hInstanceHandle;						// 실행된 인스턴스 핸들
	HWND				m_hMainWndHandle;						// 실행된 메인 윈도우 핸들

	//////////////////////////////////////////////////////////////////////////
	// 윈도우 타이버 관련 멤버 변수
	UINT				m_nTimerIDTraffic;						// 모든 서버에서 Bandwidth 계산을 위한 TimerID
	UINT				m_nTimerIDAliveCheck;					// 모든 서버에서 클라이언트의 Alive 체크를 위한 TimerID
	UINT				m_nTimerIDReconnect;					// 서버(IMServer, FieldServer, NPCServer)에서 상위 서버와 접속이 종료 되었을때 재접속을 위한 TimerID
	
	//////////////////////////////////////////////////////////////////////////
	// Config 관련 멤버 변수
	char				m_szConfigRootPath[MAX_PATH];
	char				m_szIPLocal[SIZE_MAX_IPADDRESS];		//
	char				m_szPublicIPLocal[SIZE_MAX_IPADDRESS];			// All (Pre, IM, Field, NPC)// 2006-05-23 by cmkwon
	int					m_nPortListening;						//
	
	CIOCP *				m_pGIOCP;
};

#endif // !defined(AFX_VOIPGLOBAL_H__9634F468_5556_4123_8871_213E5FC927D3__INCLUDED_)
