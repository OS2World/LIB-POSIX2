/* statx.c -- extensions to the sys/stat.h interfaces -- Parts from:
 * sys/stat.c (emx+gcc) -- Copyright (c) 1992-1996 by Eberhard Mattes 
 * See: docs/Copying
 */
/* $Id: stat.c,v 1.2 2000/11/21 18:24:22 arnd Exp $ */ 

#define OS2_API32
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#include <os2.h>

#define _BSD_SOURCE
#define _KERNEL
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/errnox.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include <sys/mmanx.h>

static __inline__ const long int __statx_fTimeDate2Time_T(FTIME,FDATE);
static struct _dstswitch *find_switch(time_t t);

extern time_t _mktime(struct tm*); 



/* Return true iff adding A to T does not overflow time_t. */
#define ADD_OK(t,a) \
    ((a) < 0 ? (t) >= (time_t)(-(a)) \
   : (a) > 0 ? (t) <= _TIME_T_MAX - (a) \
   : 1)

static struct _tzinfo 
_tzi = {{0},{0},0,0,0,0,0,0,0,0,0,0,0};

static struct _dstswitch
_dstsw[2*(2106 - 1970 +1)+2] = {{0,0}, {_TIME_T_MAX, 0}};

static int 
_dstsw_count = 2;

	static struct _dstswitch *
find_switch(time_t t)
{
  int lo, hi, i;

  if (t == _TIME_T_MAX)
    {
      i = _dstsw_count - 2;
      assert (_dstsw[i].time <= t);
      assert (t <= _dstsw[i+1].time);
    }
  else
    {
      lo = 0; hi = _dstsw_count - 2;
      for (;;)
        {
          i = (lo + hi) / 2;
          if (_dstsw[i].time > t)
            hi = i - 1;
          else if (_dstsw[i+1].time <= t)
            lo = i + 1;
          else
            break;
        }
      assert (_dstsw[i].time <= t);
      assert (t < _dstsw[i+1].time);
    }
  return &_dstsw[i];
}

/* Note that this function returns -1 on error (overflow). */
	extern int 
_localTime2GMT(time_t *p, int is_dst)
{
  time_t x;
  struct _dstswitch *sw;
  int count;

  if (is_dst > 0) {
      /* Our caller says that *P is specified as DST.  Compute UTC
         from *P, assuming that daylight saving applies. */

      if (!ADD_OK (*p, _tzi.tz - _tzi.shift))
        return -1;
      x = *p + _tzi.tz - _tzi.shift;
      sw = find_switch (x);
      *p = x;
      return sw->shift != 0;
  } else 
    if (is_dst == 0) {
      /* Our caller says that *P is specified as standard time.
         Compute UTC from *P, assuming that daylight saving does not
         apply. */

      if (!ADD_OK (*p, _tzi.tz))
        return -1;
      x = *p + _tzi.tz;
      sw = find_switch (x);
      *p = x;
      return sw->shift != 0;
  } else {
   /* Our caller does not know whether *P is specified as DST or
         standard time.  Try to find out.  First try DST. */
      count = 0;
      if (ADD_OK (*p, _tzi.tz - _tzi.shift)) {
          x = *p + _tzi.tz - _tzi.shift;
          sw = find_switch (x);
          if (sw->shift != 0) {
              *p = x;
              return 1;         /* DST */
          }
          ++count;
      }

      if (ADD_OK (*p, _tzi.tz))
        {
          x = *p + _tzi.tz;
          sw = find_switch (x);
          if (sw->shift == 0)
            {
              *p = x;
              return 0;         /* Not DST */
            }
          ++count;
        }

      if (count != 2)
        return -1;              /* Overflow */

      /* Assume that DST does not apply in the gap.  This makes moving
         into the gap from below work, but breaks moving into the gap
         from above.  The advantage of this choice is that ftime()
         works correctly in the gap if the clock is not adjusted. */

      *p += _tzi.tz;
      return 0;                 /* Not DST */
    }
}

	extern int
