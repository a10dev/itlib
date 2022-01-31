#----------------------
# EDRLIB.MAK make file
#----------------------

edrlib.dll : edrlib.obj
     $(LINKER) $(DLLFLAGS) -OUT:edrlib.dll edrlib.obj $(GUILIBS)

edrlib.obj : edrlib.c edrlib.h
     $(CC) $(CFLAGS) edrlib.c
