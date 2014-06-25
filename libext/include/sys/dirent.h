/* libExt+gcc: internal header sys/dirent.h */

/* $Header: /cvsroot/posix2/posix2/libext/include/sys/dirent.h,v 1.2 2000/08/26 09:21:10 amai Exp $	*/

#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

#include <sys/types.h>

#if !defined (MAXNAMLEN)
#define MAXNAMLEN  260
#endif

#if !defined (MAXPATHLEN)
#define MAXPATHLEN 260
#endif

/*
 * Convert between stat structure types and directory types.
 */
#define	IFTODT(mode)	(((mode) & 0170000) >> 12)
#define	DTTOIF(dirtype)	((dirtype) << 12)

#define _DIRENT_D_MODE_RENAMED_D_ATTR

struct dirent
{
  ino_t          d_ino;                 /* file number of entry      */
  int            d_reclen;              /* length of this record     */
  size_t         d_namlen;              /* Length of d_name string   */
  char           d_name[MAXNAMLEN + 1]; /* File name, 0 terminated   */
  long           d_size;                /* File size (bytes)         */
  unsigned short d_attr;                /* OS file attributes        */
  unsigned short d_time;                /* OS file modification time */
  unsigned short d_date;                /* OS file modification date */
};

/*
 * File types for d_type Fix me! (AH)
 */
#if !defined (A_RONLY)
#define A_RONLY   0x01
#define A_HIDDEN  0x02
#define A_SYSTEM  0x04
#define A_LABEL   0x08
#define A_DIR     0x10
#define A_ARCHIVE 0x20
#endif

#ifdef _BSD_SOURCE	/* compatibility with BSD,not(!) POSIX */

#define	DT_UNKNOWN	0x0000
#define	DT_FIFO		0x0040
#define	DT_CHR		0x0080
#define	DT_DIR		A_DIR
#define	DT_BLK		0x0200
#define	DT_REG		0x0400
#define	DT_LNK		0x0800
#define	DT_SOCK		0x1000
#if 0
#define	DT_WHT		0x2000
#endif

#define d_type		d_attr

#endif /* not _BSD_SOURCE */

#endif /* not SYS_DIRENT_H */
