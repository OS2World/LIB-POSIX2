/* mathx.c (emx+gcc) -  additional math function eXtensions.
   $Id: mathx.c,v 1.1 2000/08/18 19:01:57 amai Exp $
 */

#include <math.h>
#include <mathx.h>
#include "math_private.h"

/* On Sun, 7 May 2000 01:09:22 +0200 (MET DST), Stefan Neis wrote:
Try this code: */

#undef __MTH_INL
#define __MTH_INL extern /* these are external library functions */

	__MTH_INL double 
log1p(double x)
{
  register double value, tmp;
  if (x>= - 0.25 && x <= 0.4){
    __asm__("fldln2\n\t" : "=t" (tmp) );
    __asm__("fyl2xp1" : "=t" (value) : "0" (x), "u" (tmp) );
  }
  else{
    __asm__("fldln2\n\t" : "=t" (tmp) );
    __asm__("fyl2x" : "=t" (value) : "0" (1+x), "u" (tmp) );
  }
  return value;
}

	__MTH_INL float
log1pf(float x)
{
  register double value, tmp;
  if (x>= - 0.25 && x <= 0.4){
    __asm__("fldln2\n\t" : "=t" (tmp) );
    __asm__("fyl2xp1" : "=t" (value) : "0" (x), "u" (tmp) );
  }
  else{
    __asm__("fldln2\n\t" : "=t" (tmp) );
    __asm__("fyl2x" : "=t" (value) : "0" (1+x), "u" (tmp) );
  }
  return value;
}


/*    Portions: Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
   Documentation is included on the distribution media as
   Unix-style manual pages that describe the functions and their
   invocation.  The primary documentation for the library functions
   is the book by Moshier, Methods and Programs for Mathematical Functions, 
   Prentice-Hall, 1989.
 */

/*							drand.c
 *
 *	Pseudorandom number generator
 *
 *
 *
 * SYNOPSIS:
 *
 * double y, drand();
 *
 * drand( &y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Yields a random number 1.0 <= y < 2.0.
 *
 * The three-generator congruential algorithm by Brian
 * Wichmann and David Hill (BYTE magazine, March, 1987,
 * pp 127-8) is used. The period, given by them, is
 * 6953607871644.
 *
 * Versions invoked by the different arithmetic compile
 * time options DEC, IBMPC, and MIEEE, produce
 * approximately the same sequences, differing only in the
 * least significant bits of the numbers. The UNK option
 * implements the algorithm as recommended in the BYTE
 * article.  It may be used on all computers. However,
 * the low order bits of a double precision number may
 * not be adequately random, and may vary due to arithmetic
 * implementation details on different computers.
 *
 * The other compile options generate an additional random
 * integer that overwrites the low order bits of the double
 * precision number.  This reduces the period by a factor of
 * two but tends to overcome the problems mentioned.
 *
 */

/*  Three-generator random number algorithm
 * of Brian Wichmann and David Hill
 * BYTE magazine, March, 1987 pp 127-8
 *
 * The period, given by them, is (p-1)(q-1)(r-1)/4 = 6.95e12.
 */

static int sx = 1;
static int sy = 10000;
static int sz = 3000;

static union {
 double d;
 unsigned short s[4];
} unkans;

/* This function implements the three
 * congruential generators.
 */
 
	static int 
__mathx_ranwh(void)
{
int r, s;
/*  sx = sx * 171 mod 30269 */
r = sx/177;
s = sx - 177 * r;
sx = 171 * s - 2 * r;
if( sx < 0 )
	sx += 30269;


/* sy = sy * 172 mod 30307 */
r = sy/176;
s = sy - 176 * r;
sy = 172 * s - 35 * r;
if( sy < 0 )
	sy += 30307;

/* sz = 170 * sz mod 30323 */
r = sz/178;
s = sz - 178 * r;
sz = 170 * s - 63 * r;
if( sz < 0 )
	sz += 30323;
/* The results are in static sx, sy, sz. */
return 0;
}

/*	drand.c
 *
 * Random double precision floating point number between 1 and 2.
 *
 * C callable:
 *	drand( &x );
 */

	extern int 
drand(double *a)
{
unsigned short r;

/* This algorithm of Wichmann and Hill computes a floating point
 * result:
 */
__mathx_ranwh();
unkans.d = sx/30269.0  +  sy/30307.0  +  sz/30323.0;
r = unkans.d;
unkans.d -= r;
unkans.d += 1.0;

/* make a random 16 bit integer
 * to overwrite the least significant word
 * of unkans.
 */
__mathx_ranwh();
r = sx * sy + sz;
unkans.s[0] = r;
*a = unkans.d;
return 0;
}

  void
