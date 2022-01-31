#------------------------
# DIGCLOCK.MAK make file
#------------------------

digclock.exe : digclock.obj
     $(LINKER) $(GUIFLAGS) -OUT:digclock.exe digclock.obj $(GUILIBS)

digclock.obj : digclock.c
     $(CC) $(CFLAGS) digclock.c
