/* mathx.h (libExt+gcc) - Declarations and definitions for additional 
 * math eXtensions.
 * $Id: mathx.h,v 1.9 2000/08/09 14:51:18 amai Exp $
 * Extensions to ISO C Standard: 4.5 MATHEMATICS	<math.h>
 */

/* Parts from: Cephes Math Library Release 2.3:  March, 1995
      Copyright 1984, 1995 by Stephen L. Moshier
   Documentation is included on the distribution media as
   Unix-style manual pages that describe the functions and their
   invocation.  The primary documentation for the library functions
   is the book by Moshier, Methods and Programs for Mathematical Functions, 
   Prentice-Hall, 1989.
   Parts: Copyright (C) 1991, 92, 93, 95, 96, 97, 98 Free Software 
   Foundation, Inc. This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 
   330, Boston, MA 02111-1307, USA.
 */
#ifndef _SYS_MATHX_H__
#define _SYS_MATHX_H__

/* #define GNUFNS to use GNU version of some fns defined here */

#if ( !defined (__STRICT_ANSI__) && !defined (_POSIX_SOURCE) && \
 !defined (__NO_C9X) ) || defined (_WITH_UNDERSCORE)

/* Machine-dependent inline versions */
#if (!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
# define __MTH_INL extern
#else
# define __MTH_INL extern __inline__
#endif

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
# endif
extern "C" {
#endif

__MTH_INL double expm1(double);
__MTH_INL double log1p(double);
__MTH_INL long double log1pl(long double);
__MTH_INL double asinh(double);
__MTH_INL long double asinhl(long double);
__MTH_INL double acosh(double);
__MTH_INL long double acoshl(long double);
__MTH_INL double atanh(double);
__MTH_INL long double atanhl(long double);
__MTH_INL double coshm1(double);
__MTH_INL double acosh1p(double);
__MTH_INL double logb(double);
__MTH_INL double drem(double, double);
__MTH_INL void sincos(double, double *, double *);
__MTH_INL double pow2(double);
__MTH_INL double hypot(double,double);

#ifdef	__cplusplus
}
#endif

#endif /* ANSI */

/* Get machine-dependent HUGE_VAL value (returned on overflow).
	On all IEEE754 machines, this is +Infinity.  
   SVID wants HUGE instead of infinity.
   Get machine-dependent NAN value (returned for some domain errors).  */
#include <math.h>

/* X/Open wants another strange constant. */
#include <ieee.h>  /* MAXFLOAT */
#include <float.h> /* Some useful (ANSI, BSD) constants. */
#include <limits.h>

#if !defined (__STRICT_ANSI__) && !defined (_POSIX_SOURCE) && \
 !defined (__NO_C9X)

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension. (emx gcc version)
 */
#define M_El             2.71828182845904523536L	/* e */
#define M_LOG2El         1.44269504088896340736L	/* log 2e */
#define M_LOG10El        0.434294481903251827651L	/* log 10e */
#define M_LN2l           0.693147180559945309422L	/* log e2 */
#define M_LN10l          2.302585092994045684018L	/* log e10 */

#define M_PIl            3.14159265358979323846L
#define M_PI_2l          1.57079632679489661923L	/* pi/2 */
#define M_PI_4l          0.785398163397448309616L	/* pi/4 */
#define M_1_PIl          0.318309886183790671538L	/* 1/pi */
#define M_2_PIl          0.636619772367581343076L	/* 2/pi */
#define M_2_SQRTPIl      1.12837916709551257390L	/* 2/sqrt(pi) */
#define M_SQRT2l         1.41421356237309504880L	/* sqrt(2) */
#define M_SQRT1_2l       0.707106781186547524401L	/* 1/sqrt(2) */

