#-----------------------
# CALLPUB.MAK make file
#-----------------------

callpub.exe : callpub.obj callpub.res pubmem.lib
     $(LINKER) $(GUIFLAGS) -OUT:callpub.exe callpub.obj callpub.res \
	 pubmem.lib $(GUILIBS) uuid.lib ole32.lib

callpub.obj : callpub.c callpub.h pubmem.h
     $(CC) -DCINTERFACE $(CFLAGS) callpub.c

callpub.res : callpub.rc callpub.h
     $(RC) $(RCVARS) callpub.rc
