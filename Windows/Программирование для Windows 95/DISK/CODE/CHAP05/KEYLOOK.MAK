#-----------------------
# KEYLOOK.MAK make file
#-----------------------

keylook.exe : keylook.obj
     $(LINKER) $(GUIFLAGS) -OUT:keylook.exe keylook.obj $(GUILIBS)

keylook.obj : keylook.c
     $(CC) $(CFLAGS) keylook.c
