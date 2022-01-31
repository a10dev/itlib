REM -------------------------------------------------------------
REM  MSC.BAT -- Set up environment for Microsoft C/C++ 7.0 NMAKE
REM -------------------------------------------------------------
SET CC=cl
SET CFLAGS=-c -DSTRICT -G3 -Ow -W3 -Zp -Tp
SET CFLAGSMT=-c -DSTRICT -G3 -MT -Ow -W3 -Zp -Tp
SET LINKER=link
SET GUIFLAGS=-SUBSYSTEM:windows
SET DLLFLAGS=-SUBSYSTEM:windows -DLL
SET GUILIBS=-DEFAULTLIB:user32.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib
SET RC=rc
SET RCVARS=-r -DWIN32
