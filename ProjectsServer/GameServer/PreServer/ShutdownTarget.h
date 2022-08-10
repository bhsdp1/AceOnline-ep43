// ShutdownTarget.h: interface for the CShutdownTarget class.
// 2012-07-11 by hskim, 선택적 셧다운
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHUTDOWNTARGET_H__6A8B5066_ADAE_47C4_B373_BF042D5161C4__INCLUDED_)
#define AFX_SHUTDOWNTARGET_H__6A8B5066_ADAE_47C4_B373_BF042D5161C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelectiveShutdown;

class CShutdownTarget
{
public:
	CShutdownTarget();
	CShutdownTarget(char *i_pAccountName, char *i_pServiceBlock);
	virtual ~CShutdownTarget();

	void Clear();
	void SetTarget(char *i_pAccountName, char *i_pServiceBlock);
	void SetTarget(CShutdownTarget *pTarget);
	BOOL IsShutdown(int iHour);

private:
	char m_szAccountName[SIZE_MAX_ACCOUNT_NAME];
	BOOL m_bServiceBlock[SIZE_MAX_SHUTDOWN_SERVICE_BLOCK];

	friend CSelectiveShutdown;
};

#endif // !defined(AFX_SHUTDOWNTARGET_H__6A8B5066_ADAE_47C4_B373_BF042D5161C4__INCLUDED_)
