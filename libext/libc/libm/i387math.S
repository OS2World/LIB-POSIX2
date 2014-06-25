/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

/* Updated to include the fixes for acos, asin, exp */
/*	 from FreeBSD's src/lib/msun/i387 directory */
/* Note: The original code contained an additional  */ 
/*	 parameter for the fmulp statements in acos */
/*	 and asin, but this leads to wrong results  */
/*	 - at least for me.			    */

/*
 * Copyright (c) 1993,94 Winning Strategies, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Winning Strategies, Inc.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <i386/asm.h>


_ENTRY(abs)
	movl	4(%esp),%eax
	testl	%eax,%eax
	jns	1f
	negl	%eax
1:	ret

/*
 * acos(x) = atan2(sqrt(1  - x^2, x).
 * Actually evaluate (1 - x^2) as (1 - x) * (1 + x) to avoid loss of
 * precision when |x| is nearly 1.
 */
_ENTRY(__ieee754_acos)
	fldl	4(%esp)			/* x */
	fld1
	fld	%st(0)
	fsub	%st(2)			/* 1 - x */
	fxch	%st(1)
	fadd	%st(2)			/* 1 + x */
	fmulp				/* (1 - x) * (1 + x) */
	fsqrt
	fxch	%st(1)
	fpatan
	ret

/*
 * asin(x) = atan2(x, sqrt(1  - x^2).
 * Actually evaluate (1 - x^2) as (1 - x) * (1 + x) to avoid loss of
 * precision when |x| is nearly 1.
 */
_ENTRY(__ieee754_asin)
	fldl	4(%esp)			/* x */
	fld1
	fld	%st(0)
	fsub	%st(2)			/* 1 - x */
	fxch	%st(1)
	fadd	%st(2)			/* 1 + x */
	fmulp				/* (1 - x) * (1 + x) */
	fsqrt
	fpatan
	ret

_ENTRY(atan)
	fldl	4(%esp)
	fld1
	fpatan
	ret

_ENTRY(atanf)
	flds	4(%esp)
	fld1
	fpatan
	ret

_ENTRY(__ieee754_atan2)
	fldl	 4(%esp)
	fldl	12(%esp)
	fpatan
	ret


_ENTRY(__ieee754_atan2f)
	flds	4(%esp)
	flds	8(%esp)
	fpatan
	ret

_ENTRY(ceil)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$8,%esp

	fstcw	-12(%ebp)		/* store fpu control word */
	movw	-12(%ebp),%dx
	orw	$0x0800,%dx		/* round towards +oo */
	andw	$0xfbff,%dx
	movw	%dx,-16(%ebp)
	fldcw	-16(%ebp)		/* load modfied control word */

	fldl	8(%ebp);		/* round */
	frndint
	fldcw	-12(%ebp)		/* restore original control word */
	leave
	ret

_ENTRY(ceilf)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$8,%esp

	fstcw	-12(%ebp)		/* store fpu control word */
	movw	-12(%ebp),%dx
	orw	$0x0800,%dx		/* round towards +oo */
	andw	$0xfbff,%dx
	movw	%dx,-16(%ebp)
	fldcw	-16(%ebp)		/* load modfied control word */

	flds	8(%ebp);		/* round */
	frndint

	fldcw	-12(%ebp)		/* restore original control word */

	leave
	ret

_ENTRY(ceill)
	pushl %ebp
	movl %esp,%ebp
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call _floorl
	fldt 8(%ebp)
	fcomp %st(1)
	fnstsw %ax
	andb $69,%ah
	jne L6
	fld1
	faddp %st,%st(1)
L6:
	leave
	ret


_ENTRY(copysign)
	movl	16(%esp),%edx
	andl	$0x80000000,%edx
	movl	8(%esp),%eax
	andl	$0x7fffffff,%eax
	orl	%edx,%eax
	movl	%eax,8(%esp)
	fldl	4(%esp)
	ret

_ENTRY(copysignf)
	movl	8(%esp),%edx
	andl	$0x80000000,%edx
	movl	4(%esp),%eax
	andl	$0x7fffffff,%eax
	orl	%edx,%eax
	movl	%eax,4(%esp)
	flds	4(%esp)
	ret


_ENTRY(cos)
	fldl	4(%esp)
	fcos
	fnstsw	%ax
	andw	$0x400,%ax
	jnz	1f
	ret	
1:	fldpi
	fadd	%st(0)
	fxch	%st(1)
