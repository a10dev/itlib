!	Bootblock 1.2 - Minix boot block.		Author: Kees J. Bot
!					   Floppy sensing code: Guy Helmer
!
! When the PC is powered on, it will try to read the first sector of floppy
! disk 0 into address 0x7C00.  If this fails due to the absence of flexible
! magnetic media, it will read the master boot record from the first sector
! of the hard disk.  This sector not only contains executable code, but also
! the partition table of the hard disk.  When executed, it will select the
! active partition and load the first sector of that into address 0x7C00.
! This file contains the code that is eventually read from either the floppy
! disk, or the hard disk partition.  It is just smart enough to load the
! secondary boot code from the boot device into memory at address 0x10000 and
! execute that.  The disk addresses for this secondary boot code are patched
! into this code by installboot as 24-bit sector numbers and 8-bit sector
! counts above enddata upwards.  The secondary boot code is in turn smart
! enough to load the different parts of the Minix kernel into memory and
! execute them to finally get Minix started.
!
! Kees J. Bot - 91/12/21:
! Adapted Guy Helmers code and added hard disk support for my boot monitor
! package.
!

	LOADOFF	   =	0x7C00	! 0x0000:LOADOFF is where this code is loaded
	BOOTSEG    =	0x1000	! Secondary boot code segment.
	BOOTOFF	   =	0x0030	! Offset into secondary boot above header
	BUFFER	   =	0x0600	! First free memory
	DSKBASE    =	  0x1E	! Floppy disk parameter vector
	DSKPARSIZE =	    11	! 11 bytes of floppy parameters
	SECTORS	   =	     4	! Offset into parameters to sectors per track
	LOWSEC     =	     8	! Offset of logical first sector in partition
				! table

	! Variables addressed using bp register
	device	   =	     0	! The boot device
	lowsec	   =	     2	! Offset of boot partition within drive
	secpcyl	   =	     6	! Sectors per cylinder = heads * sectors

.define begtext, begdata, begbss, endtext, enddata, endbss, _main
.data
begdata:
.bss
begbss:
.text
begtext:
_main:

! Start boot procedure.

boot:
	xor	ax, ax		! ax = 0x0000, the vector segment
	mov	ds, ax
	cli			! Ignore interrupts while setting stack
	mov	ss, ax		! ss = ds = vector segment
	mov	sp, #LOADOFF	! Usual place for a bootstrap stack
	sti

	push	ax
	push	ax		! Push a zero lowsec(bp)

	push	dx		! Boot device in dl will be device(bp)
	mov	bp, sp		! Using var(bp) is one byte cheaper then var.

	push	es
	push	si		! es:si = partition table entry if hard disk

	mov	di, #LOADOFF+parameters	! char (*di)[DSKPARSIZE] = parameters;

	testb	dl, dl		! Winchester disks if dl >= 0x80
	jge	floppy

winchester:

! Get the offset of the first sector of the boot partition from the partition
! table.  The table is found at es:si, the lowsec parameter at offset LOWSEC.

	eseg
	les	ax, LOWSEC(si)	  ! es:ax = LOWSEC+2(si):LOWSEC(si)
	mov	lowsec+0(bp), ax  ! Low 16 bits of partition's first sector
	mov	lowsec+2(bp), es  ! High 16 bits of partition's first sector

! Get the drive parameters, the number of sectors is bluntly written into the
! floppy disk parameters.

	movb	ah, #0x08	! Code for drive parameters
	int	0x13		! dl still contains drive
	andb	cl, #0x3F	! cl = max sector number (1-origin)
	movb	SECTORS(di), cl	! Number of sectors per track
	incb	dh		! dh = 1 + max head number (0-origin)
	jmp	loadboot

! Floppy:
! Execute three read tests to determine the drive type.  Test for each floppy
! type by reading the last sector on the first track.  If it fails, try a type
! that has less sectors.  Therefore we start with 1.44M (18 sectors) then 1.2M
! (15 sectors) ending with 720K/360K (both 9 sectors).  (The floppy parameters
! of the last two are equal, apart from the motor start time.  This saves us
! the rather painful "try to read track 41" test.)

next:	add	di, #DSKPARSIZE	! Next set of parameters

floppy:	mov	DSKBASE*4+0, di	! Load offset of disk parameters
	mov	DSKBASE*4+2, ds	! Load segment of disk parameters

	xorb	ah, ah		! Reset drive
	int	0x13

	movb	cl, SECTORS(di)	! cl = number of last sector on track

	cmp	di, #LOADOFF+dsdd3  ! No need to do the last 720K/360K test
	jz	success

! Try to read the last sector on track 0

	mov	es, lowsec(bp)	! es = vector segment (lowsec = 0)
	mov	bx, #BUFFER	! es:bx buffer = 0x0000:0x0600
	mov	ax, #0x0201	! Read sector, #sectors = 1
	xorb	ch, ch		! Track 0, last sector
	xorb	dh, dh		! Drive dl, head 0
	int	0x13
	jb	next		! Error, try the next floppy type

success:movb	dh, #2		! Load number of heads for multiply
				! Number of sectors is still in cl

