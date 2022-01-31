#------------------------
# JUSTIFY1.MAK make file
#------------------------

justify1.exe : justify1.obj ezfont.obj
     $(LINKER) $(GUIFLAGS) -OUT:justify1.exe justify1.obj ezfont.obj $(GUILIBS)

justify1.obj : justify1.c
     $(CC) $(CFLAGS) justify1.c

ezfont.obj : ezfont.c
     $(CC) $(CFLAGS) ezfont.c
