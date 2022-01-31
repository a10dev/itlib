#----------------------
# ABOUT2.MAK make file
#----------------------

about2.exe : about2.obj about2.res
     $(LINKER) $(GUIFLAGS) -OUT:about2.exe about2.obj about2.res $(GUILIBS)

about2.obj : about2.c about2.h
     $(CC) $(CFLAGS) about2.c

about2.res : about2.rc about2.h about2.ico
     $(RC) $(RCVARS) about2.rc
