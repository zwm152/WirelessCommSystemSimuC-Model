# Microsoft Developer Studio Project File - Name="ToneGenTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ToneGenTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ToneGenTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ToneGenTest.mak" CFG="ToneGenTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ToneGenTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ToneGenTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ToneGenTest - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "ToneGenTest - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_VAR_BLOCKS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ToneGenTest - Win32 Release"
# Name "ToneGenTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\utils\bart_pdgm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\cbitrev_T.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\complex_io.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\digraph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\dit_sino_T.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\exec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\fft_T.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\gen_win.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\gensig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\hann.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\log2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\model_graph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\parmfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\psmodel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\psstream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\reports.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\samp_spect_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\siganchr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\signal_T.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\sigplot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\sim_startup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\spec_analyzer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\spec_calc_kinds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utils\spec_estim.cpp
# End Source File
# Begin Source File

SOURCE=..\..\support\syst_graph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\models\tone_gen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\sims\tonegentest_sim.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
