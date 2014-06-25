/*
 * Copyright (c) 1989 The Regents of the University of California.
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
 */
#include <signal.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>
#include <nl_types.h>

static __inline__ char *itoa(int num);
static char *__strsignal(int num, char *buf);

	const char *const 
__sys_siglist[NSIG] = {
 "Signal 0 (unused)",
 "Hangup",					/* SIGHUP 1 */
 "Interrupt",					/* SIGINT 2 */
 "Quit",					/* SIGQUIT 3 */
 "Illegal instruction (not reset when caught)",	/* SIGILL 4 */
 "Trace/BPT trap (not reset when caught)", 	/* SIGTRAP 5 */
 "Abort trap",					/* SIGABRT 6 */
 "EMT trap",					/* SIGEMT 7 */
 "Floating point exception",			/* SIGFPE 8 */
 "Killed",					/* SIGKILL 9 */
 "Bus error",					/* SIGBUS 10 */
 "Segmentation violation fault",		/* SIGSEGV 11 */
 "Bad system call",				/* SIGSYS 12 */
 "Broken pipe",					/* SIGPIPE 13 */
 "Alarm clock",					/* SIGALRM 14 */
 "Terminated on request",			/* SIGTERM 15 */
 "User defined signal 1",			/* SIGUSR1 16 */
 "User defined signal 2"			/* SIGUSR2 17 */
 "Child process exited",			/* SIGCHLD 18 */
 "Signal 19",
 "Signal 20",
 "Break (Ctrl-Break) on request",		/* SIGBREAK 21 */
 "Stopped (tty output)",			/* SIGTTOU 22 */
 "I/O possible",				/* SIGIO 23 */
 "Cputime limit exceeded",			/* SIGXCPU 24 */
 "Filesize limit exceeded",			/* SIGXFSZ 25 */
 "Virtual timer expired",			/* SIGVTALRM 26 */
 "Profiling timer expired",			/* SIGPROF 27 */
 "Window size changes",				/* SIGWINCH 28 */
 "Information request",				/* SIGINFO 29 */
 "Urgent I/O condition",			/* SIGURG 30 */
 "Suspended (by signal not from tty)",		/* SIGSTOP 31 */
 "Suspended (by tty input)",			/* SIGTSTP 32 */
 "Continued",					/* SIGCONT 33 */
 "Stopped (tty input)",				/* SIGTTIN 34 */
};
	const char *const* 
sys_siglist = __sys_siglist;

	const char *const 
sys_signame[NSIG] = {
	"Signal 0",
	"HUP",		/* SIGHUP */
	"INT",		/* SIGINT */
	"QUIT",		/* SIGQUIT */
	"ILL",		/* SIGILL */
	"TRAP",		/* SIGTRAP */
	"ABRT",		/* SIGABRT */
	"EMT",		/* SIGEMT */
	"FPE",		/* SIGFPE */
	"KILL",		/* SIGKILL */
	"BUS",		/* SIGBUS */
	"SEGV",		/* SIGSEGV */
	"SYS",		/* SIGSYS */
	"PIPE",		/* SIGPIPE */
	"ALRM",		/* SIGALRM */
	"TERM",		/* SIGTERM */
	"USR1",		/* SIGUSR1 */
	"USR2",		/* SIGUSR2 */
	"CHLD",		/* SIGCHLD */
	"SIG19",	/* SIG #19 */
	"SIG20",	/* SIG #20 */
	"BRK",		/* SIGBRK  */
	"TTOU",		/* SIGTTOU */
	"IO",		/* SIGIO */
	"XCPU",		/* SIGXCPU */
	"XFSZ",		/* SIGXFSZ */
	"VTALRM",	/* SIGVTALRM */
	"PROF",		/* SIGPROF */
	"WINCH",	/* SIGWINCH */
	"INFO",		/* SIGINFO */
	"URG",		/* SIGURG */
	"STOP",		/* SIGSTOP */
	"TSTP",		/* SIGTSTP */
	"CONT",		/* SIGCONT */
	"TTIN"		/* SIGTTIN */
};


	static __inline__ char *
itoa(int num)
{
 static char buffer[11];
 char *p = buffer + 4;
	
	while (num >= 10) {
		*--p = (num % 10) + '0';
		num /= 10;
	}
	*p = (num % 10) + '0';
	return(p);
}

	static char *
