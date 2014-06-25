/*	$Id: error_table.h,v 1.3 2000/08/18 19:17:47 amai Exp $	*/

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

#ifndef _ET_H
#define _ET_H

#include <com_err.h>

struct error_table {
    char const * const * msgs;
    long base;
    int n_msgs;
};

struct et_list {
    struct et_list *next;
    struct error_table const 	*table;
};

#define	ERRCODE_RANGE	8	/* # of bits to shift table number */
#define	BITS_PER_CHAR	6	/* # bits to shift per character in name */

struct et_list *__errorMessage_et_list;

extern char const *__etName_error_table_name(int);

#endif
