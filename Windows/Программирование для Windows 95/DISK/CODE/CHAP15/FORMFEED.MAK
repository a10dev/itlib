#------------------------
# FORMFEED.MAK make file
#------------------------
 
formfeed.exe : formfeed.obj
     $(LINKER) $(GUIFLAGS) -OUT:formfeed.exe formfeed.obj \
	 $(GUILIBS) winspool.lib
 
formfeed.obj : formfeed.c
     $(CC) $(CFLAGS) formfeed.c
