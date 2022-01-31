#----------------------
# ABOUT3.MAK make file
#----------------------

about3.exe : about3.obj about3.res
     $(LINKER) $(GUIFLAGS) -OUT:about3.exe about3.obj about3.res $(GUILIBS)

about3.obj : about3.c about3.h
     $(CC) $(CFLAGS) about3.c

about3.res : about3.rc about3.h about3.ico
     $(RC) $(RCVARS) about3.rc
