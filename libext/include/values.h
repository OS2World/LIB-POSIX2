/* values.h (libext + gcc): Old compatibility names */
/* $Id: values.h,v 1.4 2000/04/03 18:18:24 veit Exp $ */

#ifndef	_VALUES_H
#define	_VALUES_H

/* This interface is deprecated by ANSI C; use <limits.h> and/or <float.h>.
	 */

#include <limits.h>

#define CHARBITS	CHAR_BIT
#define BITSPERBYTE	CHAR_BIT
#define __BITS(t)	(sizeof(t) * CHAR_BIT)
#define SHORTBITS	__BITS(short)
#define INTBITS		__BITS(int)
#define LONGBITS	__BITS(long)
#define PTRBITS		__BITS(char*)
#define DOUBLEBITS	__BITS(double)
#define FLOATBITS	__BITS(float)

#define MINSHORT	SHRT_MIN
#define	MININT		INT_MIN
#define	MINLONG		LONG_MIN

#define	MAXSHORT	SHRT_MAX
#define	MAXINT		INT_MAX
#define	MAXLONG		LONG_MAX

#define HIBITS		MINSHORT
#define HIBITL		MINLONG

#include <float.h>

#define	MAXDOUBLE	DBL_MAX
#define	MAXFLOAT	FLT_MAX
#define	MINDOUBLE	DBL_MIN
#define	MINFLOAT	FLT_MIN
#define	DMINEXP		DBL_MIN_EXP
#define	FMINEXP		FLT_MIN_EXP
#define	DMAXEXP		DBL_MAX_EXP
#define	FMAXEXP		FLT_MAX_EXP

#endif	/* values.h */
