#------------------------
# NOPOPUPS.MAK make file
#------------------------

nopopups.exe : nopopups.obj nopopups.res
     $(LINKER) $(GUIFLAGS) -OUT:nopopups.exe nopopups.obj \
	 nopopups.res $(GUILIBS)

nopopups.obj : nopopups.c nopopups.h
     $(CC) $(CFLAGS) nopopups.c

nopopups.res : nopopups.rc nopopups.h
     $(RC) $(RCVARS) nopopups.rc
