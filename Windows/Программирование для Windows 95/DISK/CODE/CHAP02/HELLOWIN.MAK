#------------------------
# HELLOWIN.MAK make file
#------------------------

hellowin.exe : hellowin.obj
     $(LINKER) $(GUIFLAGS) -OUT:hellowin.exe hellowin.obj $(GUILIBS)

hellowin.obj : hellowin.c
     $(CC) $(CFLAGS) hellowin.c
