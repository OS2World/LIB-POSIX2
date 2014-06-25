/* signal.h (libExt + gcc) */
/* $Id: signal.h,v 1.10 2000/10/11 14:48:54 arnd Exp $ */
/*-
 * Copyright (c) 1991, 1993
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
 *	@(#)signal.h	8.3 (Berkeley) 3/30/94
 */
#ifndef __USER_SIGNAL_H
#define __USER_SIGNAL_H

/*
 * For historical reasons; programs expect signal's return value to be
 * defined by <sys/signal.h>.
 */
#include <sys/signal.h>

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

int raise(int);

#if !defined(__STRICT_ANSI__) || defined(_WITH_UNDERSCORE)

#if !defined(__STRICT_ANSI__)
int kill(int,int);
int pause(void);
typedef struct __sigaction _SIGACTION_T;
int sigaction(int,__const__ _SIGACTION_T*,_SIGACTION_T*);
int sigpending(sigset_t*);
int sigprocmask(int,__const__ sigset_t*,sigset_t*);
int sigsuspend(__const__ sigset_t*);
int sigaddset(sigset_t*,int);
int sigdelset(sigset_t*,int);
int sigemptyset(sigset_t*);
int sigfillset(sigset_t*);
int sigismember(__const__ sigset_t*,int);
#endif	/* ANSI */

extern int _kill(int,int);
extern int _pause(void);
extern int _sigaction(int,__const__ _SIGACTION_T*,_SIGACTION_T*);
extern int _sigpending(sigset_t*);
extern int _sigprocmask(int,__const__ sigset_t*,sigset_t*);
extern int _sigsuspend(__const__ sigset_t*);
extern int _sigaddset(sigset_t*,int);
extern int _sigdelset(sigset_t*,int);
extern int _sigemptyset(sigset_t*);
extern int _sigfillset(sigset_t*);
extern int _sigismember(__const__ sigset_t*,int);
extern char*_strsignal(int);

#if !defined(_POSIX_SOURCE)
	/* 4.3BSD */
extern __const__ char*__const__ sys_signame[_NSIG];
extern __const__ char*__const__*sys_siglist;
typedef struct __sigvec _SIGVEC_T;
extern int sigvec(int,__const__ _SIGVEC_T*,_SIGVEC_T*);
extern int sigsetmask(int);
extern int sigblock(int);

extern int sigpause(int);
#define sigpause(m) 	_sigsuspend((sigset_t*)&m)

extern char*strsignal(int);
void psignal(unsigned int,__const__ char*);
extern int killpg(pid_t,int);
extern int siginterrupt(int,int);
extern sigset_t __signalx_sigIntr; /* serialize calls to siginterrupt() */

#endif /* Posix */


/* List definitions after function declarations, or Reiser cpp gets upset. */
#define sigaddset(set,sig)   (*(set) |= _SIGMASK(sig),0)
#define sigdelset(set,sig)   (*(set) &= ~_SIGMASK(sig),0)
#define sigismember(set,sig) ((*(set) & _SIGMASK(sig)) ? 1 : 0)
#define	sigemptyset(set)     (*(set) = 0, 0)
#define	sigfillset(set)	     (*(set) = ~(sigset_t)0,0)

#endif /* Underscore */

#ifdef	__cplusplus
}
#endif

#endif /* not USER_SIGNAL_H */
