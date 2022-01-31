#------------------------
# CHECKER2.MAK make file
#------------------------

checker2.exe : checker2.obj
     $(LINKER) $(GUIFLAGS) -OUT:checker2.exe checker2.obj $(GUILIBS)

checker2.obj : checker2.c
     $(CC) $(CFLAGS) checker2.c
