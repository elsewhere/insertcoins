# Microsoft Developer Studio Project File - Name="efushow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=efushow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "efushow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "efushow.mak" CFG="efushow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "efushow - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "efushow - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "efushow - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "efushow - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "efushow - Win32 Release"
# Name "efushow - Win32 Debug"
# Begin Group "Effect sources"

# PROP Default_Filter "c;cpp"
# Begin Source File

SOURCE=.\effects\alku.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\asteroids.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\bands.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\city.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\laby.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\pong.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\rings.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\spark.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\starfield.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tetris.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tron.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\tunnel.cpp
# End Source File
# Begin Source File

SOURCE=.\effects\wiretunnel.cpp
# End Source File
# End Group
# Begin Group "Effect headers"

# PROP Default_Filter "h;hpp"
# Begin Source File

SOURCE=.\effects\alku.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\asteroids.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\bands.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\city.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\laby.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\pong.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\rings.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\spark.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\starfield.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tetris.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tron.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\tunnel.hpp
# End Source File
# Begin Source File

SOURCE=.\effects\wiretunnel.hpp
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\lista2.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\mathematics.cpp
# End Source File
# Begin Source File

SOURCE=.\mmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\path2.cpp
# End Source File
# Begin Source File

SOURCE=.\primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\valopallo.cpp
# End Source File
# Begin Source File

SOURCE=.\writer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\config.hpp
# End Source File
# Begin Source File

SOURCE=.\enums.hpp
# End Source File
# Begin Source File

SOURCE=.\extensions.hpp
# End Source File
# Begin Source File

SOURCE=.\externs.hpp
# End Source File
# Begin Source File

SOURCE=.\lista2.hpp
# End Source File
# Begin Source File

SOURCE=.\mathematics.hpp
# End Source File
# Begin Source File

SOURCE=.\mmgr.h
# End Source File
# Begin Source File

SOURCE=.\nommgr.h
# End Source File
# Begin Source File

SOURCE=.\path2.hpp
# End Source File
# Begin Source File

SOURCE=.\primitives.hpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\valopallo.hpp
# End Source File
# Begin Source File

SOURCE=.\writer.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# End Target
# End Project
