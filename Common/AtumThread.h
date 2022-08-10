#ifndef _ATUM_THREAD_H_
#define _ATUM_THREAD_H_

/******************************************************************************
	By Kelovon
	상속받아 사용하여야 하고, ThreadWorker()를 구현해서 사용해야 함.
	Thread를 시작하기 위해서는 InitThread()를 호출하여야 하고,
	다 사용한 이후에는 CleanThread()를 호출하여야 함.
******************************************************************************/
#include <WINDOWS.H>

// refer to the site: http://www.microsoft.com/msj/0799/Win32/Win320799.aspx
#ifndef chBEGINTHREADEX
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define chBEGINTHREADEX(psa, cbStack, pfnStartAddr, \
   pvParam, fdwCreate, pdwThreadID)                 \
      ((HANDLE) _beginthreadex(                     \
         (void *) (psa),                            \
         (unsigned) (cbStack),                      \
         (PTHREAD_START) (pfnStartAddr),            \
         (void *) (pvParam),                        \
         (unsigned) (fdwCreate),                    \
         (unsigned *) (pdwThreadID)))
#endif

class CAtumThread  
{
public:
	CAtumThread();
	virtual ~CAtumThread();

	virtual DWORD ThreadWorker() = 0;

	BOOL InitThread();
	BOOL CleanThread();

	void SetShutDownFlag() { m_bShutDownFlag = TRUE; }
	BOOL GetShutDownFlag() { return m_bShutDownFlag; }

	DWORD SleepAndWaitForEvent(DWORD dwMilliseconds)
	{
		return WaitForSingleObject(m_hAtumThreadEvent, dwMilliseconds);
	}

	// not used by user
	DWORD ThreadWorkerWrapper(); 

protected:
	HANDLE			m_hWorkerThread;
	DWORD			m_dwThreadID;

private:
	BOOL			m_bShutDownFlag;
	HANDLE			m_hAtumThreadEvent;
};

#endif // _ATUM_THREAD_H_
