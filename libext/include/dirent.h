/* libExt+gcc: user dirent.h */

/* $Header: /cvsroot/posix2/posix2/libext/include/dirent.h,v 1.4 2000/08/26 09:21:10 amai Exp $	*/

#ifndef _DIRENT_H
#define _DIRENT_H

/*
 * POSIX doesn't mandate this, but X/Open XPG 4.2 does.
 */
#ifndef _POSIX_SOURCE
#include <sys/types.h>
#endif

/*
 * The kernel defines the format of directory entries returned by 
 * the getdirentries(2) system call.
 */
#include <sys/dirent.h>

#if defined (__cplusplus)
extern "C" {
#endif

struct _dirdesc;

#ifndef __DIRENT_dir
#define __DIRENT_dir
typedef struct _dirdesc DIR;
#endif

DIR *opendir (__const__ char *);
struct dirent *readdir (DIR *);
int closedir (DIR *);
void rewinddir (DIR *);

DIR *_opendir (__const__ char *);
struct dirent *_readdir (DIR *);
void _seekdir (DIR *, long);
long _telldir (DIR *);
int _closedir (DIR *);
void _rewinddir (DIR *);
int _scandir(__const__ char*,struct dirent***,int(*)(__const__ struct dirent*), 
			 int(*)(__const__ struct dirent**,__const__ struct dirent**));
int _alphasort(__const__ struct dirent**,__const__ struct dirent**);


#if !defined (_POSIX_SOURCE)
void seekdir(DIR *, long);
long telldir(DIR *);
int scandir(__const__ char*,struct dirent***,int(*)(__const__ struct dirent*), 
			int(*)(__const__ struct dirent**,__const__ struct dirent**));
int alphasort(__const__ struct dirent**,__const__ struct dirent**);
#endif

#if defined (__cplusplus)
}
#endif


#endif /* not _DIRENT_H */
