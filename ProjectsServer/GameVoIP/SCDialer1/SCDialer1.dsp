# Microsoft Developer Studio Project File - Name="SCDialer1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCDialer1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCDialer1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCDialer1.mak" CFG="SCDialer1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCDialer1 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCDialer1 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCDialer1 - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\output\SCDialer1\Release"
# PROP Intermediate_Dir "..\..\output\SCDialer1\Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "./RAD_RTP" /I "./PlatformSDK/include" /I "./RAD_RTP/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_G7231CODEC" /D "_LOCAL_PLAY_G7231" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 wsock32.lib mpr.lib winmm.lib snmpapi.lib ./RAD_RTP/lib/rv32rtp.lib Version.lib /nologo /subsystem:windows /dll /map /machine:I386 /force /out:"..\..\Bin\GameVoIP\Release\SCDialer1.ocx" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\..\..\output\SCDialer1\Release
TargetPath=\0000_Projects(Masang_Kor)_Dev\Bin\GameVoIP\Release\SCDialer1.ocx
InputPath=\0000_Projects(Masang_Kor)_Dev\Bin\GameVoIP\Release\SCDialer1.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "SCDialer1 - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\output\SCDialer1\Debug"
# PROP Intermediate_Dir "..\..\output\SCDialer1\Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "./PlatformSDK/include" /I "./RAD_RTP" /I "./RAD_RTP/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_G7231CODEC" /D "_LOCAL_PLAY_G7231" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib mpr.lib winmm.lib snmpapi.lib ./RAD_RTP/lib/rv32rtp.lib ./PlatformSDK/lib/iphlpapi.lib Version.lib /nologo /subsystem:windows /dll /incremental:no /map /debug /machine:I386 /force /out:"..\..\Bin\GameVoIP\Debug\SCDialer1.ocx" /MAPINFO:LINES /MAPINFO:EXPORTS
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\..\..\output\SCDialer1\Debug
TargetPath=\0000_Projects(Masang_Kor)_Dev\Bin\GameVoIP\Debug\SCDialer1.ocx
InputPath=\0000_Projects(Masang_Kor)_Dev\Bin\GameVoIP\Debug\SCDialer1.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "SCDialer1 - Win32 Release"
# Name "SCDialer1 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\RAD_RTP\AudioCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\audiosound.cpp
# End Source File
# Begin Source File

SOURCE=.\Code2.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\DataQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\datasocket.cpp
# End Source File
# Begin Source File

SOURCE=.\DrvLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\file_ver.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231Codec.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G729Codec.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\H323Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\IncommingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NATAnalyzer.cpp
# End Source File
# Begin Source File

SOURCE=.\NATAnalyzerSock.cpp
# End Source File
# Begin Source File

SOURCE=.\NATSocket1.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\RouteSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPBase.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPCtrlManager.cpp
# End Source File
# Begin Source File

SOURCE=.\RTPRouter.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.def
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.odl
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.rc
# End Source File
# Begin Source File

SOURCE=.\SCDialer1Ctl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialer1Ppg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemLogManager.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowsRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\WinPacket.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\RAD_RTP\AudioCodec.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\audiosound.h
# End Source File
# Begin Source File

SOURCE=.\Code2.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\DataQueue.h
# End Source File
# Begin Source File

SOURCE=.\datasocket.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\DbgOut_C.h
# End Source File
# Begin Source File

SOURCE=.\DialerDefine.h
# End Source File
# Begin Source File

SOURCE=.\file_ver.h
# End Source File
# Begin Source File

SOURCE=.\file_ver.inl
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231Codec.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G729Codec.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\H323Connection.h
# End Source File
# Begin Source File

SOURCE=.\IncommingDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSDK\include\IPExport.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSDK\include\IPHlpApi.h
# End Source File
# Begin Source File

SOURCE=.\PlatformSDK\include\IPTypes.h
# End Source File
# Begin Source File

SOURCE=.\NATAnalyzer.h
# End Source File
# Begin Source File

SOURCE=.\NATAnalyzerSock.h
# End Source File
# Begin Source File

SOURCE=.\NATSocket1.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\include\payload.h
# End Source File
# Begin Source File

SOURCE=.\Protocol_FireTalk_II.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\Queue.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RouteSocket.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\include\rtcp.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\include\rtp.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPBase.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\RTPCtrlManager.h
# End Source File
# Begin Source File

SOURCE=.\RTPRouter.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\include\rvcommon.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\include\rvstd.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer1Ctl.h
# End Source File
# Begin Source File

SOURCE=.\SCDialer1Ppg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\sx5363api.h
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\sx80api.h
# End Source File
# Begin Source File

SOURCE=.\SystemLogManager.h
# End Source File
# Begin Source File

SOURCE=.\WindowsRegistry.h
# End Source File
# Begin Source File

SOURCE=.\WinPacket.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\RAD_RTP\G7231_dtmf\#.7231"
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\0.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\1.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\2.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\3.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\4.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\5.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\6.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\7.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\8.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\9.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\asterisk.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\ment.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\ring.7231
# End Source File
# Begin Source File

SOURCE=.\SCDialer1.ico
# End Source File
# Begin Source File

SOURCE=.\SCDialer1Ctl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\0.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\1.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\2.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\3.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\4.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\5.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\6.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\7.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\8.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\9.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\asterisk.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\back.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\Busy.7231
# End Source File
# Begin Source File

SOURCE=.\dialtone.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\dialtone.7231
# End Source File
# Begin Source File

SOURCE=.\dialtone2.7231
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\sound\ring.wav
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\lib\RV32RTP.dll
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\sendbtn.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\sharp.7231L
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\lib\Sx5363.ini
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\lib\SX5363S.DLL
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\lib\sx80lc32.dll
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\G7231_dtmf\timeout.7231
# End Source File
# Begin Source File

SOURCE=.\RAD_RTP\lib\RV32RTP.lib
# End Source File
# Begin Source File

SOURCE=.\PlatformSDK\lib\IPHlpApi.Lib
# End Source File
# End Target
# End Project
# Section SCDialer1 : {72ADFD76-2C39-11D0-9903-00A0C91BC942}
# 	1:26:CG_IDS_SOCKETS_INIT_FAILED:102
# 	2:17:WinSocketsApplied:WinSocketsApplied
# End Section
