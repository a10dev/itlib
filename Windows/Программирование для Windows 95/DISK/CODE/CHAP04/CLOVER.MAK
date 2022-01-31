#----------------------
# CLOVER.MAK make file
#----------------------

clover.exe : clover.obj
     $(LINKER) $(GUIFLAGS) -OUT:clover.exe clover.obj $(GUILIBS)

clover.obj : clover.c
     $(CC) $(CFLAGS) clover.c
