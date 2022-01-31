/*	boot 2.5.0 - Load and start Minix.		Author: Kees J. Bot
 *								27 Dec 1991
 *
 * Copyright 1996 Kees J. Bot, All rights reserved.
 * This package may be freely used and modified except that changes that
 * do not increase the functionality or that are incompatible with the
 * original may not be released to the public without permission from the
 * author.  Use of so called "C beautifiers" is explicitly prohibited.
 */

char version[]=		"2.5";

#define nil 0
#define _POSIX_SOURCE	1
#define _MINIX		1
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <a.out.h>
#include <minix/config.h>
#include <minix/const.h>
#include <minix/type.h>
#include <minix/minlib.h>
#include <kernel/const.h>
#include <kernel/type.h>
#include <ibm/partition.h>
#include "rawfs.h"
#undef EXTERN
#define EXTERN	/* Empty */
#include "boot.h"

#define arraysize(a)		(sizeof(a) / sizeof((a)[0]))
#define arraylimit(a)		((a) + arraysize(a))
#define between(a, c, z)	((unsigned) ((c) - (a)) <= ((z) - (a)))

void printk(char *fmt, ...);
#define	printf	printk

char *bios_err(int err)
/* Translate BIOS error code to a readable string.  (This is a rare trait
 * known as error checking and reporting.  Take a good look at it, you won't
 * see it often.)
 */
{
	static struct errlist {
		short	err;
		char	*what;
	} errlist[] = {
#if !DOS
		{ 0x00, "No error" },
		{ 0x01, "Invalid command" },
		{ 0x02, "Address mark not found" },
		{ 0x03, "Disk write-protected" },
		{ 0x04, "Sector not found" },
		{ 0x05, "Reset failed" },
		{ 0x06, "Floppy disk removed" },
		{ 0x07, "Bad parameter table" },
		{ 0x08, "DMA overrun" },
		{ 0x09, "DMA crossed 64 KB boundary" },
		{ 0x0A, "Bad sector flag" },
		{ 0x0B, "Bad track flag" },
		{ 0x0C, "Media type not found" },
		{ 0x0D, "Invalid number of sectors on format" },
		{ 0x0E, "Control data address mark detected" },
		{ 0x0F, "DMA arbitration level out of range" },
		{ 0x10, "Uncorrectable CRC or ECC data error" },
		{ 0x11, "ECC corrected data error" },
		{ 0x20, "Controller failed" },
		{ 0x40, "Seek failed" },
		{ 0x80, "Disk timed-out" },
		{ 0xAA, "Drive not ready" },
		{ 0xBB, "Undefined error" },
		{ 0xCC, "Write fault" },
		{ 0xE0, "Status register error" },
		{ 0xFF, "Sense operation failed" }
#else /* DOS */
		{ 0x00, "No error" },
		{ 0x01, "Function number invalid" },
		{ 0x02, "File not found" },
		{ 0x03, "Path not found" },
		{ 0x04, "Too many open files" },
		{ 0x05, "I/O error" },
		{ 0x06, "Invalid handle" },
		{ 0x0C, "Access code invalid" },
#endif /* DOS */
	};
	struct errlist *errp;

	for (errp= errlist; errp < arraylimit(errlist); errp++) {
		if (errp->err == err) return errp->what;
	}
	return "Unknown error";
}

char *unix_err(int err)
/* Translate the few errors rawfs can give. */
{
	switch (err) {
	case ENOENT:	return "No such file or directory";
	case ENOTDIR:	return "Not a directory";
	default:	return "Unknown error";
	}
}

void rwerr(char *rw, off_t sec, int err)
{
	printf("\n%s error 0x%02x (%s) at sector %ld absolute\n",
		rw, err, bios_err(err), sec);
}

void readerr(off_t sec, int err)	{ rwerr("Read", sec, err); }
void writerr(off_t sec, int err)	{ rwerr("Write", sec, err); }

/* Readblock support for rawfs.c */

#define CACHE_SIZE	32	/* More than enough. */

struct cache_entry {
	u32_t	block;
	u32_t	addr;
} cache[CACHE_SIZE];

#if !DOS
int cache_live= 0;

void init_cache(void)
/* Initialize the block cache. */
{
	struct cache_entry *pc;
	u32_t addr= FREEPOS;

	for (pc= cache; pc < arraylimit(cache); pc++) {
		pc->block= -1;
		pc->addr= addr;
		addr+= BLOCK_SIZE;
	}

	cache_live= 1;	/* Turn it on. */
}

void invalidate_cache(void)
/* The cache can't be used when Minix is loaded. */
{
	cache_live= 0;
}

#else /* DOS */
/* We can't fool around with random memory under DOS. */
#define cache_live 0
void init_cache(void) {}
void invalidate_cache(void) {}
#endif /* DOS */

void readblock(off_t blk, char *buf)
/* Read blocks for the rawfs package with caching.  Wins 2 seconds. */
{
	int r= 0;
	u32_t sec= lowsec + blk * RATIO;

	if (!cache_live) {
		/* Cache invalidated, load block directly in place. */
		r= readsectors(mon2abs(buf), sec, 1 * RATIO);
	} else {
		/* Search through the cache from 0 up.  Move the one found
		 * to the front of the cache, then optionally read a block.
		 */
		struct cache_entry *pc, lifo, tmp;

		for (pc= cache; pc < arraylimit(cache); pc++) {
			tmp= *pc;
			*pc= lifo;
			lifo= tmp;
			if (lifo.block == blk) break;
		}
		cache[0]= lifo;

		if (cache[0].block != blk) {
			r= readsectors(cache[0].addr, sec, 1 * RATIO);
			cache[0].block= blk;
		}
		raw_copy(mon2abs(buf), cache[0].addr, BLOCK_SIZE);
	}
	if (r != 0) { readerr(sec, r); exit(1); }
}

