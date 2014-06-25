/* w_logf.c -- float version of w_log.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */
#include "math.h"
#include "math_private.h"

static float ponef(float), qonef(float);
static	float __ieee754_lgammaf_r(float x, int *__Wlogf_signgamp);

#define ln2_hi 	   6.9313812256e-01F	/* 0x3f317180 */
#define ln2_lo 	   9.0580006145e-06F	/* 0x3717f7d1 */
#define two25 	    3.355443200e+07F	/* 0x4c000000 */
#define ivln10     	  4.3429449201e-01F /* 0x3ede5bd9 */
#define log10_2hi  	  3.0102920532e-01F /* 0x3e9a2080 */
#define log10_2lo  	  7.9034151668e-07F /* 0x355427db */
#define invsqrtpi	  5.6418961287e-01F /* 0x3f106ebb */
#define tpi      	  6.3661974669e-01F /* 0x3f22f983 */
#define one 	  1.0000000000e+00F /* 0x3F800000 */
#define two 	  2.0000000000e+00F /* 0x40000000 */
#define ln2 	  6.9314718246e-01F /* 0x3f317218 */
#define huge	  1.0000000000e+30F
#define Lg1 	 6.6666668653e-01F	/* 3F2AAAAB */
#define Lg2 	 4.0000000596e-01F	/* 3ECCCCCD */
#define Lg3 	 2.8571429849e-01F /* 3E924925 */
#define Lg4 	 2.2222198546e-01F /* 3E638E29 */
#define Lg5 	 1.8183572590e-01F /* 3E3A3325 */
#define Lg6 	 1.5313838422e-01F /* 3E1CD04F */
#define Lg7 	 1.4798198640e-01F /* 3E178897 */

#define zero 0.0000000000e+00F


	static float 
__ieee754_logf(float x)
{
	float hfsq,f,s,z,R,w,t1,t2,dk;
	int32_t k,ix,i,j;

	GET_FLOAT_WORD(ix,x);

	k=0;
	if (ix < 0x00800000) {			/* x < 2**-126  */
	    if ((ix&0x7fffffff)==0) 
		return -two25/zero;		/* log(+-0)=-inf */
	    if (ix<0) return (x-x)/zero;	/* log(-#) = NaN */
	    k -= 25; x *= two25; /* subnormal number, scale up x */
	    GET_FLOAT_WORD(ix,x);
	} 
	if (ix >= 0x7f800000) return x+x;
	k += (ix>>23)-127;
	ix &= 0x007fffff;
	i = (ix+(0x95f64<<3))&0x800000;
	SET_FLOAT_WORD(x,ix|(i^0x3f800000));	/* normalize x or x/2 */
	k += (i>>23);
	f = x-(float)1.0;
	if((0x007fffff&(15+ix))<16) {	/* |f| < 2**-20 */
	    if(f==zero) {
	    	if(k==0) return zero;  else {dk=(float)k;
				 return dk*ln2_hi+dk*ln2_lo;}
			}
	    R = f*f*((float)0.5-(float)0.33333333333333333*f);
	    if(k==0) return f-R; else {dk=(float)k;
	    	     return dk*ln2_hi-((R-dk*ln2_lo)-f);}
	}
 	s = f/((float)2.0+f); 
	dk = (float)k;
	z = s*s;
	i = ix-(0x6147a<<3);
	w = z*z;
	j = (0x6b851<<3)-ix;
	t1= w*(Lg2+w*(Lg4+w*Lg6)); 
	t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7))); 
	i |= j;
	R = t2+t1;
	if(i>0) {
	    hfsq=(float)0.5*f*f;
	    if(k==0) return f-(hfsq-s*(hfsq+R)); else
		     return dk*ln2_hi-((hfsq-(s*(hfsq+R)+dk*ln2_lo))-f);
	} else {
	    if(k==0) return f-s*(f-R); else
		     return dk*ln2_hi-((s*(f-R)-dk*ln2_lo)-f);
	}
}

	extern float
asinhf(float x)
{	
	float t,w;
	int32_t hx,ix;
	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x7f800000) return x+x;	/* x is inf or NaN */
	if(ix< 0x31800000) {	/* |x|<2**-28 */
	    if(huge+x>one) return x;	/* return x inexact except 0 */
	} 
	if(ix>0x4d800000) {	/* |x| > 2**28 */
	    w = __ieee754_logf(fabsf(x))+ln2;
	} else if (ix>0x40000000) {	/* 2**28 > |x| > 2.0 */
	    t = fabsf(x);
	    w = __ieee754_logf((float)2.0*t+one/(__ieee754_sqrtf(x*x+one)+t));
	} else {		/* 2.0 > |x| > 2**-28 */
	    t = x*x;
	    w =log1pf(fabsf(x)+t/(one+__ieee754_sqrtf(one+t)));
	}
	if(hx>0) return w; else return -w;
}

/*
 * wrapper logf(x)
 */
	extern float 
logf(float x)		/* wrapper logf */
{
#ifdef _IEEE_LIBM
	return __ieee754_logf(x);
#else
	float z;
	z = __ieee754_logf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) || x > (float)0.0) return z;
	if(x==(float)0.0)
	    /* logf(0) */
	    return (float)__kernel_standard((double)x,(double)x,116);
	else 
	    /* logf(x<0) */
	    return (float)__kernel_standard((double)x,(double)x,117);
#endif
}

_M_INL	float __ieee754_acoshf(float x)
{	
	float t;
	int32_t hx;
	GET_FLOAT_WORD(hx,x);
	if(hx<0x3f800000) {		/* x < 1 */
	    return (x-x)/(x-x);
	} else if(hx >=0x4d800000) {	/* x > 2**28 */
	    if(hx >=0x7f800000) {	/* x is inf of NaN */
	        return x+x;
	    } else 
		return __ieee754_logf(x)+ln2;	/* acosh(huge)=log(2x) */
	} else if (hx==0x3f800000) {
	    return 0.0;			/* acosh(1) = 0 */
	} else if (hx > 0x40000000) {	/* 2**28 > x > 2 */
	    t=x*x;
	    return __ieee754_logf((float)2.0*x-one/(x+__ieee754_sqrtf(t-one)));
	} else {			/* 1<x<2 */
	    t = x-one;
	    return log1pf(t+sqrtf((float)2.0*t+t*t));
	}
}

/* 
 * wrapper acoshf(x)
 */

	extern float
acoshf(float x)		/* wrapper acoshf */
{
#ifdef _IEEE_LIBM
	return __ieee754_acoshf(x);
#else
	float z;
	z = __ieee754_acoshf(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<(float)1.0) {
		/* acosh(x<1) */
	        return (float)__kernel_standard((double)x,(double)x,129);
	} else
	    return z;
#endif
}

static float pzerof(float), qzerof(float);

static const float
/* R0/S0 on [0, 2.00] */
R02  =  1.5625000000e-02, /* 0x3c800000 */
R03  = -1.8997929874e-04, /* 0xb947352e */
R04  =  1.8295404516e-06, /* 0x35f58e88 */
R05  = -4.6183270541e-09, /* 0xb19eaf3c */
S01  =  1.5619102865e-02, /* 0x3c7fe744 */
S02  =  1.1692678527e-04, /* 0x38f53697 */
S03  =  5.1354652442e-07, /* 0x3509daa6 */
S04  =  1.1661400734e-09; /* 0x30a045e8 */


