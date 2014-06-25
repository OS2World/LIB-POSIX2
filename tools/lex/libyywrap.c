/*	$Id: libyywrap.c,v 1.2 2000/08/14 16:26:09 amai Exp $	*/

/* libyywrap - flex run-time support library "yywrap" function */

/* $Header: /cvsroot/posix2/posix2/tools/lex/libyywrap.c,v 1.2 2000/08/14 16:26:09 amai Exp $ */

int yywrap(void);

int
yywrap(void)
	{
	return 1;
	}
