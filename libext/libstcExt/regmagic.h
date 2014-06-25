/*	$Id: regmagic.h,v 1.1 2000/08/18 19:10:28 amai Exp $ */

/*
 * The first byte of the regexp internal "program" is actually this magic
 * number; the start node begins in the second byte.
 */
#define	MAGIC 0234
