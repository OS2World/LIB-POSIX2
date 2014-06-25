/* Copyright (c)1997 by Holger.Veit@gmd.de
 * may be freely used with EMX
 * Many changes towards a more Posix-like interface: 
 * (C) 2000 Arnd.H.Hanses@rz.ruhr-uni-bochum.de
 * For copying, see COPYING.bsd
 * $Id: mm.c,v 1.2 2000/10/11 14:48:56 arnd Exp $
 */

#define INCL_BASE
#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR
#include <os2.h>

#define _POSIX_SYNCHRONIZED_IO 
#define _KERNEL
#define _BSD_SOURCE
#include <unistd.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/builtin.h>
#include <sys/smutex.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emx/syscalls.h>
#include <sys/errnox.h>	/* convert DOS*() errors to errno 	*/
#ifdef DEBUG
#define __rcheck	1 /* enable kernel malloc range checking 	*/
#define __M_WARN	1 /* enable kernel malloc warnings 	*/
#define __MSTATS	1 /* enable kernel malloc statistics 	*/
#endif
#include <sys/mmanx.h>

/* Allocate private or shared memory objects. Return file descriptor associated
 * with the allocated region or -1 on error.
 */
static int __mmapcAlloc(int fd, caddr_t *paddr, size_t len, int flags);

/* Fixed map not (really) supported, return error.
 */
static int __mmapFixed(caddr_t *pcaddr, size_t len, off_t off,
	int fd, int flags);

/* Find any (overlapping) map table entry for the memory region starting at 
 * 'cadr' and extending 'len' bytes. Returns: Map table index, -1 on error.
 */
static int __mmapIndex(c_caddr_t cadr, size_t len);
	
/* Delete map entry and free allocated memory. Return -1 on error. */
static int __mmapDelete(caddr_t caddr, int i);


/* Mapping list table, access is protected by a mutex. Nevertheless,
   signal handler functions in thread 1 would break this protection, if they
   tried to access the list. 

   Beware!
   You must not call mmap(), etc. in signal handlers !!
	 ***/
static _smutex __MM_sem; /* protect access to the table, link with INITGLOBAL */

static __MM_TB __MM_mmapTbl[NOFILE_MAX]; /* Make it system global,
		i.e. link it into a DLL with INITGLOBAL */

static const short __MM_NODE_MAX=(sizeof(__MM_mmapTbl)/sizeof(__MM_mmapTbl[0]));


/*
 * 	*** 	The Memory Management Interfaces (4.4BSD-style) 	***
 */

/* Find any (overlapping) map table entry for the memory region starting at 
 * 'cadr' and extending 'len' bytes. Returns: Map table index, -1 on error.
 */
	static __inline__ int
__mmapIndex(c_caddr_t cadr, size_t len)
{
 int i = 0;
  
	while (i < __MM_NODE_MAX) {

		if ( (cadr + len) >= __MM_mmapTbl[i].caddr &&
		    cadr <= (__MM_mmapTbl[i].caddr + __MM_mmapTbl[i].len))
			return(i); 		/* map found */

		++i; 				/* check next element */
	}
	/* fallthrough */
	_dPrintf("libExt: __mmmapIndex(): Wrong address <%p>: Fallthrough\n", cadr);
	return(-1);
}

	static int
__mmapDelete(caddr_t caddr, int i)
{
 int j = 0;
 APIRET rc;

	/* sync changes in case of shared pages ... */
	if (__MM_mmapTbl[i].flags & MAP_SHARED) {
	 
		_msync(caddr, 0, MS_SYNC);
  /* ... delete duplicate fd ... */
		if (_close(__MM_mmapTbl[i].fd))
			return(-1);
	}
	/* ... and then try to invalidate the map object */
	rc = DosFreeMem(caddr);
	if (rc) { _dPrintf("\
libExt: mm.c: _munmap(): Error! DosFreeMem(p->caddr) returns <%ld>\n", rc);
		errno = _rc2Errno(rc); return(-1);
	}

	/* gain privileges ... */
	do {/* at first waste some u-secs */ ++j;} while (j < 50);
	_smutex_request(&__MM_sem);
	/* fill map table node with zero's */
	__MM_bzero((caddr_t)&__MM_mmapTbl[i], sizeof(__MM_mmapTbl[i]));
	_smutex_release(&__MM_sem);

	return(0);
}

