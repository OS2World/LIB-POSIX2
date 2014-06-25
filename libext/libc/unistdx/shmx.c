/* 	$Id: shmx.c,v 1.3 2000/08/26 09:21:11 amai Exp $ 	*/

#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR
#include <os2.h>

#include <sys/builtin.h>
#include <sys/smutex.h>
#include <sys/types.h>
#include <machine/param.h>
#include <sys/ucred.h>
#define _KERNEL
#define _BSD_SOURCE
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/errnox.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Provides the following externally accessible functions:
 *
 * shminit(void);	                            - initialization
 * shmexit(struct vmspace*)                   - cleanup
 * shmfork(struct vmspace*, struct vmspace*)  - fork handling (not supported)
 * shmsys(arg1, arg2, arg3, arg4); = shm{at,ctl,dt,get}(arg2, arg3, arg4)
 *
 * Global structures:
 * shmsegs (an array of 'struct shmid_ds')
 * per proc array of 'struct shmmap_state' (not supported)
 */

/* Parts:
 * Copyright (c) 1994 Adam Glass and Charles M. Hannum.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Adam Glass and Charles M.
 *	Hannum.
 * 4. The names of the authors may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define	SHMSEG_FREE     	0x0200
#define	SHMSEG_REMOVED  	0x0400
#define	SHMSEG_ALLOCATED	0x0800
#define	SHMSEG_WANTED		0x1000

struct shmid_ds __SHMx_shmsegs[SHMMMNI];             /* shared data objects */
_smutex __SHMx_shm_lock = 0;

/*
 * System 5 style catch-all structure for shared memory constants that
 * might be of interest to user programs. 
 * global info struct about shm-related values
 */
struct	_shminfo 
__SHMx_shminfo = {
	SHMMAX,
	SHMMIN,
	SHMMNI,
	SHMSEG,
	SHMALL
};

unsigned long __SHMx_shm_nused = 0; /* every used id is <= max_nused */

/* You must initialize shm! The dll init does it for you, if you are lucky.
 */
	extern void 
__SHMx_shmInit(void)
{
 register unsigned long i = 0;
	/* initialize on instance */

        _dPuts("shm_init()\n");
        _smutex_request(&shm_lock);
	
	for (; i < SHMMMNI; ++i) {
		__SHMx_shmsegs[i].shm_perm.mode = SHMSEG_FREE;
		__SHMx_shmsegs[i].shm_perm.seq = 0;
	}
        _smutex_release(&shm_lock);
}

/*
 * Check user, group, other permissions for access
 * to ipc resources. return 0 if allowed
 */
	extern int
__SHMx_ipcperm(struct ucred *cred, struct ipc_perm *perm, int mode)
{  /* flag will most probably be 0 or S_...UGO */
 int requested_mode, granted_mode;

		/* we don't have credentials for now: */
	if (cred) if (mode == IPC_M) {
		if (cred->cr_uid == 0 ||
		    cred->cr_uid == perm->uid ||
		    cred->cr_uid == perm->cuid)
			return (0);
		errno =  EPERM; 		
		return(-1);
	}
	requested_mode = (mode >> 6) | (mode >> 3) | mode;
	granted_mode = perm->mode;
	granted_mode >>= 6;
	/* is there some bit set in requested_mode but not in granted_mode? */
	if (requested_mode & ~granted_mode & 0007) {
		errno = EACCES; 
		return -1;
        }
	return 0;
}

	static __inline__ int
shm_find_segment_by_key(key_t key)
{
	register unsigned int i = 0;

	while (i < __SHMx_shminfo.shmmni) {
                        _smutex_request(&shm_lock);
		_dPuts("shm_find_segment_by_key()...\n");
		if ( (__SHMx_shmsegs[i].shm_perm.mode & SHMSEG_ALLOCATED)
			 && __SHMx_shmsegs[i].shm_perm.key == key ) {
                        _smutex_release(&shm_lock);
			return(i);
                }
		++i;
        }
	_smutex_release(&shm_lock);
	return(-1);
}
	
	static struct shmid_ds *
