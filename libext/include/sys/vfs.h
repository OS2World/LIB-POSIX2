/* libExt+gcc: 	- sys/vfs.h
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/vfs.h,v 1.2 2000/10/11 14:48:55 arnd Exp $
 */

#ifndef __SYS_VFS_H_
#define __SYS_VFS_H_

#include <sys/time.h>
#include <sys/param.h>
#include <sys/mount.h>

#if defined (__cplusplus)
extern "C" {
#endif

extern int statfs(__const__ char*,struct __statfs*);
extern int _statfs(__const__ char*,struct __statfs*);
extern int _getDriveNumber(__const__ char*);

/* Note that this function returns -1 on error (overflow): */
extern int _localTime2GMT(time_t*,int);

#if defined (__cplusplus)
}
#endif

#endif /* __SYS_VFS_H_ */
