/* resource.c (libext + gcc) - bsd compatibility */
/* $Id: resource.c,v 1.3 2000/11/17 19:17:33 arnd Exp $ */

/*- Portions:
 * Copyright (c) 1982, 1986, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)kern_resource.c	8.5 (Berkeley) 1/21/94
 */

#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#include <os2.h>

#include <sys/param.h>

#ifdef SYSVSEM
#include <sys/sem.h>
#endif
#ifdef REAL_CLISTS
#include <sys/clist.h>
#endif
#if 0
#include <sys/mbuf.h>
#include <ufs/ufs/quota.h>
#include <sys/utsname.h>
#endif

#define _KERNEL
#include <sys/msg.h>
#include <sys/resource.h>
#include <sys/vlimit.h>
#include <sys/systm.h>
#include <sys/socket.h>
#include <sys/proc.h>
#include <sys/kernel.h>
#include <sys/file.h>
#include <sys/callout.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/errnox.h>
#include <paths.h>
#include "../../rev.h"

/*
 * System parameter formulae.
 *
 * This file is copied into each directory where we compile
 * the kernel; it should be modified there to suit local taste
 * if necessary.
 *
 * Compiled with -D_HZ=xx -DTIME_ZONE__=x -D_DST=x -DMAXUSERS=xx
 */

int	__Param_hz = _HZ;
int	__Param_stathz = _HZ;
int	__Param_tick = 1000000 / _HZ;
int	__Param_tickadj = 240000 / (60 * _HZ);	/* can adjust 240ms in 60s */

struct	timezone __Param_tz = { TIME_ZONE__, _DST };

volatile struct timeval __Param_mono_time = {0,0};
struct timeval 		__Param_boottime  = {0,0};
struct timeval 		__Param_runtime   = {0,0};
volatile struct timeval __Param_time      = {0,0};

int	__Param_maxproc = NPROC;
int	__Param_maxfiles = 3 * (NPROC + MAXUSERS) + 80;
int	__Param_ncallout = 16 + NPROC;
#ifdef REAL_CLISTS
int	__Param_nclist = 60 + 12 * MAXUSERS;
#endif
#if 0
int	__Param_nmbclusters = NMBCLUSTERS;
int	__Param_desiredvnodes = NVNODE;
#endif
int	__Param_fscale = FSCALE;	/* kernel uses `FSCALE', user uses `fscale' */

/*
 * Values in support of System V compatible semaphores.
 */
#ifdef SYSVSEM
struct	seminfo __Param_seminfo = {
	SEMMAP,		/* # of entries in semaphore map */
	SEMMNI,		/* # of semaphore identifiers */
	SEMMNS,		/* # of semaphores in system */
	SEMMNU,		/* # of undo structures in system */
	SEMMSL,		/* max # of semaphores per id */
	SEMOPM,		/* max # of operations per semop call */
	SEMUME,		/* max # of undo entries per process */
	SEMUSZ,		/* size in bytes of undo structure */
	SEMVMX,		/* semaphore maximum value */
	SEMAEM		/* adjust on exit max value */
};
#endif

/*
 * Values in support of System V compatible messages.
 */
struct	_msginfo __Param_msginfo = {
	MSGMAX,		/* max chars in a message */
	MSGMNI,		/* # of message queue identifiers */
	MSGMNB,		/* max chars in a queue */
	MSGTQL,		/* max messages in system */
	MSGSSZ,		/* size of a message segment */
			/* (must be small power of 2 greater than 4) */
	MSGSEG		/* number of message segments */
};

/*
 * These are initialized at bootstrap time
 * to values dependent on memory size
 */
int	__Param_nbuf = 10, __Param_nswbuf = 20;

/*
 * These have to be allocated somewhere; allocating
 * them here forces loader errors if this file is omitted
 * (if they've been externed everywhere else; hah!).
 */
struct 	callout *__Param_callout;
struct	cblock *__Param_cfree;
struct	buf *__Param_buf, *__Param_swbuf;
char	*__Param_buffers;
#if 0
struct	utsname __Param_utsname;
#endif
/*
 * Attributes stored in the kernel.
 */
char __Param_hostname[MAXHOSTNAMELEN] = _PARAM_HOST_NAME;
int  __Param_hostnamelen = sizeof(hostname);

char __Param_domainname[MAXHOSTNAMELEN] = _PARAM_DOMAIN_NAME;
int  __Param_domainnamelen = sizeof(domainname);

long __Param_hostid = _PARAM_HOST_ID;

#ifdef INSECURE
const int __Param_securelevel = -1;
#else
int __Param_securelevel = 0;
#endif

/* the following is used externally (sysctl_hw) */
const char __Param_machine[] = 		MACHINE;    	/* cpu "architecture" */
const char __Param_machine_arch[] = 	MACHINE_ARCH;   /* machine == machine_arch */

