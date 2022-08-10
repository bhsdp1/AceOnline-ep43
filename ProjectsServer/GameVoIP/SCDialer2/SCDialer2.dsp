# Microsoft Developer Studio Project File - Name="SCDialer2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCDialer2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCDialer2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCDialer2.mak" CFG="SCDialer2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCDialer2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCDialer2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCDialer2 - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\output\SCDialer2\Release"
# PROP Intermediate_Dir "..\..\output\SCDialer2\Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./../../Common" /I "./../CommonVoIP" /I "./Main" /I "./" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\Bin\GameVoIP\Release\SCDialer2.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\..\..\output\SCDialer2\Release
TargetPath=\Projects\Bin\GameVoIP\Release\SCDialer2.ocx
InputPath=\Projects\Bin\GameVoIP\Release\SCDialer2.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SCDialer2 - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\output\SCDialer2\Debug"
# PROP Intermediate_Dir "..\..\output\SCDialer2\Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../../Common" /I "./../CommonVoIP" /I "./Main" /I "./" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\Bin\GameVoIP\Debug\SCDialer2.ocx" /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\..\..\output\SCDialer2\Debug
TargetPath=\Projects\Bin\GameVoIP\Debug\SCDialer2.ocx
InputPath=\Projects\Bin\GameVoIP\Debug\SCDialer2.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "SCDialer2 - Win32 Release"
# Name "SCDialer2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Main\AudioCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Audiosound.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\AVManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\ChannelData.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\G7231Codec.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\S2CWinSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\S2DWinSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer2.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer2.def
# End Source File
# Begin Source File

SOURCE=.\SCDialer2.odl
# End Source File
# Begin Source File

SOURCE=.\SCDialer2.rc
# End Source File
# Begin Source File

SOURCE=.\SCDialer2Ctl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer2Ppg.cpp
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

SOURCE=..\CommonVoIP\VoIPParam.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPProtocol.cpp
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

SOURCE=.\Main\Audiosound.h
# End Source File
# Begin Source File

SOURCE=.\Main\AVManager.h
# End Source File
# Begin Source File

SOURCE=.\Main\ChannelData.h
# End Source File
# Begin Source File

SOURCE=.\Main\G7231Codec.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Main\S2CWinSocket.h
# End Source File
# Begin Source File

SOURCE=.\Main\S2DWinSocket.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer2.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer2Ctl.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer2Ppg.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\SocketHeader.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\SystemLogManager.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPParam.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPProtocol.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\WinPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\WinSocket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\SCDialer2Ctl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
