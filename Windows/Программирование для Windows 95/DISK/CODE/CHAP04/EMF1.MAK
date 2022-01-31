#--------------------
# EMF1.MAK make file
#--------------------

emf1.exe : emf1.obj
     $(LINKER) $(GUIFLAGS) -OUT:emf1.exe emf1.obj $(GUILIBS)

emf1.obj : emf1.c
     $(CC) $(CFLAGS) emf1.c
