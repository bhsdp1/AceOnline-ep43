# Microsoft Developer Studio Project File - Name="VCServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VCServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VCServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VCServer.mak" CFG="VCServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VCServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VCServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VCServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\..\..\output\VCServer\Release"
# PROP Intermediate_Dir ".\..\..\output\VCServer\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./../../Common" /I "./../CommonVoIP" /I "./Main" /I "./" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /machine:I386 /out:".\..\..\Bin\GameVoIP\Release\VCServer.exe" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VCServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\..\..\output\VCServer\Debug"
# PROP Intermediate_Dir ".\..\..\output\VCServer\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./../../Common" /I "./../CommonVoIP" /I "./Main" /I "./" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /map /debug /machine:I386 /out:".\..\..\Bin\GameVoIP\Debug\VCServer.exe" /pdbtype:sept /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "VCServer - Win32 Release"
# Name "VCServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Main\C2DWinSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\Config.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\Global.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\IOCP.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\IOCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\MTAutoCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\MTCriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\Overlapped.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\QueueINT.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Room.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\RoomManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\SocketHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\Common\SystemLogManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\ThreadCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\VCGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\VCIOCP.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\VCIOCPSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\VCServer.cpp
# End Source File
# Begin Source File

SOURCE=.\VCServer.rc
# End Source File
# Begin Source File

SOURCE=..\..\Common\VMem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\VMemPool.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPGlobal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\WinPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Common\WinSocket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Main\C2DWinSocket.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\Config.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\IOCP.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\IOCPSocket.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Main\Room.h
# End Source File
# Begin Source File

SOURCE=.\Main\RoomManager.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Main\VCGlobal.h
# End Source File
# Begin Source File

SOURCE=.\Main\VCIOCP.h
# End Source File
# Begin Source File

SOURCE=.\Main\VCIOCPSocket.h
# End Source File
# Begin Source File

SOURCE=.\VCServer.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPGlobal.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPParam.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPProtocol.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\WinSocket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\small.ico
# End Source File
# Begin Source File

SOURCE=.\VCServer.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