char *readline(void)
/* Read a line including a newline with echoing. */
{
	char *line;
	size_t i, z;
	int c;

	i= 0;
	z= 20;
	line= malloc(z * sizeof(char));

	do {
		c= getchar();

		if (strchr("\b\177\25\30", c) != nil) {
			/* Backspace, DEL, ctrl-U, or ctrl-X. */
			do {
				if (i == 0) break;
				printf("\b \b");
				i--;
			} while (c == '\25' || c == '\30');
		} else
		if (c < ' ' && c != '\n') {
			putchar('\7');
		} else {
			putchar(c);
			line[i++]= c;
			if (i == z) {
				z*= 2;
				line= realloc(line, z * sizeof(char));
			}
		}
	} while (c != '\n');
	line[i]= 0;
	return line;
}

int sugar(char *tok)
/* Recognize special tokens. */
{
	return strchr("=(){};\n", tok[0]) != nil;
}

char *onetoken(char **aline, int arg)
/* Returns a string with one token for tokenize.  Arg is true when reading
 * between ( and ).
 */
{
	char *line= *aline;
	size_t n;
	char *tok;

	/* Skip spaces and runs of newlines. */
	while (*line == ' ' || (*line == '\n' && line[1] == '\n')) line++;

	*aline= line;

	/* Don't do odd junk (nor the terminating 0!). */
	if ((unsigned) *line < ' ' && *line != '\n') return nil;

	if (arg) {
		/* Function argument, anything goes except ). */
		int depth= 0;

		while ((unsigned) *line >= ' ') {
			if (*line == '(') depth++;
			if (*line == ')' && --depth < 0) break;
			line++;
		}
		while (line > *aline && line[-1] == ' ') line--;
	} else
	if (sugar(line)) {
		/* Single character token. */
		line++;
	} else {
		/* Multicharacter token. */
		do line++; while ((unsigned) *line > ' ' && !sugar(line));
	}
	n= line - *aline;
	tok= malloc((n + 1) * sizeof(char));
	memcpy(tok, *aline, n);
	tok[n]= 0;
	if (tok[0] == '\n') tok[0]= ';';	/* ';' same as '\n' */

	*aline= line;
	return tok;
}

/* Typed commands form strings of tokens. */

typedef struct token {
	struct token	*next;	/* Next in a command chain. */
	char		*token;
} token;

token **tokenize(token **acmds, char *line, int *fundef)
/* Takes a line apart to form tokens.  The tokens are inserted into a command
 * chain at *acmds.  Tokenize returns a reference to where another line could
 * be added.  The fundef variable holds the state tokenize is in when decoding
 * a multiline function definition.  It is nonzero when more must be read.
 * Tokenize looks at spaces as token separators, and recognizes only
 * ';', '=', '(', ')' '{', '}', and '\n' as single character tokens.
 */
{
	int fd= *fundef;
	char *tok;
	token *newcmd;
	static char funsugar[]= { '(', 0, ')', '{', '}' };

	while ((tok= onetoken(&line, fd == 1)) != nil) {
		if (fd == 1) {
			fd++;	/* Function argument. */
		} else
		if (funsugar[fd] == tok[0]) {
			/* Recognize next token as part of a function def. */
			fd= tok[0] == '}' ? 0 : fd + 1;
		} else
		if (fd != 0) {
			if (tok[0] == ';' && fd == 3) {
				/* Kill separator between ')' and '{'. */
				free(tok);
				continue;
			}
			/* Syntax error unless between '{' and '}'. */
			if (fd != 4) fd= 0;
		}
		newcmd= malloc(sizeof(*newcmd));
		newcmd->token= tok;
		newcmd->next= *acmds;
		*acmds= newcmd;
		acmds= &newcmd->next;
	}
	*fundef= fd;
	return acmds;
}

token *cmds;		/* String of commands to execute. */
int err;		/* Set on an error. */

char *poptoken(void)
/* Pop one token off the command chain. */
{
	token *cmd= cmds;
	char *tok= cmd->token;

	cmds= cmd->next;
	free(cmd);

	return tok;
}

void voidtoken(void)
/* Remove one token from the command chain. */
{
	free(poptoken());
}

void interrupt(void)
/* Clean up after an ESC has been typed. */
{
	printf("[ESC]\n");
	while (peekchar() == ESC) (void) getchar();
	err= 1;
}

int activate;

struct biosdev {
	char name[6];
	int device, primary, secondary;
} bootdev, tmpdev;

struct part_entry boot_part;
char dskpars[DSKPARSIZE]=	/* 360K floppy disk parameters (for now). */
	{ 0xDF, 0x02, 25, 2,  9, 0x2A, 0xFF, 0x50, 0xF6, 15, 8 };

