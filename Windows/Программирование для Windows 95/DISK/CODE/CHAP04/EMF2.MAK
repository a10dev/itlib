#--------------------
# EMF2.MAK make file
#--------------------

emf2.exe : emf2.obj
     $(LINKER) $(GUIFLAGS) -OUT:emf2.exe emf2.obj $(GUILIBS)

emf2.obj : emf2.c
     $(CC) $(CFLAGS) emf2.c
