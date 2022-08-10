///////////////////////////////////
// 2011-12-12 by hskim, GLog 2Â÷

#ifndef _GLOG_EXEC_MANAGER_H_
#define _GLOG_EXEC_MANAGER_H_

#include "AtumParam.h"
#include "AtumThread.h"

class CGLogExecManager : public CAtumThread
{
public:
	CGLogExecManager();
	virtual ~CGLogExecManager();

public:
	void Clean();
	BOOL Init();

	DWORD ThreadWorker();

	void ExecCrocess(char *cmdLine);
};

#endif // _GLOG_EXEC_MANAGER_H_
