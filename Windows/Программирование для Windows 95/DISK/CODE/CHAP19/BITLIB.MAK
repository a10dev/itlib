#----------------------
# BITLIB.MAK make file
#----------------------

bitlib.dll : bitlib.obj bitlib.res
     $(LINKER) $(DLLFLAGS) -OUT:bitlib.dll bitlib.obj bitlib.res $(GUILIBS)

bitlib.obj : bitlib.c
     $(CC) $(CFLAGS) bitlib.c

bitlib.res : bitlib.rc
     $(RC) $(RCVARS) bitlib.rc
