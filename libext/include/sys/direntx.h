/* $Header: /cvsroot/posix2/posix2/libext/include/sys/direntx.h,v 1.5 2000/09/11 00:35:28 arnd Exp $ */

/* libExt+gcc: 		Internal header sys/direntx.h:
 * 			Structures describing an open directory.
 *
 * For compatibility, we currently use the internal structures from emx
 * 'dirent.c': Due to change! Do not use this header in your application 
 * programs!
 */

#ifndef _SYS_DIRENTX_H
#define _SYS_DIRENTX_H

#include <dirent.h>

#define	d_ino		d_fileno	/* backward compatibility */

/* definitions for library routines operating on directories. */
#define	DIRBLKSIZ	1024

struct _dircontents {
  char 			*dd_buf;	/* data buffer */
  long			dd_size;	/* amount of data returned by getdirentries */
  unsigned short        _d_attr;
  unsigned short        _d_time;
  unsigned short        _d_date;
  struct _dircontents * _d_next;
};

struct _dirdesc {
  int                   dd_fd;	/* file descriptor associated with directory */
  long                  dd_loc;	/* offset in current buffer */
  struct _dircontents * dd_contents;
  struct _dircontents * dd_cp;
  struct dirent         dd_dirent;
};

#define	dirfd(Dp)	((Dp)->dd_fd)
#define	dd_len(Dp)	strlen((Dp)->dd_dirent.d_name) /* size of data buffer */


/* flags for opendir2 */
#define DTF_HIDEW	0x0001	/* hide whiteout entries */
#define DTF_NODUP	0x0002	/* don't return duplicate names */
#define DTF_REWIND	0x0004	/* rewind after reading union stack */
#define __DTF_READALL	0x0008	/* everything has been read */

#ifndef NULL
#ifdef 	__GNUG__
#define	NULL	__null
#else
#define	NULL	0
#endif
#endif

/*
 * Convert between stat structure types and directory types.
 */
#define	IFTODT(mode)	(((mode) & 0170000) >> 12)
#define	DTTOIF(dirtype)	((dirtype) << 12)

#if defined(_KERNEL)
/*
 * The DIRENT_SIZE macro gives the minimum record length which will hold
 * the directory entry.  This requires the amount of space in struct dirent
 * without the d_name field, plus enough space for the name with a terminating
 * null byte (dp->d_namlen+1), rounded up to a 4 byte boundary.
 */
#define	DIRENT_SIZE(dp) \
    ((sizeof (struct dirent) - (MAXNAMLEN+1)) + (((dp)->d_namlen+1 + 3) &~ 3))

#endif	/* !_KERNEL */

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* not SYS_DIRENTX_H */