/* Some Useful Constants From Cephes Math Library */
#define _MAXNUML   LDBL_MAX
#define _MACHEP    DBL_EPSILON    
#define _MACHEPL   LDBL_EPSILON    
#define _MAXLOGL   1.1356523406294143949700E+04L  /* logl(MAXNUML) */
#define _MINLOGL (-1.1355137111933024058900E+04L) /* logl(LDBL_MIN) */
#define _LOGE2     6.9314718055994530941723E-1
#define _LOGE2L    6.9314718055994530941723E-1L
#define _LOG2EL    1.4426950408889634073599E0L
#define _PIL       3.1415926535897932384626L
#define _PIO2      1.5707963267948966192313
#define _PIO2L     1.5707963267948966192313L
#define _PIO4L     7.8539816339744830961566E-1L
#define _NEGZERO   (-0.0)
#define _NEGZEROL  (-0.0L)

#define _sgn(x)  (x == 0.0  ? 0.0  : (x > 0.0  ? 1.0  : -1.0 ))
#define _sgnl(x) (x == 0.0L ? 0.0L : (x > 0.0L ? 1.0L : -1.0L))
#define _sgn1(x)  (x >= 0.0  ? 1.0  : -1.0 )
#define _sgn1l(x) (x >= 0.0L ? 1.0L : -1.0L)

int _dprec(void);
int _sprec(void);
int _ldprec(void);
int _dprec(void);

double exp10(double);
long double exp10l(long double);

#if !(!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))

#include <errno.h>

/* BSD Functions */
	__MTH_INL double
drem(double x, double y) {
  register double value;
  __asm__ __volatile__("1: fprem1\n\t" "fstsw %%ax\n\t" "sahf\n\t" "jp 1b"
     : "=t" (value) : "0" (x), "u" (y) : "ax", "cc");
  return value;
}
	__MTH_INL double
expm1(double x) {
 register double value, exponent, temp;
  __asm__ __volatile__
    ("fldl2e			# e^x - 1 = 2^(x * log2(e)) - 1\n\t"
     "fmul	%%st(1)		# x * log2(e)\n\t"
     "fstl	%%st(1)\n\t"
     "frndint			# int(x * log2(e))\n\t"
     "fxch\n\t"
     "fsub	%%st(1)		# fract(x * log2(e))\n\t"
     "f2xm1			# 2^(fract(x * log2(e))) - 1\n\t"
     "fscale			# 2^(x * log2(e)) - 2^(int(x * log2(e)))\n\t"
     : "=t" (value), "=u" (exponent) : "0" (x));
  __asm__ __volatile__
    ("fscale			# 2^int(x * log2(e))\n\t"
     : "=t" (temp) : "0" (1.0), "u" (exponent));
  temp -= 1.0;
  return temp + value;
}

/* Optimized versions for some non-standardized functions.  */
	__MTH_INL double
hypot(double x, double y) {
  return sqrt(x * x + y * y);
}
	__MTH_INL double
log1p(double x) {
 register double value;
  if (fabs (x) >= 1.0 - 0.5 * M_SQRT2)
    value = log(1.0 + x);
  else
    __asm__ __volatile__
      ("fldln2\n\t"
       "fxch\n\t"
       "fyl2xp1"
       : "=t" (value) : "0" (x));
  return value;
}
	__MTH_INL long double
log1pl(long double x){
 register long double value;
  if (fabsl(x) >= 1.0L - 0.5L * M_SQRT2l)
    value = logl(1.0 + x);
  else
    __asm__ __volatile__
      ("fldln2\n\t"
       "fxch\n\t"
       "fyl2xp1"
       : "=t" (value) : "0" (x));
  return value;
}
	__MTH_INL double
asinh(double x) {
 register __const__ double y = fabs(x);
	if (__isnan(x)) return x;
	if (x == 0.0) return x;
	if (!(__isfinite(x))) return x;
	if (y > 1.0e10) return log(x) + _LOGE2;
#ifdef GNUFNS  /* This is the GNU original and it is wrong!!! */
/*   return log1p ((y * y / (sqrt (y * y + 1.0) + 1.0) + y)
		* _sgn1l(x));
 */
	return log1p(y +(y * y/(sqrt(y * y + 1.0)+ 1.0)))* _sgn1(x);
#else
	return   log(x +        sqrt(x * x + 1.0)       );
#endif
}
	__MTH_INL double
