/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#include "math.h"
#include "math_private.h"

/*
 * isinff(x) returns 1 is x is inf, else 0;
 * no branching!
 */
	int isinff(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	ix ^= 0x7f800000;
	return (ix == 0);
}

/*
 * isnanf(x) returns 1 is x is nan, else 0;
 * no branching!
 */
	int isnanf(float x)
{
	int32_t ix;
	GET_FLOAT_WORD(ix,x);
	ix &= 0x7fffffff;
	ix = 0x7f800000 - ix;
	return (int)(((u_int32_t)(ix))>>31);
}
