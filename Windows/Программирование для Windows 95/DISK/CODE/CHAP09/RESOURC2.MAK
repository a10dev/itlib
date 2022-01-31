#------------------------
# RESOURC2.MAK make file
#------------------------

resourc2.exe : resourc2.obj resourc2.res
     $(LINKER) $(GUIFLAGS) -OUT:resourc2.exe \
	 resourc2.obj resourc2.res $(GUILIBS)

resourc2.obj : resourc2.c
     $(CC) $(CFLAGS) resourc2.c

resourc2.res : resourc2.rc resourc2.ico resourc2.cur resourc2.bmp
     $(RC) $(RCVARS) resourc2.rc
