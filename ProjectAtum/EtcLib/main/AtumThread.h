// AtumThread.h: interface for the CAtumThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMTHREAD_H__049DFD4C_80D2_43EB_8441_2A80E08A39D0__INCLUDED_)
#define AFX_ATUMTHREAD_H__049DFD4C_80D2_43EB_8441_2A80E08A39D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD WINAPI ThreadProc(LPVOID pParam);

class CAtumThread  
{
public:
	CAtumThread();
	virtual ~CAtumThread();
	HANDLE CreateThread( DWORD dwCreationFlags = 0 );
	void Resume();
	void Suspend();
	void Priority(int pri);
	virtual DWORD Run();

	BOOL m_bRunning;
	BOOL m_bThreadMustStop;

protected:
	HANDLE m_hThread;
	DWORD m_dThreadID;

};

#endif // !defined(AFX_ATUMTHREAD_H__049DFD4C_80D2_43EB_8441_2A80E08A39D0__INCLUDED_)
