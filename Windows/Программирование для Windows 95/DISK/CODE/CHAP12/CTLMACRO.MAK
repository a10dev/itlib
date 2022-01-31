#-----------------------
# CTLMACRO.MAK make file
#-----------------------

ctlmacro.exe : ctlmacro.obj notify.obj treeview.obj ctlmacro.res
     $(LINKER) $(GUIFLAGS) -OUT:ctlmacro.exe ctlmacro.obj \
	 notify.obj treeview.obj ctlmacro.res $(GUILIBS)

ctlmacro.obj : ctlmacro.c notify.h resource.h treeview.h
     $(CC) $(CFLAGS) ctlmacro.c

notify.obj : notify.c notify.h
     $(CC) $(CFLAGS) notify.c

treeview.obj : treeview.c resource.h treeview.h treedata.h
     $(CC) $(CFLAGS) treeview.c

ctlmacro.res : ctlmacro.rc ctlmacro.ico
     $(RC) $(RCVARS) ctlmacro.rc
