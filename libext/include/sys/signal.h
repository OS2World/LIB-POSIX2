/* libExt+gcc: sys/signal.h $Id: signal.h,v 1.8 2000/10/11 14:48:55 arnd Exp $ */

#ifndef _SYS_SIGNAL_H
#define _SYS_SIGNAL_H

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ 	inline
# endif
extern "C" {
#else
#define __sigaction 	sigaction
#define __sigvec 	sigvec 
#endif

typedef int sig_atomic_t;

#define	SIGHUP	1	/* hangup */
#define	SIGINT	2	/* interrupt */
#define	SIGQUIT	3	/* quit */
#define	SIGILL	4	/* illegal instruction (not reset when caught) */
#ifndef _POSIX_SOURCE
#define	SIGTRAP	5	/* trace trap (not reset when caught) */
#endif
#define	SIGABRT	6	/* abort() */
#ifndef _POSIX_SOURCE
#define	SIGIOT	SIGABRT	/* compatibility */
#define	SIGEMT	7	/* EMT instruction */
#endif
#define	SIGFPE	8	/* floating point exception */
#define	SIGKILL	9	/* kill (cannot be caught or ignored) */
#ifndef _POSIX_SOURCE
#define	SIGBUS	10	/* bus error */
#endif
#define	SIGSEGV	11	/* segmentation violation */
#ifndef _POSIX_SOURCE
#define	SIGSYS	12	/* bad argument to system call */
#endif
#define	SIGPIPE	13	/* write on a pipe with no one to read it */
#define	SIGALRM	14	/* alarm clock */
#define	SIGTERM	15	/* software termination signal from kill */
#define SIGUSR1  16 /* User-defined signal #1 */
#define SIGUSR2  17 /* User-defined signal #2 */
#define SIGCHLD  18 /* Death of a child process */
#define SIGCLD   SIGCHLD
#define SIGBREAK 21 /* Break (Ctrl-Break)*/

	/* following signals not yet implemented ! */
#define	SIGTTOU	22	/* like TTIN for output if (tp->t_local&LTOSTOP) */
#ifndef _POSIX_SOURCE
#define	SIGIO	23	/* input/output possible signal */
#define	SIGXCPU	24	/* exceeded CPU time limit; cf. rlimit(2) */
#define	SIGXFSZ	25	/* exceeded file size limit; cf. rlimit(2) */
#ifndef SIGVTALRM 	/* see also sys/itimer.h */
#define	SIGVTALRM SIGALRM /* 26 */	/* virtual time alarm */
#define	SIGPROF	  SIGALRM /* 27 */	/* profiling time alarm */
#endif
#define SIGWINCH 28	/* window size changes */
#define SIGINFO	29	/* information request */
#define	SIGURG	30	/* urgent condition on IO channel */
#define	SIGSTOP	31	/* sendable stop signal not from tty */
#define	SIGTSTP	32	/* stop signal from tty */
#define	SIGCONT	33	/* continue a stopped process */
#define	SIGTTIN	34	/* to readers pgrp upon background tty read */
#define SIGZERO 0	/* Signal 0 (unused) */
#endif

#define _NSIG	35		/* counting 0; could be 36 (mask is 1-35) */

#define SIGPTRACENOTIFY 128     /* Notification from ptrace()*/

/*
 * Language spec sez we must list exactly one parameter, even though we
 * actually supply three.  Ugh!
 */
#define SIG_DFL	((void(*)(int))0)
#define SIG_IGN	((void(*)(int))1)
#define SIG_ERR	((void(*)(int))-1)

/*
 * For historical reasons; programs expect signal's return value to be
 * defined by <sys/signal.h>.
 */
void(*signal(int,void(*)(int)))(int);

#if !defined(__STRICT_ANSI__) || defined(_WITH_UNDERSCORE)

#include <sys/types.h>

#define NSIG _NSIG

/* all ignored now */
#ifndef _POSIX_SOURCE
#define SA_ONSTACK	  0x0001	/* take signal on signal stack */
#define SA_NODEFER	  0x0010	/* don't mask the signal we're delivering */
#define SA_NOCLDWAIT	0x0020	/* don't create zombies (assign to pid 1) */
#define SA_RESTART	  0x0200	/* restart system on signal return */
#ifdef COMPAT_SUNOS
#define	SA_USERTRAMP	0x0100	/* do not bounce off kernel's sigtramp */
#endif
#endif
#define SA_NOCLDSTOP	0x0008	/* do not generate SIGCHLD on child stop */
#define SA_SIGINFO	0x0040	/* generate siginfo_t */

/*
 * Flags for sigprocmask:
 */
#define	SIG_BLOCK	1	/* block specified signal set */
#define	SIG_UNBLOCK	2	/* unblock specified signal set */
#define	SIG_SETMASK	3	/* set specified signal set */

#if !defined(_SIGSET_T)
#define _SIGSET_T
typedef unsigned long sigset_t;
#endif

/*
 * Signal vector "template" used in sigaction call.
 */
struct __sigaction {
  void     (*sa_handler)(int);	/* signal handler */
  sigset_t sa_mask;	/* signal mask to apply */
  int      sa_flags;	/* see signal options below */
};
#define _SIGMASK(m)          ((sigset_t)1 <<((m) - 1))
#define __sigaddset(set,m)   (*(set) |= _SIGMASK(m),0)
#define __sigdelset(set,m)   (*(set) &= ~_SIGMASK(m),0)
#define __sigemptyset(set)     (*(set) = 0,0)
#define __sigfillset(set)      (*(set) = ~(sigset_t)0,0)
#define __sigismember(set,m) ((*(set) & _SIGMASK(m)) ? 1 : 0)

#endif

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)

typedef	void (*sig_t)(int);	/* type of signal function */

/*
 * Structure used in sigaltstack call.
 */
struct	sigaltstack {
	void	*ss_sp;			/* signal stack base */
	int	ss_size;		/* signal stack length */
	int	ss_flags;		/* SS_DISABLE and/or SS_ONSTACK */
};

/*
 * 4.3 compatibility:
 * Signal vector "template" used in sigvec call.
 */
struct __sigvec {
	void     (*sv_handler)(int);	/* signal handler */
	sigset_t sv_mask;	/* signal mask to apply */
	int      sv_flags;	/* see signal options below */
};

#define SV_ONSTACK	SA_ONSTACK
#define SV_INTERRUPT	SA_RESTART	/* same bit, opposite sense */
#define SV_RESETHAND	SA_RESETHAND
#define sv_onstack sv_flags	/* isn't compatibility wonderful! */

/*
 * Structure used in sigstack call.
 */
struct	sigstack {
	void	*ss_sp;			/* signal stack pointer */
	int	ss_onstack;		/* current status */
};

/*
 * Macro for converting signal number to a mask suitable for
 * sigblock().
 */
#define sigmask(m)	((sigset_t)1 << ((m)-1))

#define	BADSIG		SIG_ERR
#define SIG_ACK	((void(*)(int))4)

#define SA_SYSV       0x0002  /* reset to SIG_DFL when taking signal */
#define SA_RESETHAND  SA_SYSV
#define SA_ACK        0x0004  /* Don't unblock automatically*/

#define SIGTY void

#endif /* !ANSI/Posix */

#if defined(__cplusplus)
}
#endif

#endif /* not _SYS_SIGNAL_H */
