/* sys/types.h (libExt+gcc) - $Id: types.h,v 1.10 2000/10/11 14:48:55 arnd Exp $ */

#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <sys/cdefs.h>

#define	BSD	199306		/* System version (year & month). */
#define BSD4_3	1
#define BSD4_4	1		/* We fake a generic 4.4 BSD. */

#define __OpenBSD__	1	/* We use their headers and libraries. */
#define _POSIXOS2_VERSION	200004	/* System version (year & month). */

#include <machine/types.h>

#if !defined (_CLOCK_T)
#define _CLOCK_T
typedef long clock_t;
#endif

#if !defined (_PTRDIFF_T)
#define _PTRDIFF_T
typedef long ptrdiff_t;
#endif

#if !defined (_TIME_T)
#define _TIME_T
typedef unsigned long time_t;
#endif

#if !defined (_VA_LIST)
#define _VA_LIST
typedef char *va_list;
#endif

#if !defined (_WCHAR_T)
#define _WCHAR_T
typedef unsigned short wchar_t;
#endif

#if !defined (_SIZE_T)
#define _SIZE_T
typedef unsigned long size_t;
#endif

#if !defined (_SSIZE_T)
#define _SSIZE_T
typedef int ssize_t;
#endif

#if !defined (_TIME_T)
#define _TIME_T
typedef unsigned long time_t;
#endif

#if !defined (_INO_T)
#define _INO_T
typedef int ino_t;
#endif

#if !defined (_DEV_T)
#define _DEV_T
typedef int dev_t;
#endif

#if !defined (_OFF_T)
#define _OFF_T
typedef long off_t;
#endif

#if !defined (_MODE_T)
#define _MODE_T
typedef int mode_t;
#endif

#if !defined (_NLINK_T)
#define _NLINK_T
typedef int nlink_t;
#endif

#if !defined (_UID_T)
#define _UID_T
typedef int uid_t;
#endif

#if !defined (_PID_T)
#define _PID_T
typedef int pid_t;
#endif

#if !defined (_GID_T)
#define _GID_T
typedef int gid_t;
#endif

#if !defined (_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int	u_int;
typedef unsigned long  u_long;
typedef unsigned char  unchar;		/* Sys V compatibility */
typedef unsigned short ushort;		/* Sys V compatibility */
typedef unsigned int	uint;		/* Sys V compatibility */
typedef unsigned long  ulong;		/* Sys V compatibility */

typedef int64_t	int64;

 /* In BSD sys/types.h defines `fd_set' and the FD_* macros for `select'
  */
#define	NBBY	8		/* number of bits per byte; CHAR_BIT */
/*
 * Select uses bit masks of file descriptors in longs.  These macros
 * manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here should
 * be enough for most uses.
 */
#if !defined (FD_SETSIZE)
#define	FD_SETSIZE      256
#elif FD_SETSIZE < 256
#error FD_SETSIZE must be at least 256
#endif

#if !defined (_FD_SET_T)
#define _FD_SET_T
typedef unsigned long fd_mask;
#define NFDBITS	(sizeof(fd_mask) * NBBY)	/* bits per mask */

#ifndef howmany
#define	howmany(x, y)	(((x) + ((y) - 1)) / (y))
#endif

typedef	struct _fd_set {
	fd_mask	fds_bits[howmany(FD_SETSIZE, NFDBITS)];
} fd_set;
#endif

#ifndef FD_SET
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#ifdef _KERNEL
#define	FD_COPY(f, t)	bcopy(f, t, sizeof(*(f)))
#define	FD_ZERO(p)	bzero(p, sizeof(*(p)))
#else
#define	FD_COPY(f, t)	memcpy(t, f, sizeof(*(f)))
#define	FD_ZERO(p)	memset(p, 0, sizeof(*(p)))
#endif
#endif /* FD_SET */


#if !defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)
/* Major, minor numbers, dev_t's. */
#ifndef major
#define	major(x)	((int32_t)(((u_int32_t)(x) >> 8) & 0xff))
#define	minor(x)	((int32_t)((x) & 0xff))
#define	makedev(x,y)	((dev_t)(((x) << 8) | (y)))
#endif
#endif


/* Forward structure declarations for function prototypes.  We include the
 * common structures that cross subsystem boundaries here; others are mostly
 * used in the same place that the structure is defined.
 */
struct	proc;
struct	pgrp;
struct	ucred;
struct	rusage;
struct	file;
struct	buf;
struct	tty;
struct	uio;

#endif /* !(_POSIX_SOURCE) */
#endif /* not _SYS_TYPES_H */
