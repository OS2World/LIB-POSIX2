/*	libExt+gcc: $Id: com_err.h,v 1.4 2000/08/22 22:20:23 amai Exp $	*/

/*-
 * Header file for common error description library.
 *
 * Copyright 1988, Student Information Processing Board of the
 * Massachusetts Institute of Technology.
 *
 * For copyright and distribution info, see the documentation supplied
 * with this package.
 */

#ifndef __COM_ERR_H
#define __COM_ERR_H

#include <stdarg.h>

#if defined(__cplusplus)
# ifndef __inline__ 
#  define __inline__ inline
#  define __const__ const
# endif
extern "C" {
#endif

typedef int errcode_t;		/* XXX compatibility with newer version */

/* ANSI C -- use prototypes etc */
void com_err(__const__ char *, errcode_t, __const__ char *, ...);
void com_err_va(__const__ char *, errcode_t, __const__ char *, va_list);
char __const__*error_message(long);
/* void (*com_err_hook)(__const__ char *, errcode_t, __const__ char *,
va_list);*/
void (*set_com_err_hook(void (*) (__const__ char *, errcode_t, __const__ char *,
va_list)))
   (__const__ char *, errcode_t, __const__ char *, va_list);
void (*reset_com_err_hook(void))(__const__ char *, errcode_t, __const__ char *,
va_list);
int init_error_table(__const__ char*__const__*,int,int);

#ifdef	__cplusplus
}
#endif

#endif /* ! defined(__COM_ERR_H) */
