/* $Id: errnox.c,v 1.2 2000/10/11 14:48:56 arnd Exp $ */ 
/*  (C) 2000 Arnd Hanses: 
    This code is released into Public Domain.  
    You are free to use this code for any purpose, provided this notice
    remains intact and that we will be exempt of any liability, whatsoever. 
    Though it's released into PD I would appreciate to see any enhancements/
    fixes that you will apply.
	*/
#define INCL_BASE
#define OS2_API32
#include <os2.h>  

#include <unistd.h>
#include <errno.h>
#include <sys/errnox.h>

/* Portions:
 * Copyright (c) 1982, 1985, 1993
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
 */
const char *const 
_sys_errlist[97] = 
{
"Undefined (no) error: 0",		/*  0 - ENOERROR */ 
	/* The usage by libs (i.e. 'errno = 0;') is _NOT_ permitted */

"Operation not permitted",		/*  1 - EPERM */
"No such file or directory",		/*  2 - ENOENT */
"No such process",			/*  3 - ESRCH */
"Interrupted function call",		/*  4 - EINTR */
"Input/output error",			/*  5 - EIO */
"No such device or address",		/*  6 - ENXIO */
"Argument list or environment too big", /*  7 - E2BIG */ 
"Executable file format error",		/*  8 - ENOEXEC */
"Bad file descriptor",			/*  9 - EBADF */
"No child processes",			/* 10 - ECHILD */
"Resource temporarily unavailable",	/* 11 - EAGAIN / */ 
					/* EWOULDBLOCK */
"Cannot allocate memory",		/* 12 - ENOMEM */
"Permission denied",			/* 13 - EACCES */
"Bad address",				/* 14 - EFAULT */
"No locks available",			/* 15 - ENOLCK  */ 
"Device busy",				/* 16 - EBUSY */
"File exists",				/* 17 - EEXIST */
"Cross-device link",			/* 18 - EXDEV */
"Operation not supported by device",	/* 19 - ENODEV */
"Not a directory",			/* 20 - ENOTDIR */
"Is a directory",			/* 21 - EISDIR */
"Invalid argument",			/* 22 - EINVAL */
"Too many open files in system",	/* 23 - ENFILE */
"Too many open files",			/* 24 - EMFILE */
"Inappropriate ioctl for device",	/* 25 - ENOTTY */
"Resource deadlock avoided",		/* 26 - EDEADLK */ 
"File too large",			/* 27 - EFBIG */
"No space left on device",		/* 28 - ENOSPC */
"Illegal seek",				/* 29 - ESPIPE */
"Read-only file system",		/* 30 - EROFS */
"Too many links",			/* 31 - EMLINK */
"Broken pipe",				/* 32 - EPIPE */

	/* math software */
"Numerical argument out of domain",	/* 33 - EDOM */
"Result too large",			/* 34 - ERANGE */

"Directory not empty",  		/* 35 - ENOTEMPTY */ 

	/* ipc/network software -- argument errors */
"Operation now in progress",		/* 36 - EINPROGRESS */
"Function not implemented",		/* 37 - ENOSYS */
"File name too long",			/* 38 - ENAMETOOLONG */
"Destination address required",		/* 39 - EDESTADDRREQ */ 
"Message too long",			/* 40 - EMSGSIZE */ 
"Protocol wrong type for socket",	/* 41 - EPROTOTYPE */
"Protocol option not available",	/* 42 - ENOPROTOOPT */
"Protocol not supported",		/* 43 - EPROTONOSUPPORT */
"Socket type not supported",		/* 44 - ESOCKTNOSUPPORT */
"Operation not supported",		/* 45 - EOPNOTSUPP */
"Protocol family not supported",	/* 46 - EPFNOSUPPORT */
					/* 47 - EAFNOSUPPORT */
"Address family not supported by protocol family",
"Address already in use",		/* 48 - EADDRINUSE */
"Cannot assign requested address",	/* 49 - EADDRNOTAVAIL */

	/* ipc/network software -- operational errors */
"Network is down",			/* 50 - ENETDOWN */
"Network is unreachable",		/* 51 - ENETUNREACH */
"Network dropped connection on reset",	/* 52 - ENETRESET */
"Software caused connection abort",	/* 53 - ECONNABORTED */
"Connection reset by peer",		/* 54 - ECONNRESET */
"No buffer space available",		/* 55 - ENOBUFS */
"Socket is already connected",		/* 56 - EISCONN */
"Socket is not connected",		/* 57 - ENOTCONN */
"Cannot send after socket shutdown",	/* 58 - ESHUTDOWN */
"Too many references: can't splice",	/* 59 - ETOOMANYREFS */
"Operation timed out",			/* 60 - ETIMEDOUT */
"Connection refused",			/* 61 - ECONNREFUSED */
"Too many levels of symbolic links",	/* 62 - ELOOP */
"Socket operation on non-socket",	/* 63 - ENOTSOCK */
"Host is down",				/* 64 - EHOSTDOWN */
"No route to host",			/* 65 - EHOSTUNREACH */
"Operation already in progress",	/* 66 - EALREADY */

/* BSD extensions to errno */
	/* quotas & mush */
"Too many processes",			/* 67 - EPROCLIM */
"Too many users",			/* 68 - EUSERS */
"Disc quota exceeded",			/* 69 - EDQUOT */

	/* Network File System */
"Stale NFS file handle",		/* 70 - ESTALE */
"Too many levels of remote in path",	/* 71 - EREMOTE */
"RPC struct is bad",			/* 72 - EBADRPC */
"RPC version wrong",			/* 73 - ERPCMISMATCH */
"RPC prog. not avail",			/* 74 - EPROGUNAVAIL */
"Program version wrong",		/* 75 - EPROGMISMATCH */
"Bad procedure for program",		/* 76 - EPROCUNAVAIL */

"Inappropriate file type or format",	/* 77 - EFTYPE */
"Bad message", 				/* 78 - EBADMSG	*/ 
"Operation canceled",			/* 79 - ECANCELED */ 
"Authentication error", 		/* 80 - EAUTH */
"Need authentication", 			/* 81 - ENEEDAUTH */
"Not a block device", 			/* 82 - ENOTBLK */

/* ANSI appendix */
"Coding error in multibyte or wide character", /* 83 - EILSEQ */ 

"Not supported",			/* 84 - ENOTSUP */ 

/* some SUS extensions to errno */
"No message of the desired type",	/* ENOMSG	85 */ 
"No STREAM resources",			/* ENOSR	86 */ 
"Not a STREAM",				/* ENOSTR	87 */ 
"SUS extension: ENOLINK",		/* ENOLINK	88 */ 
"Value too large to be stored in data type", /* EOVERFLOW 89 */ 
"Protocol error",			/* EPROTO	90 */ 
"STREAM ioctl() timeout",		/* ETIME	91 */ 
"Text file busy",			/* ETXTBSY	92 */ 
"IPC object identifier removed", 	/* EIDRM	93 */ 
"SUS extension: EMULTIHOP",		/* EMULTIHOP	94 */ 
"No message available on read queue",	/* ENODATA	95 */ 
"" 					/* empty string	96 */ 
};

