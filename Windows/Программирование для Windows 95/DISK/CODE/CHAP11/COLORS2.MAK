#-----------------------
# COLORS2.MAK make file
#-----------------------

colors2.exe : colors2.obj colors2.res
     $(LINKER) $(GUIFLAGS) -OUT:colors2.exe colors2.obj colors2.res $(GUILIBS)

colors2.obj : colors2.c
     $(CC) $(CFLAGS) colors2.c

colors2.res : colors2.rc
     $(RC) $(RCVARS) colors2.rc
