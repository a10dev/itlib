#------------------------
# CLIPVIEW.MAK make file
#------------------------

clipview.exe : clipview.obj
     $(LINKER) $(GUIFLAGS) -OUT:clipview.exe clipview.obj $(GUILIBS)

clipview.obj : clipview.c
     $(CC) $(CFLAGS) clipview.c