/* Allocate and possibly overwrite overlapping (shared) memory objects.
 * Returns associated file descriptor or -1 on error.
 */
	static int
__mmapcAlloc(int fd, caddr_t *paddr, size_t len, int flags)
{
 APIRET rc;

	/* Allocate (preallocate and commit) memory region for file mapping.
	 * Initially allow write to fill it with data. Note that on the i386
	 * architecture, for some strange reasons, write access always im-
	 * plies read access and that read access automatically implies
	 * execute access.
	 * ***/

	if (*paddr != NULL) {  /* in this case POSIX 200x requires overwrite */
	 int  i= __mmapIndex(*paddr, len);

		if (i >= 0) /* try to delete any previous maps at this address */
			(void)__mmapDelete(__MM_mmapTbl[i].caddr, i);
	}
	if (flags & MAP_PRIVATE) { 
		rc = DosAllocMem((PPVOID)paddr, len, PROT_WRITE|PAG_COMMIT);
		if (rc) {
			_dPrintf("libExt: mmap(): Dos*Mem() returns # %lu", rc);
			errno = _rc2Errno(rc); return(-1);
		}
		return(fd); /* don't duplicate; we must not use it */
	} else {
	 
	/* the unique file descriptor identifies the object */
	if (fd < 0) /* anonymous memory allocation requested */
		fd =_open("NUL", O_NONBLOCK|O_RDWR|O_BINARY);
	else /* add extra reference to the mapped file */
		fd = _dup(fd);

	/* We need to call DosGetSharedMem() in client process somehow.  FixMe!*/
	rc = DosAllocSharedMem((PPVOID)paddr, NULL, len, 
			OBJ_GETTABLE|PROT_WRITE|PAG_COMMIT);
	if (rc) {
		_dPrintf("libExt: __mmap_Shared(): DosAllocSharedMem() returns #%lu\n", rc);
		errno = _rc2Errno(rc); return (-1);
	}
	return(fd);
}}

/* The external functions; cf. respective manual pages and POSIX for details.
 */
	void *
