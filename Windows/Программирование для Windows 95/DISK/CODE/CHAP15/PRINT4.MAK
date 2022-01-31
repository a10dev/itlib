#----------------------
# PRINT4.MAK make file
#----------------------
 
print4.exe : print.obj print4.obj print.res
     $(LINKER) $(GUIFLAGS) -OUT:print4.exe print.obj print4.obj \
     print.res $(GUILIBS) winspool.lib

print.obj : print.c
     $(CC) $(CFLAGS) print.c

print4.obj : print4.c
     $(CC) $(CFLAGS) print4.c

print.res : print.rc
     $(RC) $(RCVARS) print.rc

