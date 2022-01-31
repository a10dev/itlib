#------------------------
# PROPERTY.MAK make file
#------------------------

property.exe : property.obj helper.obj notify.obj \
               page1.obj page2.obj sheet.obj property.res
     $(LINKER) $(GUIFLAGS) -OUT:property.exe property.obj \
	 helper.obj notify.obj page1.obj page2.obj sheet.obj \
	 property.res $(GUILIBS)

property.obj : property.c comcthlp.h property.h
     $(CC) $(CFLAGS) property.c

helper.obj : helper.c property.h
     $(CC) $(CFLAGS) helper.c

notify.obj : notify.c notify.h
     $(CC) $(CFLAGS) notify.c

page1.obj : page1.c helper.h notify.h property.h
     $(CC) $(CFLAGS) page1.c

page2.obj : page2.c helper.h notify.h property.h
     $(CC) $(CFLAGS) page2.c

sheet.obj : sheet.c comcthlp.h property.h
     $(CC) $(CFLAGS) sheet.c

property.res : property.rc property.ico
     $(RC) $(RCVARS) property.rc
