/*	$Id: com_err.c,v 1.3 2000/08/18 19:17:47 amai Exp $	*/

/*-
 * Copyright 1987, 1988 by the Student Information Processing Board
 *	of the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose and without fee is
 * hereby granted, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation,
 * and that the names of M.I.T. and the M.I.T. S.I.P.B. not be
 * used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * M.I.T. and the M.I.T. S.I.P.B. make no representations about
 * the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 */

#include <stdio.h>
#undef VARARGS
#include <stdarg.h>

#include "com_err.h"
#include "error_table.h"

#if 0
static const char rcsid[] =
    "$Id: com_err.c,v 1.3 2000/08/18 19:17:47 amai Exp $";
#endif	/* ! lint */

static void
    default_com_err_proc (const char *whoami, errcode_t code, const char *fmt, va_list args)
{
    if (whoami) {
	fputs(whoami, stderr);
	fputs(": ", stderr);
    }
#ifdef SOLARIS
    if (code) {
	fputs(error_message(code), stderr);
	fputs(" ", stderr);
    } else {
        vfprintf (stderr, fmt, args);
    }
#else
    if (code) {
	fputs(error_message(code), stderr);
	fputs(" ", stderr);
    }
    if (fmt) {
        vfprintf (stderr, fmt, args);
    }
#endif
    putc('\n', stderr);
    /* should do this only on a tty in raw mode */
    putc('\r', stderr);
    fflush(stderr);
}

typedef void (*errf)(const char *, errcode_t, const char *, va_list);

static errf com_err_hook = default_com_err_proc;

	extern void 
com_err_va (whoami, code, fmt, args)
    const char *whoami;
    errcode_t code;
    const char *fmt;
    va_list args;
{
  if (! com_err_hook)
        com_err_hook = default_com_err_proc;
    (*com_err_hook) (whoami, code, fmt, args);
}

	extern void
com_err(const char *whoami, errcode_t code, const char *fmt, ...)
{
    va_list pvar;

    if (!com_err_hook)
	com_err_hook = default_com_err_proc;

    va_start(pvar, fmt);
    com_err_va (whoami, code, fmt, pvar);
    va_end(pvar);
}

	extern errf 
set_com_err_hook(new_proc)
    errf new_proc;
{
    errf x = com_err_hook;

    if (new_proc)
	com_err_hook = new_proc;
    else
	com_err_hook = default_com_err_proc;

    return x;
}

	extern errf 
reset_com_err_hook()
{
    errf x = com_err_hook;
    com_err_hook = default_com_err_proc;
    return x;
}

