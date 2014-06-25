/* i386/asm.h (libext+gcc) - $Id: asm.h,v 1.5 2000/04/18 12:01:52 veit Exp $ */
#ifndef _I386_ASM_H
#define _I386_ASM_H

#include <sys/errno.h>

/* enable override of entrypoint alignment */
#ifndef _ALIGN_TEXT
# define _ALIGN_TEXT .align 2, 0x90
#endif

#ifdef __GPROF__
# ifdef PIC
#  define _PROF_PROLOGUE  pushl %ebp; movl %esp, %ebp; call __mcount; popl %ebp
# else
#  define _PROF_PROLOGUE  call __mcount
# endif
#else
#define _PROF_PROLOGUE
#endif

#define __ENTR(x) \
	.text; _ALIGN_TEXT; .globl x; .type x,@function; x:

#define	_ENTRY(y)	__ENTR( _ ## y ); _PROF_PROLOGUE


#if defined (__MT__)
# define SET_ERRNO_CONST(x) \
        call    __errno ;\
        movl    x, (%eax)
#else
# define SET_ERRNO_CONST(x) \
        movl    x, _errno
#endif

#define	_ASM_LABEL(x)	x

#endif /* _I386_ASM_H */