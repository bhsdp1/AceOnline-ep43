///////////////////////////////////
// 2011-12-12 by hskim, GLog 2Â÷

#include "stdafx.h"
#include "GLogExecManager.h"

CGLogExecManager::CGLogExecManager()
{
}

CGLogExecManager::~CGLogExecManager()
{
}

BOOL CGLogExecManager::Init()
{
	return InitThread();
}

void CGLogExecManager::Clean()
{
	CleanThread();
}

DWORD CGLogExecManager::ThreadWorker()
{
	while (!GetShutDownFlag())
	{
		ExecCrocess(GLOG_EXCUTE_FILE_NAME);	
		ExecCrocess(GLOG_EXCUTE_FILE_NAME_WATCHPROC);
		ExecCrocess(GLOG_EXCUTE_FILE_NAME_VISUALC);

		Sleep(10000);
	}

	return 5;
}

void CGLogExecManager::ExecCrocess(char *cmdLine)
{
	DWORD dwExitCode;
	PROCESS_INFORMATION pi;

	STARTUPINFO si = {sizeof(si)};
	ZeroMemory(&si,sizeof(si));

	CreateProcess(
		NULL,				// name of executable module
		cmdLine,			// command line string
		NULL,				//LPSECURITY_ATTRIBUTES lpProcessAttributes, // SD
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes,  // SD
		0,					//BOOL bInheritHandles,                      // handle inheritance option
		0,					//DWORD dwCreationFlags,                     // creation flags
		NULL,				//LPVOID lpEnvironment,                      // new environment block
		NULL,				//LPCTSTR lpCurrentDirectory,                // current directory name
		&si,				//LPSTARTUPINFO lpStartupInfo,               // startup information
		&pi					//LPPROCESS_INFORMATION lpProcessInformation // process information
		);

	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_OBJECT_0) 
	{
		// Process Á¾·á
	}

	CloseHandle( pi.hThread );
	CloseHandle( pi.hProcess );
}