#-----------------------
# ENVIRON.MAK make file
#-----------------------

environ.exe : environ.obj
     $(LINKER) $(GUIFLAGS) -OUT:environ.exe environ.obj $(GUILIBS)

environ.obj : environ.c
     $(CC) $(CFLAGS) environ.c
