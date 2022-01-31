#-----------------------
# CALLER.MAK make file
#-----------------------

caller.exe : caller.obj caller.res imalloc.lib
     $(LINKER) $(GUIFLAGS) -OUT:caller.exe caller.obj caller.res \
               imalloc.lib $(GUILIBS) uuid.lib

caller.obj : caller.c caller.h imalloc.h
     $(CC) -DCINTERFACE $(CFLAGS) caller.c

caller.res : caller.rc caller.h
     $(RC) $(RCVARS) caller.rc
