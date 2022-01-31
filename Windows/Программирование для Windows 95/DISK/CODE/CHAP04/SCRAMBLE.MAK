#------------------------
# SCRAMBLE.MAK make file
#------------------------

scramble.exe : scramble.obj
     $(LINKER) $(GUIFLAGS) -OUT:scramble.EXE scramble.obj $(GUILIBS)

scramble.obj : scramble.c
     $(CC) $(CFLAGS) scramble.c
