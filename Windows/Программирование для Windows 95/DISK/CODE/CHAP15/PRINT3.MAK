#----------------------
# PRINT3.MAK make file
#----------------------
 
print3.exe : print.obj print3.obj print.res
     $(LINKER) $(GUIFLAGS) -OUT:print3.exe print.obj print3.obj \
     print.res $(GUILIBS) winspool.lib

print.obj : print.c
     $(CC) $(CFLAGS) print.c

print3.obj : print3.c
     $(CC) $(CFLAGS) print3.c

print.res : print.rc
     $(RC) $(RCVARS) print.rc
