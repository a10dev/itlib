#-----------------------
# COLORS1.MAK make file
#-----------------------

colors1.exe : colors1.obj
     $(LINKER) $(GUIFLAGS) -OUT:colors1.exe colors1.obj $(GUILIBS)

colors1.obj : colors1.c
     $(CC) $(CFLAGS) colors1.c
