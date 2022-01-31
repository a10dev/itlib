#----------------------
# PUBMEM.MAK make file
#----------------------

pubmem.dll : pubmem.obj classfac.obj compobj.obj 
     $(LINKER) /EXPORT:DllGetClassObject /EXPORT:DllCanUnloadNow \
     $(DLLFLAGS) -OUT:pubmem.dll pubmem.obj \
	 classfac.obj compobj.obj $(GUILIBS) uuid.lib ole32.lib

pubmem.obj : pubmem.cpp pubmem.h
     $(CC) $(CFLAGS) pubmem.cpp

classfac.obj : classfac.cpp pubmem.h
     $(CC) $(CFLAGS) classfac.cpp

compobj.obj : compobj.cpp pubmem.h
     $(CC) $(CFLAGS) compobj.cpp