shm_find_segment_by_shmid(int shmid)
{
 const unsigned int segnum = IPCID_TO_IX(shmid);
 struct shmid_ds shmperm;

	if (segnum >= __SHMx_shminfo.shmmni)
		return NULL;
	shmperm = __SHMx_shmsegs[segnum];
	if ( shmperm.shm_perm.mode == SHMSEG_WANTED
	  || shmperm.shm_perm.mode == SHMSEG_FREE
	  || shmperm.shm_perm.seq  != IPCID_TO_SEQ(shmid) )
		return (struct shmid_ds*)NULL;

	return(&__SHMx_shmsegs[segnum]);
}

/*
 * Here pages and shmid_ds are freed.
 */
	static __inline__ void 
killseg(struct shmid_ds *shp)
{
 APIRET rc;

	if (shp->shm_perm.mode == SHMSEG_WANTED
	 || shp->shm_perm.mode == SHMSEG_FREE || !shp->shm_pages)
		{errno = EFAULT; return;}

	/* DosFreeMem() decrements a reference count, so it should be */
	rc = DosFreeMem(shp->shm_pages); 	       /* called in shmdt() */
	if (rc) {
		_dPuts("libext: killseg(): DosFreeMem() failed\n");
		errno = _rc2Errno(rc);
		_dPrintf("DosFreeMem returned error code # %lu\n",rc);
		return;
        }
	_dPuts("libext: killseg(): DosFreeMem() successful\n");
	shp->shm_perm.mode = SHMSEG_FREE;
	while ( __SHMx_shm_nused > 0 && 
	  __SHMx_shmsegs[__SHMx_shm_nused - 1].shm_perm.mode == SHMSEG_FREE)
		--__SHMx_shm_nused;
}
	extern void 
__SHMx_shmDone(void)
        {
 register unsigned int id = 0;

	while (++id < SHMMMNI) {
			_smutex_request(&shm_lock);
		__SHMx_shmsegs[id].shm_perm.mode = SHMSEG_REMOVED;
		(void)killseg(&__SHMx_shmsegs[id]);
			_smutex_release(&shm_lock);
        }
	_dPuts("libext: shmx.c: shm_done()\n");
        }
	extern int 
shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
	if (cmd < 0 || shmid < 0)
                {errno = EINVAL; return(-1);}

 {register const unsigned
 i = IPCID_TO_IX(shmid);
  register struct shmid_ds *const
 shmseg = shm_find_segment_by_shmid(shmid);

	if (shmseg == NULL)
		{errno =  EINVAL; return(-1);}

        switch (cmd) {
        case IPC_STAT:
                if (!buf)
				{errno = EFAULT; return(-1);}
				
			if (i > __SHMx_shm_nused)
				{errno = EINVAL; return(-1);}
				
			if (shmseg->shm_perm.mode == SHMSEG_FREE
			 || shmseg->shm_perm.mode == SHMSEG_WANTED)
				{errno = EINVAL; return(-1);}
				
			if (__SHMx_ipcperm(NULL, &shmseg->shm_perm, S_IRUGO))
				return(-1);

			memcpy(buf, shmseg, sizeof(*buf));
			return(0);
        case IPC_SET:
			if (!buf)
				{errno = EFAULT; return(-1);}
				
			if (__SHMx_ipcperm(NULL, &shmseg->shm_perm, IPC_M))
				return(-1);
			shmseg->shm_perm.uid = buf->shm_perm.uid;
			shmseg->shm_perm.gid = buf->shm_perm.gid;
			shmseg->shm_perm.mode =
					(shmseg->shm_perm.mode & ~ACCESSPERMS) |
					(buf->shm_perm.mode & ACCESSPERMS);
			shmseg->shm_ctime = time(NULL);
			break;
		case IPC_RMID: {
			if (__SHMx_ipcperm(NULL, &shmseg->shm_perm, IPC_M))
				return(-1);
			shmseg->shm_perm.key = IPC_PRIVATE;
			shmseg->shm_perm.mode |= SHMSEG_REMOVED;
			if (shmseg->shm_nattch <= 0) {
                  _smutex_request(&shm_lock);
				killseg(&__SHMx_shmsegs[i]);
                  _smutex_release(&shm_lock);
                }
                break;
		}
		case SHM_LOCK:
		case SHM_UNLOCK:
        default:
			errno = EINVAL;
			return(-1);
	}
        }
        return 0;
}

