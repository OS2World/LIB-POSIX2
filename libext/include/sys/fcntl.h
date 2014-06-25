/* libExt+gcc: 	-- sys/fcntl.h 
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/fcntl.h,v 1.4 2000/11/27 22:17:17 arnd Exp $
 */

/*
 * This file includes the definitions for open and fcntl
 * described by POSIX for <fcntl.h>; it also includes
 * related kernel definitions.
 */

#ifndef _SYS_FCNTL_H
#define _SYS_FCNTL_H

#ifndef _KERNEL
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if defined (__cplusplus)
#ifndef __const__
# define __const__ const
#endif
extern "C" {
#endif

/*
 * File status flags: these are used by open(2), fcntl(2).
 * Open/fcntl flags begin with O_; kernel-internal flags begin with F.
 */


/* Don't forget to update /emx/src/dos/termio.inc when changing this! */

#if !defined (O_RDONLY)
/* open-only flags */
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */

#define	O_NONBLOCK	0x0004		/* no delay */
#define	O_APPEND	0x0008		/* set append mode */
#define O_TEXT          0x0010

#ifndef _POSIX_SOURCE 	/* The following are not implemented. Fix me! */
#define	O_SHLOCK	0x0020		/* open with shared file lock */
#define	O_EXLOCK	0x0040		/* open with exclusive file lock */
#define	O_ASYNC		0x0080		/* signal pgrp when data ready */
#define	O_FSYNC		O_SYNC		/* synchronous writes */
#endif

#define O_BINARY        0x0100
#define	O_CREAT		0x0200		/* create if nonexistant */
#define	O_TRUNC		0x0400		/* truncate to zero length */
#define	O_EXCL		0x0800		/* error if already exists */
#define O_SYNC          0x2000
			/* defined by POSIX 1003.1 */
#define	O_NOCTTY	0x4000		/* don't assign controlling terminal */

#define O_SIZE          0x8000

#if !defined (_POSIX_SOURCE)
#define O_NDELAY        O_NONBLOCK
#define O_NOINHERIT     0x1000
#endif

#endif

#if !defined (F_OK)
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4
#endif


/*
 * Constants used for fcntl(2)
 */

/* command values */
#if !defined (F_GETFL)
#define F_GETFL     1		/* get file status flags */
#define F_SETFL     2		/* set file descriptor flags */
#define F_GETFD     3		/* get file descriptor flags */
#define F_SETFD     4		/* set file status flags */
#define F_DUPFD     5		/* duplicate file descriptor */
#define F_GETOSFD   6           /* RSXNT */
#endif
#ifndef _POSIX_SOURCE
#define	F_GETOWN	7		/* get SIGIO/SIGURG proc/pgrp */
#define 	F_SETOWN	8		/* set SIGIO/SIGURG proc/pgrp */
#endif
#define	F_GETLK		9		/* get record locking information */
#define	F_SETLK		10		/* set record locking information */
#define	F_SETLKW	11		/* F_SETLK; wait if blocked */

#if !defined (FD_CLOEXEC) /* file descriptor flags (F_GETFD, F_SETFD) */
#define FD_CLOEXEC  0x01		/* close-on-exec flag */
#endif


/* record locking flags (F_GETLK, F_SETLK, F_SETLKW) */
#define	F_RDLCK		1		/* shared or read lock */
#define	F_UNLCK		2		/* unlock */
#define	F_WRLCK		3		/* exclusive or write lock */
#ifdef _KERNEL
#define	F_WAIT		0x010		/* Wait until lock is granted */
#define	F_FLOCK		0x020	 	/* Use flock(2) semantics for lock */
#define	F_POSIX		0x040	 	/* Use POSIX semantics for lock */
#endif

/*
 * Advisory file segment locking data type -
 * information passed to system by user
 */
struct flock {
	off_t	l_start;	/* starting offset */
	off_t	l_len;		/* len = 0 means until end of file */
	pid_t	l_pid;		/* lock owner */
	short	l_type;		/* lock type: read/write, etc. */
	short	l_whence;	/* type of l_start */
};


#if !defined (_POSIX_SOURCE) && !defined (LOCK_SH)
#define LOCK_SH     0x01
#define LOCK_EX     0x02
#define LOCK_NB     0x04
#define LOCK_UN     0x08
#endif

/*
 * The O_* flags used to have only F* names, which were used in the kernel
 * and by fcntl.  We retain the F* names for the kernel f_flags field
 * and for backward compatibility for fcntl.
 */
#ifndef _POSIX_SOURCE
#define	FAPPEND		O_APPEND	/* kernel/compat */
#define	FASYNC		O_ASYNC		/* kernel/compat */
#define	FFSYNC		O_FSYNC		/* kernel */
#define	FNONBLOCK	O_NONBLOCK	/* kernel */
#define	FNDELAY		O_NONBLOCK	/* compat */
#define	O_NDELAY	O_NONBLOCK	/* compat */
#endif

extern int creat(__const__ char *, int);
extern int fcntl(int, int, ...);
extern int open(__const__ char *, int, ...);

#if !defined (_POSIX_SOURCE)
extern int flock (int, int);
#endif

#if (!defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE) && \
     !defined(_XOPEN_SOURCE)) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE_EXTENDED - 0 == 1)
#ifndef F_ULOCK
#define F_ULOCK         0
#define F_LOCK          1
#define F_TLOCK         2
#define F_TEST          3
#endif
extern int lockf(int,int,off_t);
#endif /* (!defined(_POSIX_SOURCE) && ... */

#if !defined (_POSIX_SOURCE) || defined (_WITH_UNDERSCORE)
int _creat (__const__ char *, int);
int _fcntl (int, int, ...);
int _flock (int, int);
int _open (__const__ char *, int, ...);
#endif

#if defined (__cplusplus)
}
#endif

#endif /* not _SYS_FCNTL_H */
