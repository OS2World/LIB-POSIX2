/*   mtst.c
 Consistency tests for math functions.

 With NTRIALS=10000, the following are typical results for
 IEEE double precision arithmetic.

Consistency test of math functions.
Max and rms relative errors for 10000 random arguments.
x =   cbrt(   cube(x) ):  max = 0.00E+00   rms = 0.00E+00
x =   atan(    tan(x) ):  max = 2.21E-16   rms = 3.27E-17
x =    sin(   asin(x) ):  max = 2.13E-16   rms = 2.95E-17
x =   sqrt( square(x) ):  max = 0.00E+00   rms = 0.00E+00
x =    log(    exp(x) ):  max = 1.11E-16 A rms = 4.35E-18 A
x =   tanh(  atanh(x) ):  max = 2.22E-16   rms = 2.43E-17
x =  asinh(   sinh(x) ):  max = 2.05E-16   rms = 3.49E-18
x =  acosh(   cosh(x) ):  max = 1.43E-15 A rms = 1.54E-17 A
x =  log10(  exp10(x) ):  max = 5.55E-17 A rms = 1.27E-18 A
x = pow( pow(x,a),1/a ):  max = 7.60E-14   rms = 1.05E-15
x =    cos(   acos(x) ):  max = 2.22E-16 A rms = 6.90E-17 A
*/

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier
*/


#include <stdio.h>
#include <stdlib.h>
#include "math.h"

#ifndef NTRIALS
#define NTRIALS 90000
#endif

#define STRTST 0

#define WTRIALS (NTRIALS/5)

extern int _dprec(void); /*  get strict rounding on a 386 computer */

/* Provide inverses for square root and cube root: */
static double square(double x){return( x * x );}

static double cube(double x){return( x * x * x );}

/* lookup table for each function */
struct fundef {
	char *nam1;		/* the function */
	double (*name )();
	char *nam2;		/* its inverse  */
	double (*inv )();
	int nargs;		/* number of function arguments */
	int tstyp;		/* type code of the function */
	long ctrl;		/* relative error flag */
	double arg1w;		/* width of domain for 1st arg */
	double arg1l;		/* lower bound domain 1st arg */
	long arg1f;		/* flags, e.g. integer arg */
	double arg2w;		/* same info for args 2, 3, 4 */
	double arg2l;
	long arg2f;
	double arg3w;
	double arg3l;
	long arg3f;
	double arg4w;
	double arg4l;
	long arg4f;
	};


/* fundef.ctrl bits: */
#define RELERR 1

/* fundef.tstyp  test types: */
#define POWER 1 
#define ELLIP 2 
#define GAMMA 3
#define WRONK1 4
#define WRONK2 5
#define WRONK3 6

/* fundef.argNf  argument flag bits: */
#define INT 2
#define EXPSCAL 4

#define MINLOG -170.0
#define MAXLOG +170.0
/*
define PIO2 1.570796326794896619
*/

#define NTESTS 11

struct fundef defs[NTESTS] = {
{"  cube",   cube,   "  cbrt",   cbrt, 1, 0, 1, 2002.0, -1001.0, 0,
0.0, 0.0, 0},
{"   tan",    tan,   "  atan",   atan, 1, 0, 1,    0.0,     0.0,  0,
0.0, 0.0, 0},
{"  asin",   asin,   "   sin",    sin, 1, 0, 1,   2.0,      -1.0,  0,
0.0, 0.0, 0},
{"square", square,   "  sqrt",   sqrt, 1, 0, 1, 170.0,    -85.0, EXPSCAL,
0.0, 0.0, 0},
{"   exp",    exp,   "   log",    log, 1, 0, 0, 340.0,    -170.0,  0,
0.0, 0.0, 0},
{" atanh",  atanh,   "  tanh",   tanh, 1, 0, 1,    2.0,    -1.0,  0,
0.0, 0.0, 0},
{"  sinh",   sinh,   " asinh",  asinh, 1, 0, 1, 340.0,   0.0,  0,
0.0, 0.0, 0},
{"  cosh",   cosh,   " acosh",  acosh, 1, 0, 0, 340.0,      0.0,  0,
0.0, 0.0, 0},
{" exp10",  exp10,   " log10",  log10, 1, 0, 0, 340.0,    -170.0,  0,
0.0, 0.0, 0},
{"pow",       pow,      "pow",    pow, 2, POWER, 1, 21.0, 0.0,   0,
42.0, -21.0, 0},
{"  acos",   acos,   "   cos",    cos, 1, 0, 0,   2.0,      -1.0,  0,
0.0, 0.0, 0},
#if 0
{ "gamma",  gamma,     "lgamma",   lgamma, 1, GAMMA, 0, 34.0, 0.0,   0,
0.0, 0.0, 0},
#endif
};

