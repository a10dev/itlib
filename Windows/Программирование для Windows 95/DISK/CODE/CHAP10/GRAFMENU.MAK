#------------------------
# GRAFMENU.MAK make file
#------------------------

grafmenu.exe : grafmenu.obj grafmenu.res
     $(LINKER) $(GUIFLAGS) -OUT:grafmenu.exe \
	 grafmenu.obj grafmenu.res $(GUILIBS)

grafmenu.obj : grafmenu.c grafmenu.h
     $(CC) $(CFLAGS) grafmenu.c

grafmenu.res : grafmenu.rc grafmenu.h \
     editlabl.bmp filelabl.bmp fontlabl.bmp bighelp.bmp
     $(RC) $(RCVARS) grafmenu.rc
