#-----------------------
# POEPOEM.MAK make file
#-----------------------

poepoem.exe : poepoem.obj poepoem.res
     $(LINKER) $(GUIFLAGS) -OUT:poepoem.exe poepoem.obj poepoem.res $(GUILIBS)

poepoem.obj : poepoem.c poepoem.h
     $(CC) $(CFLAGS) poepoem.c

poepoem.res : poepoem.rc poepoem.ico poepoem.asc poepoem.h
     $(RC) $(RCVARS) poepoem.rc
