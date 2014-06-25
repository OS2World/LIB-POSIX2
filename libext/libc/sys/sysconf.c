/* libext+gcc: $Id: sysconf.c,v 1.2 2000/10/11 14:48:56 arnd Exp $ */

/*-
 * Copyright (c) 1993
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

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/kernel.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../rev.h"
	
	static int
__sysconf_sysctlKern(int name, void *oldp, 
			size_t *oldlenp, void *newp, size_t newlen)
{
	 switch (name) {
	case KERN_MAXFILES:
		if (newp) {
			if (newlen != sizeof(int))
				{errno = EINVAL; return(-1);}
			if (_setMaxFiles(*(int*)newp) == (-1))
				return(-1);
		}
		else if (oldp) 
			*(int*)oldp = __Param_maxfiles;
		if (oldlenp)
			*oldlenp = sizeof(int);
		return(0);
	case KERN_HOSTID:
		if (newp) {
			if (newlen != sizeof(int))
				{errno = EINVAL; return(-1);}
			__Param_hostid = *(int*)newp;
	 }
		else if (oldp) 
			__Param_hostid = gethostid();
			*(int*)oldp = __Param_hostid;
		if (oldlenp)
			*oldlenp = sizeof(int);
		return(0);
	 } /* end of switch(name) */

	if (newp)
		{errno = EPERM; return(-1);}
	if (oldlenp != NULL)
		*oldlenp = 0;
	 switch (name) {
	case KERN_OSTYPE:
		 if (oldp) {
		 	(char*)oldp = ostype;
			*oldlenp = strlen(ostype);
		 }
		 return (0);
	case KERN_OSRELEASE:
		 if (oldp) {
		 	(char*)oldp = osrelease;
			*oldlenp = strlen(osrelease);
		 }
		 return (0);
	case KERN_OSVERSION:
		 if (oldp) {
		 	(char*)oldp = osversion;
			*oldlenp = strlen(osversion);
		 }
		 return (0);
	case KERN_VERSION:
		 if (oldp) {
		 	(char*)oldp = version;
			*oldlenp = strlen(version);
		 }
		 return (0);
	case KERN_HOSTNAME:
		 if (oldp) {
			(char*)oldp = __Param_hostname;
			*oldlenp = strlen(__Param_hostname);
		 }
		return(0);
	case KERN_DOMAINNAME:
		 if (oldp) {
			(char*)oldp = __Param_domainname;
			*oldlenp = strlen(__Param_domainname);
		 }
		return(0);
	 }

	if (oldp && *oldlenp < sizeof(int))
		{errno = ENOMEM; return(-1);}
	if (oldlenp != NULL)
		*oldlenp = sizeof(int);
	if(oldp == NULL)
		return(0);

	 switch (name) {
	case KERN_OSREV:
		*(int*)oldp = _POSIXOS2_VERSION;
		 return (0);
	case KERN_MAXPROC:
		*(int*)oldp = __Param_maxproc;
		return(0);
 	case KERN_ARGMAX:
		*(int*)oldp = ARG_MAX;
		return (0);
	case KERN_SECURELVL:
		*(int*)oldp = __Param_securelevel;
		return (0);
	case KERN_CLOCKRATE: 
		(struct clockinfo*)oldp = _getClockInfo();
		if (oldp == NULL)
			return(-1);
		*oldlenp = sizeof(struct clockinfo);
		return(0);
	case KERN_BOOTTIME: 
		(struct timeval*)oldp = _bootTime();
		if (oldp == NULL)
			return(-1);
		*oldlenp = sizeof(struct timeval);
		return(0);
#if 0
	case KERN_VNODE:
		return (sysctl_vnode(oldp, oldlenp, p));
	case KERN_PROC:
		return (sysctl_doproc(name + 1, namelen - 1, oldp, oldlenp));
	case KERN_FILE:
		return (sysctl_file(oldp, oldlenp));
#ifdef GPROF
	case KERN_PROF:
		return (sysctl_doprof(name + 1, namelen - 1, oldp, oldlenp,
		    newp, newlen));
#endif
#endif
	case KERN_POSIX1:
		*(int*)oldp = _POSIX_VERSION;
		return(0);
	case KERN_NGROUPS:
		*(int*)oldp = NGROUPS_MAX;
		return(0);
	case KERN_JOB_CONTROL:
		*(int*)oldp = 1;
		return(0);
	case KERN_SAVED_IDS:
#ifdef _POSIX_SAVED_IDS
		*(int*)oldp = 1;
		return(0);
#else
		*(int*)oldp = 0;
		return(0);
#endif
	case KERN_MAXPARTITIONS:
		*(int*)oldp = MAXPARTITIONS;
		return(0);
#if 0
	case KERN_RAWPARTITION:
		*(int*)oldp = oldp, oldlenp, newp, RAW_PART));
	case KERN_NTPTIME:
		return (sysctl_ntptime(oldp, oldlenp));
	case KERN_SOMAXCONN:
		return (sysctl_int(oldp, oldlenp, newp, newlen, &somaxconn));
	case KERN_SOMINCONN:
		return (sysctl_int(oldp, oldlenp, newp, newlen, &sominconn));
	case KERN_USERMOUNT:
		return (sysctl_int(oldp, oldlenp, newp, newlen, &usermount));
	case KERN_RND:
		return (sysctl_rdstruct(oldp, oldlenp, newp, &rndstats,
		    sizeof(rndstats)));
	case KERN_ARND:
		*(int*)oldp = oldp, oldlenp, newp, arc4random()));
