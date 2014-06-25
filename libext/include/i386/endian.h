/* i386/endian.h (libext+gcc) - $Id: endian.h,v 1.5 2000/08/06 20:26:53 amai Exp $ */
/*
 * Copyright (c) 1987, 1991 Regents of the University of California.
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
 *	from: @(#)endian.h	7.8 (Berkeley) 4/3/91
 *	
 */
#ifndef _MACHINE_ENDIAN_H_
#define _MACHINE_ENDIAN_H_

/*
 * Define the order of 32-bit words in 64-bit words.
 */
#define _QUAD_HIGHWORD 1
#define _QUAD_LOWWORD 0

#define	__LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	__BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#define	__PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */

/* This file defines `__BYTE_ORDER' for the particular machine.  */
#include <bytesex.h>

#ifndef _POSIX_SOURCE
/*
 * Definitions for byte order, according to byte significance from low
 * address to high.
 */
#define	LITTLE_ENDIAN	__LITTLE_ENDIAN
#define	BIG_ENDIAN	__BIG_ENDIAN
#define	PDP_ENDIAN	__PDP_ENDIAN
#define	BYTE_ORDER	__BYTE_ORDER 

extern unsigned long   htonl(unsigned long);
extern unsigned short  htons(unsigned short);
extern unsigned long   ntohl(unsigned long);
extern unsigned short  ntohs(unsigned short);

/* #warning Please test the following macros if you want and report results ! */

#include <i386/types.h>

#define	__swap32md(x) ({						\
	u_int32_t __swap32md_x = (x);					\
	__asm ("rorw $8, %w1; rorl $16, %1; rorw $8, %w1" :		\
	    "=r" (__swap32md_x) : "0" (__swap32md_x));			\
	__swap32md_x;})
#define	__swap16md(x) ({						\
	u_int16_t __swap16md_x = (x);					\
	__asm ("rorw $8, %w1" : "=r" (__swap16md_x) :			\
	    "0" (__swap16md_x));					\
	__swap16md_x;})

#ifdef __GNUC__
#define __swap16gen(x) ({						\
	u_int16_t __swap16gen_x = (x);					\
	(u_int16_t)((__swap16gen_x & 0xff) << 8 |			\
	    (__swap16gen_x & 0xff00) >> 8);})
#define __swap32gen(x) ({						\
	u_int32_t __swap32gen_x = (x);					\
	(u_int32_t)((__swap32gen_x & 0xff) << 24 |			\
	    (__swap32gen_x & 0xff00) << 8 |				\
	    (__swap32gen_x & 0xff0000) >> 8 |				\
	    (__swap32gen_x & 0xff000000) >> 24);})
#define swap16(x) ({							\
	u_int16_t __swap16_x = (x);					\
									\
	__builtin_constant_p(x) ? __swap16gen(__swap16_x) :		\
	    __swap16md(__swap16_x);})
#define swap32(x) ({							\
	u_int32_t __swap32_x = (x);					\
	__builtin_constant_p(x) ? __swap32gen(__swap32_x) :		\
	    __swap32md(__swap32_x);})
#else /* __GNUC__ */

/* Note that these macros evaluate their arguments several times.  */
#define __swap16gen(x)							\
    (u_int16_t)(((u_int16_t)(x) & 0xff) << 8 | ((u_int16_t)(x) & 0xff00) >> 8)

#define __swap32gen(x)							\
    (u_int32_t)(((u_int32_t)(x) & 0xff) << 24 |				\
    ((u_int32_t)(x) & 0xff00) << 8 | ((u_int32_t)(x) & 0xff0000) >> 8 |	\
    ((u_int32_t)(x) & 0xff000000) >> 24)

#endif /* __GNUC__ */

#define swap16_multi(v, n) do {					        \
	size_t __swap16_multi_n = (n);					\
	u_int16_t *__swap16_multi_v = (v);				\
									\
	while (__swap16_multi_n) {					\
		*__swap16_multi_v = swap16(*__swap16_multi_v);		\
		__swap16_multi_v++;					\
		__swap16_multi_n--;					\
	}								\
} while (0)

#define htons swap16
#define htonl swap32
#define ntohs swap16
#define ntohl swap32

#define htole16(x) (x)
#define htole32(x) (x)
#define letoh16(x) (x)
#define letoh32(x) (x)

/*
 * Macros for network/external number representation conversion.
 */
#define	NTOHL(x)	(x) = ntohl((unsigned long)x)
#define	NTOHS(x)	(x) = ntohs((unsigned short)x)
#define	HTONL(x)	(x) = htonl((unsigned long)x)
#define	HTONS(x)	(x) = htons((unsigned short)x)

#endif /* _POSIX_SOURCE */
#endif /* _MACHINE_ENDIAN_H_ */
