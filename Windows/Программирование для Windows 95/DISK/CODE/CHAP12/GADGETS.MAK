#-----------------------
# GADGETS.MAK make file
#-----------------------

gadgets.exe : gadgets.obj notifdef.obj statbar.obj \
              toolbar.obj tooltip.obj gadgets.res
     $(LINKER) $(GUIFLAGS) -OUT:gadgets.exe gadgets.obj \
	 notifdef.obj statbar.obj toolbar.obj tooltip.obj \
	 gadgets.res $(GUILIBS)

gadgets.obj : gadgets.c comcthlp.h gadgets.h
     $(CC) $(CFLAGS) gadgets.c

notifdef.obj : notifdef.c notifdef.h
     $(CC) $(CFLAGS) notifdef.c

statbar.obj : statbar.c comcthlp.h gadgets.h
     $(CC) $(CFLAGS) statbar.c

toolbar.obj : toolbar.c comcthlp.h gadgets.h notifdef.h
     $(CC) $(CFLAGS) toolbar.c

tooltip.obj : tooltip.c comcthlp.h gadgets.h notifdef.h
     $(CC) $(CFLAGS) tooltip.c

gadgets.res : gadgets.rc gadgets.ico
     $(RC) $(RCVARS) gadgets.rc
