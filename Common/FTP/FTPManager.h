#ifndef _FTP_MANAGER_H_
#define _FTP_MANAGER_H_

#include "Wininet.h"

// 2007-01-05 by cmkwon, ERR_XXX로 수정함
//typedef enum
//{
//	DOWNLOAD_ERR_FTP_CONNECT			= 0,
//	DOWNLOAD_ERR_FTP_OPENFILE			= 1,
//	DOWNLOAD_ERR_CREATE_LOCAL_FILE		= 10,
//	DOWNLOAD_ERR_READ_REMOTE_FILE		= 20,
//} FTP_DOWNLOAD_ERROR;
//
//#define STR_DOWNLOAD_ERR_FTPCONNECT					STRCMD_CS_COMMON_DOWNLOAD_0000
//#define STR_DOWNLOAD_ERR_CREATE_LOCAL_FILE			STRCMD_CS_COMMON_DOWNLOAD_0001
//#define STR_DOWNLOAD_ERR_READ_REMOTE_FILE			STRCMD_CS_COMMON_DOWNLOAD_0002

class CFTPManager  
{
public:
	CFTPManager();
	virtual ~CFTPManager();

	BOOL ConnectToServer(const char *i_pServerName, int i_nServerPort, const char *i_pUserName = NULL, const char *i_pPassword = NULL);
	BOOL ChangeCurrentDirectory(const char *i_pDirName);
	char* GetCurrentDirectory();
	HINTERNET GetFileInfo(const char *i_pRemoteFileName, WIN32_FIND_DATA *o_pRemoteFileInfo);
	int GetFileSize(const char *i_pRemoteFileName, HINTERNET &i_hFile);
	BOOL DownloadFile(const char *i_pRemoteFileName, const char *i_pLocalFileName = NULL, CProgressCtrl *i_pProgressControl = NULL, HWND i_hWnd = NULL);
	BOOL DownloadFileByThread(const char *i_pRemoteFileName, const char *i_pLocalFileName = NULL, HWND i_hWnd = NULL);
	BOOL CloseConnection();

private:
	HINTERNET	m_hInternet;
	HINTERNET	m_hFtpConnect;

public:
	HANDLE		m_hDownloadThread;
	BOOL		m_bDownloadThreadCancelFlag;
	char		m_szRemoteFileName[1024];
	char		m_szLocalFileName[1024];
	HWND		m_hUpdateWindow;
};

#endif // _FTP_MANAGER_H_