2:	fprem1
	fnstsw	%ax
	andw	$0x400,%ax
	jnz	2b
	fstp	%st(1)
	fcos
	ret

/* A float's domain isn't large enough to require argument reduction. */
_ENTRY(cosf)
	flds	4(%esp)
	fcos
	ret	


_ENTRY(div)
	movl	4(%esp),%eax
	movl	8(%esp),%ecx
	cdq
	idiv	%ecx
	movl	%eax,4(%esp)
	movl	%edx,8(%esp)
	ret


_ENTRY(__divsi3)
	movl 4(%esp),%eax
	cltd
	idivl 8(%esp)
	ret

/* e^x = 2^(x * log2(e)) */
_ENTRY(__ieee754_exp)
	/*
	 * If x is +-Inf, then the subtraction would give Inf-Inf = NaN.
	 * Avoid this.	Also avoid it if x is NaN for convenience.
	 */
	movl	8(%esp),%eax
	andl	$0x7fffffff,%eax
	cmpl	$0x7ff00000,%eax
	jae	x_Inf_or_NaN

	fldl	4(%esp)

	/*
	 * Ensure that the rounding mode is to nearest (to give the smallest
	 * possible fraction) and that the precision is as high as possible.
	 * We may as well mask interrupts if we switch the mode.
	 */
	fstcw	4(%esp)			/* store fpu control word */
	movl	4(%esp),%eax
	andl	$0x0300,%eax
	cmpl	$0x0300,%eax		/* RC == 0 && PC == 3? */
	je	1f			/* jump if mode is good */
	movl	$0x137f,8(%esp)
	fldcw	8(%esp)
1:
	fldl2e
	fmulp				/* x * log2(e) */
	fstl	%st(1)
	frndint				/* int(x * log2(e)) */
	fstl	%st(2)
	fsubrp				/* fract(x * log2(e)) */
	f2xm1				/* 2^(fract(x * log2(e))) - 1 */ 
	fld1
	faddp				/* 2^(fract(x * log2(e))) */
	fscale				/* e^x */
	fstpl	%st(1)
	je	1f
	fldcw	4(%esp)			/* restore original control word */
1:	
	ret

x_Inf_or_NaN:
	/*
	 * Return 0 if x is -Inf.  Otherwise just return x, although the
	 * C version would return (x + x) (Real Indefinite) if x is a NaN.
	 */
	cmpl	$0xfff00000,8(%esp)
	jne	x_not_minus_Inf
	cmpl	$0,4(%esp)
	jne	x_not_minus_Inf
	fldz
	ret

x_not_minus_Inf:
	fldl	4(%esp)
	ret

_ENTRY(fabs)
	fldl	4(%esp)
	fabs
	ret

_ENTRY(fabsl)
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	fldt 8(%ebp)
	fabs
	leave
	ret

_ENTRY(finite)
	movl	8(%esp),%eax
	andl	$0x7ff00000, %eax
	cmpl	$0x7ff00000, %eax
	setne	%al
	andl	$0x000000ff, %eax
	ret

_ENTRY(finitef)
	movl	4(%esp),%eax
	andl	$0x7f800000, %eax
	cmpl	$0x7f800000, %eax
	setne	%al
	andl	$0x000000ff, %eax
	ret

_ENTRY(__fixdfsi)
	fldl	4(%esp)
	fistpl	4(%esp)
	movl	4(%esp),%eax
	ret

_ENTRY(__fixunsdfsi)
	fldl	4(%esp)		/* argument double to accum stack */
	frndint			/* create integer */
#ifdef PIC
	PIC_PROLOGUE
	leal	PIC_GOTOFF(fbiggestsigned),%eax
	PIC_EPILOGUE
	fcoml	(%eax)
#else
	fcoml	fbiggestsigned	/* bigger than biggest signed? */
#endif
	fstsw	%ax
	sahf
	jnb	1f

	fistpl	4(%esp)
	movl	4(%esp),%eax
	ret

1:
#ifdef PIC
	PIC_PROLOGUE
	leal	PIC_GOTOFF(fbiggestsigned),%eax
	PIC_EPILOGUE
	fsubl	(%eax)
#else
	fsubl	fbiggestsigned	/* reduce for proper conversion */
#endif
	fistpl	4(%esp)		/* convert */
	movl	4(%esp),%eax
	orl	$0x80000000,%eax	/* restore bias */
	ret

fbiggestsigned:	.double	0r2147483648.0