void migrate(void)
/* Copy the boot program to the far end of memory, this must be done asap to
 * put the data area cleanly inside a 64K chunk (no DMA problems).
 */
{
	u32_t oldaddr= caddr;
	u32_t memsize= get_memsize() * 1024L;
	u32_t newaddr= memsize - runsize;
#if !DOS
	u32_t dma64k= (memsize - 1) & ~0xFFFFL;
	vector dskbase;

	/* Check if data segment crosses a 64k boundary. */
	if (newaddr + (daddr - caddr) < dma64k) newaddr= dma64k - runsize;

	/* Get some variables into my address space before they get mashed. */
	if (device < 0x80) {
		/* Floppy disk parameters. */
		raw_copy(mon2abs(&dskbase), DSKBASE * sizeof(vector),
							sizeof(vector));
		raw_copy(mon2abs(dskpars), vec2abs(&dskbase),
							DSKPARSIZE);
	} else {
		/* Hard disk partition table entry into boot_part. */
		raw_copy(mon2abs(&boot_part), vec2abs(&rem_part),
							sizeof(boot_part));
	}
#endif /* !DOS */

	/* Set the new caddr for relocate. */
	caddr= newaddr;

	/* Copy code and data. */
	raw_copy(newaddr, oldaddr, runsize);

	relocate();	/* Make the copy running. */

#if !DOS
	/* Set the parameters for the boot device using global variables
	 * device and dskpars.  (This particular call should not fail.)
	 */
	(void) dev_geometry();
#endif /* !DOS */
}

int get_master(char *master, struct part_entry **table, u32_t pos)
/* Read a master boot sector and its partition table. */
{
	int r, n;
	struct part_entry *pe, **pt;

	if ((r= readsectors(mon2abs(master), pos, 1)) != 0) return r;

	pe= (struct part_entry *) (master + PART_TABLE_OFF);
	for (pt= table; pt < table + NR_PARTITIONS; pt++) *pt= pe++;

	/* DOS has the misguided idea that partition tables must be sorted. */
	if (pos != 0) return 0;		/* But only the primary. */

	n= NR_PARTITIONS;
	do {
		for (pt= table; pt < table + NR_PARTITIONS-1; pt++) {
			if (pt[0]->sysind == NO_PART
				|| (pt[0]->lowsec > pt[1]->lowsec
						&& pt[1]->sysind != NO_PART)) {
				pe= pt[0]; pt[0]= pt[1]; pt[1]= pe;
			}
		}
	} while (--n > 0);
	return 0;
}

void initialize(void)
{
	char master[SECTOR_SIZE];
	struct part_entry *table[NR_PARTITIONS];
	int r, p;
	u32_t masterpos;
	static char sub[]= "a";

#if !DOS
	/* Find out what the boot device and partition was. */
	bootdev.name[0]= 0;
	bootdev.device= device;
	bootdev.primary= -1;
	bootdev.secondary= -1;

	if (device < 0x80) {
		/* Floppy. */
		strcpy(bootdev.name, "fd");
		strcat(bootdev.name, u2a(bootdev.device));
		return;
	}

	/* Get the partition table from the very first sector, and determine
	 * the partition we booted from.  Migrate() was so nice to put the
	 * partition table entry of the booted partition in boot_part.
	 */

	/* The only thing really needed from the booted partition: */
	lowsec= boot_part.lowsec;

	masterpos= 0;	/* Master bootsector position. */

	for (;;) {
		/* Extract the partition table from the master boot sector. */
		if ((r= get_master(master, table, masterpos)) != 0) {
			readerr(masterpos, r); exit(1);
		}

		/* See if you can find "lowsec" back. */
		for (p= 0; p < NR_PARTITIONS; p++) {
			if (lowsec - table[p]->lowsec < table[p]->size) break;
		}

		if (lowsec == table[p]->lowsec) {	/* Found! */
			if (bootdev.primary < 0)
				bootdev.primary= p;
			else
				bootdev.secondary= p;
			break;
		}

		if (p == NR_PARTITIONS || bootdev.primary >= 0) {
			/* The boot partition cannot be named, this only means
			 * that "bootdev" doesn't work.
			 */
			bootdev.device= -1;
			return;
		}

		/* See if the primary partition is subpartitioned. */
		bootdev.primary= p;
		masterpos= table[p]->lowsec;
	}
	strcpy(bootdev.name, "hd");
	strcat(bootdev.name, u2a((device - 0x80) * (1 + NR_PARTITIONS)
						+ 1 + bootdev.primary));
	sub[0]= 'a' + bootdev.secondary;
	if (bootdev.secondary >= 0) strcat(bootdev.name, sub);

#else /* DOS */
	/* Initialize under DOS: Open virtual disk to boot Minix from, grab
	 * extended memory, etc.
	 */
	char *argp, *vdisk;

	/* Parse the command line. */
	argp= PSP + 0x81;
	argp[PSP[0x80]]= 0;
	while (between('\1', *argp, ' ')) argp++;
	vdisk= argp;
	while (!between('\0', *argp, ' ')) argp++;
	while (between('\1', *argp, ' ')) *argp++= 0;
	if (*argp != 0 || *vdisk == 0) {
		printf("\nUsage: boot <vdisk>\n");
		exit(1);
	}

	if ((r= dos_open(vdisk)) != 0) {
		printf("\n%s: Error %02x (%s)\n", vdisk, r, bios_err(r));
		exit(1);
	}

	/* Find the active partition on the virtual disk. */
	if ((r= get_master(master, table, 0)) != 0) {
		readerr(0, r); exit(1);
	}

	strcpy(bootdev.name, "dosd0");
	bootdev.primary= -1;
	for (p= 0; p < NR_PARTITIONS; p++) {
		if (table[p]->bootind != 0 && table[p]->sysind == MINIX_PART) {
			bootdev.primary= p;
			bootdev.name[4]= '1' + p;
			lowsec= table[p]->lowsec;
			break;
		}
	}
#endif /* DOS */
}

char null[]= "";	/* This kludge saves lots of memory. */

void sfree(char *s)
/* Free a non-null string. */
{
	if (s != nil && s != null) free(s);
}

