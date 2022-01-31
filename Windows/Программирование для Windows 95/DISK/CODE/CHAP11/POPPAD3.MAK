#-----------------------
# POPPAD3.MAK make file
#-----------------------

poppad3.exe : poppad.obj   popfile.obj  popfind.obj \
              popfont.obj popprnt0.obj poppad.res
     $(LINKER) $(GUIFLAGS) -OUT:poppad3.exe poppad.obj popfile.obj \
               popfind.obj popfont.obj popprnt0.obj poppad.res $(GUILIBS)

poppad.obj : poppad.c poppad.h
     $(CC) $(CFLAGS) poppad.c

popfile.obj : popfile.c
     $(CC) $(CFLAGS) popfile.c

popfind.obj : popfind.c
     $(CC) $(CFLAGS) popfind.c

popfont.obj : popfont.c
     $(CC) $(CFLAGS) popfont.c

popprnt0.obj : popprnt0.c
     $(CC) $(CFLAGS) popprnt0.c

poppad.res : poppad.rc poppad.h poppad.ico
     $(RC) $(RCVARS) poppad.rc
