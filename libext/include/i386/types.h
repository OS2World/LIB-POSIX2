/*-
 * Copyright (c) 1990 The Regents of the University of California.
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
 *	@(#)types.h	7.5 (Berkeley) 3/9/91
 */

/* Machine type dependent parameters. */

#ifndef	_MACHTYPES_H_
#define	_MACHTYPES_H_

#include <sys/cdefs.h>

/*
 * Basic integral types.  Omit thetypedef if
 * not possible for a machine/compiler combination.
 */
#ifndef	__BIT_TYPES_DEFINED__
#define	__BIT_TYPES_DEFINED__ 1

typedef	__signed__ char		   int8_t;
typedef	unsigned char		 u_int8_t;
typedef	unsigned char		  uint8_t;
typedef	short			  int16_t;
typedef	unsigned short		u_int16_t;
typedef	unsigned short		 uint16_t;
typedef	int			  int32_t;
typedef	unsigned int		u_int32_t;
typedef	unsigned int		 uint32_t;
typedef	long long		  int64_t; /* LONGLONG */
typedef	unsigned long long	u_int64_t; /* LONGLONG */
typedef	unsigned long long	 uint64_t; /* LONGLONG */
typedef int32_t		 	register_t;

#if !defined(_POSIX_SOURCE)

#if !defined(_KEY_T)
#define _KEY_T
typedef long key_t;		/* IPC key (for Sys V IPC) */
#endif

typedef int	pd_entry_t;		/* page directory entry */
typedef int	pt_entry_t;		/* Mach page table entry */
typedef struct _physadr{int r[1];} *	physadr;
typedef struct _label_t{int val[6];} 	label_t;
#endif

typedef	unsigned long	vm_offset_t;
typedef	unsigned long	vm_size_t;

typedef unsigned long	vaddr_t;
typedef unsigned long	paddr_t;
typedef unsigned long	vsize_t;
typedef unsigned long	psize_t;

typedef	u_int64_t	u_quad_t;	/* quads */
typedef	int64_t		quad_t;
typedef	quad_t *	qaddr_t;

typedef	char *		caddr_t;	/* core address */
typedef	int32_t		daddr_t;	/* disk address */
typedef	u_int32_t	fixpt_t;	/* fixed point number */
typedef quad_t		rlim_t;		/* resource limit */
typedef	int32_t		segsz_t;	/* segment size */
typedef	int32_t		swblk_t;	/* swap offset */
typedef	u_int32_t	useconds_t;	/* microseconds */
typedef	int32_t		suseconds_t;	/* microseconds (signed) */

/*
 * XPG4.2 states that inclusion of <netinet/in.h> must pull these
 * in and that inclusion of <sys/socket.h> must pull in sa_family_t.
 * We put them here because there are other headers that require
 * these types and <sys/socket.h> and <netinet/in.h> will indirectly
 * include <sys/types.h>.  Thus we are compliant without too many hoops.
 */
typedef u_int32_t	in_addr_t;	/* base type for internet address */
typedef u_int16_t	in_port_t;	/* IP port type */
typedef u_int16_t	sa_family_t;	/* sockaddr address family type */
typedef int32_t		socklen_t;	/* length type for network syscalls */

#endif /* !__BIT_TYPES_DEFINED__ */

	/* In BSD <sys/types.h> is expected to define BYTE_ORDER.  */
#include <i386/endian.h>

#endif	/* _MACHTYPES_H_ */