#endif
	case KERN_NOSUIDCOREDUMP:
		if (strstr(getenv("EMXOPT"),"-c") || strstr(getenv("EMXOPT"),"-C"))
			*(int*)oldp = 1;
		else
			*(int*)oldp = 0;
		return(0);
	case KERN_FSYNC:
		*(int*)oldp = 0;
		return(0);
	case KERN_SYSVMSG:
		*(int*)oldp = 1;
		return(0);
	case KERN_SYSVSEM:
#ifdef SYSVSEM
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return(0);
	case KERN_SYSVSHM:
		*(int*)oldp = 1;
		return(0);
	default:
		errno = ENOTSUP;
		return(-1);
	 }
}
	static int
__sysconf_sysctlHW(int name, void *oldp, 
			size_t *oldlenp, void *newp, size_t newlen)
{
	if (newp || newlen)
		{errno = EPERM; return(-1);}

	 switch (name) {
	case HW_MACHINE:
		if (oldp) (char*)oldp = machine;
		if (oldlenp) *oldlenp = strlen(machine);
		return(0);
	case HW_MODEL:
		if (oldp) (char*)oldp = cpu_model;
		if (oldlenp) *oldlenp = strlen(cpu_model);
		return(0);
	 }

	if (oldp == NULL && oldlenp != NULL) {
		*oldlenp = sizeof(int);
		return(0);
	}
	if (oldlenp != NULL) {
		if (*oldlenp < sizeof(int))
			{errno = ENOMEM; return(-1);}
		*oldlenp = sizeof(int);
	}

	 switch (name) {
	case HW_NCPU:
		*(int*)oldp = 1;
		return(0);
	case HW_BYTEORDER:
		*(int*)oldp = BYTE_ORDER;
		return(0);
	case HW_PHYSMEM: {
		const int hwMem = _getTotalPhysMem();
		if (hwMem == -1) 
			return(-1);
		*(int*)oldp = hwMem;
		return(0);
	}
	case HW_USERMEM: {
		const int hwMem = _getTotalPhysMem();
		if (hwMem == -1) 
			return(-1);
		*(int*)oldp = hwMem - 3*1024*1024; /* spare 3Mb for kernel */
		return(0);
	}
	case HW_PAGESIZE:
		*(int*)oldp = PAGE_SIZE;
		return(0);
	default:
		errno = EOPNOTSUPP;
		return(-1);
	}
	/* NOTREACHED */
}

	extern int
