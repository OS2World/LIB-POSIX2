/* sys/kernel.h (libext+gcc) - $Id: kernel.h,v 1.2 2000/04/18 12:01:52 veit Exp $	*/
/* 	-- Emulate some BSD-kernel facilities (cf. param.c) -- 	*/

#ifndef _SYS_KERNEL_H_
#define _SYS_KERNEL_H_
#include <sys/time.h>

/*-
 * Copyright (c) 1990, 1993
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
 *	@(#)kernel.h	8.3 (Berkeley) 1/21/94
 */

#include <sys/time.h>

/* Global variables for the kernel. */

/* 1.1 */
#define hostid __Param_hostid
#define hostname __Param_hostname 
#define hostnamelen __Param_hostnamelen 
#define domainname __Param_domainname 
#define domainnamelen __Param_domainnamelen 
extern long hostid;
extern char hostname[MAXHOSTNAMELEN];
extern int hostnamelen;
extern char domainname[MAXHOSTNAMELEN];
extern int domainnamelen;

#define securelevel __Param_securelevel
#ifdef INSECURE
extern __const__ int __Param_securelevel;
#else
extern int __Param_securelevel;
#endif

/* 1.2 */
extern volatile struct timeval __Param_mono_time;
extern struct timeval __Param_boottime;
extern struct timeval __Param_runtime;
extern volatile struct timeval __Param_time;
extern struct timezone __Param_tz;			/* XXX */

#define mono_time __Param_mono_time
#define boottime  *_bootTime
#define runtime   __Param_runtime
#define time      __Param_time
#define tz        __Param_tz

extern int __Param_nclist;
extern int __Param_ncallout;
extern int __Param_maxfiles;
extern int __Param_maxproc;
extern int __Param_tick;	/* usec per tick (1000000 / hz) */
extern int __Param_tickfix;	/* periodic tick adj. tick not integral */
extern int __Param_tickfixinterval; /* interval at which to apply adjustment */
extern int __Param_tickadj;	/* "standard" clock skew, us./tick */
extern int __Param_hz;		/* system clock's frequency */
extern int __Param_stathz;	/* statistics clock's frequency */

#if 0
extern int __Param_desiredvnodes;
extern int __Param_profhz;	/* profiling clock's frequency */
#endif

extern int __Param_lbolt;	/* once a second sleep address */
extern int __Param_tickdelta;
extern long __Param_timedelta;

extern __const__ char __Rev_ostype[]; 
extern __const__ char __Rev_osrelease[]; 
extern __const__ char __Rev_osversion[]; 
extern __const__ char __Rev_version[]; 
extern __const__ char sccs[8];

#define ostype		__Rev_ostype
#define osrelease	__Rev_osrelease
#define osversion	__Rev_osversion
#define version		__Rev_version

extern __const__ char __Param_machine[];	/* cpu "architecture" */
extern __const__ char __Param_machine_arch[];	/* machine == machine_arch */
extern __const__ char __Param_cpu_model[];

#define machine		__Param_machine
#define machine_arch	__Param_machine_arch
#define cpu_model 	__Param_cpu_model

#endif /* _SYS_KERNEL_H_ */

