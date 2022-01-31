#-----------------------
# POPPAD1.MAK make file
#-----------------------

poppad1.exe : poppad1.obj
     $(LINKER) $(GUIFLAGS) -OUT:poppad1.exe poppad1.obj $(GUILIBS)

poppad1.obj : poppad1.c
     $(CC) $(CFLAGS) poppad1.c
