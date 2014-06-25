/* libExt+gcc:	$Id: bm.h,v 1.1 2000/05/31 09:12:05 veit Exp $	*/

/*-
 * Copyright (c) 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Andrew Hume of AT&T Bell Laboratories.
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
 *	@(#)bm.h	8.6 (Berkeley) 6/21/94
 */

#ifndef _BM_H_
#define	_BM_H_

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__  const
# endif
extern "C" {
#endif

typedef struct {
	unsigned char*pat;		/* pattern */
	size_t	 patlen;		/* pattern length */
	size_t	*delta;			/* skip delta */
	int	 rarec;			/* rare character */
	size_t	 rareoff;		/* rare offset */
	size_t	 md2;			/* mini delta */
} bm_pat;

extern bm_pat*bm_comp(unsigned char __const__*,size_t,unsigned char __const__*);
extern unsigned char*bm_exec(bm_pat*, unsigned char*, size_t);
extern void bm_free(bm_pat*);

#ifdef	__cplusplus
}
#endif

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* !_BM_H_*/
