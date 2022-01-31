#---------------------
# TYPER.MAK make file
#---------------------

typer.exe : typer.obj
     $(LINKER) $(GUIFLAGS) -OUT:typer.exe typer.obj $(GUILIBS)

typer.obj : typer.c
     $(CC) $(CFLAGS) typer.c
