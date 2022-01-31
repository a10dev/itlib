#----------------------
# PRINT1.MAK make file
#----------------------
 
print1.exe : print.obj print1.obj
     $(LINKER) $(GUIFLAGS) -OUT:print1.exe print.obj print1.obj \
	 $(GUILIBS) winspool.lib

print.obj : print.c
     $(CC) $(CFLAGS) print.c
 
print1.obj : print1.c
     $(CC) $(CFLAGS) print1.c