_M_INL	float __ieee754_j0f(float x) 
{
	float z, s,c,ss,cc,r,u,v;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x7f800000) return one/(x*x);
	x = fabsf(x);
	if(ix >= 0x40000000) {	/* |x| >= 2.0 */
		s = sinf(x);
		c = cosf(x);
		ss = s-c;
		cc = s+c;
		if(ix<0x7f000000) {  /* make sure x+x not overflow */
		    z = -cosf(x+x);
		    if ((s*c)<zero) cc = z/ss;
		    else 	    ss = z/cc;
		}
	/*
	 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
	 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
	 */
		if(ix>0x80000000) z = (invsqrtpi*cc)/sqrtf(x);
		else {
		    u = pzerof(x); v = qzerof(x);
		    z = invsqrtpi*(u*cc-v*ss)/sqrtf(x);
		}
		return z;
	}
	if(ix<0x39000000) {	/* |x| < 2**-13 */
	    if(huge+x>one) {	/* raise inexact if x != 0 */
	        if(ix<0x32000000) return one;	/* |x|<2**-27 */
	        else 	      return one - (float)0.25*x*x;
	    }
	}
	z = x*x;
	r =  z*(R02+z*(R03+z*(R04+z*R05)));
	s =  one+z*(S01+z*(S02+z*(S03+z*S04)));
	if(ix < 0x3F800000) {	/* |x| < 1.00 */
	    return one + z*((float)-0.25+(r/s));
	} else {
	    u = (float)0.5*x;
	    return((one+u)*(one-u)+z*(r/s));
	}
}

static const float
u00  = -7.3804296553e-02, /* 0xbd9726b5 */
u01  =  1.7666645348e-01, /* 0x3e34e80d */
u02  = -1.3818567619e-02, /* 0xbc626746 */
u03  =  3.4745343146e-04, /* 0x39b62a69 */
u04  = -3.8140706238e-06, /* 0xb67ff53c */
u05  =  1.9559013964e-08, /* 0x32a802ba */
u06  = -3.9820518410e-11, /* 0xae2f21eb */
v01  =  1.2730483897e-02, /* 0x3c509385 */
v02  =  7.6006865129e-05, /* 0x389f65e0 */
v03  =  2.5915085189e-07, /* 0x348b216c */
v04  =  4.4111031494e-10; /* 0x2ff280c2 */

_M_INL	float __ieee754_y0f(float x) 
{
	float z, s,c,ss,cc,u,v;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx,x);
        ix = 0x7fffffff&hx;
    /* Y0(NaN) is NaN, y0(-inf) is Nan, y0(inf) is 0  */
	if(ix>=0x7f800000) return  one/(x+x*x); 
        if(ix==0) return -one/zero;
        if(hx<0) return zero/zero;
        if(ix >= 0x40000000) {  /* |x| >= 2.0 */
        /* y0(x) = sqrt(2/(pi*x))*(p0(x)*sin(x0)+q0(x)*cos(x0))
         * where x0 = x-pi/4
         *      Better formula:
         *              cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
         *                      =  1/sqrt(2) * (sin(x) + cos(x))
         *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
         *                      =  1/sqrt(2) * (sin(x) - cos(x))
         * To avoid cancellation, use
         *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
         * to compute the worse one.
         */
                s = sinf(x);
                c = cosf(x);
                ss = s-c;
                cc = s+c;
	/*
	 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
	 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
	 */
                if(ix<0x7f000000) {  /* make sure x+x not overflow */
                    z = -cosf(x+x);
                    if ((s*c)<zero) cc = z/ss;
                    else            ss = z/cc;
                }
                if(ix>0x80000000) z = (invsqrtpi*ss)/sqrtf(x);
                else {
                    u = pzerof(x); v = qzerof(x);
                    z = invsqrtpi*(u*ss+v*cc)/sqrtf(x);
                }
                return z;
	}
	if(ix<=0x32000000) {	/* x < 2**-27 */
	    return(u00 + tpi*__ieee754_logf(x));
	}
	z = x*x;
	u = u00+z*(u01+z*(u02+z*(u03+z*(u04+z*(u05+z*u06)))));
	v = one+z*(v01+z*(v02+z*(v03+z*v04)));
	return(u/v + tpi*(__ieee754_j0f(x)*__ieee754_logf(x)));
}

/* The asymptotic expansions of pzero is
 *	1 - 9/128 s^2 + 11025/98304 s^4 - ...,	where s = 1/x.
 * For x >= 2, We approximate pzero by
 * 	pzero(x) = 1 + (R/S)
 * where  R = pR0 + pR1*s^2 + pR2*s^4 + ... + pR5*s^10
 * 	  S = 1 + pS0*s^2 + ... + pS4*s^10
 * and
 *	| pzero(x)-1-R/S | <= 2  ** ( -60.26)
 */
#ifdef __STDC__
static const float pR8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#else
static float pR8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#endif
  0.0000000000e+00, /* 0x00000000 */
 -7.0312500000e-02, /* 0xbd900000 */
 -8.0816707611e+00, /* 0xc1014e86 */
 -2.5706311035e+02, /* 0xc3808814 */
 -2.4852163086e+03, /* 0xc51b5376 */
 -5.2530439453e+03, /* 0xc5a4285a */
};
#ifdef __STDC__
static const float pS8[5] = {
#else
static float pS8[5] = {
#endif
  1.1653436279e+02, /* 0x42e91198 */
  3.8337448730e+03, /* 0x456f9beb */
  4.0597855469e+04, /* 0x471e95db */
  1.1675296875e+05, /* 0x47e4087c */
  4.7627726562e+04, /* 0x473a0bba */
};
#ifdef __STDC__
static const float pR5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#else
static float pR5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#endif
 -1.1412546255e-11, /* 0xad48c58a */
 -7.0312492549e-02, /* 0xbd8fffff */
 -4.1596107483e+00, /* 0xc0851b88 */
 -6.7674766541e+01, /* 0xc287597b */
 -3.3123129272e+02, /* 0xc3a59d9b */
 -3.4643338013e+02, /* 0xc3ad3779 */
};
#ifdef __STDC__
static const float pS5[5] = {
#else
static float pS5[5] = {
#endif
  6.0753936768e+01, /* 0x42730408 */
  1.0512523193e+03, /* 0x44836813 */
  5.9789707031e+03, /* 0x45bad7c4 */
  9.6254453125e+03, /* 0x461665c8 */
  2.4060581055e+03, /* 0x451660ee */
};

#ifdef __STDC__
static const float pR3[6] = {/* for x in [4.547,2.8571]=1/[0.2199,0.35001] */
#else
static float pR3[6] = {/* for x in [4.547,2.8571]=1/[0.2199,0.35001] */
#endif
 -2.5470459075e-09, /* 0xb12f081b */
 -7.0311963558e-02, /* 0xbd8fffb8 */
 -2.4090321064e+00, /* 0xc01a2d95 */
 -2.1965976715e+01, /* 0xc1afba52 */
 -5.8079170227e+01, /* 0xc2685112 */
 -3.1447946548e+01, /* 0xc1fb9565 */
};
#ifdef __STDC__
static const float pS3[5] = {
#else
static float pS3[5] = {
#endif
  3.5856033325e+01, /* 0x420f6c94 */
  3.6151397705e+02, /* 0x43b4c1ca */
  1.1936077881e+03, /* 0x44953373 */
  1.1279968262e+03, /* 0x448cffe6 */
  1.7358093262e+02, /* 0x432d94b8 */
};

#ifdef __STDC__
static const float pR2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#else
static float pR2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#endif
 -8.8753431271e-08, /* 0xb3be98b7 */
 -7.0303097367e-02, /* 0xbd8ffb12 */
 -1.4507384300e+00, /* 0xbfb9b1cc */
 -7.6356959343e+00, /* 0xc0f4579f */
 -1.1193166733e+01, /* 0xc1331736 */
 -3.2336456776e+00, /* 0xc04ef40d */
};
static const float pS2[5] = {
  2.2220300674e+01, /* 0x41b1c32d */
  1.3620678711e+02, /* 0x430834f0 */
  2.7047027588e+02, /* 0x43873c32 */
  1.5387539673e+02, /* 0x4319e01a */
  1.4657617569e+01, /* 0x416a859a */
};

	static float pzerof(float x)
{
	const float *p= NULL,*q = NULL;
	float z,r,s;
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	if(ix>=0x41000000)     {p = pR8; q= pS8;}
	else if(ix>=0x40f71c58){p = pR5; q= pS5;}
	else if(ix>=0x4036db68){p = pR3; q= pS3;}
	else if(ix>=0x40000000){p = pR2; q= pS2;}
	z = one/(x*x);
	r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
	s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*q[4]))));
	return one+ r/s;
}
		

