/* sys/shm.h (libext+gcc)	- $Id: shm.h,v 1.6 2000/05/02 07:46:16 veit Exp $	*/

/*
 * Copyright (c) 1994 Adam Glass
 * All rights reserved.
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
 *      This product includes software developed by Adam Glass.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * As defined+described in "X/Open System Interfaces and Headers"
 *                         Issue 4, p. XXX
 */
#ifndef _SYS_SHM_H_
#define _SYS_SHM_H_

#include <sys/ipc.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define	SHM_RDONLY	010000	/* Attach read-only (else read-write) */

#ifndef __EMX__		/* not supported */
#define	SHM_RND		020000	/* Round attach address to SHMLBA */
#endif
#define	SHMLBA		CLBYTES	/* Segment low boundry address multiple */

/* "official" access mode definitions; braindead since you have
   to specify (SHM_* >> 3) for group and (SHM_* >> 6) for world permissions */
#define SHM_R		(IPC_R)
#define SHM_W		(IPC_W)

struct shmid_ds {
	struct ipc_perm	shm_perm;	/* operation permission structure */
	int		shm_segsz;	/* size of segment in bytes */
	pid_t		shm_lpid;	/* process ID of last shm op */
	pid_t		shm_cpid;	/* process ID of creator */
	short		shm_nattch;	/* number of current attaches */
	time_t		shm_atime;	/* time of last shmat() */
	time_t		shm_dtime;	/* time of last shmdt() */
	time_t		shm_ctime;	/* time of last change by shmctl() */
        /* private */
	void		*shm_pages;	/* sysv stupidity */
};

/*
 * Values in support of System V compatible shared memory.	XXX
 */
#define SHMMAX 	0x400000 	/* max shared seg size (bytes) */
#define	SHMMIN	1 			/* min shared seg size (bytes) */
#define	SHMALL 	(SHMMAX/NBPG)   /* max shm system wide pages (ignored) */
#define SHMMMNI 256		/* max num of segs system wide */
#define	SHMMNI	SHMMMNI 	/* <= SHMMMNI in shm.h */
#define SHMSEG  64         	/* max shared segs per process */

/* shm_mode upper byte flags */
#define SHM_DEST        01000   /* segment will be destroyed on last detach */
#define SHM_LOCKED      02000   /* segment will not be swapped */

/*
 * System 5 style catch-all structure for shared memory constants that
 * might be of interest to user programs.  Do we really want/need this?
 */
struct _shminfo {
 unsigned int 	shmmax;		/* max shared memory segment size (bytes) */
 unsigned int	shmmin;		/* min shared memory segment size (bytes) */
 unsigned int	shmmni;		/* max number of shared memory identifiers */
 unsigned int	shmseg;		/* max shared memory segments per process */
 unsigned int	shmall;		/* max amount of shared memory (pages) */
};
#define shminfo 	__SHMx_shminfo

#ifdef _KERNEL

/* Some (e.g. HP-UX) take these as second (super user cmd) arg to shmctl(). */
#define	SHM_LOCK	3	/* Lock segment in memory. */
#define	SHM_UNLOCK	4	/* Unlock a segment locked by SHM_LOCK. */

struct oshmid_ds {
	struct oipc_perm shm_perm;	/* operation permission structure */
	int		shm_segsz;	/* size of segment in bytes */
	pid_t		shm_lpid;	/* process ID of last shm op */
	pid_t		shm_cpid;	/* process ID of creator */
	short		shm_nattch;	/* number of current attaches */
	time_t		shm_atime;	/* time of last shmat() */
	time_t		shm_dtime;	/* time of last shmdt() */
	time_t		shm_ctime;	/* time of last change by shmctl() */
	void		*shm_internal;	/* sysv stupidity */
};
extern struct shmid_ds  __SHMx_shmsegs[SHMMMNI];
extern _smutex __SHMx_shm_lock;
extern unsigned long __SHMx_shm_nused;

#define shmsegs  	__SHMx_shmsegs
#define shm_lock	__SHMx_shm_lock
#define shm_nused	__SHMx_shm_nused

/* struct vmspace; */

extern void __SHMx_shmInit(void);
extern void __SHMx_shmDone(void);

#if 0
void shmfork(struct vmspace *, struct vmspace *);
extern void shmexit(struct vmspace *);
extern void shmid_n2o(struct shmid_ds *, struct oshmid_ds *));
#endif

#define shminit() 	__SHMx_shmInit()
#define shmexit(vms) 	__SHMx_shmDone()

#endif /* !_KERNEL */

extern void *shmat(int,__const__ void*,int);
extern int shmctl(int,int,struct shmid_ds*);
extern int shmdt(__const__ void*);
extern int shmget(key_t,int,int);


/* You must initialize shm! The dll init does it for you, if you are lucky.
 */
extern void __SHMx_shmInit(void);
extern void __SHMx_shmDone(void);

/* Display information about active shared memory segments
 */
extern void _SHMx_showActiveSeg(void);

#if defined (__cplusplus)
}
#endif

#endif /* !_SYS_SHM_H_ */