char *copystr(char *s)
/* Copy a non-null string using malloc. */
{
	char *c;

	if (*s == 0) return null;
	c= malloc((strlen(s) + 1) * sizeof(char));
	strcpy(c, s);
	return c;
}

int is_default(environment *e)
{
	return (e->flags & E_SPECIAL) && e->defval == nil;
}

environment **searchenv(char *name)
{
	environment **aenv= &env;

	while (*aenv != nil && strcmp((*aenv)->name, name) != 0) {
		aenv= &(*aenv)->next;
	}

	return aenv;
}

#define b_getenv(name)	(*searchenv(name))
/* Return the environment *structure* belonging to name, or nil if not found. */

char *b_value(char *name)
/* The value of a variable. */
{
	environment *e= b_getenv(name);

	return e == nil || !(e->flags & E_VAR) ? nil : e->value;
}

char *b_body(char *name)
/* The value of a function. */
{
	environment *e= b_getenv(name);

	return e == nil || !(e->flags & E_FUNCTION) ? nil : e->value;
}

int b_setenv(int flags, char *name, char *arg, char *value)
/* Change the value of an environment variable.  Returns the flags of the
 * variable if you are not allowed to change it, 0 otherwise.
 */
{
	environment **aenv, *e;

	if (*(aenv= searchenv(name)) == nil) {
		e= malloc(sizeof(*e));
		e->name= copystr(name);
		e->flags= flags;
		e->defval= nil;
		e->next= nil;
		*aenv= e;
	} else {
		e= *aenv;

		/* Don't touch reserved names and don't change special
		 * variables to functions or vv.
		 */
		if (e->flags & E_RESERVED || (e->flags & E_SPECIAL
			&& (e->flags & E_FUNCTION) != (flags & E_FUNCTION)
		)) return e->flags;

		e->flags= (e->flags & E_STICKY) | flags;
		if (is_default(e)) {
			e->defval= e->value;
		} else {
			sfree(e->value);
		}
		sfree(e->arg);
	}
	e->arg= copystr(arg);
	e->value= copystr(value);
	return 0;
}

int b_setvar(int flags, char *name, char *value)
/* Set variable or simple function. */
{
	return b_setenv(flags, name, null, value);
}

void b_unset(char *name)
/* Remove a variable from the environment.  A special variable is reset to
 * its default value.
 */
{
	environment **aenv, *e;

	if ((e= *(aenv= searchenv(name))) == nil) return;

	if (e->flags & E_SPECIAL) {
		if (e->defval != nil) {
			sfree(e->arg);
			e->arg= null;
			sfree(e->value);
			e->value= e->defval;
			e->defval= nil;
		}
	} else {
		sfree(e->name);
		sfree(e->arg);
		sfree(e->value);
		*aenv= e->next;
		free(e);
	}
}

long a2l(char *a)
/* Cheap atol(). */
{
	int sign= 1;
	long n= 0;

	if (*a == '-') { sign= -1; a++; }

	while (between('0', *a, '9')) n= n * 10 + (*a++ - '0');

	return sign * n;
}

char *ul2a(u32_t n)
/* Transform a long number to ascii digits. */
{
	static char num[3 * sizeof(n)];
	char *a= arraylimit(num) - 1;

	do *--a = (n % 10) + '0'; while ((n/= 10) > 0);
	return a;
}

char *u2a(U16_t n)
/* Transform a short number to ascii digits. */
{
	return ul2a(n);
}

unsigned a2x(char *a)
/* Ascii to hex. */
{
	unsigned n= 0;
	int c;

	for (;;) {
		c= *a;
		if (between('0', c, '9')) c= c - '0' + 0x0;
		else
		if (between('A', c, 'F')) c= c - 'A' + 0xA;
		else
		if (between('a', c, 'f')) c= c - 'a' + 0xa;
		else
			break;
		n= (n<<4) | c;
		a++;
	}
	return n;
}

void get_parameters(void)
{
	char params[SECTOR_SIZE + 1];
	token **acmds;
	int r, fundef= 0;
	static char bus_type[][4] = {
		"xt", "at", "mca"
	};
	static char vid_type[][4] = {
		"mda", "cga", "ega", "ega", "vga", "vga"
	};
	static char vid_chrome[][6] = {
		"mono", "color"
	};

	/* Variables that Minix needs: */
	b_setvar(E_SPECIAL|E_VAR|E_DEV, "rootdev", "ram");
	b_setvar(E_SPECIAL|E_VAR|E_DEV, "ramimagedev", "bootdev");
	b_setvar(E_SPECIAL|E_VAR, "ramsize", "0");
	b_setvar(E_SPECIAL|E_VAR, "processor", u2a(getprocessor()));
	b_setvar(E_SPECIAL|E_VAR, "bus", bus_type[get_bus()]);
	b_setvar(E_SPECIAL|E_VAR, "memsize", u2a(get_memsize()));
	b_setvar(E_SPECIAL|E_VAR, "emssize", ul2a(get_ext_memsize()));
	b_setvar(E_SPECIAL|E_VAR, "video", vid_type[get_video()]);
	b_setvar(E_SPECIAL|E_VAR, "chrome", vid_chrome[get_video() & 1]);

	/* Variables boot needs: */
	b_setvar(E_SPECIAL|E_VAR, "image", "minix");
	b_setvar(E_SPECIAL|E_FUNCTION, "main", "menu");

	/* Default menu function: */
	b_setenv(E_RESERVED|E_FUNCTION, "\1", "=,Start Minix", "boot");

	/* Reserved names: */
	b_setvar(E_RESERVED, "boot", null);
	b_setvar(E_RESERVED, "menu", null);
	b_setvar(E_RESERVED, "set", null);
	b_setvar(E_RESERVED, "unset", null);
	b_setvar(E_RESERVED, "save", null);
	b_setvar(E_RESERVED, "ls", null);
	b_setvar(E_RESERVED, "echo", null);
	b_setvar(E_RESERVED, "trap", null);
	b_setvar(E_RESERVED, "help", null);
	b_setvar(E_RESERVED, "exit", null);

	/* Tokenize bootparams sector. */
	if ((r= readsectors(mon2abs(params), lowsec+PARAMSEC, 1)) != 0) {
		readerr(lowsec+PARAMSEC, r);
		exit(1);
	}
	params[SECTOR_SIZE]= 0;
	acmds= tokenize(&cmds, params, &fundef);

	/* Stuff the default action into the command chain. */
	(void) tokenize(acmds, ":;main", &fundef);
}

