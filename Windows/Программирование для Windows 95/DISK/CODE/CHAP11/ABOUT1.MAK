#----------------------
# ABOUT1.MAK make file
#----------------------

about1.exe : about1.obj about1.res
     $(LINKER) $(GUIFLAGS) -OUT:about1.exe about1.obj about1.res $(GUILIBS)

about1.obj : about1.c about1.h
     $(CC) $(CFLAGS) about1.c

about1.res : about1.rc about1.h about1.ico
     $(RC) $(RCVARS) about1.rc