loadboot:
! Load the secondary boot code from the boot device

	movb	al, cl		! al = cl = sectors per track
	mulb	dh		! dh = heads, ax = heads * sectors
	mov	secpcyl(bp), ax	! Sectors per cylinder = heads * sectors

	mov	ax, #BOOTSEG	! Segment to load secondary boot code into
	mov	es, ax
	xor	bx, bx		! Load first sector at es:bx = BOOTSEG:0x0000
	mov	si, #LOADOFF+addresses	! Start of the boot code addresses
load:
	mov	ax, 1(si)	! Get next sector number: low 16 bits
	movb	dl, 3(si)	! Bits 16-23 for your 8GB disk
	xorb	dh, dh		! dx:ax = sector within partition
	add	ax, lowsec+0(bp)
	adc	dx, lowsec+2(bp)! dx:ax = sector within drive
	div	secpcyl(bp)	! ax = cylinder, dx = sector within cylinder
	xchg	ax, dx		! ax = sector within cylinder, dx = cylinder
	movb	ch, dl		! ch = low 8 bits of cylinder
	divb	SECTORS(di)	! al = head, ah = sector (0-origin)
	xorb	dl, dl		! About to shift bits 8-9 of cylinder into dl
	shr	dx, #1
	shr	dx, #1		! dl[6..7] = high cylinder
	orb	dl, ah		! dl[0..5] = sector (0-origin)
	movb	cl, dl		! cl[0..5] = sector, cl[6..7] = high cyl
	incb	cl		! cl[0..5] = sector (1-origin)
	movb	dh, al		! dh = al = head
	movb	dl, device(bp)	! dl = device to read
	movb	al, SECTORS(di)	! Sectors per track - Sector number (0-origin)
	subb	al, ah		! = Sectors left on this track
	cmpb	al, (si)	! Compare with # sectors to read
	jbe	read		! Can't read past the end of a cylinder?
	movb	al, (si)	! (si) < sectors left on this track
read:	push	ax		! Save al = sectors to read
	movb	ah, #2		! Code for disk read (all registers in use now!)
	int	0x13		! Call the BIOS for a read
	pop	cx		! Restore al in cl
	jb	error		! Jump on disk read error
	movb	al, cl		! Restore al = sectors read
	addb	bh, al		! bx += 2 * al * 256 (add bytes read)
	addb	bh, al		! es:bx = where next sector must be read
	add	1(si), ax	! Update address by sectors read
	adcb	3(si), ah	! Don't forget bits 16-23 (add ah = 0)
	subb	(si), al	! Decrement sector count by sectors read
	jnz	load		! Not all sectors have been read
	add	si, #4		! Next (address, count) pair
	cmpb	ah, (si)	! Done when no sectors to read
	jnz	load		! Read next chunk of secondary boot code

done:

! Call secondary boot, assuming a long a.out header (48 bytes).  The a.out
! header is usually short (32 bytes), but secondary boot has two entry points:
! One at offset 0 for the long, and one at offset 16 for the short header.
! Parameters passed in registers are:
!
!	dl	= Boot-device.
!	es:si	= Partition table entry if hard disk.
!
	pop	si		! Restore es:si = partition table entry
	pop	es		! dl is still loaded
	jmpf	BOOTOFF, BOOTSEG  ! jmp to sec. boot (skipping header).

! Read error: print message, hang forever
error:
!	mov	si, #LOADOFF+errno+1	! Uncomment this at disaster time
!prnum:	movb	al, ah		! Error number in ah
!	andb	al, #0x0F	! Low 4 bits
!	cmpb	al, #10		! A-F?
!	jb	digit		! 0-9!
!	addb	al, #7		! 'A' - ':'
!digit:	addb	(si), al	! Modify '0' in string
!	dec	si
!	movb	cl, #4		! Next 4 bits
!	shrb	ah, cl
!	jnz	prnum		! Again if digit > 0

	mov	si, #LOADOFF+rderr  ! String to print
print:	lodsb			! al = *si++ is char to be printed
	movb	ah, #14		! 14 = print char
	mov	bx, #0x0001	! Page 0, foreground color
	int	0x10		! Call BIOS VIDEO_IO
	cmp	si, #LOADOFF+errend  ! End of string reached?
	jb	print

! Hang forever waiting for CTRL-ALT-DEL
hang:	jmp	hang

.data
rderr:	.ascii	"Read error "
!errno:	.ascii	"00 "
errend:

parameters:
! Floppy disk parameters sorted down by sectors per track.  (The format gap
! length params of the 3.5" disks might be wrong, but that won't matter.)

! 1.44M 3.5"
dshd3:	.data1	0xAF, 0x02, 25, 2, 18, 0x1B, 0xFF, 0x54, 0xF6, 15, 8

! 1.2M 5.25"
dshd5:	.data1	0xDF, 0x02, 25, 2, 15, 0x1B, 0xFF, 0x54, 0xF6, 15, 8

! 720K 3.5", also used for 360K 5.25"
dsdd3:	.data1	0xDF, 0x02, 25, 2,  9, 0x2A, 0xFF, 0x50, 0xF6, 15, 8

! Just for completeness, here are the real 360K params.
!dsdd5:	.data1	0xDF, 0x02, 25, 2,  9, 0x2A, 0xFF, 0x54, 0xF6, 15, 3

.text
endtext:
.data
enddata:
addresses:
! The space below this is for disk addresses for a 62K secondary boot
! program (worst case, i.e. file is fragmented).  It should be enough.
.bss
endbss:
