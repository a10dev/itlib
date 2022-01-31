#------------------------
# SHOWPOP2.MAK make file
#------------------------

showpop2.exe : showpop2.obj
     $(LINKER) $(GUIFLAGS) -OUT:showpop2.exe showpop2.obj $(GUILIBS)

showpop2.obj : showpop2.c showpop.h
     $(CC) $(CFLAGS) showpop2.c
