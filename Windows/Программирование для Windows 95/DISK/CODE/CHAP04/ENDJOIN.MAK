#-----------------------
# ENDJOIN.MAK make file
#-----------------------

endjoin.exe : endjoin.obj
     $(LINKER) $(GUIFLAGS) -OUT:endjoin.exe endjoin.obj $(GUILIBS)

endjoin.obj : endjoin.c
     $(CC) $(CFLAGS) endjoin.c
