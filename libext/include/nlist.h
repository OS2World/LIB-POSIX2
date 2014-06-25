/* libExt+gcc: nlist.h -- Stabs structures and constants. */

/* $Header: /cvsroot/posix2/posix2/libext/include/nlist.h,v 1.1 2000/11/17 19:11:45 arnd Exp $ */
/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)nlist.h	8.2 (Berkeley) 1/21/94
 */

#ifndef _NLIST_H_
#define	_NLIST_H_

/*
 * Symbol table entry format.  The #ifdef's are so that programs including
 * nlist.h can initialize nlist structures statically.
 */
struct nlist {

#ifdef _AOUT_INCLUDE_
	union {
	 char *n_name;		/* symbol name (in memory) */
	 struct nlist *n_next;
	 long n_strx;		/* file string table offset (on disk) */
	} n_un;
#else
	char *n_name;		/* symbol name (in memory) */
#endif

#define	N_UNDF	0x00	/* undefined */
#define	N_EXT	0x01	/* symbol is external, (global) bit, OR'ed in */
#define	N_ABS	0x02	/* absolute address */
#define N_TEXT  0x04    /* in .text segment */
#define N_DATA  0x06    /* in .data segment */
#define N_BSS   0x08    /* in .bss segment  */
	/* Indirect reference to another symbol, following this;
	   libraries will be searched to find a definition.  */
#define N_INDR 	0x0a	/* alias definition */
#define	N_SIZE	0x0c	/* pseudo type, defines a symbol's size */
	/* GNU extension: Copied from aout64.h of BFD */
#define N_WEAKU 0x0d    /* weak undefined symbol: Includes N_EXT! */
#define N_WEAKA 0x0e	/* Weak absolute symbol.  */
#define N_WEAKT 0x0f	/* Weak text symbol.  */
#define N_WEAKD 0x10	/* Weak data symbol.  */
#define N_WEAKB 0x11	/* Weak bss symbol.  */
#define	N_COMM	0x12	/* common reference */
	/* These appear as input to LD, in a .o file.  */
#define	N_SETA	0x14	/* absolute set element symbol */
#define	N_SETT	0x16	/* text set element symbol */
#define	N_SETD	0x18	/* data set element symbol */
#define	N_SETB	0x1a	/* bss set element symbol */
	/* This is output from LD.  */
#define N_SETV	0x1c	/* pointer to set vector in data area.  */
#define	N_FN	0x1e	/* file name (N_EXT on) */
#define	N_WARN	0x1e	/* warning message (N_EXT off) */

#define	N_TYPE	0x1e		/* mask for all the type bits */
	unsigned char n_type;	/* type defines */

	char n_other;		/* spare */

	short n_desc;		/* used by stab entries */
#define	n_hash	n_desc		/* used internally by ld(1); XXX */

	unsigned long n_value;	/* address/value of the symbol */
};


#define	N_FORMAT	"%08x"	/* namelist value format; XXX */

#ifndef _AOUT_INCLUDE_
#ifndef N_GSYM
#define N_GSYM          0x20
#define N_FUN           0x24
#define N_STSYM         0x26
#define N_LCSYM         0x28
#define N_RSYM          0x40
#define N_SLINE         0x44
#define N_SO            0x64
#endif
	/* not stabs; aaarrrghh */
#define N_IMP1          0x68    /* Import reference (emx specific) */
#define N_IMP2          0x6a    /* Import definition (emx specific) */

#ifndef _AOUT_INCLUDE_
#define N_LSYM          0x80
#define N_SOL           0x84
#define N_PSYM          0xa0
#define N_LBRAC         0xc0
#define N_RBRAC         0xe0
#endif
#endif

#define	N_STAB		0xe0	/* mask for debugger symbols -- stab(5) */
	/* Import reference / definition (emx specific) is not stabs!! */
#define IS_STAB(flg) (((flg)&N_STAB)&&(flg)!=(N_IMP1|N_EXT)&&(flg)!=(N_IMP2|N_EXT))
#define IS_STABS(flg) IS_STAB(flg) 


#if 0
extern int nlist(__const__ char *, struct nlist *);
#endif 

#endif /* !_NLIST_H_ */
