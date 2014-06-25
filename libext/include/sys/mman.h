/* sys/mman.h: libExt+gcc - Definitions for BSD-style memory management. */
/*	$Id: mman.h,v 1.8 2000/11/17 19:14:59 arnd Exp $	*/

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

/*-Portions:
 * Copyright (c) 1982, 1986 The Regents of the University of California.
 * All rights reserved.
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
 *	from: @(#)mman.h	7.5 (Berkeley) 6/27/91
 *	mman.h,v 1.3 1993/05/20 16:22:41 cgd Exp
 */

#if !defined (_OS2_H) && !defined (_OS2EMX_H)
#define INCL_DOSMEMMGR
#include <os2.h>
#endif

#include <sys/types.h>

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

/* largest object size 'len'; adapt to your version of OS/2! */
#ifndef MAP_MAX
#define MAP_MAX      (440*1024*1024) 	 /* allow 440 MB maximum mem object */
#endif

/* Return value of `mmap' in case of an error.  */
#define MAP_FAILED	((void*)-1)

/*
 * Protections are chosen from these bits, or-ed together
 */
#define	PROT_NONE	0x00	/* no permissions */
#define	PROT_READ	PAG_READ	/* pages can be read */
#define	PROT_WRITE	PAG_WRITE	/* pages can be written */
#define	PROT_EXEC	PAG_EXECUTE	/* pages can be executed */

#ifndef _POSIX_SOURCE
#define	PROT_GUARD	PAG_GUARD	/* raise guard page entered condition */
#endif

/*
 * Flags contain sharing type and options.
 * Sharing types; choose one.
 */
#define	MAP_SHARED	0x0001	/* share all changes */
#define	MAP_PRIVATE	0x0002	/* changes are private */
#ifndef _POSIX_SOURCE
#define	MAP_COPY	MAP_SHARED	/* "copy" region at mmap time (0x0004) */
#endif
#define	MAP_FIXED	0x0010		/* map exactly as requested at addr */

   /* Other flags. Unsupported: returns error */
#ifdef _BSD_SOURCE
#define	MAP_RENAME	 0x0020	/* Sun: rename private pages to file */
#define	MAP_NORESERVE	 0x0040	/* Sun: don't reserve needed swap area */
#define	MAP_INHERIT	 0x0080	/* region is retained after exec */
#define	MAP_NOEXTEND	 0x0100	/* for MAP_FILE, don't change file size */
#define	MAP_HASSEMAPHORE 0x0200	/* region may contain semaphores */
# define MAP_TYPE 		/* mask for type field */ 			\
(MAP_ANON|MAP_FILE|MAP_FIXED|MAP_HASSEMAPHORE|MAP_INHERIT|MAP_PRIVATE|MAP_SHARED|MAP_COPY)

#if 0 /* Advice to madvise, noops, like madvise */
#define	MADV_NORMAL	0	/* no further special treatment */
#define	MADV_RANDOM	1	/* expect random page references */
#define	MADV_SEQUENTIAL	2	/* expect sequential page references */
#define	MADV_WILLNEED	3	/* will need these pages */
#define	MADV_DONTNEED	4	/* dont need these pages */

extern int madvise(void*,size_t,int); 		/* stub */
#endif

#endif /* !_BSD_SOURCE */

#ifndef _POSIX_SOURCE
/*
 * Mapping type
 */
# define	MAP_FILE	0x0000	/* map from file (default) */
# define	MAP_ANON	0x1000	/* allocated from memory, swap space */
#ifndef MAP_ANONYMOUS
# define 	MAP_ANONYMOUS 	MAP_ANON
#endif

#endif

/*
 * Flags to msync (#ifdef _POSIX_SYNCHRONIZED_IO): bits just ignored (!)
 */
#define	MS_ASYNC	0x01	/* perform asynchronous writes */
#define	MS_SYNC		0x02	/* perform synchronous writes */
#define	MS_INVALIDATE	0x04	/* invalidate cached data */

extern void*_mmap(void*,size_t,int,int,int,off_t);
extern int _munmap(void*,size_t);
extern int _mprotect(__const__ void*,size_t,int);
extern int _msync(__const__ void*,size_t,int);
extern int _madvise(void*,size_t,int); 		/* stub */

extern void*mmap(void*,size_t,int,int,int,off_t);
extern int munmap(void*,size_t);
extern int mprotect(__const__ void*,size_t,int);
extern int msync(__const__ void*,size_t,int);

/* Inline versions */
#if !(!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
	extern __inline__ void*
mmap(void*a,size_t l,int p,int f,int fd,off_t o){return _mmap(a,l,p,f,fd,o);}
	extern __inline__ int 
munmap(void*a,size_t l){return _munmap(a,l);}
	extern __inline__ int
mprotect(__const__ void*a,size_t l,int p){return _mprotect(a,l,p);}
	extern __inline__ int
msync(__const__ void*a,size_t l,int f){return _msync(a,l,f);}
#endif /* !GNUC, etc. */

#if defined (__cplusplus)
}
#endif

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* !_SYS_MMAN_H_ */
