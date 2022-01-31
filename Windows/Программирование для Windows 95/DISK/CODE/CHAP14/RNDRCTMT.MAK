#------------------------
# RNDRCTMT.MAK make file
#------------------------

rndrctmt.exe : rndrctmt.obj
     $(LINKER) $(GUIFLAGS) -OUT:rndrctmt.exe rndrctmt.obj $(GUILIBS)

rndrctmt.obj : rndrctmt.c
     $(CC) $(CFLAGSMT) rndrctmt.c
