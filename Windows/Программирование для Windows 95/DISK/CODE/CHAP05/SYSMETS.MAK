#-----------------------
# SYSMETS.MAK make file
#-----------------------

sysmets.exe : sysmets.obj
     $(LINKER) $(GUIFLAGS) -OUT:sysmets.exe sysmets.obj $(GUILIBS)

sysmets.obj : sysmets.c sysmets.h
     $(CC) $(CFLAGS) sysmets.c
