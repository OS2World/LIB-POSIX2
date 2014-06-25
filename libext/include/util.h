/* libExt+gcc: $Id: util.h,v 1.3 2000/11/26 16:43:01 arnd Exp $ */
/*-
 * Copyright (c) 1995
 *	The Regents of the University of California.  All rights reserved.
 * Portions Copyright (c) 1996, Jason Downs.  All rights reserved.
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
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <pwd.h>
#include <utmp.h>
#include <termios.h>
/* #include <sys/ttycom.h> */
#include <sys/types.h>


#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__ const
# endif
extern "C" {
#endif

/*
 * fparseln() specific operation flags.
 */
#define FPARSELN_UNESCESC	0x01
#define FPARSELN_UNESCCONT	0x02
#define FPARSELN_UNESCCOMM	0x04
#define FPARSELN_UNESCREST	0x08
#define FPARSELN_UNESCALL	0x0f

/*
 * opendev() specific operation flags.
 */
#define OPENDEV_PART	0x01		/* Try to open the raw partition. */
#define OPENDEV_DRCT	0x02		/* Try to open the device directly. */

char   *fparseln(FILE *, size_t *, size_t *, __const__ char[3], int);
int	getmaxpartitions(void);
int	getrawpartition(void);

#if 0
void	login(struct utmp *);
int	login_tty(int);
int	logout(__const__ char *);
void	logwtmp(__const__ char *, __const__ char *, __const__ char *);
int	opendev(char *, int, int, char **);
void	pw_setdir(__const__ char *);
char   *pw_file(__const__ char *);
int	pw_lock(int retries);
int	pw_mkdb(void);
int	pw_abort(void);
void	pw_init(void);
void	pw_edit(int, __const__ char *);
void	pw_prompt(void);
void	pw_copy(int, int, struct passwd *);
void	pw_getconf(char *, size_t, __const__ char *, __const__ char *);
int	pw_scan(char *, struct passwd *, int *);
void	pw_error(__const__ char *, int, int);
int	openpty(int *, int *, char *, struct termios *,
		     struct winsize *);
pid_t	forkpty(int *, char *, struct termios *, struct winsize *);
void	login_fbtab(char *, uid_t, gid_t);
char   *readlabelfs(char *, int);
#endif

#define UU_LOCK_INUSE (1)
#define UU_LOCK_OK (0)
#define UU_LOCK_OPEN_ERR (-1)
#define UU_LOCK_READ_ERR (-2)
#define UU_LOCK_CREAT_ERR (-3)
#define UU_LOCK_WRITE_ERR (-4)
#define UU_LOCK_LINK_ERR (-5)
#define UU_LOCK_TRY_ERR (-6)
#define UU_LOCK_OWNER_ERR (-7)

__const__ char *uu_lockerr(int);
int     uu_lock(__const__ char *); 
int	uu_lock_txfr(__const__ char *, pid_t);
int     uu_unlock(__const__ char *);

#if defined (__cplusplus)
}
#endif

#endif /* !_UTIL_H_ */