void remote_code(void)
/* A rebooting Minix returns a bit of code for the monitor. */
{
	if (reboot_code != 0) {
		char code[SECTOR_SIZE + 2];
		int fundef= 0;

		raw_copy(mon2abs(code), reboot_code, SECTOR_SIZE);
		code[SECTOR_SIZE]= 0;
		strcat(code, ";");
		(void) tokenize(&cmds, code, &fundef);
		reboot_code= 0;
	}
}

char *addptr;

void addparm(char *n)
{
	while (*n != 0 && *addptr != 0) *addptr++ = *n++;
}

void save_parameters(void)
/* Save nondefault environment variables to the bootparams sector. */
{
	environment *e;
	char params[SECTOR_SIZE + 1];
	int r;

	/* Default filling: */
	memset(params, '\n', SECTOR_SIZE);

	/* Don't touch the 0! */
	params[SECTOR_SIZE]= 0;
	addptr= params;

	for (e= env; e != nil; e= e->next) {
		if (e->flags & E_RESERVED || is_default(e)) continue;

		addparm(e->name);
		if (e->flags & E_FUNCTION) {
			addparm("(");
			addparm(e->arg);
			addparm("){");
		} else {
			addparm((e->flags & (E_DEV|E_SPECIAL)) != E_DEV
							? "=" : "=d ");
		}
		addparm(e->value);
		if (e->flags & E_FUNCTION) addparm("}");
		if (*addptr == 0) {
			printf("The environment is too big\n");
			return;
		}
		*addptr++= '\n';
	}

	/* Save the parameters on disk. */
	if ((r= writesectors(mon2abs(params), lowsec+PARAMSEC, 1)) != 0) {
		writerr(lowsec+PARAMSEC, r);
		printf("Can't save environment\n");
	}
}

void show_env(void)
/* Show the environment settings. */
{
	environment *e;

	for (e= env; e != nil; e= e->next) {
		if (e->flags & E_RESERVED) continue;

		if (e->flags & E_FUNCTION) {
			printf("%s(%s) {%s}\n", e->name, e->arg, e->value);
		} else {
			printf(is_default(e) ? "%s = (%s)\n" : "%s = %s\n",
				e->name, e->value);
		}
	}
}

int numprefix(char *s, char **ps)
/* True iff s is a string of digits.  *ps will be set to the first nondigit
 * if non-nil, otherwise the string should end.
 */
{
	char *n= s;

	while (between('0', *n, '9')) n++;

	if (n == s) return 0;

	if (ps == nil) return *n == 0;

	*ps= n;
	return 1;
}

int numeric(char *s)
{
	return numprefix(s, (char **) nil);
}

/* Device numbers of standard Minix devices. */
#define DEV_RAM		0x0100
#define DEV_FD0		0x0200
#define DEV_HD0		0x0300
#define DEV_SD0  	0x0A00
#define minor_1a	   128

dev_t name2dev(char *name)
/* Translate, say, /dev/hd3 to a device number.  If the name can't be
 * found on the boot device, then do some guesswork.  The global structure
 * "tmpdev" will be filled in based on the name, so that "boot hd6" knows
 * what device to boot without interpreting device numbers.
 */
{
	dev_t dev;
	ino_t ino;
	int drive;
	struct stat st;
	char *n, *s;

	/* "boot *hd3" means: make partition 3 active before you boot it. */
	if ((activate= (name[0] == '*'))) name++;

	/* The special name "bootdev" must be translated to the boot device. */
	if (strcmp(name, "bootdev") == 0) {
		if (bootdev.device == -1) {
			printf("The boot device could not be named\n");
			errno= 0;
			return -1;
		}
		name= bootdev.name;
	}

	/* If our boot device doesn't have a file system, or we want to know
	 * what a name means for the BIOS, then we need to interpret the
	 * device name ourselves: "fd" = floppy, "hd" = hard disk, etc.
	 */
	tmpdev.device= tmpdev.primary= tmpdev.secondary= -1;
	dev= -1;
	n= name;
	if (strncmp(n, "/dev/", 5) == 0) n+= 5;

	if (strcmp(n, "ram") == 0) {
		dev= DEV_RAM;
	} else
	if (n[0] == 'f' && n[1] == 'd' && numeric(n+2)) {
		/* Floppy. */
		tmpdev.device= a2l(n+2);
		dev= DEV_FD0 + tmpdev.device;
	} else
	if ((n[0] == 'h' || n[0] == 's') && n[1] == 'd' && numprefix(n+2, &s)
		&& (*s == 0 || (between('a', *s, 'd') && s[1] == 0))
	) {
		/* Hard disk. */
		dev= a2l(n+2);
		tmpdev.device= dev / (1 + NR_PARTITIONS);
		tmpdev.primary= (dev % (1 + NR_PARTITIONS)) - 1;
		if (*s != 0) {
			/* Subpartition. */
			tmpdev.secondary= *s - 'a';
			dev= minor_1a
				+ (tmpdev.device * NR_PARTITIONS
					+ tmpdev.primary) * NR_PARTITIONS
				+ tmpdev.secondary;
		}
		tmpdev.device+= 0x80;
		dev+= n[0] == 'h' ? DEV_HD0 : DEV_SD0;
	}

	/* Look the name up on the boot device for the UNIX device number. */
	if (fsok) {
		/* The current working directory is "/dev". */
		ino= r_lookup(r_lookup(ROOT_INO, "dev"), name);

		if (ino != 0) {
			/* Name has been found, extract the device number. */
			r_stat(ino, &st);
			if (!S_ISBLK(st.st_mode)) {
				printf("%s is not a block device\n", name);
				errno= 0;
				return (dev_t) -1;
			}
			dev= st.st_rdev;
		}
	}

	if (tmpdev.primary < 0) activate= 0;	/* Careful now! */

	if (dev == -1) {
		printf("Can't recognize '%s' as a device\n", name);
		errno= 0;
	}
	return dev;
}