/* Info for CTL_HW */
const char  __Param_cpu_model[] = _PARAM_CPU_MODEL;
extern const char  __Rev_version[];

	extern struct clockinfo *  /* point to statical clockinfo */
_getClockInfo(void)
{
 static struct clockinfo cl;
 ULONG _buf;
 const APIRET 
  	rc = DosQuerySysInfo(QSV_TIMER_INTERVAL, QSV_TIMER_INTERVAL,
  				&_buf, sizeof(_buf));
  	if (rc) {
		errno = _rc2Errno(rc); 
		return (struct clockinfo*)NULL;
	}
		cl.tick = _buf * 100;
		cl.hz = 10000 / _buf;
		cl.tickadj = 240000 / (60 * cl.hz);
		cl.stathz = cl.hz;
		cl.profhz = 0;
	return(&cl);
}

	extern struct timeval *  /* set bootime and return it */
_bootTime(void)
{
 time_t _buf;
 const APIRET 
	rc = DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &_buf, sizeof(_buf));
  	if (rc) {
		errno = _rc2Errno(rc); 
		return (struct timeval*)NULL;
	}
	__Param_boottime.tv_sec = _buf / 1000;
	__Param_boottime.tv_usec = (_buf % 1000)*1000;
	return(&__Param_boottime);
}
	extern int
_setMaxFiles(int new) /* set/return maximal file handles or -1 (error) */
{
 APIRET rc;

	if (new > NOFILE_MAX) {
		errno = EPERM;
		return(-1);
	}
	rc = DosSetMaxFH((ULONG)new);
	if (rc)	{
		errno = _rc2Errno(rc); 
		return(-1);
	}
	__Param_maxfiles = new;
	return(__Param_maxfiles);
}

	extern __const__ int
_mxPath(void) /* return maximal path length or -1 (error) */
{
 int _buf;
 const APIRET 
  rc = DosQuerySysInfo(QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH, 
						&_buf, sizeof(_buf));
  if (rc) {errno = _rc2Errno(rc); return(-1);}

  return(_buf);
}

static const char *__Param_PosixOS2Root = "/";

	extern const char *
_getPosixOS2Root(void)
{
	if (!(__Param_PosixOS2Root = _getenv("POSIXOS2ROOT")) ) 
		fputs(
"param.c: _getPosixOS2Root(): Error: You must set the environment\n\
variable POSIXOS2ROOT to the root of your Unix-tree!\n", stderr);

	return(__Param_PosixOS2Root);
}

#undef getpagesize
	extern const int
getpagesize(void) {return(PAGE_SIZE);}

	extern int
sethostid(long __id) /* dummy fn; cf. tcpip configuration */
{
	__Param_hostid = __id; /* overwritten by sysctl()/gethostid() */
	return (0);
}


/* Restrictions: Returns only priority of current thread, both arguments
 * WHICH and WHO are just ignored. 
 */
	extern int
getpriority(int which, int who)
{
	if (!(which == PRIO_PROCESS || which == PRIO_PGRP || which == 
	  PRIO_USER) || who < 0 ) {
		errno = EINVAL;
		return(-1);
	} {
	 PTIB tib_p;
	 PPIB ppib_p;
	 APIRET rc;
	 int prio;

	rc = DosGetInfoBlocks(&tib_p, &ppib_p);
	if (rc) {errno = _rc2Errno(rc); return(-1);}

	prio = (tib_p->tib_ptib2->tib2_ulpri & 0x7f) - 16;
	return(prio);
	}
}

	extern int
setpriority(int which, int who, int prio)
{
	if (!(which == PRIO_PROCESS || which == PRIO_PGRP || which == 
	  PRIO_USER) || who < 0 || (prio < 20 && prio < 0) ||
	  (prio > 20 && prio > 0) ) {
		errno = EINVAL;
		return(-1);
	}
	if (prio < 0) {
		if (getgid() != 0) {
			errno = EACCES;
			return(-1);
		}
		if (prio < PRIO_MIN) 
			prio = PRIO_MIN;

		prio = (-prio + 15);
	}
	else if (prio > PRIO_MAX)
		prio = PRIO_MAX;
	{
	APIRET rc;
	if (which == PRIO_USER) { 	/* change all prio's */
		rc = DosSetPriority(PRTYS_PROCESS, 0, prio, (ULONG)who);
		if (rc) {errno = _rc2Errno(rc); return(-1);}

	} else {
		if (which == PRIO_PGRP || 
				which == PRIO_PROCESS) { 	/* change prio of grp WHO */
			rc = DosSetPriority(PRTYS_PROCESSTREE, 0, prio, (ULONG)who);
			if (rc) {errno = _rc2Errno(rc); return(-1);}
		}
	}}
	return(0);
}

	extern unsigned long  /* return MAXWAIT time for low priority threads */
