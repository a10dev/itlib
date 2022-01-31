#------------------------
# SYSMETS3.MAK make file
#------------------------

sysmets3.exe : sysmets3.obj
     $(LINKER) $(GUIFLAGS) -OUT:sysmets3.exe sysmets3.obj $(GUILIBS)

sysmets3.obj : sysmets3.c sysmets.h
     $(CC) $(CFLAGS) sysmets3.c