_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) 
{
 caddr_t adr = (caddr_t)addr; 	/* preferably NULL */
 APIRET rc;

	if (flags < 0 || flags > MAP_TYPE || (!(flags & MAP_SHARED) && 
	 !(flags & MAP_PRIVATE)) || prot < 0 || prot > __PROTMASK ||	len > MAP_MAX)
		{errno = EINVAL; return(MAP_FAILED);}

	/* handle unportable MAP_FIXED first */
	if  (flags & MAP_FIXED) {
		if (NULL == adr || adr + len > __MAXADDRESS || __NO_ALIGN(adr)) {
			_dPrintf("Error: Region too large! End address: %p \n", adr+len);
			errno = ENOMEM;	return(MAP_FAILED); 		/* address too large */
		}
		if (__mmapFixed(&adr, len, off, fd, flags))
			return(MAP_FAILED);
	}

	if (! __IS_ANON(flags)) { /* Not mapping unassociated anonymous memory */
	 ULONG htype;

		rc = DosQueryFHState((ULONG)fd, &htype);
		if (rc) { 	/* is fd okay? */
			_dPrintf(
"libExt: _mmap(): Bad file descriptor; DosQueryFHState() returns: #%ld\n", rc);
			errno = _rc2Errno(rc); return(MAP_FAILED);
		}
		if (htype & OPEN_FLAGS_DASD) { /* no support for mapping such device */
			errno = ENODEV; return(MAP_FAILED);
		}
		/* red/write access denied */  
		if (htype & OPEN_SHARE_DENYREAD  || (prot & PROT_WRITE && 
		    htype & OPEN_SHARE_DENYWRITE &&  flags & MAP_SHARED)) { 
			errno = EACCES; return(MAP_FAILED);
		}
	}
	_dPrintf("libExt: mm.c: _mmap(): Requested protection=<%2d> \n", prot);
 {
 int i=0;
 ssize_t newlen = len; 			/* actual bytes read into map object */
 off_t	 initialPos = -1; 	/* positon of file pointer of mapped file */

	if  ( !(flags & MAP_FIXED) )
		fd = __mmapcAlloc(fd, &adr, len, flags);

	do {/* waste some u-secs */ ;;} while (++i < 10);

	_smutex_request(&__MM_sem); 	/* Protect access; then list map */

	if (! __IS_ANON(flags)) { 	/* not mapping anonymous memory */
	 off_t	newPos;

	/* Copy the entire mapped file into the memory area, starting at offset off.
	   The memory region is initially filled with zeros. 
	   OS/2 will swap out, if necessary. Large sparse map pools of demand
	   pages are faster for most applications, because today's modern 
	   workstations contain huge physical RAM. On the other hand, 
	   frequent drive paging of small and densely used mapped areas 
	   from busy drives gives lousy performance, although preferable 
	   with smaller older machines or with enormous file sizes.
			***/
		/* Safe intial position of the mapped file's position indicator.
		   Beware! Race conditions, not yet thread safe!! FixMe! (AH)
		   ***/
		if ((initialPos = _tell(fd)) == -1) 
			goto errexit; 			/* ?? */
		if ((newPos = _lseek(fd, off, SEEK_CUR)) == -1)
			goto errexit; 			/* Bad (negative) offset ? */
		/* Now we read the byte stream into the memory map object. */ 
		/* ToDo: Generate SIGBUS upon access beyond 'newlen'!! (AH) */
		newlen = _read(fd, adr, len);
		/* Move file pointer to initial position */
		newPos = _lseek(fd, initialPos, SEEK_SET);
		if (-1 == newlen) {
			_dPrintf("libExt: _mmap(): _read() error on file # <%d>\n", fd);
			goto errexit;
		}
		if (-1 == newPos)
			goto errexit;
	}
	/*  Set the protection to the type that was initially requested
	 * by the caller. In this way we can now protect against 
	 * read access and/or write access. We are also able to forbid
	 * any access. Cf. note above about implied protection bits. 
	 * ***/
	rc = DosSetMem(adr, len, (ULONG)prot);
	if (rc) {
		_dPrintf("libExt: _mmap(): DosSetMem() returns <%ld>\n", rc);
		errno = _rc2Errno(rc); goto errexit;
	}
	/*  Store the data object descriptions into our system-global array.
	    Every mapped object must have a map description table node that is
	    unique throughout the system, a pseudo 'kernel' table entry.
			***/
	for (i = 0; __MM_mmapTbl[i].caddr; ++i) { /* hash table won't be faster */
		if (i == __MM_NODE_MAX) {
			errno = ENOMEM;
			goto errexit;
		}
	}
	__MM_mmapTbl[i].caddr = adr;
	__MM_mmapTbl[i].len   = newlen;
	__MM_mmapTbl[i].flags = flags;
	__MM_mmapTbl[i].off   = off;
	__MM_mmapTbl[i].pos   = initialPos;
	__MM_mmapTbl[i].fd    = fd;

	_smutex_release(&__MM_sem);

	return(adr);				/* Success. Poohff.. */

errexit:
	_smutex_release(&__MM_sem);
 }
 return(MAP_FAILED);
}

	extern int
_munmap(void *addr, size_t len)
{
 caddr_t caddr = (caddr_t)addr;
 const int i = __mmapIndex(caddr, len);

	if (NULL == caddr || caddr > __MAXADDRESS || i < 0)
		{errno = EFAULT; return(-1);}
	if (__NO_ALIGN(caddr) || len > MAP_MAX)
		{errno = EINVAL; return(-1);}

	return __mmapDelete(__MM_mmapTbl[i].caddr, i);
}

	extern int
