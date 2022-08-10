// CrashReporterDlg.h
//

#pragma once


#include <conio.h> 
#include <stdio.h>
#include <string>

#ifdef _USE_FTP
#include <XOR_Lib.h>
#define MEMSET_ZERO(p, Size) (memset((p), 0x00, (Size)))
#endif

#define STARTPRM_DMPFILE "dmpfile"
#define STARTPRM_ACCNAME "accountname"
#define STARTPRM_CHARNUMBER "charnumber"
#define STARTPRM_MAPNUMBER "mapnumber"
#define STARTPRM_EXCEPTIONCODE "code"

#ifdef _USE_FTP
#define XOR_KEY "J60!_J18WaW7%{!FW2}qs};H6WY3m?3zyw4RmWssK8Cn0Dse3g6'g2;*lm2#mEk"
#define FTP_HOST "2659534033225E4B23" //localhost
#define FTP_PORT 21
#define FTP_ACCOUNT "2E5B40742F265E5933" //dmpUpload
#define FTP_PASSWORD "2E5B40742F265E5933"	//dmpUpload
#define HOST_DMPFILE_PATH "/dmp/"
#endif
#ifdef _USE_HTTP
#define HTTP_HOST "localhost"
#define HTTP_PORT 80
#define HTTP_DIRECTORY "/dmp/upload.php"
#endif


#ifdef _DEBUG
#define LOG _cprintf
#else
#define LOG __noop
#endif

using namespace std;

class CCrashReporterDlg : public CDialogEx
{
public:
	CCrashReporterDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CRASHREPORTER_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	string m_szDmpFile;
	string m_szAccName;
	int m_iMapNumber;
	int m_iCharNumber;
	string m_szExceptionCode;


	BOOL CheckDmpFile(const std::string& name);
	void UploadFile(string szFileName);
	void UploadData();
	string GetCPUName();
	string GetGPUName();
	string GetOSVersion();
	float GetRAMSize();
};