_getDriveNumber(const char *name)
{
 ULONG DriveNumber;
 char driveLetter;

	if (name == NULL)
		{errno = EFAULT; return(-1);}
	if (name[1] == ':') {
		if ( name[0] >= 'A' && name[0] <= 'Z' ) 
			driveLetter = _tolower(name[0]);
		else {
			if ( name[0] >= 'a' && name[0] <= 'z' ) 
				driveLetter = name[0];
			else 
				{errno = EINVAL; return(-1);}
		}
		DriveNumber = driveLetter - 'a' + 1;
	}
	else {
	 ULONG DriveMap;
	 APIRET rc =
		DosQueryCurrentDisk(&DriveNumber, &DriveMap);
		if (rc) {
			_dPrintf(
"libExt: _getDriveNumber() DosQueryCurrentDisk() returns %ld!\n", rc);
			errno = _rc2Errno(rc); return(-1);
		}
	}
	return DriveNumber;
}

#undef __statfs

	extern int
_statfs(const char *path, struct __statfs *buf)
{

	if (path == NULL || buf == NULL)
		{errno = EFAULT; return(-1);}

	else {
	 ULONG 	DriveNumber = _getDriveNumber(path); 
	 /* fs type name */
	 char		DeviceName[MFSNAMELEN] = {DriveNumber + 'A' - 1, ':', 0};
	 FSALLOCATE 	FSInfoBuf;
	 FSQBUFFER2	FSQBuf;
	 ULONG   	DataBufferLen = sizeof(FSQBuf);
	 APIRET  	rc = 
    		DosQueryFSInfo(DriveNumber, FSIL_ALLOC, &FSInfoBuf,
			                            sizeof(FSALLOCATE));
		if (rc) {
		        _dPrintf("libExt: _statfs(): DosQueryFSInfo() returns %ld\n", rc);
		        errno = _rc2Errno(rc); return(-1);
		}
 		/* Fields that are undefined for a particular file 
		 * system are set to (-1).  
		 */
		__MM_bminus1((caddr_t)buf, sizeof(struct __statfs));

		  /* type of file system (unused; zero) */
		buf->f_type = 0;
		  /* fundamental file system block size */
		buf->f_bsize =  FSInfoBuf.cSectorUnit * FSInfoBuf.cbSector;
	 		     /* sectors per allocation unit * bytes per sector) */
		  /* optimal transfer block size */
		buf->f_iosize = 512;   /* assume physical size of 512 byte */
		  /* total data blocks in file system */
		buf->f_blocks = FSInfoBuf.cUnit; /* number of allocation units */
		  /* free blocks in file system */
		buf->f_bfree =  FSInfoBuf.cUnitAvail; 	/* total avail. units */
		  /* free blocks available to non-superuser */
		buf->f_bavail = FSInfoBuf.cUnitAvail;	/* total avail. units */
		  /* file system id */
		buf->f_fsid.val[0] = 0;
		buf->f_fsid.val[1] = FSInfoBuf.idFileSystem;
		  /* user that mounted the file system */
		buf->f_owner = 0;			/* fake root */

 		rc = DosQueryFSAttach(DeviceName, 0UL, FSAIL_QUERYNAME, 
			                    &FSQBuf, &DataBufferLen);
		if (rc) {
		        _dPrintf("libExt: _statfs(): DosQueryFSAttach() returns %ld\n", rc);
		        errno = _rc2Errno(rc); return(-1);
		}
		  /* fs type name */
		strncpy(buf->f_fstypename, FSQBuf.szFSDName, sizeof(buf->f_fstypename - 1));
		  /* directory on which mounted */   /* no mounts yet; FixMe! (AH) */
		strncpy(buf->f_mntonname, "", sizeof(buf->f_mntonname - 1));
		  /* mounted file system */   /* item name */
		strncpy(buf->f_mntfromname, FSQBuf.szName, sizeof(buf->f_mntonname - 1));
		  /* maximum length of filenames */
		if (NULL != strstr(buf->f_fstypename, "FAT"))
			buf->f_namelen = 256;
		else
			buf->f_namelen = 8;

		  /* mount flags */
		buf->f_flags = 0;
		if (NULL != strstr(buf->f_fstypename, "CDFS"))
			buf->f_flags &= MNT_RDONLY;
		if (FSQBuf.iType == FSAT_LOCALDRV) 
			buf->f_flags |= MNT_LOCAL;
		else if (FSQBuf.iType != FSAT_REMOTEDRV) {
			errno = ENOSYS; return(-1);
		}

		return(0);
	}
}


	static __inline__ const long int
