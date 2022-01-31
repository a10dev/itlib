#-----------------------
# POPMENU.MAK make file
#-----------------------

popmenu.exe : popmenu.obj popmenu.res
     $(LINKER) $(GUIFLAGS) -OUT:popmenu.exe popmenu.obj \
	 popmenu.res $(GUILIBS)

popmenu.obj : popmenu.c popmenu.h
     $(CC) $(CFLAGS) popmenu.c

popmenu.res : popmenu.rc popmenu.h
     $(RC) $(RCVARS) popmenu.rc
