/* $Id: param.h,v 1.15 2000/10/11 14:48:55 arnd Exp $
 * libExt+gcc: sys/param.h (BSD compatibility header)
 */
#ifndef _SYS_PARAM_H
#define _SYS_PARAM_H

/*- Portions:
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)param.h	8.2 (Berkeley) 1/21/94
 */

/* 
 * We want to remain source compatible with emx legacy and to provide 
 * most of that hackish stuff that BSD expects to be here. Pls report
 * all outstanding bugs! We know: The macros here severely pollute the 
 * user namespace and violate the important ANSI/POSIX rule to underscore 
 * all non-standard symbols in system libaries! (AH)
 */

#if defined (__cplusplus)
# if !defined (NULL)
#  define NULL (0)
# endif
extern "C" {
#endif

#if !defined (NULL)
#define NULL ((void*)0)
#endif


/*
 * System parameter formulae; it should be modified there to suit local taste
 * if necessary; cf. param.c.
 *
 * Compiled with -DCLOCKS_PER_SEC=xx -DTIME_ZONE__=x -D_DST=x -D_MAXUSERS=xx
 */

#include <time.h>
#ifndef TIME_ZONE__
# define TIME_ZONE__ 0
#endif
#ifndef _DST
# define _DST 0
#endif
#ifndef _HZ
#define	_HZ CLOCKS_PER_SEC
#else
#undef CLK_TCK
#undef CLOCKS_PER_SEC
#define	CLOCKS_PER_SEC 	_HZ 
#define	CLK_TCK 	_HZ 
#endif

/*
 * Various (Security) System Limits Describing Static Buffers, etc. 
 */

#define INSECURE (-1) 	/* no BSD security features yet; cf. sys/sysctl.h */

#include <sys/syslimits.h>
#include <limits.h>


#define _PARAM_HOST_NAME 	"libextp2host"
#define _PARAM_DOMAIN_NAME  	"libextp2domain"
#define _PARAM_HOST_ID 		0xffffffff

#define MAXHOSTNAMELEN	120	/* max length of hostname; cf. OS/2 toolkit */

extern int _setMaxFiles(int);  /* set/return maximal file handles or -1 (error) */

/* Machine type dependent parameters. */
#define	_MACHINE	i386
#define	MACHINE		"i386"
#define	_MACHINE_ARCH	i386
#define	MACHINE_ARCH	"i386"
#define	MID_MACHINE	MID_I386
#define _PARAM_CPU_MODEL "Intel 486 (486-class)" /* anybody still using 386? 
	anyway, this is good, if you compile with -m486. If you compile 
	with -mpentium, tell it the user and define:
	#define _PARAM_CPU_MODEL "Intel Pentium (586-class)"
	*/

#include <sys/types.h>		/* BSD expects sys/types.h in sys/param.h */
#include <sys/simplelock.h> 	/* ?? Need that?? (AH) */

/* Signals. BSD even expects this one to be here?! */
#include <sys/signal.h>

#if !defined (EOF)
#define EOF (-1)
#endif

/* File system parameters and macros. */

#ifndef _POSIX_OS2_ROOT
extern __const__ char*_getPosixOS2Root(void);
#define _POSIX_OS2_ROOT 	_getPosixOS2Root();
#endif 

/*
 * The file system is made out of blocks of at most MAXBSIZE units, with
 * smaller units (fragments) only in the last direct block.  MAXBSIZE
 * primarily determines the size of buffers in the buffer pool.  It may be
 * made larger without any effect on existing file systems; however making
 * it smaller makes some file systems unmountable.
 */
#ifndef MAXBSIZE	/* XXX temp until sun3 DMA chaining */
#define	MAXBSIZE	MAXPHYS
#endif
#define MAXFRAG 	8

/*
 * MAXPATHLEN defines the longest permissable path length after expanding
 * symbolic links. It is used to allocate a temporary buffer from the buffer
 * pool in which to do the name expansion, hence should be a power of two,
 * and must be less than or equal to MAXBSIZE.  MAXSYMLINKS defines the
 * maximum number of symbolic links that may be expanded in a path name.
 * It should be set high enough to allow all legitimate uses, but halt
 * infinite loops reasonably quickly.
 */

 /* OS/2 does not use a constant but a FS query */
extern __const__ int _mxPath(void); /* return max path length or -1 (error) */

#ifndef MAXPATHLEN 
#define MAXPATHLEN 	PATH_MAX
#endif
#if !defined (MAXNAMLEN)
#define MAXNAMLEN  MAXPATHLEN 
#endif
#if !defined (FILENAME_MAX)
#define	FILENAME_MAX    PATH_MAX	/* must be <= PATH_MAX */
#endif
#define	MAXSYMLINKS	3		/* fake with copy for now */

#define	LINK_MAX   32767	/* fake max file link count: no hard links! */

/* System V/ANSI C; this is the wrong way to do this, do *not* use these. */
#if !defined (P_tmpdir)
# define	P_tmpdir	"/var/tmp/"
# define	L_tmpnam	PATH_MAX /* XXX must be == PATH_MAX */
#endif
#if !defined (TMP_MAX)
#define TMP_MAX         1000
#endif

/* defaults are probably too small, redefine them (AH)
 */
#if !defined (BUFSIZ)
#define BUFSIZ 5120
#endif
#if !defined (FOPEN_MAX)
#define	FOPEN_MAX	14	/* must be <= OPEN_MAX */
#endif 		/* FOPEN_MAX is a minimum maximum; should be the number of 
 * descriptors that the kernel can provide without allocation of a resource 
 * that can fail without the process sleeping.  Do not use this for anything. */

#define	MAXPARTITIONS	26		/* number of partitions */
/*
 * The absolute maximum number of disk partitions allowed.
 * This is the maximum value of MAXPARTITIONS for which 'struct disklabel'
 * is <= DEV_BSIZE bytes long.  If MAXPARTITIONS is greater than this, beware.
 */
#define	MAXMAXPARTITIONS	26
#if MAXPARTITIONS > MAXMAXPARTITIONS
#warning beware: MAXPARTITIONS bigger than MAXMAXPARTITIONS
#endif

/* Bit map related macros. */
#ifndef setbit
#define	setbit(a,i)	((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define	clrbit(a,i)	((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define	isset(a,i)	((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define	isclr(a,i)	(((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)
#endif

/*
 * Scale factor for scaled integers used to count %cpu time and load avgs.
 *
 * The number of CPU `tick's that map to a unique `%age' can be expressed
 * by the formula (1 / (2 ^ (FSHIFT - 11))).  The maximum load average that
 * can be calculated (assuming 32 bits) can be closely approximated using
 * the formula (2 ^ (2 * (16 - FSHIFT))) for (FSHIFT < 15).
 *
 * For the scheduler to maintain a 1:1 mapping of CPU `tick' to `%age',
 * FSHIFT must be at least 11; this gives us a maximum load avg of ~1024.
 */
#define	FSHIFT	11		/* bits to right of fixed binary point */
#define FSCALE	(1<<FSHIFT)

/* Macros for counting and rounding. */
#ifndef howmany
#define	howmany(x, y)	(((x)+((y)-1))/(y))
#endif
#ifndef roundup
#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))
#endif
#ifndef powerof2
#define powerof2(x)	((((x)-1)&(x))==0)
#endif

/* Macros for min/max. */
#define	MIN(a,b) (((a)<(b))?(a):(b))
#define	MAX(a,b) (((a)>(b))?(a):(b))

/* Don't try to do this inline for now */
extern unsigned short _swaps(unsigned short);
extern unsigned long _swapl(unsigned long);

#if !defined (htonl)
#define htonl(X) _swapl(X)
#define ntohl(X) _swapl(X)
#define htons(X) _swaps(X)
#define ntohs(X) _swaps(X)
#endif

#if defined (__cplusplus)
}
#endif

#endif /* not _SYS_PARAM_H */
