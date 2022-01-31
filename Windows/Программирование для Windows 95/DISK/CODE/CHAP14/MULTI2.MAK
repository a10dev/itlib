#----------------------
# MULTI2.MAK make file
#----------------------

multi2.exe : multi2.obj
     $(LINKER) $(GUIFLAGS) -OUT:multi2.exe multi2.obj $(GUILIBS)

multi2.obj : multi2.c
     $(CC) $(CFLAGSMT) multi2.c