/*
 * allocate new shmid_ds. protected by: 
 * "seg_p->shm_perm.mode = SHMSEG_ALLOCATED|SHMSEG_REMOVED;"
 */
	static int 
newseg(key_t key, int mode, size_t size)
{
 register unsigned int id = 0;

	if (size < __SHMx_shminfo.shmmin || size > __SHMx_shminfo.shmmax)
		{errno = EINVAL; return(-1);}
	if (__SHMx_shm_nused >= __SHMx_shminfo.shmmni) 		/* any shmids left? */
		{errno = ENOSPC; return(-1);}
			
	while (id < __SHMx_shminfo.shmmni) {
		if (__SHMx_shmsegs[id].shm_perm.mode == SHMSEG_FREE)
			break;
		 ++id;
	}
	if (id == __SHMx_shminfo.shmmni) {
		fputs("libext: shmx: newseg(): Shmseg free count inconsistent!\n", stderr);
		errno = ENOSPC; return(-1);
	} {
	 APIRET rc;
	 struct shmid_ds *const seg_p= &__SHMx_shmsegs[id];
	_smutex_request(&shm_lock);
	/* mark present but deleted so that noone else tries to create the same key. */
	seg_p->shm_perm.mode = SHMSEG_ALLOCATED|SHMSEG_REMOVED;
	seg_p->shm_perm.key = key;
	seg_p->shm_perm.seq = (seg_p->shm_perm.seq + 1) & 0x7fff;
 
	rc = DosAllocSharedMem(&seg_p->shm_pages, NULL, size,
			OBJ_GETTABLE|PAG_COMMIT|PAG_READ|PAG_WRITE);
	if (rc) {
		_dPrintf("libext: newseg(): DosAllocSharedMem() error: rc = %lu\n",rc);
		__SHMx_shmsegs[id].shm_perm.mode = SHMSEG_FREE;
		_smutex_release(&shm_lock);
		errno = _rc2Errno(rc); return(-1);
	}
	++__SHMx_shm_nused;
	_dPrintf("libext: newseg(): Shmseg free count = %lu - ", __SHMx_shm_nused);
	_dPuts("DosAllocSharedMem() successful\n");

	seg_p->shm_perm.cuid = seg_p->shm_perm.uid = 0;
	seg_p->shm_perm.cgid = seg_p->shm_perm.gid = 0;
	seg_p->shm_perm.mode = (seg_p->shm_perm.mode & SHMSEG_WANTED)|
	    (mode & ACCESSPERMS)|SHMSEG_ALLOCATED;
	seg_p->shm_segsz = size;
	seg_p->shm_cpid = getpid();
	seg_p->shm_lpid = seg_p->shm_nattch = 0;
	seg_p->shm_atime = seg_p->shm_dtime = 0;
	seg_p->shm_ctime = time(NULL);
	_smutex_release(&shm_lock);
	_dPuts("newseg() successful\n");
	return(IXSEQ_TO_IPCID(id, seg_p->shm_perm)); 
 }
}

	extern int 
shmget(key_t key, int size, int shmflg)
{
	if (size < 0 || (size_t)size > __SHMx_shminfo.shmmax)
		{errno = EINVAL; return(-1);}

        if (key == IPC_PRIVATE)
                return newseg(key, shmflg, (size_t)size);
		 
 {const int
	id = shm_find_segment_by_key(key);
	if (id == -1) {
                if (!(shmflg & IPC_CREAT))
			{errno = ENOENT; return(-1);}
			
                return newseg(key, shmflg, (size_t)size);
        }

        if ((shmflg & IPC_CREAT) && (shmflg & IPC_EXCL))
		{errno = EEXIST; return(-1);}
 {int i = 0;
 struct shmid_ds *const
  seg = &__SHMx_shmsegs[id];

	while (seg->shm_perm.mode & SHMSEG_REMOVED) {
	/* This segment is in the process of being allocated.  Wait
	 * until it's done, and look the key up again (in case the
	 * allocation failed or it was freed).
	 */
		seg->shm_perm.mode |= SHMSEG_WANTED;
		(void)DosSleep(2000);
		if (++i > 30)
			{errno = EAGAIN; return(-1);}
	}
	if (__SHMx_ipcperm(NULL, &seg->shm_perm, shmflg))
		return(-1);
		
	if (size > seg->shm_segsz)
		{errno = EINVAL; return(-1);}

        _dPuts("shmget() successful");
	return(IXSEQ_TO_IPCID(id, seg->shm_perm));
 }}
        }


