/* $Id: itimertest.c,v 1.3 2000/04/17 08:25:49 veit Exp $ */ 
/******* -- test code --
    (C) 2000 Arnd Hanses, Alexander Mai <st002279@hrzpub.tu-darmstadt.de>:
    This code is released into Public Domain.
    You are free to use this code for any purpose, provided this notice
    remains intact and that we will be exempt of any liability, whatsoever.
Though it's released into PD I would appreciate to see any enhancements/fixes
that you will apply.
The following code sets a `timebomb'. After Timer is called, the program
will continue execution until `n' milliseconds have passed, then normal
execution will be interrupted and `onalarm()' will be called before
normal execution is resumed. This same method can be used to catch 
emergency signals like SIGBUS (bus error) too.
    ITIMER_VIRTUAL  and ITIMER_PROF are not yet implemented, they just
    measure real time only, Fix me !

Thanks in advance!

Important notes:
===============

 - compile/link _everything_ with -Zmt (you should do for _all_ your projects)

 - Link with -Zbsd-signals ! (well, probably necessary.
   Check for sigaction() in the code and read the appropriate EMX docs)

 - Check that the source is using sigaction() exclusively and
   not mix signal() with it !!

 - Only ITIMER_REAL is implemented (if at all ...)
   The others may be a bit hard to implement. But perhaps
   for some purposes it will be sufficient to just use
   (copy) the ITIMER_REAL capabilities (code)

 - If the calling thread is thread #1 (of the current process)
   it will work as expected, a SIGALRM will be raised there.
   If not, the sigaction()-defined handler will be called, i.e.
   the handler will be called from a different (the itimer-)thread
  
   Note that for X11 software it is currently necessary to make
   all X calls from within a single thread. Since this is usually
   thread #1, make sure this is also calling setitimer()

 - The source may not be thread-safe itself yet! 

 - #undef DEBUG if you don't want debugging 
   *******/
#define DEFAULT_TIME_PERIOD 5555
#define MAX_LOOP  5

#define INCL_DOSPROCESS /* DosSleep */
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>

static void onalarm(int dummy)
{
  _dPuts("*********** BAAAAANG! This was the time bomb ***********\n");
	fflush(NULL);
#if 0
  signal(SIGALRM, onalarm);   /* reinstall handler (SYSV): */
#endif /* This is often too late, so you'll see the default action 
  happen: Termination. So better use -Zbsd */
}

int main(int argc, char *argv[])
{
 unsigned long uSec = DEFAULT_TIME_PERIOD * 1000UL;
 struct itimerval it = {{0L, 0L}, {0L, 0L}}; /* always initialize */
	if (argc == 2) 	uSec = atoi (argv[1]) * 1000;
	printf("ITIMER-TEST - A short skeleton to demonstrate the use \n\
	Usage: itimer [TIMEPERIOD (msecs)]\n\
* Setting itimer to <%lu> micro-seconds\n", uSec);
	if ( ! uSec )	_dPuts("Zero value!\n");

	if ( getitimer(ITIMER_REAL, &it) ) {
		perror("getitimer()"); return (-1);
	} else {
	 int i = 0;
	 struct itimerval old_it;	/* struct sigaction * act; */
	 char buf[BUFSIZ];

		_dPuts("* install handler... *");
		signal(SIGALRM, onalarm);            /* install handler */
		
		_dPuts("* Start one shot real time alarm clock, signal alarm *...\n");
		_itSetValue(it, uSec);
		if ( setitimer(ITIMER_REAL, &it, &old_it) ) {
					perror("Setitimer()"); return (-2);
		}	
		_dPuts(" Now sleeping..."); fflush(NULL);
		DosSleep(uSec / 1000);
		_dPuts("*** Hit return, if you want to continue, hit 'e' to end! ***\n");
		gets(buf); if (! _stricmp(buf, "e") ) 
			return(0);
		_dPuts("Start interval alarm clock, signal alarm\n");

		_itSetInterval(it, uSec);
		if ( setitimer(ITIMER_REAL, &it, &old_it) ) {
					perror("Setitimer()"); return (-2);
		}	
		_dPuts("Sleep for 3 interval alarms, then stop the timing intervals *\n");
		DosSleep(uSec);
		DosSleep(uSec);
		DosSleep(uSec);

		_dPuts("Stop 2 times the timings, second try should warn...\n");
		_itClear(it);
		if ( setitimer(ITIMER_REAL, &it, &old_it) ) {
					perror("Setitimer()"); return (-2);
		}	
		if ( setitimer(ITIMER_REAL, &it, &old_it) ) {
			perror("Setitimer()"); return (-2);
		}	
		while (i++ < MAX_LOOP) {
			_itSetValue(it, uSec / 100);
			_dPuts("Start alarm clock with uSec/100, signal alarm after interval\n");
			fflush(NULL);
			if ( setitimer(ITIMER_REAL, &it, &old_it) ) {
						perror("Setitimer()");
						return (-2);
			}	
			_dPuts(" Now sleeping..."); fflush(NULL);
		DosSleep(uSec /10000);
		}
	 	return(0);
	}
}

