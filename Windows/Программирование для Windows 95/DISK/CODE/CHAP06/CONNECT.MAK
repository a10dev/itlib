#-----------------------
# CONNECT.MAK make file
#-----------------------

connect.exe : connect.obj
     $(LINKER) $(GUIFLAGS) -OUT:connect.exe connect.obj $(GUILIBS)

connect.obj : connect.c
     $(CC) $(CFLAGS) connect.c