__strsignal(int num, char *buf)
{
#define	UPREFIX	"Unknown signal: "

#ifdef NLS
	nl_catd catd = catopen("libc", 0);
#endif

	if (num < NSIG) {
#ifdef NLS
		strncpy(buf, catgets(catd, 2, num,
		    (char *)sys_siglist[num]), NL_TEXTMAX-1);
		buf[NL_TEXTMAX-1] = '\0';
#else
		return((char *)sys_siglist[num]);
#endif
	} else {
#ifdef NLS
		strncpy(buf, catgets(catd, 1, 0xffff, UPREFIX), NL_TEXTMAX-1);
		buf[NL_TEXTMAX-1] = '\0';
#else
		strcpy(buf, UPREFIX);
#endif
		strncat(buf, itoa(num), NL_TEXTMAX - strlen(buf) -1);
	}
#ifdef NLS
	catclose(catd);
#endif
	return(buf);
}

	extern char *
_strsignal(int sig)
{
	static char buf[NL_TEXTMAX] = "nothing";

	if (sig < 0)
		{errno = EINVAL; return NULL;}

	return( __strsignal(sig, buf) );
}
	extern void
psignal(unsigned int sig, const char *s)
{
 static char buf[NL_TEXTMAX];
 register const char *c = __strsignal((int)sig, buf);
 register int n;
 struct iovec iov[4];
 int niov = 0;

	if (s && *s) {
		n = strlen(s);
		iov[0].iov_base = (void *)s;
		iov[0].iov_len = n;
		iov[1].iov_base = ": ";
		iov[1].iov_len = 2;
		niov = 2;
	}
	iov[niov].iov_base = (void *)c;
	iov[niov].iov_len = strlen(c);
	iov[niov+1].iov_base = "\n";
	iov[niov+1].iov_len = 1;
	(void)writev(STDERR_FILENO, iov, niov+2);
}
	extern int
sigvec(int signo, const _SIGVEC_T *sv, _SIGVEC_T *osv)
{
	int ret;
	_SIGVEC_T nsv;

	if (sv) {
		nsv = *sv;
		nsv.sv_flags ^= SV_INTERRUPT;	/* !SA_INTERRUPT */
	}
	ret = _sigaction(signo, (sv ? (_SIGACTION_T*)&nsv 
				: NULL), (_SIGACTION_T*)osv);

	if (ret == 0 && osv)
		osv->sv_flags ^= SV_INTERRUPT;	/* !SA_INTERRUPT */
	return(ret);
}

#undef sigpause(m)
#undef sigemptyset(x)
#undef sigfillset(x)
#undef sigaddset(x,y)
#undef sigdelset(x,y)
#undef sigismember(x,y)

	extern int
sigsetmask(int mask)
{
	int omask, n;

	n = _sigprocmask(SIG_SETMASK, (sigset_t*)&mask, (sigset_t*)&omask);
	if (n)
		return(n);
	return(omask);
}

	extern int
sigblock(int mask)
{
	int omask, n;

	n = _sigprocmask(SIG_BLOCK, (sigset_t*)&mask, (sigset_t*)&omask);
	if (n)
		return(n);
	return(omask);
}

	extern int
sigpause(int mask)
{
	return(_sigsuspend((sigset_t*)&mask));
}

	extern int
sigemptyset(sigset_t *set)
{
	*set = 0;
	return (0);
}

	extern int
sigfillset(sigset_t *set)
{
	*set = ~(sigset_t)0;
	return (0);
}

	extern int
sigaddset(sigset_t *set, int signo)
{
	if (signo <= 0 || signo >= NSIG) {
		errno = EINVAL;
		return -1;
	}
	*set |= _SIGMASK(signo);
	return (0);
}

	extern int
sigdelset(sigset_t *set, int signo)
{
	if (signo <= 0 || signo >= NSIG) {
		errno = EINVAL;
		return -1;
	}
	*set &= ~_SIGMASK(signo);
	return (0);
}

	extern int
sigismember(const sigset_t *set, int signo)
{
	if (signo <= 0 || signo >= NSIG) {
		errno = EINVAL;
		return -1;
	}
	return((*set & _SIGMASK(signo)) != 0);
}


/*
 * Set signal state to prevent restart of system calls
 * after an instance of the indicated signal.
 */
sigset_t __signalx_sigIntr;

	extern int
siginterrupt(int sig, int flag)
{
 _SIGACTION_T sa;
 int ret = _sigaction(sig, (_SIGACTION_T*)0, &sa);

	if (ret < 0)
		return(ret);
	if (flag) {
		_sigaddset(&__signalx_sigIntr, sig);
		sa.sa_flags &= ~SA_RESTART;
	} else {
		_sigdelset(&__signalx_sigIntr, sig);
		sa.sa_flags |= SA_RESTART;
	}
	return(_sigaction(sig, &sa, (_SIGACTION_T*)0));
}


/*
 * Backwards-compatible killpg().
 */
	extern int
killpg(pid_t pgid, int sig)
{
	return (_kill(-pgid, sig));
}

extern char *strsignal(int sig){ return _strsignal(sig);}

