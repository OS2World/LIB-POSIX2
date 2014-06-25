/* $Id: itimer.c,v 1.4 2000/09/11 00:35:29 arnd Exp $ */ 
/* -- Crude itimer implementation for emx/gcc. -- Lots of functionality missing.
    ITIMER_VIRTUAL  and ITIMER_PROF are not yet implemented, they just
    measure real time only, Fix me !
    (C) 2000 Arnd Hanses, Alexander Mai <st002279@hrzpub.tu-darmstadt.de>: 
    This code is released into Public Domain.  
    You are free to use this code for any purpose, provided this notice
    remains intact and that we will be exempt of any liability, whatsoever. 
    Though it's released into PD I would appreciate to see any enhancements/
    fixes that you will apply.
    */
/* #undef DEBUG if you don't want debugging */

#ifndef _THREAD_EXPERIMENTAL
#define _THREAD_EXPERIMENTAL
#endif

#define INCL_BASE
#define OS2_API32
#define INCL_DOS
#include <os2.h>
#include <sys/time.h>
#include <sys/process.h>	/* _getpid */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>     /* _threadid */
#include <sys/types.h>  /* pid_t */
#include <sys/errnox.h>

#if !defined (ITIMER_REAL)
# include "sys/timext.h"
#endif

static __inline__ int _CpItimerVals(const struct itimerval *,
			struct itimerval *); 			/* Convenience utility */

static void _AlarmThread(int); 	/* On timing event send SIGALRM */

/*  global: Time remaining on system interrupt timers (0 when timer disabled)
     Storage of current decremented values not implemented yet. Fix me!
     ITIMER_VIRTUAL  and ITIMER_PROF are not yet implemented, they just
     measure real time only. Fix me !*/
	struct itimerval
__itimer_timeValueOnInterruptTimer[3] = {{{0L},{0L}},{{0L},{0L}},{{0L},{0L}}}; 


/* Convenience utility: Copy two itimerval structs. */
	static __inline__ int
_CpItimerVals (const struct itimerval *from, struct itimerval *to) {
  if (!(from && to) || (from == to)) {			/* invalid it pointers */
     errno = EFAULT; return(-1); } else { 
     to->it_interval.tv_sec  = from->it_interval.tv_sec;
     to->it_interval.tv_usec = from->it_interval.tv_usec;
     to->it_value.tv_sec     = from->it_value.tv_sec;
     to->it_value.tv_usec    = from->it_value.tv_usec;
     return (0); } 
}

/* Get Remaining Time on Timer 'which'  (exported API):

Errors: EFAULT - Argument no valid pointer to struct itimerval 
        EINVAL - Invalid timer selection argument
        ENOSYS - Functionality not implemented on this system
        */
	int
_getitimer(const unsigned int which, struct itimerval *value)
{
  if ( !value ) {			 	/* sanity checks */
     errno = EFAULT;
     return (-1);
  }
  if ( (which != (ITIMER_REAL)) /* ITIMER_REAL more or less implemented... */
       && (which != ITIMER_VIRTUAL) && (which != ITIMER_PROF) ) {
    errno = EINVAL;
    return (-1); 				/* invalid "which" */
  } else {
    if (_CpItimerVals(&__itimer_timeValueOnInterruptTimer[which], value)) return (-1);
    if ( _itOneIsSet(__itimer_timeValueOnInterruptTimer[which]) ){ /* not decremented */
      _dPuts("itimer.c: getitimer(): Fix me! Values are not yet correct\n");
      errno = ENOSYS;
      return (-1);  /* the stored values are not yet decremented correctly */
    }
    return (0);
  }
}

/* setitimer():
 * Set the interrupt timer 'which': One of the exported API calls
 * Errors: EFAULT - Argument no valid pointer to struct itimerval 
 *         EINVAL - Invalid timer selection argument
 *         ENOSYS - Functionality not implemented on this system
 */

/* setitimer() will stop/reset the three system timers: Serialize the access  */
TID __itimer_tidAlarmThreads[3] = {0UL,0UL,0UL}; /* Global threads repository */

HEV __itimer_hevSem[3] = {0UL,0UL,0UL}; /* Global event semaphore handles */

