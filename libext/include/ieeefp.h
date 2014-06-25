/* (libext+gcc): $Id: ieeefp.h,v 1.3 2000/05/02 07:45:56 veit Exp $ */

/* 
 * Written by J.T. Conklin, Apr 6, 1995
 * Public domain.
 */

#ifndef _IEEEFP_H
#define _IEEEFP_H

#if defined (__cplusplus)
extern "C" {
#endif

typedef int fp_except;
#define FP_X_INV	0x01	/* invalid operation exception */
#define FP_X_DNML	0x02	/* denormalization exception */
#define FP_X_DZ		0x04	/* divide-by-zero exception */
#define FP_X_OFL	0x08	/* overflow exception */
#define FP_X_UFL	0x10	/* underflow exception */
#define FP_X_IMP	0x20	/* imprecise (loss of precision) */

typedef enum {
    FP_RN=0,			/* round to nearest representable number */
    FP_RM=1,			/* round toward negative infinity */
    FP_RP=2,			/* round toward positive infinity */
    FP_RZ=3			/* round to zero (truncate) */
} fp_rnd;

extern fp_rnd    fpgetround(void);
extern fp_rnd    fpsetround(fp_rnd);
extern fp_except fpgetmask(void);
extern fp_except fpsetmask(fp_except);
extern fp_except fpgetsticky(void);
extern fp_except fpsetsticky(fp_except);

#if defined (__cplusplus)
}
#endif

#include <ieee.h>

#endif /* _IEEEFP_H_ */
