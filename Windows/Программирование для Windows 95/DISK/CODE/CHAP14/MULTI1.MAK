#----------------------
# MULTI1.MAK make file
#----------------------

multi1.exe : multi1.obj
     $(LINKER) $(GUIFLAGS) -OUT:multi1.exe multi1.obj $(GUILIBS)

multi1.obj : multi1.c
     $(CC) $(CFLAGS) multi1.c