_sysctl(int *name, size_t namelen, void *oldp, size_t *oldlenp,
	 void *newp, size_t newlen)
{
	/*
	 * all top-level sysctl names are non-terminal
	 */
	if (namelen > CTL_MAXNAME || namelen < 2)
		{errno = EINVAL; return(-1);}
	if ( namelen != 2 )
		{errno = EINVAL; return(-1);}
	if (name[0] == CTL_KERN) 
		return( __sysconf_sysctlKern(name[1], oldp, oldlenp,
								newp, newlen) );
	if (name[0] == CTL_HW)
		return( __sysconf_sysctlHW(name[1], oldp, oldlenp,
								newp, newlen) );
	/* start of last block: User level control parameters */
	if (newp != NULL) {
		errno = EPERM; return(-1);
	}
	if (name[0] != CTL_USER) 
		{errno = EINVAL; return(-1);}
	switch (name[1]) {
	case USER_CS_PATH:
		if (oldp && *oldlenp < sizeof(_PATH_STDPATH))
			{errno = ENOMEM; return(-1);}
		if (oldlenp != NULL) *oldlenp = sizeof(_PATH_STDPATH);
		if (oldp != NULL)
			memmove(oldp, _PATH_STDPATH, sizeof(_PATH_STDPATH));
		return(0);
	}
	if (oldp && *oldlenp < sizeof(int))
		return(ENOMEM);
	if (oldlenp != NULL) *oldlenp = sizeof(int);
	if(oldp == NULL)
		return(0);

	switch (name[1]) {
	case USER_BC_BASE_MAX:
		*(int*)oldp = BC_BASE_MAX;
		return (0);
	case USER_BC_DIM_MAX:
		*(int*)oldp = BC_DIM_MAX;
		return (0);
	case USER_BC_SCALE_MAX:
		*(int*)oldp = BC_SCALE_MAX;
		return (0);
	case USER_BC_STRING_MAX:
		*(int*)oldp = BC_STRING_MAX;
		return (0);
	case USER_COLL_WEIGHTS_MAX:
		*(int*)oldp = COLL_WEIGHTS_MAX;
		return (0);
	case USER_EXPR_NEST_MAX:
		*(int*)oldp = EXPR_NEST_MAX;
		return (0);
	case USER_LINE_MAX:
		*(int*)oldp = LINE_MAX;
		return (0);
	case USER_RE_DUP_MAX:
		*(int*)oldp = RE_DUP_MAX;
		return (0);
	case USER_POSIX2_VERSION:
		*(int*)oldp = _POSIX2_VERSION;
		return (0);
	case USER_POSIX2_C_BIND:
/*** Adapt the Makefile accordingly: 
# Define the relevant options that your system configuration supports:
#
# POSIX2_CHAR_TERM
# Define if if the system supports at least one terminal type capa-
#             ble of all operations described in POSIX 1003.2.
# POSIX2_C_BIND
# Define if if the system's C-language development facilities sup-
#             port the C-Language Bindings Option.
# POSIX2_C_DEV
# Define if if the system supports the C-Language Development Utili-
#             ties Option.
# POSIX2_FORT_DEV
# Define if if the system supports the FORTRAN Development Utilities
#             Option.
# POSIX2_FORT_RUN
# Define if if the system supports the FORTRAN Runtime Utilities Op-
#             tion.
# POSIX2_LOCALEDEF
# Define if if the system supports the creation of locales, other-
#             wise 0.
# POSIX2_SW_DEV
# Define if if the system supports the Software Development Utili-
#             ties Option.
# POSIX2_UPE
# Define if if the system supports the User Portability Utilities
#             Option.
	***/
#ifdef POSIX2_C_BIND
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_C_DEV:
#ifdef	POSIX2_C_DEV
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_CHAR_TERM:
#ifdef	POSIX2_CHAR_TERM
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_FORT_DEV:
#ifdef	POSIX2_FORT_DEV
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_FORT_RUN:
#ifdef	POSIX2_FORT_RUN
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_LOCALEDEF:
#ifdef	POSIX2_LOCALEDEF
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_SW_DEV:
#ifdef	POSIX2_SW_DEV
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_POSIX2_UPE:
#ifdef	POSIX2_UPE
		*(int*)oldp = 1;
#else
		*(int*)oldp = 0;
#endif
		return (0);
	case USER_STREAM_MAX:
		*(int*)oldp = FOPEN_MAX;
		return (0);
	case USER_TZNAME_MAX:
		*(int*)oldp = NAME_MAX;
		return (0);
	default:
		errno = EINVAL;
		return (-1);
	}
	/* NOTREACHED */
}

