#-----------------------
# BIGJOB2.MAK make file
#-----------------------

bigjob2.exe : bigjob2.obj
     $(LINKER) $(GUIFLAGS) -OUT:bigjob2.exe bigjob2.obj $(GUILIBS)

bigjob2.obj : bigjob2.c
     $(CC) $(CFLAGSMT) bigjob2.c
