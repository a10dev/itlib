#-----------------------
# STRPROG.MAK make file
#-----------------------

strprog.exe : strprog.obj strprog.res strlib.lib
     $(LINKER) $(GUIFLAGS) -OUT:strprog.exe strprog.obj strprog.res \
               strlib.lib $(GUILIBS)

strprog.obj : strprog.c strprog.h strlib.h
     $(CC) $(CFLAGS) strprog.c

strprog.res : strprog.rc strprog.h
     $(RC) $(RCVARS) strprog.rc
