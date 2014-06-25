/* libExt+gcc: exec_aout.h: struct exec, etc. for UNIX-style binaries */

/* $Header: /cvsroot/posix2/posix2/libext/include/sys/exec_aout.h,v 1.3 2000/11/17 19:14:59 arnd Exp $ */

/*
 * Header prepended to each a.out file.
 * only manipulate the a_midmag field via the
 * N_SETMAGIC/N_GET{MAGIC,MID,FLAG} macros below.
 */
struct exec {
  unsigned long a_midmag;	/* htonl(flags<<26|mid<<16|magic) */
#define a_info	a_midmag	/* Use macros N_MAGIC,etc for access */
  unsigned a_text;		/* size of text segment,in bytes */
  unsigned a_data;		/* size of initialized data,in bytes */
  unsigned a_bss;		/* size of uninitialized data area for file,in bytes */
  unsigned a_syms;		/* size of symbol table data in file,in bytes */
  unsigned a_entry;		/* start address (entry point) */
  unsigned a_trsize;		/* size of relocation info for text,in bytes */
  unsigned a_drsize;		/* size of relocation info for data,in bytes */
};

/* a_magic */
#define	OMAGIC		0407	/* object file or old impure executable (emx_a.out!) */
#define	NMAGIC		0410	/* pure executable: read-only text */
#define	ZMAGIC		0413	/* demand load format */
#define BMAGIC 		0415	/* Used by a b.out object.  */
#define	QMAGIC		0314	/* "compact" demand load format; deprecated */

#define N_MAGIC(x) 	((x).a_midmag & 0xffff)
#define N_BADMAG(x)	  (N_MAGIC(x) != OMAGIC	\
			&& N_MAGIC(x) != NMAGIC	\
  			&& N_MAGIC(x) != ZMAGIC \
		        && N_MAGIC(x) != QMAGIC)

#define N_IS_QMAGIC(x) (N_MAGIC(x) == QMAGIC)

/*
 * a_mid - keep sorted in numerical order for sanity's sake
 * ensure that: 0 < mid < 0x3ff
 */
#undef M_UNKNOWN	 /* these go in the N_MACHTYPE field */
#undef M_68010
#undef M_68020
#undef M_SPARC

enum machine_type
  {
  M_UNKNOWN = 		0,
#define	MID_ZERO	0	/* unknown - implementation dependent */
  M_68010 = 		1,
#define	MID_SUN010	1	/* sun 68010/68020 binary */
  M_68020 = 		2,
#define	MID_SUN020	2	/* sun 68020-only binary */
  M_SPARC = 		3,
#define	MID_SPARC	3
  /* skip a bunch so we don't run into any of sun's numbers */
  /* make these up for the ns32k*/
#define	MID_NS32032 	64	/* ns32032 running ? */
  M_NS32032 = 		64,
#define	MID_NS32532 	(64+5)	/* ns32532 running mach */
  M_NS32532 = 		(64+5),	
#define	MID_PC386	100	/* 386 PC binary. (so quoth BFD) */
  M_386 = 		100,
#define	MID_29K		101
  M_29K = 		101,
#define MID_RS6000	102
  M_RS6000 = 		102,	/* IBM RS/6000 */
#define	MID_ARM		103
  M_ARM = 		103,	/* Advanced Risc Machines ARM */

#define MID_ROMPAOS	104	/* old IBM RT */
  M_ROMPAOS = 		104,
#define	MID_I386	134	/* i386 BSD binary */
  M_386_NETBSD = 	134,
#define	MID_M68K	135	/* m68k BSD binary with 8K page sizes */
  M_68K_NETBSD = 	135,
#define	MID_M68K4K	136	/* DO NOT USE: m68k BSD binary with 4K page sizes */
  M_68K4K_NETBSD = 	136,	/* NetBSD/m68k4k binary */
#define	MID_NS32532	137
  M_532_NETBSD = 	137,	/* NetBSD/ns32k binary */
#define	MID_SPARC	138	/* NetBSD/sparc */
  M_SPARC_NETBSD =	138,
#define	MID_PMAX	139	/* pmax */
  M_PMAX = 		139,
#define	MID_VAX		140	/* vax */
  M_VAX = 		140,
#define	MID_ALPHA	141	/* Alpha BSD binary */
  M_ALPHA = 		141,
#define	MID_MIPS	142	/* big-endian MIPS */
  M_MIPS = 		142,
#define	MID_ARM6	143	/* ARM6 */
  M_ARM6 = 		143,
#define MID_ROMP	149	/* IBM RT */
  M_ROMP = 		149,
#define MID_M88K        151     /* m88k MIPS R2000/R3000 BSD binary */ 
  M_MIPS1 =    		151,
#define MID_M88K2	152
  M_MIPS2 = 		152,    /* MIPS R4000/R6000 binary */
    /* HP/BSD formats */
#define	MID_HP200	200
  M_HP200 = 		200,	/* hp200 (68010) BSD binary */
#define	MID_HP300	300
  M_HP300 = 		300,	/* hp300 (68020+68881) BSD binary */
#define	MID_HPUX800     0x20B   /* hp800 HP-UX binary */
  M_HPUX800 = 		0x20B,	
#define	MID_HPUX	0x20C
  M_HPUX = 		0x20C	/* hp200/300 HPUX binary */
};


