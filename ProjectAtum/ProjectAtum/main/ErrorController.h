// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

#ifndef __ERROR_CONTROLLER_H__
#define __ERROR_CONTROLLER_H__

#include "NTMiniDump.h"
#include "NTDbgHelperWindow.h"
#include "NTSystemInfo.h"
#include "NTFtp.h"
#include "excpt.h"

#define MAX_CALLSTACK 20

#define g_cErrorController CErrorController::GetInstance()


const int N_BUF_SIZE = 1024;

class CErrorController
{
public:
	CErrorController();
	~CErrorController();

	static CErrorController& GetInstance()
	{
		static CErrorController Instance;
		return Instance;
	}

	LONG OnPlayMiniDump(_EXCEPTION_POINTERS *pException);
	void AddDebugData(const TCHAR* pszData)	{ m_szMyDebugData += pszData; }
	void SetSerialCode(int n)				{ m_nSerialCode = n; }
	void SetBuildCount(int n)				{ m_nBuildCount = n; }
	void SetUserName(const TCHAR *pszName)	{ m_pstrUserName = pszName; }
	void SetServerInfo(const TCHAR *pszServerIP, WORD dPort);
	void SetFtpInfo(const TCHAR *pszFTPIP, WORD dFTPPort, const TCHAR *pszFTPID, const TCHAR *pszFTPPassword);

	TCHAR		m_strOutInfo[N_BUF_SIZE]; // 크래쉬난 정보를 문자열로 가지기

private:
	void SendBaseExceptionInfo(const TCHAR* pszDumpCode);
	void SetCallStack(_EXCEPTION_POINTERS *pException);
	bool OnLoadDbgDllFile(void);

	// 크래쉬난 정보를 문자열로만 가지고 있기
	bool OnlyDumpString(_EXCEPTION_POINTERS *pException, CNTSystemInfo* pSystemInfo);

	CNTMiniDump			m_cMiniDump;
	CNTDbgHelperWindow	m_cDbgWindow;
	CNTSystemInfo		m_cSystemInfo;
	CNTFtp				m_cUploadFtp;
	bool				m_bUseServer;
	bool				m_bUseDumpServer;
	bool				m_bOnlyDumpString; // 크래쉬난 정보를 문자열로만 가지고 있기
	TString				m_strSysInfo;
	TString				m_szUserData;
	TString				m_szMyDebugData;

	TString				m_pstrFTPServerIP;
	TString				m_strFTPID;
	TString				m_strFTPPassword;
	WORD				m_dFTPPort;

	TString				m_pstrServerIP;
	TString				m_pstrUserName;
	WORD				m_dPort;

	DWORD				m_dwCallStack[MAX_CALLSTACK];

	HMODULE				m_hDllModule;
	int					m_nSerialCode;
	int					m_nBuildCount;
};

#endif//(__ERROR_CONTROLLER_H__)

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송