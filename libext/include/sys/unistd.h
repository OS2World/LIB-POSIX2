/* sys/unistd.h (libExt+gcc)
 * $Id: unistd.h,v 1.4 2000/08/18 19:17:47 amai Exp $ */

/* 	-- Definitions for some Posix2 extensions -- 	*/

/*
 * Copyright (c) 1989, 1993
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
 *	@(#)unistd.h	8.2 (Berkeley) 1/7/94
 */

#ifndef _SYS_UNISTD_H_
#define	_SYS_UNISTD_H_

/* compile-time symbolic constants */
#undef	_POSIX_JOB_CONTROL	/* implementation does not support job control */

#undef	_POSIX_SAVED_IDS	/* no saved set-user-ID and set-group-ID */

#undef _POSIX_VERSION
#define	_POSIX_VERSION		199009L
#define	_POSIX2_VERSION		199212L

/* execution-time symbolic constants */

/* Get values of POSIX options:

   If these symbols are defined, the corresponding features are
   always available.  If not, they may be available sometimes.
   The current values can be obtained with `sysconf'.

   _POSIX_JOB_CONTROL		Job control is supported.
   _POSIX_SAVED_IDS		Processes have a saved set-user-ID
   				and a saved set-group-ID.
   _POSIX_REALTIME_SIGNALS	Real-time, queued signals are supported.
   _POSIX_PRIORITY_SCHEDULING	Priority scheduling is supported.
   _POSIX_TIMERS		POSIX.4 clocks and timers are supported.
   _POSIX_ASYNCHRONOUS_IO	Asynchronous I/O is supported.
   _POSIX_PRIORITIZED_IO	Prioritized asynchronous I/O is supported.
   _POSIX_SYNCHRONIZED_IO	Synchronizing file data is supported.
   _POSIX_FSYNC			The fsync function is present.
   _POSIX_MAPPED_FILES		Mapping of files to memory is supported.
   _POSIX_MEMLOCK		Locking of all memory is supported.
   _POSIX_MEMLOCK_RANGE		Locking of ranges of memory is supported.
   _POSIX_MEMORY_PROTECTION	Setting of memory protections is supported.
   _POSIX_MESSAGE_PASSING	POSIX.4 message queues are supported.
   _POSIX_SEMAPHORES		POSIX.4 counting semaphores are supported.
   _POSIX_SHARED_MEMORY_OBJECTS	POSIX.4 shared memory objects are supported.
   _POSIX_PII			Protocol-independent interfaces are supported.
   _POSIX_PII_XTI		XTI protocol-indep. interfaces are supported.
   _POSIX_PII_SOCKET		Socket protocol-indep. interfaces are supported.
   _POSIX_PII_INTERNET		Internet family of protocols supported.
   _POSIX_PII_INTERNET_STREAM	Connection-mode Internet protocol supported.
   _POSIX_PII_INTERNET_DGRAM	Connectionless Internet protocol supported.
   _POSIX_PII_OSI		ISO/OSI family of protocols supported.
   _POSIX_PII_OSI_COTS		Connection-mode ISO/OSI service supported.
   _POSIX_PII_OSI_CLTS		Connectionless ISO/OSI service supported.
   _POSIX_POLL			Implementation supports `poll' function.
   _POSIX_SELECT		Implementation supports `select' and `pselect'.

   _XOPEN_SHM			Shared memory interface according to XPG4.2.

   If any of these symbols is defined as -1, the corresponding option is not
   true for any file.  If any is defined as other than -1, the corresponding
   option is true for all files.  If a symbol is not defined at all, the value
   for a specific file can be obtained from `pathconf' and `fpathconf'.

   _POSIX_CHOWN_RESTRICTED	Only the super user can use `chown' to change
   				the owner of a file.  `chown' can only be used
				to change the group ID of a file to a group of
				which the calling process is a member.
   _POSIX_NO_TRUNC		Pathname components longer than
   				NAME_MAX generate an error.
   _POSIX_VDISABLE		If defined, if the value of an element of the
				`c_cc' member of `struct termios' is
				_POSIX_VDISABLE, no character will have the
				effect associated with that element.
   _POSIX_SYNC_IO		Synchronous I/O may be performed.
   _POSIX_ASYNC_IO		Asynchronous I/O may be performed.
   _POSIX_PRIO_IO		Prioritized Asynchronous I/O may be performed.
   */

				/* chown requires appropriate privileges */
