#------------------------
# SYSMETS2.MAK make file
#------------------------

sysmets2.exe : sysmets2.obj
     $(LINKER) $(GUIFLAGS) -OUT:sysmets2.exe sysmets2.obj $(GUILIBS)

sysmets2.obj : sysmets2.c sysmets.h
     $(CC) $(CFLAGS) sysmets2.c
