/* * * usleep.c/.h --- emx/gcc --- (C), 2000 Arnd Hanses: 
   You are free to use this code for any purpose, provided the Copyright
   remains intact and that I will be exempt of any liability, whatsoever. 
   */

#define INCL_BASE
#define OS2_API32
#define INCL_DOSFILEMGR
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/errnox.h>

#include <sys/unistdx.h>

#ifdef DEBUG
#define dprintf(x,y) fprintf(stderr,x,y)
#else
#define dprintf(x,y) /* empty */
#endif

/* Open the hfTimer, initialize it, call this once fn before doing any timings:
RETURNS: 0, on success
        -1, on error, ERRNO is set accordingly         */

int __usleep_hfUsleepTimer = 0;  /* unfortunately must be global; succesive */
ULONG __usleep_ul2HRTimerReadsOverhead = 0UL; /* calls of usleep will access
	the values without calling _initHrTimer(), hence they must not be static */

	extern int /* For speed this is not static, call once upon initialization */
_initHrTimer(void)
{
 ULONG ulStart, ulStop;
 	__usleep_hfUsleepTimer =
		_open("TIMER$", O_RDONLY|O_BINARY); /* Timer handle */;	

	if (__usleep_hfUsleepTimer == (-1)) {
		fputs(
"_initHrTimer(): Opening hfTimer device 'TIMER$' failed!\n\
Please make sure that you correctly installed HRTIMER.SYS !\n", stderr);
		return (-1);
	}
	dprintf("\
* Opening the hfTimer device 'TIMER$', handle: '%d'...\n\
* Determine overhead for two successive reads...\n", __usleep_hfUsleepTimer);

	/* dummy */
	if ( 1 > _read(__usleep_hfUsleepTimer, &ulStop, sizeof(ULONG)) ) {
		fputs("_initHrTimer(): Reading device 'TIMER$' failed!\n", stderr);
		return (-1);
	}
	if ( 1 > _read(__usleep_hfUsleepTimer, &ulStart, sizeof(ULONG)) ) 
		return (-1);
	_read(__usleep_hfUsleepTimer, &ulStop, sizeof(ULONG));
	/* the overhead of 2 successive Timer reads; 
		 calculated in _initHrTimer() and later subtracted  */
	(int)__usleep_ul2HRTimerReadsOverhead = ulStop - ulStart;
	dprintf ("Overhead = %10lu us\n", __usleep_ul2HRTimerReadsOverhead);
	return (0);
}


/* Close the hfTimer, do cleanup, call this fn after doing all timings:
   RETURNS
         0, on success
        -1, on error, ERRNO is set accordingly
        */
	int
_termHrTimer(void)
{
	return _close(__usleep_hfUsleepTimer);
}

	void	 			/* sleep for ulWait microseconds */
_uSleep1(const ULONG ulWait)
{ 
 const ULONG ulStart; 
 ULONG ulStop;
 
	if (ulWait == 0)
		return;

	if ( 1 > read (__usleep_hfUsleepTimer, (PULONG)&ulStart, sizeof(ULONG)) ) {
		perror("usleep(): Did you initially call initHrTimer() ? Error");
		return;
	} else {
		const ULONG ulCompareTime = ulWait - __usleep_ul2HRTimerReadsOverhead;
		do { read (__usleep_hfUsleepTimer, &ulStop, sizeof(ULONG));
		} while (ulCompareTime > (ulStop - ulStart) );
	}
}
	void	 			/* sleep for ulWait microseconds */
_uSleep2(const ULONG ulWait)
{ 			/* DosSleep() with resolution of milliseconds. */ 
 __const__ APIRET rc = 
	DosSleep(ulWait / 1000); /* argument to my_usleep is in microseconds */
	if (rc) errno = _rc2Errno(rc);	return;
}

