/* -- sys/unistdx.h (libExt+gcc) -- to be included in unistd.h
	$Id: unistdx.h,v 1.22 2000/11/25 16:33:35 arnd Exp $  
  */

#ifndef _SYS_UNISTDX_H
#define _SYS_UNISTDX_H

#ifndef	_POSIX_SOURCE
# include <machine/types.h>
# ifndef _XOPEN_SOURCE
#  include <sys/time.h>	/* structure timeval required for select() */
#  include <sys/select.h>
# endif
# include <unistd.h>
#endif

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__  const
# endif
extern "C" {
#endif

#ifndef _POSIX_OS2_ROOT    /* ((__const__ char*)_getenv(POSIXOS2ROOT)) 	"/" */
extern __const__ char*_getPosixOS2Root(void); 		/* cf. param.c */
#define _POSIX_OS2_ROOT _getPosixOS2Root()
#endif 

 /* max bytes of fully qualified path name: a file system property  */
extern __const__ int _mxPath(void); /* OS/2 does not use a constant * 
	but a FS query! Return maximal path length or -1 (error)    */
#ifndef PATH_MAX
#define PATH_MAX 260 /* for strict ANSI C compliance */
#endif /* ! PATH_MAX */

#ifndef	_POSIX2_RE_DUP_MAX
#define	_POSIX2_RE_DUP_MAX	255
#endif

#ifdef _CPYFILE_FOR_SYMLINK  /* Fake symlink() by copying */
# define link(i,o) _Link1(i,o)
# define _link(i,o) _Link1(i,o)
# define symlink(i,o) _Link1(i,o)
# define _symlink(i,o) _Link1(i,o)
#else				/* Simply return EPERM */
# define link(i,o) _Link2(i,o)
# define _link(i,o) _Link2(i,o)
# define symlink(i,o) _Link2(i,o)
# define _symlink(i,o) _Link2(i,o)
#endif

#ifdef _USE_HRTIMER_SYS 
# define usleep _uSleep1
# define _usleep _uSleep1
#else	/* DosSleep() with resolution of milliseconds. */ 
# define usleep _uSleep2
# define _usleep _uSleep2
#endif

extern unsigned int alarm(unsigned int);

extern int _sync(void); /* NAME sync - fight paranoia, commit cache to disk 
   SYNOPSYS: 
       #include <unistd.h>  [here we may find the system's sync()]
       int sync(void); 
    RETURNS: 
        sync() always returns 0  */ 

extern int _cpFile(__const__ char*,__const__ char*);
/* Copy file from in_name to out_name.  Returns:
   0 if all's well
  -1 on error:  errno is set accordingly
 * Curtis W. Rendon 12/20/1999 initial (modified 04/01/00 A. Hanses)
 * hereby released to the public domain, please keep my comments,
 * no warranty inferred, implied, or specified.
 *  Curtis W. Rendon mithra@earthling.net  12/29/1999
 */
extern int _chown(__const__ char*,uid_t, gid_t);	/* stub fn */
extern int _lchown(__const__ char*,uid_t, gid_t);	/* stub fn */
extern int _fchown(int,uid_t, gid_t);			/* stub fn */
extern char *_getwd(char *buf);
extern int _initHrTimer(void);  /* Open the HR-Timer, initialize it, 
	call this fn before doing any timings,  RETURNS  0, on success -1, 
        on error, ERRNO is set accordingly call once at the beginning of your 
        program or when inializing your dll */
extern int _termHrTimer(void); /* Close the hfTimer, do cleanup, 
	call this fn after doing all timings:
   RETURNS
         0, on success
        -1, on error, ERRNO is set accordingly */
extern void _uSleep1(__const__ unsigned long int ulMcsec);
extern void _uSleep2(__const__ unsigned long int ulMcsec); /* sleep for 
	ulWait microseconds */
extern int _Link1(__const__ char*,__const__ char*);
extern int _Link2(__const__ char*,__const__ char*);

extern int _maxPathLen(void);	 /* maximal path length */
extern pid_t vfork(void); 
extern int fsync(int);  /* buggy: We disable; see emx lib reference */

#ifndef _IO_H /* we are not compatible with io.h */
#define fsync(fd) (0)
#define _IO_H /* we are not compatible with emx io.h */
extern long filelength (int);
extern int ftruncate (int, long);
extern int ioctl (int, int request, ...);
extern int isatty (int);
extern long lseek (int, long, int);
extern int mkstemp (char *);
extern char *mktemp (char *);
extern int open (__const__ char *, int, ...);
extern int pipe (int *);
extern int sopen (__const__ char *, int, int, ...);
extern long tell (int);
extern int truncate (char *, long);

extern int _access (__const__ char *, int);
extern int _chmod (__const__ char *, int);
extern int _chsize (int, long);
extern int _close (int);
extern int _creat (__const__ char *, int);
extern int _crlf (char *, size_t, size_t *);
extern int _dup (int);
extern int _dup2 (int, int);
extern int _eof (int);
extern long _filelength (int);
extern int _ftruncate (int, long);
extern int _imphandle (int);
extern int _ioctl (int, int request, ...);
extern int _isatty (int);
extern int _isterm (int);
extern long _lseek (int, long, int);
extern int _mkstemp (char *);
extern char *_mktemp (char *);
extern int _open (__const__ char *, int, ...);
extern int _pipe (int *);
extern int _read (int, void *, size_t);
extern ssize_t _readlink(__const__ char*,char*,size_t);
extern int _seek_hdr (int);
extern int _select (int, struct _fd_set *, struct _fd_set *, struct _fd_set *,
    struct timeval *);
extern int _setmode (int, int);
extern int _sopen (__const__ char *, int, int, ...);
extern long _tell (int);
extern int _truncate (char *, long);
extern int _umask (int);
extern int _unlink (__const__ char *);
extern int _write (int, __const__ void *, size_t);
#endif /* IO_H */

extern int _getdtablesize(void);

extern int sync(void); 
extern int chown(__const__ char *path, uid_t owner, gid_t group);
extern int lchown(__const__ char *path, uid_t owner, gid_t group);

#ifndef	_POSIX_SOURCE

extern int fchown(int filedes, uid_t owner, gid_t group);
extern int gethostname(char*, socklen_t);
extern int getdomainname(char*,size_t);
extern int setdomainname(__const__ char*,size_t);
#undef getpagesize
extern __const__ int getpagesize(void);
#define getpagesize() 0x1000
extern char *getwd(char *buf);
extern void*valloc(size_t);
extern int getdtablesize(void);
char*getwd(char*);		/* obsoleted by getcwd() */
extern int gethostid(void); 	/* BSD: long gethostid(); */
extern int sethostid(long); 	/* dummy fn.; cf. tcpip configuration */
extern int seteuid(uid_t); 	/* dummy fn. */
extern int setegid(uid_t); 	/* dummy fn. */
extern int setreuid(uid_t,uid_t); 	/* dummy fn. */
extern int setregid(gid_t,gid_t); 	/* dummy fn. */
extern ssize_t readlink(__const__ char*,char*,size_t);
extern char *re_comp(__const__ char*); 	/* BSD 4.3 compatibility */
extern int re_exec(__const__ char *); 	/* BSD 4.3 compatibility */
extern int nice(int);
extern useconds_t ualarm(useconds_t,useconds_t);
extern mode_t getmode(__const__ void*,mode_t);
extern void *setmode(__const__ char*);

#endif /* ! _POSIX_SOURCE */ 

/* Machine-dependent inline versions */
#if !(!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))

extern __inline__ pid_t vfork(void) {return _fork();}
extern __inline__ int   sync(void)  {return _sync();}
extern __inline__ int   chown(__const__ char*p,uid_t o,gid_t g)
	{return _chown(p,o,g);}
#ifndef	_POSIX_SOURCE
extern __inline__ int fchown(int f,uid_t o,gid_t g){return _fchown(f,o,g);}
extern int issetugid(void); 	/* FixMe! No suid/sgid yet! */
#endif
extern __inline__ int lchown(__const__ char*p,uid_t o,gid_t g){return _chown(p,o,g);}

#endif 		/* !__GNUC__, etc. */

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
#endif /* (!defined(_POSIX_SOURCE) && !defined(_XOPEN_SOURCE)) || ... */

#if defined (__cplusplus)
}
#endif

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* _SYS_UNISTDX_H */
