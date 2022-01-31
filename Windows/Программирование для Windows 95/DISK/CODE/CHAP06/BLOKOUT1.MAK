#------------------------
# BLOKOUT1.MAK make file
#------------------------

blokout1.exe : blokout1.obj
     $(LINKER) $(GUIFLAGS) -OUT:blokout1.exe blokout1.obj $(GUILIBS)

blokout1.obj : blokout1.c
     $(CC) $(CFLAGS) blokout1.c
