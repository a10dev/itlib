#--------------------
# HEAD.MAK make file
#--------------------

head.exe : head.obj
     $(LINKER) $(GUIFLAGS) -OUT:head.exe head.obj $(GUILIBS)

head.obj : head.c
     $(CC) $(CFLAGS) head.c
