/* libExt+gcc: -- File Status User Header
 * $Header stat.h,v 1.14 2000/08/18 19:17:47 amai Exp $
 */

/*-
 * Copyright(c) 1982,1986,1989,1993
 *	The Regents of the University of California.  All rights reserved.
 *(c) UNIX System Laboratories,Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories,Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories,Inc.
 *
 * Redistribution and use in source and binary forms,with or without
 * modification,are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice,this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice,this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California,Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES,INCLUDING,BUT NOT LIMITED TO,THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT,INDIRECT,INCIDENTAL,SPECIAL,EXEMPLARY,OR CONSEQUENTIAL
 * DAMAGES(INCLUDING,BUT NOT LIMITED TO,PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE,DATA,OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,WHETHER IN CONTRACT,STRICT
 * LIABILITY,OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE,EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)stat.h	8.9(Berkeley) 8/17/94
 */

#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <sys/time.h>

#if defined (_KERNEL) || defined (_BSD_SOURCE) 
#include <sys/param.h>
#else
#ifdef _XOPEN_SOURCE
#include <sys/types.h>
#endif
#endif

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__ const
# endif
extern "C" {
#endif

#if !defined (_STAT)
#define _STAT
struct stat {
  dev_t	  st_dev;		/* inode's device */
	ino_t	  st_ino;		/* inode's number */
	mode_t  st_mode;		/* inode protection mode */
  nlink_t st_nlink;		/* number of hard links */
  uid_t	  st_uid;		/* user ID of the file's owner */
  gid_t	  st_gid;		/* group ID of the file's group */
  dev_t	  st_rdev;		/* device type */
#ifdef _BSD_SOURCE 	/* compatibility with BSD, not(!) POSIX */
	struct	timespec st_atimespec;	/* time of last access */
	struct	timespec st_mtimespec;	/* time of last data modification */
	struct	timespec st_ctimespec;	/* time of last file status change */
#else
	time_t	  st_atime;		/* time of last access */
	long	  st_atimensec;		/* nsec of last access */
	time_t	  st_mtime;		/* time of last data modification */
	long	  st_mtimensec;		/* nsec of last data modification */
	time_t	  st_ctime;		/* time of last file status change */
	long	  st_ctimensec;		/* nsec of last file status change */
#endif
	off_t	    st_size;		/* file size, in bytes */
	int64_t   st_blocks;		/* blocks allocated for file */
	u_int32_t st_blksize;		/* optimal blocksize for I/O */
	u_int32_t st_flags;		/* user defined flags for file */
	u_int32_t st_gen;			/* file generation number */
  int32_t   st_lspare;		/* reserved for BSD UFS extension ? */
  int64_t   st_qspare[2];	/* reserved for BSD UFS extension ? */
  long    st_attr;			/* OS/2 extension */
  long    st_reserved;			/* emx extension */
};
#endif

#ifdef _BSD_SOURCE	/* compatibility with BSD,not(!) POSIX */
#define	st_atime	st_atimespec.tv_sec
#define	st_atimensec	st_atimespec.tv_nsec
#define	st_mtime	st_mtimespec.tv_sec
#define	st_mtimensec	st_mtimespec.tv_nsec
#define	st_ctime	st_ctimespec.tv_sec
#define	st_ctimensec	st_ctimespec.tv_nsec
#endif

#if !defined(S_ISUID)
#define	S_ISUID	04000	/* set user id on execution */
#define	S_ISGID	02000	/* set group id on execution */
#define S_ISVTX 01000	/* sticky bit */
#if !defined (_POSIX_SOURCE)
#define S_ISTXT S_ISVTX /* save swapped text even after use */
#endif
#define S_IRWXU 00700	/* RWX mask for owner */
#define S_IRUSR 00400	/* R for owner */
#define S_IWUSR 00200	/* W for owner */
#define S_IXUSR 00100	/* X for owner */

#define S_IRWXG 00070	/* dito for group: */
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007	/* dito for world(others): */
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001
#endif

#if !defined (S_IFMT)
#ifndef _POSIX_SOURCE
#define S_IFMT   0170000  /* Mask for file type */
#define S_IFIFO  0010000  /* Pipe */
#define S_IFCHR  0020000  /* Character device */
#define S_IFDIR  0040000  /* Directory */
#define	S_IFBLK	 0060000  /* block special */
#define S_IFREG  0100000  /* Regular file */
#define	S_IFLNK	 0120000  /* symbolic link  -- Not implemented. Fix Me! (AH) */
#define S_IFSOCK 0140000  /* Socket */
#if 0
#define	S_IFWHT  0160000  /* whiteout */
#endif
#endif

#define	S_ISDIR(m)	((m & 0170000) == 0040000)	/* directory */
#define	S_ISCHR(m)	((m & 0170000) == 0020000)	/* char special */
#define	S_ISBLK(m)	((m & 0170000) == 0060000)	/* block special */
#define	S_ISREG(m)	((m & 0170000) == 0100000)	/* regular file */
#define	S_ISFIFO(m)	((m & 0170000) == 0010000)	/* fifo */
#ifndef _POSIX_SOURCE
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
#if 0 		/* as yet no symbolic links implemented Fix me! (AH) */
#define	S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)	/* symbolic link */
#define	S_ISWHT(m)  (((m) & S_IFMT) == S_IFWHT)	/* whiteout */
#else
#define	S_ISLNK(m)  (0)
#endif /* 0 */
#endif /* !_POSIX_SOURCE */
#endif

#ifdef _BSD_SOURCE	/* compatibility with BSD,not(!) POSIX */

#define	ACCESSPERMS	(S_IRWXU|S_IRWXG|S_IRWXO)			      /* 0777 */
#define	ALLPERMS   	(S_ISUID|S_ISGID|S_ISTXT|S_IRWXU|S_IRWXG|S_IRWXO) /* 7777 */
#define	DEFFILEMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) /* 0666 */

#define S_BLKSIZE		512		/* block size used in the stat struct */

/*
 * Definitions of flags stored in file flags word.
 *
 * Super-user and owner changeable flags.	Not(!) implemented(AH)
 */
#define	UF_SETTABLE	0x0000ffff	/* mask of owner changeable flags */
#define	UF_NODUMP	0x00000001	/* do not dump file */
#define	UF_IMMUTABLE	0x00000002	/* file may not be changed */
#define	UF_APPEND	0x00000004	/* writes to file may only append */
#define UF_OPAQUE	0x00000008	/* directory is opaque wrt. union */
/*
 * Super-user changeable flags.
 */
#define	SF_SETTABLE	0xffff0000	/* mask of superuser changeable flags */
#define	SF_ARCHIVED	0x00010000	/* file is archived */
#define	SF_IMMUTABLE	0x00020000	/* file may not be changed */
#define	SF_APPEND	0x00040000	/* writes to file may only append */

#ifdef _KERNEL
/*
 * Shorthand abbreviations of above.
 */
#define	OPAQUE		(UF_OPAQUE)
#define	APPEND		(UF_APPEND | SF_APPEND)
#define	IMMUTABLE	(UF_IMMUTABLE | SF_IMMUTABLE)
#endif

#endif /* !_BSD_SOURCE */

#ifndef _POSIX_SOURCE

#if !defined (S_IREAD)
#define S_IREAD  0400     /* Owner: read */
#define S_IWRITE 0200     /* Owner: write */
#define S_IEXEC  0100     /* Owner: exec */
#endif

#define S_IRUGO 	(S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO 	(S_IWUSR|S_IWGRP|S_IWOTH)

/*
 * Definitions of flags stored in file flags word.
 *
 * Super-user and owner changeable flags.
 */
#define	UF_SETTABLE	0x0000ffff	/* mask of owner changeable flags */
#define	UF_NODUMP	0x00000001	/* do not dump file */
#define	UF_IMMUTABLE	0x00000002	/* file may not be changed */
#define	UF_APPEND	0x00000004	/* writes to file may only append */
#define UF_OPAQUE	0x00000008	/* directory is opaque wrt. union */
/*
 * Super-user changeable flags.
 */
#define	SF_SETTABLE	0xffff0000	/* mask of superuser changeable flags */
#define	SF_ARCHIVED	0x00010000	/* file is archived */
#define	SF_IMMUTABLE	0x00020000	/* file may not be changed */
#define	SF_APPEND	0x00040000	/* writes to file may only append */

#endif

#if !defined (_POSIX_SOURCE) || defined (_WITH_UNDERSCORE)

/* Is the character C a path name separator?  Under OS/2,you can use
   either / or \.  */
#define _ISDIRSEP(c)(c == '/' || c == '\\')
#define _ISABSPATH(s)( _ISDIRSEP(s[0]) ||(s[0] != '\0' && 				\
                         s[1] == ':' && _ISDIRSEP(s[2])) )