#undef 	_POSIX_CHOWN_RESTRICTED	/*1*/
				/* too-long path components generate errors */
#undef 	_POSIX_NO_TRUNC		/*1*/
				/* may disable terminal special characters */
#undef 	_POSIX_VDISABLE		/* (0377) */
				/* file syncronization is available */
#undef 	_POSIX_FSYNC		 /* 1 */

/* access function */
#if !defined (F_OK)
#define	F_OK		0	/* test for existence of file */
#define	X_OK		0x01	/* test for execute or search permission */
#define	W_OK		0x02	/* test for write permission */
#define	R_OK		0x04	/* test for read permission */
#endif

/* whence values for lseek(2) */
#if !defined (SEEK_SET)
#define	SEEK_SET	0	/* set file offset to offset */
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif

#ifndef _POSIX_SOURCE
/* old BSD whence values for lseek(2); renamed by POSIX 1003.1 */
#define	L_SET		SEEK_SET
#define	L_INCR		SEEK_CUR
#define	L_XTND		SEEK_END
#endif

/* configurable pathname variables */
#if !defined (_PC_LINK_MAX)
#define	_PC_LINK_MAX		 1
#define	_PC_MAX_CANON		 2
#define	_PC_MAX_INPUT		 3
#define	_PC_NAME_MAX		 4
#define	_PC_PATH_MAX		 5
#define	_PC_PIPE_BUF		 6
#define	_PC_CHOWN_RESTRICTED	 7
#define	_PC_NO_TRUNC		 8
#define	_PC_VDISABLE		 9
#endif

/* configurable system variables */
#define	_SC_ARG_MAX		 1
#define	_SC_CHILD_MAX		 2
#define	_SC_CLK_TCK		 3
#define	_SC_NGROUPS_MAX		 4
#define	_SC_OPEN_MAX		 5
#define	_SC_JOB_CONTROL		 6
#define	_SC_SAVED_IDS		 7
#define	_SC_VERSION		 8
#define	_SC_BC_BASE_MAX		 9
#define	_SC_BC_DIM_MAX		10
#define	_SC_BC_SCALE_MAX	11
#define	_SC_BC_STRING_MAX	12
#define	_SC_COLL_WEIGHTS_MAX	13
#define	_SC_EXPR_NEST_MAX	14
#define	_SC_LINE_MAX		15
#define	_SC_RE_DUP_MAX		16
#define	_SC_2_VERSION		17
#define	_SC_2_C_BIND		18
#define	_SC_2_C_DEV		19
#define	_SC_2_CHAR_TERM		20
#define	_SC_2_FORT_DEV		21
#define	_SC_2_FORT_RUN		22
#define	_SC_2_LOCALEDEF		23
#define	_SC_2_SW_DEV		24
#define	_SC_2_UPE		25
#define	_SC_STREAM_MAX		26
#define	_SC_TZNAME_MAX		27
#define	_SC_PAGESIZE		28
#define	_SC_PAGE_SIZE		_SC_PAGESIZE	/* 1170 compatibility */
#define	_SC_FSYNC		29
#define	_SC_XOPEN_SHM		30

/* P1003.1c */
#define _SC_GETGR_R_SIZE_MAX
#define _SC_GETPW_R_SIZE_MAX
#define _SC_LOGIN_NAME_MAX
#define _SC_THREAD_DESTRUCTOR_ITERATIONS
#define _SC_THREAD_KEYS_MAX
#define _SC_THREAD_STACK_MIN
#define _SC_THREAD_THREADS_MAX
#define _SC_TTY_NAME_MAX
#define _SC_THREADS
#define _SC_THREAD_ATTR_STACKADDR
#define _SC_THREAD_ATTR_STACKSIZE
#define _SC_THREAD_PRIORITY_SCHEDULING
#define _SC_THREAD_PRIO_INHERIT
#define _SC_THREAD_PRIO_PROTECT
#define _SC_THREAD_PROCESS_SHARED
#define _SC_THREAD_SAFE_FUNCTIONS

/* configurable system strings */
#define	_CS_PATH		 1

#include <unistd.h>

#endif /* !_SYS_UNISTD_H_ */