_msync(const void *adr, size_t len, int flag)
{
 c_caddr_t  caddr = (c_caddr_t)adr;
 int i;

	if (__NO_ALIGN(caddr) || (long)len < 0 || flag < 0 ||
		 flag >= (MS_SYNC|MS_ASYNC|MS_INVALIDATE) || flag == (MS_SYNC|MS_ASYNC))
  		{errno = EINVAL; return(-1);}

	if (NULL == caddr || caddr > __MAXADDRESS
			|| (i  = __mmapIndex(caddr, len)) < 0)
		{errno = ENOMEM; return(-1);}

	/* msync() must have no effect on MAP_PRIVATE mappings */
	if (__MM_mmapTbl[i].flags & MAP_PRIVATE)
		return(0);
	else {
	 const int fd = __MM_mmapTbl[i].fd;
	 off_t  base = __MM_mmapTbl[i].pos + __MM_mmapTbl[i].off;
	 size_t sz   = len ? MIN(len, __MM_mmapTbl[i].len) : __MM_mmapTbl[i].len;
	 off_t initialPos = _tell(fd);

		if (initialPos == -1)
			return(-1);

		if (len) 
			base += (caddr - __MM_mmapTbl[i].caddr);
		/* FixMe: race conditions for later write, etc. */
		if (flag & MS_ASYNC) {
			if (_fcntl(fd, F_SETFL, O_NONBLOCK))
				return(-1);
		} else
		if (_fcntl(fd, F_SETFL, O_SYNC))
			return(-1);

		base = _lseek(fd, base, SEEK_SET);
		if (base != -1)  	/* Bad offset ? */
			sz = _write(fd, caddr, sz); 	/* write changes back to file */

		initialPos = _lseek(fd, initialPos, SEEK_SET); /* move back pointer */
		if (base == -1 || sz == (size_t)-1 || initialPos == -1)
			return(-1);

		if (flag & MS_INVALIDATE) { /* invalidate other maps of the file */
		/* FixMe!! Just a kludge (AH) */
			if (flag & MS_SYNC) 
				return _sync(); /* run DosShutdown() */
			/* TODO: implement it! (AH) */;
			errno = EBUSY; return(-1);
		}
		return(0);
	}
}

	/***
	mprotect() controls how a section of memory may be accessed.
	If an access is disallowed by the protection given it, the
	program receives a SIGSEGV.
			***/
	extern int
_mprotect(const void *adr, size_t len, int prot)
{
 c_caddr_t caddr = adr;

	if (caddr == NULL || caddr > __MAXADDRESS)
		{errno = EFAULT; return(-1);}

	if (__NO_ALIGN(caddr) || (long)len < 0 || prot < 0 || prot > __PROTMASK)
		{errno = EINVAL; return(-1);}
	{
	 APIRET rc =
		DosSetMem(caddr, len, (ULONG)prot);
		if (rc) {
			errno = _rc2Errno(rc);
			_dPrintf("libExt: _mprotect(): DosSetMem returns <%ld>", rc);
			return(-1);
		}
	}
	return(0);
}

	extern int
_madvise(void *addr, size_t len, int beh)
{
 caddr_t caddr = (caddr_t)addr;

	/* This function is not implemented yet */
	if (NULL == caddr || caddr > __MAXADDRESS)
		{errno = EFAULT; return(-1);}
	if ((long)len < 0 || beh > 5 || beh < 0)
		{ errno = EINVAL; return(-1);}
	return 0;
}

	extern int
msync(const void*a,size_t l,int d){return _msync(a,l,d);}
	extern int
mprotect(const void*a, size_t l, int p){return _mprotect(a,l,p);}
	extern int
madvise(void *addr, size_t len, int beh) {return _madvise(addr,len,beh);}
	extern void *
mmap(void*a,const size_t l,const int p,const int f,int fd,off_t o) 
{return _mmap(a,l,p,f,fd,o);}
	extern int
munmap(void *A,size_t l) {return _munmap(A,l);}

	extern char *