__statx_fTimeDate2Time_T(const FTIME ft, const FDATE fd)
{
 struct tm tmBuf;
	tmBuf.tm_sec = ft.twosecs * 2;
	tmBuf.tm_min = ft.minutes;
	tmBuf.tm_hour = ft.hours;
	tmBuf.tm_mday = fd.day;
	tmBuf.tm_mon = fd.month - 1;
	tmBuf.tm_year = fd.year + 1980 - 1900;
	tmBuf.tm_isdst = -1;             /* unknown */
	return _mktime(&tmBuf);
}
							
#define FTIMEZEROP(x) (*(USHORT*)&(x) == 0)
#define FDATEZEROP(x) (*(USHORT*)&(x) == 0)

long __statx_inodeNumberCounter = 0x100000; /* global inode counter */


#undef __stat

 	extern int 
_stat(const char *name, struct stat *statPbuf)
{
 const size_t
  len = strlen(name); /* Strip any trailing slash/backslash: */

  if ( _strHasTrailingSlash(name, len, 1) ) { /* trailing slash detected... */
    if ((statPbuf->st_mode & S_IFMT) != S_IFDIR) { /* but not a directory !? */
      errno = ENOTDIR; return(-1);
    } else {
     char *tmp = __builtin_alloca(len);	/* using local copy tmp */
      __builtin_memcpy(tmp, name, len - 1);
      tmp[len-1] = 0; /* zero the trailing */
      name = tmp;
    }
  }
  __MM_bzero((caddr_t)statPbuf, sizeof(*statPbuf));

  if ((name[0] == '/' || name[0] == '\\') && strlen(name) >= 6 &&
      _memicmp(name+1, "pipe", 4) == 0 && (name[5] == '/' || name[5] == '\\')) {
	errno = ENOENT;
	return -1; /* Fix me ! Todo: Posix named pipes/mkfifo() */
  } else {
  APIRET rc;
  FILESTATUS3 	filestatus3Info;
  FSALLOCATE 	fsallocBuf;
  ULONG  	DriveNumber = _getDriveNumber(name);
	
	rc = DosQueryPathInfo(name, FIL_STANDARD, (PFILESTATUS3)&filestatus3Info,
  			sizeof(filestatus3Info));
	if (rc) {
		_dPrintf("statx.c: stat(): DosQueryPathInfo(): Error # %ld\n", rc);
		errno = _rc2Errno(rc); return(-1);
	}
  statPbuf->st_attr = filestatus3Info.attrFile;
  statPbuf->st_mtime = __statx_fTimeDate2Time_T(filestatus3Info.ftimeLastWrite,
						 filestatus3Info.fdateLastWrite);
  if (FTIMEZEROP (filestatus3Info.ftimeCreation) &&
      FDATEZEROP (filestatus3Info.fdateCreation))
    statPbuf->st_ctime = statPbuf->st_mtime;
  else
    statPbuf->st_ctime = __statx_fTimeDate2Time_T(filestatus3Info.ftimeCreation,
						 filestatus3Info.fdateCreation);
  if (FTIMEZEROP(filestatus3Info.ftimeLastAccess) &&
      		FDATEZEROP(filestatus3Info.fdateLastAccess))
    statPbuf->st_atime = statPbuf->st_mtime;
  else
    statPbuf->st_atime = __statx_fTimeDate2Time_T(filestatus3Info.
    				ftimeLastAccess, filestatus3Info.fdateLastAccess);

  rc = DosQueryFSInfo(DriveNumber, FSIL_ALLOC, &fsallocBuf, sizeof(FSALLOCATE));
  if (rc) {
    _dPrintf("statx.c: stat(): DosQueryFSInfo(): Error # %ld\n", rc);
    errno = _rc2Errno(rc);
    return(-1);
  }
  statPbuf->st_blksize = fsallocBuf.cSectorUnit * fsallocBuf.cbSector;
 		/* sectors per allocation unit * bytes per sector) */
  statPbuf->st_flags   = 0; /* unused; for bsd compatibility */
  statPbuf->st_gen     = 0; /* unused; for bsd compatibility */
  statPbuf->st_lspare  = 0; /* unused; for bsd compatibility */
  statPbuf->st_qspare[0] = 0LL; /* unused; for bsd compatibility */
  statPbuf->st_qspare[1] = 0LL; /* unused; for bsd compatibility */

  if (filestatus3Info.attrFile & 0x10) { /* directory */
      statPbuf->st_mode = S_IFDIR;
      statPbuf->st_mode |= ((S_IREAD|S_IWRITE|S_IEXEC) >> 6) * 0111;
      statPbuf->st_size = 0;
      statPbuf->st_blocks = 1;
  }  else {
      statPbuf->st_size = filestatus3Info.cbFile;
      statPbuf->st_blocks = (statPbuf->st_size / statPbuf->st_blksize) + 1;
      statPbuf->st_mode = S_IFREG;
      if (filestatus3Info.attrFile & 1)
        statPbuf->st_mode |= (S_IREAD >> 6) * 0111;
      else
        statPbuf->st_mode |= ((S_IREAD|S_IWRITE) >> 6) * 0111;
  }
  /* The following is reserved for future Posix-file-system emulation */
  statPbuf->st_reserved = 0;
  statPbuf->st_uid = 0; 
  statPbuf->st_gid = 0;
  statPbuf->st_ino = __statx_inodeNumberCounter++; /* incremented; fix me ! */
  if (__statx_inodeNumberCounter == 0)
  	__statx_inodeNumberCounter = 1; /* wrap? */
  statPbuf->st_dev = DriveNumber;
  statPbuf->st_rdev = 0;
  statPbuf->st_nlink = 1;

  if (!_tzset_flag)
    _tzset();
  _localTime2GMT(&statPbuf->st_atime, -1);
  _localTime2GMT(&statPbuf->st_mtime, -1);
  _localTime2GMT(&statPbuf->st_ctime, -1);

  if ((statPbuf->st_mode & S_IFMT) == S_IFREG) {
    const char*buf = _getext(name);
    if (buf && (_stricmp(buf, ".exe") == 0 || 	/* depend on suffix: Fix me! */
               _stricmp(buf, ".com") == 0 ||
               _stricmp(buf, ".cmd") == 0 ||
               _stricmp(buf, ".bat") == 0) )
        statPbuf->st_mode |= (S_IEXEC >> 6) * 0111;
  }
  return(0);
 }
}
	extern int
