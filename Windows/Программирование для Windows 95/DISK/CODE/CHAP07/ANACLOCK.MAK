#------------------------
# ANACLOCK.MAK make file
#------------------------

anaclock.exe : anaclock.obj
     $(LINKER) $(GUIFLAGS) -OUT:anaclock.exe anaclock.obj $(GUILIBS)

anaclock.obj : anaclock.c
     $(CC) $(CFLAGS) anaclock.c