_ENTRY(floor)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$8,%esp

	fstcw	-12(%ebp)		/* store fpu control word */
	movw	-12(%ebp),%dx
	orw	$0x0400,%dx		/* round towards -oo */
	andw	$0xf7ff,%dx
	movw	%dx,-16(%ebp)
	fldcw	-16(%ebp)		/* load modfied control word */

	fldl	8(%ebp);		/* round */
	frndint

	fldcw	-12(%ebp)		/* restore original control word */
	leave
	ret

_ENTRY(floorf)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$8,%esp

	fstcw	-12(%ebp)		/* store fpu control word */
	movw	-12(%ebp),%dx
	orw	$0x0400,%dx		/* round towards -oo */
	andw	$0xf7ff,%dx
	movw	%dx,-16(%ebp)
	fldcw	-16(%ebp)		/* load modfied control word */

	flds	8(%ebp);		/* round */
	frndint

	fldcw	-12(%ebp)		/* restore original control word */
	leave
	ret

_ENTRY(floorl)
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	fstcw -4(%ebp)
	fwait
	movw -4(%ebp),%ax
	andw $0xf3ff,%ax
	orw $0x400,%ax
	movw %ax,-2(%ebp)
	fldcw -2(%ebp)
	fldt 8(%ebp)
	frndint
	fldcw -4(%ebp)
	leave
	ret



_map:
	.byte 1		/* round to nearest */
	.byte 3		/* round to negative infinity */
	.byte 2		/* round to positive infinity */
	.byte 0		/* round to zero */

_ENTRY(__fltRounds)
	subl $4,%esp
	fnstcw (%esp)
	movl (%esp),%eax
	shrl $10,%eax
	andl $3,%eax
#ifdef PIC
	PIC_PROLOGUE
	leal PIC_GOTOFF(_map),%ecx
	PIC_EPILOGUE
	movb (%ecx,%eax,1),%al
#else
	movb _map(,%eax,1),%al
#endif
	addl $4,%esp
	ret

_ENTRY(__ieee754_fmod)
	fldl	12(%esp)
	fldl	4(%esp)
1:	fprem
	fstsw	%ax
	sahf
	jp	1b
	fstpl	%st(1)
	ret

_ENTRY(fpgetmask)
	subl $4,%esp
	fnstcw (%esp)
	movl (%esp),%eax
	notl %eax
	andl $63,%eax
	addl $4,%esp
	ret

_ENTRY(fpgetround)
	subl $4,%esp
	fnstcw (%esp)
	movl (%esp),%eax
	rorl $10,%eax
	andl $3,%eax
	addl $4,%esp
	ret

_ENTRY(fpgetsticky)
	subl $4,%esp
	fnstsw (%esp)
	movl (%esp),%eax
	andl $63,%eax
	addl $4,%esp
	ret

  /* Written by Charles M. Hannum, Apr 9, 1995 * Public domain. */
_ENTRY(fpsetmask) 
	subl $4,%esp

	fnstcw (%esp)
	movl (%esp),%eax
	movl %eax,%edx

	notl %eax
	andl $63,%eax

	addl %eax,%edx
	movl 8(%esp),%ecx
	andl $63,%ecx
	subl %ecx,%edx
	movl %edx,(%esp)
	fldcw (%esp)

	addl $4,%esp
	ret

_ENTRY(fpsetround)
	subl $4,%esp

	fnstcw (%esp)
	movl (%esp),%eax

	rorl $10,%eax
	movl %eax,%edx
	andl $3,%eax

	subl %eax,%edx
	movl 8(%esp),%ecx
	andl $3,%ecx
	orl %ecx,%edx
	roll $10,%edx
	movl %edx,(%esp)
	fldcw (%esp)

	addl $4,%esp
	ret

_ENTRY(fpsetsticky)
	subl $28,%esp

	fnstenv (%esp)
	movl 4(%esp),%eax
	movl %eax,%edx

	andl $63,%eax

	subl %eax,%edx
	movl 32(%esp),%ecx
	andl $63,%ecx
	addl %ecx,%edx
	movl %edx,4(%esp)
	fldenv (%esp)

	addl $28,%esp
	ret

_ENTRY(frexpl)
	pushl %ebp
	movl %esp,%ebp
	subl $24,%esp
	pushl %esi
	pushl %ebx
	fldt 8(%ebp)
	movl 20(%ebp),%ebx
	fld %st(0)
	fstpt -12(%ebp)
	leal -4(%ebp),%ecx
	movw -4(%ebp),%dx
	andl $32767,%edx
	jne L25
	fldz
	fucompp
	fnstsw %ax
	andb $68,%ah
	xorb $64,%ah
	jne L21
	movl $0,(%ebx)
	fldz
	jmp L24
	.align 2,0x90
	.align 2,0x90
