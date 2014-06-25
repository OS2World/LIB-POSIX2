/* libExt+gcc: $Id: limits.h,v 1.7 2000/08/26 09:21:10 amai Exp $ */

/*
 * Copyright (c) 1988 The Regents of the University of California.
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
 *	@(#)limits.h	5.9 (Berkeley) 4/3/91
 */

#ifndef _LIMITS_H
#define _LIMITS_H

#define CHAR_BIT      8
#define SCHAR_MIN   (-128)
#define SCHAR_MAX     127
#define UCHAR_MAX     0xffU

#if defined (__CHAR_UNSIGNED__)
#define CHAR_MIN      0
#define CHAR_MAX      UCHAR_MAX
#else
#define CHAR_MIN      SCHAR_MIN
#define CHAR_MAX      SCHAR_MAX
#endif

#define MB_LEN_MAX    6
#define SHRT_MIN    (-32768)
#define SHRT_MAX      32767
#define USHRT_MAX     0xffffU
#define LONG_MIN    (-0x7fffffffL-1L)
#define LONG_MAX      0x7fffffffL
#define ULONG_MAX     0xffffffffUL
#define INT_MIN     (-0x7fffffff-1)
#define INT_MAX       0x7fffffff
#define UINT_MAX      0xffffffffU

#define	SIZE_T_MAX	UINT_MAX	/* max value for a size_t */

#ifndef __STRICT_ANSI__

#include <sys/syslimits.h> 	/* POSIX-related extensions */

#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
#define	UID_MAX		INT_MAX		/* max value for a uid_t */
#define	GID_MAX		INT_MAX		/* max value for a gid_t */

#define	ULLONG_MAX  0xffffffffffffffffULL
#define	UQUAD_MAX       0xffffffffffffffffULL	/* max unsigned quad */
#define LLONG_MAX   0x7fffffffffffffffLL
#define	QUAD_MAX        0x7fffffffffffffffLL	  /* max signed quad */
#define LLONG_MIN  (-0x7fffffffffffffffLL-1LL)
#define	QUAD_MIN       (-0x7fffffffffffffffLL-1LL) /* min signed quad */

#define USI_MAX         UINT_MAX
#endif /* !_POSIX_SOURCE && !_XOPEN_SOURCE */

#if !defined(_POSIX_SOURCE) || defined(_XOPEN_SOURCE)
#define LONG_BIT	32
#define WORD_BIT	32

#define DBL_DIG		15
#define DBL_MAX		1.7976931348623157E+308
#define DBL_MIN		2.2250738585072014E-308

#define FLT_DIG		6
#define FLT_MAX		3.40282347E+38F
#define FLT_MIN		1.17549435E-38F
#endif

#endif /* !__STRICT_ANSI__ */

#endif /* not _LIMITS_H */
