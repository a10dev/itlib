# Makefile for the kernel image.

u=/usr
CC=	exec cc
CFLAGS=	-O -D_MINIX -D_POSIX_SOURCE
MDEC=	/usr/mdec
MAKE=	exec make -$(MAKEFLAGS)

# Specify the programs that are part of the system image.
# Multiple boot medium drivers can be included in the image, 
# but only one is actually be loaded. The driver to load is
# determined by the 'label=...' boot monitor parameter. 
PROGRAMS=	../kernel/kernel \
	../servers/pm/pm \
	../servers/fs/fs \
	../servers/rs/rs \
	../drivers/tty/tty \
	../drivers/memory/memory \
	../drivers/log/log \
	AT:../drivers/at_wini/at_wini \
	../servers/init/init \
#	bootdev.img


usage:	
	@echo " " >&2
	@echo "This is the Makefile to recompile the MINIX 3 BOOK version." >& 2
	@echo "Change to the standard /usr/include/src directory to compile" >& 2
	@echo "system libraries, user commands, or a full Minix 3 system." >& 2
	@echo "You should be logged in as bin to recompile Minix 3." >&2
	@echo " " >&2
	@echo "Usage:" >&2
	@echo "	make includes   # Install BOOK version include files" >&2
	@echo "	make image      # Make needed services and create boot image" >&2
	@echo "	make clean      # Remove all compiler results, except libs" >&2
	@echo " " >&2
	@echo " " >&2

all: services image

image:  includes
	cd ../kernel && $(MAKE) 
	cd ../servers && $(MAKE) image
	cd ../drivers && $(MAKE) image
	installboot -image $@ $(PROGRAMS)

includes:
	cd ../include && $(MAKE) install

services: includes 
	cd ../kernel && $(MAKE) 
	cd ../servers && $(MAKE) install
	cd ../drivers && $(MAKE) install

# clean up compile results
clean:
	cd ../kernel && $(MAKE) $@
	cd ../servers && $(MAKE) $@
	cd ../drivers && $(MAKE) $@
	rm -f *.bak image image_small *.iso *.iso.gz cdfdimage rootimage
