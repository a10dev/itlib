#-----------------------
# HEXCALC.MAK make file
#-----------------------

hexcalc.exe : hexcalc.obj hexcalc.res
     $(LINKER) $(GUIFLAGS) -OUT:hexcalc.exe hexcalc.obj hexcalc.res $(GUILIBS)

hexcalc.obj : hexcalc.c
     $(CC) $(CFLAGS) hexcalc.c

hexcalc.res : hexcalc.rc hexcalc.ico
     $(RC) $(RCVARS) hexcalc.rc
