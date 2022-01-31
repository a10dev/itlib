#------------------------
# SINEWAVE.MAK make file
#------------------------

sinewave.exe : sinewave.obj
     $(LINKER) $(GUIFLAGS) -OUT:sinewave.exe sinewave.obj $(GUILIBS)

sinewave.obj : sinewave.c
     $(CC) $(CFLAGS) sinewave.c
