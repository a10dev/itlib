#-----------------------
# DDEPOP2.MAK make file
#-----------------------

ddepop2.exe : ddepop2.obj ddepop2.res
     $(LINKER) $(GUIFLAGS) -OUT:ddepop2.exe ddepop2.obj ddepop2.res $(GUILIBS)

ddepop2.obj : ddepop2.c ddepop.h
     $(CC) $(CFLAGS) ddepop2.c

ddepop2.res : ddepop2.rc ddepop.ico
     $(RC) $(RCVARS) ddepop2.rc
