#------------------------
# RANDRECT.MAK make file
#------------------------

randrect.exe : randrect.obj
     $(LINKER) $(GUIFLAGS) -OUT:randrect.exe randrect.obj $(GUILIBS)

randrect.obj : randrect.c
     $(CC) $(CFLAGS) randrect.c
