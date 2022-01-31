#-----------------------
# SHOWBIT.MAK make file
#-----------------------

showbit.exe : showbit.obj
     $(LINKER) $(GUIFLAGS) -OUT:showbit.exe showbit.obj $(GUILIBS)

showbit.obj : showbit.c
     $(CC) $(CFLAGS) showbit.c