/*
 * a_flags
 */
#define EX_DYNAMIC	0x20
#define EX_PIC		0x10
#define EX_DPMASK	0x30
/*
 * Interpretation of the (a_flags & EX_DPMASK) bits:
 *
 *	00		traditional executable or object file
 *	01		object file contains PIC code (set by `as -k')
 *	10		dynamic executable
 *	11		position independent executable image
 * 			(eg. a shared library)
 *
 */

#define N_TRSIZE(a)	((a).a_trsize)
#define N_DRSIZE(a)	((a).a_drsize)
#define N_SYMSIZE(a)	((a).a_syms)

#ifdef _KERNEL
#define STACK_TOP	TASK_SIZE
#endif


/*
 * The a.out structure's a_midmag field is a network-byteorder encoding
 * of this int
 *	FFFFFFmmmmmmmmmmMMMMMMMMMMMMMMMM
 * Where `F' is 6 bits of flag like EX_DYNAMIC,
 *       `m' is 10 bits of machine-id like MID_I386,and
 *       `M' is 16 bits worth of magic number,ie. ZMAGIC.
 * The macros below will set/get the needed fields.
 */

#define	N_GETMAGIC(x) \
    ( (((x).a_midmag)&0xffff0000)?(ntohl(((x).a_midmag))&0xffff):((x).a_midmag))
#define	N_GETMAGIC2(x) \
    ( (((x).a_midmag)&0xffff0000)?(ntohl(((x).a_midmag))&0xffff):\
    (((x).a_midmag)|0x10000) )
#define N_MACHTYPE(x) ((enum machine_type)(((x).a_midmag >> 16) & 0xff))
#define	N_GETMID(x) \
    ( (((x).a_midmag)&0xffff0000)?((ntohl(((x).a_midmag))>>16)&0x03ff):MID_ZERO )
#define N_FLAGS(x) (((x).a_midmag >> 24) & 0xff)
#define	N_GETFLAG(x) \
    ( (((x).a_midmag)&0xffff0000)?((ntohl(((x).a_midmag))>>26)&0x3f):0 )
#define N_SET_INFO(x,mag,typ,flgs) \
	((x).a_midmag =((mag)&0xffff)|(((int)(typ)&0xff)<<16)|(((flgs)&0xff)<<24))
#define	N_SETMAGIC(x,mag,mid,flag) \
    ((x).a_midmag = htonl((((flag)&0x3f)<<26)|(((mid)&0x03ff)<<16)|(((mag)&0xffff))))
#define N_SET_MAGIC(x,magic) \
	((x).a_midmag = (((x).a_midmag & 0xffff0000)|((magic) & 0xffff)))
