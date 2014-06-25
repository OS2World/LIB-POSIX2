/* libExt+gcc: $Id: compat.h,v 1.3 2000/05/31 09:12:05 veit Exp $ */
/* Fake missing symbols for BSD compatibility: This is only the 
 * compatibility kitchen sink for bad hacks, you cannot live without.
 */

#include <machine/endian.h>
/* BSD compatibility: adapted to emx/gcc
 * Types which are fundamental to the implementation and may appear in
 * more than one standard header are defined here.  Standard headers
 * then use:
 *	#ifdef	_BSD_SIZE_T_
 *	typedef	_BSD_SIZE_T_ size_t;
 *	#undef	_BSD_SIZE_T_
 *	#endif
 */
#define	_BSD_CLOCK_T_	long			/* clock() */
#define	_BSD_PTRDIFF_T_	long			/* ptr1 - ptr2 */
#define	_BSD_SIZE_T_	unsigned long 		/* sizeof() */
#define	_BSD_SSIZE_T_	int			/* byte count or error */
#define	_BSD_TIME_T_	unsigned long 		/* time() */
#define	_BSD_VA_LIST_	char *			/* va_list */
#define _BSD_CLOCKID_T_	int
#define _BSD_TIMER_T_	int

/* The following is adapted for emx compatibility und no longer true!:
 * Runes (wchar_t) is declared to be an ``int'' instead of the more natural
 * ``unsigned long'' or ``long''.  Two things are happening here.  It is not
 * unsigned so that EOF (-1) can be naturally assigned to it and used.  Also,
 * it looks like 10646 will be a 31 bit standard.  This means that if your
 * ints cannot hold 32 bits, you will be in trouble.  The reason an int was
 * chosen over a long is that the is*() and to*() routines take ints (says
 * ANSI C), but they use _RUNE_T_ instead of int.  By changing it here, you
 * lose a bit of ANSI conformance, but your programs will still work.
 *    
 * Note that _WCHAR_T_ and _RUNE_T_ must be of the same type.  When wchar_t
 * and rune_t are typedef'd, _WCHAR_T_ will be undef'd, but _RUNE_T remains
 * defined for ctype.h.
 */
#define	_BSD_WCHAR_T_	unsigned short		/* wchar_t */
#define _BSD_WINT_T_	int			/* wint_t */
#define	_BSD_RUNE_T_	unsigned short		/* rune_t */

#define	O_EXLOCK	0
#define	O_SHLOCK	0
#define	WCOREDUMP(a)	0
#define	TCSASOFT	0

#ifndef	_POSIX2_RE_DUP_MAX
#define	_POSIX2_RE_DUP_MAX	255
#endif

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

typedef char *_BSD_VA_LIST_;