/* For x >= 8, the asymptotic expansions of qzero is
 *	-1/8 s + 75/1024 s^3 - ..., where s = 1/x.
 * We approximate pzero by
 * 	qzero(x) = s*(-1.25 + (R/S))
 * where  R = qR0 + qR1*s^2 + qR2*s^4 + ... + qR5*s^10
 * 	  S = 1 + qS0*s^2 + ... + qS5*s^12
 * and
 *	| qzero(x)/s +1.25-R/S | <= 2  ** ( -61.22)
 */
static const float qR8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
  0.0000000000e+00, /* 0x00000000 */
  7.3242187500e-02, /* 0x3d960000 */
  1.1768206596e+01, /* 0x413c4a93 */
  5.5767340088e+02, /* 0x440b6b19 */
  8.8591972656e+03, /* 0x460a6cca */
  3.7014625000e+04, /* 0x471096a0 */
};
static const float qS8[6] = {
  1.6377603149e+02, /* 0x4323c6aa */
  8.0983447266e+03, /* 0x45fd12c2 */
  1.4253829688e+05, /* 0x480b3293 */
  8.0330925000e+05, /* 0x49441ed4 */
  8.4050156250e+05, /* 0x494d3359 */
 -3.4389928125e+05, /* 0xc8a7eb69 */
};

static const float qR5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
  1.8408595828e-11, /* 0x2da1ec79 */
  7.3242180049e-02, /* 0x3d95ffff */
  5.8356351852e+00, /* 0x40babd86 */
  1.3511157227e+02, /* 0x43071c90 */
  1.0272437744e+03, /* 0x448067cd */
  1.9899779053e+03, /* 0x44f8bf4b */
};
static const float qS5[6] = {
  8.2776611328e+01, /* 0x42a58da0 */
  2.0778142090e+03, /* 0x4501dd07 */
  1.8847289062e+04, /* 0x46933e94 */
  5.6751113281e+04, /* 0x475daf1d */
  3.5976753906e+04, /* 0x470c88c1 */
 -5.3543427734e+03, /* 0xc5a752be */
};
static const float qR3[6] = {/* for x in [4.547,2.8571]=1/[0.2199,0.35001] */
  4.3774099900e-09, /* 0x3196681b */
  7.3241114616e-02, /* 0x3d95ff70 */
  3.3442313671e+00, /* 0x405607e3 */
  4.2621845245e+01, /* 0x422a7cc5 */
  1.7080809021e+02, /* 0x432acedf */
  1.6673394775e+02, /* 0x4326bbe4 */
};
static const float qS3[6] = {
  4.8758872986e+01, /* 0x42430916 */
  7.0968920898e+02, /* 0x44316c1c */
  3.7041481934e+03, /* 0x4567825f */
  6.4604252930e+03, /* 0x45c9e367 */
  2.5163337402e+03, /* 0x451d4557 */
 -1.4924745178e+02, /* 0xc3153f59 */
};

static const float qR2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
  1.5044444979e-07, /* 0x342189db */
  7.3223426938e-02, /* 0x3d95f62a */
  1.9981917143e+00, /* 0x3fffc4bf */
  1.4495602608e+01, /* 0x4167edfd */
  3.1666231155e+01, /* 0x41fd5471 */
  1.6252708435e+01, /* 0x4182058c */
};
static const float qS2[6] = {
  3.0365585327e+01, /* 0x41f2ecb8 */
  2.6934811401e+02, /* 0x4386ac8f */
  8.4478375244e+02, /* 0x44533229 */
  8.8293585205e+02, /* 0x445cbbe5 */
  2.1266638184e+02, /* 0x4354aa98 */
 -5.3109550476e+00, /* 0xc0a9f358 */
};

	static float qzerof(float x)
{
	const float *p=NULL,*q=NULL;
	float s,r,z;
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	if(ix>=0x41000000)     {p = qR8; q= qS8;}
	else if(ix>=0x40f71c58){p = qR5; q= qS5;}
	else if(ix>=0x4036db68){p = qR3; q= qS3;}
	else if(ix>=0x40000000){p = qR2; q= qS2;}
	z = one/(x*x);
	r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
	s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*(q[4]+z*q[5])))));
	return (-(float).125 + r/s)/x;
}

/*
 * wrapper j0f(float x), y0f(float x)
 */
	extern float 
j0f(float x)		/* wrapper j0f */
{
#ifdef _IEEE_LIBM
	return __ieee754_j0f(x);
#else
	float z = __ieee754_j0f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(fabsf(x)>(float)X_TLOSS) {
		/* j0f(|x|>X_TLOSS) */
	        return (float)__kernel_standard((double)x,(double)x,134);
	} else
	    return z;
#endif
}

	extern float 
y0f(float x)		/* wrapper y0f */
{
#ifdef _IEEE_LIBM
	return __ieee754_y0f(x);
#else
	float z;
	z = __ieee754_y0f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
        if(x <= (float)0.0){
                if(x==(float)0.0)
                    /* d= -one/(x-x); */
                    return (float)__kernel_standard((double)x,(double)x,108);
                else
                    /* d = zero/(x-x); */
                    return (float)__kernel_standard((double)x,(double)x,109);
        }
	if(x>(float)X_TLOSS) {
		/* y0(x>X_TLOSS) */
	        return (float)__kernel_standard((double)x,(double)x,135);
	} else
	    return z;
#endif
}

/* w_j1f.c -- float version of w_j1.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

static const float 
	/* R0/S0 on [0,2] */
