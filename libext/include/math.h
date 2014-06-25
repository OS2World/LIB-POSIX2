/* math.h - (libExt+gcc)
 * $Id: math.h,v 1.9 2000/07/06 07:04:36 veit Exp $
 * Portions: @(#)fdlibm.h 5.1 93/09/24
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems,Inc. All rights reserved.
 *
 * Developed at SunPro,a Sun Microsystems,Inc. business.
 * Permission to use,copy,modify,and distribute this
 * software is freely granted,provided that this notice 
 * is preserved.
 * ====================================================
 */
#ifndef _MATH_H
#define _MATH_H

#if defined (__cplusplus)
# define __inline__ inline
extern "C" {
#endif

#if !defined(__STRICT_ANSI__) && !defined(_POSIX_SOURCE)
				/* XOPEN/SVID require this in math.h */
# include <float.h>
# ifndef	MAXFLOAT
#  define	MAXFLOAT	((float)3.40282346638528860e+38)
# endif
extern int signgam; 	/* Take care: Not thread-local,but global! */

# if !defined(_XOPEN_SOURCE)
#  define	HUGE		MAXFLOAT

/* if global variable _LIB_VERSION is not desirable,one may 
 * change the following to be a constant by: 
 *	#define _LIB_VERSION_TYPE const enum version
 * In that case,after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time,it cannot be modified
 * in the middle of a program
 */ 
#  define _LIB_VERSION_TYPE enum __fdlibmVersionEnum
#  define _LIB_VERSION __fdlibm_SLibVersion

#  define _IEEE_  __fdlibm_IEEE_
#  define _SVID_  __fdlibm_SVID_
#  define _XOPEN_ __fdlibm_XOPEN_
#  define _POSIX_ __fdlibm_POSIX_

_LIB_VERSION_TYPE {_IEEE_=(-1),_SVID_,_XOPEN_,_POSIX_};
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#  define 	X_TLOSS 	1.41484755040568800000e+16  /* pi*2**52 */

#  define	DOMAIN		1
#  define	SING		2
#  define	OVERFLOW	3
#  define	UNDERFLOW	4
#  define	TLOSS		5
#  define	PLOSS		6

# endif /* !_XOPEN_SOURCE */
#endif /* !_ANSI && !_POSIX_SOURCE */

/* ANSI/POSIX */
#define HUGE_VAL  ((double)(1.0/0.0))

extern int    abs(int);
extern long   labs(long);
extern double acos(double);
extern double asin(double);
extern double atan(double);
extern double atan2(double,double);
extern double ceil(double);
extern double cos(double);
extern double cosh(double);
extern double exp(double);
extern double fabs(double);
extern double floor(double);
extern double fmod(double,double);
extern double frexp(double,int*);
extern double hypot(double,double);
#undef isnan(x)
extern int    isnan(double);
extern double ldexp(double,int);
extern double log(double);
extern double log10(double);
extern double modf(double,double*);
extern double pow(double,double);
extern double rint(double);
extern double sin(double);
extern double sinh(double);
extern double sqrt(double);
extern double tan(double);
extern double tanh(double);
extern double trunc(double);

/* float versions of ANSI/POSIX functions */
extern float acosf(float);
extern float asinf(float);
extern float atanf(float);
extern float atan2f(float,float);
extern float cosf(float);
extern float sinf(float);
extern float tanf(float);

extern float coshf(float);
extern float sinhf(float);
extern float tanhf(float);

extern float expf(float);
extern float frexpf(float,int *);
extern float ldexpf(float,int);
extern float logf(float);
extern float log10f(float);
extern float modff(float,float *);

extern float powf(float,float);
extern float sqrtf(float);

extern float ceilf(float);
extern float fabsf(float);
extern float floorf(float);
extern float fmodf(float,float);

extern float erff(float);
extern float erfcf(float);
extern float gammaf(float);
extern float hypotf(float,float);
extern int isinff(float);
#undef isnan(x)
extern int isnanf(float);
extern int finitef(float);
extern float j0f(float);
extern float j1f(float);
extern float jnf(int,float);
extern float lgammaf(float);
extern float y0f(float);
extern float y1f(float);
extern float ynf(int,float);

extern float acoshf(float);
extern float asinhf(float);
extern float atanhf(float);
extern float cbrtf(float);
extern float logbf(float);
extern float nextafterf(float,float);
extern float remainderf(float,float);
extern float scalbf(float,float);

#if !defined (__NO_C99)

#define HUGE_VALF       ((float)(1.0F/0.0F))
#define HUGE_VALL       ((long double)(1.0LL/0.0LL))
#define INFINITY        HUGE_VAL
#define NAN             ((double)(1.0/0.0 - 1.0/0.0))
#define NANF            ((float)(1.0F/0.0F - 1.0F/0.0F))
#define NANL            ((long double)(1.0LL/0.0LL - 1.0LL/0.0LL))

#define DECIMAL_DIG     21

#define FP_ZERO         0
#define FP_SUBNORMAL    1
#define FP_NORMAL       2
#define FP_INFINITE     3
#define FP_NAN          4

#define fpclassify(x) \
    ((sizeof(x) == sizeof(float))  ? __fpclassifyf(x) \
   : (sizeof(x) == sizeof(double)) ? __fpclassify(x) \
   :                                   __fpclassifyl(x))

#define signbit(x) \
    ((sizeof(x) == sizeof(float))  ? __signbitf(x) \
   : (sizeof(x) == sizeof(double)) ? __signbit(x) \
   :                                   __signbitl(x))

#define isfinite(x) \
    ((sizeof(x) == sizeof(float))  ? __isfinitef(x) \
   : (sizeof(x) == sizeof(double)) ? __isfinite(x) \
   :                                   __isfinitel(x))

#define isnormal(x) \
    ((sizeof(x) == sizeof(float))  ? __isnormalf(x) \
   : (sizeof(x) == sizeof(double)) ? __isnormal(x) \
   :                                   __isnormall(x))

#define isnan(x) \
    ((sizeof(x) == sizeof(float))  ? __isnanf(x) \
   : (sizeof(x) == sizeof(double)) ? __isnan(x) \
   :                                 __isnanl(x))

extern float copysignf(float,float);
extern double copysign(double,double);
extern long double copysignl(long double,long double);

extern float nextafterf(float,float);
extern double nextafter(double,double);
extern long double nextafterl(long double,long double);

extern long double acosl(long double);
extern long double asinl(long double);
extern long double atanl(long double);
extern long double atan2l(long double,long double);
extern long double ceill(long double);
extern long double cosl(long double);
extern long double coshl(long double);
extern long double expl(long double);
extern long double fabsl(long double);
extern long double floorl(long double);
extern long double fmodl(long double,long double);
extern long double frexpl(long double,int *);
extern long double hypotl(long double,long double);
extern long double ldexpl(long double,int);
extern long double logl(long double);
extern long double log10l(long double);
extern long double modfl(long double,long double*);
extern long double powl(long double,long double);
extern long double rintl(long double);
extern long double sinl(long double);
extern long double sinhl(long double);
extern long double sqrtl(long double);
extern long double tanl(long double);
extern long double tanhl(long double);
extern long double truncl(long double);

int __fpclassify(double);
int __fpclassifyf (float);
int __fpclassifyl(long double);

int __signbit(double);
int __signbitf (float);
int __signbitl(long double);

int __isfinite(double);
int __isfinitef (float);
int __isfinitel(long double);

int __isnormal(double);
int __isnormalf (float);
int __isnormall(long double);

int __isnan(double);
int __isnanf (float);
int __isnanl(long double);

#endif

#if !defined (__STRICT_ANSI__)

extern double atof(__const__ char *);
extern double cbrt(double);

#if !defined (_ABS)		/* see also stdlib.h */
#define _ABS
/* Machine-dependent inline versions */
#if (!__GNUC__ || __GNUC__ < 2 || (__GNUC__ == 2 && __GNUC_MINOR__ <= 7))
extern int abs(int);
extern long labs(long);
#else
extern __inline__ int abs(int _n) { return (_n < 0 ? -_n : _n); }
extern __inline__ long labs(long _n) { return (_n < 0 ? -_n : _n); }
#endif
#endif

#if !defined(_POSIX_SOURCE)

extern double erf(double);
extern double erfc(double);
extern double gamma(double);
extern double hypot(double,double);
extern int isinf(double);
extern int finite(double);
extern double j0(double);
extern double j1(double);
extern double jn(int,double);
extern double lgamma(double);
extern double y0(double);
extern double y1(double);
extern double yn(int,double);

#if !defined(_XOPEN_SOURCE)
extern double acosh(double);
extern double asinh(double);
extern double atanh(double);
extern double cbrt(double);
extern double logb(double);
extern double nextafter(double,double);
extern double remainder(double,double);
extern double scalb(double,double);

/* Random double precision floating point number between 1 and 2 */
int drand(double*);

/* IEEE Test vectors for math functions.  See C9X section F.9.  */
extern double significand(double);
extern void pvec(double x);

/* Functions callable from C,intended to support IEEE arithmetic. */
extern double copysign(double,double);
extern int ilogb(double);
extern double rint(double);
extern double scalbn(double,int);

/* BSD math library entry points */
typedef struct __cmplx{double r;double i;} _cmplx_t;
typedef struct __cmplxl{long double r;long double i;} _cmplxl_t;
extern double cabs(_cmplx_t);
extern double drem(double,double);
extern double expm1(double);
extern double log1p(double);

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#if defined(__MT__) || defined(__ST_MT_ERRNO__)
#ifndef _REENTRANT 
#define _REENTRANT 1
#endif /* !_REENTRANT */
extern double gamma_r(double,int *);
extern double lgamma_r(double,int *);
#endif 

/* float version of IEEE Test Vector */
extern float significandf(float);

/* Float versions of functions callable from C,intended to support
 * IEEE arithmetic. */
extern float copysignf(float,float);
extern int ilogbf(float);
extern float rintf(float);
extern float scalbnf(float,int);

/* float versions of BSD math library entry points */
typedef struct __complxf{float x;float y;} _complexf_t;
extern float cabsf(_complexf_t);
extern float dremf(float,float);
extern float expm1f(float);
extern float log1pf(float);

/*
 * Float versions of reentrant version of gamma & lgamma; passes
 * signgam back by reference as the second argument; user must
 * allocate space for signgam.
 */
#ifdef _REENTRANT
extern float gammaf_r(float,int *);
extern float lgammaf_r(float,int *);
#endif	/* _REENTRANT */

#define INFINITYL        ((long double)(1.0L/0.0L))

#endif /* !_XOPEN_SOURCE */
#endif /* !_POSIX_SOURCE */
#endif /*!__STRICT_ANSI__ */


#if !defined(__STRICT_ANSI__) || defined(_WITH_UNDERSCORE)

#define _HUGE_VALF       ((float)(1.0F/0.0F))
#define _HUGE_VALL       ((long double)(1.0L/0.0L))
#define _INFINITY        HUGE_VAL
#define _INFINITYL       ((long double)(1.0L/0.0L))
#define _NAN             ((double)(1.0/0.0 - 1.0/0.0))
#define _NANF            ((float)(1.0F/0.0F - 1.0F/0.0F))
#define _NANL            ((long double)(1.0LL/0.0LL - 1.0LL/0.0LL))

#define _LHUGE_VAL      _HUGE_VALL
#define _LHUGE          _HUGE_VALL

#if !defined(__STRICT_ANSI__)
#define LHUGE_VAL      _HUGE_VALL
#define LHUGE          _HUGE_VALL
#endif

float _copysignf (float,float);
extern double _copysign(double,double);
extern long double _copysignl(long double,long double);

float _nextafterf (float,float);
extern double _nextafter(double,double);
extern long double _nextafterl(long double,long double);

extern long double _acosl(long double);
extern long double _asinl(long double);
extern long double _atanl(long double);
extern long double _atan2l(long double,long double);
extern long double _atofl (__const__ char *);
extern long double _cbrtl(long double);
extern long double _ceill(long double);
extern long double _cosl(long double);
extern long double _coshl(long double);
extern long double _hypotl(long double,long double);
extern long double _expl(long double);
extern long double _fabsl(long double);
extern long double _floorl(long double);
extern long double _fmodl(long double,long double);
extern long double _frexpl(long double,int *);
extern long double _ldexpl(long double,int);
extern long double _logl(long double);
extern long double _log10l(long double);
extern long double _modfl(long double,long double*);
extern long double _powl(long double,long double);
extern long double _rintl(long double);
extern long double _sinl(long double);
extern long double _sinhl(long double);
extern long double _sqrtl(long double);
extern long double _tanl(long double);
extern long double _tanhl(long double);
extern long double _truncl(long double);

#endif

#include <mathx.h>

#if defined (__cplusplus)
}
#endif

#endif /* not _MATH_H */
