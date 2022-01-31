#------------------------
# DEVCAPS2.MAK make file
#------------------------
 
devcaps2.exe : devcaps2.obj devcaps2.res
     $(LINKER) $(GUIFLAGS) -OUT:devcaps2.exe devcaps2.obj \
          devcaps2.res $(GUILIBS) winspool.lib
 
devcaps2.obj : devcaps2.c devcaps2.h
     $(CC) $(CFLAGS) devcaps2.c

devcaps2.res : devcaps2.rc devcaps2.h
     $(RC) $(RCVARS) devcaps2.rc