/*
 * sysconf --
 *	get configurable system variables.
 *
 * XXX
 * POSIX 1003.1 (ISO/IEC 9945-1, 4.8.1.3) states that the variable values
 * not change during the lifetime of the calling process.  This would seem
 * to require that any change to system limits kill all running processes.
 * A workaround might be to cache the values when they are first retrieved
 * and then simply return the cached value on subsequent calls.  This is
 * less useful than returning up-to-date values, however.
 */
	extern long
_sysconf(int name)
{
	struct rlimit rl;
	size_t len;
	int mib[2], value;

	len = sizeof(value);

	switch (name) {
/* 1003.1 */
	case _SC_ARG_MAX:
		mib[0] = CTL_KERN;
		mib[1] = KERN_ARGMAX;
		break;
	case _SC_CHILD_MAX:
		return (_getrlimit(RLIMIT_NPROC, &rl) ? -1 : rl.rlim_cur);
	case _SC_CLK_TCK:
		return (CLK_TCK);
	case _SC_JOB_CONTROL:
		mib[0] = CTL_KERN;
		mib[1] = KERN_JOB_CONTROL;
		goto yesno;
	case _SC_NGROUPS_MAX:
		mib[0] = CTL_KERN;
		mib[1] = KERN_NGROUPS;
		break;
	case _SC_OPEN_MAX:
		return (_getrlimit(RLIMIT_NOFILE, &rl) ? -1 : rl.rlim_cur);
	case _SC_STREAM_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_STREAM_MAX;
		break;
	case _SC_TZNAME_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_TZNAME_MAX;
		break;
	case _SC_SAVED_IDS:
		mib[0] = CTL_KERN;
		mib[1] = KERN_SAVED_IDS;
		goto yesno;
	case _SC_VERSION:
		mib[0] = CTL_KERN;
		mib[1] = KERN_POSIX1;
		break;

/* 1003.1b */
	case _SC_PAGESIZE:
		mib[0] = CTL_HW;
		mib[1] = HW_PAGESIZE;
		break;
	case _SC_FSYNC:
		mib[0] = CTL_KERN;
		mib[1] = KERN_FSYNC;
		goto yesno;

/* 1003.2 */
	case _SC_BC_BASE_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_BC_BASE_MAX;
		break;
	case _SC_BC_DIM_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_BC_DIM_MAX;
		break;
	case _SC_BC_SCALE_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_BC_SCALE_MAX;
		break;
	case _SC_BC_STRING_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_BC_STRING_MAX;
		break;
	case _SC_COLL_WEIGHTS_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_COLL_WEIGHTS_MAX;
		break;
	case _SC_EXPR_NEST_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_EXPR_NEST_MAX;
		break;
	case _SC_LINE_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_LINE_MAX;
		break;
	case _SC_RE_DUP_MAX:
		mib[0] = CTL_USER;
		mib[1] = USER_RE_DUP_MAX;
		break;
	case _SC_2_VERSION:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_VERSION;
		break;
	case _SC_2_C_BIND:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_C_BIND;
		goto yesno;
	case _SC_2_C_DEV:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_C_DEV;
		goto yesno;
	case _SC_2_CHAR_TERM:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_CHAR_TERM;
		goto yesno;
	case _SC_2_FORT_DEV:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_FORT_DEV;
		goto yesno;
	case _SC_2_FORT_RUN:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_FORT_RUN;
		goto yesno;
	case _SC_2_LOCALEDEF:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_LOCALEDEF;
		goto yesno;
	case _SC_2_SW_DEV:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_SW_DEV;
		goto yesno;
	case _SC_2_UPE:
		mib[0] = CTL_USER;
		mib[1] = USER_POSIX2_UPE;
		goto yesno;

/* XPG 4.2 */
	case _SC_XOPEN_SHM:
		mib[0] = CTL_KERN;
		mib[1] = KERN_SYSVSHM;

yesno:		if (_sysctl(mib, 2, &value, &len, NULL, 0) == -1)
			return (-1);
		if (value == 0)
			return (-1);
		return (value);
		break;
	default:
		errno = EINVAL;
		return (-1);
	}
	return (_sysctl(mib, 2, &value, &len, NULL, 0) == -1 ? -1 : value); 
}

	extern long int
