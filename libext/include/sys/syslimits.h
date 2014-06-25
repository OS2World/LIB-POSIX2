/* libExt+gcc: -- POSIX-related and other system specific limits
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/syslimits.h,v 1.7 2000/11/17 19:14:59 arnd Exp $
 */

/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)syslimits.h	8.1 (Berkeley) 6/2/93
 */

#ifndef __SYS_SYSLIMITS_H_
#define	__SYS_SYSLIMITS_H_

#define __MAXADDRESS ((caddr_t)(3UL*1024*1024*1024 - 1))   /* 3 GB address range */

/* 
  ARG_MAX 
          length of arguments and environment for exec*(), in bytes

  CHILD_MAX 
          number of simultaneous processes per real user ID
          
  CLK_TCK 
          number of clock ticks per second, for times() 

  NGROUPS_MAX 
          maximum number of simultaneous 'supplementary group IDs'
          ('access group IDs') per process

  STREAM_MAX 
          number of streams that one process can have open at one time 

  TZNAME_MAX 
          number of bytes supported for the name of a time zone

  JOB_CONTROL 
          defined if implementation supports job control -- emx doesn't

  SAVED_IDS 
          defined if each process has a saved set-user-ID and a saved 
          set-group-ID -- not true for emx

  VERSION 
          corresponds to _POSIX_VERSION (the integer 199009L) 

  LINK_MAX 
          maximum value of a file's link count

  MAX_CANON 
          maximum number of bytes in a terminal canonical input line

  MAX_INPUT 
          maximum number of bytes for which space will be available in a
          terminal input queue

  NAME_MAX 
          maximum number of bytes in a file name

  OPEN_MAX
	Maximum number of files that one process can have open at anyone time.

  PATH_MAX 
          maximum number of bytes in a path name

  PIPE_BUF 
          maximum number of bytes that can be written atomically to a pipe 

  _POSIX_NO_TRUNC 
          path name components longer than NAME_MAX generate an error

  _POSIX_VDISABLE 
          terminal special characters can be disabled using this 
          character value, if it is defined

  See POSIX.1 for more details. 
 */


/* only default/minimal values; do not use */

#define _POSIX_ARG_MAX          4096
#define _POSIX_CHILD_MAX        6
#define _POSIX_LINK_MAX         8
#define _POSIX_MAX_CANON        255
#define _POSIX_MAX_INPUT        255
#define _POSIX_NAME_MAX         14
#define _POSIX_NGROUPS_MAX      0
#define _POSIX_OPEN_MAX         16
#define _POSIX_PATH_MAX         255
#define _POSIX_PIPE_BUF         512
#define _POSIX_SSIZE_MAX        32767
#define _POSIX_STREAM_MAX       8
#define _POSIX_TZNAME_MAX       3

#define	_POSIX2_BC_BASE_MAX	99
#define	_POSIX2_BC_DIM_MAX	2048
#define	_POSIX2_BC_SCALE_MAX	99
#define	_POSIX2_BC_STRING_MAX	1000
#define	_POSIX2_COLL_WEIGHTS_MAX	2
#define	_POSIX2_EXPR_NEST_MAX	32
#define	_POSIX2_LINE_MAX	2048
#define	_POSIX2_RE_DUP_MAX	255

/* P1003.1c */
#define _POSIX_TTY_NAME_MAX	260
#define _POSIX_LOGIN_NAME_MAX	12


#ifndef __STRICT_ANSI__

#define	SSIZE_MAX	INT_MAX		/* max value for a ssize_t */

/* where does this belong? it is defined by P1003.1c */
#define TTY_NAME_MAX		_POSIX_TTY_NAME_MAX
#define LOGIN_NAME_MAX		_POSIX_LOGIN_NAME_MAX

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)

/* Additional definitions from X/Open Portability Guide, Issue 4, Version 2
   System Interfaces and Headers, 4.16 <limits.h>. sysconf() should be used 
   to obtain the actual value.

   ATEXIT_MAX	Maximum number of functions that may be registered
		with `atexit'.
   PAGESIZE
   PAGE_SIZE	Size of bytes of a page.

   PASS_MAX	Maximum number of significant bytes in a password.
	  */

#define PASS_MAX		128
#define NL_ARGMAX		9
#define NL_LANGMAX		14
#define NL_MSGMAX		32767
#define NL_NMAX			1
#define NL_SETMAX		255
#define NL_TEXTMAX		255

#if !defined (TMP_MAX)
#define TMP_MAX         1000
#endif

#if !defined (PAGE_SIZE)
#define PAGE_SIZE 		0x1000
#endif
#if !defined (PAGESIZE)
#define PAGESIZE 		0x1000
#endif


