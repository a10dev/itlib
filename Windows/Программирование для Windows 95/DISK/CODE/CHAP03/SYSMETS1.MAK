#------------------------
# SYSMETS1.MAK make file
#------------------------

sysmets1.exe : sysmets1.obj
     $(LINKER) $(GUIFLAGS) -OUT:sysmets1.exe sysmets1.obj $(GUILIBS)

sysmets1.obj : sysmets1.c sysmets.h
     $(CC) $(CFLAGS) sysmets1.c
