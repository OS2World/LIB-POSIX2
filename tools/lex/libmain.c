/*	$Id: libmain.c,v 1.2 2000/08/14 16:26:09 amai Exp $	*/

/* libmain - flex run-time support library "main" function */

/* $Header: /cvsroot/posix2/posix2/tools/lex/libmain.c,v 1.2 2000/08/14 16:26:09 amai Exp $ */

int yylex(void);
int main(int, char **, char **);

/* ARGSUSED */
int
main( argc, argv, envp )
int argc;
char *argv[];
char *envp[];
	{
	while ( yylex() != 0 )
		;

	return 0;
	}
