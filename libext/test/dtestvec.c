/* Test vectors for math functions.  See C9X section F.9.  
   On some systems it may be necessary to modify the default exception
   settings of the floating point arithmetic unit.  */
   
/* Cephes Math Library Release 2.7:  May, 1998
Copyright 1998 by Stephen L. Moshier */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define PI M_PI
#define MACHEP DBL_EPSILON

#if 0
#define PIO2 1.570796326794896619231E0
#define PIO4 7.853981633974483096157E-1
#define THPIO4 2.35619449019234492884698
#define SQRT2 1.414213562373095048802E0
#define SQRTH 7.071067811865475244008E-1
#define MINF (-1.0/0.0)
extern double MACHEP, SQRTH, SQRT2;
extern double nan, INFINITY, NEGZERO;
#endif

struct oneargument {
    char *name;			/* Name of the function. */
    double (*func) (double);
    double *arg1;
    double *answer;
    int thresh;			/* Error report threshold. */
  };

struct twoarguments {
    char *name;			/* Name of the function. */
    double (*func) (double, double);
    double *arg1;
    double *arg2;
    double *answer;
    int thresh;
  };

struct intans {
    char *name;			/* Name of the function. */
    int (*func) (double);
    double *arg1;
    int ianswer;
  };

static double ZERO = 0.0;
static double MZERO = _NEGZERO;
static double HALF = 0.5;
static double MHALF = -0.5;
static double ONE = 1.0;
static double MONE = -1.0;
static double TWO = 2.0;
static double MTWO = -2.0;
static double THREE = 3.0;
static double MTHREE = -3.0;
static double INF  = INFINITY;
static double MINF = -INFINITY;
static double nan  = NAN;
static double pi  = M_PI;

static double MPI = -PI;
static double PIO2 = 0.5 * PI;
static double MPIO2 = -0.5 * PI;
static double PIO4 = 0.5 * 0.5 * PI;
static double MPIO4 = -0.5 * 0.5 * PI;
static double THPIO4 = 3.0 * 0.5 * 0.5 * PI;
static double MTHPIO4 = -3.0 * 0.5 * 0.5 * PI;


static volatile double x1;
static volatile double x2;
static volatile double y;
static volatile double answer;