_getMaxPrMem(void)
{
 ULONG maxprmem;
 const APIRET 
	rc = DosQuerySysInfo(QSV_MAXPRMEM, QSV_MAXPRMEM,&maxprmem,
					sizeof(maxprmem));
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	return (unsigned long)maxprmem;
}
	extern unsigned long  /* return MAXWAIT time for low priority threads */
_getMaxWait(void)
{
 ULONG maxwait;
 APIRET 
	rc = DosQuerySysInfo(QSV_DYN_PRI_VARIATION, QSV_DYN_PRI_VARIATION,&maxwait,
					sizeof(maxwait));
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	if (!maxwait)
		fputs("\
libext: resource.c: _getMaxWait(): Warning: Absolute priorities!\n\
Maximum CPU-time value is not reliable.",stderr);

	rc = DosQuerySysInfo(QSV_MAX_WAIT, QSV_MAX_WAIT,&maxwait,
					sizeof(maxwait));
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	return (unsigned long)maxwait;
}
	extern unsigned long  		/* return total available physical memory */
_getTotalPhysMem(void)
{
 ULONG totphymem;
 const APIRET 
	rc = DosQuerySysInfo(QSV_TOTPHYSMEM, QSV_TOTPHYSMEM,&totphymem,
					sizeof(totphymem));
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	return (unsigned long)totphymem;
}

	extern unsigned long  /* adjusts and returns process max file handles */
_relMaxFileHandles(long int adjust)
{
 ULONG CurMaxFH;
 const APIRET
	rc = DosSetRelMaxFH(&adjust, &CurMaxFH);
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	return(CurMaxFH);
}
	extern size_t 
_vlimit(int res, int value)
{
	if (res < 0 || value < -2)
		{errno = EINVAL; return(size_t)-1;}

	switch(res) {
	 case LIM_NORAISE:
		return(0);
	 case LIM_CPU: 	/* maximum cpu time in seconds per process */
		return _getMaxWait(); /* we use config.sys MAXWAIT as approx. */
	 case LIM_FSIZE: 	/* maximum file size for current process */
		return (size_t)_MAX_LIMIT_FSIZE; 	/* declaratory */
	 case LIM_DATA: 	/* maximum data size for current process */
		return _getMaxPrMem();
	 case LIM_CORE:	/* maximum core file size for current process */
		return (size_t)_MAX_LIMIT_CORE; /* declaratory */
	 case LIM_STACK:	/* maximum stack size for current process */
		return _MAX_LIMIT_STACK; /* declaratory */
	 case LIM_MAXRSS:	/* maximum resident set size for current process */
		return(_getTotalPhysMem() - 4*1024*1024);
	 default:
		errno = ENOTSUP;
	}
	return(size_t)-1;
}
	extern int 
_getrlimit(int res, struct rlimit *rl)
{
	if (res < 0 || res >= RLIM_NLIMITS)
		{errno = EINVAL; return(-1);}
	if (rl == NULL)
		{errno = EFAULT; return(-1);}

	switch(res) {
	 case RLIMIT_CPU: 		/* maximum cpu time in seconds per process */
		rl->rlim_cur = _getMaxWait(); /* we use config.sys MAXWAIT as approx. */
		if (rl->rlim_cur == (unsigned long)-1) 
			return(-1); 
		rl->rlim_max = _MAX_LIMIT_CPU;
		return(0);
	 case RLIMIT_FSIZE: 		/* maximum file size for current process */
		rl->rlim_cur = _MAX_LIMIT_FSIZE; /* declaratory */
		rl->rlim_max = _MAX_LIMIT_FSIZE; /* declaratory */
		return(0);
	 case RLIMIT_DATA: 		/* maximum data size for current process */
		rl->rlim_cur = _getMaxPrMem();
		if (rl->rlim_cur == (unsigned long)-1)
			return(-1);
		rl->rlim_max = _getMaxPrMem();
		if (rl->rlim_cur == (unsigned long)-1)
			return(-1);
		return(0);
	 case RLIMIT_CORE:		/* maximum core file size for current process */
		rl->rlim_cur = _MAX_LIMIT_CORE; /* declaratory */
		rl->rlim_max = _MAX_LIMIT_CORE; /* declaratory */
		return(0);
	 case RLIMIT_STACK:		/* maximum stack size for current process */
		rl->rlim_cur = _MAX_LIMIT_STACK; /* declaratory */
		rl->rlim_max = _MAX_LIMIT_STACK; /* declaratory */
		return(0);
	 case RLIMIT_RSS:		/* maximum resident set size for current process */
		rl->rlim_cur = _getTotalPhysMem() / 2;
		if (rl->rlim_cur == (unsigned long)-1)
			return(-1);
		rl->rlim_max = _getTotalPhysMem() - 4*1024*1024; /* spare 4Mb for kernel */
		if (rl->rlim_cur == (unsigned long)-1)
			return(-1);
		return(0);
	 case RLIMIT_NPROC:		/* number of processes (current user) */
		rl->rlim_cur = NPROC; /* single user system, use global max */
		rl->rlim_max = 4095 - 5;
		return(0);
	 case RLIMIT_NOFILE:		/* number of open files (current process) */
		rl->rlim_cur = _relMaxFileHandles(0L);
		if (rl->rlim_cur == (unsigned long)-1)
			return(-1);
		rl->rlim_max = NOFILE_MAX;
		return(0);
	 case RLIMIT_MEMLOCK:	/* locked-in-memory address space */
		/* not yet implemented */
	 default:
		errno = ENOTSUP;
		return(-1);
	}
	return(0);
}
	extern unsigned long  /* adjusts process max file handles */