_fstat(int handle, struct stat *statPbuf)
{
 ULONG htype, hflags;
 const FILESTATUS3 filestatusInfo;
 ULONG rc = 
  DosQueryHType((HFILE)handle, &htype, &hflags);
  if (rc) {
    errno = _rc2Errno(rc); return(-1);
  }
  __MM_bzero((caddr_t)statPbuf, sizeof(*statPbuf));
  switch(htype & 0xff) {
    case 1:
      statPbuf->st_mode = S_IFCHR;
      break;
    case 2:
      statPbuf->st_mode = S_IFSOCK;
      break;
    default:
      statPbuf->st_mode = S_IFREG;
      break;
  }
	if (statPbuf->st_mode == S_IFREG) {
	
      rc = DosQueryFileInfo((HFILE)handle, FIL_STANDARD,
      			(PFILESTATUS3)&filestatusInfo, sizeof(filestatusInfo));
      if (rc) { errno = _rc2Errno(rc); return -1; }
#if 0
	else {
	 const FILESTATUS3  filestatus3Info;
	 FSALLOCATE fsallocBuf;
	 ULONG  DriveNumber; /* unsafe for double byte code pages (DBCS). Fix me ! */

		DriveNumber = _fngetdrive(name) & 0x1f;  /* # 1 - 26; 0 current. So 
				use only the lower 5 bits. Todo: Rewrite for DBCS !*/
		if (!DriveNumber)
			DriveNumber = _getdrive() & 0x1f; 
#endif
      statPbuf->st_attr = filestatusInfo.attrFile;
      statPbuf->st_size = filestatusInfo.cbFile;
      statPbuf->st_mtime = __statx_fTimeDate2Time_T(filestatusInfo.ftimeLastWrite, 					filestatusInfo.fdateLastWrite);
      if (FTIMEZEROP (filestatusInfo.ftimeCreation) &&
          FDATEZEROP (filestatusInfo.fdateCreation))
        statPbuf->st_ctime = statPbuf->st_mtime;
      else
        statPbuf->st_ctime = __statx_fTimeDate2Time_T(filestatusInfo.ftimeCreation,
 			filestatusInfo.fdateCreation);
      if (FTIMEZEROP (filestatusInfo.ftimeLastAccess) &&
          FDATEZEROP (filestatusInfo.fdateLastAccess))
        statPbuf->st_atime = statPbuf->st_mtime;
      else
        statPbuf->st_atime = __statx_fTimeDate2Time_T(filestatusInfo.ftimeLastAccess,
				 filestatusInfo.fdateLastAccess);
      if (filestatusInfo.attrFile & 1)
        statPbuf->st_mode |= (S_IREAD >> 6) * 0111;
      else
        statPbuf->st_mode |= ((S_IREAD|S_IWRITE) >> 6) * 0111;
  } 
  else { 					/* not a regular file */
      statPbuf->st_size = 0; 	/* This is a hack! */
      rc = DosQueryFHState((HFILE)handle, &hflags);
      if (rc) { errno = _rc2Errno(rc); return -1; }
      if ((hflags & 7) == 0)
        statPbuf->st_mode |= (S_IREAD >> 6) * 0111;
      else
        statPbuf->st_mode |= ((S_IREAD|S_IWRITE) >> 6) * 0111;
  }
  statPbuf->st_reserved = 0;
  statPbuf->st_uid = 0; 
  statPbuf->st_gid = 0;
  statPbuf->st_ino = __statx_inodeNumberCounter++; /* incremented; fix me ! */
  if (__statx_inodeNumberCounter == 0)
  	__statx_inodeNumberCounter = 1; /* wrap? */
  statPbuf->st_dev   = 0;
  statPbuf->st_rdev  = 0;
  statPbuf->st_nlink = 1;
 /* BSD compatibility hacks: Fix me! */
  statPbuf->st_blksize = 512; /* preferred blocksize: Assume HPFS: 512; Fix me! */
  statPbuf->st_blocks  = (statPbuf->st_size / 512) + 1;
  statPbuf->st_flags   = 0; /* unused; for bsd compatibility */
  statPbuf->st_gen     = 0; /* unused; for bsd compatibility */
  if (!_tzset_flag)
    _tzset();
  _localTime2GMT(&statPbuf->st_atime, -1);
  _localTime2GMT(&statPbuf->st_mtime, -1);
  _localTime2GMT(&statPbuf->st_ctime, -1);

  return(0);
}


/* --- lstat() dummy implementation --- released into Public Domain */
	extern int
_lstat(const char *path, struct stat *statPbuf)
{
 const int statrc =
 	_stat(path, statPbuf);  /* to ease debugging */
  
  fputs("libextensions: _lstat(): _lstat() mapped to _stat()!\n",stderr);
  return(statrc);
}
   /* lstat() is the very same as stat() except when a symbolic
   link is being specified: while stat() resolves the link, lstat() 
   doesn't dereference it.
   However to be on the safe for future enhancements we
   don't #define lstat to stat even if by default OS/2
   has no filesystems supporting symbolic links.
   To be on the safe side we create an entry point for
   lstat() in libextension however!

   The linux manpage gives lstat as conforming to 4.3BSD and SVr4,
   it also appears in SUSV2.  */



/* fchmod():   change permission of a file.  -- emx/gcc --     */

	extern int  			/* References:  4.4BSD and SVr4 */
_fchmod(const int filedes, const mode_t mode)
{
	/* our implementation has similar limits as chmod(), therefore
	 * read the according section in the EMX docs.
	 * TODO: improve analysis when it fails, see below
	   */
	if (mode < S_IXOTH || mode > 
			(S_ISUID|S_ISGID|S_ISVTX|S_IRWXU|S_IRWXG|S_IRWXO) ) {
		errno = EINVAL;	return(-1);
	} else {
	 const int htype;
		if ( _ioctl(filedes, FGETHTYPE, (int)&htype) ) { /* is fd okay? */
			/* errno = EBADF; */ /* errno set by ioctl */
			return(-1);
		}
		if ( HT_ISDEV(htype) ) {  /* cannot fchmod() clock, ttys, etc. */
			errno = EINVAL; return(-1);
		}
		/* we assume that _POSIX_SOURCE is not! defined now: 
		    file _must_ be writable/readable or readable */
		if (!(mode & S_IREAD)) {
			fputs("\
libextensions: fchmod(): File is not readable. FAT and HPFS do not allow this\n",stderr);
			errno = EINVAL;	return(-1);
		}
	}{		 		/* end of sanity checks; begin of main block */
	 FILESTATUS3 fsts3FileInfo;				/* file information   */
	 ULONG attr = FILE_READONLY;
	 APIRET rc = 		  				/* Get standard info */
		DosQueryFileInfo((HFILE)filedes, FIL_STANDARD, &fsts3FileInfo,
		 					sizeof(FILESTATUS3));
		if (rc) {
				_dPrintf("DosQueryFileInfo error: return code = %lu\n", rc);
				errno = _rc2Errno(rc); return(-1);
		}
		if (mode & S_IWRITE)
			attr = FILE_NORMAL; 
		fsts3FileInfo.attrFile  = attr;

		rc = DosSetFileInfo((HFILE)filedes, FIL_STANDARD,
					(PFILESTATUS3)&fsts3FileInfo, sizeof(FILESTATUS3));
		if (rc) {
			_dPrintf("DosSetfFileInfo error: return code = %lu\n", rc);
			errno = _rc2Errno(rc); return(-1);
		}
		return 0; /* success */
	}
}


/*
 * Returns non-zero if the argument file is a directory, or is a symbolic
 * link which points to a directory.
 */
	extern int
_isdir(const char *file)
{
    struct stat sb;

    if (stat(file, &sb) < 0)
	return (0);
    return (S_ISDIR(sb.st_mode));
}

/*
 * Returns non-zero if the argument file is a symbolic link.
 */
	extern int
_islink(const char *file)
{
#ifdef S_ISLNK
    struct stat sb;

    if (lstat(file, &sb) < 0)
	return (0);
    return (S_ISLNK(sb.st_mode));
#else
    return (0);
#endif
}

/*
 * Returns non-zero if the argument file exists.
 */
	extern int
_isfile(const char *file)
{
    struct stat sb;

    if (stat(file, &sb) < 0)
	return (0);
    return (1);
}

/*
 * Returns non-zero if the argument file is readable.
 * XXX - must be careful if "cvs" is ever made setuid!
 */
	extern int
_isreadable(const char *file) { return(access(file, R_OK) != -1); }

/*
 * Returns non-zero if the argument file is writable
 * XXX - muct be careful if "cvs" is ever made setuid!
 */
	extern int
_iswritable(const char *file) { return(access(file, W_OK) != -1); }

/*
 * Returns non-zero if the argument file is accessable according to
 * mode.  If compiled with SETXID_SUPPORT also works if cvs has setxid
 * bits set.
 */
	extern int
_isaccessible(const char *file, const mode_t mode) { return access(file, mode) == 0; }

/* Return non-zero iff FILENAME is absolute.
   */
	extern int
_isabsolute(const char *string)
{
 return( _ISABSPATH(string) );
}

extern int statfs(const char*p,struct __statfs*b){return _statfs(p,b);}
extern int fstat(const int h,struct stat*b){return _fstat(h,b);}
extern int stat(const char*p,struct stat*b){return _stat(p,b);}
extern int lstat(const char*p,struct stat*b){return _lstat(p,b);}
/* lchmod(): a wrapper for chmod, until we implement symlinks */
extern int _lchmod(const char*p,const mode_t m){return _chmod(p,m);}
extern int lchmod(const char*p,const mode_t m){return _chmod(p,m);}
extern int fchmod(const int f,const mode_t m){return _fchmod(f,m);}
