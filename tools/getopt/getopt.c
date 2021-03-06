/*	$Id: getopt.c,v 1.4 2000/08/18 18:44:14 amai Exp $	*/

#ifndef lint
static char rcsid[] = "$OpenBSD: getopt.c,v 1.3 1997/01/15 23:42:32 millert Exp $";
#endif /* not lint */

#include <stdio.h>
#include <stdlib.h>

int main(int,char**);
int
main(argc, argv)
int argc;
char *argv[];
{
	extern int optind;
	extern char *optarg;
	int c;
	int status = 0;
	optind = 2;	/* Past the program name and the option letters. */

	__progname = argv[0];
	_wildcard(&argc, &argv);

	while ((c = getopt(argc, argv, argv[1])) != -1)
		switch (c) {
		case '?':
			status = 1;	/* getopt routine gave message */
			break;
		default:
			if (optarg != NULL)
				printf(" -%c %s", c, optarg);
			else
				printf(" -%c", c);
			break;
		}
	printf(" --");
	for (; optind < argc; optind++)
		printf(" %s", argv[optind]);
	printf("\n");
	exit(status);
}
