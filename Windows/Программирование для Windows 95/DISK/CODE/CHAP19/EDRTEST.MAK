#-----------------------
# EDRTEST.MAK make file
#-----------------------

edrtest.exe : edrtest.obj edrlib.lib
     $(LINKER) $(GUIFLAGS) -OUT:edrtest.exe edrtest.obj edrlib.lib $(GUILIBS)

edrtest.obj : edrtest.c edrlib.h
     $(CC) $(CFLAGS) edrtest.c
