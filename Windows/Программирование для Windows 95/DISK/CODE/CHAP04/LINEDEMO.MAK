#------------------------
# LINEDEMO.MAK make file
#------------------------

linedemo.exe : linedemo.obj
     $(LINKER) $(GUIFLAGS) -OUT:linedemo.exe linedemo.obj $(GUILIBS)

linedemo.obj : linedemo.c
     $(CC) $(CFLAGS) linedemo.c
