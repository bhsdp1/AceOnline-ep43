
#include "stdafx.h"
#include <winsvc.h>

BOOL InstallAndStartDriver(SC_HANDLE hSCManager, LPCTSTR DriverName,
                                                        LPCTSTR ServiceExe)
{
    SC_HANDLE  hService;
    BOOL       bReturn;


    // Create the driver entry in the SC Manager.
    hService = CreateService(hSCManager,            // SCManager database
                             DriverName,            // name of service
                             DriverName,            // name to display
                             SERVICE_ALL_ACCESS,    // desired access//SERVICE_START//SERVICE_ALL_ACCESS
                             SERVICE_KERNEL_DRIVER, // service type
                             SERVICE_DEMAND_START,  // start type
                             SERVICE_ERROR_NORMAL,  // error control type
                             ServiceExe,            // service's binary
                             NULL,                  // no load ordering group
                             NULL,                  // no tag identifier
                             NULL,                  // no dependencies
                             NULL,                  // LocalSystem account
                             NULL                   // no password
                             );

    // This may fail because the service entry already exists.  However,
    // we've tried to load it earlier, so something clearly is wrong if
    // we can't create it!
//////////////////////////////수정된 부분입니다.//////////////////////START
    if(!hService)
	{
		   hService = OpenService(hSCManager, "Event", SC_MANAGER_ALL_ACCESS);
		  
		    if (hService == NULL){
				CloseServiceHandle(hSCManager);
				return FALSE;
			}
			else{
					StartService(hService, 0, NULL ); 
					CloseServiceHandle(hService);
			}
			
	}
	else{
		  bReturn = StartService(hService, 0, NULL);
		  CloseServiceHandle(hService);
		 }
///////////////////////////수정된 부분입니다.//////////////////////////END
	return bReturn;
}


BOOL LoadDynamicNTDriver()
{
    SC_HANDLE   hSCManager;
    char        currentDirectory[128];
    BOOL        bReturn;


    // Open Service Control Manager on the local machine...
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    // Assume driver is in the same directory as the DLL.  Hence, create
    // a fully qualified pathname for the SCM to add to the registry.
//    GetCurrentDirectory(128, currentDirectory);
	GetSystemDirectory(currentDirectory, 128);
	lstrcat(currentDirectory, "\\Drivers");
    lstrcat(currentDirectory,"\\Event.sys");
    
    // Install drive in the registry and start it...
    bReturn = InstallAndStartDriver(hSCManager, "Event", currentDirectory);

    // Close the Service Control Manager...
    CloseServiceHandle(hSCManager);

    return bReturn;
}


////////////////////////////////////////////////////////////////////////////
//  Unload routines
////////////////////////////////////////////////////////////////////////////


BOOL UnloadDynamicNTDriver()
{
    SC_HANDLE       hSCManager;
    SC_HANDLE       hService;
    SERVICE_STATUS  serviceStatus;
    BOOL            bReturn;

	char        currentDirectory[128];
   // Open Service Control Manager on the local machine...
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    // Open the Service Control Manager for our driver service...
//	GetCurrentDirectory(128, currentDirectory);
	GetSystemDirectory(currentDirectory, 128);
	lstrcat(currentDirectory, "\\Drivers");
    lstrcat(currentDirectory,"\\Event.sys");

    hService = OpenService(hSCManager, currentDirectory, SERVICE_ALL_ACCESS);//SERVICE_ALL_ACCESS

    // Stop the driver.  Will return TRUE on success... 
    bReturn = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);

    // Delete the driver from the registry...
    if(bReturn == TRUE)
        bReturn = DeleteService(hService);

    // Close the SC Manager
    CloseServiceHandle(hService);

    // Close Service Control Manager
    CloseServiceHandle(hSCManager);

    return bReturn;
}


HANDLE LoadDriver(OSVERSIONINFO *os, BOOL *fNTDynaLoaded)
{
    HANDLE hDevice;

    
    // Default to FALSE on informing the caller is we've dynamically
    // loaded the NT driver via the SCM.
    *fNTDynaLoaded = FALSE;

    // If we're under NT...
    if(os->dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        // Try opening the device driver (in case it's static).  We'll
        // Deal with a failure in a moment...
        hDevice = CreateFile("\\\\.\\Event",
                             GENERIC_READ | GENERIC_WRITE, 
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             0,                     // Default security
                             OPEN_EXISTING,
                             NULL,//FILE_FLAG_OVERLAPPED,  // Perform asynchronous I/O
                             0);                    // No template
        
        // If the device driver wasn't started, let's dynamically load it
        // here...
        if(hDevice == INVALID_HANDLE_VALUE)
        {
            // Load and start the driver...
            LoadDynamicNTDriver();

            // Open the driver...
            hDevice = CreateFile("\\\\.\\Event",
                             GENERIC_READ | GENERIC_WRITE, 
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             0,                     // Default security
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,  // Perform asynchronous I/O
                             0);                    // No template
            
            // Set the global to indicate that we've Dynaloaded.  This will
            // Cause DllMain to stop and unload on a process detach.
            *fNTDynaLoaded = TRUE;
        }

        return hDevice;
    }
	else {
		hDevice = CreateFile("\\\\.\\event.vxd",
							0,
							0,
							NULL,
							0,
							FILE_FLAG_DELETE_ON_CLOSE,
							NULL);
		return hDevice;
	}
}
