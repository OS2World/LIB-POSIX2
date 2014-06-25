/* libExt+gcc: 	-- strings.h: 
 * Deprecated header for compatibility with BSD LEGACY.
 * $Header: /cvsroot/posix2/posix2/libext/include/strings.h,v 1.4 2000/11/25 16:33:35 arnd Exp $
 */

#ifndef _STRINGS_H
#define _STRINGS_H

#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif

 /* bsd style functions (mostly deprecated by ANSI C90)
  * all new systems declare them in <string.h>, though(?!)
  */
extern int  bcmp(__const__ void*,__const__ void*,size_t);
extern void bcopy(__const__ void*,void*,size_t);
extern void bzero(void*,size_t);
extern int  ffs(int);		/* no ANSI equivalent yet, not deprecated */
extern char*index(__const__ char*,int);
extern char*rindex(__const__ char*,int);
extern size_t strlcpy(char*,__const__ char*,size_t);
extern void strmode(mode_t,char*);
extern char*strdup(__const__ char*);
extern int  stricmp(__const__ char*, __const__ char*);
extern char*strlwr(char*);
extern int  strnicmp(__const__ char*, __const__ char*, size_t);
extern int  strcasecmp(__const__ char*,__const__ char*);
extern int  strncasecmp(__const__ char*,__const__ char*,size_t);
extern char*strnset(char*, int, size_t);
extern char*strrev(char*);
extern char*strset(char*, int);
extern char*strupr(char*);
extern char*strsep(char**, __const__ char*); /* BSD */
extern long long strtoq(__const__ char*,char**,int);
extern unsigned long long strtouq(__const__ char*,char**,int);
extern long long strtoll(__const__ char*,char**,int);
extern unsigned long long strtoull(__const__ char*,char**,int);

#if !(!__GNUC__ || __GNUC__ < 2 ||(__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
extern __inline__ char*index(__const__ char*__s,int __c)	{return strchr(__s,__c);}
extern __inline__ char*rindex(__const__ char*__s,int __c)	{return strrchr(__s,__c);}
extern __inline__ int strcasecmp(__const__ char*__s,__const__ char*__t)	
		/* Compare S and T, ignoring case.*/		{return _stricmp(__s,__t);}
extern __inline__ int strncasecmp(__const__ char*__s,__const__ char*__t,size_t __n)
								{return _strnicmp(__s,__t,__n);}
extern __inline__ char*strsep(char**__s,__const__ char*__d)	{return _strsep(__s,__d);}
extern __inline__ long long strtoll(__const__ char*__s,char**__e,int __r)
								{return _strtoll(__s,__e,__r);}
extern __inline__ unsigned long long strtoull(__const__ char*__s,char**__e,int __r)
								{return _strtoull(__s,__e,__r);}
#else
#define index(__s,__n)			(strchr((__s),(__n)))
#define rindex(__s,__n)			(strrchr((__s),(__n)))
#define strcasecmp(__s,__t)		(_stricmp((__s),(__t)))
#define strncasecmp(__s,__t,__n)	(_strnicmp((__s),(__t),(__n)))
#endif

#if defined(__cplusplus)
}
#endif

#endif /* not _STRINGS_H */
