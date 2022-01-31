#-----------------------
# DDEPOP1.MAK make file
#-----------------------

ddepop1.exe : ddepop1.obj ddepop1.res
     $(LINKER) $(GUIFLAGS) -OUT:ddepop1.exe ddepop1.obj ddepop1.res $(GUILIBS)

ddepop1.obj : ddepop1.c ddepop.h
     $(CC) $(CFLAGS) ddepop1.c

ddepop1.res : ddepop1.rc ddepop.ico
     $(RC) $(RCVARS) ddepop1.rc