pvec(double x)
{
  union
  {
    double d;
    unsigned short s[4];
  } u;
  int i;

  u.d = x;
  for (i = 0; i < 4; i++)
    printf ("0x%04x ", u.s[i]);
  printf ("\n");
}

/*
Cephes Math Library Release 2.2:  July, 1992
Copyright 1984, 1987, 1988, 1992 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

/*							polevl.c
 *							p1evl.c
 *
 *	Evaluate polynomial
 *
 *
 *
 * SYNOPSIS:
 *
 * int N;
 * double x, y, coef[N+1], polevl[];
 *
 * y = polevl( x, coef, N );
 *
 *
 *
 * DESCRIPTION:
 *
 * Evaluates polynomial of degree N:
 *
 *                     2          N
 * y  =  C  + C x + C x  +...+ C x
 *        0    1     2          N
 *
 * Coefficients are stored in reverse order:
 *
 * coef[0] = C  , ..., coef[N] = C  .
 *            N                   0
 *
 *  The function p1evl() assumes that coef[N] = 1.0 and is
 * omitted from the array.  Its calling arguments are
 * otherwise the same as polevl().
 *
 *
 * SPEED:
 *
 * In the interest of speed, there are no checks for out
 * of bounds arithmetic.  This routine is used by most of
 * the functions in the library.  Depending on available
 * equipment features, the user may wish to rewrite the
 * program in microcode or assembly language.
 *
 */

	static __inline__ double 
polevl( double x, double *coef, int N )
{
double *p = coef;
register double ans = *p++;
register int i = N;

do
	ans = ans * x  +  *p++;
while( --i );

return( ans );
}

/*							p1evl()	*/
/*                                          N
 * Evaluate polynomial when coefficient of x  is 1.0.
 * Otherwise same as polevl.
 */

	static __inline__ double 
p1evl( double x, double *coef, int N )
{
double *p = coef;
register double ans = x + *p++;
register int i = N - 1;

do
	ans = ans * x  + *p++;
while( --i );

return( ans );
}

/*							polevll.c
 *							p1evll.c
 *
 *	Evaluate polynomial
 *
 *
 *
 * SYNOPSIS:
 *
 * int N;
 * long double x, y, coef[N+1], polevl[];
 *
 * y = polevll( x, coef, N );
 *
 *
 *
 * DESCRIPTION:
 *
 * Evaluates polynomial of degree N:
 *
 *                     2          N
 * y  =  C  + C x + C x  +...+ C x
 *        0    1     2          N
 *
 * Coefficients are stored in reverse order:
 *
 * coef[0] = C  , ..., coef[N] = C  .
 *            N                   0
 *
 *  The function p1evll() assumes that coef[N] = 1.0 and is
 * omitted from the array.  Its calling arguments are
 * otherwise the same as polevll().
 *
 * SPEED:
 *
 * In the interest of speed, there are no checks for out
 * of bounds arithmetic.  This routine is used by most of
 * the functions in the library.  Depending on available
 * equipment features, the user may wish to rewrite the
 * program in microcode or assembly language.
 *
 */

/* Polynomial evaluator:
 *  P[0] x^n  +  P[1] x^(n-1)  +  ...  +  P[n]
 */
	static __inline__ long double
polevll( long double x, long double *p, register int n )
{
register long double y;
register long double *P = p;

y = *P++;
do
	{
	y = y * x + *P++;
	}
while( --n );
return(y);
}



/* Polynomial evaluator:
 *  x^n  +  P[0] x^(n-1)  +  P[1] x^(n-2)  +  ...  +  P[n]
 */
	static __inline__ long double
p1evll( long double x, long double *p, register int n )
{
register long double y;
register long double *P = p;

n -= 1;
y = x + *P++;
do
	{
	y = y * x + *P++;
	}
while( --n );
return( y );
}