acosh(double x) {
	if (__isnan(x)) return x;
	if (x > 1.0e10) {
		if (x == INFINITY) return INFINITY;
		return(log(x) + _LOGE2); }
	return log(x + sqrt(x - 1.0) * sqrt(x + 1.0));
}
	__MTH_INL double
atanh(double x) {
  register __const__ double y = fabs(x);
	if (x == 0.0) return x;
	if (y >= 1.0) {
		if (x == 1.0) return INFINITY;
		if (x == -1.0) return -INFINITY;
	}
	if (y < 1.0e-8) return x;
#ifdef GNUFNS
	return -0.5 * log1p(-(y + y)/(1.0 + y)) * _sgn1(x);
#else
	return  0.5 * log  ( (1.0 + x)/(1.0 - x) );
#endif
}
		__MTH_INL long double
asinhl(long double x) {
 register __const__ long double y = fabsl(x);
	if (__isnanl(x)) return x;
	if (x == 0.0L) return x;
	if (!(__isfinitel(x))) return x;
	if (y > 1.0e10L) return logl(x) + _LOGE2L;
#ifdef GNUFNS  
	return log1pl( (y * y /(sqrtl(y * y + 1.0L) + 1.0L) + y) )
			* _sgn1l(x); 	/* This is corrected */
#else
	return logl( x + sqrtl(x * x + 1.0L));
#endif
}
	__MTH_INL long double
acoshl(long double x) {
	if (__isnanl(x)) return x;
	if (x > 1.0e10L) {
		if (x == _INFINITYL) return _INFINITYL; 
		return logl(x) + _LOGE2L; }
        return logl(x + sqrtl(x - 1.0L) * sqrtl(x + 1.0L));
}
	__MTH_INL long double
atanhl(long double x) {
  register __const__ long double y = fabsl(x);
	if (x == 0.0L) return x;
	if (y >= 1.0L) {
		if (x == 1.0L) return _INFINITYL;
		if (x == -1.0L) return -_INFINITYL;
	}
	if (y < 1.0e-8L) return x;
#ifdef GNUFNS
	return -0.5L * log1pl(-(y + y)/(1.0L + y)) * _sgn1(x);
#else
	return  0.5L *   logl( (1.0L + x)/(1.0L - x) );
#endif
}
	__MTH_INL double
coshm1(double x) {
  register __const__ double __exm1 = expm1 (fabs (x));
  return 0.5 * (__exm1 / (__exm1 + 1.0)) * __exm1;
}
	__MTH_INL double
acosh1p(double x) {
  return log1p(x + sqrt (x) * sqrt (x + 2.0));
}
	__MTH_INL double
logb(double x) {
 register double value, __junk;
  __asm__ __volatile__
    ("fxtract\n\t"
     : "=t" (__junk), "=u" (value) : "0" (x));
  return value;
}
	__MTH_INL void
sincos(double x, double *__sinx, double *__cosx) {
 register double __cosr, __sinr;
  __asm__ __volatile__ ("fsincos"
     : "=t" (__cosr), "=u" (__sinr) : "0" (x));
  *__sinx = __sinr;
  *__cosx = __cosr;
}
	__MTH_INL double
pow2(double x) {
  register double value, exponent;
  int __p = (int) x;

  if (x ==(double) __p)
    return ldexp (1.0, __p);

  __asm__ __volatile__
    ("fldl	%%st(0)\n\t"
     "frndint			# int(x)\n\t"
     "fxch\n\t"
     "fsub	%%st(1)		# fract(x)\n\t"
     "f2xm1			# 2^(fract(x)) - 1\n\t"
     : "=t" (value), "=u" (exponent) : "0" (x));
  value += 1.0;
  __asm__ __volatile__
    ("fscale"
     : "=t" (value) : "0" (value), "u" (exponent));

  return value;
}

#endif /* __NO_MATH_INLINES */

/* support for lgamma's signgam */

extern int  __wlog_signGam;
#ifndef signgam 
#define signgam __wlog_signGam
#endif

#endif /* _POSIX_SOURCE */


#ifdef _DEBUG
# include <sys/debugx.h>
#endif

#endif /* _SYS_MATHX_H__*/