_fpathconf(const int handle, int name)
{
	if (handle < 0) {
		errno = EBADF;
		return(-1);
	}

  switch(name) {
    case _PC_LINK_MAX:
      return(LINK_MAX);

    case _PC_MAX_CANON:
      return _POSIX_MAX_CANON;

    case _PC_MAX_INPUT:
      return _POSIX_MAX_INPUT;

    case _PC_NAME_MAX:
      return _POSIX_NAME_MAX;

    case _PC_PATH_MAX:
      return _POSIX_PATH_MAX;

    case _PC_PIPE_BUF:
      return _POSIX_PIPE_BUF;

    case _PC_CHOWN_RESTRICTED:
      return 1;

    case _PC_NO_TRUNC:
      return 1;                 /* TODO */

    case _PC_VDISABLE:
      return 0;

    default:
      errno = EINVAL;
      return -1;
  }
}


size_t
confstr(int name, char *buf, size_t len)
{
	size_t tlen;
	int mib[2], sverrno;
	char *p;

	switch (name) {
	case _CS_PATH:
		mib[0] = CTL_USER;
		mib[1] = USER_CS_PATH;
		if (sysctl(mib, 2, NULL, &tlen, NULL, 0) == -1)
			return ((size_t) -1);
		if (len != 0 && buf != NULL) {
			if ((p = malloc(tlen)) == NULL)
				return ((size_t) -1);
			if (sysctl(mib, 2, p, &tlen, NULL, 0) == -1) {
				sverrno = errno;
				free(p);
				errno = sverrno;
				return ((size_t) -1);
			}
			/*
			 * POSIX 1003.2 requires partial return of
			 * the string -- that should be *real* useful.
			 */
			(void)strncpy(buf, p, len - 1);
			buf[len - 1] = '\0';
			free(p);
		}
		return (tlen + 1);
	default:
		errno = EINVAL;
		return (0);
	}
	/* NOTREACHED */
}

	extern int
sysctl(int *name, size_t namelen, void *oldp, size_t *oldlenp,
	 void *newp, size_t newlen)
{return(_sysctl(name,namelen,oldp,oldlenp,newp,newlen));}

	extern long int
fpathconf(const int fd, int name) { return(_fpathconf(fd,name));}

	extern long int
pathconf(const char *path, int name)
{
 const int fd = _open(path, O_RDONLY|O_BINARY);

	if (fd == (-1))
		return(-1);

	else {
	 const int
		err = _fpathconf(fd, name);
		close(fd);

		return(err);
	}
}
#if 0
/*
 * getloadavg() -- Get system load averages.
 *
 * Put `nelem' samples into `loadavg' array.
 * Return number of samples retrieved, or -1 on error.
 */
int
getloadavg(double loadavg, int nelem)
{
	struct loadavg loadinfo;
	int i, mib[2];
	size_t size;

	mib[0] = CTL_VM;
	mib[1] = VM_LOADAVG;
	size = sizeof(loadinfo);
	if (_sysctl(mib, 2, &loadinfo, &size, NULL, 0) < 0)
		return (-1);

	nelem = MIN(nelem, sizeof(loadinfo.ldavg) / sizeof(fixpt_t));
	for (i = 0; i < nelem; i++)
		loadavg[i] = (double) loadinfo.ldavg[i] / loadinfo.fscale;
	return (nelem);
}
#endif

	extern long
sysconf(int n) {return _sysconf(n);}

	extern int
_getdtablesize(void) {return _sysconf(_SC_OPEN_MAX);}

extern int getdtablesize(void) {return _getdtablesize();}
