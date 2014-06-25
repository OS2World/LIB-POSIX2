/* gcc+libExt: -- Unix Style Time Data Structures and Declarations
 * $Header: /cvsroot/posix2/posix2/libext/include/sys/time.h,v 1.5 2000/08/26 09:21:10 amai Exp $
 */
/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)time.h	8.2 (Berkeley) 7/10/94
 */

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <time.h>
#include <sys/types.h>

#if defined (__cplusplus)
extern "C" {
#endif

#if !defined (_TM)
#define _TM
struct tm         /* cf. <emx/thread.h> */
{
  int tm_sec;     /* 0..59 */
  int tm_min;     /* 0..59 */
  int tm_hour;    /* 0..23 */
  int tm_mday;    /* 1..31 */
  int tm_mon;     /* 0..11 */
  int tm_year;    /* 0(:=1900).. */
  int tm_wday;    /* 0..6 */
  int tm_yday;    /* 0..365 */
  int tm_isdst;   /* 0 */
};
#endif

#if !defined (_TIMEVAL)
#define _TIMEVAL

struct timeval { /* Structure returned by gettimeofday(2) system call. */
  long tv_sec;		/* seconds */
  long tv_usec;		/* and microseconds */
};

struct timespec { /* Structure defined by POSIX.1b to be like a timeval. */
  time_t tv_sec;		/* seconds */
  long   tv_nsec;		/* and nanoseconds */
};
struct clockinfo {	/* Getkerninfo clock information structure */
	int	hz;		/* clock frequency */
	int	tick;		/* micro-seconds per hz tick */
	int	tickadj;	/* clock skew rate for adjtime() */
	int	stathz;		/* statistics clock frequency */
	int	profhz;		/* profiling clock frequency */
};
struct itimerval { 	/* for *itimer() interfaces */
    struct timeval it_interval;    /* next value when reset */
    struct timeval it_value;    /* current expiration value */
};

#define	TIMEVAL_TO_TIMESPEC(tv, ts) {					\
	(ts)->tv_sec = (tv)->tv_sec;					\
	(ts)->tv_nsec = (tv)->tv_usec * 1000;				\
}
#define	TIMESPEC_TO_TIMEVAL(tv, ts) {					\
	(tv)->tv_sec = (ts)->tv_sec;					\
	(tv)->tv_usec = (ts)->tv_nsec / 1000;				\
}
#endif

#if !defined (_TIMEZONE)
#define _TIMEZONE
struct timezone {
  int	tz_minuteswest;	/* minutes west of Greenwich */
  int	tz_dsttime;	/* type of dst correction */
};
#define	DST_NONE	0	/* not on dst */
#define	DST_USA		1	/* USA style dst */
#define	DST_AUST	2	/* Australian style dst */
#define	DST_WET		3	/* Western European dst */
#define	DST_MET		4	/* Middle European dst */
#define	DST_EET		5	/* Eastern European dst */
#define	DST_CAN		6	/* Canada */
#endif /* !(_TIMEZONE) */

#if defined(_KERNEL) || defined(_STANDALONE)
#define _TIME_T_MAX 0xffffffffUL
struct _tzinfo {
  char tzname[4];
  char dstzname[4];
  int tz, dst, shift;
  int sm, sw, sd, st;
  int em, ew, ed, et;
};
struct _dstswitch {
  time_t time;                  /* UTC */
  int shift;
};
extern int _tzset_flag;
#endif

/* Operations on timevals. */
#define	timerclear(tvp)		(tvp)->tv_sec = (tvp)->tv_usec = 0
#define	timerisset(tvp)		((tvp)->tv_sec || (tvp)->tv_usec)
#define	timercmp(tvp, uvp, cmp)						\
	(((tvp)->tv_sec == (uvp)->tv_sec) ?				\
	    ((tvp)->tv_usec cmp (uvp)->tv_usec) :			\
	    ((tvp)->tv_sec cmp (uvp)->tv_sec))
#define	timeradd(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec + (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec + (uvp)->tv_usec;	\
		if ((vvp)->tv_usec >= 1000000) {			\
			(vvp)->tv_sec++;				\
			(vvp)->tv_usec -= 1000000;			\
		}							\
	} while (0)
#define	timersub(tvp, uvp, vvp)						\
	do {								\
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
		if ((vvp)->tv_usec < 0) {				\
			(vvp)->tv_sec--;				\
			(vvp)->tv_usec += 1000000;			\
		}							\
	} while (0)

/* Operations on timespecs. */
#define	timespecclear(tsp)		(tsp)->tv_sec = (tsp)->tv_nsec = 0
#define	timespecisset(tsp)		((tsp)->tv_sec || (tsp)->tv_nsec)
#define	timespeccmp(tsp, usp, cmp)					\
	(((tsp)->tv_sec == (usp)->tv_sec) ?				\
	    ((tsp)->tv_nsec cmp (usp)->tv_nsec) :			\
	    ((tsp)->tv_sec cmp (usp)->tv_sec))
#define	timespecadd(tsp, usp, vsp)					\
	do {								\
		(vsp)->tv_sec = (tsp)->tv_sec + (usp)->tv_sec;		\
		(vsp)->tv_nsec = (tsp)->tv_nsec + (usp)->tv_nsec;	\
		if ((vsp)->tv_nsec >= 1000000000L) {			\
			(vsp)->tv_sec++;				\
			(vsp)->tv_nsec -= 1000000000L;			\
		}							\
	} while (0)
#define	timespecsub(tsp, usp, vsp)					\
	do {								\
		(vsp)->tv_sec = (tsp)->tv_sec - (usp)->tv_sec;		\
		(vsp)->tv_nsec = (tsp)->tv_nsec - (usp)->tv_nsec;	\
		if ((vsp)->tv_nsec < 0) {				\
			(vsp)->tv_sec--;				\
			(vsp)->tv_nsec += 1000000000L;			\
		}							\
	} while (0)

extern int utimes(__const__ char*,__const__ struct timeval*);
extern int gettimeofday(struct timeval*,struct timezone*);
extern int settimeofday (__const__ struct timeval*, __const__ struct timezone*);


extern int _utimes(__const__ char*, __const__ struct timeval*);
extern int _gettimeofday(struct timeval*, struct timezone*);
extern int _settimeofday(__const__ struct timeval*, __const__ struct timezone*);
extern struct clockinfo*_getClockInfo(void);
extern struct timeval*_bootTime(void);  /* set bootime and return it */


#if defined (__cplusplus)
}
#endif

#include <sys/itimer.h>

#endif /* not _SYS_TIME_H */