r00  = -6.2500000000e-02, /* 0xbd800000 */
r01  =  1.4070566976e-03, /* 0x3ab86cfd */
r02  = -1.5995563444e-05, /* 0xb7862e36 */
r03  =  4.9672799207e-08, /* 0x335557d2 */
s01  =  1.9153760746e-02, /* 0x3c9ce859 */
s02  =  1.8594678841e-04, /* 0x3942fab6 */
s03  =  1.1771846857e-06, /* 0x359dffc2 */
s04  =  5.0463624390e-09, /* 0x31ad6446 */
s05  =  1.2354227016e-11; /* 0x2d59567e */

_M_INL	float __ieee754_j1f(float x) 
{
	float z, s,c,ss,cc,r,u,v,y;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix>=0x7f800000) return one/x;
	y = fabsf(x);
	if(ix >= 0x40000000) {	/* |x| >= 2.0 */
		s = sinf(y);
		c = cosf(y);
		ss = -s-c;
		cc = s-c;
		if(ix<0x7f000000) {  /* make sure y+y not overflow */
		    z = cosf(y+y);
		    if ((s*c)>zero) cc = z/ss;
		    else 	    ss = z/cc;
		}
	/*
	 * j1(x) = 1/sqrt(pi) * (P(1,x)*cc - Q(1,x)*ss) / sqrt(x)
	 * y1(x) = 1/sqrt(pi) * (P(1,x)*ss + Q(1,x)*cc) / sqrt(x)
	 */
		if(ix>0x80000000) z = (invsqrtpi*cc)/sqrtf(y);
		else {
		    u = ponef(y); v = qonef(y);
		    z = invsqrtpi*(u*cc-v*ss)/sqrtf(y);
		}
		if(hx<0) return -z;
		else  	 return  z;
	}
	if(ix<0x32000000) {	/* |x|<2**-27 */
	    if(huge+x>one) return (float)0.5*x;/* inexact if x!=0 necessary */
	}
	z = x*x;
	r =  z*(r00+z*(r01+z*(r02+z*r03)));
	s =  one+z*(s01+z*(s02+z*(s03+z*(s04+z*s05))));
	r *= x;
	return(x*(float)0.5+r/s);
}

static const float U0[5] = {
 -1.9605709612e-01, /* 0xbe48c331 */
  5.0443872809e-02, /* 0x3d4e9e3c */
 -1.9125689287e-03, /* 0xbafaaf2a */
  2.3525259166e-05, /* 0x37c5581c */
 -9.1909917899e-08, /* 0xb3c56003 */
};
static const float V0[5] = {
  1.9916731864e-02, /* 0x3ca3286a */
  2.0255257550e-04, /* 0x3954644b */
  1.3560879779e-06, /* 0x35b602d4 */
  6.2274145840e-09, /* 0x31d5f8eb */
  1.6655924903e-11, /* 0x2d9281cf */
};

_M_INL	float __ieee754_y1f(float x) 
{
	float z, s,c,ss,cc,u,v;
	int32_t hx,ix;

	GET_FLOAT_WORD(hx,x);
        ix = 0x7fffffff&hx;
    /* if Y1(NaN) is NaN, Y1(-inf) is NaN, Y1(inf) is 0 */
	if(ix>=0x7f800000) return  one/(x+x*x); 
        if(ix==0) return -one/zero;
        if(hx<0) return zero/zero;
        if(ix >= 0x40000000) {  /* |x| >= 2.0 */
                s = sinf(x);
                c = cosf(x);
                ss = -s-c;
                cc = s-c;
                if(ix<0x7f000000) {  /* make sure x+x not overflow */
                    z = cosf(x+x);
                    if ((s*c)>zero) cc = z/ss;
                    else            ss = z/cc;
                }
        /* y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x0)+q1(x)*cos(x0))
         * where x0 = x-3pi/4
         *      Better formula:
         *              cos(x0) = cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
         *                      =  1/sqrt(2) * (sin(x) - cos(x))
         *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
         *                      = -1/sqrt(2) * (cos(x) + sin(x))
         * To avoid cancellation, use
         *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
         * to compute the worse one.
         */
                if(ix>0x48000000) z = (invsqrtpi*ss)/sqrtf(x);
                else {
                    u = ponef(x); v = qonef(x);
                    z = invsqrtpi*(u*ss+v*cc)/sqrtf(x);
                }
                return z;
        } 
        if(ix<=0x24800000) {    /* x < 2**-54 */
            return(-tpi/x);
        } 
        z = x*x;
        u = U0[0]+z*(U0[1]+z*(U0[2]+z*(U0[3]+z*U0[4])));
        v = one+z*(V0[0]+z*(V0[1]+z*(V0[2]+z*(V0[3]+z*V0[4]))));
        return(x*(u/v) + tpi*(__ieee754_j1f(x)*__ieee754_logf(x)-one/x));
}

/* For x >= 8, the asymptotic expansions of pone is
 *	1 + 15/128 s^2 - 4725/2^15 s^4 - ...,	where s = 1/x.
 * We approximate pone by
 * 	pone(x) = 1 + (R/S)
 * where  R = pr0 + pr1*s^2 + pr2*s^4 + ... + pr5*s^10
 * 	  S = 1 + ps0*s^2 + ... + ps4*s^10
 * and
 *	| pone(x)-1-R/S | <= 2  ** ( -60.06)
 */

#ifdef __STDC__
static const float pr8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#else
static float pr8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#endif
  0.0000000000e+00, /* 0x00000000 */
  1.1718750000e-01, /* 0x3df00000 */
  1.3239480972e+01, /* 0x4153d4ea */
  4.1205184937e+02, /* 0x43ce06a3 */
  3.8747453613e+03, /* 0x45722bed */
  7.9144794922e+03, /* 0x45f753d6 */
};
#ifdef __STDC__
static const float ps8[5] = {
#else
static float ps8[5] = {
#endif
  1.1420736694e+02, /* 0x42e46a2c */
  3.6509309082e+03, /* 0x45642ee5 */
  3.6956207031e+04, /* 0x47105c35 */
  9.7602796875e+04, /* 0x47bea166 */
  3.0804271484e+04, /* 0x46f0a88b */
};

#ifdef __STDC__
static const float pr5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#else
static float pr5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#endif
  1.3199052094e-11, /* 0x2d68333f */
  1.1718749255e-01, /* 0x3defffff */
  6.8027510643e+00, /* 0x40d9b023 */
  1.0830818176e+02, /* 0x42d89dca */
  5.1763616943e+02, /* 0x440168b7 */
  5.2871520996e+02, /* 0x44042dc6 */
};
#ifdef __STDC__
static const float ps5[5] = {
#else
static float ps5[5] = {
#endif
  5.9280597687e+01, /* 0x426d1f55 */
  9.9140142822e+02, /* 0x4477d9b1 */
  5.3532670898e+03, /* 0x45a74a23 */
  7.8446904297e+03, /* 0x45f52586 */
  1.5040468750e+03, /* 0x44bc0180 */
};

#ifdef __STDC__
static const float pr3[6] = {
#else
static float pr3[6] = {/* for x in [4.547,2.8571]=1/[0.2199,0.35001] */
#endif
  3.0250391081e-09, /* 0x314fe10d */
  1.1718686670e-01, /* 0x3defffab */
  3.9329774380e+00, /* 0x407bb5e7 */
  3.5119403839e+01, /* 0x420c7a45 */
  9.1055007935e+01, /* 0x42b61c2a */
  4.8559066772e+01, /* 0x42423c7c */
};
#ifdef __STDC__
static const float ps3[5] = {
#else
static float ps3[5] = {
#endif
  3.4791309357e+01, /* 0x420b2a4d */
  3.3676245117e+02, /* 0x43a86198 */
  1.0468714600e+03, /* 0x4482dbe3 */
  8.9081134033e+02, /* 0x445eb3ed */
  1.0378793335e+02, /* 0x42cf936c */
};

#ifdef __STDC__
static const float pr2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#else
static float pr2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#endif
  1.0771083225e-07, /* 0x33e74ea8 */
  1.1717621982e-01, /* 0x3deffa16 */
  2.3685150146e+00, /* 0x401795c0 */
  1.2242610931e+01, /* 0x4143e1bc */
  1.7693971634e+01, /* 0x418d8d41 */
  5.0735230446e+00, /* 0x40a25a4d */
};
#ifdef __STDC__
static const float ps2[5] = {
#else
static float ps2[5] = {
#endif
  2.1436485291e+01, /* 0x41ab7dec */
  1.2529022980e+02, /* 0x42fa9499 */
  2.3227647400e+02, /* 0x436846c7 */
  1.1767937469e+02, /* 0x42eb5bd7 */
  8.3646392822e+00, /* 0x4105d590 */
};

	static float ponef(float x)
{
	const float *p=NULL,*q=NULL;
	float z,r,s;
        int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
        if(ix>=0x41000000)     {p = pr8; q= ps8;}
        else if(ix>=0x40f71c58){p = pr5; q= ps5;}
        else if(ix>=0x4036db68){p = pr3; q= ps3;}
        else if(ix>=0x40000000){p = pr2; q= ps2;}
        z = one/(x*x);
        r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
        s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*q[4]))));
        return one+ r/s;
}
		