#if !DOS
#define B_NODEV		-1
#define B_NOSIG		-2

int exec_bootstrap(dev_t dev)
/* Load boot sector from the disk or floppy described by tmpdev and execute it.
 * The floppy parameters may not be right for the floppy we want to read, but
 * reading sector 0 seems to be no problem.
 */
{
	int r, n, dirty= 0;
	char master[SECTOR_SIZE];
	struct part_entry *table[NR_PARTITIONS], dummy, *active= &dummy;
	u32_t masterpos;

	device= tmpdev.device;
	if (!dev_geometry()) return B_NODEV;

	active->lowsec= 0;

	/* Select a partition table entry. */
	while (tmpdev.primary >= 0) {
		masterpos= active->lowsec;

		if ((r= get_master(master, table, masterpos)) != 0) return r;

		active= table[tmpdev.primary];

		/* How does one check a partition table entry? */
		if (active->sysind == NO_PART) return B_NOSIG;

		tmpdev.primary= tmpdev.secondary;
		tmpdev.secondary= -1;
	}

	if (activate && !active->bootind) {
		for (n= 0; n < NR_PARTITIONS; n++) table[n]->bootind= 0;
		active->bootind= ACTIVE_FLAG;
		dirty= 1;
	}

	/* Read the boot sector. */
	if ((r= readsectors(BOOTPOS, active->lowsec, 1)) != 0) return r;

	/* Check signature word. */
	if (get_word(BOOTPOS+SIGNATOFF) != SIGNATURE) return B_NOSIG;

	/* Write the partition table if a member must be made active. */
	if (dirty && (r= writesectors(mon2abs(master), masterpos, 1)) != 0)
		return r;

	bootstrap(device, active);
}

void boot_device(char *devname)
/* Boot the device named by devname. */
{
	dev_t dev= name2dev(devname);
	int save_dev= device;
	int r;

	if (tmpdev.device < 0) {
		if (dev != -1) printf("Can't boot from %s\n", devname);
		return;
	}

	switch (r= exec_bootstrap(dev)) {
	case B_NODEV:
		printf("%s: device not present\n", devname);
		break;
	case B_NOSIG:
		printf("%s is not bootable\n", devname);
		break;
	default:
		printf("Can't boot %s: %s\n", devname, bios_err(r));
	}

	/* Restore boot device setting. */
	device= save_dev;
	(void) dev_geometry();
}

#else /* DOS */

void boot_device(char *devname)
/* No booting of other devices under DOS */
{
	printf("Can't boot devices under MS-DOS\n");
}
#endif /* DOS */

void ls(char *dir)
/* List the contents of a directory. */
{
	ino_t ino;
	struct stat st;
	char name[NAME_MAX+1];

	if (!fsok) return;

	if ((ino= r_lookup(ROOT_INO, dir)) == 0
		|| (r_stat(ino, &st), r_readdir(name)) == -1
	) {
		printf("ls: %s: %s\n", dir, unix_err(errno));
		return;
	}
	(void) r_readdir(name);	/* Skip ".." too. */

	while ((ino= r_readdir(name)) != 0) printf("%s/%s\n", dir, name);
}

u32_t milli_time(void)
{
	return get_tick() * MSEC_PER_TICK;
}

u32_t milli_since(u32_t base)
{
	return (milli_time() + (TICKS_PER_DAY*MSEC_PER_TICK) - base)
			% (TICKS_PER_DAY*MSEC_PER_TICK);
}

char *Thandler;
u32_t Tbase, Tcount;

void unschedule(void)
/* Invalidate a waiting command. */
{
	if (Thandler != nil) {
		free(Thandler);
		Thandler= nil;
	}
}

void schedule(long msec, char *cmd)
/* Schedule command at a certain time from now. */
{
	unschedule();
	Thandler= cmd;
	Tbase= milli_time();
	Tcount= msec;
}

int expired(void)
/* Check if the timer expired.  If so prepend the scheduled command to
 * the command chain and return 1.
 */
{
	int fundef= 0;

	if (Thandler == nil || milli_since(Tbase) < Tcount) return 0;

	(void) tokenize(tokenize(&cmds, Thandler, &fundef), ";", &fundef);
	unschedule();
	return 1;
}

