/* Internal definitions and declarations for libExt mman.
 * $Id: mmanx.h,v 1.2 2000/10/11 14:48:55 arnd Exp $
 * Do not include this implementation header in application programs!
 *
 * Copyright (c)1997 by Holger.Veit@gmd.de
 * may be freely used with EMX
 * Many changes towards a more Posix-like interface: 
 * (C) 2000 Arnd.H.Hanses@rz.ruhr-uni-bochum.de
 * For copying, see COPYING.bsd
 */

#ifndef __P2_MMAN_H__
#define __P2_MMAN_H__

#include <sys/mman.h>

extern char _etext; /* end of pure storage: Warn when __MM_malloc() gets near */

	/* check correct page alignment; unaligned address is unallowed */
#define __PAGEMASK    ((size_t)(PAGESIZE - 1)) 
#define __NO_ALIGN(a) ( (size_t)(a) & __PAGEMASK )
#define __ALIGN(a)    ((caddr_t)((size_t)(a)&~__PAGEMASK))

	/* all admissible protection bits */
#define __PROTMASK    (PROT_NONE|PROT_EXEC|PROT_WRITE|PROT_READ|PROT_GUARD)
#define __IS_ANON(f)  (((f) & MAP_ANON) != 0) 

#ifdef _KERNEL

/* A map table entry node */
struct __MM_mapTbl {
  caddr_t	caddr;	/* virtual (flat) memory starting address */
  size_t	len;	/* actual bytes read into memory region   */
/* TODO: Memory access beyond 'caddr+len' shall generate SIGBUS! FixMe! (AH) */
  short	flags;	/* type of map: MAP_SHARED, etc.          */
  off_t	pos;	/* save inital STREAM position before map */
  off_t	off;	/* STREAM offset mapping underlying file  */
  int 		fd;	/* underlying file (duplicate) descriptor */
};
typedef struct __MM_mapTbl __MM_TB;

typedef __const__ char* c_caddr_t;
  
#define __MM_bzero(a,n) {caddr_t __p=(a);int __I=(n); for(;__I>0;++__p,--__I) *__p='\0';}
#define __MM_bminus1(a,n) {caddr_t __p=(a);int __I=(n); for(;__I>0;++__p,--__I) *__p=(-1);}

extern char *__MM_l64a(size_t value); /* internal number to ascii utility */

/* definitions and declarations for kernel __MM_malloc() */

/* If range checking is not turned on, all we have is a flag indicating
   whether memory is allocated, an index in nextf[], and a size field; to
   realloc() memory we copy either size bytes or 1<<(index+3) bytes depending
   on whether the former can hold the exact size (given the value of
   'index').  If range checking (__rcheck) is on, we always need to know how 
   much space is allocated, so the 'size' field is never used.
	 */
struct __mhead {
	char     mh_alloc;	/* __ISALLOC or __ISFREE */
	char     mh_index;	/* index in nextf[] */
/* Remainder are valid only when block is allocated */
	unsigned short mh_size;	/* size, if < 0x10000 */
#ifdef __rcheck
	unsigned mh_nbytes;	/* number of bytes allocated */
	int      mh_magic4;	/* should be == __MAGIC4 */
#endif /* __rcheck */
};

#define __ISALLOC ((char) 0xf7)	/* magic byte that implies allocation */
#define __ISFREE  ((char) 0x54)	/* magic byte that implies free block */
				/* this is for error checking only    */
#define __CHAIN(a) (*(struct __mhead**)(sizeof(caddr_t) + (caddr_t)(a)))

#ifdef __MSTATS
/* Return pointer to statically allocated buffer that contains 
 * statistics describing allocation of blocks of size 2**n.
 */
struct __mstats_value {
	int blocksize;
	int nfree;
	int nused;
};
extern struct __mstats_value *__MM_malloc_stats(size_t size);
#endif

extern size_t __MM_freespace(void);
extern size_t __MM_mc_size(caddr_t cp);
/* Cause reinitialization based on job parameters;
 * also declare where the end of pure storage is. */
extern void __MM_malloc_init(const caddr_t end);

/* get a block */
extern caddr_t __MM_malloc(size_t n);
extern void __MM_free(void*mem);
extern caddr_t __MM_realloc(void*mem, size_t n);

	static __inline__  void
__MM_strcat(char *s, __const__ char *t) {
  while(*s) ++s; while((*s=*t)) ++s,++t;
}

#endif /* ! _KERNEL */

extern int madvise(void*,size_t,int); 	/* stub fn */
extern int _madvise(void*,size_t,int); 	/* stub fn */

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* !__P2_MMAN_H__ */
