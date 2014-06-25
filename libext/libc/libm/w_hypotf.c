/* w_hypotf.c -- float version of w_hypot.c.
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

_M_INL	float __ieee754_hypotf(float x, float y)
{
	float a=x,b=y,t1,t2,float_y1,float_y2,w;
	int32_t j,k,ha,hb;

	GET_FLOAT_WORD(ha,x);
	ha &= 0x7fffffff;
	GET_FLOAT_WORD(hb,y);
	hb &= 0x7fffffff;
	if(hb > ha) {a=y;b=x;j=ha; ha=hb;hb=j;} else {a=x;b=y;}
	SET_FLOAT_WORD(a,ha);	/* a <- |a| */
	SET_FLOAT_WORD(b,hb);	/* b <- |b| */
	if((ha-hb)>0xf000000) {return a+b;} /* x/y > 2**30 */
	k=0;
	if(ha > 0x58800000) {	/* a>2**50 */
	   if(ha >= 0x7f800000) {	/* Inf or NaN */
	       w = a+b;			/* for sNaN */
	       if(ha == 0x7f800000) w = a;
	       if(hb == 0x7f800000) w = b;
	       return w;
	   }
	   /* scale a and b by 2**-60 */
	   ha -= 0x5d800000; hb -= 0x5d800000;	k += 60;
	   SET_FLOAT_WORD(a,ha);
	   SET_FLOAT_WORD(b,hb);
	}
	if(hb < 0x26800000) {	/* b < 2**-50 */
	    if(hb <= 0x007fffff) {	/* subnormal b or 0 */	
	        if(hb==0) return a;
		SET_FLOAT_WORD(t1,0x3f000000);	/* t1=2^126 */
		b *= t1;
		a *= t1;
		k -= 126;
	    } else {		/* scale a and b by 2^60 */
	        ha += 0x5d800000; 	/* a *= 2^60 */
		hb += 0x5d800000;	/* b *= 2^60 */
		k -= 60;
		SET_FLOAT_WORD(a,ha);
		SET_FLOAT_WORD(b,hb);
	    }
	}
    /* medium size a and b */
	w = a-b;
	if (w>b) {
	    SET_FLOAT_WORD(t1,ha&0xfffff000);
	    t2 = a-t1;
	    w  = __ieee754_sqrtf(t1*t1-(b*(-b)-t2*(a+t1)));
	} else {
	    a  = a+a;
	    SET_FLOAT_WORD(float_y1,hb&0xfffff000);
	    float_y2 = b - float_y1;
	    SET_FLOAT_WORD(t1,ha+0x00800000);
	    t2 = a - t1;
	    w  = __ieee754_sqrtf(t1*float_y1-(w*(-w)-(t1*float_y2+t2*b)));
	}
	if(k!=0) {
	    SET_FLOAT_WORD(t1,0x3f800000+(k<<23));
	    return t1*w;
	} else return w;
}

/*
 * wrapper hypotf(x,y)
 */
	float
hypotf(float x, float y)	/* wrapper hypotf */
{
#ifdef _IEEE_LIBM
	return __ieee754_hypotf(x,y);
#else
	float z;
	z = __ieee754_hypotf(x,y);
	if(_LIB_VERSION == _IEEE_) return z;
	if((!finitef(z))&&finitef(x)&&finitef(y))
	    /* hypot overflow */
	    return (float)__kernel_standard((double)x,(double)y,104);
	else
	    return z;
#endif
}

	float
cabsf(_complexf_t z)
{
	return hypotf(z.x, z.y);
}
