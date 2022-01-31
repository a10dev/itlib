#------------------------
# METAFILE.MAK make file
#------------------------

metafile.exe : metafile.obj
     $(LINKER) $(GUIFLAGS) -OUT:metafile.exe metafile.obj $(GUILIBS)

metafile.obj : metafile.c
     $(CC) $(CFLAGS) metafile.c
