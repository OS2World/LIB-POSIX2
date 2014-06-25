/* libExt+gcc: sys/resource.h 	-- resource usage interfaces (BSD-style)
 *
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/resource.h,v 1.10 2000/11/25 16:33:35 arnd Exp $
 */

/*
 * Copyright (c) 1982, 1986, 1993
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
 *	@(#)resource.h	8.2 (Berkeley) 1/4/94
 */

#ifndef _SYS_RESOURCE_H_
#define	_SYS_RESOURCE_H_

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#if defined (__cplusplus)
#define __inline__ inline
#define __const__ const
extern "C" {
#endif

/*
 * Process priority specifications to get/setpriority.
 */
#define	PRIO_MIN	-16
#define	PRIO_MAX	15

#define	PRIO_PROCESS	0
#define	PRIO_PGRP	1
#define	PRIO_USER	2

/*
 * Resource utilization information.
 */

#define	RUSAGE_SELF	0
#define	RUSAGE_CHILDREN	-1

struct	rusage {
	struct timeval ru_utime;	/* user time used */
	struct timeval ru_stime;	/* system time used */
	long	ru_maxrss;		/* max resident set size */
#define	ru_first	ru_ixrss
	long	ru_ixrss;		/* integral shared text memory size */
	long	ru_idrss;		/* integral unshared data " */
	long	ru_isrss;		/* integral unshared stack " */
	long	ru_minflt;		/* page reclaims */
	long	ru_majflt;		/* page faults */
	long	ru_nswap;		/* swaps */
	long	ru_inblock;		/* block input operations */
	long	ru_oublock;		/* block output operations */
	long	ru_msgsnd;		/* messages sent */
	long	ru_msgrcv;		/* messages received */
	long	ru_nsignals;		/* signals received */
	long	ru_nvcsw;		/* voluntary context switches */
	long	ru_nivcsw;		/* involuntary " */
#define	ru_last		ru_nivcsw
};

/*
 * Resource limits
 */
#define	RLIMIT_CPU	0		/* cpu time in milliseconds */
#define	RLIMIT_FSIZE	1		/* maximum file size */
#define	RLIMIT_DATA	2		/* data size */
#define	RLIMIT_STACK	3		/* stack size */
#define	RLIMIT_CORE	4		/* core file size */
#define	RLIMIT_RSS	5		/* resident set size */
#define	RLIMIT_MEMLOCK	6		/* locked-in-memory address space */
#define	RLIMIT_NPROC	7		/* number of processes */
#define	RLIMIT_NOFILE	8		/* number of open files */

#define	RLIM_NLIMITS	9		/* number of resource limits */

#define	RLIM_INFINITY	(((u_quad_t)1 << 63) - 1)

/* see resource.c */
#define _MAX_LIMIT_CORE   RLIM_INFINITY	/* largest core file */
#define _MAX_LIMIT_CPU    3  	   	/* max cpu time (in seconds) per process */
#define _MAX_LIMIT_FSIZE   RLIM_INFINITY	/* largest file that may be created */
#define _MAX_LIMIT_MEMLOCK (64*1024*1024)	/* max size for mlock(); soft */
#define _MAX_LIMIT_STACK   (32*1024*1024)  /* max process stack; declaratory */

struct orlimit {
	int32_t	rlim_cur;		/* current (soft) limit */
	int32_t	rlim_max;		/* maximum value for rlim_cur */
};

struct rlimit {
	rlim_t	rlim_cur;		/* current (soft) limit */
	rlim_t	rlim_max;		/* maximum value for rlim_cur */
};

/* Load average structure. */
struct loadavg {
	fixpt_t	ldavg[3];
	long	fscale;
};

#if 0				/* not implemented */
extern struct loadavg averunnable;
extern int dosetrlimit(struct proc *, u_int, struct rlimit *);
extern int donice(struct proc *, struct proc *, int);
#endif					/* dummies, mostly */
extern int getpriority(int,int);
extern int _getrlimit(int,struct rlimit*);
extern int getrlimit(int,struct rlimit*);
extern int _getrusage(int,struct rusage*);
extern int getrusage(int,struct rusage*);
extern int setpriority(int,int,int);
extern int setrlimit(int,__const__ struct rlimit*);
extern ulong _getMaxWait(void); 	/* return MAXWAIT time for low priority threads */
extern ulong _getMaxPrMem(void); 	/* return max process privatememory */
extern ulong _getTotalPhysMem(void); 	/* return total available physical memory */
extern ulong _relMaxFileHandles(long); 	/* adjusts and returns process 
								max file handles */
extern ulong _setMaxFileHandles(ulong); /* adjusts process max file handles */
extern pid_t _wait3(int*,int,struct rusage*);
extern pid_t _wait4(pid_t,int*,int,struct rusage*);
extern pid_t wait3(int*,int,struct rusage*);
extern pid_t wait4(pid_t,int*,int,struct rusage*);

	extern __inline__ pid_t 
wait3(int *__s, int __o, struct rusage *__r) {return _wait4(-1,__s,__o,__r);}
	extern __inline__ pid_t 
wait4(pid_t __p, int *__s, int __o, struct rusage *__r)
	 {return _wait4(__p,__s,__o,__r);}

#if defined (__cplusplus)
}
#endif

#endif	/* !_SYS_RESOURCE_H_ */