__MM_l64a(size_t value)
{
 static char buf[10]; int i = 0;
	while (0 != value && i < 9) {
	 size_t v = value & 0x3f;
		if ( (v) < 2 )
			buf[i] = v + '#';
		else { if (v < 12)
			buf[i] = v + '0' - 2;
		else { if (v < 38)
			buf[i] = v + 'A' - 12;
		else
			buf[i] = v + 0xd0 - 38;
		}}
		value >>= 6; ++i;
	}
	buf[i] = '\0';
	return(buf);
}

/* Check for errors. We cannot really allocate fixed locations.
   But the region check for __MM_freepages should be the only really
   useful utilization of MAP_FIXED. As MAP_FIXED is deprecated, the code 
   has been banned to an extra function at the end of this module.
			***/
	static int
__mmapFixed(caddr_t *pcaddr, size_t len, off_t off, int fd, int flags)
{
	
	/*  Is 'address AND off' multiple of page size?  ***/
	if (__NO_ALIGN(*pcaddr) || __NO_ALIGN(off)) {
		_dPrintf(
"libExt: _mmap_fixed(): Error: Not page aligned! \nAddress: %p ", *pcaddr);
		_dPrintf("Offset: %lu\n", off);
		errno = EINVAL;
		return(-1);
	} {
	 caddr_t caddr = *pcaddr;
	 int i = __mmapIndex(caddr, len);
	/* replace any previous mapping in region */
	if (i >= 0)
		(void)__mmapDelete(__MM_mmapTbl[i].caddr, i);
	fd = __mmapcAlloc(fd, pcaddr, len, flags);
	if (caddr == *pcaddr) /* just by chance :-) */
		return(0);

	errno = ENOMEM; 	  /* OS/2 always determines the exact location of new 	*/
	return(-1);	/* memory objects, cannot allocate a fixed address. 	*/
}}


	/* Kernel malloc utility functions */
static void __MM_getpool(void);
static inline void __MM_morecore(int nu); /* ask system for more memory */

/*
 *	from: @(#)nmalloc.c 1 (Caltech) 2/21/82
 *
 *	U of M Modified: 20 Jun 1983 ACT: strange hacks for Emacs
 *
 *	Nov 1983, Mike@BRL, Added support for 4.1C/4.2 BSD.
 *
 * This is a very fast storage allocator.  It allocates blocks of a small 
 * number of different sizes, and keeps freelists of each size.  Blocks
 * that don't exactly fit are passed up to the next larger size.  In this 
 * implementation, the available sizes are (2^n)-4 (or -16) bytes long.
 * This is designed for use in a program that uses vast quantities of
 * memory, but bombs when it runs out.  To make it a little better, it
 * warns the user when he starts to get near the end.
 *
 * June 84, ACT: modified __rcheck code to check the range given to malloc,
 * rather than the range determined by the 2-power used.
 *
 * Jan 85, RMS: calls malloc_warning to issue warning on nearly full.
 * No longer Emacs-specific; can serve as all-purpose malloc for GNU.
 * You should call malloc_init to reinitialize after loading dumped Emacs.
 * Call malloc_stats to get info on memory stats if __MSTATS turned on.
 * realloc knows how to return same block given, just changing its size,
 * if the power of 2 is correct.
 */

/*
 * nextf[i] is the pointer to the next __MM_freeblock of size 2^(i+3).  The
 * smallest allocatable block is 8 bytes.  The overhead information will
 * go in the first int of the block, and the returned pointer will point
 * to the second.
 *
 */

/* end of the program; can be changed by calling init_malloc */
extern char _etext;
static caddr_t endofpure = &_etext;

#ifdef __rcheck
/* To implement range checking, we write magic values in at the beginning and
   end of each allocated block, and make sure they are undisturbed whenever a
   __MM_free or a __MM_realloc occurs. */
