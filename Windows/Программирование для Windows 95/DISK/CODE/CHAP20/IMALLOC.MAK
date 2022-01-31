#-----------------------
# IMALLOC.MAK make file
#-----------------------

imalloc.dll : imalloc.obj
     $(LINKER) $(DLLFLAGS) -OUT:imalloc.dll imalloc.obj $(GUILIBS) uuid.lib

imalloc.obj : imalloc.cpp
     $(CC) $(CFLAGS) imalloc.cpp
