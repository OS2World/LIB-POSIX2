/* 	$Id: stubs.c,v 1.5 2000/11/25 16:33:36 arnd Exp $ 
 *
 * (C) 2000 P/2-Team: All rights reserved.
 * (C) 2000 Arnd Hanses: All rights reserved.
 * This file is released into Public Domain
 */

/* chown() is in SVr4, SVID, POSIX, X/OPEN: Change owner/group of file.
   Meaningless on plain OS/2 with FAT/HPFS.
   I leave the details on other file systems for other people 
   and provide the stub only! */

#define _BSD_SOURCE
#define _KERNEL
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <grp.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

	extern int
_chown(const char *file, uid_t user, gid_t group)
{
	if (!file) {
		errno = EFAULT; return(-1);
	}
	if (user < 0 || group < 0) {
		errno = EINVAL; return(-1);
	}
	_dPuts("chown(): not yet really implemented!\n");
	return (0);
}   /* If we assume we don't have any "owners" and 
	"groups" I guess it's best to keep the apps happy and return 0
	instead of indicating a failure ...  */


/* fchown() is in SVr4, SVID, X/OPEN: Change owner/group of file.
   Meaningless on plain OS/2 with FAT/HPFS.
   I leave the details on other file systems for other people 
   and provide the stub only! */
	extern int
_fchown(const int fd, uid_t user, gid_t group)
{
 int htype;
	if ( ioctl(fd,FGETHTYPE,&htype) ) { 		/* is fd okay? */
		/* errno = EBADF; */ /* errno set by ioctl */
		return(-1);
	}
	if ( HT_ISDEV(htype) ) { /* cannot chown clock, ttys, etc. */
		errno = ENODEV; return(-1);
	}
 	if (user < 0 || group < 0) {
		errno = EINVAL; return(-1);
	}
	_dPuts("chown(): not yet really implemented!\n");
  return (0); 	  /* If we assume we don't have any "owners" and 
     "groups" I guess it's best to keep the apps happy and return 0
     instead of indicating a failure ...
      */
}


	extern int 
_readlink(const char *__pathname, char *__buf, size_t __bufsiz)
{
 struct stat st;
 size_t len;

	if (!__buf || (__buf + __bufsiz) > __MAXADDRESS)
		{errno = EFAULT; return(-1);}
	if (!__pathname)
		{errno = ENOENT; return(-1);}
	if ((long)__bufsiz < 0L)
		{errno = EINVAL; return(-1);}
	if ((len = strlen(__pathname)) > PATH_MAX)
		{errno = ENAMETOOLONG; return(-1);}
		
	if (_stat(__pathname, &st) == (-1))
		return(-1);

	/* no symbolic links to read are implemented */
#define	S_ISLNK(m) (0) 				/* :-( */
	if (S_ISLNK(st.st_mode)) 		
		return strlen( __builtin_memcpy(__buf, __pathname, 
					MIN(__bufsiz, len)) );

	errno = EINVAL; 
	return(-1);
}

	extern int 
readlink(const char *p, char *b, size_t s){return _readlink(p,b,s);}

/* FixMe! No suid/sgid yet! */
	extern int
issetugid(void) {return(0);}

	extern pid_t
vfork(void) {return _fork();}

	extern uid_t
geteuid(void)
{
	return(0); 	/* FixMe: no security yet: always root (AH) */
}


/* Stubs for user/group id management; (-1) leaves them unchanged */

	extern int
seteuid(uid_t uid)
{
	if (uid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}
	extern int
setegid(uid_t uid)
{
	if (uid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}

	extern int
initgroups(const char *user, gid_t grp)
{
	if (grp < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}

	extern int
setreuid(uid_t ruid, uid_t euid)
{
	if (ruid < (-1) || euid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}
	extern int
setregid(gid_t rgid, gid_t egid)
{
	if (rgid < (-1) || egid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}

#if 0
	extern int
setgid(uid_t gid)
{
	if (gid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}
	extern int
setuid(uid_t uid)
{
	if (uid < (-1)) {errno = EINVAL; return(-1);}
	return(0); 	/* FixMe: no security yet: always root (AH) */
}
	extern uid_t
getuid(void)
{
	return(0); 	/* FixMe: no security yet: always root (AH) */
}
#endif /* 0 */

/* Two stubs for now, Fix me! (AH) */

	extern int
_mkfifo(const char *path, mode_t mode)
{
	if (path == NULL)
		{errno = EFAULT; return(-1);}
	if (mode < 0 || mode > DEFFILEMODE)
		{errno = EINVAL; return(-1);}

	errno = EOPNOTSUPP;
	return(-1);
}
	extern int
_mknod(const char *path, mode_t mode, dev_t dev)
{
	if (path == NULL)
		{errno = EFAULT; return(-1);}
	if (mode < 0 || mode > DEFFILEMODE)
		{errno = EINVAL; return(-1);}

	errno = EROFS;
	return(-1);
}

/* Fix me! Implement attributes (AH) */
	extern int
_chflags(const char *path, unsigned int flags)
{
	if (!path)
		{errno = EFAULT; return(-1);}
	if (flags == UF_IMMUTABLE)
		return chmod(path, S_IREAD);

	errno = EOPNOTSUPP;
	return(-1);
}

	extern int
_fchflags(int fd, unsigned int flags)
{
	if (fd < 0)
		{errno = EINVAL; return(-1);}

	errno = EOPNOTSUPP;
	return(-1);
}

extern int chown(const char*d1,uid_t d2,gid_t d3){return _chown(d1,d2,d3);}
extern int _lchown(const char*d1,uid_t d2,gid_t d3){return _chown(d1,d2,d3);}
extern int lchown(const char*d1,uid_t d2,gid_t d3){return _chown(d1,d2,d3);}
extern int fchown(const int d1,uid_t d2,gid_t d3){return _fchown(d1,d2,d3);}
extern int mkfifo(const char *p, mode_t m){return _mkfifo(p,m);}
extern int mknod(const char *p, mode_t m, dev_t d){return _mknod(p,m,d);}
extern int chflags(const char*p,unsigned m){return _chflags(p,m);}
extern int fchflags(int f,unsigned m){return _fchflags(f,m);}


/* This API set *grent() is deprecated and the interface is
   for compatibility with legacy code only.
   It was supposed to return entries from the group database
   (etc/group).
   getgrent(): return pointer to group entries
   setgrent(): rewind "pointer", so the next call go getgrent() will 
               return the first entry from database
   endgrent(): close database
   CONFORMING TO SVID 3, BSD 4.3 */

static int database_open=0; /* status of "virtual user database:
                               0=closed, 1=open
                               in fact 0 does only indicate to return the
                               "first" entry in our current implementation */


	extern struct group *	/* return group entries */
getgrent(void)
{
  if (0==database_open) {
     database_open=1;
  }
  else {
  }
  return (struct group*)NULL;	  /* shall we return a value here ?? */
}

	extern void 	/* "rewind pointer to database entries" */
setgrent(void)
{
  database_open=0;
}

	extern void 	  /* "close" database */
endgrent(void)
{
  database_open = 0;
}
