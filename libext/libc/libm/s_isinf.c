/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: s_isinf.c,v 1.3 1995/05/11 23:20:14 jtc Exp $";
#endif

#include "math.h"
#include "math_private.h"

/*
 * isinf(x) returns 1 is x is inf, else 0;
 * no branching!
 */
#undef isinf(x)
	extern int
isinf(double x)
{
	int32_t hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	hx &= 0x7fffffff;
	hx ^= 0x7ff00000;
	hx |= lx;
	return (hx == 0);
}

/*
 * isnan(x) returns 1 is x is nan, else 0;
 * no branching!
 */
#undef isnan(x)
	extern int
isnan(double x)
{
	int32_t hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	hx &= 0x7fffffff;
	hx |= (u_int32_t)(lx|(-lx))>>31;	
	hx = 0x7ff00000 - hx;
	return (int)((u_int32_t)(hx))>>31;
}
