/* libExt+gcc: */
/* $Header: /cvsroot/posix2/posix2/libext/include/arpa/inet.h,v 1.1 2000/11/17 19:11:58 arnd Exp $ */

/*
 * Copyright (c) 1983, 1993
 *    The Regents of the University of California.  All rights reserved.
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
 * 	This product includes software developed by the University of
 * 	California, Berkeley and its contributors.
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
 * -
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 *	@(#)inet.h	8.1 (Berkeley) 6/2/93
 *	$From: inet.h,v 8.6 1996/08/08 06:54:29 vixie Exp $
 */

#ifndef _INET_H_
#define	_INET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#if defined (__cplusplus)
extern "C" {
#endif

/* External definitions for functions in inet(3) */

extern in_addr_t inet_addr(__const__ char *);
extern int	 inet_aton(__const__ char *, struct in_addr *);
extern in_addr_t inet_lnaof(struct in_addr);
extern struct in_addr inet_makeaddr(in_addr_t,in_addr_t);
extern in_addr_t inet_netof(struct in_addr);
extern in_addr_t inet_network(__const__ char *);
extern char	*inet_ntoa(struct in_addr);

#if 0
extern char *	 inet_neta(in_addr_t, char *, size_t);
extern char	*inet_net_ntop(int, __const__ void *, int, char *, size_t);
extern int	 inet_net_pton(int, __const__ char *, void *, size_t);
extern int	 inet_pton(int, __const__ char *, void *);
extern __const__ char*inet_ntop(int, __const__ void *, char *, size_t);
extern u_int	 inet_nsap_addr(__const__ char *, u_char *, int);
extern char	*inet_nsap_ntoa(int, __const__ u_char *, char *);
#endif

#if defined (__cplusplus)
}
#endif

#endif /* !_INET_H_ */