/*
 * Fix shmaddr, allocate descriptor, map shm, add attach descriptor to lists.
 */
extern void *
shmat(const int shmid, const void *shmaddr, int shmflg)
{
        struct shmid_ds *shp;
 const unsigned int segnum = IPCID_TO_IX(shmid);

	if (segnum >= __SHMx_shminfo.shmmni)
                {errno = EINVAL; return (void*)NULL;}

	shp = &__SHMx_shmsegs[segnum];

	if (shp->shm_perm.mode == SHMSEG_FREE
	 || shp->shm_perm.mode == SHMSEG_WANTED)
                {errno = EINVAL; return (void*)NULL;}

        if (shmaddr) { /* just ignore for now */
        	_dPuts("libext: shmat(): Address is currently ignored!\n");
        } else _dPuts("shmat() 1 ...\n");

        if (__SHMx_ipcperm(NULL, &shp->shm_perm, shmflg & SHM_RDONLY ? 
        		S_IRUGO : S_IRUGO|S_IWUGO))
          {errno = EACCES; return (void *)NULL;} 
        if (shp->shm_perm.seq != IPCID_TO_SEQ(shmid))
          {errno = EIDRM; return(void*)NULL;}

_dPuts("shmat() 2 ...\n");

        ++shp->shm_nattch;            /* prevent destruction */

        shp->shm_lpid = getpid();
        shp->shm_atime = time(NULL);
        {APIRET rc = 
        DosGetSharedMem(shp->shm_pages, PAG_READ | PAG_WRITE );
        if (rc) {
                _dPuts("DosGetSharedMem() failed\n");
                errno = _rc2Errno(rc);
                return(void*)NULL;
        } else
          _dPuts("DosGetSharedMem() successful\n");
        }
        return(shp->shm_pages);
}

/*
 * detach segment
 */
extern int 
shmdt(const void *shmaddr)
{
 register unsigned int i = 0;

  for (; i < __SHMx_shm_nused; ++i) {
	 register struct shmid_ds *const 
 		shp = &__SHMx_shmsegs[i];
		while (shp->shm_perm.mode == SHMSEG_WANTED) { /* slow loop */
                        _smutex_request(&shm_lock);
				DosSleep(3000);
                        _smutex_release(&shm_lock);
                }
		if (shp->shm_perm.mode == SHMSEG_FREE)
                        continue;

		if (((vaddr_t)shp->shm_pages <= (vaddr_t)shmaddr)
		 && ((vaddr_t)shmaddr <= ((vaddr_t)shp->shm_pages + shp->shm_segsz))) {
			if (shp->shm_nattch > 0) {
			  --shp->shm_nattch;
				if (shp->shm_nattch > 0) 		/* only shmctl() physically destroys */
					DosFreeMem(shp->shm_pages);			/* DosFreeMem() decrements 
			   a reference count, so it should be called here */
			}
                        _dPuts("shmdt() successful\n");
			return(0);
                }
        }
	return(-1);
}

#ifdef DEBUG
/* Display information about active shared memory segments */
	extern void
_SHMx_showActiveSeg(void)
{
 register unsigned int id = 0;
 struct shmid_ds *shp = &__SHMx_shmsegs[id];

	_dPuts("\nActive shared memory segments: \n");
	for (;id < __SHMx_shm_nused; ++id, ++shp) {
		while (shp->shm_perm.mode == SHMSEG_WANTED) {
                        _smutex_request(&shm_lock);
                        _smutex_release(&shm_lock);
                }
		if (shp->shm_perm.mode != SHMSEG_FREE) {
                        _smutex_request(&shm_lock);
			fprintf(stderr, "# = %d, key = %ld, nattch = %d\n",
				id, shp->shm_perm.key, shp->shm_nattch);
                        _smutex_release(&shm_lock);
                }
        }
}
#endif