HTIMER __itimer_htimerInterruptTimer[3] = {0UL,0UL,0UL}; /* Global handle to
	stop or reset the three system timers; both, for single and interval */

 
#ifdef _THREAD_EXPERIMENTAL
static struct sigaction oact;
static void(*sighandlers[3])(int) = {NULL, NULL, NULL};
#endif

static ULONG interval_mSecs = 0UL;


	int
_setitimer(unsigned int which, const struct itimerval *value,
              struct itimerval *outvalue) 
{ /* Note: Additional overhead (besides the constant system timer's
 * granularity) depends on the system load. No exact results here !
 * Nevertheless we will Warp to a higher priority, because we supply
 * a critical real time timing service to other functions, that may 
 * depend on correct results. Adjust to your hardware's granularity;
 * cf. the header for more details!
 */
 APIRET rc = DosEnterCritSec();
 		/* Now we can forcedly stop timing */
  if (rc) {
		(void)DosExitCritSec();
    _dPuts("itimer.c: setitimer(): Critical section failed!\n");
    errno = _rc2Errno(rc); /* need return ? */ 
  } /* now we should have reduced the impact of heavy system load */
  if ( !value || (value == outvalue) ) {		  /* sanity checks */
		(void)DosExitCritSec();
     errno = EFAULT;
     return(-1);
  }
  if ( (value->it_value.tv_usec <0L) || (value->it_value.tv_usec >1000000L)
     || (value->it_value.tv_sec <0L)||(value->it_interval.tv_usec <0L) ||
     (value->it_interval.tv_usec >1000000L)||(value->it_interval.tv_sec <0L)) {
		(void)DosExitCritSec();
		errno = EINVAL;
		return(-1);
  } 
  if (outvalue)
  	if (_CpItimerVals(&__itimer_timeValueOnInterruptTimer[which],
  					outvalue)) {
			(void)DosExitCritSec();
  	return(-1);
  	}
	/* invalid "which"? */
  if ( (which != ITIMER_REAL) /* ITIMER_REAL more or less implemented... */
       && (which != ITIMER_VIRTUAL) && (which != ITIMER_PROF) ) {
		(void)DosExitCritSec();
		errno = EINVAL; return(-1);
  }
	/* First store requested values for real time itimer */
	if (_CpItimerVals(value, &__itimer_timeValueOnInterruptTimer[which])) {
		(void)DosExitCritSec();
		return (-1); /* If the
			interval timer is reset, it waits for expiration of current and
			ends the loop. */
	}
	/* End of critical section: */
	if ( (rc = DosExitCritSec()) ) {
		_dPuts("itimer.c: _setitimer(): Error exiting the critical section.\n");
		errno = _rc2Errno(rc); 
		return(-1);
	}
	if ( !_itOneIsSet(*value) ) {
		if ( !__itimer_tidAlarmThreads[which] ) {
			_dPuts("itimer.c: No timer was running, no need to reset\n"); 
			return(0);
		} 
		if ( (rc = DosPostEventSem(__itimer_hevSem[which])) ) {
			_dPrintf("itimer.c: setitimer(): Semaphore post error #%ld\n", rc); 
			errno = _rc2Errno(rc); 
			return(-1);
		}
		return(0);
	} else {       /* Here starts the timer setting block */
	 register long mSecs = _tv2mSec(value->it_value);
	 const int ourThreadId = *(_threadid);       /* Only #1 is safe */

		if (!(ourThreadId == 1)) {
			_dPuts("itimer.c: setitimer(): Thread # > 1! This is unsafe. Fix me!\n");
#ifdef _THREAD_EXPERIMENTAL
		/* Not thread #1 is calling. Assume it doesn't matter who
		 does the job. So get the signal handler of _this_ thread now
		 and call it from the AlarmThread.  */
			sigaction(SIGALRM, NULL, &oact);
			sighandlers[which] = oact.sa_handler;
#else	/* Not primary thread is calling; may be crucial it does the jobs itself.
         Therefore better raise SIGALRM only in #1 */
			errno = ENOSYS;
			return(-1);
#endif
		} 
#ifdef DEBUG
		_dPrintf("itimer.c: which = %i  -- ", which); 
#ifdef _THREAD_EXPERIMENTAL
		if (sighandlers[which])
			_dPuts( "itimer.c: _setitimer(): Call sighandler\n");
		else
			_dPuts( "itimer.c: _setitimer(): raise SIGALRM here \n");
#endif    
#endif    
		_dPrintf("\
itimer.c: set single shot itimer value to '%ld' msec...\n", mSecs); 
		if (mSecs < 0) {
			mSecs = 0; /* interval too short for this machine */
			_dPuts("itimer.c: Interval too short for this machine: adjusted! *\n");
		}   
		_dPuts("itimer.c: First create a reset event semaphore...\n"); 
		if ( (rc = DosCreateEventSem(NULL, &__itimer_hevSem[which], 
							DC_SEM_SHARED, FALSE)) ) {
			_dPrintf("itimer.c: setitimer(): Semaphore creation error #%ld\n", rc);
			errno = _rc2Errno(rc); return(-1);
		}
		_dPuts("itimer.c: Start async (one-shot) timer; post semaphore\n");
		if ( (rc = DosAsyncTimer( (ULONG)mSecs,   /* unsigned (milisecs)! */
						 (HSEM)__itimer_hevSem[which],    /* Semaphore handle */
						 &__itimer_htimerInterruptTimer[which])) ) {  /* handle to stop */
			_dPrintf("itimer.c: setitimer(): DosAsyncTimer() error #%ld\n", rc);
			errno = _rc2Errno(rc); return(-1);
		}
		if (_itIntvlIsSet(*value)) { /* interval timing block */
			mSecs = _tv2mSec(value->it_interval);
			if (mSecs < 0)
				mSecs = 0; /* too short an interval for machine */
			interval_mSecs = mSecs;
		}
		if ( __itimer_tidAlarmThreads[which] ) { 	/* AlarmThread still running */
		_dPuts("itimer.c: AlarmThread is still running: Stop and restart\n"); 
			if ( (rc = DosPostEventSem(__itimer_hevSem[which])) ) { /* stop it */
				_dPrintf("itimer.c: setitimer(): Semaphore post error #%ld\n", rc); 
				errno = _rc2Errno(rc); return (-1);
			}
		} {  /* wait in newly created _AlarmThread() until timer expires */
		 const int tid =
			_beginthread((void(*)(void*))_AlarmThread, NULL, 131072, (void*)which);
			if (tid < 2) {
				_dPuts("setitimer(): beginthread error"); return(-1);
			}
			__itimer_tidAlarmThreads[which] = (TID)tid;
		}

		if ((rc = DosSetPriority(PRTYS_THREAD, PRTYC_FOREGROUNDSERVER,0L,0UL))) {
			_dPuts("itimer.c: _setitimer(): Error exiting the critical section.\n");
			errno = _rc2Errno(rc); return(-1);
		}
		return (0); /* Poooof..... */ 
	}    /* end of timer setting block */
  _dPuts("itimer.c: setitimer(): Internal error (fall through)");
  errno = EFAULT; return(-1);  /* should never happen */
}


	static void