/*							exp10.c
 *
 *	Base 10 exponential function
 *      (Common antilogarithm)
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, exp10();
 *
 * y = exp10( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns 10 raised to the x power.
 *
 * Range reduction is accomplished by expressing the argument
 * as 10**x = 2**n 10**f, with |f| < 0.5 log10(2).
 * The Pade' form
 *
 *    1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 *
 * is used to approximate 10**f.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -307,+307    30000       2.2e-16     5.5e-17
 * Test result from an earlier version (2.1):
 *    DEC       -38,+38     70000       3.1e-17     7.0e-18
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * exp10 underflow    x < -MAXL10        0.0
 * exp10 overflow     x > MAXL10       MAXNUM
 *
 * DEC arithmetic: MAXL10 = 38.230809449325611792.
 * IEEE arithmetic: MAXL10 = 308.2547155599167.
 *
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#define MAXLOG10 308.2547155599167 
/*static long double LOG102 = 3.0102999566398119521373889e-1L;*/
#define LOG210  3.3219280948873623478703L
#define LG102A  3.01025390625e-1L
#define LG102B  4.6050389811952137388947e-6L

static double P1[]= {
 4.09962519798587023075E-2,
 1.17452732554344059015E1,
 4.06717289936872725516E2,
 2.39423741207388267439E3,
};
static double Q1[] = {
/* 1.00000000000000000000E0,*/
 8.50936160849306532625E1,
 1.27209271178345121210E3,
 2.07960819286001865907E3,
};

static long double P2[] = {
 3.1341179396892496811523E1L,
 4.5618283154904699073999E3L,
 1.3433113468542797218610E5L,
 7.6025447914440301593592E5L,
};
static long double Q2[] = {
/* 1.0000000000000000000000E0,*/
 4.7705440288425157637739E2L,
 2.9732606548049614870598E4L,
 4.0843697951001026189583E5L,
 6.6034865026929015925608E5L,
};

	double
exp10(double x)
{
double px, xx;
short n;

if( __isnan(x) )
	return(x);

if( x > MAXLOG10 )
	{
	return( INFINITY );
	}

if( x < -MAXLOG10 )	/* Would like to use MINLOG but can't */
	{
	return(0.0);
	}

/* Express 10**x = 10**g 2**n
 *   = 10**g 10**( n log10(2) )
 *   = 10**( g + n log10(2) )
 */
px = floor( LOG210 * x + 0.5 );
n = px;
x -= px * LG102A;
x -= px * LG102B;

/* rational approximation for exponential
 * of the fractional part:
 * 10**x = 1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 */
xx = x * x;
px = x * polevl( xx, P1, 3 );
x =  px/( p1evl( xx, Q1, 3 ) - px );
x = 1.0 + ldexp( x, 1 );

/* multiply by power of 2 */
x = ldexp( x, n );

return(x);
}

/*							exp10l.c
 *
 *	Base 10 exponential function, long double precision
 *      (Common antilogarithm)
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, exp10l()
 *
 * y = exp10l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns 10 raised to the x power.
 *
 * Range reduction is accomplished by expressing the argument
 * as 10**x = 2**n 10**f, with |f| < 0.5 log10(2).
 * The Pade' form
 *
 *     1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 *
 * is used to approximate 10**f.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      +-4900      30000       1.0e-19     2.7e-20
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * exp10l underflow    x < -MAXL10        0.0
 * exp10l overflow     x > MAXL10       MAXNUM
 *
 * IEEE arithmetic: MAXL10 = 4932.0754489586679023819
 *
 */
#define MAXLOG10L 4932.0754489586679023819L

	long double 
exp10l(long double x)
{
long double px, xx;
short n;

if( __isnanl(x) )
	return(x);
if( x > MAXLOG10L )
	{
	return( INFINITYL );
	}

if( x < -MAXLOG10L )	/* Would like to use MINLOG but can't */
	{
	return(0.0L);
	}

/* Express 10**x = 10**g 2**n
 *   = 10**g 10**( n log10(2) )
 *   = 10**( g + n log10(2) )
 */
px = floorl( LOG210 * x + 0.5L );
n = px;
x -= px * LG102A;
x -= px * LG102B;

/* rational approximation for exponential
 * of the fractional part:
 * 10**x = 1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 */
xx = x * x;
px = x * polevll( xx, P2, 3 );
x =  px/( p1evll( xx, Q2, 4 ) - px );
x = 1.0L + ldexpl( x, 1 );

/* multiply by power of 2 */
x = ldexpl( x, n );
return(x);
}


/* Portions: 
   Copyright (C) 1991, 92, 93, 95, 96, 97, 98 Free Software 
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
 *
 *	Extensions to ISO C Standard: 4.5 MATHEMATICS	<math.h>
 */

/* BSD Functions */
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
		if (x == -1.0L) return(- _INFINITYL);
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
  int p = (int)x;

  if (x ==(double)p)
    return ldexp (1.0, p);

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

