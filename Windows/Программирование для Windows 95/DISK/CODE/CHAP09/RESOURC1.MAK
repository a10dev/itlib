#------------------------
# RESOURC1.MAK make file
#------------------------

resourc1.exe : resourc1.obj resourc1.res
     $(LINKER) $(GUIFLAGS) -OUT:resourc1.exe \
	 resourc1.obj resourc1.res $(GUILIBS)

resourc1.obj : resourc1.c
     $(CC) $(CFLAGS) resourc1.c

resourc1.res : resourc1.rc resourc1.ico resourc1.cur
     $(RC) $(RCVARS) resourc1.rc