#endif /* !_POSIX_SOURCE || _XOPEN_SOURCE */

#define	ARG_MAX		 (256 * 1024)	/* max bytes for an exec function (safe?) */
#define	CHILD_MAX		   80	/* max simultaneous processes */
#define	LINK_MAX		32767	/* max file link count */
#define	MAX_CANON		  255	/* max bytes in term canon input line */
#define	MAX_INPUT		  255	/* max bytes in terminal input */
#define	NAME_MAX		  260	/* max bytes in a file name */
#define	NGROUPS_MAX		 16	/* max supplemental group id's; dummy (AH) */
#define	OPEN_MAX		   64	/* max open files per process */
#define	PATH_MAX		  260	/* max bytes in pathname */
#define	PIPE_BUF		  512	/* max bytes for atomic pipe writes; safe? (AH) */

#define	BC_BASE_MAX	      INT_MAX	/* max ibase/obase values in bc(1) */
#define	BC_DIM_MAX		65535	/* max array elements in bc(1) */
#define	BC_SCALE_MAX	      INT_MAX	/* max scale value in bc(1) */
#define	BC_STRING_MAX	      INT_MAX	/* max const string length in bc(1) */
#define	COLL_WEIGHTS_MAX	    2	/* max weights for order keyword */
#define	EXPR_NEST_MAX		   32	/* max expressions nested in expr(1) */
#define	LINE_MAX		 2048	/* max bytes in an input line (safe?) */
#ifndef RE_DUP_MAX
#define	RE_DUP_MAX		  255	/* max RE's in interval notation */
#endif


#if !defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)

#define	IOV_MAX			 1024	/* max # of iov's (readv,sendmsg,etc) */
#define	NZERO		 	   15	/* No! default BSD "nice" feature!! */

#endif /* !_POSIX_C_SOURCE || _XOPEN_SOURCE */


/*
 * Various (Security) System Limits Describing Static Buffers, etc. 
*/
#if !(defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE)) || defined(_BSD_SOURCE)

/* MAXCOMLEN should be >= sizeof(ac_comm) (see <acct.h>)
 * MAXLOGNAME should be >= UT_NAMESIZE (see <utmp.h>)
 */
#define	MAXCOMLEN        16	/* max command name remembered */
#define	MAXLOGNAME	  0	/* No! max login name length. */
#define	MAXINTERP  	 64	/* max interpreter file name length */

#define	MAXUPRC     CHILD_MAX	/* max simultaneous processes per user */

#define	NOFILE      OPEN_MAX	/* max open files per process (soft) */
#define	NOFILE_MAX  1024	/* max open files per process (hard) */

#ifndef	NGROUPS			/* BSD compat. requires > 7 access groups p.proc.; */
#define	NGROUPS	NGROUPS_MAX 	/* supplemental group ID sets are available */
#endif
#ifndef NOGROUP
#define NOGROUP	(-1)		/* marker for empty group set member */
#endif

#define	CANBSIZ	  MAX_CANON 	/* max bytes in term canonical input line */
/* largest mmap() size; adapt to your version of OS/2! (AH) */
#ifndef MAP_MAX
#define MAP_MAX		(64*1024*1024) /* allow 64 MB maximum mmap() object */
#endif

#ifndef MAXUSERS
#define MAXUSERS 100	/* fake multi-user system; influences various limits (AH) */
#endif

#define	NPROC (20 + 16 * MAXUSERS) /* OS/2 supports up to 4095 processes/threads */
#define	NTEXT (80 + NPROC / 8)	/* actually the object cache */

#if 0 /* no POSIX file system yet (AH) */
#define	NVNODE (NPROC * 2 + NTEXT + 100) */
#endif

/*
 * Round p (pointer or byte index) up to a correctly-aligned value
 * for all data types (int, long, ...).   The result is u_int and
 * must be cast to any desired pointer type.
 */
#define ALIGNBYTES	(sizeof(int) - 1)
#define ALIGN(p)	(((u_int)(p) + ALIGNBYTES) &~ ALIGNBYTES)

#define	PGSHIFT		12		/* LOG2(NBPG) */
#define	NBPG		(1 << PGSHIFT)	/* bytes/page: 0x1000 */
#define	PGOFSET		(NBPG-1)	/* byte offset into page */
#define	NPTEPG		(NBPG/(sizeof (pt_entry_t)))

#define	DEV_BSHIFT	9		/* log2(DEV_BSIZE) */
#define	DEV_BSIZE	(1 << DEV_BSHIFT)
#define	BLKDEV_IOSIZE	2048
#define	MAXPHYS		(64 * 1024)	/* max raw I/O transfer size */