const char*const*
sys_errlist = _sys_errlist;

const int 
_sys_nerr = sizeof(_sys_errlist)/sizeof(_sys_errlist[0]);

const int 
sys_nerr = sizeof(_sys_errlist)/sizeof(_sys_errlist[0]);

unsigned const char 
__errnox_sysErr2ErrnoTab[348] =
{
  ENOERROR,EINVAL,ENOENT, ENOENT, EMFILE,  /* 0..4 */
  EACCES, EBADF,  EIO,    ENOMEM, EIO,     /* 5..9 */
  EINVAL, ENOEXEC,EINVAL, EINVAL, ENOERROR,/* 10..14 */
  ENOTBLK,EBUSY,  EXDEV,  ENOENT, EROFS,   /* 15..19 */
  ENXIO,  EIO,    EIO,    EIO,    EIO,     /* 20..24 */
  EIO,  ENOTBLK,  EIO,    ENOSPC, EIO,     /* 25..29 */
  EIO,    EIO,    EACCES, EACCES, EIO,     /* 30..34 */
  EIO,    EIO,    EIO,    EIO,    ENOSPC,  /* 35..39 */
  EIO,    EIO,    EIO,    EIO,    EIO,     /* 40..44 */
  EIO,    EIO,    EIO,    EIO,    EIO,     /* 45..49 */
  EIO,    EIO,    EIO,    EIO,    EBUSY,   /* 50..54 */
  ENXIO,  EIO,    EIO,    EIO,    EIO,     /* 55..59 */
  EIO,    ENOSPC, ENOSPC, EIO,    EIO,     /* 60..64 */
  EACCES, EIO,    EIO,    EIO,    EIO,     /* 65..69 */
  EIO,    EIO,    EIO,    EROFS,  EIO,     /* 70..74 */
  EIO,    EIO,    EIO,    EIO,    EIO,     /* 75..79 */
  EEXIST, EIO,    ENOENT, EIO,    EIO,     /* 80..84 */
  EIO,    EIO,    EINVAL, EIO,    EAGAIN,  /* 85..89 */
  EIO,    EIO,    EIO,    EIO,    EIO,     /* 90..94 */
  EINTR,  EIO,    EIO,    EIO,    EACCES,  /* 95..99 */
  ENOMEM, EINVAL, EINVAL, ENOMEM, EINVAL,  /* 100..104 */
  EINVAL, ENOMEM, EIO,    EACCES, EPIPE,   /* 105..109 */
  ENOENT, E2BIG,  ENOSPC, ENOMEM, EBADF,   /* 110..114 */
  EINVAL, EINVAL, EINVAL, EINVAL, ENODEV,  /* 115..119 */
  ENOTSUP,EAGAIN, ENOMEM, ENOTDIR,ENOTSUP, /* 120..124 */
  ENOENT, ENOENT, ENOENT, ECHILD, ECHILD,  /* 125..129 */
  EACCES, ENXIO,  ESPIPE, EINVAL, EINVAL,  /* 130..134 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 135..139 */
  EINVAL, EINVAL, EBUSY,  EINVAL, EINVAL,  /* 140..144 */
  EINVAL, EINVAL, EINVAL, EBUSY,  EINVAL,  /* 145..149 */
  EINVAL, EINVAL, ENOMEM, EINVAL, EINVAL,  /* 150..154 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 155..159 */
  EINVAL, EINVAL, EINVAL, EINVAL, EAGAIN,  /* 160..164 */
  EINVAL, EINVAL, EACCES, EINVAL, EINVAL,  /* 165..169 */
  EBUSY,  EINVAL, EINVAL, EINVAL, EINVAL,  /* 170..174 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 175..179 */
  EINVAL, EINVAL, EINVAL, EINVAL, ECHILD,  /* 180..184 */
  EINVAL, EINVAL, ENOENT, EINVAL, EINVAL,  /* 185..189 */
  ENOEXEC,ENOEXEC,ENOEXEC,ENOEXEC,ENOEXEC, /* 190..194 */
  ENOEXEC,ENOEXEC,ENOEXEC,ENOEXEC,ENOEXEC, /* 195..199 */
  ENOEXEC,ENOEXEC,ENOEXEC,ENOENT, EINVAL,  /* 200..204 */
  EINVAL,ENAMETOOLONG,EINVAL,EINVAL,EINVAL,/* 205..209 */
  EINVAL, EINVAL, EACCES, ENOEXEC,ENOEXEC, /* 210..214 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 215..219 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 220..224 */
  EINVAL, EINVAL, EINVAL, ECHILD, EINVAL,  /* 225..229 */
  EINVAL, EBUSY,  EAGAIN,ENOTCONN,EINVAL,  /* 230..234 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 235..239 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 240..244 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 245..249 */
  EACCES, EACCES, EINVAL, ENOENT, EINVAL,  /* 250..254 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 255..259 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 260..264 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 265..269 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 270..274 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 275..279 */
  EINVAL, EINVAL, EINVAL, EINVAL, EEXIST,  /* 280..284 */
  EEXIST, EINVAL, EINVAL, EINVAL, EINVAL,  /* 285..289 */
  ENOMEM, EMFILE, EINVAL, EINVAL, EINVAL,  /* 290..294 */
  EINVAL, EINVAL, EINVAL, EINVAL, EINVAL,  /* 295..299 */
  EINVAL, EBUSY,  EINVAL, ESRCH,  EINVAL,  /* 300..304 */
  ESRCH,  EINVAL, EINVAL, EINVAL, ESRCH,   /* 305..309 */
  EINVAL, ENOMEM, EINVAL, EINVAL, EINVAL,  /* 310..314 */
  EINVAL, E2BIG,  ENOENT, EIO,    EIO,     /* 315..319 */
  EINVAL, EINVAL, EINVAL, EINVAL, EAGAIN,  /* 320..324 */
  EINVAL, EINVAL, EINVAL, EIO,    ENOENT,  /* 325..329 */
  EACCES, EACCES, EACCES, ENOENT, ENOMEM,  /* 330..334 */
  EINVAL, EINVAL, EINVAL, ENOENT, ENOMEM,  /* 335..339 */
  ENXIO,  EPERM,  ENXIO,  ENOENT, ENXIO,   /* 340..344 */
  ENXIO,  ENXIO,  ENXIO  /* 345..347 */
};

	extern unsigned const char
_rc2Errno(APIRET rc)
{
	return( rc >= sizeof(__errnox_sysErr2ErrnoTab) 
		? EINVAL : __errnox_sysErr2ErrnoTab[rc] ); 
}

