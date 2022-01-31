#-----------------------
# poppad.MAK make file
#-----------------------

poppad.exe : poppad.obj popfile.obj popfind.obj \
             popfont.obj popprnt.obj poppad.res
     $(LINKER) $(GUIFLAGS) -OUT:poppad.exe poppad.obj popfile.obj \
     popfind.obj popfont.obj popprnt.obj poppad.res $(GUILIBS)

poppad.obj : poppad.c poppad.h
     $(CC) $(CFLAGS) poppad.c

popfile.obj : popfile.c
     $(CC) $(CFLAGS) popfile.c

popfind.obj : popfind.c
     $(CC) $(CFLAGS) popfind.c

popfont.obj : popfont.c
     $(CC) $(CFLAGS) popfont.c

popprnt.obj : popprnt.c
     $(CC) $(CFLAGS) popprnt.c

poppad.res : poppad.rc poppad.h poppad.ico
     $(RC) $(RCVARS) poppad.rc
