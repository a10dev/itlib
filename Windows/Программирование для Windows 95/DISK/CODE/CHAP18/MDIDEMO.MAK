#------------------------
# MDIDEMO.MAK make file
#------------------------

mdidemo.exe : mdidemo.obj mdidemo.res
     $(LINKER) $(GUIFLAGS) -OUT:mdidemo.exe mdidemo.obj mdidemo.res $(GUILIBS)

mdidemo.obj : mdidemo.c mdidemo.h
     $(CC) $(CFLAGS) mdidemo.c

mdidemo.res : mdidemo.rc mdidemo.h
     $(RC) $(RCVARS) mdidemo.rc