static char *headrs[] = {
"x = %s( %s(x) ): ",
"x = %s( %s(x,a),1/a ): ",	/* power */
"Legendre %s, %s: ",		/* ellip */
"%s(x) = log(%s(x)): ",		/* gamma */
"Wronksian of %s, %s: ",
"Wronksian of %s, %s: ",
"Wronksian of %s, %s: "
};
 
static double yy1 = 0.0;
static double y2 = 0.0;
static double y3 = 0.0;
static double y4 = 0.0;
static double a = 0.0;
static double x = 0.0;
static double y = 0.0;
static double z = 0.0;
static double e = 0.0;
static double max = 0.0;
static double rmsa = 0.0;
static double rms = 0.0;
static double ave = 0.0;


int main(void)
{
double (*fun )();
double (*ifun )();
struct fundef *d;
int i, k, itst;
int m, ntr;

_dprec();  /* set coprocessor precision */
ntr = NTRIALS;
printf( "Consistency test of math functions.\n" );
printf( "Max and rms relative errors for %d random arguments.\n",
	ntr );

/* Initialize machine dependent parameters: */
defs[1].arg1w = M_PI;
defs[1].arg1l = -M_PI/2.0;
/* Microsoft C has trouble with denormal numbers. */
defs[3].arg1w = MAXLOG;
defs[3].arg1l = -MAXLOG/2.0;
defs[4].arg1w = 2*MAXLOG;
defs[4].arg1l = -MAXLOG;
defs[6].arg1w = 2.0*MAXLOG;
defs[6].arg1l = -MAXLOG;
defs[7].arg1w = MAXLOG;
defs[7].arg1l = 0.0;


/* Outer loop, on the test number: */

for( itst=STRTST; itst<NTESTS; itst++ )
{
d = &defs[itst];
k = 0;
m = 0;
max = 0.0;
rmsa = 0.0;
ave = 0.0;
fun = d->name;
ifun = d->inv;

/* Absolute error criterion starts with gamma function
 * (put all such at end of table)
 */
if( d->tstyp == GAMMA )
	printf( "Absolute error criterion (but relative if >1):\n" );

/* Smaller number of trials for Wronksians
 * (put them at end of list)
 */
if( d->tstyp == WRONK1 )
	{
	ntr = WTRIALS;
	printf( "Absolute error and only %d trials:\n", ntr );
	}

printf( headrs[d->tstyp], d->nam2, d->nam1 );

for( i=0; i<ntr; i++ )
{
m++;

/* make random number(s) in desired range(s) */
switch( d->nargs )
{

default:
goto illegn;
	
case 2:
drand( &a );
a = d->arg2w *  ( a - 1.0 )  +  d->arg2l;
if( d->arg2f & EXPSCAL )
	{
	a = exp(a);
	drand( &y2 );
	a -= 1.0e-13 * a * y2;
	}
if( d->arg2f & INT )
	{
	k = a + 0.25;
	a = k;
	}

case 1:
drand( &x );
x = d->arg1w *  ( x - 1.0 )  +  d->arg1l;
if( d->arg1f & EXPSCAL )
	{
	x = exp(x);
	drand( &a );
	x += 1.0e-13 * x * a;
	}
}


/* compute function under test */
switch( d->nargs )
	{
	case 1:
	switch( d->tstyp )
		{
		case ELLIP:
		yy1 = ( *(fun) )(x);
		y2 = ( *(fun) )(1.0-x);
		y3 = ( *(ifun) )(x);
		y4 = ( *(ifun) )(1.0-x);
		break;

		y = lgamma(x);
		x = log( gamma(x) );
		break;
		default:
		z = ( *(fun) )(x);
		y = ( *(ifun) )(z);
		}
	break;
	
	case 2:
	if( d->arg2f & INT )
		{
		switch( d->tstyp )
			{
			case WRONK1:
			yy1 = (*fun)( k, x ); /* jn */
			y2 = (*fun)( k+1, x );
			y3 = (*ifun)( k, x ); /* yn */
			y4 = (*ifun)( k+1, x );	
			break;

			case WRONK2:
			yy1 = (*fun)( a, x ); /* iv */
			y2 = (*fun)( a+1.0, x );
			y3 = (*ifun)( k, x ); /* kn */	
			y4 = (*ifun)( k+1, x );	
			break;

			default:
			z = (*fun)( k, x );
			y = (*ifun)( k, z );
			}
		}
	else
		{
		if( d->tstyp == POWER )
			{
			z = (*fun)( x, a );
			y = (*ifun)( z, 1.0/a );
			}
		else
			{
			z = (*fun)( a, x );
			y = (*ifun)( a, z );
			}
		}
	break;


	default:
illegn:
	printf( "Illegal nargs= %d", d->nargs );
	exit(1);
	}	

switch( d->tstyp )
	{
	case WRONK1:
	e = (y2*y3 - yy1*y4) - 2.0/(M_PI*x); /* Jn, Yn */
	break;

	case WRONK2:
	e = (y2*y3 + yy1*y4) - 1.0/x; /* In, Kn */
	break;
	
	case ELLIP:
	e = (yy1-y3)*y4 + y3*y2 - _PIO2;
	break;

	default:
	e = y - x;
	break;
	}

if( d->ctrl & RELERR )
	e /= x;
else
	{
	if( fabs(x) > 1.0 )
		e /= x;
	}

ave += e;
/* absolute value of error */
if( e < 0 )
	e = -e;

/* peak detect the error */
if( e > max )
	{
	max = e;

	if( e > 1.0e-10 )
		{
		printf("x %.6E z %.6E y %.6E max %.4E\n",
		 x, z, y, max);
		if( d->tstyp == POWER )
			{
			printf( "a %.6E\n", a );
			}
		if( d->tstyp >= WRONK1 )
			{
		printf( "yy1 %.4E y2 %.4E y3 %.4E y4 %.4E k %d x %.4E\n",
		 yy1, y2, y3, y4, k, x );
			}
		}

/*
	printf("%.8E %.8E %.4E %6ld \n", x, y, max, n);
	printf("%d %.8E %.8E %.4E %6ld \n", k, x, y, max, n);
	printf("%.6E %.6E %.6E %.4E %6ld \n", a, x, y, max, n);
	printf("%.6E %.6E %.6E %.6E %.4E %6ld \n", a, b, x, y, max, n);
	printf("%.4E %.4E %.4E %.4E %.4E %.4E %6ld \n",
		a, b, c, x, y, max, n);
*/
	}

/* accumulate rms error	*/
e *= 1.0e16;	/* adjust range */
rmsa += e * e;	/* accumulate the square of the error */
}

/* report after NTRIALS trials */
rms = 1.0e-16 * sqrt( rmsa/m );
if(d->ctrl & RELERR)
	printf(" max = %.2E   rms = %.2E\n", max, rms );
else
	printf(" max = %.2E A rms = %.2E A\n", max, rms );
} /* loop on itst */

exit(0);
}