#define	CLSIZELOG2	0
#define	CLSIZE		(1 << CLSIZELOG2)

/* NOTE: SSIZE, SINCR and UPAGES must be multiples of CLSIZE */
#define	SSIZE		1		/* initial stack size/NBPG */
#define	SINCR		1		/* increment of stack/NBPG */
#define	UPAGES		2		/* pages of u-area */
#define	USPACE		(UPAGES * NBPG)	/* total size of u-area */

#if defined (_KERNEL) && !defined (HZ)
#define HZ        100
#endif


/*
 * Clustering of hardware pages on machines with ridiculously small
 * page sizes is done here.  The paging subsystem deals with units of
 * CLSIZE pte's describing NBPG (from machine/machparam.h) pages each.
 */
#define	CLBYTES		(CLSIZE*NBPG)
#define	CLOFSET		(CLSIZE*NBPG-1)	/* for clusters, like PGOFSET */
#define	claligned(x)	((((int)(x))&CLOFSET)==0)
#define	CLOFF		CLOFSET
#define	CLSHIFT		(PGSHIFT+CLSIZELOG2)

#if CLSIZE==1
#define	clbase(i)	(i)
#define	clrnd(i)	(i)
#else
/* Give the base virtual address (first of CLSIZE). */
#define	clbase(i)	((i) &~ (CLSIZE-1))
/* Round a number of clicks up to a whole cluster. */
#define	clrnd(i)	(((i) + (CLSIZE-1)) &~ (CLSIZE-1))
#endif

#define	CBLOCK	64		/* Clist block size, must be a power of 2. */
#define CBQSIZE	(CBLOCK/NBBY)	/* Quote bytes/cblock - can do better. */
				/* Data chars/clist. */
#define	CBSIZE	(CBLOCK - sizeof(struct cblock *) - CBQSIZE)
#define	CROUND	(CBLOCK - 1)	/* Clist rounding. */

#define	NBPW	sizeof(int)	/* number of bytes per word (integer) */

/*
 * Constants related to network buffer management.
 * MCLBYTES must be no larger than CLBYTES (the software page size), and,
 * on machines that exchange pages of input or output buffers with mbuf
 * clusters (MAPPED_MBUFS), MCLBYTES must also be an integral multiple
 * of the hardware page size.
 */
#define	MSIZE		128		/* size of an mbuf */
#define	MCLSHIFT	11		/* convert bytes to m_buf clusters */
#define	MCLBYTES	(1 << MCLSHIFT)	/* size of a m_buf cluster */
#define	MCLOFSET	(MCLBYTES - 1)	/* offset within a m_buf cluster */

#ifndef NMBCLUSTERS
#define	NMBCLUSTERS	2048		/* map size, max cluster allocation */
#endif

/*
 * Size of kernel malloc arena in CLBYTES-sized logical pages
 */ 
#ifndef NKMEMCLUSTERS
#define	NKMEMCLUSTERS	(16 * 1024 * 1024 / CLBYTES)
#endif

/* pages ("clicks") to disk blocks */
#define	ctod(x)		((x) << (PGSHIFT - DEV_BSHIFT))
#define	dtoc(x)		((x) >> (PGSHIFT - DEV_BSHIFT))

/* bytes to pages */
#define	ctob(x)		((x) << PGSHIFT)
#define	btoc(x)		(((x) + PGOFSET) >> PGSHIFT)

/* bytes to disk blocks */
#define	dbtob(x)	((x) << DEV_BSHIFT)
#define	btodb(x)	((x) >> DEV_BSHIFT)

/*
 * Mach derived conversion macros
 */
#define	i386_round_pdr(x)	((((unsigned)(x)) + PDOFSET) & ~PDOFSET)
#define	i386_trunc_pdr(x)	((unsigned)(x) & ~PDOFSET)
#define	i386_btod(x)		((unsigned)(x) >> PDSHIFT)
#define	i386_dtob(x)		((unsigned)(x) << PDSHIFT)
#define	i386_round_page(x)	((((unsigned)(x)) + PGOFSET) & ~PGOFSET)
#define	i386_trunc_page(x)	((unsigned)(x) & ~PGOFSET)
#define	i386_btop(x)		((unsigned)(x) >> PGSHIFT)
#define	i386_ptob(x)		((unsigned)(x) << PGSHIFT)

#define	CMASK	022		/* default file mask: S_IWGRP|S_IWOTH */
#define	NODEV	(dev_t)(-1)	/* non-existent device */

#endif /* ! defined(_BSD_SOURCE), etc. */
#endif /* !__STRICT_ANSI__ */

#endif /* !_SYS_SYSLIMITS_H_ */
