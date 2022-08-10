# Microsoft Developer Studio Project File - Name="SCVoIP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SCVoIP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCVoIP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCVoIP.mak" CFG="SCVoIP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCVoIP - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SCVoIP - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCVoIP - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\output\SCVoIP\Release"
# PROP Intermediate_Dir "..\..\output\SCVoIP\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./../../Common" /I "./../CommonVoIP" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"..\..\Bin\GameVoIP\Release\SCVoIP.exe"

!ELSEIF  "$(CFG)" == "SCVoIP - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\output\SCVoIP\Debug"
# PROP Intermediate_Dir "..\..\output\SCVoIP\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./../../Common" /I "./../CommonVoIP" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\Bin\GameVoIP\Debug\SCVoIP.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SCVoIP - Win32 Release"
# Name "SCVoIP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\scdialer1.cpp
# End Source File
# Begin Source File

SOURCE=.\scdialer2.cpp
# End Source File
# Begin Source File

SOURCE=.\SCVoIP.cpp
# End Source File
# Begin Source File

SOURCE=.\SCVoIP.rc
# End Source File
# Begin Source File

SOURCE=.\SCVoIPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPProtocol.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\scdialer1.h
# End Source File
# Begin Source File

SOURCE=.\scdialer2.h
# End Source File
# Begin Source File

SOURCE=.\SCVoIP.h
# End Source File
# Begin Source File

SOURCE=.\SCVoIPDlg.h
# End Source File
# Begin Source File

SOURCE=.\SocketSerial.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\VoIPError.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPParam.h
# End Source File
# Begin Source File

SOURCE=..\CommonVoIP\VoIPProtocol.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SCVoIP.ico
# End Source File
# Begin Source File

SOURCE=.\res\SCVoIP.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section SCVoIP : {37712FDF-7878-48C2-8B9B-3F8CFB645B3D}
# 	2:5:Class:CMiraDialer
# 	2:10:HeaderFile:miradialer.h
# 	2:8:ImplFile:miradialer.cpp
# End Section
# Section SCVoIP : {E2FA309F-02DE-4A35-B977-B9B7D305C2A7}
# 	2:5:Class:CMiraDialer
# 	2:10:HeaderFile:miradialer.h
# 	2:8:ImplFile:miradialer.cpp
# End Section
# Section SCVoIP : {7B3A6A8C-35BB-4D33-881E-EA781A671457}
# 	2:21:DefaultSinkHeaderFile:miradialer.h
# 	2:16:DefaultSinkClass:CMiraDialer
# End Section
# Section SCVoIP : {ADAA5F78-0B23-4C82-B40D-9A547B2F9049}
# 	2:21:DefaultSinkHeaderFile:scdialer2.h
# 	2:16:DefaultSinkClass:CSCDialer2
# End Section
# Section SCVoIP : {3FD74C61-F842-4176-8704-5CB6A14563D8}
# 	2:5:Class:CWCWDialer
# 	2:10:HeaderFile:wcwdialer.h
# 	2:8:ImplFile:wcwdialer.cpp
# End Section
# Section SCVoIP : {3D96CA2E-E0C3-4B45-80FD-0B86C7937389}
# 	2:5:Class:CSCDialer2
# 	2:10:HeaderFile:scdialer2.h
# 	2:8:ImplFile:scdialer2.cpp
# End Section
# Section SCVoIP : {E43303FE-BDE8-44E2-87BF-259D920F3ADD}
# 	2:21:DefaultSinkHeaderFile:scdialer1.h
# 	2:16:DefaultSinkClass:CSCDialer1
# End Section
# Section SCVoIP : {A5F872C7-268E-42B8-81BE-548316D2AB4B}
# 	2:21:DefaultSinkHeaderFile:wcwdialer.h
# 	2:16:DefaultSinkClass:CWCWDialer
# End Section