L21:
	fldt -12(%ebp)
	fadd %st(0),%st
	fstpt -12(%ebp)
	decl %edx
	movw (%ecx),%si
	andl $32767,%esi
	jne L22
	cmpl $-66,%edx
	jg L21
L22:
	addl %esi,%edx
	jmp L19
	.align 2,0x90
L25:
	fstp %st(0)
L19:
	addl $-16382,%edx
	movl %edx,(%ebx)
	movw (%ecx),%ax
	andl $-32768,%eax
	orl $16382,%eax
	movw %ax,(%ecx)
	fldt -12(%ebp)
L24:
	leal -32(%ebp),%esp
	popl %ebx
	popl %esi
	leave
	ret

_ENTRY(ilogb)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$4,%esp

	fldl	8(%ebp)
	fxtract
	fstpl	%st

	fistpl	-4(%ebp)
	movl	-4(%ebp),%eax
	leave
	ret

_ENTRY(ilogbf)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$4,%esp

	flds	8(%ebp)
	fxtract
	fstpl	%st

	fistpl	-4(%ebp)
	movl	-4(%ebp),%eax
	leave
	ret


_ENTRY(__ieee754_log)
	fldln2
	fldl	4(%esp)
	fyl2x
	ret

_ENTRY(__ieee754_log10)
	fldlg2
	fldl	4(%esp)
	fyl2x
	ret


_ENTRY(labs)
	movl	4(%esp),%eax
	testl	%eax,%eax
	jns	1f
	negl	%eax
1:	ret


_ENTRY(ldiv)
	movl	4(%esp),%eax
	movl	8(%esp),%ecx
	cdq
	idiv	%ecx
	movl	%eax,4(%esp)
	movl	%edx,8(%esp)
	ret

#if 0 /* see inline asm version in mathx.c */
/*
 * Since the fyl2xp1 instruction has such a limited range:
 *	-(1 - (sqrt(2) / 2)) <= x <= sqrt(2) - 1
 * it's not worth trying to use it.  
 */
_ENTRY(log1p)
	fldln2
	fldl 4(%esp)
	fld1
	faddp
	fyl2x
	ret

/*
 * Since the fyl2xp1 instruction has such a limited range:
 *	-(1 - (sqrt(2) / 2)) <= x <= sqrt(2) - 1
 * it's not worth trying to use it.  
 */
_ENTRY(log1pf)
	fldln2
	flds 4(%esp)
	fld1
	faddp
	fyl2x
	ret
#endif

_ENTRY(logb)
	fldl	4(%esp)
	fxtract
	fstpl	%st
	ret

_ENTRY(logbf)
	flds	4(%esp)
	fxtract
	fstpl	%st
	ret

/*
 * modf(value, iptr): return fractional part of value, and stores the
 * integral part into iptr (a pointer to double).
 *
 * Written by Sean Eric Fagan (sef@kithrup.COM)
 * Sun Mar 11 20:27:30 PST 1990
 */

/* With CHOP mode on, frndint behaves as TRUNC does.  Useful. */
_ENTRY(modf)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$16,%esp
	fnstcw	-12(%ebp)
	movw	-12(%ebp),%dx
	orw	$3072,%dx
	movw	%dx,-16(%ebp)
	fldcw	-16(%ebp)
	fldl	8(%ebp)
	frndint
	fstpl	-8(%ebp)
	fldcw	-12(%ebp)
	movl	16(%ebp),%eax
	movl	-8(%ebp),%edx
	movl	-4(%ebp),%ecx
	movl	%edx,(%eax)
	movl	%ecx,4(%eax)
	fldl	8(%ebp)
	fsubl	-8(%ebp)
	jmp	L1
L1:
	leave
	ret

_ENTRY(__ieee754_remainder)
	fldl	12(%esp)
	fldl	4(%esp)
1:	fprem1
	fstsw	%ax
	sahf
	jp	1b
	fstpl	%st(1)
	ret

_ENTRY(__ieee754_remainderf)
	flds	8(%esp)
	flds	4(%esp)
1:	fprem1
	fstsw	%ax
	sahf
	jp	1b
	fstpl	%st(1)
	ret

_ENTRY(rint)
	fldl	4(%esp)
	frndint
	ret