int main(void)
{
int i, nerrors, k, ianswer, ntests;
double (*fun1) (double);
double (*fun2) (double, double);
int (*fun3) (double);
double e;
union {double d;char c[8];} u, v;
struct oneargument test1[] =
{
  {"atan", atan, &ONE, &PIO4, 0},
  {"sin", sin, &PIO2, &ONE, 0},
#if 0
  {"cos", cos, &PIO4, &SQRTH, 0},
  {"sin", sin, 32767., 1.8750655394138942394239E-1, 0},
  {"cos", cos, 32767., 9.8226335176928229845654E-1, 0},
  {"tan", tan, 32767., 1.9089234430221485740826E-1, 0},
  {"sin", sin, 8388607., 9.9234509376961249835628E-1, 0},
  {"cos", cos, 8388607., -1.2349580912475928183718E-1, 0},
  {"tan", tan, 8388607., -8.0354556223613614748329E0, 0},
  /*
  {"sin", sin, 2147483647., -7.2491655514455639054829E-1, 0},
  {"cos", cos, 2147483647., -6.8883669187794383467976E-1, 0},
  {"tan", tan, 2147483647., 1.0523779637351339136698E0, 0},
  */
  {"cos", cos, &PIO2, 6.1232339957367574e-17, 1},
  {"sin", sin, &PIO4, &SQRTH, 1},
#endif
  {"acos", acos, &nan, &nan, 0},
  {"acos", acos, &ONE, &ZERO, 0},
  {"acos", acos, &TWO, &nan, 0},
  {"acos", acos, &MTWO, &nan, 0},
  {"asin", asin, &nan, &nan, 0},
  {"asin", asin, &ZERO, &ZERO, 0},
  {"asin", asin, &MZERO, &MZERO, 0},
  {"asin", asin, &TWO, &nan, 0},
  {"asin", asin, &MTWO, &nan, 0},
  {"atan", atan, &nan, &nan, 0},
  {"atan", atan, &ZERO, &ZERO, 0},
  {"atan", atan, &MZERO, &MZERO, 0},
  {"atan", atan, &INF, &PIO2, 0},
  {"atan", atan, &MINF, &MPIO2, 0},
  {"cos", cos, &nan, &nan, 0},
  {"cos", cos, &ZERO, &ONE, 0},
  {"cos", cos, &MZERO, &ONE, 0},
  {"cos", cos, &INF, &nan, 0},
  {"cos", cos, &MINF, &nan, 0},
  {"sin", sin, &nan, &nan, 0},
  {"sin", sin, &MZERO, &MZERO, 0},
  {"sin", sin, &ZERO, &ZERO, 0},
  {"sin", sin, &INF, &nan, 0},
  {"sin", sin, &MINF, &nan, 0},
  {"tan", tan, &nan, &nan, 0},
  {"tan", tan, &ZERO, &ZERO, 0},
  {"tan", tan, &MZERO, &MZERO, 0},
  {"tan", tan, &INF, &nan, 0},
  {"tan", tan, &MINF, &nan, 0},
  {"acosh", acosh, &nan, &nan, 0},
  {"acosh", acosh, &ONE, &ZERO, 0},
  {"acosh", acosh, &INF, &INF, 0},
  {"acosh", acosh, &HALF, &nan, 0},
  {"acosh", acosh, &MONE, &nan, 0},
  {"asinh", asinh, &nan, &nan, 0},
  {"asinh", asinh, &ZERO, &ZERO, 0},
  {"asinh", asinh, &MZERO, &MZERO, 0},
  {"asinh", asinh, &INF, &INF, 0},
  {"asinh", asinh, &MINF, &MINF, 0},
  {"atanh", atanh, &nan, &nan, 0},
  {"atanh", atanh, &ZERO, &ZERO, 0},
  {"atanh", atanh, &MZERO, &MZERO, 0},
  {"atanh", atanh, &ONE, &INF, 0},
  {"atanh", atanh, &MONE, &MINF, 0},
  {"atanh", atanh, &TWO, &nan, 0},
  {"atanh", atanh, &MTWO, &nan, 0},
  {"cosh", cosh, &nan, &nan, 0},
  {"cosh", cosh, &ZERO, &ONE, 0},
  {"cosh", cosh, &MZERO, &ONE, 0},
  {"cosh", cosh, &INF, &INF, 0},
  {"cosh", cosh, &MINF, &INF, 0},
  {"sinh", sinh, &nan, &nan, 0},
  {"sinh", sinh, &ZERO, &ZERO, 0},
  {"sinh", sinh, &MZERO, &MZERO, 0},
  {"sinh", sinh, &INF, &INF, 0},
  {"sinh", sinh, &MINF, &MINF, 0},
  {"tanh", tanh, &nan, &nan, 0},
  {"tanh", tanh, &ZERO, &ZERO, 0},
  {"tanh", tanh, &MZERO, &MZERO, 0},
  {"tanh", tanh, &INF, &ONE, 0},
  {"tanh", tanh, &MINF, &MONE, 0},
  {"exp", exp, &nan, &nan, 0},
  {"exp", exp, &ZERO, &ONE, 0},
  {"exp", exp, &MZERO, &ONE, 0},
  {"exp", exp, &INF, &INF, 0},
  {"exp", exp, &MINF, &ZERO, 0},
#if 0
  {"exp2", exp2, &nan, &nan, 0},
  {"exp2", exp2, &ZERO, &ONE, 0},
  {"exp2", exp2, &MZERO, &ONE, 0},
  {"exp2", exp2, &INF, &INF, 0},
  {"exp2", exp2, &MINF, &ZERO, 0},
#endif
  {"expm1", expm1, &nan, &nan, 0},
  {"expm1", expm1, &ZERO, &ZERO, 0},
  {"expm1", expm1, &MZERO, &MZERO, 0},
  {"expm1", expm1, &INF, &INF, 0},
  {"expm1", expm1, &MINF, &MONE, 0},
  {"log", log, &nan, &nan, 0},
  {"log", log, &ZERO, &MINF, 0},
  {"log", log, &MZERO, &MINF, 0},
  {"log", log, &ONE, &ZERO, 0},
  {"log", log, &MONE, &nan, 0},
  {"log", log, &INF, &INF, 0},
  {"log10", log10, &nan, &nan, 0},
  {"log10", log10, &ZERO, &MINF, 0},
  {"log10", log10, &MZERO, &MINF, 0},
  {"log10", log10, &ONE, &ZERO, 0},
  {"log10", log10, &MONE, &nan, 0},
  {"log10", log10, &INF, &INF, 0},
  {"log1p", log1p, &nan, &nan, 0},
  {"log1p", log1p, &ZERO, &ZERO, 0},
  {"log1p", log1p, &MZERO, &MZERO, 0},
  {"log1p", log1p, &MONE, &MINF, 0},
  {"log1p", log1p, &MTWO, &nan, 0},
  {"log1p", log1p, &INF, &INF, 0},
#if 0
  {"log2", log2, &nan, &nan, 0},
  {"log2", log2, &ZERO, &MINF, 0},
  {"log2", log2, &MZERO, &MINF, 0},
  {"log2", log2, &MONE, &nan, 0},
  {"log2", log2, &INF, &INF, 0},
#endif
  /*  {"fabs", fabs, nan, nan, 0}, */
  {"fabs", fabs, &ONE, &ONE, 0},
  {"fabs", fabs, &MONE, &ONE, 0},
  {"fabs", fabs, &ZERO, &ZERO, 0},
  {"fabs", fabs, &MZERO, &ZERO, 0},
  {"fabs", fabs, &INF, &INF, 0},
  {"fabs", fabs, &MINF, &INF, 0},
  {"cbrt", cbrt, &nan, &nan, 0},
  {"cbrt", cbrt, &ZERO, &ZERO, 0},
  {"cbrt", cbrt, &MZERO, &MZERO, 0},
  {"cbrt", cbrt, &INF, &INF, 0},
  {"cbrt", cbrt, &MINF, &MINF, 0},
/* Get these from cprob.shar */
  {"erf", erf, &nan, &nan, 0},
  {"erf", erf, &ZERO, &ZERO, 0},
  {"erf", erf, &MZERO, &MZERO, 0},
  {"erf", erf, &INF, &ONE, 0},
  {"erf", erf, &MINF, &MONE, 0},
  {"erfc", erfc, &nan, &nan, 0},
  {"erfc", erfc, &INF, &ZERO, 0},
  {"erfc", erfc, &MINF, &TWO, 0},
#if 0 /* our gamma() is just an alias of lgamma(), like in BSD */
  {"gamma", gamma, &nan, &nan, 0},
  {"gamma", gamma, &INF, &INF, 0},
  {"gamma", gamma, &MONE, &nan, 0},
  {"gamma", gamma, &ZERO, &nan, 0},
  {"gamma", gamma, &MINF, &nan, 0},
#endif
  {"lgamma", lgamma, &nan, &nan, 0},
  {"lgamma", lgamma, &INF, &INF, 0},
  {"lgamma", lgamma, &MONE, &INF, 0},
  {"lgamma", lgamma, &ZERO, &INF, 0},
  {"lgamma", lgamma, &MINF, &INF, 0},

  {"ceil", ceil, &nan, &nan, 0},
  {"ceil", ceil, &ZERO, &ZERO, 0},
  {"ceil", ceil, &MZERO, &MZERO, 0},
  {"ceil", ceil, &INF, &INF, 0},
  {"ceil", ceil, &MINF, &MINF, 0},
  {"floor", floor, &nan, &nan, 0},
  {"floor", floor, &ZERO, &ZERO, 0},
  {"floor", floor, &MZERO, &MZERO, 0},
  {"floor", floor, &INF, &INF, 0},
  {"floor", floor, &MINF, &MINF, 0},
  {"null", NULL, &ZERO, &ZERO, 0},
};

struct twoarguments test2[] =
{
  {"atan2", atan2, &ZERO, &ONE, &ZERO, 0},
  {"atan2", atan2, &MZERO, &ONE, &MZERO, 0},
  {"atan2", atan2, &ZERO, &ZERO, &ZERO, 0},
  {"atan2", atan2, &MZERO, &ZERO, &MZERO, 0},
  {"atan2", atan2, &ZERO, &MONE, &pi, 0},
  {"atan2", atan2, &MZERO, &MONE, &MPI, 0},
  {"atan2", atan2, &ZERO, &MZERO, &pi, 0},
  {"atan2", atan2, &MZERO, &MZERO, &MPI, 0},
  {"atan2", atan2, &ONE, &ZERO, &PIO2, 0},
  {"atan2", atan2, &ONE, &MZERO, &PIO2, 0},
  {"atan2", atan2, &MONE, &ZERO, &MPIO2, 0},
  {"atan2", atan2, &MONE, &MZERO, &MPIO2, 0},
  {"atan2", atan2, &ONE, &INF, &ZERO, 0},
  {"atan2", atan2, &MONE, &INF, &MZERO, 0},
  {"atan2", atan2, &INF, &ONE, &PIO2, 0},
  {"atan2", atan2, &INF, &MONE, &PIO2, 0},
  {"atan2", atan2, &MINF, &ONE, &MPIO2, 0},
  {"atan2", atan2, &MINF, &MONE, &MPIO2, 0},
  {"atan2", atan2, &ONE, &MINF, &pi, 0},
  {"atan2", atan2, &MONE, &MINF, &MPI, 0},
  {"atan2", atan2, &INF, &INF, &PIO4, 0},
  {"atan2", atan2, &MINF, &INF, &MPIO4, 0},
  {"atan2", atan2, &INF, &MINF, &THPIO4, 0},
  {"atan2", atan2, &MINF, &MINF, &MTHPIO4, 0},
  {"atan2", atan2, &ONE, &ONE, &PIO4, 0},
  {"atan2", atan2, &nan, &ONE, &nan, 0},
  {"atan2", atan2, &ONE, &nan, &nan, 0},
  {"atan2", atan2, &nan, &nan, &nan, 0},
  {"pow", pow, &ONE, &ZERO, &ONE, 0},
  {"pow", pow, &ONE, &MZERO, &ONE, 0},
  {"pow", pow, &MONE, &ZERO, &ONE, 0},
  {"pow", pow, &MONE, &MZERO, &ONE, 0},
  {"pow", pow, &INF, &ZERO, &ONE, 0},
  {"pow", pow, &INF, &MZERO, &ONE, 0},
  {"pow", pow, &nan, &ZERO, &ONE, 0},
  {"pow", pow, &nan, &MZERO, &ONE, 0},
  {"pow", pow, &TWO, &INF, &INF, 0},
  {"pow", pow, &MTWO, &INF, &INF, 0},
  {"pow", pow, &HALF, &INF, &ZERO, 0},
  {"pow", pow, &MHALF, &INF, &ZERO, 0},
  {"pow", pow, &TWO, &MINF, &ZERO, 0},
  {"pow", pow, &MTWO, &MINF, &ZERO, 0},
  {"pow", pow, &HALF, &MINF, &INF, 0},
  {"pow", pow, &MHALF, &MINF, &INF, 0},
  {"pow", pow, &INF, &HALF, &INF, 0},
  {"pow", pow, &INF, &TWO, &INF, 0},
  {"pow", pow, &INF, &MHALF, &ZERO, 0},
  {"pow", pow, &INF, &MTWO, &ZERO, 0},
  {"pow", pow, &MINF, &THREE, &MINF, 0},
  {"pow", pow, &MINF, &TWO, &INF, 0},
  {"pow", pow, &MINF, &MTHREE, &MZERO, 0},
  {"pow", pow, &MINF, &MTWO, &ZERO, 0},
  {"pow", pow, &nan, &ONE, &nan, 0},
  {"pow", pow, &ONE, &nan, &nan, 0},
  {"pow", pow, &nan, &nan, &nan, 0},
  {"pow", pow, &ONE, &INF, &nan, 0},
  {"pow", pow, &MONE, &INF, &nan, 0},
  {"pow", pow, &ONE, &MINF, &nan, 0},
  {"pow", pow, &MONE, &MINF, &nan, 0},
  {"pow", pow, &MTWO, &HALF, &nan, 0},
  {"pow", pow, &ZERO, &MTHREE, &INF, 0},
  {"pow", pow, &MZERO, &MTHREE, &MINF, 0},
  {"pow", pow, &ZERO, &MHALF, &INF, 0},
  {"pow", pow, &MZERO, &MHALF, &INF, 0},
  {"pow", pow, &ZERO, &THREE, &ZERO, 0},
  {"pow", pow, &MZERO, &THREE, &MZERO, 0},
  {"pow", pow, &ZERO, &HALF, &ZERO, 0},
  {"pow", pow, &MZERO, &HALF, &ZERO, 0},
  {"null", NULL, &ZERO, &ZERO, &ZERO, 0},
};

struct intans test3[] =
{
  {"isfinite", __isfinite, &ZERO, 1},
  {"isfinite", __isfinite, &INF, 0},
  {"isfinite", __isfinite, &MINF, 0},
  {"isnan", __isnan, &nan, 1},
  {"isnan", __isnan, &INF, 0},
  {"isnan", __isnan, &ZERO, 0},
  {"isnan", __isnan, &MZERO, 0},
  {"signbit", __signbit, &MZERO, 1},
  {"signbit", __signbit, &MONE, 1},
  {"signbit", __signbit, &ZERO, 0},
  {"signbit", __signbit, &ONE, 0},
  {"signbit", __signbit, &MINF, 1},
  {"signbit", __signbit, &INF, 0},
  {"null", NULL, &ZERO, 0},
};

  /* We test the IEEE conformance of fdlibm. */
  _LIB_VERSION = _IEEE_;

    /* This masks off fpu exceptions on i386.  */
  _setfpu(0x137f);
  _control87(PC_53, MCW_PC);
        
  nerrors = 0;
  ntests = 0;
  i = 0;
  for (;;)
    {
      fun1 = test1[i].func;
      if (fun1 == NULL)
	break;
      x1 = *(test1[i].arg1);
      y = (*(fun1)) (x1);
      answer = *(test1[i].answer);
      if (test1[i].thresh == 0)
	{
	  v.d = answer;
	  u.d = y;
	  if (memcmp(u.c, v.c, 8) != 0)
	    {
	      if( __isnan(v.d) && __isnan(u.d) )
		goto nxttest1;
	      goto wrongone;
	    }
	  else
	    goto nxttest1;
	}
      if (y != answer)
	{
	  e = y - answer;
	  if (answer != 0.0)
	    e = e / answer;
	  if (e < 0)
	    e = -e;
	  if (e > test1[i].thresh * MACHEP)
	    {
wrongone:
	      printf ("%s (%.16e) = %.16e\n    should be %.16e\n",
		      test1[i].name, x1, y, answer);
	      nerrors += 1;
	    }
	}
nxttest1:
      ntests += 1;
      i += 1;
    }

  i = 0;
  for (;;)
    {
      fun2 = test2[i].func;
      if (fun2 == NULL)
	break;
      x1 = *(test2[i].arg1);
      x2 = *(test2[i].arg2);
      y = (*(fun2)) (x1, x2);
      answer = *(test2[i].answer);
      if (test2[i].thresh == 0)
	{
	  v.d = answer;
	  u.d = y;
	  if (memcmp(u.c, v.c, 8) != 0)
	    {
	      if( __isnan(v.d) && __isnan(u.d) )
		goto nxttest2;
#if 0
	      if( isnan(v.d) )
		pvec(v.d);
	      if( isnan(u.d) )
		pvec(u.d);
#endif
	    goto wrongtwo;
	    }
	  else
	    goto nxttest2;
	}
      if (y != answer)
	{
	  e = y - answer;
	  if (answer != 0.0)
	    e = e / answer;
	  if (e < 0)
	    e = -e;
	  if (e > test2[i].thresh * MACHEP)
	    {
wrongtwo:
	      printf ("%s (%.16e, %.16e) = %.16e\n    should be %.16e\n",
		      test2[i].name, x1, x2, y, answer);
	      nerrors += 1;
	    }
	}
nxttest2:
      ntests += 1;
      i += 1;
    }


  i = 0;
  for (;;)
    {
      fun3 = test3[i].func;
      if (fun3 == NULL)
	break;
      x1 = *(test3[i].arg1);
      k = (*(fun3)) (x1);
      ianswer = test3[i].ianswer;
      if (k != ianswer)
	{
	  printf ("%s(%.16e) = %d\n should be %d\n",
		  test3[i].name, x1, k, ianswer);
	  nerrors += 1;
	}
      ntests += 1;
      i += 1;
    }

  printf ("testvect: %d errors in %d tests\n", nerrors, ntests);
#undef isnan(x)
  printf ("isnan(NAN): %d\n", isnan(NAN));
  exit (0);
}
