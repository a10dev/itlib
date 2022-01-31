#------------------------
# BLOKOUT2.MAK make file
#------------------------

blokout2.exe : blokout2.obj
     $(LINKER) $(GUIFLAGS) -OUT:blokout2.exe blokout2.obj $(GUILIBS)

blokout2.obj : blokout2.c
     $(CC) $(CFLAGS) blokout2.c
