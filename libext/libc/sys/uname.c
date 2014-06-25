/*
From: "Brian Smith" <dbsoft@technologist.com>
To: "posix2@borneo.gmd.de" <posix2@borneo.gmd.de>
Subject: [PosixOS2] replacement uname

Would Posix/2 be interested in this replacement uname() 
I used in PMBitchX and a couple other of my applications?
 */

/* 	$Id: uname.c,v 1.1 2000/10/11 14:48:56 arnd Exp $ 	*/

#define OS2_API32
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#include <os2.h>

#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/errnox.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <emx/syscalls.h>
#include "../../rev.h"

	int
_uname(struct utsname *buf)
{
	if (buf == (struct utsname *)NULL) {
		errno = EFAULT;
		return(-1);
	}
 {
	 ULONG aulBuffer[4];
	 APIRET rc =
	DosQuerySysInfo(QSV_VERSION_MAJOR, QSV_MS_COUNT, 
		            aulBuffer, sizeof(aulBuffer));
	if (rc) {
		errno = _rc2Errno(rc);
		return(-1);
	}
	__builtin_strcpy(buf->sysname, "OS/2");

	if (aulBuffer[0] == 20)	{

	 int 
		i = (u_int)aulBuffer[1];
		if (i > 20) {
			sprintf(buf->release, "%d.%d", 
			   i/10, i-((i/10)*10));
		}
		else if (i == 10)
			__builtin_strcpy(buf->release, "2.1");
		else if (i == 0)
			__builtin_strcpy(buf->release, "2.0");

		__builtin_strcpy(buf->version, "libExt" _LIBEXTNS_VPRT);
		__builtin_strcpy(buf->machine, MACHINE);

		return __gethostname(buf->nodename, sizeof(buf->nodename));
	} 
	errno = ENOTSUP;
	_dPuts("libExt: _uname(): OS/2 version is unknown and not supported\n");
	return(-1);
}}

int uname(struct utsname*b) {return _uname(b);}
