/* libExt+gcc: 		-- memory.h
* $Header: /cvsroot/posix2/posix2/libext/include/memory.h,v 1.1 2000/11/25 16:33:35 arnd Exp $
*/

#ifndef _MEMORY_H
#define _MEMORY_H

#if defined (__cplusplus)
#ifndef __inline__
# define __inline__ inline
# define __const__ const
#endif
extern "C" {
#endif

#if !defined (_SIZE_T)
#define _SIZE_T
typedef unsigned long size_t;
#endif

#if !defined (NULL)
#if defined (__cplusplus)
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

extern void*memchr(__const__ void*,int,size_t);
extern int memcmp(__const__ void*,__const__ void*,size_t);
extern void*memcpy(void*,__const__ void*,size_t);
extern void*memmove(void*,__const__ void*,size_t);
extern void*memset(void*s,int,size_t);

#if(!defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)) \
    || defined(_WITH_UNDERSCORE)

#if !defined (_MEMDIF_EQ)
#define _MEMDIF_EQ 0xffffffff
#endif

extern int  _bcmp(__const__ void*,__const__ void*,size_t);
extern void _bcopy(__const__ void*,void*,size_t);
extern void _bzero(void*,size_t);
extern size_t _memcount(__const__ void*,int,size_t);
extern size_t _memdif(__const__ void*,__const__ void*,size_t);
extern void*_memccpy(void*,__const__ void*,int,size_t);
extern int _memicmp(__const__ void*,__const__ void*,size_t);
extern void*_memrchr(__const__ void*,int,size_t);
extern void _memswap(void*,void*,size_t);

#endif

#if(!defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE))
extern int  bcmp(__const__ void*,__const__ void*,size_t);
extern void bcopy(__const__ void*,void*,size_t);
extern void bzero(void*,size_t);
extern void*memccpy(void*,__const__ void*,int,size_t);
extern int  memcasecmp(__const__ void*,__const__ void*,size_t);
extern int  memicmp(__const__ void*,__const__ void*,size_t);

#if !(!__GNUC__ || __GNUC__ < 2 ||(__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
extern __inline__ void bcopy(__const__ void*__s,void*__t, size_t __n)
								{(void)memmove(__t,__s,__n);}
extern __inline__ int bcmp(__const__ void*__s,__const__ void*__t,size_t __n)
								{return memcmp(__s,__t,__n);}
extern __inline__ void bzero(void*__s,size_t __n)		{(void)memset(__s,'\0',__n);}
extern __inline__ int memcasecmp(__const__ void*__s,__const__ void*__t,size_t __n)
	/* Compare no more than N chars, ignore case.*/ 	{return _memicmp(__s,__t,__n);}
#else
#define bcopy(__s,__t,__n)	((void)memmove((__t),(__s),(__n)))
#define bcmp(__s,__t,__n)	(memcmp((__s),(__t),(__n)))
#define bzero(__s,__n)		((void)memset((__s),'\0',(n)))
#define memcasecmp(__s,__t,__n)	(_memicmp((__s),(__t),(__n)))
#endif

#endif

#if defined (__cplusplus)
}
#endif

#endif /* not _MEMORY_H*/