/* Written in each of the 4 bytes following the block's real space */
# define __MAGIC1 0x55
/* Written in the 4 bytes before the block's real space */
# define __MAGIC4 0x55555555
static void __botch(const char*s){printf("assertion botched: %s\n",s);abort();}
# define __ASSERT(p) if (!(p)) __botch("(p)"); 
# define __EXTRA  4		/* 4 bytes extra for __MAGIC1s */
#else
# define __ASSERT(p)
# define __EXTRA  0
#endif /* __rcheck */

/* Access free-list pointer of a block.
  It is stored at block + 4.
  This is not a field in the mhead structure
  because we want sizeof (struct mhead)
  to describe the overhead for when the block is in use,
  and we do not want the free-list pointer to count in that.  */

/* nextf[i] is __MM_freelist of blocks of size 2**(i + 3)  */
static struct __mhead *nextf[30];


#ifdef __MSTATS
static int nmalloc[30];
static int nmal, nfre;
#endif /* __MSTATS */

#ifdef	__M_WARN
/* Number of bytes of writable memory we can expect to be able to get */
static size_t lim_data;
/* Level number of warnings already issued.
  0 -- no warnings issued.
  1 -- 75% warning already issued.
  2 -- 85% warning already issued.
*/
static int  warnlevel;
#endif /* __M_WARN */

/* nonzero once initial bunch of __MM_freeblocks made */
static int gotpool;

/* Cause reinitialization based on job parameters;
  also declare where the end of pure storage is. */
	extern void
__MM_malloc_init(const caddr_t end)
{
	endofpure = end;
#ifdef	__M_WARN
	lim_data = 0;
	warnlevel = 0;
#endif /* __M_WARN */
}

	static void
__MM_getpool(void)
{
	int nu;
	caddr_t cp = __sbrk(0);

	if ((int)cp & 0x3ff)	/* land on 1K boundaries */
	    __sbrk(1024 - ((int)cp & 0x3ff));

	/* Get 2k of storage */
	cp = __sbrk(04000);
	if (cp == (caddr_t ) -1)
	    return;

	/* Divide it into an initial 8-word block
	 * plus one block of size 2**nu for nu = 3 ... 10.
	 */
	__CHAIN(cp) = nextf[0];
	nextf[0] = (struct __mhead*)cp;
	((struct __mhead*)cp)->mh_alloc = __ISFREE;
	((struct __mhead*)cp)->mh_index = 0;
	cp += 8;

	for (nu = 0; nu < 7; nu++) {
		__CHAIN(cp) = nextf[nu];
		nextf[nu] = (struct __mhead*)cp;
		((struct __mhead*)cp)->mh_alloc = __ISFREE;
		((struct __mhead*)cp)->mh_index = nu;
		cp += 8 << nu;
	}
}

/* ask system for more memory */
	static inline void
__MM_morecore(int nu) 
{		 	/* size index to get more of  */
	caddr_t  cp;
	size_t   nblks;
	size_t   siz;

#ifdef	__M_WARN
	if (lim_data == 0) {		/* find out how much we can get */
		struct rlimit   XXrlimit;

		getrlimit (RLIMIT_DATA, &XXrlimit);
		lim_data = XXrlimit.rlim_cur;
	}	/* soft limit */
#endif /* __M_WARN */

	/* On initial startup, get two blocks of each size up to 1k bytes */
	if (!gotpool)
	   __MM_getpool(), __MM_getpool(), gotpool = 1;

	/* Find current end of memory and issue warning if getting near max */
	cp = __sbrk(0);
	siz = cp - endofpure;
#ifdef	__M_WARN
	switch (warnlevel) {
	    case 0: 
		if (siz > (lim_data / 4UL) * 3UL) {
			warnlevel++;
			_dPuts("Warning: past 75% of memory limit");}
		break;
	    case 1: 
		if (siz > (lim_data / 20UL) * 17UL) {
			warnlevel++;
			_dPuts("Warning: past 85% of memory limit");}
		break;
	    case 2: 
		if (siz > (lim_data / 20UL) * 19UL) {
			warnlevel++;
			_dPuts("Warning: past 95% of memory limit");}
		break;
	}
#endif /* __M_WARN */

	if ((size_t)cp & 0x3ffUL)	/* land on 1K boundaries */
	    __sbrk(1024 - ((long)cp & 0x3ff));

	/* Take at least 2k, and figure out how many blocks 
	 * of the desired size we're about to get
	 */
	nblks = 1;
	if ((siz = nu) < 8)
	    nblks = 1 << ((siz = 8) - nu);

	if ((cp = __sbrk(1 << (siz + 3))) == (caddr_t)-1)
	    return;			/* no more room! */
	if ((size_t)cp & 7) {		/* shouldn't happen, but just in case */
		cp = (caddr_t )(((size_t)cp + 8UL) & ~7UL);
		--nblks;
	}
	/* save new header and link the nblks blocks together */
	nextf[nu] = (struct __mhead*)cp;
	siz = 1 << (nu + 3);
	while (1) {
		((struct __mhead*)cp)->mh_alloc = __ISFREE;
		((struct __mhead*)cp)->mh_index = nu;
		if (--nblks <= 0) break;
		__CHAIN((struct __mhead*)cp) = (struct __mhead*)(cp + siz);
		cp += siz;
	}
	__CHAIN((struct __mhead*)cp) = 0; /* since __sbrk() returns cleared core, 
					   * this is already set */
}

	extern caddr_t 
