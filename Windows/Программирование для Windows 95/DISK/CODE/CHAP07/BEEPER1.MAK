#-----------------------
# BEEPER1.MAK make file
#-----------------------

beeper1.exe : beeper1.obj
     $(LINKER) $(GUIFLAGS) -OUT:beeper1.exe beeper1.obj $(GUILIBS)

beeper1.obj : beeper1.c
     $(CC) $(CFLAGS) beeper1.c