_ENTRY(rintf)
	flds	4(%esp)
	frndint
	ret

_ENTRY(scalbn)
	fildl	12(%esp)
	fldl	4(%esp)
	fscale
	fstp	%st(1)		/* bug fix for fp stack overflow */
	ret

_ENTRY(scalbnf)
	fildl	8(%esp)
	flds	4(%esp)
	fscale
	fstp	%st(1)		/* bug fix for fp stack overflow */
	ret

_ENTRY(__ieee754_scalb)
	fldl	12(%esp)
	fldl	4(%esp)
	fscale
	fstp	%st(1)			/* bug fix for fp stack overflow */
	ret

_ENTRY(significand)
	fldl	4(%esp)
	fxtract
	fstpl	%st(1)
	ret

_ENTRY(significandf)
	flds	4(%esp)
	fxtract
	fstpl	%st(1)
	ret

_ENTRY(sin)
	fldl	4(%esp)
	fsin
	fnstsw	%ax
	andw	$0x400,%ax
	jnz	1f
	ret
1:	fldpi
	fadd	%st(0)
	fxch	%st(1)
2:	fprem1
	fnstsw	%ax
	andw	$0x400,%ax
	jnz	2b
	fstp	%st(1)
	fsin
	ret

/* A float's domain isn't large enough to require argument reduction. */
_ENTRY(sinf)
	flds	4(%esp)
	fsin
	ret

_ENTRY(__ieee754_sqrt)
	fldl	4(%esp)
	fsqrt
	ret

_ENTRY(__ieee754_sqrtf)
	flds	4(%esp)
	fsqrt
	ret

_ENTRY(sqrtl)
	pushl %ebp
	movl %esp,%ebp
	fldt 8(%ebp)
	fsqrt
	leave
	ret

_ENTRY(tan)
	fldl	4(%esp)
	fptan
	fnstsw	%ax
	andw	$0x400,%ax
	jnz	1f
	fstp	%st(0)
	ret
1:	fldpi
	fadd	%st(0)
	fxch	%st(1)
2:	fprem1
	fstsw	%ax
	andw	$0x400,%ax
	jnz	2b
	fstp	%st(1)
	fptan
	fstp	%st(0)
	ret

/* A float's domain isn't large enough to require argument reduction. */
_ENTRY(tanf)
	flds	4(%esp)
	fptan
	fstp	%st(0)
	ret


_ENTRY(__udivsi3)
	movl 4(%esp),%eax
	xorl %edx,%edx
	divl 8(%esp)
	ret


/* Set 80387 floating point hardware rounding precision */
_ENTRY(_sprec)
	pushl %ebp
	movl %esp,%ebp
	pushl %eax
	subl $4,%esp
	fstcw (%esp)
	fwait
	movl (%esp),%eax
	andl $0xfcff,%eax
	movl %eax,(%esp)
	fldcw (%esp)
	popl %eax
	popl %eax
	leave
	ret

_ENTRY(_dprec)
	pushl %ebp
	movl %esp,%ebp
	pushl %eax
	subl $4,%esp
	fstcw (%esp)
	fwait
	movl (%esp),%eax
	andl $0xfcff,%eax
	orl $0x200,%eax
	movl %eax,(%esp)
	fldcw (%esp)
	popl %eax
	popl %eax
	leave
	ret

_ENTRY(_ldprec)
	pushl %ebp
	movl %esp,%ebp
	pushl %eax
	subl $4,%esp
	fstcw (%esp)
	fwait
	movl (%esp),%eax
	orl $0x300,%eax
	movl %eax,(%esp)
	fldcw (%esp)
	popl %eax
	popl %eax
	leave
	ret

_ENTRY(_noexcept)
	pushl %ebp
	movl %esp,%ebp
	pushl %eax
	subl $4,%esp
	fnclex
	fwait
	fstcw (%esp)
	fwait
	movl (%esp),%eax
	andl $0xffc0,%eax
	orl $0x3f,%eax
	movl %eax,(%esp)
	fldcw (%esp)
	popl %eax
	popl %eax
	leave
	ret

_ENTRY(_fpclear)
	pushl %ebp
	movl %esp,%ebp
	fnclex
	fwait
	leave
	ret

_ENTRY(_getprec)
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	fstcw (%esp)
	fwait
	movl (%esp),%eax
	leave
	ret

_ENTRY(_setfpu)
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%eax
	pushl %eax
	fldcw (%esp)
	fwait
	movl %ebp,%esp
	popl %ebp
	ret
