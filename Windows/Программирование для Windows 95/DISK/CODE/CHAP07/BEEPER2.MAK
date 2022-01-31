#-----------------------
# BEEPER2.MAK make file
#-----------------------

beeper2.exe : beeper2.obj
     $(LINKER) $(GUIFLAGS) -OUT:beeper2.exe beeper2.obj $(GUILIBS)

beeper2.obj : beeper2.c
     $(CC) $(CFLAGS) beeper2.c