int delay(char *msec)
/* Delay for a given time.  Returns true iff delay was not interrupted.
 * Make sure no time functions are used if msec == 0, because get_tick()
 * may do funny things on the original IBM PC (not the XT!).
 * If msec happens to be the string "swap" then wait till the user hits
 * return after changing diskettes.
 */
{
	int swap= 0;
	u32_t base, count;

	if (strcmp(msec, "swap") == 0) {
		swap= 1;
		count= 0;
		printf("\nInsert the root diskette then hit RETURN\n");
	} else
	if ((count= a2l(msec)) > 0) {
		base= milli_time();
	}

	do {
		switch (peekchar()) {
		case -1:	break;
		case ESC:	interrupt(); return 0;
		case '\n':	swap= 0;
		default:	(void) getchar();
		}
	} while (!expired()
		&& (swap || (count > 0 && milli_since(base) < count))
	);
	return 1;
}

enum whatfun { NOFUN, SELECT, DEFFUN, USERFUN } menufun(environment *e)
{
	if (!(e->flags & E_FUNCTION) || e->arg[0] == 0) return NOFUN;
	if (e->arg[1] != ',') return SELECT;
	return e->flags & E_RESERVED ? DEFFUN : USERFUN;
}

void menu(void)
/* By default:  Show a simple menu.
 * Multiple kernels/images:  Show extra selection options.
 * User defined function:  Kill the defaults and show these.
 * Wait for a keypress and execute the given function.
 */
{
	int fundef= 0, c, def= 1;
	char *choice= nil;
	environment *e;

	/* Just a default menu? */
	for (e= env; e != nil; e= e->next) if (menufun(e) == USERFUN) def= 0;

	printf("\nHit a key as follows:\n\n");

	/* Show the choices. */
	for (e= env; e != nil; e= e->next) {
		switch (menufun(e)) {
		case DEFFUN:
			if (!def) break;
			/*FALL THROUGH*/
		case USERFUN:
			printf("    %c  %s\n", e->arg[0], e->arg+2);
			break;
		case SELECT:
			printf("    %c  Select %s kernel\n", e->arg[0],e->name);
		}
	}

	/* Wait for a keypress. */
	do {
		while (peekchar() == -1) if (expired()) return;

		unschedule();

		if ((c= getchar()) == ESC) { interrupt(); return; }

		for (e= env; e != nil; e= e->next) {
			switch (menufun(e)) {
			case DEFFUN:
				if (!def) break;
			case USERFUN:
			case SELECT:
				if (c == e->arg[0]) choice= e->value;
			}
		}
	} while (choice == nil);

	/* Execute the chosen function. */
	printf("%c\n", c);
	(void) tokenize(&cmds, choice, &fundef);
}

void help(void)
/* Not everyone is a rocket scientist. */
{
	struct help {
		char	*thing;
		char	*help;
	} *pi;
	static struct help info[] = {
		{ nil,	"Names:" },
		{ "rootdev",		"Root device" },
		{ "ramimagedev",	"RAM disk image if root is RAM" },
		{ "ramsize",		"RAM disk size if root is not RAM" },
		{ "bootdev",		"Special name for the boot device" },
		{ "fd0, hd3, hd2a",	"Devices (as in /dev)" },
		{ "image",		"Name of the kernel image" },
		{ "main",		"Startup function" },
		{ nil,	"Commands:" },
		{ "name = [device] value",  "Set environment variable" },
		{ "name() { ... }",	    "Define function" },
		{ "name(key,text) { ... }",
			"A menu function like: minix(=,Start Minix) {boot}" },
		{ "name",		"Call function" },
		{ "boot [device]",	"Boot Minix or another O.S." },
		{ "delay [msec]",	"Delay (500 msec default)" },
		{ "echo word ...",	"Print the words" },
		{ "ls [directory]",	"List contents of directory" },
		{ "menu",		"Choose a menu function" },
		{ "save",		"Save environment" },
		{ "set",		"Show environment" },
		{ "trap msec command",	"Schedule command" },
		{ "unset name ...",	"Unset variable or set to default" },
		{ "exit",		"Exit the Monitor" },
	};

	for (pi= info; pi < arraylimit(info); pi++) {
		if (pi->thing != nil) printf("    %-24s- ", pi->thing);
		printf("%s\n", pi->help);
	}
}