_setMaxFileHandles(unsigned long adjust)
{
 const APIRET
	rc = DosSetMaxFH(adjust);
	if (rc) {
		errno = _rc2Errno(rc); 
		return((unsigned long)-1);
	}
	return(adjust);
}
	extern int 
setrlimit(int res, __const__ struct rlimit *rlim)
{
	if (res < 0 || res > RLIM_NLIMITS) 
		{errno = EINVAL; return(-1);}
	if (rlim == NULL)
		{errno = EFAULT; return(-1);}
		
	switch(res) {
	 case RLIMIT_CPU: 		/* cpu time in milliseconds */

		break;
	 case RLIMIT_FSIZE: 		/* maximum file size */

		break;
	 case RLIMIT_DATA: 		/* data size */

		break;
	 case RLIMIT_CORE:		/* core file size */

		break;
	 case RLIMIT_STACK:		/* stack size */

		break;
	 case RLIMIT_RSS:		/* resident set size */

		break;
	 case RLIMIT_NPROC:		/* number of processes */

		break;
	 case RLIMIT_NOFILE: {		/* number of open files */
	  unsigned long adjust;
		(rlim->rlim_cur >= rlim->rlim_max) ? (adjust = rlim->rlim_cur)
				: (adjust = rlim->rlim_max);
		adjust = _setMaxFileHandles(adjust);
		if (adjust == (unsigned long)-1)
			return(-1);
		return(0);
	 }
	 case RLIMIT_MEMLOCK:	/* locked-in-memory address space */

		break;
	 default:				/* fall-through for unimplemented values */
		errno = ENOTSUP;
		return(-1);
	}
	errno = EPERM;
	return(-1);
}
	extern int  
_getrusage(int who, struct rusage *usage)
{		/* falsify some statistics for to keep W. Churchill happy ;-) */
 struct clockinfo *cl_p;

	if ( !(cl_p = _getClockInfo()) )
		return(-1);
	if (usage == NULL)
		{errno = EFAULT; return(-1);}
	_bzero(usage, sizeof(struct rusage));

	{clock_t cl = clock()/cl_p->tick;
	switch (who) {
	 case RUSAGE_SELF:
		usage->ru_utime.tv_sec = cl - cl/10;
		usage->ru_utime.tv_sec = cl/10;
		usage->ru_maxrss = 512; 	/* fake something bogus */
		usage->ru_ixrss = 100;
		usage->ru_idrss = 100;
		usage->ru_minflt = 10;
		usage->ru_majflt = 100;
		usage->ru_nswap = 10;
		usage->ru_inblock = 100;
		usage->ru_oublock = 100;
		usage->ru_msgsnd = 100;
		usage->ru_msgrcv = 100;
		usage->ru_nsignals = 100;
		usage->ru_nvcsw = 100;
		usage->ru_nivcsw = 100;
		return(0);
	 case RUSAGE_CHILDREN:
		usage->ru_utime.tv_sec = cl - cl/10;
		usage->ru_utime.tv_sec = cl/10;
		usage->ru_maxrss = 512; 	/* fake something bogus */
		usage->ru_ixrss = 100;
		usage->ru_idrss = 100;
		usage->ru_minflt = 10;
		usage->ru_majflt = 100;
		usage->ru_nswap = 10;
		usage->ru_inblock = 100;
		usage->ru_oublock = 100;
		usage->ru_msgsnd = 100;
		usage->ru_msgrcv = 100;
		usage->ru_nsignals = 100;
		usage->ru_nvcsw = 100;
		usage->ru_nivcsw = 100;
		return(0);
	}}
	errno = EINVAL; 
	return(-1);
}
extern int getrusage(int w,struct rusage *u) {return _getrusage(w,u);}
extern int getrlimit(int r, struct rlimit *l){return _getrlimit(r,l);}
extern size_t vlimit(int r, int v){return _vlimit(r,v);}

