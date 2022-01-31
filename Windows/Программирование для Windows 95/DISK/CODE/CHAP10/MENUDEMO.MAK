#------------------------
# MENUDEMO.MAK make file
#------------------------

menudemo.exe : menudemo.obj menudemo.res
     $(LINKER) $(GUIFLAGS) -OUT:menudemo.exe menudemo.obj \
	 menudemo.res $(GUILIBS)

menudemo.obj : menudemo.c menudemo.h
     $(CC) $(CFLAGS) menudemo.c

menudemo.res : menudemo.rc menudemo.h
     $(RC) $(RCVARS) menudemo.rc
