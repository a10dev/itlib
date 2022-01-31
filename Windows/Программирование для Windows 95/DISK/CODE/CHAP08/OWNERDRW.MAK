#------------------------
# OWNERDRW.MAK make file
#------------------------

ownerdrw.exe : ownerdrw.obj
     $(LINKER) $(GUIFLAGS) -OUT:ownerdrw.exe ownerdrw.obj $(GUILIBS)

ownerdrw.obj : ownerdrw.c
     $(CC) $(CFLAGS) ownerdrw.c