__MM_malloc(size_t n)		/* get a block */
{
	struct __mhead 	*p;
	size_t 	nbytes;
	int    	nunits = 0;

	/* Figure out how many bytes are required, rounding up to the nearest
	multiple of 4, then figure out which nextf[] area to use */
	nbytes = ((long)(n + sizeof *p + __EXTRA + 3) & ~3);

	{
	 size_t shiftr = (nbytes - 1) >> 2;

		while (shiftr >>= 1)
		    nunits++;
	}
	/* If there are no blocks of the appropriate size, go get some */
	/* COULD SPLIT UP A LARGER BLOCK HERE ... ACT */
	if (nextf[nunits] == 0)
	    __MM_morecore(nunits);

	/* Get one block off the list, and set the new list head */
	if ((p = nextf[nunits]) == 0)
	    return 0;
	nextf[nunits] = __CHAIN(p);

	/* Check for __MM_freeblock clobbered */
	/* If not for this check, we would gobble a clobbered __MM_freechain ptr */
	/* and bomb out on the NEXT allocate of this size block */
	if (p->mh_alloc != __ISFREE || p->mh_index != nunits)
#ifdef __rcheck
	    __botch("block on __MM_freelist clobbered");
#else
	    abort();
#endif /* __rcheck */

	/* Fill in the info, and if range checking, set up the magic numbers */
	p->mh_alloc = __ISALLOC;
#ifdef __rcheck
	p->mh_nbytes = n;
	p->mh_magic4 = __MAGIC4;
	{
	 caddr_t  m = (caddr_t )(p + 1) + n;

		*m++ = __MAGIC1, *m++ = __MAGIC1, *m++ = __MAGIC1, *m = __MAGIC1;
	}
#else
	p->mh_size = n;
#endif /* __rcheck */
#ifdef __MSTATS
	nmalloc[nunits]++;
	nmal++;
#endif /* __MSTATS */
	return (caddr_t )(p + 1);
}

	extern void
__MM_free(void *mem)
{
	struct __mhead *p;
	{
	 caddr_t ap = mem;

		__ASSERT(ap != 0);
		p = (struct __mhead*)ap - 1;
		__ASSERT(p->mh_alloc == __ISALLOC);
#ifdef __rcheck
		__ASSERT(p->mh_magic4 == __MAGIC4);
		ap += p->mh_nbytes;
		__ASSERT(*ap++ == __MAGIC1); __ASSERT(*ap++ == __MAGIC1);
		__ASSERT(*ap++ == __MAGIC1); __ASSERT(*ap   == __MAGIC1);
#endif /* __rcheck */
	}
	{
	 int nunits = p->mh_index;

		__ASSERT(nunits <= 29);
		p->mh_alloc = __ISFREE;
		__CHAIN(p) = nextf[nunits];
		nextf[nunits] = p;
#ifdef __MSTATS
		nmalloc[nunits]--;
		nfre++;
#endif /* __MSTATS */
	}
}

	extern caddr_t 
