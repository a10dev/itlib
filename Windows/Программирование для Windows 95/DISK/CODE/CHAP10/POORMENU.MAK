#------------------------
# POORMENU.MAK make file
#------------------------

poormenu.exe : poormenu.obj
     $(LINKER) $(GUIFLAGS) -OUT:poormenu.exe poormenu.obj $(GUILIBS)

poormenu.obj : poormenu.c
     $(CC) $(CFLAGS) poormenu.c