/* For x >= 8, the asymptotic expansions of qone is
 *	3/8 s - 105/1024 s^3 - ..., where s = 1/x.
 * We approximate pone by
 * 	qone(x) = s*(0.375 + (R/S))
 * where  R = qr1*s^2 + qr2*s^4 + ... + qr5*s^10
 * 	  S = 1 + qs1*s^2 + ... + qs6*s^12
 * and
 *	| qone(x)/s -0.375-R/S | <= 2  ** ( -61.13)
 */

#ifdef __STDC__
static const float qr8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#else
static float qr8[6] = { /* for x in [inf, 8]=1/[0,0.125] */
#endif
  0.0000000000e+00, /* 0x00000000 */
 -1.0253906250e-01, /* 0xbdd20000 */
 -1.6271753311e+01, /* 0xc1822c8d */
 -7.5960174561e+02, /* 0xc43de683 */
 -1.1849806641e+04, /* 0xc639273a */
 -4.8438511719e+04, /* 0xc73d3683 */
};
#ifdef __STDC__
static const float qs8[6] = {
#else
static float qs8[6] = {
#endif
  1.6139537048e+02, /* 0x43216537 */
  7.8253862305e+03, /* 0x45f48b17 */
  1.3387534375e+05, /* 0x4802bcd6 */
  7.1965775000e+05, /* 0x492fb29c */
  6.6660125000e+05, /* 0x4922be94 */
 -2.9449025000e+05, /* 0xc88fcb48 */
};

#ifdef __STDC__
static const float qr5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#else
static float qr5[6] = { /* for x in [8,4.5454]=1/[0.125,0.22001] */
#endif
 -2.0897993405e-11, /* 0xadb7d219 */
 -1.0253904760e-01, /* 0xbdd1fffe */
 -8.0564479828e+00, /* 0xc100e736 */
 -1.8366960144e+02, /* 0xc337ab6b */
 -1.3731937256e+03, /* 0xc4aba633 */
 -2.6124443359e+03, /* 0xc523471c */
};
#ifdef __STDC__
static const float qs5[6] = {
#else
static float qs5[6] = {
#endif
  8.1276550293e+01, /* 0x42a28d98 */
  1.9917987061e+03, /* 0x44f8f98f */
  1.7468484375e+04, /* 0x468878f8 */
  4.9851425781e+04, /* 0x4742bb6d */
  2.7948074219e+04, /* 0x46da5826 */
 -4.7191835938e+03, /* 0xc5937978 */
};

#ifdef __STDC__
static const float qr3[6] = {
#else
static float qr3[6] = {/* for x in [4.547,2.8571]=1/[0.2199,0.35001] */
#endif
 -5.0783124372e-09, /* 0xb1ae7d4f */
 -1.0253783315e-01, /* 0xbdd1ff5b */
 -4.6101160049e+00, /* 0xc0938612 */
 -5.7847221375e+01, /* 0xc267638e */
 -2.2824453735e+02, /* 0xc3643e9a */
 -2.1921012878e+02, /* 0xc35b35cb */
};
#ifdef __STDC__
static const float qs3[6] = {
#else
static float qs3[6] = {
#endif
  4.7665153503e+01, /* 0x423ea91e */
  6.7386511230e+02, /* 0x4428775e */
  3.3801528320e+03, /* 0x45534272 */
  5.5477290039e+03, /* 0x45ad5dd5 */
  1.9031191406e+03, /* 0x44ede3d0 */
 -1.3520118713e+02, /* 0xc3073381 */
};

#ifdef __STDC__
static const float qr2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#else
static float qr2[6] = {/* for x in [2.8570,2]=1/[0.3499,0.5] */
#endif
 -1.7838172539e-07, /* 0xb43f8932 */
 -1.0251704603e-01, /* 0xbdd1f475 */
 -2.7522056103e+00, /* 0xc0302423 */
 -1.9663616180e+01, /* 0xc19d4f16 */
 -4.2325313568e+01, /* 0xc2294d1f */
 -2.1371921539e+01, /* 0xc1aaf9b2 */
};
static const float qs2[6] = {
  2.9533363342e+01, /* 0x41ec4454 */
  2.5298155212e+02, /* 0x437cfb47 */
  7.5750280762e+02, /* 0x443d602e */
  7.3939318848e+02, /* 0x4438d92a */
  1.5594900513e+02, /* 0x431bf2f2 */
 -4.9594988823e+00, /* 0xc09eb437 */
};

	static float qonef(float x)
{
	const float *p=NULL,*q=NULL;
	float  s,r,z;
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	if(ix>=0x40200000)     {p = qr8; q= qs8;}
	else if(ix>=0x40f71c58){p = qr5; q= qs5;}
	else if(ix>=0x4036db68){p = qr3; q= qs3;}
	else if(ix>=0x40000000){p = qr2; q= qs2;}
	z = one/(x*x);
	r = p[0]+z*(p[1]+z*(p[2]+z*(p[3]+z*(p[4]+z*p[5]))));
	s = one+z*(q[0]+z*(q[1]+z*(q[2]+z*(q[3]+z*(q[4]+z*q[5])))));
	return ((float).375 + r/s)/x;
}

/* 
 * wrapper of j1f,y1f 
 */
	extern float 
j1f(float x)		/* wrapper j1f */
{
#ifdef _IEEE_LIBM
	return __ieee754_j1f(x);
#else
	float z;
	z = __ieee754_j1f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
	if(fabsf(x)>(float)X_TLOSS) {
		/* j1(|x|>X_TLOSS) */
	        return (float)__kernel_standard((double)x,(double)x,136);
	} else
	    return z;
#endif
}

	extern float 
y1f(float x)		/* wrapper y1f */
{
#ifdef _IEEE_LIBM
	return __ieee754_y1f(x);
#else
	float z;
	z = __ieee754_y1f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
        if(x <= (float)0.0){
                if(x==(float)0.0)
                    /* d= -one/(x-x); */
                    return (float)__kernel_standard((double)x,(double)x,110);
                else
                    /* d = zero/(x-x); */
                    return (float)__kernel_standard((double)x,(double)x,111);
        }
	if(x>(float)X_TLOSS) {
		/* y1(x>X_TLOSS) */
	        return (float)__kernel_standard((double)x,(double)x,137);
	} else
	    return z;
#endif
}

_M_INL	float __ieee754_jnf(int n, float x)
{
	int32_t i,hx,ix, sgn;
	float a, b, temp, di;
	float z, w;

    /* J(-n,x) = (-1)^n * J(n, x), J(n, -x) = (-1)^n * J(n, x)
     * Thus, J(-n,x) = J(n,-x)
     */
	GET_FLOAT_WORD(hx,x);
	ix = 0x7fffffff&hx;
    /* if J(n,NaN) is NaN */
	if(ix>0x7f800000) return x+x;
	if(n<0){		
		n = -n;
		x = -x;
		hx ^= 0x80000000;
	}
	if(n==0) return(__ieee754_j0f(x));
	if(n==1) return(__ieee754_j1f(x));
	sgn = (n&1)&(hx>>31);	/* even n -- 0, odd n -- sign(x) */
	x = fabsf(x);
	if(ix==0||ix>=0x7f800000) 	/* if x is 0 or inf */
	    b = zero;
	else if((float)n<=x) {   
		/* Safe to use J(n+1,x)=2n/x *J(n,x)-J(n-1,x) */
	    a = __ieee754_j0f(x);
	    b = __ieee754_j1f(x);
	    for(i=1;i<n;i++){
		temp = b;
		b = b*((float)(i+i)/x) - a; /* avoid underflow */
		a = temp;
	    }
	} else {
	    if(ix<0x30800000) {	/* x < 2**-29 */
    /* x is tiny, return the first Taylor expansion of J(n,x) 
     * J(n,x) = 1/n!*(x/2)^n  - ...
     */
		if(n>33)	/* underflow */
		    b = zero;
		else {
		    temp = x*(float)0.5; b = temp;
		    for (a=one,i=2;i<=n;i++) {
			a *= (float)i;		/* a = n! */
			b *= temp;		/* b = (x/2)^n */
		    }
		    b = b/a;
		}
	    } else {
		/* use backward recurrence */
		/* 			x      x^2      x^2       
		 *  J(n,x)/J(n-1,x) =  ----   ------   ------   .....
		 *			2n  - 2(n+1) - 2(n+2)
		 *
		 * 			1      1        1       
		 *  (for large x)   =  ----  ------   ------   .....
		 *			2n   2(n+1)   2(n+2)
		 *			-- - ------ - ------ - 
		 *			 x     x         x
		 *
		 * Let w = 2n/x and h=2/x, then the above quotient
		 * is equal to the continued fraction:
		 *		    1
		 *	= -----------------------
		 *		       1
		 *	   w - -----------------
		 *			  1
		 * 	        w+h - ---------
		 *		       w+2h - ...
		 *
		 * To determine how many terms needed, let
		 * Q(0) = w, Q(1) = w(w+h) - 1,
		 * Q(k) = (w+k*h)*Q(k-1) - Q(k-2),
		 * When Q(k) > 1e4	good for single 
		 * When Q(k) > 1e9	good for double 
		 * When Q(k) > 1e17	good for quadruple 
		 */
	    /* determine k */
		float t,v;
		float q0,q1,h,tmp; int32_t k,m;
		w  = (n+n)/(float)x; h = (float)2.0/(float)x;
		q0 = w;  z = w+h; q1 = w*z - (float)1.0; k=1;
		while(q1<(float)1.0e9) {
			k += 1; z += h;
			tmp = z*q1 - q0;
			q0 = q1;
			q1 = tmp;
		}
		m = n+n;
		for(t=zero, i = 2*(n+k); i>=m; i -= 2) t = one/(i/x-t);
		a = t;
		b = one;
		/*  estimate log((2/x)^n*n!) = n*log(2/x)+n*ln(n)
		 *  Hence, if n*(log(2n/x)) > ...
		 *  single 8.8722839355e+01
		 *  double 7.09782712893383973096e+02
		 *  long double 1.1356523406294143949491931077970765006170e+04
		 *  then recurrent value may overflow and the result is 
		 *  likely underflow to zero
		 */
		tmp = n;
		v = two/x;
		tmp = tmp*__ieee754_logf(fabsf(v*tmp));
		if(tmp<(float)8.8721679688e+01) {
	    	    for(i=n-1,di=(float)(i+i);i>0;i--){
		        temp = b;
			b *= di;
			b  = b/x - a;
		        a = temp;
			di -= two;
	     	    }
		} else {
	    	    for(i=n-1,di=(float)(i+i);i>0;i--){
		        temp = b;
			b *= di;
			b  = b/x - a;
		        a = temp;
			di -= two;
		    /* scale b to avoid spurious overflow */
			if(b>(float)1e10) {
			    a /= b;
			    t /= b;
			    b  = one;
			}
	     	    }
		}
	    	b = (t*__ieee754_j0f(x)/b);
	    }
	}
	if(sgn==1) return -b; else return b;
}

_M_INL	float __ieee754_ynf(int n, float x) 
{
	int32_t i,hx,ix,ib;
	int32_t sign;
	float a, b, temp;

	GET_FLOAT_WORD(hx,x);
	ix = 0x7fffffff&hx;
    /* if Y(n,NaN) is NaN */
	if(ix>0x7f800000) return x+x;
	if(ix==0) return -one/zero;
	if(hx<0) return zero/zero;
	sign = 1;
	if(n<0){
		n = -n;
		sign = 1 - ((n&1)<<1);
	}
	if(n==0) return(__ieee754_y0f(x));
	if(n==1) return(sign*__ieee754_y1f(x));
	if(ix==0x7f800000) return zero;

	a = __ieee754_y0f(x);
	b = __ieee754_y1f(x);
	/* quit if b is -inf */
	GET_FLOAT_WORD(ib,b);
	for(i=1;i<n&&ib!=0xff800000;i++){ 
	    temp = b;
	    b = ((float)(i+i)/x)*b - a;
	    GET_FLOAT_WORD(ib,b);
	    a = temp;
	}
	if(sign>0) return b; else return -b;
}

	float jnf(int n, float x)	/* wrapper jnf */
{
#ifdef _IEEE_LIBM
	return __ieee754_jnf(n,x);
#else
	float z;
	z = __ieee754_jnf(n,x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
	if(fabsf(x)>(float)X_TLOSS) {
	    /* jn(|x|>X_TLOSS,n) */
	    return (float)__kernel_standard((double)n,(double)x,138);
	} else
	    return z;
#endif
}

	float ynf(int n, float x)	/* wrapper ynf */
{
#ifdef _IEEE_LIBM
	return __ieee754_ynf(n,x);
#else
	float z;
	z = __ieee754_ynf(n,x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x) ) return z;
        if(x <= (float)0.0){
                if(x==(float)0.0)
                    /* d= -one/(x-x); */
                    return (float)__kernel_standard((double)n,(double)x,112);
                else
                    /* d = zero/(x-x); */
                    return (float)__kernel_standard((double)n,(double)x,113);
        }
	if(x>(float)X_TLOSS) {
	    /* yn(x>X_TLOSS,n) */
	    return (float)__kernel_standard((double)n,(double)x,139);
	} else
	    return z;
#endif
}

_M_INL	float __ieee754_log10f(float x)
{
	float y,z;
	int32_t i,k,hx;

	GET_FLOAT_WORD(hx,x);

        k=0;
        if (hx < 0x00800000) {                  /* x < 2**-126  */
            if ((hx&0x7fffffff)==0)
                return -two25/zero;             /* log(+-0)=-inf */
            if (hx<0) return (x-x)/zero;        /* log(-#) = NaN */
            k -= 25; x *= two25; /* subnormal number, scale up x */
	    GET_FLOAT_WORD(hx,x);
        }
	if (hx >= 0x7f800000) return x+x;
	k += (hx>>23)-127;
	i  = ((u_int32_t)k&0x80000000)>>31;
        hx = (hx&0x007fffff)|((0x7f-i)<<23);
        y  = (float)(k+i);
	SET_FLOAT_WORD(x,hx);
	z  = y*log10_2lo + ivln10*__ieee754_logf(x);
	return  z+y*log10_2hi;
}

/* 
 * wrapper log10f(X)
 */
	extern float 
log10f(float x)		/* wrapper log10f */
{
#ifdef _IEEE_LIBM
	return __ieee754_log10f(x);
#else
	float z;
	z = __ieee754_log10f(x);
	if(_LIB_VERSION == _IEEE_ || isnanf(x)) return z;
	if(x<=(float)0.0) {
	    if(x==(float)0.0)
	        /* log10(0) */
	        return (float)__kernel_standard((double)x,(double)x,118);
	    else 
	        /* log10(x<0) */
	        return (float)__kernel_standard((double)x,(double)x,119);
	} else
	    return z;
#endif
}

static const float 
two23=  8.3886080000e+06, /* 0x4b000000 */
half=  5.0000000000e-01, /* 0x3f000000 */
pi  =  3.1415927410e+00, /* 0x40490fdb */
a0  =  7.7215664089e-02, /* 0x3d9e233f */
a1  =  3.2246702909e-01, /* 0x3ea51a66 */
a2  =  6.7352302372e-02, /* 0x3d89f001 */
a3  =  2.0580807701e-02, /* 0x3ca89915 */
a4  =  7.3855509982e-03, /* 0x3bf2027e */
a5  =  2.8905137442e-03, /* 0x3b3d6ec6 */
a6  =  1.1927076848e-03, /* 0x3a9c54a1 */
a7  =  5.1006977446e-04, /* 0x3a05b634 */
a8  =  2.2086278477e-04, /* 0x39679767 */
a9  =  1.0801156895e-04, /* 0x38e28445 */
a10 =  2.5214456400e-05, /* 0x37d383a2 */
a11 =  4.4864096708e-05, /* 0x383c2c75 */
tc  =  1.4616321325e+00, /* 0x3fbb16c3 */
tf  = -1.2148628384e-01, /* 0xbdf8cdcd */
/* tt = -(tail of tf) */
tt  =  6.6971006518e-09, /* 0x31e61c52 */
t0  =  4.8383611441e-01, /* 0x3ef7b95e */
t1  = -1.4758771658e-01, /* 0xbe17213c */
t2  =  6.4624942839e-02, /* 0x3d845a15 */
t3  = -3.2788541168e-02, /* 0xbd064d47 */
t4  =  1.7970675603e-02, /* 0x3c93373d */
t5  = -1.0314224288e-02, /* 0xbc28fcfe */
t6  =  6.1005386524e-03, /* 0x3bc7e707 */
t7  = -3.6845202558e-03, /* 0xbb7177fe */
t8  =  2.2596477065e-03, /* 0x3b141699 */
t9  = -1.4034647029e-03, /* 0xbab7f476 */
t10 =  8.8108185446e-04, /* 0x3a66f867 */
t11 = -5.3859531181e-04, /* 0xba0d3085 */
t12 =  3.1563205994e-04, /* 0x39a57b6b */
t13 = -3.1275415677e-04, /* 0xb9a3f927 */
t14 =  3.3552918467e-04, /* 0x39afe9f7 */
u0  = -7.7215664089e-02, /* 0xbd9e233f */
u1  =  6.3282704353e-01, /* 0x3f2200f4 */
u2  =  1.4549225569e+00, /* 0x3fba3ae7 */
u3  =  9.7771751881e-01, /* 0x3f7a4bb2 */
u4  =  2.2896373272e-01, /* 0x3e6a7578 */
u5  =  1.3381091878e-02, /* 0x3c5b3c5e */
v1  =  2.4559779167e+00, /* 0x401d2ebe */
v2  =  2.1284897327e+00, /* 0x4008392d */
v3  =  7.6928514242e-01, /* 0x3f44efdf */
v4  =  1.0422264785e-01, /* 0x3dd572af */
v5  =  3.2170924824e-03, /* 0x3b52d5db */
s0  = -7.7215664089e-02, /* 0xbd9e233f */
s1  =  2.1498242021e-01, /* 0x3e5c245a */
s2  =  3.2577878237e-01, /* 0x3ea6cc7a */
s3  =  1.4635047317e-01, /* 0x3e15dce6 */
s4  =  2.6642270386e-02, /* 0x3cda40e4 */
s5  =  1.8402845599e-03, /* 0x3af135b4 */
s6  =  3.1947532989e-05, /* 0x3805ff67 */
r1  =  1.3920053244e+00, /* 0x3fb22d3b */
r2  =  7.2193557024e-01, /* 0x3f38d0c5 */
r3  =  1.7193385959e-01, /* 0x3e300f6e */
r4  =  1.8645919859e-02, /* 0x3c98bf54 */
r5  =  7.7794247773e-04, /* 0x3a4beed6 */
r6  =  7.3266842264e-06, /* 0x36f5d7bd */
w0  =  4.1893854737e-01, /* 0x3ed67f1d */
w1  =  8.3333335817e-02, /* 0x3daaaaab */
w2  = -2.7777778450e-03, /* 0xbb360b61 */
w3  =  7.9365057172e-04, /* 0x3a500cfd */
w4  = -5.9518753551e-04, /* 0xba1c065c */
w5  =  8.3633989561e-04, /* 0x3a5b3dd2 */
w6  = -1.6309292987e-03; /* 0xbad5c4e8 */

	_M_INL float 
sin_pif(float x)
{
	float y,z;
	int n,ix;

	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;

	if(ix<0x3e800000) return __kernel_sinf(pi*x,zero,0);
	y = -x;		/* x is assume negative */

    /*
     * argument reduction, make sure inexact flag not raised if input
     * is an integer
     */
	z = floorf(y);
	if(z!=y) {				/* inexact anyway */
	    y  *= (float)0.5;
	    y   = (float)2.0*(y - floorf(y));	/* y = |x| mod 2.0 */
	    n   = (int) (y*(float)4.0);
	} else {
            if(ix>=0x4b800000) {
                y = zero; n = 0;                 /* y must be even */
            } else {
                if(ix<0x4b000000) z = y+two23;	/* exact */
		GET_FLOAT_WORD(n,z);
		n &= 1;
                y  = n;
                n<<= 2;
            }
        }
	switch (n) {
	    case 0:   y =  __kernel_sinf(pi*y,zero,0); break;
	    case 1:   
	    case 2:   y =  __kernel_cosf(pi*((float)0.5-y),zero); break;
	    case 3:  
	    case 4:   y =  __kernel_sinf(pi*(one-y),zero,0); break;
	    case 5:
	    case 6:   y = -__kernel_cosf(pi*(y-(float)1.5),zero); break;
	    default:  y =  __kernel_sinf(pi*(y-(float)2.0),zero,0); break;
	    }
	return -y;
}


	static	float
__ieee754_lgammaf_r(float x, int *__Wlogf_signgamp)
{
	float t,y,z,nadj=0,p,p1,p2,p3,q,r,w;
	int i,hx,ix;

	GET_FLOAT_WORD(hx,x);

    /* purge off +-inf, NaN, +-0, and negative arguments */
	*__Wlogf_signgamp = 1;
	ix = hx&0x7fffffff;
	if(ix>=0x7f800000) return x*x;
	if(ix==0) return one/zero;
	if(ix<0x1c800000) {	/* |x|<2**-70, return -log(|x|) */
	    if(hx<0) {
	        *__Wlogf_signgamp = -1;
	        return -__ieee754_logf(-x);
	    } else return -__ieee754_logf(x);
	}
	if(hx<0) {
	    if(ix>=0x4b000000) 	/* |x|>=2**23, must be -integer */
		return one/zero;
	    t = sin_pif(x);
	    if(t==zero) return one/zero; /* -integer */
	    nadj = __ieee754_logf(pi/fabsf(t*x));
	    if(t<zero) *__Wlogf_signgamp = -1;
	    x = -x;
	}

    /* purge off 1 and 2 */
	if (ix==0x3f800000||ix==0x40000000) r = 0;
    /* for x < 2.0 */
	else if(ix<0x40000000) {
	    if(ix<=0x3f666666) { 	/* lgamma(x) = lgamma(x+1)-log(x) */
		r = -__ieee754_logf(x);
		if(ix>=0x3f3b4a20) {y = one-x; i= 0;}
		else if(ix>=0x3e6d3308) {y= x-(tc-one); i=1;}
	  	else {y = x; i=2;}
	    } else {
	  	r = zero;
	        if(ix>=0x3fdda618) {y=(float)2.0-x;i=0;} /* [1.7316,2] */
	        else if(ix>=0x3F9da620) {y=x-tc;i=1;} /* [1.23,1.73] */
		else {y=x-one;i=2;}
	    }
	    switch(i) {
	      case 0:
		z = y*y;
		p1 = a0+z*(a2+z*(a4+z*(a6+z*(a8+z*a10))));
		p2 = z*(a1+z*(a3+z*(a5+z*(a7+z*(a9+z*a11)))));
		p  = y*p1+p2;
		r  += (p-(float)0.5*y); break;
	      case 1:
		z = y*y;
		w = z*y;
		p1 = t0+w*(t3+w*(t6+w*(t9 +w*t12)));	/* parallel comp */
		p2 = t1+w*(t4+w*(t7+w*(t10+w*t13)));
		p3 = t2+w*(t5+w*(t8+w*(t11+w*t14)));
		p  = z*p1-(tt-w*(p2+y*p3));
		r += (tf + p); break;
	      case 2:	
		p1 = y*(u0+y*(u1+y*(u2+y*(u3+y*(u4+y*u5)))));
		p2 = one+y*(v1+y*(v2+y*(v3+y*(v4+y*v5))));
		r += (-(float)0.5*y + p1/p2);
	    }
	}
	else if(ix<0x41000000) { 			/* x < 8.0 */
	    i = (int)x;
	    t = zero;
	    y = x-(float)i;
	    p = y*(s0+y*(s1+y*(s2+y*(s3+y*(s4+y*(s5+y*s6))))));
	    q = one+y*(r1+y*(r2+y*(r3+y*(r4+y*(r5+y*r6)))));
	    r = half*y+p/q;
	    z = one;	/* lgamma(1+s) = log(s) + lgamma(s) */
	    switch(i) {
	    case 7: z *= (y+(float)6.0);	/* FALLTHRU */
	    case 6: z *= (y+(float)5.0);	/* FALLTHRU */
	    case 5: z *= (y+(float)4.0);	/* FALLTHRU */
	    case 4: z *= (y+(float)3.0);	/* FALLTHRU */
	    case 3: z *= (y+(float)2.0);	/* FALLTHRU */
		    r += __ieee754_logf(z); break;
	    }
    /* 8.0 <= x < 2**58 */
	} else if (ix < 0x5c800000) {
	    t = __ieee754_logf(x);
	    z = one/x;
	    y = z*z;
	    w = w0+z*(w1+y*(w2+y*(w3+y*(w4+y*(w5+y*w6)))));
	    r = (x-half)*(t-one)+w;
	} else 
    /* 2**58 <= x <= inf */
	    r =  x*(__ieee754_logf(x)-one);
	if(hx<0) r = nadj - r;
	return r;
}

/* 
 * wrapper float lgammaf_r(float x, int *__Wlogf_signgamp)
 */

	float lgammaf_r(float x, int *__Wlogf_signgamp) /* wrapper lgammaf_r */
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,__Wlogf_signgamp);
#else
        float y;
        y = __ieee754_lgammaf_r(x,__Wlogf_signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=(float)0.0)
	        /* lgamma pole */
                return (float)__kernel_standard((double)x,(double)x,115);
            else
	        /* lgamma overflow */
	        return (float)__kernel_standard((double)x,(double)x,114);
        } else
            return y;
#endif
}             

extern int signgam;

	extern float
gammaf(float x)
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,&__Wlogf_signgam);
#else
        float y;
        y = __ieee754_lgammaf_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=(float)0.0)
	        /* gammaf pole */
                return (float)__kernel_standard((double)x,(double)x,141);
            else
	        /* gammaf overflow */
                return (float)__kernel_standard((double)x,(double)x,140);
        } else
            return y;
#endif
}             


	float gammaf_r(float x, int *__Wlogf_signgamp) /* wrapper lgammaf_r */
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,__Wlogf_signgamp);
#else
        float y;
        y = __ieee754_lgammaf_r(x,__Wlogf_signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=(float)0.0)
	        /* gammaf pole */
                return (float)__kernel_standard((double)x,(double)x,141);
            else
	        /* gamma overflow */
                return (float)__kernel_standard((double)x,(double)x,140);
        } else
            return y;
#endif
}             

	extern float
lgammaf(float x)
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,&__Wlogf_signgam);
#else
        float y;
        y = __ieee754_lgammaf_r(x,&signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
            if(floorf(x)==x&&x<=(float)0.0)
	        /* lgamma pole */
                return (float)__kernel_standard((double)x,(double)x,115);
            else
	        /* lgamma overflow */
                return (float)__kernel_standard((double)x,(double)x,114);
        } else
            return y;
#endif
}             

