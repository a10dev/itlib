#-----------------------
# BIGJOB1.MAK make file
#-----------------------

bigjob1.exe : bigjob1.obj
     $(LINKER) $(GUIFLAGS) -OUT:bigjob1.exe bigjob1.obj $(GUILIBS)

bigjob1.obj : bigjob1.c
     $(CC) $(CFLAGSMT) bigjob1.c