void execute(void)
/* Get one command from the command chain and execute it. */
{
	token *second, *third, *fourth, *fifth, *sep;
	char *name= cmds->token;
	size_t n= 0;

	/* There must be a separator lurking somewhere. */
	for (sep= cmds; sep != nil && sep->token[0] != ';'; sep= sep->next) n++;

	if ((second= cmds->next) != nil
		&& (third= second->next) != nil
		&& (fourth= third->next) != nil)
			fifth= fourth->next;

		/* Null command? */
	if (n == 0) {
		voidtoken();
		return;
	} else
		/* name = [device] value? */
	if ((n == 3 || n == 4)
		&& !sugar(name)
		&& second->token[0] == '='
		&& !sugar(third->token)
		&& (n == 3 || (n == 4 && third->token[0] == 'd'
					&& !sugar(fourth->token)
	))) {
		char *value= third->token;
		int flags= E_VAR;

		if (n == 4) { value= fourth->token; flags|= E_DEV; }

		if ((flags= b_setvar(flags, name, value)) != 0) {
			printf("%s is a %s\n", name,
				flags & E_RESERVED ? "reserved word" :
						"special function");
			err= 1;
		}
		while (cmds != sep) voidtoken();
		return;
	} else
		/* name '(' arg ')' '{' ... '}'? */
	if (n >= 5
		&& !sugar(name)
		&& second->token[0] == '('
		&& fourth->token[0] == ')'
		&& fifth->token[0] == '{'
	) {
		token *fun= fifth->next;
		int ok= 1, flags;
		char *body;
		size_t len= 1;

		sep= fun;
		while (sep != nil && sep->token[0] != '}') {
			len+= strlen(sep->token) + 1;
			sep= sep->next;
		}
		if (sep == nil || (sep= sep->next) == nil
			|| sep->token[0] != ';'
		) ok= 0;

		if (ok) {
			body= malloc(len * sizeof(char));
			*body= 0;

			while (fun->token[0] != '}') {
				strcat(body, fun->token);
				if (!sugar(fun->token)
					&& !sugar(fun->next->token)
				) strcat(body, " ");
				fun= fun->next;
			}

			if ((flags= b_setenv(E_FUNCTION, name,
						third->token, body)) != 0) {
				printf("%s is a %s\n", name,
					flags & E_RESERVED ? "reserved word" :
							"special variable");
				err= 1;
			}
			while (cmds != sep) voidtoken();
			free(body);
			return;
		}
	} else
		/* Command coming up, check if ESC typed. */
	if (peekchar() == ESC) {
		interrupt();
		return;
	} else
		/* unset name ..., echo word ...? */
	if (n >= 1 && (
		strcmp(name, "unset") == 0
		|| strcmp(name, "echo") == 0
	)) {
		int cmd= name[0];
		char *arg= poptoken();

		for (;;) {
			free(arg);
			if (cmds == sep) break;
			arg= poptoken();
			if (cmd == 'u') {
				b_unset(arg);
			} else {
				printf("%s", arg);
				if (cmds != sep) putchar(' ');
			}
		}
		if (cmd == 'e') putchar('\n');
		return;
	} else
		/* boot -opts? */
	if (n == 2 && strcmp(name, "boot") == 0 && second->token[0] == '-') {
		static char optsvar[]= "bootopts";
		(void) b_setvar(E_VAR, optsvar, second->token);
		bootminix();
		b_unset(optsvar);
		voidtoken();
		voidtoken();
		return;
	} else
		/* boot device, ls dir, delay msec? */
	if (n == 2 && (
		strcmp(name, "boot") == 0
		|| strcmp(name, "delay") == 0
		|| strcmp(name, "ls") == 0
	)) {
		if (name[0] == 'b') boot_device(second->token);
		if (name[0] == 'd') (void) delay(second->token);
		if (name[0] == 'l') ls(second->token);
		voidtoken();
		voidtoken();
		return;
	} else
		/* trap msec command? */
	if (n == 3 && strcmp(name, "trap") == 0 && numeric(second->token)) {
		long msec= a2l(second->token);

		voidtoken();
		voidtoken();
		schedule(msec, poptoken());
		return;
	} else
		/* Simple command. */
	if (n == 1) {
		char *cmd= poptoken();
		char *body;
		int fundef= 0;
		int ok= 0;

		if (strcmp(cmd, "boot") == 0) { bootminix(); ok= 1; }
		if (strcmp(cmd, "delay") == 0) { (void) delay("500"); ok= 1; }
		if (strcmp(cmd, "ls") == 0) { ls(null); ok= 1; }
		if (strcmp(cmd, "menu") == 0) { menu(); ok= 1; }
		if (strcmp(cmd, "save") == 0) { save_parameters(); ok= 1; }
		if (strcmp(cmd, "set") == 0) { show_env(); ok= 1; }
		if (strcmp(cmd, "help") == 0) { help(); ok= 1; }
		if (strcmp(cmd, "exit") == 0) { exit(0); }

		/* Command to check bootparams: */
		if (strcmp(cmd, ":") == 0) ok= 1;

		/* User defined function. */
		if (!ok && (body= b_body(cmd)) != nil) {
			(void) tokenize(&cmds, body, &fundef);
			ok= 1;
		}
		if (!ok) printf("%s: unknown function", cmd);
		free(cmd);
		if (ok) return;
	} else {
		/* Syntax error. */
		printf("Can't parse:");
		while (cmds != sep) {
			printf(" %s", cmds->token); voidtoken();
		}
	}

	/* Getting here means that the command is not understood. */
	printf("\nTry 'help'\n");
	err= 1;
}

void monitor(void)
/* Read one or more lines and tokenize them. */
{
	char *line;
	int fundef= 0;
	token **acmds= &cmds;

	unschedule();	/* Kill a trap. */

	do {
		printf("%s%c", bootdev.name, fundef == 0 ? '>' : '+');
		line= readline();
		acmds= tokenize(acmds, line, &fundef);
		free(line);
	} while (fundef != 0);
}

void boot(void)
/* Load Minix and start it, among other things. */
{
	/* Print greeting message.  The copyright message is not yet displayed,
	 * because this boot program need not necessarily start Minix.
	 */
	reset_video(get_video() & 1 ? COLOR_MODE : MONO_MODE);

	printf("\nMinix boot monitor %s\n", version);
	printf("\nPress ESC to enter the monitor\n");

	/* Initialize tables under DOS. */
	if (DOS) initialize();

	/* Relocate program to the end of memory. */
	migrate();

	/* Initialize tables under the BIOS. */
	if (!DOS) initialize();

	/* Block cache. */
	init_cache();

	/* Get environment variables from the parameter sector. */
	get_parameters();

	/* Read and check the superblock. */
	fsok= r_super() != 0;

	while (1) {
		/* While there are commands, execute them! */
		while (cmds != nil) {
			execute();
			if (err) {
				/* An error, stop interpreting. */
				while (cmds != nil) voidtoken();
				err= 0;
				break;
			}
			(void) expired();
			remote_code();
		}
		/* The "monitor" is just a "read one command" thing. */
		monitor();
	}
}
