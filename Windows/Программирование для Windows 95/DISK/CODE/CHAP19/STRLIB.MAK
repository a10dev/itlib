#----------------------
# STRLIB.MAK make file
#----------------------

strlib.dll : strlib.obj
     $(LINKER) $(DLLFLAGS) -SECTION:shared,rws -OUT:strlib.dll \
               strlib.obj $(GUILIBS)

strlib.obj : strlib.c strlib.h
     $(CC) $(CFLAGS) strlib.c
