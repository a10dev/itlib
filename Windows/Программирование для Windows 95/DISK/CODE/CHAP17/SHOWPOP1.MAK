#------------------------
# SHOWPOP1.MAK make file
#------------------------

showpop1.exe : showpop1.obj
     $(LINKER) $(GUIFLAGS) -OUT:showpop1.exe showpop1.obj $(GUILIBS)

showpop1.obj : showpop1.c showpop.h
     $(CC) $(CFLAGS) showpop1.c
