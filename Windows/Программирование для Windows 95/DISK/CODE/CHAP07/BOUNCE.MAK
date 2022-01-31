#----------------------
# BOUNCE.MAK make file
#----------------------

bounce.exe : bounce.obj
     $(LINKER) $(GUIFLAGS) -OUT:bounce.exe bounce.obj $(GUILIBS)

bounce.obj : bounce.c
     $(CC) $(CFLAGS) bounce.c
