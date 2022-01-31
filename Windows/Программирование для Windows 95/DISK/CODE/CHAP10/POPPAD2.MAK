#-----------------------
# POPPAD2.MAK make file
#-----------------------

poppad2.exe : poppad2.obj poppad2.res
     $(LINKER) $(GUIFLAGS) -OUT:poppad2.exe poppad2.obj poppad2.res $(GUILIBS)

poppad2.obj : poppad2.c poppad2.h
     $(CC) $(CFLAGS) poppad2.c

poppad2.res : poppad2.rc poppad2.h poppad2.ico
     $(RC) $(RCVARS) poppad2.rc
