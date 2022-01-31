#-----------------------
# COLORS3.MAK make file
#-----------------------

colors3.exe : colors3.obj
     $(LINKER) $(GUIFLAGS) -OUT:colors3.exe colors3.obj $(GUILIBS)

colors3.obj : colors3.c
     $(CC) $(CFLAGS) colors3.c
