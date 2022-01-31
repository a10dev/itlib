#-----------------------
# BTNLOOK.MAK make file
#-----------------------

btnlook.exe : btnlook.obj
     $(LINKER) $(GUIFLAGS) -OUT:btnlook.exe btnlook.obj $(GUILIBS)

btnlook.obj : btnlook.c
     $(CC) $(CFLAGS) btnlook.c