__MM_realloc(void *mem, size_t n)
{
 struct __mhead *p = (struct __mhead*)mem;
 size_t tocopy;
 size_t nbytes;
 size_t nunits;

	if (p == 0)
	    return __MM_malloc(n);
	--p;
	nunits = p->mh_index;
	__ASSERT(p->mh_alloc == __ISALLOC);
#ifdef __rcheck
	__ASSERT(p->mh_magic4 == __MAGIC4);
	{
	 caddr_t m = (caddr_t)mem + (tocopy = p->mh_nbytes);
		__ASSERT(*m++ == __MAGIC1); __ASSERT(*m++ == __MAGIC1);
		__ASSERT(*m++ == __MAGIC1); __ASSERT(*m   == __MAGIC1);
	}
#else
	if (p->mh_index >= 13)
	    tocopy = (1 << (p->mh_index + 3)) - sizeof *p;
	else
	    tocopy = p->mh_size;
#endif /* __rcheck */

	/* See if desired size rounds to same power of 2 as actual size. */
	nbytes =  ((long)(n + sizeof *p + __EXTRA + 7)) & ~7;

	/* If ok, use the same block, just marking its size as changed.  */
	if (nbytes > (4UL << nunits) && nbytes <= (8UL << nunits))
	{
#ifdef __rcheck
	 caddr_t m = (caddr_t)mem + tocopy;
		*m++ = 0;  *m++ = 0;  *m++ = 0;  *m++ = 0;
		p-> mh_nbytes = n;
		m = (caddr_t)mem + n;
		*m++ = __MAGIC1;  *m++ = __MAGIC1;  *m++ = __MAGIC1;  *m++ = __MAGIC1;
#else
		p->mh_size = n;
#endif /* __rcheck */
		return mem;
	}

	if (n < tocopy)
	    tocopy = n;
	{
	 caddr_t  new = __MM_malloc(n);

		if (new == 0)
		    return 0;
		__builtin_memcpy(new, mem, tocopy);
		__MM_free(mem);
		return new;
	}
}

#ifdef __MSTATS
/* Return statistics describing allocation of blocks of size 2**n. */
	extern struct __mstats_value *
__MM_malloc_stats(size)
    size_t size;
{
	static struct __mstats_value v;
	struct __mhead *p;

	v.nfree = 0;

	if ((int)size < 0 || size >= 30) {
		v.blocksize = 0;
		v.nused = 0;
		return &v;
	}
	v.blocksize = 1 << (size + 3);
	v.nused = nmalloc[size];

	for (p = nextf[size]; p; p = __CHAIN(p))
	    ++v.nfree;

	return &v;
}
#endif

/* how much space is available? */
	extern size_t
__MM_freespace() 
{
 int i, j;
 struct __mhead *p;
 size_t space = 0;
 int local;	/* address only is used */

	space = ((caddr_t)&local - (caddr_t)__sbrk(0));	/* stack space */

  	for (i = 0; i < 30; ++i) {
  		for (j = 0, p = nextf[i]; p; p = __CHAIN(p), ++j) ;
  		space += j * (1 << (i + 3));
	}
	return(space);
}

/* How big is this cell? */
	extern size_t 
__MM_mc_size(caddr_t  cp)
{
 struct __mhead *p;

	if ((p = (struct __mhead*)cp) == 0)
		return (u_long)-1;
	--p;
#ifdef __rcheck
	return p->mh_nbytes;
#else
	return (1 << (p->mh_index + 3)) - sizeof *p;
#if 0
	if (p->mh_index >= 13)
	    return (1 << (p->mh_index + 3)) - sizeof *p;
	else
	    return p->mh_size;
#endif
#endif /* __rcheck */
}