#define N_SET_MACHTYPE(x,mach) \
	((x).a_midmag = ((x).a_midmag&0xff00ffff)|((((int)(mach))&0xff) << 16))
#define N_SET_FLAGS(x,flags) ((x).a_midmag=((x).a_midmag&0x00ffffff)|(((flags)& 0xff)<<24))

#define N_SHARED_LIB(x) ((x).a_entry < TEXT_START_ADDR)

#define	N_ALIGN(x,x) \
	(N_GETMAGIC(x) == ZMAGIC || N_GETMAGIC(x) == QMAGIC?\
	((x) + __LDPGSZ - 1) & ~(__LDPGSZ - 1):(x))

/* Address of the bottom of the text segment in memory after it is loaded. */
#define N_TXTADDR(x) SEGMENT_SIZE      	/* emx */

#define	N_TXTSIZE(x) \
    (/* For QMAGIC, we don't consider the header part of the text section.  */\
     N_IS_QMAGIC(x) ? (x).a_text - EXEC_BYTES_SIZE : \
     (N_MAGIC(x) != ZMAGIC || N_SHARED_LIB(x)) ? (x).a_text : \
     N_HEADER_IN_TEXT(x)  ?	\
	    (x).a_text - EXEC_BYTES_SIZE:	/* no padding */\
	    (x).a_text				/* a page of padding */\
    )

/* Address of the bottom of data segment in memory after it is loaded. */
#define N_DATADDR(x) 	(N_MAGIC(x)==OMAGIC? (N_TXTADDR(x)+(x).a_text):\
     (SEGMENT_SIZE + ((N_TXTADDR(x)+(x).a_text-1) & ~(SEGMENT_SIZE-1))))

/* Address of the bottom of bss segment in memory after it is loaded.  */
#define N_BSSADDR(x) (N_DATADDR(x) + (x).a_data)

#define N_HEADER_IN_TEXT(x) 0
#define _N_HDROFF(x) (1024 - sizeof(struct exec))
	/* Text segment offset. */
#define N_TXTOFF(x)	\
    (N_MAGIC(x) != ZMAGIC ? EXEC_BYTES_SIZE : /* object file or NMAGIC */\
     N_SHARED_LIB(x) ? 0 : \
     N_HEADER_IN_TEXT(x) ?	\
	    EXEC_BYTES_SIZE :			/* no padding */\
            0x400 + (x).emx_add \
    )
	/* Data segment offset. */
#define N_DATOFF(x) (N_TXTOFF(x) + N_TXTSIZE(x))
	/* Text relocation table offset. */
#define N_TRELOFF(x) (N_DATOFF(x)  + (x).a_data)
	/* Data relocation table offset. */
#define N_DRELOFF(x) (N_TRELOFF(x) + (x).a_trsize)
	/* Symbol table offset. */
#define N_SYMOFF(x)  (N_DRELOFF(x) + (x).a_drsize)
	/* String table offset. */
#define N_STROFF(x)  (N_SYMOFF(x)  + (x).a_syms)

#if 0
#ifdef _KERNEL

/* the "a.out" format's entry in the exec switch */
int	exec_aout_makecmds(struct proc *, struct exec_package *);

/* functions which prepare various a.out executable types */
/*
 * MI portion
 */
int	exec_aout_prep_zmagic(struct proc *, struct exec_package *);
int	exec_aout_prep_nmagic(struct proc *, struct exec_package *);
int	exec_aout_prep_omagic(struct proc *, struct exec_package *);

/* For compatibility modules */
int	exec_aout_prep_oldzmagic(struct proc *, struct exec_package *);
int	exec_aout_prep_oldnmagic(struct proc *, struct exec_package *);
int	exec_aout_prep_oldomagic(struct proc *, struct exec_package *);

/*
 * MD portion
 */
int cpu_exec_aout_makecmds(struct proc *, struct exec_package *);

#endif /* _KERNEL */
#endif 