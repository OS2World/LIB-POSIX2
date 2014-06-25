/* libExt+gcc: 	- sys/statvfs.h (POSIX 1003.1-200x compatibility header)
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/statvfs.h,v 1.1 2000/10/11 14:48:55 arnd Exp $
 */

#ifndef __SYS_STATVFS_H_
#define __SYS_STATVFS_H_

#include <sys/time.h>
#include <sys/param.h>


#define statfs statvfs
#define _statfs _statvfs
#define __statfs __statvfs
#undef _SYS_MOUNT_H_
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
