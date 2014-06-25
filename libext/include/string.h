/* libext+gcc: 		-- string.h --
 * $Header: /cvsroot/posix2/posix2/libext/include/string.h,v 1.3 2000/11/25 16:33:35 arnd Exp $
 */

#ifndef _STRING_H
#define _STRING_H

#include <memory.h>

#if defined (__cplusplus)
extern "C" {
#endif

char *strcat (char *, __const__ char *);
char *strchr (__const__ char *, int);
int strcmp (__const__ char *, __const__ char *);
int strcoll (__const__ char *, __const__ char *);
char *strcpy (char *, __const__ char *);
size_t strcspn (__const__ char *, __const__ char *);
char *strerror (int);
size_t strlen (__const__ char *);
char *strncat (char *, __const__ char *, size_t);
int strncmp (__const__ char *, __const__ char *, size_t);
char *strncpy (char *, __const__ char *, size_t);
char *strpbrk (__const__ char *, __const__ char *);
char *strrchr (__const__ char *, int);
size_t strspn (__const__ char *, __const__ char *);
char *strstr (__const__ char *, __const__ char *);
char *strtok (char *, __const__ char *);
size_t strxfrm (char *, __const__ char *, size_t);

#if(!defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)) \
    || defined(_WITH_UNDERSCORE)

#if !defined(_MODE_T)
#define _MODE_T
typedef int mode_t;
#endif

extern char*_strncpy(char*, __const__ char*, size_t);
extern char*_strdup(__const__ char*);
extern int _stricmp(__const__ char*, __const__ char*);
extern char*_strlwr(char*);
extern int _strnicmp(__const__ char*, __const__ char*, size_t);
extern char*_strnset(char*, int, size_t);
extern char*_strrev(char*);
extern char*_strset(char*, int);
extern char*_strupr(char*);
extern char*_strsep(char**, __const__ char*);
extern long long _strtoll(__const__ char*,char**,int);
extern unsigned long long _strtoull(__const__ char*,char**,int);

extern int _ffs(int);		/* no ANSI equivalent yet, not deprecated*/
extern char*_index(__const__ char*,int);
extern char*_rindex(__const__ char*,int);

/* not a standard interface; due to change !*/
extern int _strHasTrailingSlash(__const__ char*,__const__ size_t,__const__ int);
/* Remove trailing slashes from non-const variable PATH.*/
extern void _strRemoveTrailingSlashes(char*);

#endif /* !_POSIX_SOURCE, etc. */

#if defined (__cplusplus)
}
#endif

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)
#include <strings.h>
#endif

#endif /* not _STRING_H */
