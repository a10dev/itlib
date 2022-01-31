#------------------------
# CHECKER3.MAK make file
#------------------------

checker3.exe : checker3.obj
     $(LINKER) $(GUIFLAGS) -OUT:checker3.exe checker3.obj $(GUILIBS)

checker3.obj : checker3.c
     $(CC) $(CFLAGS) checker3.c
