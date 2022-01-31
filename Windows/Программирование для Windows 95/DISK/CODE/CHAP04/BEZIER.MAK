#----------------------
# BEZIER.MAK make file
#----------------------

bezier.exe : bezier.obj
    $(LINKER) $(GUIFLAGS) -OUT:bezier.exe bezier.obj $(GUILIBS)

bezier.obj : bezier.c
    $(CC) $(CFLAGS) bezier.c
