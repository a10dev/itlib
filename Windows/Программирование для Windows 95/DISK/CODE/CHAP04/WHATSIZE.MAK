#------------------------
# WHATSIZE.MAK make file
#------------------------

whatsize.exe : whatsize.obj
     $(LINKER) $(GUIFLAGS) -OUT:whatsize.exe whatsize.obj $(GUILIBS)

whatsize.obj : whatsize.c
     $(CC) $(CFLAGS) whatsize.c