extern int _chflags(__const__ char *, unsigned int);
extern int _chmod(__const__ char*,mode_t);
extern int _fchflags(int, unsigned);
extern int _fchmod(__const__ int,mode_t);
extern int _lchmod(__const__ char*,mode_t); 	/* chmod() alias (AH) */
extern int _fstat(int,struct stat*);
extern int _lstat(__const__ char*,struct stat*); 	/* stat() alias */
extern int _mkdir(__const__ char*,long);
extern int _mkfifo(__const__ char*,mode_t); 		/* Fix me! dummy (AH) */
extern int _mknod(__const__ char*,mode_t,dev_t);	/* Fix me! dummy (AH) */
extern int _stat(__const__ char*,struct stat*);
extern int _umask(mode_t);
extern int _isdir(__const__ char*);
extern int _isfile(__const__ char*);
extern int _islink(__const__ char*);
extern int _isdevice(__const__ char*);
extern int _isreadable(__const__ char*);
extern int _iswritable(__const__ char*);
extern int _isaccessible(__const__ char*,mode_t);
extern int _isabsolute(__const__ char*);
#endif

extern int chmod(__const__ char*,mode_t);
extern int fchmod(__const__ int,__const__ mode_t);
extern int fstat(int,struct stat*);
extern int lchmod(__const__ char*,mode_t); /* Fix me! chmod() alias */
extern int lstat(__const__ char *, struct stat*);   /* Fix me! stat() alias */
extern int mkdir(__const__ char*,long);
extern int mkfifo(__const__ char*,mode_t);		/* Fix me! dummy (AH) */
extern int mknod(__const__ char*,mode_t,dev_t);		/* Fix me! dummy (AH) */
extern int stat(__const__ char*,struct stat*);
extern mode_t umask(mode_t);

#ifdef _BSD_SOURCE
extern int chflags(__const__ char*,unsigned);
extern int fchflags(int,unsigned);
#endif

/* Machine-dependent inline versions */

#if !(!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
extern __inline__ int stat(__const__ char*p,struct stat*b){return _stat(p,b);}
extern __inline__ int lstat(__const__ char*p,struct stat*b){return _lstat(p,b);}
extern __inline__ int fchmod(__const__ int f,mode_t m){return _fchmod(f,m);}
extern __inline__ int lchmod(__const__ char*p,mode_t m){return _lchmod(p,m);}
#endif

#if defined (__cplusplus)
}
#endif

#endif /* not SYS_STAT_H */