_AlarmThread(int which)
{
 APIRET rc = DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, +2, 0);
 const pid_t ourpid = _getpid();
 
  _dPrintf("itimer.c: AlarmThread# <%lu>: Warp to higher priority...\n",
  	__itimer_tidAlarmThreads[which]);
  if (rc) 
    fputs("libextensions: itimer.c: AlarmThread(): DosSetPriority failed!\n",
    		stderr); 		/* impact of heavy system load has been reduced */

  if (_itOneIsSet(__itimer_timeValueOnInterruptTimer[which])) {
		_dPuts("\
itimer.c: AlarmThread(): Wait until one shot timer expires...\n");
		if ( (rc = DosWaitEventSem(__itimer_hevSem[which], SEM_INDEFINITE_WAIT)) ) {
			_dPrintf("\nitimer.c: AlarmThread(): Wait error <#%ld>\n", rc);
			__itimer_tidAlarmThreads[which] = 0;
			exit(99);
		}
		/* Do we still want alarm? */
		if (_itOneIsSet(__itimer_timeValueOnInterruptTimer[which])) {
#ifdef _THREAD_EXPERIMENTAL
			if (sighandlers[which]) 
				sighandlers[which](SIGALRM);
			else
#endif    
			if (_kill(ourpid, SIGALRM)) { 	/* raise SIGALRM in main thread 1 */
				perror("\nitimer.c: AlarmThread(): Signal error: signal()");
				__itimer_tidAlarmThreads[which] = 0;
				exit(99);
			}
		}
	} /* start of interval timing block */
	if (_itIntvlIsSet(__itimer_timeValueOnInterruptTimer[which])) 
		if (_itOneIsSet(__itimer_timeValueOnInterruptTimer[which])) {
		 ULONG ulPostCount; /* # of event semaphore posts before resetting */
			_dPrintf("itimer.c: Set interval to '%ld' msec...\n", interval_mSecs); 
			/* First reset event semaphore. */
			if ( (rc = DosResetEventSem(__itimer_hevSem[which], &ulPostCount)) ) {
				_dPrintf("\nitimer.c: AlarmThread(): Semaphore reset error #%ld\n", rc);
				__itimer_tidAlarmThreads[which] = 0;
				exit(99);
			}
			if (ulPostCount > 1) fprintf(stderr, "\n\
libExt: itimer.c: AlarmThread(): Warning: <%lu> timer events missed !",
							ulPostCount - 1);
			if ( (rc = DosStartTimer(interval_mSecs, (HSEM)__itimer_hevSem[which],
						&__itimer_htimerInterruptTimer[which])) ) {
				_dPrintf("itimer.c: AlarmThread(): Interval start error #%ld\n", rc);
				__itimer_tidAlarmThreads[which] = 0; exit(99);
			}
			do {     /* interval timer endless alarm loop */
				_dPuts("\
itimer.c: AlarmThread(): Wait until interval timer expires...\n");
				if ( (rc = DosWaitEventSem(__itimer_hevSem[which], SEM_INDEFINITE_WAIT)) ) {
						_dPrintf("\
	itimer.c: AlarmThread(): Wait for interval timer error #%ld\n", rc); 
						__itimer_tidAlarmThreads[which] = 0;
						exit(99);
				}
				if (_itOneIsSet(__itimer_timeValueOnInterruptTimer[which])) { /* Do we still want alarm? */
					if (sighandlers[which]) sighandlers[which](SIGALRM);
					else if(_kill(ourpid, SIGALRM)) { /* raise SIGALRM in main thread 1 */
						perror("\nitimer.c: AlarmThread(): Alarm signal error");
						__itimer_tidAlarmThreads[which] = 0;
						exit(99);
					}
				}
				else 
					break; /* value has been cleared by another call to setitimer */
				if ( (rc = DosResetEventSem(__itimer_hevSem[which], &ulPostCount)) ) {
					_dPrintf("itimer.c: AlarmThread(): Semaphore reset error #%ld\n", rc);
					__itimer_tidAlarmThreads[which] = 0;
					exit(99);
				}
				if (ulPostCount > 1) fprintf(stderr, "\
	libextensions: itimer.c: AlarmThread(): Warning: <%lu> timer events missed !",
								ulPostCount - 1);
			} while (1); 
			if	(	(rc = DosStopTimer(__itimer_htimerInterruptTimer[which])) ) {
				_dPrintf("itimer.c: _setitimer(): Interval stop error <#> %lu\n", rc);
				errno = _rc2Errno(rc);
				exit(99);
			}
		} /* end of interval timing block */
	_dPuts("Now the cleanup...");
	if ((rc = DosCloseEventSem(__itimer_hevSem[which]))) {
		_dPrintf("itimer.c: AlarmThread(): Semaphore close error #%ld\n", rc);
	}
	_dPuts("Done. Ending _AlarmThread(). Poof ! \n");
	__itimer_tidAlarmThreads[which] = 0;
	return;
}
#if 0
static int GetThreadNumber(void) {
/* exit(99) number of current thread inside this process */

  int tid;

  PTIB   pptib;
  PPIB   pppib;
  APIRET rc;

  rc = DosGetInfoBlocks(&pptib, &pppib);
  tid = (int)pptib->tib_ptib2->tib2_ultid;
  tid = (int) _gettid();
  return tid;
}
#endif
	extern int
getitimer(const unsigned w,struct itimerval*it) {return _getitimer(w,it);}
	extern int
setitimer(const unsigned w,const struct itimerval*it,struct itimerval*o)
{return _setitimer(w,it,o);}

