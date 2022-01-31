#------------------------
# CHECKER1.MAK make file
#------------------------

checker1.exe : checker1.obj
     $(LINKER) $(GUIFLAGS) -OUT:checker1.exe checker1.obj $(GUILIBS)

checker1.obj : checker1.c
     $(CC) $(CFLAGS) checker1.c
