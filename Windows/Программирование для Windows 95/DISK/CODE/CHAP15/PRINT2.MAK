#----------------------
# PRINT2.MAK make file
#----------------------
 
print2.exe : print.obj print2.obj
     $(LINKER) $(GUIFLAGS) -OUT:print2.exe print.obj print2.obj \
	 $(GUILIBS) winspool.lib

print.obj : print.c
     $(CC) $(CFLAGS) print.c

print2.obj : print2.c
     $(CC) $(CFLAGS) print2.c
