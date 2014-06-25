/* pwd.h (libExt+gcc): $Id: pwd.h,v 1.3 2000/08/15 08:05:01 amai Exp $ */

#ifndef _PWD_H
#define _PWD_H

#if defined (__cplusplus)
extern "C" {
#endif

#if !defined (_PASSWORD_LEN)
#define	_PASSWORD_LEN 128
#endif

#if !defined (_GID_T)
#define _GID_T
typedef int gid_t;
#endif

#if !defined (_UID_T)
#define _UID_T
typedef int uid_t;
#endif

struct passwd
{
  char *pw_name;
  char *pw_passwd;
  uid_t pw_uid;
  gid_t pw_gid;
  char *pw_age;
  char *pw_comment;
  char *pw_gecos;
  char *pw_dir;
  char *pw_shell;
};

char *getpass (__const__ char *);
char *_getpass1 (__const__ char *);
char *_getpass2 (__const__ char *, int);

struct passwd *getpwuid (uid_t);
struct passwd *getpwnam (__const__ char *);

struct passwd *_getpwuid (uid_t);
struct passwd *_getpwnam (__const__ char *);

#if !defined (_POSIX_SOURCE)

struct passwd *getpwent (void);
void setpwent (void);
void endpwent (void);

struct passwd *_getpwent (void);
void _setpwent (void);
void _endpwent (void);

char *user_from_uid(uid_t, int);
char *group_from_gid(gid_t, int);

#define setpassent(x) (0)
#define setgroupent(x) (0)


#endif

#if defined (__cplusplus)
}
#endif

#endif /* not _PWD_H */
