/* libExt+gcc: $Id: regexp.h,v 1.1 2000/05/17 08:09:18 veit Exp $ */

#ifndef _REGEXP_H
#define _REGEXP_H

#if defined (__cplusplus)
extern "C" {
#endif

/* Definitions etc. for regexp(3) routines.
 *
 * Caveat: this is the obsolete V8 regexp(3), not the System V one
 * [actually, a reimplementation thereof].
 */

#define NSUBEXP  10
typedef struct _regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	size_t regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *__43re_exp_v8_regcomp(__const__ char *);
extern int __43re_exp_v8_regexec(__const__  regexp *, __const__ char *);
extern void __43re_exp_v8_regsub(__const__  regexp *, __const__ char *, char *);
extern void __43re_exp_v8_regerror(__const__ char *);

/*
 * Redefine these to prevent conflicts with the POSIX routines.
 */
#define	regcomp		__43re_exp_v8_regcomp 
#define regexec		__43re_exp_v8_regexec 
#define regsub		__43re_exp_v8_regsub
#define	regerror	__43re_exp_v8_regerror
#define	v8_regcomp 	__43re_exp_v8_regcomp 	
#define v8_regexec 	__43re_exp_v8_regexec 
#define v8_regsub	__43re_exp_v8_regsub
#define	v8_regerror	__43re_exp_v8_regerror

#if defined (__cplusplus)
}
#endif

#endif /* not _REGEXP_H */
