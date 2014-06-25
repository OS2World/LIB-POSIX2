/* libExt+gcc: $Header: /cvsroot/posix2/posix2/libext/include/grp.h,v 1.1 2000/08/26 09:46:35 amai Exp $ */

#ifndef _GRP_H
#define _GRP_H

#if defined (__cplusplus)
# define __const__ const
extern "C" {
#endif

#if !defined (_GID_T)
#define _GID_T
typedef int gid_t;
#endif

struct group
{
  char *gr_name;
  gid_t gr_gid;
  char **gr_mem;
};

struct group *getgrgid (gid_t);
struct group *getgrnam (__const__ char *);

extern struct group *getgrent(void); /* return group entries */
extern void setgrent(void); /* "rewind pointer to database entries" */
extern void endgrent(void);  /* "close" database */
extern int initgroups(__const__ char *, gid_t);

#if defined (__cplusplus)
}
#endif

#endif /* not _GRP_H */
