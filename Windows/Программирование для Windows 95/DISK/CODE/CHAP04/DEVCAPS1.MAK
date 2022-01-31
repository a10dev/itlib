#------------------------
# DEVCAPS1.MAK make file
#------------------------

devcaps1.exe : devcaps1.obj
     $(LINKER) $(GUIFLAGS) -OUT:devcaps1.exe devcaps1.obj $(GUILIBS)

devcaps1.obj : devcaps1.c
     $(CC) $(CFLAGS) devcaps1.c
