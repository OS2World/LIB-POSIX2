/* sys/errnox.h - Global error translation table -- emx-OS/2-gcc
    This code is released into Public Domain. */
/* $Id: errnox.h,v 1.8 2000/11/17 19:14:59 arnd Exp $ */

#ifndef _SYS_ERRNOX_H_
#define _SYS_ERRNOX_H_

#ifndef _OS2_H
# warning First appropiately include <os2.h> !
# define INCL_BASE
# define OS2_API32
# include <os2.h>
#endif

#include <sys/errno.h>

#ifdef	__cplusplus
extern "C" {
#endif

extern unsigned __const__ char __errnox_sysErr2ErrnoTab[348];
extern unsigned __const__ char _rc2Errno(APIRET);

#ifdef	__cplusplus
}
#endif

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* _SYS_ERRNOX_H_*/