/* sys/timext.h (libext/gcc) $Id: itimer.h,v 1.6 2000/08/06 20:30:27 amai Exp $ */

/* (to be included in <sys/time.h>)
     -- Crude itimer implementation --- Lots of functionality missing.
     ITIMER_VIRTUAL  and ITIMER_PROF are not yet implemented, they just
     measure real time only, Fix me !
   This code is released into Public Domain.
    Alexander Mai <st002279@hrzpub.tu-darmstadt.de>
    A. Hanses <Arnd.H.Hanses@rz.ruhr-uni-bochum.de>
    */
#ifndef _SYS_ITIMER_H
#define _SYS_ITIMER_H

#if 0
#ifndef __MT__
# warning You must compile with '-Zmt -Zbsd-signals' !
# define __MT__
#endif
#endif

#if !defined (_TIMEVAL)  	/* need struct timeval */
# warning 'sys/time.h' must be included before 'sys/itimer.h'
# include <sys/time.h>
#endif

#include <signal.h>

#if !defined (ITIMER_REAL)
/* finest measurable interval: It is physically impossible to measure a time 
 * window as short as this; a constant value. Adapt to hardware! */

#ifndef _IT_TIMING_GRANULARITY 
#define _IT_TIMING_GRANULARITY 1000UL /* (u-secs) */
#endif

#define ITIMER_REAL     (const unsigned int)0

/* The others may be a bit hard to implement. But perhaps
 * for some purposes it will be sufficient to just use
 * (copy) the ITIMER_REAL capabilities (code)
 */
#define ITIMER_VIRTUAL  (const unsigned int)1
#define ITIMER_PROF     (const unsigned int)2

/* Convenience macros for operations on timevals:

    Convert 'struct timeval' to m-sec (32-bit signed) */
#define _tv2mSec(x) ( ( (x).tv_sec * 1000L ) + ( (x).tv_usec / 1000L ) ) \
   - (_IT_TIMING_GRANULARITY / 1000L) 

#define	_itOneIsSet(x)   ((x).it_value.tv_sec    || (x).it_value.tv_usec )
#define	_itIntvlIsSet(x) ((x).it_interval.tv_sec || (x).it_interval.tv_usec )

#define	_itClear(x) (x).it_value.tv_sec=(x).it_value.tv_usec=0L

#define	_itSetValue(it,x)	{if(x>=1000000L){ /* more than 1 second: */ \
it.it_value.tv_sec = x / 1000000L; it.it_value.tv_usec = x % 1000000L;        \
 }else it.it_value.tv_usec = x;it.it_interval.tv_sec=it.it_interval.tv_usec=0L;}

#define	_itSetInterval(it,x)	{if (x >= 1000000L){ /* more than 1 second: */ \
it.it_value.tv_sec = x / 1000000L; it.it_value.tv_usec = x % 1000000L;        \
 } else it.it_value.tv_usec = x; if (x >= 1000000L){ /* more than 1 second: */ \
it.it_interval.tv_sec = x / 1000000L; it.it_interval.tv_usec = x % 1000000L;  \
 } else it.it_interval.tv_usec = x;}
#define	_itimeradd(a,b,result) do{(result)->tv_sec =(a)->tv_sec +(b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;			      \
    if ((result)->tv_usec >= 1000000L){++(result)->tv_sec;		      \
	(result)->tv_usec -= 1000000L;}} while (0)
#define	_itimersub(a,b,result) do{(result)->tv_sec =(a)->tv_sec -(b)->tv_sec; \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {--(result)->tv_sec;			      \
      (result)->tv_usec += 1000000L;}} while (0)

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

extern  int _getitimer(const unsigned int,struct itimerval*);
extern  int _setitimer(const unsigned int,const struct itimerval*,struct itimerval*);
extern  int getitimer(const unsigned,struct itimerval*);
extern  int setitimer(const unsigned,const struct itimerval *,struct itimerval *);

#if defined (__cplusplus)
}
#endif

/* Machine-dependent inline versions */
#if (!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))

/* no inlining; use library functions */

#else

/* Get Remaining Time on Timer 'which'  (exported API):
    Errors: 
        EFAULT - Argument no valid pointer to struct itimerval 
        EINVAL - Invalid timer selection argument
        ENOSYS - Functionality not implemented on this system
        */
	extern __inline__ int 
getitimer(const unsigned which,struct itimerval*it) {
	return (_getitimer(which,it));
}
/* Set the interrupt timer 'which (exported API):
    Errors: 
        EFAULT - Argument no valid pointer to struct itimerval 
        EINVAL - Invalid timer selection argument
        ENOSYS - Functionality not implemented on this system
         */
	extern __inline__ int
setitimer(const unsigned which,const struct itimerval*it,struct itimerval*oit) { 
	return (_setitimer(which,it,oit));
}
#endif /* !__GNUC__ etc. */
#endif /* !defined (ITIMER_REAL)     */

#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* _SYS_ITIMER_H */
